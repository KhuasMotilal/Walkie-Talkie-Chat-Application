#pragma once
#include <string>
#include <winsock.h>
#include<windows.h>

using namespace std;

class CMsgSenderDlg : public CDialogEx
{
public:
	CMsgSenderDlg(CWnd* pParent = nullptr);	// standard constructor
	CFont m_headingFont;

	#ifdef AFX_DESIGN_TIME
		enum { IDD = IDD_MSGSENDER_DIALOG };
	#endif

		CEdit m_msg;
		CEdit m_CEInputAddress;
		CEdit m_CEInputPort;

		CListBox m_displayBox;

		CFont m_FontHeading;
		CFont m_FontHeadingClientInfo;
		CFont m_FontHeadingSettings;
		CFont m_FontTitleReceived;
		CFont m_FontTitleSend;
		CFont m_FontTitleIP;
		CFont m_FontTitlePort;
		CFont m_FontButtonConnect;
		CFont m_FontButtonClear;
		CFont m_FontButtonAdvance;
		CFont m_FontButtonSend;
		CFont m_FontButtonClose;

		WSADATA m_WsaData;
		int m_ServerSocket;
		int m_ClientSocket;
		int m_iServerSideClientSocket;
		int m_iClientArray[10];
		struct sockaddr_in m_ServerSockAddr;
		struct sockaddr_in m_ClientSockAddr;
		struct timeval m_sTimeVal;
		fd_set m_Fread;
		int m_iMaxFd;
		bool m_bIfConnectButtonClicked;
		char buff[255];
		
		afx_msg void OnBnClickedSend();
		afx_msg void OnBnClickedButtonClearchat();
		afx_msg void OnBnClickedButtonConnect();
		afx_msg void OnBnClickedCancel();

		void FontStyling(CFont& font, int fontSize, int fontWeight, bool italic, bool underline, LPCTSTR fontName, CStatic* pStatic, CButton* pButton = nullptr);

		bool StartServer(int p_iPort = 9909);
		void ClientConnection(const string p_SAddress, int p_IPort);
		static UINT __cdecl ProcessingRequest(LPVOID pParam);
		static UINT __cdecl ReceivingMsg(LPVOID pParam);

protected:
	HICON m_hIcon;
	virtual BOOL OnInitDialog();
	virtual void DoDataExchange(CDataExchange* pDX);

	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);

	DECLARE_MESSAGE_MAP()	
public:
	afx_msg void OnBnClickedButtonAdvance();
};
