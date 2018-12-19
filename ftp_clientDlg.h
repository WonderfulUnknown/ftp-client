
// ftp_clientDlg.h : ͷ�ļ�
//

#pragma once
#include "afxwin.h"
#include "afxcmn.h"
#include "MySocket.h"

// Cftp_clientDlg �Ի���
class Cftp_clientDlg : public CDialogEx
{
// ����
public:
	Cftp_clientDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_FTP_CLIENT_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
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
	char *msg;//���͵���Ϣ
	CStdioFile file;

	void GetList();
	bool Compare_Recv(const char* recvstr, const char* instruction);

	afx_msg void OnBnClickedDisconnect();
	afx_msg void OnBnClickedConnect();
	afx_msg void OnBnClickedDownload();
	afx_msg void OnBnClickedUpload();
};
