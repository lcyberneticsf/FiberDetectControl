// CameraSettingDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "FiberDetectControl.h"
#include "CameraSettingDlg.h"
#include "afxdialogex.h"
#include  "VCameraMessageTrans.h"
#include "FiberDetectControlDlg.h"


// CCameraSettingDlg 对话框

IMPLEMENT_DYNAMIC(CCameraSettingDlg, CDialogEx)

CCameraSettingDlg::CCameraSettingDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CCameraSettingDlg::IDD, pParent)
{

}

CCameraSettingDlg::~CCameraSettingDlg()
{
}

void CCameraSettingDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CCameraSettingDlg, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON_CAMERA1_NET, &CCameraSettingDlg::OnBnClickedButtonCamera1Net)
	ON_BN_CLICKED(IDC_BUTTON_OPEN_CAMERA_A, &CCameraSettingDlg::OnBnClickedButtonOpenCameraA)
	ON_BN_CLICKED(IDC_BUTTON_STOP_CAMERA_A, &CCameraSettingDlg::OnBnClickedButtonStopCameraA)
	ON_BN_CLICKED(IDC_BUTTON_START_GRAB_A, &CCameraSettingDlg::OnBnClickedButtonStartGrabA)
	ON_BN_CLICKED(IDC_BUTTON_STOP_GRAB_A, &CCameraSettingDlg::OnBnClickedButtonStopGrabA)
	ON_BN_CLICKED(IDC_BUTTON_START_GRAB_B, &CCameraSettingDlg::OnBnClickedButtonStartGrabB)
	ON_BN_CLICKED(IDC_BUTTON_START_GRAB_C, &CCameraSettingDlg::OnBnClickedButtonStartGrabC)
	ON_BN_CLICKED(IDC_BUTTON_START_GRAB_D, &CCameraSettingDlg::OnBnClickedButtonStartGrabD)
	ON_BN_CLICKED(IDC_BUTTON_STOP_GRAB_B, &CCameraSettingDlg::OnBnClickedButtonStopGrabB)
	ON_BN_CLICKED(IDC_BUTTON_STOP_GRAB_C, &CCameraSettingDlg::OnBnClickedButtonStopGrabC)
	ON_BN_CLICKED(IDC_BUTTON_STOP_GRAB_D, &CCameraSettingDlg::OnBnClickedButtonStopGrabD)
	ON_BN_CLICKED(IDC_BUTTON_CAMERA2_NET, &CCameraSettingDlg::OnBnClickedButtonCamera2Net)
	ON_BN_CLICKED(IDC_BUTTON_OPEN_CAMERA_B, &CCameraSettingDlg::OnBnClickedButtonOpenCameraB)
	ON_BN_CLICKED(IDC_BUTTON_STOP_CAMERA_B, &CCameraSettingDlg::OnBnClickedButtonStopCameraB)
	ON_BN_CLICKED(IDC_BUTTON_CAMERA3_NET, &CCameraSettingDlg::OnBnClickedButtonCamera3Net)
	ON_BN_CLICKED(IDC_BUTTON_OPEN_CAMERA_C, &CCameraSettingDlg::OnBnClickedButtonOpenCameraC)
	ON_BN_CLICKED(IDC_BUTTON_STOP_CAMERA_C, &CCameraSettingDlg::OnBnClickedButtonStopCameraC)
	ON_BN_CLICKED(IDC_BUTTON_CAMERA4_NET, &CCameraSettingDlg::OnBnClickedButtonCamera4Net)
	ON_BN_CLICKED(IDC_BUTTON_OPEN_CAMERA_D, &CCameraSettingDlg::OnBnClickedButtonOpenCameraD)
	ON_BN_CLICKED(IDC_BUTTON_STOP_CAMERA_D, &CCameraSettingDlg::OnBnClickedButtonStopCameraD)
END_MESSAGE_MAP()


// CCameraSettingDlg 消息处理程序
/*************************************************************
功能：获得当前程序的运行目录(.exe)
**************************************************************/
CString CCameraSettingDlg::GetModulePath(void)
{
	CString    sPath;
	GetModuleFileNameA(NULL, sPath.GetBufferSetLength(MAX_PATH + 1), MAX_PATH);
	sPath.ReleaseBuffer();
	int    nPos;
	nPos = sPath.ReverseFind('\\');
	sPath = sPath.Left(nPos);
	return   sPath;
}

void CCameraSettingDlg::ReadIni()
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

	return;
}

