// DetectParametersSetting.cpp : 实现文件
//

#include "stdafx.h"
#include "FiberDetectControl.h"
#include "DetectParametersSetting.h"
#include "afxdialogex.h"
#include  "VCameraMessageTrans.h"



// CDetectParametersSetting 对话框

IMPLEMENT_DYNAMIC(CDetectParametersSetting, CDialogEx)

CDetectParametersSetting::CDetectParametersSetting(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDetectParametersSetting::IDD, pParent)
{

}

CDetectParametersSetting::~CDetectParametersSetting()
{
}

void CDetectParametersSetting::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TAB_PARAMETERS_SETTING, m_cTabControl);
}


BEGIN_MESSAGE_MAP(CDetectParametersSetting, CDialogEx)
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB_PARAMETERS_SETTING, &CDetectParametersSetting::OnTcnSelchangeTabParametersSetting)
	ON_BN_CLICKED(IDOK, &CDetectParametersSetting::OnBnClickedOk)
	ON_BN_CLICKED(IDC_DIALOG_SEND_MESSAGE, &CDetectParametersSetting::OnBnClickedDialogSendMessage)
END_MESSAGE_MAP()


// CDetectParametersSetting 消息处理程序
BOOL CDetectParametersSetting::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	m_cTabControl.InsertItem(1, _T("基本参数"));
	m_dlgBaseDetectParametersSettting.Create(IDD_DIALOG_BASE_DETECT_PARAMETERS_SETTING, &m_cTabControl);

	m_cTabControl.InsertItem(2, _T("调试参数"));
	m_dlgDebugParametersSetting.Create(IDD_DIALOG_DEBUG_PARAMETERS, &m_cTabControl);		//m_dlgValveParametersSettting

	m_cTabControl.InsertItem(3, _T("喷阀参数"));
	m_dlgValveParametersSettting.Create(IDD_DIALOG_VALVA_PARAMETERS, &m_cTabControl);		//

	m_cTabControl.InsertItem(4, _T("彩色相机A"));
	m_dlgFiberDetectParametersSettingA.Create(IDD_DIALOG_FIBER_DETECT_PARAMETERS_SETTING, &m_cTabControl);//

	m_cTabControl.InsertItem(5, _T("彩色相机B"));
	m_dlgFiberDetectParametersSettingB.Create(IDD_DIALOG_FIBER_DETECT_PARAMETERS_SETTING, &m_cTabControl);//

	m_cTabControl.InsertItem(6, _T("紫外相机C"));
	m_dlgFiberDetectParametersSettingC.Create(IDD_DIALOG_FIBER_DETECT_PARAMETERS_SETTING, &m_cTabControl);//

	m_cTabControl.InsertItem(7, _T("紫外相机D"));
	m_dlgFiberDetectParametersSettingD.Create(IDD_DIALOG_FIBER_DETECT_PARAMETERS_SETTING, &m_cTabControl);//

	CRect rect;
	m_cTabControl.GetClientRect(&rect);
	rect.top += 20;
	rect.left += 1;
	rect.right -= 1;

	m_dlgBaseDetectParametersSettting.MoveWindow(rect);
	m_dlgBaseDetectParametersSettting.ShowWindow(true);

	m_dlgDebugParametersSetting.MoveWindow(rect);
	m_dlgDebugParametersSetting.ShowWindow(false);

	m_dlgValveParametersSettting.MoveWindow(rect);
	m_dlgValveParametersSettting.ShowWindow(false);

	m_dlgFiberDetectParametersSettingA.MoveWindow(rect);
	m_dlgFiberDetectParametersSettingA.ShowWindow(false);

	m_dlgFiberDetectParametersSettingB.MoveWindow(rect);
	m_dlgFiberDetectParametersSettingB.ShowWindow(false);

	m_dlgFiberDetectParametersSettingC.MoveWindow(rect);
	m_dlgFiberDetectParametersSettingC.ShowWindow(false);

	m_dlgFiberDetectParametersSettingD.MoveWindow(rect);
	m_dlgFiberDetectParametersSettingD.ShowWindow(false);
	ReadIni();
	

	//Read the 

	return true;
}
void CDetectParametersSetting::ReadIni()
{
	char IniRead[255];
	memset(IniRead, 0, 255);
	char IniPath[255];
	memset(IniPath, 0, 255);
	//camera_ip_info ci_info;
	CString strPath = GetModulePath();
	strcpy_s(IniPath, 255, strPath.GetBuffer());
	strcat_s(IniPath, "/param.ini");


	GetPrivateProfileStringA("System", "model_select", "", IniRead, 100, IniPath);
	int m_nModleSelect = atoi(IniRead);
	parameters_setting_trans.model_select = m_nModleSelect;
	//m_dlgNetworkParameters.GetDlgItem(IDC_EDIT_IP_PORT_NUMBER)->SetWindowTextA(IniRead);

	parameters_setting_trans.signalling = 1;     //Use siggnalling = 1 to indicate send the network setting command;

	GetPrivateProfileStringA("System", "box_num", "", IniRead, 24, IniPath);
	parameters_setting_trans.box_num = atoi(IniRead);

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

	GetPrivateProfileStringA("BaseDetectParameter", "cameraA_begin", "", IniRead, 24, IniPath);
	parameters_setting_trans.cameraA_begin = atoi(IniRead);
	m_dlgBaseDetectParametersSettting.GetDlgItem(IDC_EDIT_CAMERA1_BEGIN_POINT)->SetWindowTextA(IniRead);
	GetPrivateProfileStringA("BaseDetectParameter", "cameraA_end", "", IniRead, 24, IniPath);
	parameters_setting_trans.cameraA_end = atoi(IniRead);
	m_dlgBaseDetectParametersSettting.GetDlgItem(IDC_EDIT_CAMERA1_END_POINT)->SetWindowTextA(IniRead);
	GetPrivateProfileStringA("BaseDetectParameter", "cameraA_align_begin", "", IniRead, 24, IniPath);
	parameters_setting_trans.cameraA_align_begin = atoi(IniRead);
	m_dlgBaseDetectParametersSettting.GetDlgItem(IDC_EDIT_CAMERA1_ALIGNING_BEGIN_POINT)->SetWindowTextA(IniRead);
	GetPrivateProfileStringA("BaseDetectParameter", "cameraA_align_end", "", IniRead, 24, IniPath);
	parameters_setting_trans.cameraA_align_end = atoi(IniRead);
	m_dlgBaseDetectParametersSettting.GetDlgItem(IDC_EDIT_CAMERA1_ALIGNING_END_POINT)->SetWindowTextA(IniRead);
	GetPrivateProfileStringA("BaseDetectParameter", "cameraA_detect_begin", "", IniRead, 24, IniPath);
	parameters_setting_trans.cameraA_detect_begin = atoi(IniRead);
	m_dlgBaseDetectParametersSettting.GetDlgItem(IDC_EDIT_CAMERA1_DETECT_BEGIN_POINT)->SetWindowTextA(IniRead);
	GetPrivateProfileStringA("BaseDetectParameter", "cameraA_detect_end", "", IniRead, 24, IniPath);
	parameters_setting_trans.cameraA_detect_end = atoi(IniRead);
	m_dlgBaseDetectParametersSettting.GetDlgItem(IDC_EDIT_CAMERA1_DETECT_END_POINT)->SetWindowTextA(IniRead);
	GetPrivateProfileStringA("BaseDetectParameter", "cameraA_valve_amount", "", IniRead, 24, IniPath);
	parameters_setting_trans.cameraA_valve_amount = atoi(IniRead);
	m_dlgBaseDetectParametersSettting.GetDlgItem(IDC_EDIT_CAMERA1_VALVE_AMOUNT)->SetWindowTextA(IniRead);

	GetPrivateProfileStringA("BaseDetectParameter", "cameraB_begin", "", IniRead, 24, IniPath);
	parameters_setting_trans.cameraB_begin = atoi(IniRead);
	m_dlgBaseDetectParametersSettting.GetDlgItem(IDC_EDIT_CAMERA2_BEGIN_POINT)->SetWindowTextA(IniRead);
	GetPrivateProfileStringA("BaseDetectParameter", "cameraB_end", "", IniRead, 24, IniPath);
	parameters_setting_trans.cameraB_end = atoi(IniRead);
	m_dlgBaseDetectParametersSettting.GetDlgItem(IDC_EDIT_CAMERA2_END_POINT)->SetWindowTextA(IniRead);
	GetPrivateProfileStringA("BaseDetectParameter", "cameraB_align_begin", "", IniRead, 24, IniPath);
	parameters_setting_trans.cameraB_align_begin = atoi(IniRead);
	m_dlgBaseDetectParametersSettting.GetDlgItem(IDC_EDIT_CAMERA2_ALIGNING_BEGIN_POINT)->SetWindowTextA(IniRead);
	GetPrivateProfileStringA("BaseDetectParameter", "cameraB_align_end", "", IniRead, 24, IniPath);
	parameters_setting_trans.cameraB_align_end = atoi(IniRead);
	m_dlgBaseDetectParametersSettting.GetDlgItem(IDC_EDIT_CAMERA2_ALIGNING_END_POINT)->SetWindowTextA(IniRead);
	GetPrivateProfileStringA("BaseDetectParameter", "cameraB_detect_begin", "", IniRead, 24, IniPath);
	parameters_setting_trans.cameraB_detect_begin = atoi(IniRead);
	m_dlgBaseDetectParametersSettting.GetDlgItem(IDC_EDIT_CAMERA2_DETECT_BEGIN_POINT)->SetWindowTextA(IniRead);
	GetPrivateProfileStringA("BaseDetectParameter", "cameraB_detect_end", "", IniRead, 24, IniPath);
	parameters_setting_trans.cameraB_detect_end = atoi(IniRead);
	m_dlgBaseDetectParametersSettting.GetDlgItem(IDC_EDIT_CAMERA2_DETECT_END_POINT)->SetWindowTextA(IniRead);
	GetPrivateProfileStringA("BaseDetectParameter", "cameraB_valve_amount", "", IniRead, 24, IniPath);
	parameters_setting_trans.cameraB_valve_amount = atoi(IniRead);
	m_dlgBaseDetectParametersSettting.GetDlgItem(IDC_EDIT_CAMERA2_VALVE_AMOUNT)->SetWindowTextA(IniRead);

	GetPrivateProfileStringA("BaseDetectParameter", "cameraC_begin", "", IniRead, 24, IniPath);
	parameters_setting_trans.cameraC_begin = atoi(IniRead);
	m_dlgBaseDetectParametersSettting.GetDlgItem(IDC_EDIT_CAMERA3_BEGIN_POINT)->SetWindowTextA(IniRead);
	GetPrivateProfileStringA("BaseDetectParameter", "cameraC_end", "", IniRead, 24, IniPath);
	parameters_setting_trans.cameraC_end = atoi(IniRead);
	m_dlgBaseDetectParametersSettting.GetDlgItem(IDC_EDIT_CAMERA3_END_POINT)->SetWindowTextA(IniRead);
	GetPrivateProfileStringA("BaseDetectParameter", "cameraC_align_begin", "", IniRead, 24, IniPath);
	parameters_setting_trans.cameraC_align_begin = atoi(IniRead);
	m_dlgBaseDetectParametersSettting.GetDlgItem(IDC_EDIT_CAMERA3_ALIGNING_BEGIN_POINT)->SetWindowTextA(IniRead);
	GetPrivateProfileStringA("BaseDetectParameter", "cameraC_align_end", "", IniRead, 24, IniPath);
	parameters_setting_trans.cameraC_align_end = atoi(IniRead);
	m_dlgBaseDetectParametersSettting.GetDlgItem(IDC_EDIT_CAMERA3_ALIGNING_END_POINT)->SetWindowTextA(IniRead);
	GetPrivateProfileStringA("BaseDetectParameter", "cameraC_detect_begin", "", IniRead, 24, IniPath);
	parameters_setting_trans.cameraC_detect_begin = atoi(IniRead);
	m_dlgBaseDetectParametersSettting.GetDlgItem(IDC_EDIT_CAMERA3_DETECT_BEGIN_POINT)->SetWindowTextA(IniRead);
	GetPrivateProfileStringA("BaseDetectParameter", "cameraC_detect_end", "", IniRead, 24, IniPath);
	parameters_setting_trans.cameraC_detect_end = atoi(IniRead);
	m_dlgBaseDetectParametersSettting.GetDlgItem(IDC_EDIT_CAMERA3_DETECT_END_POINT)->SetWindowTextA(IniRead);
	GetPrivateProfileStringA("BaseDetectParameter", "cameraC_valve_amount", "", IniRead, 24, IniPath);
	parameters_setting_trans.cameraC_valve_amount = atoi(IniRead);
	m_dlgBaseDetectParametersSettting.GetDlgItem(IDC_EDIT_CAMERA3_VALVE_AMOUNT)->SetWindowTextA(IniRead);

	GetPrivateProfileStringA("BaseDetectParameter", "cameraD_begin", "", IniRead, 24, IniPath);
	parameters_setting_trans.cameraD_begin = atoi(IniRead);
	m_dlgBaseDetectParametersSettting.GetDlgItem(IDC_EDIT_CAMERA4_BEGIN_POINT)->SetWindowTextA(IniRead);
	GetPrivateProfileStringA("BaseDetectParameter", "cameraD_end", "", IniRead, 24, IniPath);
	parameters_setting_trans.cameraD_end = atoi(IniRead);
	m_dlgBaseDetectParametersSettting.GetDlgItem(IDC_EDIT_CAMERA4_END_POINT)->SetWindowTextA(IniRead);
	GetPrivateProfileStringA("BaseDetectParameter", "cameraD_align_begin", "", IniRead, 24, IniPath);
	parameters_setting_trans.cameraD_align_begin = atoi(IniRead);
	m_dlgBaseDetectParametersSettting.GetDlgItem(IDC_EDIT_CAMERA4_ALIGNING_BEGIN_POINT)->SetWindowTextA(IniRead);
	GetPrivateProfileStringA("BaseDetectParameter", "cameraD_align_end", "", IniRead, 24, IniPath);
	parameters_setting_trans.cameraD_align_end = atoi(IniRead);
	m_dlgBaseDetectParametersSettting.GetDlgItem(IDC_EDIT_CAMERA4_ALIGNING_END_POINT)->SetWindowTextA(IniRead);
	GetPrivateProfileStringA("BaseDetectParameter", "cameraD_detect_begin", "", IniRead, 24, IniPath);
	parameters_setting_trans.cameraD_detect_begin = atoi(IniRead);
	m_dlgBaseDetectParametersSettting.GetDlgItem(IDC_EDIT_CAMERA4_DETECT_BEGIN_POINT)->SetWindowTextA(IniRead);
	GetPrivateProfileStringA("BaseDetectParameter", "cameraD_detect_end", "", IniRead, 24, IniPath);
	parameters_setting_trans.cameraD_detect_end = atoi(IniRead);
	m_dlgBaseDetectParametersSettting.GetDlgItem(IDC_EDIT_CAMERA4_DETECT_END_POINT)->SetWindowTextA(IniRead);
	GetPrivateProfileStringA("BaseDetectParameter", "cameraD_valve_amount", "", IniRead, 24, IniPath);
	parameters_setting_trans.cameraD_valve_amount = atoi(IniRead);
	m_dlgBaseDetectParametersSettting.GetDlgItem(IDC_EDIT_CAMERA4_VALVE_AMOUNT)->SetWindowTextA(IniRead);

	//Camera A detect parameter setting;
	GetPrivateProfileStringA("ValveParameter", "cameraA_delay", "", IniRead, 24, IniPath);
	parameters_setting_trans.cameraA_delay = atoi(IniRead);
	m_dlgValveParametersSettting.GetDlgItem(IDC_EDIT_VALVE_A_DELAY)->SetWindowTextA(IniRead);

	GetPrivateProfileStringA("ValveParameter", "cameraA_hold", "", IniRead, 24, IniPath);
	parameters_setting_trans.cameraA_hold = atoi(IniRead);
	m_dlgValveParametersSettting.GetDlgItem(IDC_EDIT_VALVE_A_HOLD)->SetWindowTextA(IniRead);

	GetPrivateProfileStringA("ValveParameter", "cameraB_delay", "", IniRead, 24, IniPath);
	parameters_setting_trans.cameraB_delay = atoi(IniRead);
	m_dlgValveParametersSettting.GetDlgItem(IDC_EDIT_VALVE_B_DELAY)->SetWindowTextA(IniRead);

	GetPrivateProfileStringA("ValveParameter", "cameraB_hold", "", IniRead, 24, IniPath);
	parameters_setting_trans.cameraB_hold = atoi(IniRead);
	m_dlgValveParametersSettting.GetDlgItem(IDC_EDIT_VALVE_B_HOLD)->SetWindowTextA(IniRead);

	GetPrivateProfileStringA("ValveParameter", "cameraC_delay", "", IniRead, 24, IniPath);
	parameters_setting_trans.cameraC_delay = atoi(IniRead);
	m_dlgValveParametersSettting.GetDlgItem(IDC_EDIT_VALVE_C_DELAY)->SetWindowTextA(IniRead);

	GetPrivateProfileStringA("ValveParameter", "cameraC_hold", "", IniRead, 24, IniPath);
	parameters_setting_trans.cameraC_hold = atoi(IniRead);
	m_dlgValveParametersSettting.GetDlgItem(IDC_EDIT_VALVE_C_HOLD)->SetWindowTextA(IniRead);

	GetPrivateProfileStringA("ValveParameter", "cameraD_delay", "", IniRead, 24, IniPath);
	parameters_setting_trans.cameraD_delay = atoi(IniRead);
	m_dlgValveParametersSettting.GetDlgItem(IDC_EDIT_VALVE_D_DELAY)->SetWindowTextA(IniRead);

	GetPrivateProfileStringA("ValveParameter", "cameraD_hold", "", IniRead, 24, IniPath);
	parameters_setting_trans.cameraD_hold = atoi(IniRead);
	m_dlgValveParametersSettting.GetDlgItem(IDC_EDIT_VALVE_D_HOLD)->SetWindowTextA(IniRead);

	GetPrivateProfileStringA("ValveParameter", "valve_work_times", "", IniRead, 24, IniPath);
	parameters_setting_trans.valve_work_times = atoi(IniRead);
	m_dlgValveParametersSettting.GetDlgItem(IDC_EDIT_VALVE_WORK_TIMES)->SetWindowTextA(IniRead);

	GetPrivateProfileStringA("ValveParameter", "valve_stop_time", "", IniRead, 24, IniPath);
	parameters_setting_trans.valve_stop_time = atoi(IniRead);
	m_dlgValveParametersSettting.GetDlgItem(IDC_EDIT_VALVE_STOP_TIMES)->SetWindowTextA(IniRead);

	GetPrivateProfileStringA("DetectParameter", "cameraA_point_area_min", "", IniRead, 24, IniPath);
	parameters_setting_trans.cameraA_point_area_min = atoi(IniRead);
	m_dlgFiberDetectParametersSettingA.GetDlgItem(IDC_EDIT_POINT_AREA_MIN)->SetWindowTextA(IniRead);

	GetPrivateProfileStringA("DetectParameter", "cameraA_point_area_max", "", IniRead, 24, IniPath);
	parameters_setting_trans.cameraA_point_area_max = atoi(IniRead);
	m_dlgFiberDetectParametersSettingA.GetDlgItem(IDC_EDIT_POINT_AREA_MAX)->SetWindowTextA(IniRead);

	GetPrivateProfileStringA("DetectParameter", "cameraA_point_length_min", "", IniRead, 24, IniPath);
	parameters_setting_trans.cameraA_point_length_min = atoi(IniRead);
	m_dlgFiberDetectParametersSettingA.GetDlgItem(IDC_EDIT_POINT_LENGTH_MIN)->SetWindowTextA(IniRead);

	GetPrivateProfileStringA("DetectParameter", "cameraA_point_length_max", "", IniRead, 24, IniPath);
	parameters_setting_trans.cameraA_point_length_max = atoi(IniRead);
	m_dlgFiberDetectParametersSettingA.GetDlgItem(IDC_EDIT_POINT_LENGTH_MAX)->SetWindowTextA(IniRead);

	GetPrivateProfileStringA("DetectParameter", "cameraA_point_width_min", "", IniRead, 24, IniPath);
	parameters_setting_trans.cameraA_point_width_min = atoi(IniRead);
	m_dlgFiberDetectParametersSettingA.GetDlgItem(IDC_EDIT_POINT_WIDTH_MIN)->SetWindowTextA(IniRead);

	GetPrivateProfileStringA("DetectParameter", "cameraA_point_width_max", "", IniRead, 24, IniPath);
	parameters_setting_trans.cameraA_point_width_max = atoi(IniRead);
	m_dlgFiberDetectParametersSettingA.GetDlgItem(IDC_EDIT_POINT_WIDTH_MAX)->SetWindowTextA(IniRead);

	GetPrivateProfileStringA("DetectParameter", "cameraA_thread_area_min", "", IniRead, 24, IniPath);
	parameters_setting_trans.cameraA_thread_area_min = atoi(IniRead);
	m_dlgFiberDetectParametersSettingA.GetDlgItem(IDC_EDIT_THREAD_AREA_MIN)->SetWindowTextA(IniRead);

	GetPrivateProfileStringA("DetectParameter", "cameraA_thread_area_max", "", IniRead, 24, IniPath);
	parameters_setting_trans.cameraA_thread_area_max = atoi(IniRead);
	m_dlgFiberDetectParametersSettingA.GetDlgItem(IDC_EDIT_THREAD_AREA_MAX)->SetWindowTextA(IniRead);

	GetPrivateProfileStringA("DetectParameter", "cameraA_thread_length_min", "", IniRead, 24, IniPath);
	parameters_setting_trans.cameraA_thread_length_min = atoi(IniRead);
	m_dlgFiberDetectParametersSettingA.GetDlgItem(IDC_EDIT_THREAD_LENGTH_MIN)->SetWindowTextA(IniRead);

	GetPrivateProfileStringA("DetectParameter", "cameraA_thread_length_max", "", IniRead, 24, IniPath);
	parameters_setting_trans.cameraA_thread_length_max = atoi(IniRead);
	m_dlgFiberDetectParametersSettingA.GetDlgItem(IDC_EDIT_THREAD_LENGTH_MAX)->SetWindowTextA(IniRead);

	GetPrivateProfileStringA("DetectParameter", "cameraA_thread_width_min", "", IniRead, 24, IniPath);
	parameters_setting_trans.cameraA_thread_width_min = atoi(IniRead);
	m_dlgFiberDetectParametersSettingA.GetDlgItem(IDC_EDIT_THREAD_WIDTH_MIN)->SetWindowTextA(IniRead);

	GetPrivateProfileStringA("DetectParameter", "cameraA_thread_width_max", "", IniRead, 24, IniPath);
	parameters_setting_trans.cameraA_thread_width_max = atoi(IniRead);
	m_dlgFiberDetectParametersSettingA.GetDlgItem(IDC_EDIT_THREAD_WIDTH_MAX)->SetWindowTextA(IniRead);


	//Camera B detect parameter setting;
	GetPrivateProfileStringA("DetectParameter", "cameraB_point_area_min", "", IniRead, 24, IniPath);
	parameters_setting_trans.cameraB_point_area_min = atoi(IniRead);
	m_dlgFiberDetectParametersSettingB.GetDlgItem(IDC_EDIT_POINT_AREA_MIN)->SetWindowTextA(IniRead);

	GetPrivateProfileStringA("DetectParameter", "cameraB_point_area_max", "", IniRead, 24, IniPath);
	parameters_setting_trans.cameraB_point_area_max = atoi(IniRead);
	m_dlgFiberDetectParametersSettingB.GetDlgItem(IDC_EDIT_POINT_AREA_MAX)->SetWindowTextA(IniRead);

	GetPrivateProfileStringA("DetectParameter", "cameraB_point_length_min", "", IniRead, 24, IniPath);
	parameters_setting_trans.cameraB_point_length_min = atoi(IniRead);
	m_dlgFiberDetectParametersSettingB.GetDlgItem(IDC_EDIT_POINT_LENGTH_MIN)->SetWindowTextA(IniRead);

	GetPrivateProfileStringA("DetectParameter", "cameraB_point_length_max", "", IniRead, 24, IniPath);
	parameters_setting_trans.cameraB_point_length_max = atoi(IniRead);
	m_dlgFiberDetectParametersSettingB.GetDlgItem(IDC_EDIT_POINT_LENGTH_MAX)->SetWindowTextA(IniRead);

	GetPrivateProfileStringA("DetectParameter", "cameraB_point_width_min", "", IniRead, 24, IniPath);
	parameters_setting_trans.cameraB_point_width_min = atoi(IniRead);
	m_dlgFiberDetectParametersSettingB.GetDlgItem(IDC_EDIT_POINT_WIDTH_MIN)->SetWindowTextA(IniRead);

	GetPrivateProfileStringA("DetectParameter", "cameraB_point_width_max", "", IniRead, 24, IniPath);
	parameters_setting_trans.cameraB_point_width_max = atoi(IniRead);
	m_dlgFiberDetectParametersSettingB.GetDlgItem(IDC_EDIT_POINT_WIDTH_MAX)->SetWindowTextA(IniRead);

	GetPrivateProfileStringA("DetectParameter", "cameraB_thread_area_min", "", IniRead, 24, IniPath);
	parameters_setting_trans.cameraB_thread_area_min = atoi(IniRead);
	m_dlgFiberDetectParametersSettingB.GetDlgItem(IDC_EDIT_THREAD_AREA_MIN)->SetWindowTextA(IniRead);

	GetPrivateProfileStringA("DetectParameter", "cameraB_thread_area_max", "", IniRead, 24, IniPath);
	parameters_setting_trans.cameraB_thread_area_max = atoi(IniRead);
	m_dlgFiberDetectParametersSettingB.GetDlgItem(IDC_EDIT_THREAD_AREA_MAX)->SetWindowTextA(IniRead);

	GetPrivateProfileStringA("DetectParameter", "cameraB_thread_length_min", "", IniRead, 24, IniPath);
	parameters_setting_trans.cameraB_thread_length_min = atoi(IniRead);
	m_dlgFiberDetectParametersSettingB.GetDlgItem(IDC_EDIT_THREAD_LENGTH_MIN)->SetWindowTextA(IniRead);

	GetPrivateProfileStringA("DetectParameter", "cameraB_thread_length_max", "", IniRead, 24, IniPath);
	parameters_setting_trans.cameraB_thread_length_max = atoi(IniRead);
	m_dlgFiberDetectParametersSettingB.GetDlgItem(IDC_EDIT_THREAD_LENGTH_MAX)->SetWindowTextA(IniRead);

	GetPrivateProfileStringA("DetectParameter", "cameraB_thread_width_min", "", IniRead, 24, IniPath);
	parameters_setting_trans.cameraB_thread_width_min = atoi(IniRead);
	m_dlgFiberDetectParametersSettingB.GetDlgItem(IDC_EDIT_THREAD_WIDTH_MIN)->SetWindowTextA(IniRead);

	GetPrivateProfileStringA("DetectParameter", "cameraB_thread_width_max", "", IniRead, 24, IniPath);
	parameters_setting_trans.cameraB_thread_width_max = atoi(IniRead);
	m_dlgFiberDetectParametersSettingB.GetDlgItem(IDC_EDIT_THREAD_WIDTH_MAX)->SetWindowTextA(IniRead);


	//Camera C detect parameter setting;
	GetPrivateProfileStringA("DetectParameter", "cameraC_point_area_min", "", IniRead, 24, IniPath);
	parameters_setting_trans.cameraC_point_area_min = atoi(IniRead);
	m_dlgFiberDetectParametersSettingC.GetDlgItem(IDC_EDIT_POINT_AREA_MIN)->SetWindowTextA(IniRead);

	GetPrivateProfileStringA("DetectParameter", "cameraC_point_area_max", "", IniRead, 24, IniPath);
	parameters_setting_trans.cameraC_point_area_max = atoi(IniRead);
	m_dlgFiberDetectParametersSettingC.GetDlgItem(IDC_EDIT_POINT_AREA_MAX)->SetWindowTextA(IniRead);

	GetPrivateProfileStringA("DetectParameter", "cameraC_point_length_min", "", IniRead, 24, IniPath);
	parameters_setting_trans.cameraC_point_length_min = atoi(IniRead);
	m_dlgFiberDetectParametersSettingC.GetDlgItem(IDC_EDIT_POINT_LENGTH_MIN)->SetWindowTextA(IniRead);

	GetPrivateProfileStringA("DetectParameter", "cameraC_point_length_max", "", IniRead, 24, IniPath);
	parameters_setting_trans.cameraC_point_length_max = atoi(IniRead);
	m_dlgFiberDetectParametersSettingC.GetDlgItem(IDC_EDIT_POINT_LENGTH_MAX)->SetWindowTextA(IniRead);

	GetPrivateProfileStringA("DetectParameter", "cameraC_point_width_min", "", IniRead, 24, IniPath);
	parameters_setting_trans.cameraC_point_width_min = atoi(IniRead);
	m_dlgFiberDetectParametersSettingC.GetDlgItem(IDC_EDIT_POINT_WIDTH_MIN)->SetWindowTextA(IniRead);

	GetPrivateProfileStringA("DetectParameter", "cameraC_point_width_max", "", IniRead, 24, IniPath);
	parameters_setting_trans.cameraC_point_width_max = atoi(IniRead);
	m_dlgFiberDetectParametersSettingC.GetDlgItem(IDC_EDIT_POINT_WIDTH_MAX)->SetWindowTextA(IniRead);

	GetPrivateProfileStringA("DetectParameter", "cameraC_thread_area_min", "", IniRead, 24, IniPath);
	parameters_setting_trans.cameraC_thread_area_min = atoi(IniRead);
	m_dlgFiberDetectParametersSettingC.GetDlgItem(IDC_EDIT_THREAD_AREA_MIN)->SetWindowTextA(IniRead);

	GetPrivateProfileStringA("DetectParameter", "cameraC_thread_area_max", "", IniRead, 24, IniPath);
	parameters_setting_trans.cameraC_thread_area_max = atoi(IniRead);
	m_dlgFiberDetectParametersSettingC.GetDlgItem(IDC_EDIT_THREAD_AREA_MAX)->SetWindowTextA(IniRead);

	GetPrivateProfileStringA("DetectParameter", "cameraC_thread_length_min", "", IniRead, 24, IniPath);
	parameters_setting_trans.cameraC_thread_length_min = atoi(IniRead);
	m_dlgFiberDetectParametersSettingC.GetDlgItem(IDC_EDIT_THREAD_LENGTH_MIN)->SetWindowTextA(IniRead);

	GetPrivateProfileStringA("DetectParameter", "cameraC_thread_length_max", "", IniRead, 24, IniPath);
	parameters_setting_trans.cameraC_thread_length_max = atoi(IniRead);
	m_dlgFiberDetectParametersSettingC.GetDlgItem(IDC_EDIT_THREAD_LENGTH_MAX)->SetWindowTextA(IniRead);

	GetPrivateProfileStringA("DetectParameter", "cameraC_thread_width_min", "", IniRead, 24, IniPath);
	parameters_setting_trans.cameraC_thread_width_min = atoi(IniRead);
	m_dlgFiberDetectParametersSettingC.GetDlgItem(IDC_EDIT_THREAD_WIDTH_MIN)->SetWindowTextA(IniRead);

	GetPrivateProfileStringA("DetectParameter", "cameraC_thread_width_max", "", IniRead, 24, IniPath);
	parameters_setting_trans.cameraC_thread_width_max = atoi(IniRead);
	m_dlgFiberDetectParametersSettingC.GetDlgItem(IDC_EDIT_THREAD_WIDTH_MAX)->SetWindowTextA(IniRead);


	//Camera D detect parameter setting;
	GetPrivateProfileStringA("DetectParameter", "cameraD_point_area_min", "", IniRead, 24, IniPath);
	parameters_setting_trans.cameraD_point_area_min = atoi(IniRead);
	m_dlgFiberDetectParametersSettingD.GetDlgItem(IDC_EDIT_POINT_AREA_MIN)->SetWindowTextA(IniRead);

	GetPrivateProfileStringA("DetectParameter", "cameraD_point_area_max", "", IniRead, 24, IniPath);
	parameters_setting_trans.cameraD_point_area_max = atoi(IniRead);
	m_dlgFiberDetectParametersSettingD.GetDlgItem(IDC_EDIT_POINT_AREA_MAX)->SetWindowTextA(IniRead);

	GetPrivateProfileStringA("DetectParameter", "cameraD_point_length_min", "", IniRead, 24, IniPath);
	parameters_setting_trans.cameraD_point_length_min = atoi(IniRead);
	m_dlgFiberDetectParametersSettingD.GetDlgItem(IDC_EDIT_POINT_LENGTH_MIN)->SetWindowTextA(IniRead);

	GetPrivateProfileStringA("DetectParameter", "cameraD_point_length_max", "", IniRead, 24, IniPath);
	parameters_setting_trans.cameraD_point_length_max = atoi(IniRead);
	m_dlgFiberDetectParametersSettingD.GetDlgItem(IDC_EDIT_POINT_LENGTH_MAX)->SetWindowTextA(IniRead);

	GetPrivateProfileStringA("DetectParameter", "cameraD_point_width_min", "", IniRead, 24, IniPath);
	parameters_setting_trans.cameraD_point_width_min = atoi(IniRead);
	m_dlgFiberDetectParametersSettingD.GetDlgItem(IDC_EDIT_POINT_WIDTH_MIN)->SetWindowTextA(IniRead);

	GetPrivateProfileStringA("DetectParameter", "cameraD_point_width_max", "", IniRead, 24, IniPath);
	parameters_setting_trans.cameraD_point_width_max = atoi(IniRead);
	m_dlgFiberDetectParametersSettingD.GetDlgItem(IDC_EDIT_POINT_WIDTH_MAX)->SetWindowTextA(IniRead);

	GetPrivateProfileStringA("DetectParameter", "cameraD_thread_area_min", "", IniRead, 24, IniPath);
	parameters_setting_trans.cameraD_thread_area_min = atoi(IniRead);
	m_dlgFiberDetectParametersSettingD.GetDlgItem(IDC_EDIT_THREAD_AREA_MIN)->SetWindowTextA(IniRead);

	GetPrivateProfileStringA("DetectParameter", "cameraD_thread_area_max", "", IniRead, 24, IniPath);
	parameters_setting_trans.cameraD_thread_area_max = atoi(IniRead);
	m_dlgFiberDetectParametersSettingD.GetDlgItem(IDC_EDIT_THREAD_AREA_MAX)->SetWindowTextA(IniRead);

	GetPrivateProfileStringA("DetectParameter", "cameraD_thread_length_min", "", IniRead, 24, IniPath);
	parameters_setting_trans.cameraD_thread_length_min = atoi(IniRead);
	m_dlgFiberDetectParametersSettingD.GetDlgItem(IDC_EDIT_THREAD_LENGTH_MIN)->SetWindowTextA(IniRead);

	GetPrivateProfileStringA("DetectParameter", "cameraD_thread_length_max", "", IniRead, 24, IniPath);
	parameters_setting_trans.cameraD_thread_length_max = atoi(IniRead);
	m_dlgFiberDetectParametersSettingD.GetDlgItem(IDC_EDIT_THREAD_LENGTH_MAX)->SetWindowTextA(IniRead);

	GetPrivateProfileStringA("DetectParameter", "cameraD_thread_width_min", "", IniRead, 24, IniPath);
	parameters_setting_trans.cameraD_thread_width_min = atoi(IniRead);
	m_dlgFiberDetectParametersSettingD.GetDlgItem(IDC_EDIT_THREAD_WIDTH_MIN)->SetWindowTextA(IniRead);

	GetPrivateProfileStringA("DetectParameter", "cameraD_thread_width_max", "", IniRead, 24, IniPath);
	parameters_setting_trans.cameraD_thread_width_max = atoi(IniRead);
	m_dlgFiberDetectParametersSettingD.GetDlgItem(IDC_EDIT_THREAD_WIDTH_MAX)->SetWindowTextA(IniRead);

	//DebugParaneters setting;
	GetPrivateProfileStringA("DebugParameter", "debug_cameraA", "", IniRead, 24, IniPath);
	parameters_setting_trans.debug_cameraA = atoi(IniRead);
	m_dlgDebugParametersSetting.m_checkDebugCameraA.SetCheck(parameters_setting_trans.debug_cameraA);

	GetPrivateProfileStringA("DebugParameter", "debug_cameraB", "", IniRead, 24, IniPath);
	parameters_setting_trans.debug_cameraB = atoi(IniRead);
	m_dlgDebugParametersSetting.m_checkDebugCameraB.SetCheck(parameters_setting_trans.debug_cameraB);

	GetPrivateProfileStringA("DebugParameter", "debug_cameraC", "", IniRead, 24, IniPath);
	parameters_setting_trans.debug_cameraC = atoi(IniRead);
	m_dlgDebugParametersSetting.m_checkDebugCameraC.SetCheck(parameters_setting_trans.debug_cameraC);

	GetPrivateProfileStringA("DebugParameter", "debug_cameraD", "", IniRead, 24, IniPath);
	parameters_setting_trans.debug_cameraD = atoi(IniRead);
	m_dlgDebugParametersSetting.m_checkDebugCameraD.SetCheck(parameters_setting_trans.debug_cameraD);

	GetPrivateProfileStringA("DebugParameter", "stop_volve", "", IniRead, 24, IniPath);
	parameters_setting_trans.stop_volve = atoi(IniRead);
	m_dlgDebugParametersSetting.m_checkStopValve.SetCheck(parameters_setting_trans.stop_volve);

	GetPrivateProfileStringA("DebugParameter", "save_blob", "", IniRead, 24, IniPath);
	parameters_setting_trans.save_blob = atoi(IniRead);
	m_dlgDebugParametersSetting.m_checkSaveBolb.SetCheck(parameters_setting_trans.save_blob);

	return;

}

