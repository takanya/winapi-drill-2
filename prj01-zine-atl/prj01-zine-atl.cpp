// prj01-zine-atl.cpp : アプリケーションのエントリ ポイントを定義します。
//

#include "framework.h"
#include "prj01-zine-atl.h"

#define MAX_LOADSTRING 100


CComModule _Module;

int WINAPI _tWinMain(HINSTANCE hInstance,
  HINSTANCE, LPTSTR lpCmdLine, int nCmdShow)
{
  _Module.Init(NULL, hInstance);

  // ウィンドウを作成
  CMyWindow wnd;
  wnd.Create(NULL, CWindow::rcDefault,
    _T("Hello, ATL/WTL"), WS_OVERLAPPEDWINDOW | WS_VISIBLE);

  MSG msg;
  while (GetMessage(&msg, NULL, 0, 0) > 0) {
    TranslateMessage(&msg);
    DispatchMessage(&msg);
  }

  _Module.Term();

  return static_cast<int>(msg.wParam);
}
