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


// CMsgSenderDlg dialog



CMsgSenderDlg::CMsgSenderDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_MSGSENDER_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	if (WSAStartup(MAKEWORD(2, 2), &m_WsaData) < 0) {
		MessageBox(L"WSA Failed to initialized!", L"Failed", MB_OK | MB_ICONERROR);
		WSACleanup();
		exit(EXIT_FAILURE);
	}

	m_sTimeVal.tv_sec = 1;
	m_sTimeVal.tv_usec = 0;
	m_bIfConnectButtonClicked = false;
	buff[255] = { 0, };
}

void CMsgSenderDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT_MSG, m_msg);
	DDX_Control(pDX, IDC_EDIT_IP, m_CEInputAddress);
	DDX_Control(pDX, IDC_EDIT_PORT, m_CEInputPort);
	DDX_Control(pDX, IDC_LIST_DISPLAY, m_displayBox);
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

	//ShowWindow(SW_MAXIMIZE);

	ShowWindow(SW_MINIMIZE);
	// TODO: Add extra initialization here


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

	if(StartServer())	AfxBeginThread(ProcessingRequest, this);

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CMsgSenderDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
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
	if (IsIconic())
	{
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
	else
	{
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
HCURSOR CMsgSenderDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



/********************************************************************************************/
/******************************* FUNCTION FOR STARTING SERVER *******************************/
/********************************************************************************************/
bool CMsgSenderDlg::StartServer(int l_iPort) {
	int l_IRetVal;

	m_ServerSocket = socket(AF_INET, SOCK_STREAM, 0);
	if (m_ServerSocket < 0) {
		MessageBox(L"Failed to open socket", L"Failed", MB_OK | MB_ICONERROR);
		return false;
	}

	m_ServerSockAddr.sin_family = AF_INET;
	m_ServerSockAddr.sin_port = l_iPort;
	m_ServerSockAddr.sin_addr.s_addr = INADDR_ANY;
	memset(&m_ServerSockAddr.sin_zero, 0, 8);

	l_IRetVal = bind(m_ServerSocket, (sockaddr*)&m_ServerSockAddr, sizeof(sockaddr));
	if (l_IRetVal < 0) {
		MessageBox(L"Failed to bind with local port", L"Failed", MB_OK | MB_ICONERROR);
		return false;
	}

	l_IRetVal = listen(m_ServerSocket, 1);
	if (l_IRetVal < 0) {
		MessageBox(L"Failed to listen from the local port", L"Failed", MB_OK | MB_ICONERROR);
		return false;
	}	
	return true;
}



/**********************************************************************************************/
/************************** THREAD FUNCTION FOR REQUEST PROCESSING ****************************/
/**********************************************************************************************/
UINT __cdecl CMsgSenderDlg::ProcessingRequest(LPVOID pParam){
	CMsgSenderDlg* pThisClass = reinterpret_cast<CMsgSenderDlg*>(pParam);
	CString l_csDateTime;
	int l_iLength = sizeof(struct sockaddr);
	pThisClass->m_iServerSideClientSocket = accept(pThisClass->m_ServerSocket, NULL, &l_iLength);
	if (pThisClass->m_iServerSideClientSocket < 0) {
		pThisClass->MessageBox(L"ERRRRRR", L"success", MB_OK | MB_ICONERROR);
	}
	else {
		pThisClass->MessageBox(L"Connected with the Client", L"success", MB_OK | MB_ICONASTERISK);
	}
	while (true)
	{
		int bytereceived;
		CString l_string_msg;
		bytereceived = recv(pThisClass->m_iServerSideClientSocket, pThisClass->buff, 255, 0);
		pThisClass->buff[bytereceived + 1] = '\0';
		l_string_msg = pThisClass->buff;
		l_csDateTime = CTime::GetCurrentTime().Format("%d.%m.%y %H:%M");
		l_string_msg.Format((L"[ ") + (l_csDateTime)+(L" ] ") + (l_string_msg)+("\n"));
		if(bytereceived > 0)
			pThisClass->m_displayBox.AddString(l_string_msg);
	}
	return 0;
}



/*********************************************************************************************/
/****************************** FUNCTION FOR CLIENT CONECTION ********************************/
/*********************************************************************************************/
void CMsgSenderDlg::ClientConnection(const string p_SAddress, int p_IPort) {
	int l_IRetVal;

	m_ClientSocket = socket(AF_INET, SOCK_STREAM, 0);
	if (m_ClientSocket < 0) {
		MessageBox(L"Failed to opened socket", L"Failed", MB_OK | MB_ICONERROR);
	}

	m_ClientSockAddr.sin_family = AF_INET;
	m_ClientSockAddr.sin_port = p_IPort;
	m_ClientSockAddr.sin_addr.s_addr = inet_addr(p_SAddress.c_str());

	l_IRetVal = connect(m_ClientSocket, (sockaddr*)&m_ClientSockAddr, sizeof(m_ClientSockAddr));
	if (l_IRetVal < 0) {
		MessageBox(L"Failed to connect with server", L"Failed", MB_OK | MB_ICONERROR);
	}
	else {
		m_bIfConnectButtonClicked = true;
		MessageBox(L"Connected with the server", L"Success", MB_OK | MB_ICONASTERISK);
		CButton* pButtonConnect = (CButton*)GetDlgItem(IDC_BUTTON_CONNECT);
		pButtonConnect->EnableWindow(false);
		CString l_string_msg;
		int bytereceived;

		AfxBeginThread(ReceivingMsg, this);
	}
}



/**********************************************************************************************/
/********************** THREAD FUNCTION FOR CLIENT SIDE DATA RECEIVING ************************/
/**********************************************************************************************/
UINT __cdecl CMsgSenderDlg::ReceivingMsg(LPVOID pParam) {
	CMsgSenderDlg* pThisClass = reinterpret_cast<CMsgSenderDlg*>(pParam);
	int bytereceived;
	CString l_string_msg;
	CString l_csDateTime;
	if (pThisClass->m_bIfConnectButtonClicked) {
		while (true)
		{
			bytereceived = recv(pThisClass->m_ClientSocket, pThisClass->buff, 255, 0);
			pThisClass->buff[bytereceived + 1] = '\0';
			l_string_msg = pThisClass->buff;
			l_csDateTime = CTime::GetCurrentTime().Format("%d.%m.%y %H:%M");
			l_string_msg.Format((L"[ ") + (l_csDateTime)+(L" ] ") + (l_string_msg)+("\n"));
			if (bytereceived > 0)
				pThisClass->m_displayBox.AddString(l_string_msg);
		}
	}
	return 0;
}



/*********************************************************************************************/
/******************************** FUNCTION FOR FONT STYLING **********************************/
/*********************************************************************************************/
void CMsgSenderDlg::FontStyling(CFont& font, int fontSize, int fontWeight, bool italic, 
								bool underline, LPCTSTR fontName, CStatic* pStatic, 
								CButton* pButton){
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
	if(pStatic != nullptr) pStatic->SetFont(&font);
	else if(pButton != nullptr) pButton->SetFont(&font);
}



/*********************************************************************************************/
/************************************ SEND BUTTON HADLER *************************************/
/*********************************************************************************************/
void CMsgSenderDlg::OnBnClickedSend(){
	CString l_csInputMsg, l_csDateTime, l_string_msg;
	m_msg.GetWindowTextW(l_csInputMsg);

	l_csDateTime = CTime::GetCurrentTime().Format("%d.%m.%y %H:%M");
	m_msg.GetWindowTextW(l_string_msg);
	l_string_msg.Format((L"[ ") + (l_csDateTime)+(L" ] ")+ (L"[ Self ] ") + (l_string_msg)+("\n"));
	m_displayBox.AddString(l_string_msg);

	int length = l_csInputMsg.GetLength();
	int bufferSize = min(length + 1, 255);
	wchar_t* buffer = l_csInputMsg.GetBuffer(bufferSize);
	wcstombs_s(nullptr, buff, buffer, _TRUNCATE);
	l_csInputMsg.ReleaseBuffer();

	if (m_bIfConnectButtonClicked) {
		send(m_ClientSocket, buff, 255, 0);
	}

	if (!m_bIfConnectButtonClicked) {   
		send(m_iServerSideClientSocket, buff, 255, 0);
	}
}


/*********************************************************************************************/
/*********************************** CLEAR BUTTON HADLER *************************************/
/*********************************************************************************************/
void CMsgSenderDlg::OnBnClickedButtonClearchat(){
	m_displayBox.ResetContent();
	m_msg.SetWindowText(_T(""));  
}



/*********************************************************************************************/
/*********************************** CONNECT BUTTON HADLER ***********************************/
/*********************************************************************************************/
void CMsgSenderDlg::OnBnClickedButtonConnect(){
	CString l_CSInputAddress, l_CSInputPort;
	string l_SInputAddress;
	int l_IInuptPort;

	m_CEInputAddress.GetWindowTextW(l_CSInputAddress);
	m_CEInputPort.GetWindowTextW(l_CSInputPort);

	l_SInputAddress = CT2A(l_CSInputAddress.GetString());
	l_IInuptPort = _wtoi(l_CSInputPort);

	ClientConnection(l_SInputAddress, l_IInuptPort);
}

void CMsgSenderDlg::OnBnClickedCancel()
{
	// TODO: Add your control notification handler code here
	CDialogEx::OnOK();
}


void CMsgSenderDlg::OnBnClickedButtonAdvance()
{
	// TODO: Add your control notification handler code here
	AdvanceDialog advanceDlg(nullptr, this);
	advanceDlg.DoModal();
}
