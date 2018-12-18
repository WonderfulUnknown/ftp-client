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
	memset(data, 0, sizeof(data));
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


void MySocket::OnReceive(int nErrorCode)
{
	Cftp_clientDlg *dlg = (Cftp_clientDlg*)AfxGetApp()->GetMainWnd();//主窗口指针对象

	//length存储返回收到消息的长度，接收到的数据存到data中
	length = ReceiveFrom(data, sizeof(data), server_ip, server_port,0);

	receive = data;
	if (length != SOCKET_ERROR)
	{
		if (receive.Left(36) == "331 USER command is OK,require PASS")
			IsName = TRUE;
		//else
		//	IsName = FALSE;
		if (receive.Left(29) == "230 USER login successfully")
			IsLogin = TRUE;
		//else
		//	IsLogin = FALSE;
	}
	//else
		//AfxMessageBox(L"FTP服务器不存在！", MB_ICONSTOP);


	//把缓冲区清零
	memset(data, 0, sizeof(data));
	CAsyncSocket::OnReceive(nErrorCode);
}

void MySocket::OnSend(int nErrorCode)
{
//	USES_CONVERSION;
//	msg = T2A(mess);
	SendTo(msg, strlen(msg), server_port, server_ip, 0);
	//继续触发FD_READ事件,接收socket消息  
	AsyncSelect(FD_READ);

	CAsyncSocket::OnSend(nErrorCode);
}