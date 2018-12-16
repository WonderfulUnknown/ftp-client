
// ftp_clientDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "ftp_client.h"
#include "ftp_clientDlg.h"
#include "afxdialogex.h"
#include <afxinet.h>
#include "MySocket.h"
#include <atlconv.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

	// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);   //DDX/DDV 支持

// 实现
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


// Cftp_clientDlg 对话框



Cftp_clientDlg::Cftp_clientDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_FTP_CLIENT_DIALOG, pParent)
	, m_Name(_T(""))
	, m_Pwd(_T(""))
	, m_Ip(_T(""))
	, m_Port(_T(""))
	, curr_port(0)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void Cftp_clientDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_FileList, m_FileList);
	DDX_Text(pDX, IDC_Account, m_Name);
	DDX_Text(pDX, IDC_PassWord, m_Pwd);
	DDX_Text(pDX, IDC_Ip, m_Ip);
	DDX_Text(pDX, IDC_Port, m_Port);
}

BEGIN_MESSAGE_MAP(Cftp_clientDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_Connect, &Cftp_clientDlg::OnBnClickedConnect)
	//ON_BN_CLICKED(IDC_DownLoad, &Cftp_clientDlg::OnBnClickedDownload)
	//ON_BN_CLICKED(IDC_UpLoad, &Cftp_clientDlg::OnBnClickedUpload)
	ON_BN_CLICKED(IDC_Disconnect, &Cftp_clientDlg::OnBnClickedDisconnect)
END_MESSAGE_MAP()


// Cftp_clientDlg 消息处理程序

BOOL Cftp_clientDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

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

	// TODO: 在此添加额外的初始化代码
	//GetDlgItem(IDC_Ip)->SetWindowText(_T("192.168.100.99"));
	GetDlgItem(IDC_Ip)->SetWindowText(_T("127.0.0.1"));
	GetDlgItem(IDC_Account)->SetWindowText(_T("123"));
	GetDlgItem(IDC_PassWord)->SetWindowText(_T("123"));
	GetDlgItem(IDC_Port)->SetWindowTextW(_T("21"));

	GetDlgItem(IDC_Disconnect)->EnableWindow(FALSE);
	GetDlgItem(IDC_UpLoad)->EnableWindow(FALSE);
	GetDlgItem(IDC_DownLoad)->EnableWindow(FALSE);
	GetDlgItem(IDC_Delete)->EnableWindow(FALSE);
	return TRUE; //除非将焦点设置到控件，否则返回 TRUE
}

void Cftp_clientDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void Cftp_clientDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this);//用于绘制的设备上下文

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
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR Cftp_clientDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void Cftp_clientDlg::OnBnClickedConnect()
{
	// TODO: 在此添加控件通知处理程序代码
	//获取对话框输入
	UpdateData(TRUE);
	curr_port = _ttoi(m_Port);

	//if (!socket.Create(0, SOCK_STREAM, FD_CONNECT))
	if (!socket.Create(0, SOCK_DGRAM, FD_CONNECT))
	{
		AfxMessageBox(L"Socket创建失败！", MB_ICONSTOP);
		socket.Close();
		return;
	}
	socket.Connect(m_Ip, curr_port);

	//if (socket.if_connect)
	//{

	//	AfxMessageBox(L"连接FTP服务器成功！", MB_ICONINFORMATION);

	GetDlgItem(IDC_Connect)->EnableWindow(FALSE);
	GetDlgItem(IDC_Disconnect)->EnableWindow(TRUE);
	GetDlgItem(IDC_UpLoad)->EnableWindow(TRUE);
	GetDlgItem(IDC_DownLoad)->EnableWindow(TRUE);
	GetDlgItem(IDC_Delete)->EnableWindow(TRUE);
	//}
	//else
	//{
	//	AfxMessageBox(L"连接FTP服务器失败！", MB_ICONSTOP);
	//	socket.Close();
	//}
	CString temp;
	temp = L"USER " + m_Name;
	//CString转char *
	USES_CONVERSION;
	char *msg = T2A(temp);
	socket.Send(msg, strlen(msg), 0);
}

