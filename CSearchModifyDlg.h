#pragma once

#include <vector>
#include "DCMBrowserDlg.h"
#include "CPublic.h"
// CSearchModifyDlg 对话框

class CSearchModifyDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CSearchModifyDlg)

public:
	CSearchModifyDlg(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CSearchModifyDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_SEARCHANDMODIFY };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CListCtrl *list;
	CDCMBrowserDlg* mainFrame = (CDCMBrowserDlg*)CPublic::mythis;
	afx_msg void OnBnClickedButtonSm();
};
