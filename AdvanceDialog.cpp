// AdvanceDialog.cpp : implementation file
//

#include "pch.h"
#include "MsgSender.h"
#include "AdvanceDialog.h"
#include "afxdialogex.h"


// AdvanceDialog dialog

IMPLEMENT_DYNAMIC(AdvanceDialog, CDialogEx)

AdvanceDialog::AdvanceDialog(CWnd* pParent, CMsgSenderDlg* p_obj)
	: CDialogEx(IDD_ADVANCE_DIALOG, pParent)
{
	this->m_obj = p_obj;
	this->m_cstrPort = L"9909";
}

AdvanceDialog::~AdvanceDialog()
{
}

void AdvanceDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT_NEW_IP, m_newPort);
}


BEGIN_MESSAGE_MAP(AdvanceDialog, CDialogEx)
	ON_WM_PAINT()
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(ID_OK, &AdvanceDialog::OnBnClickedOk)
	ON_BN_CLICKED(ID_CANCEL, &AdvanceDialog::OnBnClickedCancel)
END_MESSAGE_MAP()


BOOL AdvanceDialog::OnInitDialog() {
	SetDlgItemTextW(IDC_EDIT_NEW_IP, m_cstrPort);
	return TRUE;
}


void AdvanceDialog::OnPaint()
{
	CPaintDC dc2(this);
	CRect rect2;
	GetClientRect(&rect2);
	CBrush brush;
	brush.CreateSolidBrush(RGB(200, 200, 200));
	dc2.FillRect(&rect2, &brush);
}

HBRUSH AdvanceDialog::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	if (pWnd->GetDlgCtrlID() == IDC_STATIC_ADVANCE_PORT_TITLE){

		pDC->SetBkColor(RGB(200, 200, 200));
		hbr = CreateSolidBrush(RGB(200, 200, 200)); // Set the desired RGB color values
	}
	return hbr;
}

void AdvanceDialog::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	CString l_csPort;
	int l_iPort;
	UpdateData(TRUE);
	m_newPort.GetWindowTextW(l_csPort);
	l_iPort = _wtoi(l_csPort);

	/*if (m_obj->StartServer(l_iPort)) {
		m_cstrPort = l_csPort;
		AfxBeginThread(m_obj->ProcessingRequest, m_obj);
	}*/

	m_obj->StartServer(l_iPort);
	AfxBeginThread(m_obj->ProcessingRequest, m_obj);

	CDialogEx::OnOK();
}


void AdvanceDialog::OnBnClickedCancel()
{
	// TODO: Add your control notification handler code here
	CDialogEx::OnOK();
}
