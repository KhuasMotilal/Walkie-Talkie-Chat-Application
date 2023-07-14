// MsgSenderDlg.cpp : implementation file
#include "pch.h"
#include "framework.h"
#include "MsgSender.h"
#include "MsgSenderDlg.h"
#include "AdvanceDialog.h"
#include "afxdialogex.h"
#define _CRT_SECURE_NO_WARNINGS
#pragma warning(disable:4996)

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


class CAboutDlg : public CDialogEx{
	public:
		CAboutDlg();

		#ifdef AFX_DESIGN_TIME
			enum { IDD = IDD_ABOUTBOX };
		#endif

	protected:
		virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	protected:
		DECLARE_MESSAGE_MAP()
};



CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX) {	}



void CAboutDlg::DoDataExchange(CDataExchange* pDX){
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()



CMsgSenderDlg::CMsgSenderDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_MSGSENDER_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	if (WSAStartup(MAKEWORD(2, 2), &m_WsaData) < 0) {
		MessageBox(L"WSA Failed to initialized!", L"Failed", MB_OK | MB_ICONERROR);
		WSACleanup();
		exit(EXIT_FAILURE);
	}

	m_strAdvancePort = 9909;
	m_bIsClientWindow = false;
	m_cBuff[255] = { 0, };
}



void CMsgSenderDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT_MSG, m_InputMsg);
	DDX_Control(pDX, IDC_EDIT_IP, m_InputAddress);
	DDX_Control(pDX, IDC_EDIT_PORT, m_InputPort);
	DDX_Control(pDX, IDC_LIST_DISPLAY, m_displayBox);
	DDX_Control(pDX, IDC_BUTTON_CONNECT, m_ButtonConnect);
	DDX_Control(pDX, IDC_BUTTON_ADVANCE, m_ButtonAdvance);
	DDX_Control(pDX, IDC_BUTTON_DISCONNECT, m_ButtonDisconnect);
	DDX_Control(pDX, ID_SEND, m_ButtonSend);
}



