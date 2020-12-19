// DlgGetTrainImage.cpp : 实现文件
//

#include "stdafx.h"
#include "FiberDetectControl.h"
#include "DlgGetTrainImage.h"
#include "afxdialogex.h"
#include <string>
#include "VCameraMessageTrans.h"
#include "communicator.h"


// CDlgGetTrainImage 对话框
std::string  CDlgGetTrainImage::m_path = "f:/save_image/";
IMPLEMENT_DYNAMIC(CDlgGetTrainImage, CDialogEx)

CDlgGetTrainImage::CDlgGetTrainImage(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDlgGetTrainImage::IDD, pParent)
{

}

CDlgGetTrainImage::~CDlgGetTrainImage()
{
}

void CDlgGetTrainImage::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_MFCBUTTON2, m_MFCButton1);
	DDX_Control(pDX, IDC_MFCBUTTON3, m_MFCButton2);
	DDX_Control(pDX, IDC_MFCBUTTON4, m_MFCButton3);
	DDX_Control(pDX, IDC_COMBO_CAMERA_SERIAL, m_comboCameraList);
}


BEGIN_MESSAGE_MAP(CDlgGetTrainImage, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON_GET_PATH, &CDlgGetTrainImage::OnBnClickedButtonGetPath)
	ON_BN_CLICKED(IDC_BUTTON_STOP_DETECT, &CDlgGetTrainImage::OnBnClickedButtonStopDetect)
	ON_BN_CLICKED(IDC_BUTTON_START_GRAB, &CDlgGetTrainImage::OnBnClickedButtonStartGrab)
	ON_BN_CLICKED(IDC_BUTTON_STOP_GRAB, &CDlgGetTrainImage::OnBnClickedButtonStopGrab)
	ON_BN_CLICKED(IDC_BUTTON_UPLOAD_IMAGE, &CDlgGetTrainImage::OnBnClickedButtonUploadImage)
	ON_BN_CLICKED(IDOK, &CDlgGetTrainImage::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CDlgGetTrainImage::OnBnClickedCancel)
END_MESSAGE_MAP()


// CDlgGetTrainImage 消息处理程序
BOOL CDlgGetTrainImage::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	m_MFCButton1.m_bTransparent = TRUE;
	m_MFCButton1.m_bDontUseWinXPTheme = TRUE;
	m_MFCButton1.m_bDrawFocus = FALSE;
	m_MFCButton1.m_nFlatStyle = CMFCButton::BUTTONSTYLE_NOBORDERS;
	//m_MFCButton1.SetFaceColor(RGB(0, 255, 0));

	m_MFCButton2.m_bTransparent = TRUE;
	m_MFCButton2.m_bDontUseWinXPTheme = TRUE;
	m_MFCButton2.m_bDrawFocus = FALSE;
	m_MFCButton2.m_nFlatStyle = CMFCButton::BUTTONSTYLE_NOBORDERS;
	//m_MFCButton1.SetFaceColor(RGB(0, 255, 0));

	m_MFCButton3.m_bTransparent = TRUE;
	m_MFCButton3.m_bDontUseWinXPTheme = TRUE;
	m_MFCButton3.m_bDrawFocus = FALSE;
	m_MFCButton3.m_nFlatStyle = CMFCButton::BUTTONSTYLE_NOBORDERS;
	//m_MFCButton1.SetFaceColor(RGB(0, 255, 0));

	m_comboCameraList.AddString("相机A");
	m_comboCameraList.AddString("相机B");
	m_comboCameraList.AddString("相机C");
	m_comboCameraList.AddString("相机D");
	m_comboCameraList.SetCurSel(0);
	//GetDlgItem(IDC_EDIT_COMMAND)->EnableWindow(false);
	GetDlgItem(IDC_EDIT_SAVE_PATH)->SetWindowTextA("F:/save_image/");

	return true;
}


void CDlgGetTrainImage::OnBnClickedButtonGetPath()
{
	// TODO:  在此添加控件通知处理程序代码
	TCHAR szPath[_MAX_PATH];
	std::string InputPath;;
	BROWSEINFO bi;
	bi.hwndOwner = GetSafeHwnd();
	bi.pidlRoot = NULL;
	bi.lpszTitle = "Please select the input path";
	bi.pszDisplayName = szPath;
	bi.ulFlags = BIF_RETURNONLYFSDIRS;
	bi.lpfn = NULL;
	bi.lParam = NULL;

	LPITEMIDLIST pItemIDList = SHBrowseForFolder(&bi);

	if (pItemIDList)
	{
		if (SHGetPathFromIDList(pItemIDList, szPath))
		{
			InputPath = szPath;
			//videoPath = inputImgPath;
		}

		//use IMalloc interface for avoiding memory leak
		IMalloc* pMalloc;
		if (SHGetMalloc(&pMalloc) != NOERROR)
		{
			TRACE(_T("Can't get the IMalloc interface\n"));
		}

		pMalloc->Free(pItemIDList);
		if (pMalloc)
			pMalloc->Release();
		UpdateData(FALSE);
	}
	else
		InputPath = "";
	m_path = InputPath;
	GetDlgItem(IDC_EDIT_SAVE_PATH)->SetWindowTextA(InputPath.c_str());
}


