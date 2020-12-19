// MeasureResultDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "FiberDetectControl.h"
#include "MeasureResultDlg.h"
#include "afxdialogex.h"
#include "mmsystem.h"
#include "FiberDetectControlDlg.h"
#include <math.h>
#include <Mmsystem.h>
#pragma comment(lib,"winmm.lib")


HWND CMeasureResultDlg::m_hMRDWnd;
double CMeasureResultDlg::m_dfValue = 0.0f;
// CMeasureResultDlg 对话框

IMPLEMENT_DYNAMIC(CMeasureResultDlg, CDialogEx)

CMeasureResultDlg::CMeasureResultDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CMeasureResultDlg::IDD, pParent)
{

}

CMeasureResultDlg::~CMeasureResultDlg()
{
}

void CMeasureResultDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_SCOPE,	 m_wndScope);
	DDX_Control(pDX, IDC_SCOPE2, m_wndScope2);
	DDX_Control(pDX, IDC_SCOPE3, m_wndScope3);
	DDX_Control(pDX, IDC_SCOPE4, m_wndScope4);
	DDX_Control(pDX, IDC_PROGRESS1, m_pro1);
	DDX_Control(pDX, IDC_PROGRESS2, m_pro2);
	DDX_Control(pDX, IDC_PROGRESS3, m_pro3);
	DDX_Control(pDX, IDC_PROGRESS4, m_pro4);
}


BEGIN_MESSAGE_MAP(CMeasureResultDlg, CDialogEx)
	ON_WM_TIMER()
	ON_MESSAGE(ID_SHOWMEASURERESULT, ShowMeaureResult)
	ON_BN_CLICKED(IDOK, &CMeasureResultDlg::OnBnClickedOk)

	ON_WM_CLOSE()
END_MESSAGE_MAP()


// CMeasureResultDlg 消息处理程序
BOOL CMeasureResultDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	// TODO:  Add extra initialization here
	m_pro1.SetRange(0, 100);
	m_pro2.SetRange(0, 100);
	m_pro1.SetStep(1);
	m_pro2.SetStep(1);
	HDC hdc;
	hdc = (HDC)this->GetDC();
	PAINTSTRUCT  ps;
	CPen*        myPen;
	CDC* pDC = GetDC();
	ReleaseDC(pDC);

	m_wndScope.SetTitle("相机A检测个数");
	m_wndScope.SetValueString("个数", "像素");

	m_wndScope2.SetTitle("相机B检测个数");
	m_wndScope2.SetValueString("个数", "像素");

	m_wndScope3.SetTitle("相机C检测个数");
	m_wndScope3.SetValueString("个数", "像素");

	m_wndScope4.SetTitle("相机D检测个数");
	m_wndScope4.SetValueString("个数", "像素");

	//::SendMessage(m_Sposi.GetSafeHwnd(),PBM_SETBKCOLOR,0,RGB(0,0,255));
	//::SendMessage(m_Snega.GetSafeHwnd(),PBM_SETBKCOLOR,0,RGB(0,0,255));
	CMeasureResultDlg::m_hMRDWnd = this->GetSafeHwnd();

	//int n = SetTimer(1, 100, NULL);

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
	
}
void CMeasureResultDlg::OnOk()
{

}
void CMeasureResultDlg::OnCancel()

{

}

void CMeasureResultDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: Add your message handler code here and/or call default
	double dTime = ::timeGetTime() / 1000.0;
	double dValue[3];

	if(nIDEvent==1)
	{
		dValue[0] = 4 * sin(dTime + 2);				//CImageMeasure::WtoTdiata;//
		m_wndScope.AddValue(dTime, dValue[0]);
		m_wndScope.SetValue(dValue[0]);
		m_wndScope.UpdateCurve();
		
	}
	ShowMeaureResult(0, 0);

	CDialog::OnTimer(nIDEvent);
}
LRESULT CMeasureResultDlg::ShowMeaureResult(WPARAM wParam, LPARAM lParam)		//显示示图和柱状图
{
	int n = wParam;
	switch (n)
	{
	case 1:
	{
			double dTime = ::timeGetTime() / 1000.0;
			double dValue[3];
			m_pro1.SetPos(0);
			//m_pro2.SetPos(0);
			//::SendMessage(m_pro2.GetSafeHwnd(), PBM_SETBKCOLOR, 0, RGB(0, 0, 0));
			::SendMessage(m_pro1.GetSafeHwnd(), PBM_SETBKCOLOR, 0, RGB(0, 0, 0));
			dTime = ::timeGetTime() / 1000.0;
			//vealue = (double*)wParam;
			//dValue[0] = *vealue;
			dValue[0] = m_dfValue;

			m_wndScope.AddValue(dTime, dValue[0]);
			m_wndScope.SetValue(dValue[0]);
			m_wndScope.UpdateCurve();
			m_pro1.SetPos(int(dValue[0] * 10));
	}
		break;
	case 2:
	{
			  double dTime = ::timeGetTime() / 1000.0;
			  double dValue[3];
			  m_pro2.SetPos(0);
			  //m_pro2.SetPos(0);
			  //::SendMessage(m_pro2.GetSafeHwnd(), PBM_SETBKCOLOR, 0, RGB(0, 0, 0));
			  ::SendMessage(m_pro2.GetSafeHwnd(), PBM_SETBKCOLOR, 0, RGB(0, 0, 0));
			  dTime = ::timeGetTime() / 1000.0;
			  //vealue = (double*)wParam;
			  //dValue[0] = *vealue;
			  dValue[1] = m_dfValue;

			  m_wndScope2.AddValue(dTime, dValue[1]);
			  m_wndScope2.SetValue(dValue[1]);
			  m_wndScope2.UpdateCurve();
			  m_pro2.SetPos(int(dValue[1] * 10));
	}
		break;
	case 3:
	{
			  double dTime = ::timeGetTime() / 1000.0;
			  double dValue[3];
			  m_pro3.SetPos(0);
			  //m_pro2.SetPos(0);
			  //::SendMessage(m_pro2.GetSafeHwnd(), PBM_SETBKCOLOR, 0, RGB(0, 0, 0));
			  ::SendMessage(m_pro3.GetSafeHwnd(), PBM_SETBKCOLOR, 0, RGB(0, 0, 0));
			  dTime = ::timeGetTime() / 1000.0;
			  //vealue = (double*)wParam;
			  //dValue[0] = *vealue;
			  dValue[1] = m_dfValue;

			  m_wndScope3.AddValue(dTime, dValue[1]);
			  m_wndScope3.SetValue(dValue[1]);
			  m_wndScope3.UpdateCurve();
			  m_pro3.SetPos(int(dValue[1] * 10));
	}
		break;
	case 4:
	{
			  double dTime = ::timeGetTime() / 1000.0;
			  double dValue[3];
			  m_pro4.SetPos(0);
			  //m_pro2.SetPos(0);
			  //::SendMessage(m_pro2.GetSafeHwnd(), PBM_SETBKCOLOR, 0, RGB(0, 0, 0));
			  ::SendMessage(m_pro4.GetSafeHwnd(), PBM_SETBKCOLOR, 0, RGB(0, 0, 0));
			  dTime = ::timeGetTime() / 1000.0;
			  //vealue = (double*)wParam;
			  //dValue[0] = *vealue;
			  dValue[1] = m_dfValue;

			  m_wndScope4.AddValue(dTime, dValue[1]);
			  m_wndScope4.SetValue(dValue[1]);
			  m_wndScope4.UpdateCurve();
			  m_pro4.SetPos(int(dValue[1] * 10));
	}
		break;
	default:
		break;
	}
	
	
	return 0;
}

void CMeasureResultDlg::OnBnClickedOk()
{
	// TODO:  在此添加控件通知处理程序代码
	CDialogEx::OnOK();
}


void CMeasureResultDlg::OnClose()
{
	// TODO:  在此添加消息处理程序代码和/或调用默认值
	CFiberDetectControlDlg::m_MeasureResultDlg = NULL;
	EndDialog(IDCANCEL);	//关闭窗口
	CDialogEx::OnClose();
}
