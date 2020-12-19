// MagneticValve.cpp : 实现文件
//

#include "stdafx.h"
#include "FiberDetectControl.h"
#include "MagneticValve.h"
#include "afxdialogex.h"
#include "VCameraMessageTrans.h"
#include "communicator.h"


// CMagneticValve 对话框

IMPLEMENT_DYNAMIC(CMagneticValve, CDialogEx)

CMagneticValve::CMagneticValve(CWnd* pParent /*=NULL*/)
	: CDialogEx(CMagneticValve::IDD, pParent)
{

}

CMagneticValve::~CMagneticValve()
{
}

void CMagneticValve::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CMagneticValve, CDialogEx)
	ON_BN_CLICKED(IDC_MAGNETIC_VALVE1, &CMagneticValve::OnBnClickedMagneticValve1)
	ON_BN_CLICKED(IDC_MAGNETIC_VALVE2, &CMagneticValve::OnBnClickedMagneticValve2)
	ON_BN_CLICKED(IDC_MAGNETIC_VALVE3, &CMagneticValve::OnBnClickedMagneticValve3)
	ON_BN_CLICKED(IDC_MAGNETIC_VALVE4, &CMagneticValve::OnBnClickedMagneticValve4)
	ON_BN_CLICKED(IDC_MAGNETIC_VALVE5, &CMagneticValve::OnBnClickedMagneticValve5)
	ON_BN_CLICKED(IDC_MAGNETIC_VALVE6, &CMagneticValve::OnBnClickedMagneticValve6)
	ON_BN_CLICKED(IDC_MAGNETIC_VALVE7, &CMagneticValve::OnBnClickedMagneticValve7)
	ON_BN_CLICKED(IDC_MAGNETIC_VALVE8, &CMagneticValve::OnBnClickedMagneticValve8)
	ON_BN_CLICKED(IDC_MAGNETIC_VALVE9, &CMagneticValve::OnBnClickedMagneticValve9)
	ON_BN_CLICKED(IDC_MAGNETIC_VALVE10, &CMagneticValve::OnBnClickedMagneticValve10)
	ON_BN_CLICKED(IDC_MAGNETIC_VALVE11, &CMagneticValve::OnBnClickedMagneticValve11)
	ON_BN_CLICKED(IDC_MAGNETIC_VALVE12, &CMagneticValve::OnBnClickedMagneticValve12)
	ON_BN_CLICKED(IDC_MAGNETIC_VALVE13, &CMagneticValve::OnBnClickedMagneticValve13)
	ON_BN_CLICKED(IDC_MAGNETIC_VALVE14, &CMagneticValve::OnBnClickedMagneticValve14)
	ON_BN_CLICKED(IDC_MAGNETIC_VALVE15, &CMagneticValve::OnBnClickedMagneticValve15)
	ON_BN_CLICKED(IDC_MAGNETIC_VALVE16, &CMagneticValve::OnBnClickedMagneticValve16)
	ON_BN_CLICKED(IDC_MAGNETIC_VALVE17, &CMagneticValve::OnBnClickedMagneticValve17)
	ON_BN_CLICKED(IDC_MAGNETIC_VALVE18, &CMagneticValve::OnBnClickedMagneticValve18)
	ON_BN_CLICKED(IDC_MAGNETIC_VALVE19, &CMagneticValve::OnBnClickedMagneticValve19)
	ON_BN_CLICKED(IDC_MAGNETIC_VALVE20, &CMagneticValve::OnBnClickedMagneticValve20)
	ON_BN_CLICKED(IDC_MAGNETIC_VALVE21, &CMagneticValve::OnBnClickedMagneticValve21)
	ON_BN_CLICKED(IDC_MAGNETIC_VALVE22, &CMagneticValve::OnBnClickedMagneticValve22)
	ON_BN_CLICKED(IDC_MAGNETIC_VALVE23, &CMagneticValve::OnBnClickedMagneticValve23)
	ON_BN_CLICKED(IDC_MAGNETIC_VALVE24, &CMagneticValve::OnBnClickedMagneticValve24)
	ON_BN_CLICKED(IDOK, &CMagneticValve::OnBnClickedOk)
END_MESSAGE_MAP()


// CMagneticValve 消息处理程序


void CMagneticValve::OnBnClickedMagneticValve1()
{
	// TODO:  在此添加控件通知处理程序代码	
	for (int i = 0; i < MAGNETIC_VALVE_SUM; i++)   //Clean all the magnetic valve indicator to be zero;
	{
		aq::Communicator::box_ctrl_msg.magnetic_valve[i] = 0;
	}
	aq::Communicator::box_ctrl_msg.signalling = 3;
	aq::Communicator::box_ctrl_msg.magnetic_valve_num = 1;
	aq::Communicator::box_ctrl_msg.magnetic_valve[aq::Communicator::box_ctrl_msg.magnetic_valve_num-1] = 1;		//Set the magnetic valve indicator;
	Vsee::VCameraMessageTrans *frame=new Vsee::VCameraMessageTrans;
	frame->signalling = aq::Communicator::box_ctrl_msg.signalling;
	frame->magnetic_valve_num = aq::Communicator::box_ctrl_msg.magnetic_valve_num;
	memcpy(frame, &aq::Communicator::box_ctrl_msg, sizeof(Vsee::VCameraMessageTrans));
	::PostMessage(AfxGetMainWnd()->m_hWnd, ID_SHOWCTRLMESSAGE, aq::Communicator::box_ctrl_msg.signalling, (LPARAM)frame);
}


