#include "stdafx.h"
#include "communicator.h"
#include "VCameraEnum.h"
#include<io.h>
#include <VAsyncIO.h>

#include <thread>
#include <sstream>
#include <iostream>
#include <opencv2/opencv.hpp>
#include "FiberDetectControl.h"
#include "VCameraSession.h"
using namespace Vsee;

bool aq::Communicator::batch_image_load = true;
bool aq::Communicator::single_image_load = false;
bool aq::Communicator::m_bServerSendMode=true;
bool aq::Communicator::m_bClientSendMode=false;
bool	aq::Communicator::m_bServerRun=true;
bool	aq::Communicator::m_bClientRun = true;
Vsee::VCameraMessageTrans  aq::Communicator::box_ctrl_msg;       //use to transmit control parameters from server to box;
extern HWND m_hMainWnd;
CLog	aq::Communicator::m_Log;
std::mutex    aq::Communicator::mutex_magnetic_valve;
Vsee::VCameraMessageTrans*  aq::Communicator::valve_ctrl_msg;    //use to transmit magenetic valve control parameters from server to box;
aq::_io_valve_friver_tag_  aq::Communicator::valve_driver[MAGNETIC_VALVE_SUM];
bool aq::Communicator::m_bMagneticValveShow=false;
bool aq::Communicator::m_bWriteLog = false;

struct information
{
	std::uint32_t height;
	std::uint32_t width;
	std::uint32_t channels;
	std::uint32_t data_legnth;
	std::uint32_t camera_serial;
	std::uint32_t current_time;
	char* m_picture_data;
}Info1,Info2;

std::vector<std::string> aq::Communicator::split(std::string str, char seg)
{
    std::vector<std::string> split_strs;
    std::string temp;
    std::istringstream iss(str);
    while (getline(iss, temp, seg))
    {
        split_strs.push_back(temp);
    }
    return split_strs;
}

void aq::Communicator::print_hex(unsigned char *_buf, int _len)
{
    for (int i = 0 ; i < _len ; i++)
    {
        printf("%02x ", _buf[i]);
    }
    std::cout << std::endl;
}

aq::Communicator::Communicator()
{
	ios[0] = new asio::io_service(1);
	ios[1] = new asio::io_service(1);
	ios[2] = new asio::io_service(1);
	ios[3] = new asio::io_service(1);
	ios[4] = new asio::io_service(1);
	ios[5] = new asio::io_service(1);
	ios[6] = new asio::io_service(1);
	ios[7] = new asio::io_service(1);

	sockets[0] = new tcp::socket(*ios[0]);//将socket->io_service变量设置为对应的ios[i]，在VCameraSession<Derived>::sendMessage(VCameraMessage && msg)中要用到io_service来发送信息
	sockets[1] = new tcp::socket(*ios[1]);
	sockets[2] = new tcp::socket(*ios[2]);
	sockets[3] = new tcp::socket(*ios[3]);
	sockets[4] = new tcp::socket(*ios[4]);
	sockets[5] = new tcp::socket(*ios[5]);
	sockets[6] = new tcp::socket(*ios[6]);
	sockets[7] = new tcp::socket(*ios[7]);

	sessions[0] = new Vsee::VTx2Session(*sockets[0]);
	sessions[1] = new Vsee::VTx2Session(*sockets[1]);
	using MutexLock = std::lock_guard<std::mutex>;

	eps[0] = tcp::endpoint(address::from_string("127.0.0.1"), 60000);
	eps[1] = tcp::endpoint(address::from_string("127.0.0.1"), 60000);
	eps[2] = tcp::endpoint(address::from_string("192.168.1.13"), PortNumber::PRC);
	eps[3] = tcp::endpoint(address::from_string("192.168.1.14"), PortNumber::PRC);
	eps[4] = tcp::endpoint(address::from_string("192.168.1.15"), PortNumber::PRC);
	eps[5] = tcp::endpoint(address::from_string("192.168.1.16"), PortNumber::PRC);
	eps[6] = tcp::endpoint(address::from_string("192.168.1.17"), PortNumber::PRC);
	eps[7] = tcp::endpoint(address::from_string("192.168.1.18"), PortNumber::PRC);
	//frame_queue_ = new std::queue<Vsee::VCameraFrame>;
	data_buffer_.resize(50000000);
}

bool aq::Communicator::tcp_connect(std::string _ip, int _port, std::string &_error_str)
{
    _error_str.clear();
    // check input
    std::vector<std::string> ip = split(_ip, '.');
    if (ip.size() != 4)
    {
        _error_str = "Invalid Ip. There is a size error.";
        return false;
    }
    for (auto iter : ip)
    {
        int tmp = -1;
        try
        {
            tmp = std::stoi(iter);
        }
        catch (std::invalid_argument e)
        {
            std::cerr << e.what() << std::endl;
            _error_str = std::string("Invalid Ip. There is an invalid argument: ") + iter + ".";
            return false;
        }
        if ((tmp < 0) || (tmp > 255))
        {
            _error_str = std::string("Invalid Ip. There is an argument: ") + iter + " out of range.";
            return false;
        }
    }
    if ((_port < 0) || (_port > 65535))
    {
        _error_str = std::string("Invalid Port. ") + std::to_string(_port) + " is out of range.";
        return false;
    }

	io_service_ = new asio::io_service;
	ep_ = new asio::ip::tcp::endpoint(asio::ip::address::from_string(_ip), _port);
	socket_ = new asio::ip::tcp::socket(*io_service_);
	asio::error_code ec;
	
	socket_->connect(*ep_, ec);
	if (ec)
	{
		_error_str = "The connection could have timed out.";
		return false;
	}
	data_buffer_.clear();
	data_buffer_.resize(1000000);
    _error_str = "Ok.";
    return true;
}

bool aq::Communicator::receive_img(std::string &_error_str)
{
	
	_error_str.clear();
	std::int64_t size = asio::read(*socket_, asio::buffer((char*)data_buffer_.data(), sizeof(VCameraMessageTrans)));
	VCameraMessageTrans* msg_in = (VCameraMessageTrans*)data_buffer_.data();
	int width = msg_in->width;
	int height = msg_in->height;
	int channels = msg_in->channels;
	int data_size = width*height*channels;
	int signalling = msg_in->signalling;
	int magnetic_valve_num = msg_in->magnetic_valve_num;
	VCameraMessageTrans* MsgPost = new VCameraMessageTrans;
	memcpy(MsgPost, msg_in, sizeof(VCameraMessageTrans));
	CString strMsg;
	strMsg.Format("receive_img: signalling=%d,magnetic_valve_num=%d", signalling, magnetic_valve_num);

	//Use ShowCtrlMedssage() function to do with the siganlling
	::PostMessage(m_hMainWnd, ID_SHOWCTRLMESSAGE, signalling, (LPARAM)MsgPost);

	std::int64_t next = Vsee::VCameraMessageTrans::nextLoad(data_buffer_.data(), data_buffer_.size());
	next = data_size;

	if (next > 0 && false)
	{
		asio::read(*socket_, asio::buffer((char*)data_buffer_.data(), next));
		//std::cout << "next : " << next << std::endl;
		//print_hex((unsigned char *)data_buffer_.c_str() + size, next);
	}
	else
		return true;		


	cv::Mat img(height, width, CV_8UC3, (void*)((char*)data_buffer_.data()));
	//cv::cvtColor(img, img, cv::COLOR_RGB2BGR);
	std::string file_path = "f:/save_image/" + std::to_string(counter) + ".bmp";
	cv::imwrite(file_path, img);
	counter++;

	Vsee::VCameraMessageTrans msg = Vsee::VCameraMessageTrans::load(data_buffer_.data(), data_buffer_.size());
	Vsee::VCameraMessageTrans frame(std::move(msg));
	if (msg.empty())
	{
		_error_str = "Empty camera message.";
		return false;
	}
	if (msg.checkCrc())
	{
		switch (msg.token())
		{
		case 51:
		{
			Vsee::VCameraFrame frame;// (std::move(msg));
			if (frame.format() == Vsee::FrameFormat::RGB)
			{
				m_.lock();
				frame_queue_->push(frame);
				m_.unlock();
			}
			break;
		}
		default:
			break;
		}
	}
	else
	{
		_error_str = "CRC error.";
		return false;
	}
	
    return true;
}

