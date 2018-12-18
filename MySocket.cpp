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
	Cftp_clientDlg *dlg = (Cftp_clientDlg*)AfxGetApp()->GetMainWnd();//������ָ�����

	//length�洢�����յ���Ϣ�ĳ��ȣ����յ������ݴ浽data��
	length = ReceiveFrom(data, sizeof(data), server_ip, server_port,0);

	receive = data;
	if (length != SOCKET_ERROR)
	{
		if (receive.Left(36) == "331 USER command is OK,require PASS")
			IsName = TRUE;
		else if (receive.Left(29) == "230 USER login successfully")
			IsLogin = TRUE;
		//else if(receive.Left)
	}
	//else
		//AfxMessageBox(L"FTP�����������ڣ�", MB_ICONSTOP);


	//�ѻ���������
	memset(data, 0, sizeof(data));
	CAsyncSocket::OnReceive(nErrorCode);
}

void MySocket::OnSend(int nErrorCode)
{
//	USES_CONVERSION;
//	msg = T2A(mess);
	SendTo(msg, strlen(msg), server_port, server_ip, 0);
	//��������FD_READ�¼�,����socket��Ϣ  
	AsyncSelect(FD_READ);

	CAsyncSocket::OnSend(nErrorCode);
}