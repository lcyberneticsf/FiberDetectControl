// DebugParametersSetting.cpp : 实现文件
//

#include "stdafx.h"
#include "FiberDetectControl.h"
#include "DebugParametersSetting.h"
#include "afxdialogex.h"


// CDebugParametersSetting 对话框

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


// CDebugParametersSetting 消息处理程序
BOOL CDebugParametersSetting::OnInitDialog()
{
	CDialogEx::OnInitDialog();


	return true;
}


void CDebugParametersSetting::OnBnClickedOk()
{
	// TODO:  在此添加控件通知处理程序代码
	//CDialogEx::OnOK();
}


void CDebugParametersSetting::OnBnClickedCancel()
{
	// TODO:  在此添加控件通知处理程序代码
	//CDialogEx::OnCancel();
}
