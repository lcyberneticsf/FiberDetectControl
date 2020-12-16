// SaveDetectResult.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "FiberDetectControl.h"
#include "SaveDetectResult.h"
#include "afxdialogex.h"
#include "VCameraMessageTrans.h"
#include "communicator.h"



// CSaveDetectResult �Ի���

IMPLEMENT_DYNAMIC(CSaveDetectResult, CDialogEx)

CSaveDetectResult::CSaveDetectResult(CWnd* pParent /*=NULL*/)
	: CDialogEx(CSaveDetectResult::IDD, pParent)
{

}

CSaveDetectResult::~CSaveDetectResult()
{
}

void CSaveDetectResult::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_CAMERA_SERIAL, m_comboCameraList);
}


BEGIN_MESSAGE_MAP(CSaveDetectResult, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON_START_SAVE, &CSaveDetectResult::OnBnClickedButtonStartSave)
	ON_BN_CLICKED(IDC_BUTTON_GET_PATH, &CSaveDetectResult::OnBnClickedButtonGetPath)
	ON_BN_CLICKED(IDOK, &CSaveDetectResult::OnBnClickedOk)
END_MESSAGE_MAP()


// CSaveDetectResult ��Ϣ�������
BOOL CSaveDetectResult::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	m_comboCameraList.AddString("���A:������Label");
	m_comboCameraList.AddString("���B:������Label");
	m_comboCameraList.AddString("���C:������Label");
	m_comboCameraList.AddString("���D:������Label");
	m_comboCameraList.SetCurSel(0);
	//GetDlgItem(IDC_EDIT_COMMAND)->EnableWindow(false);
	m_strSavePath = "F:/save_image/";
	GetDlgItem(IDC_EDIT_SAVE_PATH)->SetWindowTextA(m_strSavePath.c_str());
	GetDlgItem(IDOK)->SetWindowTextA("�˳��洢Label����");
	return true;
}


void CSaveDetectResult::OnBnClickedButtonStartSave()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	m_bSaveImage = !m_bSaveImage;
	aq::Communicator::box_ctrl_msg.signalling = 15;			//To upload  grab image to server.
	if (m_bSaveImage)
	{
		GetDlgItem(IDC_BUTTON_START_SAVE)->SetWindowTextA("��ͣ...");
		aq::Communicator::box_ctrl_msg.magnetic_valve_num = 1;
		::PostMessage(AfxGetMainWnd()->m_hWnd, ID_SHOWCTRLMESSAGE, 16, (LPARAM)m_strSavePath.c_str());
	}
	else
	{
		GetDlgItem(IDC_BUTTON_START_SAVE)->SetWindowTextA("��������");
		aq::Communicator::box_ctrl_msg.magnetic_valve_num = 0;
	}

	Vsee::VCameraMessageTrans *frame = new Vsee::VCameraMessageTrans;
	frame->signalling = aq::Communicator::box_ctrl_msg.signalling;
	frame->magnetic_valve_num = aq::Communicator::box_ctrl_msg.magnetic_valve_num;
	int nSelect = m_comboCameraList.GetCurSel()+1;
	aq::Communicator::box_ctrl_msg.channels = nSelect;
	memcpy(frame, &aq::Communicator::box_ctrl_msg, sizeof(Vsee::VCameraMessageTrans));
	::PostMessage(AfxGetMainWnd()->m_hWnd, ID_SHOWCTRLMESSAGE, aq::Communicator::box_ctrl_msg.signalling, (LPARAM)frame);
}


void CSaveDetectResult::OnBnClickedButtonGetPath()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	TCHAR szPath[_MAX_PATH];
	//std::string InputPath;
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
			m_strSavePath = szPath;
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
		m_strSavePath = "";
	GetDlgItem(IDC_EDIT_SAVE_PATH)->SetWindowTextA(m_strSavePath.c_str());
}


void CSaveDetectResult::OnBnClickedOk()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	CDialogEx::OnOK();
}
