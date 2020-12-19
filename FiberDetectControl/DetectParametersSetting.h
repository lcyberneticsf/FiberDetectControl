#pragma once
#include "afxcmn.h"
#include "BaseDetectParametersSetting.h"
#include "DebugParametersSetting.h"
#include "ValveParametersSetting.h"
#include "FiberDetectParametersSettting.h"
#include "ParameterSetting.h"


// CDetectParametersSetting 对话框

class CDetectParametersSetting : public CDialogEx
{
	DECLARE_DYNAMIC(CDetectParametersSetting)

public:
	CDetectParametersSetting(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDetectParametersSetting();

// 对话框数据
	enum { IDD = IDD_DIALOG_DETECT_PARAMETERS_SETTINGS };

protected:
	virtual BOOL OnInitDialog();
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CTabCtrl m_cTabControl;
	CBaseDetectParametersSetting	m_dlgBaseDetectParametersSettting;
	CDebugParametersSetting			m_dlgDebugParametersSetting;
	CValveParametersSetting			m_dlgValveParametersSettting;

	CFiberDetectParametersSettting  m_dlgFiberDetectParametersSettingA;
	CFiberDetectParametersSettting  m_dlgFiberDetectParametersSettingB;
	CFiberDetectParametersSettting  m_dlgFiberDetectParametersSettingC;
	CFiberDetectParametersSettting  m_dlgFiberDetectParametersSettingD;
	CParameterSetting				parameters_setting_trans;            //parameters settig dialog;

	afx_msg void OnTcnSelchangeTabParametersSetting(NMHDR *pNMHDR, LRESULT *pResult);
	CString		GetModulePath(void);
	afx_msg void OnBnClickedOk();
	void ReadIni();
	void WriteIni();
	afx_msg void OnBnClickedDialogSendMessage();
};
