#pragma once
#include "VCameraMessageTrans.h"

// CBoxSelect 对话框

class CBoxSelect : public CDialogEx
{
	DECLARE_DYNAMIC(CBoxSelect)

public:
	CBoxSelect(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CBoxSelect();

// 对话框数据
	enum { IDD = IDD_DIALOG_BOX_CHOOSE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButtonBoxSelect();
	void ReadIni();
	int GetBoxnum(void);
	CParameterSetting	parameters_setting_trans;
	CString  GetModulePath(void);
	afx_msg void OnBnClickedAiboxCheck1();
	afx_msg void OnBnClickedAiboxCheck2();
	afx_msg void OnBnClickedAiboxCheck3();
};
