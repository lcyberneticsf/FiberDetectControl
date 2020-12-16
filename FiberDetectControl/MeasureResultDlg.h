#pragma once
#include "Scope.h"
#include "afxcmn.h"
#include "afxwin.h"

// CMeasureResultDlg �Ի���

class CMeasureResultDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CMeasureResultDlg)

public:
	CMeasureResultDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CMeasureResultDlg();

// �Ի�������
	enum { IDD = IDD_MEASURE_RESULT };

	CScope	m_wndScope;
	CScope	m_wndScope2;
	CScope	m_wndScope3;
	CScope	m_wndScope4;
	static HWND m_hMRDWnd;			//=AfxGetMainWnd()->m_hWnd;
	static double m_dfValue;
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
	afx_msg LRESULT ShowMeaureResult(WPARAM wParam, LPARAM lParam);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()	
public:
	CProgressCtrl m_pro1;
	CProgressCtrl m_pro2;
	
	CProgressCtrl m_pro3;
	CProgressCtrl m_pro4;
	afx_msg void OnBnClickedOk();
};
