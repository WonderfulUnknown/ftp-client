
// ftp_clientDlg.h : 头文件
//

#pragma once
#include "afxwin.h"
#include "afxcmn.h"
#include "MySocket.h"

// Cftp_clientDlg 对话框
class Cftp_clientDlg : public CDialogEx
{
// 构造
public:
	Cftp_clientDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_FTP_CLIENT_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	MySocket socket;
	CListBox m_FileList;
	CString m_Name;
	CString m_Pwd;
	CString m_Ip;
	UINT m_Port;
	char data[2048];
	char *msg;//发送的消息
	CStdioFile file;

	void GetList();
	bool Compare_Recv(const char* recvstr, const char* instruction);

	afx_msg void OnBnClickedDisconnect();
	afx_msg void OnBnClickedConnect();
	afx_msg void OnBnClickedDownload();
	afx_msg void OnBnClickedUpload();
};
