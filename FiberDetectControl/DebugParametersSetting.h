#pragma once
#include "afxwin.h"


// CDebugParametersSetting 对话框

class CDebugParametersSetting : public CDialogEx
{
	DECLARE_DYNAMIC(CDebugParametersSetting)

public:
	CDebugParametersSetting(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDebugParametersSetting();
	

// 对话框数据
	enum { IDD = IDD_DIALOG_DEBUG_PARAMETERS };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
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
