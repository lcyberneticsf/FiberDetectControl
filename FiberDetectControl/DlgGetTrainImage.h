#pragma once
#include "afxbutton.h"
#include "afxwin.h"
#include <stdio.h>
#include <iostream>

// CDlgGetTrainImage 对话框

class CDlgGetTrainImage : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgGetTrainImage)

public:
	CDlgGetTrainImage(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgGetTrainImage();

// 对话框数据
	enum { IDD = IDD_DIALOG_GET_TRAIN_IMAGE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()
public:
	CMFCButton m_MFCButton1;
	CMFCButton m_MFCButton2;
	CMFCButton m_MFCButton3;
	CComboBox m_comboCameraList;
	afx_msg void OnBnClickedButtonGetPath();
	afx_msg void OnBnClickedButtonStopDetect();
	afx_msg void OnBnClickedButtonStartGrab();
	afx_msg void OnBnClickedButtonStopGrab();
	afx_msg void OnBnClickedButtonUploadImage();
	bool m_bSaveImage = false;
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	static std::string  m_path;
};
