#pragma once

/*
まずはビューウィンドウのためのクラスを定義します。
例ではプロジェクトに「view.h」というヘッダファイルを追加し、
そこにCWindowImplクラスから派生したCFontViewというクラスを定義しています。
CWindowImplクラスの第2引数には、ベースとなるCListViewCtrlクラスを指定します。
これにより、CFontViewはCListViewCtrlの派生クラスとなります。

CFontViewクラス内では、まずDECLARE_WND_SUPERCLASS()マクロを使って
リストビューコントロールをスーパークラス化します。
スーパークラス化とは、定義済みのウィンドウクラス（"BUTTON"など）を拡張する
新しいウィンドウクラスを定義することです。
DECLARE_WND_SUPERCLASS()の第1引数には新しいウィンドウクラス名を指定し、
第2引数には元になるウィンドウクラス名を指定します。
今回の例では第1引数にはNULLを指定して名前を任意名にし、
第2引数にはCListViewCtrl::GetWndClassName()を指定しています。
これにより、CFontViewはリストビューコントロールに基づくウィンドウクラスになります。

CFontViewクラスでは、次にWM_CREATEメッセージハンドラを追加します。
WM_CREATEメッセージハンドラでは始めにDefWindowProc()を呼び出し、
デフォルトのGUIフォントを設定してからカラムを2つ挿入して
システムで利用可能なフォント名を列挙します。

*/


class CFontView : public CWindowImpl<CFontView, CListViewCtrl>
{
public:
	DECLARE_WND_SUPERCLASS(NULL, CListViewCtrl::GetWndClassName())

	BOOL PreTranslateMessage(MSG* pMsg)
	{
		pMsg;
		return FALSE;
	}

	BEGIN_MSG_MAP(CFontView)
    MESSAGE_HANDLER(WM_CREATE, OnCreate)
	END_MSG_MAP()

// Handler prototypes (uncomment arguments if needed):
//	LRESULT MessageHandler(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
//	LRESULT CommandHandler(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
//	LRESULT NotifyHandler(int /*idCtrl*/, LPNMHDR /*pnmh*/, BOOL& /*bHandled*/)

  LRESULT OnCreate(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
  {
    LRESULT lRet = DefWindowProc();

    SetFont(AtlGetDefaultGuiFont());

    SetExtendedListViewStyle(LVS_EX_INFOTIP | LVS_EX_FULLROWSELECT);

    // リストビューにカラム挿入
    CRect rcList;
    ::GetClientRect(GetParent(), rcList);
    int nScrollWidth = GetSystemMetrics(SM_CXVSCROLL);
    int n3DEdge = GetSystemMetrics(SM_CXEDGE);
    InsertColumn(0, _T("フォント名"), LVCFMT_LEFT, 190, -1);
    InsertColumn(1, _T("種類"), LVCFMT_LEFT, rcList.Width() - 190 - nScrollWidth - n3DEdge * 2, -1);

    FillList();

    return lRet;
  }

  void FillList() {
    DeleteAllItems();

    CClientDC dc(m_hWnd);
    EnumFontFamilies(dc, NULL,
      (FONTENUMPROC)FontProc, (LPARAM)this);
  }

  static int CALLBACK FontProc(ENUMLOGFONT* lpelf, NEWTEXTMETRIC* lpntm, int nFontType, LPARAM lParam)
  {
    CFontView* pList = (CFontView*)lParam;

    // フォント名追加
    int nIndex = pList->InsertItem(pList->GetItemCount(), lpelf->elfLogFont.lfFaceName);

    if (nFontType & RASTER_FONTTYPE) {
      pList->SetItemText(nIndex, 1, _T("ラスタフォント"));
    }
    else if (nFontType & TRUETYPE_FONTTYPE) {
      pList->SetItemText(nIndex, 1, _T("TrueTypeフォント"));
    }
    else {
      pList->SetItemText(nIndex, 1, _T("ベクタフォント"));
    }

    return 1;
  }
};

