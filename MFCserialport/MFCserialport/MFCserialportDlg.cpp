
// MFCserialportDlg.cpp: 구현 파일
//

#include "stdafx.h"
#include "framework.h"
#include "MFCserialport.h"
#include "MFCserialportDlg.h"
#include "afxdialogex.h"
#include "source.h"
#include "memory.h"
#include "string.h"
#include <stdlib.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CMFCserialportDlg 대화 상자

FILE* downloadThing;

CMFCserialportDlg::CMFCserialportDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_MFCSERIALPORT_DIALOG, pParent)
	, m_str_comport(_T(""))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CMFCserialportDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_COMPORT, m_combo_comport_list);
	DDX_CBString(pDX, IDC_COMBO_COMPORT, m_str_comport);
	DDX_Control(pDX, IDC_COMBO_BAUDRATE, m_combo_baudrate_list);
	DDX_CBString(pDX, IDC_COMBO_BAUDRATE, m_str_baudrate);
	DDX_Control(pDX, IDC_EDIT_RCV_VIEW, m_edit_rcv_view);
	DDX_Control(pDX, IDC_EDIT_SEND_DATA, m_edit_send_data);
}

BEGIN_MESSAGE_MAP(CMFCserialportDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_MESSAGE(WM_MYCLOSE, &CMFCserialportDlg::OnThreadClosed)
	ON_MESSAGE(WM_MYRECEIVE, &CMFCserialportDlg::OnReceive)
	ON_CBN_SELCHANGE(IDC_COMBO_COMPORT, &CMFCserialportDlg::OnCbnSelchangeComboComport)
	ON_CBN_SELCHANGE(IDC_COMBO_BAUDRATE, &CMFCserialportDlg::OnCbnSelchangeComboBaudrate)
	ON_BN_CLICKED(IDC_BT_CONNECT, &CMFCserialportDlg::OnBnClickedBtConnect)
	ON_BN_CLICKED(IDC_BT_CLEAR, &CMFCserialportDlg::OnBnClickedBtClear)
	ON_EN_CHANGE(IDC_EDIT_RCV_VIEW, &CMFCserialportDlg::OnEnChangeEditRcvView)
	ON_EN_CHANGE(IDC_EDIT_SEND_DATA, &CMFCserialportDlg::OnEnChangeEditSendData)
	ON_BN_CLICKED(IDC_BT_SEND, &CMFCserialportDlg::OnBnClickedBtSend)
	ON_BN_CLICKED(IDC_BUTTON_FILE_OPEN, &CMFCserialportDlg::OnBnClickedButtonFileOpen)
END_MESSAGE_MAP()


// CMFCserialportDlg 메시지 처리기

BOOL CMFCserialportDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 이 대화 상자의 아이콘을 설정합니다.  응용 프로그램의 주 창이 대화 상자가 아닐 경우에는
	//  프레임워크가 이 작업을 자동으로 수행합니다.
	SetIcon(m_hIcon, TRUE);			// 큰 아이콘을 설정합니다.
	SetIcon(m_hIcon, FALSE);		// 작은 아이콘을 설정합니다.

	m_combo_comport_list.AddString(_T("COM1"));
	m_combo_comport_list.AddString(_T("COM2"));
	m_combo_comport_list.AddString(_T("COM3"));
	m_combo_comport_list.AddString(_T("COM4"));

	m_combo_baudrate_list.AddString(_T("9600"));
	m_combo_baudrate_list.AddString(_T("19200"));
	m_combo_baudrate_list.AddString(_T("28800"));
	m_combo_baudrate_list.AddString(_T("33600"));
	m_combo_baudrate_list.AddString(_T("38400"));
	m_combo_baudrate_list.AddString(_T("56000"));
	m_combo_baudrate_list.AddString(_T("115200"));

	comport_state = false;
	GetDlgItem(IDC_BT_CONNECT)->SetWindowText(_T("OPEN"));
	m_str_comport = _T("COM2");
	m_str_baudrate = _T("115200");
	UpdateData(FALSE);

	return TRUE;  // 포커스를 컨트롤에 설정하지 않으면 TRUE를 반환합니다.
}