void CMagneticValve::OnBnClickedMagneticValve2()
{
	// TODO:  在此添加控件通知处理程序代码
	for (int i = 0; i < MAGNETIC_VALVE_SUM; i++)   //Clean all the magnetic valve indicator to be zero;
	{
		aq::Communicator::box_ctrl_msg.magnetic_valve[i] = 0;
	}
	aq::Communicator::box_ctrl_msg.signalling = 3;
	aq::Communicator::box_ctrl_msg.magnetic_valve_num = 2;
	aq::Communicator::box_ctrl_msg.magnetic_valve[aq::Communicator::box_ctrl_msg.magnetic_valve_num - 1] = 1;		//Set the magnetic valve indicator;
	Vsee::VCameraMessageTrans *frame = new Vsee::VCameraMessageTrans;
	frame->signalling = aq::Communicator::box_ctrl_msg.signalling;
	frame->magnetic_valve_num = aq::Communicator::box_ctrl_msg.magnetic_valve_num;
	memcpy(frame, &aq::Communicator::box_ctrl_msg, sizeof(Vsee::VCameraMessageTrans));
	::PostMessage(AfxGetMainWnd()->m_hWnd, ID_SHOWCTRLMESSAGE, aq::Communicator::box_ctrl_msg.signalling, (LPARAM)frame);

}


void CMagneticValve::OnBnClickedMagneticValve3()
{
	// TODO:  在此添加控件通知处理程序代码
	for (int i = 0; i < MAGNETIC_VALVE_SUM; i++)   //Clean all the magnetic valve indicator to be zero;
	{
		aq::Communicator::box_ctrl_msg.magnetic_valve[i] = 0;
	}
	aq::Communicator::box_ctrl_msg.signalling = 3;
	aq::Communicator::box_ctrl_msg.magnetic_valve_num = 3;
	aq::Communicator::box_ctrl_msg.magnetic_valve[aq::Communicator::box_ctrl_msg.magnetic_valve_num - 1] = 1;		//Set the magnetic valve indicator;
	Vsee::VCameraMessageTrans *frame = new Vsee::VCameraMessageTrans;
	frame->signalling = aq::Communicator::box_ctrl_msg.signalling;
	frame->magnetic_valve_num = aq::Communicator::box_ctrl_msg.magnetic_valve_num;
	memcpy(frame, &aq::Communicator::box_ctrl_msg, sizeof(Vsee::VCameraMessageTrans));
	::PostMessage(AfxGetMainWnd()->m_hWnd, ID_SHOWCTRLMESSAGE, aq::Communicator::box_ctrl_msg.signalling, (LPARAM)frame);

}


void CMagneticValve::OnBnClickedMagneticValve4()
{
	// TODO:  在此添加控件通知处理程序代码
	for (int i = 0; i < MAGNETIC_VALVE_SUM; i++)   //Clean all the magnetic valve indicator to be zero;
	{
		aq::Communicator::box_ctrl_msg.magnetic_valve[i] = 0;
	}
	aq::Communicator::box_ctrl_msg.signalling = 3;
	aq::Communicator::box_ctrl_msg.magnetic_valve_num = 4;
	aq::Communicator::box_ctrl_msg.magnetic_valve[aq::Communicator::box_ctrl_msg.magnetic_valve_num - 1] = 1;		//Set the magnetic valve indicator;
	Vsee::VCameraMessageTrans *frame = new Vsee::VCameraMessageTrans;
	frame->signalling = aq::Communicator::box_ctrl_msg.signalling;
	frame->magnetic_valve_num = aq::Communicator::box_ctrl_msg.magnetic_valve_num;
	memcpy(frame, &aq::Communicator::box_ctrl_msg, sizeof(Vsee::VCameraMessageTrans));
	::PostMessage(AfxGetMainWnd()->m_hWnd, ID_SHOWCTRLMESSAGE, aq::Communicator::box_ctrl_msg.signalling, (LPARAM)frame);

}


