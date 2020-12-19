// DlgGetTrainImage.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "FiberDetectControl.h"
#include "DlgGetTrainImage.h"
#include "afxdialogex.h"
#include <string>
#include "VCameraMessageTrans.h"
#include "communicator.h"


// CDlgGetTrainImage �Ի���
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


// CDlgGetTrainImage ��Ϣ�������
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

	m_comboCameraList.AddString("���A");
	m_comboCameraList.AddString("���B");
	m_comboCameraList.AddString("���C");
	m_comboCameraList.AddString("���D");
	m_comboCameraList.SetCurSel(0);
	//GetDlgItem(IDC_EDIT_COMMAND)->EnableWindow(false);
	GetDlgItem(IDC_EDIT_SAVE_PATH)->SetWindowTextA("F:/save_image/");

	return true;
}


void CDlgGetTrainImage::OnBnClickedButtonGetPath()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
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
	// TODO:  �ڴ���ӿؼ�֪ͨ����������

	aq::Communicator::box_ctrl_msg.signalling = 2;
	aq::Communicator::box_ctrl_msg.magnetic_valve_num = 0;
	Vsee::VCameraMessageTrans *frame = new Vsee::VCameraMessageTrans;
	frame->signalling = aq::Communicator::box_ctrl_msg.signalling;
	frame->magnetic_valve_num = aq::Communicator::box_ctrl_msg.magnetic_valve_num;
	memcpy(frame, &aq::Communicator::box_ctrl_msg, sizeof(Vsee::VCameraMessageTrans));
	::PostMessage(AfxGetMainWnd()->m_hWnd, ID_SHOWCTRLMESSAGE, aq::Communicator::box_ctrl_msg.signalling, (LPARAM)frame);
	//GetDlgItem(IDC_EDIT_COMMAND)->SetWindowTextA("ֹͣ���˼��.");
	GetDlgItem(IDC_STATIC_COMMAND)->SetWindowTextA("ֹͣ���˼��.");
}


void CDlgGetTrainImage::OnBnClickedButtonStartGrab()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	aq::Communicator::box_ctrl_msg.signalling = 7;					//To start grab image and save image.
	aq::Communicator::box_ctrl_msg.magnetic_valve_num = 0;
	Vsee::VCameraMessageTrans *frame = new Vsee::VCameraMessageTrans;
	frame->signalling = aq::Communicator::box_ctrl_msg.signalling;
	frame->magnetic_valve_num = aq::Communicator::box_ctrl_msg.magnetic_valve_num;
	memcpy(frame, &aq::Communicator::box_ctrl_msg, sizeof(Vsee::VCameraMessageTrans));
	::PostMessage(AfxGetMainWnd()->m_hWnd, ID_SHOWCTRLMESSAGE, aq::Communicator::box_ctrl_msg.signalling, (LPARAM)frame);
	//GetDlgItem(IDC_EDIT_COMMAND)->SetWindowTextA("��ʼͼ��ɼ�.");
	GetDlgItem(IDC_STATIC_COMMAND)->SetWindowTextA("��ʼͼ��ɼ�.");
}


void CDlgGetTrainImage::OnBnClickedButtonStopGrab()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	m_bSaveImage = false;
	aq::Communicator::box_ctrl_msg.signalling = 8;					//To stop grab image and save image.
	aq::Communicator::box_ctrl_msg.magnetic_valve_num = 0;
	Vsee::VCameraMessageTrans *frame = new Vsee::VCameraMessageTrans;
	frame->signalling = aq::Communicator::box_ctrl_msg.signalling;
	frame->magnetic_valve_num = aq::Communicator::box_ctrl_msg.magnetic_valve_num;
	memcpy(frame, &aq::Communicator::box_ctrl_msg, sizeof(Vsee::VCameraMessageTrans));
	::PostMessage(AfxGetMainWnd()->m_hWnd, ID_SHOWCTRLMESSAGE, aq::Communicator::box_ctrl_msg.signalling, (LPARAM)frame);
	//GetDlgItem(IDC_EDIT_COMMAND)->SetWindowTextA("ֹͣͼ��ɼ�.");
	GetDlgItem(IDC_STATIC_COMMAND)->SetWindowTextA("ֹͣͼ��ɼ�.");
}


void CDlgGetTrainImage::OnBnClickedButtonUploadImage()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	m_bSaveImage = !m_bSaveImage;
	aq::Communicator::box_ctrl_msg.signalling = 9;					//To upload  grab image to server.
	if (m_bSaveImage)
	{
		GetDlgItem(IDC_BUTTON_UPLOAD_IMAGE)->SetWindowTextA("��ͣ...");
		aq::Communicator::box_ctrl_msg.magnetic_valve_num = 1;
		//GetDlgItem(IDC_EDIT_COMMAND)->SetWindowTextA("����ѵ����ͼ��.");
		GetDlgItem(IDC_STATIC_COMMAND)->SetWindowTextA("����ѵ����ͼ��.");
	}	
	else
	{
		GetDlgItem(IDC_BUTTON_UPLOAD_IMAGE)->SetWindowTextA("��ͼ�ϴ�������");
		aq::Communicator::box_ctrl_msg.magnetic_valve_num = 0;
		//GetDlgItem(IDC_EDIT_COMMAND)->SetWindowTextA("��ͣ����ѵ����ͼ��.");
		GetDlgItem(IDC_STATIC_COMMAND)->SetWindowTextA("��ͣ����ѵ����ͼ��.");
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
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	CDialogEx::OnOK();
}


void CDlgGetTrainImage::OnBnClickedCancel()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	CDialogEx::OnCancel();
}