void CDetectParametersSetting::OnTcnSelchangeTabParametersSetting(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO:  在此添加控件通知处理程序代码
	switch (m_cTabControl.GetCurSel())
	{
	case 0:
		m_dlgBaseDetectParametersSettting.ShowWindow(true);
		m_dlgDebugParametersSetting.ShowWindow(false);
		m_dlgValveParametersSettting.ShowWindow(false);
		m_dlgFiberDetectParametersSettingA.ShowWindow(false);
		m_dlgFiberDetectParametersSettingB.ShowWindow(false);
		m_dlgFiberDetectParametersSettingC.ShowWindow(false);
		m_dlgFiberDetectParametersSettingD.ShowWindow(false);
		break;
	case 1:
		m_dlgBaseDetectParametersSettting.ShowWindow(false);
		m_dlgDebugParametersSetting.ShowWindow(true);
		m_dlgValveParametersSettting.ShowWindow(false);
		m_dlgFiberDetectParametersSettingA.ShowWindow(false);
		m_dlgFiberDetectParametersSettingB.ShowWindow(false);
		m_dlgFiberDetectParametersSettingC.ShowWindow(false);
		m_dlgFiberDetectParametersSettingD.ShowWindow(false);
		break;	
	case 2:
		m_dlgBaseDetectParametersSettting.ShowWindow(false);
		m_dlgDebugParametersSetting.ShowWindow(false);
		m_dlgValveParametersSettting.ShowWindow(true);
		m_dlgFiberDetectParametersSettingA.ShowWindow(false);
		m_dlgFiberDetectParametersSettingB.ShowWindow(false);
		m_dlgFiberDetectParametersSettingC.ShowWindow(false);
		m_dlgFiberDetectParametersSettingD.ShowWindow(false);
		break;
	case 3:
		m_dlgBaseDetectParametersSettting.ShowWindow(false);
		m_dlgDebugParametersSetting.ShowWindow(false);
		m_dlgValveParametersSettting.ShowWindow(false);
		m_dlgFiberDetectParametersSettingA.ShowWindow(true);
		m_dlgFiberDetectParametersSettingB.ShowWindow(false);
		m_dlgFiberDetectParametersSettingC.ShowWindow(false);
		m_dlgFiberDetectParametersSettingD.ShowWindow(false);
		break;
	case 4:
		m_dlgBaseDetectParametersSettting.ShowWindow(false);
		m_dlgDebugParametersSetting.ShowWindow(false);
		m_dlgValveParametersSettting.ShowWindow(false);
		m_dlgFiberDetectParametersSettingA.ShowWindow(false);
		m_dlgFiberDetectParametersSettingB.ShowWindow(true);
		m_dlgFiberDetectParametersSettingC.ShowWindow(false);
		m_dlgFiberDetectParametersSettingD.ShowWindow(false);
		break;
	case 5:
		m_dlgBaseDetectParametersSettting.ShowWindow(false);
		m_dlgDebugParametersSetting.ShowWindow(false);
		m_dlgValveParametersSettting.ShowWindow(false);
		m_dlgFiberDetectParametersSettingA.ShowWindow(false);
		m_dlgFiberDetectParametersSettingB.ShowWindow(false);
		m_dlgFiberDetectParametersSettingC.ShowWindow(true);
		m_dlgFiberDetectParametersSettingD.ShowWindow(false);
		break;
	case 6:
		m_dlgBaseDetectParametersSettting.ShowWindow(false);
		m_dlgDebugParametersSetting.ShowWindow(false);
		m_dlgValveParametersSettting.ShowWindow(false);
		m_dlgFiberDetectParametersSettingA.ShowWindow(false);
		m_dlgFiberDetectParametersSettingB.ShowWindow(false);
		m_dlgFiberDetectParametersSettingC.ShowWindow(false);
		m_dlgFiberDetectParametersSettingD.ShowWindow(true);
		break;
	default:
		break;
	}
	*pResult = 0;
}

