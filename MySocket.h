#pragma once
class MySocket :
	public CAsyncSocket
{
public:
	MySocket();
	~MySocket();

	void OnReceive(int nErrorCode);
	void OnSend(int nErrorCode);

	CString data;//从Dlg传入数据
	char *msg;//发送的数据
	int length;//消息长度  
	char recv[98000];//消息缓冲区  
	CString receive;//存放从缓冲区中取出来的数据
	CString server_ip;//存储服务器地址
	UINT server_port;//服务器端口

	bool IsError;//判断命令是否有错
	bool Quit;//是否接受到quit命令
	bool IsName;//用户名是否正确
	bool IsLogin;//是否登录
};

//接受文件时的数据结构
class packet
{
public:
	packet::packet();
	packet::~packet();

	char data[1024];//数据
	int number;//数据包的号码
	int length;//数据长度
	bool end;//代表结束的数据包
};