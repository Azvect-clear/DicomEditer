// CSearchModifyDlg.cpp: 实现文件
//

#include "stdafx.h"
#include "DCMBrowser.h"
#include "CSearchModifyDlg.h"
#include "afxdialogex.h"
#include "DCMBrowserDlg.h"
#include "CPublic.h"

// CSearchModifyDlg 对话框

IMPLEMENT_DYNAMIC(CSearchModifyDlg, CDialogEx)

CSearchModifyDlg::CSearchModifyDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG_SEARCHANDMODIFY, pParent)
{

}

CSearchModifyDlg::~CSearchModifyDlg()
{
}

void CSearchModifyDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CSearchModifyDlg, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON_SM, &CSearchModifyDlg::OnBnClickedButtonSm)
END_MESSAGE_MAP()


// CSearchModifyDlg 消息处理程序

void CSearchModifyDlg::OnBnClickedButtonSm()
{
	// TODO: 在此添加控件通知处理程序代码
	CString searchValue;
	CString newValue;
	GetDlgItem(IDC_EDIT1)->GetWindowText(searchValue);
	GetDlgItem(IDC_EDIT2)->GetWindowText(newValue);
	list = &mainFrame->m_list;
	for (int i = 0; i < list->GetItemCount(); i++)
	{
		CString cur = list->GetItemText(i, 4);
		if (cur == searchValue)
		{
			list->SetItemText(i, 4, newValue);
			mainFrame->m_edit.SetWindowTextA((LPCSTR)newValue);
			mainFrame->modifyTag.push_back(mainFrame->strtagTotag(list, i));
			mainFrame->modifyValue.push_back(newValue);
		}
	}
	SetDlgItemText(IDC_EDIT1, "");
	SetDlgItemText(IDC_EDIT2, "");
}