void CMagneticValve::OnBnClickedMagneticValve5()
{
	// TODO:  在此添加控件通知处理程序代码
	for (int i = 0; i < MAGNETIC_VALVE_SUM; i++)   //Clean all the magnetic valve indicator to be zero;
	{
		aq::Communicator::box_ctrl_msg.magnetic_valve[i] = 0;
	}
	aq::Communicator::box_ctrl_msg.signalling = 3;
	aq::Communicator::box_ctrl_msg.magnetic_valve_num = 5;
	aq::Communicator::box_ctrl_msg.magnetic_valve[aq::Communicator::box_ctrl_msg.magnetic_valve_num - 1] = 1;		//Set the magnetic valve indicator;
	Vsee::VCameraMessageTrans *frame = new Vsee::VCameraMessageTrans;
	frame->signalling = aq::Communicator::box_ctrl_msg.signalling;
	frame->magnetic_valve_num = aq::Communicator::box_ctrl_msg.magnetic_valve_num;
	memcpy(frame, &aq::Communicator::box_ctrl_msg, sizeof(Vsee::VCameraMessageTrans));
	::PostMessage(AfxGetMainWnd()->m_hWnd, ID_SHOWCTRLMESSAGE, aq::Communicator::box_ctrl_msg.signalling, (LPARAM)frame);

}


void CMagneticValve::OnBnClickedMagneticValve6()
{
	// TODO:  在此添加控件通知处理程序代码
	for (int i = 0; i < MAGNETIC_VALVE_SUM; i++)   //Clean all the magnetic valve indicator to be zero;
	{
		aq::Communicator::box_ctrl_msg.magnetic_valve[i] = 0;
	}
	aq::Communicator::box_ctrl_msg.signalling = 3;
	aq::Communicator::box_ctrl_msg.magnetic_valve_num = 6;
	aq::Communicator::box_ctrl_msg.magnetic_valve[aq::Communicator::box_ctrl_msg.magnetic_valve_num - 1] = 1;		//Set the magnetic valve indicator;
	Vsee::VCameraMessageTrans *frame = new Vsee::VCameraMessageTrans;
	frame->signalling = aq::Communicator::box_ctrl_msg.signalling;
	frame->magnetic_valve_num = aq::Communicator::box_ctrl_msg.magnetic_valve_num;
	memcpy(frame, &aq::Communicator::box_ctrl_msg, sizeof(Vsee::VCameraMessageTrans));
	::PostMessage(AfxGetMainWnd()->m_hWnd, ID_SHOWCTRLMESSAGE, aq::Communicator::box_ctrl_msg.signalling, (LPARAM)frame);

}


void CMagneticValve::OnBnClickedMagneticValve7()
{
	// TODO:  在此添加控件通知处理程序代码
	for (int i = 0; i < MAGNETIC_VALVE_SUM; i++)   //Clean all the magnetic valve indicator to be zero;
	{
		aq::Communicator::box_ctrl_msg.magnetic_valve[i] = 0;
	}
	aq::Communicator::box_ctrl_msg.signalling = 3;
	aq::Communicator::box_ctrl_msg.magnetic_valve_num = 7;
	aq::Communicator::box_ctrl_msg.magnetic_valve[aq::Communicator::box_ctrl_msg.magnetic_valve_num - 1] = 1;		//Set the magnetic valve indicator;
	Vsee::VCameraMessageTrans *frame = new Vsee::VCameraMessageTrans;
	frame->signalling = aq::Communicator::box_ctrl_msg.signalling;
	frame->magnetic_valve_num = aq::Communicator::box_ctrl_msg.magnetic_valve_num;
	memcpy(frame, &aq::Communicator::box_ctrl_msg, sizeof(Vsee::VCameraMessageTrans));
	::PostMessage(AfxGetMainWnd()->m_hWnd, ID_SHOWCTRLMESSAGE, aq::Communicator::box_ctrl_msg.signalling, (LPARAM)frame);

}


void CMagneticValve::OnBnClickedMagneticValve8()
{
	// TODO:  在此添加控件通知处理程序代码
	for (int i = 0; i < MAGNETIC_VALVE_SUM; i++)   //Clean all the magnetic valve indicator to be zero;
	{
		aq::Communicator::box_ctrl_msg.magnetic_valve[i] = 0;
	}
	aq::Communicator::box_ctrl_msg.signalling = 3;
	aq::Communicator::box_ctrl_msg.magnetic_valve_num = 8;
	aq::Communicator::box_ctrl_msg.magnetic_valve[aq::Communicator::box_ctrl_msg.magnetic_valve_num - 1] = 1;		//Set the magnetic valve indicator;
	Vsee::VCameraMessageTrans *frame = new Vsee::VCameraMessageTrans;
	frame->signalling = aq::Communicator::box_ctrl_msg.signalling;
	frame->magnetic_valve_num = aq::Communicator::box_ctrl_msg.magnetic_valve_num;
	memcpy(frame, &aq::Communicator::box_ctrl_msg, sizeof(Vsee::VCameraMessageTrans));
	::PostMessage(AfxGetMainWnd()->m_hWnd, ID_SHOWCTRLMESSAGE, aq::Communicator::box_ctrl_msg.signalling, (LPARAM)frame);

}