// 대화 상자에 최소화 단추를 추가할 경우 아이콘을 그리려면
//  아래 코드가 필요합니다.  문서/뷰 모델을 사용하는 MFC 애플리케이션의 경우에는
//  프레임워크에서 이 작업을 자동으로 수행합니다.

void CMFCserialportDlg::OnPaint()
{
	AfxGetMainWnd()->SetWindowText("ESP8266 Serial monitor and download tool");

	if (IsIconic())
	{
		CPaintDC dc(this); // 그리기를 위한 디바이스 컨텍스트입니다.

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 클라이언트 사각형에서 아이콘을 가운데에 맞춥니다.
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 아이콘을 그립니다.
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

LRESULT CMFCserialportDlg::OnThreadClosed(WPARAM length, LPARAM lpara)
{
	//overlapped i/o 핸들을닫는다.
	((CMycomm*)lpara)->HandleClose();
	delete ((CMycomm*)lpara);

	return 0;
}

int Check = 0;

LRESULT CMFCserialportDlg::OnReceive(WPARAM length, LPARAM lpara)
{
	CString str;
	char data[20000];
	if (m_comm)
	{
		m_comm->Receive(data, length); //length 길이만큼데이터를받는다.
		data[length] = _T('\0');
		str += _T("\r\n");
		for (int i = 0; i < length; i++)
		{
			char tmp[20] = "";
			sprintf(tmp, "%02X", data[i]);
			str += tmp;
		}
		m_edit_rcv_view.ReplaceSel(str); //에디트박스에표시하기위함
		str = "";
		//UpdateData(FALSE);
		m_edit_rcv_view.LineScroll(m_edit_rcv_view.GetLineCount());

		Check = 1;
	}
	return 0;
}

// 사용자가 최소화된 창을 끄는 동안에 커서가 표시되도록 시스템에서
//  이 함수를 호출합니다.
HCURSOR CMFCserialportDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CMFCserialportDlg::OnCbnSelchangeComboComport()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData();
}


void CMFCserialportDlg::OnCbnSelchangeComboBaudrate()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData();
}


void CMFCserialportDlg::OnBnClickedBtConnect()
{
	if (comport_state)
	{
		if (m_comm)        //컴포트가존재하면
		{
			m_comm->Close();
			m_comm = NULL;
			AfxMessageBox(_T("COM 포트닫힘"));
			comport_state = false;
			GetDlgItem(IDC_BT_CONNECT)->SetWindowText(_T("OPEN"));
			GetDlgItem(IDC_BT_SEND)->EnableWindow(false);
		}
	}
	else
	{
		m_comm = new CMycomm(_T("\\\\.\\") + m_str_comport, m_str_baudrate, _T("None"), _T("8 Bit"), _T("1 Bit"));         // initial Comm port
		if (m_comm->Create(GetSafeHwnd()) != 0) //통신포트를열고윈도우의핸들을넘긴다.
		{
			AfxMessageBox(_T("COM 포트열림"));
			comport_state = true;
			GetDlgItem(IDC_BT_CONNECT)->SetWindowText(_T("CLOSE"));
			GetDlgItem(IDC_BT_SEND)->EnableWindow(true);
		}
		else
		{
			AfxMessageBox(_T("ERROR!"));
		}

	}
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}


void CMFCserialportDlg::OnBnClickedBtClear()
{
	GetDlgItem(IDC_EDIT_RCV_VIEW)->SetWindowText(_T(" "));
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}


void CMFCserialportDlg::OnEnChangeEditRcvView()
{
	// TODO:  RICHEDIT 컨트롤인 경우, 이 컨트롤은
	// CDialogEx::OnInitDialog() 함수를 재지정 
	//하고 마스크에 OR 연산하여 설정된 ENM_CHANGE 플래그를 지정하여 CRichEditCtrl().SetEventMask()를 호출하지 않으면
	// 이 알림 메시지를 보내지 않습니다.

	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.
}