bool aq::Communicator::receive_img(std::string &_error_str, asio::basic_stream_socket<asio::ip::tcp>* socket)
{

	_error_str.clear();
	std::int64_t size = asio::read(*socket, asio::buffer((char*)data_buffer_.data(), sizeof(VCameraMessageTrans)));
	VCameraMessageTrans* msg_in = (VCameraMessageTrans*)data_buffer_.data();
	int width = msg_in->width;
	int height = msg_in->height;
	int channels = msg_in->channels;
	int data_size = width*height*channels;
	int data_length = msg_in->data_length;
	int signalling = msg_in->signalling;
	int magnetic_valve_num = msg_in->magnetic_valve_num;    //Here Use magnetic_valve_num to indicate the camera serial number
	VCameraMessageTrans* MsgPost = new VCameraMessageTrans;
	memcpy(MsgPost, msg_in, sizeof(VCameraMessageTrans));
	CString strMsg;
	strMsg.Format("receive_img: signalling=%d,magnetic_valve_num=%d", signalling, magnetic_valve_num);
	//::PostMessage(AfxGetMainWnd()->m_hWnd, ID_SHOWCTRLMESSAGE, signalling, (LPARAM)magnetic_valve_num);
	if (signalling==3)
		::PostMessage(m_hMainWnd, ID_SHOWCTRLMESSAGE, signalling, (LPARAM)MsgPost);

	//std::int64_t next = Vsee::VCameraMessageTrans::nextLoad(data_buffer_.data(), data_buffer_.size());
	//next = data_size;

	if (signalling == 20)
	{
		std::int64_t m_size = asio::read(*socket, asio::buffer((char*)data_buffer_.data(), data_length));
		
		Info1.channels = channels;
		Info1.width = width;
		Info1.height = height;
		Info1.data_legnth = data_length;
		//Info.m_picture_data = new char[data_length];
		Info1.m_picture_data = (char*)data_buffer_.data();
		//memcpy(Info.m_picture_data, (char*)data_buffer_.data(), data_length);
		Info1.camera_serial = magnetic_valve_num;
		::PostMessage(m_hMainWnd, ID_SHOWPICTURE, signalling, (LPARAM)&Info1);
	}
	else
		return true;

	return true;
}


bool aq::Communicator::receive_img(std::string &_error_str, asio::basic_stream_socket<asio::ip::tcp>* socket,std::string &data_buffer)
{
	//struct information Info1;
	int width ;
	int height;
	int channels;
	int data_size;
	int data_length;
	int signalling;
	int magnetic_valve_num;
	int flag1 = 0;
	_error_str.clear();
	std::string sClientIp1 = socket->remote_endpoint().address().to_string();
	if (sClientIp1 == "192.168.1.100") 
		flag1 = 1;
	else if (sClientIp1 == "192.168.1.101")
		flag1 = 2;
	std::int64_t size = asio::read(*socket, asio::buffer((char*)data_buffer.data(), sizeof(VCameraMessageTrans)));
	VCameraMessageTrans* msg_in = (VCameraMessageTrans*)data_buffer.data();
	width = msg_in->width;
	height = msg_in->height;
	channels = msg_in->channels;
	data_size = width*height*channels;
	data_length = msg_in->data_length;
	signalling = msg_in->signalling;
	magnetic_valve_num = msg_in->magnetic_valve_num;    //Here Use magnetic_valve_num to indicate the camera serial number
	VCameraMessageTrans* MsgPost = new VCameraMessageTrans;
	memcpy(MsgPost, msg_in, sizeof(VCameraMessageTrans));
	CString strMsg;
	strMsg.Format("receive_img: signalling=%d,magnetic_valve_num=%d", signalling, magnetic_valve_num);
	//::PostMessage(AfxGetMainWnd()->m_hWnd, ID_SHOWCTRLMESSAGE, signalling, (LPARAM)magnetic_valve_num);
	if (signalling == 3)
		::PostMessage(m_hMainWnd, ID_SHOWCTRLMESSAGE, signalling, (LPARAM)MsgPost);
	else if (signalling == 20)
	{
		std::string sClientIp2 = socket->remote_endpoint().address().to_string();
		if (flag1 == 1)
		{
			std::int64_t m_size = asio::read(*socket, asio::buffer((char*)data_buffer.data(), data_length));
			Info1.channels = channels;
			Info1.width = width;
			Info1.height = height;
			Info1.data_legnth = data_length;
			Info1.m_picture_data = (char*)data_buffer.data();
			//Info.m_picture_data = new char[data_length];
			//memcpy(Info.m_picture_data, (char*)data_buffer.data(), data_length);
			Info1.camera_serial = magnetic_valve_num;
			::PostMessage(m_hMainWnd, ID_SHOWPICTURE, signalling, (LPARAM)&Info1);
			flag1 = 0;
		}
		else if (flag1 == 2)
		{
			std::int64_t m_size = asio::read(*socket, asio::buffer((char*)data_buffer.data(), data_length));
			Info2.channels = channels;
			Info2.width = width;
			Info2.height = height;
			Info2.data_legnth = data_length;
			Info2.m_picture_data = (char*)data_buffer.data();
			//Info.m_picture_data = new char[data_length];
			//memcpy(Info.m_picture_data, (char*)data_buffer.data(), data_length);
			Info2.camera_serial = magnetic_valve_num;
			::PostMessage(m_hMainWnd, ID_SHOWPICTURE, signalling, (LPARAM)&Info2);
			flag1 = 0;
		}
	}

	else
		return true;
	//}
	//::PostMessage(AfxGetMainWnd()->m_hWnd, ID_SHOWCTRLMESSAGE, signalling, (LPARAM)magnetic_valve_num);


	return true;
}