void CCameraSettingDlg::OnBnClickedButtonCamera1Net()
{
	// TODO:  在此添加控件通知处理程序代码
	int camera_total;
	char IniRead[255];
	memset(IniRead, 0, 255);
	char IniPath[255];
	memset(IniPath, 0, 255);
	//camera_ip_info ci_info;
	CString strPath = GetModulePath();
	strcpy_s(IniPath, 255, strPath.GetBuffer());
	strcat_s(IniPath, "/param.ini");

	GetPrivateProfileStringA("CameraSetting", "cameraA_ip", "", IniRead, 24, IniPath);
	//m_dlgNetworkParameters.GetDlgItem(IDC_EDIT_IP_ADRESS)->SetWindowTextA(IniRead);
	m_dlgCameraParametersSettting.m_strNetString = IniRead;

	GetPrivateProfileStringA("CameraSetting", "cameraA_port", "", IniRead, 24, IniPath);
	m_dlgCameraParametersSettting.m_u32NetPort = atoi(IniRead);

	GetPrivateProfileStringA("CameraSetting", "cameraA_scan_speed", "", IniRead, 24, IniPath);
	m_dlgCameraParametersSettting.m_nScanSpeed = atoi(IniRead);

	GetPrivateProfileStringA("CameraSetting", "cameraA_tirgging_mode", "", IniRead, 24, IniPath);
	m_dlgCameraParametersSettting.m_nTriggerMode = atoi(IniRead);

	GetPrivateProfileStringA("CameraSetting", "cameraA_exposure_time", "", IniRead, 24, IniPath);
	m_dlgCameraParametersSettting.m_nExposureTime = atoi(IniRead);

	GetPrivateProfileStringA("CameraSetting", "cameraA_gain", "", IniRead, 24, IniPath);
	m_dlgCameraParametersSettting.m_nGain = atoi(IniRead);

	GetPrivateProfileStringA("CameraSetting", "cameraA_width", "", IniRead, 24, IniPath);
	m_dlgCameraParametersSettting.m_nImageWidth = atoi(IniRead);

	GetPrivateProfileStringA("CameraSetting", "cameraA_height", "", IniRead, 24, IniPath);
	m_dlgCameraParametersSettting.m_nImageHeight = atoi(IniRead);

	GetPrivateProfileStringA("CameraSetting", "cameraA_mac", "", IniRead, 24, IniPath);
	m_dlgCameraParametersSettting.m_strMacAddress = IniRead;

	m_dlgCameraParametersSettting.title = "相机A参数设置";

	if (m_dlgCameraParametersSettting.DoModal() != IDOK)
		return;

	cameraA_ip = m_dlgCameraParametersSettting.m_strNetString;
	cameraA_port = m_dlgCameraParametersSettting.m_u32NetPort;
	cameraA_scan_speed = m_dlgCameraParametersSettting.m_nScanSpeed;
	cameraA_tirgging_mode = m_dlgCameraParametersSettting.m_nTriggerMode;
	cameraA_exposure_time = m_dlgCameraParametersSettting.m_nExposureTime;
	cameraA_gain = m_dlgCameraParametersSettting.m_nGain;
	cameraA_width = m_dlgCameraParametersSettting.m_nImageWidth;
	cameraA_height = m_dlgCameraParametersSettting.m_nImageHeight;
	cameraA_mac = m_dlgCameraParametersSettting.m_strMacAddress;

	CString IniWrite;
	IniWrite.Format("%s", cameraA_ip.c_str());
	WritePrivateProfileString("CameraSetting", "cameraA_ip", IniWrite, IniPath);

	IniWrite.Format("%d", cameraA_port);
	WritePrivateProfileString("CameraSetting", "cameraA_port", IniWrite, IniPath);

	IniWrite.Format("%d", cameraA_scan_speed);
	WritePrivateProfileString("CameraSetting", "cameraA_scan_speed", IniWrite, IniPath);

	IniWrite.Format("%d", cameraA_tirgging_mode);
	WritePrivateProfileString("CameraSetting", "cameraA_tirgging_mode", IniWrite, IniPath);

	IniWrite.Format("%d", cameraA_exposure_time);
	WritePrivateProfileString("CameraSetting", "cameraA_exposure_time", IniWrite, IniPath);

	IniWrite.Format("%d", cameraA_gain);
	WritePrivateProfileString("CameraSetting", "cameraA_gain", IniWrite, IniPath);

	IniWrite.Format("%d", cameraA_width);
	WritePrivateProfileString("CameraSetting", "cameraA_width", IniWrite, IniPath);

	IniWrite.Format("%d", cameraA_height);
	WritePrivateProfileString("CameraSetting", "cameraA_height", IniWrite, IniPath);

	IniWrite.Format("%s", cameraA_mac.c_str());
	WritePrivateProfileString("CameraSetting", "cameraA_mac", IniWrite, IniPath);
	ReadIni();

	Vsee::VCameraMessageTrans *frame = new Vsee::VCameraMessageTrans;
	parameters_setting_trans.signalling = 5;
	memcpy(&frame->parameters_setting_trans, &parameters_setting_trans, sizeof(parameters_setting_trans));
	frame->magnetic_valve_num = 1;
	frame->signalling = 5;
	//::PostMessage(AfxGetMainWnd()->m_hWnd, ID_SHOWCTRLMESSAGE, frame->signalling, (LPARAM)frame);//CFiberDetectControlDlg
	::PostMessage(CFiberDetectControlDlg::m_hWnd, ID_SHOWCTRLMESSAGE, frame->signalling, (LPARAM)frame);//CFiberDetectControlDlg

}


