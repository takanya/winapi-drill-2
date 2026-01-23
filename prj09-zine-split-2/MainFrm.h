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

  /*
  * コマンドバー
  * Internet Explorerのメニューバーのようなグリップが付いていて移動可能なバーのことです
  * CommandBarCtrlを使うと、メニューリソースを用意するだけで
  * 簡単にコマンドバーを作成することができます。
  * また、画像リソースを用意すると、メニューアイテムに画像を関連付けることができます。
  */
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

  /*
  * CUpdateUIは、適切なタイミング（アイドル時やWM_INITMENUPOPUPメッセージが送られてきた時など）に
  * ユーザーインターフェイス（UI）の状態を更新するための仕組みです
  * 例えば、メニューアイテムにチェックマークを入れたり、ツールバーボタンを使用不可にするときなどに使用します。
  * WTLはUI更新ハンドラを実現するためにCUpdateUIを用意しています。
  */

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
    // CMainFrameクラスのWM_CREATEメッセージハンドラでは、
    // m_CmdBarに対してCreate()を呼び出すことでコマンドバーを作成します。
    // Create()の第4引数にはコマンドバーのスタイルを指定します。
    // ATL_SIMPLE_CMDBAR_PANE_STYLEはコマンドバーの標準的なスタイルを意味します。
    HWND hWndCmdBar = m_CmdBar.Create(m_hWnd, rcDefault, NULL, ATL_SIMPLE_CMDBAR_PANE_STYLE);

    // コマンドバーに現在のメニューバーのアイテムと画像をセット
    // コマンドバーを作成した後は、コマンドバーにメニューアイテムを追加します。
    // 今回のプログラムはAttachMenu()を呼び出すことによってフレームウィンドウの
    // メニューバーからメニューアイテムを読み込んでいますが、LoadMenu()を呼び出して
    // メニューリソースから直接アイテムをロードすることもできます。
    m_CmdBar.AttachMenu(GetMenu());
    // 次に、LoadImages()を呼び出すことによってツールバーの画像をロードしています。
    // これにより、IDが等しいメニューアイテムとツールバーの画像が関連付けられ、
    // メニューアイテムの横に画像が表示されるようになります。
    // なお、メニューアイテムの横に画像を表示するためには、
    // AddBitmap()やAddIcon()を使うこともできます。
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
    // まず、CMainFrameクラスのメンバ変数としてスプリッタウィンドウクラスである
    // CSplitterWindowのインスタンスであるm_wndSplitterを宣言します。
    // さらに、各ペインのクラスであるCFontListViewとCFontPreviewViewの
    // インスタンスもメンバ変数として宣言します。
    // CMainFrameクラスのWM_CREATEメッセージハンドラでは、
    // m_wndSplitterに対してCreate()を呼び出すことでスプリッタウィンドウを作成します。
    m_wndSplitter.Create(m_hWnd, rcDefault, NULL,
      WS_CHILD | WS_VISIBLE | WS_CLIPCHILDREN | WS_CLIPSIBLINGS);

    // スプリッタウィンドウ拡張スタイルを設定
    // 次に、SetSplitterExtendedStyle()を呼び出してスプリッタウィンドウの
    // 拡張スタイルを設定します。
    // 今回のプログラムでは0を設定していますが、これは、スプリッタウィンドウの
    // サイズが変更された場合に、左ペインの幅
    // （水平分割スプリッタウィンドウの場合は上ペインの高さ）を固定することを意味します。
    // スプリッタウィンドウ拡張スタイルには次のスタイルが指定できます。
    m_wndSplitter.SetSplitterExtendedStyle(0);

    // 左ペインのビューウィンドウを作成
    m_viewFontList.Create(m_wndSplitter, rcDefault, NULL,
      WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_VSCROLL |
      LBS_NOINTEGRALHEIGHT | LBS_NOTIFY | LBS_WANTKEYBOARDINPUT | LBS_SORT,
      WS_EX_CLIENTEDGE);

    // スプリッタウィンドウを作成した後は、スプリッタウィンドウを親として
    // 各ペインのウィンドウを作成します。
    // 各ペインはSetSplitterPane()を呼び出すことによってスプリッタウィンドウにセットされます。
    // SetSplitterPane()の第1引数にはペインを識別するための次のようなフラグを指定します。
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

  // 今回のプログラムは、ステータスバーの右端にフォント数を表示する領域（ペイン）
  // を持ちます。
  // この領域はフレームウィンドウのサイズが変わるたびに設定する必要があるため、
  // CMainFrameクラスのWM_SIZEメッセージハンドラで設定します。
  // WM_SIZEメッセージハンドラでは、ステータスバーに2つのペインを追加します。
  // 1番目はフレームウィンドウの大きさに合わせて幅を調整するデフォルトペイン、
  // 2番目はフォント数を表示するペインです。
  // ペインを設定するにはSetParts()を呼び出します。
  // 第1引数にはペインの数、第2引数にはペインの右端の座標を要素とする配列への
  // ポインタを指定します。
  // なお、WM_SIZEメッセージは基底クラスであるCFrameWindowImplクラス内でもマッピング
  // されているため、最後にSetMsgHandled(false)を呼び出す必要があります。
  // フォント数はOnIdle()内で取得してステータスバーに表示します。
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