BEGIN_MESSAGE_MAP(CMsgSenderDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(ID_SEND, &CMsgSenderDlg::OnBnClickedSend)
	ON_BN_CLICKED(IDC_BUTTON_CLEARCHAT, &CMsgSenderDlg::OnBnClickedButtonClearchat)
	ON_BN_CLICKED(IDC_BUTTON_CONNECT, &CMsgSenderDlg::OnBnClickedButtonConnect)
	ON_BN_CLICKED(ID_CANCEL, &CMsgSenderDlg::OnBnClickedCancel)
	ON_BN_CLICKED(IDC_BUTTON_ADVANCE, &CMsgSenderDlg::OnBnClickedButtonAdvance)
	ON_BN_CLICKED(IDC_BUTTON_DISCONNECT, &CMsgSenderDlg::OnBnClickedButtonDisconnect)
	ON_EN_CHANGE(IDC_EDIT_MSG, &CMsgSenderDlg::OnEnChangeEditMsg)
END_MESSAGE_MAP()



// CMsgSenderDlg message handlers
BOOL CMsgSenderDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon


	ShowWindow(SW_MINIMIZE);


	CStatic* pHeading = (CStatic*)GetDlgItem(IDC_STATIC_HEADING);
	FontStyling(m_FontHeading, 25, FW_MEDIUM, FALSE, FALSE, _T("Arial"), pHeading);

	CStatic* pHeadingClientInfo = (CStatic*)GetDlgItem(IDC_STATIC_HEADING_CLIENTINFO);
	FontStyling(m_FontHeadingClientInfo, 17, FW_MEDIUM, FALSE, FALSE, _T("Arial"), pHeadingClientInfo);

	CStatic* pHeadingSettings = (CStatic*)GetDlgItem(IDC_STATIC_HEADING_SETTINGS);
	FontStyling(m_FontHeadingSettings , 17, FW_MEDIUM, FALSE, FALSE, _T("Arial"), pHeadingSettings);

	CStatic* pTitleReceived = (CStatic*)GetDlgItem(IDC_STATIC_TITLE_RECEIVED);
	FontStyling(m_FontTitleReceived , 17, FW_MEDIUM, FALSE, FALSE, _T("Arial"), pTitleReceived);

	CStatic* pTitleSend = (CStatic*)GetDlgItem(IDC_STATIC_TITLE_SEND);
	FontStyling(m_FontTitleSend , 17, FW_MEDIUM, FALSE, FALSE, _T("Arial"), pTitleSend);

	CStatic* pTitleIP = (CStatic*)GetDlgItem(IDC_STATIC_TITLE_IP);
	FontStyling(m_FontTitleIP, 17, FW_MEDIUM, FALSE, FALSE, _T("Arial"), pTitleIP);

	CStatic* pTitlePort = (CStatic*)GetDlgItem(IDC_STATIC_TITLE_PORT);
	FontStyling(m_FontTitlePort, 17, FW_MEDIUM, FALSE, FALSE, _T("Arial"), pTitlePort);

	CButton* pButtonConnect = (CButton*)GetDlgItem(IDC_BUTTON_CONNECT);
	FontStyling(m_FontButtonConnect, 17, FW_MEDIUM, FALSE, FALSE, _T("Arial"), nullptr, pButtonConnect);

	CButton* pButtonClear = (CButton*)GetDlgItem(IDC_BUTTON_CLEARCHAT);
	FontStyling(m_FontButtonClear, 17, FW_MEDIUM, FALSE, FALSE, _T("Arial"), nullptr, pButtonClear);

	CButton* pButtonAdvance = (CButton*)GetDlgItem(IDC_BUTTON_ADVANCE);
	FontStyling(m_FontButtonAdvance, 17, FW_MEDIUM, FALSE, FALSE, _T("Arial"), nullptr, pButtonAdvance);

	CButton* pButtonSend = (CButton*)GetDlgItem(ID_SEND);
	FontStyling(m_FontButtonSend, 17, FW_MEDIUM, FALSE, FALSE, _T("Arial"), nullptr, pButtonSend);

	CButton* pButtonClose = (CButton*)GetDlgItem(ID_CANCEL);
	FontStyling(m_FontButtonClose, 17, FW_MEDIUM, FALSE, FALSE, _T("Arial"), nullptr, pButtonClose);

	StartServer();

	m_ButtonSend.EnableWindow(false);

	return TRUE;  // return TRUE  unless you set the focus to a control
}



void CMsgSenderDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX){
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}



// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CMsgSenderDlg::OnPaint()
{
	CPaintDC dc2(this);
	CRect rect2;
	GetClientRect(&rect2);
	CBrush brush;
	brush.CreateSolidBrush(RGB(180, 180, 180));
	dc2.FillRect(&rect2, &brush);

	if (IsIconic()){
		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc2.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect2;
		GetClientRect(&rect2);
		int x = (rect2.Width() - cxIcon + 1) / 2;
		int y = (rect2.Height() - cyIcon + 1) / 2;
		// Draw the icon
		dc2.DrawIcon(x, y, m_hIcon);
	}
	else{
		CDialogEx::OnPaint();
	}

	
	GetDlgItem(IDC_STATIC_IMAGE_BOX)->GetClientRect(rect2);
	CImage image;
	image.Load(L"D:\\Coading\\Socket\\MsgSender\\res\\OditekLogo-removebg-preview.jpg");
	CBitmap m_bitmap;
	m_bitmap.Attach(image.Detach());
	CDC memoryDC;
	memoryDC.CreateCompatibleDC(&dc2);
	memoryDC.SelectObject(m_bitmap);
	BITMAP bmp;
	m_bitmap.GetBitmap(&bmp);
	dc2.SetStretchBltMode(COLORONCOLOR);
	dc2.StretchBlt(170, -8, rect2.Width(), rect2.Height(), &memoryDC, 0, 0, bmp.bmWidth, bmp.bmHeight, SRCCOPY);
}



