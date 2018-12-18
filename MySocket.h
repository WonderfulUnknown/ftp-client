#pragma once
class MySocket :
	public CAsyncSocket
{
public:
	MySocket();
	~MySocket();

	void OnReceive(int nErrorCode);
	void OnSend(int nErrorCode);

	//CString mess;//从Dlg传入要发送的数据
	char *msg;//发送的数据
	int length;//消息长度  
	char data[98000];//消息缓冲区  
	CString receive;//存放从缓冲区中取出来的数据
	CString server_ip;//存储服务器地址
	UINT server_port;//服务器端口

	bool IsError;//判断命令是否有错
	bool Quit;//是否接受到quit命令
	bool IsName;//用户名是否正确
	bool IsLogin;//是否登录
};

