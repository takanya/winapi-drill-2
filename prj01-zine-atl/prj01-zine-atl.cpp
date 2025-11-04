// prj01-zine-atl.cpp : アプリケーションのエントリ ポイントを定義します。
//

#include "framework.h"
#include "prj01-zine-atl.h"

#define MAX_LOADSTRING 100


CComModule _Module;

int WINAPI _tWinMain(HINSTANCE hInstance,
  HINSTANCE, LPTSTR lpCmdLine, int nCmdShow)
{
  // 空のオブジェクトマップを渡すことで警告を回避
  static ATL::_ATL_OBJMAP_ENTRY* pObjMap = nullptr;
  _Module.Init(pObjMap, hInstance);

  // ウィンドウを作成
  CMyWindow wnd;
  wnd.Create(NULL, CWindow::rcDefault, _T("Hello, ATL/WTL"), WS_OVERLAPPEDWINDOW | WS_VISIBLE);
  /*
  * ATLでは、ウィンドウのスタイルは、CWinTraitsクラスによる「ウィンドウ特性」として表現できます
  * 上では、CWindowImpl::Create()の第4引数でウィンドウスタイルを指定しました
  * CWindowImplクラスの第3テンプレート引数に独自のウィンドウ特性を指定することによって、
  * 独自のウィンドウスタイルを指定することも可能です
  * しかし、CWindowImpl::Create()で指定したウィンドウスタイルの方が優先されます
  */

  MSG msg;
  while (GetMessage(&msg, NULL, 0, 0) > 0) {
    TranslateMessage(&msg);
    DispatchMessage(&msg);
  }

  _Module.Term();

  return static_cast<int>(msg.wParam);
}
