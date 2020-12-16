// FiberDetectParametersSettting.cpp : 实现文件
//

#include "stdafx.h"
#include "FiberDetectControl.h"
#include "FiberDetectParametersSettting.h"
#include "afxdialogex.h"


// CFiberDetectParametersSettting 对话框

IMPLEMENT_DYNAMIC(CFiberDetectParametersSettting, CDialogEx)

CFiberDetectParametersSettting::CFiberDetectParametersSettting(CWnd* pParent /*=NULL*/)
	: CDialogEx(CFiberDetectParametersSettting::IDD, pParent)
{

}

CFiberDetectParametersSettting::~CFiberDetectParametersSettting()
{
}

void CFiberDetectParametersSettting::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CFiberDetectParametersSettting, CDialogEx)
	ON_BN_CLICKED(IDOK, &CFiberDetectParametersSettting::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CFiberDetectParametersSettting::OnBnClickedCancel)
END_MESSAGE_MAP()


// CFiberDetectParametersSettting 消息处理程序


void CFiberDetectParametersSettting::OnBnClickedOk()
{
	// TODO:  在此添加控件通知处理程序代码
	//CDialogEx::OnOK();
}


void CFiberDetectParametersSettting::OnBnClickedCancel()
{
	// TODO:  在此添加控件通知处理程序代码
	//CDialogEx::OnCancel();
}