HBRUSH CMsgSenderDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	if (pWnd->GetDlgCtrlID() == IDC_STATIC_HEADING ||
		pWnd->GetDlgCtrlID() == IDC_STATIC_HEADING_CLIENTINFO ||
		pWnd->GetDlgCtrlID() == IDC_STATIC_TITLE_RECEIVED ||
		pWnd->GetDlgCtrlID() == IDC_STATIC_TITLE_SEND ||
		pWnd->GetDlgCtrlID() == IDC_STATIC_HEADING_SETTINGS ||
		pWnd->GetDlgCtrlID() == IDC_STATIC_TITLE_IP ||
		pWnd->GetDlgCtrlID() == IDC_STATIC_IMAGE_BOX ||
		pWnd->GetDlgCtrlID() == IDC_STATIC_TITLE_PORT)
	{

		pDC->SetBkColor(RGB(180, 180, 180));
		//pDC->SetTextColor(RGB(255, 255, 255));
		hbr = CreateSolidBrush(RGB(180, 180, 180)); // Set the desired RGB color values
	}
	return hbr;
}



// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CMsgSenderDlg::OnQueryDragIcon(){
	return static_cast<HCURSOR>(m_hIcon);
}



/*==========================================================================================*/
/*=============================| FUNCTION FOR FONT STYLING |================================*/
/*==========================================================================================*/
void CMsgSenderDlg::FontStyling(CFont& font, int fontSize, int fontWeight, bool italic,
	bool underline, LPCTSTR fontName, CStatic* pStatic,
	CButton* pButton) {
	font.CreateFont(
		fontSize,                   // Font size
		0,                          // Font width (0 for default)
		0,                          // Font rotation angle
		0,                          // Font escapement angle
		fontWeight,                 // Font weight
		italic,                     // Italic
		underline,                  // Underline
		0,                          // Strikeout
		ANSI_CHARSET,               // Character set
		OUT_DEFAULT_PRECIS,         // Output precision
		CLIP_DEFAULT_PRECIS,        // Clipping precision
		DEFAULT_QUALITY,            // Output quality
		DEFAULT_PITCH | FF_DONTCARE,// Pitch and family
		fontName                    // Font name
	);
	if (pStatic != nullptr) pStatic->SetFont(&font);

	else if (pButton != nullptr) pButton->SetFont(&font);

}



/*==========================================================================================*/
/*=============================| FUNCTION FOR STARTING SERVER |=============================*/
/*==========================================================================================*/
bool CMsgSenderDlg::StartServer(int p_iPort) {

	int l_iRetVal;

	m_iServerSocket = socket(AF_INET, SOCK_STREAM, 0);
	if (m_iServerSocket < 0) {
		MessageBox(L"Failed to open socket", L"Failed", MB_OK | MB_ICONERROR);
		return false;
	}

	m_structServerSockAddr.sin_family = AF_INET;
	m_structServerSockAddr.sin_port = p_iPort;
	m_structServerSockAddr.sin_addr.s_addr = INADDR_ANY;
	memset(&m_structServerSockAddr.sin_zero, 0, 8);

	l_iRetVal = bind(m_iServerSocket, (sockaddr*)&m_structServerSockAddr, sizeof(sockaddr));
	if (l_iRetVal < 0) {
		MessageBox(L"Failed to bind with local port", L"Failed", MB_OK | MB_ICONERROR);
		return false;
	}

	l_iRetVal = listen(m_iServerSocket, 1);
	if (l_iRetVal < 0) {
		MessageBox(L"Failed to listen from the local port", L"Failed", MB_OK | MB_ICONERROR);
		return false;
	}	
	AfxBeginThread(ProcessingRequest, this);
	return true;
}



