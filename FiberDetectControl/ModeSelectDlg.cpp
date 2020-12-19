// ModeSelectDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "FiberDetectControl.h"
#include "ModeSelectDlg.h"
#include "afxdialogex.h"


// CModeSelectDlg 对话框

IMPLEMENT_DYNAMIC(CModeSelectDlg, CDialogEx)

CModeSelectDlg::CModeSelectDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CModeSelectDlg::IDD, pParent)
{

}

CModeSelectDlg::~CModeSelectDlg()
{
}

void CModeSelectDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_MODEL_SELETC, m_comboModelSelect);
	DDX_Control(pDX, IDC_EDIT_TEXT, m_editText);
}


BEGIN_MESSAGE_MAP(CModeSelectDlg, CDialogEx)
	ON_BN_CLICKED(IDOK, &CModeSelectDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDC_BUTTON_EDIT, &CModeSelectDlg::OnBnClickedButtonEdit)
	ON_CBN_SELCHANGE(IDC_COMBO_MODEL_SELETC, &CModeSelectDlg::OnCbnSelchangeComboModelSeletc)
END_MESSAGE_MAP()


// CModeSelectDlg 消息处理程序

BOOL CModeSelectDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	std::string combo_show_string;
	for (int i = 0; i < 6; i++)
	{
		combo_show_string = std::to_string(i+1) + ":" + model_type[i];
		m_comboModelSelect.AddString(combo_show_string.c_str());
	}	
	m_comboModelSelect.SetCurSel(m_nModelSeletc);
	m_editText.SetWindowTextA(model_type_text[m_nModelSeletc].c_str());
	m_editText.EnableWindow(FALSE);

	if (m_bEditText)
		GetDlgItem(IDC_BUTTON_EDIT)->SetWindowTextA("结束\r\n编辑");
	else
		GetDlgItem(IDC_BUTTON_EDIT)->SetWindowTextA("编辑\r\n文本\r\n说明");

	return true;
}


void CModeSelectDlg::OnBnClickedOk()
{
	// TODO:  在此添加控件通知处理程序代码
	m_nModelSeletc = m_comboModelSelect.GetCurSel();

	CString    sPath;
	GetModuleFileNameA(NULL, sPath.GetBufferSetLength(MAX_PATH + 1), MAX_PATH);
	sPath.ReleaseBuffer();
	int    nPos;
	nPos = sPath.ReverseFind('\\');
	sPath = sPath.Left(nPos);
	char IniPath[255];
	memset(IniPath, 0, 255);
	//camera_ip_info ci_info;
	CString strPath = sPath; 
	strcpy_s(IniPath, 255, strPath.GetBuffer());
	strcat_s(IniPath, "/param.ini");
	CString IniRead;
	CString IniWrite;
	CString modle_select;
	IniWrite.Format("%d", model_type_text[m_nModelSeletc].c_str());
	switch (m_nModelSeletc)
	{
	case 1:modle_select = "model1_text"; break;
	case 2:modle_select = "model2_text"; break;
	case 3:modle_select = "model3_text"; break;
	case 4:modle_select = "model4_text"; break;
	case 5:modle_select = "model5_text"; break;
	case 6:modle_select = "model6_text"; break;
	default:break;
	}
	WritePrivateProfileString("System", modle_select, IniWrite, IniPath);
	CDialogEx::OnOK();
}


void CModeSelectDlg::OnBnClickedButtonEdit()
{
	// TODO:  在此添加控件通知处理程序代码
	m_bEditText = !m_bEditText;
	m_editText.EnableWindow(m_bEditText);
	if (m_bEditText)
	{
		GetDlgItem(IDOK)->EnableWindow(FALSE);
		GetDlgItem(IDC_COMBO_MODEL_SELETC)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_EDIT)->SetWindowTextA("结束\r\n编辑");
	}
	else
	{
		GetDlgItem(IDOK)->EnableWindow(TRUE);
		GetDlgItem(IDC_COMBO_MODEL_SELETC)->EnableWindow(TRUE);
		GetDlgItem(IDC_BUTTON_EDIT)->SetWindowTextA("编辑\r\n文本\r\n说明");
		CString strText;
		m_editText.GetWindowTextA(strText);
		model_type_text[m_nModelSeletc] = strText;
	}
}


void CModeSelectDlg::OnCbnSelchangeComboModelSeletc()
{
	// TODO:  在此添加控件通知处理程序代码
	m_nModelSeletc = m_comboModelSelect.GetCurSel();

	m_editText.SetWindowTextA(model_type_text[m_nModelSeletc].c_str());

}
