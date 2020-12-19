#pragma once


// CMagneticValve 对话框

class CMagneticValve : public CDialogEx
{
	DECLARE_DYNAMIC(CMagneticValve)

public:
	CMagneticValve(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CMagneticValve();

// 对话框数据
	enum { IDD = IDD_DIALOG_HARDWARE_TEST };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedMagneticValve1();
	afx_msg void OnBnClickedMagneticValve2();
	afx_msg void OnBnClickedMagneticValve3();
	afx_msg void OnBnClickedMagneticValve4();
	afx_msg void OnBnClickedMagneticValve5();
	afx_msg void OnBnClickedMagneticValve6();
	afx_msg void OnBnClickedMagneticValve7();
	afx_msg void OnBnClickedMagneticValve8();
	afx_msg void OnBnClickedMagneticValve9();
	afx_msg void OnBnClickedMagneticValve10();
	afx_msg void OnBnClickedMagneticValve11();
	afx_msg void OnBnClickedMagneticValve12();
	afx_msg void OnBnClickedMagneticValve13();
	afx_msg void OnBnClickedMagneticValve14();
	afx_msg void OnBnClickedMagneticValve15();
	afx_msg void OnBnClickedMagneticValve16();
	afx_msg void OnBnClickedMagneticValve17();
	afx_msg void OnBnClickedMagneticValve18();
	afx_msg void OnBnClickedMagneticValve19();
	afx_msg void OnBnClickedMagneticValve20();
	afx_msg void OnBnClickedMagneticValve21();
	afx_msg void OnBnClickedMagneticValve22();
	afx_msg void OnBnClickedMagneticValve23();
	afx_msg void OnBnClickedMagneticValve24();
	afx_msg void OnBnClickedOk();
};