void CCameraSettingDlg::OnBnClickedButtonOpenCameraA()
{
	// TODO:  在此添加控件通知处理程序代码
	Vsee::VCameraMessageTrans *frame = new Vsee::VCameraMessageTrans;
	parameters_setting_trans.signalling = 11;
	memcpy(&frame->parameters_setting_trans, &parameters_setting_trans, sizeof(parameters_setting_trans));
	frame->signalling = 11;
	frame->magnetic_valve_num = 1;
	frame->width = 4096;
	frame->height = 200;
	//::PostMessage(AfxGetMainWnd()->m_hWnd, ID_SHOWCTRLMESSAGE, frame->signalling, (LPARAM)frame);
	::PostMessage(CFiberDetectControlDlg::m_hWnd, ID_SHOWCTRLMESSAGE, frame->signalling, (LPARAM)frame);
}


void CCameraSettingDlg::OnBnClickedButtonStopCameraA()
{
	// TODO:  在此添加控件通知处理程序代码
	Vsee::VCameraMessageTrans *frame = new Vsee::VCameraMessageTrans;
	parameters_setting_trans.signalling = 12;
	memcpy(&frame->parameters_setting_trans, &parameters_setting_trans, sizeof(parameters_setting_trans));
	frame->signalling = 12;
	frame->magnetic_valve_num = 1;  //To indicate camera serial number;
	//::PostMessage(AfxGetMainWnd()->m_hWnd, ID_SHOWCTRLMESSAGE, frame->signalling, (LPARAM)frame);
	::PostMessage(CFiberDetectControlDlg::m_hWnd, ID_SHOWCTRLMESSAGE, frame->signalling, (LPARAM)frame);
}


void CCameraSettingDlg::OnBnClickedButtonStartGrabA()
{
	// TODO:  在此添加控件通知处理程序代码
	Vsee::VCameraMessageTrans *frame = new Vsee::VCameraMessageTrans;
	parameters_setting_trans.signalling = 13;
	memcpy(&frame->parameters_setting_trans, &parameters_setting_trans, sizeof(parameters_setting_trans));
	frame->signalling = 13;
	frame->magnetic_valve_num = 1;
	frame->width = 4096;
	frame->height = 200;
	//::PostMessage(AfxGetMainWnd()->m_hWnd, ID_SHOWCTRLMESSAGE, frame->signalling, (LPARAM)frame);
	::PostMessage(CFiberDetectControlDlg::m_hWnd, ID_SHOWCTRLMESSAGE, frame->signalling, (LPARAM)frame);
}


void CCameraSettingDlg::OnBnClickedButtonStopGrabA()
{
	// TODO:  在此添加控件通知处理程序代码
	Vsee::VCameraMessageTrans *frame = new Vsee::VCameraMessageTrans;
	parameters_setting_trans.signalling = 14;
	memcpy(&frame->parameters_setting_trans, &parameters_setting_trans, sizeof(parameters_setting_trans));
	frame->signalling = 14;
	frame->magnetic_valve_num = 1;
	frame->width = 4096;
	frame->height = 200;
	//::PostMessage(AfxGetMainWnd()->m_hWnd, ID_SHOWCTRLMESSAGE, frame->signalling, (LPARAM)frame);
	::PostMessage(CFiberDetectControlDlg::m_hWnd, ID_SHOWCTRLMESSAGE, frame->signalling, (LPARAM)frame);
}


void CCameraSettingDlg::OnBnClickedButtonStartGrabB()
{
	// TODO:  在此添加控件通知处理程序代码
	Vsee::VCameraMessageTrans *frame = new Vsee::VCameraMessageTrans;
	parameters_setting_trans.signalling = 13;
	memcpy(&frame->parameters_setting_trans, &parameters_setting_trans, sizeof(parameters_setting_trans));
	frame->signalling = 13;
	frame->magnetic_valve_num = 2;
	frame->width = 4096;
	frame->height = 200;
	//::PostMessage(AfxGetMainWnd()->m_hWnd, ID_SHOWCTRLMESSAGE, frame->signalling, (LPARAM)frame);
	::PostMessage(CFiberDetectControlDlg::m_hWnd, ID_SHOWCTRLMESSAGE, frame->signalling, (LPARAM)frame);
}


