
// DCMBrowserDlg.cpp: 实现文件
//


#include "stdafx.h"
#include "DCMBrowser.h"
#include "DCMBrowserDlg.h"
#include "afxdialogex.h"
#include "CSearchModifyDlg.h"
#include "CPublic.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CDCMBrowserDlg 对话框



CDCMBrowserDlg::CDCMBrowserDlg(CWnd* pParent /*=nullptr*/)
	: CDialog(IDD_DCMBROWSER_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CDCMBrowserDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_list);
	DDX_Control(pDX, IDC_EDIT1, m_edit);
}

BEGIN_MESSAGE_MAP(CDCMBrowserDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST1, &CDCMBrowserDlg::OnLvnItemchangedList1)
	ON_WM_SIZE()
	ON_COMMAND(ID_32771, &CDCMBrowserDlg::OnFileOpen)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST1, &CDCMBrowserDlg::OnNMDblclkList1)
	ON_EN_KILLFOCUS(IDC_EDIT1, &CDCMBrowserDlg::OnEnKillfocusEdit1)
	ON_COMMAND(ID_32772, &CDCMBrowserDlg::OnSaveFile)
	ON_COMMAND(ID_Menu, &CDCMBrowserDlg::OnSaveAsFile)
	ON_WM_DROPFILES()
	ON_COMMAND(ID_32786, &CDCMBrowserDlg::OnSearchModify)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_LIST1, &CDCMBrowserDlg::OnCustomdrawList1)
END_MESSAGE_MAP()


// CDCMBrowserDlg 消息处理程序

BOOL CDCMBrowserDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
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

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码

	CPublic::mythis = this;

	//Win10下拖拽文件处理
	ChangeWindowMessageFilter(WM_DROPFILES, MSGFLT_ADD);
	ChangeWindowMessageFilter(0x0049, MSGFLT_ADD);

	//添加菜单栏
	CMenu menu;
	menu.LoadMenu(IDR_MENU1); 
	SetMenu(&menu);


	//列表头初始化
	m_list.ModifyStyle(NULL, LVS_REPORT, 0);
	m_list.SetExtendedStyle(LVS_EX_FULLROWSELECT);
	while (m_list.DeleteColumn(0));
	m_list.InsertColumn(0, _T("Tag"), LVCFMT_LEFT, 180, 0);
	m_list.InsertColumn(1, _T("Tag Name"), LVCFMT_LEFT, 200, 1);
	m_list.InsertColumn(2, _T("VR"), LVCFMT_LEFT, 80, 1);
	m_list.InsertColumn(3, _T("Length"), LVCFMT_LEFT, 80, 1);
	m_list.InsertColumn(4, _T("Value"), LVCFMT_LEFT, 500, 1);

	//CFont m_font;
	//LOGFONT lf;
	//memset(&lf, 0, sizeof(LOGFONT)); 
	//lf.lfHeight = 18;
	//lf.lfWeight = FW_THIN;
	//strcpy_s(lf.lfFaceName, _T("Times New Roman"));
	//m_font.CreateFontIndirect(&lf);
	//m_list.SetFont(&m_font);

	//窗口自适应
	CRect rect;
	GetClientRect(&rect); //取客户区大小   
	Old.x = rect.right - rect.left;
	Old.y = rect.bottom - rect.top;

	//编辑框隐藏
	m_edit.ShowWindow(SW_HIDE);

	//快捷键
	hAccel = ::LoadAccelerators(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDR_ACCELERATOR2));

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CDCMBrowserDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CDCMBrowserDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CDCMBrowserDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CDCMBrowserDlg::OnLvnItemchangedList1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	*pResult = 0;
}