bool aq::Communicator::send_cmd(aq::CMD _cmd, std::string &_error_str, std::vector<int> _state, int _sequence)
{
	
    _error_str.clear();
    if (_cmd == CMD::SEND_RESULTS)
    {
        std::uint32_t result[2];
        result[0] = _sequence;
		std::uint32_t state = 0;
        for (int i = 0 ; i < 8 ; i++)
        {
            if (i < _state.size())
            {
				state += _state[i] * std::pow(16, 7 - i);
            }
            else
            {
            }
        }
		result[1] = state;

		Vsee::VCameraMessageTrans response((int)_cmd, (const char*)result, sizeof(result));
		std::cout << "send_cmd================================>> : " << _sequence << std::endl;
		print_hex((unsigned char*)response.data(), response.dataSize());
		std::cout << "<<================================send_cmd" << std::endl;
		//const std::string &tmp = response.;
		auto buf = asio::buffer(response.bytes(), response.byteSize());
		//std::this_thread::sleep_for(std::chrono::milliseconds(20));
		socket_->write_some(buf);
    }
    else
    {
		std::uint32_t result[1];
		result[0] = _sequence;
		//for (int i = 0; i < 8; i++)
		//{
		//	if (i < _state.size())
		//	{
		//		result[i + 1] = _state[i];
		//	}
		//	else
		//	{
		//		result[i + 1] = 0;
		//	}
		//}

		Vsee::VCameraMessageTrans response((int)_cmd, nullptr, 0);
		socket_->write_some(asio::buffer(response.data(), response.dataSize()));
    }
	
    return true;
}


//*****************************************************************************************************
//author :	LF
//time:		2020-9-16
//parameters:
//			std::string _ip : the local server ip adress
//			int _port : the local server  net port for machine server 
//function: To start a Server state machine,to wait for all the net request from client,and when require income,
//			then start a server thread for the net connect;
//******************************************************************************************************
bool aq::Communicator::server(std::string _ip, int _port, std::string &_error_str)
{
	
	_error_str.clear();
//	MessageBox(NULL, "Communicator::server", "ddd", MB_OK);
	// check input
	std::vector<std::string> ip = split(_ip, '.');
	if (ip.size() != 4)
	{
		_error_str = "Invalid Ip. There is a size error.";
		return false;
	}
	for (auto iter : ip)
	{
		int tmp = -1;
		try
		{
			tmp = std::stoi(iter);
		}
		catch (std::invalid_argument e)
		{
			std::cerr << e.what() << std::endl;
			_error_str = std::string("Invalid Ip. There is an invalid argument: ") + iter + ".";
			return false;
		}
		if ((tmp < 0) || (tmp > 255))
		{
			_error_str = std::string("Invalid Ip. There is an argument: ") + iter + " out of range.";
			return false;
		}
	}
	if ((_port < 0) || (_port > 65535))
	{
		_error_str = std::string("Invalid Port. ") + std::to_string(_port) + " is out of range.";
		return false;
	}

	asio::error_code ec;
	//asio::io_service ios;
	
	while (true)
	{
		try
		{
			m_Log.Add("server run again!");
			io_service_ = new asio::io_service;
			ep_ = new asio::ip::tcp::endpoint(asio::ip::address::from_string(_ip), _port);
			socket_ = new asio::ip::tcp::socket(*io_service_);
			asio::ip::tcp::acceptor acceptor(*io_service_, *ep_);
			int a = 1;
			acceptor.accept(*socket_);            //When a net connect is requseted,then start a server thrread for the connection;
			//acceptor.accept(*sockets[0]);
			{
				char* MsgPost = new char[64];
				strcpy(MsgPost, "AIDI Box Client Connetc in!");
				::PostMessage(m_hMainWnd, ID_SHOWCTRLMESSAGE, 10, (LPARAM)MsgPost);
				//std::thread t(&aq::Communicator::server_run, this, sockets[0], _ip, _port, _error_str);	//Start receive&send serer function
				std::thread t(&aq::Communicator::server_run, this, socket_, _ip, _port, _error_str);	//Start receive&send serer function
				//std::thread t(&aq::Communicator::server_run, this, socket_, _ip, _port, _error_str);	//Start receive&send serer function
				t.detach();
			}
		}
		catch (CMemoryException* e)
		{
			
		}
		catch (CFileException* e)
		{
		}
		catch (CException* e)
		{
		}
		catch (...)
		{
		}
		
		//服务器端程序里要注意的是自由函数buffer(),它可用包装很多种类的容器成为asio组件可用的缓冲区类型。通常我们不能直接把数组，vector等容器用作asio的读写参数，必须使用buffer()函数包装。
	}

	if (ec)
	{
		_error_str = "The connection could have timed out.";
		return false;
	}
	
	return true;
}


//*****************************************************************************************************
//author :	LF
//time:		2020-9-16
//parameters:
//			std::string _ip : the local server ip adress
//			int _port : the local server  net port for machine server 
//function: To start a Server state machine,to wait for all the net request from client,and when require income,
//			then start a server thread for the net connect;
//******************************************************************************************************
bool aq::Communicator::img_show_server(std::string _ip, int _port, std::string &_error_str)
{

	_error_str.clear();
	//	MessageBox(NULL, "Communicator::server", "ddd", MB_OK);
	// check input
	std::vector<std::string> ip = split(_ip, '.');
	if (ip.size() != 4)
	{
		_error_str = "Invalid Ip. There is a size error.";
		return false;
	}
	for (auto iter : ip)
	{
		int tmp = -1;
		try
		{
			tmp = std::stoi(iter);
		}
		catch (std::invalid_argument e)
		{
			std::cerr << e.what() << std::endl;
			_error_str = std::string("Invalid Ip. There is an invalid argument: ") + iter + ".";
			return false;
		}
		if ((tmp < 0) || (tmp > 255))
		{
			_error_str = std::string("Invalid Ip. There is an argument: ") + iter + " out of range.";
			return false;
		}
	}
	if ((_port < 0) || (_port > 65535))
	{
		_error_str = std::string("Invalid Port. ") + std::to_string(_port) + " is out of range.";
		return false;
	}

	asio::error_code ec;
	//asio::io_service ios;

	while (true)
	{
		try
		{
			m_Log.Add("server run again!");
			img_io_service_ = new asio::io_service;
			img_ep_ = new asio::ip::tcp::endpoint(asio::ip::address::from_string(_ip), _port);
			img_socket_ = new asio::ip::tcp::socket(*img_io_service_);
			asio::ip::tcp::acceptor acceptor(*img_io_service_, *img_ep_);

			acceptor.accept(*img_socket_);            //When a net connect is requseted,then start a server thrread for the connection;
			//acceptor.accept(*sockets[0]);
			{
				char* MsgPost = new char[64];
				strcpy(MsgPost, "AIDI Box Image Show Client Connetc in!");
				::PostMessage(m_hMainWnd, ID_SHOWCTRLMESSAGE, 10, (LPARAM)MsgPost);
				//std::thread t(&aq::Communicator::server_run, this, sockets[0], _ip, _port, _error_str);	//Start receive&send serer function
				std::thread t(&aq::Communicator::server_get_image_run, this, img_socket_, _ip, _port, _error_str);	//Start receive&send serer function
				t.detach();
			}
		}
		catch (CMemoryException* e)
		{

		}
		catch (CFileException* e)
		{
		}
		catch (CException* e)
		{
		}
		catch (...)
		{
		}

		//服务器端程序里要注意的是自由函数buffer(),它可用包装很多种类的容器成为asio组件可用的缓冲区类型。通常我们不能直接把数组，vector等容器用作asio的读写参数，必须使用buffer()函数包装。
	}

	if (ec)
	{
		_error_str = "The connection could have timed out.";
		return false;
	}

	return true;
}



