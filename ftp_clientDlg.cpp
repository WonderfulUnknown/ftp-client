
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
	ON_BN_CLICKED(IDC_DownLoad, &Cftp_clientDlg::OnBnClickedDownload)
	ON_BN_CLICKED(IDC_UpLoad, &Cftp_clientDlg::OnBnClickedUpload)
	ON_BN_CLICKED(IDC_Delete, &Cftp_clientDlg::OnBnClickedDelete)
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
	m_FileList.ResetContent();
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
	socket.data = T2A(L"USER " + m_Name);
	socket.OnSend(0);
	//����10ms�������޷��յ����žͽ��к���Ĳ���
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

	GetList();//��ȡ�ļ��б�
}



void Cftp_clientDlg::OnBnClickedDisconnect()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
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
	//���FileList
	m_FileList.ResetContent();
}

void Cftp_clientDlg::GetList()
{
	socket.data = L"LIST";
	socket.OnSend(0);

	//Sleep(10);
	//socket.OnReceive(0);//�޷����ܵ�����
	int length;
	while (1)
	{
		Sleep(1);
		length = socket.ReceiveFrom(data, sizeof(data), m_Ip, m_Port, 0);
		//while (length == SOCKET_ERROR)
		//	socket.ReceiveFrom(data, sizeof(data), m_Ip, m_Port, 0);
		if (length != SOCKET_ERROR)
		{
			data[length] = '\0';
			CString List(data);
			int index = List.Find(L",");
			while (index != -1)
			{
				m_FileList.AddString(List.Left(index));
				List = List.Right(List.GetLength() - index - 1);
				index = List.Find(L",");
			}
			m_FileList.AddString(List);
			break;
		}
	}
}


void Cftp_clientDlg::OnBnClickedDownload()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CString filename, filepath;
	//��ȡ��ѡ�е��ļ�����
	m_FileList.GetText(m_FileList.GetCurSel(), filename);

	//�������Ϊ�Ի���
	CFileDialog File(FALSE, NULL, filename, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
		_T("�����ļ�(*.*)|*.*|"), this);

	if (File.DoModal() == IDOK)
	{
		filename = File.GetFileName();
		filepath = File.GetPathName();
	}
	else return;

	USES_CONVERSION;
	filename = L"DOWNLOAD " + filename;
	msg = T2A(filename);
	//��֪�������Լ���Ҫ���ļ���
	socket.SendTo(msg, strlen(msg), m_Port, m_Ip, 0);

	file.Open(filepath, CFile::modeCreate | CFile::modeWrite | CFile::typeBinary);

	int length, num = 0;
	packet send, recv;

	//ֻ����ACK
	send.length = 3;
	strcpy_s(send.data, send.length + 1, "ACK");//��ȫ����

	while (1)
	{
		Sleep(1);
		length = socket.ReceiveFrom((char*)&recv, sizeof(recv), m_Ip, m_Port, 0);
		if (length != SOCKET_ERROR)
		{
			if (recv.end == true)
				break;
			if (recv.number == num)
			{
				num++;

				file.Seek(0, CFile::end);
				file.Write(recv.data, recv.length);
				//file.Write(recv.data, strlen(recv.data));
			}
			send.number = num;
			socket.SendTo((char*)&send, sizeof(send), m_Port, m_Ip, 0);
		}
	}
	file.Close();
	AfxMessageBox(L"�ļ����سɹ���", MB_ICONINFORMATION);
}


void Cftp_clientDlg::OnBnClickedUpload()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CString path, filename, directory;
	//�����򿪶Ի���
	CFileDialog File(TRUE, NULL, NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
		_T("�����ļ�(*.*)|*.*|"), this);

	if (File.DoModal() == IDOK)
	{
		path = File.GetPathName();
		filename = File.GetFileName();
	}
	else return;

	file.Open(path, CFile::modeRead | CFile::typeBinary);

	USES_CONVERSION;
	msg = T2A(L"UPLOAD " + filename);
	socket.SendTo(msg, strlen(msg), m_Port, m_Ip, 0);

	packet send, recv;
	int length, num = 0;

	file.Seek(0, CFile::begin);
	send.length = file.Read(send.data, 1024);
	while (send.length)
	{
		send.number = num;
		socket.SendTo((char*)&send, sizeof(send), m_Port, m_Ip, 0);
		Sleep(1);

		length = socket.ReceiveFrom((char*)&recv, sizeof(recv), m_Ip, m_Port, 0);

		if (length != SOCKET_ERROR)
		{
			//ͨ��number�жϸշ��͵����ݰ��Ƿ��ʹ�
			if (recv.number == num + 1)
			{
				num++;

				memset(send.data, 0, sizeof(send.data));
				send.length = file.Read(send.data, 1024);
			}
		}
	}
	send.end = true;
	//��������Ϊ�գ���־���������ݰ�
	socket.SendTo((char*)&send, sizeof(send), m_Port, m_Ip, 0);
	file.Close();

	AfxMessageBox(L"�ļ��ϴ��ɹ���", MB_ICONINFORMATION);
	//GetList();
	m_FileList.AddString(filename);
}


void Cftp_clientDlg::OnBnClickedDelete()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CString filename;
	//��ȡ��ѡ�е��ļ�����
	m_FileList.GetText(m_FileList.GetCurSel(), filename);
	
	filename = L"DELETE " + filename;
	USES_CONVERSION;
	msg = T2A(filename);
	socket.SendTo(msg, strlen(msg), m_Port, m_Ip, 0);

	int length;
	CString temp;
	while (1)
	{
		Sleep(1);
		length = socket.ReceiveFrom(data, sizeof(data), m_Ip, m_Port, 0);

		if (length != SOCKET_ERROR)
		{
			temp = data;
			if (temp.Left(24) = "230 Delete successfully")
			{
				AfxMessageBox(L"�ļ�ɾ���ɹ���", MB_ICONINFORMATION);
				m_FileList.DeleteString(m_FileList.GetCurSel());
			}
			else
				AfxMessageBox(L"�ļ�ɾ��ʧ�ܣ�", MB_ICONSTOP);
			break;
		}
	}
	//GetList();

}
