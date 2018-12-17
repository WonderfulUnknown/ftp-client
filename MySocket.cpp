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
		AfxMessageBox(L"����FTP�������ɹ���", MB_ICONINFORMATION);
	}
	else
		if_connect = FALSE;
	CAsyncSocket::OnConnect(nErrorCode);
}

void MySocket::OnReceive(int nErrorCode)
{
	Cftp_clientDlg *dlg = (Cftp_clientDlg*)AfxGetApp()->GetMainWnd();//������ָ�����

	//length�洢�����յ���Ϣ�ĳ��ȣ����յ������ݴ浽data��
	length = Receive(data, sizeof(data), 0);

	receive = data;
	if (length != SOCKET_ERROR)
		AfxMessageBox(receive, MB_ICONINFORMATION);
	else
		AsyncSelect(FD_READ);

	//�ѻ���������
	memset(data, 0, sizeof(data));
	CAsyncSocket::OnReceive(nErrorCode);
}

void MySocket::OnSend(int nErrorCode)
{
	Send(msg, strlen(msg), 0);
	//��������FD_READ�¼�,����socket��Ϣ  
	AsyncSelect(FD_READ);

	CAsyncSocket::OnSend(nErrorCode);
}