//窗口自适应
void CDCMBrowserDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);

	// TODO: 在此处添加消息处理程序代码
	if (nType == SIZE_RESTORED || nType == SIZE_MAXIMIZED)//窗体大小发生变动。处理程序
	{
		float fsp[2];
		POINT Newp; //获取现在对话框的大小
		CRect recta;
		GetClientRect(&recta); //取客户区大小   
		Newp.x = recta.right - recta.left;
		Newp.y = recta.bottom - recta.top;
		fsp[0] = (float)Newp.x / Old.x;
		fsp[1] = (float)Newp.y / Old.y;
		CRect Rect;
		int woc;
		CPoint OldTLPoint, TLPoint; //左上角
		CPoint OldBRPoint, BRPoint; //右下角
		HWND hwndChild = ::GetWindow(m_hWnd, GW_CHILD); //列出所有控件   
		while (hwndChild)
		{
			woc = ::GetDlgCtrlID(hwndChild);//取得ID
			GetDlgItem(woc)->GetWindowRect(Rect);
			ScreenToClient(Rect);
			OldTLPoint = Rect.TopLeft();
			TLPoint.x = long(OldTLPoint.x*fsp[0]);
			TLPoint.y = long(OldTLPoint.y*fsp[1]);
			OldBRPoint = Rect.BottomRight();
			BRPoint.x = long(OldBRPoint.x *fsp[0]);
			BRPoint.y = long(OldBRPoint.y *fsp[1]); //高度不可读的控件
			Rect.SetRect(TLPoint, BRPoint);
			GetDlgItem(woc)->MoveWindow(Rect, TRUE);
			hwndChild = ::GetWindow(hwndChild, GW_HWNDNEXT);
		}
		Old = Newp;
	}
}

//十进制整型转换成4位十六进制字符串
string decTohex(int num)
{ 
	stringstream input;
	string tmp;
	input << hex << num;
	input >> tmp;
	string result(4 - tmp.size(), '0');
	result += tmp;
	return result;
}

//tag字符串转换成tag
DcmTagKey CDCMBrowserDlg::strtagTotag(CListCtrl *list, int n)
{
	string strTag = list->GetItemText(n, 0);
	string strGtag = strTag.substr(1, 4);
	string strEtag = strTag.substr(7, 4);
	int GTag = strtol(strGtag.c_str(), NULL, 16);
	int ETag = strtol(strEtag.c_str(), NULL, 16);
	DcmTagKey tag(GTag, ETag);
	return tag;
}

//修改数据写入dcm文件
void CDCMBrowserDlg::ModifyData()
{
	DcmDataset *data = dfile.getDataset();
	for (int i = 0; i < modifyTag.size(); ++i)
	{
		delete data->remove(modifyTag[i]);
		data->putAndInsertString(modifyTag[i], modifyValue[i]);
	}
	modifyTag.clear();
	modifyValue.clear();
}

//处理OB
CString CDCMBrowserDlg::valueOfOB(OFString str)
{
	char *newstr = new char[str.size() + 1];
	strcpy_s(newstr, str.size() + 1, str.c_str());
	string tmpstr = newstr;
	size_t len = str.size();
	string res = "";
	int i = 0;
	while (i < len)
	{
		string p = tmpstr.substr(i, i + 2);
		int tmp = strtol(p.c_str(), nullptr, 16);
		i += 3;
		res += to_string(tmp);
		if (i < len)
			res += '\\';
	}
	return res.c_str();
}

//处理SQ
void CDCMBrowserDlg::InsertSequenceItems(DcmTag tag, int &index, int &cnt)
{
	DcmItem *sq;
	DcmDataset *data = dfile.getDataset();
	data->findAndGetSequenceItem(tag, sq);
	if (sq == nullptr)
		return;
	string tmpStr = sqSpace[1] + sqStr;
	m_list.InsertItem(index++, tmpStr.c_str());
	DcmObject *obj = sq->nextInContainer(NULL);
	while (obj != nullptr)
	{
		//标签对象
		DcmTag STag(obj->getGTag(), obj->getETag());
		OFString value;
		CString strLength;
		//获取值
		sq->findAndGetOFStringArray(STag, value);
		//获取长度
		strLength.Format(_T("%d"), obj->getLength());
		string tmpTag = "(" + decTohex(obj->getGTag()) + ", " + decTohex(obj->getETag()) + ")";
		//插入数据元素
		InsertIntoList(index, tmpTag, STag, strLength, value, 2);
		if (strcmp(STag.getVRName(), "SQ") == 0)
		{
			DcmItem *subItem;
			sq->findAndGetSequenceItem(STag, subItem);
			if (subItem == nullptr)
				return;
			string tmpStr = sqSpace[2] + sqStr;
			m_list.InsertItem(index++, tmpStr.c_str());
			DcmObject *subNext = subItem->nextInContainer(NULL);
			while (subNext != nullptr)
			{
				//标签对象
				DcmTag subTag(subNext->getGTag(), subNext->getETag());
				OFString subValue;
				CString subStrLength;
				//获取值
				subItem->findAndGetOFStringArray(subTag, subValue);
				//获取长度
				subStrLength.Format(_T("%d"), subNext->getLength());
				string tmpSubTag = "(" + decTohex(subNext->getGTag()) + ", " + decTohex(subNext->getETag()) + ")";
				//插入数据元素
				InsertIntoList(index, tmpSubTag, subTag, subStrLength, subValue, 3);
				subNext = subItem->nextInContainer(subNext);
			}
		}
		obj = sq->nextInContainer(obj);
	}
}

