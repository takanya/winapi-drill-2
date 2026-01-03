// MainDlg.h : interface of the CMainDlg class
//
/////////////////////////////////////////////////////////////////////////////

#pragma once

class CMainDlg : public CDialogImpl<CMainDlg>, public CDialogResize<CMainDlg>
{
public:
	enum { IDD = IDD_MAINDLG };

	CEdit m_edit_width;
	CEdit m_edit_height;
	CSortListViewCtrl m_list_window;  // ソートリストビュー

	// ダイアログリサイズマップ
	BEGIN_DLGRESIZE_MAP(CMainDlg)
		DLGRESIZE_CONTROL(IDC_STATIC_WIDTH, DLSZ_MOVE_X)
		DLGRESIZE_CONTROL(IDC_EDIT_WIDTH, DLSZ_MOVE_X)
		DLGRESIZE_CONTROL(IDC_STATIC_HEIGHT, DLSZ_MOVE_X)
		DLGRESIZE_CONTROL(IDC_EDIT_HEIGHT, DLSZ_MOVE_X)
		DLGRESIZE_CONTROL(IDC_BUTTON_APPLY, DLSZ_MOVE_X)
		DLGRESIZE_CONTROL(IDC_BUTTON_UPDATE, DLSZ_MOVE_X)
		DLGRESIZE_CONTROL(IDC_LIST_WINDOW, DLSZ_SIZE_X | DLSZ_SIZE_Y)
	END_DLGRESIZE_MAP()

	BEGIN_MSG_MAP(CMainDlg)
		MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
		COMMAND_ID_HANDLER(ID_APP_ABOUT, OnAppAbout)
		COMMAND_ID_HANDLER(IDOK, OnOK)
		COMMAND_ID_HANDLER(IDCANCEL, OnCancel)
		COMMAND_ID_HANDLER(IDC_BUTTON_APPLY, OnButtonApply)
		COMMAND_ID_HANDLER(IDC_BUTTON_UPDATE, OnButtonUpdate)
		CHAIN_MSG_MAP(CDialogResize<CMainDlg>)  // CDialogResizeクラスへのチェーンが必須
	END_MSG_MAP()

// Handler prototypes (uncomment arguments if needed):
//	LRESULT MessageHandler(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
//	LRESULT CommandHandler(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
//	LRESULT NotifyHandler(int /*idCtrl*/, LPNMHDR /*pnmh*/, BOOL& /*bHandled*/)

	LRESULT OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
	{
		// center the dialog on the screen
		CenterWindow();

		// set icons
		HICON hIcon = AtlLoadIconImage(IDR_MAINFRAME, LR_DEFAULTCOLOR, ::GetSystemMetrics(SM_CXICON), ::GetSystemMetrics(SM_CYICON));
		SetIcon(hIcon, TRUE);
		HICON hIconSmall = AtlLoadIconImage(IDR_MAINFRAME, LR_DEFAULTCOLOR, ::GetSystemMetrics(SM_CXSMICON), ::GetSystemMetrics(SM_CYSMICON));
		SetIcon(hIconSmall, FALSE);

		// 以下のあたり追加
		// コントロール設定
		m_edit_width = GetDlgItem(IDC_EDIT_WIDTH);
		m_edit_height = GetDlgItem(IDC_EDIT_HEIGHT);
		m_list_window.SubclassWindow(GetDlgItem(IDC_LIST_WINDOW));

		m_list_window.SetExtendedListViewStyle(
			LVS_EX_CHECKBOXES | LVS_EX_FULLROWSELECT | LVS_EX_INFOTIP);

		// ソートリストビューにカラム設定
		CRect rcList;
		m_list_window.GetWindowRect(rcList);
		int nScrollWidth = GetSystemMetrics(SM_CXVSCROLL);
		int n3DEdge = GetSystemMetrics(SM_CXEDGE);
		m_list_window.InsertColumn(0, _T("タイトル"), LVCFMT_LEFT, rcList.Width() - 100 - nScrollWidth - n3DEdge * 2, -1);
		m_list_window.InsertColumn(1, _T("幅"), LVCFMT_RIGHT, 50, -1);
		m_list_window.InsertColumn(2, _T("高さ"), LVCFMT_RIGHT, 50, -1);

		// ソートタイプ設定
		m_list_window.SetColumnSortType(0, LVCOLSORT_TEXT);
		m_list_window.SetColumnSortType(1, LVCOLSORT_LONG);
		m_list_window.SetColumnSortType(2, LVCOLSORT_LONG);

		::EnumWindows((WNDENUMPROC)SetWindowListFunc, (LPARAM)&m_list_window);

		// ダイアログリサイズ初期化
		DlgResize_Init(true, true, WS_THICKFRAME | WS_MAXIMIZEBOX | WS_CLIPCHILDREN);

		return TRUE;
	}

	LRESULT OnAppAbout(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
	{
		CSimpleDialog<IDD_ABOUTBOX, FALSE> dlg;
		dlg.DoModal();
		return 0;
	}

	LRESULT OnOK(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
	{
		// TODO: Add validation code 
		EndDialog(wID);
		return 0;
	}

	LRESULT OnCancel(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
	{
		EndDialog(wID);
		return 0;
	}

	static BOOL CALLBACK SetWindowListFunc(HWND hWnd, LPARAM lParam) {
		// ウィンドウクラス名取得
		TCHAR szClassName[256];
		::GetClassName(hWnd, szClassName, sizeof(szClassName) / sizeof(TCHAR));

		ATLTRACE2("クラスは", szClassName);

		//if(lstrcmp(szClassName, _T("IEFrame")) == 0){
		CSortListViewCtrl* pList = (CSortListViewCtrl*)lParam;

		int index = pList->GetItemCount();

		// ウィンドウのタイトル名をリストに設定
		TCHAR szTitle[256];
		::GetWindowText(hWnd, szTitle, sizeof(szTitle) / sizeof(TCHAR));
		pList->InsertItem(index, szTitle);

		// ウィンドウの幅と高さをリストに設定
		CRect rcWindow;
		::GetWindowRect(hWnd, rcWindow);
		CString tmp;
		tmp.Format(_T("%d"), rcWindow.Width());
		pList->SetItemText(index, 1, tmp);
		tmp.Format(_T("%d"), rcWindow.Height());
		pList->SetItemText(index, 2, tmp);

		// ウィンドウハンドルをリストアイテムに設定
		pList->SetItemData(index, (DWORD)hWnd);
		//}
		return TRUE;
	}

	LRESULT OnButtonApply(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/) {
		int width = GetDlgItemInt(IDC_EDIT_WIDTH, NULL, FALSE);
		int height = GetDlgItemInt(IDC_EDIT_HEIGHT, NULL, FALSE);
		for (int i = 0; i < m_list_window.GetItemCount(); i++) {
			if (m_list_window.GetCheckState(i)) {
				HWND hWnd = (HWND)m_list_window.GetItemData(i);
				::SetWindowPos(hWnd, NULL, 0, 0, width, height, SWP_NOMOVE | SWP_NOZORDER | SWP_SHOWWINDOW);
			}
		}
    return 0;
	}

	LRESULT OnButtonUpdate(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/) {
		m_list_window.DeleteAllItems();
		::EnumWindows((WNDENUMPROC)SetWindowListFunc, (LPARAM)&m_list_window);
    return 0;
	}
};
