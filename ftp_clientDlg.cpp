
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
	ON_BN_CLICKED(IDC_DownLoad, &Cftp_clientDlg::OnBnClickedDownload)
	ON_BN_CLICKED(IDC_UpLoad, &Cftp_clientDlg::OnBnClickedUpload)
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
	m_FileList.ResetContent();
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
	//休眠10ms，否则无法收到回信就进行后面的操作
	Sleep(10);
	socket.OnReceive(0);
	if (socket.IsName)
	{
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
	m_FileList.ResetContent();
}

void Cftp_clientDlg::GetList()
{
	socket.data = L"LIST";
	socket.OnSend(0);

	int length;
	Sleep(10);
	//socket.OnReceive(0);//无法接受到数据
	length = socket.ReceiveFrom(data, sizeof(data), m_Ip, m_Port, 0);
	if (length != SOCKET_ERROR)
	{
		data[length] = '\0';//避免后面收到的数据造成影响
		CString List(data);
		int index = List.Find(L",");
		while (index != -1)
		{
			m_FileList.AddString(List.Left(index));
			List = List.Right(List.GetLength() - index - 1);
			index = List.Find(L",");
		}
		m_FileList.AddString(List);
	}
}


void Cftp_clientDlg::OnBnClickedDownload()
{
	// TODO: 在此添加控件通知处理程序代码
	CString filename;
	//获取被选中的文件名称
	m_FileList.GetText(m_FileList.GetCurSel(), filename);
	CString temp_filename = filename;
	filename = L"DOWNLOAD:" + filename + L"\r\n";
	CString filepath = L"DOWNLOAD\\" + temp_filename;
	//创建并以二进制打开要下载的文件
	file.Open(filepath, CFile::modeCreate | CFile::modeWrite | CFile::typeBinary);       
	char sendbuf[1024];
	char recvbuf[1024];
	char recvbuffer[1024];
	USES_CONVERSION;
	msg = T2A(filename);
	socket.SendTo(msg, strlen(msg), m_Port, m_Ip, 0);

	int length;
	int num = 0;  //下一个要接受的数据包id
	packet send;
	packet recv;
	send.end = false;
	while (1)
	{
		length = socket.ReceiveFrom((char*)&recv, sizeof(recv), m_Ip, m_Port, 0);
		//length = recvfrom(socket, (char*)&recv, sizeof(recv), 0, (sockaddr*)&addr_aim, &len);
		//if (length = SOCKET_ERROR)
		//continue;
		if (length != SOCKET_ERROR)
		{
			if (recv.end == true)
				break;
			else
			{
				if (recv.number == num)
				{
					recv.data[recv.length] = '\0';
					file.SeekToEnd();
					file.Write(recv.data, recv.length);
					//file.Write(recv.data, strlen(recv.data));
					num++;
				}
				strcpy(send.data, "ACK");
				send.length = strlen(send.data);
				send.number = num;
				//sendto(socket, (char*)&send, sizeof(send), 0, (sockaddr*)&addr_aim, sizeof(addr_aim));
				socket.SendTo((char*)&send, sizeof(send), m_Port, m_Ip, 0);
			}
		}
	}
	file.Close();
	AfxMessageBox(L"文件下载成功！", MB_ICONINFORMATION);
}


void Cftp_clientDlg::OnBnClickedUpload()
{
	// TODO: 在此添加控件通知处理程序代码
	CString path, filename, directory;
	//弹出打开对话框
	CFileDialog File(TRUE, NULL, NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
		_T("所有文件(*.*)|*.*|"), this);
	if (File.DoModal() == IDOK)
	{
		path = File.GetPathName();
		filename = File.GetFileName();
	}

	filename = L"UPLOAD:" + filename + L"\r\n";
	//以二进制打开要上传的文件
	file.Open(path, CFile::modeRead | CFile::typeBinary);       

	USES_CONVERSION;
	msg = T2A(filename);
	socket.SendTo(msg, strlen(msg), m_Port, m_Ip, 0);
	packet send;
	packet recv;
	int num = 0;
	file.SeekToBegin();
	int length_packet = file.GetLength();
	send.length = file.Read(send.data, 1024);
	send.end = false;
	while (send.length)
	{
		send.number = num;
		//socket.SendTo(msg, strlen(msg), m_Port, m_Ip, 0);
		socket.SendTo((char*)&send, sizeof(send), m_Port, m_Ip, 0);
		socket.ReceiveFrom((char*)&recv, sizeof(recv), m_Ip, m_Port, 0);
		//如果发回来的数据包表明刚刚发送的数据包没有错误
		if (recv.number == num + 1)         
		{
			num++;
			send.length = file.Read(send.data, 1024);
		}
	}
	send.end = true;
	socket.SendTo((char*)&send, sizeof(send), m_Port, m_Ip, 0);
	file.Close();
	AfxMessageBox(L"文件上传成功！", MB_ICONINFORMATION);
}
