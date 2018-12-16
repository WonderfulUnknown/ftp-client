#include "stdafx.h"
#include "MySocket.h"
#include "ftp_client.h"
#include "ftp_clientDlg.h"

using namespace std;

MySocket::MySocket()
{
	if_connect = false;
	length = 0;
	//缓冲区清空置0
	memset(data, 0, sizeof(data));
}

MySocket::~MySocket()
{
	if (m_hSocket != INVALID_SOCKET)
	{
		Close();
	}
}

void MySocket::OnConnect(int nErrorCode)
{
	MySocket *sock = new MySocket();
	//连接成功  
	if (nErrorCode == 0)
	{
		if_connect = TRUE;
		AfxMessageBox(L"连接FTP服务器成功！", MB_ICONINFORMATION);
	}
	else
		if_connect = FALSE;
	CAsyncSocket::OnConnect(nErrorCode);
}

void MySocket::OnReceive(int nErrorCode)
{
	Cftp_clientDlg *dlg = (Cftp_clientDlg*)AfxGetApp()->GetMainWnd();//主窗口指针对象
	//每次receive之前需要把缓冲区清零
	memset(data, 0, sizeof(data));
	//length存储返回收到消息的长度，接收到的数据存到data中
	length = Receive(data, sizeof(data), 0);

	receive = data;
}

void MySocket::OnSend(int nErrorCode)
{
	Send(msg, strlen(msg), 0);
	//继续触发FD_READ事件,接收socket消息  
	AsyncSelect(FD_READ);
	CAsyncSocket::OnSend(nErrorCode);
}