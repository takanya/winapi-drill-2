#pragma once

class CMyWindow : public CWindowImpl<CMyWindow>,
  public CMessageFilter, public CIdleHandler
{
public:
  // ウィンドウクラス名を登録
  DECLARE_WND_CLASS(_T("Hello"));

private:
  // メッセージフィルタ処理
  virtual BOOL PreTranslateMessage(MSG* pMsg) {
    return FALSE;
  }

  // アイドル処理
  virtual BOOL OnIdle() {
    return FALSE;
  }

  // メッセージマップ
  BEGIN_MSG_MAP_EX(CMyWindow)
    MSG_WM_PAINT(OnPaint)
    MSG_WM_CREATE(OnCreate)
    MSG_WM_DESTROY(OnDestroy)
    END_MSG_MAP()

  void OnPaint(HDC /*hDC*/) {
    CPaintDC dc(m_hWnd);
    CRect rect;
    GetClientRect(rect);
    dc.DrawText(_T("Hello, ATL/WTL"), -1,
      rect, DT_SINGLELINE | DT_CENTER | DT_VCENTER);
  }

  LRESULT OnCreate(LPCREATESTRUCT lpcs) {
    // メッセージループにメッセージフィルタとアイドルハンドラを追加
    CMessageLoop* pLoop = _Module.GetMessageLoop();
    pLoop->AddMessageFilter(this);
    pLoop->AddIdleHandler(this);
    return 0;
  }

  void OnDestroy() {
    PostQuitMessage(0);
  }
};