void CDlgGetTrainImage::OnBnClickedButtonStopDetect()
{
	// TODO:  在此添加控件通知处理程序代码

	aq::Communicator::box_ctrl_msg.signalling = 2;
	aq::Communicator::box_ctrl_msg.magnetic_valve_num = 0;
	Vsee::VCameraMessageTrans *frame = new Vsee::VCameraMessageTrans;
	frame->signalling = aq::Communicator::box_ctrl_msg.signalling;
	frame->magnetic_valve_num = aq::Communicator::box_ctrl_msg.magnetic_valve_num;
	memcpy(frame, &aq::Communicator::box_ctrl_msg, sizeof(Vsee::VCameraMessageTrans));
	::PostMessage(AfxGetMainWnd()->m_hWnd, ID_SHOWCTRLMESSAGE, aq::Communicator::box_ctrl_msg.signalling, (LPARAM)frame);
	//GetDlgItem(IDC_EDIT_COMMAND)->SetWindowTextA("停止异纤检测.");
	GetDlgItem(IDC_STATIC_COMMAND)->SetWindowTextA("停止异纤检测.");
}


void CDlgGetTrainImage::OnBnClickedButtonStartGrab()
{
	// TODO:  在此添加控件通知处理程序代码
	aq::Communicator::box_ctrl_msg.signalling = 7;					//To start grab image and save image.
	aq::Communicator::box_ctrl_msg.magnetic_valve_num = 0;
	Vsee::VCameraMessageTrans *frame = new Vsee::VCameraMessageTrans;
	frame->signalling = aq::Communicator::box_ctrl_msg.signalling;
	frame->magnetic_valve_num = aq::Communicator::box_ctrl_msg.magnetic_valve_num;
	memcpy(frame, &aq::Communicator::box_ctrl_msg, sizeof(Vsee::VCameraMessageTrans));
	::PostMessage(AfxGetMainWnd()->m_hWnd, ID_SHOWCTRLMESSAGE, aq::Communicator::box_ctrl_msg.signalling, (LPARAM)frame);
	//GetDlgItem(IDC_EDIT_COMMAND)->SetWindowTextA("开始图像采集.");
	GetDlgItem(IDC_STATIC_COMMAND)->SetWindowTextA("开始图像采集.");
}


void CDlgGetTrainImage::OnBnClickedButtonStopGrab()
{
	// TODO:  在此添加控件通知处理程序代码
	m_bSaveImage = false;
	aq::Communicator::box_ctrl_msg.signalling = 8;					//To stop grab image and save image.
	aq::Communicator::box_ctrl_msg.magnetic_valve_num = 0;
	Vsee::VCameraMessageTrans *frame = new Vsee::VCameraMessageTrans;
	frame->signalling = aq::Communicator::box_ctrl_msg.signalling;
	frame->magnetic_valve_num = aq::Communicator::box_ctrl_msg.magnetic_valve_num;
	memcpy(frame, &aq::Communicator::box_ctrl_msg, sizeof(Vsee::VCameraMessageTrans));
	::PostMessage(AfxGetMainWnd()->m_hWnd, ID_SHOWCTRLMESSAGE, aq::Communicator::box_ctrl_msg.signalling, (LPARAM)frame);
	//GetDlgItem(IDC_EDIT_COMMAND)->SetWindowTextA("停止图像采集.");
	GetDlgItem(IDC_STATIC_COMMAND)->SetWindowTextA("停止图像采集.");
}


void CDlgGetTrainImage::OnBnClickedButtonUploadImage()
{
	// TODO:  在此添加控件通知处理程序代码
	m_bSaveImage = !m_bSaveImage;
	aq::Communicator::box_ctrl_msg.signalling = 9;					//To upload  grab image to server.
	if (m_bSaveImage)
	{
		GetDlgItem(IDC_BUTTON_UPLOAD_IMAGE)->SetWindowTextA("暂停...");
		aq::Communicator::box_ctrl_msg.magnetic_valve_num = 1;
		//GetDlgItem(IDC_EDIT_COMMAND)->SetWindowTextA("保存训练集图像.");
		GetDlgItem(IDC_STATIC_COMMAND)->SetWindowTextA("保存训练集图像.");
	}	
	else
	{
		GetDlgItem(IDC_BUTTON_UPLOAD_IMAGE)->SetWindowTextA("存图上传服务器");
		aq::Communicator::box_ctrl_msg.magnetic_valve_num = 0;
		//GetDlgItem(IDC_EDIT_COMMAND)->SetWindowTextA("暂停保存训练集图像.");
		GetDlgItem(IDC_STATIC_COMMAND)->SetWindowTextA("暂停保存训练集图像.");
	}
		
	Vsee::VCameraMessageTrans *frame = new Vsee::VCameraMessageTrans;
	frame->signalling = aq::Communicator::box_ctrl_msg.signalling;
	frame->magnetic_valve_num = aq::Communicator::box_ctrl_msg.magnetic_valve_num;
	int nSelect = m_comboCameraList.GetCurSel() + 1;
	aq::Communicator::box_ctrl_msg.channels = nSelect;
	memcpy(frame, &aq::Communicator::box_ctrl_msg, sizeof(Vsee::VCameraMessageTrans));
	::PostMessage(AfxGetMainWnd()->m_hWnd, ID_SHOWCTRLMESSAGE, aq::Communicator::box_ctrl_msg.signalling, (LPARAM)frame);
}


void CDlgGetTrainImage::OnBnClickedOk()
{
	// TODO:  在此添加控件通知处理程序代码
	CDialogEx::OnOK();
}


void CDlgGetTrainImage::OnBnClickedCancel()
{
	// TODO:  在此添加控件通知处理程序代码
	CDialogEx::OnCancel();
}
