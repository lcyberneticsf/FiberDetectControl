// NetWorkParameters.cpp : 实现文件
//

#include "stdafx.h"
#include "FiberDetectControl.h"
#include "NetWorkParameters.h"
#include "afxdialogex.h"


// CNetWorkParameters 对话框

IMPLEMENT_DYNAMIC(CNetWorkParameters, CDialogEx)

CNetWorkParameters::CNetWorkParameters(CWnd* pParent /*=NULL*/)
	: CDialogEx(CNetWorkParameters::IDD, pParent)
{

}

CNetWorkParameters::~CNetWorkParameters()
{
}

void CNetWorkParameters::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CNetWorkParameters, CDialogEx)
	ON_BN_CLICKED(IDOK, &CNetWorkParameters::OnBnClickedOk)
END_MESSAGE_MAP()


// CNetWorkParameters 消息处理程序
BOOL CNetWorkParameters::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	GetDlgItem(IDC_EDIT_IP_ADRESS)->SetWindowTextA(m_strNetString.c_str());
	GetDlgItem(IDC_EDIT_IP_PORT_NUMBER)->SetWindowTextA(std::to_string(m_u32NetPort).c_str());

	return true;
}


void CNetWorkParameters::OnBnClickedOk()
{
	// TODO:  在此添加控件通知处理程序代码
	CString str;
	GetDlgItem(IDC_EDIT_IP_ADRESS)->GetWindowText(str);
	m_strNetString = str.GetBuffer();
	GetDlgItem(IDC_EDIT_IP_PORT_NUMBER)->GetWindowText(str);
	m_u32NetPort = atoi(str);

	CDialogEx::OnOK();
}
