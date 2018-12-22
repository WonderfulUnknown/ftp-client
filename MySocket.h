#pragma once
class MySocket :
	public CAsyncSocket
{
public:
	MySocket();
	~MySocket();

	void OnReceive(int nErrorCode);
	void OnSend(int nErrorCode);

	CString data;//��Dlg��������
	char *msg;//���͵�����
	int length;//��Ϣ����  
	char recv[98000];//��Ϣ������  
	CString receive;//��Ŵӻ�������ȡ����������
	CString server_ip;//�洢��������ַ
	UINT server_port;//�������˿�

	bool IsError;//�ж������Ƿ��д�
	bool Quit;//�Ƿ���ܵ�quit����
	bool IsName;//�û����Ƿ���ȷ
	bool IsLogin;//�Ƿ��¼
};

//�����ļ�ʱ�����ݽṹ
class packet
{
public:
	packet::packet();
	packet::~packet();

	char data[1024];//����
	int number;//���ݰ��ĺ���
	int length;//���ݳ���
	bool end;//������������ݰ�
};