//向list插入数据行
void CDCMBrowserDlg::InsertIntoList(int &index, string strTag, DcmTag tag, CString strLength, OFString value, int level)
{
	//根据数据集的层级来定义输出格式
	strTag = spaceBefore[level] + strTag;
	//插入Tag的值
	m_list.InsertItem(index, strTag.c_str());
	
	CString tagName = tag.getTagName();
	string tmpTagName = CT2A(tagName.GetString());
	string strTagName = "";
	for (int i = tmpTagName.size() - 1; i >= 1; --i)
	{
		char letter = tmpTagName[i];
		char preLetter = tmpTagName[i - 1];
		strTagName = letter + strTagName;
		if (letter >= 'A' && letter <= 'Z' && preLetter >= 'a' && preLetter <= 'z')
		{
			strTagName = " " + strTagName;
		}
	}
	strTagName = tmpTagName[0] + strTagName;
	CString curTagName(strTagName.c_str());
	
	CString vrName = tag.getVRName();
	OFString cvalue = value;
	//VR为OB
	if (vrName == "OB")
	{
		cvalue = valueOfOB(value);
	}
	//私有数据（未定义）
	if (tag.isPrivate())
	{
		curTagName = "";
		vrName = "";
	}
	//插入Tag的描述
	m_list.SetItemText(index, 1, curTagName);
	//插入数据类型
	m_list.SetItemText(index, 2, vrName);
	//插入数据长度
	m_list.SetItemText(index, 3, strLength);
	//插入数据值
	m_list.SetItemText(index++, 4, cvalue.c_str());
}


//文件浏览
void CDCMBrowserDlg::OpenFile()
{
	if (m_list.GetItemCount() > 0)
		m_list.DeleteAllItems();
	//打开对应的路径文件
	result = dfile.loadFile(filepath.c_str());
	DcmDataset *data = dfile.getDataset();
	DcmMetaInfo *meta = dfile.getMetaInfo();
	DcmObject *metaNext = meta->nextInContainer(NULL);
	DcmObject *dataNext = data->nextInContainer(NULL);
	int index = 0;
	int cnt = 0;
	bool A_flag = true;
	//遍历文件元信息
	while (metaNext != nullptr)
	{
		//标签对象
		DcmTag tag(metaNext->getGTag(), metaNext->getETag());
		//if (A_flag && (metaNext->getGTag() > 2))
		//{
		//	DcmTag AETag(0x0002, 0x0016);
		//	string tmpTag("(0x0002, 0x0016)");
		//	char c_localName[128];
		//	gethostname(c_localName,128);
		//	OFString  localName(c_localName);
		//	CString strLength;
		//	strLength.Format(_T("%d"), ((localName.size()%2)?(localName.size()+1):localName.size()));
		//	InsertIntoList(index, tmpTag, tag, strLength, localName, 1);
		//	A_flag = false;
		//}
		OFString value;
		CString strLength;
		//获取长度
		strLength.Format(_T("%d"), metaNext->getLength());
		string tmpTag = "(" + decTohex(metaNext->getGTag()) + ", " + decTohex(metaNext->getETag()) + ")";

		//获取值
		if (metaNext->getLength() > 10000)
		{
			value = "< large value not displayed >";
		}
		else
		{
			meta->findAndGetOFStringArray(tag, value);
		}

		//插入数据元素
		InsertIntoList(index, tmpTag, tag, strLength, value, 1);
		if (strcmp(tag.getVRName(), "SQ") == 0)
		{
			InsertSequenceItems(tag, index, cnt);
		}
		metaNext = meta->nextInContainer(metaNext);
	}
	//根据Tag遍历所有数据
	while (dataNext != nullptr)
	{
		//标签对象
		DcmTag tag(dataNext->getGTag(), dataNext->getETag());
		OFString value;
		CString strLength;
		//获取长度
		strLength.Format(_T("%d"), dataNext->getLength());
		string tmpTag = "(" + decTohex(dataNext->getGTag()) + ", " + decTohex(dataNext->getETag()) + ")";

		//获取值
		if (dataNext->getLength() > 10000)
		{
			value = "< large value not displayed >";
		}
		else
		{
			data->findAndGetOFStringArray(tag, value);
		}
		
		//插入数据元素
		InsertIntoList(index, tmpTag, tag, strLength, value, 1);
		if (strcmp(tag.getVRName(),"SQ")==0)
		{
			InsertSequenceItems(tag, index, cnt);
		}
		dataNext = data->nextInContainer(dataNext);
	}
}

