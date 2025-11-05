#include "stdafx.h"

#include "prj02-zine-wtl.h"

CAppModule _Module;  // CComModuleからCAppModuleに置き換える

int WINAPI _tWinMain(HINSTANCE hInstance, HINSTANCE, LPTSTR lpCmdLine, int nCmdShow)
{
  _Module.Init(NULL, hInstance);

  CMessageLoop theLoop;
  _Module.AddMessageLoop(&theLoop);

  // 独自ウィンドウを作成
  CMyWindow wnd;
  wnd.Create(NULL, CWindow::rcDefault, _T("Hello, ATL/WTL"), WS_OVERLAPPEDWINDOW | WS_VISIBLE);

  int nRet = theLoop.Run();

  _Module.RemoveMessageLoop();

  _Module.Term();

  return nRet;
}