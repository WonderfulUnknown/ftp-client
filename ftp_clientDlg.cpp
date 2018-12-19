
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
	, m_Port(0)
	//, curr_port(0)
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

	if (!socket.Create(0, SOCK_DGRAM, FD_READ))
	{
		AfxMessageBox(L"Socket创建失败！", MB_ICONSTOP);
		socket.Close();
	}
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
	if (m_Name.IsEmpty())
	{
		AfxMessageBox(L"请输入用户名！", MB_ICONSTOP);
		return;
	}
	else if (m_Pwd.IsEmpty())
	{
		AfxMessageBox(L"请输入密码！", MB_ICONSTOP);
		return;
	}

	socket.server_ip = m_Ip;
	socket.server_port = m_Port;
	
	USES_CONVERSION;
	socket.data = T2A(L"USER " + m_Name);
	socket.OnSend(0);
	//msg = T2A(L"USER " + m_Name);
	//socket.SendTo(msg, strlen(msg), m_Port, m_Ip, 0);
	//休眠10ms，否则无法收到回信就进行后面的操作
	Sleep(10);
	socket.OnReceive(0);
	if (socket.IsName)
	{
		//msg = T2A(L"PASS " + m_Pwd);
		//socket.SendTo(msg, strlen(msg), m_Port, m_Ip, 0);
		socket.data = T2A(L"PASS " + m_Pwd);
		socket.OnSend(0);

		Sleep(10);
		socket.OnReceive(0);
		if (socket.IsLogin)
		{
			AfxMessageBox(L"成功登录FTP服务器！", MB_ICONINFORMATION);
			GetDlgItem(IDC_Connect)->EnableWindow(FALSE);
			GetDlgItem(IDC_Disconnect)->EnableWindow(TRUE);
			GetDlgItem(IDC_UpLoad)->EnableWindow(TRUE);
			GetDlgItem(IDC_DownLoad)->EnableWindow(TRUE);
			GetDlgItem(IDC_Delete)->EnableWindow(TRUE);
		}
		else
			AfxMessageBox(L"密码错误！", MB_ICONSTOP);
	}
	else
		AfxMessageBox(L"用户名不存在！", MB_ICONSTOP);

	GetList();//获取文件列表
}



void Cftp_clientDlg::OnBnClickedDisconnect()
{
	// TODO: 在此添加控件通知处理程序代码
	//socket.Close();
	//msg = "QUIT";
	//socket.SendTo(msg, strlen(msg), m_Port, m_Ip, 0);
	socket.data = L"QUIT";
	socket.OnSend(0);
	GetDlgItem(IDC_Disconnect)->EnableWindow(FALSE);
	GetDlgItem(IDC_Connect)->EnableWindow(TRUE);
	GetDlgItem(IDC_UpLoad)->EnableWindow(FALSE);
	GetDlgItem(IDC_DownLoad)->EnableWindow(FALSE);
	GetDlgItem(IDC_Delete)->EnableWindow(FALSE);
	
	socket.IsName = FALSE;
	socket.IsLogin = FALSE;
	//清空FileList
}

// 对收到的指令进行判断
bool Cftp_clientDlg::Compare_Recv(const char* recvstr, const char* instruction)
{
	int i;
	for (i = 0; i<strlen(instruction); i++)
	{
		if (i >= strlen(recvstr))          //如果收到字符长度小于命令字的长度，则退出
		{
			break;
		}
		if (!(recvstr[i] == instruction[i] || (recvstr[i] + 32) == instruction[i]))  //假设命令字为全小写，若接受字符的命令字字段与命令字不符合，退出
		{
			break;
		}
	}
	if (i != strlen(instruction))
	{
		return false;
	}
	else
	{
		return true;
	}
}

void Cftp_clientDlg::GetList()
{
	char sendbuf[1024];

	char recvbuffer[2048];
	char recvbuf[2048];
	char list[2048];
	memset(recvbuffer, 0, sizeof(recvbuffer));
	CString exist;

	socket.data = L"LIST";
	socket.OnSend(0);
	
	//int recv;
//	int num = 0;
//	while (num < 2)//此处需要确保接收到两个数据包，分别为服务器发来的目录信息和完成信息 
//	{
		Sleep(10);
		//socket.OnReceive(0);
		//recv = socket.ReceiveFrom(recvbuffer, sizeof(recvbuffer), m_Ip, m_Port, 0);
		//没收到数据
		socket.ReceiveFrom(recvbuffer, sizeof(recvbuffer), m_Ip, m_Port, 0);
		if (socket.length != SOCKET_ERROR)
		{
			recvbuffer[socket.length] = '\0';
			strcpy(list, recvbuffer);
			//if (Compare_Recv(recvbuffer, "Finished!") == true)
			//{
			//	strcpy(recvbuf, recvbuffer);
			//}
			//else
			//{
			//	strcpy(list, recvbuffer);
			//}
			//num++;
		}
		//num++;
	//}
	CString List(list);
	int index = List.Find(L",");
	while (index != -1)
	{
		m_FileList.AddString(List.Left(index));
		List = List.Right(List.GetLength() - index - 1);
		index = List.Find(L",");
	}
	m_FileList.AddString(List);
	//CString send_s(sendbuf);
	//CString recv_s(recvbuf);
	//m_chat.GetWindowTextW(exist);
	//exist = exist + L"C:" + send_s + L"S:" + recv_s;
	//m_chat.SetWindowTextW(exist);
	//return 0;
}
