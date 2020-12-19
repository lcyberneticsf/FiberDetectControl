#pragma once
#include <cstdint>
#include <cstddef>
#include <array>
#include <string>
#include <string.h>
#include <cstdint>

// CNetWorkParameters 对话框

class CNetWorkParameters : public CDialogEx
{
	DECLARE_DYNAMIC(CNetWorkParameters)

public:
	CNetWorkParameters(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CNetWorkParameters();

// 对话框数据
	enum { IDD = IDD_DIALOG_NETWORK_PARAMETERS };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	std::string m_strNetString;
	std::uint32_t m_u32NetPort;
};