//打开文件对话框
void CDCMBrowserDlg::OnFileOpen()
{
	// 打开文件对话框
	//设置过滤器
	TCHAR dcmFile[] = _T("DICOM文件(*.dcm; *.dic)|*.dcm; *.dic||");
	//构建打开文件对话框
	CFileDialog fileDlg(TRUE, _T("dcm"), NULL, 0, dcmFile, this);
	//显示打开文件对话框
	if (IDOK == fileDlg.DoModal())
	{
		filepath = fileDlg.GetPathName();
		OpenFile();
	}
}

//双击编辑子项
void CDCMBrowserDlg::OnNMDblclkList1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	CRect rc;
	//获得选中的行
	row = pNMListView->iItem;
	//获得选中列
	column = pNMListView->iSubItem;

	//选择的子项是value值
	if (pNMListView->iSubItem == 4) 
	{
		//获得子项的RECT
		m_list.GetSubItemRect(row, column, LVIR_LABEL, rc);
		//转换坐标为列表框中的坐标
		m_edit.SetParent(&m_list);
		//移动Edit到RECT坐在的位置
		m_edit.MoveWindow(rc);
		//将该子项中的值放在Edit控件中
		m_edit.SetWindowText(m_list.GetItemText(row, column));
		//显示Edit控件
		m_edit.ShowWindow(SW_SHOW);
		//设置Edit焦点
		m_edit.SetFocus();
		//显示光标
		m_edit.ShowCaret();
		//将光标移动到最后
		m_edit.SetSel(-1);
	}
	*pResult = 0;
}

//单击保存编辑框内容
void CDCMBrowserDlg::OnEnKillfocusEdit1()
{
	CString tmp;
	//得到用户输入的新的内容
	CString newLength;
	m_edit.GetWindowText(tmp);
	//if (m_list.GetItemText(row, column) == tmp)
	//{
	//	m_edit.ShowWindow(SW_HIDE);
	//}
	//else
	{
		//设置编辑框的新内容
		m_list.SetItemText(row, column, tmp);
		//隐藏编辑框
		m_edit.ShowWindow(SW_HIDE);
		//修改Length
		int modifyLength = (tmp.GetLength() % 2 == 0) ? tmp.GetLength() : (tmp.GetLength() + 1);
		newLength.Format(_T("%d"), modifyLength);
		m_list.SetItemText(row, column - 1, newLength);
		modifyTag.push_back(strtagTotag(&m_list, row));
		modifyValue.push_back(tmp);
	}
} 

//保存
void CDCMBrowserDlg::OnSaveFile()
{
	ModifyData();
	OFCondition status = dfile.saveFile(filepath.c_str(), EXS_Unknown, EET_ExplicitLength, EGL_recalcGL, EPD_noChange, 0, 0, EWM_dontUpdateMeta);
	if (status.good())
	{
		MessageBox(TEXT("保存成功！"), TEXT("完成"), MB_OK | MB_ICONASTERISK);
	}
	else
	{
		MessageBox(TEXT("保存失败！ 请重试！"), TEXT("错误"), MB_OK | MB_ICONERROR);
	}
}