void CMagneticValve::OnBnClickedMagneticValve9()
{
	// TODO:  在此添加控件通知处理程序代码
	for (int i = 0; i < MAGNETIC_VALVE_SUM; i++)   //Clean all the magnetic valve indicator to be zero;
	{
		aq::Communicator::box_ctrl_msg.magnetic_valve[i] = 0;
	}
	aq::Communicator::box_ctrl_msg.signalling = 3;
	aq::Communicator::box_ctrl_msg.magnetic_valve_num = 9;
	aq::Communicator::box_ctrl_msg.magnetic_valve[aq::Communicator::box_ctrl_msg.magnetic_valve_num - 1] = 1;		//Set the magnetic valve indicator;
	Vsee::VCameraMessageTrans *frame = new Vsee::VCameraMessageTrans;
	frame->signalling = aq::Communicator::box_ctrl_msg.signalling;
	frame->magnetic_valve_num = aq::Communicator::box_ctrl_msg.magnetic_valve_num;
	memcpy(frame, &aq::Communicator::box_ctrl_msg, sizeof(Vsee::VCameraMessageTrans));
	::PostMessage(AfxGetMainWnd()->m_hWnd, ID_SHOWCTRLMESSAGE, aq::Communicator::box_ctrl_msg.signalling, (LPARAM)frame);

}


void CMagneticValve::OnBnClickedMagneticValve10()
{
	// TODO:  在此添加控件通知处理程序代码
	for (int i = 0; i < MAGNETIC_VALVE_SUM; i++)   //Clean all the magnetic valve indicator to be zero;
	{
		aq::Communicator::box_ctrl_msg.magnetic_valve[i] = 0;
	}
	aq::Communicator::box_ctrl_msg.signalling = 3;
	aq::Communicator::box_ctrl_msg.magnetic_valve_num = 10;
	aq::Communicator::box_ctrl_msg.magnetic_valve[aq::Communicator::box_ctrl_msg.magnetic_valve_num - 1] = 1;		//Set the magnetic valve indicator;
	Vsee::VCameraMessageTrans *frame = new Vsee::VCameraMessageTrans;
	frame->signalling = aq::Communicator::box_ctrl_msg.signalling;
	frame->magnetic_valve_num = aq::Communicator::box_ctrl_msg.magnetic_valve_num;
	memcpy(frame, &aq::Communicator::box_ctrl_msg, sizeof(Vsee::VCameraMessageTrans));
	::PostMessage(AfxGetMainWnd()->m_hWnd, ID_SHOWCTRLMESSAGE, aq::Communicator::box_ctrl_msg.signalling, (LPARAM)frame);

}


void CMagneticValve::OnBnClickedMagneticValve11()
{
	// TODO:  在此添加控件通知处理程序代码
	for (int i = 0; i < MAGNETIC_VALVE_SUM; i++)   //Clean all the magnetic valve indicator to be zero;
	{
		aq::Communicator::box_ctrl_msg.magnetic_valve[i] = 0;
	}
	aq::Communicator::box_ctrl_msg.signalling = 3;
	aq::Communicator::box_ctrl_msg.magnetic_valve_num = 11;
	aq::Communicator::box_ctrl_msg.magnetic_valve[aq::Communicator::box_ctrl_msg.magnetic_valve_num - 1] = 1;		//Set the magnetic valve indicator;
	Vsee::VCameraMessageTrans *frame = new Vsee::VCameraMessageTrans;
	frame->signalling = aq::Communicator::box_ctrl_msg.signalling;
	frame->magnetic_valve_num = aq::Communicator::box_ctrl_msg.magnetic_valve_num;
	memcpy(frame, &aq::Communicator::box_ctrl_msg, sizeof(Vsee::VCameraMessageTrans));
	::PostMessage(AfxGetMainWnd()->m_hWnd, ID_SHOWCTRLMESSAGE, aq::Communicator::box_ctrl_msg.signalling, (LPARAM)frame);

}


void CMagneticValve::OnBnClickedMagneticValve12()
{
	// TODO:  在此添加控件通知处理程序代码
	for (int i = 0; i < MAGNETIC_VALVE_SUM; i++)   //Clean all the magnetic valve indicator to be zero;
	{
		aq::Communicator::box_ctrl_msg.magnetic_valve[i] = 0;
	}
	aq::Communicator::box_ctrl_msg.signalling = 3;
	aq::Communicator::box_ctrl_msg.magnetic_valve_num = 12;
	aq::Communicator::box_ctrl_msg.magnetic_valve[aq::Communicator::box_ctrl_msg.magnetic_valve_num - 1] = 1;		//Set the magnetic valve indicator;
	Vsee::VCameraMessageTrans *frame = new Vsee::VCameraMessageTrans;
	frame->signalling = aq::Communicator::box_ctrl_msg.signalling;
	frame->magnetic_valve_num = aq::Communicator::box_ctrl_msg.magnetic_valve_num;
	memcpy(frame, &aq::Communicator::box_ctrl_msg, sizeof(Vsee::VCameraMessageTrans));
	::PostMessage(AfxGetMainWnd()->m_hWnd, ID_SHOWCTRLMESSAGE, aq::Communicator::box_ctrl_msg.signalling, (LPARAM)frame);

}


