// MainFrm.h : interface of the CMainFrame class
//
/////////////////////////////////////////////////////////////////////////////

#pragma once

class CMainFrame : public CFrameWindowImpl<CMainFrame>, public CUpdateUI<CMainFrame>,
  public CMessageFilter, public CIdleHandler
{
public:
  DECLARE_FRAME_WND_CLASS(NULL, IDR_MAINFRAME)

  CSplitterWindow  m_wndSplitter;      // スプリッタウィンドウ
  CFontListView m_viewFontList;        // 左ペイン
  CFontPreviewView m_viewFontPreview;  // 右ペイン

  CCommandBarCtrl m_CmdBar;

  virtual BOOL PreTranslateMessage(MSG* pMsg) {
    if (CFrameWindowImpl<CMainFrame>::PreTranslateMessage(pMsg))
      return TRUE;

    // 左ペインのPreTranslateMessageを呼び出す
    if (m_viewFontList.PreTranslateMessage(pMsg))
      return TRUE;

    // 右ペインのPreTranslateMessageを呼び出す
    return m_viewFontPreview.PreTranslateMessage(pMsg);
  }

  virtual BOOL OnIdle() {
    UIUpdateToolBar();
    UIUpdateStatusBar();

    CString strCount;
    strCount.Format(_T("フォント数:%d"), m_viewFontList.GetCount());
    CStatusBarCtrl bar = m_hWndStatusBar;
    bar.SetText(1, strCount);

    return FALSE;
  }

  BEGIN_UPDATE_UI_MAP(CMainFrame)
    UPDATE_ELEMENT(ID_MENUITEM_CHANGEVIEW, UPDUI_MENUPOPUP | UPDUI_TOOLBAR)
  END_UPDATE_UI_MAP()

  BEGIN_MSG_MAP_EX(CMainFrame)
    MSG_WM_CREATE(OnCreate)
    MSG_WM_SIZE(OnSize)
    COMMAND_CODE_HANDLER_EX(LBN_SELCHANGE, OnListSelChange)
    COMMAND_ID_HANDLER_EX(ID_MENUITEM_CHANGEVIEW, OnMenuChangeView)
    COMMAND_ID_HANDLER_EX(ID_APP_EXIT, OnFileExit)
    CHAIN_MSG_MAP(CUpdateUI<CMainFrame>)
    CHAIN_MSG_MAP(CFrameWindowImpl<CMainFrame>)
    END_MSG_MAP()

  LRESULT OnCreate(LPCREATESTRUCT lpcs) {
    // コマンドバーを作成
    HWND hWndCmdBar = m_CmdBar.Create(m_hWnd, rcDefault, NULL, ATL_SIMPLE_CMDBAR_PANE_STYLE);

    // コマンドバーに現在のメニューバーのアイテムと画像をセット
    m_CmdBar.AttachMenu(GetMenu());
    m_CmdBar.LoadImages(IDR_MAINFRAME);

    // メニューバーを削除
    SetMenu(NULL);

    // ツールバーを作成
    HWND hWndToolBar = CreateSimpleToolBarCtrl(m_hWnd, IDR_MAINFRAME, FALSE, ATL_SIMPLE_TOOLBAR_PANE_STYLE);

    // リバーを作成
    CreateSimpleReBar(ATL_SIMPLE_REBAR_NOBORDER_STYLE);

    // リバーのバンドにコマンドバーとツールバーを追加
    AddSimpleReBarBand(hWndCmdBar);
    AddSimpleReBarBand(hWndToolBar, NULL, TRUE);

    // ステータスバーを作成
    CreateSimpleStatusBar();

    UIAddToolBar(hWndToolBar);
    UIAddStatusBar(m_hWndStatusBar);

    UISetCheck(ID_MENUITEM_CHANGEVIEW, 1);

    // スプリッタウィンドウを作成
    m_wndSplitter.Create(m_hWnd, rcDefault, NULL,
      WS_CHILD | WS_VISIBLE | WS_CLIPCHILDREN | WS_CLIPSIBLINGS);

    // スプリッタウィンドウ拡張スタイルを設定
    m_wndSplitter.SetSplitterExtendedStyle(0);

    // 左ペインのビューウィンドウを作成
    m_viewFontList.Create(m_wndSplitter, rcDefault, NULL,
      WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_VSCROLL |
      LBS_NOINTEGRALHEIGHT | LBS_NOTIFY | LBS_WANTKEYBOARDINPUT | LBS_SORT,
      WS_EX_CLIENTEDGE);

    m_wndSplitter.SetSplitterPane(SPLIT_PANE_LEFT, m_viewFontList);

    // 右ペインのビューウィンドウを作成
    m_viewFontPreview.Create(m_wndSplitter, rcDefault, NULL,
      WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN,
      WS_EX_CLIENTEDGE);

    m_wndSplitter.SetSplitterPane(SPLIT_PANE_RIGHT, m_viewFontPreview);

    m_hWndClient = m_wndSplitter;
    UpdateLayout();

    // 分割バーの位置を設定
    m_wndSplitter.SetSplitterPos(120);

    // メッセージループにメッセージフィルタとアイドルハンドラを追加
    CMessageLoop* pLoop = _Module.GetMessageLoop();
    pLoop->AddMessageFilter(this);
    pLoop->AddIdleHandler(this);

    return 0;
  }

  void OnSize(UINT uType, CSize size) {
    // ステータスバーにペインを設定
    if (m_hWndStatusBar != NULL) {
      CStatusBarCtrl bar = m_hWndStatusBar;

      // フォント数を表示するペインの幅
      int cxPosPane = 80;

      // デフォルトペインの幅
      CRect rcClient;
      GetClientRect(rcClient);
      int cxDefaultPane = rcClient.right - cxPosPane
        - ::GetSystemMetrics(SM_CXVSCROLL) - ::GetSystemMetrics(SM_CXEDGE);

      int nPanes[] = { cxDefaultPane, cxDefaultPane + cxPosPane };
      bar.SetParts(sizeof(nPanes) / sizeof(nPanes[0]), nPanes);
    }

    // 基底クラスのWM_SIZEメッセージハンドラも呼び出すため
    SetMsgHandled(false);
  }

  void OnListSelChange(UINT uNotifyCode, int nID, HWND hWndCtl) {
    // 現在選択されているアイテムを取得
    int nIndex = m_viewFontList.GetCurSel();
    if (nIndex != LB_ERR) {
      CString strText;
      m_viewFontList.GetText(nIndex, strText);

      // 取得した文字列を右ペインに設定
      m_viewFontPreview.SetFontName(strText);
    }
  }

  void OnMenuChangeView(UINT uNotifyCode, int nID, HWND hWndCtl) {
    if (m_wndSplitter.GetSinglePaneMode() == SPLIT_PANE_RIGHT) {
      m_wndSplitter.SetSinglePaneMode(SPLIT_PANE_NONE);    // 両ペイン表示
      UISetCheck(ID_MENUITEM_CHANGEVIEW, 1);
    }
    else {
      m_wndSplitter.SetSinglePaneMode(SPLIT_PANE_RIGHT);   // 右ペインのみ表示
      UISetCheck(ID_MENUITEM_CHANGEVIEW, 0);
    }
  }

  void OnFileExit(UINT uNotifyCode, int nID, HWND hWndCtl) {
    PostMessage(WM_CLOSE);
  }
};

