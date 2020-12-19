#pragma once
#include <cstdint>
#include <cstddef>
#include <array>
#include <string>
#include <string.h>
#include <cstdint>
#include "afxwin.h"

// CCameraParametersSetting 对话框

class CCameraParametersSetting : public CDialogEx
{
	DECLARE_DYNAMIC(CCameraParametersSetting)

public:
	CCameraParametersSetting(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CCameraParametersSetting();

// 对话框数据
	enum { IDD = IDD_DIALOG_CAMERA_SETTING };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual BOOL  OnInitDialog();

	DECLARE_MESSAGE_MAP()
public:
	CString title;
	std::string m_strNetString;		//Camera IP address
	std::uint32_t m_u32NetPort;		//Camera network port
	std::uint32_t m_nScanSpeed;		//Camera scan speed  ,unit :Hz
	std::uint32_t m_nTriggerMode;   //Camera trigger mode,0:auto trigger:1:hardware trigger
	std::uint32_t m_nExposureTime;  //Camera exposure time,unit : us 
	std::uint32_t m_nGain;			//Camera digital gain, unit : db
	std::uint32_t m_nImageWidth;	//Camera image width
	std::uint32_t m_nImageHeight;	//Camera image height
	std::string m_strMacAddress;	//Camera MAC address

	afx_msg void OnBnClickedOk();
	CButton m_checkInternalTrigger;
	CButton m_externalTrigger;
	afx_msg void OnBnClickedCheckInternalTrigger();
	afx_msg void OnBnClickedCheckExternalTrigger2();
};
