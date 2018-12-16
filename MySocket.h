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

	int length;//消息长度  
	char data[8192];//消息缓冲区  
	char *msg;//发送的数据
	CString receive;//存放从缓冲区中取出来的数据
	int step;//用于检验ftp协议是否按顺序

	bool if_connect;//是否连接
	bool IsError;//判断命令是否有错
	bool Quit;//是否接受到quit命令
};