bool aq::Communicator::img_show_server1(std::string _ip, int _port, std::string &_error_str)
{

	_error_str.clear();
	//	MessageBox(NULL, "Communicator::server", "ddd", MB_OK);
	// check input
	std::vector<std::string> ip = split(_ip, '.');
	if (ip.size() != 4)
	{
		_error_str = "Invalid Ip. There is a size error.";
		return false;
	}
	for (auto iter : ip)
	{
		int tmp = -1;
		try
		{
			tmp = std::stoi(iter);
		}
		catch (std::invalid_argument e)
		{
			std::cerr << e.what() << std::endl;
			_error_str = std::string("Invalid Ip. There is an invalid argument: ") + iter + ".";
			return false;
		}
		if ((tmp < 0) || (tmp > 255))
		{
			_error_str = std::string("Invalid Ip. There is an argument: ") + iter + " out of range.";
			return false;
		}
	}
	if ((_port < 0) || (_port > 65535))
	{
		_error_str = std::string("Invalid Port. ") + std::to_string(_port) + " is out of range.";
		return false;
	}

	asio::error_code ec;
	//asio::io_service ios;

	while (true)
	{
		try
		{
			m_Log.Add("server run again!");
			img_io_service_1 = new asio::io_service;
			img_ep_1 = new asio::ip::tcp::endpoint(asio::ip::address::from_string(_ip), _port);
			img_socket_1 = new asio::ip::tcp::socket(*img_io_service_1);
			asio::ip::tcp::acceptor acceptor1(*img_io_service_1, *img_ep_1);

			acceptor1.accept(*img_socket_1);            //When a net connect is requseted,then start a server thrread for the connection;
			//acceptor.accept(*sockets[0]);
			{
				char* MsgPost = new char[64];
				strcpy(MsgPost, "AIDI Box Image Show Client Connetc in!");
				::PostMessage(m_hMainWnd, ID_SHOWCTRLMESSAGE, 10, (LPARAM)MsgPost);
				//std::thread t(&aq::Communicator::server_run, this, sockets[0], _ip, _port, _error_str);	//Start receive&send serer function
				std::thread t(&aq::Communicator::server_get_image_run, this, img_socket_1, _ip, _port, _error_str);	//Start receive&send serer function
				t.detach();
			}
		}
		catch (CMemoryException* e)
		{

		}
		catch (CFileException* e)
		{
		}
		catch (CException* e)
		{
		}
		catch (...)
		{
		}

		//服务器端程序里要注意的是自由函数buffer(),它可用包装很多种类的容器成为asio组件可用的缓冲区类型。通常我们不能直接把数组，vector等容器用作asio的读写参数，必须使用buffer()函数包装。
	}

	if (ec)
	{
		_error_str = "The connection could have timed out.";
		return false;
	}

	return true;
}

