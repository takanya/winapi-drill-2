#pragma once

class CFontListView : public CWindowImpl<CFontListView, CListBox>
{
public:
  DECLARE_WND_SUPERCLASS(NULL, CListBox::GetWndClassName())

  BOOL PreTranslateMessage(MSG* pMsg) {
    return FALSE;
  }

  // メッセージマップ
  BEGIN_MSG_MAP_EX(CFontListView)
    MSG_WM_CREATE(OnCreate)
    END_MSG_MAP()

  LRESULT OnCreate(LPCREATESTRUCT lpcs) {
    LRESULT lRet = DefWindowProc();

    SetFont(AtlGetDefaultGuiFont());

    CClientDC dc(m_hWnd);
    EnumFontFamilies(dc, NULL,
      (FONTENUMPROC)FontProc, (LPARAM)this);

    return lRet;
  }

  static int CALLBACK FontProc(ENUMLOGFONT* lpelf,
    NEWTEXTMETRIC* lpntm, int nFontType, LPARAM lParam)
  {
    CFontListView* pList = (CFontListView*)lParam;

    // TrueTypeフォント名のみリストに追加
    if (nFontType & TRUETYPE_FONTTYPE)
      pList->AddString(lpelf->elfLogFont.lfFaceName);

    return 1;
  }
};