void CCameraSettingDlg::OnBnClickedButtonStartGrabC()
{
	// TODO:  在此添加控件通知处理程序代码
	Vsee::VCameraMessageTrans *frame = new Vsee::VCameraMessageTrans;
	parameters_setting_trans.signalling = 13;
	memcpy(&frame->parameters_setting_trans, &parameters_setting_trans, sizeof(parameters_setting_trans));
	frame->signalling = 13;
	frame->magnetic_valve_num = 3;
	frame->width = 4096;
	frame->height = 200;
	//::PostMessage(AfxGetMainWnd()->m_hWnd, ID_SHOWCTRLMESSAGE, frame->signalling, (LPARAM)frame);
	::PostMessage(CFiberDetectControlDlg::m_hWnd, ID_SHOWCTRLMESSAGE, frame->signalling, (LPARAM)frame);
}


void CCameraSettingDlg::OnBnClickedButtonStartGrabD()
{
	// TODO:  在此添加控件通知处理程序代码
	Vsee::VCameraMessageTrans *frame = new Vsee::VCameraMessageTrans;
	parameters_setting_trans.signalling = 13;
	memcpy(&frame->parameters_setting_trans, &parameters_setting_trans, sizeof(parameters_setting_trans));
	frame->signalling = 13;
	frame->magnetic_valve_num = 4;
	frame->width = 4096;
	frame->height = 200;
	//::PostMessage(AfxGetMainWnd()->m_hWnd, ID_SHOWCTRLMESSAGE, frame->signalling, (LPARAM)frame);
	::PostMessage(CFiberDetectControlDlg::m_hWnd, ID_SHOWCTRLMESSAGE, frame->signalling, (LPARAM)frame);
}


void CCameraSettingDlg::OnBnClickedButtonStopGrabB()
{
	// TODO:  在此添加控件通知处理程序代码
	Vsee::VCameraMessageTrans *frame = new Vsee::VCameraMessageTrans;
	parameters_setting_trans.signalling = 14;
	memcpy(&frame->parameters_setting_trans, &parameters_setting_trans, sizeof(parameters_setting_trans));
	frame->signalling = 14;
	frame->magnetic_valve_num = 2;
	frame->width = 4096;
	frame->height = 200;
	//::PostMessage(AfxGetMainWnd()->m_hWnd, ID_SHOWCTRLMESSAGE, frame->signalling, (LPARAM)frame);
	::PostMessage(CFiberDetectControlDlg::m_hWnd, ID_SHOWCTRLMESSAGE, frame->signalling, (LPARAM)frame);
}


void CCameraSettingDlg::OnBnClickedButtonStopGrabC()
{
	// TODO:  在此添加控件通知处理程序代码
	Vsee::VCameraMessageTrans *frame = new Vsee::VCameraMessageTrans;
	parameters_setting_trans.signalling = 14;
	memcpy(&frame->parameters_setting_trans, &parameters_setting_trans, sizeof(parameters_setting_trans));
	frame->signalling = 14;
	frame->magnetic_valve_num = 3;
	frame->width = 4096;
	frame->height = 200;
	::PostMessage(CFiberDetectControlDlg::m_hWnd, ID_SHOWCTRLMESSAGE, frame->signalling, (LPARAM)frame);
}


void CCameraSettingDlg::OnBnClickedButtonStopGrabD()
{
	// TODO:  在此添加控件通知处理程序代码
	Vsee::VCameraMessageTrans *frame = new Vsee::VCameraMessageTrans;
	parameters_setting_trans.signalling = 14;
	memcpy(&frame->parameters_setting_trans, &parameters_setting_trans, sizeof(parameters_setting_trans));
	frame->signalling = 14;
	frame->magnetic_valve_num = 4;
	frame->width = 4096;
	frame->height = 200;
	::PostMessage(CFiberDetectControlDlg::m_hWnd, ID_SHOWCTRLMESSAGE, frame->signalling, (LPARAM)frame);
}