bool aq::Communicator::server_run(asio::basic_stream_socket<asio::ip::tcp>* socket_, std::string _ip, int _port, std::string &_error_str)
{
	size_t size = 100;
	data_buffer_.clear();
	data_buffer_.resize(1000000);
	char* buffer = new char[size];
	int nWidth ;
	int nHeight ;
	int nChannnels ;
	int file_size;
	Vsee::VCameraFrame VFrame;
	cv::Mat image;
	image = cv::imread("f:/" + std::to_string(1) + ".bmp");
	int file_total=0;
	int file_countr = 0;
	int length = 0;
	VCameraMessageTrans msg(buffer, size);
	std::string sClientIp = socket_->remote_endpoint().address().to_string();
	try
	{
		while (m_bServerRun)
		{
			if (m_bServerSendMode)  //if the mode is server sending,and client receive image ,when in control mode ,the control softwear is this mode;
			{
				if ((sClientIp == "192.168.1.100"&&box_ctrl_msg.parameters_setting_trans.box_num == 1)\
					|| (sClientIp == "192.168.1.101"&&box_ctrl_msg.parameters_setting_trans.box_num == 2)
					|| box_ctrl_msg.parameters_setting_trans.box_num == 3)
				{
				nWidth = image.cols;
				nHeight = image.rows;
				nChannnels = image.channels();
				file_size = file_list.size();
				std::string file_path;

				file_total = file_list.size();

				//VCameraMessageTrans* msg = new VCameraMessageTrans;
				if (box_ctrl_msg.signalling == 0)		 //Idle time,keep loop;
				{
					//Sleep(200);
					std::this_thread::sleep_for(std::chrono::microseconds(100));
					continue;
				}
				else if (box_ctrl_msg.signalling == 1)       //Start detect fiber
				{

					auto& io = socket_->get_io_service();
					if (!socket_->is_open())
						return false;
					asio::error_code er;
					auto buf = asio::buffer(&box_ctrl_msg, sizeof(box_ctrl_msg));
					//int nLenght = msg.byteSize();
					socket_->write_some(buf);
					box_ctrl_msg.signalling = 0;
				}
				else if (box_ctrl_msg.signalling == 2)       //Stop detect fiber
				{
					auto& io = socket_->get_io_service();
					if (!socket_->is_open())
						return false;
					asio::error_code er;
					auto buf = asio::buffer(&box_ctrl_msg, sizeof(box_ctrl_msg));
					//int nLenght = msg.byteSize();
					socket_->write_some(buf);
					box_ctrl_msg.signalling = 0;
				}
				else if (box_ctrl_msg.signalling == 3)       //Start test  electro magnetic valve
				{                                            //box_ctrl_msg.magnetic_valve_num: the serial number of electro magnetic valve;
					auto& io = socket_->get_io_service();
					if (!socket_->is_open())
						return false;
					asio::error_code er;
					auto buf = asio::buffer(&box_ctrl_msg, sizeof(box_ctrl_msg));
					socket_->write_some(buf);
					box_ctrl_msg.signalling = 0;
				}
				else if (box_ctrl_msg.signalling == 4)       //To set camera detect parameters
				{
					auto& io = socket_->get_io_service();
					if (!socket_->is_open())
						return false;
					asio::error_code er;
					auto buf = asio::buffer(&box_ctrl_msg, sizeof(box_ctrl_msg));
					socket_->write_some(buf);
					box_ctrl_msg.signalling = 0;
				}
				else if (box_ctrl_msg.signalling == 5)       //To set box and camera network parameters
				{
					auto& io = socket_->get_io_service();
					if (!socket_->is_open())
						return false;
					asio::error_code er;
					auto buf = asio::buffer(&box_ctrl_msg, sizeof(box_ctrl_msg));
					socket_->write_some(buf);
					box_ctrl_msg.signalling = 0;
				}
				else if (box_ctrl_msg.signalling == 6)       //To set box model select parameters
				{
					auto& io = socket_->get_io_service();
					if (!socket_->is_open())
						return false;
					asio::error_code er;
					auto buf = asio::buffer(&box_ctrl_msg, sizeof(box_ctrl_msg));
					socket_->write_some(buf);
					box_ctrl_msg.signalling = 0;
				}

				else if (box_ctrl_msg.signalling == 7)       //To start grab image and save image.
				{
					auto& io = socket_->get_io_service();
					if (!socket_->is_open())
						return false;
					asio::error_code er;
					auto buf = asio::buffer(&box_ctrl_msg, sizeof(box_ctrl_msg));
					socket_->write_some(buf);
					box_ctrl_msg.signalling = 0;
				}

				else if (box_ctrl_msg.signalling == 8)      //To stop grab image and save image.
				{
					auto& io = socket_->get_io_service();
					if (!socket_->is_open())
						return false;
					asio::error_code er;
					auto buf = asio::buffer(&box_ctrl_msg, sizeof(box_ctrl_msg));
					socket_->write_some(buf);
					box_ctrl_msg.signalling = 0;
				}
				else if (box_ctrl_msg.signalling == 9)      //To upload  grab image to server.
				{
					auto& io = socket_->get_io_service();
					if (!socket_->is_open())
						return false;
					asio::error_code er;
					auto buf = asio::buffer(&box_ctrl_msg, sizeof(box_ctrl_msg));
					socket_->write_some(buf);
					box_ctrl_msg.signalling = 0;
				}
				else if (box_ctrl_msg.signalling == 11)      //To start camera  grab image to server.
				{
					auto& io = socket_->get_io_service();
					if (!socket_->is_open())
						return false;
					asio::error_code er;
					auto buf = asio::buffer(&box_ctrl_msg, sizeof(box_ctrl_msg));
					socket_->write_some(buf);
					box_ctrl_msg.signalling = 0;
				}
				else if (box_ctrl_msg.signalling == 12)      //To stop camera  grab image .
				{
					auto& io = socket_->get_io_service();
					if (!socket_->is_open())
						return false;
					asio::error_code er;
					auto buf = asio::buffer(&box_ctrl_msg, sizeof(box_ctrl_msg));
					socket_->write_some(buf);
					box_ctrl_msg.signalling = 0;
				}
				else if (box_ctrl_msg.signalling == 13)      //To start camera  grab mode .
				{
					auto& io = socket_->get_io_service();
					if (!socket_->is_open())
						return false;
					asio::error_code er;
					auto buf = asio::buffer(&box_ctrl_msg, sizeof(box_ctrl_msg));
					socket_->write_some(buf);
					box_ctrl_msg.signalling = 0;
				}
				else if (box_ctrl_msg.signalling == 14)      //To stop camera  grab mode .
				{
					auto& io = socket_->get_io_service();
					if (!socket_->is_open())
						return false;
					asio::error_code er;
					auto buf = asio::buffer(&box_ctrl_msg, sizeof(box_ctrl_msg));
					socket_->write_some(buf);
					box_ctrl_msg.signalling = 0;
				}
				else if (box_ctrl_msg.signalling == 20)
				{
					while (file_countr < file_total - 1)
					{
						if (aq::Communicator::batch_image_load)
						{
							file_path = file_list[file_countr];
							file_countr++;
							image = cv::imread(file_path);
							nWidth = image.cols;
							nHeight = image.rows;
							nChannnels = image.channels();
							msg.width = image.cols;
							msg.height = image.rows;
							msg.channels = image.channels();
							length = nWidth*nHeight*nChannnels;

							//data_buffer_.clear();
							//std::int64_t size = asio::read(*socket_, asio::buffer((char*)data_buffer_.data(), 8));
							size = asio::read(*socket_, asio::buffer((char*)data_buffer_.data(), sizeof(VCameraMessageTrans)));
							VCameraMessageTrans* msg2 = (VCameraMessageTrans*)data_buffer_.data();
							nWidth = msg2->width;
							nHeight = msg2->height;
							int nSignalling = msg2->signalling;

							if (size != 0)
							{
								//sessions[0]->sendMessage(msg);
								{
									auto& io = socket_->get_io_service();
									if (!socket_->is_open())
										return false;
									asio::error_code er;
									auto buf = asio::buffer(&msg, sizeof(msg));
									int nLenght = msg.byteSize();
									socket_->write_some(buf);

									//auto buf = asio::buffer(msg.bytes(), msg.byteSize());
									buf = asio::buffer(image.data, length);

									//asio::async_write(_socket, buf, [keep = std::move(msg)](asio::error_code, std::size_t){});
									//asio::write(*_socket, buf, NULL, &er);
									socket_->write_some(buf);
								}
								std::cout << "client:ip:" << socket_->remote_endpoint().address() << "   port:" << socket_->remote_endpoint().port() << std::endl;

							}
							else
							{
								Sleep(1000);
							}
							int sleep_times = 100 / camera_scan_speed;
							Sleep(sleep_times);
						}
						else if (aq::Communicator::single_image_load)
						{
							aq::Communicator::single_image_load = false;
							file_path = file_list[file_countr];
							file_countr++;
							image = cv::imread(file_path);
							nWidth = image.cols;
							nHeight = image.rows;
							nChannnels = image.channels();
							msg.width = image.cols;
							msg.height = image.rows;
							msg.channels = image.channels();
							length = nWidth*nHeight*nChannnels;

							//data_buffer_.clear();
							//std::int64_t size = asio::read(*socket_, asio::buffer((char*)data_buffer_.data(), 8));
							size = asio::read(*socket_, asio::buffer((char*)data_buffer_.data(), sizeof(VCameraMessageTrans)));
							VCameraMessageTrans* msg2 = (VCameraMessageTrans*)data_buffer_.data();
							nWidth = msg2->width;
							nHeight = msg2->height;
							int nSignalling = msg2->signalling;

							if (size != 0)
							{
								//sessions[0]->sendMessage(msg);
								{
									auto& io = socket_->get_io_service();
									if (!socket_->is_open())
										return false;
									asio::error_code er;
									auto buf = asio::buffer(&msg, sizeof(msg));
									int nLenght = msg.byteSize();
									socket_->write_some(buf);

									//auto buf = asio::buffer(msg.bytes(), msg.byteSize());
									buf = asio::buffer(image.data, length);

									//asio::async_write(_socket, buf, [keep = std::move(msg)](asio::error_code, std::size_t){});
									//asio::write(*_socket, buf, NULL, &er);
									socket_->write_some(buf);
								}
								std::cout << "client:ip:" << socket_->remote_endpoint().address() << "   port:" << socket_->remote_endpoint().port() << std::endl;
							}
							else
							{
								Sleep(1000);
							}
						}
						else
						{
							Sleep(300);
						}

					}

				}
			}
			}
			else  //the mode is server receive,and clietn send image;
			{
				VCameraMessageTrans msg;
				std::string error_str;
				msg.width = 115;
				msg.height = 115;
				msg.signalling = 2;
				counter = 0;
				//		send_cmd(aq::CMD::START_RECV_IMG, error_str);
				while (1)
				{
					//auto buf = asio::buffer(&msg, sizeof(msg));
					//socket_->write_some(buf);
					std::string s;
					if (!receive_img(s, socket_))
					{
						std::cerr << s << std::endl;
					}
				}

			}
		}
	}
	catch (CMemoryException* e)
	{
		
	}
	catch (CFileException* e)
	{
	}
	catch (CException* e)
	{
	}
	catch (...)
	{
	}
	
	return true;
}
bool aq::Communicator::get_file_list(std::string file_load_path)
{
	CString m_strFolderPath;
	CString m_strFileExt;
	char strFilePath[256];
	memset(strFilePath, 0, 256);
	strcpy(strFilePath, file_load_path.data());
	int nNameLength = strlen(strFilePath);
	char strFileExt[8];
	memset(strFileExt, 0, 8);
	int i = 0;
	int nDotIndex = 0;
	file_amount = 0;
	for (i = nNameLength - 1; i > 0; i--)
	{
		char ch = strFilePath[i];
		if (ch == '.')
			break;
	}
	strncpy(strFileExt, strFilePath + i + 1, nNameLength - i - 1);
	m_strFileExt.Format("%s", strFileExt);
	for (i = nNameLength - 1; i > 0; i--)
	{
		char ch = strFilePath[i];
		if (ch == '\\')
			break;
	}
	strFilePath[i] = '\0';
	m_strFolderPath.Format("%s", strFilePath);
	std::string strFolderPath = m_strFolderPath;
	get_files(strFolderPath, strFileExt, file_amount);
	return true;
}

