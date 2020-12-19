// ValveParametersSetting.cpp : 实现文件
//

#include "stdafx.h"
#include "FiberDetectControl.h"
#include "ValveParametersSetting.h"
#include "afxdialogex.h"


// CValveParametersSetting 对话框

IMPLEMENT_DYNAMIC(CValveParametersSetting, CDialogEx)

CValveParametersSetting::CValveParametersSetting(CWnd* pParent /*=NULL*/)
	: CDialogEx(CValveParametersSetting::IDD, pParent)
{

}

CValveParametersSetting::~CValveParametersSetting()
{
}

void CValveParametersSetting::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CValveParametersSetting, CDialogEx)
	ON_BN_CLICKED(IDOK, &CValveParametersSetting::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CValveParametersSetting::OnBnClickedCancel)
END_MESSAGE_MAP()


// CValveParametersSetting 消息处理程序


void CValveParametersSetting::OnBnClickedOk()
{
	// TODO:  在此添加控件通知处理程序代码
	//CDialogEx::OnOK();
}


void CValveParametersSetting::OnBnClickedCancel()
{
	// TODO:  在此添加控件通知处理程序代码
	//CDialogEx::OnCancel();
}