void CMagneticValve::OnBnClickedMagneticValve13()
{
	// TODO:  在此添加控件通知处理程序代码
	for (int i = 0; i < MAGNETIC_VALVE_SUM; i++)   //Clean all the magnetic valve indicator to be zero;
	{
		aq::Communicator::box_ctrl_msg.magnetic_valve[i] = 0;
	}
	aq::Communicator::box_ctrl_msg.signalling = 3;
	aq::Communicator::box_ctrl_msg.magnetic_valve_num = 13;
	aq::Communicator::box_ctrl_msg.magnetic_valve[aq::Communicator::box_ctrl_msg.magnetic_valve_num - 1] = 1;		//Set the magnetic valve indicator;
	Vsee::VCameraMessageTrans *frame = new Vsee::VCameraMessageTrans;
	frame->signalling = aq::Communicator::box_ctrl_msg.signalling;
	frame->magnetic_valve_num = aq::Communicator::box_ctrl_msg.magnetic_valve_num;
	memcpy(frame, &aq::Communicator::box_ctrl_msg, sizeof(Vsee::VCameraMessageTrans));
	::PostMessage(AfxGetMainWnd()->m_hWnd, ID_SHOWCTRLMESSAGE, aq::Communicator::box_ctrl_msg.signalling, (LPARAM)frame);

}


void CMagneticValve::OnBnClickedMagneticValve14()
{
	// TODO:  在此添加控件通知处理程序代码
	for (int i = 0; i < MAGNETIC_VALVE_SUM; i++)   //Clean all the magnetic valve indicator to be zero;
	{
		aq::Communicator::box_ctrl_msg.magnetic_valve[i] = 0;
	}
	aq::Communicator::box_ctrl_msg.signalling = 3;
	aq::Communicator::box_ctrl_msg.magnetic_valve_num = 14;
	aq::Communicator::box_ctrl_msg.magnetic_valve[aq::Communicator::box_ctrl_msg.magnetic_valve_num - 1] = 1;		//Set the magnetic valve indicator;
	Vsee::VCameraMessageTrans *frame = new Vsee::VCameraMessageTrans;
	frame->signalling = aq::Communicator::box_ctrl_msg.signalling;
	frame->magnetic_valve_num = aq::Communicator::box_ctrl_msg.magnetic_valve_num;
	memcpy(frame, &aq::Communicator::box_ctrl_msg, sizeof(Vsee::VCameraMessageTrans));
	::PostMessage(AfxGetMainWnd()->m_hWnd, ID_SHOWCTRLMESSAGE, aq::Communicator::box_ctrl_msg.signalling, (LPARAM)frame);


}


void CMagneticValve::OnBnClickedMagneticValve15()
{
	// TODO:  在此添加控件通知处理程序代码
	for (int i = 0; i < MAGNETIC_VALVE_SUM; i++)   //Clean all the magnetic valve indicator to be zero;
	{
		aq::Communicator::box_ctrl_msg.magnetic_valve[i] = 0;
	}
	aq::Communicator::box_ctrl_msg.signalling = 3;
	aq::Communicator::box_ctrl_msg.magnetic_valve_num = 15;
	aq::Communicator::box_ctrl_msg.magnetic_valve[aq::Communicator::box_ctrl_msg.magnetic_valve_num - 1] = 1;		//Set the magnetic valve indicator;
	Vsee::VCameraMessageTrans *frame = new Vsee::VCameraMessageTrans;
	frame->signalling = aq::Communicator::box_ctrl_msg.signalling;
	frame->magnetic_valve_num = aq::Communicator::box_ctrl_msg.magnetic_valve_num;
	memcpy(frame, &aq::Communicator::box_ctrl_msg, sizeof(Vsee::VCameraMessageTrans));
	::PostMessage(AfxGetMainWnd()->m_hWnd, ID_SHOWCTRLMESSAGE, aq::Communicator::box_ctrl_msg.signalling, (LPARAM)frame);

}


void CMagneticValve::OnBnClickedMagneticValve16()
{
	// TODO:  在此添加控件通知处理程序代码
	for (int i = 0; i < MAGNETIC_VALVE_SUM; i++)   //Clean all the magnetic valve indicator to be zero;
	{
		aq::Communicator::box_ctrl_msg.magnetic_valve[i] = 0;
	}
	aq::Communicator::box_ctrl_msg.signalling = 3;
	aq::Communicator::box_ctrl_msg.magnetic_valve_num = 16;
	aq::Communicator::box_ctrl_msg.magnetic_valve[aq::Communicator::box_ctrl_msg.magnetic_valve_num - 1] = 1;		//Set the magnetic valve indicator;
	Vsee::VCameraMessageTrans *frame = new Vsee::VCameraMessageTrans;
	frame->signalling = aq::Communicator::box_ctrl_msg.signalling;
	frame->magnetic_valve_num = aq::Communicator::box_ctrl_msg.magnetic_valve_num;
	memcpy(frame, &aq::Communicator::box_ctrl_msg, sizeof(Vsee::VCameraMessageTrans));
	::PostMessage(AfxGetMainWnd()->m_hWnd, ID_SHOWCTRLMESSAGE, aq::Communicator::box_ctrl_msg.signalling, (LPARAM)frame);

}