int aq::Communicator::get_files(std::string fileFolderPath, std::string fileExtension, int& nFileNum)
{
	std::string fileFolder = fileFolderPath + "\\*" + fileExtension;
	std::string fileName;
	struct _finddata_t fileInfo;
	nFileNum = 0;
	long long findResult = _findfirst(fileFolder.c_str(), &fileInfo);
	if (findResult == -1)
	{
		_findclose(findResult);
		return 0;
	}
	bool flag = 0;

	do
	{
		fileName = fileFolderPath + "\\" + fileInfo.name;
		if (fileInfo.attrib == _A_ARCH)
		{
			file_list.push_back(fileName);
			nFileNum++;
		}
	} while (_findnext(findResult, &fileInfo) == 0);

	_findclose(findResult);
}

bool  aq::Communicator::run_tcp_connect(std::string _ip, int _port)
{
	std::thread t(&aq::Communicator::tcp_connect_thread, this ,_ip, _port);
	//std::thread t(&aq::Communicator::server_run, this, socket_, _ip, _port, _error_str);
	t.detach();
	return true;
}


bool aq::Communicator::tcp_connect_thread(std::string _ip, int _port)
{
	//CWnd::MessageBox("to tcp_connetct!");
	//MessageBox(NULL, "tcp_connect", "ddd", MB_OK);

	if (!m_bClientSendMode)		// if mode is client receive,and server send image mode
	{
		try
		{
			std::string error_str;
			if (!tcp_connect(_ip, _port, error_str))
			{
				std::cerr << error_str << std::endl;
				return false;
			}
			std::cout << "connected " << _ip << ":" << _port << std::endl;
			VCameraMessageTrans msg;
			msg.width = 115;
			msg.height = 115;
			msg.signalling = 2;
			counter = 0;
			//send_cmd(aq::CMD::START_RECV_IMG, error_str);
			while (m_bClientRun)
			{
				auto buf = asio::buffer(&msg, sizeof(msg));
				//socket_->write_some(buf);
				std::string s;
				if (!receive_img(s))
				{
					std::cerr << s << std::endl;
				}
			}
		}
		catch (CMemoryException* e)
		{

		}
		catch (CFileException* e)
		{
		}
		catch (CException* e)
		{
		}
		catch (...)
		{
		}
		m_bClientRun = false;

	}
	else if (m_bClientSendMode)  //if the mode is client sending,and server receive image 
	{
		try
		{
			std::string error_str;
			if (!tcp_connect(_ip, _port, error_str))
			{
				std::cerr << error_str << std::endl;
				return false;
			}
			Vsee::VCameraFrame VFrame;
			cv::Mat image;
			image = cv::imread("f:/" + std::to_string(1) + ".bmp");
			int nWidth = image.cols;
			int nHeight = image.rows;
			int nChannnels = image.channels();
			size_t size = 10000000;
			char* buffer = new char[size];
			//	VCameraMessagePrivate* pre = new VCameraMessagePrivate();
			VCameraMessageTrans msg(buffer, size);
			int n_size = sizeof(VCameraMessageTrans);
			int n_size2 = sizeof(msg);
			msg.width = image.cols;
			msg.height = image.rows;
			msg.channels = image.channels();
			int length = nWidth*nHeight*nChannnels;
			//data_buffer_.clear();
			//data_buffer_.resize(1000000);
			//get_file_list(file_load_path);
			int file_size = file_list.size();
			std::string file_path;

			msg.setData((char*)image.data, nWidth*nHeight*nChannnels, true);
			//sessions[0]->sendMessage(msg);

			//Vsee::VCameraFrame frame;// (std::move(msg));
			//nWidth = frame.width();
			//nHeight = frame.height();
			int file_total = file_list.size();
			int file_countr = 0;
			//VCameraMessageTrans* msg = new VCameraMessageTrans;
			while (m_bClientRun)
			{
				if (box_ctrl_msg.signalling == 0)		 //Idle time,keep loop;
				{
					Sleep(200);
					continue;
				}
				else if (box_ctrl_msg.signalling == 1)       //Start detect fiber
				{

					auto& io = socket_->get_io_service();
					if (!socket_->is_open())
						return false;
					asio::error_code er;
					auto buf = asio::buffer(&box_ctrl_msg, sizeof(box_ctrl_msg));
					int nLenght = msg.byteSize();
					socket_->write_some(buf);
					box_ctrl_msg.signalling = 0;
				}
				else if (box_ctrl_msg.signalling == 2)       //Stop detect fiber
				{
					auto& io = socket_->get_io_service();
					if (!socket_->is_open())
						return false;
					asio::error_code er;
					auto buf = asio::buffer(&box_ctrl_msg, sizeof(box_ctrl_msg));
					int nLenght = msg.byteSize();
					socket_->write_some(buf);
					box_ctrl_msg.signalling = 0;
				}
				else if (box_ctrl_msg.signalling == 3)       //Start test  electro magnetic valve
				{                                            //box_ctrl_msg.magnetic_valve_num: the serial number of electro magnetic valve;
					auto& io = socket_->get_io_service();
					if (!socket_->is_open())
						return false;
					asio::error_code er;
					//CString strMsg;
					//strMsg.Format("box_ctrl_msg.signalling == 3");
					//MessageBox(NULL, strMsg, "magnetic_valve testing", MB_OK);
					auto buf = asio::buffer(&box_ctrl_msg, sizeof(box_ctrl_msg));
					int nLenght = msg.byteSize();
					socket_->write_some(buf);
					box_ctrl_msg.signalling = 0;
				}
				else if (box_ctrl_msg.signalling == 4)       //To set camera detect parameters
				{
					auto& io = socket_->get_io_service();
					if (!socket_->is_open())
						return false;
					asio::error_code er;
					//CString strMsg;
					//strMsg.Format("box_ctrl_msg.signalling == 3");
					//MessageBox(NULL, strMsg, "magnetic_valve testing", MB_OK);
					auto buf = asio::buffer(&box_ctrl_msg, sizeof(box_ctrl_msg));
					int nLenght = msg.byteSize();
					socket_->write_some(buf);
					box_ctrl_msg.signalling = 0;
				}
				else if (box_ctrl_msg.signalling == 20)
				{
					while (file_countr < file_total - 1)
					{
						if (aq::Communicator::batch_image_load)
						{
							file_path = file_list[file_countr];
							file_countr++;
							image = cv::imread(file_path);
							nWidth = image.cols;
							nHeight = image.rows;
							nChannnels = image.channels();
							msg.width = image.cols;
							msg.height = image.rows;
							msg.channels = image.channels();
							length = nWidth*nHeight*nChannnels;

							//data_buffer_.clear();
							//std::int64_t size = asio::read(*socket_, asio::buffer((char*)data_buffer_.data(), 8));
							size = asio::read(*socket_, asio::buffer((char*)data_buffer_.data(), sizeof(VCameraMessageTrans)));
							VCameraMessageTrans* msg2 = (VCameraMessageTrans*)data_buffer_.data();
							nWidth = msg2->width;
							nHeight = msg2->height;
							int nSignalling = msg2->signalling;

							if (size != 0)
							{
								//sessions[0]->sendMessage(msg);
								{
									auto& io = socket_->get_io_service();
									if (!socket_->is_open())
										return false;
									asio::error_code er;
									auto buf = asio::buffer(&msg, sizeof(msg));
									int nLenght = msg.byteSize();
									socket_->write_some(buf);

									//auto buf = asio::buffer(msg.bytes(), msg.byteSize());
									buf = asio::buffer(image.data, length);

									//asio::async_write(_socket, buf, [keep = std::move(msg)](asio::error_code, std::size_t){});
									//asio::write(*_socket, buf, NULL, &er);
									socket_->write_some(buf);
								}
								std::cout << "client:ip:" << socket_->remote_endpoint().address() << "   port:" << socket_->remote_endpoint().port() << std::endl;

							}
							else
							{
								Sleep(1000);
							}
							int sleep_times = 100 / camera_scan_speed;
							Sleep(sleep_times);
						}
						else if (aq::Communicator::single_image_load)
						{
							aq::Communicator::single_image_load = false;
							file_path = file_list[file_countr];
							file_countr++;
							image = cv::imread(file_path);
							nWidth = image.cols;
							nHeight = image.rows;
							nChannnels = image.channels();
							msg.width = image.cols;
							msg.height = image.rows;
							msg.channels = image.channels();
							length = nWidth*nHeight*nChannnels;

							//data_buffer_.clear();
							//std::int64_t size = asio::read(*socket_, asio::buffer((char*)data_buffer_.data(), 8));
							size = asio::read(*socket_, asio::buffer((char*)data_buffer_.data(), sizeof(VCameraMessageTrans)));
							VCameraMessageTrans* msg2 = (VCameraMessageTrans*)data_buffer_.data();
							nWidth = msg2->width;
							nHeight = msg2->height;
							int nSignalling = msg2->signalling;

							if (size != 0)
							{
								//sessions[0]->sendMessage(msg);
								{
									auto& io = socket_->get_io_service();
									if (!socket_->is_open())
										return false;
									asio::error_code er;
									auto buf = asio::buffer(&msg, sizeof(msg));
									int nLenght = msg.byteSize();
									socket_->write_some(buf);

									//auto buf = asio::buffer(msg.bytes(), msg.byteSize());
									buf = asio::buffer(image.data, length);

									//asio::async_write(_socket, buf, [keep = std::move(msg)](asio::error_code, std::size_t){});
									//asio::write(*_socket, buf, NULL, &er);
									socket_->write_some(buf);
								}
								std::cout << "client:ip:" << socket_->remote_endpoint().address() << "   port:" << socket_->remote_endpoint().port() << std::endl;
							}
							else
							{
								Sleep(1000);
							}
						}
						else
						{
							Sleep(300);
						}

					}

				}
			}
		}
		catch (CMemoryException* e)
		{

		}
		catch (CFileException* e)
		{
		}
		catch (CException* e)
		{
		}
		catch (...)
		{
		}
		m_bClientRun = false;

	}
	else
	{
		;
	}

	return true;
}



