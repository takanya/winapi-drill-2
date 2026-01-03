// MainFrm.h : interface of the CMainFrame class
//
/////////////////////////////////////////////////////////////////////////////

#pragma once

class CMainFrame : 
	public CFrameWindowImpl<CMainFrame>, 
	public CUpdateUI<CMainFrame>,
	public CMessageFilter, public CIdleHandler
{
public:
	DECLARE_FRAME_WND_CLASS(NULL, IDR_MAINFRAME)

	CSplitterWindow  m_wndSplitter;      // スプリッタウィンドウ
	CFontListView m_viewFontList;        // 左ペイン
	CFontPreviewView m_viewFontPreview;  // 右ペイン

	CCommandBarCtrl m_CmdBar;            // コマンドバー

	virtual BOOL PreTranslateMessage(MSG* pMsg)
	{
		if(CFrameWindowImpl<CMainFrame>::PreTranslateMessage(pMsg))
			return TRUE;

		// 左ペインのPreTranslateMessageを呼び出す
		if (m_viewFontList.PreTranslateMessage(pMsg))
			return TRUE;

		// 右ペインのPreTranslateMessageを呼び出す
		return m_viewFontPreview.PreTranslateMessage(pMsg);
	}

	virtual BOOL OnIdle()
	{
		UIUpdateToolBar();
		UIUpdateStatusBar();

		CString strCount;
		strCount.Format(_T("フォント数:%d"),
			m_viewFontList.GetCount());
		CStatusBarCtrl bar = m_hWndStatusBar;
		bar.SetText(1, strCount);

		return FALSE;
	}

	BEGIN_UPDATE_UI_MAP(CMainFrame)
		UPDATE_ELEMENT(ID_VIEW_TOOLBAR, UPDUI_MENUPOPUP)
		UPDATE_ELEMENT(ID_VIEW_STATUS_BAR, UPDUI_MENUPOPUP)
	END_UPDATE_UI_MAP()

	BEGIN_MSG_MAP(CMainFrame)
		MESSAGE_HANDLER(WM_CREATE, OnCreate)
		MESSAGE_HANDLER(WM_DESTROY, OnDestroy)
		COMMAND_ID_HANDLER(ID_APP_EXIT, OnFileExit)
		COMMAND_ID_HANDLER(ID_FILE_NEW, OnFileNew)
		COMMAND_ID_HANDLER(ID_VIEW_TOOLBAR, OnViewToolBar)
		COMMAND_ID_HANDLER(ID_VIEW_STATUS_BAR, OnViewStatusBar)
		COMMAND_ID_HANDLER(ID_APP_ABOUT, OnAppAbout)
		CHAIN_MSG_MAP(CUpdateUI<CMainFrame>)
		CHAIN_MSG_MAP(CFrameWindowImpl<CMainFrame>)
	END_MSG_MAP()

// Handler prototypes (uncomment arguments if needed):
//	LRESULT MessageHandler(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
//	LRESULT CommandHandler(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
//	LRESULT NotifyHandler(int /*idCtrl*/, LPNMHDR /*pnmh*/, BOOL& /*bHandled*/)

	LRESULT OnCreate(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
	{
		// create command bar window
		HWND hWndCmdBar = m_CmdBar.Create(m_hWnd, rcDefault, NULL, ATL_SIMPLE_CMDBAR_PANE_STYLE);
		// attach menu
		m_CmdBar.AttachMenu(GetMenu());
		// load command bar images
		m_CmdBar.LoadImages(IDR_MAINFRAME);
		// remove old menu
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
		UISetCheck(ID_VIEW_TOOLBAR, 1);
		UISetCheck(ID_VIEW_STATUS_BAR, 1);

		UIAddStatusBar(m_hWndStatusBar);
		UISetCheck(ID_MENUITEM_CHANGEVIEW, 1);

		// スプリッタウィンドウを作成
		m_wndSplitter.Create(m_hWnd, rcDefault, NULL,	WS_CHILD | WS_VISIBLE | WS_CLIPCHILDREN | WS_CLIPSIBLINGS);

		// スプリッタウィンドウ拡張スタイルを設定
		m_wndSplitter.SetSplitterExtendedStyle(0);

		// 左ペインのビューウィンドウを作成
		m_viewFontList.Create(m_wndSplitter, rcDefault, NULL,
			WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_VSCROLL | 
			LBS_NOINTEGRALHEIGHT | LBS_NOTIFY | LBS_WANTKEYBOARDINPUT | LBS_SORT, WS_EX_CLIENTEDGE);

		m_wndSplitter.SetSplitterPane(SPLIT_PANE_LEFT, m_viewFontList);

		// スプリッタウィンドウを作成
		m_wndSplitter.Create(m_hWnd, rcDefault, NULL, WS_CHILD | WS_VISIBLE | WS_CLIPCHILDREN | WS_CLIPSIBLINGS);

		// スプリッタウィンドウ拡張スタイルを設定
		m_wndSplitter.SetSplitterExtendedStyle(0);

		// 左ペインのビューウィンドウを作成
		m_viewFontList.Create(m_wndSplitter, rcDefault, NULL,
			WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_VSCROLL | LBS_NOINTEGRALHEIGHT |
			LBS_NOTIFY | LBS_WANTKEYBOARDINPUT | LBS_SORT, WS_EX_CLIENTEDGE);

		m_wndSplitter.SetSplitterPane(SPLIT_PANE_LEFT, m_viewFontList);

		m_hWndClient = m_wndSplitter;
		UpdateLayout();

		// 分割バーの位置を設定
		m_wndSplitter.SetSplitterPos(120);

		// register object for message filtering and idle updates
		CMessageLoop* pLoop = _Module.GetMessageLoop();
		ATLASSERT(pLoop != NULL);
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
				- ::GetSystemMetrics(SM_CXVSCROLL)
				- ::GetSystemMetrics(SM_CXEDGE);

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
			// 両ペイン表示
			m_wndSplitter.SetSinglePaneMode(SPLIT_PANE_NONE);
			UISetCheck(ID_MENUITEM_CHANGEVIEW, 1);
		}
		else {
			// 右ペインのみ表示
			m_wndSplitter.SetSinglePaneMode(SPLIT_PANE_RIGHT);
			UISetCheck(ID_MENUITEM_CHANGEVIEW, 0);
		}
	}


	LRESULT OnDestroy(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled)
	{
		// unregister message filtering and idle updates
		CMessageLoop* pLoop = _Module.GetMessageLoop();
		ATLASSERT(pLoop != NULL);
		pLoop->RemoveMessageFilter(this);
		pLoop->RemoveIdleHandler(this);

		bHandled = FALSE;
		return 1;
	}

	LRESULT OnFileExit(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
	{
		PostMessage(WM_CLOSE);
		return 0;
	}

	LRESULT OnFileNew(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
	{
		// TODO: add code to initialize document

		return 0;
	}

	LRESULT OnViewToolBar(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
	{
		static BOOL bVisible = TRUE;	// initially visible
		bVisible = !bVisible;
		CReBarCtrl rebar = m_hWndToolBar;
		int nBandIndex = rebar.IdToIndex(ATL_IDW_BAND_FIRST + 1);	// toolbar is 2nd added band
		rebar.ShowBand(nBandIndex, bVisible);
		UISetCheck(ID_VIEW_TOOLBAR, bVisible);
		UpdateLayout();
		return 0;
	}

	LRESULT OnViewStatusBar(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
	{
		BOOL bVisible = !::IsWindowVisible(m_hWndStatusBar);
		::ShowWindow(m_hWndStatusBar, bVisible ? SW_SHOWNOACTIVATE : SW_HIDE);
		UISetCheck(ID_VIEW_STATUS_BAR, bVisible);
		UpdateLayout();
		return 0;
	}

	LRESULT OnAppAbout(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
	{
		CAboutDlg dlg;
		dlg.DoModal();
		return 0;
	}
};