void CCameraSettingDlg::OnBnClickedButtonCamera2Net()
{
	// TODO:  在此添加控件通知处理程序代码
	int camera_total;
	char IniRead[255];
	memset(IniRead, 0, 255);
	char IniPath[255];
	memset(IniPath, 0, 255);
	//camera_ip_info ci_info;
	CString strPath = GetModulePath();
	strcpy_s(IniPath, 255, strPath.GetBuffer());
	strcat_s(IniPath, "/param.ini");

	GetPrivateProfileStringA("CameraSetting", "cameraB_ip", "", IniRead, 24, IniPath);
	//m_dlgNetworkParameters.GetDlgItem(IDC_EDIT_IP_ADRESS)->SetWindowTextA(IniRead);
	m_dlgCameraParametersSettting.m_strNetString = IniRead;

	GetPrivateProfileStringA("CameraSetting", "cameraB_port", "", IniRead, 24, IniPath);
	m_dlgCameraParametersSettting.m_u32NetPort = atoi(IniRead);

	GetPrivateProfileStringA("CameraSetting", "cameraB_scan_speed", "", IniRead, 24, IniPath);
	m_dlgCameraParametersSettting.m_nScanSpeed = atoi(IniRead);

	GetPrivateProfileStringA("CameraSetting", "cameraB_tirgging_mode", "", IniRead, 24, IniPath);
	m_dlgCameraParametersSettting.m_nTriggerMode = atoi(IniRead);

	GetPrivateProfileStringA("CameraSetting", "cameraB_exposure_time", "", IniRead, 24, IniPath);
	m_dlgCameraParametersSettting.m_nExposureTime = atoi(IniRead);

	GetPrivateProfileStringA("CameraSetting", "cameraB_gain", "", IniRead, 24, IniPath);
	m_dlgCameraParametersSettting.m_nGain = atoi(IniRead);

	GetPrivateProfileStringA("CameraSetting", "cameraB_width", "", IniRead, 24, IniPath);
	m_dlgCameraParametersSettting.m_nImageWidth = atoi(IniRead);

	GetPrivateProfileStringA("CameraSetting", "cameraB_height", "", IniRead, 24, IniPath);
	m_dlgCameraParametersSettting.m_nImageHeight = atoi(IniRead);

	GetPrivateProfileStringA("CameraSetting", "cameraB_mac", "", IniRead, 24, IniPath);
	m_dlgCameraParametersSettting.m_strMacAddress = IniRead;

	m_dlgCameraParametersSettting.title = "相机B参数设置";

	if (m_dlgCameraParametersSettting.DoModal() != IDOK)
		return;

	cameraB_ip = m_dlgCameraParametersSettting.m_strNetString;
	cameraB_port = m_dlgCameraParametersSettting.m_u32NetPort;
	cameraB_scan_speed = m_dlgCameraParametersSettting.m_nScanSpeed;
	cameraB_tirgging_mode = m_dlgCameraParametersSettting.m_nTriggerMode;
	cameraB_exposure_time = m_dlgCameraParametersSettting.m_nExposureTime;
	cameraB_gain = m_dlgCameraParametersSettting.m_nGain;
	cameraB_width = m_dlgCameraParametersSettting.m_nImageWidth;
	cameraB_height = m_dlgCameraParametersSettting.m_nImageHeight;
	cameraB_mac = m_dlgCameraParametersSettting.m_strMacAddress;

	CString IniWrite;
	IniWrite.Format("%s", cameraB_ip.c_str());
	WritePrivateProfileString("CameraSetting", "cameraB_ip", IniWrite, IniPath);

	IniWrite.Format("%d", cameraB_port);
	WritePrivateProfileString("CameraSetting", "cameraB_port", IniWrite, IniPath);

	IniWrite.Format("%d", cameraB_scan_speed);
	WritePrivateProfileString("CameraSetting", "cameraB_scan_speed", IniWrite, IniPath);

	IniWrite.Format("%d", cameraB_tirgging_mode);
	WritePrivateProfileString("CameraSetting", "cameraB_tirgging_mode", IniWrite, IniPath);

	IniWrite.Format("%d", cameraB_exposure_time);
	WritePrivateProfileString("CameraSetting", "cameraB_exposure_time", IniWrite, IniPath);

	IniWrite.Format("%d", cameraB_gain);
	WritePrivateProfileString("CameraSetting", "cameraB_gain", IniWrite, IniPath);

	IniWrite.Format("%d", cameraB_width);
	WritePrivateProfileString("CameraSetting", "cameraB_width", IniWrite, IniPath);

	IniWrite.Format("%d", cameraB_height);
	WritePrivateProfileString("CameraSetting", "cameraB_height", IniWrite, IniPath);

	IniWrite.Format("%s", cameraB_mac.c_str());
	WritePrivateProfileString("CameraSetting", "cameraB_mac", IniWrite, IniPath);
	ReadIni();

	Vsee::VCameraMessageTrans *frame = new Vsee::VCameraMessageTrans;
	parameters_setting_trans.signalling = 5;
	memcpy(&frame->parameters_setting_trans, &parameters_setting_trans, sizeof(parameters_setting_trans));
	frame->magnetic_valve_num = 2;     //The camera B serial number is 2; 
	frame->signalling = 5;
	::PostMessage(CFiberDetectControlDlg::m_hWnd, ID_SHOWCTRLMESSAGE, frame->signalling, (LPARAM)frame);
}


void CCameraSettingDlg::OnBnClickedButtonOpenCameraB()
{
	// TODO:  在此添加控件通知处理程序代码
	Vsee::VCameraMessageTrans *frame = new Vsee::VCameraMessageTrans;
	parameters_setting_trans.signalling = 11;
	memcpy(&frame->parameters_setting_trans, &parameters_setting_trans, sizeof(parameters_setting_trans));
	frame->signalling = 11;
	frame->magnetic_valve_num = 2;
	frame->width = 4096;
	frame->height = 200;
	::PostMessage(CFiberDetectControlDlg::m_hWnd, ID_SHOWCTRLMESSAGE, frame->signalling, (LPARAM)frame);
}


