// DebugParametersSetting.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "FiberDetectControl.h"
#include "DebugParametersSetting.h"
#include "afxdialogex.h"


// CDebugParametersSetting �Ի���

IMPLEMENT_DYNAMIC(CDebugParametersSetting, CDialogEx)

CDebugParametersSetting::CDebugParametersSetting(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDebugParametersSetting::IDD, pParent)
{

}

CDebugParametersSetting::~CDebugParametersSetting()
{
}

void CDebugParametersSetting::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CHECK_DEBUG_CAMAMETERA, m_checkDebugCameraA);
	DDX_Control(pDX, IDC_CHECK_DEBUG_CAMAMETERB, m_checkDebugCameraB);
	DDX_Control(pDX, IDC_CHECK_DEBUG_CAMAMETERC, m_checkDebugCameraC);
	DDX_Control(pDX, IDC_CHECK_DEBUG_CAMAMETERD, m_checkDebugCameraD);
	DDX_Control(pDX, IDC_CHECK_STOP_VALVE, m_checkStopValve);
	DDX_Control(pDX, IDC_CHECK_SAVE_BLOB, m_checkSaveBolb);
}


BEGIN_MESSAGE_MAP(CDebugParametersSetting, CDialogEx)
	ON_BN_CLICKED(IDOK, &CDebugParametersSetting::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CDebugParametersSetting::OnBnClickedCancel)
END_MESSAGE_MAP()


// CDebugParametersSetting ��Ϣ�������
BOOL CDebugParametersSetting::OnInitDialog()
{
	CDialogEx::OnInitDialog();


	return true;
}


void CDebugParametersSetting::OnBnClickedOk()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	//CDialogEx::OnOK();
}


void CDebugParametersSetting::OnBnClickedCancel()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	//CDialogEx::OnCancel();
}
