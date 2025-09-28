// DeleteDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Delete.h"
#include "DeleteDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif




/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
		// No message handlers
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDeleteDlg dialog

CDeleteDlg::CDeleteDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CDeleteDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDeleteDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CDeleteDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDeleteDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CDeleteDlg, CDialog)
	//{{AFX_MSG_MAP(CDeleteDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON_F, OnButtonF)
	ON_BN_CLICKED(IDC_BUTTON_E, OnButtonE)
	ON_BN_CLICKED(IDC_BUTTON_D, OnButtonD)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDeleteDlg message handlers

BOOL CDeleteDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
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
	
	// TODO: Add extra initialization here
	
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CDeleteDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CDeleteDlg::OnPaint() 
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

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CDeleteDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}



//삭제논리

#define DEFAULT_PASS 7
char RandChar(void);
int DODWrite(HANDLE);

void DODFileDelete(LPCTSTR path)
{
#define NAME_NUM	9
	int		ret = 0;
	HANDLE	hFile;
	char	tmpFileName[NAME_NUM][MAX_PATH];
	int		idx;
	
	if (path == NULL || strlen(path) < 1)
		return;

	idx = strrchr(path, '\\') - path;
	idx ++;

	//
	// 1. 파일명 변경을 위해 임의의 파일명을 생성한다.
	srand(time(NULL));
	int i, j;
	int l = strlen(path) - idx;
	for (i = 0; i < NAME_NUM; i++)
	{
		memcpy (tmpFileName[i], path, idx);

		for (j = 0; j < l; j++)
			tmpFileName[i][idx + j] = RandChar();

		tmpFileName[i][idx + j] = 0;
	}

	//
	// 2. DoD cleanning write
	//
	hFile = CreateFile(path, GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL,
		OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL|FILE_FLAG_WRITE_THROUGH, NULL);
	if (hFile == INVALID_HANDLE_VALUE)
		return;
	
	ret = DODWrite(hFile);
	CloseHandle(hFile);

	//
	// 3. change filename : 변경이 실패나면 바로 삭제
	//
	if (!MoveFile(path, tmpFileName[0]))
	{
		DeleteFile(path);
		return;
	}
	for (i = 0; i < NAME_NUM - 1; i++)
	{
		if (!MoveFile(tmpFileName[i], tmpFileName[i + 1]))
			DeleteFile (tmpFileName[i]);
	}

	//
	// 4. delete file entry
	//
	DeleteFile(tmpFileName[NAME_NUM - 1]);	
}

char RandChar(void)
{
	static char chTbl[] = {
		'A', 'a', '1', 'B', 'b', '2', 'C', 'c', '3',
		'D', 'd', '4', 'E', 'e', '5', 'F', 'f', '6',
		'G', 'g', '7', 'H', 'h', '8', 'I', 'i', '9',
		'J', 'j', '0', 'K', 'k', '1', 'L', 'l', '2',
		'M', 'm', '3', 'N', 'n', '4', 'O', 'o', '5',
		'P', 'p', '6', 'Q', 'q', '7', 'R', 'r', '8',
		'S', 's', '9', 'T', 't', '0', 'U', 'u', '1',
		'V', 'v', '2', 'W', 'w', '3', 'X', 'x', '4',
		'Y', 'y', '5', 'Z', 'z', '6',
	};

	int idx = rand() % sizeof (chTbl);
	return chTbl[idx];
}

