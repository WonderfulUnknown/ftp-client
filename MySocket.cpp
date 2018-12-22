#include "stdafx.h"
#include "MySocket.h"
#include "ftp_client.h"
#include "ftp_clientDlg.h"
#include <atlconv.h>

using namespace std;

MySocket::MySocket()
{
	length = -1;
	//缓冲区清空置0
	memset(recv, 0, sizeof(recv));
	IsName = FALSE;
	IsLogin = FALSE;
}

MySocket::~MySocket()
{
	if (m_hSocket != INVALID_SOCKET)
	{
		Close();
	}
}

packet::packet()
{
	memset(data, 0, 1024);
	end = FALSE;
}

packet::~packet()
{
}

void MySocket::OnReceive(int nErrorCode)
{
	Cftp_clientDlg *dlg = (Cftp_clientDlg*)AfxGetApp()->GetMainWnd();//主窗口指针对象

	//length存储返回收到消息的长度，接收到的数据存到recv中
	length = ReceiveFrom(recv, sizeof(recv), server_ip, server_port,0);

	receive = recv;
	if (length != SOCKET_ERROR)
	{
		if (receive.Left(36) == "331 USER command is OK,require PASS")
			IsName = TRUE;
		else if (receive.Left(29) == "230 USER login successfully")
			IsLogin = TRUE;
	}
	//把缓冲区清零
	memset(recv, 0, sizeof(recv));
	CAsyncSocket::OnReceive(nErrorCode);
}

void MySocket::OnSend(int nErrorCode)
{
	USES_CONVERSION;
	msg = T2A(data);
	SendTo(msg, strlen(msg), server_port, server_ip, 0);
	//继续触发FD_READ事件,接收socket消息  
	AsyncSelect(FD_READ);

	CAsyncSocket::OnSend(nErrorCode);
}