bool aq::Communicator::server_get_image_run(asio::basic_stream_socket<asio::ip::tcp>* socket, std::string _ip, int _port, std::string& _error_str)
{
	std::string data_buffer;
	data_buffer.resize(50000000);
	VCameraMessageTrans msg;
	std::string error_str;
	msg.width = 115;
	msg.height = 115;
	msg.signalling = 20;
	int nsize = sizeof(msg);	//sizeof(VCameraMessageTrans)=512,here must keep all the windows and Linux programe this data structrue to be same length;
	counter = 0;
	//		send_cmd(aq::CMD::START_RECV_IMG, error_str);
	try
	{
		while (1)
		{
			//std::cout << "execute to Communicator:552" << std::endl;
			//auto buf = asio::buffer(&msg, sizeof(msg));
			//socket_->write_some(buf);
			//std::cout << "execute to Communicator:554" << std::endl;
			std::string s;
			if (!receive_img(s, socket, data_buffer))
			//if (!receive_img(s, socket))
			{
				std::cerr << s << std::endl;
			}
		}
	}
	catch (std::invalid_argument e)
	{
		return false;

	}
	catch (...)
	{
		return false;
	}
	data_buffer.clear();
	return true;
}


int aq::Communicator::udp_send_server(std::string _ip, int _port)
{
	io_service ios;
	ip::udp::socket udp_server(ios);
	ip::udp::endpoint local_addr(ip::address::from_string(_ip), 9001);
	udp_server.open(local_addr.protocol());
	udp_server.bind(local_addr);
	//char buf[BUF_SIZE] = "";
	ip::udp::endpoint send_point;
	int recv_len = 0;
	while (true)
	{
		//recv_len = udp_server.receive_from(buffer(buf, BUF_SIZE), send_point);
		recv_len = udp_server.receive_from(asio::buffer((char*)data_buffer_.data(), sizeof(VCameraMessageTrans)), send_point);
		std::cout << "server recv size = " << recv_len << std::endl;
		//std::cout << "server recv message = " << buf << std::endl;
		//std::cout << "server send back size = " << udp_server.send_to(buffer(buf, recv_len), send_point) << std::endl;
	}
	return 0;
}

int aq::Communicator::udp_send_client(std::string _ip, int _port)
{
	Vsee::VCameraMessageTrans MagneticVolveMsg;
	io_service ios;
	ip::udp::socket sock(ios);
	ip::udp::endpoint end_point(ip::address::from_string(_ip), 9001);
	sock.open(end_point.protocol());
	//char buf[BUF_SIZE] = "";
	string str;
	while (true)
	{
		std::cout << "input str = ";
		std::cin >> str;
		try
		{
			//std::cout << "client send size = " << sock.send_to(buffer(str.c_str(), str.size()), end_point) << std::endl;
			//std::cout << "client recv size = " << sock.receive_from(buffer(buf, BUF_SIZE), end_point) << std::endl;
			//std::cout << "client recv message = " << buf << std::endl;

			//MutexLock lock(mutex_magnetic_valve);
			std::lock_guard<std::mutex> lock(mutex_magnetic_valve);
			memcpy(aq::Communicator::valve_ctrl_msg, &MagneticVolveMsg, sizeof(Vsee::VCameraMessageTrans));
			if (!sock.is_open())
				return false;
			asio::error_code er;
			auto buf = asio::buffer(aq::Communicator::valve_ctrl_msg, sizeof(Vsee::VCameraMessageTrans));
			sock.send_to(buf, end_point);
			valve_ctrl_msg->signalling = 0;
		}
		catch (asio::system_error & e)
		{
			std::cerr << e.what() << std::endl;
		}
	}


	return 0;
}


