#pragma once
#include "NetWorkParameters.h"
#include "CameraParametersSetting.h"
#include "ParameterSetting.h"
#include <cstdint>
#include <cstddef>
#include <array>
#include <string>
#include <string.h>
#include <cstdint>
#include "afxwin.h"


// CNetSetting 对话框

class CNetSetting : public CDialogEx
{
	DECLARE_DYNAMIC(CNetSetting)

public:
	CNetSetting(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CNetSetting();

// 对话框数据
	enum { IDD = IDD_DIALOG_NET_SETTING };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedMfcbuttonCamera4Net();
	afx_msg void OnBnClickedButtonCamera3Net();
	afx_msg void OnBnClickedButtonCamera2Net();
	afx_msg void OnBnClickedButtonCamera1Net();
	afx_msg void OnBnClickedButtonElectricalLocalNetSetting();
	afx_msg void OnBnClickedButtonElectricalDriverNetSetting();
	afx_msg void OnBnClickedButtonComputer2NetSetting();
	afx_msg void OnBnClickedButtonComputer1NetSetting();
	afx_msg void OnBnClickedOk();
	CString		GetModulePath(void);
	void    ReadIni();

	CNetWorkParameters m_dlgNetworkParameters;
	CCameraParametersSetting m_dlgCameraParametersSettting;
	CParameterSetting  m_paramSetting;

	std::string local_ip = "192.168.1.16";
	std::uint32_t local_port = 9712;
	std::string box1_ip = "192.168.1.11";
	std::uint32_t box1_port = 9712;
	std::string box2_ip = "192.168.1.12";
	std::uint32_t box2_port = 9712;
	std::string electric_board_ip = "192.168.1.15";
	std::uint32_t electric_board_port = 9712;

	std::string cameraA_ip = "192.168.1.13";
	std::uint32_t cameraA_port = 9712;
	std::uint32_t cameraA_scan_speed = 10000;
	std::uint32_t cameraA_tirgging_mode = 0;
	std::uint32_t cameraA_exposure_time = 15;
	std::uint32_t cameraA_gain = 2;
	std::uint32_t cameraA_width = 4096;
	std::uint32_t cameraA_height = 100;
	std::string cameraA_mac = "00:01:0D:05:EF:0C";
	std::string cameraB_ip = "192.168.1.14";
	std::uint32_t cameraB_port = 9712;
	std::uint32_t cameraB_scan_speed = 10000;
	std::uint32_t cameraB_tirgging_mode = 0;
	std::uint32_t cameraB_exposure_time = 15;
	std::uint32_t cameraB_gain = 2;
	std::uint32_t cameraB_width = 4096;
	std::uint32_t cameraB_height = 100;
	std::string cameraB_mac = "00:01:0D:05:EF:0C";
	std::string cameraC_ip = "192.168.1.14";
	std::uint32_t cameraC_port = 9712;
	std::uint32_t cameraC_scan_speed = 10000;
	std::uint32_t cameraC_tirgging_mode = 0;
	std::uint32_t cameraC_exposure_time = 15;
	std::uint32_t cameraC_gain = 2;
	std::uint32_t cameraC_width = 4096;
	std::uint32_t cameraC_height = 100;
	std::string cameraC_mac = "00:01:0D:05:EF:0C";
	std::string cameraD_ip = "192.168.1.14";
	std::uint32_t cameraD_port = 9712;
	std::uint32_t cameraD_scan_speed = 10000;
	std::uint32_t cameraD_tirgging_mode = 0;
	std::uint32_t cameraD_exposure_time = 15;
	std::uint32_t cameraD_gain = 2;
	std::uint32_t cameraD_width = 4096;
	std::uint32_t cameraD_height = 100;
	std::string cameraD_mac = "00:01:0D:05:EF:0C";

	CParameterSetting				parameters_setting_trans;
	

	afx_msg void OnBnClickedButtonOpenCameraA();
	afx_msg void OnBnClickedButtonStopCameraA();
	afx_msg void OnBnClickedButtonOpenCameraB();
	afx_msg void OnBnClickedButtonOpenCameraC();
	afx_msg void OnBnClickedButtonOpenCameraD();
	afx_msg void OnBnClickedButtonStopCameraB();
	afx_msg void OnBnClickedButtonStopCameraC();
	afx_msg void OnBnClickedButtonStopCameraD();
	afx_msg void OnBnClickedButtonCamera4Net();
	afx_msg void OnBnClickedButtonStartGrabA();
	afx_msg void OnBnClickedButtonStopGrabA();
};
