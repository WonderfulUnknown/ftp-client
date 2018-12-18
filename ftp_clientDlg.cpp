
// ftp_clientDlg.cpp : ʵ���ļ�
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


// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

	// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);   //DDX/DDV ֧��

// ʵ��
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


// Cftp_clientDlg �Ի���



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


// Cftp_clientDlg ��Ϣ�������

BOOL Cftp_clientDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ��������...���˵�����ӵ�ϵͳ�˵��С�

	// IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
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

	// ���ô˶Ի����ͼ�ꡣ  ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO: �ڴ���Ӷ���ĳ�ʼ������
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
		AfxMessageBox(L"Socket����ʧ�ܣ�", MB_ICONSTOP);
		socket.Close();
	}
	return TRUE; //���ǽ��������õ��ؼ������򷵻� TRUE
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

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ  ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void Cftp_clientDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this);//���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR Cftp_clientDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void Cftp_clientDlg::OnBnClickedConnect()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	//��ȡ�Ի�������
	UpdateData(TRUE);
	if (m_Name.IsEmpty())
	{
		AfxMessageBox(L"�������û�����", MB_ICONSTOP);
		return;
	}
	else if (m_Pwd.IsEmpty())
	{
		AfxMessageBox(L"���������룡", MB_ICONSTOP);
		return;
	}

	socket.server_ip = m_Ip;
	socket.server_port = m_Port;

	USES_CONVERSION;
	char *msg = T2A(L"USER " + m_Name);
	socket.SendTo(msg, strlen(msg), m_Port, m_Ip, 0);
	//socket.AsyncSelect(FD_READ);
	socket.OnReceive(0);
	if (socket.IsName)
	{
		msg = T2A(L"PASS " + m_Pwd);
		socket.SendTo(msg, strlen(msg), m_Port, m_Ip, 0);
		//socket.AsyncSelect(FD_READ);
		socket.OnReceive(0);
		if (socket.IsLogin)
		{
			AfxMessageBox(L"�ɹ���¼FTP��������", MB_ICONINFORMATION);
			GetDlgItem(IDC_Connect)->EnableWindow(FALSE);
			GetDlgItem(IDC_Disconnect)->EnableWindow(TRUE);
			GetDlgItem(IDC_UpLoad)->EnableWindow(TRUE);
			GetDlgItem(IDC_DownLoad)->EnableWindow(TRUE);
			GetDlgItem(IDC_Delete)->EnableWindow(TRUE);
		}
		else
			AfxMessageBox(L"�������", MB_ICONSTOP);
	}
	else
		AfxMessageBox(L"�û��������ڣ�", MB_ICONSTOP);



	//socket.mess = L"USER " + m_Name;
	//socket.AsyncSelect(FD_WRITE);
	//socket.mess = L"PASS " + m_Pwd;
	//socket.AsyncSelect(FD_WRITE);
}

//void Cftp_clientDlg::FindFile()//����ʱ�����
//{
//	//���FileList֮ǰ������
//	while (m_FileList.GetCount() != 0)
//		m_FileList.DeleteString(0);
//
//	CFtpFileFind ftpfind(m_FtpConn.pConnection);
//	if (ftpfind.FindFile(L"*"))//�������е��ļ�
//	{
//		CString FileName;
//		while (ftpfind.FindNextFile())
//		{
//			//�ж��Ƿ���Ŀ¼
//			if (!ftpfind.IsDirectory())
//			{
//				FileName = ftpfind.GetFileName();
//				m_FileList.AddString(FileName);
//			}
//		}
//		//���һ��
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
//	// TODO: �ڴ���ӿؼ�֪ͨ����������
//	m_FtpConn.InitConnect(m_Ip, m_Name, m_Pwd, curr_port, TRUE);
//
//	CString curr_file, filename;
//	//��ȡ��ѡ�е��ļ�����
//	m_FileList.GetText(m_FileList.GetCurSel(), curr_file);
//	if (!curr_file.IsEmpty())
//	{
//		//�������Ϊ�Ի���
//		CFileDialog file(FALSE, NULL, curr_file, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
//			_T("�����ļ�(*.*)|*.*|"), this);
//		if (file.DoModal() == IDOK)
//		{
//			filename = file.GetPathName();
//			//�����ļ���ѡ���ı���λ��
//			if (m_FtpConn.pConnection->GetFile(curr_file, filename))
//				AfxMessageBox(L"���سɹ���", MB_ICONINFORMATION);
//			else
//				AfxMessageBox(L"����ʧ�ܣ�", MB_ICONSTOP);
//		}
//	}
//	m_FtpConn.ExitConnect();
//
//	//ʹ��Get,Set�ı�Ftp�������ļ�Ŀ¼
//	//CString address;
//	//m_FtpConn.pConnection->GetCurrentDirectory(address);
//	//m_FtpConn.pConnection->SetCurrentDirectory(address);
//}
//
//
//void Cftp_clientDlg::OnBnClickedUpload()
//{
//	// TODO: �ڴ���ӿؼ�֪ͨ����������
//	m_FtpConn.InitConnect(m_Ip, m_Name, m_Pwd, curr_port, TRUE);
//	CString path, filename, directory;
//	//�����򿪶Ի���
//	CFileDialog file(TRUE, NULL, NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
//		_T("�����ļ�(*.*)|*.*|"), this);
//	if (file.DoModal() == IDOK)
//	{
//		path = file.GetPathName();
//		filename = file.GetFileName();
//	}
//	m_FtpConn.pConnection->GetCurrentDirectory(directory);
//	//�ϴ��ļ�
//	if (m_FtpConn.pConnection->PutFile((LPCTSTR)path, (LPCTSTR)filename))
//	{
//		m_FtpConn.pConnection->SetCurrentDirectory(directory);
//		AfxMessageBox(_T("�ϴ��ɹ���"));
//	}
//	FindFile();
//}


void Cftp_clientDlg::OnBnClickedDisconnect()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	socket.Close();
	GetDlgItem(IDC_Disconnect)->EnableWindow(FALSE);
	GetDlgItem(IDC_Connect)->EnableWindow(TRUE);
	GetDlgItem(IDC_UpLoad)->EnableWindow(FALSE);
	GetDlgItem(IDC_DownLoad)->EnableWindow(FALSE);
	GetDlgItem(IDC_Delete)->EnableWindow(FALSE);
	//���FileList
}