//author :	LF
//time:		2020-12-11
//parameters:
//			std::string _ip : the local server ip adress
//			int _port : the local server  net port for machine server 
//function: To start a Server state machine,to wait for all the net request from client,and when require income,
//			then start a server thread for the net connect;
bool aq::Communicator::udp_rcv_server(std::string _ip, int _port, std::string &_error_str)
{

	_error_str.clear();
	data_buffer_.clear();
	data_buffer_.resize(1000000);
	std::vector<std::string> ip = split(_ip, '.');
	if (ip.size() != 4)
	{
		_error_str = "Invalid Ip. There is a size error.";
		return false;
	}
	for (auto iter : ip)
	{
		int tmp = -1;
		try
		{
			tmp = std::stoi(iter);
		}
		catch (std::invalid_argument e)
		{
			std::cerr << e.what() << std::endl;
			_error_str = std::string("Invalid Ip. There is an invalid argument: ") + iter + ".";
			return false;
		}
		if ((tmp < 0) || (tmp > 255))
		{
			_error_str = std::string("Invalid Ip. There is an argument: ") + iter + " out of range.";
			return false;
		}
	}
	if ((_port < 0) || (_port > 65535))
	{
		_error_str = std::string("Invalid Port. ") + std::to_string(_port) + " is out of range.";
		return false;
	}

	asio::error_code ec;
	//asio::io_service ios;
	io_service_ = new asio::io_service;
	ep_ = new asio::ip::tcp::endpoint(asio::ip::address::from_string(_ip), _port);
	socket_ = new asio::ip::tcp::socket(*io_service_);
	asio::ip::tcp::acceptor acceptor(*io_service_, *ep_);

	io_service ios;
	asio::ip::udp::socket udp_server(ios);
	asio::ip::udp::endpoint local_addr(ip::address::from_string(_ip), 9001);
	udp_server.open(local_addr.protocol());
	udp_server.bind(local_addr);
	asio::ip::udp::endpoint send_point;
	int recv_len = 0;

	VCameraMessageTrans msg;
	std::string error_str;
	msg.width = 115;
	msg.height = 115;
	msg.signalling = 3;
	counter = 0;
	bool m_bLeihangProtocal = true;

	if (!m_bLeihangProtocal)
	{
		while (true)
		{
			try
			{
				recv_len = 0;
				auto buf = asio::buffer(&msg, sizeof(msg));
				int nLenght = msg.byteSize();
				std::string s;
				_error_str.clear();
				recv_len = udp_server.receive_from(asio::buffer((char*)data_buffer_.data(), sizeof(VCameraMessageTrans)), local_addr);
				//std::int64_t size = streamReadMaxBytesInTime(udp_server, (char*)data_buffer_.data(), sizeof(VCameraMessageTrans), 100, send_point);
				//std::int64_t size = asio::read(*socket_, asio::buffer((char*)data_buffer_.data(), sizeof(VCameraMessageTrans)));
				if (recv_len == sizeof(VCameraMessageTrans))
				{
					VCameraMessageTrans* msg_in = (VCameraMessageTrans*)data_buffer_.data();
					memcpy(&msg, data_buffer_.data(), sizeof(VCameraMessageTrans));
					int width = msg.width;
					int height = msg.height;
					int channels = msg.channels;
					int data_size = width*height*channels;
					int signalling = msg_in->signalling;
					int magnetic_valve_num = msg.magnetic_valve_num;  //to indicate the camera series number;

					memcpy(&MsgPost, &msg, sizeof(VCameraMessageTrans));
					//char* strMsg=new char[64];
					sprintf(m_strMsgSend, "receive_img: signalling=%d,camera number=%d", signalling, magnetic_valve_num);
					//::PostMessage(AfxGetMainWnd()->m_hWnd, ID_SHOWCTRLMESSAGE, signalling, (LPARAM)magnetic_valve_num);
					::PostMessage(m_hMainWnd, ID_SHOWCTRLMESSAGE, signalling, (LPARAM)&MsgPost);
				}
				else
				{
					int n = 5;
					n = 6;
				}

			}
			catch (CMemoryException* e)
			{

			}
			catch (CFileException* e)
			{
			}
			catch (CException* e)
			{
			}
			catch (...)
			{
			}
		}
	}
	else  //Simulate Leihang protocol
	{
		while (m_bMagneticValveShow)
		{
			try
			{
				recv_len = 0;
				auto buf = asio::buffer(&msg, sizeof(msg));
				int nLenght = msg.byteSize();
				std::string s;
				_error_str.clear();
				_time_calib_req_tag_* alib_req_tag;
				_ack_tag_  ack_tag;
				_io_output_transac_tag_* valve_driver_frame;
				uint32_t type = 0;		// = valve_driver_frame->type;
				uint32_t src = 0;		// = valve_driver_frame->src;
				uint32_t port = 0;		// = valve_driver_frame->port;
				uint32_t ref = 0;		// = valve_driver_frame->ref;
				uint32_t delay = 0;		// = valve_driver_frame->delay;
				uint32_t dura = 0;		// = valve_driver_frame->dura;
				recv_len = udp_server.receive_from(asio::buffer((char*)data_buffer_.data(), sizeof(_io_output_transac_tag_)), local_addr);
				//std::int64_t size = streamReadMaxBytesInTime(udp_server, (char*)data_buffer_.data(), sizeof(VCameraMessageTrans), 100, send_point);
				//std::int64_t size = asio::read(*socket_, asio::buffer((char*)data_buffer_.data(), sizeof(VCameraMessageTrans)));

				if (recv_len == sizeof(_time_calib_req_tag_))
				{
					alib_req_tag = (_time_calib_req_tag_*)data_buffer_.data();
					ack_tag.type = 0x80000000;
					ack_tag.ack_src = 0xCC8800DE;
					ack_tag.ack_val = 0xCC8800DE12345678;
					auto buf = asio::buffer(&ack_tag, sizeof(ack_tag));
					udp_server.send_to(buf, local_addr);
				}
				else if (recv_len == sizeof(_ack_tag_))
				{
					//ack_tag = *(_ack_tag_*)data_buffer_.data();
					;
				}
				else if (recv_len == sizeof(_io_output_transac_tag_))
				{

					valve_driver_frame = (_io_output_transac_tag_*)data_buffer_.data();
					//valve_driver_frame.type = 0x80000010;    // 0x80000010 （请求）
					//valve_driver_frame.type = 0xCC880004;   // 0xCC880004（请求类型：为I/O驱动请求）
					//valve_driver_frame.ref = 0x10000;
					//valve_driver_frame.port = i;			// Electric Valve serial number
					//valve_driver_frame.delay;				// delay time
					//valve_driver_frame.dura;				// Hold time

					type = valve_driver_frame->type;
					src = valve_driver_frame->src;
					port = valve_driver_frame->port;
					ref = valve_driver_frame->ref;
					delay = valve_driver_frame->delay / 10000;
					dura = valve_driver_frame->dura / 10000;

					valve_driver[port].type = type;
					valve_driver[port].src = src;
					valve_driver[port].port = port;
					valve_driver[port].ref = ref;
					valve_driver[port].delay = delay;
					valve_driver[port].dura = dura;
				}
				else
				{
					;
				}
			}
			catch (CMemoryException* e)
			{

			}
			catch (CFileException* e)
			{
			}
			catch (CException* e)
			{
			}
			catch (...)
			{
			}
		}
	}
	if (ec)
	{
		_error_str = "The connection could have timed out.";
		return false;
	}
	return true;
}