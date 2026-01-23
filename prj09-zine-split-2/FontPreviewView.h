#pragma once

/*
* スプリッタウィンドウの右ペインは、左ペインで選択されたフォントを表示します。
* フォントプレビューペインは一般的なビューウィンドウです。
* WM_PAINTメッセージハンドラでフォント名を描画しますが、そのフォント名はSetFontName()という
* publicなメンバ関数を外部から呼び出すことによって設定します。
*/
class CFontPreviewView : public CWindowImpl<CFontPreviewView>
{
  CString m_strFontName;

public:
  BOOL PreTranslateMessage(MSG* pMsg) {
    return FALSE;
  }

  // メッセージマップ
  BEGIN_MSG_MAP_EX(CFontPreviewView)
    MSG_WM_PAINT(OnPaint)
    END_MSG_MAP()

  void OnPaint(HDC /*hDC*/) {
    CPaintDC dc(m_hWnd);

    if (!m_strFontName.IsEmpty()) {
      // サイズ20ポイントのフォントを作成し、選択
      CFont font;
      font.CreatePointFont(200, m_strFontName);
      HFONT hOldFont = dc.SelectFont(font);

      // ビューウィンドウにフォント名を描画
      CRect rect;
      GetClientRect(rect);
      dc.SetBkMode(TRANSPARENT);
      dc.DrawText(m_strFontName, m_strFontName.GetLength(),
        rect, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

      // 元のフォントを選択
      dc.SelectFont(hOldFont);
    }
  }

  void SetFontName(LPCTSTR lpszFontName) {
    m_strFontName = lpszFontName;
    Invalidate();
  }
};
