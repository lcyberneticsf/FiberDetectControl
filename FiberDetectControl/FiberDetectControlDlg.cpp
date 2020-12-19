
// FiberDetectControlDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "FiberDetectControl.h"
#include "FiberDetectControlDlg.h"
#include "afxdialogex.h"
#include "MagneticValve.h"
#include "MeasureResultDlg.h"
#include "NetSetting.h"
#include "DetectParametersSetting.h"
#include	<thread>
#include	<queue>
#include	<string>
#include	<io.h>
#include "DlgGetTrainImage.h"
#include "detect_recog.h"
#include "CameraSettingDlg.h"
#include "aidi_vision.h"
#include "SaveDetectResult.h"
#include "BoxSelect.h"
#include <mutex>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

using namespace aq;
using namespace std;
using namespace cv;
using namespace Vsee;
std::queue<InferFrame>		CFiberDetectControlDlg::infer_queue;
HWND CFiberDetectControlDlg::m_hWnd;
HWND m_hMainWnd;
CLog	CFiberDetectControlDlg::m_Log;
CMeasureResultDlg* CFiberDetectControlDlg::m_MeasureResultDlg = NULL;
using MutexLock = std::lock_guard<std::mutex>;
std::mutex cout_mutex;
CMFCButton* CFiberDetectControlDlg::m_mfcMagneticValve[MAGNETIC_VALVE_SUM];
_io_output_transac_tag_  CFiberDetectControlDlg::valve_driver[MAGNETIC_VALVE_SUM];
bool start_grab = false;
bool end_garb = true;
struct center
{
	/*float m_center_x[DATA_SHOW_LENGHT] ;
	float m_center_y[DATA_SHOW_LENGHT] ;*/
	double m_center_x;
	double m_area;
	double m_center_y;
}Information;

// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CFiberDetectControlDlg 对话框



CFiberDetectControlDlg::CFiberDetectControlDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CFiberDetectControlDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

	communicator_ = new aq::Communicator;	
	communicator_->camera_scan_speed = 10;
	m_nWindowWidth = 1100;
	m_nWindowHeight = 700;
}

void CFiberDetectControlDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_MFCBUTTON_MAGNETIC_VOLVE1, m_mfcMagneticValve1);
	DDX_Control(pDX, IDC_MFCBUTTON_MAGNETIC_VOLVE2, m_mfcMagneticValve2);
	DDX_Control(pDX, IDC_MFCBUTTON_MAGNETIC_VOLVE3, m_mfcMagneticValve3);
	DDX_Control(pDX, IDC_MFCBUTTON_MAGNETIC_VOLVE4, m_mfcMagneticValve4);
	DDX_Control(pDX, IDC_MFCBUTTON_MAGNETIC_VOLVE5, m_mfcMagneticValve5);
	DDX_Control(pDX, IDC_MFCBUTTON_MAGNETIC_VOLVE6, m_mfcMagneticValve6);
	DDX_Control(pDX, IDC_MFCBUTTON_MAGNETIC_VOLVE7, m_mfcMagneticValve7);
	DDX_Control(pDX, IDC_MFCBUTTON_MAGNETIC_VOLVE8, m_mfcMagneticValve8);
	DDX_Control(pDX, IDC_MFCBUTTON_MAGNETIC_VOLVE9, m_mfcMagneticValve9);
	DDX_Control(pDX, IDC_MFCBUTTON_MAGNETIC_VOLVE10, m_mfcMagneticValve10);
	DDX_Control(pDX, IDC_MFCBUTTON_MAGNETIC_VOLVE11, m_mfcMagneticValve11);
	DDX_Control(pDX, IDC_MFCBUTTON_MAGNETIC_VOLVE12, m_mfcMagneticValve12);
	DDX_Control(pDX, IDC_MFCBUTTON_MAGNETIC_VOLVE13, m_mfcMagneticValve13);
	DDX_Control(pDX, IDC_MFCBUTTON_MAGNETIC_VOLVE14, m_mfcMagneticValve14);
	DDX_Control(pDX, IDC_MFCBUTTON_MAGNETIC_VOLVE15, m_mfcMagneticValve15);
	DDX_Control(pDX, IDC_MFCBUTTON_MAGNETIC_VOLVE16, m_mfcMagneticValve16);
	DDX_Control(pDX, IDC_MFCBUTTON_MAGNETIC_VOLVE17, m_mfcMagneticValve17);
	DDX_Control(pDX, IDC_MFCBUTTON_MAGNETIC_VOLVE18, m_mfcMagneticValve18);
	DDX_Control(pDX, IDC_MFCBUTTON_MAGNETIC_VOLVE19, m_mfcMagneticValve19);
	DDX_Control(pDX, IDC_MFCBUTTON_MAGNETIC_VOLVE20, m_mfcMagneticValve20);
	DDX_Control(pDX, IDC_MFCBUTTON_MAGNETIC_VOLVE21, m_mfcMagneticValve21);
	DDX_Control(pDX, IDC_MFCBUTTON_MAGNETIC_VOLVE22, m_mfcMagneticValve22);
	DDX_Control(pDX, IDC_MFCBUTTON_MAGNETIC_VOLVE23, m_mfcMagneticValve23);
	DDX_Control(pDX, IDC_MFCBUTTON_MAGNETIC_VOLVE24, m_mfcMagneticValve24);
	DDX_Control(pDX, IDC_CHECK_SERVER_MODE_SELECT, m_checkServerModeSelect);
	DDX_Control(pDX, IDC_CHECK_MODE_SELECT, m_checkClientMode);
	DDX_Control(pDX, IDC_CHECK_SERVER_SEND, m_checkServerSend);
	DDX_Control(pDX, IDC_CHECK_CLIENT_SEND, m_checkClientSend);
	DDX_Control(pDX, IDC_MFCBUTTON_MAGNETIC_VOLVE_TEST1, m_mfcMagneticValveTest1);
	DDX_Control(pDX, IDC_MFCBUTTON_MAGNETIC_VOLVE_TEST2, m_mfcMagneticValveTest2);
	DDX_Control(pDX, IDC_MFCBUTTON_MAGNETIC_VOLVE_TEST3, m_mfcMagneticValveTest3);
	DDX_Control(pDX, IDC_MFCBUTTON_MAGNETIC_VOLVE_TEST4, m_mfcMagneticValveTest4);
	DDX_Control(pDX, IDC_MFCBUTTON_MAGNETIC_VOLVE_TEST5, m_mfcMagneticValveTest5);
	DDX_Control(pDX, IDC_MFCBUTTON_MAGNETIC_VOLVE_TEST6, m_mfcMagneticValveTest6);
	DDX_Control(pDX, IDC_MFCBUTTON_MAGNETIC_VOLVE_TEST7, m_mfcMagneticValveTest7);
	DDX_Control(pDX, IDC_MFCBUTTON_MAGNETIC_VOLVE_TEST8, m_mfcMagneticValveTest8);
	DDX_Control(pDX, IDC_MFCBUTTON_MAGNETIC_VOLVE_TEST9, m_mfcMagneticValveTest9);
	DDX_Control(pDX, IDC_MFCBUTTON_MAGNETIC_VOLVE_TEST10, m_mfcMagneticValveTest10);
	DDX_Control(pDX, IDC_MFCBUTTON_MAGNETIC_VOLVE_TEST11, m_mfcMagneticValveTest11);
	DDX_Control(pDX, IDC_MFCBUTTON_MAGNETIC_VOLVE_TEST12, m_mfcMagneticValveTest12);
	DDX_Control(pDX, IDC_MFCBUTTON_MAGNETIC_VOLVE_TEST13, m_mfcMagneticValveTest13);
	DDX_Control(pDX, IDC_MFCBUTTON_MAGNETIC_VOLVE_TEST14, m_mfcMagneticValveTest14);
	DDX_Control(pDX, IDC_MFCBUTTON_MAGNETIC_VOLVE_TEST15, m_mfcMagneticValveTest15);
	DDX_Control(pDX, IDC_MFCBUTTON_MAGNETIC_VOLVE_TEST16, m_mfcMagneticValveTest16);
	DDX_Control(pDX, IDC_MFCBUTTON_MAGNETIC_VOLVE_TEST17, m_mfcMagneticValveTest17);
	DDX_Control(pDX, IDC_MFCBUTTON_MAGNETIC_VOLVE_TEST18, m_mfcMagneticValveTest18);
	DDX_Control(pDX, IDC_MFCBUTTON_MAGNETIC_VOLVE_TEST19, m_mfcMagneticValveTest19);
	DDX_Control(pDX, IDC_MFCBUTTON_MAGNETIC_VOLVE_TEST20, m_mfcMagneticValveTest20);
	DDX_Control(pDX, IDC_MFCBUTTON_MAGNETIC_VOLVE_TEST21, m_mfcMagneticValveTest21);
	DDX_Control(pDX, IDC_MFCBUTTON_MAGNETIC_VOLVE_TEST22, m_mfcMagneticValveTest22);
	DDX_Control(pDX, IDC_MFCBUTTON_MAGNETIC_VOLVE_TEST23, m_mfcMagneticValveTest23);
	DDX_Control(pDX, IDC_MFCBUTTON_MAGNETIC_VOLVE_TEST24, m_mfcMagneticValveTest24);
}

BEGIN_MESSAGE_MAP(CFiberDetectControlDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON_HARDWARE_TEST, &CFiberDetectControlDlg::OnBnClickedButtonHardwareTest)
	ON_BN_CLICKED(IDC_BUTTON_NET_PARAMETERS_SETTING, &CFiberDetectControlDlg::OnBnClickedButtonNetParametersSetting)
	ON_BN_CLICKED(IDC_BUTTON_PARAMETERS_SETTING, &CFiberDetectControlDlg::OnBnClickedButtonParametersSetting)
	
	ON_BN_CLICKED(IDC_START_SERVER, &CFiberDetectControlDlg::OnBnClickedStartServer)
	ON_BN_CLICKED(IDC_CHECK_SERVER_MODE_SELECT, &CFiberDetectControlDlg::OnBnClickedCheckServerModeSelect)
	ON_BN_CLICKED(IDC_CHECK_MODE_SELECT, &CFiberDetectControlDlg::OnBnClickedCheckModeSelect)
	ON_BN_CLICKED(IDC_CHECK_SERVER_SEND, &CFiberDetectControlDlg::OnBnClickedCheckServerSend)
	ON_BN_CLICKED(IDC_CHECK_CLIENT_SEND, &CFiberDetectControlDlg::OnBnClickedCheckClientSend)
	ON_MESSAGE(ID_SHOWCTRLMESSAGE, ShowCtrlMessage)
	ON_MESSAGE(ID_SHOWPICTURE, UpdateImg)
	ON_MESSAGE(ID_OPEN_CAMERA, Setwindow)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BUTTON_START_DETECT, &CFiberDetectControlDlg::OnBnClickedButtonStartDetect)
	ON_BN_CLICKED(IDC_BUTTON_STOP_DETECT2, &CFiberDetectControlDlg::OnBnClickedButtonStopDetect2)
	ON_BN_CLICKED(IDC_BUTTON_DISPLAY_SETTING, &CFiberDetectControlDlg::OnBnClickedButtonDisplaySetting)
	ON_WM_SIZE()
	ON_BN_CLICKED(IDC_BUTTON_CAMERA1, &CFiberDetectControlDlg::OnBnClickedButtonCamera1)
	ON_BN_CLICKED(IDC_BUTTON_CAMERA2, &CFiberDetectControlDlg::OnBnClickedButtonCamera2)
	ON_BN_CLICKED(IDC_BUTTON_CAMERA3, &CFiberDetectControlDlg::OnBnClickedButtonCamera3)
	ON_BN_CLICKED(IDC_BUTTON_CAMERA4, &CFiberDetectControlDlg::OnBnClickedButtonCamera4)
	ON_BN_CLICKED(IDC_BUTTON_RECOVER_WINDOW, &CFiberDetectControlDlg::OnBnClickedButtonRecoverWindow)
	ON_BN_CLICKED(IDC_BUTTON_MODEL_SELETC, &CFiberDetectControlDlg::OnBnClickedButtonModelSeletc)
	ON_BN_CLICKED(IDC_UPLOAD_TRAIN_IMAGE, &CFiberDetectControlDlg::OnBnClickedUploadTrainImage)
	ON_BN_CLICKED(IDC_BUTIDC_UPLOAD_TRAIN_IMAGETON, &CFiberDetectControlDlg::OnBnClickedButidcUploadTrainImageton)
	ON_BN_CLICKED(IDC_SHOW_MEASURE_RESULT, &CFiberDetectControlDlg::OnBnClickedShowMeasureResult)
	ON_BN_CLICKED(IDC_BUTTON_CAMERA_PARAMETERS_SETTING, &CFiberDetectControlDlg::OnBnClickedButtonCameraParametersSetting)
	ON_BN_CLICKED(IDC_BUTTON_STATISTICS_SETTING, &CFiberDetectControlDlg::OnBnClickedButtonStatisticsSetting)
	ON_BN_CLICKED(IDC_SAVE_DETECT_RESULT, &CFiberDetectControlDlg::OnBnClickedSaveDetectResult)
	ON_BN_CLICKED(IDC_CHECK_MAGNETIC_VALVE_SHOW, &CFiberDetectControlDlg::OnBnClickedCheckMagneticValveShow)
END_MESSAGE_MAP()


// CFiberDetectControlDlg 消息处理程序

BOOL CFiberDetectControlDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中.

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标
	

	//ShowWindow(SW_MINIMIZE);
	MoveWindow(0, 0, m_nWindowWidth, m_nWindowHeight, 1);
	m_hWnd = AfxGetMainWnd()->m_hWnd;            //To get the main Window Handle;
	m_hMainWnd = m_hWnd;

	DrawWindows(0);

	//m_mfcMagneticValve9.SetWindowPos(0, 0, 0, 500, 500, NULL);
	//m_mfcMagneticValve9.MoveWindow(0, 0, 100, 100, 1);

	CMFCButton* mfcb = (CMFCButton*)GetDlgItem(IDC_START_SERVER);
	mfcb->m_bTransparent = FALSE;
	mfcb->m_bDontUseWinXPTheme = TRUE;
	mfcb->m_bDrawFocus = FALSE;
	mfcb->m_nFlatStyle = CMFCButton::BUTTONSTYLE_3D;
	//mfcb->SetFaceColor(RGB(240,240 , 240));

	m_mfcMagneticValve1.m_bTransparent = FALSE;
	m_mfcMagneticValve1.m_bDontUseWinXPTheme = TRUE;
	m_mfcMagneticValve1.m_bDrawFocus = FALSE;
	m_mfcMagneticValve1.m_nFlatStyle = CMFCButton::BUTTONSTYLE_NOBORDERS;
	m_mfcMagneticValve1.SetFaceColor(RGB(0, 255, 0));
	


	m_mfcMagneticValve2.m_bTransparent = FALSE;
	m_mfcMagneticValve2.m_bDontUseWinXPTheme = TRUE;
	m_mfcMagneticValve2.m_bDrawFocus = FALSE;
	m_mfcMagneticValve2.m_nFlatStyle = CMFCButton::BUTTONSTYLE_NOBORDERS;
	m_mfcMagneticValve2.SetFaceColor(RGB(0, 255, 0));

	m_mfcMagneticValve3.m_bTransparent = FALSE;
	m_mfcMagneticValve3.m_bDontUseWinXPTheme = TRUE;
	m_mfcMagneticValve3.m_bDrawFocus = FALSE;
	m_mfcMagneticValve3.m_nFlatStyle = CMFCButton::BUTTONSTYLE_NOBORDERS;
	m_mfcMagneticValve3.SetFaceColor(RGB(0, 255, 0));

	m_mfcMagneticValve4.m_bTransparent = FALSE;
	m_mfcMagneticValve4.m_bDontUseWinXPTheme = TRUE;
	m_mfcMagneticValve4.m_bDrawFocus = FALSE;
	m_mfcMagneticValve4.m_nFlatStyle = CMFCButton::BUTTONSTYLE_NOBORDERS;
	m_mfcMagneticValve4.SetFaceColor(RGB(0, 255, 0));

	m_mfcMagneticValve5.m_bTransparent = FALSE;
	m_mfcMagneticValve5.m_bDontUseWinXPTheme = TRUE;
	m_mfcMagneticValve5.m_bDrawFocus = FALSE;
	m_mfcMagneticValve5.m_nFlatStyle = CMFCButton::BUTTONSTYLE_NOBORDERS;
	m_mfcMagneticValve5.SetFaceColor(RGB(0, 255, 0));

	m_mfcMagneticValve6.m_bTransparent = FALSE;
	m_mfcMagneticValve6.m_bDontUseWinXPTheme = TRUE;
	m_mfcMagneticValve6.m_bDrawFocus = FALSE;
	m_mfcMagneticValve6.m_nFlatStyle = CMFCButton::BUTTONSTYLE_NOBORDERS;
	m_mfcMagneticValve6.SetFaceColor(RGB(0, 255, 0));

	m_mfcMagneticValve7.m_bTransparent = FALSE;
	m_mfcMagneticValve7.m_bDontUseWinXPTheme = TRUE;
	m_mfcMagneticValve7.m_bDrawFocus = FALSE;
	m_mfcMagneticValve7.m_nFlatStyle = CMFCButton::BUTTONSTYLE_NOBORDERS;
	m_mfcMagneticValve7.SetFaceColor(RGB(0, 255, 0));

	m_mfcMagneticValve8.m_bTransparent = FALSE;
	m_mfcMagneticValve8.m_bDontUseWinXPTheme = TRUE;
	m_mfcMagneticValve8.m_bDrawFocus = FALSE;
	m_mfcMagneticValve8.m_nFlatStyle = CMFCButton::BUTTONSTYLE_NOBORDERS;
	m_mfcMagneticValve8.SetFaceColor(RGB(0, 255, 0));

	m_mfcMagneticValve9.m_bTransparent = FALSE;
	m_mfcMagneticValve9.m_bDontUseWinXPTheme = TRUE;
	m_mfcMagneticValve9.m_bDrawFocus = FALSE;
	m_mfcMagneticValve9.m_nFlatStyle = CMFCButton::BUTTONSTYLE_NOBORDERS;
	m_mfcMagneticValve9.SetFaceColor(RGB(0, 255, 0));
	//m_mfcMagneticValve9.SetWindowPos(0, 0, 0, 500, 500, NULL);
	//m_mfcMagneticValve9.MoveWindow(0, 0, 100, 100, 1);
	COLORREF Green = RGB(0, 255, 0);
	COLORREF Blue = RGB(0, 250, 250);


	m_mfcMagneticValve10.m_bTransparent = FALSE;
	m_mfcMagneticValve10.m_bDontUseWinXPTheme = TRUE;
	m_mfcMagneticValve10.m_bDrawFocus = FALSE;
	m_mfcMagneticValve10.m_nFlatStyle = CMFCButton::BUTTONSTYLE_NOBORDERS;
	m_mfcMagneticValve10.SetFaceColor(Green);

	m_mfcMagneticValve11.m_bTransparent = FALSE;
	m_mfcMagneticValve11.m_bDontUseWinXPTheme = TRUE;
	m_mfcMagneticValve11.m_bDrawFocus = FALSE;
	m_mfcMagneticValve11.m_nFlatStyle = CMFCButton::BUTTONSTYLE_NOBORDERS;
	m_mfcMagneticValve11.SetFaceColor(RGB(0, 255, 0));

	m_mfcMagneticValve12.m_bTransparent = FALSE;
	m_mfcMagneticValve12.m_bDontUseWinXPTheme = TRUE;
	m_mfcMagneticValve12.m_bDrawFocus = FALSE;
	m_mfcMagneticValve12.m_nFlatStyle = CMFCButton::BUTTONSTYLE_NOBORDERS;
	m_mfcMagneticValve12.SetFaceColor(RGB(0, 255, 0));

	m_mfcMagneticValve13.m_bTransparent = FALSE;
	m_mfcMagneticValve13.m_bDontUseWinXPTheme = TRUE;
	m_mfcMagneticValve13.m_bDrawFocus = FALSE;
	m_mfcMagneticValve13.m_nFlatStyle = CMFCButton::BUTTONSTYLE_NOBORDERS;
	m_mfcMagneticValve13.SetFaceColor(RGB(0, 255, 0));

	m_mfcMagneticValve14.m_bTransparent = FALSE;
	m_mfcMagneticValve14.m_bDontUseWinXPTheme = TRUE;
	m_mfcMagneticValve14.m_bDrawFocus = FALSE;
	m_mfcMagneticValve14.m_nFlatStyle = CMFCButton::BUTTONSTYLE_NOBORDERS;
	m_mfcMagneticValve14.SetFaceColor(RGB(0, 255, 0));

	m_mfcMagneticValve15.m_bTransparent = FALSE;
	m_mfcMagneticValve15.m_bDontUseWinXPTheme = TRUE;
	m_mfcMagneticValve15.m_bDrawFocus = FALSE;
	m_mfcMagneticValve15.m_nFlatStyle = CMFCButton::BUTTONSTYLE_NOBORDERS;
	m_mfcMagneticValve15.SetFaceColor(RGB(0, 255, 0));

	m_mfcMagneticValve16.m_bTransparent = FALSE;
	m_mfcMagneticValve16.m_bDontUseWinXPTheme = TRUE;
	m_mfcMagneticValve16.m_bDrawFocus = FALSE;
	m_mfcMagneticValve16.m_nFlatStyle = CMFCButton::BUTTONSTYLE_NOBORDERS;
	m_mfcMagneticValve16.SetFaceColor(RGB(0, 255, 0));

	m_mfcMagneticValve17.m_bTransparent = FALSE;
	m_mfcMagneticValve17.m_bDontUseWinXPTheme = TRUE;
	m_mfcMagneticValve17.m_bDrawFocus = FALSE;
	m_mfcMagneticValve17.m_nFlatStyle = CMFCButton::BUTTONSTYLE_NOBORDERS;
	m_mfcMagneticValve17.SetFaceColor(RGB(0, 255, 0));

	m_mfcMagneticValve18.m_bTransparent = FALSE;
	m_mfcMagneticValve18.m_bDontUseWinXPTheme = TRUE;
	m_mfcMagneticValve18.m_bDrawFocus = FALSE;
	m_mfcMagneticValve18.m_nFlatStyle = CMFCButton::BUTTONSTYLE_NOBORDERS;
	m_mfcMagneticValve18.SetFaceColor(RGB(0, 255, 0));

	m_mfcMagneticValve19.m_bTransparent = FALSE;
	m_mfcMagneticValve19.m_bDontUseWinXPTheme = TRUE;
	m_mfcMagneticValve19.m_bDrawFocus = FALSE;
	m_mfcMagneticValve19.m_nFlatStyle = CMFCButton::BUTTONSTYLE_NOBORDERS;
	m_mfcMagneticValve19.SetFaceColor(RGB(0, 255, 0));

	m_mfcMagneticValve20.m_bTransparent = FALSE;
	m_mfcMagneticValve20.m_bDontUseWinXPTheme = TRUE;
	m_mfcMagneticValve20.m_bDrawFocus = FALSE;
	m_mfcMagneticValve20.m_nFlatStyle = CMFCButton::BUTTONSTYLE_NOBORDERS;
	m_mfcMagneticValve20.SetFaceColor(RGB(0, 255, 0));

	m_mfcMagneticValve21.m_bTransparent = FALSE;
	m_mfcMagneticValve21.m_bDontUseWinXPTheme = TRUE;
	m_mfcMagneticValve21.m_bDrawFocus = FALSE;
	m_mfcMagneticValve21.m_nFlatStyle = CMFCButton::BUTTONSTYLE_NOBORDERS;
	m_mfcMagneticValve21.SetFaceColor(RGB(0, 255, 0));

	m_mfcMagneticValve22.m_bTransparent = FALSE;
	m_mfcMagneticValve22.m_bDontUseWinXPTheme = TRUE;
	m_mfcMagneticValve22.m_bDrawFocus = FALSE;
	m_mfcMagneticValve22.m_nFlatStyle = CMFCButton::BUTTONSTYLE_NOBORDERS;
	m_mfcMagneticValve22.SetFaceColor(RGB(0, 255, 0));

	m_mfcMagneticValve23.m_bTransparent = FALSE;
	m_mfcMagneticValve23.m_bDontUseWinXPTheme = TRUE;
	m_mfcMagneticValve23.m_bDrawFocus = FALSE;
	m_mfcMagneticValve23.m_nFlatStyle = CMFCButton::BUTTONSTYLE_NOBORDERS;
	m_mfcMagneticValve23.SetFaceColor(RGB(0, 255, 0));

	m_mfcMagneticValve24.m_bTransparent = FALSE;
	m_mfcMagneticValve24.m_bDontUseWinXPTheme = TRUE;
	m_mfcMagneticValve24.m_bDrawFocus = FALSE;
	m_mfcMagneticValve24.m_nFlatStyle = CMFCButton::BUTTONSTYLE_NOBORDERS;
	m_mfcMagneticValve24.SetFaceColor(RGB(0, 255, 0));

	m_mfcMagneticValveTest1.m_bTransparent = FALSE;
	m_mfcMagneticValveTest1.m_bDontUseWinXPTheme = TRUE;
	m_mfcMagneticValveTest1.m_bDrawFocus = FALSE;
	m_mfcMagneticValveTest1.m_nFlatStyle = CMFCButton::BUTTONSTYLE_NOBORDERS;
	m_mfcMagneticValveTest1.SetFaceColor(Blue);

	m_mfcMagneticValveTest2.m_bTransparent = FALSE;
	m_mfcMagneticValveTest2.m_bDontUseWinXPTheme = TRUE;
	m_mfcMagneticValveTest2.m_bDrawFocus = FALSE;
	m_mfcMagneticValveTest2.m_nFlatStyle = CMFCButton::BUTTONSTYLE_NOBORDERS;
	m_mfcMagneticValveTest2.SetFaceColor(RGB(0, 250, 250));

	m_mfcMagneticValveTest3.m_bTransparent = FALSE;
	m_mfcMagneticValveTest3.m_bDontUseWinXPTheme = TRUE;
	m_mfcMagneticValveTest3.m_bDrawFocus = FALSE;
	m_mfcMagneticValveTest3.m_nFlatStyle = CMFCButton::BUTTONSTYLE_NOBORDERS;
	m_mfcMagneticValveTest3.SetFaceColor(RGB(0, 250, 250));

	m_mfcMagneticValveTest4.m_bTransparent = FALSE;
	m_mfcMagneticValveTest4.m_bDontUseWinXPTheme = TRUE;
	m_mfcMagneticValveTest4.m_bDrawFocus = FALSE;
	m_mfcMagneticValveTest4.m_nFlatStyle = CMFCButton::BUTTONSTYLE_NOBORDERS;
	m_mfcMagneticValveTest4.SetFaceColor(RGB(0, 250, 250));

	m_mfcMagneticValveTest5.m_bTransparent = FALSE;
	m_mfcMagneticValveTest5.m_bDontUseWinXPTheme = TRUE;
	m_mfcMagneticValveTest5.m_bDrawFocus = FALSE;
	m_mfcMagneticValveTest5.m_nFlatStyle = CMFCButton::BUTTONSTYLE_NOBORDERS;
	m_mfcMagneticValveTest5.SetFaceColor(RGB(0, 250, 250));

	m_mfcMagneticValveTest6.m_bTransparent = FALSE;
	m_mfcMagneticValveTest6.m_bDontUseWinXPTheme = TRUE;
	m_mfcMagneticValveTest6.m_bDrawFocus = FALSE;
	m_mfcMagneticValveTest6.m_nFlatStyle = CMFCButton::BUTTONSTYLE_NOBORDERS;
	m_mfcMagneticValveTest6.SetFaceColor(RGB(0, 250, 250));

	m_mfcMagneticValveTest7.m_bTransparent = FALSE;
	m_mfcMagneticValveTest7.m_bDontUseWinXPTheme = TRUE;
	m_mfcMagneticValveTest7.m_bDrawFocus = FALSE;
	m_mfcMagneticValveTest7.m_nFlatStyle = CMFCButton::BUTTONSTYLE_NOBORDERS;
	m_mfcMagneticValveTest7.SetFaceColor(RGB(0, 250, 250));

	m_mfcMagneticValveTest8.m_bTransparent = FALSE;
	m_mfcMagneticValveTest8.m_bDontUseWinXPTheme = TRUE;
	m_mfcMagneticValveTest8.m_bDrawFocus = FALSE;
	m_mfcMagneticValveTest8.m_nFlatStyle = CMFCButton::BUTTONSTYLE_NOBORDERS;
	m_mfcMagneticValveTest8.SetFaceColor(RGB(0, 250, 250));

	m_mfcMagneticValveTest9.m_bTransparent = FALSE;
	m_mfcMagneticValveTest9.m_bDontUseWinXPTheme = TRUE;
	m_mfcMagneticValveTest9.m_bDrawFocus = FALSE;
	m_mfcMagneticValveTest9.m_nFlatStyle = CMFCButton::BUTTONSTYLE_NOBORDERS;
	m_mfcMagneticValveTest9.SetFaceColor(RGB(0, 250, 250));

	m_mfcMagneticValveTest10.m_bTransparent = FALSE;
	m_mfcMagneticValveTest10.m_bDontUseWinXPTheme = TRUE;
	m_mfcMagneticValveTest10.m_bDrawFocus = FALSE;
	m_mfcMagneticValveTest10.m_nFlatStyle = CMFCButton::BUTTONSTYLE_NOBORDERS;
	m_mfcMagneticValveTest10.SetFaceColor(RGB(0, 250, 250));

	m_mfcMagneticValveTest11.m_bTransparent = FALSE;
	m_mfcMagneticValveTest11.m_bDontUseWinXPTheme = TRUE;
	m_mfcMagneticValveTest11.m_bDrawFocus = FALSE;
	m_mfcMagneticValveTest11.m_nFlatStyle = CMFCButton::BUTTONSTYLE_NOBORDERS;
	m_mfcMagneticValveTest11.SetFaceColor(RGB(0, 250, 250));

	m_mfcMagneticValveTest12.m_bTransparent = FALSE;
	m_mfcMagneticValveTest12.m_bDontUseWinXPTheme = TRUE;
	m_mfcMagneticValveTest12.m_bDrawFocus = FALSE;
	m_mfcMagneticValveTest12.m_nFlatStyle = CMFCButton::BUTTONSTYLE_NOBORDERS;
	m_mfcMagneticValveTest12.SetFaceColor(RGB(0, 250, 250));

	m_mfcMagneticValveTest13.m_bTransparent = FALSE;
	m_mfcMagneticValveTest13.m_bDontUseWinXPTheme = TRUE;
	m_mfcMagneticValveTest13.m_bDrawFocus = FALSE;
	m_mfcMagneticValveTest13.m_nFlatStyle = CMFCButton::BUTTONSTYLE_NOBORDERS;
	m_mfcMagneticValveTest13.SetFaceColor(RGB(0, 250, 250));

	m_mfcMagneticValveTest14.m_bTransparent = FALSE;
	m_mfcMagneticValveTest14.m_bDontUseWinXPTheme = TRUE;
	m_mfcMagneticValveTest14.m_bDrawFocus = FALSE;
	m_mfcMagneticValveTest14.m_nFlatStyle = CMFCButton::BUTTONSTYLE_NOBORDERS;
	m_mfcMagneticValveTest14.SetFaceColor(RGB(0, 250, 250));

	m_mfcMagneticValveTest15.m_bTransparent = FALSE;
	m_mfcMagneticValveTest15.m_bDontUseWinXPTheme = TRUE;
	m_mfcMagneticValveTest15.m_bDrawFocus = FALSE;
	m_mfcMagneticValveTest15.m_nFlatStyle = CMFCButton::BUTTONSTYLE_NOBORDERS;
	m_mfcMagneticValveTest15.SetFaceColor(RGB(0, 250, 250));

	m_mfcMagneticValveTest16.m_bTransparent = FALSE;
	m_mfcMagneticValveTest16.m_bDontUseWinXPTheme = TRUE;
	m_mfcMagneticValveTest16.m_bDrawFocus = FALSE;
	m_mfcMagneticValveTest16.m_nFlatStyle = CMFCButton::BUTTONSTYLE_NOBORDERS;
	m_mfcMagneticValveTest16.SetFaceColor(RGB(0, 250, 250));

	m_mfcMagneticValveTest17.m_bTransparent = FALSE;
	m_mfcMagneticValveTest17.m_bDontUseWinXPTheme = TRUE;
	m_mfcMagneticValveTest17.m_bDrawFocus = FALSE;
	m_mfcMagneticValveTest17.m_nFlatStyle = CMFCButton::BUTTONSTYLE_NOBORDERS;
	m_mfcMagneticValveTest17.SetFaceColor(RGB(0, 250, 250));

	m_mfcMagneticValveTest18.m_bTransparent = FALSE;
	m_mfcMagneticValveTest18.m_bDontUseWinXPTheme = TRUE;
	m_mfcMagneticValveTest18.m_bDrawFocus = FALSE;
	m_mfcMagneticValveTest18.m_nFlatStyle = CMFCButton::BUTTONSTYLE_NOBORDERS;
	m_mfcMagneticValveTest18.SetFaceColor(RGB(0, 250, 250));

	m_mfcMagneticValveTest19.m_bTransparent = FALSE;
	m_mfcMagneticValveTest19.m_bDontUseWinXPTheme = TRUE;
	m_mfcMagneticValveTest19.m_bDrawFocus = FALSE;
	m_mfcMagneticValveTest19.m_nFlatStyle = CMFCButton::BUTTONSTYLE_NOBORDERS;
	m_mfcMagneticValveTest19.SetFaceColor(RGB(0, 250, 250));

	m_mfcMagneticValveTest20.m_bTransparent = FALSE;
	m_mfcMagneticValveTest20.m_bDontUseWinXPTheme = TRUE;
	m_mfcMagneticValveTest20.m_bDrawFocus = FALSE;
	m_mfcMagneticValveTest20.m_nFlatStyle = CMFCButton::BUTTONSTYLE_NOBORDERS;
	m_mfcMagneticValveTest20.SetFaceColor(RGB(0, 250, 250));

	m_mfcMagneticValveTest21.m_bTransparent = FALSE;
	m_mfcMagneticValveTest21.m_bDontUseWinXPTheme = TRUE;
	m_mfcMagneticValveTest21.m_bDrawFocus = FALSE;
	m_mfcMagneticValveTest21.m_nFlatStyle = CMFCButton::BUTTONSTYLE_NOBORDERS;
	m_mfcMagneticValveTest21.SetFaceColor(RGB(0, 250, 250));

	m_mfcMagneticValveTest22.m_bTransparent = FALSE;
	m_mfcMagneticValveTest22.m_bDontUseWinXPTheme = TRUE;
	m_mfcMagneticValveTest22.m_bDrawFocus = FALSE;
	m_mfcMagneticValveTest22.m_nFlatStyle = CMFCButton::BUTTONSTYLE_NOBORDERS;
	m_mfcMagneticValveTest22.SetFaceColor(RGB(0, 250, 250));

	m_mfcMagneticValveTest23.m_bTransparent = FALSE;
	m_mfcMagneticValveTest23.m_bDontUseWinXPTheme = TRUE;
	m_mfcMagneticValveTest23.m_bDrawFocus = FALSE;
	m_mfcMagneticValveTest23.m_nFlatStyle = CMFCButton::BUTTONSTYLE_NOBORDERS;
	m_mfcMagneticValveTest23.SetFaceColor(RGB(0, 250, 250));

	m_mfcMagneticValveTest24.m_bTransparent = FALSE;
	m_mfcMagneticValveTest24.m_bDontUseWinXPTheme = TRUE;
	m_mfcMagneticValveTest24.m_bDrawFocus = FALSE;
	m_mfcMagneticValveTest24.m_nFlatStyle = CMFCButton::BUTTONSTYLE_NOBORDERS;
	m_mfcMagneticValveTest24.SetFaceColor(RGB(0, 250, 250));

	m_mfcMagneticValve[0] = new CMFCButton;
	m_mfcMagneticValve[0] = &m_mfcMagneticValveTest1;

	m_mfcMagneticValve[1] = new CMFCButton;
	m_mfcMagneticValve[1] = &m_mfcMagneticValveTest2;

	m_mfcMagneticValve[2] = new CMFCButton;
	m_mfcMagneticValve[2] = &m_mfcMagneticValveTest3;

	m_mfcMagneticValve[3] = new CMFCButton;
	m_mfcMagneticValve[3] = &m_mfcMagneticValveTest4;

	m_mfcMagneticValve[4] = new CMFCButton;
	m_mfcMagneticValve[4] = &m_mfcMagneticValveTest5;

	m_mfcMagneticValve[5] = new CMFCButton;
	m_mfcMagneticValve[5] = &m_mfcMagneticValveTest6;

	m_mfcMagneticValve[6] = new CMFCButton;
	m_mfcMagneticValve[6] = &m_mfcMagneticValveTest7;

	m_mfcMagneticValve[7] = new CMFCButton;
	m_mfcMagneticValve[7] = &m_mfcMagneticValveTest8;

	m_mfcMagneticValve[8] = new CMFCButton;
	m_mfcMagneticValve[8] = &m_mfcMagneticValveTest9;

	m_mfcMagneticValve[9] = new CMFCButton;
	m_mfcMagneticValve[9] = &m_mfcMagneticValveTest10;

	m_mfcMagneticValve[10] = new CMFCButton;
	m_mfcMagneticValve[10] = &m_mfcMagneticValveTest11;

	m_mfcMagneticValve[11] = new CMFCButton;
	m_mfcMagneticValve[11] = &m_mfcMagneticValveTest12;

	m_mfcMagneticValve[12] = new CMFCButton;
	m_mfcMagneticValve[12] = &m_mfcMagneticValveTest13;

	m_mfcMagneticValve[13] = new CMFCButton;
	m_mfcMagneticValve[13] = &m_mfcMagneticValveTest14;

	m_mfcMagneticValve[14] = new CMFCButton;
	m_mfcMagneticValve[14] = &m_mfcMagneticValveTest15;

	m_mfcMagneticValve[15] = new CMFCButton;
	m_mfcMagneticValve[15] = &m_mfcMagneticValveTest16;

	m_mfcMagneticValve[16] = new CMFCButton;
	m_mfcMagneticValve[16] = &m_mfcMagneticValveTest17;

	m_mfcMagneticValve[17] = new CMFCButton;
	m_mfcMagneticValve[17] = &m_mfcMagneticValveTest18;

	m_mfcMagneticValve[18] = new CMFCButton;
	m_mfcMagneticValve[18] = &m_mfcMagneticValveTest19;

	m_mfcMagneticValve[19] = new CMFCButton;
	m_mfcMagneticValve[19] = &m_mfcMagneticValveTest20;

	m_mfcMagneticValve[20] = new CMFCButton;
	m_mfcMagneticValve[20] = &m_mfcMagneticValveTest21;

	m_mfcMagneticValve[21] = new CMFCButton;
	m_mfcMagneticValve[21] = &m_mfcMagneticValveTest22;

	m_mfcMagneticValve[22] = new CMFCButton;
	m_mfcMagneticValve[22] = &m_mfcMagneticValveTest23;

	m_mfcMagneticValve[23] = new CMFCButton;
	m_mfcMagneticValve[23] = &m_mfcMagneticValveTest24;

	for (int i = 0; i < MAGNETIC_VALVE_SUM; i++)  //MAGNETIC_VALVE_SUM
	{
		valve_driver[i].dura = 0;
	}

	m_bServerMode = true;
	m_bClientMode = false;
	m_checkServerModeSelect.SetCheck(m_bServerMode);
	m_checkClientMode.SetCheck(m_bClientMode);

	m_bServerSendMode = true;
	m_bClientSendMode = false;
	communicator_->m_bServerSendMode = m_bServerSendMode;
	communicator_->m_bClientSendMode = m_bClientSendMode;
	m_checkServerSend.SetCheck(m_bServerSendMode);
	m_checkClientSend.SetCheck(m_bClientSendMode);

	m_pwnd = GetDlgItem(IDC_STATIC_IMAGE_SHOW);//IDC_SHOWIMAGE
	//m_pwnd = GetDlgItem(IDC_SHOWIMAGE);//IDC_SHOWIMAGE
	//pwnd->MoveWindow(35,30,352,288);
	m_pDC = m_pwnd->GetDC();
	//pDC =GetDC();
	m_hDC = m_pDC->GetSafeHdc();
	m_pwnd->GetClientRect(&m_ImageRect);
	m_pDC->TextOut(20, 20, "写字");
	m_nRectWidth = m_ImageRect.Width();
	m_nRectHeight = m_ImageRect.Height();

	CString cs_str;
	string str_str;
	str_str = cs_str;
	Point P1, P2, P0, P3;
	Point pRect1, pRect2;
	POINT  pTopLeft, pRightBottom;
	pTopLeft.x = 5000; pTopLeft.y = 5000; pRightBottom.x = 0; pRightBottom.y = 0;
	CString strText = "经纬异纤检测";
	CRect DrawRect(100, 100, 300, 100);
	ShowText(m_pwnd, strText, DrawRect);
	for (int i = 0; i < SHOWTEXTLENGTH; i++)
	{
		memset(m_strShowText[i], 0, 255);
	}
	ReadIni();
	
//	Sleep(200);
//	run_tcp_server("192.168.1.11", 9713);
//	Sleep(200);

	// TODO:  在此添加额外的初始化代码
	//run_tcp_img_show_server("192.168.1.11", 9005);//mode 0
	OnBnClickedStartServer();  //Start a server to connect the AI-BOX;
	//SetTimer(1, 2, nullptr);
	GetDlgItem(IDC_CHECK_SERVER_MODE_SELECT)->EnableWindow(FALSE);
	GetDlgItem(IDC_CHECK_MODE_SELECT)->EnableWindow(FALSE);
	GetDlgItem(IDC_CHECK_SERVER_SEND)->EnableWindow(FALSE);
	GetDlgItem(IDC_CHECK_CLIENT_SEND)->EnableWindow(FALSE);
	((CButton*)GetDlgItem(IDC_CHECK_MAGNETIC_VALVE_SHOW))->SetCheck(false);


	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}



void CFiberDetectControlDlg::DrawWindows(int nCameraIndex)
{
	m_bResizeEnable = true;


	CRect	WindowRect;
	GetClientRect(&WindowRect);
	m_nWindowWidth = WindowRect.Width();
	m_nWindowHeight = WindowRect.Height();
	int nButtonNum = 13;
	int nButtonGap = (m_nWindowHeight - nButtonNum * m_nButtonHeight - m_nButtonUpleftX) / (nButtonNum - 1);

	GetDlgItem(IDC_BUTTON_MODEL_SELETC)->MoveWindow(m_nButtonUpleftX, m_nButtonUpleftY + 0 * (m_nButtonHeight + nButtonGap), m_nButtonWidth, m_nButtonHeight);
	GetDlgItem(IDC_BUTTON_START_DETECT)->MoveWindow(m_nButtonUpleftX, m_nButtonUpleftY + 1 * (m_nButtonHeight + nButtonGap), m_nButtonWidth, m_nButtonHeight);
	GetDlgItem(IDC_BUTTON_STOP_DETECT2)->MoveWindow(m_nButtonUpleftX, m_nButtonUpleftY + 2 * (m_nButtonHeight + nButtonGap), m_nButtonWidth, m_nButtonHeight);
	GetDlgItem(IDC_BUTTON_NET_PARAMETERS_SETTING)->MoveWindow(m_nButtonUpleftX, m_nButtonUpleftY + 3 * (m_nButtonHeight + nButtonGap), m_nButtonWidth, m_nButtonHeight);
	GetDlgItem(IDC_BUTTON_CAMERA_PARAMETERS_SETTING)->MoveWindow(m_nButtonUpleftX, m_nButtonUpleftY + 4 * (m_nButtonHeight + nButtonGap), m_nButtonWidth, m_nButtonHeight);
	GetDlgItem(IDC_START_SERVER)->MoveWindow(m_nButtonUpleftX, m_nButtonUpleftY + 5 * (m_nButtonHeight + nButtonGap), m_nButtonWidth, m_nButtonHeight);
	GetDlgItem(IDC_CHECK_MODE_SELECT)->MoveWindow(m_nButtonUpleftX, m_nButtonUpleftY + 6 * (m_nButtonHeight + nButtonGap), m_nButtonWidth, m_nButtonHeight);
	GetDlgItem(IDC_CHECK_SERVER_MODE_SELECT)->MoveWindow(m_nButtonUpleftX, m_nButtonUpleftY + 7 * (m_nButtonHeight + nButtonGap), m_nButtonWidth, m_nButtonHeight);
	GetDlgItem(IDC_CHECK_SERVER_SEND)->MoveWindow(m_nButtonUpleftX, m_nButtonUpleftY + 8 * (m_nButtonHeight + nButtonGap), m_nButtonWidth, m_nButtonHeight);
	GetDlgItem(IDC_CHECK_CLIENT_SEND)->MoveWindow(m_nButtonUpleftX, m_nButtonUpleftY + 9 * (m_nButtonHeight + nButtonGap), m_nButtonWidth, m_nButtonHeight);
	GetDlgItem(IDC_UPLOAD_TRAIN_IMAGE)->MoveWindow(m_nButtonUpleftX, m_nButtonUpleftY + 10 * (m_nButtonHeight + nButtonGap), m_nButtonWidth, m_nButtonHeight);
	GetDlgItem(IDC_CHECK_MAGNETIC_VALVE_SHOW)->MoveWindow(m_nButtonUpleftX, m_nButtonUpleftY + 10 * (m_nButtonHeight + nButtonGap), m_nButtonWidth, m_nButtonHeight);
	GetDlgItem(IDC_CHECK_MAGNETIC_VALVE_SHOW)->ShowWindow(FALSE);


	GetDlgItem(IDC_BUTTON_CAMERA1)->MoveWindow(m_nWindowWidth - m_nButtonUpleftX - m_nButtonWidth, m_nButtonUpleftY + 0 * (m_nButtonHeight + nButtonGap), m_nButtonWidth, m_nButtonHeight);
	GetDlgItem(IDC_BUTTON_CAMERA2)->MoveWindow(m_nWindowWidth - m_nButtonUpleftX - m_nButtonWidth, m_nButtonUpleftY + 1 * (m_nButtonHeight +  nButtonGap), m_nButtonWidth, m_nButtonHeight);
	GetDlgItem(IDC_BUTTON_CAMERA3)->MoveWindow(m_nWindowWidth - m_nButtonUpleftX - m_nButtonWidth, m_nButtonUpleftY + 2 * (m_nButtonHeight +  nButtonGap), m_nButtonWidth, m_nButtonHeight);
	GetDlgItem(IDC_BUTTON_CAMERA4)->MoveWindow(m_nWindowWidth - m_nButtonUpleftX - m_nButtonWidth, m_nButtonUpleftY + 3 * (m_nButtonHeight +  nButtonGap), m_nButtonWidth, m_nButtonHeight);
	GetDlgItem(IDC_BUTTON_RECOVER_WINDOW)->MoveWindow(m_nWindowWidth - m_nButtonUpleftX - m_nButtonWidth, m_nButtonUpleftY + 4 * (m_nButtonHeight + nButtonGap), m_nButtonWidth, m_nButtonHeight);
	GetDlgItem(IDC_BUTIDC_UPLOAD_TRAIN_IMAGETON)->MoveWindow(m_nWindowWidth - m_nButtonUpleftX - m_nButtonWidth, m_nButtonUpleftY + 5 * (m_nButtonHeight +  nButtonGap), m_nButtonWidth, m_nButtonHeight);
	GetDlgItem(IDC_SHOW_MEASURE_RESULT)->MoveWindow(m_nWindowWidth - m_nButtonUpleftX - m_nButtonWidth, m_nButtonUpleftY + 6 * (m_nButtonHeight + nButtonGap), m_nButtonWidth, m_nButtonHeight);
	GetDlgItem(IDC_SAVE_DETECT_RESULT)->MoveWindow(m_nWindowWidth - m_nButtonUpleftX - m_nButtonWidth, m_nButtonUpleftY + 7 * (m_nButtonHeight + nButtonGap), m_nButtonWidth, m_nButtonHeight);
	//IDC_SAVE_DETECT_RESULT

	int nButtonHeight = 30;
	int nGap = 10;
	int nButtonBegin = m_nButtonUpleftX + m_nButtonWidth / 2;
	int nButtonWidth = ((m_nWindowWidth - (m_nButtonUpleftX + m_nButtonWidth / 2) * 2) - 23 * nGap) / 24;
	GetDlgItem(IDC_MFCBUTTON_MAGNETIC_VOLVE1)->MoveWindow(nButtonBegin, m_nWindowHeight - (int)(2.5 *(float(nButtonHeight))), nButtonWidth, nButtonHeight);
	GetDlgItem(IDC_MFCBUTTON_MAGNETIC_VOLVE2)->MoveWindow(nButtonBegin + 1 * (nButtonWidth + nGap), m_nWindowHeight -(int)( 2.5 *(float( nButtonHeight))), nButtonWidth, nButtonHeight);
	GetDlgItem(IDC_MFCBUTTON_MAGNETIC_VOLVE3)->MoveWindow(nButtonBegin + 2 * (nButtonWidth + nGap), m_nWindowHeight - (int)(2.5 *(float(nButtonHeight))), nButtonWidth, nButtonHeight);
	GetDlgItem(IDC_MFCBUTTON_MAGNETIC_VOLVE4)->MoveWindow(nButtonBegin + 3 * (nButtonWidth + nGap), m_nWindowHeight - (int)(2.5 *(float(nButtonHeight))), nButtonWidth, nButtonHeight);
	GetDlgItem(IDC_MFCBUTTON_MAGNETIC_VOLVE5)->MoveWindow(nButtonBegin + 4 * (nButtonWidth + nGap), m_nWindowHeight - (int)(2.5 *(float(nButtonHeight))), nButtonWidth, nButtonHeight);
	GetDlgItem(IDC_MFCBUTTON_MAGNETIC_VOLVE6)->MoveWindow(nButtonBegin + 5 * (nButtonWidth + nGap), m_nWindowHeight - (int)(2.5 *(float(nButtonHeight))), nButtonWidth, nButtonHeight);
	GetDlgItem(IDC_MFCBUTTON_MAGNETIC_VOLVE7)->MoveWindow(nButtonBegin + 6 * (nButtonWidth + nGap), m_nWindowHeight - (int)(2.5 *(float(nButtonHeight))), nButtonWidth, nButtonHeight);
	GetDlgItem(IDC_MFCBUTTON_MAGNETIC_VOLVE8)->MoveWindow(nButtonBegin + 7 * (nButtonWidth + nGap), m_nWindowHeight - (int)(2.5 *(float(nButtonHeight))), nButtonWidth, nButtonHeight);
	GetDlgItem(IDC_MFCBUTTON_MAGNETIC_VOLVE9)->MoveWindow(nButtonBegin + 8 * (nButtonWidth + nGap), m_nWindowHeight - (int)(2.5 *(float(nButtonHeight))), nButtonWidth, nButtonHeight);
	GetDlgItem(IDC_MFCBUTTON_MAGNETIC_VOLVE10)->MoveWindow(nButtonBegin + 9 * (nButtonWidth + nGap), m_nWindowHeight - (int)(2.5 *(float(nButtonHeight))), nButtonWidth, nButtonHeight);
	GetDlgItem(IDC_MFCBUTTON_MAGNETIC_VOLVE11)->MoveWindow(nButtonBegin + 10 * (nButtonWidth + nGap), m_nWindowHeight - (int)(2.5 *(float(nButtonHeight))), nButtonWidth, nButtonHeight);
	GetDlgItem(IDC_MFCBUTTON_MAGNETIC_VOLVE12)->MoveWindow(nButtonBegin + 11 * (nButtonWidth + nGap), m_nWindowHeight - (int)(2.5 *(float(nButtonHeight))), nButtonWidth, nButtonHeight);
	GetDlgItem(IDC_MFCBUTTON_MAGNETIC_VOLVE13)->MoveWindow(nButtonBegin + 12 * (nButtonWidth + nGap), m_nWindowHeight - (int)(2.5 *(float(nButtonHeight))), nButtonWidth, nButtonHeight);
	GetDlgItem(IDC_MFCBUTTON_MAGNETIC_VOLVE14)->MoveWindow(nButtonBegin + 13 * (nButtonWidth + nGap), m_nWindowHeight - (int)(2.5 *(float(nButtonHeight))), nButtonWidth, nButtonHeight);
	GetDlgItem(IDC_MFCBUTTON_MAGNETIC_VOLVE15)->MoveWindow(nButtonBegin + 14 * (nButtonWidth + nGap), m_nWindowHeight - (int)(2.5 *(float(nButtonHeight))), nButtonWidth, nButtonHeight);
	GetDlgItem(IDC_MFCBUTTON_MAGNETIC_VOLVE16)->MoveWindow(nButtonBegin + 15 * (nButtonWidth + nGap), m_nWindowHeight - (int)(2.5 *(float(nButtonHeight))), nButtonWidth, nButtonHeight);
	GetDlgItem(IDC_MFCBUTTON_MAGNETIC_VOLVE17)->MoveWindow(nButtonBegin + 16 * (nButtonWidth + nGap), m_nWindowHeight - (int)(2.5 *(float(nButtonHeight))), nButtonWidth, nButtonHeight);
	GetDlgItem(IDC_MFCBUTTON_MAGNETIC_VOLVE18)->MoveWindow(nButtonBegin + 17 * (nButtonWidth + nGap), m_nWindowHeight - (int)(2.5 *(float(nButtonHeight))), nButtonWidth, nButtonHeight);
	GetDlgItem(IDC_MFCBUTTON_MAGNETIC_VOLVE19)->MoveWindow(nButtonBegin + 18 * (nButtonWidth + nGap), m_nWindowHeight - (int)(2.5 *(float(nButtonHeight))), nButtonWidth, nButtonHeight);
	GetDlgItem(IDC_MFCBUTTON_MAGNETIC_VOLVE20)->MoveWindow(nButtonBegin + 19 * (nButtonWidth + nGap), m_nWindowHeight - (int)(2.5 *(float(nButtonHeight))), nButtonWidth, nButtonHeight);
	GetDlgItem(IDC_MFCBUTTON_MAGNETIC_VOLVE21)->MoveWindow(nButtonBegin + 20 * (nButtonWidth + nGap), m_nWindowHeight - (int)(2.5 *(float(nButtonHeight))), nButtonWidth, nButtonHeight);
	GetDlgItem(IDC_MFCBUTTON_MAGNETIC_VOLVE22)->MoveWindow(nButtonBegin + 21 * (nButtonWidth + nGap), m_nWindowHeight - (int)(2.5 *(float(nButtonHeight))), nButtonWidth, nButtonHeight);
	GetDlgItem(IDC_MFCBUTTON_MAGNETIC_VOLVE23)->MoveWindow(nButtonBegin + 22 * (nButtonWidth + nGap), m_nWindowHeight - (int)(2.5 *(float(nButtonHeight))), nButtonWidth, nButtonHeight);
	GetDlgItem(IDC_MFCBUTTON_MAGNETIC_VOLVE24)->MoveWindow(nButtonBegin + 23 * (nButtonWidth + nGap), m_nWindowHeight - (int)(2.5 *(float(nButtonHeight))), nButtonWidth, nButtonHeight);

	GetDlgItem(IDC_MFCBUTTON_MAGNETIC_VOLVE_TEST1)->MoveWindow(nButtonBegin + 0 * (nButtonWidth + nGap), m_nWindowHeight - (int)(1.2 *(float(nButtonHeight))), nButtonWidth, nButtonHeight);
	GetDlgItem(IDC_MFCBUTTON_MAGNETIC_VOLVE_TEST2)->MoveWindow(nButtonBegin + 1 * (nButtonWidth + nGap), m_nWindowHeight - (int)(1.2 *(float(nButtonHeight))), nButtonWidth, nButtonHeight);
	GetDlgItem(IDC_MFCBUTTON_MAGNETIC_VOLVE_TEST3)->MoveWindow(nButtonBegin + 2 * (nButtonWidth + nGap), m_nWindowHeight - (int)(1.2 *(float(nButtonHeight))), nButtonWidth, nButtonHeight);
	GetDlgItem(IDC_MFCBUTTON_MAGNETIC_VOLVE_TEST4)->MoveWindow(nButtonBegin + 3 * (nButtonWidth + nGap), m_nWindowHeight - (int)(1.2 *(float(nButtonHeight))), nButtonWidth, nButtonHeight);
	GetDlgItem(IDC_MFCBUTTON_MAGNETIC_VOLVE_TEST5)->MoveWindow(nButtonBegin + 4 * (nButtonWidth + nGap), m_nWindowHeight - (int)(1.2 *(float(nButtonHeight))), nButtonWidth, nButtonHeight);
	GetDlgItem(IDC_MFCBUTTON_MAGNETIC_VOLVE_TEST6)->MoveWindow(nButtonBegin + 5 * (nButtonWidth + nGap), m_nWindowHeight - (int)(1.2 *(float(nButtonHeight))), nButtonWidth, nButtonHeight);
	GetDlgItem(IDC_MFCBUTTON_MAGNETIC_VOLVE_TEST7)->MoveWindow(nButtonBegin + 6 * (nButtonWidth + nGap), m_nWindowHeight - (int)(1.2 *(float(nButtonHeight))), nButtonWidth, nButtonHeight);
	GetDlgItem(IDC_MFCBUTTON_MAGNETIC_VOLVE_TEST8)->MoveWindow(nButtonBegin + 7 * (nButtonWidth + nGap), m_nWindowHeight - (int)(1.2 *(float(nButtonHeight))), nButtonWidth, nButtonHeight);
	GetDlgItem(IDC_MFCBUTTON_MAGNETIC_VOLVE_TEST9)->MoveWindow(nButtonBegin + 8 * (nButtonWidth + nGap), m_nWindowHeight - (int)(1.2 *(float(nButtonHeight))), nButtonWidth, nButtonHeight);
	GetDlgItem(IDC_MFCBUTTON_MAGNETIC_VOLVE_TEST10)->MoveWindow(nButtonBegin + 9 * (nButtonWidth + nGap), m_nWindowHeight - (int)(1.2 *(float(nButtonHeight))), nButtonWidth, nButtonHeight);
	GetDlgItem(IDC_MFCBUTTON_MAGNETIC_VOLVE_TEST11)->MoveWindow(nButtonBegin + 10 * (nButtonWidth + nGap), m_nWindowHeight - (int)(1.2 *(float(nButtonHeight))), nButtonWidth, nButtonHeight);
	GetDlgItem(IDC_MFCBUTTON_MAGNETIC_VOLVE_TEST12)->MoveWindow(nButtonBegin + 11 * (nButtonWidth + nGap), m_nWindowHeight - (int)(1.2 *(float(nButtonHeight))), nButtonWidth, nButtonHeight);
	GetDlgItem(IDC_MFCBUTTON_MAGNETIC_VOLVE_TEST13)->MoveWindow(nButtonBegin + 12 * (nButtonWidth + nGap), m_nWindowHeight - (int)(1.2 *(float(nButtonHeight))), nButtonWidth, nButtonHeight);
	GetDlgItem(IDC_MFCBUTTON_MAGNETIC_VOLVE_TEST14)->MoveWindow(nButtonBegin + 13 * (nButtonWidth + nGap), m_nWindowHeight - (int)(1.2 *(float(nButtonHeight))), nButtonWidth, nButtonHeight);
	GetDlgItem(IDC_MFCBUTTON_MAGNETIC_VOLVE_TEST15)->MoveWindow(nButtonBegin + 14 * (nButtonWidth + nGap), m_nWindowHeight - (int)(1.2 *(float(nButtonHeight))), nButtonWidth, nButtonHeight);
	GetDlgItem(IDC_MFCBUTTON_MAGNETIC_VOLVE_TEST16)->MoveWindow(nButtonBegin + 15 * (nButtonWidth + nGap), m_nWindowHeight - (int)(1.2 *(float(nButtonHeight))), nButtonWidth, nButtonHeight);
	GetDlgItem(IDC_MFCBUTTON_MAGNETIC_VOLVE_TEST17)->MoveWindow(nButtonBegin + 16 * (nButtonWidth + nGap), m_nWindowHeight - (int)(1.2 *(float(nButtonHeight))), nButtonWidth, nButtonHeight);
	GetDlgItem(IDC_MFCBUTTON_MAGNETIC_VOLVE_TEST18)->MoveWindow(nButtonBegin + 17 * (nButtonWidth + nGap), m_nWindowHeight - (int)(1.2 *(float(nButtonHeight))), nButtonWidth, nButtonHeight);
	GetDlgItem(IDC_MFCBUTTON_MAGNETIC_VOLVE_TEST19)->MoveWindow(nButtonBegin + 18 * (nButtonWidth + nGap), m_nWindowHeight - (int)(1.2 *(float(nButtonHeight))), nButtonWidth, nButtonHeight);
	GetDlgItem(IDC_MFCBUTTON_MAGNETIC_VOLVE_TEST20)->MoveWindow(nButtonBegin + 19 * (nButtonWidth + nGap), m_nWindowHeight - (int)(1.2 *(float(nButtonHeight))), nButtonWidth, nButtonHeight);
	GetDlgItem(IDC_MFCBUTTON_MAGNETIC_VOLVE_TEST21)->MoveWindow(nButtonBegin + 20 * (nButtonWidth + nGap), m_nWindowHeight - (int)(1.2 *(float(nButtonHeight))), nButtonWidth, nButtonHeight);
	GetDlgItem(IDC_MFCBUTTON_MAGNETIC_VOLVE_TEST22)->MoveWindow(nButtonBegin + 21 * (nButtonWidth + nGap), m_nWindowHeight - (int)(1.2 *(float(nButtonHeight))), nButtonWidth, nButtonHeight);
	GetDlgItem(IDC_MFCBUTTON_MAGNETIC_VOLVE_TEST23)->MoveWindow(nButtonBegin + 22 * (nButtonWidth + nGap), m_nWindowHeight - (int)(1.2 *(float(nButtonHeight))), nButtonWidth, nButtonHeight);
	GetDlgItem(IDC_MFCBUTTON_MAGNETIC_VOLVE_TEST24)->MoveWindow(nButtonBegin + 23 * (nButtonWidth + nGap), m_nWindowHeight - (int)(1.2 *(float(nButtonHeight))), nButtonWidth, nButtonHeight);
	GetDlgItem(IDC_MFCBUTTON_MAGNETIC_VOLVE_TEST1)->ShowWindow(false);
	GetDlgItem(IDC_MFCBUTTON_MAGNETIC_VOLVE_TEST2)->ShowWindow(false);
	GetDlgItem(IDC_MFCBUTTON_MAGNETIC_VOLVE_TEST3)->ShowWindow(false);
	GetDlgItem(IDC_MFCBUTTON_MAGNETIC_VOLVE_TEST4)->ShowWindow(false);
	GetDlgItem(IDC_MFCBUTTON_MAGNETIC_VOLVE_TEST5)->ShowWindow(false);
	GetDlgItem(IDC_MFCBUTTON_MAGNETIC_VOLVE_TEST6)->ShowWindow(false);
	GetDlgItem(IDC_MFCBUTTON_MAGNETIC_VOLVE_TEST7)->ShowWindow(false);
	GetDlgItem(IDC_MFCBUTTON_MAGNETIC_VOLVE_TEST8)->ShowWindow(false);
	GetDlgItem(IDC_MFCBUTTON_MAGNETIC_VOLVE_TEST9)->ShowWindow(false);
	GetDlgItem(IDC_MFCBUTTON_MAGNETIC_VOLVE_TEST10)->ShowWindow(false);
	GetDlgItem(IDC_MFCBUTTON_MAGNETIC_VOLVE_TEST11)->ShowWindow(false);
	GetDlgItem(IDC_MFCBUTTON_MAGNETIC_VOLVE_TEST12)->ShowWindow(false);
	GetDlgItem(IDC_MFCBUTTON_MAGNETIC_VOLVE_TEST13)->ShowWindow(false);
	GetDlgItem(IDC_MFCBUTTON_MAGNETIC_VOLVE_TEST14)->ShowWindow(false);
	GetDlgItem(IDC_MFCBUTTON_MAGNETIC_VOLVE_TEST15)->ShowWindow(false);
	GetDlgItem(IDC_MFCBUTTON_MAGNETIC_VOLVE_TEST16)->ShowWindow(false);
	GetDlgItem(IDC_MFCBUTTON_MAGNETIC_VOLVE_TEST17)->ShowWindow(false);
	GetDlgItem(IDC_MFCBUTTON_MAGNETIC_VOLVE_TEST18)->ShowWindow(false);
	GetDlgItem(IDC_MFCBUTTON_MAGNETIC_VOLVE_TEST19)->ShowWindow(false);
	GetDlgItem(IDC_MFCBUTTON_MAGNETIC_VOLVE_TEST20)->ShowWindow(false);
	GetDlgItem(IDC_MFCBUTTON_MAGNETIC_VOLVE_TEST21)->ShowWindow(false);
	GetDlgItem(IDC_MFCBUTTON_MAGNETIC_VOLVE_TEST22)->ShowWindow(false);
	GetDlgItem(IDC_MFCBUTTON_MAGNETIC_VOLVE_TEST23)->ShowWindow(false);
	GetDlgItem(IDC_MFCBUTTON_MAGNETIC_VOLVE_TEST24)->ShowWindow(false);


	nButtonGap = (m_nWindowWidth - (nButtonBegin + 3 * (nButtonWidth + nGap)) * 2 - 5 * m_nButtonWidth) / 4;
	nButtonBegin = nButtonBegin + 3 * (nButtonWidth + nGap);
	GetDlgItem(IDC_BUTTON_HARDWARE_TEST)->MoveWindow(nButtonBegin, m_nWindowHeight - 3 * nButtonHeight - m_nButtonHeight, m_nButtonWidth, m_nButtonHeight);
	GetDlgItem(IDC_BUTTON_PARAMETERS_SETTING)->MoveWindow(nButtonBegin + 1 * (nButtonGap + m_nButtonWidth), m_nWindowHeight - 3 * nButtonHeight - m_nButtonHeight, m_nButtonWidth, m_nButtonHeight);
	GetDlgItem(IDC_BUTTON_DISPLAY_SETTING)->MoveWindow(nButtonBegin + 2 * (nButtonGap + m_nButtonWidth), m_nWindowHeight - 3 * nButtonHeight - m_nButtonHeight, m_nButtonWidth, m_nButtonHeight);
	GetDlgItem(IDC_BUTTON_STATISTICS_SETTING)->MoveWindow(nButtonBegin + 3 * (nButtonGap + m_nButtonWidth), m_nWindowHeight - 3 * nButtonHeight - m_nButtonHeight, m_nButtonWidth, m_nButtonHeight);
	GetDlgItem(IDCANCEL)->MoveWindow(nButtonBegin + 4 * (nButtonGap + m_nButtonWidth), m_nWindowHeight - 3 * nButtonHeight - m_nButtonHeight, m_nButtonWidth, m_nButtonHeight);

	int nShowWindowWidth = (m_nWindowWidth - (m_nButtonUpleftX + m_nButtonWidth + nGap) * 2 - nGap) / 2;
	int nShowWindowHeight = (m_nWindowHeight - 3 * nButtonHeight - m_nButtonHeight - 2 * nGap) / 2;
	int nShowWindowWidth2 = 2 * nShowWindowWidth + nGap;
	int nShowWindowHeight2 = (2 * nShowWindowHeight + nGap - 3 * nGap) / 5;
	//GetDlgItem(IDC_STATIC_IMAGE_SHOW)->MoveWindow(m_nButtonUpleftX + m_nButtonWidth + nGap, 0, 2 * nShowWindowWidth + nGap, 2 * nShowWindowHeight + nGap);
	//GetDlgItem(IDC_STATIC_IMAGE_SHOW1)->MoveWindow(m_nButtonUpleftX + m_nButtonWidth + nGap, 0, nShowWindowWidth, nShowWindowHeight);
	//GetDlgItem(IDC_STATIC_IMAGE_SHOW2)->MoveWindow(m_nButtonUpleftX + m_nButtonWidth + nGap + nShowWindowWidth + nGap, 0, nShowWindowWidth, nShowWindowHeight);
	//GetDlgItem(IDC_STATIC_IMAGE_SHOW3)->MoveWindow(m_nButtonUpleftX + m_nButtonWidth + nGap, nShowWindowHeight + nGap, nShowWindowWidth, nShowWindowHeight);
	//GetDlgItem(IDC_STATIC_IMAGE_SHOW4)->MoveWindow(m_nButtonUpleftX + m_nButtonWidth + nGap + nShowWindowWidth + nGap, nShowWindowHeight + nGap, nShowWindowWidth, nShowWindowHeight);

	switch (nCameraIndex)
	{
	case 1:
		GetDlgItem(IDC_STATIC_IMAGE_SHOW)->MoveWindow(m_nButtonUpleftX + m_nButtonWidth + nGap, 0, 2 * nShowWindowWidth + nGap, 2 * nShowWindowHeight + nGap);
		GetDlgItem(IDC_STATIC_IMAGE_SHOW1)->MoveWindow(m_nButtonUpleftX + m_nButtonWidth + nGap, 0, 2 * nShowWindowWidth + nGap, 2 * nShowWindowHeight + nGap);
		GetDlgItem(IDC_STATIC_IMAGE_SHOW2)->MoveWindow(m_nButtonUpleftX + m_nButtonWidth + nGap + nShowWindowWidth + nGap, 0, nShowWindowWidth, nShowWindowHeight);
		GetDlgItem(IDC_STATIC_IMAGE_SHOW3)->MoveWindow(m_nButtonUpleftX + m_nButtonWidth + nGap, nShowWindowHeight + nGap, nShowWindowWidth, nShowWindowHeight);
		GetDlgItem(IDC_STATIC_IMAGE_SHOW4)->MoveWindow(m_nButtonUpleftX + m_nButtonWidth + nGap + nShowWindowWidth + nGap, nShowWindowHeight + nGap, nShowWindowWidth, nShowWindowHeight);

		GetDlgItem(IDC_STATIC_IMAGE_SHOW)->ShowWindow(false);
		GetDlgItem(IDC_STATIC_IMAGE_SHOW1)->ShowWindow(true);
		GetDlgItem(IDC_STATIC_IMAGE_SHOW2)->ShowWindow(false);
		GetDlgItem(IDC_STATIC_IMAGE_SHOW3)->ShowWindow(false);
		GetDlgItem(IDC_STATIC_IMAGE_SHOW4)->ShowWindow(false);
		break;
	case 2:
		GetDlgItem(IDC_STATIC_IMAGE_SHOW)->MoveWindow(m_nButtonUpleftX + m_nButtonWidth + nGap, 0, 2 * nShowWindowWidth + nGap, 2 * nShowWindowHeight + nGap);
		GetDlgItem(IDC_STATIC_IMAGE_SHOW1)->MoveWindow(m_nButtonUpleftX + m_nButtonWidth + nGap, 0, nShowWindowWidth, nShowWindowHeight);
		GetDlgItem(IDC_STATIC_IMAGE_SHOW2)->MoveWindow(m_nButtonUpleftX + m_nButtonWidth + nGap, 0, 2 * nShowWindowWidth + nGap, 2 * nShowWindowHeight + nGap);
		GetDlgItem(IDC_STATIC_IMAGE_SHOW3)->MoveWindow(m_nButtonUpleftX + m_nButtonWidth + nGap, nShowWindowHeight + nGap, nShowWindowWidth, nShowWindowHeight);
		GetDlgItem(IDC_STATIC_IMAGE_SHOW4)->MoveWindow(m_nButtonUpleftX + m_nButtonWidth + nGap + nShowWindowWidth + nGap, nShowWindowHeight + nGap, nShowWindowWidth, nShowWindowHeight);
		GetDlgItem(IDC_STATIC_IMAGE_SHOW)->ShowWindow(false);
		GetDlgItem(IDC_STATIC_IMAGE_SHOW1)->ShowWindow(false);
		GetDlgItem(IDC_STATIC_IMAGE_SHOW2)->ShowWindow(true);
		GetDlgItem(IDC_STATIC_IMAGE_SHOW3)->ShowWindow(false);
		GetDlgItem(IDC_STATIC_IMAGE_SHOW4)->ShowWindow(false);
		break;
	case 3:
		GetDlgItem(IDC_STATIC_IMAGE_SHOW)->MoveWindow(m_nButtonUpleftX + m_nButtonWidth + nGap, 0, 2 * nShowWindowWidth + nGap, 2 * nShowWindowHeight + nGap);
		GetDlgItem(IDC_STATIC_IMAGE_SHOW1)->MoveWindow(m_nButtonUpleftX + m_nButtonWidth + nGap, 0, nShowWindowWidth, nShowWindowHeight);
		GetDlgItem(IDC_STATIC_IMAGE_SHOW2)->MoveWindow(m_nButtonUpleftX + m_nButtonWidth + nGap + nShowWindowWidth + nGap, 0, nShowWindowWidth, nShowWindowHeight);
		GetDlgItem(IDC_STATIC_IMAGE_SHOW3)->MoveWindow(m_nButtonUpleftX + m_nButtonWidth + nGap, 0, 2 * nShowWindowWidth + nGap, 2 * nShowWindowHeight + nGap);
		GetDlgItem(IDC_STATIC_IMAGE_SHOW4)->MoveWindow(m_nButtonUpleftX + m_nButtonWidth + nGap + nShowWindowWidth + nGap, nShowWindowHeight + nGap, nShowWindowWidth, nShowWindowHeight);
		GetDlgItem(IDC_STATIC_IMAGE_SHOW)->ShowWindow(false);
		GetDlgItem(IDC_STATIC_IMAGE_SHOW1)->ShowWindow(false);
		GetDlgItem(IDC_STATIC_IMAGE_SHOW2)->ShowWindow(false);
		GetDlgItem(IDC_STATIC_IMAGE_SHOW3)->ShowWindow(true);
		GetDlgItem(IDC_STATIC_IMAGE_SHOW4)->ShowWindow(false);
		break;
	case 4:
		GetDlgItem(IDC_STATIC_IMAGE_SHOW)->MoveWindow(m_nButtonUpleftX + m_nButtonWidth + nGap, 0, 2 * nShowWindowWidth + nGap, 2 * nShowWindowHeight + nGap);
		GetDlgItem(IDC_STATIC_IMAGE_SHOW1)->MoveWindow(m_nButtonUpleftX + m_nButtonWidth + nGap, 0, nShowWindowWidth, nShowWindowHeight);
		GetDlgItem(IDC_STATIC_IMAGE_SHOW2)->MoveWindow(m_nButtonUpleftX + m_nButtonWidth + nGap + nShowWindowWidth + nGap, 0, nShowWindowWidth, nShowWindowHeight);
		GetDlgItem(IDC_STATIC_IMAGE_SHOW3)->MoveWindow(m_nButtonUpleftX + m_nButtonWidth + nGap, nShowWindowHeight + nGap, nShowWindowWidth, nShowWindowHeight);
		GetDlgItem(IDC_STATIC_IMAGE_SHOW4)->MoveWindow(m_nButtonUpleftX + m_nButtonWidth + nGap, 0, 2 * nShowWindowWidth + nGap, 2 * nShowWindowHeight + nGap);
		GetDlgItem(IDC_STATIC_IMAGE_SHOW)->ShowWindow(false);
		GetDlgItem(IDC_STATIC_IMAGE_SHOW1)->ShowWindow(false);
		GetDlgItem(IDC_STATIC_IMAGE_SHOW2)->ShowWindow(false);
		GetDlgItem(IDC_STATIC_IMAGE_SHOW3)->ShowWindow(false);
		GetDlgItem(IDC_STATIC_IMAGE_SHOW4)->ShowWindow(true);
		break;
	default:
		GetDlgItem(IDC_STATIC_IMAGE_SHOW)->ShowWindow(true);
		GetDlgItem(IDC_STATIC_IMAGE_SHOW1)->ShowWindow(true);
		GetDlgItem(IDC_STATIC_IMAGE_SHOW2)->ShowWindow(true);
		GetDlgItem(IDC_STATIC_IMAGE_SHOW3)->ShowWindow(true);
		GetDlgItem(IDC_STATIC_IMAGE_SHOW4)->ShowWindow(true);
		//GetDlgItem(IDC_STATIC_IMAGE_SHOW)->MoveWindow(m_nButtonUpleftX + m_nButtonWidth + nGap, 0, 2 * nShowWindowWidth + nGap, 2 * nShowWindowHeight + nGap);
		GetDlgItem(IDC_STATIC_IMAGE_SHOW1)->MoveWindow(m_nButtonUpleftX + m_nButtonWidth + nGap, 0 * (nShowWindowHeight2 + nGap), nShowWindowWidth2, nShowWindowHeight2);
		GetDlgItem(IDC_STATIC_IMAGE_SHOW2)->MoveWindow(m_nButtonUpleftX + m_nButtonWidth + nGap, 1 * (nShowWindowHeight2 + nGap), nShowWindowWidth2, nShowWindowHeight2);
		GetDlgItem(IDC_STATIC_IMAGE_SHOW3)->MoveWindow(m_nButtonUpleftX + m_nButtonWidth + nGap, 2 * (nShowWindowHeight2 + nGap), nShowWindowWidth2, nShowWindowHeight2);
		GetDlgItem(IDC_STATIC_IMAGE_SHOW4)->MoveWindow(m_nButtonUpleftX + m_nButtonWidth + nGap, 3 * (nShowWindowHeight2 + nGap), nShowWindowWidth2, nShowWindowHeight2);
		GetDlgItem(IDC_STATIC_IMAGE_SHOW)->MoveWindow(m_nButtonUpleftX + m_nButtonWidth + nGap, 4 * (nShowWindowHeight2 + nGap), nShowWindowWidth2, nShowWindowHeight2);
		GetDlgItem(IDC_EDIT_INFO_SHOW)->MoveWindow(m_nButtonUpleftX + m_nButtonWidth + nGap, 4 * (nShowWindowHeight2 + nGap), nShowWindowWidth2, nShowWindowHeight2);
		break;
	}
	

	RedrawWindow();
}

void CFiberDetectControlDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CFiberDetectControlDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);

		CString cs_str;
		string str_str;
		str_str = cs_str;
		Point P1, P2, P0, P3;
		Point pRect1, pRect2;
		POINT  pTopLeft, pRightBottom;
		pTopLeft.x = 5000; pTopLeft.y = 5000; pRightBottom.x = 0; pRightBottom.y = 0;
		CString strText = "经纬异纤检测";
		CRect DrawRect(0, 0, 300, 100);
		ShowText(m_pwnd, strText, DrawRect);
		//ShowText(m_pwnd, strText);
	}
	else
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		//SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		//// 使图标在工作区矩形中居中
		//int cxIcon = GetSystemMetrics(SM_CXICON);
		//int cyIcon = GetSystemMetrics(SM_CYICON);
		//CRect rect;
		//GetClientRect(&rect);
		//int x = (rect.Width() - cxIcon + 1) / 2;
		//int y = (rect.Height() - cyIcon + 1) / 2;

		RefreshWnd(m_pwnd);   //clear the print words from the show window;
		CString cs_str;
		string str_str;
		str_str = cs_str;
		CString strText = "经纬异纤检测\n";
		PushTailShowText(strText);
		CRect DrawRect(0, 0, 300, 50);
		//ShowText(m_pwnd, m_strShowText, DrawRect);
		//ShowText(m_pwnd, m_strShowText);
		strText.Format("服务器地址:%s,端口号:%d", parameters_setting_trans.local_ip, parameters_setting_trans.local_port);
		PushTailShowText(strText);
		//ShowText(m_pwnd, m_strShowText, nullptr);
		ShowText(m_pwnd, m_strShowText);

		CDialogEx::OnPaint();
		
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CFiberDetectControlDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CFiberDetectControlDlg::OnBnClickedButtonHardwareTest()
{
	// TODO:  在此添加控件通知处理程序代码
	GetDlgItem(IDC_STATIC_IMAGE_SHOW)->ShowWindow(true);
	GetDlgItem(IDC_STATIC_IMAGE_SHOW1)->ShowWindow(false);
	GetDlgItem(IDC_STATIC_IMAGE_SHOW2)->ShowWindow(false);
	GetDlgItem(IDC_STATIC_IMAGE_SHOW3)->ShowWindow(false);
	GetDlgItem(IDC_STATIC_IMAGE_SHOW4)->ShowWindow(false);
	m_bShowInfoWindow = true;
	m_bShowImageWidhow = false;
	CMagneticValve dlg;
	if (dlg.DoModal() != IDOK)
	{
		//GetDlgItem(IDC_STATIC_IMAGE_SHOW)->ShowWindow(false);
		//GetDlgItem(IDC_STATIC_IMAGE_SHOW1)->ShowWindow(true);
		//GetDlgItem(IDC_STATIC_IMAGE_SHOW2)->ShowWindow(true);
		//GetDlgItem(IDC_STATIC_IMAGE_SHOW3)->ShowWindow(true);
		//GetDlgItem(IDC_STATIC_IMAGE_SHOW4)->ShowWindow(true);
		return;
	}

	//GetDlgItem(IDC_STATIC_IMAGE_SHOW)->ShowWindow(false);
	//GetDlgItem(IDC_STATIC_IMAGE_SHOW1)->ShowWindow(true);
	//GetDlgItem(IDC_STATIC_IMAGE_SHOW2)->ShowWindow(true);
	//GetDlgItem(IDC_STATIC_IMAGE_SHOW3)->ShowWindow(true);
	//GetDlgItem(IDC_STATIC_IMAGE_SHOW4)->ShowWindow(true);

}


void CFiberDetectControlDlg::OnBnClickedButtonNetParametersSetting()
{
	// TODO:  在此添加控件通知处理程序代码
	CNetSetting dlg;
	if (dlg.DoModal() != IDOK)
		return;

	int camera_total;
	char IniRead[255];
	memset(IniRead, 0, 255);
	char IniPath[255];
	memset(IniPath, 0, 255);
	//camera_ip_info ci_info;
	CString strPath = GetModulePath();
	strcpy_s(IniPath, 255, strPath.GetBuffer());
	strcat_s(IniPath, "/param.ini");
	GetPrivateProfileStringA("System", "box1_ip", "", IniRead, 24, IniPath);
	//m_dlgNetworkParameters.GetDlgItem(IDC_EDIT_IP_ADRESS)->SetWindowTextA(IniRead);
	GetPrivateProfileStringA("System", "box1_port", "", IniRead, 24, IniPath);
	//m_dlgNetworkParameters.GetDlgItem(IDC_EDIT_IP_PORT_NUMBER)->SetWindowTextA(IniRead);

	parameters_setting_trans.signalling = 1;     //Use siggnalling = 1 to indicate send the network setting command;

	
	GetPrivateProfileStringA("System", "box1_ip", "", IniRead, 24, IniPath);
	strcpy(parameters_setting_trans.box1_ip,IniRead);
	GetPrivateProfileStringA("System", "box1_port", "", IniRead, 24, IniPath);
	parameters_setting_trans.box1_port = atoi(IniRead);

	GetPrivateProfileStringA("System", "box2_ip", "", IniRead, 24, IniPath);
	strcpy(parameters_setting_trans.box2_ip , IniRead);
	GetPrivateProfileStringA("System", "box2_port", "", IniRead, 24, IniPath);
	parameters_setting_trans.box2_port = atoi(IniRead);

	GetPrivateProfileStringA("System", "electric_board_ip", "", IniRead, 24, IniPath);
	strcpy(parameters_setting_trans.electric_board_ip , IniRead);
	GetPrivateProfileStringA("System", "electric_board_port", "", IniRead, 24, IniPath);
	parameters_setting_trans.electric_board_port = atoi(IniRead);

	GetPrivateProfileStringA("System", "local_ip", "", IniRead, 24, IniPath);
	strcpy(parameters_setting_trans.local_ip , IniRead);
	GetPrivateProfileStringA("System", "local_port", "", IniRead, 24, IniPath);
	parameters_setting_trans.local_port = atoi(IniRead);

	GetPrivateProfileStringA("CameraSetting", "cameraA_ip", "", IniRead, 24, IniPath);
	strcpy(parameters_setting_trans.cameraA_ip , IniRead);
	GetPrivateProfileStringA("CameraSetting", "cameraA_port", "", IniRead, 24, IniPath);
	parameters_setting_trans.cameraA_port = atoi(IniRead);
	GetPrivateProfileStringA("CameraSetting", "cameraA_scan_speed", "", IniRead, 24, IniPath);
	parameters_setting_trans.cameraA_scan_speed = atoi(IniRead);
	GetPrivateProfileStringA("CameraSetting", "cameraA_tirgging_mode", "", IniRead, 24, IniPath);
	parameters_setting_trans.cameraA_tirgging_mode = atoi(IniRead);
	GetPrivateProfileStringA("CameraSetting", "cameraA_exposure_time", "", IniRead, 24, IniPath);
	parameters_setting_trans.cameraA_exposure_time = atoi(IniRead);
	GetPrivateProfileStringA("CameraSetting", "cameraA_gain", "", IniRead, 24, IniPath);
	parameters_setting_trans.cameraA_gain = atoi(IniRead);
	GetPrivateProfileStringA("CameraSetting", "cameraA_width", "", IniRead, 24, IniPath);
	parameters_setting_trans.cameraA_width = atoi(IniRead);
	GetPrivateProfileStringA("CameraSetting", "cameraA_height", "", IniRead, 24, IniPath);
	parameters_setting_trans.cameraA_height = atoi(IniRead);
	GetPrivateProfileStringA("CameraSetting", "cameraA_mac", "", IniRead, 24, IniPath);
	strcpy(parameters_setting_trans.cameraA_mac , IniRead);

	GetPrivateProfileStringA("CameraSetting", "cameraB_ip", "", IniRead, 24, IniPath);
	strcpy(parameters_setting_trans.cameraB_ip , IniRead);
	GetPrivateProfileStringA("CameraSetting", "cameraB_port", "", IniRead, 24, IniPath);
	parameters_setting_trans.cameraB_port = atoi(IniRead);
	GetPrivateProfileStringA("CameraSetting", "cameraB_scan_speed", "", IniRead, 24, IniPath);
	parameters_setting_trans.cameraB_scan_speed = atoi(IniRead);
	GetPrivateProfileStringA("CameraSetting", "cameraB_tirgging_mode", "", IniRead, 24, IniPath);
	parameters_setting_trans.cameraB_tirgging_mode = atoi(IniRead);
	GetPrivateProfileStringA("CameraSetting", "cameraB_exposure_time", "", IniRead, 24, IniPath);
	parameters_setting_trans.cameraB_exposure_time = atoi(IniRead);
	GetPrivateProfileStringA("CameraSetting", "cameraB_gain", "", IniRead, 24, IniPath);
	parameters_setting_trans.cameraB_gain = atoi(IniRead);
	GetPrivateProfileStringA("CameraSetting", "cameraB_width", "", IniRead, 24, IniPath);
	parameters_setting_trans.cameraB_width = atoi(IniRead);
	GetPrivateProfileStringA("CameraSetting", "cameraB_height", "", IniRead, 24, IniPath);
	parameters_setting_trans.cameraB_height = atoi(IniRead);
	GetPrivateProfileStringA("CameraSetting", "cameraB_mac", "", IniRead, 24, IniPath);
	strcpy(parameters_setting_trans.cameraB_mac, IniRead);

	GetPrivateProfileStringA("CameraSetting", "cameraC_ip", "", IniRead, 24, IniPath);
	strcpy(parameters_setting_trans.cameraC_ip , IniRead);
	GetPrivateProfileStringA("CameraSetting", "cameraC_port", "", IniRead, 24, IniPath);
	parameters_setting_trans.cameraC_port = atoi(IniRead);
	GetPrivateProfileStringA("CameraSetting", "cameraC_scan_speed", "", IniRead, 24, IniPath);
	parameters_setting_trans.cameraC_scan_speed = atoi(IniRead);
	GetPrivateProfileStringA("CameraSetting", "cameraC_tirgging_mode", "", IniRead, 24, IniPath);
	parameters_setting_trans.cameraC_tirgging_mode = atoi(IniRead);
	GetPrivateProfileStringA("CameraSetting", "cameraC_exposure_time", "", IniRead, 24, IniPath);
	parameters_setting_trans.cameraC_exposure_time = atoi(IniRead);
	GetPrivateProfileStringA("CameraSetting", "cameraC_gain", "", IniRead, 24, IniPath);
	parameters_setting_trans.cameraC_gain = atoi(IniRead);
	GetPrivateProfileStringA("CameraSetting", "cameraC_width", "", IniRead, 24, IniPath);
	parameters_setting_trans.cameraC_width = atoi(IniRead);
	GetPrivateProfileStringA("CameraSetting", "cameraC_height", "", IniRead, 24, IniPath);
	parameters_setting_trans.cameraC_height = atoi(IniRead);
	GetPrivateProfileStringA("CameraSetting", "cameraC_mac", "", IniRead, 24, IniPath);
	strcpy(parameters_setting_trans.cameraC_mac, IniRead);

	GetPrivateProfileStringA("CameraSetting", "cameraD_ip", "", IniRead, 24, IniPath);
	strcpy(parameters_setting_trans.cameraD_ip , IniRead);
	GetPrivateProfileStringA("CameraSetting", "cameraD_port", "", IniRead, 24, IniPath);
	parameters_setting_trans.cameraD_port = atoi(IniRead);
	GetPrivateProfileStringA("CameraSetting", "cameraD_scan_speed", "", IniRead, 24, IniPath);
	parameters_setting_trans.cameraD_scan_speed = atoi(IniRead);
	GetPrivateProfileStringA("CameraSetting", "cameraD_tirgging_mode", "", IniRead, 24, IniPath);
	parameters_setting_trans.cameraD_tirgging_mode = atoi(IniRead);
	GetPrivateProfileStringA("CameraSetting", "cameraD_exposure_time", "", IniRead, 24, IniPath);
	parameters_setting_trans.cameraD_exposure_time = atoi(IniRead);
	GetPrivateProfileStringA("CameraSetting", "cameraD_gain", "", IniRead, 24, IniPath);
	parameters_setting_trans.cameraD_gain = atoi(IniRead);
	GetPrivateProfileStringA("CameraSetting", "cameraD_width", "", IniRead, 24, IniPath);
	parameters_setting_trans.cameraD_width = atoi(IniRead);
	GetPrivateProfileStringA("CameraSetting", "cameraD_height", "", IniRead, 24, IniPath);
	parameters_setting_trans.cameraD_height = atoi(IniRead);
	GetPrivateProfileStringA("CameraSetting", "cameraD_mac", "", IniRead, 24, IniPath);
	strcpy(parameters_setting_trans.cameraD_mac, IniRead);
	//****************************To read the fiber detect parameters form ini files************************
	memset(IniRead, 0, 255);
	//sprintf(IniWrite, "%d,", parameters_setting_trans.cameraA_begin);
	GetPrivateProfileString("BaseDetectParameter", "cameraA_begin", "", IniRead, 24, IniPath);
	parameters_setting_trans.cameraA_begin = atoi(IniRead);

	//IniWrite.Format("%d", parameters_setting_trans.cameraA_end);
	memset(IniRead, 0, 255);
	//sprintf(IniWrite, "%d,", parameters_setting_trans.cameraA_end);
	GetPrivateProfileString("BaseDetectParameter", "cameraA_end", "", IniRead, 24, IniPath);
	parameters_setting_trans.cameraA_end = atoi(IniRead);

	//IniWrite.Format("%d", parameters_setting_trans.cameraA_align_begin);
	memset(IniRead, 0, 255);
	//sprintf(IniWrite, "%d,", parameters_setting_trans.cameraA_align_begin);
	GetPrivateProfileString("BaseDetectParameter", "cameraA_align_begin", "", IniRead, 24, IniPath);
	parameters_setting_trans.cameraA_align_begin = atoi(IniRead);

	//IniWrite.Format("%d", parameters_setting_trans.cameraA_align_end);
	memset(IniRead, 0, 255);
	//sprintf(IniWrite, "%d,", parameters_setting_trans.cameraA_align_end);
	GetPrivateProfileString("BaseDetectParameter", "cameraA_align_end", "", IniRead, 24, IniPath);
	parameters_setting_trans.cameraA_align_end = atoi(IniRead);

	//IniWrite.Format("%d", parameters_setting_trans.cameraA_detect_begin);
	memset(IniRead, 0, 255);
	//sprintf(IniWrite, "%d,", parameters_setting_trans.cameraA_detect_begin);
	GetPrivateProfileString("BaseDetectParameter", "cameraA_detect_begin", "", IniRead, 24, IniPath);
	parameters_setting_trans.cameraA_detect_begin = atoi(IniRead);

	//IniWrite.Format("%d", parameters_setting_trans.cameraA_detect_end);
	memset(IniRead, 0, 255);
	//sprintf(IniWrite, "%d,", parameters_setting_trans.cameraA_detect_end);
	GetPrivateProfileString("BaseDetectParameter", "cameraA_detect_end", "", IniRead, 24, IniPath);
	parameters_setting_trans.cameraA_detect_begin = atoi(IniRead);//-----------------------------------------------

	//IniWrite.Format("%d", parameters_setting_trans.cameraA_valve_amount);
	memset(IniRead, 0, 255);
	//sprintf(IniWrite, "%d,", parameters_setting_trans.cameraA_valve_amount);
	GetPrivateProfileString("BaseDetectParameter", "cameraA_valve_amount", "", IniRead, 24, IniPath);
	parameters_setting_trans.cameraA_valve_amount = atoi(IniRead);

	//IniWrite.Format("%d", parameters_setting_trans.cameraB_begin);
	memset(IniRead, 0, 255);
	//sprintf(IniWrite, "%d,", parameters_setting_trans.cameraB_begin);
	GetPrivateProfileString("BaseDetectParameter", "cameraB_begin", "", IniRead, 24, IniPath);
	parameters_setting_trans.cameraB_begin = atoi(IniRead);

	//IniWrite.Format("%d", parameters_setting_trans.cameraB_end);
	memset(IniRead, 0, 255);
	//sprintf(IniWrite, "%d,", parameters_setting_trans.cameraB_end);
	GetPrivateProfileString("BaseDetectParameter", "cameraB_end", "", IniRead, 24, IniPath);
	parameters_setting_trans.cameraB_end = atoi(IniRead);

	//IniWrite.Format("%d", parameters_setting_trans.cameraB_align_begin);
	memset(IniRead, 0, 255);
	//sprintf(IniWrite, "%d,", parameters_setting_trans.cameraB_align_begin);
	GetPrivateProfileString("BaseDetectParameter", "cameraB_align_begin", "", IniRead, 24, IniPath);
	parameters_setting_trans.cameraB_align_begin = atoi(IniRead);

	//IniWrite.Format("%d", parameters_setting_trans.cameraB_align_end);
	memset(IniRead, 0, 255);
	//sprintf(IniWrite, "%d,", parameters_setting_trans.cameraB_align_end);
	GetPrivateProfileString("BaseDetectParameter", "cameraB_align_end", "", IniRead, 24, IniPath);
	parameters_setting_trans.cameraB_align_end = atoi(IniRead);

	//IniWrite.Format("%d", parameters_setting_trans.cameraB_detect_begin);
	memset(IniRead, 0, 255);
	//sprintf(IniWrite, "%d,", parameters_setting_trans.cameraB_detect_begin);
	GetPrivateProfileString("BaseDetectParameter", "cameraB_detect_begin", "", IniRead, 24, IniPath);
	parameters_setting_trans.cameraB_detect_begin = atoi(IniRead);

	//IniWrite.Format("%d", parameters_setting_trans.cameraB_detect_end);
	memset(IniRead, 0, 255);
	//sprintf(IniWrite, "%d,", parameters_setting_trans.cameraB_detect_end);
	GetPrivateProfileString("BaseDetectParameter", "cameraB_detect_end", "", IniRead, 24, IniPath);
	parameters_setting_trans.cameraB_detect_end = atoi(IniRead);

	//IniWrite.Format("%d", parameters_setting_trans.cameraB_valve_amount);
	memset(IniRead, 0, 255);
	//sprintf(IniWrite, "%d,", parameters_setting_trans.cameraB_valve_amount);
	GetPrivateProfileString("BaseDetectParameter", "cameraB_valve_amount", "", IniRead, 24, IniPath);
	parameters_setting_trans.cameraB_valve_amount = atoi(IniRead);

	//IniWrite.Format("%d", parameters_setting_trans.cameraC_begin);
	memset(IniRead, 0, 255);
	//sprintf(IniWrite, "%d,", parameters_setting_trans.cameraC_begin);
	GetPrivateProfileString("BaseDetectParameter", "cameraC_begin", "", IniRead, 24, IniPath);
	parameters_setting_trans.cameraC_begin = atoi(IniRead);

	//IniWrite.Format("%d", parameters_setting_trans.cameraC_end);
	memset(IniRead, 0, 255);
	//sprintf(IniWrite, "%d,", parameters_setting_trans.cameraC_end);
	GetPrivateProfileString("BaseDetectParameter", "cameraC_end", "", IniRead, 24, IniPath);
	parameters_setting_trans.cameraC_end = atoi(IniRead);

	//IniWrite.Format("%d", parameters_setting_trans.cameraC_align_begin);
	memset(IniRead, 0, 255);
	//sprintf(IniWrite, "%d,", parameters_setting_trans.cameraC_align_begin);
	GetPrivateProfileString("BaseDetectParameter", "cameraC_align_begin", "", IniRead, 24, IniPath);
	parameters_setting_trans.cameraC_align_begin = atoi(IniRead);

	//IniWrite.Format("%d", parameters_setting_trans.cameraC_align_end);
	memset(IniRead, 0, 255);
	//sprintf(IniWrite, "%d,", parameters_setting_trans.cameraC_align_end);
	GetPrivateProfileString("BaseDetectParameter", "cameraC_align_end", "", IniRead, 24, IniPath);
	parameters_setting_trans.cameraC_align_end = atoi(IniRead);

	//IniWrite.Format("%d", parameters_setting_trans.cameraC_detect_begin);
	memset(IniRead, 0, 255);
	//sprintf(IniWrite, "%d,", parameters_setting_trans.cameraC_detect_begin);
	GetPrivateProfileString("BaseDetectParameter", "cameraC_detect_begin", "", IniRead, 24, IniPath);
	parameters_setting_trans.cameraC_detect_begin = atoi(IniRead);

	//IniWrite.Format("%d", parameters_setting_trans.cameraC_detect_end);
	memset(IniRead, 0, 255);
	//sprintf(IniWrite, "%d,", parameters_setting_trans.cameraC_detect_end);
	GetPrivateProfileString("BaseDetectParameter", "cameraC_detect_end", "", IniRead, 24, IniPath);
	parameters_setting_trans.cameraC_detect_end = atoi(IniRead);

	//IniWrite.Format("%d", parameters_setting_trans.cameraC_valve_amount);
	memset(IniRead, 0, 255);
	//sprintf(IniWrite, "%d,", parameters_setting_trans.cameraC_valve_amount);
	GetPrivateProfileString("BaseDetectParameter", "cameraC_valve_amount", "", IniRead, 24, IniPath);
	parameters_setting_trans.cameraC_valve_amount = atoi(IniRead);

	//IniWrite.Format("%d", parameters_setting_trans.cameraD_begin);
	memset(IniRead, 0, 255);
	//sprintf(IniWrite, "%d,", parameters_setting_trans.cameraD_begin);
	GetPrivateProfileString("BaseDetectParameter", "cameraD_begin", "", IniRead, 24, IniPath);
	parameters_setting_trans.cameraD_begin = atoi(IniRead);

	//IniWrite.Format("%d", parameters_setting_trans.cameraD_end);
	memset(IniRead, 0, 255);
	//sprintf(IniWrite, "%d,", parameters_setting_trans.cameraD_end);
	GetPrivateProfileString("BaseDetectParameter", "cameraD_end", "", IniRead, 24, IniPath);
	parameters_setting_trans.cameraD_end = atoi(IniRead);

	//IniWrite.Format("%d", parameters_setting_trans.cameraD_align_begin);
	memset(IniRead, 0, 255);
	//sprintf(IniWrite, "%d,", parameters_setting_trans.cameraD_align_begin);
	GetPrivateProfileString("BaseDetectParameter", "cameraD_align_begin", "", IniRead, 24, IniPath);
	parameters_setting_trans.cameraD_align_begin = atoi(IniRead);

	//IniWrite.Format("%d", parameters_setting_trans.cameraD_align_end);
	memset(IniRead, 0, 255);
	//sprintf(IniWrite, "%d,", parameters_setting_trans.cameraD_align_end);
	GetPrivateProfileString("BaseDetectParameter", "cameraD_align_end", "", IniRead, 24, IniPath);
	parameters_setting_trans.cameraD_align_end = atoi(IniRead);

	//IniWrite.Format("%d", parameters_setting_trans.cameraD_detect_begin);
	memset(IniRead, 0, 255);
	//sprintf(IniWrite, "%d,", parameters_setting_trans.cameraD_detect_begin);
	GetPrivateProfileString("BaseDetectParameter", "cameraD_detect_begin", "", IniRead, 24, IniPath);
	parameters_setting_trans.cameraD_detect_begin = atoi(IniRead);

	//IniWrite.Format("%d", parameters_setting_trans.cameraD_detect_end);
	memset(IniRead, 0, 255);
	//sprintf(IniWrite, "%d,", parameters_setting_trans.cameraD_detect_end);
	GetPrivateProfileString("BaseDetectParameter", "cameraD_detect_end", "", IniRead, 24, IniPath);
	parameters_setting_trans.cameraD_detect_end = atoi(IniRead);

	//IniWrite.Format("%d", parameters_setting_trans.cameraD_valve_amount);
	memset(IniRead, 0, 255);
	//sprintf(IniWrite, "%d,", parameters_setting_trans.cameraD_valve_amount);
	GetPrivateProfileString("BaseDetectParameter", "cameraD_valve_amount", "", IniRead, 24, IniPath);
	parameters_setting_trans.cameraD_valve_amount = atoi(IniRead);

	//IniWrite.Format("%d", parameters_setting_trans.cameraA_delay);
	memset(IniRead, 0, 255);
	//sprintf(IniWrite, "%d,", parameters_setting_trans.cameraA_delay);
	GetPrivateProfileString("ValveParameter", "cameraA_delay", "", IniRead, 24, IniPath);
	parameters_setting_trans.cameraA_delay = atoi(IniRead);

	//IniWrite.Format("%d", parameters_setting_trans.cameraA_hold);
	memset(IniRead, 0, 255);
	//sprintf(IniWrite, "%d,", parameters_setting_trans.cameraA_hold);
	GetPrivateProfileString("ValveParameter", "cameraA_hold", "", IniRead, 24, IniPath);
	parameters_setting_trans.cameraA_hold = atoi(IniRead);

	//IniWrite.Format("%d", parameters_setting_trans.cameraB_delay);
	memset(IniRead, 0, 255);
	//sprintf(IniWrite, "%d,", parameters_setting_trans.cameraB_delay);
	GetPrivateProfileString("ValveParameter", "cameraB_delay", "", IniRead, 24, IniPath);
	parameters_setting_trans.cameraB_delay = atoi(IniRead);
	//-----------------------------------------------------------------------------------------

	//IniWrite.Format("%d", parameters_setting_trans.cameraB_hold);
	memset(IniRead, 0, 255);
	//sprintf(IniWrite, "%d,", parameters_setting_trans.cameraB_hold);
	GetPrivateProfileString("ValveParameter", "cameraB_hold", "", IniRead, 24, IniPath);
	parameters_setting_trans.cameraB_hold = atoi(IniRead);

	//IniWrite.Format("%d", parameters_setting_trans.cameraC_delay);
	memset(IniRead, 0, 255);
	//sprintf(IniWrite, "%d,", parameters_setting_trans.cameraC_delay);
	GetPrivateProfileString("ValveParameter", "cameraC_delay", "", IniRead, 24, IniPath);
	parameters_setting_trans.cameraC_delay = atoi(IniRead);

	//IniWrite.Format("%d", parameters_setting_trans.cameraC_hold);
	memset(IniRead, 0, 255);
	//sprintf(IniWrite, "%d,", parameters_setting_trans.cameraC_hold);
	GetPrivateProfileString("ValveParameter", "cameraC_hold", "", IniRead, 24, IniPath);
	parameters_setting_trans.cameraC_hold = atoi(IniRead);

	//IniWrite.Format("%d", parameters_setting_trans.cameraD_delay);
	memset(IniRead, 0, 255);
	//sprintf(IniWrite, "%d,", parameters_setting_trans.cameraD_delay);
	GetPrivateProfileString("ValveParameter", "cameraD_delay", "", IniRead, 24, IniPath);
	parameters_setting_trans.cameraD_delay = atoi(IniRead);

	//IniWrite.Format("%d", parameters_setting_trans.cameraD_hold);
	memset(IniRead, 0, 255);
	//sprintf(IniWrite, "%d,", parameters_setting_trans.cameraD_hold);
	GetPrivateProfileString("ValveParameter", "cameraD_hold", "", IniRead, 24, IniPath);
	parameters_setting_trans.cameraD_hold = atoi(IniRead);

	//IniWrite.Format("%d", parameters_setting_trans.valve_work_times);
	memset(IniRead, 0, 255);
	//sprintf(IniWrite, "%d,", parameters_setting_trans.valve_work_times);
	GetPrivateProfileString("ValveParameter", "valve_work_times", "", IniRead, 24, IniPath);
	parameters_setting_trans.valve_work_times = atoi(IniRead);

	//IniWrite.Format("%d", parameters_setting_trans.valve_stop_time);
	memset(IniRead, 0, 255);
	//sprintf(IniWrite, "%d,", parameters_setting_trans.valve_stop_time);
	GetPrivateProfileString("ValveParameter", "valve_stop_time", "", IniRead, 24, IniPath);
	parameters_setting_trans.valve_stop_time = atoi(IniRead);

	//IniWrite.Format("%d", parameters_setting_trans.cameraA_point_area_min);
	memset(IniRead, 0, 255);
	//sprintf(IniWrite, "%d,", parameters_setting_trans.cameraA_point_area_min);
	GetPrivateProfileString("DetectParameter", "cameraA_point_area_min", "", IniRead, 24, IniPath);
	parameters_setting_trans.cameraA_point_area_min = atoi(IniRead);

	//IniWrite.Format("%d", parameters_setting_trans.cameraA_point_area_max);
	memset(IniRead, 0, 255);
	//sprintf(IniWrite, "%d,", parameters_setting_trans.cameraA_point_area_max);
	GetPrivateProfileString("DetectParameter", "cameraA_point_area_max", "", IniRead, 24, IniPath);
	parameters_setting_trans.cameraA_point_area_max = atoi(IniRead);

	//IniWrite.Format("%d", parameters_setting_trans.cameraA_point_length_min);
	memset(IniRead, 0, 255);
	//sprintf(IniWrite, "%d,", parameters_setting_trans.cameraA_point_length_min);
	GetPrivateProfileString("DetectParameter", "cameraA_point_length_min", "", IniRead, 24, IniPath);
	parameters_setting_trans.cameraA_point_length_min = atoi(IniRead);

	//IniWrite.Format("%d", parameters_setting_trans.cameraA_point_length_max);
	memset(IniRead, 0, 255);
	//sprintf(IniWrite, "%d,", parameters_setting_trans.cameraA_point_length_max);
	GetPrivateProfileString("DetectParameter", "cameraA_point_length_max", "", IniRead, 24, IniPath);
	parameters_setting_trans.cameraA_point_length_max = atoi(IniRead);

	//IniWrite.Format("%d", parameters_setting_trans.cameraA_point_width_min);
	memset(IniRead, 0, 255);
	//sprintf(IniWrite, "%d,", parameters_setting_trans.cameraA_point_width_min);
	GetPrivateProfileString("DetectParameter", "cameraA_point_width_min", "", IniRead, 24, IniPath);
	parameters_setting_trans.cameraA_point_width_min = atoi(IniRead);

	//IniWrite.Format("%d", parameters_setting_trans.cameraA_point_width_max);
	memset(IniRead, 0, 255);
	//sprintf(IniWrite, "%d,", parameters_setting_trans.cameraA_point_width_max);
	GetPrivateProfileString("DetectParameter", "cameraA_point_width_max", "", IniRead, 24, IniPath);
	parameters_setting_trans.cameraA_point_width_max = atoi(IniRead);

	//IniWrite.Format("%d", parameters_setting_trans.cameraA_thread_area_min);
	memset(IniRead, 0, 255);
	//sprintf(IniWrite, "%d,", parameters_setting_trans.cameraA_thread_area_min);
	GetPrivateProfileString("DetectParameter", "cameraA_thread_area_min", "", IniRead, 24, IniPath);
	parameters_setting_trans.cameraA_thread_area_min = atoi(IniRead);

	//IniWrite.Format("%d", parameters_setting_trans.cameraA_thread_area_max);
	memset(IniRead, 0, 255);
	//sprintf(IniWrite, "%d,", parameters_setting_trans.cameraA_thread_area_max);
	GetPrivateProfileString("DetectParameter", "cameraA_thread_area_max", "", IniRead, 24, IniPath);
	parameters_setting_trans.cameraA_thread_area_max = atoi(IniRead);

	//IniWrite.Format("%d", parameters_setting_trans.cameraA_thread_length_min);
	memset(IniRead, 0, 255);
	//sprintf(IniWrite, "%d,", parameters_setting_trans.cameraA_thread_length_min);
	GetPrivateProfileString("DetectParameter", "cameraA_thread_length_min", "", IniRead, 24, IniPath);
	parameters_setting_trans.cameraA_thread_length_min = atoi(IniRead);

	//IniWrite.Format("%d", parameters_setting_trans.cameraA_thread_length_max);
	memset(IniRead, 0, 255);
	//sprintf(IniWrite, "%d,", parameters_setting_trans.cameraA_thread_length_max);
	GetPrivateProfileString("DetectParameter", "cameraA_thread_length_max", "", IniRead, 24, IniPath);
	parameters_setting_trans.cameraA_thread_length_max = atoi(IniRead);

	//IniWrite.Format("%d", parameters_setting_trans.cameraA_thread_width_min);
	memset(IniRead, 0, 255);
	//sprintf(IniWrite, "%d,", parameters_setting_trans.cameraA_thread_width_min);
	GetPrivateProfileString("DetectParameter", "cameraA_thread_width_min", "", IniRead, 24, IniPath);
	parameters_setting_trans.cameraA_thread_width_min = atoi(IniRead);

	//IniWrite.Format("%d", parameters_setting_trans.cameraA_thread_width_max);
	memset(IniRead, 0, 255);
	//sprintf(IniWrite, "%d,", parameters_setting_trans.cameraA_thread_width_max);
	GetPrivateProfileString("DetectParameter", "cameraA_thread_width_max", "", IniRead, 24, IniPath);
	parameters_setting_trans.cameraA_thread_width_max = atoi(IniRead);

	//Camera B detect parameter setting;
	//IniWrite.Format("%d", parameters_setting_trans.cameraB_point_area_min);
	memset(IniRead, 0, 255);
	//sprintf(IniWrite, "%d,", parameters_setting_trans.cameraB_point_area_min);
	GetPrivateProfileString("DetectParameter", "cameraB_point_area_min", "", IniRead, 24, IniPath);
	parameters_setting_trans.cameraB_point_area_min = atoi(IniRead);

	//IniWrite.Format("%d", parameters_setting_trans.cameraB_point_area_max);
	memset(IniRead, 0, 255);
	//sprintf(IniWrite, "%d,", parameters_setting_trans.cameraB_point_area_max);
	GetPrivateProfileString("DetectParameter", "cameraB_point_area_max", "", IniRead, 24, IniPath);
	parameters_setting_trans.cameraB_point_area_max = atoi(IniRead);

	//IniWrite.Format("%d", parameters_setting_trans.cameraB_point_length_min);
	memset(IniRead, 0, 255);
	//sprintf(IniWrite, "%d,", parameters_setting_trans.cameraB_point_length_min);
	GetPrivateProfileString("DetectParameter", "cameraB_point_length_min", "", IniRead, 24, IniPath);
	parameters_setting_trans.cameraB_point_length_min = atoi(IniRead);

	//IniWrite.Format("%d", parameters_setting_trans.cameraB_point_length_max);
	memset(IniRead, 0, 255);
	//sprintf(IniWrite, "%d,", parameters_setting_trans.cameraB_point_length_max);
	GetPrivateProfileString("DetectParameter", "cameraB_point_length_max", "", IniRead, 24, IniPath);
	parameters_setting_trans.cameraB_point_length_max = atoi(IniRead);

	//IniWrite.Format("%d", parameters_setting_trans.cameraB_point_width_min);
	memset(IniRead, 0, 255);
	//sprintf(IniWrite, "%d,", parameters_setting_trans.cameraB_point_width_min);
	GetPrivateProfileString("DetectParameter", "cameraB_point_width_min", "", IniRead, 24, IniPath);
	parameters_setting_trans.cameraB_point_width_min = atoi(IniRead);

	//IniWrite.Format("%d", parameters_setting_trans.cameraB_point_width_max);
	memset(IniRead, 0, 255);
	//sprintf(IniWrite, "%d,", parameters_setting_trans.cameraB_point_width_max);
	GetPrivateProfileString("DetectParameter", "cameraB_point_width_max", "", IniRead, 24, IniPath);
	parameters_setting_trans.cameraB_point_width_max = atoi(IniRead);

	//IniWrite.Format("%d", parameters_setting_trans.cameraB_thread_area_min);
	memset(IniRead, 0, 255);
	//sprintf(IniWrite, "%d,", parameters_setting_trans.cameraB_thread_area_min);
	GetPrivateProfileString("DetectParameter", "cameraB_thread_area_min", "", IniRead, 24, IniPath);
	parameters_setting_trans.cameraB_thread_area_min = atoi(IniRead);

	//IniWrite.Format("%d", parameters_setting_trans.cameraB_thread_area_max);
	memset(IniRead, 0, 255);
	//sprintf(IniWrite, "%d,", parameters_setting_trans.cameraB_thread_area_max);
	GetPrivateProfileString("DetectParameter", "cameraB_thread_area_max", "", IniRead, 24, IniPath);
	parameters_setting_trans.cameraB_thread_area_max = atoi(IniRead);

	//IniWrite.Format("%d", parameters_setting_trans.cameraB_thread_length_min);
	memset(IniRead, 0, 255);
	//sprintf(IniWrite, "%d,", parameters_setting_trans.cameraB_thread_length_min);
	GetPrivateProfileString("DetectParameter", "cameraB_thread_length_min", "", IniRead, 24, IniPath);
	parameters_setting_trans.cameraB_thread_length_min = atoi(IniRead);

	//IniWrite.Format("%d", parameters_setting_trans.cameraB_thread_length_max);
	memset(IniRead, 0, 255);
	//sprintf(IniWrite, "%d,", parameters_setting_trans.cameraB_thread_length_max);
	GetPrivateProfileString("DetectParameter", "cameraB_thread_length_max", "", IniRead, 24, IniPath);
	parameters_setting_trans.cameraB_thread_length_max = atoi(IniRead);


	//IniWrite.Format("%d", parameters_setting_trans.cameraB_thread_width_min);
	memset(IniRead, 0, 255);
	//sprintf(IniWrite, "%d,", parameters_setting_trans.cameraB_thread_width_min);
	GetPrivateProfileString("DetectParameter", "cameraB_thread_width_min", "", IniRead, 24, IniPath);
	parameters_setting_trans.cameraB_thread_width_min = atoi(IniRead);


	//IniWrite.Format("%d", parameters_setting_trans.cameraB_thread_width_max);
	memset(IniRead, 0, 255);
	//sprintf(IniWrite, "%d,", parameters_setting_trans.cameraB_thread_width_max);
	GetPrivateProfileString("DetectParameter", "cameraB_thread_width_max", "", IniRead, 24, IniPath);
	parameters_setting_trans.cameraB_thread_width_max = atoi(IniRead);



	//Camera C detect parameter setting;
	//IniWrite.Format("%d", parameters_setting_trans.cameraC_point_area_min);
	memset(IniRead, 0, 255);
	//sprintf(IniWrite, "%d,", parameters_setting_trans.cameraC_point_area_min);
	GetPrivateProfileString("DetectParameter", "cameraC_point_area_min", "", IniRead, 24, IniPath);
	parameters_setting_trans.cameraC_point_area_min = atoi(IniRead);


	//IniWrite.Format("%d", parameters_setting_trans.cameraC_point_area_max);
	memset(IniRead, 0, 255);
	//sprintf(IniWrite, "%d,", parameters_setting_trans.cameraC_point_area_max);
	GetPrivateProfileString("DetectParameter", "cameraC_point_area_max", "", IniRead, 24, IniPath);
	parameters_setting_trans.cameraC_point_area_max = atoi(IniRead);

	//IniWrite.Format("%d", parameters_setting_trans.cameraC_point_length_min);
	memset(IniRead, 0, 255);
	//sprintf(IniWrite, "%d,", parameters_setting_trans.cameraC_point_length_min);
	GetPrivateProfileString("DetectParameter", "cameraC_point_length_min", "", IniRead, 24, IniPath);
	parameters_setting_trans.cameraC_point_length_min = atoi(IniRead);

	//IniWrite.Format("%d", parameters_setting_trans.cameraC_point_length_max);
	memset(IniRead, 0, 255);
	//sprintf(IniWrite, "%d,", parameters_setting_trans.cameraC_point_length_max);
	GetPrivateProfileString("DetectParameter", "cameraC_point_length_max", "", IniRead, 24, IniPath);
	parameters_setting_trans.cameraC_point_length_max = atoi(IniRead);

	//IniWrite.Format("%d", parameters_setting_trans.cameraC_point_width_min);
	memset(IniRead, 0, 255);
	//sprintf(IniWrite, "%d,", parameters_setting_trans.cameraC_point_width_min);
	GetPrivateProfileString("DetectParameter", "cameraC_point_width_min", "", IniRead, 24, IniPath);
	parameters_setting_trans.cameraC_point_width_min = atoi(IniRead);

	//IniWrite.Format("%d", parameters_setting_trans.cameraC_point_width_max);
	memset(IniRead, 0, 255);
	//sprintf(IniWrite, "%d,", parameters_setting_trans.cameraC_point_width_max);
	GetPrivateProfileString("DetectParameter", "cameraC_point_width_max", "", IniRead, 24, IniPath);
	parameters_setting_trans.cameraC_point_width_max = atoi(IniRead);

	//IniWrite.Format("%d", parameters_setting_trans.cameraC_thread_area_min);
	memset(IniRead, 0, 255);
	//sprintf(IniWrite, "%d,", parameters_setting_trans.cameraC_thread_area_min);
	GetPrivateProfileString("DetectParameter", "cameraC_thread_area_min", "", IniRead, 24, IniPath);
	parameters_setting_trans.cameraC_thread_area_min = atoi(IniRead);

	//IniWrite.Format("%d", parameters_setting_trans.cameraC_thread_area_max);
	memset(IniRead, 0, 255);
	//sprintf(IniWrite, "%d,", parameters_setting_trans.cameraC_thread_area_max);
	GetPrivateProfileString("DetectParameter", "cameraC_thread_area_max", "", IniRead, 24, IniPath);
	parameters_setting_trans.cameraC_thread_area_max = atoi(IniRead);

	//IniWrite.Format("%d", parameters_setting_trans.cameraC_thread_length_min);
	memset(IniRead, 0, 255);
	//sprintf(IniWrite, "%d,", parameters_setting_trans.cameraC_thread_length_min);
	GetPrivateProfileString("DetectParameter", "cameraC_thread_length_min", "", IniRead, 24, IniPath);
	parameters_setting_trans.cameraC_thread_length_min = atoi(IniRead);

	//IniWrite.Format("%d", parameters_setting_trans.cameraC_thread_length_max);
	memset(IniRead, 0, 255);
	//sprintf(IniWrite, "%d,", parameters_setting_trans.cameraC_thread_length_max);
	GetPrivateProfileString("DetectParameter", "cameraC_thread_length_max", "", IniRead, 24, IniPath);
	parameters_setting_trans.cameraC_thread_length_max = atoi(IniRead);

	//IniWrite.Format("%d", parameters_setting_trans.cameraC_thread_width_min);
	memset(IniRead, 0, 255);
	//sprintf(IniWrite, "%d,", parameters_setting_trans.cameraC_thread_width_min);
	GetPrivateProfileString("DetectParameter", "cameraC_thread_width_min", "", IniRead, 24, IniPath);
	parameters_setting_trans.cameraC_thread_width_min = atoi(IniRead);

	//IniWrite.Format("%d", parameters_setting_trans.cameraC_thread_width_max);
	memset(IniRead, 0, 255);
	//sprintf(IniWrite, "%d,", parameters_setting_trans.cameraC_thread_width_max);
	GetPrivateProfileString("DetectParameter", "cameraC_thread_width_max", "", IniRead, 24, IniPath);
	parameters_setting_trans.cameraC_thread_width_max = atoi(IniRead);


	//Camera D detect parameter setting;
	//IniWrite.Format("%d", parameters_setting_trans.cameraD_point_area_min);
	memset(IniRead, 0, 255);
	//sprintf(IniWrite, "%d,", parameters_setting_trans.cameraD_point_area_min);
	GetPrivateProfileString("DetectParameter", "cameraD_point_area_min", "", IniRead, 24, IniPath);
	parameters_setting_trans.cameraD_point_area_min = atoi(IniRead);

	//IniWrite.Format("%d", parameters_setting_trans.cameraD_point_area_max);
	memset(IniRead, 0, 255);
	//sprintf(IniWrite, "%d,", parameters_setting_trans.cameraD_point_area_max);
	GetPrivateProfileString("DetectParameter", "cameraD_point_area_max", "", IniRead, 24, IniPath);
	parameters_setting_trans.cameraD_point_area_max = atoi(IniRead);

	//IniWrite.Format("%d", parameters_setting_trans.cameraD_point_length_min);
	memset(IniRead, 0, 255);
	//sprintf(IniWrite, "%d,", parameters_setting_trans.cameraD_point_length_min);
	GetPrivateProfileString("DetectParameter", "cameraD_point_length_min", "", IniRead, 24, IniPath);
	parameters_setting_trans.cameraD_point_length_min = atoi(IniRead);

	//IniWrite.Format("%d", parameters_setting_trans.cameraD_point_length_max);
	memset(IniRead, 0, 255);
	//sprintf(IniWrite, "%d,", parameters_setting_trans.cameraD_point_length_max);
	GetPrivateProfileString("DetectParameter", "cameraD_point_length_max", "", IniRead, 24, IniPath);
	parameters_setting_trans.cameraD_point_length_max = atoi(IniRead);

	//IniWrite.Format("%d", parameters_setting_trans.cameraD_point_width_min);
	memset(IniRead, 0, 255);
	//sprintf(IniWrite, "%d,", parameters_setting_trans.cameraD_point_width_min);
	GetPrivateProfileString("DetectParameter", "cameraD_point_width_min", "", IniRead, 24, IniPath);
	parameters_setting_trans.cameraD_point_width_min = atoi(IniRead);

	//IniWrite.Format("%d", parameters_setting_trans.cameraD_point_width_max);
	memset(IniRead, 0, 255);
	//sprintf(IniWrite, "%d,", parameters_setting_trans.cameraD_point_width_max);
	GetPrivateProfileString("DetectParameter", "cameraD_point_width_max", "", IniRead, 24, IniPath);
	parameters_setting_trans.cameraD_point_width_max = atoi(IniRead);

	//IniWrite.Format("%d", parameters_setting_trans.cameraD_thread_area_min);
	memset(IniRead, 0, 255);
	//sprintf(IniWrite, "%d,", parameters_setting_trans.cameraD_thread_area_min);
	GetPrivateProfileString("DetectParameter", "cameraD_thread_area_min", "", IniRead, 24, IniPath);
	parameters_setting_trans.cameraD_thread_area_min = atoi(IniRead);

	//IniWrite.Format("%d", parameters_setting_trans.cameraD_thread_area_max);
	memset(IniRead, 0, 255);
	//sprintf(IniWrite, "%d,", parameters_setting_trans.cameraD_thread_area_max);
	GetPrivateProfileString("DetectParameter", "cameraD_thread_area_max", "", IniRead, 24, IniPath);
	parameters_setting_trans.cameraD_thread_area_max = atoi(IniRead);

	//IniWrite.Format("%d", parameters_setting_trans.cameraD_thread_length_min);
	memset(IniRead, 0, 255);
	//sprintf(IniWrite, "%d,", parameters_setting_trans.cameraD_thread_length_min);
	GetPrivateProfileString("DetectParameter", "cameraD_thread_length_min", "", IniRead, 24, IniPath);
	parameters_setting_trans.cameraD_thread_length_min = atoi(IniRead);

	//IniWrite.Format("%d", parameters_setting_trans.cameraD_thread_length_max);
	memset(IniRead, 0, 255);
	//sprintf(IniWrite, "%d,", parameters_setting_trans.cameraD_thread_length_max);
	GetPrivateProfileString("DetectParameter", "cameraD_thread_length_max", "", IniRead, 24, IniPath);
	parameters_setting_trans.cameraD_thread_length_max = atoi(IniRead);

	//IniWrite.Format("%d", parameters_setting_trans.cameraD_thread_width_min);
	memset(IniRead, 0, 255);
	//sprintf(IniWrite, "%d,", parameters_setting_trans.cameraD_thread_width_min);
	GetPrivateProfileString("DetectParameter", "cameraD_thread_width_min", "", IniRead, 24, IniPath);
	parameters_setting_trans.cameraD_thread_width_min = atoi(IniRead);

	//IniWrite.Format("%d", parameters_setting_trans.cameraD_thread_width_max);
	memset(IniRead, 0, 255);
	//sprintf(IniWrite, "%d,", parameters_setting_trans.cameraD_thread_width_max);
	GetPrivateProfileString("DetectParameter", "cameraD_thread_width_max", "", IniRead, 24, IniPath);
	parameters_setting_trans.cameraD_thread_width_max = atoi(IniRead);


	//DebugParaneters setting;
	//IniWrite.Format("%d", parameters_setting_trans.debug_cameraA);
	memset(IniRead, 0, 255);
	//sprintf(IniWrite, "%d,", parameters_setting_trans.debug_cameraA);
	GetPrivateProfileString("DebugParameter", "debug_cameraA", "", IniRead, 24, IniPath);
	parameters_setting_trans.debug_cameraA = atoi(IniRead);

	//IniWrite.Format("%d", parameters_setting_trans.debug_cameraB);
	memset(IniRead, 0, 255);
	//sprintf(IniWrite, "%d,", parameters_setting_trans.debug_cameraB);
	GetPrivateProfileString("DebugParameter", "debug_cameraB", "", IniRead, 24, IniPath);
	parameters_setting_trans.debug_cameraB = atoi(IniRead);

	//IniWrite.Format("%d", parameters_setting_trans.debug_cameraC);
	memset(IniRead, 0, 255);
	//sprintf(IniWrite, "%d,", parameters_setting_trans.debug_cameraC);
	GetPrivateProfileString("DebugParameter", "debug_cameraC", "", IniRead, 24, IniPath);
	parameters_setting_trans.debug_cameraC = atoi(IniRead);

	//IniWrite.Format("%d", parameters_setting_trans.debug_cameraD);
	memset(IniRead, 0, 255);
	//sprintf(IniWrite, "%d,", parameters_setting_trans.debug_cameraD);
	GetPrivateProfileString("DebugParameter", "debug_cameraD", "", IniRead, 24, IniPath);
	parameters_setting_trans.debug_cameraD = atoi(IniRead);

	//IniWrite.Format("%d", parameters_setting_trans.stop_volve);
	memset(IniRead, 0, 255);
	//sprintf(IniWrite, "%d,", parameters_setting_trans.stop_volve);
	GetPrivateProfileString("DebugParameter", "stop_volve", "", IniRead, 24, IniPath);
	parameters_setting_trans.stop_volve = atoi(IniRead);

	//IniWrite.Format("%d", parameters_setting_trans.save_blob);
	memset(IniRead, 0, 255);
	//sprintf(IniWrite, "%d,", parameters_setting_trans.save_blob);
	GetPrivateProfileString("DebugParameter", "save_blob", "", IniRead, 24, IniPath);
	parameters_setting_trans.save_blob = atoi(IniRead);
	//****************************End read the fiber detect parameters form ini files************************
	parameters_setting_trans.signalling = 8;    //To set the camera and box device parameters settings;

	Vsee::VCameraMessageTrans *frame = new Vsee::VCameraMessageTrans;
	memcpy(&frame->parameters_setting_trans, &parameters_setting_trans, sizeof(parameters_setting_trans));
	frame->signalling = 4;
	::PostMessage(AfxGetMainWnd()->m_hWnd, ID_SHOWCTRLMESSAGE, frame->signalling, (LPARAM)frame);
}


void CFiberDetectControlDlg::OnBnClickedButtonParametersSetting()
{
	// TODO:  在此添加控件通知处理程序代码
	GetDlgItem(IDC_STATIC_IMAGE_SHOW)->ShowWindow(true);
	GetDlgItem(IDC_STATIC_IMAGE_SHOW1)->ShowWindow(false);
	GetDlgItem(IDC_STATIC_IMAGE_SHOW2)->ShowWindow(false);
	GetDlgItem(IDC_STATIC_IMAGE_SHOW3)->ShowWindow(false);
	GetDlgItem(IDC_STATIC_IMAGE_SHOW4)->ShowWindow(false);
	m_bShowInfoWindow = true;
	m_bShowImageWidhow = false;
	CDetectParametersSetting dlg;
	if (dlg.DoModal() != IDOK)
	{
		//GetDlgItem(IDC_STATIC_IMAGE_SHOW)->ShowWindow(false);
		//GetDlgItem(IDC_STATIC_IMAGE_SHOW1)->ShowWindow(true);
		//GetDlgItem(IDC_STATIC_IMAGE_SHOW2)->ShowWindow(true);
		//GetDlgItem(IDC_STATIC_IMAGE_SHOW3)->ShowWindow(true);
		//GetDlgItem(IDC_STATIC_IMAGE_SHOW4)->ShowWindow(true);
		return;
	}

	Vsee::VCameraMessageTrans *frame = new Vsee::VCameraMessageTrans;
	memcpy(&frame->parameters_setting_trans, &dlg.parameters_setting_trans, sizeof(dlg.parameters_setting_trans));
	frame->signalling = 4;
	::PostMessage(AfxGetMainWnd()->m_hWnd, ID_SHOWCTRLMESSAGE, frame->signalling, (LPARAM)frame);

	//GetDlgItem(IDC_STATIC_IMAGE_SHOW)->ShowWindow(false);
	//GetDlgItem(IDC_STATIC_IMAGE_SHOW1)->ShowWindow(true);
	//GetDlgItem(IDC_STATIC_IMAGE_SHOW2)->ShowWindow(true);
	//GetDlgItem(IDC_STATIC_IMAGE_SHOW3)->ShowWindow(true);
	//GetDlgItem(IDC_STATIC_IMAGE_SHOW4)->ShowWindow(true);
}


void CFiberDetectControlDlg::run_tcp_server(std::string _ip, int _port)
{
	std::thread t(&CFiberDetectControlDlg::tcp_server, this, _ip, _port);
	t.detach();
}



void CFiberDetectControlDlg::tcp_server(std::string _ip, int _port)
{

	std::string error_str;
	if (!communicator_->server(_ip, _port, error_str))	//Start a Server state machine,to server for all the net request from client,
	{													//here the the function "server()" actuall is an endless loop.
		std::cerr << error_str << std::endl;
		return;
	}
}


void CFiberDetectControlDlg::run_tcp_img_show_server(std::string _ip, int _port)
{
	if (_port == 9000)
	{
		std::thread t(&CFiberDetectControlDlg::tcp_img_show_server, this, _ip, _port);
		t.detach();
	}
	else if (_port == 5555)
	{
		std::thread t(&CFiberDetectControlDlg::tcp_img_show_server1, this, _ip, _port);
		t.detach();
	}
}

void CFiberDetectControlDlg::tcp_img_show_server(std::string _ip, int _port)
{

	std::string error_str;
	if (!communicator_->img_show_server(_ip, _port, error_str))	//Start a Server state machine,to server for all the net request from client,
	{													//here the the function "server()" actuall is an endless loop.
		std::cerr << error_str << std::endl;
		return;
	}
	//else if (_port == 9005)
	//{
	//	if (!communicator_->img_show_server1(_ip, _port, error_str))	//Start a Server state machine,to server for all the net request from client,
	//	{													//here the the function "server()" actuall is an endless loop.
	//		std::cerr << error_str << std::endl;
	//		return;
	//	}
	//}
	
}
void CFiberDetectControlDlg::tcp_img_show_server1(std::string _ip, int _port)
{

	std::string error_str;
	if (!communicator_->img_show_server1(_ip, _port, error_str))	//Start a Server state machine,to server for all the net request from client,
	{													//here the the function "server()" actuall is an endless loop.
		std::cerr << error_str << std::endl;
		return;
	}

}

void CFiberDetectControlDlg::run_tcp_client(std::string _ip, int _port)
{
	//ios[0] = new io_service(1);
	//sockets[0] = new tcp::socket(*ios[0]);
	//eps[0] = tcp::endpoint(address::from_string(_ip), _port);
	////asio::io_service* io;
	//std::thread t(&CDataSimulatorDlg::tcp_client, this, _ip, _port, ios[0]);
	//t.detach();
	int nsize = sizeof(std::size_t);
	nsize = sizeof(int);
	nsize = sizeof(std::string);

	communicator_->run_tcp_connect(_ip, _port);
}

void CFiberDetectControlDlg::tcp_client(std::string _ip, int port, io_service* io)
{
	while (true)
	{
		//connect_and_reconnect(_ip, _port, io);
		connect_and_reconnect(0, 1, *io);
		io->run();			//只有在IO.run()运行后，socket才开始起启动
		//	sessions[0]->startSession();

	}
}


bool  CFiberDetectControlDlg::connect_and_reconnect(int beg, int end, asio::io_service& io)
{
	for (int i = beg; i < end; ++i)
	{
		auto socket = sockets[i];

		if (!sessions[i])		//如果第一次执行sessions[i]还没有初始化，则进行如下的初始化操作
		{						//如果不是第一次执行，则下述代码不执行
			sessions[i] = new Vsee::VTx2Session(*socket);

			sessions[i]->setProcessFrameFunc([&, i](VCameraFrame&& frame) //将sessions[i]->_process_frame_func设置为以下{ ...}中的执行代码,但仅仅是定义_process_frame_func，
			{								 //在此时并不执行_process_frame_func()函数，需在VCameraSession::asyncRecv()->VTx2Session::ProcessMessage()函数被调用时才会执行

				//MutexLock lock(infer_mutex);
				infer_queue.push(InferFrame(sessions[i], std::move(frame)));//定义一个InferFrame,InferFrame._session=sessions[i],InferFrame._frame=frame,并将InferFrame;
				//push进infer_queue中
				VCameraFrame _frame;
			});

			sessions[i]->setSessionAbortedFunc([&, i, beg, end]()
			{

				connect_and_reconnect(beg, end, io);		//这里用嵌套设计，在connect_and_reconnect（）执行完后面的代码还是要继续执行的
			});
		}

		if (socket->is_open())//如果socket已经打开并连接，则忽略以后代码，进入无限循环
			continue;

		asio::error_code ec;
		socket->cancel(ec);

		socket->async_connect(eps[i], [&, i, beg, end](asio::error_code ec)//第一次执行，进行socket的连接操作
		{
			if (ec)//执行socket->cancel(ec)出错，socket还没初始化好
			{
				connect_and_reconnect(beg, end, io);
				return;
			}

			sessions[i]->startSession();
		});
	}
	return true;
}
/*
功能：获得当前程序的运行目录(.exe)
*/
CString CFiberDetectControlDlg::GetModulePath(void)
{
	CString    sPath;
	GetModuleFileNameA(NULL, sPath.GetBufferSetLength(MAX_PATH + 1), MAX_PATH);
	sPath.ReleaseBuffer();
	int    nPos;
	nPos = sPath.ReverseFind('\\');
	sPath = sPath.Left(nPos);
	return   sPath;
}


void CFiberDetectControlDlg::ReadIni()
{
	int camera_total;
	char IniRead[255];
	memset(IniRead, 0, 255);
	char IniPath[255];
	memset(IniPath, 0, 255);
	locale china("chs");//use china characte
	//camera_ip_info ci_info;
	CString strPath = GetModulePath();
	strcpy_s(IniPath, 255, strPath.GetBuffer());
	strcat_s(IniPath, "/param.ini");
	//std::string model_type[6];
	CString str;
	int nWriteLog;

	GetPrivateProfileStringA("System", "write_log", "", IniRead, 255, IniPath);
	nWriteLog = atoi(IniRead);
	communicator_->m_bWriteLog = nWriteLog;
	GetPrivateProfileStringA("System", "model1_text", "", IniRead, 255, IniPath);
	model_type_text[0] = IniRead;
	GetPrivateProfileStringA("System", "model2_text", "", IniRead, 255, IniPath);
	model_type_text[1] = IniRead;
	GetPrivateProfileStringA("System", "model3_text", "", IniRead, 255, IniPath);
	model_type_text[2] = IniRead;
	GetPrivateProfileStringA("System", "model4_text", "", IniRead, 255, IniPath);
	model_type_text[3] = IniRead;
	GetPrivateProfileStringA("System", "model5_text", "", IniRead, 255, IniPath);
	model_type_text[4] = IniRead;
	GetPrivateProfileStringA("System", "model4_text", "", IniRead, 255, IniPath);
	model_type_text[5] = IniRead;
	
	
	GetPrivateProfileStringA("System", "model1", "", IniRead, 100, IniPath);
	model_type[0] = IniRead;
	//m_dlgNetworkParameters.GetDlgItem(IDC_EDIT_IP_ADRESS)->SetWindowTextA(IniRead);
	GetPrivateProfileStringA("System", "model2", "", IniRead, 100, IniPath);
	model_type[1] = IniRead;
	GetPrivateProfileStringA("System", "model3", "", IniRead, 100, IniPath);
	model_type[2] = IniRead;
	GetPrivateProfileStringA("System", "model4", "", IniRead, 100, IniPath);
	model_type[3] = IniRead;
	GetPrivateProfileStringA("System", "model5", "", IniRead, 100, IniPath);
	model_type[4] = IniRead;
	GetPrivateProfileStringA("System", "model6", "", IniRead, 100, IniPath);
	model_type[5] = IniRead;
	GetPrivateProfileStringA("System", "model_select", "", IniRead, 100, IniPath);
	m_nModleSelect = atoi(IniRead);
	parameters_setting_trans.model_select = m_nModleSelect;
	//m_dlgNetworkParameters.GetDlgItem(IDC_EDIT_IP_PORT_NUMBER)->SetWindowTextA(IniRead);

	parameters_setting_trans.signalling = 1;     //Use siggnalling = 1 to indicate send the network setting command;

	GetPrivateProfileStringA("System", "box_num", "", IniRead, 24, IniPath);
	parameters_setting_trans.box_num = atoi(IniRead);

	GetPrivateProfileStringA("System", "box1_ip", "", IniRead, 24, IniPath);
	strcpy(parameters_setting_trans.box1_ip , IniRead);
	GetPrivateProfileStringA("System", "box1_port", "", IniRead, 24, IniPath);
	parameters_setting_trans.box1_port = atoi(IniRead);

	GetPrivateProfileStringA("System", "box2_ip", "", IniRead, 24, IniPath);
	strcpy(parameters_setting_trans.box2_ip , IniRead);
	GetPrivateProfileStringA("System", "box2_port", "", IniRead, 24, IniPath);
	parameters_setting_trans.box2_port = atoi(IniRead);

	GetPrivateProfileStringA("System", "electric_board_ip", "", IniRead, 24, IniPath);
	strcpy(parameters_setting_trans.electric_board_ip , IniRead);
	GetPrivateProfileStringA("System", "electric_board_port", "", IniRead, 24, IniPath);
	parameters_setting_trans.electric_board_port = atoi(IniRead);

	GetPrivateProfileStringA("System", "local_ip", "", IniRead, 24, IniPath);
	strcpy(parameters_setting_trans.local_ip , IniRead);
	GetPrivateProfileStringA("System", "local_port", "", IniRead, 24, IniPath);
	parameters_setting_trans.local_port = atoi(IniRead);

	GetPrivateProfileStringA("CameraSetting", "cameraA_ip", "", IniRead, 24, IniPath);
	strcpy(parameters_setting_trans.cameraA_ip , IniRead);
	GetPrivateProfileStringA("CameraSetting", "cameraA_port", "", IniRead, 24, IniPath);
	parameters_setting_trans.cameraA_port = atoi(IniRead);
	GetPrivateProfileStringA("CameraSetting", "cameraA_scan_speed", "", IniRead, 24, IniPath);
	parameters_setting_trans.cameraA_scan_speed = atoi(IniRead);
	GetPrivateProfileStringA("CameraSetting", "cameraA_tirgging_mode", "", IniRead, 24, IniPath);
	parameters_setting_trans.cameraA_tirgging_mode = atoi(IniRead);
	GetPrivateProfileStringA("CameraSetting", "cameraA_exposure_time", "", IniRead, 24, IniPath);
	parameters_setting_trans.cameraA_exposure_time = atoi(IniRead);
	GetPrivateProfileStringA("CameraSetting", "cameraA_gain", "", IniRead, 24, IniPath);
	parameters_setting_trans.cameraA_gain = atoi(IniRead);
	GetPrivateProfileStringA("CameraSetting", "cameraA_width", "", IniRead, 24, IniPath);
	parameters_setting_trans.cameraA_width = atoi(IniRead);
	GetPrivateProfileStringA("CameraSetting", "cameraA_height", "", IniRead, 24, IniPath);
	parameters_setting_trans.cameraA_height = atoi(IniRead);
	GetPrivateProfileStringA("CameraSetting", "cameraA_mac", "", IniRead, 24, IniPath);
	strcpy(parameters_setting_trans.cameraA_mac , IniRead);

	GetPrivateProfileStringA("CameraSetting", "cameraB_ip", "", IniRead, 24, IniPath);
	strcpy(parameters_setting_trans.cameraB_ip , IniRead);
	GetPrivateProfileStringA("CameraSetting", "cameraB_port", "", IniRead, 24, IniPath);
	parameters_setting_trans.cameraB_port = atoi(IniRead);
	GetPrivateProfileStringA("CameraSetting", "cameraB_scan_speed", "", IniRead, 24, IniPath);
	parameters_setting_trans.cameraB_scan_speed = atoi(IniRead);
	GetPrivateProfileStringA("CameraSetting", "cameraB_tirgging_mode", "", IniRead, 24, IniPath);
	parameters_setting_trans.cameraB_tirgging_mode = atoi(IniRead);
	GetPrivateProfileStringA("CameraSetting", "cameraB_exposure_time", "", IniRead, 24, IniPath);
	parameters_setting_trans.cameraB_exposure_time = atoi(IniRead);
	GetPrivateProfileStringA("CameraSetting", "cameraB_gain", "", IniRead, 24, IniPath);
	parameters_setting_trans.cameraB_gain = atoi(IniRead);
	GetPrivateProfileStringA("CameraSetting", "cameraB_width", "", IniRead, 24, IniPath);
	parameters_setting_trans.cameraB_width = atoi(IniRead);
	GetPrivateProfileStringA("CameraSetting", "cameraB_height", "", IniRead, 24, IniPath);
	parameters_setting_trans.cameraB_height = atoi(IniRead);
	GetPrivateProfileStringA("CameraSetting", "cameraB_mac", "", IniRead, 24, IniPath);
	strcpy(parameters_setting_trans.cameraB_mac, IniRead);

	GetPrivateProfileStringA("CameraSetting", "cameraC_ip", "", IniRead, 24, IniPath);
	strcpy(parameters_setting_trans.cameraC_ip , IniRead);
	GetPrivateProfileStringA("CameraSetting", "cameraC_port", "", IniRead, 24, IniPath);
	parameters_setting_trans.cameraC_port = atoi(IniRead);
	GetPrivateProfileStringA("CameraSetting", "cameraC_scan_speed", "", IniRead, 24, IniPath);
	parameters_setting_trans.cameraC_scan_speed = atoi(IniRead);
	GetPrivateProfileStringA("CameraSetting", "cameraC_tirgging_mode", "", IniRead, 24, IniPath);
	parameters_setting_trans.cameraC_tirgging_mode = atoi(IniRead);
	GetPrivateProfileStringA("CameraSetting", "cameraC_exposure_time", "", IniRead, 24, IniPath);
	parameters_setting_trans.cameraC_exposure_time = atoi(IniRead);
	GetPrivateProfileStringA("CameraSetting", "cameraC_gain", "", IniRead, 24, IniPath);
	parameters_setting_trans.cameraC_gain = atoi(IniRead);
	GetPrivateProfileStringA("CameraSetting", "cameraC_width", "", IniRead, 24, IniPath);
	parameters_setting_trans.cameraC_width = atoi(IniRead);
	GetPrivateProfileStringA("CameraSetting", "cameraC_height", "", IniRead, 24, IniPath);
	parameters_setting_trans.cameraC_height = atoi(IniRead);
	GetPrivateProfileStringA("CameraSetting", "cameraC_mac", "", IniRead, 24, IniPath);
	strcpy(parameters_setting_trans.cameraC_mac, IniRead);

	GetPrivateProfileStringA("CameraSetting", "cameraD_ip", "", IniRead, 24, IniPath);
	strcpy(parameters_setting_trans.cameraD_ip , IniRead);
	GetPrivateProfileStringA("CameraSetting", "cameraD_port", "", IniRead, 24, IniPath);
	parameters_setting_trans.cameraD_port = atoi(IniRead);
	GetPrivateProfileStringA("CameraSetting", "cameraD_scan_speed", "", IniRead, 24, IniPath);
	parameters_setting_trans.cameraD_scan_speed = atoi(IniRead);
	GetPrivateProfileStringA("CameraSetting", "cameraD_tirgging_mode", "", IniRead, 24, IniPath);
	parameters_setting_trans.cameraD_tirgging_mode = atoi(IniRead);
	GetPrivateProfileStringA("CameraSetting", "cameraD_exposure_time", "", IniRead, 24, IniPath);
	parameters_setting_trans.cameraD_exposure_time = atoi(IniRead);
	GetPrivateProfileStringA("CameraSetting", "cameraD_gain", "", IniRead, 24, IniPath);
	parameters_setting_trans.cameraD_gain = atoi(IniRead);
	GetPrivateProfileStringA("CameraSetting", "cameraD_width", "", IniRead, 24, IniPath);
	parameters_setting_trans.cameraD_width = atoi(IniRead);
	GetPrivateProfileStringA("CameraSetting", "cameraD_height", "", IniRead, 24, IniPath);
	parameters_setting_trans.cameraD_height = atoi(IniRead);
	GetPrivateProfileStringA("CameraSetting", "cameraD_mac", "", IniRead, 24, IniPath);
	strcpy(parameters_setting_trans.cameraD_mac, IniRead);

	return;

}


void CFiberDetectControlDlg::OnBnClickedStartServer()
{
	// TODO:  在此添加控件通知处理程序代码
	m_bClientMode = m_checkClientMode.GetCheck();
	if (1)			//To get the latest parameters setting form param.ini file.
	{
		int camera_total;
		char IniRead[255];
		memset(IniRead, 0, 255);
		char IniPath[255];
		memset(IniPath, 0, 255);
		//camera_ip_info ci_info;
		CString strPath = GetModulePath();
		strcpy_s(IniPath, 255, strPath.GetBuffer());
		strcat_s(IniPath, "/param.ini");
		GetPrivateProfileStringA("System", "box1_ip", "", IniRead, 24, IniPath);
		//m_dlgNetworkParameters.GetDlgItem(IDC_EDIT_IP_ADRESS)->SetWindowTextA(IniRead);
		GetPrivateProfileStringA("System", "box1_port", "", IniRead, 24, IniPath);
		//m_dlgNetworkParameters.GetDlgItem(IDC_EDIT_IP_PORT_NUMBER)->SetWindowTextA(IniRead);

		parameters_setting_trans.signalling = 1;     //Use siggnalling = 1 to indicate send the network setting command;

		GetPrivateProfileStringA("System", "box1_ip", "", IniRead, 24, IniPath);
		strcpy(parameters_setting_trans.box1_ip ,IniRead);
		GetPrivateProfileStringA("System", "box1_port", "", IniRead, 24, IniPath);
		parameters_setting_trans.box1_port = atoi(IniRead);

		GetPrivateProfileStringA("System", "box2_ip", "", IniRead, 24, IniPath);
		strcpy(parameters_setting_trans.box2_ip , IniRead);
		GetPrivateProfileStringA("System", "box2_port", "", IniRead, 24, IniPath);
		parameters_setting_trans.box2_port = atoi(IniRead);

		GetPrivateProfileStringA("System", "electric_board_ip", "", IniRead, 24, IniPath);
		strcpy(parameters_setting_trans.electric_board_ip , IniRead);
		GetPrivateProfileStringA("System", "electric_board_port", "", IniRead, 24, IniPath);
		parameters_setting_trans.electric_board_port = atoi(IniRead);

		GetPrivateProfileStringA("System", "local_ip", "", IniRead, 24, IniPath);
		strcpy(parameters_setting_trans.local_ip , IniRead);
		GetPrivateProfileStringA("System", "local_port", "", IniRead, 24, IniPath);
		parameters_setting_trans.local_port = atoi(IniRead);

		GetPrivateProfileStringA("CameraSetting", "cameraA_ip", "", IniRead, 24, IniPath);
		strcpy(parameters_setting_trans.cameraA_ip , IniRead);
		GetPrivateProfileStringA("CameraSetting", "cameraA_port", "", IniRead, 24, IniPath);
		parameters_setting_trans.cameraA_port = atoi(IniRead);
		GetPrivateProfileStringA("CameraSetting", "cameraA_scan_speed", "", IniRead, 24, IniPath);
		parameters_setting_trans.cameraA_scan_speed = atoi(IniRead);
		GetPrivateProfileStringA("CameraSetting", "cameraA_tirgging_mode", "", IniRead, 24, IniPath);
		parameters_setting_trans.cameraA_tirgging_mode = atoi(IniRead);
		GetPrivateProfileStringA("CameraSetting", "cameraA_exposure_time", "", IniRead, 24, IniPath);
		parameters_setting_trans.cameraA_exposure_time = atoi(IniRead);
		GetPrivateProfileStringA("CameraSetting", "cameraA_gain", "", IniRead, 24, IniPath);
		parameters_setting_trans.cameraA_gain = atoi(IniRead);

		GetPrivateProfileStringA("CameraSetting", "cameraB_ip", "", IniRead, 24, IniPath);
		strcpy(parameters_setting_trans.cameraB_ip , IniRead);
		GetPrivateProfileStringA("CameraSetting", "cameraB_port", "", IniRead, 24, IniPath);
		parameters_setting_trans.cameraB_port = atoi(IniRead);
		GetPrivateProfileStringA("CameraSetting", "cameraB_scan_speed", "", IniRead, 24, IniPath);
		parameters_setting_trans.cameraB_scan_speed = atoi(IniRead);
		GetPrivateProfileStringA("CameraSetting", "cameraB_tirgging_mode", "", IniRead, 24, IniPath);
		parameters_setting_trans.cameraB_tirgging_mode = atoi(IniRead);
		GetPrivateProfileStringA("CameraSetting", "cameraB_exposure_time", "", IniRead, 24, IniPath);
		parameters_setting_trans.cameraB_exposure_time = atoi(IniRead);
		GetPrivateProfileStringA("CameraSetting", "cameraB_gain", "", IniRead, 24, IniPath);
		parameters_setting_trans.cameraB_gain = atoi(IniRead);

		GetPrivateProfileStringA("CameraSetting", "cameraC_ip", "", IniRead, 24, IniPath);
		strcpy(parameters_setting_trans.cameraC_ip , IniRead);
		GetPrivateProfileStringA("CameraSetting", "cameraC_port", "", IniRead, 24, IniPath);
		parameters_setting_trans.cameraC_port = atoi(IniRead);
		GetPrivateProfileStringA("CameraSetting", "cameraC_scan_speed", "", IniRead, 24, IniPath);
		parameters_setting_trans.cameraC_scan_speed = atoi(IniRead);
		GetPrivateProfileStringA("CameraSetting", "cameraC_tirgging_mode", "", IniRead, 24, IniPath);
		parameters_setting_trans.cameraC_tirgging_mode = atoi(IniRead);
		GetPrivateProfileStringA("CameraSetting", "cameraC_exposure_time", "", IniRead, 24, IniPath);
		parameters_setting_trans.cameraC_exposure_time = atoi(IniRead);
		GetPrivateProfileStringA("CameraSetting", "cameraC_gain", "", IniRead, 24, IniPath);
		parameters_setting_trans.cameraC_gain = atoi(IniRead);

		GetPrivateProfileStringA("CameraSetting", "cameraD_ip", "", IniRead, 24, IniPath);
		strcpy(parameters_setting_trans.cameraD_ip , IniRead);
		GetPrivateProfileStringA("CameraSetting", "cameraD_port", "", IniRead, 24, IniPath);
		parameters_setting_trans.cameraD_port = atoi(IniRead);
		GetPrivateProfileStringA("CameraSetting", "cameraD_scan_speed", "", IniRead, 24, IniPath);
		parameters_setting_trans.cameraD_scan_speed = atoi(IniRead);
		GetPrivateProfileStringA("CameraSetting", "cameraD_tirgging_mode", "", IniRead, 24, IniPath);
		parameters_setting_trans.cameraD_tirgging_mode = atoi(IniRead);
		GetPrivateProfileStringA("CameraSetting", "cameraD_exposure_time", "", IniRead, 24, IniPath);
		parameters_setting_trans.cameraD_exposure_time = atoi(IniRead);
		GetPrivateProfileStringA("CameraSetting", "cameraD_gain", "", IniRead, 24, IniPath);
		parameters_setting_trans.cameraD_gain = atoi(IniRead);
	}
	if (!m_bClientMode)
	{
		run_tcp_server(parameters_setting_trans.local_ip, parameters_setting_trans.local_port);   //to start a server mode;
		//run_udp_server(parameters_setting_trans.electric_board_ip, parameters_setting_trans.electric_board_port);   //to start a server mode;
		//run_valve_server();
	}

	else
		run_tcp_client(parameters_setting_trans.local_ip, parameters_setting_trans.local_port);   //to start a client mode;
	run_tcp_img_show_server(parameters_setting_trans.local_ip, 9000);//mode 0
	CString cs_str;
	string str_str;
	str_str = cs_str;
	Point P1, P2, P0, P3;
	Point pRect1, pRect2;
	POINT  pTopLeft, pRightBottom;
	pTopLeft.x = 5000; pTopLeft.y = 5000; pRightBottom.x = 0; pRightBottom.y = 0;
	CString strText = "启动服务";
	CRect DrawRect(0, 0, 600, 50);
	RefreshWnd(m_pwnd);
	PushTailShowText(strText);
	//ShowText(m_pwnd, m_strShowText, DrawRect);
	ShowText(m_pwnd, m_strShowText);
}


void    CFiberDetectControlDlg::PushTailShowText(CString strText)
{
	int i = 0;
	bool bPush = false;
	for (i = 0; i < SHOWTEXTLENGTH; i++)
	{
		if (m_strShowText[i][0] == '\0')
		{
			strcpy(m_strShowText[i], strText);
			bPush = true;
			break;
		}
	}
	if (bPush)
		return;
	else
	{
		for (i = 0; i < SHOWTEXTLENGTH-1; i++)
		{
			strcpy(m_strShowText[i], m_strShowText[i + 1]);
		}
		strcpy(m_strShowText[i], strText);
		return;
	}
}


void CFiberDetectControlDlg::OnBnClickedCheckServerModeSelect()
{
	// TODO:  在此添加控件通知处理程序代码
	int n = m_checkServerModeSelect.GetCheck();
	if (m_checkServerModeSelect.GetCheck() == 1)
	{
		m_bServerMode = true;
		m_bClientMode = false;
		m_checkServerModeSelect.SetCheck(m_bServerMode);
		m_checkClientMode.SetCheck(m_bClientMode);
	}
	else
	{
		m_bServerMode = false;
		m_bClientMode = true;
		m_checkServerModeSelect.SetCheck(m_bServerMode);
		m_checkClientMode.SetCheck(m_bClientMode);
	}
}


void CFiberDetectControlDlg::OnBnClickedCheckModeSelect() //Client mode set
{
	// TODO:  在此添加控件通知处理程序代码
	int n = m_checkClientMode.GetCheck();
	//int m_checkServerModeSelect.GetCheck();

	if (m_checkClientMode.GetCheck() == 1)
	{
		m_bServerMode = false;
		m_bClientMode = true;
		m_checkServerModeSelect.SetCheck(m_bServerMode);
		m_checkClientMode.SetCheck(m_bClientMode);
	}
	else
	{
		m_bServerMode = true;
		m_bClientMode = false;
		m_checkServerModeSelect.SetCheck(m_bServerMode);
		m_checkClientMode.SetCheck(m_bClientMode);
	}
}


void CFiberDetectControlDlg::OnBnClickedCheckServerSend()
{
	// TODO:  在此添加控件通知处理程序代码
	if (m_checkServerSend.GetCheck() == 1)
	{
		m_bServerSendMode = true;
		m_bClientSendMode = false;
		communicator_->m_bServerSendMode = m_bServerSendMode;
		communicator_->m_bClientSendMode = m_bClientSendMode;
		m_checkServerSend.SetCheck(m_bServerSendMode);
		m_checkClientSend.SetCheck(m_bClientSendMode);
	}
	else
	{
		m_bServerSendMode = false;
		m_bClientSendMode = true;
		communicator_->m_bServerSendMode = m_bServerSendMode;
		communicator_->m_bClientSendMode = m_bClientSendMode;
		m_checkServerSend.SetCheck(m_bServerSendMode);
		m_checkClientSend.SetCheck(m_bClientSendMode);
	}
}


void CFiberDetectControlDlg::OnBnClickedCheckClientSend()
{
	// TODO:  在此添加控件通知处理程序代码
	if (m_checkClientSend.GetCheck() == 1)
	{
		m_bClientSendMode = true;
		m_bServerSendMode = false;
		communicator_->m_bServerSendMode = m_bServerSendMode;
		communicator_->m_bClientSendMode = m_bClientSendMode;
		m_checkClientSend.SetCheck(m_bClientSendMode);
		m_checkServerSend.SetCheck(m_bServerSendMode);
	}
	else
	{
		m_bClientSendMode = false;
		m_bServerSendMode = true;
		communicator_->m_bServerSendMode = m_bServerSendMode;
		communicator_->m_bClientSendMode = m_bClientSendMode;
		m_checkClientSend.SetCheck(m_bClientSendMode);
		m_checkServerSend.SetCheck(m_bServerSendMode);
	}
}


void CFiberDetectControlDlg::ShowText(CWnd *pwnd, CString strText, CRect DrawRect)
{
	
		//CWnd *pwnd = GetDlgItem(IDC_ShowImage);
		RefreshWnd(pwnd);   //clear the print words from the show window;
		CClientDC pDc(pwnd);
		CPen pen(PS_SOLID, 5, RGB(255, 0, 0));//创建一个画笔工具	
		m_nRectWidth = m_ImageRect.Width();
		m_nRectHeight = m_ImageRect.Height();

		pDc.SetTextColor(RGB(0, 255, 0));
		//	pDc.DrawText(strText, DrawRect, DT_SINGLELINE | DT_LEFT | DT_VCENTER); //显示文本

		CFont font;

		font.CreateFont(
			20, // nHeight
			0, // nWidth
			0, // nEscapement
			0, // nOrientation
			FW_NORMAL, // nWeight
			FALSE, // bItalic
			FALSE, // bUnderline
			0, // cStrikeOut
			ANSI_CHARSET, // nCharSet
			OUT_DEFAULT_PRECIS, // nOutPrecision
			CLIP_DEFAULT_PRECIS, // nClipPrecision
			DEFAULT_QUALITY, // nQuality
			DEFAULT_PITCH | FF_SWISS,
			_T("Arial") // nPitchAndFamily Arial
			);
		pDc.SelectObject(&font);
		pDc.SetBkMode(TRANSPARENT);


		pDc.SetTextColor(RGB(0, 0, 0));

		pDc.SetBkColor(RGB(255, 255, 0));
		//pDc.TextOut(0, 0, "写字");
		char str[5][255];
		memset(str[0], 0, 255);
		strcpy(str[0], strText);

		pDc.DrawText(str[0], DrawRect, DT_SINGLELINE | DT_LEFT | DT_VCENTER); //显示文本

		POINT pt1;
		pt1.x = 0; pt1.y = 0;
		POINT pt2;
		pt2.x = 100;// (100, 100);
		pt2.y = 100;
		//pDc.SelectObject(&pen);//选择画笔
		//pDc.MoveTo(pt1);
		//pDc.LineTo(pt2);	
}

void CFiberDetectControlDlg::ShowText(CWnd *pwnd, char strText[SHOWTEXTLENGTH][255], CRect DrawRect)
{

	//CWnd *pwnd = GetDlgItem(IDC_ShowImage);
	
	RefreshWnd(pwnd);
	CClientDC pDc(pwnd);
	CPen pen(PS_SOLID, 5, RGB(255, 0, 0));//创建一个画笔工具	
	m_nRectWidth = m_ImageRect.Width();
	m_nRectHeight = m_ImageRect.Height();

	pDc.SetTextColor(RGB(0, 255, 0));
	//	pDc.DrawText(strText, DrawRect, DT_SINGLELINE | DT_LEFT | DT_VCENTER); //显示文本

	CFont font;

	font.CreateFont(
		20, // nHeight
		0, // nWidth
		0, // nEscapement
		0, // nOrientation
		FW_NORMAL, // nWeight
		FALSE, // bItalic
		FALSE, // bUnderline
		0, // cStrikeOut
		ANSI_CHARSET, // nCharSet
		OUT_DEFAULT_PRECIS, // nOutPrecision
		CLIP_DEFAULT_PRECIS, // nClipPrecision
		DEFAULT_QUALITY, // nQuality
		DEFAULT_PITCH | FF_SWISS,
		_T("Arial") // nPitchAndFamily Arial
		);
	pDc.SelectObject(&font);
	pDc.SetBkMode(TRANSPARENT);

	pDc.SetTextColor(RGB(0, 0, 0));

	pDc.SetBkColor(RGB(255, 255, 0));
	//pDc.TextOut(0, 0, "写字");
	int i = 0;
	for (i = 0; i < SHOWTEXTLENGTH; i++)
	{
		if (m_strShowText[i][0] != '\0')
		{
			CRect TextRect(10, i*30, 600, (i+1)*30);
			pDc.DrawText(m_strShowText[i], TextRect, DT_SINGLELINE | DT_LEFT | DT_VCENTER); //显示文本
		}	
	}
}


void CFiberDetectControlDlg::ShowText(CWnd *pwnd, char strText[SHOWTEXTLENGTH][255])
{
	int i = 0;
	CString strOut;
	for (i = 0; i < SHOWTEXTLENGTH; i++)
	{
		strOut.Append(m_strShowText[i]);
		strOut.Append("\r\n");
	}
	//strOut.Format("%s\r\n%s\r\n%s\r\n%s\r\n%s\r\n%s\r\n%s\r\n%s", m_strShowText[0], m_strShowText[1], m_strShowText[2], m_strShowText[3], m_strShowText[4], m_strShowText[5]);
	GetDlgItem(IDC_EDIT_INFO_SHOW)->SetWindowText(strOut);
	strOut.ReleaseBuffer();
	GetDlgItem(IDC_EDIT_INFO_SHOW)->UpdateData(true);
	//RedrawWindow();
}

void CFiberDetectControlDlg::RefreshWnd(CWnd *pwnd)
{

	//CWnd *pwnd = GetDlgItem(IDC_ShowImage);
	CRect PictureRect; 
	pwnd->GetClientRect(&PictureRect);
	int nWidth = PictureRect.Width();
	int nHeight = PictureRect.Height();
	CClientDC pDc(pwnd);
	CPen pen(PS_SOLID, 1, RGB(0, 0, 0));//创建一个画笔工具
	CBrush brush;
	brush.CreateSolidBrush(RGB(BACK_COLOR_GRAY, BACK_COLOR_GRAY, BACK_COLOR_GRAY));   //BACK_COLOR_GRAY: Window default color;

	//	pDc.DrawText(strText, DrawRect, DT_SINGLELINE | DT_LEFT | DT_VCENTER); //显示文本

	pDc.FillRect(PictureRect, &brush);

	POINT pt1;
	pt1.x = 0; pt1.y = 0;
	POINT pt2;
	pt2.x = 100;// (100, 100);
	pt2.y = 100;
	pDc.SelectObject(&pen);//选择画笔
	POINT point;
	point.x = 0;
	point.y = 0;
	pDc.MoveTo(point);

	point.x = nWidth;
	point.y = 0;
	pDc.LineTo(point);

	point.x = nWidth;
	point.y = nHeight;
	pDc.LineTo(point);

	point.x = 0;
	point.y = nHeight;
	pDc.LineTo(point);

	point.x = 0;
	point.y = 0;
	pDc.LineTo(point);

}

void CFiberDetectControlDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO:  在此添加消息处理程序代码和/或调用默认值
	if (nIDEvent == 1)
	{
		for (int i = 0; i < MAGNETIC_VALVE_SUM; i++)  //MAGNETIC_VALVE_SUM
		{
			if (Communicator::valve_driver[i].dura > 0)
			{
				Communicator::valve_driver[i].dura -= 20;
				m_mfcMagneticValve[i]->m_bTransparent = FALSE;
				m_mfcMagneticValve[i]->m_bDontUseWinXPTheme = TRUE;
				m_mfcMagneticValve[i]->m_bDrawFocus = FALSE;
				m_mfcMagneticValve[i]->m_nFlatStyle = CMFCButton::BUTTONSTYLE_NOBORDERS;
				m_mfcMagneticValve[i]->SetFaceColor(RGB(255, 0, 0));
			}
			else
			{
				m_mfcMagneticValve[i]->m_bTransparent = FALSE;
				m_mfcMagneticValve[i]->m_bDontUseWinXPTheme = TRUE;
				m_mfcMagneticValve[i]->m_bDrawFocus = FALSE;
				m_mfcMagneticValve[i]->m_nFlatStyle = CMFCButton::BUTTONSTYLE_NOBORDERS;
				m_mfcMagneticValve[i]->SetFaceColor(RGB(0, 250, 250));
			}
		}
	}

	CDialogEx::OnTimer(nIDEvent);
}

void CFiberDetectControlDlg::OnTimerShowValve(UINT_PTR nIDEvent)
{
	// TODO:  在此添加消息处理程序代码和/或调用默认值
	while (communicator_->m_bMagneticValveShow)
	{
		if (nIDEvent == 1)
		{
			for (int i = 0; i < MAGNETIC_VALVE_SUM; i++)  //MAGNETIC_VALVE_SUM
			{
				if (Communicator::valve_driver[i].dura > 0)
				{
					Communicator::valve_driver[i].dura -= 10;
					m_mfcMagneticValve[i]->m_bTransparent = FALSE;
					m_mfcMagneticValve[i]->m_bDontUseWinXPTheme = TRUE;
					m_mfcMagneticValve[i]->m_bDrawFocus = FALSE;
					m_mfcMagneticValve[i]->m_nFlatStyle = CMFCButton::BUTTONSTYLE_NOBORDERS;
					m_mfcMagneticValve[i]->SetFaceColor(RGB(255, 0, 0));
				}
				else
				{
					m_mfcMagneticValve[i]->m_bTransparent = FALSE;
					m_mfcMagneticValve[i]->m_bDontUseWinXPTheme = TRUE;
					m_mfcMagneticValve[i]->m_bDrawFocus = FALSE;
					m_mfcMagneticValve[i]->m_nFlatStyle = CMFCButton::BUTTONSTYLE_NOBORDERS;
					m_mfcMagneticValve[i]->SetFaceColor(RGB(0, 250, 250));
				}
			}
		}
		std::this_thread::sleep_for(std::chrono::milliseconds(1));
	}
}


/*
Author :LF
time:   2020-10-10
function: To process with the command from the control compurator
signalling commnad table
1:   Start  fiber detect
2:   Stop	fiber detect
3:   To test the magnetic valve
4:	 To send and write the camera detect parameters into ini-setting file and reset the detect parameters
5:   To send and write the boxes network parameters and cameras network and device parameters into ini-setting
		file and reset the device network parameters and camera device parameters;
6:	 To set and send the model seletc  parameters to box;
20:  To send image from camera simulator software to box fiber detect prorgram;
*/
LRESULT CFiberDetectControlDlg::ShowCtrlMessage(WPARAM wParam, LPARAM lParam)
{
	if (m_bShowInfoWindow == false)
	{	
		m_bShowInfoWindow = true;
		m_bShowImageWidhow = false;
		GetDlgItem(IDC_STATIC_IMAGE_SHOW)->ShowWindow(m_bShowInfoWindow);
		GetDlgItem(IDC_STATIC_IMAGE_SHOW1)->ShowWindow(m_bShowImageWidhow);
		GetDlgItem(IDC_STATIC_IMAGE_SHOW2)->ShowWindow(m_bShowImageWidhow);
		GetDlgItem(IDC_STATIC_IMAGE_SHOW3)->ShowWindow(m_bShowImageWidhow);
		GetDlgItem(IDC_STATIC_IMAGE_SHOW4)->ShowWindow(m_bShowImageWidhow);
		RefreshWnd(m_pwnd);   //clear the print words from the show window;
	}
	CString strMsg;
	int nCtrlSignalling = (int)wParam;
	Vsee::VCameraMessageTrans *MagneticVolveMsg = (Vsee::VCameraMessageTrans *)lParam;
	
	//AfxMessageBox(strMsg);
	
	if (nCtrlSignalling == 10)  //To clear the windows show message,and this command local command,it is nothing to box detect program
	{
		RefreshWnd(m_pwnd);   //clear the print words from the show window;
		CString str;
		char * strMessage = (char*)lParam;
		str.Format("%s", strMessage);
		PushTailShowText(str);
		//ShowText(m_pwnd, m_strShowText, NULL);
		ShowText(m_pwnd, m_strShowText);
		MagneticVolveMsg = (Vsee::VCameraMessageTrans*)lParam;
	}
	else if(nCtrlSignalling == 1)		//To start detect fiber
	{
		aq::Communicator::box_ctrl_msg.signalling = 1;     //To start detect fiber;
		aq::Communicator::box_ctrl_msg.magnetic_valve_num = 0;

		CString str;
		str.Format("Start detect fiber.");
		PushTailShowText(str);
		RefreshWnd(m_pwnd);
		ShowText(m_pwnd, m_strShowText);
	}
	else if (nCtrlSignalling == 2)			 //To start detect fiber;
	{
		aq::Communicator::box_ctrl_msg.signalling = 2;     //To start detect fiber;
		aq::Communicator::box_ctrl_msg.magnetic_valve_num = 0;

		CString str;
		str.Format("Stop detect fiber.");
		PushTailShowText(str);
		RefreshWnd(m_pwnd);
		ShowText(m_pwnd, m_strShowText);
	}
	else if (nCtrlSignalling == 3)      //To test the magnetic valve hardware;
	{
		RefreshWnd(m_pwnd);   //clear the print words from the show window;
		MagneticVolveMsg = (Vsee::VCameraMessageTrans*)lParam;
		int nMagneticNum = MagneticVolveMsg->magnetic_valve_num;
		strMsg.Format("wParam=%d,MagneticNum=%d", wParam, MagneticVolveMsg->magnetic_valve_num);
		memcpy(&aq::Communicator::box_ctrl_msg, MagneticVolveMsg, sizeof(Vsee::VCameraMessageTrans));
		for (int i = 0; i < 24; i++)
		{
			if (MagneticVolveMsg->magnetic_valve[i] == 1)
			{
				//if (i==0)
				nMagneticNum = i+1;
			}
			else
			{
				continue;
			}

			if (nMagneticNum == 1)  
			{
				aq::Communicator::box_ctrl_msg.signalling = nCtrlSignalling;
				aq::Communicator::box_ctrl_msg.magnetic_valve_num = nMagneticNum;
				m_mfcMagneticValve1.m_bTransparent = FALSE;
				m_mfcMagneticValve1.m_bDontUseWinXPTheme = TRUE;
				m_mfcMagneticValve1.m_bDrawFocus = FALSE;
				m_mfcMagneticValve1.m_nFlatStyle = CMFCButton::BUTTONSTYLE_NOBORDERS;
				m_mfcMagneticValve1.SetFaceColor(RGB(255, 0, 0));
				//Sleep(100);
				//m_mfcMagneticValve1.SetFaceColor(RGB(0, 255, 0));
				//Sleep(100);
				CString str;
				str.Format("Trig the %d electro_magnetic_valve.", nMagneticNum);
				PushTailShowText(str);
				//RefreshWnd(m_pwnd);
				//ShowText(m_pwnd, m_strShowText, NULL);
			}

			else if (nMagneticNum == 2)  
			{
				aq::Communicator::box_ctrl_msg.signalling = nCtrlSignalling;
				aq::Communicator::box_ctrl_msg.magnetic_valve_num = nMagneticNum;
				m_mfcMagneticValve2.m_bTransparent = FALSE;
				m_mfcMagneticValve2.m_bDontUseWinXPTheme = TRUE;
				m_mfcMagneticValve2.m_bDrawFocus = FALSE;
				m_mfcMagneticValve2.m_nFlatStyle = CMFCButton::BUTTONSTYLE_NOBORDERS;
				m_mfcMagneticValve2.SetFaceColor(RGB(255, 0, 0));
				//Sleep(100);
				//m_mfcMagneticValve2.SetFaceColor(RGB(0, 255, 0));
				//Sleep(100);
				CString str;
				str.Format("Trig the %d electro_magnetic_valve.", nMagneticNum);
				PushTailShowText(str);
				//RefreshWnd(m_pwnd);
				//ShowText(m_pwnd, m_strShowText, NULL);
			}

			else if (nMagneticNum == 3)   
			{
				aq::Communicator::box_ctrl_msg.signalling = nCtrlSignalling;
				aq::Communicator::box_ctrl_msg.magnetic_valve_num = nMagneticNum;
				m_mfcMagneticValve3.m_bTransparent = FALSE;
				m_mfcMagneticValve3.m_bDontUseWinXPTheme = TRUE;
				m_mfcMagneticValve3.m_bDrawFocus = FALSE;
				m_mfcMagneticValve3.m_nFlatStyle = CMFCButton::BUTTONSTYLE_NOBORDERS;
				m_mfcMagneticValve3.SetFaceColor(RGB(255, 0, 0));
				//Sleep(100);
				//m_mfcMagneticValve3.SetFaceColor(RGB(0, 255, 0));
				//Sleep(100);
				CString str;
				str.Format("Trig the %d electro_magnetic_valve.", nMagneticNum);
				PushTailShowText(str);
				//RefreshWnd(m_pwnd);
				//ShowText(m_pwnd, m_strShowText, NULL);
			}
			else if (nMagneticNum == 4)   
			{
				aq::Communicator::box_ctrl_msg.signalling = nCtrlSignalling;
				aq::Communicator::box_ctrl_msg.magnetic_valve_num = nMagneticNum;
				m_mfcMagneticValve4.m_bTransparent = FALSE;
				m_mfcMagneticValve4.m_bDontUseWinXPTheme = TRUE;
				m_mfcMagneticValve4.m_bDrawFocus = FALSE;
				m_mfcMagneticValve4.m_nFlatStyle = CMFCButton::BUTTONSTYLE_NOBORDERS;
				m_mfcMagneticValve4.SetFaceColor(RGB(255, 0, 0));
				//Sleep(100);
				//m_mfcMagneticValve4.SetFaceColor(RGB(0, 255, 0));
				//Sleep(100);
				CString str;
				str.Format("Trig the %d electro_magnetic_valve.", nMagneticNum);
				PushTailShowText(str);
				//RefreshWnd(m_pwnd);
				//ShowText(m_pwnd, m_strShowText, NULL);

			}
			else if (nMagneticNum == 5)  //To send the box ip address parameters and camera device parameters
			{
				aq::Communicator::box_ctrl_msg.signalling = nCtrlSignalling;
				aq::Communicator::box_ctrl_msg.magnetic_valve_num = nMagneticNum;
				m_mfcMagneticValve5.m_bTransparent = FALSE;
				m_mfcMagneticValve5.m_bDontUseWinXPTheme = TRUE;
				m_mfcMagneticValve5.m_bDrawFocus = FALSE;
				m_mfcMagneticValve5.m_nFlatStyle = CMFCButton::BUTTONSTYLE_NOBORDERS;
				m_mfcMagneticValve5.SetFaceColor(RGB(255, 0, 0));
				//Sleep(100);
				//m_mfcMagneticValve5.SetFaceColor(RGB(0, 255, 0));
				//Sleep(100);
				CString str;
				str.Format("To send the box ip address parameters and camera device parameters.");
				PushTailShowText(str);
				//RefreshWnd(m_pwnd);
				//ShowText(m_pwnd, m_strShowText, NULL);

			}
			else if (nMagneticNum == 6)  //To send model select number to box.
			{
				aq::Communicator::box_ctrl_msg.signalling = nCtrlSignalling;
				aq::Communicator::box_ctrl_msg.magnetic_valve_num = nMagneticNum;
				m_mfcMagneticValve6.m_bTransparent = FALSE;
				m_mfcMagneticValve6.m_bDontUseWinXPTheme = TRUE;
				m_mfcMagneticValve6.m_bDrawFocus = FALSE;
				m_mfcMagneticValve6.m_nFlatStyle = CMFCButton::BUTTONSTYLE_NOBORDERS;
				m_mfcMagneticValve6.SetFaceColor(RGB(255, 0, 0));
				//Sleep(100);
				//m_mfcMagneticValve6.SetFaceColor(RGB(0, 255, 0));
				//Sleep(100);
				CString str;
				str.Format("To send model select number to box.");
				PushTailShowText(str);
				//RefreshWnd(m_pwnd);
				//ShowText(m_pwnd, m_strShowText, NULL);

			}
			else if (nMagneticNum == 7)
			{
				aq::Communicator::box_ctrl_msg.signalling = nCtrlSignalling;
				aq::Communicator::box_ctrl_msg.magnetic_valve_num = nMagneticNum;
				m_mfcMagneticValve7.m_bTransparent = FALSE;
				m_mfcMagneticValve7.m_bDontUseWinXPTheme = TRUE;
				m_mfcMagneticValve7.m_bDrawFocus = FALSE;
				m_mfcMagneticValve7.m_nFlatStyle = CMFCButton::BUTTONSTYLE_NOBORDERS;
				m_mfcMagneticValve7.SetFaceColor(RGB(255, 0, 0));
				//Sleep(100);
				//m_mfcMagneticValve7.SetFaceColor(RGB(0, 255, 0));
				//Sleep(100);
				CString str;
				str.Format("Trig the %d electro_magnetic_valve.", nMagneticNum);
				PushTailShowText(str);
				//RefreshWnd(m_pwnd);
				//ShowText(m_pwnd, m_strShowText, NULL);

			}
			else if (nMagneticNum == 8)
			{
				aq::Communicator::box_ctrl_msg.signalling = nCtrlSignalling;
				aq::Communicator::box_ctrl_msg.magnetic_valve_num = nMagneticNum;
				m_mfcMagneticValve8.m_bTransparent = FALSE;
				m_mfcMagneticValve8.m_bDontUseWinXPTheme = TRUE;
				m_mfcMagneticValve8.m_bDrawFocus = FALSE;
				m_mfcMagneticValve8.m_nFlatStyle = CMFCButton::BUTTONSTYLE_NOBORDERS;
				m_mfcMagneticValve8.SetFaceColor(RGB(255, 0, 0));
				//Sleep(100);
				//m_mfcMagneticValve8.SetFaceColor(RGB(0, 255, 0));
				//Sleep(100);
				CString str;
				str.Format("Trig the %d electro_magnetic_valve.", nMagneticNum);
				PushTailShowText(str);
				//RefreshWnd(m_pwnd);
				//ShowText(m_pwnd, m_strShowText, NULL);

			}
			else if (nMagneticNum == 9)
			{
				aq::Communicator::box_ctrl_msg.signalling = nCtrlSignalling;
				aq::Communicator::box_ctrl_msg.magnetic_valve_num = nMagneticNum;
				
				m_mfcMagneticValve9.m_bTransparent = FALSE;
				m_mfcMagneticValve9.m_bDontUseWinXPTheme = TRUE;
				m_mfcMagneticValve9.m_bDrawFocus = FALSE;
				m_mfcMagneticValve9.m_nFlatStyle = CMFCButton::BUTTONSTYLE_NOBORDERS;
				m_mfcMagneticValve9.SetFaceColor(RGB(255, 0, 0));
				//Sleep(100);
				//m_mfcMagneticValve9.SetFaceColor(RGB(0, 255, 0));
				//Sleep(100);
				CString str;
				str.Format("Trig the %d electro_magnetic_valve.", nMagneticNum);
				PushTailShowText(str);
				//RefreshWnd(m_pwnd);
				//ShowText(m_pwnd, m_strShowText, NULL);

			}
			else if (nMagneticNum == 10)
			{
				aq::Communicator::box_ctrl_msg.signalling = nCtrlSignalling;
				aq::Communicator::box_ctrl_msg.magnetic_valve_num = nMagneticNum;
				m_mfcMagneticValve10.m_bTransparent = FALSE;
				m_mfcMagneticValve10.m_bDontUseWinXPTheme = TRUE;
				m_mfcMagneticValve10.m_bDrawFocus = FALSE;
				m_mfcMagneticValve10.m_nFlatStyle = CMFCButton::BUTTONSTYLE_NOBORDERS;
				m_mfcMagneticValve10.SetFaceColor(RGB(255, 0, 0));
				//Sleep(100);
				//m_mfcMagneticValve10.SetFaceColor(RGB(0, 255, 0));
				//Sleep(100);
				CString str;
				str.Format("Trig the %d electro_magnetic_valve.", nMagneticNum);
				PushTailShowText(str);
				//RefreshWnd(m_pwnd);
				//ShowText(m_pwnd, m_strShowText, NULL);

			}
			else if (nMagneticNum == 11)
			{
				aq::Communicator::box_ctrl_msg.signalling = nCtrlSignalling;
				aq::Communicator::box_ctrl_msg.magnetic_valve_num = nMagneticNum;
				m_mfcMagneticValve11.m_bTransparent = FALSE;
				m_mfcMagneticValve11.m_bDontUseWinXPTheme = TRUE;
				m_mfcMagneticValve11.m_bDrawFocus = FALSE;
				m_mfcMagneticValve11.m_nFlatStyle = CMFCButton::BUTTONSTYLE_NOBORDERS;
				m_mfcMagneticValve11.SetFaceColor(RGB(255, 0, 0));
				//Sleep(100);
				//m_mfcMagneticValve11.SetFaceColor(RGB(0, 255, 0));
				//Sleep(100);
				CString str;
				str.Format("Trig the %d electro_magnetic_valve.", nMagneticNum);
				PushTailShowText(str);
				//RefreshWnd(m_pwnd);
				//ShowText(m_pwnd, m_strShowText, NULL);

			}
			else if (nMagneticNum == 12)
			{
				aq::Communicator::box_ctrl_msg.signalling = nCtrlSignalling;
				aq::Communicator::box_ctrl_msg.magnetic_valve_num = nMagneticNum;
				m_mfcMagneticValve12.m_bTransparent = FALSE;
				m_mfcMagneticValve12.m_bDontUseWinXPTheme = TRUE;
				m_mfcMagneticValve12.m_bDrawFocus = FALSE;
				m_mfcMagneticValve12.m_nFlatStyle = CMFCButton::BUTTONSTYLE_NOBORDERS;
				m_mfcMagneticValve12.SetFaceColor(RGB(255, 0, 0));
				//Sleep(100);
				//m_mfcMagneticValve12.SetFaceColor(RGB(0, 255, 0));
				//Sleep(100);
				CString str;
				str.Format("Trig the %d electro_magnetic_valve.", nMagneticNum);
				PushTailShowText(str);
				//RefreshWnd(m_pwnd);
				//ShowText(m_pwnd, m_strShowText, NULL);

			}
			else if (nMagneticNum == 13)
			{
				aq::Communicator::box_ctrl_msg.signalling = nCtrlSignalling;
				aq::Communicator::box_ctrl_msg.magnetic_valve_num = nMagneticNum;
				m_mfcMagneticValve13.m_bTransparent = FALSE;
				m_mfcMagneticValve13.m_bDontUseWinXPTheme = TRUE;
				m_mfcMagneticValve13.m_bDrawFocus = FALSE;
				m_mfcMagneticValve13.m_nFlatStyle = CMFCButton::BUTTONSTYLE_NOBORDERS;
				m_mfcMagneticValve13.SetFaceColor(RGB(255, 0, 0));
				//Sleep(100);
				//m_mfcMagneticValve13.SetFaceColor(RGB(0, 255, 0));
				//Sleep(100);
				CString str;
				str.Format("Trig the %d electro_magnetic_valve.", nMagneticNum);
				PushTailShowText(str);
				//RefreshWnd(m_pwnd);
				//ShowText(m_pwnd, m_strShowText, NULL);

			}
			else if (nMagneticNum == 14)
			{
				aq::Communicator::box_ctrl_msg.signalling = nCtrlSignalling;
				aq::Communicator::box_ctrl_msg.magnetic_valve_num = nMagneticNum;
				m_mfcMagneticValve14.m_bTransparent = FALSE;
				m_mfcMagneticValve14.m_bDontUseWinXPTheme = TRUE;
				m_mfcMagneticValve14.m_bDrawFocus = FALSE;
				m_mfcMagneticValve14.m_nFlatStyle = CMFCButton::BUTTONSTYLE_NOBORDERS;
				m_mfcMagneticValve14.SetFaceColor(RGB(255, 0, 0));
				//Sleep(100);
				//m_mfcMagneticValve14.SetFaceColor(RGB(0, 255, 0));
				//Sleep(100);
				CString str;
				str.Format("Trig the %d electro_magnetic_valve.", nMagneticNum);
				PushTailShowText(str);
				//RefreshWnd(m_pwnd);
				//ShowText(m_pwnd, m_strShowText, NULL);

			}
			else if (nMagneticNum == 15)
			{
				aq::Communicator::box_ctrl_msg.signalling = nCtrlSignalling;
				aq::Communicator::box_ctrl_msg.magnetic_valve_num = nMagneticNum;
				m_mfcMagneticValve15.m_bTransparent = FALSE;
				m_mfcMagneticValve15.m_bDontUseWinXPTheme = TRUE;
				m_mfcMagneticValve15.m_bDrawFocus = FALSE;
				m_mfcMagneticValve15.m_nFlatStyle = CMFCButton::BUTTONSTYLE_NOBORDERS;
				m_mfcMagneticValve15.SetFaceColor(RGB(255, 0, 0));
				//Sleep(100);
				//m_mfcMagneticValve15.SetFaceColor(RGB(0, 255, 0));
				//Sleep(100);
				CString str;
				str.Format("Trig the %d electro_magnetic_valve.", nMagneticNum);
				PushTailShowText(str);
				//RefreshWnd(m_pwnd);
				//ShowText(m_pwnd, m_strShowText, NULL);

			}
			else if (nMagneticNum == 16)
			{
				aq::Communicator::box_ctrl_msg.signalling = nCtrlSignalling;
				aq::Communicator::box_ctrl_msg.magnetic_valve_num = nMagneticNum;
				m_mfcMagneticValve16.m_bTransparent = FALSE;
				m_mfcMagneticValve16.m_bDontUseWinXPTheme = TRUE;
				m_mfcMagneticValve16.m_bDrawFocus = FALSE;
				m_mfcMagneticValve16.m_nFlatStyle = CMFCButton::BUTTONSTYLE_NOBORDERS;
				m_mfcMagneticValve16.SetFaceColor(RGB(255, 0, 0));
				//Sleep(100);
				//m_mfcMagneticValve16.SetFaceColor(RGB(0, 255, 0));
				//Sleep(100);
				CString str;
				str.Format("Trig the %d electro_magnetic_valve.", nMagneticNum);
				PushTailShowText(str);
				//RefreshWnd(m_pwnd);
				//ShowText(m_pwnd, m_strShowText, NULL);

			}
			else if (nMagneticNum == 17)
			{
				aq::Communicator::box_ctrl_msg.signalling = nCtrlSignalling;
				aq::Communicator::box_ctrl_msg.magnetic_valve_num = nMagneticNum;
				m_mfcMagneticValve17.m_bTransparent = FALSE;
				m_mfcMagneticValve17.m_bDontUseWinXPTheme = TRUE;
				m_mfcMagneticValve17.m_bDrawFocus = FALSE;
				m_mfcMagneticValve17.m_nFlatStyle = CMFCButton::BUTTONSTYLE_NOBORDERS;
				m_mfcMagneticValve17.SetFaceColor(RGB(255, 0, 0));
				//Sleep(100);
				//m_mfcMagneticValve17.SetFaceColor(RGB(0, 255, 0));
				//Sleep(100);
				CString str;
				str.Format("Trig the %d electro_magnetic_valve.", nMagneticNum);
				PushTailShowText(str);
				//RefreshWnd(m_pwnd);
				//ShowText(m_pwnd, m_strShowText, NULL);

			}
			else if (nMagneticNum == 18)
			{
				aq::Communicator::box_ctrl_msg.signalling = nCtrlSignalling;
				aq::Communicator::box_ctrl_msg.magnetic_valve_num = nMagneticNum;
				m_mfcMagneticValve18.m_bTransparent = FALSE;
				m_mfcMagneticValve18.m_bDontUseWinXPTheme = TRUE;
				m_mfcMagneticValve18.m_bDrawFocus = FALSE;
				m_mfcMagneticValve18.m_nFlatStyle = CMFCButton::BUTTONSTYLE_NOBORDERS;
				m_mfcMagneticValve18.SetFaceColor(RGB(255, 0, 0));
				//Sleep(100);
				//m_mfcMagneticValve18.SetFaceColor(RGB(0, 255, 0));
				//Sleep(100);
				CString str;
				str.Format("Trig the %d electro_magnetic_valve.", nMagneticNum);
				PushTailShowText(str);
				//RefreshWnd(m_pwnd);
				//ShowText(m_pwnd, m_strShowText, NULL);

			}
			else if (nMagneticNum == 19)
			{
				aq::Communicator::box_ctrl_msg.signalling = nCtrlSignalling;
				aq::Communicator::box_ctrl_msg.magnetic_valve_num = nMagneticNum;
				m_mfcMagneticValve19.m_bTransparent = FALSE;
				m_mfcMagneticValve19.m_bDontUseWinXPTheme = TRUE;
				m_mfcMagneticValve19.m_bDrawFocus = FALSE;
				m_mfcMagneticValve19.m_nFlatStyle = CMFCButton::BUTTONSTYLE_NOBORDERS;
				m_mfcMagneticValve19.SetFaceColor(RGB(255, 0, 0));
				//Sleep(100);
				//m_mfcMagneticValve19.SetFaceColor(RGB(0, 255, 0));
				//Sleep(100);
				CString str;
				str.Format("Trig the %d electro_magnetic_valve.", nMagneticNum);
				PushTailShowText(str);
				//RefreshWnd(m_pwnd);
				//ShowText(m_pwnd, m_strShowText, NULL);

			}
			else if (nMagneticNum == 20)
			{
				aq::Communicator::box_ctrl_msg.signalling = nCtrlSignalling;
				aq::Communicator::box_ctrl_msg.magnetic_valve_num = nMagneticNum;
				m_mfcMagneticValve20.m_bTransparent = FALSE;
				m_mfcMagneticValve20.m_bDontUseWinXPTheme = TRUE;
				m_mfcMagneticValve20.m_bDrawFocus = FALSE;
				m_mfcMagneticValve20.m_nFlatStyle = CMFCButton::BUTTONSTYLE_NOBORDERS;
				m_mfcMagneticValve20.SetFaceColor(RGB(255, 0, 0));
				//Sleep(100);
				//m_mfcMagneticValve20.SetFaceColor(RGB(0, 255, 0));
				//Sleep(100);
				CString str;
				str.Format("Trig the %d electro_magnetic_valve.", nMagneticNum);
				PushTailShowText(str);
				//RefreshWnd(m_pwnd);
				//ShowText(m_pwnd, m_strShowText, NULL);

			}
			else if (nMagneticNum == 21)
			{
				aq::Communicator::box_ctrl_msg.signalling = nCtrlSignalling;
				aq::Communicator::box_ctrl_msg.magnetic_valve_num = nMagneticNum;
				m_mfcMagneticValve21.m_bTransparent = FALSE;
				m_mfcMagneticValve21.m_bDontUseWinXPTheme = TRUE;
				m_mfcMagneticValve21.m_bDrawFocus = FALSE;
				m_mfcMagneticValve21.m_nFlatStyle = CMFCButton::BUTTONSTYLE_NOBORDERS;
				m_mfcMagneticValve21.SetFaceColor(RGB(255, 0, 0));
				//Sleep(100);
				//m_mfcMagneticValve21.SetFaceColor(RGB(0, 255, 0));
				//Sleep(100);
				CString str;
				str.Format("Trig the %d electro_magnetic_valve.", nMagneticNum);
				PushTailShowText(str);
				//RefreshWnd(m_pwnd);
				//ShowText(m_pwnd, m_strShowText, NULL);

			}
			else if (nMagneticNum == 22)
			{
				aq::Communicator::box_ctrl_msg.signalling = nCtrlSignalling;
				aq::Communicator::box_ctrl_msg.magnetic_valve_num = nMagneticNum;
				m_mfcMagneticValve22.m_bTransparent = FALSE;
				m_mfcMagneticValve22.m_bDontUseWinXPTheme = TRUE;
				m_mfcMagneticValve22.m_bDrawFocus = FALSE;
				m_mfcMagneticValve22.m_nFlatStyle = CMFCButton::BUTTONSTYLE_NOBORDERS;
				m_mfcMagneticValve22.SetFaceColor(RGB(255, 0, 0));
				//Sleep(100);
				//m_mfcMagneticValve22.SetFaceColor(RGB(0, 255, 0));
				//Sleep(100);
				CString str;
				str.Format("Trig the %d electro_magnetic_valve.", nMagneticNum);
				PushTailShowText(str);
				//RefreshWnd(m_pwnd);
				//ShowText(m_pwnd, m_strShowText, NULL);

			}
			else if (nMagneticNum == 23)
			{
				aq::Communicator::box_ctrl_msg.signalling = nCtrlSignalling;
				aq::Communicator::box_ctrl_msg.magnetic_valve_num = nMagneticNum;
				m_mfcMagneticValve23.m_bTransparent = FALSE;
				m_mfcMagneticValve23.m_bDontUseWinXPTheme = TRUE;
				m_mfcMagneticValve23.m_bDrawFocus = FALSE;
				m_mfcMagneticValve23.m_nFlatStyle = CMFCButton::BUTTONSTYLE_NOBORDERS;
				m_mfcMagneticValve23.SetFaceColor(RGB(255, 0, 0));
				//Sleep(100);
				//m_mfcMagneticValve23.SetFaceColor(RGB(0, 255, 0));
				//Sleep(100);
				CString str;
				str.Format("Trig the %d electro_magnetic_valve.", nMagneticNum);
				PushTailShowText(str);
				//RefreshWnd(m_pwnd);
				//ShowText(m_pwnd, m_strShowText, NULL);

			}
			else if (nMagneticNum == 24)
			{
				aq::Communicator::box_ctrl_msg.signalling = nCtrlSignalling;
				aq::Communicator::box_ctrl_msg.magnetic_valve_num = nMagneticNum;
				m_mfcMagneticValve24.m_bTransparent = FALSE;
				m_mfcMagneticValve24.m_bDontUseWinXPTheme = TRUE;
				m_mfcMagneticValve24.m_bDrawFocus = FALSE;
				m_mfcMagneticValve24.m_nFlatStyle = CMFCButton::BUTTONSTYLE_NOBORDERS;
				m_mfcMagneticValve24.SetFaceColor(RGB(255, 0, 0));
				//Sleep(100);
				//m_mfcMagneticValve24.SetFaceColor(RGB(0, 255, 0));
				//Sleep(100);
				CString str;
				str.Format("Trig the %d electro_magnetic_valve.", nMagneticNum);
				PushTailShowText(str);
				//RefreshWnd(m_pwnd);
				//ShowText(m_pwnd, m_strShowText, NULL);
			}
			else
			{
				;
			}
		}
		if (1)
		{
			Sleep(100);
			m_mfcMagneticValve1.SetFaceColor(RGB(0, 255, 0));
			m_mfcMagneticValve2.SetFaceColor(RGB(0, 255, 0));
			m_mfcMagneticValve3.SetFaceColor(RGB(0, 255, 0));
			m_mfcMagneticValve4.SetFaceColor(RGB(0, 255, 0));
			m_mfcMagneticValve5.SetFaceColor(RGB(0, 255, 0));
			m_mfcMagneticValve6.SetFaceColor(RGB(0, 255, 0));
			m_mfcMagneticValve7.SetFaceColor(RGB(0, 255, 0));
			m_mfcMagneticValve8.SetFaceColor(RGB(0, 255, 0));
			m_mfcMagneticValve9.SetFaceColor(RGB(0, 255, 0));
			m_mfcMagneticValve10.SetFaceColor(RGB(0, 255, 0));
			m_mfcMagneticValve11.SetFaceColor(RGB(0, 255, 0));
			m_mfcMagneticValve12.SetFaceColor(RGB(0, 255, 0));
			m_mfcMagneticValve13.SetFaceColor(RGB(0, 255, 0));
			m_mfcMagneticValve14.SetFaceColor(RGB(0, 255, 0));
			m_mfcMagneticValve15.SetFaceColor(RGB(0, 255, 0));
			m_mfcMagneticValve16.SetFaceColor(RGB(0, 255, 0));
			m_mfcMagneticValve17.SetFaceColor(RGB(0, 255, 0));
			m_mfcMagneticValve18.SetFaceColor(RGB(0, 255, 0));
			m_mfcMagneticValve19.SetFaceColor(RGB(0, 255, 0));
			m_mfcMagneticValve20.SetFaceColor(RGB(0, 255, 0));
			m_mfcMagneticValve21.SetFaceColor(RGB(0, 255, 0));
			m_mfcMagneticValve22.SetFaceColor(RGB(0, 255, 0));
			m_mfcMagneticValve23.SetFaceColor(RGB(0, 255, 0));
			m_mfcMagneticValve24.SetFaceColor(RGB(0, 255, 0));
			Sleep(100);
			//RefreshWnd(m_pwnd);
			ShowText(m_pwnd, m_strShowText);
		}
		
	}
	else if (nCtrlSignalling == 4)     //To set and send the camera detect parameters;
	{
		MagneticVolveMsg = (Vsee::VCameraMessageTrans*)lParam;
		int nMagneticNum = MagneticVolveMsg->magnetic_valve_num;
		strMsg.Format("wParam=%d,MagneticNum=%d", wParam, MagneticVolveMsg->magnetic_valve_num);
		memcpy(&aq::Communicator::box_ctrl_msg, MagneticVolveMsg, sizeof(Vsee::VCameraMessageTrans));
		RefreshWnd(m_pwnd);   //clear the print words from the show window;
		CString str;
		str.Format("To set camera detect parameters.");
		PushTailShowText(str);
		ShowText(m_pwnd, m_strShowText);
		WriteIni(MagneticVolveMsg->parameters_setting_trans);

	}
	else if (nCtrlSignalling == 5)     //To set and send the box and camera network parameters and camera device  parameters;
	{
		MagneticVolveMsg = (Vsee::VCameraMessageTrans*)lParam;
		int nMagneticNum = MagneticVolveMsg->magnetic_valve_num;
		strMsg.Format("wParam=%d,MagneticNum=%d", wParam, MagneticVolveMsg->magnetic_valve_num);
		memcpy(&aq::Communicator::box_ctrl_msg, MagneticVolveMsg, sizeof(Vsee::VCameraMessageTrans));
		RefreshWnd(m_pwnd);   //clear the print words from the show window;
		CString str;
		str.Format("To set camera detect parameters.");
		PushTailShowText(str);
		ShowText(m_pwnd, m_strShowText);
		WriteIni(MagneticVolveMsg->parameters_setting_trans);

	}
	else if (nCtrlSignalling == 6)     //To set and send the model seletc  parameters to box;
	{
		MagneticVolveMsg = (Vsee::VCameraMessageTrans*)lParam;
		int nMagneticNum = MagneticVolveMsg->magnetic_valve_num;
		strMsg.Format("wParam=%d,MagneticNum=%d", wParam, MagneticVolveMsg->magnetic_valve_num);
		memcpy(&aq::Communicator::box_ctrl_msg, MagneticVolveMsg, sizeof(Vsee::VCameraMessageTrans));
		RefreshWnd(m_pwnd);				//clear the print words from the show window;
		CString str;
		str.Format("To set model select serial number.");
		PushTailShowText(str);
		ShowText(m_pwnd, m_strShowText);
		WriteIni(MagneticVolveMsg->parameters_setting_trans);

	}
	else if (nCtrlSignalling == 7)					//To start grab image and save image.
	{
		MagneticVolveMsg = (Vsee::VCameraMessageTrans*)lParam;
		int nMagneticNum = MagneticVolveMsg->magnetic_valve_num;
		strMsg.Format("wParam=%d,MagneticNum=%d", wParam, MagneticVolveMsg->magnetic_valve_num);
		memcpy(&aq::Communicator::box_ctrl_msg, MagneticVolveMsg, sizeof(Vsee::VCameraMessageTrans));
		RefreshWnd(m_pwnd);							//clear the print words from the show window;
		CString str;
		str.Format("To start grab image and save image.");
		PushTailShowText(str);
		ShowText(m_pwnd, m_strShowText);
		WriteIni(MagneticVolveMsg->parameters_setting_trans);

	}
	else if (nCtrlSignalling == 8)					//To stop grab image and save image.
	{
		MagneticVolveMsg = (Vsee::VCameraMessageTrans*)lParam;
		int nMagneticNum = MagneticVolveMsg->magnetic_valve_num;
		strMsg.Format("wParam=%d,MagneticNum=%d", wParam, MagneticVolveMsg->magnetic_valve_num);
		memcpy(&aq::Communicator::box_ctrl_msg, MagneticVolveMsg, sizeof(Vsee::VCameraMessageTrans));
		RefreshWnd(m_pwnd);							//clear the print words from the show window;
		CString str;
		str.Format("To stop grab image and save image.");
		PushTailShowText(str);
		ShowText(m_pwnd, m_strShowText);
		WriteIni(MagneticVolveMsg->parameters_setting_trans);
	}
	else if (nCtrlSignalling == 9)					//To upload  grab image to server.
	{
		MagneticVolveMsg = (Vsee::VCameraMessageTrans*)lParam;
		int nMagneticNum = MagneticVolveMsg->magnetic_valve_num;
		if (nMagneticNum == 1)
			m_bSaveImage = true;
		else
			m_bSaveImage = false;
		strMsg.Format("wParam=%d,MagneticNum=%d", wParam, MagneticVolveMsg->magnetic_valve_num);
		m_nSaveTrainImageCamera = MagneticVolveMsg->channels;
		memcpy(&aq::Communicator::box_ctrl_msg, MagneticVolveMsg, sizeof(Vsee::VCameraMessageTrans));
		RefreshWnd(m_pwnd);							//clear the print words from the show window;
		CString str;
		str.Format("To upload  grab image to server.");
		PushTailShowText(str);
		ShowText(m_pwnd, m_strShowText);
		//WriteIni(MagneticVolveMsg->parameters_setting_trans);

	}
	else if (nCtrlSignalling == 11)					//To start camera grab image.
	{
		MagneticVolveMsg = (Vsee::VCameraMessageTrans*)lParam;
		int nMagneticNum = MagneticVolveMsg->magnetic_valve_num;
		strMsg.Format("wParam=%d,MagneticNum=%d", wParam, MagneticVolveMsg->magnetic_valve_num);
		memcpy(&aq::Communicator::box_ctrl_msg, MagneticVolveMsg, sizeof(Vsee::VCameraMessageTrans));
		RefreshWnd(m_pwnd);							//clear the print words from the show window;
		CString str;
		str.Format("To start camera grab image.");
		PushTailShowText(str);
		ShowText(m_pwnd, m_strShowText);
		//WriteIni(MagneticVolveMsg->parameters_setting_trans);

	}
	else if (nCtrlSignalling == 12)					//To stop camera grab image.
	{
		MagneticVolveMsg = (Vsee::VCameraMessageTrans*)lParam;
		int nMagneticNum = MagneticVolveMsg->magnetic_valve_num;
		strMsg.Format("wParam=%d,MagneticNum=%d", wParam, MagneticVolveMsg->magnetic_valve_num);
		memcpy(&aq::Communicator::box_ctrl_msg, MagneticVolveMsg, sizeof(Vsee::VCameraMessageTrans));
		RefreshWnd(m_pwnd);							//clear the print words from the show window;
		CString str;
		str.Format("To stop camera grab image.");
		PushTailShowText(str);
		ShowText(m_pwnd, m_strShowText);
		//WriteIni(MagneticVolveMsg->parameters_setting_trans);

	}
	else if (nCtrlSignalling == 13)					//To start camera grab mode.
	{
		MagneticVolveMsg = (Vsee::VCameraMessageTrans*)lParam;
		int nMagneticNum = MagneticVolveMsg->magnetic_valve_num;
		strMsg.Format("wParam=%d,MagneticNum=%d", wParam, MagneticVolveMsg->magnetic_valve_num);
		memcpy(&aq::Communicator::box_ctrl_msg, MagneticVolveMsg, sizeof(Vsee::VCameraMessageTrans));
		RefreshWnd(m_pwnd);							//clear the print words from the show window;
		CString str;
		str.Format("To start camera grab mode.");
		PushTailShowText(str);
		ShowText(m_pwnd, m_strShowText);
		//WriteIni(MagneticVolveMsg->parameters_setting_trans);

	}
	else if (nCtrlSignalling == 14)					//To stop camera grab mode.
	{
		MagneticVolveMsg = (Vsee::VCameraMessageTrans*)lParam;
		int nMagneticNum = MagneticVolveMsg->magnetic_valve_num;
		strMsg.Format("wParam=%d,MagneticNum=%d", wParam, MagneticVolveMsg->magnetic_valve_num);
		memcpy(&aq::Communicator::box_ctrl_msg, MagneticVolveMsg, sizeof(Vsee::VCameraMessageTrans));
		RefreshWnd(m_pwnd);							//clear the print words from the show window;
		CString str;
		str.Format("To stop camera grab mode.");
		PushTailShowText(str);
		ShowText(m_pwnd, m_strShowText);
		//WriteIni(MagneticVolveMsg->parameters_setting_trans);

	}
	else if (nCtrlSignalling == 15)					//To save detect result label.
	{
		MagneticVolveMsg = (Vsee::VCameraMessageTrans*)lParam;
		int nMagneticNum = MagneticVolveMsg->magnetic_valve_num;
		if (nMagneticNum == 1)
			m_bSaveDetectResultLabel = true;
		else
			m_bSaveDetectResultLabel = false;
		strMsg.Format("wParam=%d,MagneticNum=%d", wParam, MagneticVolveMsg->magnetic_valve_num);
		m_nSaveDetectResultLabelCamera = MagneticVolveMsg->channels;
		memcpy(&aq::Communicator::box_ctrl_msg, MagneticVolveMsg, sizeof(Vsee::VCameraMessageTrans));
		RefreshWnd(m_pwnd);							//clear the print words from the show window;
		CString str;
		str.Format("To save detect result label.");
		PushTailShowText(str);
		ShowText(m_pwnd, m_strShowText);
		//WriteIni(MagneticVolveMsg->parameters_setting_trans);

	}
	else if (nCtrlSignalling == 16)					//To save detect result label.
	{
		char* strPath = (char*)lParam;
		m_strSaveDetectResultPath = strPath;
		return 1;
	}
	else if (nCtrlSignalling == 21)     //To set and send the model seletc  parameters to box;
	{
		MagneticVolveMsg = (Vsee::VCameraMessageTrans*)lParam;
		memcpy(&aq::Communicator::box_ctrl_msg, MagneticVolveMsg, sizeof(Vsee::VCameraMessageTrans));
		RefreshWnd(m_pwnd);				//clear the print words from the show window;
		CString str;
		str.Format("To set model select AI BOX number.");
		PushTailShowText(str);
		ShowText(m_pwnd, m_strShowText);
		WriteIni(MagneticVolveMsg->parameters_setting_trans);
	}
	else
	{
		MagneticVolveMsg = (Vsee::VCameraMessageTrans*)lParam;
		int nMagneticNum = MagneticVolveMsg->magnetic_valve_num;
		strMsg.Format("wParam=%d,MagneticNum=%d", wParam, MagneticVolveMsg->magnetic_valve_num);
		memcpy(&aq::Communicator::box_ctrl_msg, MagneticVolveMsg, sizeof(Vsee::VCameraMessageTrans));
		RefreshWnd(m_pwnd);							//clear the print words from the show window;
		CString str;
		str.Format("Unknown signalling.");
		PushTailShowText(str);
		ShowText(m_pwnd, m_strShowText);
	}
	if (MagneticVolveMsg != nullptr)    //Here need to delete the data pointer that transmit from ::PostMessage() function;
	{
		delete MagneticVolveMsg;
		MagneticVolveMsg = nullptr;
	}
	
	UpdateData(true);
	//RedrawWindow();
	
	//Invalidate(TRUE);   //If use function "Invalidate(TRUE)" will induce window flashing;
	return 1;
}


void CFiberDetectControlDlg::OnBnClickedButtonStartDetect()
{
	// TODO:  在此添加控件通知处理程序代码
	aq::Communicator::box_ctrl_msg.signalling = 1;     //To start detect fiber;
	aq::Communicator::box_ctrl_msg.magnetic_valve_num = 0;

	start_grab = false;
	
	::PostMessage(AfxGetMainWnd()->m_hWnd, ID_OPEN_CAMERA, start_grab, 0);
	CString str;
	str.Format("Start detect fiber.");
	PushTailShowText(str);
	RefreshWnd(m_pwnd);
	ShowText(m_pwnd, m_strShowText);
}


void CFiberDetectControlDlg::OnBnClickedButtonStopDetect2()
{
	// TODO:  在此添加控件通知处理程序代码
	aq::Communicator::box_ctrl_msg.signalling = 2;     //To start detect fiber;
	aq::Communicator::box_ctrl_msg.magnetic_valve_num = 0;

	start_grab = true;
	
	::PostMessage(AfxGetMainWnd()->m_hWnd, ID_OPEN_CAMERA, start_grab, 0);
	CString str;
	str.Format("Stop detect fiber.");
	PushTailShowText(str);
	RefreshWnd(m_pwnd);
	ShowText(m_pwnd, m_strShowText);
}



void CFiberDetectControlDlg::WriteIni(CParameterSetting parameters_setting_trans)
{
	//char IniRead[255];
	//memset(IniRead, 0, 255);
	char IniPath[255];
	memset(IniPath, 0, 255);
	//camera_ip_info ci_info;
	CString strPath = GetModulePath();
	strcpy_s(IniPath, 255, strPath.GetBuffer());
	strcat_s(IniPath, "/param.ini");
	CString IniRead;
	CString IniWrite;

	IniWrite.Format("%d", parameters_setting_trans.model_select);
	WritePrivateProfileString("System", "model_select", IniWrite, IniPath);

	//GetPrivateProfileStringA("BaseDetectParameter", "cameraA_begin", "", IniRead, 24, IniPath);	
	//m_dlgBaseDetectParametersSettting.GetDlgItem(IDC_EDIT_CAMERA1_BEGIN_POINT)->GetWindowTextA(IniRead);
	IniWrite.Format("%d", parameters_setting_trans.cameraA_begin);
	WritePrivateProfileString("BaseDetectParameter", "cameraA_begin", IniWrite, IniPath);
	//parameters_setting_trans.cameraA_begin = atoi(IniWrite);

	//GetPrivateProfileStringA("BaseDetectParameter", "cameraA_end", "", IniRead, 24, IniPath);
	//m_dlgBaseDetectParametersSettting.GetDlgItem(IDC_EDIT_CAMERA1_END_POINT)->GetWindowTextA(IniRead);
	IniWrite.Format("%d", parameters_setting_trans.cameraA_end);
	WritePrivateProfileString("BaseDetectParameter", "cameraA_end", IniWrite, IniPath);
	//parameters_setting_trans.cameraA_end = atoi(IniWrite);

	//GetPrivateProfileStringA("BaseDetectParameter", "cameraA_align_begin", "", IniRead, 24, IniPath);
	//m_dlgBaseDetectParametersSettting.GetDlgItem(IDC_EDIT_CAMERA1_ALIGNING_BEGIN_POINT)->GetWindowTextA(IniRead);
	IniWrite.Format("%d", parameters_setting_trans.cameraA_align_begin);
	WritePrivateProfileString("BaseDetectParameter", "cameraA_align_begin", IniWrite, IniPath);
	//parameters_setting_trans.cameraA_align_begin = atoi(IniRead);


	//GetPrivateProfileStringA("BaseDetectParameter", "cameraA_align_end", "", IniRead, 24, IniPath);
	//m_dlgBaseDetectParametersSettting.GetDlgItem(IDC_EDIT_CAMERA1_ALIGNING_END_POINT)->GetWindowTextA(IniRead);
	IniWrite.Format("%d", parameters_setting_trans.cameraA_align_end);
	WritePrivateProfileString("BaseDetectParameter", "cameraA_align_end", IniWrite, IniPath);
	//parameters_setting_trans.cameraA_align_end = atoi(IniRead);

	//GetPrivateProfileStringA("BaseDetectParameter", "cameraA_detect_begin", "", IniRead, 24, IniPath);
	//m_dlgBaseDetectParametersSettting.GetDlgItem(IDC_EDIT_CAMERA1_DETECT_BEGIN_POINT)->GetWindowTextA(IniRead);
	IniWrite.Format("%d", parameters_setting_trans.cameraA_detect_begin);
	WritePrivateProfileString("BaseDetectParameter", "cameraA_detect_begin", IniWrite, IniPath);
	//parameters_setting_trans.cameraA_detect_begin = atoi(IniRead);


	//GetPrivateProfileStringA("BaseDetectParameter", "cameraA_detect_end", "", IniRead, 24, IniPath);
	//m_dlgBaseDetectParametersSettting.GetDlgItem(IDC_EDIT_CAMERA1_DETECT_END_POINT)->GetWindowTextA(IniRead);
	//parameters_setting_trans.cameraA_detect_end = atoi(IniRead);
	IniWrite.Format("%d", parameters_setting_trans.cameraA_detect_end);
	WritePrivateProfileString("BaseDetectParameter", "cameraA_detect_end", IniWrite, IniPath);


	//GetPrivateProfileStringA("BaseDetectParameter", "cameraA_valve_amount", "", IniRead, 24, IniPath);
	//m_dlgBaseDetectParametersSettting.GetDlgItem(IDC_EDIT_CAMERA1_VALVE_AMOUNT)->GetWindowTextA(IniRead);
	//parameters_setting_trans.cameraA_valve_amount = atoi(IniRead);
	IniWrite.Format("%d", parameters_setting_trans.cameraA_valve_amount);
	WritePrivateProfileString("BaseDetectParameter", "cameraA_valve_amount", IniWrite, IniPath);


	//GetPrivateProfileStringA("BaseDetectParameter", "cameraB_begin", "", IniRead, 24, IniPath);
	//m_dlgBaseDetectParametersSettting.GetDlgItem(IDC_EDIT_CAMERA2_BEGIN_POINT)->GetWindowTextA(IniRead);
	//parameters_setting_trans.cameraB_begin = atoi(IniRead);
	IniWrite.Format("%d", parameters_setting_trans.cameraB_begin);
	WritePrivateProfileString("BaseDetectParameter", "cameraB_begin", IniWrite, IniPath);


	//GetPrivateProfileStringA("BaseDetectParameter", "cameraB_end", "", IniRead, 24, IniPath);
	//m_dlgBaseDetectParametersSettting.GetDlgItem(IDC_EDIT_CAMERA2_END_POINT)->GetWindowTextA(IniRead);
	//parameters_setting_trans.cameraB_end = atoi(IniRead);
	IniWrite.Format("%d", parameters_setting_trans.cameraB_end);
	WritePrivateProfileString("BaseDetectParameter", "cameraB_end", IniWrite, IniPath);


	//GetPrivateProfileStringA("BaseDetectParameter", "cameraB_align_begin", "", IniRead, 24, IniPath);
	//m_dlgBaseDetectParametersSettting.GetDlgItem(IDC_EDIT_CAMERA2_ALIGNING_BEGIN_POINT)->GetWindowTextA(IniRead);
	//parameters_setting_trans.cameraB_align_begin = atoi(IniRead);
	IniWrite.Format("%d", parameters_setting_trans.cameraB_align_begin);
	WritePrivateProfileString("BaseDetectParameter", "cameraB_align_begin", IniWrite, IniPath);


	//GetPrivateProfileStringA("BaseDetectParameter", "cameraB_align_end", "", IniRead, 24, IniPath);
	//m_dlgBaseDetectParametersSettting.GetDlgItem(IDC_EDIT_CAMERA2_ALIGNING_END_POINT)->GetWindowTextA(IniRead);
	//parameters_setting_trans.cameraB_align_end = atoi(IniRead);
	IniWrite.Format("%d", parameters_setting_trans.cameraB_align_end);
	WritePrivateProfileString("BaseDetectParameter", "cameraB_align_end", IniWrite, IniPath);


	//GetPrivateProfileStringA("BaseDetectParameter", "cameraB_detect_begin", "", IniRead, 24, IniPath);
	//m_dlgBaseDetectParametersSettting.GetDlgItem(IDC_EDIT_CAMERA2_DETECT_BEGIN_POINT)->GetWindowTextA(IniRead);
	//parameters_setting_trans.cameraB_detect_begin = atoi(IniRead);
	IniWrite.Format("%d", parameters_setting_trans.cameraB_detect_begin);
	WritePrivateProfileString("BaseDetectParameter", "cameraB_detect_begin", IniWrite, IniPath);


	//GetPrivateProfileStringA("BaseDetectParameter", "cameraB_detect_end", "", IniRead, 24, IniPath);
	//m_dlgBaseDetectParametersSettting.GetDlgItem(IDC_EDIT_CAMERA2_DETECT_END_POINT)->GetWindowTextA(IniRead);
	//parameters_setting_trans.cameraB_detect_end = atoi(IniRead);
	IniWrite.Format("%d", parameters_setting_trans.cameraB_detect_end);
	WritePrivateProfileString("BaseDetectParameter", "cameraB_detect_end", IniWrite, IniPath);


	//GetPrivateProfileStringA("BaseDetectParameter", "cameraB_valve_amount", "", IniRead, 24, IniPath);
	//m_dlgBaseDetectParametersSettting.GetDlgItem(IDC_EDIT_CAMERA2_VALVE_AMOUNT)->GetWindowTextA(IniRead);
	//parameters_setting_trans.cameraB_valve_amount = atoi(IniRead);
	IniWrite.Format("%d", parameters_setting_trans.cameraB_valve_amount);
	WritePrivateProfileString("BaseDetectParameter", "cameraB_valve_amount", IniWrite, IniPath);


	//GetPrivateProfileStringA("BaseDetectParameter", "cameraC_begin", "", IniRead, 24, IniPath);
	//m_dlgBaseDetectParametersSettting.GetDlgItem(IDC_EDIT_CAMERA3_BEGIN_POINT)->GetWindowTextA(IniRead);
	//parameters_setting_trans.cameraC_begin = atoi(IniRead);
	IniWrite.Format("%d", parameters_setting_trans.cameraC_begin);
	WritePrivateProfileString("BaseDetectParameter", "cameraC_begin", IniWrite, IniPath);


	//GetPrivateProfileStringA("BaseDetectParameter", "cameraC_end", "", IniRead, 24, IniPath);
	//m_dlgBaseDetectParametersSettting.GetDlgItem(IDC_EDIT_CAMERA3_END_POINT)->GetWindowTextA(IniRead);
	//parameters_setting_trans.cameraC_end = atoi(IniRead);
	IniWrite.Format("%d", parameters_setting_trans.cameraC_end);
	WritePrivateProfileString("BaseDetectParameter", "cameraC_end", IniWrite, IniPath);


	//GetPrivateProfileStringA("BaseDetectParameter", "cameraC_align_begin", "", IniRead, 24, IniPath);
	//m_dlgBaseDetectParametersSettting.GetDlgItem(IDC_EDIT_CAMERA3_ALIGNING_BEGIN_POINT)->GetWindowTextA(IniRead);
	//parameters_setting_trans.cameraC_align_begin = atoi(IniRead);
	IniWrite.Format("%d", parameters_setting_trans.cameraC_align_begin);
	WritePrivateProfileString("BaseDetectParameter", "cameraC_align_begin", IniWrite, IniPath);


	//GetPrivateProfileStringA("BaseDetectParameter", "cameraC_align_end", "", IniRead, 24, IniPath);
	//m_dlgBaseDetectParametersSettting.GetDlgItem(IDC_EDIT_CAMERA3_ALIGNING_END_POINT)->GetWindowTextA(IniRead);
	//parameters_setting_trans.cameraC_align_end = atoi(IniRead);
	IniWrite.Format("%d", parameters_setting_trans.cameraC_align_end);
	WritePrivateProfileString("BaseDetectParameter", "cameraC_align_end", IniWrite, IniPath);


	//GetPrivateProfileStringA("BaseDetectParameter", "cameraC_detect_begin", "", IniRead, 24, IniPath);
	//m_dlgBaseDetectParametersSettting.GetDlgItem(IDC_EDIT_CAMERA3_DETECT_BEGIN_POINT)->GetWindowTextA(IniRead);
	//parameters_setting_trans.cameraC_detect_begin = atoi(IniRead);
	IniWrite.Format("%d", parameters_setting_trans.cameraC_detect_begin);
	WritePrivateProfileString("BaseDetectParameter", "cameraC_detect_begin", IniWrite, IniPath);


	//GetPrivateProfileStringA("BaseDetectParameter", "cameraC_detect_end", "", IniRead, 24, IniPath);
	//m_dlgBaseDetectParametersSettting.GetDlgItem(IDC_EDIT_CAMERA3_DETECT_END_POINT)->GetWindowTextA(IniRead);
	//parameters_setting_trans.cameraC_detect_end = atoi(IniRead);
	IniWrite.Format("%d", parameters_setting_trans.cameraC_detect_end);
	WritePrivateProfileString("BaseDetectParameter", "cameraC_detect_end", IniWrite, IniPath);


	//GetPrivateProfileStringA("BaseDetectParameter", "cameraC_valve_amount", "", IniRead, 24, IniPath);
	//m_dlgBaseDetectParametersSettting.GetDlgItem(IDC_EDIT_CAMERA3_VALVE_AMOUNT)->GetWindowTextA(IniRead);
	//parameters_setting_trans.cameraC_valve_amount = atoi(IniRead);
	IniWrite.Format("%d", parameters_setting_trans.cameraC_valve_amount);
	WritePrivateProfileString("BaseDetectParameter", "cameraC_valve_amount", IniWrite, IniPath);


	//GetPrivateProfileStringA("BaseDetectParameter", "cameraD_begin", "", IniRead, 24, IniPath);
	//m_dlgBaseDetectParametersSettting.GetDlgItem(IDC_EDIT_CAMERA4_BEGIN_POINT)->GetWindowTextA(IniRead);
	//parameters_setting_trans.cameraD_begin = atoi(IniRead);
	IniWrite.Format("%d", parameters_setting_trans.cameraD_begin);
	WritePrivateProfileString("BaseDetectParameter", "cameraD_begin", IniWrite, IniPath);


	//GetPrivateProfileStringA("BaseDetectParameter", "cameraD_end", "", IniRead, 24, IniPath);
	//m_dlgBaseDetectParametersSettting.GetDlgItem(IDC_EDIT_CAMERA4_END_POINT)->GetWindowTextA(IniRead);
	//parameters_setting_trans.cameraD_end = atoi(IniRead);
	IniWrite.Format("%d", parameters_setting_trans.cameraD_end);
	WritePrivateProfileString("BaseDetectParameter", "cameraD_end", IniWrite, IniPath);


	//GetPrivateProfileStringA("BaseDetectParameter", "cameraD_align_begin", "", IniRead, 24, IniPath);
	//m_dlgBaseDetectParametersSettting.GetDlgItem(IDC_EDIT_CAMERA4_ALIGNING_BEGIN_POINT)->GetWindowTextA(IniRead);
	//parameters_setting_trans.cameraD_align_begin = atoi(IniRead);
	IniWrite.Format("%d", parameters_setting_trans.cameraD_align_begin);
	WritePrivateProfileString("BaseDetectParameter", "cameraD_align_begin", IniWrite, IniPath);

	//GetPrivateProfileStringA("BaseDetectParameter", "cameraD_align_end", "", IniRead, 24, IniPath);
	//m_dlgBaseDetectParametersSettting.GetDlgItem(IDC_EDIT_CAMERA4_ALIGNING_END_POINT)->GetWindowTextA(IniRead);
	//parameters_setting_trans.cameraD_align_end = atoi(IniRead);
	IniWrite.Format("%d", parameters_setting_trans.cameraD_align_end);
	WritePrivateProfileString("BaseDetectParameter", "cameraD_align_end", IniWrite, IniPath);


	//GetPrivateProfileStringA("BaseDetectParameter", "cameraD_detect_begin", "", IniRead, 24, IniPath);
	//m_dlgBaseDetectParametersSettting.GetDlgItem(IDC_EDIT_CAMERA4_DETECT_BEGIN_POINT)->GetWindowTextA(IniRead);
	//parameters_setting_trans.cameraD_detect_begin = atoi(IniRead);
	IniWrite.Format("%d", parameters_setting_trans.cameraD_detect_begin);
	WritePrivateProfileString("BaseDetectParameter", "cameraD_detect_begin", IniWrite, IniPath);


	//GetPrivateProfileStringA("BaseDetectParameter", "cameraD_detect_end", "", IniRead, 24, IniPath);
	//m_dlgBaseDetectParametersSettting.GetDlgItem(IDC_EDIT_CAMERA4_DETECT_END_POINT)->GetWindowTextA(IniRead);
	//parameters_setting_trans.cameraD_detect_end = atoi(IniRead);
	IniWrite.Format("%d", parameters_setting_trans.cameraD_detect_end);
	WritePrivateProfileString("BaseDetectParameter", "cameraD_detect_end", IniWrite, IniPath);


	//GetPrivateProfileStringA("BaseDetectParameter", "cameraD_valve_amount", "", IniRead, 24, IniPath);
	//m_dlgBaseDetectParametersSettting.GetDlgItem(IDC_EDIT_CAMERA4_VALVE_AMOUNT)->GetWindowTextA(IniRead);
	//parameters_setting_trans.cameraD_valve_amount = atoi(IniRead);
	IniWrite.Format("%d", parameters_setting_trans.cameraD_valve_amount);
	WritePrivateProfileString("BaseDetectParameter", "cameraD_valve_amount", IniWrite, IniPath);


	//Camera A detect parameter setting;
	//GetPrivateProfileStringA("ValveParameter", "cameraA_delay", "", IniRead, 24, IniPath);
	//m_dlgValveParametersSettting.GetDlgItem(IDC_EDIT_VALVE_A_DELAY)->GetWindowTextA(IniRead);
	//parameters_setting_trans.cameraA_delay = atoi(IniRead);
	IniWrite.Format("%d", parameters_setting_trans.cameraA_delay);
	WritePrivateProfileString("ValveParameter", "cameraA_delay", IniWrite, IniPath);


	//GetPrivateProfileStringA("ValveParameter", "cameraA_hold", "", IniRead, 24, IniPath);
	//m_dlgValveParametersSettting.GetDlgItem(IDC_EDIT_VALVE_A_HOLD)->GetWindowTextA(IniRead);
	//parameters_setting_trans.cameraA_hold = atoi(IniRead);
	IniWrite.Format("%d", parameters_setting_trans.cameraA_hold);
	WritePrivateProfileString("ValveParameter", "cameraA_hold", IniWrite, IniPath);


	//GetPrivateProfileStringA("ValveParameter", "cameraB_delay", "", IniRead, 24, IniPath);
	//m_dlgValveParametersSettting.GetDlgItem(IDC_EDIT_VALVE_B_DELAY)->GetWindowTextA(IniRead);
	//parameters_setting_trans.cameraB_delay = atoi(IniRead);
	IniWrite.Format("%d", parameters_setting_trans.cameraB_delay);
	WritePrivateProfileString("ValveParameter", "cameraB_delay", IniWrite, IniPath);
	//-----------------------------------------------------------------------------------------

	//GetPrivateProfileStringA("ValveParameter", "cameraB_hold", "", IniRead, 24, IniPath);
	//m_dlgValveParametersSettting.GetDlgItem(IDC_EDIT_VALVE_B_HOLD)->GetWindowTextA(IniRead);
	//parameters_setting_trans.cameraB_hold = atoi(IniRead);
	IniWrite.Format("%d", parameters_setting_trans.cameraB_hold);
	WritePrivateProfileString("ValveParameter", "cameraB_hold", IniWrite, IniPath);


	//GetPrivateProfileStringA("ValveParameter", "cameraC_delay", "", IniRead, 24, IniPath);
	//m_dlgValveParametersSettting.GetDlgItem(IDC_EDIT_VALVE_C_DELAY)->GetWindowTextA(IniRead);
	//parameters_setting_trans.cameraC_delay = atoi(IniRead);
	IniWrite.Format("%d", parameters_setting_trans.cameraC_delay);
	WritePrivateProfileString("ValveParameter", "cameraC_delay", IniWrite, IniPath);


	//GetPrivateProfileStringA("ValveParameter", "cameraC_hold", "", IniRead, 24, IniPath);
	//m_dlgValveParametersSettting.GetDlgItem(IDC_EDIT_VALVE_C_HOLD)->GetWindowTextA(IniRead);
	//parameters_setting_trans.cameraC_hold = atoi(IniRead);
	IniWrite.Format("%d", parameters_setting_trans.cameraC_hold);
	WritePrivateProfileString("ValveParameter", "cameraC_hold", IniWrite, IniPath);

	//GetPrivateProfileStringA("ValveParameter", "cameraD_delay", "", IniRead, 24, IniPath);
	//m_dlgValveParametersSettting.GetDlgItem(IDC_EDIT_VALVE_D_DELAY)->GetWindowTextA(IniRead);
	//parameters_setting_trans.cameraD_delay = atoi(IniRead);
	IniWrite.Format("%d", parameters_setting_trans.cameraD_delay);
	WritePrivateProfileString("ValveParameter", "cameraD_delay", IniWrite, IniPath);

	//GetPrivateProfileStringA("ValveParameter", "cameraD_hold", "", IniRead, 24, IniPath);
	//m_dlgValveParametersSettting.GetDlgItem(IDC_EDIT_VALVE_D_HOLD)->GetWindowTextA(IniRead);
	//parameters_setting_trans.cameraD_hold = atoi(IniRead);
	IniWrite.Format("%d", parameters_setting_trans.cameraD_hold);
	WritePrivateProfileString("ValveParameter", "cameraD_hold", IniWrite, IniPath);

	//GetPrivateProfileStringA("ValveParameter", "valve_work_times", "", IniRead, 24, IniPath);
	//m_dlgValveParametersSettting.GetDlgItem(IDC_EDIT_VALVE_WORK_TIMES)->GetWindowTextA(IniRead);
	//parameters_setting_trans.valve_work_times = atoi(IniRead);
	IniWrite.Format("%d", parameters_setting_trans.valve_work_times);
	WritePrivateProfileString("ValveParameter", "valve_work_times", IniWrite, IniPath);

	//GetPrivateProfileStringA("ValveParameter", "valve_stop_time", "", IniRead, 24, IniPath);
	//m_dlgValveParametersSettting.GetDlgItem(IDC_EDIT_VALVE_STOP_TIMES)->GetWindowTextA(IniRead);
	//parameters_setting_trans.valve_stop_time = atoi(IniRead);
	IniWrite.Format("%d", parameters_setting_trans.valve_stop_time);
	WritePrivateProfileString("ValveParameter", "valve_stop_time", IniWrite, IniPath);

	//GetPrivateProfileStringA("DetectParameter", "cameraA_point_area_min", "", IniRead, 24, IniPath);
	//m_dlgFiberDetectParametersSettingA.GetDlgItem(IDC_EDIT_POINT_AREA_MIN)->GetWindowTextA(IniRead);
	//parameters_setting_trans.cameraA_point_area_min = atoi(IniRead);
	IniWrite.Format("%d", parameters_setting_trans.cameraA_point_area_min);
	WritePrivateProfileString("DetectParameter", "cameraA_point_area_min", IniWrite, IniPath);

	//GetPrivateProfileStringA("DetectParameter", "cameraA_point_area_max", "", IniRead, 24, IniPath);
	//m_dlgFiberDetectParametersSettingA.GetDlgItem(IDC_EDIT_POINT_AREA_MAX)->GetWindowTextA(IniRead);
	//parameters_setting_trans.cameraA_point_area_max = atoi(IniRead);
	IniWrite.Format("%d", parameters_setting_trans.cameraA_point_area_max);
	WritePrivateProfileString("DetectParameter", "cameraA_point_area_max", IniWrite, IniPath);

	//GetPrivateProfileStringA("DetectParameter", "cameraA_point_length_min", "", IniRead, 24, IniPath);
	//m_dlgFiberDetectParametersSettingA.GetDlgItem(IDC_EDIT_POINT_LENGTH_MIN)->GetWindowTextA(IniRead);
	//parameters_setting_trans.cameraA_point_length_min = atoi(IniRead);
	IniWrite.Format("%d", parameters_setting_trans.cameraA_point_length_min);
	WritePrivateProfileString("DetectParameter", "cameraA_point_length_min", IniWrite, IniPath);

	//GetPrivateProfileStringA("DetectParameter", "cameraA_point_length_max", "", IniRead, 24, IniPath);
	//m_dlgFiberDetectParametersSettingA.GetDlgItem(IDC_EDIT_POINT_LENGTH_MAX)->GetWindowTextA(IniRead);
	//parameters_setting_trans.cameraA_point_length_max = atoi(IniRead);
	IniWrite.Format("%d", parameters_setting_trans.cameraA_point_length_max);
	WritePrivateProfileString("DetectParameter", "cameraA_point_length_max", IniWrite, IniPath);

	//GetPrivateProfileStringA("DetectParameter", "cameraA_point_width_min", "", IniRead, 24, IniPath);
	//m_dlgFiberDetectParametersSettingA.GetDlgItem(IDC_EDIT_POINT_WIDTH_MIN)->GetWindowTextA(IniRead);
	//parameters_setting_trans.cameraA_point_width_min = atoi(IniRead);
	IniWrite.Format("%d", parameters_setting_trans.cameraA_point_width_min);
	WritePrivateProfileString("DetectParameter", "cameraA_point_width_min", IniWrite, IniPath);

	//GetPrivateProfileStringA("DetectParameter", "cameraA_point_width_max", "", IniRead, 24, IniPath);
	//m_dlgFiberDetectParametersSettingA.GetDlgItem(IDC_EDIT_POINT_WIDTH_MAX)->GetWindowTextA(IniRead);
	//parameters_setting_trans.cameraA_point_width_max = atoi(IniRead);
	IniWrite.Format("%d", parameters_setting_trans.cameraA_point_width_max);
	WritePrivateProfileString("DetectParameter", "cameraA_point_width_max", IniWrite, IniPath);

	//GetPrivateProfileStringA("DetectParameter", "cameraA_thread_area_min", "", IniRead, 24, IniPath);
	//m_dlgFiberDetectParametersSettingA.GetDlgItem(IDC_EDIT_THREAD_AREA_MIN)->GetWindowTextA(IniRead);
	//parameters_setting_trans.cameraA_thread_area_min = atoi(IniRead);
	IniWrite.Format("%d", parameters_setting_trans.cameraA_thread_area_min);
	WritePrivateProfileString("DetectParameter", "cameraA_thread_area_min", IniWrite, IniPath);

	//GetPrivateProfileStringA("DetectParameter", "cameraA_thread_area_max", "", IniRead, 24, IniPath);
	//m_dlgFiberDetectParametersSettingA.GetDlgItem(IDC_EDIT_THREAD_AREA_MAX)->GetWindowTextA(IniRead);
	//parameters_setting_trans.cameraA_thread_area_max = atoi(IniRead);
	IniWrite.Format("%d", parameters_setting_trans.cameraA_thread_area_max);
	WritePrivateProfileString("DetectParameter", "cameraA_thread_area_max", IniWrite, IniPath);

	//GetPrivateProfileStringA("DetectParameter", "cameraA_thread_length_min", "", IniRead, 24, IniPath);
	//m_dlgFiberDetectParametersSettingA.GetDlgItem(IDC_EDIT_THREAD_LENGTH_MIN)->GetWindowTextA(IniRead);
	//parameters_setting_trans.cameraA_thread_length_min = atoi(IniRead);
	IniWrite.Format("%d", parameters_setting_trans.cameraA_thread_length_min);
	WritePrivateProfileString("DetectParameter", "cameraA_thread_length_min", IniWrite, IniPath);

	//GetPrivateProfileStringA("DetectParameter", "cameraA_thread_length_max", "", IniRead, 24, IniPath);
	//m_dlgFiberDetectParametersSettingA.GetDlgItem(IDC_EDIT_THREAD_LENGTH_MAX)->GetWindowTextA(IniRead);
	//parameters_setting_trans.cameraA_thread_length_max = atoi(IniRead);
	IniWrite.Format("%d", parameters_setting_trans.cameraA_thread_length_max);
	WritePrivateProfileString("DetectParameter", "cameraA_thread_length_max", IniWrite, IniPath);

	//GetPrivateProfileStringA("DetectParameter", "cameraA_thread_width_min", "", IniRead, 24, IniPath);
	//m_dlgFiberDetectParametersSettingA.GetDlgItem(IDC_EDIT_THREAD_WIDTH_MIN)->GetWindowTextA(IniRead);
	//parameters_setting_trans.cameraA_thread_width_min = atoi(IniRead);
	IniWrite.Format("%d", parameters_setting_trans.cameraA_thread_width_min);
	WritePrivateProfileString("DetectParameter", "cameraA_thread_width_min", IniWrite, IniPath);

	//GetPrivateProfileStringA("DetectParameter", "cameraA_thread_width_max", "", IniRead, 24, IniPath);
	//m_dlgFiberDetectParametersSettingA.GetDlgItem(IDC_EDIT_THREAD_WIDTH_MAX)->GetWindowTextA(IniRead);
	//parameters_setting_trans.cameraA_thread_width_max = atoi(IniRead);
	IniWrite.Format("%d", parameters_setting_trans.cameraA_thread_width_max);
	WritePrivateProfileString("DetectParameter", "cameraA_thread_width_max", IniWrite, IniPath);


	//Camera B detect parameter setting;
	//GetPrivateProfileStringA("DetectParameter", "cameraB_point_area_min", "", IniRead, 24, IniPath);
	//m_dlgFiberDetectParametersSettingB.GetDlgItem(IDC_EDIT_POINT_AREA_MIN)->GetWindowTextA(IniRead);
	//parameters_setting_trans.cameraB_point_area_min = atoi(IniRead);
	IniWrite.Format("%d", parameters_setting_trans.cameraB_point_area_min);
	WritePrivateProfileString("DetectParameter", "cameraB_point_area_min", IniWrite, IniPath);

	//GetPrivateProfileStringA("DetectParameter", "cameraB_point_area_max", "", IniRead, 24, IniPath);
	//m_dlgFiberDetectParametersSettingB.GetDlgItem(IDC_EDIT_POINT_AREA_MAX)->SetWindowTextA(IniRead);
	//parameters_setting_trans.cameraB_point_area_max = atoi(IniRead);
	IniWrite.Format("%d", parameters_setting_trans.cameraB_point_area_max);
	WritePrivateProfileString("DetectParameter", "cameraB_point_area_max", IniWrite, IniPath);

	//GetPrivateProfileStringA("DetectParameter", "cameraB_point_length_min", "", IniRead, 24, IniPath);
	//m_dlgFiberDetectParametersSettingB.GetDlgItem(IDC_EDIT_POINT_LENGTH_MIN)->GetWindowTextA(IniRead);
	//parameters_setting_trans.cameraB_point_length_min = atoi(IniRead);
	IniWrite.Format("%d", parameters_setting_trans.cameraB_point_length_min);
	WritePrivateProfileString("DetectParameter", "cameraB_point_length_min", IniWrite, IniPath);

	//GetPrivateProfileStringA("DetectParameter", "cameraB_point_length_max", "", IniRead, 24, IniPath);
	//m_dlgFiberDetectParametersSettingB.GetDlgItem(IDC_EDIT_POINT_LENGTH_MAX)->GetWindowTextA(IniRead);
	//parameters_setting_trans.cameraB_point_length_max = atoi(IniRead);
	IniWrite.Format("%d", parameters_setting_trans.cameraB_point_length_max);
	WritePrivateProfileString("DetectParameter", "cameraB_point_length_max", IniWrite, IniPath);

	//GetPrivateProfileStringA("DetectParameter", "cameraB_point_width_min", "", IniRead, 24, IniPath);
	//m_dlgFiberDetectParametersSettingB.GetDlgItem(IDC_EDIT_POINT_WIDTH_MIN)->GetWindowTextA(IniRead);
	//parameters_setting_trans.cameraB_point_width_min = atoi(IniRead);
	IniWrite.Format("%d", parameters_setting_trans.cameraB_point_width_min);
	WritePrivateProfileString("DetectParameter", "cameraB_point_width_min", IniWrite, IniPath);

	//GetPrivateProfileStringA("DetectParameter", "cameraB_point_width_max", "", IniRead, 24, IniPath);
	//m_dlgFiberDetectParametersSettingB.GetDlgItem(IDC_EDIT_POINT_WIDTH_MAX)->GetWindowTextA(IniRead);
	//parameters_setting_trans.cameraB_point_width_max = atoi(IniRead);
	IniWrite.Format("%d", parameters_setting_trans.cameraB_point_width_max);
	WritePrivateProfileString("DetectParameter", "cameraB_point_width_max", IniWrite, IniPath);

	//GetPrivateProfileStringA("DetectParameter", "cameraB_thread_area_min", "", IniRead, 24, IniPath);
	//m_dlgFiberDetectParametersSettingB.GetDlgItem(IDC_EDIT_THREAD_AREA_MIN)->GetWindowTextA(IniRead);
	//parameters_setting_trans.cameraB_thread_area_min = atoi(IniRead);
	IniWrite.Format("%d", parameters_setting_trans.cameraB_thread_area_min);
	WritePrivateProfileString("DetectParameter", "cameraB_thread_area_min", IniWrite, IniPath);

	//GetPrivateProfileStringA("DetectParameter", "cameraB_thread_area_max", "", IniRead, 24, IniPath);
	//m_dlgFiberDetectParametersSettingB.GetDlgItem(IDC_EDIT_THREAD_AREA_MAX)->GetWindowTextA(IniRead);
	//parameters_setting_trans.cameraB_thread_area_max = atoi(IniRead);
	IniWrite.Format("%d", parameters_setting_trans.cameraB_thread_area_max);
	WritePrivateProfileString("DetectParameter", "cameraB_thread_area_max", IniWrite, IniPath);

	//GetPrivateProfileStringA("DetectParameter", "cameraB_thread_length_min", "", IniRead, 24, IniPath);
	//m_dlgFiberDetectParametersSettingB.GetDlgItem(IDC_EDIT_THREAD_LENGTH_MIN)->GetWindowTextA(IniRead);
	//parameters_setting_trans.cameraB_thread_length_min = atoi(IniRead);
	IniWrite.Format("%d", parameters_setting_trans.cameraB_thread_length_min);
	WritePrivateProfileString("DetectParameter", "cameraB_thread_length_min", IniWrite, IniPath);

	//GetPrivateProfileStringA("DetectParameter", "cameraB_thread_length_max", "", IniRead, 24, IniPath);
	//m_dlgFiberDetectParametersSettingB.GetDlgItem(IDC_EDIT_THREAD_LENGTH_MAX)->GetWindowTextA(IniRead);
	//parameters_setting_trans.cameraB_thread_length_max = atoi(IniRead);
	IniWrite.Format("%d", parameters_setting_trans.cameraB_thread_length_max);
	WritePrivateProfileString("DetectParameter", "cameraB_thread_length_max", IniWrite, IniPath);

	//GetPrivateProfileStringA("DetectParameter", "cameraB_thread_width_min", "", IniRead, 24, IniPath);
	//m_dlgFiberDetectParametersSettingB.GetDlgItem(IDC_EDIT_THREAD_WIDTH_MIN)->GetWindowTextA(IniRead);
	//parameters_setting_trans.cameraB_thread_width_min = atoi(IniRead);
	IniWrite.Format("%d", parameters_setting_trans.cameraB_thread_width_min);
	WritePrivateProfileString("DetectParameter", "cameraB_thread_width_min", IniWrite, IniPath);

	//GetPrivateProfileStringA("DetectParameter", "cameraB_thread_width_max", "", IniRead, 24, IniPath);
	//m_dlgFiberDetectParametersSettingB.GetDlgItem(IDC_EDIT_THREAD_WIDTH_MAX)->GetWindowTextA(IniRead);
	//parameters_setting_trans.cameraB_thread_width_max = atoi(IniRead);
	IniWrite.Format("%d", parameters_setting_trans.cameraB_thread_width_max);
	WritePrivateProfileString("DetectParameter", "cameraB_thread_width_max", IniWrite, IniPath);


	//Camera C detect parameter setting;
	//GetPrivateProfileStringA("DetectParameter", "cameraC_point_area_min", "", IniRead, 24, IniPath);
	//m_dlgFiberDetectParametersSettingC.GetDlgItem(IDC_EDIT_POINT_AREA_MIN)->GetWindowTextA(IniRead);
	//parameters_setting_trans.cameraC_point_area_min = atoi(IniRead);
	IniWrite.Format("%d", parameters_setting_trans.cameraC_point_area_min);
	WritePrivateProfileString("DetectParameter", "cameraC_point_area_min", IniWrite, IniPath);

	//GetPrivateProfileStringA("DetectParameter", "cameraC_point_area_max", "", IniRead, 24, IniPath);
	//m_dlgFiberDetectParametersSettingC.GetDlgItem(IDC_EDIT_POINT_AREA_MAX)->GetWindowTextA(IniRead);
	//parameters_setting_trans.cameraC_point_area_max = atoi(IniRead);
	IniWrite.Format("%d", parameters_setting_trans.cameraC_point_area_max);
	WritePrivateProfileString("DetectParameter", "cameraC_point_area_max", IniWrite, IniPath);

	//GetPrivateProfileStringA("DetectParameter", "cameraC_point_length_min", "", IniRead, 24, IniPath);
	//m_dlgFiberDetectParametersSettingC.GetDlgItem(IDC_EDIT_POINT_LENGTH_MIN)->GetWindowTextA(IniRead);
	//parameters_setting_trans.cameraC_point_length_min = atoi(IniRead);
	IniWrite.Format("%d", parameters_setting_trans.cameraC_point_length_min);
	WritePrivateProfileString("DetectParameter", "cameraC_point_length_min", IniWrite, IniPath);

	//GetPrivateProfileStringA("DetectParameter", "cameraC_point_length_max", "", IniRead, 24, IniPath);
	//m_dlgFiberDetectParametersSettingC.GetDlgItem(IDC_EDIT_POINT_LENGTH_MAX)->GetWindowTextA(IniRead);
	//parameters_setting_trans.cameraC_point_length_max = atoi(IniRead);
	IniWrite.Format("%d", parameters_setting_trans.cameraC_point_length_max);
	WritePrivateProfileString("DetectParameter", "cameraC_point_length_max", IniWrite, IniPath);

	//GetPrivateProfileStringA("DetectParameter", "cameraC_point_width_min", "", IniRead, 24, IniPath);
	//m_dlgFiberDetectParametersSettingC.GetDlgItem(IDC_EDIT_POINT_WIDTH_MIN)->GetWindowTextA(IniRead);
	//parameters_setting_trans.cameraC_point_width_min = atoi(IniRead);
	IniWrite.Format("%d", parameters_setting_trans.cameraC_point_width_min);
	WritePrivateProfileString("DetectParameter", "cameraC_point_width_min", IniWrite, IniPath);

	//GetPrivateProfileStringA("DetectParameter", "cameraC_point_width_max", "", IniRead, 24, IniPath);
	//m_dlgFiberDetectParametersSettingC.GetDlgItem(IDC_EDIT_POINT_WIDTH_MAX)->GetWindowTextA(IniRead);
	//parameters_setting_trans.cameraC_point_width_max = atoi(IniRead);
	IniWrite.Format("%d", parameters_setting_trans.cameraC_point_width_max);
	WritePrivateProfileString("DetectParameter", "cameraC_point_width_max", IniWrite, IniPath);

	//GetPrivateProfileStringA("DetectParameter", "cameraC_thread_area_min", "", IniRead, 24, IniPath);
	//m_dlgFiberDetectParametersSettingC.GetDlgItem(IDC_EDIT_THREAD_AREA_MIN)->GetWindowTextA(IniRead);
	//parameters_setting_trans.cameraC_thread_area_min = atoi(IniRead);
	IniWrite.Format("%d", parameters_setting_trans.cameraC_thread_area_min);
	WritePrivateProfileString("DetectParameter", "cameraC_thread_area_min", IniWrite, IniPath);

	//GetPrivateProfileStringA("DetectParameter", "cameraC_thread_area_max", "", IniRead, 24, IniPath);
	//m_dlgFiberDetectParametersSettingC.GetDlgItem(IDC_EDIT_THREAD_AREA_MAX)->GetWindowTextA(IniRead);
	//parameters_setting_trans.cameraC_thread_area_max = atoi(IniRead);
	IniWrite.Format("%d", parameters_setting_trans.cameraC_thread_area_max);
	WritePrivateProfileString("DetectParameter", "cameraC_thread_area_max", IniWrite, IniPath);

	//GetPrivateProfileStringA("DetectParameter", "cameraC_thread_length_min", "", IniRead, 24, IniPath);
	//m_dlgFiberDetectParametersSettingC.GetDlgItem(IDC_EDIT_THREAD_LENGTH_MIN)->GetWindowTextA(IniRead);
	//parameters_setting_trans.cameraC_thread_length_min = atoi(IniRead);
	IniWrite.Format("%d", parameters_setting_trans.cameraC_thread_length_min);
	WritePrivateProfileString("DetectParameter", "cameraC_thread_length_min", IniWrite, IniPath);

	//GetPrivateProfileStringA("DetectParameter", "cameraC_thread_length_max", "", IniRead, 24, IniPath);
	//m_dlgFiberDetectParametersSettingC.GetDlgItem(IDC_EDIT_THREAD_LENGTH_MAX)->GetWindowTextA(IniRead);
	//parameters_setting_trans.cameraC_thread_length_max = atoi(IniRead);
	IniWrite.Format("%d", parameters_setting_trans.cameraC_thread_length_max);
	WritePrivateProfileString("DetectParameter", "cameraC_thread_length_max", IniWrite, IniPath);

	//GetPrivateProfileStringA("DetectParameter", "cameraC_thread_width_min", "", IniRead, 24, IniPath);
	//m_dlgFiberDetectParametersSettingC.GetDlgItem(IDC_EDIT_THREAD_WIDTH_MIN)->GetWindowTextA(IniRead);
	//parameters_setting_trans.cameraC_thread_width_min = atoi(IniRead);
	IniWrite.Format("%d", parameters_setting_trans.cameraC_thread_width_min);
	WritePrivateProfileString("DetectParameter", "cameraC_thread_width_min", IniWrite, IniPath);

	//GetPrivateProfileStringA("DetectParameter", "cameraC_thread_width_max", "", IniRead, 24, IniPath);
	//m_dlgFiberDetectParametersSettingC.GetDlgItem(IDC_EDIT_THREAD_WIDTH_MAX)->GetWindowTextA(IniRead);
	//parameters_setting_trans.cameraC_thread_width_max = atoi(IniRead);
	IniWrite.Format("%d", parameters_setting_trans.cameraC_thread_width_max);
	WritePrivateProfileString("DetectParameter", "cameraC_thread_width_max", IniWrite, IniPath);


	//Camera D detect parameter setting;
	//GetPrivateProfileStringA("DetectParameter", "cameraD_point_area_min", "", IniRead, 24, IniPath);
	//m_dlgFiberDetectParametersSettingD.GetDlgItem(IDC_EDIT_POINT_AREA_MIN)->GetWindowTextA(IniRead);
	//parameters_setting_trans.cameraD_point_area_min = atoi(IniRead);
	IniWrite.Format("%d", parameters_setting_trans.cameraD_point_area_min);
	WritePrivateProfileString("DetectParameter", "cameraD_point_area_min", IniWrite, IniPath);

	//GetPrivateProfileStringA("DetectParameter", "cameraD_point_area_max", "", IniRead, 24, IniPath);
	//m_dlgFiberDetectParametersSettingD.GetDlgItem(IDC_EDIT_POINT_AREA_MAX)->GetWindowTextA(IniRead);
	//parameters_setting_trans.cameraD_point_area_max = atoi(IniRead);
	IniWrite.Format("%d", parameters_setting_trans.cameraD_point_area_max);
	WritePrivateProfileString("DetectParameter", "cameraD_point_area_max", IniWrite, IniPath);

	//GetPrivateProfileStringA("DetectParameter", "cameraD_point_length_min", "", IniRead, 24, IniPath);
	//m_dlgFiberDetectParametersSettingD.GetDlgItem(IDC_EDIT_POINT_LENGTH_MIN)->GetWindowTextA(IniRead);
	//parameters_setting_trans.cameraD_point_length_min = atoi(IniRead);
	IniWrite.Format("%d", parameters_setting_trans.cameraD_point_length_min);
	WritePrivateProfileString("DetectParameter", "cameraD_point_length_min", IniWrite, IniPath);

	//GetPrivateProfileStringA("DetectParameter", "cameraD_point_length_max", "", IniRead, 24, IniPath);
	//m_dlgFiberDetectParametersSettingD.GetDlgItem(IDC_EDIT_POINT_LENGTH_MAX)->GetWindowTextA(IniRead);
	//parameters_setting_trans.cameraD_point_length_max = atoi(IniRead);
	IniWrite.Format("%d", parameters_setting_trans.cameraD_point_length_max);
	WritePrivateProfileString("DetectParameter", "cameraD_point_length_max", IniWrite, IniPath);

	//GetPrivateProfileStringA("DetectParameter", "cameraD_point_width_min", "", IniRead, 24, IniPath);
	//m_dlgFiberDetectParametersSettingD.GetDlgItem(IDC_EDIT_POINT_WIDTH_MIN)->GetWindowTextA(IniRead);
	//parameters_setting_trans.cameraD_point_width_min = atoi(IniRead);
	IniWrite.Format("%d", parameters_setting_trans.cameraD_point_width_min);
	WritePrivateProfileString("DetectParameter", "cameraD_point_width_min", IniWrite, IniPath);

	//GetPrivateProfileStringA("DetectParameter", "cameraD_point_width_max", "", IniRead, 24, IniPath);
	//m_dlgFiberDetectParametersSettingD.GetDlgItem(IDC_EDIT_POINT_WIDTH_MAX)->GetWindowTextA(IniRead);
	//parameters_setting_trans.cameraD_point_width_max = atoi(IniRead);
	IniWrite.Format("%d", parameters_setting_trans.cameraD_point_width_max);
	WritePrivateProfileString("DetectParameter", "cameraD_point_width_max", IniWrite, IniPath);

	//GetPrivateProfileStringA("DetectParameter", "cameraD_thread_area_min", "", IniRead, 24, IniPath);
	//m_dlgFiberDetectParametersSettingD.GetDlgItem(IDC_EDIT_THREAD_AREA_MIN)->GetWindowTextA(IniRead);
	//parameters_setting_trans.cameraD_thread_area_min = atoi(IniRead);
	IniWrite.Format("%d", parameters_setting_trans.cameraD_thread_area_min);
	WritePrivateProfileString("DetectParameter", "cameraD_thread_area_min", IniWrite, IniPath);

	//GetPrivateProfileStringA("DetectParameter", "cameraD_thread_area_max", "", IniRead, 24, IniPath);
	//m_dlgFiberDetectParametersSettingD.GetDlgItem(IDC_EDIT_THREAD_AREA_MAX)->GetWindowTextA(IniRead);
	//parameters_setting_trans.cameraD_thread_area_max = atoi(IniRead);
	IniWrite.Format("%d", parameters_setting_trans.cameraD_thread_area_max);
	WritePrivateProfileString("DetectParameter", "cameraD_thread_area_max", IniWrite, IniPath);

	//GetPrivateProfileStringA("DetectParameter", "cameraD_thread_length_min", "", IniRead, 24, IniPath);
	//m_dlgFiberDetectParametersSettingD.GetDlgItem(IDC_EDIT_THREAD_LENGTH_MIN)->GetWindowTextA(IniRead);
	//parameters_setting_trans.cameraD_thread_length_min = atoi(IniRead);
	IniWrite.Format("%d", parameters_setting_trans.cameraD_thread_length_min);
	WritePrivateProfileString("DetectParameter", "cameraD_thread_length_min", IniWrite, IniPath);

	//GetPrivateProfileStringA("DetectParameter", "cameraD_thread_length_max", "", IniRead, 24, IniPath);
	//m_dlgFiberDetectParametersSettingD.GetDlgItem(IDC_EDIT_THREAD_LENGTH_MAX)->GetWindowTextA(IniRead);
	//parameters_setting_trans.cameraD_thread_length_max = atoi(IniRead);
	IniWrite.Format("%d", parameters_setting_trans.cameraD_thread_length_max);
	WritePrivateProfileString("DetectParameter", "cameraD_thread_length_max", IniWrite, IniPath);

	//GetPrivateProfileStringA("DetectParameter", "cameraD_thread_width_min", "", IniRead, 24, IniPath);
	//m_dlgFiberDetectParametersSettingD.GetDlgItem(IDC_EDIT_THREAD_WIDTH_MIN)->GetWindowTextA(IniRead);
	//parameters_setting_trans.cameraD_thread_width_min = atoi(IniRead);
	IniWrite.Format("%d", parameters_setting_trans.cameraD_thread_width_min);
	WritePrivateProfileString("DetectParameter", "cameraD_thread_width_min", IniWrite, IniPath);

	//GetPrivateProfileStringA("DetectParameter", "cameraD_thread_width_max", "", IniRead, 24, IniPath);
	//m_dlgFiberDetectParametersSettingD.GetDlgItem(IDC_EDIT_THREAD_WIDTH_MAX)->GetWindowTextA(IniRead);
	//parameters_setting_trans.cameraD_thread_width_max = atoi(IniRead);
	IniWrite.Format("%d", parameters_setting_trans.cameraD_thread_width_max);
	WritePrivateProfileString("DetectParameter", "cameraD_thread_width_max", IniWrite, IniPath);


	//DebugParaneters setting;
	//GetPrivateProfileStringA("DebugParameter", "debug_cameraA", "", IniRead, 24, IniPath);
	//parameters_setting_trans.debug_cameraA = m_dlgDebugParametersSetting.m_checkDebugCameraA.GetCheck();
	IniWrite.Format("%d", parameters_setting_trans.debug_cameraA);
	WritePrivateProfileString("DebugParameter", "debug_cameraA", IniWrite, IniPath);


	//GetPrivateProfileStringA("DebugParameter", "debug_cameraB", "", IniRead, 24, IniPath);
	//parameters_setting_trans.debug_cameraB = m_dlgDebugParametersSetting.m_checkDebugCameraB.GetCheck();
	IniWrite.Format("%d", parameters_setting_trans.debug_cameraB);
	WritePrivateProfileString("DebugParameter", "debug_cameraB", IniWrite, IniPath);

	//GetPrivateProfileStringA("DebugParameter", "debug_cameraC", "", IniRead, 24, IniPath);
	//parameters_setting_trans.debug_cameraC = m_dlgDebugParametersSetting.m_checkDebugCameraC.GetCheck();
	IniWrite.Format("%d", parameters_setting_trans.debug_cameraC);
	WritePrivateProfileString("DebugParameter", "debug_cameraC", IniWrite, IniPath);

	//GetPrivateProfileStringA("DebugParameter", "debug_cameraD", "", IniRead, 24, IniPath);
	//parameters_setting_trans.debug_cameraD = m_dlgDebugParametersSetting.m_checkDebugCameraD.GetCheck();
	IniWrite.Format("%d", parameters_setting_trans.debug_cameraD);
	WritePrivateProfileString("DebugParameter", "debug_cameraD", IniWrite, IniPath);

	//GetPrivateProfileStringA("DebugParameter", "stop_volve", "", IniRead, 24, IniPath);
	//parameters_setting_trans.stop_volve = m_dlgDebugParametersSetting.m_checkStopValve.GetCheck();
	IniWrite.Format("%d", parameters_setting_trans.stop_volve);
	WritePrivateProfileString("DebugParameter", "stop_volve", IniWrite, IniPath);

	//GetPrivateProfileStringA("DebugParameter", "save_blob", "", IniRead, 24, IniPath);
	//parameters_setting_trans.save_blob = m_dlgDebugParametersSetting.m_checkSaveBolb.GetCheck();
	IniWrite.Format("%d", parameters_setting_trans.save_blob);
	WritePrivateProfileString("DebugParameter", "save_blob", IniWrite, IniPath);

	return;
}


void CFiberDetectControlDlg::OnBnClickedButtonDisplaySetting()
{
	// TODO:  在此添加控件通知处理程序代码
}

/*
void CFiberDetectControlDlg::DrawMat(cv::Mat& img, UINT nID)
{
	cv::Mat imgTmp = img;
	CRect rect;
	GetDlgItem(nID)->GetClientRect(&rect); // 获取控件大小  
	//cv::resize(img, imgTmp, cv::Size(rect.Width(), rect.Height()));// 缩放Mat并备份 // 转一下格式 ,这段可以放外面, 
	switch (imgTmp.channels())
	{
	case 1:
		cv::cvtColor(imgTmp, imgTmp, CV_GRAY2BGRA); // GRAY单通道
		break;
	case 3:
		cv::cvtColor(imgTmp, imgTmp, CV_BGR2BGRA); // BGR三通道 
		break;
	default:
		break;
	}
	float magnification = (float)(rect.Width()) / (float)(imgTmp.cols);
	float comapre_magnification = (float)(rect.Height()) / (float)(imgTmp.rows);
	if (comapre_magnification >= magnification)
	{
		cv::Mat image_max;
		cv::resize(imgTmp, image_max, Size(imgTmp.cols * magnification, imgTmp.rows * magnification));
		int pixelBytes = image_max.channels() * (image_max.depth() + 1); // 计算一个像素多少个字节 // 制作bitmapinfo(数据头) 
		BITMAPINFO bitInfo;
		bitInfo.bmiHeader.biBitCount = 8 * pixelBytes;
		bitInfo.bmiHeader.biWidth = image_max.cols;
		bitInfo.bmiHeader.biHeight = -image_max.rows;
		bitInfo.bmiHeader.biPlanes = 1;
		bitInfo.bmiHeader.biSize = sizeof(BITMAPINFOHEADER); bitInfo.bmiHeader.biCompression = BI_RGB;
		bitInfo.bmiHeader.biClrImportant = 0;
		bitInfo.bmiHeader.biClrUsed = 0;
		bitInfo.bmiHeader.biSizeImage = 0;
		bitInfo.bmiHeader.biXPelsPerMeter = 0;
		bitInfo.bmiHeader.biYPelsPerMeter = 0; // Mat.data + bitmap数据头 -> MFC 
		CDC* pDC = GetDlgItem(nID)->GetDC();
		int nYbegin = 0;
		nYbegin = (rect.Height() - image_max.rows) / 2;

		CDC memDC;
		CDC * dc = GetDC();
		CBitmap *oldBitmap;
		memDC.CreateCompatibleDC(pDC);
		//CRect rect;
		//GetDlgItem(IDC_STATIC_IMAGE_SHOW)->GetClientRect(&rect); // 获取控件大小  
		//ScreenToClient(&rect);
		CBitmap bufferMemBitmap;
		GetClientRect(&rect);
		//CRect rect1;
		//GetDlgItem(IDC_BUTTON_HARDWARE_TEST)->GetWindowRect(&rect1);//获取控件的屏幕坐标
		//GetDlgItem(IDC_BUTTON_HARDWARE_TEST)->GetWindowRect(&rect1);//获取控件的屏幕坐标
		//ScreenToClient(&rect1);//转换为对话框上的客户坐标
		CRect rect2;
		GetDlgItem(IDC_BUTTON_CAMERA1)->GetWindowRect(&rect2);
		ScreenToClient(&rect2);
		bufferMemBitmap.CreateCompatibleBitmap(pDC, image_max.cols, rect1.top-5 );
		oldBitmap = memDC.SelectObject(&bufferMemBitmap);
		CBrush brush = GetSysColor(COLOR_WINDOW);
		//memDC.FillRect(CRect(0, 0, rect.Width(), rect.Height()), &brush);
		memDC.FillRect(CRect(rect.left, rect.top, image_max.cols, rect1.top-5), &brush);
		// 使用memDC做绘图操作

		// 将内存缓冲DC拷贝至当前DC
		pDC->BitBlt(rect.left, rect.top, image_max.cols, rect1.top-5, &memDC, 0, 0, SRCCOPY);
		bufferMemBitmap.DeleteObject();
		brush.DeleteObject();
		memDC.DeleteDC();
		::StretchDIBits(pDC->GetSafeHdc(), 0, nYbegin, image_max.cols, image_max.rows, 0, 0, image_max.cols, image_max.rows, image_max.data, &bitInfo, DIB_RGB_COLORS, SRCCOPY);
		ReleaseDC(pDC);
	}
	else
	{
		int pixelBytes = imgTmp.channels() * (imgTmp.depth() + 1); // 计算一个像素多少个字节 // 制作bitmapinfo(数据头) 
		BITMAPINFO bitInfo;
		bitInfo.bmiHeader.biBitCount = 8 * pixelBytes;
		bitInfo.bmiHeader.biWidth = imgTmp.cols;
		bitInfo.bmiHeader.biHeight = -imgTmp.rows;
		bitInfo.bmiHeader.biPlanes = 1;
		bitInfo.bmiHeader.biSize = sizeof(BITMAPINFOHEADER); bitInfo.bmiHeader.biCompression = BI_RGB;
		bitInfo.bmiHeader.biClrImportant = 0;
		bitInfo.bmiHeader.biClrUsed = 0;
		bitInfo.bmiHeader.biSizeImage = 0;
		bitInfo.bmiHeader.biXPelsPerMeter = 0;
		bitInfo.bmiHeader.biYPelsPerMeter = 0; // Mat.data + bitmap数据头 -> MFC 
		CDC* pDC = GetDlgItem(nID)->GetDC();
		int nYbegin = 0;
		int nHeight = int(100.00*(float)(rect.Width()) / (float)(imgTmp.cols));
		nYbegin = (rect.Height() - nHeight) / 2;

		::StretchDIBits(pDC->GetSafeHdc(), 0, nYbegin, rect.Width(), nHeight, 0, 0, imgTmp.cols, imgTmp.rows, imgTmp.data, &bitInfo, DIB_RGB_COLORS, SRCCOPY);
		ReleaseDC(pDC);
	}
}
*/

void CFiberDetectControlDlg::DrawMat(cv::Mat& img, UINT nID)
{
	cv::Mat imgTmp = img;
	CRect rect;
	GetDlgItem(nID)->GetClientRect(&rect); // 获取控件大小  
	//cv::resize(img, imgTmp, cv::Size(rect.Width(), rect.Height()));// 缩放Mat并备份 // 转一下格式 ,这段可以放外面,
	switch (imgTmp.channels())
	{
	case 1:
		cv::cvtColor(imgTmp, imgTmp, CV_GRAY2BGRA); // GRAY单通道
		break;
	case 3:
		cv::cvtColor(imgTmp, imgTmp, CV_BGR2BGRA); // BGR三通道 
		break;
	default:
		break;
	}
	float magnification = (float)(rect.Width()) / (float)(imgTmp.cols);
	cv::Mat image_max;
	cv::resize(imgTmp, image_max, Size(imgTmp.cols * magnification, imgTmp.rows * magnification));
	//int pixelBytes = imgTmp.channels() * (imgTmp.depth() + 1); // 计算一个像素多少个字节 // 制作bitmapinfo(数据头) 
	//BITMAPINFO bitInfo;
	//bitInfo.bmiHeader.biBitCount = 8 * pixelBytes;
	//bitInfo.bmiHeader.biWidth = imgTmp.cols;
	//bitInfo.bmiHeader.biHeight = -imgTmp.rows;
	//bitInfo.bmiHeader.biPlanes = 1;
	//bitInfo.bmiHeader.biSize = sizeof(BITMAPINFOHEADER); bitInfo.bmiHeader.biCompression = BI_RGB;
	//bitInfo.bmiHeader.biClrImportant = 0;
	//bitInfo.bmiHeader.biClrUsed = 0;
	//bitInfo.bmiHeader.biSizeImage = 0; 
	//bitInfo.bmiHeader.biXPelsPerMeter = 0;
	//bitInfo.bmiHeader.biYPelsPerMeter = 0; // Mat.data + bitmap数据头 -> MFC 
	int pixelBytes = image_max.channels() * (image_max.depth() + 1); // 计算一个像素多少个字节 // 制作bitmapinfo(数据头) 
	BITMAPINFO bitInfo;
	bitInfo.bmiHeader.biBitCount = 8 * pixelBytes;
	bitInfo.bmiHeader.biWidth = image_max.cols;
	bitInfo.bmiHeader.biHeight = -image_max.rows;
	bitInfo.bmiHeader.biPlanes = 1;
	bitInfo.bmiHeader.biSize = sizeof(BITMAPINFOHEADER); bitInfo.bmiHeader.biCompression = BI_RGB;
	bitInfo.bmiHeader.biClrImportant = 0;
	bitInfo.bmiHeader.biClrUsed = 0;
	bitInfo.bmiHeader.biSizeImage = 0;
	bitInfo.bmiHeader.biXPelsPerMeter = 0;
	bitInfo.bmiHeader.biYPelsPerMeter = 0; // Mat.data + bitmap数据头 -> MFC 
	CDC* pDC = GetDlgItem(nID)->GetDC();
	int nYbegin = 0;
	//int nHeight = int(100.00*(float)(rect.Width()) / (float)(imgTmp.cols));
	//nYbegin = (rect.Height() - nHeight) / 2;
	nYbegin = (rect.Height() - image_max.rows) / 2;
	//::StretchDIBits(pDC->GetSafeHdc(), 0, nYbegin, rect.Width(), nHeight, 0, 0, imgTmp.cols, imgTmp.rows, imgTmp.data, &bitInfo, DIB_RGB_COLORS, SRCCOPY);
	::StretchDIBits(pDC->GetSafeHdc(), 0, nYbegin, image_max.cols, image_max.rows, 0, 0, image_max.cols, image_max.rows, image_max.data, &bitInfo, DIB_RGB_COLORS, SRCCOPY);
	ReleaseDC(pDC);
}

void CFiberDetectControlDlg::camera_show_image(int camera_index, cv::Mat show_img)
{
	switch (camera_index)
	{
	case 1:
		DrawMat(show_img, IDC_STATIC_IMAGE_SHOW1); 
		break;
	case 2:
		DrawMat(show_img, IDC_STATIC_IMAGE_SHOW2); 
		break;
	case 3:
		DrawMat(show_img, IDC_STATIC_IMAGE_SHOW3); 
		break;
	case 4:
		DrawMat(show_img, IDC_STATIC_IMAGE_SHOW4); 
		break;
	default:
		break;
	}
}
LRESULT CFiberDetectControlDlg::Setwindow(WPARAM wParam, LPARAM lParam)
{
	int nCtrlSignalling = (int)wParam;
	if (nCtrlSignalling)
	{
		GetDlgItem(IDC_BUTTON_START_DETECT)->EnableWindow(true);
		GetDlgItem(IDC_BUTTON_STOP_DETECT2)->EnableWindow(false);
	}
	else
	{
		GetDlgItem(IDC_BUTTON_START_DETECT)->EnableWindow(false);
		GetDlgItem(IDC_BUTTON_STOP_DETECT2)->EnableWindow(true);
	}
	return 1;
}

LRESULT CFiberDetectControlDlg::UpdateImg(WPARAM wParam, LPARAM lParam)
{
	static int number_A = 0;
	static int number_B = 0;
	static int number_C = 0;
	static int number_D = 0;
	if (m_bShowImageWidhow == false)
	{
		m_bShowInfoWindow = false;
		m_bShowImageWidhow = true;
		GetDlgItem(IDC_STATIC_IMAGE_SHOW)->ShowWindow(m_bShowInfoWindow);
		GetDlgItem(IDC_STATIC_IMAGE_SHOW1)->ShowWindow(m_bShowImageWidhow);
		GetDlgItem(IDC_STATIC_IMAGE_SHOW2)->ShowWindow(m_bShowImageWidhow);
		GetDlgItem(IDC_STATIC_IMAGE_SHOW3)->ShowWindow(m_bShowImageWidhow);
		GetDlgItem(IDC_STATIC_IMAGE_SHOW4)->ShowWindow(m_bShowImageWidhow);
		RefreshWnd(m_pwnd);   //clear the print words from the show window;
	}

	struct information
	{
		std::uint32_t height;
		std::uint32_t width;
		std::uint32_t channels;
		std::uint32_t data_legnth;
		std::uint32_t camera_index;
		std::uint32_t current_time;
		char* m_picture_data;
	}*Info;


	int nCtrlSignalling = (int)wParam;
	Info = (information*)lParam;
	//float m_center_x[DATA_SHOW_LENGHT];
	//float m_center_y[DATA_SHOW_LENGHT];

	if (nCtrlSignalling == 20)
	{
		double s_area = 0.0f;
		int rect_total = 0;
		std::string strLog;
		int x; int y;
		std::vector<float> point_x;
		std::vector<float> point_y;
		float center_x = 0.0f;
		float center_y = 0.0f;
		float max_x = 0.0f;
		float min_x = 0.0f;
		float max_y = 0.0f;
		float min_y = 0.0f;
		int m_width = Info->width;
		int m_height = Info->height;
		int m_channels = Info->channels;
		int data_length = Info->data_legnth;
		int image_length = m_width*m_height*m_channels;
		int json_length = data_length - m_width*m_height*m_channels;
		char* strJson = new char[json_length];
		memcpy(strJson, Info->m_picture_data + image_length, json_length);

		cv::Mat show_img(m_height, m_width, CV_8UC3, Info->m_picture_data);

		std::vector<BaseDetectResult> results;
		all_from_json(strJson, results);			// Transform the detect ends into polygon structure
		if (m_bSaveImage)
		{
			if (Info->camera_index == m_nSaveTrainImageCamera)
			{
				std::string m_frame_camera = "";
				switch (m_nSaveTrainImageCamera)
				{
				case 1:m_frame_camera = "CameraA/"; m_imageACounter++; m_nImageCounter = m_imageACounter; break;
				case 2:m_frame_camera = "CameraB/"; m_imageBCounter++; m_nImageCounter = m_imageBCounter; break;
				case 3:m_frame_camera = "CameraC/"; m_imageCCounter++; m_nImageCounter = m_imageCCounter; break;
				case 4:m_frame_camera = "CameraD/"; m_imageDCounter++; m_nImageCounter = m_imageDCounter; break;
				default: break;

				}
				std::string file_path = CDlgGetTrainImage::m_path + "/train_image/";
				_mkdir(file_path.c_str());
				std::string new_path = file_path + m_frame_camera;
				_mkdir(new_path.c_str());
				std::string tarin_img_path = new_path + std::to_string(m_nImageCounter) + ".bmp";
				cv::imwrite(tarin_img_path, show_img);
				//m_nImageCounter++;
			}
		}
	
		if (m_bSaveDetectResultLabel)
		{
			if (Info->camera_index == m_nSaveDetectResultLabelCamera)
			{
				std::string m_label_camera = "";
				switch (m_nSaveDetectResultLabelCamera)
				{
				case 1:m_label_camera = "/CameraA/"; m_labelACounter++; m_nImageCounter = m_labelACounter; break;
				case 2:m_label_camera = "/CameraB/"; m_labelBCounter++; m_nImageCounter = m_labelBCounter; break;
				case 3:m_label_camera = "/CameraC/"; m_labelCCounter++; m_nImageCounter = m_labelCCounter; break;
				case 4:m_label_camera = "/CameraD/"; m_labelDCounter++; m_nImageCounter = m_labelDCounter; break;
				default: break;

				}
				//m_nImageCounter++;
				std::string m_path_label_n = m_strSaveDetectResultPath + m_label_camera;
				_mkdir(m_path_label_n.c_str());
				std::string m_path_label = m_strSaveDetectResultPath + m_label_camera + "m_images/";
				_mkdir(m_path_label.c_str());
				m_path_label = m_strSaveDetectResultPath + m_label_camera+"/m_images/label/";
				std::string m_path_source = m_strSaveDetectResultPath + m_label_camera+ "/m_images/source/";
				_mkdir(m_path_label.c_str());
				_mkdir(m_path_source.c_str());
				Aqlabel m_aqlabel;
				std::string img_path = m_path_source + std::to_string(m_nImageCounter) + ".png";
				std::string  label_path = m_path_label + std::to_string(m_nImageCounter) + ".aqlabel";

				cv::imwrite(img_path, show_img);

				aq::aidi::Label label;
				//label.ParseFromArray(result.data(), result.size());

				get_json_information(strJson, m_aqlabel, m_path_label, m_nImageCounter);
				//SaveLabel(label, label_path);
			}
			
		}
		//if (Info->camera_index == 1)
		//{
		////	DrawMat(show_img, IDC_STATIC_IMAGE_SHOW1);
		//	Sleep(20);
		//	for (int i = 0; i < results.size(); i++)
		//	{
		//		double defects_size = results[i].defects.size();
		//		CGraphshow::CameraA_yBuff[number_A++] = defects_size;
		//		if (number_A == DATA_SHOW_LENGHT-1)
		//		{
		//			CGraphshow::start_graph_A = true;
		//			number_A = 0;
		//			//memcpy(CGraphshow::CameraA_yBuff, 0, DATA_SHOW_LENGHT);
		//		}
		//	}
		//}
		//else if (Info->camera_index == 2)
		//{
		////	DrawMat(show_img, IDC_STATIC_IMAGE_SHOW2);
		//	Sleep(20);
		//	for (int i = 0; i < results.size(); i++)
		//	{
		//		double defects_size = results[i].defects.size();
		//		CGraphshow::CameraB_yBuff[number_B++] = defects_size;
		//		if (number_B == DATA_SHOW_LENGHT)
		//		{
		//			CGraphshow::start_graph_B = true;
		//			number_B = 0;
		//			//memcpy(CGraphshow::CameraB_yBuff, 0, DATA_SHOW_LENGHT);
		//		}
		//	}
		//}

		//else if (Info->camera_index == 3)
		//{
		////	DrawMat(show_img, IDC_STATIC_IMAGE_SHOW3);
		//	Sleep(20);
		//	for (int i = 0; i < results.size(); i++)
		//	{
		//		double defects_size = results[i].defects.size();
		//		CGraphshow::CameraC_yBuff[number_C++] = defects_size;
		//		if (number_C == DATA_SHOW_LENGHT)
		//		{
		//			CGraphshow::start_graph_C = true;
		//			number_C = 0;
		//			//memcpy(CGraphshow::CameraC_yBuff, 0, DATA_SHOW_LENGHT);
		//		}
		//	}
		//}

		//else if (Info->camera_index == 4)
		//{
		////	DrawMat(show_img, IDC_STATIC_IMAGE_SHOW4);
		//	Sleep(20);
		//	for (int i = 0; i < results.size(); i++)
		//	{
		//		double defects_size = results[i].defects.size();
		//		CGraphshow::CameraD_yBuff[number_D++] = defects_size;
		//		if (number_D == DATA_SHOW_LENGHT)
		//		{
		//			CGraphshow::start_graph_D = true;
		//			number_D = 0;
		//			//memcpy(CGraphshow::CameraD_yBuff, 0, DATA_SHOW_LENGHT);
		//		}
		//	}
		//}

		//if (CGraphshow::start_graph_A == true && CGraphshow::start_graph_B == true && CGraphshow::start_graph_C == true && CGraphshow::start_graph_D == true)
		//{
		//	HWND graph_hWnd = pWnd1->m_hWnd;
		//	::PostMessage(graph_hWnd, ID_STARTGRAPH, 0, NULL);
		//	CGraphshow::start_graph_A = false;
		//	CGraphshow::start_graph_B = false;
		//	CGraphshow::start_graph_C = false;
		//	CGraphshow::start_graph_D = false;
		//}
		for (int i = 0; i < results.size(); i++)
		{
			int defects_size = results[i].defects.size();
			for (int j = 0; j < results[i].defects.size(); j++)
			{
				int n_size = results[i].defects[j].contours.size();
				if (results[i].defects[j].contours.size() >= 3)
				{
					s_area = cv::contourArea(results[i].defects[j].contours);
					cv::RotatedRect s_rotate_rect = cv::minAreaRect(results[i].defects[j].contours);		//最小外接矩形
					cv::Rect s_bounding_rect = s_rotate_rect.boundingRect();		//最小包围盒(旋转外接矩形)
					cv::Point2f s_center = s_rotate_rect.center;					//检测目标质心
					switch (Info->camera_index)
					{
					case 1:      //CameraA defect parameters setting
					{
						rect_total++;
						if (s_area >= parameters_setting_trans.cameraA_point_area_min && s_area <= parameters_setting_trans.cameraA_point_area_max)
						{
							static int num = 0;
							for (int ii = 0; ii < results[i].defects[j].contours.size(); ii++)
							{
								cv::Point c_point = results[i].defects[j].contours[ii];
								x = c_point.x;
								point_x.push_back(x);
								y = c_point.y;
								point_y.push_back(y);
								if (communicator_->m_bWriteLog)
								{
									strLog = std::string("cameraA:  ") + std::string("point_x=") + std::to_string(x) + std::string(",") + std::string("point_y=") + std::to_string(y) + ",push valve.";
									m_Log.Add(strLog.c_str());
								}
							}
							if (communicator_->m_bWriteLog)
							{
								strLog = std::string("cameraA:  ") + std::string("current time=") + std::to_string(Info->current_time);
								m_Log.Add(strLog.c_str());
							}
							//center_x = accumulate(point_x.begin(), point_x.end(), 0.0f);
							//center_y = accumulate(point_y.begin(), point_y.end(), 0.0f);
							//Information.m_center_x = center_x;
							//Information.m_center_y = center_y;
							//Information.m_area = s_area;
							//graph_hWnd = pWnd1->m_hWnd;
							//if (CGraphshow::showgraph == true){
							//	::PostMessage(graph_hWnd, ID_SHOWPICTURE, 1, (LPARAM)&Information);
							//	//Thread2 = AfxBeginThread((AFX_THREADPROC)MyThread2, this);//启动线程
							//	CGraphshow::showgraph = false;
							//}
							////if (num < DATA_SHOW_LENGHT)
							////{
							////	m_center_x[num] = center_x;
							////	m_center_y[num] = center_y;
							////	num++;
							////}
							////else if (num == DATA_SHOW_LENGHT)
							////{
							////	memcpy(Information.m_center_x, m_center_x, DATA_SHOW_LENGHT);
							////	memcpy(Information.m_center_y, m_center_y, DATA_SHOW_LENGHT);
							////	CWnd* pWnd1 = FindWindow(NULL, _T("数据统计"));
							////	HWND hWnd = pWnd1->m_hWnd;
							////	::PostMessage(hWnd, ID_SHOWPICTURE, 1, (LPARAM)(void *)&Information);
							////	memset(Information.m_center_x, 0, DATA_SHOW_LENGHT);
							////	memset(Information.m_center_y, 0, DATA_SHOW_LENGHT);
							////	memset(m_center_y, 0, DATA_SHOW_LENGHT);
							////	memset(m_center_y, 0, DATA_SHOW_LENGHT);
							////	num = 0;
							////}
							////else
							////{
							////	num = 0;
							////}	
							max_x = *max_element(point_x.begin(), point_x.end());
							min_x = *min_element(point_x.begin(), point_x.end());
							max_y = *max_element(point_y.begin(), point_y.end());
							min_y = *min_element(point_y.begin(), point_y.end());
							cv::rectangle(show_img, Point(min_x, min_y), Point(max_x, max_y), Scalar(0, 0, 255), 1, LINE_8, 0);//左上角点的(x,y)，右下角点的(x,y)
							//if (((max_x - min_x) > (m_width / 10)) && ((max_y - min_y) > (m_height / 5)))
							//{
							//	cv::rectangle(show_img, Point(min_x, min_y), Point(max_x, max_y), Scalar(0, 0, 255), 1, LINE_8, 0);//左上角点的(x,y)，右下角点的(x,y)
							//	//Rect rect(min_x, min_y, max_x, max_y);//左上坐标（x,y）和矩形的长(x)宽(y)
							//	//cv::rectangle(show_img, rect, Scalar(0, 0, 255), 1, LINE_8, 0);
							//}
							//else
							//{
							//	float left_bottom_x = min_x;
							//	float right_top_x = max_x;
							//	float left_bottom_y = min_y;
							//	float right_top_y = max_y;
							//	if ((max_x - min_x) < (m_width / 10))
							//	{
							//		left_bottom_x = min_x - ((m_width / 10) - (max_x - min_x)) / 2;
							//		right_top_x = max_x + ((m_width / 10) - (max_x - min_x)) / 2;
							//	}
							//	if ((max_y - min_y) > (m_height / 5))
							//	{
							//		left_bottom_y = min_y - ((m_height / 5) - (max_y - min_y)) / 2;
							//		right_top_y = max_y + ((m_height / 5) - (max_y - min_y)) / 2;
							//	}
							//	cv::rectangle(show_img, Point(left_bottom_x, left_bottom_y), Point(right_top_y, right_top_y), Scalar(0, 0, 255), 1, LINE_8, 0);//左上角点的(x,y)，右下角点的(x,y)
							//}
							if (communicator_->m_bWriteLog)
							{
								strLog = std::string("cameraA:  ") + std::string("max_x=") + std::to_string(max_x) + std::string(",") + std::string("min_x=") + std::to_string(min_x) + std::string(",") + std::string("center_x=") + std::to_string(center_x) + std::string(",") + \
									std::string("max_y=") + std::to_string(max_y) + std::string(",") + std::string("min_y=") + std::to_string(min_y) + std::string(",") + std::string("center_y=") + std::to_string(center_y) + ",push valve.";
								m_Log.Add(strLog.c_str());
								strLog = std::string("cameraA:  ") + std::string("s_area=") + std::to_string(s_area) + ",push valve.";
								m_Log.Add(strLog.c_str());
							}
							point_x.clear();
							point_y.clear();

						}
						else
						{
							if (communicator_->m_bWriteLog)
							{
								strLog = std::string("cameraA:  ") + std::string("s_area=") + std::to_string(s_area) + std::string(",cameraA_point_area_min=") + std::to_string(parameters_setting_trans.cameraA_point_area_min) + std::string(",the object is ignore.");
								m_Log.Add(strLog.c_str());
							}
						}

					}
						break;
					case 2:		 //CameraB defect parameters setting
					{
						rect_total++;
						if (s_area >= parameters_setting_trans.cameraB_point_area_min && s_area <= parameters_setting_trans.cameraB_point_area_max)
						{
							for (int ii = 0; ii < results[i].defects[j].contours.size(); ii++)
							{
								cv::Point c_point = results[i].defects[j].contours[ii];
								x = c_point.x;
								point_x.push_back(x);
								y = c_point.y;
								point_y.push_back(y);
								if (communicator_->m_bWriteLog)
								{
									strLog = std::string("cameraB:  ") + std::string("point_x=") + std::to_string(x) + std::string(",") + std::string("point_y=") + std::to_string(y) + ",push valve.";
									m_Log.Add(strLog.c_str());
								}
							}
							if (communicator_->m_bWriteLog)
							{
								strLog = std::string("cameraB:  ") + std::string("current time=") + std::to_string(Info->current_time);
								m_Log.Add(strLog.c_str());
							}
							//center_x = accumulate(point_x.begin(), point_x.end(), 0.0f);
							//center_y = accumulate(point_y.begin(), point_y.end(), 0.0f);
							//Information.m_center_x = center_x;
							//Information.m_center_y = center_y;
							//Information.m_area = s_area;
							//graph_hWnd = pWnd1->m_hWnd;
							//if (CGraphshow::showgraph == true){
							//	//::PostMessage(graph_hWnd, ID_SHOWPICTURE, 1, (LPARAM)&Information);
							//	CGraphshow::showgraph = false;
							//}
							max_x = *max_element(point_x.begin(), point_x.end());
							min_x = *min_element(point_x.begin(), point_x.end());
							max_y = *max_element(point_y.begin(), point_y.end());
							min_y = *min_element(point_y.begin(), point_y.end());
							cv::rectangle(show_img, Point(min_x, min_y), Point(max_x, max_y), Scalar(0, 0, 255), 1, LINE_8, 0);//左上角点的(x,y)，右下角点的(x,y)
							if (communicator_->m_bWriteLog)
							{
								strLog = std::string("cameraB:  ") + std::string("max_x=") + std::to_string(max_x) + std::string(",") + std::string("min_x=") + std::to_string(min_x) + std::string(",") + std::string("center_x=") + std::to_string(center_x) + std::string(",") + \
									std::string("max_y=") + std::to_string(max_y) + std::string(",") + std::string("min_y=") + std::to_string(min_y) + std::string(",") + std::string("center_y=") + std::to_string(center_y) + ",push valve.";
								m_Log.Add(strLog.c_str());
								strLog = std::string("cameraB:  ") + std::string("s_area=") + std::to_string(s_area) + ",push valve.";
								m_Log.Add(strLog.c_str());
							}
							point_x.clear();
							point_y.clear();
						}
						else
						{
							if (communicator_->m_bWriteLog)
							{
								strLog = std::string("cameraB:  ") + std::string("s_area=") + std::to_string(s_area) + std::string(",cameraB_point_area_min=") + std::to_string(parameters_setting_trans.cameraA_point_area_min) + std::string(",the object is ignore.");
								m_Log.Add(strLog.c_str());
							}
						}
					}
						break;
					case 3:      //CameraC defect parameters setting
					{
						rect_total++;
						if (s_area >= parameters_setting_trans.cameraC_point_area_min && s_area <= parameters_setting_trans.cameraC_point_area_max)
						{
							for (int ii = 0; ii < results[i].defects[j].contours.size(); ii++)
							{
								cv::Point c_point = results[i].defects[j].contours[ii];
								x = c_point.x;
								point_x.push_back(x);
								y = c_point.y;
								point_y.push_back(y);
								if (communicator_->m_bWriteLog)
								{
									strLog = std::string("cameraC:  ") + std::string("point_x=") + std::to_string(x) + std::string(",") + std::string("point_y=") + std::to_string(y) + ",push valve.";
									m_Log.Add(strLog.c_str());
								}
							}
							if (communicator_->m_bWriteLog)
							{
								strLog = std::string("cameraC:  ") + std::string("current time=") + std::to_string(Info->current_time);
								m_Log.Add(strLog.c_str());
							}
							//center_x = accumulate(point_x.begin(), point_x.end(), 0.0f);
							//center_y = accumulate(point_y.begin(), point_y.end(), 0.0f);
							//Information.m_center_x = center_x;
							//Information.m_center_y = center_y;
							//Information.m_area = s_area;
							//graph_hWnd = pWnd1->m_hWnd;
							//if (CGraphshow::showgraph == true){
							//	//::PostMessage(graph_hWnd, ID_SHOWPICTURE, 1, (LPARAM)&Information);
							//	CGraphshow::showgraph = false;
							//}
							max_x = *max_element(point_x.begin(), point_x.end());
							min_x = *min_element(point_x.begin(), point_x.end());
							max_y = *max_element(point_y.begin(), point_y.end());
							min_y = *min_element(point_y.begin(), point_y.end());
							cv::rectangle(show_img, Point(min_x, min_y), Point(max_x, max_y), Scalar(0, 0, 255), 1, LINE_8, 0);//左上角点的(x,y)，右下角点的(x,y)
							if (communicator_->m_bWriteLog)
							{
								strLog = std::string("cameraC:  ") + std::string("max_x=") + std::to_string(max_x) + std::string(",") + std::string("min_x=") + std::to_string(min_x) + std::string(",") + std::string("center_x=") + std::to_string(center_x) + std::string(",") + \
									std::string("max_y=") + std::to_string(max_y) + std::string(",") + std::string("min_y=") + std::to_string(min_y) + std::string(",") + std::string("center_y=") + std::to_string(center_y) + ",push valve.";
								m_Log.Add(strLog.c_str());
								strLog = std::string("cameraC:  ") + std::string("s_area=") + std::to_string(s_area) + ",push valve.";
								m_Log.Add(strLog.c_str());
							}
							point_x.clear();
							point_y.clear();
						}
						else
						{
							if (communicator_->m_bWriteLog)
							{
								strLog = std::string("cameraC:  ") + std::string("s_area=") + std::to_string(s_area) + std::string(",cameraC_point_area_min=") + std::to_string(parameters_setting_trans.cameraA_point_area_min) + std::string(",the object is ignore.");
								m_Log.Add(strLog.c_str());
							}
						}
					}
						break;
					case 4:      //CameraD defect parameters setting
					{
						rect_total++;
						if (s_area >= parameters_setting_trans.cameraD_point_area_min && s_area <= parameters_setting_trans.cameraD_point_area_max)
						{
							for (int ii = 0; ii < results[i].defects[j].contours.size(); ii++)
							{
								cv::Point c_point = results[i].defects[j].contours[ii];
								x = c_point.x;
								point_x.push_back(x);
								y = c_point.y;
								point_y.push_back(y);
								if (communicator_->m_bWriteLog)
								{
									strLog = std::string("cameraD:  ") + std::string("point_x=") + std::to_string(x) + std::string(",") + std::string("point_y=") + std::to_string(y) + ",push valve.";
									m_Log.Add(strLog.c_str());
								}
							}
							if (communicator_->m_bWriteLog)
							{
								strLog = std::string("cameraD:  ") + std::string("current time=") + std::to_string(Info->current_time);
								m_Log.Add(strLog.c_str());
							}
							//center_x = accumulate(point_x.begin(), point_x.end(), 0.0f);
							//center_y = accumulate(point_y.begin(), point_y.end(), 0.0f);
							//Information.m_center_x = center_x;
							//Information.m_center_y = center_y;
							//Information.m_area = s_area;
							//graph_hWnd = pWnd1->m_hWnd;
							//if (CGraphshow::showgraph == true){
							//	//::PostMessage(graph_hWnd, ID_SHOWPICTURE, 1, (LPARAM)&Information);
							//	CGraphshow::showgraph = false;
							//}
							max_x = *max_element(point_x.begin(), point_x.end());
							min_x = *min_element(point_x.begin(), point_x.end());
							max_y = *max_element(point_y.begin(), point_y.end());
							min_y = *min_element(point_y.begin(), point_y.end());
							cv::rectangle(show_img, Point(min_x, min_y), Point(max_x, max_y), Scalar(0, 0, 255), 1, LINE_8, 0);//左上角点的(x,y)，右下角点的(x,y)
							if (communicator_->m_bWriteLog)
							{
								strLog = std::string("cameraD:  ") + std::string("max_x=") + std::to_string(max_x) + std::string(",") + std::string("min_x=") + std::to_string(min_x) + std::string(",") + std::string("center_x=") + std::to_string(center_x) + std::string(",") + \
									std::string("max_y=") + std::to_string(max_y) + std::string(",") + std::string("min_y=") + std::to_string(min_y) + std::string(",") + std::string("center_y=") + std::to_string(center_y) + ",push valve.";
								m_Log.Add(strLog.c_str());
								strLog = std::string("cameraD:  ") + std::string("s_area=") + std::to_string(s_area) + ",push valve.";
								m_Log.Add(strLog.c_str());
							}
							point_x.clear();
							point_y.clear();
						}
						else
						{
							if (communicator_->m_bWriteLog)
							{
								strLog = std::string("cameraD:  ") + std::string("s_area=") + std::to_string(s_area) + std::string(",cameraD_point_area_min=") + std::to_string(parameters_setting_trans.cameraA_point_area_min) + std::string(",the object is ignore.");
								m_Log.Add(strLog.c_str());
							}
						}

					}
						break;
					default:
						break;
					}
				}
			}
		}
		//m_Log.Add(strJson, json_length);
		//m_Log.Add("\n");
		//m_camera_index = Info->camera_index;
		//m_img = show_img;
		//Thread1 = AfxBeginThread((AFX_THREADPROC)MyThread1, this);//启动线程
		m_MeasureResultDlg->m_dfValue = double(rect_total);
		::PostMessage(m_MeasureResultDlg->GetSafeHwnd(), ID_SHOWMEASURERESULT, Info->camera_index, 0);
		camera_show_image(Info->camera_index, show_img);
		//delete[] Info->m_picture_data;
		delete[] strJson;
	}
	//UpdateData(true);

	return 1;
}


void CFiberDetectControlDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);
	//AfxMessageBox("to herer");
	if (m_bResizeEnable)
		DrawWindows(0);
	
	
	// TODO:  在此处添加消息处理程序代码
}


void CFiberDetectControlDlg::OnBnClickedButtonCamera1()
{
	// TODO:  在此添加控件通知处理程序代码
	DrawWindows(1);
}



void CFiberDetectControlDlg::OnBnClickedButtonCamera2()
{
	// TODO:  在此添加控件通知处理程序代码

	DrawWindows(2);
}


void CFiberDetectControlDlg::OnBnClickedButtonCamera3()
{
	// TODO:  在此添加控件通知处理程序代码
	DrawWindows(3);
}


void CFiberDetectControlDlg::OnBnClickedButtonCamera4()
{
	// TODO:  在此添加控件通知处理程序代码
	DrawWindows(4);
}


void CFiberDetectControlDlg::OnBnClickedButtonRecoverWindow()
{
	// TODO:  在此添加控件通知处理程序代码
	GetDlgItem(IDC_STATIC_IMAGE_SHOW)->ShowWindow(true);
	GetDlgItem(IDC_STATIC_IMAGE_SHOW1)->ShowWindow(true);
	GetDlgItem(IDC_STATIC_IMAGE_SHOW2)->ShowWindow(true);
	GetDlgItem(IDC_STATIC_IMAGE_SHOW3)->ShowWindow(true);
	GetDlgItem(IDC_STATIC_IMAGE_SHOW4)->ShowWindow(true);
	DrawWindows(0);
}


void CFiberDetectControlDlg::OnBnClickedButtonModelSeletc()
{
	// TODO:  在此添加控件通知处理程序代码
	CModeSelectDlg dlg;
	ReadIni();
	m_nModleSelect = parameters_setting_trans.model_select;
	dlg.m_nModelSeletc = m_nModleSelect;
	for (int i = 0; i < MODEL_NUMBER;i++)
	{
		dlg.model_type[i] = model_type[i];
		dlg.model_type_text[i] = model_type_text[i];
	}
	if (dlg.DoModal() != IDOK)
		return;

	for (int i = 0; i < MODEL_NUMBER; i++)
	{
		model_type[i] = dlg.model_type[i];
		model_type_text[i] = dlg.model_type_text[i];
	}

	parameters_setting_trans.model_select = dlg.m_nModelSeletc;
	m_nModleSelect = parameters_setting_trans.model_select;
	char IniPath[255];
	memset(IniPath, 0, 255);
	//camera_ip_info ci_info;
	CString strPath = GetModulePath();
	strcpy_s(IniPath, 255, strPath.GetBuffer());
	strcat_s(IniPath, "/param.ini");
	CString IniRead;
	CString IniWrite;

	IniWrite.Format("%d", parameters_setting_trans.model_select);
	WritePrivateProfileString("System", "model_select", IniWrite, IniPath);       //Write back the model selected serial number to ini file 

	//**********************Write back the mode explain text into ini file**********************
	IniWrite.Format("%s", model_type_text[0].c_str());
	WritePrivateProfileString("System", "model1_text", IniWrite, IniPath);

	IniWrite.Format("%s", model_type_text[1].c_str());
	WritePrivateProfileString("System", "model2_text", IniWrite, IniPath);

	IniWrite.Format("%s", model_type_text[2].c_str());
	WritePrivateProfileString("System", "model3_text", IniWrite, IniPath);

	IniWrite.Format("%s", model_type_text[3].c_str());
	WritePrivateProfileString("System", "model4_text", IniWrite, IniPath);

	IniWrite.Format("%s", model_type_text[4].c_str());
	WritePrivateProfileString("System", "model5_text", IniWrite, IniPath);

	IniWrite.Format("%s", model_type_text[5].c_str());
	WritePrivateProfileString("System", "model6_text", IniWrite, IniPath);	
	//*************************End Write back the mode explain text into ini file*****************

	//*****************Send the model select information to box***********************************
	Vsee::VCameraMessageTrans *frame = new Vsee::VCameraMessageTrans;
	memcpy(&frame->parameters_setting_trans, &parameters_setting_trans, sizeof(parameters_setting_trans));
	frame->signalling = 6;
	::PostMessage(AfxGetMainWnd()->m_hWnd, ID_SHOWCTRLMESSAGE, frame->signalling, (LPARAM)frame);
	//****************End Send the model select information to box********************************
}


void CFiberDetectControlDlg::OnBnClickedUploadTrainImage()
{
	// TODO:  在此添加控件通知处理程序代码

}


void CFiberDetectControlDlg::OnBnClickedButidcUploadTrainImageton()
{
	// TODO:  在此添加控件通知处理程序代码
	CDlgGetTrainImage dlg;
	if (dlg.DoModal() != IDOK)
		//return;
		;
	m_bSaveImage = false;
	aq::Communicator::box_ctrl_msg.signalling = 8;					//To stop grab image and save image.
	aq::Communicator::box_ctrl_msg.magnetic_valve_num = 0;
	Vsee::VCameraMessageTrans *frame = new Vsee::VCameraMessageTrans;
	frame->signalling = aq::Communicator::box_ctrl_msg.signalling;
	frame->magnetic_valve_num = aq::Communicator::box_ctrl_msg.magnetic_valve_num;
	memcpy(frame, &aq::Communicator::box_ctrl_msg, sizeof(Vsee::VCameraMessageTrans));
	::PostMessage(AfxGetMainWnd()->m_hWnd, ID_SHOWCTRLMESSAGE, aq::Communicator::box_ctrl_msg.signalling, (LPARAM)frame);
	return;
}


void CFiberDetectControlDlg::ShowMeasureResult(void* pParam)
{
	CFiberDetectControlDlg* pThis = (CFiberDetectControlDlg *)pParam;
	CMeasureResultDlg wwwdlg;
	if (m_MeasureResultDlg == NULL)
	{
		m_MeasureResultDlg = new CMeasureResultDlg;
		m_MeasureResultDlg->DoModal();
	}
	//m_MeasureResultDlg->Create(IDD_MEASURE_RESULT); //创建一个非模态对话框
	//m_MeasureResultDlg->ShowWindow(SW_SHOWNORMAL); //显示非模态对话框
	_endthread();
}

void CFiberDetectControlDlg::ShowCameraCtrl()
{
	CCameraSettingDlg dlg;
	if (dlg.DoModal() != IDOK)
		return;

	int camera_total;
	char IniRead[255];
	memset(IniRead, 0, 255);
	char IniPath[255];
	memset(IniPath, 0, 255);
	//camera_ip_info ci_info;
	CString strPath = GetModulePath();
	strcpy_s(IniPath, 255, strPath.GetBuffer());
	strcat_s(IniPath, "/param.ini");
	GetPrivateProfileStringA("System", "box1_ip", "", IniRead, 24, IniPath);
	//m_dlgNetworkParameters.GetDlgItem(IDC_EDIT_IP_ADRESS)->SetWindowTextA(IniRead);
	GetPrivateProfileStringA("System", "box1_port", "", IniRead, 24, IniPath);
	//m_dlgNetworkParameters.GetDlgItem(IDC_EDIT_IP_PORT_NUMBER)->SetWindowTextA(IniRead);

	parameters_setting_trans.signalling = 1;     //Use siggnalling = 1 to indicate send the network setting command;


	GetPrivateProfileStringA("System", "box1_ip", "", IniRead, 24, IniPath);
	strcpy(parameters_setting_trans.box1_ip, IniRead);
	GetPrivateProfileStringA("System", "box1_port", "", IniRead, 24, IniPath);
	parameters_setting_trans.box1_port = atoi(IniRead);

	GetPrivateProfileStringA("System", "box2_ip", "", IniRead, 24, IniPath);
	strcpy(parameters_setting_trans.box2_ip, IniRead);
	GetPrivateProfileStringA("System", "box2_port", "", IniRead, 24, IniPath);
	parameters_setting_trans.box2_port = atoi(IniRead);

	GetPrivateProfileStringA("System", "electric_board_ip", "", IniRead, 24, IniPath);
	strcpy(parameters_setting_trans.electric_board_ip, IniRead);
	GetPrivateProfileStringA("System", "electric_board_port", "", IniRead, 24, IniPath);
	parameters_setting_trans.electric_board_port = atoi(IniRead);

	GetPrivateProfileStringA("System", "local_ip", "", IniRead, 24, IniPath);
	strcpy(parameters_setting_trans.local_ip, IniRead);
	GetPrivateProfileStringA("System", "local_port", "", IniRead, 24, IniPath);
	parameters_setting_trans.local_port = atoi(IniRead);

	GetPrivateProfileStringA("CameraSetting", "cameraA_ip", "", IniRead, 24, IniPath);
	strcpy(parameters_setting_trans.cameraA_ip, IniRead);
	GetPrivateProfileStringA("CameraSetting", "cameraA_port", "", IniRead, 24, IniPath);
	parameters_setting_trans.cameraA_port = atoi(IniRead);
	GetPrivateProfileStringA("CameraSetting", "cameraA_scan_speed", "", IniRead, 24, IniPath);
	parameters_setting_trans.cameraA_scan_speed = atoi(IniRead);
	GetPrivateProfileStringA("CameraSetting", "cameraA_tirgging_mode", "", IniRead, 24, IniPath);
	parameters_setting_trans.cameraA_tirgging_mode = atoi(IniRead);
	GetPrivateProfileStringA("CameraSetting", "cameraA_exposure_time", "", IniRead, 24, IniPath);
	parameters_setting_trans.cameraA_exposure_time = atoi(IniRead);
	GetPrivateProfileStringA("CameraSetting", "cameraA_gain", "", IniRead, 24, IniPath);
	parameters_setting_trans.cameraA_gain = atoi(IniRead);
	GetPrivateProfileStringA("CameraSetting", "cameraA_width", "", IniRead, 24, IniPath);
	parameters_setting_trans.cameraA_width = atoi(IniRead);
	GetPrivateProfileStringA("CameraSetting", "cameraA_height", "", IniRead, 24, IniPath);
	parameters_setting_trans.cameraA_height = atoi(IniRead);
	GetPrivateProfileStringA("CameraSetting", "cameraA_mac", "", IniRead, 24, IniPath);
	strcpy(parameters_setting_trans.cameraA_mac, IniRead);

	GetPrivateProfileStringA("CameraSetting", "cameraB_ip", "", IniRead, 24, IniPath);
	strcpy(parameters_setting_trans.cameraB_ip, IniRead);
	GetPrivateProfileStringA("CameraSetting", "cameraB_port", "", IniRead, 24, IniPath);
	parameters_setting_trans.cameraB_port = atoi(IniRead);
	GetPrivateProfileStringA("CameraSetting", "cameraB_scan_speed", "", IniRead, 24, IniPath);
	parameters_setting_trans.cameraB_scan_speed = atoi(IniRead);
	GetPrivateProfileStringA("CameraSetting", "cameraB_tirgging_mode", "", IniRead, 24, IniPath);
	parameters_setting_trans.cameraB_tirgging_mode = atoi(IniRead);
	GetPrivateProfileStringA("CameraSetting", "cameraB_exposure_time", "", IniRead, 24, IniPath);
	parameters_setting_trans.cameraB_exposure_time = atoi(IniRead);
	GetPrivateProfileStringA("CameraSetting", "cameraB_gain", "", IniRead, 24, IniPath);
	parameters_setting_trans.cameraB_gain = atoi(IniRead);
	GetPrivateProfileStringA("CameraSetting", "cameraB_width", "", IniRead, 24, IniPath);
	parameters_setting_trans.cameraB_width = atoi(IniRead);
	GetPrivateProfileStringA("CameraSetting", "cameraB_height", "", IniRead, 24, IniPath);
	parameters_setting_trans.cameraB_height = atoi(IniRead);
	GetPrivateProfileStringA("CameraSetting", "cameraB_mac", "", IniRead, 24, IniPath);
	strcpy(parameters_setting_trans.cameraB_mac, IniRead);

	GetPrivateProfileStringA("CameraSetting", "cameraC_ip", "", IniRead, 24, IniPath);
	strcpy(parameters_setting_trans.cameraC_ip, IniRead);
	GetPrivateProfileStringA("CameraSetting", "cameraC_port", "", IniRead, 24, IniPath);
	parameters_setting_trans.cameraC_port = atoi(IniRead);
	GetPrivateProfileStringA("CameraSetting", "cameraC_scan_speed", "", IniRead, 24, IniPath);
	parameters_setting_trans.cameraC_scan_speed = atoi(IniRead);
	GetPrivateProfileStringA("CameraSetting", "cameraC_tirgging_mode", "", IniRead, 24, IniPath);
	parameters_setting_trans.cameraC_tirgging_mode = atoi(IniRead);
	GetPrivateProfileStringA("CameraSetting", "cameraC_exposure_time", "", IniRead, 24, IniPath);
	parameters_setting_trans.cameraC_exposure_time = atoi(IniRead);
	GetPrivateProfileStringA("CameraSetting", "cameraC_gain", "", IniRead, 24, IniPath);
	parameters_setting_trans.cameraC_gain = atoi(IniRead);
	GetPrivateProfileStringA("CameraSetting", "cameraC_width", "", IniRead, 24, IniPath);
	parameters_setting_trans.cameraC_width = atoi(IniRead);
	GetPrivateProfileStringA("CameraSetting", "cameraC_height", "", IniRead, 24, IniPath);
	parameters_setting_trans.cameraC_height = atoi(IniRead);
	GetPrivateProfileStringA("CameraSetting", "cameraC_mac", "", IniRead, 24, IniPath);
	strcpy(parameters_setting_trans.cameraC_mac, IniRead);

	GetPrivateProfileStringA("CameraSetting", "cameraD_ip", "", IniRead, 24, IniPath);
	strcpy(parameters_setting_trans.cameraD_ip, IniRead);
	GetPrivateProfileStringA("CameraSetting", "cameraD_port", "", IniRead, 24, IniPath);
	parameters_setting_trans.cameraD_port = atoi(IniRead);
	GetPrivateProfileStringA("CameraSetting", "cameraD_scan_speed", "", IniRead, 24, IniPath);
	parameters_setting_trans.cameraD_scan_speed = atoi(IniRead);
	GetPrivateProfileStringA("CameraSetting", "cameraD_tirgging_mode", "", IniRead, 24, IniPath);
	parameters_setting_trans.cameraD_tirgging_mode = atoi(IniRead);
	GetPrivateProfileStringA("CameraSetting", "cameraD_exposure_time", "", IniRead, 24, IniPath);
	parameters_setting_trans.cameraD_exposure_time = atoi(IniRead);
	GetPrivateProfileStringA("CameraSetting", "cameraD_gain", "", IniRead, 24, IniPath);
	parameters_setting_trans.cameraD_gain = atoi(IniRead);
	GetPrivateProfileStringA("CameraSetting", "cameraD_width", "", IniRead, 24, IniPath);
	parameters_setting_trans.cameraD_width = atoi(IniRead);
	GetPrivateProfileStringA("CameraSetting", "cameraD_height", "", IniRead, 24, IniPath);
	parameters_setting_trans.cameraD_height = atoi(IniRead);
	GetPrivateProfileStringA("CameraSetting", "cameraD_mac", "", IniRead, 24, IniPath);
	strcpy(parameters_setting_trans.cameraD_mac, IniRead);

	parameters_setting_trans.signalling = 8;    //To set the camera and box device parameters setting;

	Vsee::VCameraMessageTrans *frame = new Vsee::VCameraMessageTrans;
	memcpy(&frame->parameters_setting_trans, &parameters_setting_trans, sizeof(parameters_setting_trans));
	frame->signalling = 4;
	//::PostMessage(AfxGetMainWnd()->m_hWnd, ID_SHOWCTRLMESSAGE, frame->signalling, (LPARAM)frame);
}


void CFiberDetectControlDlg::OnBnClickedShowMeasureResult()
{
	// TODO:  在此添加控件通知处理程序代码
	//_beginthread(ShowMeasureResult, 10240, NULL);
	std::thread t(&CFiberDetectControlDlg::ShowMeasureResult, this, nullptr);
	t.detach();
}




void CFiberDetectControlDlg::OnBnClickedButtonCameraParametersSetting()
{
	// TODO:  在此添加控件通知处理程序代码
	//_beginthread(ShowMeasureResult, 10240, NULL);
	/*std::thread t(&CFiberDetectControlDlg::ShowCameraCtrl, this, nullptr);
	t.detach();*/
	ShowCameraCtrl();
}


void CFiberDetectControlDlg::OnBnClickedButtonStatisticsSetting()
{
	// TODO:  在此添加控件通知处理程序代码
	//_beginthread(ShowMeasureResult, 10240, NULL);
	//std::thread t(&CFiberDetectControlDlg::ShowMeasureResult, this, nullptr);
	////std::thread t(&aq::Communicator::server_get_image_run, this, img_socket_, _ip, _port, _error_str);	//Start receive&send serer function
	//t.detach();
	CBoxSelect dlg;
	if (dlg.DoModal() != IDOK)
	{
		if (dlg.parameters_setting_trans.box_num == 1)
		{
			SetWindowTextA("经纬异纤检测管理系统，盒子1");
		}
		else if (dlg.parameters_setting_trans.box_num == 2)
		{
			SetWindowTextA("经纬异纤检测管理系统，盒子2");
		}
		else
		{

		}
		return;
	}
}


void CFiberDetectControlDlg::OnBnClickedSaveDetectResult()
{
	// TODO:  在此添加控件通知处理程序代码
	CSaveDetectResult dlg;
	
	if (dlg.DoModal() != IDOK)
	{
		//std::string m_strSaveDetectResultPath = dlg.m_strSavePath;
		;
	}
		//return;
		;
	m_bSaveDetectResultLabel = false;
	aq::Communicator::box_ctrl_msg.signalling = 15;					//To stop grab image and save image.
	aq::Communicator::box_ctrl_msg.magnetic_valve_num = 0;
	Vsee::VCameraMessageTrans *frame = new Vsee::VCameraMessageTrans;
	frame->signalling = aq::Communicator::box_ctrl_msg.signalling;
	frame->magnetic_valve_num = aq::Communicator::box_ctrl_msg.magnetic_valve_num;
	memcpy(frame, &aq::Communicator::box_ctrl_msg, sizeof(Vsee::VCameraMessageTrans));
	::PostMessage(AfxGetMainWnd()->m_hWnd, ID_SHOWCTRLMESSAGE, aq::Communicator::box_ctrl_msg.signalling, (LPARAM)frame);	
}

void CFiberDetectControlDlg::run_udp_server(std::string _ip, int _port)
{
	std::thread t(&CFiberDetectControlDlg::udp_server, this, _ip, _port);
	t.detach();
}

void CFiberDetectControlDlg::run_valve_server()
{
	std::thread t(&CFiberDetectControlDlg::OnTimerShowValve, this, 1);
	t.detach();
}

void CFiberDetectControlDlg::udp_server(std::string _ip, int _port)
{

	std::string error_str;
	if (!communicator_->udp_rcv_server(_ip, _port, error_str))	//Start a Server state machine,to server for all the net request from client,
	{													//here the the function "server()" actuall is an endless loop.
		std::cerr << error_str << std::endl;
		return;
	}
}


void CFiberDetectControlDlg::OnBnClickedCheckMagneticValveShow()
{
	// TODO:  在此添加控件通知处理程序代码
	m_bMagneticValveShow = !m_bMagneticValveShow;
	if (m_bMagneticValveShow)
	{
		((CButton*)GetDlgItem(IDC_CHECK_MAGNETIC_VALVE_SHOW))->SetCheck(true);
		communicator_->m_bMagneticValveShow = m_bMagneticValveShow;
		run_valve_server();
		run_udp_server(parameters_setting_trans.electric_board_ip, parameters_setting_trans.electric_board_port);   //to start a server mode;
	}
	else
	{
		((CButton*)GetDlgItem(IDC_CHECK_MAGNETIC_VALVE_SHOW))->SetCheck(false);
		communicator_->m_bMagneticValveShow = m_bMagneticValveShow;
	}
}