//另存为
void CDCMBrowserDlg::OnSaveAsFile()
{
	if (m_list.GetItemCount() == 0)
	{
		MessageBox(TEXT("该文件为空，另存为失败！"), TEXT("错误"), MB_OK | MB_ICONERROR);
		return;
	}
	ModifyData();
	//设置过滤器
	TCHAR dcmFile[] = _T("DICOM文件(*.dcm; *.dic)|*.dcm; *.dic||");
	//构造保存文件对话框
	CFileDialog fileDlg(FALSE, _T("dcm"), _T("new"), OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, dcmFile, this);
	CString strFilePath;
	//显示保存文件对话框
	if (IDOK == fileDlg.DoModal())
	{
		strFilePath = fileDlg.GetPathName();
		OFCondition status = dfile.saveFile(strFilePath.GetString(), EXS_Unknown, EET_ExplicitLength, EGL_recalcGL, EPD_noChange, 0, 0, EWM_dontUpdateMeta);
		if (status.good())
		{
			MessageBox(TEXT("保存成功！"), TEXT("完成"), MB_OK | MB_ICONASTERISK);
		}
		else
		{
			MessageBox(TEXT("保存失败！ 请重试！"), TEXT("错误"), MB_OK | MB_ICONERROR);
		}
	}
}

//单个文件拖拽
void CDCMBrowserDlg::OnDropFiles(HDROP hDropInfo)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	int count = DragQueryFile(hDropInfo, 0xFFFFFFFF, NULL, 0);
	//拖拽一个文件
	DragQueryFile(hDropInfo, 0, (LPSTR)filepath.c_str(), MAX_PATH - 1);
	DragFinish(hDropInfo);

	OpenFile();
	CDialog::OnDropFiles(hDropInfo);
}

//快捷键
BOOL CDCMBrowserDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 在此添加专用代码和/或调用基类
	if (pMsg->message == WM_KEYDOWN)
	{
		int nkeyDown = pMsg->wParam;
		SHORT nkeyState = GetKeyState(VK_CONTROL); 
		SHORT mkeyState = GetKeyState(VK_SHIFT);
		if (nkeyDown == 'Z' && nkeyState)
		{
			OnFileOpen();
		}
		if (nkeyDown == 'S' && nkeyState && !mkeyState)
		{
			OnSaveFile();
		}
		if (nkeyDown == 'S' && nkeyState && mkeyState)
		{
			OnSaveAsFile();
		}
		if (nkeyDown == 'H' && nkeyState)
		{
			OnSearchModify();
		}
	}
	return CDialog::PreTranslateMessage(pMsg);
}

//打开查找替换对话框
void CDCMBrowserDlg::OnSearchModify()
{
	CSearchModifyDlg *pDlg = new CSearchModifyDlg();
	pDlg->Create(IDD_DIALOG_SEARCHANDMODIFY, this);
	pDlg->ShowWindow(SW_SHOW);       
}



void CDCMBrowserDlg::OnCustomdrawList1(NMHDR *pNMHDR, LRESULT *pResult)
{
	// LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	NMLVCUSTOMDRAW* pNMCD = (NMLVCUSTOMDRAW*)(pNMHDR);

	if (CDDS_PREPAINT == pNMCD->nmcd.dwDrawStage)
	{
		*pResult = CDRF_NOTIFYITEMDRAW;
	}
	else if (CDDS_ITEMPREPAINT == pNMCD->nmcd.dwDrawStage)
	{
		*pResult = CDRF_NOTIFYSUBITEMDRAW;
	}
	else if ((CDDS_ITEMPREPAINT | CDDS_SUBITEM) == pNMCD->nmcd.dwDrawStage)
	{
		COLORREF  clrNewTextColor, clrNewBkColor;
		int nItem = static_cast<int>(pNMCD->nmcd.dwItemSpec);
		if (nItem % 2 == 0)
		{
			clrNewTextColor = RGB(0, 0, 0);
			clrNewBkColor = RGB(204, 255, 255);
		}
		else
		{
			clrNewTextColor = RGB(0, 0, 0);
			clrNewBkColor = RGB(255, 255, 255);
		}

		pNMCD->clrText = clrNewTextColor;
		pNMCD->clrTextBk = clrNewBkColor;
		*pResult = CDRF_DODEFAULT;
	}

	*pResult = 0;
}
