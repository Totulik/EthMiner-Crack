#pragma once
#include "../lib_json/json/json.h"


class CEthParamsDlg : public CDialogEx
{

public:
	CEthParamsDlg(CWnd* pParent = nullptr);	


#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ETHPARAMS_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);


protected:
	HICON m_hIcon;

	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	CString m_strEthWallet;
	CString m_strEthPool;
	CString m_strEthWorker;
	CString m_strEtcWallet;
	CString m_strEtcPool;
	CString m_strEtcWorker;
	CMFCEditBrowseCtrl m_EditBrowse;

	Json::Value m_jsonRoot;
	CString m_strExeFilepath;


	afx_msg void OnBnClickedButtonModify();

protected:
	bool UpdateWallet();
public:
	int LoadFromResource(LPCTSTR lpszFilepath);
	afx_msg void OnEnChangeMfceditbrowseFile();
};
