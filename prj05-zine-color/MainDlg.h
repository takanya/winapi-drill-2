// MainDlg.h : interface of the CMainDlg class
//
/////////////////////////////////////////////////////////////////////////////

#pragma once

class CMainDlg : public CDialogImpl<CMainDlg>, public CUpdateUI<CMainDlg>,
  public CMessageFilter, public CIdleHandler
{
public:
  enum { IDD = IDD_MAINDLG };

  CButton m_button_spuit;      // [スポイト]ラジオボタン
  CButton m_button_copy;       // [コピー]ラジオボタン
  CStatic m_static_color;      // 色表示エリア
  CEdit m_edit_color;          // 色値
  CButton m_check_topmost;     // [常に手前に表示]チェックボックス

  bool m_bClick;
  COLORREF m_color;


  virtual BOOL PreTranslateMessage(MSG* pMsg)
  {
    return CWindow::IsDialogMessage(pMsg);
  }

  virtual BOOL OnIdle()
  {
    UIUpdateChildWindows();
    return FALSE;
  }

  BEGIN_UPDATE_UI_MAP(CMainDlg)
  END_UPDATE_UI_MAP()

  BEGIN_MSG_MAP(CMainDlg)
    MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
    MESSAGE_HANDLER(WM_DESTROY, OnDestroy)
    MESSAGE_HANDLER(WM_PAINT, OnPaint)
    MESSAGE_HANDLER(WM_LBUTTONDOWN, OnLButtonDown)
    COMMAND_HANDLER(IDC_BTN_SPUIT, BN_CLICKED, OnBnClickedBtnSpuit)
    COMMAND_HANDLER(IDC_BTN_COPY, BN_CLICKED, OnBnClickedBtnCopy)
    COMMAND_HANDLER(IDC_CHK_TOPMOST, BN_CLICKED, OnBnClickedChkTopmost)
    COMMAND_ID_HANDLER(ID_APP_ABOUT, OnAppAbout)
    COMMAND_ID_HANDLER(IDOK, OnOK)
    COMMAND_ID_HANDLER(IDCANCEL, OnCancel)
  END_MSG_MAP()

  // Handler prototypes (uncomment arguments if needed):
  //	LRESULT MessageHandler(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
  //	LRESULT CommandHandler(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
  //	LRESULT NotifyHandler(int /*idCtrl*/, LPNMHDR /*pnmh*/, BOOL& /*bHandled*/)

  LRESULT OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
  {
    // center the dialog on the screen
    CenterWindow();

    // コントロール設定
    m_button_spuit = GetDlgItem(IDC_BTN_SPUIT);
    m_button_copy = GetDlgItem(IDC_BTN_COPY);
    m_static_color = GetDlgItem(IDC_STATIC_COLOR);
    m_edit_color = GetDlgItem(IDC_EDIT_COLOR);
    m_check_topmost = GetDlgItem(IDC_CHK_TOPMOST);

    // set icons
    HICON hIcon = AtlLoadIconImage(IDR_MAINFRAME, LR_DEFAULTCOLOR, ::GetSystemMetrics(SM_CXICON), ::GetSystemMetrics(SM_CYICON));
    SetIcon(hIcon, TRUE);
    HICON hIconSmall = AtlLoadIconImage(IDR_MAINFRAME, LR_DEFAULTCOLOR, ::GetSystemMetrics(SM_CXSMICON), ::GetSystemMetrics(SM_CYSMICON));
    SetIcon(hIconSmall, FALSE);

    // register object for message filtering and idle updates
    CMessageLoop* pLoop = _Module.GetMessageLoop();
    ATLASSERT(pLoop != NULL);
    pLoop->AddMessageFilter(this);
    pLoop->AddIdleHandler(this);

    UIAddChildWindowContainer(m_hWnd);

    m_bClick = false;
    m_color = RGB(255, 255, 255);

    return TRUE;
  }

  LRESULT OnDestroy(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
  {
    // unregister message filtering and idle updates
    CMessageLoop* pLoop = _Module.GetMessageLoop();
    ATLASSERT(pLoop != NULL);
    pLoop->RemoveMessageFilter(this);
    pLoop->RemoveIdleHandler(this);

    return 0;
  }

  LRESULT OnAppAbout(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
  {
    OutputDebugString(L"ここにログメッセージを書きますーー\n");
    CAboutDlg dlg;
    dlg.DoModal();
    return 0;
  }

  LRESULT OnOK(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
  {
    // TODO: Add validation code 
    CloseDialog(wID);
    return 0;
  }

  LRESULT OnCancel(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
  {
    CloseDialog(wID);
    return 0;
  }

  void CloseDialog(int nVal)
  {
    DestroyWindow();
    ::PostQuitMessage(nVal);
  }

  LRESULT CMainDlg::OnLButtonDown(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM lParam, BOOL& /*bHandled*/)
  {
    // TODO: ここにメッセージ ハンドラー コードを追加するか、既定の処理を呼び出します。
    if (m_bClick) {
      CPoint point(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
      ClientToScreen(&point);
      CClientDC dc(NULL);
      m_color = dc.GetPixel(point);

      ReleaseCapture();
      m_bClick = false;

      CString strColor;
      strColor.Format(_T("#%02x%02x%02x"),
        GetRValue(m_color), GetGValue(m_color), GetBValue(m_color));
      m_edit_color.SetWindowText(strColor);

      CRect rcColor;
      m_static_color.GetWindowRect(rcColor);
      ScreenToClient(rcColor);

      InvalidateRect(rcColor);
    }
    return 0;
  }

  LRESULT CMainDlg::OnPaint(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled)
  {
    // TODO: ここにメッセージ ハンドラー コードを追加するか、既定の処理を呼び出します。
    CPaintDC dc(m_hWnd);

    CRect rcColor;
    m_static_color.GetWindowRect(rcColor);
    ScreenToClient(rcColor);

    dc.FillSolidRect(rcColor, m_color);

    bHandled = FALSE;
    return 0;
  }

  LRESULT CMainDlg::OnBnClickedBtnSpuit(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
  {
    // TODO: ここにコントロール通知ハンドラー コードを追加します。
    m_bClick = true;
    SetCapture();

    return 0;
  }

  LRESULT CMainDlg::OnBnClickedBtnCopy(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
  {
    // TODO: ここにコントロール通知ハンドラー コードを追加します。
    m_edit_color.SetSel(0, -1);  //エディットボックス内を全て選択する
    m_edit_color.Copy();        //選択したものをコピーする	
    return 0;
  }

  LRESULT CMainDlg::OnBnClickedChkTopmost(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
  {
    // TODO: ここにコントロール通知ハンドラー コードを追加します。
    SetWindowPos(m_check_topmost.GetCheck() ? HWND_TOPMOST : HWND_NOTOPMOST,
      0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE | SWP_SHOWWINDOW);
    return 0;
  }

};