void CCameraSettingDlg::OnBnClickedButtonStopCameraB()
{
	// TODO:  在此添加控件通知处理程序代码
	Vsee::VCameraMessageTrans *frame = new Vsee::VCameraMessageTrans;
	parameters_setting_trans.signalling = 12;
	memcpy(&frame->parameters_setting_trans, &parameters_setting_trans, sizeof(parameters_setting_trans));
	frame->signalling = 12;
	frame->magnetic_valve_num = 2;  //To indicate camera serial number;
	::PostMessage(CFiberDetectControlDlg::m_hWnd, ID_SHOWCTRLMESSAGE, frame->signalling, (LPARAM)frame);
}


void CCameraSettingDlg::OnBnClickedButtonCamera3Net()
{
	// TODO:  在此添加控件通知处理程序代码
	int camera_total;
	char IniRead[255];
	memset(IniRead, 0, 255);
	char IniPath[255];
	memset(IniPath, 0, 255);
	//camera_ip_info ci_info;
	CString strPath = GetModulePath();
	strcpy_s(IniPath, 255, strPath.GetBuffer());
	strcat_s(IniPath, "/param.ini");

	GetPrivateProfileStringA("CameraSetting", "cameraC_ip", "", IniRead, 24, IniPath);
	//m_dlgNetworkParameters.GetDlgItem(IDC_EDIT_IP_ADRESS)->SetWindowTextA(IniRead);
	m_dlgCameraParametersSettting.m_strNetString = IniRead;

	GetPrivateProfileStringA("CameraSetting", "cameraC_port", "", IniRead, 24, IniPath);
	m_dlgCameraParametersSettting.m_u32NetPort = atoi(IniRead);

	GetPrivateProfileStringA("CameraSetting", "cameraC_scan_speed", "", IniRead, 24, IniPath);
	m_dlgCameraParametersSettting.m_nScanSpeed = atoi(IniRead);

	GetPrivateProfileStringA("CameraSetting", "cameraC_tirgging_mode", "", IniRead, 24, IniPath);
	m_dlgCameraParametersSettting.m_nTriggerMode = atoi(IniRead);

	GetPrivateProfileStringA("CameraSetting", "cameraC_exposure_time", "", IniRead, 24, IniPath);
	m_dlgCameraParametersSettting.m_nExposureTime = atoi(IniRead);

	GetPrivateProfileStringA("CameraSetting", "cameraC_gain", "", IniRead, 24, IniPath);
	m_dlgCameraParametersSettting.m_nGain = atoi(IniRead);

	GetPrivateProfileStringA("CameraSetting", "cameraC_width", "", IniRead, 24, IniPath);
	m_dlgCameraParametersSettting.m_nImageWidth = atoi(IniRead);

	GetPrivateProfileStringA("CameraSetting", "cameraC_height", "", IniRead, 24, IniPath);
	m_dlgCameraParametersSettting.m_nImageHeight = atoi(IniRead);

	GetPrivateProfileStringA("CameraSetting", "cameraC_mac", "", IniRead, 24, IniPath);
	m_dlgCameraParametersSettting.m_strMacAddress = IniRead;

	m_dlgCameraParametersSettting.title = "相机C参数设置";

	if (m_dlgCameraParametersSettting.DoModal() != IDOK)
		return;

	cameraC_ip = m_dlgCameraParametersSettting.m_strNetString;
	cameraC_port = m_dlgCameraParametersSettting.m_u32NetPort;
	cameraC_scan_speed = m_dlgCameraParametersSettting.m_nScanSpeed;
	cameraC_tirgging_mode = m_dlgCameraParametersSettting.m_nTriggerMode;
	cameraC_exposure_time = m_dlgCameraParametersSettting.m_nExposureTime;
	cameraC_gain = m_dlgCameraParametersSettting.m_nGain;
	cameraC_width = m_dlgCameraParametersSettting.m_nImageWidth;
	cameraC_height = m_dlgCameraParametersSettting.m_nImageHeight;
	cameraC_mac = m_dlgCameraParametersSettting.m_strMacAddress;

	CString IniWrite;
	IniWrite.Format("%s", cameraC_ip.c_str());
	WritePrivateProfileString("CameraSetting", "cameraC_ip", IniWrite, IniPath);

	IniWrite.Format("%d", cameraC_port);
	WritePrivateProfileString("CameraSetting", "cameraC_port", IniWrite, IniPath);

	IniWrite.Format("%d", cameraC_scan_speed);
	WritePrivateProfileString("CameraSetting", "cameraC_scan_speed", IniWrite, IniPath);

	IniWrite.Format("%d", cameraC_tirgging_mode);
	WritePrivateProfileString("CameraSetting", "cameraC_tirgging_mode", IniWrite, IniPath);

	IniWrite.Format("%d", cameraC_exposure_time);
	WritePrivateProfileString("CameraSetting", "cameraC_exposure_time", IniWrite, IniPath);

	IniWrite.Format("%d", cameraC_gain);
	WritePrivateProfileString("CameraSetting", "cameraC_gain", IniWrite, IniPath);

	IniWrite.Format("%d", cameraC_width);
	WritePrivateProfileString("CameraSetting", "cameraC_width", IniWrite, IniPath);

	IniWrite.Format("%d", cameraC_height);
	WritePrivateProfileString("CameraSetting", "cameraC_height", IniWrite, IniPath);

	IniWrite.Format("%s", cameraC_mac.c_str());
	WritePrivateProfileString("CameraSetting", "cameraC_mac", IniWrite, IniPath);
	ReadIni();

	Vsee::VCameraMessageTrans *frame = new Vsee::VCameraMessageTrans;
	parameters_setting_trans.signalling = 5;
	memcpy(&frame->parameters_setting_trans, &parameters_setting_trans, sizeof(parameters_setting_trans));
	frame->magnetic_valve_num = 3;     //The camera C serial number is 2; 
	frame->signalling = 5;
	::PostMessage(CFiberDetectControlDlg::m_hWnd, ID_SHOWCTRLMESSAGE, frame->signalling, (LPARAM)frame);
}


