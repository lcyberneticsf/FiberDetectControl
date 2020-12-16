#pragma once


// CBaseDetectParametersSetting 对话框

class CBaseDetectParametersSetting : public CDialogEx
{
	DECLARE_DYNAMIC(CBaseDetectParametersSetting)

public:
	CBaseDetectParametersSetting(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CBaseDetectParametersSetting();

// 对话框数据
	enum { IDD = IDD_DIALOG_BASE_DETECT_PARAMETERS_SETTING };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnEnChangeEditCamera1BeginPoint();
};