// file을 512 byte 단위로 DoD 삭제 수행
int DODWrite(HANDLE hFile)
{
	PBYTE cbuf[3]; // zero fill, 0xff fill, random fill

	DWORD dwSize[2]; // high, low
	
	if (hFile == INVALID_HANDLE_VALUE)
		return 1;

	dwSize[1] = GetFileSize (hFile, &dwSize[0]);
    
	if (dwSize[1] == 0xFFFFFFFF)
		return 2;

	srand (time(NULL));
	srand (rand());
	
	//
	// 512 단위의 메모리를 할당한다.
	//
	cbuf[0] = (PBYTE)VirtualAlloc(NULL, 512, MEM_COMMIT, PAGE_READWRITE);
	if (cbuf[0] == NULL)
		return 3; // failure
	cbuf[1] = (PBYTE)VirtualAlloc(NULL, 512, MEM_COMMIT, PAGE_READWRITE);
	if (cbuf[1] == NULL)
	{
		VirtualFree (cbuf[0], 0, MEM_RELEASE);
		return 4;
	}
	cbuf[2] = (PBYTE)VirtualAlloc(NULL, 512, MEM_COMMIT, PAGE_READWRITE);
	if (cbuf[2] == NULL)
	{
		VirtualFree (cbuf[0], 0, MEM_RELEASE);
		VirtualFree (cbuf[1], 0, MEM_RELEASE);
		return 5; 
	}
	
	memset (cbuf[1], 0xFF, 512);
	
	// 
	// 랜덤값을 약간 복잡하게 설정한다.
	//
	UINT i;
	int r;
	for (i = 0; i < (512 / 4); i++) 
	{
		r = rand();
		cbuf[2][i + 0] = (BYTE)(r >> 24);
		cbuf[2][i + 1] = (BYTE)(r >> 16);
		cbuf[2][i + 2] = (BYTE)(r >>  8);
		cbuf[2][i + 3] = (BYTE)(r >>  0);
		
		cbuf[2][i + 0] ^= cbuf[2][i + 1];
		cbuf[2][i + 1] ^= cbuf[2][i + 2];
		cbuf[2][i + 2] ^= cbuf[2][i + 3];
		cbuf[2][i + 3] ^= cbuf[2][i + 0];
	}

	__int64 nCount = (dwSize[0] << (32 - 9)) | (dwSize[1] >> 9); // 512 로 나눈 효과

	int pass;
	for (pass = 0; pass < DEFAULT_PASS; pass++)
	{
		// 삭제진행 표시
	//	SetDlgItemText(NULL,IDC_STATIC_DD,"진행중");
		
		// DOD 5220.22-M Setion 8.306, clear E
		UINT i;
		for (i = 0; i < 3; i++)
		{
			SetFilePointer(hFile, 0, 0, FILE_BEGIN);
			
			BOOL res;
			ULONG wbytes;
			__int64 j;
			for (j = 0; j < nCount; j++)

			{   
				if (dwSize[1] < 0xFF) res = WriteFile(hFile, cbuf[i], 512, &wbytes, NULL);
				if (dwSize[1] < 0xFFF) res = WriteFile(hFile, cbuf[i], 512, &wbytes, NULL);
				if (dwSize[1] < 0xFFFF) res = WriteFile(hFile, cbuf[i], 512, &wbytes, NULL);
				if (dwSize[1] < 0xFFFFF) res = WriteFile(hFile, cbuf[i], 512, &wbytes, NULL);
				if (dwSize[1] < 0xFFFFFF) res = WriteFile(hFile, cbuf[i], 512, &wbytes, NULL);
				if (dwSize[1] < 0xFFFFFFF) res = WriteFile(hFile, cbuf[i], 512, &wbytes, NULL);
				res = WriteFile(hFile, cbuf[i], 512, &wbytes, NULL);
				if (!res)
				{
					// 에러를 무시하고 진행
					break;
				}
			}
		}
			
	//	SetDlgItemText(NULL,IDC_STATIC_DD,"  ");
	}

	VirtualFree (cbuf[0], 0, MEM_RELEASE);
	VirtualFree (cbuf[1], 0, MEM_RELEASE);
	VirtualFree (cbuf[2], 0, MEM_RELEASE);

	return 0;
}



CString buf,pathbuf,m_FileName,templist;
CString fs1, fs2, fs3, fs4, fs5;
CString flist;


