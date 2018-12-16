#include "stdafx.h"
#include "MySocket.h"
#include "ftp_client.h"
#include "ftp_clientDlg.h"

using namespace std;

MySocket::MySocket()
{
	if_connect = false;
	length = 0;
	//�����������0
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
	//���ӳɹ�  
	if (nErrorCode == 0)
	{
		if_connect = TRUE;
		//����FD_READ�¼�������OnReceive����
		AsyncSelect(FD_READ);
	}
	CAsyncSocket::OnConnect(nErrorCode);
}

void MySocket::OnReceive(int nErrorCode)
{
	Cftp_clientDlg *dlg = (Cftp_clientDlg*)AfxGetApp()->GetMainWnd();//������ָ�����
	//ÿ��receive֮ǰ��Ҫ�ѻ���������
	memset(data, 0, sizeof(data));
	//length�洢�����յ���Ϣ�ĳ��ȣ����յ������ݴ浽data��
	length = Receive(data, sizeof(data), 0);

	receive = data;
}

void MySocket::OnSend(int nErrorCode)
{
	Send(msg, strlen(msg), 0);
	//��������FD_READ�¼�,����socket��Ϣ  
	AsyncSelect(FD_READ);
	CAsyncSocket::OnSend(nErrorCode);
}