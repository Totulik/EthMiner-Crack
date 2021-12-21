
// EthParamsDlg.cpp:
//

#include "pch.h"
#include "framework.h"
#include "EthParams.h"
#include "EthParamsDlg.h"
#include "afxdialogex.h"

#include <string>
using namespace std;

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);


protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()



CEthParamsDlg::CEthParamsDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_ETHPARAMS_DIALOG, pParent)
	, m_strEthWallet(_T(""))
	, m_strEthPool(_T(""))
	, m_strEthWorker(_T(""))
	, m_strEtcWallet(_T(""))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CEthParamsDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_ETH_WALLET, m_strEthWallet);
	DDX_Text(pDX, IDC_EDIT_ETH_POOL, m_strEthPool);
	DDX_Text(pDX, IDC_EDIT_ETH_WORKER, m_strEthWorker);
	DDX_Text(pDX, IDC_EDIT_ETC_WALLET, m_strEtcWallet);
	DDX_Text(pDX, IDC_EDIT_ETC_POOL, m_strEtcPool);
	DDX_Text(pDX, IDC_EDIT_ETC_WORKER, m_strEtcWorker);
	DDX_Control(pDX, IDC_MFCEDITBROWSE_FILE, m_EditBrowse);
}

BEGIN_MESSAGE_MAP(CEthParamsDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON_MODIFY, &CEthParamsDlg::OnBnClickedButtonModify)
	ON_EN_CHANGE(IDC_MFCEDITBROWSE_FILE, &CEthParamsDlg::OnEnChangeMfceditbrowseFile)
END_MESSAGE_MAP()



BOOL CEthParamsDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
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

	SetIcon(m_hIcon, TRUE);
	SetIcon(m_hIcon, FALSE);

	m_EditBrowse.EnableFileBrowseButton();
	m_EditBrowse.EnableFileBrowseButton(_T(""), _T("Ethereum File|Ethereum.exe||"));

	return TRUE;
}

void CEthParamsDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CEthParamsDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this);

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

HCURSOR CEthParamsDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CEthParamsDlg::OnBnClickedButtonModify()
{
	if (UpdateData()) {
		if (this->m_strEthWallet.IsEmpty()) {
			AfxMessageBox(IDS_WANG_WALLET);
			GetDlgItem(IDC_EDIT_ETH_WALLET)->SetFocus();
		}
		else if (this->m_strEthPool.IsEmpty()) {
			AfxMessageBox(IDS_WANG_POOL);
			GetDlgItem(IDC_EDIT_ETH_POOL)->SetFocus();
		}
		else if (this->m_strEthWorker.IsEmpty()) {
			AfxMessageBox(IDS_WANG_WORKER);
			GetDlgItem(IDC_EDIT_ETH_WORKER)->SetFocus();
		}
		else if (this->m_strEtcWallet.IsEmpty()) {
			AfxMessageBox(IDS_WANG_WALLET);
			GetDlgItem(IDC_EDIT_ETC_WALLET)->SetFocus();
		}
		else if (this->m_strEtcPool.IsEmpty()) {
			AfxMessageBox(IDS_WANG_POOL);
			GetDlgItem(IDC_EDIT_ETC_POOL)->SetFocus();
		}
		else if (this->m_strEtcWorker.IsEmpty()) {
			AfxMessageBox(IDS_WANG_WORKER);
			GetDlgItem(IDC_EDIT_ETC_WORKER)->SetFocus();
		}
		else {
			if (UpdateWallet()) {
				AfxMessageBox(IDS_WANG_UPDATE_OK);
			}
			else {
				AfxMessageBox(IDS_WANG_UPDATE_ERROR);
			}
		}
	}

}

bool CEthParamsDlg::UpdateWallet()
{
	bool bRes = false;

	int poolCount = m_jsonRoot["pools"].size();
	if (poolCount <2 ) {
		AfxMessageBox(IDS_WANG_JSON);
		return false;
	}

	Json::Value root;

	Json::Value ethItem;
	ethItem["coinType"] = "eth";
	ethItem["protocol"] = "stratum+tcp";
	ethItem["wallet"] = CStringA(m_strEthWallet).GetBuffer();;
	ethItem["pool"] = CStringA(m_strEthPool).GetBuffer();;
	ethItem["workname"] = CStringA(m_strEthWorker).GetBuffer();

	Json::Value etcItem;
	etcItem["coinType"] = "etc";
	etcItem["protocol"] = "stratum+tcp";
	etcItem["wallet"] = CStringA(m_strEtcWallet).GetBuffer();;
	etcItem["pool"] = CStringA(m_strEtcPool).GetBuffer();;
	etcItem["workname"] = CStringA(m_strEtcWorker).GetBuffer();

	Json::Value pools;
	pools.append(ethItem);
	pools.append(etcItem);

	root["pools"] = pools;

	std::string out = root.toStyledString();



	HANDLE hexe = BeginUpdateResource(m_strExeFilepath, FALSE);
	int language = 0;

	if (hexe) {

		BOOL r = UpdateResource(hexe, RT_HTML, _T("parames.json"), language,
			(LPVOID)out.c_str(), (DWORD)out.size());
		if (r) {
			bRes = EndUpdateResource(hexe, FALSE);
		}
	}


	return bRes;
}


int CEthParamsDlg::LoadFromResource(LPCTSTR lpszFilepath)
{
	HMODULE hexe = LoadLibrary(lpszFilepath);
	if (hexe != NULL) {

		//查找资源
		HRSRC src = FindResource(hexe, _T("parames.json"), RT_HTML);
		if (src)
		{
			LPBYTE lpData = (LPBYTE)LoadResource(hexe, src);

			int nSize = SizeofResource(hexe, src);

			string  strJson = string((char*)lpData, nSize);



			Json::Reader reader;
			if (reader.parse(strJson, m_jsonRoot))
			{
				int poolCount = m_jsonRoot["pools"].size();
				for (int i = 0; i < poolCount; ++i) {
					Json::Value poolRoot = m_jsonRoot["pools"][i];

					string coinType = poolRoot["coinType"].asCString();
					string wallet = poolRoot["wallet"].asCString();
					string pool = poolRoot["pool"].asCString();
					string protocol = poolRoot["protocol"].asCString();
					string workname = poolRoot["workname"].asCString();

					if (coinType == "eth") {
						m_strEthWallet = CString(wallet.c_str());
						m_strEthPool = CString(pool.c_str());
						m_strEthWorker = CString(workname.c_str());
					}
					else if (coinType == "etc") {
						m_strEtcWallet = CString(wallet.c_str());
						m_strEtcPool = CString(pool.c_str());
						m_strEtcWorker = CString(workname.c_str());
					}
				}

				this->UpdateData(false);

			}

			FreeResource(src);
		}


		FreeLibrary(hexe);
	}

	return 0;
}


void CEthParamsDlg::OnEnChangeMfceditbrowseFile()
{
	CString strFilepath;

	bool isExeFile = false;
	m_EditBrowse.GetWindowText(strFilepath);
	if (PathFileExists(strFilepath)) {
		int k= strFilepath.ReverseFind(_T('.'));
		if (k > 0) {
			k++;
			int len = strFilepath.GetLength() - k;
			CString ext = strFilepath.Right(len);

			if (ext.CompareNoCase(_T("exe"))==0) {
				isExeFile = true;
			}
		}

	}

	if (isExeFile) {
		m_strExeFilepath = strFilepath;
		LoadFromResource(strFilepath);
	}
	

	
}