/*==========================================================================================*/
/*========================| THREAD FUNCTION FOR REQUEST PROCESSING |========================*/
/*==========================================================================================*/
UINT CMsgSenderDlg::ProcessingRequest(LPVOID pParam){

	CMsgSenderDlg* l_pcThisClass = reinterpret_cast<CMsgSenderDlg*>(pParam);
	CString l_strDateTime, l_strMessage;
	int l_iLength = sizeof(sockaddr);
	int l_iByteReceived;

	l_pcThisClass->m_iServerSideClientSocket = accept(l_pcThisClass->m_iServerSocket, NULL, &l_iLength);
	if (l_pcThisClass->m_iServerSideClientSocket > 0) {
		l_pcThisClass->MessageBox(L"Connected with the Client", L"success", MB_OK | MB_ICONASTERISK);
		l_pcThisClass->m_ButtonConnect.EnableWindow(false);
	}
	while (true){
		l_iByteReceived = recv(l_pcThisClass->m_iServerSideClientSocket, l_pcThisClass->m_cBuff, 255, 0);

		l_strMessage = l_pcThisClass->m_cBuff;

		l_strDateTime = CTime::GetCurrentTime().Format("%d.%m.%y %H:%M");

		l_strMessage.Format((L"[ ") + (l_strDateTime)+(L" ] ") + (l_strMessage)+("\n"));

		if(l_iByteReceived > 0)
			l_pcThisClass->m_displayBox.AddString(l_strMessage);
	}
	return 0;
}



/*==========================================================================================*/
/*============================| FUNCTION FOR CLIENT CONECTION |=============================*/
/*==========================================================================================*/
bool CMsgSenderDlg::ClientConnection(const string p_strIpAddress, int p_iPort) {

	int l_iRetVal; 

	m_iClientSocket = socket(AF_INET, SOCK_STREAM, 0);
	if (m_iClientSocket < 0) {
		MessageBox(L"Failed to opened socket", L"Failed", MB_OK | MB_ICONERROR);
		return false;
	}

	m_structClientSockAddr.sin_family = AF_INET;
	m_structClientSockAddr.sin_port = p_iPort;
	m_structClientSockAddr.sin_addr.s_addr = inet_addr(p_strIpAddress.c_str());

	l_iRetVal = connect(m_iClientSocket, (sockaddr*)&m_structClientSockAddr, sizeof(sockaddr));
	if (l_iRetVal < 0) {
		MessageBox(L"Failed to connect with server", L"Failed", MB_OK | MB_ICONERROR);
		return false;
	}
	else {
		MessageBox(L"Connected with the server", L"Success", MB_OK | MB_ICONASTERISK);

		AfxBeginThread(ClientReceivingMsg, this);


		closesocket(m_iServerSocket);
		m_ButtonConnect.EnableWindow(false);
		m_ButtonAdvance.EnableWindow(false);

		return true;
	}
}



/*==========================================================================================*/
/*===================| THREAD FUNCTION FOR CLIENT SIDE DATA RECEIVING |=====================*/
/*==========================================================================================*/
UINT CMsgSenderDlg::ClientReceivingMsg(LPVOID pParam) {

	CMsgSenderDlg* l_pcThisClass = reinterpret_cast<CMsgSenderDlg*>(pParam);
	int l_iByteReceived;
	CString l_strMessage, l_strDateTime;
	
	while (true){
		l_iByteReceived = recv(l_pcThisClass->m_iClientSocket, l_pcThisClass->m_cBuff, 255, 0);

		l_strMessage = l_pcThisClass->m_cBuff;

		l_strDateTime = CTime::GetCurrentTime().Format("%d.%m.%y %H:%M");

		l_strMessage.Format((L"[ ") + (l_strDateTime)+(L" ] ") + (l_strMessage)+("\n"));

		if (l_iByteReceived > 0) {
			l_pcThisClass->m_displayBox.AddString(l_strMessage);
		}
	}
	
	return 0;
}




