#pragma once
#include "afxwin.h"


// CDebugParametersSetting �Ի���

class CDebugParametersSetting : public CDialogEx
{
	DECLARE_DYNAMIC(CDebugParametersSetting)

public:
	CDebugParametersSetting(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDebugParametersSetting();
	

// �Ի�������
	enum { IDD = IDD_DIALOG_DEBUG_PARAMETERS };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	CButton m_checkDebugCameraA;
	CButton m_checkDebugCameraB;
	CButton m_checkDebugCameraC;
	CButton m_checkDebugCameraD;
	CButton m_checkStopValve;
	CButton m_checkSaveBolb;
};