void CMagneticValve::OnBnClickedMagneticValve17()
{
	// TODO:  在此添加控件通知处理程序代码
	for (int i = 0; i < MAGNETIC_VALVE_SUM; i++)   //Clean all the magnetic valve indicator to be zero;
	{
		aq::Communicator::box_ctrl_msg.magnetic_valve[i] = 0;
	}
	aq::Communicator::box_ctrl_msg.signalling = 3;
	aq::Communicator::box_ctrl_msg.magnetic_valve_num = 17;
	aq::Communicator::box_ctrl_msg.magnetic_valve[aq::Communicator::box_ctrl_msg.magnetic_valve_num - 1] = 1;		//Set the magnetic valve indicator;
	Vsee::VCameraMessageTrans *frame = new Vsee::VCameraMessageTrans;
	frame->signalling = aq::Communicator::box_ctrl_msg.signalling;
	frame->magnetic_valve_num = aq::Communicator::box_ctrl_msg.magnetic_valve_num;
	memcpy(frame, &aq::Communicator::box_ctrl_msg, sizeof(Vsee::VCameraMessageTrans));
	::PostMessage(AfxGetMainWnd()->m_hWnd, ID_SHOWCTRLMESSAGE, aq::Communicator::box_ctrl_msg.signalling, (LPARAM)frame);

}


void CMagneticValve::OnBnClickedMagneticValve18()
{
	// TODO:  在此添加控件通知处理程序代码
	for (int i = 0; i < MAGNETIC_VALVE_SUM; i++)   //Clean all the magnetic valve indicator to be zero;
	{
		aq::Communicator::box_ctrl_msg.magnetic_valve[i] = 0;
	}
	aq::Communicator::box_ctrl_msg.signalling = 3;
	aq::Communicator::box_ctrl_msg.magnetic_valve_num = 18;
	aq::Communicator::box_ctrl_msg.magnetic_valve[aq::Communicator::box_ctrl_msg.magnetic_valve_num - 1] = 1;		//Set the magnetic valve indicator;
	Vsee::VCameraMessageTrans *frame = new Vsee::VCameraMessageTrans;
	frame->signalling = aq::Communicator::box_ctrl_msg.signalling;
	frame->magnetic_valve_num = aq::Communicator::box_ctrl_msg.magnetic_valve_num;
	memcpy(frame, &aq::Communicator::box_ctrl_msg, sizeof(Vsee::VCameraMessageTrans));
	::PostMessage(AfxGetMainWnd()->m_hWnd, ID_SHOWCTRLMESSAGE, aq::Communicator::box_ctrl_msg.signalling, (LPARAM)frame);

}


void CMagneticValve::OnBnClickedMagneticValve19()
{
	// TODO:  在此添加控件通知处理程序代码
	for (int i = 0; i < MAGNETIC_VALVE_SUM; i++)   //Clean all the magnetic valve indicator to be zero;
	{
		aq::Communicator::box_ctrl_msg.magnetic_valve[i] = 0;
	}
	aq::Communicator::box_ctrl_msg.signalling = 3;
	aq::Communicator::box_ctrl_msg.magnetic_valve_num = 19;
	aq::Communicator::box_ctrl_msg.magnetic_valve[aq::Communicator::box_ctrl_msg.magnetic_valve_num - 1] = 1;		//Set the magnetic valve indicator;
	Vsee::VCameraMessageTrans *frame = new Vsee::VCameraMessageTrans;
	frame->signalling = aq::Communicator::box_ctrl_msg.signalling;
	frame->magnetic_valve_num = aq::Communicator::box_ctrl_msg.magnetic_valve_num;
	memcpy(frame, &aq::Communicator::box_ctrl_msg, sizeof(Vsee::VCameraMessageTrans));
	::PostMessage(AfxGetMainWnd()->m_hWnd, ID_SHOWCTRLMESSAGE, aq::Communicator::box_ctrl_msg.signalling, (LPARAM)frame);

}