/*==========================================================================================*/
/*==================================| SEND BUTTON HANDLER |=================================*/
/*==========================================================================================*/
void CMsgSenderDlg::OnBnClickedSend(){
	CString l_strInputMsg, l_strDateTime;
	string l_sMsgToBeSend;
	int l_iLength, l_iBufferSize;

	m_InputMsg.GetWindowTextW(l_strInputMsg);

	l_sMsgToBeSend = CT2A(l_strInputMsg);

	if (m_bIsClientWindow) {
		send(m_iClientSocket, l_sMsgToBeSend.c_str(), 255, 0);
	}

	if (!m_bIsClientWindow) {
		send(m_iServerSideClientSocket, l_sMsgToBeSend.c_str(), 255, 0);
	}

	m_InputMsg.SetWindowText(_T(""));
	l_strDateTime = CTime::GetCurrentTime().Format("%d.%m.%y %H:%M");
	l_strInputMsg.Format((L"[ ") + (l_strDateTime)+(L" ] ") + (L"[ Self ] ") + (l_strInputMsg)+("\n"));
	m_displayBox.AddString(l_strInputMsg);

}



/*==========================================================================================*/
/*================================| CLEAR BUTTON HANDLER |==================================*/
/*==========================================================================================*/
void CMsgSenderDlg::OnBnClickedButtonClearchat(){
	m_displayBox.ResetContent();
	m_InputMsg.SetWindowText(_T(""));
}



/*==========================================================================================*/
/*================================| CONNECT BUTTON HANDLER |================================*/
/*==========================================================================================*/
void CMsgSenderDlg::OnBnClickedButtonConnect(){
	CString l_stringInputIpAddress, l_stringInputPort;
	string l_SInputAddress;
	int l_IInuptPort;

	m_InputAddress.GetWindowTextW(l_stringInputIpAddress);
	m_InputPort.GetWindowTextW(l_stringInputPort);

	l_SInputAddress = CT2A(l_stringInputIpAddress);
	l_IInuptPort = _wtoi(l_stringInputPort);

	m_bIsClientWindow = ClientConnection(l_SInputAddress, l_IInuptPort);
}



/*==========================================================================================*/
/*================================| CANCEL BUTTON HANDLER |=================================*/
/*==========================================================================================*/
void CMsgSenderDlg::OnBnClickedCancel(){
	CDialogEx::OnOK();
}



/*==========================================================================================*/
/*=================================| ADVANCE BUTTON HANDLER |===============================*/
/*==========================================================================================*/
void CMsgSenderDlg::OnBnClickedButtonAdvance(){
	AdvanceDialog advanceDlg(nullptr, this);
	advanceDlg.DoModal();
}



/*==========================================================================================*/
/*================================| DISCONNECT BUTTON HANDLER |=============================*/
/*==========================================================================================*/
void CMsgSenderDlg::OnBnClickedButtonDisconnect(){
	/*if (m_bIsClientWindow) {

		StartServer();

		closesocket(m_iClientSocket);

		m_bIsClientWindow = false;
		m_ButtonConnect.EnableWindow(true);
		m_ButtonAdvance.EnableWindow(true);
	}
	else{
		send(m_iServerSideClientSocket, "^^", 255, 0);
		closesocket(m_iServerSocket);
		StartServer(m_strAdvancePort);
		m_ButtonConnect.EnableWindow(true);
	}*/
}


void CMsgSenderDlg::OnEnChangeEditMsg(){
	CString l_strInputTxt;

	m_InputMsg.GetWindowText(l_strInputTxt);
	l_strInputTxt.Trim();
	if (l_strInputTxt.IsEmpty()) m_ButtonSend.EnableWindow(false);
	else m_ButtonSend.EnableWindow(true);
}