//void Cftp_clientDlg::FindFile()//花费时间过长
//{
//	//清除FileList之前的内容
//	while (m_FileList.GetCount() != 0)
//		m_FileList.DeleteString(0);
//
//	CFtpFileFind ftpfind(m_FtpConn.pConnection);
//	if (ftpfind.FindFile(L"*"))//查找所有的文件
//	{
//		CString FileName;
//		while (ftpfind.FindNextFile())
//		{
//			//判断是否是目录
//			if (!ftpfind.IsDirectory())
//			{
//				FileName = ftpfind.GetFileName();
//				m_FileList.AddString(FileName);
//			}
//		}
//		//最后一项
//		if (!ftpfind.IsDirectory())
//		{
//			FileName = ftpfind.GetFileName();
//			m_FileList.AddString(FileName);
//		}
//	}
//}
//
//void Cftp_clientDlg::OnBnClickedDownload()
//{
//	// TODO: 在此添加控件通知处理程序代码
//	m_FtpConn.InitConnect(m_Ip, m_Name, m_Pwd, curr_port, TRUE);
//
//	CString curr_file, filename;
//	//获取被选中的文件名称
//	m_FileList.GetText(m_FileList.GetCurSel(), curr_file);
//	if (!curr_file.IsEmpty())
//	{
//		//弹出另存为对话框
//		CFileDialog file(FALSE, NULL, curr_file, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
//			_T("所有文件(*.*)|*.*|"), this);
//		if (file.DoModal() == IDOK)
//		{
//			filename = file.GetPathName();
//			//下载文件到选定的本地位置
//			if (m_FtpConn.pConnection->GetFile(curr_file, filename))
//				AfxMessageBox(L"下载成功！", MB_ICONINFORMATION);
//			else
//				AfxMessageBox(L"下载失败！", MB_ICONSTOP);
//		}
//	}
//	m_FtpConn.ExitConnect();
//
//	//使用Get,Set改变Ftp服务器文件目录
//	//CString address;
//	//m_FtpConn.pConnection->GetCurrentDirectory(address);
//	//m_FtpConn.pConnection->SetCurrentDirectory(address);
//}
//
//
//void Cftp_clientDlg::OnBnClickedUpload()
//{
//	// TODO: 在此添加控件通知处理程序代码
//	m_FtpConn.InitConnect(m_Ip, m_Name, m_Pwd, curr_port, TRUE);
//	CString path, filename, directory;
//	//弹出打开对话框
//	CFileDialog file(TRUE, NULL, NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
//		_T("所有文件(*.*)|*.*|"), this);
//	if (file.DoModal() == IDOK)
//	{
//		path = file.GetPathName();
//		filename = file.GetFileName();
//	}
//	m_FtpConn.pConnection->GetCurrentDirectory(directory);
//	//上传文件
//	if (m_FtpConn.pConnection->PutFile((LPCTSTR)path, (LPCTSTR)filename))
//	{
//		m_FtpConn.pConnection->SetCurrentDirectory(directory);
//		AfxMessageBox(_T("上传成功！"));
//	}
//	FindFile();
//}


void Cftp_clientDlg::OnBnClickedDisconnect()
{
	// TODO: 在此添加控件通知处理程序代码
	socket.Close();
	GetDlgItem(IDC_Disconnect)->EnableWindow(FALSE);
	GetDlgItem(IDC_Connect)->EnableWindow(TRUE);
	GetDlgItem(IDC_UpLoad)->EnableWindow(FALSE);
	GetDlgItem(IDC_DownLoad)->EnableWindow(FALSE);
	GetDlgItem(IDC_Delete)->EnableWindow(FALSE);
	//清空FileList
}
