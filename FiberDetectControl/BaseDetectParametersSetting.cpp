// BaseDetectParametersSetting.cpp : 实现文件
//

#include "stdafx.h"
#include "FiberDetectControl.h"
#include "BaseDetectParametersSetting.h"
#include "afxdialogex.h"


// CBaseDetectParametersSetting 对话框

IMPLEMENT_DYNAMIC(CBaseDetectParametersSetting, CDialogEx)

CBaseDetectParametersSetting::CBaseDetectParametersSetting(CWnd* pParent /*=NULL*/)
	: CDialogEx(CBaseDetectParametersSetting::IDD, pParent)
{

}

CBaseDetectParametersSetting::~CBaseDetectParametersSetting()
{
}

void CBaseDetectParametersSetting::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CBaseDetectParametersSetting, CDialogEx)
	ON_BN_CLICKED(IDOK, &CBaseDetectParametersSetting::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CBaseDetectParametersSetting::OnBnClickedCancel)
	ON_EN_CHANGE(IDC_EDIT_CAMERA1_BEGIN_POINT, &CBaseDetectParametersSetting::OnEnChangeEditCamera1BeginPoint)
END_MESSAGE_MAP()


// CBaseDetectParametersSetting 消息处理程序


void CBaseDetectParametersSetting::OnBnClickedOk()
{
	// TODO:  在此添加控件通知处理程序代码
	//CDialogEx::OnOK();
}


void CBaseDetectParametersSetting::OnBnClickedCancel()
{
	// TODO:  在此添加控件通知处理程序代码
	//CDialogEx::OnCancel();
}


void CBaseDetectParametersSetting::OnEnChangeEditCamera1BeginPoint()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CDialogEx::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
}