void CMagneticValve::OnBnClickedMagneticValve20()
{
	// TODO:  在此添加控件通知处理程序代码
	for (int i = 0; i < MAGNETIC_VALVE_SUM; i++)   //Clean all the magnetic valve indicator to be zero;
	{
		aq::Communicator::box_ctrl_msg.magnetic_valve[i] = 0;
	}
	aq::Communicator::box_ctrl_msg.signalling = 3;
	aq::Communicator::box_ctrl_msg.magnetic_valve_num = 20;
	aq::Communicator::box_ctrl_msg.magnetic_valve[aq::Communicator::box_ctrl_msg.magnetic_valve_num - 1] = 1;		//Set the magnetic valve indicator;
	Vsee::VCameraMessageTrans *frame = new Vsee::VCameraMessageTrans;
	frame->signalling = aq::Communicator::box_ctrl_msg.signalling;
	frame->magnetic_valve_num = aq::Communicator::box_ctrl_msg.magnetic_valve_num;
	memcpy(frame, &aq::Communicator::box_ctrl_msg, sizeof(Vsee::VCameraMessageTrans));
	::PostMessage(AfxGetMainWnd()->m_hWnd, ID_SHOWCTRLMESSAGE, aq::Communicator::box_ctrl_msg.signalling, (LPARAM)frame);

}


void CMagneticValve::OnBnClickedMagneticValve21()
{
	// TODO:  在此添加控件通知处理程序代码
	for (int i = 0; i < MAGNETIC_VALVE_SUM; i++)   //Clean all the magnetic valve indicator to be zero;
	{
		aq::Communicator::box_ctrl_msg.magnetic_valve[i] = 0;
	}
	aq::Communicator::box_ctrl_msg.signalling = 3;
	aq::Communicator::box_ctrl_msg.magnetic_valve_num = 21;
	aq::Communicator::box_ctrl_msg.magnetic_valve[aq::Communicator::box_ctrl_msg.magnetic_valve_num - 1] = 1;		//Set the magnetic valve indicator;
	Vsee::VCameraMessageTrans *frame = new Vsee::VCameraMessageTrans;
	frame->signalling = aq::Communicator::box_ctrl_msg.signalling;
	frame->magnetic_valve_num = aq::Communicator::box_ctrl_msg.magnetic_valve_num;
	memcpy(frame, &aq::Communicator::box_ctrl_msg, sizeof(Vsee::VCameraMessageTrans));
	::PostMessage(AfxGetMainWnd()->m_hWnd, ID_SHOWCTRLMESSAGE, aq::Communicator::box_ctrl_msg.signalling, (LPARAM)frame);

}


void CMagneticValve::OnBnClickedMagneticValve22()
{
	// TODO:  在此添加控件通知处理程序代码
	for (int i = 0; i < MAGNETIC_VALVE_SUM; i++)   //Clean all the magnetic valve indicator to be zero;
	{
		aq::Communicator::box_ctrl_msg.magnetic_valve[i] = 0;
	}
	aq::Communicator::box_ctrl_msg.signalling = 3;
	aq::Communicator::box_ctrl_msg.magnetic_valve_num = 22;
	aq::Communicator::box_ctrl_msg.magnetic_valve[aq::Communicator::box_ctrl_msg.magnetic_valve_num - 1] = 1;		//Set the magnetic valve indicator;
	Vsee::VCameraMessageTrans *frame = new Vsee::VCameraMessageTrans;
	frame->signalling = aq::Communicator::box_ctrl_msg.signalling;
	frame->magnetic_valve_num = aq::Communicator::box_ctrl_msg.magnetic_valve_num;
	memcpy(frame, &aq::Communicator::box_ctrl_msg, sizeof(Vsee::VCameraMessageTrans));
	::PostMessage(AfxGetMainWnd()->m_hWnd, ID_SHOWCTRLMESSAGE, aq::Communicator::box_ctrl_msg.signalling, (LPARAM)frame);

}


void CMagneticValve::OnBnClickedMagneticValve23()
{
	// TODO:  在此添加控件通知处理程序代码
	for (int i = 0; i < MAGNETIC_VALVE_SUM; i++)   //Clean all the magnetic valve indicator to be zero;
	{
		aq::Communicator::box_ctrl_msg.magnetic_valve[i] = 0;
	}
	aq::Communicator::box_ctrl_msg.signalling = 3;
	aq::Communicator::box_ctrl_msg.magnetic_valve_num = 23;
	aq::Communicator::box_ctrl_msg.magnetic_valve[aq::Communicator::box_ctrl_msg.magnetic_valve_num - 1] = 1;		//Set the magnetic valve indicator;
	Vsee::VCameraMessageTrans *frame = new Vsee::VCameraMessageTrans;
	frame->signalling = aq::Communicator::box_ctrl_msg.signalling;
	frame->magnetic_valve_num = aq::Communicator::box_ctrl_msg.magnetic_valve_num;
	memcpy(frame, &aq::Communicator::box_ctrl_msg, sizeof(Vsee::VCameraMessageTrans));
	::PostMessage(AfxGetMainWnd()->m_hWnd, ID_SHOWCTRLMESSAGE, aq::Communicator::box_ctrl_msg.signalling, (LPARAM)frame);

}


