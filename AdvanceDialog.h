#pragma once
#include "MsgSenderDlg.h"

// AdvanceDialog dialog

class AdvanceDialog : public CDialogEx{

	DECLARE_DYNAMIC(AdvanceDialog)

	public:
		AdvanceDialog(CWnd* pParent = nullptr, CMsgSenderDlg* p_obj = nullptr);
		virtual ~AdvanceDialog();
		#ifdef AFX_DESIGN_TIME
			enum { IDD = IDD_ADVANCE_DIALOG };
		#endif

		afx_msg void OnBnClickedOk();
		afx_msg void OnBnClickedCancel();

		CMsgSenderDlg* m_obj;
		CEdit m_newPort;
		int m_DisplayPort;
		

	protected:
		virtual void DoDataExchange(CDataExchange* pDX);
		afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
		afx_msg void OnPaint();

		DECLARE_MESSAGE_MAP()		
};