void CCameraSettingDlg::OnBnClickedButtonOpenCameraC()
{
	// TODO:  在此添加控件通知处理程序代码
	Vsee::VCameraMessageTrans *frame = new Vsee::VCameraMessageTrans;
	parameters_setting_trans.signalling = 11;
	memcpy(&frame->parameters_setting_trans, &parameters_setting_trans, sizeof(parameters_setting_trans));
	frame->signalling = 11;
	frame->magnetic_valve_num = 3;
	frame->width = 4096;
	frame->height = 200;
	::PostMessage(CFiberDetectControlDlg::m_hWnd, ID_SHOWCTRLMESSAGE, frame->signalling, (LPARAM)frame);
}


void CCameraSettingDlg::OnBnClickedButtonStopCameraC()
{
	// TODO:  在此添加控件通知处理程序代码
	Vsee::VCameraMessageTrans *frame = new Vsee::VCameraMessageTrans;
	parameters_setting_trans.signalling = 12;
	memcpy(&frame->parameters_setting_trans, &parameters_setting_trans, sizeof(parameters_setting_trans));
	frame->signalling = 12;
	frame->magnetic_valve_num = 3;  //To indicate camera serial number;
	::PostMessage(CFiberDetectControlDlg::m_hWnd, ID_SHOWCTRLMESSAGE, frame->signalling, (LPARAM)frame);
}


