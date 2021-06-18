
// MFCserialportDlg.h: 헤더 파일
//

#pragma once
#include "afxwin.h"
#include "Mycomm.h"

// CMFCserialportDlg 대화 상자
class CMFCserialportDlg : public CDialogEx
{
// 생성입니다.
public:
	CMFCserialportDlg(CWnd* pParent = nullptr);	// 표준 생성자입니다.
	CMycomm* m_comm;
	LRESULT      OnThreadClosed(WPARAM length, LPARAM lpara);
	LRESULT      OnReceive(WPARAM length, LPARAM lpara);
	void On(char*);
	void OnHex(char*);
	void OnRec(int);
	void Send(char*, int);
	void Wait(DWORD dwMillisecond);
// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MFCSERIALPORT_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 지원입니다.


// 구현입니다.
protected:
	HICON m_hIcon;

	// 생성된 메시지 맵 함수
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	BOOL comport_state;
	CComboBox m_combo_comport_list;
	afx_msg void OnCbnSelchangeComboComport();
	CString m_str_comport;
	CString m_str_baudrate;
	CComboBox m_combo_baudrate_list;
	CEdit m_edit_rcv_view;
	CEdit m_edit_send_data;
	afx_msg void OnCbnSelchangeComboBaudrate();
	afx_msg void OnBnClickedBtConnect();
	afx_msg void OnBnClickedBtClear();
	afx_msg void OnEnChangeEditRcvView();
	afx_msg void OnEnChangeEditSendData();
	afx_msg void OnBnClickedBtSend();
	afx_msg void OnBnClickedButtonFileOpen();
};