void CDeleteDlg::OnButtonF() 
{
	// TODO: Add your control notification handler code here
	    int temp;
	OPENFILENAME m_ofn;
	m_ofn.nMaxFile = 100;
    char* pc = new char[4000];
    m_ofn.lpstrFile = pc;
    m_ofn.lpstrFile[0] = NULL;
	int selectedCount;
    POSITION startPosition;
	char tempbuf[100];
	char messagebuf[50];
    
		

	i = 0;
	   SetDlgItemText(IDC_STATIC_S,  "1. ----------------------------------------------");
	   SetDlgItemText(IDC_STATIC_S2, "2. ----------------------------------------------");
	   SetDlgItemText(IDC_STATIC_S3, "3. ----------------------------------------------");
	   SetDlgItemText(IDC_STATIC_S4, "4. ----------------------------------------------");
	   SetDlgItemText(IDC_STATIC_S5, "5. ----------------------------------------------");

		memset(messagebuf, NULL, sizeof messagebuf);  		
	  	CFileDialog dlg(TRUE, NULL, NULL, OFN_HIDEREADONLY | OFN_ALLOWMULTISELECT, NULL);
		startPosition = dlg.GetStartPosition();
		if(dlg.DoModal() == IDOK)

		{
        
	//	buf= dlg.GetFileName();
      

     templist.Empty();
        
        while(startPosition)

        {     
           
		   pathbuf.Empty();
		   str[i].Empty();
		   memset(tempbuf,NULL, sizeof(tempbuf));
            //char temp[40960 + 1];
			char tempw[50];
            m_FileName.Empty();
		
            m_FileName = dlg.GetNextPathName(startPosition);  
	       // MessageBox(m_FileName,MB_OK) ;
			GetFileTitle(m_FileName.GetBuffer(m_FileName.GetLength()), tempbuf, m_FileName.GetLength());
			// templist = m_FileName.ReverseFind('\\');
		    //pathbuf = dlg.GetNextFileName(startPosition) + "\r\n" ;
          
			 
            // 화면표시 
		     str[i] = m_FileName;//삭제할때 사용하기 위해 임시 저장함
			  //MessageBox(str[i],MB_OK) ;
		
			++i;
			
			switch (i)
			{
			case 1:				
				  fs1 = tempbuf;	
				  SetDlgItemText(IDC_STATIC_S, "1. "+fs1);			
				  break;
				 
             case 2:		 
		          fs2 = tempbuf;
				  SetDlgItemText(IDC_STATIC_S2, "2. "+fs2);
			      break;
              
             case 3:
				  fs3 = tempbuf;
				  SetDlgItemText(IDC_STATIC_S3, "3. "+fs3);
			      break;

             case 4:
				  fs4 = tempbuf;
				  SetDlgItemText(IDC_STATIC_S4, "4. "+fs4);
			      break;

             case 5:
				  fs5 = tempbuf;
				  SetDlgItemText(IDC_STATIC_S5, "5. "+fs5);
			      break;

			}
		        

			 if(SetFileAttributes(m_FileName, FILE_ATTRIBUTE_NORMAL) == 0)  // 읽기 전용일 수 있으므로 일반 노멀로 속성 변경	
			 {
				sprintf(tempbuf,"%s 읽기전용으로 속성변경을 먼저 하십시오[실패]", m_FileName);
			 	MessageBox(tempbuf,MB_OK) ;
				return ;
			 }	  
           // MessageBox(m_FileName);
           // m_FileName = temp;
            selectedCount++;            



        }  //while
     
	   if( i > 5) //5개 이상 선택일때
	   {
		  
		   sprintf(messagebuf, "목록창에 5개만 보이지만 선택한 %d 개 모두 삭제됨", i);
            
           if (IDNO == AfxMessageBox(messagebuf,MB_YESNO))

		   {
			   SetDlgItemText(IDC_STATIC_S,  "1. --------------------------------------------");
			   SetDlgItemText(IDC_STATIC_S2, "2. --------------------------------------------");
			   SetDlgItemText(IDC_STATIC_S3, "3. --------------------------------------------");
			   SetDlgItemText(IDC_STATIC_S4, "4. --------------------------------------------");
			   SetDlgItemText(IDC_STATIC_S5, "5. --------------------------------------------");
              return ;
		   }
			  


	   }// 5개 이상 
  // 삭제 논리 적용 





	}  //dlg 







        //temp = SetFileAttributes(buf, FILE_ATTRIBUTE_NORMAL) ;	   // 읽기 전용일 수 있으므로 일반 노멀로 속성 변경	
	

	
	 
		
	//	pathbuf=dlg.GetPathName();		 
	//	SetDlgItemText(IDC_STATIC_S, buf);
	//	AfxMessageBox(pathbuf);(HWND)IDD_DELETE_DIALOG
}
	




void CDeleteDlg::OnButtonE() 
{
	// TODO: Add your control notification handler code here
	((CDeleteDlg*)AfxGetMainWnd())->OnOK(); 

}

BOOL CDeleteDlg::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class

	//ESC enter 키 방어함
if (pMsg->message == 273) return TRUE;  //alt + F4
if (pMsg->message == WM_KEYDOWN)


{

      if (pMsg->wParam == VK_RETURN || pMsg->wParam == VK_ESCAPE) 

           return TRUE;

}


	return CDialog::PreTranslateMessage(pMsg);
}


//삭제 논리

void CDeleteDlg::OnButtonD() 
{
	// TODO: Add your control notification handler code here
//	if(pathbuf.IsEmpty()) {MessageBox("삭제파일 선택오류"); return ;}
	SetDlgItemText(IDC_STATIC_DD,"삭제진행중");
	
    
   
	for (int ntemp = 0; ntemp < i; ntemp++)
	{	
	 DODFileDelete((LPCTSTR) str[ntemp]);	
	}

	//SetDlgItemText(IDC_STATIC_DD,"삭제완료");


	   CString delcount;
	   delcount.Format(" %d개 파일 삭제완료",i);
	   MessageBox(delcount,MB_OK);
	   MessageBeep(MB_OK);	
	   SetDlgItemText(IDC_STATIC_DD,"         ");

	//	::SetDlgItemText((HWND)IDD_DELETE_DIALOG, IDC_STATIC_S, pathbuf);

}
