#pragma once
#include "afxwin.h"
#include <string> 


// CModeSelectDlg �Ի���

class CModeSelectDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CModeSelectDlg)

public:
	CModeSelectDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CModeSelectDlg();

// �Ի�������
	enum { IDD = IDD_DIALOG_MODEL_SELECT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()
public:
	CComboBox m_comboModelSelect;
	std::string model_type[6];
	int m_nModelSeletc=0;
	afx_msg void OnBnClickedOk();
	CEdit m_editText;
	std::string model_type_text[6];
	bool m_bEditText = false;
	afx_msg void OnBnClickedButtonEdit();
	afx_msg void OnCbnSelchangeComboModelSeletc();
};
