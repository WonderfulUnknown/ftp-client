#pragma once
class MySocket:
	public CAsyncSocket
{
public:
	MySocket();
	~MySocket();

	void OnConnect(int nErrorCode);
	void OnReceive(int nErrorCode);
	void OnSend(int nErrorCode);

	int length;//��Ϣ����  
	char data[8192];//��Ϣ������  
	char *msg;//���͵�����
	CString receive;//��Ŵӻ�������ȡ����������
	int step;//���ڼ���ftpЭ���Ƿ�˳��

	bool if_connect;//�Ƿ�����
	bool IsError;//�ж������Ƿ��д�
	bool Quit;//�Ƿ���ܵ�quit����
};

