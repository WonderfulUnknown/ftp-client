#pragma once
class MySocket :
	public CAsyncSocket
{
public:
	MySocket();
	~MySocket();

	void OnReceive(int nErrorCode);
	void OnSend(int nErrorCode);

	//CString mess;//��Dlg����Ҫ���͵�����
	char *msg;//���͵�����
	int length;//��Ϣ����  
	char data[98000];//��Ϣ������  
	CString receive;//��Ŵӻ�������ȡ����������
	CString server_ip;//�洢��������ַ
	UINT server_port;//�������˿�

	bool IsError;//�ж������Ƿ��д�
	bool Quit;//�Ƿ���ܵ�quit����
	bool IsName;//�û����Ƿ���ȷ
	bool IsLogin;//�Ƿ��¼
};