void CCameraSettingDlg::OnBnClickedButtonCamera4Net()
{
	// TODO:  在此添加控件通知处理程序代码
	int camera_total;
	char IniRead[255];
	memset(IniRead, 0, 255);
	char IniPath[255];
	memset(IniPath, 0, 255);
	//camera_ip_info ci_info;
	CString strPath = GetModulePath();
	strcpy_s(IniPath, 255, strPath.GetBuffer());
	strcat_s(IniPath, "/param.ini");

	GetPrivateProfileStringA("CameraSetting", "cameraD_ip", "", IniRead, 24, IniPath);
	//m_dlgNetworkParameters.GetDlgItem(IDC_EDIT_IP_ADRESS)->SetWindowTextA(IniRead);
	m_dlgCameraParametersSettting.m_strNetString = IniRead;

	GetPrivateProfileStringA("CameraSetting", "cameraD_port", "", IniRead, 24, IniPath);
	m_dlgCameraParametersSettting.m_u32NetPort = atoi(IniRead);

	GetPrivateProfileStringA("CameraSetting", "cameraD_scan_speed", "", IniRead, 24, IniPath);
	m_dlgCameraParametersSettting.m_nScanSpeed = atoi(IniRead);

	GetPrivateProfileStringA("CameraSetting", "cameraD_tirgging_mode", "", IniRead, 24, IniPath);
	m_dlgCameraParametersSettting.m_nTriggerMode = atoi(IniRead);

	GetPrivateProfileStringA("CameraSetting", "cameraD_exposure_time", "", IniRead, 24, IniPath);
	m_dlgCameraParametersSettting.m_nExposureTime = atoi(IniRead);

	GetPrivateProfileStringA("CameraSetting", "cameraD_gain", "", IniRead, 24, IniPath);
	m_dlgCameraParametersSettting.m_nGain = atoi(IniRead);

	GetPrivateProfileStringA("CameraSetting", "cameraD_width", "", IniRead, 24, IniPath);
	m_dlgCameraParametersSettting.m_nImageWidth = atoi(IniRead);

	GetPrivateProfileStringA("CameraSetting", "cameraD_height", "", IniRead, 24, IniPath);
	m_dlgCameraParametersSettting.m_nImageHeight = atoi(IniRead);

	GetPrivateProfileStringA("CameraSetting", "cameraD_mac", "", IniRead, 24, IniPath);
	m_dlgCameraParametersSettting.m_strMacAddress = IniRead;

	m_dlgCameraParametersSettting.title = "相机D参数设置";

	if (m_dlgCameraParametersSettting.DoModal() != IDOK)
		return;

	cameraD_ip = m_dlgCameraParametersSettting.m_strNetString;
	cameraD_port = m_dlgCameraParametersSettting.m_u32NetPort;
	cameraD_scan_speed = m_dlgCameraParametersSettting.m_nScanSpeed;
	cameraD_tirgging_mode = m_dlgCameraParametersSettting.m_nTriggerMode;
	cameraD_exposure_time = m_dlgCameraParametersSettting.m_nExposureTime;
	cameraD_gain = m_dlgCameraParametersSettting.m_nGain;
	cameraD_width = m_dlgCameraParametersSettting.m_nImageWidth;
	cameraD_height = m_dlgCameraParametersSettting.m_nImageHeight;
	cameraD_mac = m_dlgCameraParametersSettting.m_strMacAddress;

	CString IniWrite;
	IniWrite.Format("%s", cameraD_ip.c_str());
	WritePrivateProfileString("CameraSetting", "cameraD_ip", IniWrite, IniPath);

	IniWrite.Format("%d", cameraD_port);
	WritePrivateProfileString("CameraSetting", "cameraD_port", IniWrite, IniPath);

	IniWrite.Format("%d", cameraD_scan_speed);
	WritePrivateProfileString("CameraSetting", "cameraD_scan_speed", IniWrite, IniPath);

	IniWrite.Format("%d", cameraD_tirgging_mode);
	WritePrivateProfileString("CameraSetting", "cameraD_tirgging_mode", IniWrite, IniPath);

	IniWrite.Format("%d", cameraD_exposure_time);
	WritePrivateProfileString("CameraSetting", "cameraD_exposure_time", IniWrite, IniPath);

	IniWrite.Format("%d", cameraD_gain);
	WritePrivateProfileString("CameraSetting", "cameraD_gain", IniWrite, IniPath);

	IniWrite.Format("%d", cameraD_width);
	WritePrivateProfileString("CameraSetting", "cameraD_width", IniWrite, IniPath);

	IniWrite.Format("%d", cameraD_height);
	WritePrivateProfileString("CameraSetting", "cameraD_height", IniWrite, IniPath);

	IniWrite.Format("%s", cameraD_mac.c_str());
	WritePrivateProfileString("CameraSetting", "cameraD_mac", IniWrite, IniPath);
	ReadIni();

	Vsee::VCameraMessageTrans *frame = new Vsee::VCameraMessageTrans;
	parameters_setting_trans.signalling = 5;
	memcpy(&frame->parameters_setting_trans, &parameters_setting_trans, sizeof(parameters_setting_trans));
	frame->magnetic_valve_num = 4;     //The camera D serial number is 4; 
	frame->signalling = 5;
	::PostMessage(CFiberDetectControlDlg::m_hWnd, ID_SHOWCTRLMESSAGE, frame->signalling, (LPARAM)frame);
}


void CCameraSettingDlg::OnBnClickedButtonOpenCameraD()
{
	// TODO:  在此添加控件通知处理程序代码
	Vsee::VCameraMessageTrans *frame = new Vsee::VCameraMessageTrans;
	parameters_setting_trans.signalling = 11;
	memcpy(&frame->parameters_setting_trans, &parameters_setting_trans, sizeof(parameters_setting_trans));
	frame->signalling = 11;
	frame->magnetic_valve_num = 4;
	frame->width = 4096;
	frame->height = 200;
	::PostMessage(CFiberDetectControlDlg::m_hWnd, ID_SHOWCTRLMESSAGE, frame->signalling, (LPARAM)frame);
}


void CCameraSettingDlg::OnBnClickedButtonStopCameraD()
{
	// TODO:  在此添加控件通知处理程序代码
	Vsee::VCameraMessageTrans *frame = new Vsee::VCameraMessageTrans;
	parameters_setting_trans.signalling = 12;
	memcpy(&frame->parameters_setting_trans, &parameters_setting_trans, sizeof(parameters_setting_trans));
	frame->signalling = 12;
	frame->magnetic_valve_num = 4;  //To indicate camera serial number;
	::PostMessage(CFiberDetectControlDlg::m_hWnd, ID_SHOWCTRLMESSAGE, frame->signalling, (LPARAM)frame);
}
