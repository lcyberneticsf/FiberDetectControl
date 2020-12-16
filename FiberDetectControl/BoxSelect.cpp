// BoxSelect.cpp : 实现文件
//

#include "stdafx.h"
#include "FiberDetectControl.h"
#include "BoxSelect.h"
#include "afxdialogex.h"
#include "communicator.h"
#include "FiberDetectControlDlg.h"
// CBoxSelect 对话框

IMPLEMENT_DYNAMIC(CBoxSelect, CDialogEx)

CBoxSelect::CBoxSelect(CWnd* pParent /*=NULL*/)
	: CDialogEx(CBoxSelect::IDD, pParent)
{
	
}

CBoxSelect::~CBoxSelect()
{
}

void CBoxSelect::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	int num = GetBoxnum();
	switch (num)
	{
	case 1:((CButton *)GetDlgItem(IDC_AIBOX_CHECK1))->SetCheck(1); break;
	case 2:((CButton *)GetDlgItem(IDC_AIBOX_CHECK2))->SetCheck(1); break;
	default: break;
	}
}


BEGIN_MESSAGE_MAP(CBoxSelect, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON_BOX_SELECT, &CBoxSelect::OnBnClickedButtonBoxSelect)
	ON_BN_CLICKED(IDC_AIBOX_CHECK1, &CBoxSelect::OnBnClickedAiboxCheck1)
	ON_BN_CLICKED(IDC_AIBOX_CHECK2, &CBoxSelect::OnBnClickedAiboxCheck2)
	ON_BN_CLICKED(IDC_AIBOX_CHECK3, &CBoxSelect::OnBnClickedAiboxCheck3)
END_MESSAGE_MAP()


// CBoxSelect 消息处理程序
CString CBoxSelect::GetModulePath(void)
{
	CString    sPath;
	GetModuleFileNameA(NULL, sPath.GetBufferSetLength(MAX_PATH + 1), MAX_PATH);
	sPath.ReleaseBuffer();
	int    nPos;
	nPos = sPath.ReverseFind('\\');
	sPath = sPath.Left(nPos);
	return   sPath;
}

int  CBoxSelect::GetBoxnum(void)
{
	char IniPath[255];
	memset(IniPath, 0, 255);
	CString strPath = GetModulePath();
	strcpy_s(IniPath, 255, strPath.GetBuffer());
	strcat_s(IniPath, "/param.ini");
	char IniRead[255];
	memset(IniRead, 0, 255);
	GetPrivateProfileStringA("System", "box_num", "", IniRead, 24, IniPath);
	int num = atoi(IniRead);
	return num;
}
BOOL CBoxSelect::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	ReadIni();
	if (parameters_setting_trans.box_num == 1)
	{
		((CButton *)GetDlgItem(IDC_AIBOX_CHECK1))->SetCheck(true);
		((CButton *)GetDlgItem(IDC_AIBOX_CHECK2))->SetCheck(false);
		((CButton *)GetDlgItem(IDC_AIBOX_CHECK3))->SetCheck(false);
	}
	else if (parameters_setting_trans.box_num == 2)
	{
		((CButton *)GetDlgItem(IDC_AIBOX_CHECK1))->SetCheck(false);
		((CButton *)GetDlgItem(IDC_AIBOX_CHECK2))->SetCheck(true);
		((CButton *)GetDlgItem(IDC_AIBOX_CHECK3))->SetCheck(false);
	}
	else
	{
		((CButton *)GetDlgItem(IDC_AIBOX_CHECK1))->SetCheck(false);
		((CButton *)GetDlgItem(IDC_AIBOX_CHECK2))->SetCheck(false);
		((CButton *)GetDlgItem(IDC_AIBOX_CHECK3))->SetCheck(true);
	}
		
	return true;
}
void CBoxSelect::OnBnClickedButtonBoxSelect()
{
	// TODO:  在此添加控件通知处理程序代码
	int state1 = ((CButton *)GetDlgItem(IDC_AIBOX_CHECK1))->GetCheck();
	int state2 = ((CButton *)GetDlgItem(IDC_AIBOX_CHECK2))->GetCheck();
	int state3 = ((CButton *)GetDlgItem(IDC_AIBOX_CHECK3))->GetCheck();
	if (state1)
	{
		parameters_setting_trans.box_num = 1;
	}
	else if (state2)
	{
		parameters_setting_trans.box_num = 2;
	}
	else if (state3)
	{
		parameters_setting_trans.box_num = 3;
	}
	char IniPath[255];
	memset(IniPath, 0, 255);
	//camera_ip_info ci_info;
	CString strPath = GetModulePath();
	strcpy_s(IniPath, 255, strPath.GetBuffer());
	strcat_s(IniPath, "/param.ini");
	CString IniWrite;
	IniWrite.Format("%d", parameters_setting_trans.box_num);
	WritePrivateProfileString("System", "box_num", IniWrite, IniPath);
	ReadIni();
	Vsee::VCameraMessageTrans *frame = new Vsee::VCameraMessageTrans;
	parameters_setting_trans.signalling = 21;
	memcpy(&frame->parameters_setting_trans, &parameters_setting_trans, sizeof(parameters_setting_trans));
	frame->signalling = 21;
	::PostMessage(CFiberDetectControlDlg::m_hWnd, ID_SHOWCTRLMESSAGE, frame->signalling, (LPARAM)frame);
	EndDialog(0);
}

void CBoxSelect::ReadIni()
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

void CBoxSelect::OnBnClickedAiboxCheck1()
{
	// TODO:  在此添加控件通知处理程序代码
	((CButton *)GetDlgItem(IDC_AIBOX_CHECK1))->SetCheck(true);
	((CButton *)GetDlgItem(IDC_AIBOX_CHECK2))->SetCheck(false);
	((CButton *)GetDlgItem(IDC_AIBOX_CHECK3))->SetCheck(false);

	/*if (((CButton *)GetDlgItem(IDC_AIBOX_CHECK1))->GetCheck())
	{
	((CButton *)GetDlgItem(IDC_AIBOX_CHECK2))->SetCheck(0);
	}
	else
	{
	((CButton *)GetDlgItem(IDC_AIBOX_CHECK2))->SetCheck(1);
	}*/
}


void CBoxSelect::OnBnClickedAiboxCheck2()
{
	// TODO:  在此添加控件通知处理程序代码
	((CButton *)GetDlgItem(IDC_AIBOX_CHECK1))->SetCheck(false);
	((CButton *)GetDlgItem(IDC_AIBOX_CHECK2))->SetCheck(true);
	((CButton *)GetDlgItem(IDC_AIBOX_CHECK3))->SetCheck(false);

	/*if (((CButton *)GetDlgItem(IDC_AIBOX_CHECK2))->GetCheck())
	{
	((CButton *)GetDlgItem(IDC_AIBOX_CHECK1))->SetCheck(0);
	}
	else
	{
	((CButton *)GetDlgItem(IDC_AIBOX_CHECK1))->SetCheck(1);
	}*/
}


void CBoxSelect::OnBnClickedAiboxCheck3()
{
	// TODO:  在此添加控件通知处理程序代码
	((CButton *)GetDlgItem(IDC_AIBOX_CHECK1))->SetCheck(false);
	((CButton *)GetDlgItem(IDC_AIBOX_CHECK2))->SetCheck(false);
	((CButton *)GetDlgItem(IDC_AIBOX_CHECK3))->SetCheck(true);
}
