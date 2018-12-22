#include "stdafx.h"
#include "MySocket.h"
#include "ftp_client.h"
#include "ftp_clientDlg.h"
#include <atlconv.h>

using namespace std;

MySocket::MySocket()
{
	length = -1;
	//�����������0
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
	Cftp_clientDlg *dlg = (Cftp_clientDlg*)AfxGetApp()->GetMainWnd();//������ָ�����

	//length�洢�����յ���Ϣ�ĳ��ȣ����յ������ݴ浽recv��
	length = ReceiveFrom(recv, sizeof(recv), server_ip, server_port,0);

	receive = recv;
	if (length != SOCKET_ERROR)
	{
		if (receive.Left(36) == "331 USER command is OK,require PASS")
			IsName = TRUE;
		else if (receive.Left(29) == "230 USER login successfully")
			IsLogin = TRUE;
	}
	//�ѻ���������
	memset(recv, 0, sizeof(recv));
	CAsyncSocket::OnReceive(nErrorCode);
}

void MySocket::OnSend(int nErrorCode)
{
	USES_CONVERSION;
	msg = T2A(data);
	SendTo(msg, strlen(msg), server_port, server_ip, 0);
	//��������FD_READ�¼�,����socket��Ϣ  
	AsyncSelect(FD_READ);

	CAsyncSocket::OnSend(nErrorCode);
}