void CMagneticValve::OnBnClickedMagneticValve24()
{
	// TODO:  在此添加控件通知处理程序代码
	for (int i = 0; i < MAGNETIC_VALVE_SUM; i++)   //Clean all the magnetic valve indicator to be zero;
	{
		aq::Communicator::box_ctrl_msg.magnetic_valve[i] = 0;
	}
	aq::Communicator::box_ctrl_msg.signalling = 3;
	aq::Communicator::box_ctrl_msg.magnetic_valve_num = 24;
	aq::Communicator::box_ctrl_msg.magnetic_valve[aq::Communicator::box_ctrl_msg.magnetic_valve_num - 1] = 1;		//Set the magnetic valve indicator;
	Vsee::VCameraMessageTrans *frame = new Vsee::VCameraMessageTrans;
	frame->signalling = aq::Communicator::box_ctrl_msg.signalling;
	frame->magnetic_valve_num = aq::Communicator::box_ctrl_msg.magnetic_valve_num;
	memcpy(frame, &aq::Communicator::box_ctrl_msg, sizeof(Vsee::VCameraMessageTrans));
	::PostMessage(AfxGetMainWnd()->m_hWnd, ID_SHOWCTRLMESSAGE, aq::Communicator::box_ctrl_msg.signalling, (LPARAM)frame);

}


void CMagneticValve::OnBnClickedOk()
{
	// TODO:  在此添加控件通知处理程序代码
	//aq::Communicator::box_ctrl_msg.signalling = 3;
	for (int i = 0; i <MAGNETIC_VALVE_SUM; i++)
	{
		Vsee::VCameraMessageTrans *frame = new Vsee::VCameraMessageTrans;
		frame->signalling = 3;
		for (int ii = 0; ii < MAGNETIC_VALVE_SUM; ii++)   //Clean all the magnetic valve indicator to be zero;
		{
			aq::Communicator::box_ctrl_msg.magnetic_valve[ii] = 0;
			//frame->magnetic_valve[ii] = 0;
		}
		aq::Communicator::box_ctrl_msg.magnetic_valve_num = i+1;
		aq::Communicator::box_ctrl_msg.magnetic_valve[aq::Communicator::box_ctrl_msg.magnetic_valve_num - 1] = 1;		//Set the magnetic valve indicator;
		aq::Communicator::box_ctrl_msg.signalling = 3;
		
		//aq::Communicator::box_ctrl_msg.magnetic_valve[i] = 1;
		//frame->magnetic_valve_num = i + 1;
		//frame->magnetic_valve[frame->magnetic_valve_num-1] = 1;
		//frame->signalling = 3;
		//frame->parameters_setting_trans.box_num=
		//frame->magnetic_valve_num = aq::Communicator::box_ctrl_msg.magnetic_valve_num;
		memcpy(frame, &aq::Communicator::box_ctrl_msg, sizeof(Vsee::VCameraMessageTrans));
		::PostMessage(AfxGetMainWnd()->m_hWnd, ID_SHOWCTRLMESSAGE, frame->signalling, (LPARAM)frame);
		//Sleep(200);			//Here does not need this sleep function,the Windows message will send to main wnd will form a message queue;
	}
	for (int i = 0; i < MAGNETIC_VALVE_SUM-1; i++)
	{
		Vsee::VCameraMessageTrans *frame = new Vsee::VCameraMessageTrans;
		frame->signalling = 3;
		for (int ii = 0; ii < MAGNETIC_VALVE_SUM; ii++)   //Clean all the magnetic valve indicator to be zero;
		{
			aq::Communicator::box_ctrl_msg.magnetic_valve[ii] = 0;
			//frame->magnetic_valve[ii] = 0;
		}
		aq::Communicator::box_ctrl_msg.magnetic_valve_num = i + 1;
		aq::Communicator::box_ctrl_msg.magnetic_valve[aq::Communicator::box_ctrl_msg.magnetic_valve_num - 1] = 1;		//Set the magnetic valve indicator;
		aq::Communicator::box_ctrl_msg.magnetic_valve[aq::Communicator::box_ctrl_msg.magnetic_valve_num ] = 1;		//Set the magnetic valve indicator;
		aq::Communicator::box_ctrl_msg.signalling = 3;

		//aq::Communicator::box_ctrl_msg.magnetic_valve[i] = 1;
		//frame->magnetic_valve_num = i + 1;
		//frame->magnetic_valve[i] = 1;
		//frame->magnetic_valve[i+1] = 1;
		//frame->signalling = 3;
		//frame->magnetic_valve_num = aq::Communicator::box_ctrl_msg.magnetic_valve_num;
		memcpy(frame, &aq::Communicator::box_ctrl_msg, sizeof(Vsee::VCameraMessageTrans));
		::PostMessage(AfxGetMainWnd()->m_hWnd, ID_SHOWCTRLMESSAGE, frame->signalling, (LPARAM)frame);
		//Sleep(200);			//Here does not need this sleep function,the Windows message will send to main wnd will form a message queue;
	}
	
	//CDialogEx::OnOK();
}
