// BaseDetectParametersSetting.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "FiberDetectControl.h"
#include "BaseDetectParametersSetting.h"
#include "afxdialogex.h"


// CBaseDetectParametersSetting �Ի���

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


// CBaseDetectParametersSetting ��Ϣ�������


void CBaseDetectParametersSetting::OnBnClickedOk()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	//CDialogEx::OnOK();
}


void CBaseDetectParametersSetting::OnBnClickedCancel()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	//CDialogEx::OnCancel();
}


void CBaseDetectParametersSetting::OnEnChangeEditCamera1BeginPoint()
{
	// TODO:  ����ÿؼ��� RICHEDIT �ؼ���������
	// ���ʹ�֪ͨ��������д CDialogEx::OnInitDialog()
	// ���������� CRichEditCtrl().SetEventMask()��
	// ͬʱ�� ENM_CHANGE ��־�������㵽�����С�

	// TODO:  �ڴ���ӿؼ�֪ͨ����������
}
