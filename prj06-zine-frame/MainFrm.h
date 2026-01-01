// MainFrm.h : interface of the CMainFrame class
//
/////////////////////////////////////////////////////////////////////////////

#pragma once
/*
WTLには基本的なウィンドウの機能を持つCFrameWindowImplが用意されています。
CFrameWindowImpl（または、その基底クラスであるCFrameWindowImplBase）クラスには、
WM_DESTROYメッセージ用のデフォルトのハンドラが用意されている他、
ウィンドウサイズが変更されたときに自動的にレイアウトを調整するWM_SIZEメッセージハンドラ、
ツールバーやステータスバーを作成する関数、
キーボードアクセラレータ用のコードやツールチップ用のメッセージハンドラなどが備えられています。
また、メニューバーなどのリソースを簡単に設定するマクロをサポートしています
*/
class CMainFrame : 
	public CFrameWindowImpl<CMainFrame>, 
	public CUpdateUI<CMainFrame>,
	public CMessageFilter, public CIdleHandler
{
public:
  // ウィンドウクラス名、アイコン、カーソル、背景などを定義します。
  // DECLARE_FRAME_WND_CLASS(ウィンドウクラス名、共通リソースID)
	DECLARE_FRAME_WND_CLASS(NULL, IDR_MAINFRAME)

	// メッセージフィルタ処理
	/*
	メッセージフィルタでCFrameWindowImplのメンバ関数であるPreTranslateMessage()を呼び出します。
	この関数ではキーボードアクセラレータ用に::TranslateAccelerator()を呼び出していますが、
	今回はキーボードアクセラレータを使用しないのであまり関係ありません。
	*/
	virtual BOOL PreTranslateMessage(MSG* pMsg)
	{
    // 基底クラスの PreTranslateMessage を呼び出す
		return CFrameWindowImpl<CMainFrame>::PreTranslateMessage(pMsg);
	}

  // アイドル処理
	virtual BOOL OnIdle()
	{
		UIUpdateToolBar();
		return FALSE;
	}

	BEGIN_UPDATE_UI_MAP(CMainFrame)
		UPDATE_ELEMENT(ID_VIEW_TOOLBAR, UPDUI_MENUPOPUP)
		UPDATE_ELEMENT(ID_VIEW_STATUS_BAR, UPDUI_MENUPOPUP)
	END_UPDATE_UI_MAP()

  // メッセージマップ
	/*
  チェーンとは、あるメッセージマップを別のメッセージマップに繋ぐ仕組みです。
	今回の例ではCHAIN_MSG_MAP()マクロで基底クラスのメッセージマップに繋いでいます。
	こうすることにより、派生クラスでメッセージハンドラが見つからない場合は、
	基底クラスのメッセージマップを検索するようになります。
	CFrameWindowImplはWM_DESTROYメッセージハンドラを用意しており、
	最終的に::PostQuitMessage()を呼び出すようになっています。
	チェーンを利用することによって、CMainFrameクラスでWM_DESTROYメッセージを処理しなくとも、
	CFrameWindowImplのWM_DESTROYメッセージハンドラに処理を任せることができます。
 */
	BEGIN_MSG_MAP(CMainFrame)
		MESSAGE_HANDLER(WM_CREATE, OnCreate)
		MESSAGE_HANDLER(WM_DESTROY, OnDestroy)
		MESSAGE_HANDLER(WM_PAINT, OnPaint)
		COMMAND_ID_HANDLER(ID_APP_EXIT, OnFileExit)
		COMMAND_ID_HANDLER(ID_FILE_NEW, OnFileNew)
		COMMAND_ID_HANDLER(ID_VIEW_TOOLBAR, OnViewToolBar)
		COMMAND_ID_HANDLER(ID_VIEW_STATUS_BAR, OnViewStatusBar)
		COMMAND_ID_HANDLER(ID_APP_ABOUT, OnAppAbout)
		CHAIN_MSG_MAP(CUpdateUI<CMainFrame>)
    // CFrameWindowImpl<CMainFrame> のメッセージ ハンドラを呼び出す
		CHAIN_MSG_MAP(CFrameWindowImpl<CMainFrame>)
	END_MSG_MAP()


// Handler prototypes (uncomment arguments if needed):
//	LRESULT MessageHandler(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
//	LRESULT CommandHandler(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
//	LRESULT NotifyHandler(int /*idCtrl*/, LPNMHDR /*pnmh*/, BOOL& /*bHandled*/)

	LRESULT OnCreate(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
	{
		// ツールバーを作成
		/*
	  フレームウィンドウにツールバーを追加するにはCreateSimpleToolBar()を呼び出します。
		CreateSimpleToolBar()には3つの引数を渡すことができます。
		第1引数にはツールバーリソースIDを指定します。
		デフォルト引数は0で、この場合は共通リソースID（今回の例ではIDR_MAINFRAME）が使用されます。
		第2引数にはツールバーのスタイルを指定します。
		デフォルト引数はATL_SIMPLE_TOOLBAR_STYLEで、これは「atlframe.h」ヘッダで次のように定義されています。
    ATL_SIMPLE_TOOLBAR_STYLE定義
    #define ATL_SIMPLE_TOOLBAR_STYLE \
      (WS_CHILD | WS_VISIBLE | WS_CLIPCHILDREN | WS_CLIPSIBLINGS | TBSTYLE_TOOLTIPS)
　  第3引数には識別子を指定します。デフォルト引数はATL_IDW_TOOLBARです。
　  CreateSimpleToolBar()は、内部でCreateSimpleToolBarCtrl()というメンバ関数を呼び出しており、
	  その呼び出しによって得られたツールバーのハンドルを、
		CFrameWindowImplの基底クラスであるCFrameWindowImplBaseクラスのm_hWndToolBarというHWND型の
		メンバ変数に代入します。
		*/
		CreateSimpleToolBar();

		// ステータスバーを作成
		/*
		フレームウィンドウにステータスバーを追加するにはCreateSimpleStatusBar()を呼び出します。
		CreateSimpleStatusBar()には2つバージョンがあります。
		両方とも3つの引数を指定できますが、一方は、第1引数にアイドル時に表示する文字列を指定するもので、
		他方は、第1引数にアイドル時に表示する文字列リソースのIDを指定するものです。
		後者はすべての引数を省略可能です。
		今回はすべての引数を省略したので後者が使用されます。
		この場合、第1引数にはデフォルト引数としてATL_IDS_IDLEMESSAGEが指定されます。
		文字列リソースでATL_IDS_IDLEMESSAGEに文字列を割り当てると、
		アイドル時にその文字列がステータスバーに表示されます。
		両バージョンとも第2引数はステータスバーのスタイルです。
		デフォルト引数は次のスタイルです。
		ステータスバーのデフォルトスタイル
		WS_CHILD | WS_VISIBLE | WS_CLIPCHILDREN | WS_CLIPSIBLINGS | SBARS_SIZEGRIP
　	両バージョンとも第3引数は識別子です。デフォルト引数はATL_IDW_STATUS_BARです。
　	どちらのバージョンのCreateSimpleStatusBar()も、結果的に内部で::CreateStatusWindow()を呼び出しており、
		その呼び出しによって得られたステータスバーのハンドルを、
		CFrameWindowImplの基底クラスであるCFrameWindowImplBaseクラスのm_hWndStatusBarという
		HWND型のメンバ変数に代入します。
		*/
		CreateSimpleStatusBar();

		UIAddToolBar(m_hWndToolBar);
		UISetCheck(ID_VIEW_TOOLBAR, 1);
		UISetCheck(ID_VIEW_STATUS_BAR, 1);

		// register object for message filtering and idle updates
		CMessageLoop* pLoop = _Module.GetMessageLoop();
		ATLASSERT(pLoop != NULL);
		pLoop->AddMessageFilter(this);
		pLoop->AddIdleHandler(this);

		return 0;
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


	LRESULT OnPaint(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled)
	{
		CPaintDC dc(m_hWnd);
		CRect rect;
		GetClientRect(rect);
		dc.DrawText(_T("Hello, ATL/WTL"), -1, rect, DT_SINGLELINE | DT_CENTER | DT_VCENTER);
		
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
		BOOL bVisible = !::IsWindowVisible(m_hWndToolBar);
		::ShowWindow(m_hWndToolBar, bVisible ? SW_SHOWNOACTIVATE : SW_HIDE);
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
