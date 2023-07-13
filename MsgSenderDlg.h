#pragma once
#include <string>
#include <winsock.h>
#include<windows.h>

using namespace std;

class CMsgSenderDlg : public CDialogEx
{
	public:
		CMsgSenderDlg(CWnd* pParent = nullptr);	// standard constructor

		#ifdef AFX_DESIGN_TIME
			enum { IDD = IDD_MSGSENDER_DIALOG };
		#endif

		CEdit m_InputMsg;
		CEdit m_InputAddress;
		CEdit m_InputPort;

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

		CButton m_ButtonConnect;
		CButton m_ButtonAdvance;
		CButton m_ButtonDisconnect;

		WSADATA m_WsaData;
		int m_strAdvancePort;
		int m_iServerSocket;
		int m_iClientSocket;
		int m_iServerSideClientSocket;
		struct sockaddr_in m_structServerSockAddr;
		struct sockaddr_in m_structClientSockAddr;
		bool m_bIsClientWindow;
		char m_cBuff[255];
		
		afx_msg void OnBnClickedSend();
		afx_msg void OnBnClickedButtonClearchat();
		afx_msg void OnBnClickedButtonConnect();
		afx_msg void OnBnClickedCancel();
		afx_msg void OnBnClickedButtonAdvance();

		void FontStyling(CFont& font, int fontSize, int fontWeight, bool italic, bool underline, LPCTSTR fontName, CStatic* pStatic, CButton* pButton = nullptr);

		bool StartServer(int p_iPort = 9909);
		bool ClientConnection(const string p_strIpAddress, int p_iPort);
		static UINT __cdecl ProcessingRequest(LPVOID pParam);
		static UINT __cdecl ClientReceivingMsg(LPVOID pParam);

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
	afx_msg void OnBnClickedButtonDisconnect();
};
