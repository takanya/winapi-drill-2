#pragma once

class CMyWindow : public CWindowImpl<CMyWindow>
{
public:
  // ウィンドウクラス名を登録
  DECLARE_WND_CLASS(_T("Hello"));

private:
  // メッセージマップ
  BEGIN_MSG_MAP(CMyWindow)
    MESSAGE_HANDLER(WM_PAINT, OnPaint)
    MESSAGE_HANDLER(WM_DESTROY, OnDestroy)
  END_MSG_MAP()

  LRESULT OnPaint(UINT, WPARAM, LPARAM, BOOL&) {
    PAINTSTRUCT ps;
    HDC hDC = BeginPaint(&ps);
    RECT rect;
    GetClientRect(&rect);
    DrawText(hDC, _T("Hello, ATL/WTL"),
      -1, &rect, DT_SINGLELINE | DT_CENTER | DT_VCENTER);
    EndPaint(&ps);
    return 0;
  }

  LRESULT OnDestroy(UINT, WPARAM, LPARAM, BOOL&) {
    PostQuitMessage(0);
    return 0;
  }
};