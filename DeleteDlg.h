// DeleteDlg.h : header file
//

#if !defined(AFX_DELETEDLG_H__EAECFCAE_41AA_4154_8CC5_FCF3243C65C9__INCLUDED_)
#define AFX_DELETEDLG_H__EAECFCAE_41AA_4154_8CC5_FCF3243C65C9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CDeleteDlg dialog

class CDeleteDlg : public CDialog
{
// Construction
public:
	CDeleteDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CDeleteDlg)
	enum { IDD = IDD_DELETE_DIALOG };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDeleteDlg)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL
    CString str[100];
	int i; // 선택 파일 숫자
// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CDeleteDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnButtonF();
	afx_msg void OnButtonE();
	afx_msg void OnButtonD();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DELETEDLG_H__EAECFCAE_41AA_4154_8CC5_FCF3243C65C9__INCLUDED_)
