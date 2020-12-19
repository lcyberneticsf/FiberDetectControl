#pragma once


// CValveParametersSetting 对话框

class CValveParametersSetting : public CDialogEx
{
	DECLARE_DYNAMIC(CValveParametersSetting)

public:
	CValveParametersSetting(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CValveParametersSetting();

// 对话框数据
	enum { IDD = IDD_DIALOG_VALVA_PARAMETERS };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
};
