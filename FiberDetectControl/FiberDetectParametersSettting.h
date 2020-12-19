#pragma once


// CFiberDetectParametersSettting 对话框

class CFiberDetectParametersSettting : public CDialogEx
{
	DECLARE_DYNAMIC(CFiberDetectParametersSettting)

public:
	CFiberDetectParametersSettting(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CFiberDetectParametersSettting();

// 对话框数据
	enum { IDD = IDD_DIALOG_FIBER_DETECT_PARAMETERS_SETTING };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
};
