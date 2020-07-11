
// DCMBrowserDlg.h: 头文件
//

#pragma once
#include<vector>

// CDCMBrowserDlg 对话框
class CDCMBrowserDlg : public CDialog
{
// 构造
public:
	CDCMBrowserDlg(CWnd* pParent = nullptr);	// 标准构造函数

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DCMBROWSER_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnLvnItemchangedList1(NMHDR *pNMHDR, LRESULT *pResult);
	CListCtrl m_list;
	POINT Old;
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnFileOpen();

public:
	DcmFileFormat dfile;
	OFCondition result;
	string filepath;
	int row;
	int column;
	vector<string> spaceBefore{ "", "", "            ", "                        " };
	vector<string> sqSpace{ "", "    ", "                " };
	string sqStr = "Sequence Items:";
	CEdit m_edit;
	afx_msg void OnNMDblclkList1(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnEnKillfocusEdit1();
	afx_msg void OnSaveFile();

public:
	//显示文件内容
	void OpenFile();
	//修改数据
	void ModifyData();
	//tag字符串转换成tag
	DcmTagKey strtagTotag(CListCtrl *list, int index);
	//向list中插入数据行
	void InsertIntoList(int &index, string strTag, DcmTag tag, CString strLength, OFString value, int level);

	//处理OB
	CString valueOfOB(OFString str);

	//处理SQ
	void InsertSequenceItems(DcmTag tag, int &index, int &cnt);

	//存放修改的tag及修改值
	vector<DcmTagKey> modifyTag;
	vector<CString> modifyValue;

	afx_msg void OnSaveAsFile();
	afx_msg void OnDropFiles(HDROP hDropInfo);

public:
	HACCEL hAccel;
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnSearchModify();
	afx_msg void OnCustomdrawList1(NMHDR *pNMHDR, LRESULT *pResult);
};

