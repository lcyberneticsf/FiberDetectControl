#pragma once
#include "afxwin.h"
#include <string>

// CSaveDetectResult �Ի���

class CSaveDetectResult : public CDialogEx
{
	DECLARE_DYNAMIC(CSaveDetectResult)

public:
	CSaveDetectResult(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CSaveDetectResult();
	virtual BOOL OnInitDialog();
	std::string m_strSavePath;
	bool m_bSaveImage = false;
// �Ի�������
	enum { IDD = IDD_DIALOG_SAVE_DETECT_RESULT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	CComboBox m_comboCameraList;
	afx_msg void OnBnClickedButtonStartSave();
	afx_msg void OnBnClickedButtonGetPath();
	afx_msg void OnBnClickedOk();
};