/*
功能：获得当前程序的运行目录(.exe)
*/
CString CDetectParametersSetting::GetModulePath(void)
{
	CString    sPath;
	GetModuleFileNameA(NULL, sPath.GetBufferSetLength(MAX_PATH + 1), MAX_PATH);
	sPath.ReleaseBuffer();
	int    nPos;
	nPos = sPath.ReverseFind('\\');
	sPath = sPath.Left(nPos);
	return   sPath;
}



/*
Function: To write the parameters into the ini file;
The function that send PostMessage and socket message is at CFiberDetectControlDlg::OnBnClickedButtonParametersSetting()';
*/
void CDetectParametersSetting::OnBnClickedOk()
{
	// TODO:  在此添加控件通知处理程序代码
	WriteIni();	

	int nSelect = 0;

	Vsee::VCameraMessageTrans *frame = new Vsee::VCameraMessageTrans;
	parameters_setting_trans.signalling = nSelect;
	memcpy(&frame->parameters_setting_trans, &parameters_setting_trans, sizeof(parameters_setting_trans));
	frame->signalling = 4;

	CDialogEx::OnOK();
}

void CDetectParametersSetting::WriteIni()
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

	//GetPrivateProfileStringA("BaseDetectParameter", "cameraA_begin", "", IniRead, 24, IniPath);	
	m_dlgBaseDetectParametersSettting.GetDlgItem(IDC_EDIT_CAMERA1_BEGIN_POINT)->GetWindowTextA(IniRead);
	IniWrite.Format("%s", IniRead);
	WritePrivateProfileString("BaseDetectParameter", "cameraA_begin", IniWrite, IniPath);
	parameters_setting_trans.cameraA_begin = atoi(IniWrite);

	//GetPrivateProfileStringA("BaseDetectParameter", "cameraA_end", "", IniRead, 24, IniPath);
	m_dlgBaseDetectParametersSettting.GetDlgItem(IDC_EDIT_CAMERA1_END_POINT)->GetWindowTextA(IniRead);
	IniWrite.Format("%s", IniRead);
	WritePrivateProfileString("BaseDetectParameter", "cameraA_end", IniWrite, IniPath);
	parameters_setting_trans.cameraA_end = atoi(IniWrite);
	
	//GetPrivateProfileStringA("BaseDetectParameter", "cameraA_align_begin", "", IniRead, 24, IniPath);
	m_dlgBaseDetectParametersSettting.GetDlgItem(IDC_EDIT_CAMERA1_ALIGNING_BEGIN_POINT)->GetWindowTextA(IniRead);
	IniWrite.Format("%s", IniRead);
	WritePrivateProfileString("BaseDetectParameter", "cameraA_align_begin", IniWrite, IniPath);
	parameters_setting_trans.cameraA_align_begin = atoi(IniRead);
	

	//GetPrivateProfileStringA("BaseDetectParameter", "cameraA_align_end", "", IniRead, 24, IniPath);
	m_dlgBaseDetectParametersSettting.GetDlgItem(IDC_EDIT_CAMERA1_ALIGNING_END_POINT)->GetWindowTextA(IniRead);
	IniWrite.Format("%s", IniRead);
	WritePrivateProfileString("BaseDetectParameter", "cameraA_align_end", IniWrite, IniPath);
	parameters_setting_trans.cameraA_align_end = atoi(IniRead);
	
	//GetPrivateProfileStringA("BaseDetectParameter", "cameraA_detect_begin", "", IniRead, 24, IniPath);
	m_dlgBaseDetectParametersSettting.GetDlgItem(IDC_EDIT_CAMERA1_DETECT_BEGIN_POINT)->GetWindowTextA(IniRead);
	IniWrite.Format("%s", IniRead);
	WritePrivateProfileString("BaseDetectParameter", "cameraA_detect_begin", IniWrite, IniPath);
	parameters_setting_trans.cameraA_detect_begin = atoi(IniRead);
	

	//GetPrivateProfileStringA("BaseDetectParameter", "cameraA_detect_end", "", IniRead, 24, IniPath);
	m_dlgBaseDetectParametersSettting.GetDlgItem(IDC_EDIT_CAMERA1_DETECT_END_POINT)->GetWindowTextA(IniRead);
	parameters_setting_trans.cameraA_detect_end = atoi(IniRead);
	IniWrite.Format("%s", IniRead);
	WritePrivateProfileString("BaseDetectParameter", "cameraA_detect_end", IniWrite, IniPath);
	

	//GetPrivateProfileStringA("BaseDetectParameter", "cameraA_valve_amount", "", IniRead, 24, IniPath);
	m_dlgBaseDetectParametersSettting.GetDlgItem(IDC_EDIT_CAMERA1_VALVE_AMOUNT)->GetWindowTextA(IniRead);
	parameters_setting_trans.cameraA_valve_amount = atoi(IniRead);
	IniWrite.Format("%s", IniRead);
	WritePrivateProfileString("BaseDetectParameter", "cameraA_valve_amount", IniWrite, IniPath);
	

	//GetPrivateProfileStringA("BaseDetectParameter", "cameraB_begin", "", IniRead, 24, IniPath);
	m_dlgBaseDetectParametersSettting.GetDlgItem(IDC_EDIT_CAMERA2_BEGIN_POINT)->GetWindowTextA(IniRead);
	parameters_setting_trans.cameraB_begin = atoi(IniRead);
	IniWrite.Format("%s", IniRead);
	WritePrivateProfileString("BaseDetectParameter", "cameraB_begin", IniWrite, IniPath);
	

	//GetPrivateProfileStringA("BaseDetectParameter", "cameraB_end", "", IniRead, 24, IniPath);
	m_dlgBaseDetectParametersSettting.GetDlgItem(IDC_EDIT_CAMERA2_END_POINT)->GetWindowTextA(IniRead);
	parameters_setting_trans.cameraB_end = atoi(IniRead);
	IniWrite.Format("%s", IniRead);
	WritePrivateProfileString("BaseDetectParameter", "cameraB_end", IniWrite, IniPath);
	

	//GetPrivateProfileStringA("BaseDetectParameter", "cameraB_align_begin", "", IniRead, 24, IniPath);
	m_dlgBaseDetectParametersSettting.GetDlgItem(IDC_EDIT_CAMERA2_ALIGNING_BEGIN_POINT)->GetWindowTextA(IniRead);
	parameters_setting_trans.cameraB_align_begin = atoi(IniRead);
	IniWrite.Format("%s", IniRead);
	WritePrivateProfileString("BaseDetectParameter", "cameraB_align_begin", IniWrite, IniPath);
	

	//GetPrivateProfileStringA("BaseDetectParameter", "cameraB_align_end", "", IniRead, 24, IniPath);
	m_dlgBaseDetectParametersSettting.GetDlgItem(IDC_EDIT_CAMERA2_ALIGNING_END_POINT)->GetWindowTextA(IniRead);
	parameters_setting_trans.cameraB_align_end = atoi(IniRead);
	IniWrite.Format("%s", IniRead);
	WritePrivateProfileString("BaseDetectParameter", "cameraB_align_end", IniWrite, IniPath);
	

	//GetPrivateProfileStringA("BaseDetectParameter", "cameraB_detect_begin", "", IniRead, 24, IniPath);
	m_dlgBaseDetectParametersSettting.GetDlgItem(IDC_EDIT_CAMERA2_DETECT_BEGIN_POINT)->GetWindowTextA(IniRead);
	parameters_setting_trans.cameraB_detect_begin = atoi(IniRead);
	IniWrite.Format("%s", IniRead);
	WritePrivateProfileString("BaseDetectParameter", "cameraB_detect_begin", IniWrite, IniPath);
	

	//GetPrivateProfileStringA("BaseDetectParameter", "cameraB_detect_end", "", IniRead, 24, IniPath);
	m_dlgBaseDetectParametersSettting.GetDlgItem(IDC_EDIT_CAMERA2_DETECT_END_POINT)->GetWindowTextA(IniRead);
	parameters_setting_trans.cameraB_detect_end = atoi(IniRead);
	IniWrite.Format("%s", IniRead);
	WritePrivateProfileString("BaseDetectParameter", "cameraB_detect_end", IniWrite, IniPath);
	

	//GetPrivateProfileStringA("BaseDetectParameter", "cameraB_valve_amount", "", IniRead, 24, IniPath);
	m_dlgBaseDetectParametersSettting.GetDlgItem(IDC_EDIT_CAMERA2_VALVE_AMOUNT)->GetWindowTextA(IniRead);
	parameters_setting_trans.cameraB_valve_amount = atoi(IniRead);
	IniWrite.Format("%s", IniRead);
	WritePrivateProfileString("BaseDetectParameter", "cameraB_valve_amount", IniWrite, IniPath);
	

	//GetPrivateProfileStringA("BaseDetectParameter", "cameraC_begin", "", IniRead, 24, IniPath);
	m_dlgBaseDetectParametersSettting.GetDlgItem(IDC_EDIT_CAMERA3_BEGIN_POINT)->GetWindowTextA(IniRead);
	parameters_setting_trans.cameraC_begin = atoi(IniRead);
	IniWrite.Format("%s", IniRead);
	WritePrivateProfileString("BaseDetectParameter", "cameraC_begin", IniWrite, IniPath);
	

	//GetPrivateProfileStringA("BaseDetectParameter", "cameraC_end", "", IniRead, 24, IniPath);
	m_dlgBaseDetectParametersSettting.GetDlgItem(IDC_EDIT_CAMERA3_END_POINT)->GetWindowTextA(IniRead);
	parameters_setting_trans.cameraC_end = atoi(IniRead);
	IniWrite.Format("%s", IniRead);
	WritePrivateProfileString("BaseDetectParameter", "cameraC_end", IniWrite, IniPath);
	

	//GetPrivateProfileStringA("BaseDetectParameter", "cameraC_align_begin", "", IniRead, 24, IniPath);
	m_dlgBaseDetectParametersSettting.GetDlgItem(IDC_EDIT_CAMERA3_ALIGNING_BEGIN_POINT)->GetWindowTextA(IniRead);
	parameters_setting_trans.cameraC_align_begin = atoi(IniRead);
	IniWrite.Format("%s", IniRead);
	WritePrivateProfileString("BaseDetectParameter", "cameraC_align_begin", IniWrite, IniPath);
	

	//GetPrivateProfileStringA("BaseDetectParameter", "cameraC_align_end", "", IniRead, 24, IniPath);
	m_dlgBaseDetectParametersSettting.GetDlgItem(IDC_EDIT_CAMERA3_ALIGNING_END_POINT)->GetWindowTextA(IniRead);
	parameters_setting_trans.cameraC_align_end = atoi(IniRead);
	IniWrite.Format("%s", IniRead);
	WritePrivateProfileString("BaseDetectParameter", "cameraC_align_end", IniWrite, IniPath);
	

	//GetPrivateProfileStringA("BaseDetectParameter", "cameraC_detect_begin", "", IniRead, 24, IniPath);
	m_dlgBaseDetectParametersSettting.GetDlgItem(IDC_EDIT_CAMERA3_DETECT_BEGIN_POINT)->GetWindowTextA(IniRead);
	parameters_setting_trans.cameraC_detect_begin = atoi(IniRead);
	IniWrite.Format("%s", IniRead);
	WritePrivateProfileString("BaseDetectParameter", "cameraC_detect_begin", IniWrite, IniPath);
	

	//GetPrivateProfileStringA("BaseDetectParameter", "cameraC_detect_end", "", IniRead, 24, IniPath);
	m_dlgBaseDetectParametersSettting.GetDlgItem(IDC_EDIT_CAMERA3_DETECT_END_POINT)->GetWindowTextA(IniRead);
	parameters_setting_trans.cameraC_detect_end = atoi(IniRead);
	IniWrite.Format("%s", IniRead);
	WritePrivateProfileString("BaseDetectParameter", "cameraC_detect_end", IniWrite, IniPath);
	

	//GetPrivateProfileStringA("BaseDetectParameter", "cameraC_valve_amount", "", IniRead, 24, IniPath);
	m_dlgBaseDetectParametersSettting.GetDlgItem(IDC_EDIT_CAMERA3_VALVE_AMOUNT)->GetWindowTextA(IniRead);
	parameters_setting_trans.cameraC_valve_amount = atoi(IniRead);
	IniWrite.Format("%s", IniRead);
	WritePrivateProfileString("BaseDetectParameter", "cameraC_valve_amount", IniWrite, IniPath);
	

	//GetPrivateProfileStringA("BaseDetectParameter", "cameraD_begin", "", IniRead, 24, IniPath);
	m_dlgBaseDetectParametersSettting.GetDlgItem(IDC_EDIT_CAMERA4_BEGIN_POINT)->GetWindowTextA(IniRead);
	parameters_setting_trans.cameraD_begin = atoi(IniRead);
	IniWrite.Format("%s", IniRead);
	WritePrivateProfileString("BaseDetectParameter", "cameraD_begin", IniWrite, IniPath);
	

	//GetPrivateProfileStringA("BaseDetectParameter", "cameraD_end", "", IniRead, 24, IniPath);
	m_dlgBaseDetectParametersSettting.GetDlgItem(IDC_EDIT_CAMERA4_END_POINT)->GetWindowTextA(IniRead);
	parameters_setting_trans.cameraD_end = atoi(IniRead);
	IniWrite.Format("%s", IniRead);
	WritePrivateProfileString("BaseDetectParameter", "cameraD_end", IniWrite, IniPath);
	

	//GetPrivateProfileStringA("BaseDetectParameter", "cameraD_align_begin", "", IniRead, 24, IniPath);
	m_dlgBaseDetectParametersSettting.GetDlgItem(IDC_EDIT_CAMERA4_ALIGNING_BEGIN_POINT)->GetWindowTextA(IniRead);
	parameters_setting_trans.cameraD_align_begin = atoi(IniRead);
	IniWrite.Format("%s", IniRead);
	WritePrivateProfileString("BaseDetectParameter", "cameraD_align_begin", IniWrite, IniPath);
	
	//GetPrivateProfileStringA("BaseDetectParameter", "cameraD_align_end", "", IniRead, 24, IniPath);
	m_dlgBaseDetectParametersSettting.GetDlgItem(IDC_EDIT_CAMERA4_ALIGNING_END_POINT)->GetWindowTextA(IniRead);
	parameters_setting_trans.cameraD_align_end = atoi(IniRead);
	IniWrite.Format("%s", IniRead);
	WritePrivateProfileString("BaseDetectParameter", "cameraD_align_end", IniWrite, IniPath);
	

	//GetPrivateProfileStringA("BaseDetectParameter", "cameraD_detect_begin", "", IniRead, 24, IniPath);
	m_dlgBaseDetectParametersSettting.GetDlgItem(IDC_EDIT_CAMERA4_DETECT_BEGIN_POINT)->GetWindowTextA(IniRead);
	parameters_setting_trans.cameraD_detect_begin = atoi(IniRead);
	IniWrite.Format("%s", IniRead);
	WritePrivateProfileString("BaseDetectParameter", "cameraD_detect_begin", IniWrite, IniPath);
	

	//GetPrivateProfileStringA("BaseDetectParameter", "cameraD_detect_end", "", IniRead, 24, IniPath);
	m_dlgBaseDetectParametersSettting.GetDlgItem(IDC_EDIT_CAMERA4_DETECT_END_POINT)->GetWindowTextA(IniRead);
	parameters_setting_trans.cameraD_detect_end = atoi(IniRead);
	IniWrite.Format("%s", IniRead);
	WritePrivateProfileString("BaseDetectParameter", "cameraD_detect_end", IniWrite, IniPath);
	

	//GetPrivateProfileStringA("BaseDetectParameter", "cameraD_valve_amount", "", IniRead, 24, IniPath);
	m_dlgBaseDetectParametersSettting.GetDlgItem(IDC_EDIT_CAMERA4_VALVE_AMOUNT)->GetWindowTextA(IniRead);
	parameters_setting_trans.cameraD_valve_amount = atoi(IniRead);
	IniWrite.Format("%s", IniRead);
	WritePrivateProfileString("BaseDetectParameter", "cameraD_valve_amount", IniWrite, IniPath);
	

	//Camera A detect parameter setting;
	//GetPrivateProfileStringA("ValveParameter", "cameraA_delay", "", IniRead, 24, IniPath);
	m_dlgValveParametersSettting.GetDlgItem(IDC_EDIT_VALVE_A_DELAY)->GetWindowTextA(IniRead);
	parameters_setting_trans.cameraA_delay = atoi(IniRead);
	IniWrite.Format("%s", IniRead);
	WritePrivateProfileString("ValveParameter", "cameraA_delay", IniWrite, IniPath);
	

	//GetPrivateProfileStringA("ValveParameter", "cameraA_hold", "", IniRead, 24, IniPath);
	m_dlgValveParametersSettting.GetDlgItem(IDC_EDIT_VALVE_A_HOLD)->GetWindowTextA(IniRead);
	parameters_setting_trans.cameraA_hold = atoi(IniRead);
	IniWrite.Format("%s", IniRead);
	WritePrivateProfileString("ValveParameter", "cameraA_hold", IniWrite, IniPath);
	

	//GetPrivateProfileStringA("ValveParameter", "cameraB_delay", "", IniRead, 24, IniPath);
	m_dlgValveParametersSettting.GetDlgItem(IDC_EDIT_VALVE_B_DELAY)->GetWindowTextA(IniRead);
	parameters_setting_trans.cameraB_delay = atoi(IniRead);
	IniWrite.Format("%s", IniRead);
	WritePrivateProfileString("ValveParameter", "cameraB_delay", IniWrite, IniPath);
	//-----------------------------------------------------------------------------------------

	//GetPrivateProfileStringA("ValveParameter", "cameraB_hold", "", IniRead, 24, IniPath);
	m_dlgValveParametersSettting.GetDlgItem(IDC_EDIT_VALVE_B_HOLD)->GetWindowTextA(IniRead);
	parameters_setting_trans.cameraB_hold = atoi(IniRead);
	IniWrite.Format("%s", IniRead);
	WritePrivateProfileString("ValveParameter", "cameraB_hold", IniWrite, IniPath);
	

	//GetPrivateProfileStringA("ValveParameter", "cameraC_delay", "", IniRead, 24, IniPath);
	m_dlgValveParametersSettting.GetDlgItem(IDC_EDIT_VALVE_C_DELAY)->GetWindowTextA(IniRead);
	parameters_setting_trans.cameraC_delay = atoi(IniRead);
	IniWrite.Format("%s", IniRead);
	WritePrivateProfileString("ValveParameter", "cameraC_delay", IniWrite, IniPath);


	//GetPrivateProfileStringA("ValveParameter", "cameraC_hold", "", IniRead, 24, IniPath);
	m_dlgValveParametersSettting.GetDlgItem(IDC_EDIT_VALVE_C_HOLD)->GetWindowTextA(IniRead);
	parameters_setting_trans.cameraC_hold = atoi(IniRead);
	IniWrite.Format("%s", IniRead);
	WritePrivateProfileString("ValveParameter", "cameraC_hold", IniWrite, IniPath);

	//GetPrivateProfileStringA("ValveParameter", "cameraD_delay", "", IniRead, 24, IniPath);
	m_dlgValveParametersSettting.GetDlgItem(IDC_EDIT_VALVE_D_DELAY)->GetWindowTextA(IniRead);
	parameters_setting_trans.cameraD_delay = atoi(IniRead);
	IniWrite.Format("%s", IniRead);
	WritePrivateProfileString("ValveParameter", "cameraD_delay", IniWrite, IniPath);

	//GetPrivateProfileStringA("ValveParameter", "cameraD_hold", "", IniRead, 24, IniPath);
	m_dlgValveParametersSettting.GetDlgItem(IDC_EDIT_VALVE_D_HOLD)->GetWindowTextA(IniRead);
	parameters_setting_trans.cameraD_hold = atoi(IniRead);
	IniWrite.Format("%s", IniRead);
	WritePrivateProfileString("ValveParameter", "cameraD_hold", IniWrite, IniPath);

	//GetPrivateProfileStringA("ValveParameter", "valve_work_times", "", IniRead, 24, IniPath);
	m_dlgValveParametersSettting.GetDlgItem(IDC_EDIT_VALVE_WORK_TIMES)->GetWindowTextA(IniRead);
	parameters_setting_trans.valve_work_times = atoi(IniRead);
	IniWrite.Format("%s", IniRead);
	WritePrivateProfileString("ValveParameter", "valve_work_times", IniWrite, IniPath);

	//GetPrivateProfileStringA("ValveParameter", "valve_stop_time", "", IniRead, 24, IniPath);
	m_dlgValveParametersSettting.GetDlgItem(IDC_EDIT_VALVE_STOP_TIMES)->GetWindowTextA(IniRead);
	parameters_setting_trans.valve_stop_time = atoi(IniRead);
	IniWrite.Format("%s", IniRead);
	WritePrivateProfileString("ValveParameter", "valve_stop_time", IniWrite, IniPath);

	//GetPrivateProfileStringA("DetectParameter", "cameraA_point_area_min", "", IniRead, 24, IniPath);
	m_dlgFiberDetectParametersSettingA.GetDlgItem(IDC_EDIT_POINT_AREA_MIN)->GetWindowTextA(IniRead);
	parameters_setting_trans.cameraA_point_area_min = atoi(IniRead);
	IniWrite.Format("%s", IniRead);
	WritePrivateProfileString("DetectParameter", "cameraA_point_area_min", IniWrite, IniPath);

	//GetPrivateProfileStringA("DetectParameter", "cameraA_point_area_max", "", IniRead, 24, IniPath);
	m_dlgFiberDetectParametersSettingA.GetDlgItem(IDC_EDIT_POINT_AREA_MAX)->GetWindowTextA(IniRead);
	parameters_setting_trans.cameraA_point_area_max = atoi(IniRead);
	IniWrite.Format("%s", IniRead);
	WritePrivateProfileString("DetectParameter", "cameraA_point_area_max", IniWrite, IniPath);

	//GetPrivateProfileStringA("DetectParameter", "cameraA_point_length_min", "", IniRead, 24, IniPath);
	m_dlgFiberDetectParametersSettingA.GetDlgItem(IDC_EDIT_POINT_LENGTH_MIN)->GetWindowTextA(IniRead);
	parameters_setting_trans.cameraA_point_length_min = atoi(IniRead);
	IniWrite.Format("%s", IniRead);
	WritePrivateProfileString("DetectParameter", "cameraA_point_length_min", IniWrite, IniPath);

	//GetPrivateProfileStringA("DetectParameter", "cameraA_point_length_max", "", IniRead, 24, IniPath);
	m_dlgFiberDetectParametersSettingA.GetDlgItem(IDC_EDIT_POINT_LENGTH_MAX)->GetWindowTextA(IniRead);
	parameters_setting_trans.cameraA_point_length_max = atoi(IniRead);
	IniWrite.Format("%s", IniRead);
	WritePrivateProfileString("DetectParameter", "cameraA_point_length_max", IniWrite, IniPath);

	//GetPrivateProfileStringA("DetectParameter", "cameraA_point_width_min", "", IniRead, 24, IniPath);
	m_dlgFiberDetectParametersSettingA.GetDlgItem(IDC_EDIT_POINT_WIDTH_MIN)->GetWindowTextA(IniRead);
	parameters_setting_trans.cameraA_point_width_min = atoi(IniRead);
	IniWrite.Format("%s", IniRead);
	WritePrivateProfileString("DetectParameter", "cameraA_point_width_min", IniWrite, IniPath);

	//GetPrivateProfileStringA("DetectParameter", "cameraA_point_width_max", "", IniRead, 24, IniPath);
	m_dlgFiberDetectParametersSettingA.GetDlgItem(IDC_EDIT_POINT_WIDTH_MAX)->GetWindowTextA(IniRead);
	parameters_setting_trans.cameraA_point_width_max = atoi(IniRead);
	IniWrite.Format("%s", IniRead);
	WritePrivateProfileString("DetectParameter", "cameraA_point_width_max", IniWrite, IniPath);

	//GetPrivateProfileStringA("DetectParameter", "cameraA_thread_area_min", "", IniRead, 24, IniPath);
	m_dlgFiberDetectParametersSettingA.GetDlgItem(IDC_EDIT_THREAD_AREA_MIN)->GetWindowTextA(IniRead);
	parameters_setting_trans.cameraA_thread_area_min = atoi(IniRead);
	IniWrite.Format("%s", IniRead);
	WritePrivateProfileString("DetectParameter", "cameraA_thread_area_min", IniWrite, IniPath);

	//GetPrivateProfileStringA("DetectParameter", "cameraA_thread_area_max", "", IniRead, 24, IniPath);
	m_dlgFiberDetectParametersSettingA.GetDlgItem(IDC_EDIT_THREAD_AREA_MAX)->GetWindowTextA(IniRead);
	parameters_setting_trans.cameraA_thread_area_max = atoi(IniRead);
	IniWrite.Format("%s", IniRead);
	WritePrivateProfileString("DetectParameter", "cameraA_thread_area_max", IniWrite, IniPath);

	//GetPrivateProfileStringA("DetectParameter", "cameraA_thread_length_min", "", IniRead, 24, IniPath);
	m_dlgFiberDetectParametersSettingA.GetDlgItem(IDC_EDIT_THREAD_LENGTH_MIN)->GetWindowTextA(IniRead);
	parameters_setting_trans.cameraA_thread_length_min = atoi(IniRead);
	IniWrite.Format("%s", IniRead);
	WritePrivateProfileString("DetectParameter", "cameraA_thread_length_min", IniWrite, IniPath);

	//GetPrivateProfileStringA("DetectParameter", "cameraA_thread_length_max", "", IniRead, 24, IniPath);
	m_dlgFiberDetectParametersSettingA.GetDlgItem(IDC_EDIT_THREAD_LENGTH_MAX)->GetWindowTextA(IniRead);
	parameters_setting_trans.cameraA_thread_length_max = atoi(IniRead);
	IniWrite.Format("%s", IniRead);
	WritePrivateProfileString("DetectParameter", "cameraA_thread_length_max", IniWrite, IniPath);

	//GetPrivateProfileStringA("DetectParameter", "cameraA_thread_width_min", "", IniRead, 24, IniPath);
	m_dlgFiberDetectParametersSettingA.GetDlgItem(IDC_EDIT_THREAD_WIDTH_MIN)->GetWindowTextA(IniRead);
	parameters_setting_trans.cameraA_thread_width_min = atoi(IniRead);
	IniWrite.Format("%s", IniRead);
	WritePrivateProfileString("DetectParameter", "cameraA_thread_width_min", IniWrite, IniPath);

	//GetPrivateProfileStringA("DetectParameter", "cameraA_thread_width_max", "", IniRead, 24, IniPath);
	m_dlgFiberDetectParametersSettingA.GetDlgItem(IDC_EDIT_THREAD_WIDTH_MAX)->GetWindowTextA(IniRead);
	parameters_setting_trans.cameraA_thread_width_max = atoi(IniRead);
	IniWrite.Format("%s", IniRead);
	WritePrivateProfileString("DetectParameter", "cameraA_thread_width_max", IniWrite, IniPath);


	//Camera B detect parameter setting;
	//GetPrivateProfileStringA("DetectParameter", "cameraB_point_area_min", "", IniRead, 24, IniPath);
	m_dlgFiberDetectParametersSettingB.GetDlgItem(IDC_EDIT_POINT_AREA_MIN)->GetWindowTextA(IniRead);
	parameters_setting_trans.cameraB_point_area_min = atoi(IniRead);
	IniWrite.Format("%s", IniRead);
	WritePrivateProfileString("DetectParameter", "cameraB_point_area_min", IniWrite, IniPath);

	//GetPrivateProfileStringA("DetectParameter", "cameraB_point_area_max", "", IniRead, 24, IniPath);
	m_dlgFiberDetectParametersSettingB.GetDlgItem(IDC_EDIT_POINT_AREA_MAX)->GetWindowTextA(IniRead);
	parameters_setting_trans.cameraB_point_area_max = atoi(IniRead);
	IniWrite.Format("%s", IniRead);
	WritePrivateProfileString("DetectParameter", "cameraB_point_area_max", IniWrite, IniPath);

	//GetPrivateProfileStringA("DetectParameter", "cameraB_point_length_min", "", IniRead, 24, IniPath);
	m_dlgFiberDetectParametersSettingB.GetDlgItem(IDC_EDIT_POINT_LENGTH_MIN)->GetWindowTextA(IniRead);
	parameters_setting_trans.cameraB_point_length_min = atoi(IniRead);
	IniWrite.Format("%s", IniRead);
	WritePrivateProfileString("DetectParameter", "cameraB_point_length_min", IniWrite, IniPath);

	//GetPrivateProfileStringA("DetectParameter", "cameraB_point_length_max", "", IniRead, 24, IniPath);
	m_dlgFiberDetectParametersSettingB.GetDlgItem(IDC_EDIT_POINT_LENGTH_MAX)->GetWindowTextA(IniRead);
	parameters_setting_trans.cameraB_point_length_max = atoi(IniRead);
	IniWrite.Format("%s", IniRead);
	WritePrivateProfileString("DetectParameter", "cameraB_point_length_max", IniWrite, IniPath);

	//GetPrivateProfileStringA("DetectParameter", "cameraB_point_width_min", "", IniRead, 24, IniPath);
	m_dlgFiberDetectParametersSettingB.GetDlgItem(IDC_EDIT_POINT_WIDTH_MIN)->GetWindowTextA(IniRead);
	parameters_setting_trans.cameraB_point_width_min = atoi(IniRead);
	IniWrite.Format("%s", IniRead);
	WritePrivateProfileString("DetectParameter", "cameraB_point_width_min", IniWrite, IniPath);

	//GetPrivateProfileStringA("DetectParameter", "cameraB_point_width_max", "", IniRead, 24, IniPath);
	m_dlgFiberDetectParametersSettingB.GetDlgItem(IDC_EDIT_POINT_WIDTH_MAX)->GetWindowTextA(IniRead);
	parameters_setting_trans.cameraB_point_width_max = atoi(IniRead);
	IniWrite.Format("%s", IniRead);
	WritePrivateProfileString("DetectParameter", "cameraB_point_width_max", IniWrite, IniPath);

	//GetPrivateProfileStringA("DetectParameter", "cameraB_thread_area_min", "", IniRead, 24, IniPath);
	m_dlgFiberDetectParametersSettingB.GetDlgItem(IDC_EDIT_THREAD_AREA_MIN)->GetWindowTextA(IniRead);
	parameters_setting_trans.cameraB_thread_area_min = atoi(IniRead);
	IniWrite.Format("%s", IniRead);
	WritePrivateProfileString("DetectParameter", "cameraB_thread_area_min", IniWrite, IniPath);

	//GetPrivateProfileStringA("DetectParameter", "cameraB_thread_area_max", "", IniRead, 24, IniPath);
	m_dlgFiberDetectParametersSettingB.GetDlgItem(IDC_EDIT_THREAD_AREA_MAX)->GetWindowTextA(IniRead);
	parameters_setting_trans.cameraB_thread_area_max = atoi(IniRead);
	IniWrite.Format("%s", IniRead);
	WritePrivateProfileString("DetectParameter", "cameraB_thread_area_max", IniWrite, IniPath);

	//GetPrivateProfileStringA("DetectParameter", "cameraB_thread_length_min", "", IniRead, 24, IniPath);
	m_dlgFiberDetectParametersSettingB.GetDlgItem(IDC_EDIT_THREAD_LENGTH_MIN)->GetWindowTextA(IniRead);
	parameters_setting_trans.cameraB_thread_length_min = atoi(IniRead);
	IniWrite.Format("%s", IniRead);
	WritePrivateProfileString("DetectParameter", "cameraB_thread_length_min", IniWrite, IniPath);

	//GetPrivateProfileStringA("DetectParameter", "cameraB_thread_length_max", "", IniRead, 24, IniPath);
	m_dlgFiberDetectParametersSettingB.GetDlgItem(IDC_EDIT_THREAD_LENGTH_MAX)->GetWindowTextA(IniRead);
	parameters_setting_trans.cameraB_thread_length_max = atoi(IniRead);
	IniWrite.Format("%s", IniRead);
	WritePrivateProfileString("DetectParameter", "cameraB_thread_length_max", IniWrite, IniPath);

	//GetPrivateProfileStringA("DetectParameter", "cameraB_thread_width_min", "", IniRead, 24, IniPath);
	m_dlgFiberDetectParametersSettingB.GetDlgItem(IDC_EDIT_THREAD_WIDTH_MIN)->GetWindowTextA(IniRead);
	parameters_setting_trans.cameraB_thread_width_min = atoi(IniRead);
	IniWrite.Format("%s", IniRead);
	WritePrivateProfileString("DetectParameter", "cameraB_thread_width_min", IniWrite, IniPath);

	//GetPrivateProfileStringA("DetectParameter", "cameraB_thread_width_max", "", IniRead, 24, IniPath);
	m_dlgFiberDetectParametersSettingB.GetDlgItem(IDC_EDIT_THREAD_WIDTH_MAX)->GetWindowTextA(IniRead);
	parameters_setting_trans.cameraB_thread_width_max = atoi(IniRead);
	IniWrite.Format("%s", IniRead);
	WritePrivateProfileString("DetectParameter", "cameraB_thread_width_max", IniWrite, IniPath);


	//Camera C detect parameter setting;
	//GetPrivateProfileStringA("DetectParameter", "cameraC_point_area_min", "", IniRead, 24, IniPath);
	m_dlgFiberDetectParametersSettingC.GetDlgItem(IDC_EDIT_POINT_AREA_MIN)->GetWindowTextA(IniRead);
	parameters_setting_trans.cameraC_point_area_min = atoi(IniRead);
	IniWrite.Format("%s", IniRead);
	WritePrivateProfileString("DetectParameter", "cameraC_point_area_min", IniWrite, IniPath);

	//GetPrivateProfileStringA("DetectParameter", "cameraC_point_area_max", "", IniRead, 24, IniPath);
	m_dlgFiberDetectParametersSettingC.GetDlgItem(IDC_EDIT_POINT_AREA_MAX)->GetWindowTextA(IniRead);
	parameters_setting_trans.cameraC_point_area_max = atoi(IniRead);
	IniWrite.Format("%s", IniRead);
	WritePrivateProfileString("DetectParameter", "cameraC_point_area_max", IniWrite, IniPath);

	//GetPrivateProfileStringA("DetectParameter", "cameraC_point_length_min", "", IniRead, 24, IniPath);
	m_dlgFiberDetectParametersSettingC.GetDlgItem(IDC_EDIT_POINT_LENGTH_MIN)->GetWindowTextA(IniRead);
	parameters_setting_trans.cameraC_point_length_min = atoi(IniRead);
	IniWrite.Format("%s", IniRead);
	WritePrivateProfileString("DetectParameter", "cameraC_point_length_min", IniWrite, IniPath);

	//GetPrivateProfileStringA("DetectParameter", "cameraC_point_length_max", "", IniRead, 24, IniPath);
	m_dlgFiberDetectParametersSettingC.GetDlgItem(IDC_EDIT_POINT_LENGTH_MAX)->GetWindowTextA(IniRead);
	parameters_setting_trans.cameraC_point_length_max = atoi(IniRead);
	IniWrite.Format("%s", IniRead);
	WritePrivateProfileString("DetectParameter", "cameraC_point_length_max", IniWrite, IniPath);

	//GetPrivateProfileStringA("DetectParameter", "cameraC_point_width_min", "", IniRead, 24, IniPath);
	m_dlgFiberDetectParametersSettingC.GetDlgItem(IDC_EDIT_POINT_WIDTH_MIN)->GetWindowTextA(IniRead);
	parameters_setting_trans.cameraC_point_width_min = atoi(IniRead);
	IniWrite.Format("%s", IniRead);
	WritePrivateProfileString("DetectParameter", "cameraC_point_width_min", IniWrite, IniPath);

	//GetPrivateProfileStringA("DetectParameter", "cameraC_point_width_max", "", IniRead, 24, IniPath);
	m_dlgFiberDetectParametersSettingC.GetDlgItem(IDC_EDIT_POINT_WIDTH_MAX)->GetWindowTextA(IniRead);
	parameters_setting_trans.cameraC_point_width_max = atoi(IniRead);
	IniWrite.Format("%s", IniRead);
	WritePrivateProfileString("DetectParameter", "cameraC_point_width_max", IniWrite, IniPath);

	//GetPrivateProfileStringA("DetectParameter", "cameraC_thread_area_min", "", IniRead, 24, IniPath);
	m_dlgFiberDetectParametersSettingC.GetDlgItem(IDC_EDIT_THREAD_AREA_MIN)->GetWindowTextA(IniRead);
	parameters_setting_trans.cameraC_thread_area_min = atoi(IniRead);
	IniWrite.Format("%s", IniRead);
	WritePrivateProfileString("DetectParameter", "cameraC_thread_area_min", IniWrite, IniPath);

	//GetPrivateProfileStringA("DetectParameter", "cameraC_thread_area_max", "", IniRead, 24, IniPath);
	m_dlgFiberDetectParametersSettingC.GetDlgItem(IDC_EDIT_THREAD_AREA_MAX)->GetWindowTextA(IniRead);
	parameters_setting_trans.cameraC_thread_area_max = atoi(IniRead);
	IniWrite.Format("%s", IniRead);
	WritePrivateProfileString("DetectParameter", "cameraC_thread_area_max", IniWrite, IniPath);

	//GetPrivateProfileStringA("DetectParameter", "cameraC_thread_length_min", "", IniRead, 24, IniPath);
	m_dlgFiberDetectParametersSettingC.GetDlgItem(IDC_EDIT_THREAD_LENGTH_MIN)->GetWindowTextA(IniRead);
	parameters_setting_trans.cameraC_thread_length_min = atoi(IniRead);
	IniWrite.Format("%s", IniRead);
	WritePrivateProfileString("DetectParameter", "cameraC_thread_length_min", IniWrite, IniPath);

	//GetPrivateProfileStringA("DetectParameter", "cameraC_thread_length_max", "", IniRead, 24, IniPath);
	m_dlgFiberDetectParametersSettingC.GetDlgItem(IDC_EDIT_THREAD_LENGTH_MAX)->GetWindowTextA(IniRead);
	parameters_setting_trans.cameraC_thread_length_max = atoi(IniRead);
	IniWrite.Format("%s", IniRead);
	WritePrivateProfileString("DetectParameter", "cameraC_thread_length_max", IniWrite, IniPath);

	//GetPrivateProfileStringA("DetectParameter", "cameraC_thread_width_min", "", IniRead, 24, IniPath);
	m_dlgFiberDetectParametersSettingC.GetDlgItem(IDC_EDIT_THREAD_WIDTH_MIN)->GetWindowTextA(IniRead);
	parameters_setting_trans.cameraC_thread_width_min = atoi(IniRead);
	IniWrite.Format("%s", IniRead);
	WritePrivateProfileString("DetectParameter", "cameraC_thread_width_min", IniWrite, IniPath);

	//GetPrivateProfileStringA("DetectParameter", "cameraC_thread_width_max", "", IniRead, 24, IniPath);
	m_dlgFiberDetectParametersSettingC.GetDlgItem(IDC_EDIT_THREAD_WIDTH_MAX)->GetWindowTextA(IniRead);
	parameters_setting_trans.cameraC_thread_width_max = atoi(IniRead);
	IniWrite.Format("%s", IniRead);
	WritePrivateProfileString("DetectParameter", "cameraC_thread_width_max", IniWrite, IniPath);


	//Camera D detect parameter setting;
	//GetPrivateProfileStringA("DetectParameter", "cameraD_point_area_min", "", IniRead, 24, IniPath);
	m_dlgFiberDetectParametersSettingD.GetDlgItem(IDC_EDIT_POINT_AREA_MIN)->GetWindowTextA(IniRead);
	parameters_setting_trans.cameraD_point_area_min = atoi(IniRead);
	IniWrite.Format("%s", IniRead);
	WritePrivateProfileString("DetectParameter", "cameraD_point_area_min", IniWrite, IniPath);

	//GetPrivateProfileStringA("DetectParameter", "cameraD_point_area_max", "", IniRead, 24, IniPath);
	m_dlgFiberDetectParametersSettingD.GetDlgItem(IDC_EDIT_POINT_AREA_MAX)->GetWindowTextA(IniRead);
	parameters_setting_trans.cameraD_point_area_max = atoi(IniRead);
	IniWrite.Format("%s", IniRead);
	WritePrivateProfileString("DetectParameter", "cameraD_point_area_max", IniWrite, IniPath);

	//GetPrivateProfileStringA("DetectParameter", "cameraD_point_length_min", "", IniRead, 24, IniPath);
	m_dlgFiberDetectParametersSettingD.GetDlgItem(IDC_EDIT_POINT_LENGTH_MIN)->GetWindowTextA(IniRead);
	parameters_setting_trans.cameraD_point_length_min = atoi(IniRead);
	IniWrite.Format("%s", IniRead);
	WritePrivateProfileString("DetectParameter", "cameraD_point_length_min", IniWrite, IniPath);

	//GetPrivateProfileStringA("DetectParameter", "cameraD_point_length_max", "", IniRead, 24, IniPath);
	m_dlgFiberDetectParametersSettingD.GetDlgItem(IDC_EDIT_POINT_LENGTH_MAX)->GetWindowTextA(IniRead);
	parameters_setting_trans.cameraD_point_length_max = atoi(IniRead);
	IniWrite.Format("%s", IniRead);
	WritePrivateProfileString("DetectParameter", "cameraD_point_length_max", IniWrite, IniPath);

	//GetPrivateProfileStringA("DetectParameter", "cameraD_point_width_min", "", IniRead, 24, IniPath);
	m_dlgFiberDetectParametersSettingD.GetDlgItem(IDC_EDIT_POINT_WIDTH_MIN)->GetWindowTextA(IniRead);
	parameters_setting_trans.cameraD_point_width_min = atoi(IniRead);
	IniWrite.Format("%s", IniRead);
	WritePrivateProfileString("DetectParameter", "cameraD_point_width_min", IniWrite, IniPath);

	//GetPrivateProfileStringA("DetectParameter", "cameraD_point_width_max", "", IniRead, 24, IniPath);
	m_dlgFiberDetectParametersSettingD.GetDlgItem(IDC_EDIT_POINT_WIDTH_MAX)->GetWindowTextA(IniRead);
	parameters_setting_trans.cameraD_point_width_max = atoi(IniRead);
	IniWrite.Format("%s", IniRead);
	WritePrivateProfileString("DetectParameter", "cameraD_point_width_max", IniWrite, IniPath);

	//GetPrivateProfileStringA("DetectParameter", "cameraD_thread_area_min", "", IniRead, 24, IniPath);
	m_dlgFiberDetectParametersSettingD.GetDlgItem(IDC_EDIT_THREAD_AREA_MIN)->GetWindowTextA(IniRead);
	parameters_setting_trans.cameraD_thread_area_min = atoi(IniRead);
	IniWrite.Format("%s", IniRead);
	WritePrivateProfileString("DetectParameter", "cameraD_thread_area_min", IniWrite, IniPath);

	//GetPrivateProfileStringA("DetectParameter", "cameraD_thread_area_max", "", IniRead, 24, IniPath);
	m_dlgFiberDetectParametersSettingD.GetDlgItem(IDC_EDIT_THREAD_AREA_MAX)->GetWindowTextA(IniRead);
	parameters_setting_trans.cameraD_thread_area_max = atoi(IniRead);
	IniWrite.Format("%s", IniRead);
	WritePrivateProfileString("DetectParameter", "cameraD_thread_area_max", IniWrite, IniPath);

	//GetPrivateProfileStringA("DetectParameter", "cameraD_thread_length_min", "", IniRead, 24, IniPath);
	m_dlgFiberDetectParametersSettingD.GetDlgItem(IDC_EDIT_THREAD_LENGTH_MIN)->GetWindowTextA(IniRead);
	parameters_setting_trans.cameraD_thread_length_min = atoi(IniRead);
	IniWrite.Format("%s", IniRead);
	WritePrivateProfileString("DetectParameter", "cameraD_thread_length_min", IniWrite, IniPath);

	//GetPrivateProfileStringA("DetectParameter", "cameraD_thread_length_max", "", IniRead, 24, IniPath);
	m_dlgFiberDetectParametersSettingD.GetDlgItem(IDC_EDIT_THREAD_LENGTH_MAX)->GetWindowTextA(IniRead);
	parameters_setting_trans.cameraD_thread_length_max = atoi(IniRead);
	IniWrite.Format("%s", IniRead);
	WritePrivateProfileString("DetectParameter", "cameraD_thread_length_max", IniWrite, IniPath);

	//GetPrivateProfileStringA("DetectParameter", "cameraD_thread_width_min", "", IniRead, 24, IniPath);
	m_dlgFiberDetectParametersSettingD.GetDlgItem(IDC_EDIT_THREAD_WIDTH_MIN)->GetWindowTextA(IniRead);
	parameters_setting_trans.cameraD_thread_width_min = atoi(IniRead);
	IniWrite.Format("%s", IniRead);
	WritePrivateProfileString("DetectParameter", "cameraD_thread_width_min", IniWrite, IniPath);

	//GetPrivateProfileStringA("DetectParameter", "cameraD_thread_width_max", "", IniRead, 24, IniPath);
	m_dlgFiberDetectParametersSettingD.GetDlgItem(IDC_EDIT_THREAD_WIDTH_MAX)->GetWindowTextA(IniRead);
	parameters_setting_trans.cameraD_thread_width_max = atoi(IniRead);
	IniWrite.Format("%s", IniRead);
	WritePrivateProfileString("DetectParameter", "cameraD_thread_width_max", IniWrite, IniPath);


	//DebugParaneters setting;
	//GetPrivateProfileStringA("DebugParameter", "debug_cameraA", "", IniRead, 24, IniPath);
	parameters_setting_trans.debug_cameraA=m_dlgDebugParametersSetting.m_checkDebugCameraA.GetCheck();
	IniWrite.Format("%d", parameters_setting_trans.debug_cameraA);
	WritePrivateProfileString("DebugParameter", "debug_cameraA", IniWrite, IniPath);
	

	//GetPrivateProfileStringA("DebugParameter", "debug_cameraB", "", IniRead, 24, IniPath);
	parameters_setting_trans.debug_cameraB=m_dlgDebugParametersSetting.m_checkDebugCameraB.GetCheck();
	IniWrite.Format("%d", parameters_setting_trans.debug_cameraB);
	WritePrivateProfileString("DebugParameter", "debug_cameraB", IniWrite, IniPath);

	//GetPrivateProfileStringA("DebugParameter", "debug_cameraC", "", IniRead, 24, IniPath);
	parameters_setting_trans.debug_cameraC=m_dlgDebugParametersSetting.m_checkDebugCameraC.GetCheck();
	IniWrite.Format("%d", parameters_setting_trans.debug_cameraC);
	WritePrivateProfileString("DebugParameter", "debug_cameraC", IniWrite, IniPath);

	//GetPrivateProfileStringA("DebugParameter", "debug_cameraD", "", IniRead, 24, IniPath);
	parameters_setting_trans.debug_cameraD=m_dlgDebugParametersSetting.m_checkDebugCameraD.GetCheck();
	IniWrite.Format("%d", parameters_setting_trans.debug_cameraD);
	WritePrivateProfileString("DebugParameter", "debug_cameraD", IniWrite, IniPath);

	//GetPrivateProfileStringA("DebugParameter", "stop_volve", "", IniRead, 24, IniPath);
	parameters_setting_trans.stop_volve=m_dlgDebugParametersSetting.m_checkStopValve.GetCheck();
	IniWrite.Format("%d", parameters_setting_trans.stop_volve);
	WritePrivateProfileString("DebugParameter", "stop_volve", IniWrite, IniPath);

	//GetPrivateProfileStringA("DebugParameter", "save_blob", "", IniRead, 24, IniPath);
	parameters_setting_trans.save_blob = m_dlgDebugParametersSetting.m_checkSaveBolb.GetCheck();
	IniWrite.Format("%d", parameters_setting_trans.save_blob);
	WritePrivateProfileString("DebugParameter", "save_blob", IniWrite, IniPath);

	return;
}


void CDetectParametersSetting::OnBnClickedDialogSendMessage()
{
	// TODO:  在此添加控件通知处理程序代码
	WriteIni();

	int nSelect = m_cTabControl.GetCurSel()+1;

	Vsee::VCameraMessageTrans *frame = new Vsee::VCameraMessageTrans;
	parameters_setting_trans.signalling = nSelect;
	memcpy(&frame->parameters_setting_trans, &parameters_setting_trans, sizeof(parameters_setting_trans));
	frame->signalling = 4;
	::PostMessage(AfxGetMainWnd()->m_hWnd, ID_SHOWCTRLMESSAGE, frame->signalling, (LPARAM)frame);
	//CDialogEx::OnOK();
}