void CMFCserialportDlg::OnEnChangeEditSendData()
{
	// TODO:  RICHEDIT 컨트롤인 경우, 이 컨트롤은
	// CDialogEx::OnInitDialog() 함수를 재지정 
	//하고 마스크에 OR 연산하여 설정된 ENM_CHANGE 플래그를 지정하여 CRichEditCtrl().SetEventMask()를 호출하지 않으면
	// 이 알림 메시지를 보내지 않습니다.

	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.
}


void CMFCserialportDlg::On(char* in) {
	CString str;
	str.Format("%s",in);

	str += _T("\r\n");

	m_edit_rcv_view.ReplaceSel(str); //에디트박스에표시하기위함
	//UpdateData(FALSE);
	m_edit_rcv_view.LineScroll(m_edit_rcv_view.GetLineCount());
}

void CMFCserialportDlg::Send(char * in, int length) {

	m_comm->Send(in, length);

	Check = 0;
}

void CMFCserialportDlg::OnBnClickedBtSend()
{
	/*
	CString str;
	GetDlgItem(IDC_EDIT_SEND_DATA)->GetWindowText(str);
	str += "\r\n";
	m_comm->Send(str, str.GetLength());
	*/

	On("\nSync\n");
	Send(sync, 50);

	//On("\nMem Begin...\n");
	//Send(memBegin, SizeofChar(memBegin));

	On("\nWrite Mem Data..\n");
	
	char getData[50], changeDB[2] = { 0xDB, 0xDD }, changeCO[2] = {0xDB, 0xDC};
	int allLength = 32, percent = 0;
	
	//프로그램 다운로드!
	/*
	while (fgets(getData, 32, downloadThing) != NULL) {
		allLength = 32;
		for (int i = 0; i < allLength; i++) {
			if (getData[i] == 0xDB) { StrNinsert(getData, changeDB, i, 2); allLength++; i++; }
			if (i % 32 != 0 && getData[i] == 0xC0) { StrNinsert(getData, changeCO, i, 2); allLength++; i++; }
		}

		char* check = checksum(getData, allLength);
		
		//읽어온 메모리카피!
		for (int i = 0; i < allLength; i++) {
			memData[9 + i] = getData[i];
		}
		memData[allLength] = 0xC0;

		//첵섬 넣기
		for (int i = 0; i < 4 ; i++) {
			memData[5 + i] = check[i];
		}

		Send(memData, allLength);

		char onPercentChar[20] = "";

		percent++;

		if (percent % 4 == 10) {
			sprintf(onPercentChar, "%f", 64 / percent);
			On(onPercentChar);
		}
	}
	*/
}

void CMFCserialportDlg::OnBnClickedButtonFileOpen()
{
	CString str = _T("All files(*.*)|*.*|"); // 모든 파일 표시
	// _T("Excel 파일 (*.xls, *.xlsx) |*.xls; *.xlsx|"); 와 같이 확장자를 제한하여 표시할 수 있음
	CFileDialog dlg(TRUE, _T("*.dat"), NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, str, this);

	if (dlg.DoModal() == IDOK)
	{
		CString strPathName = dlg.GetPathName();
		char* pChar = strPathName.GetBuffer();
		char* comp = "\\";

		for(int i = 0 ; i < SizeofChar(pChar); i++) {
			if (strncmp(&pChar[i], comp, 1) == 0) {
				StrNinsert(pChar, comp, i, 1);
				i++;
			}
		}
		
		strPathName.Format(pChar);
		downloadThing = fopen(strPathName,"rb");
		// 파일 경로를 가져와 사용할 경우, Edit Control에 값 저장
		SetDlgItemText(IDC_EDIT_SEND_DATA, strPathName);
	}
}

void CMFCserialportDlg::Wait(DWORD dwMillisecond)
{
	MSG msg;
	DWORD dwStart;
	dwStart = GetTickCount();

	while (GetTickCount() - dwStart < dwMillisecond)
	{
		while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}
}
