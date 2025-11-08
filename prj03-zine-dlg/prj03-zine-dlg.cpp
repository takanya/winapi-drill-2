#include "stdafx.h"

#include "resource.h"

#include "prj03-zine-dlg.h"

CAppModule _Module;

int WINAPI _tWinMain(HINSTANCE hInstance, HINSTANCE,
  LPTSTR lpCmdLine, int nCmdShow)
{
  _Module.Init(NULL, hInstance);

  // モーダルダイアログ表示
  CMainDlg dlgMain;
  int nRet = dlgMain.DoModal();

  _Module.Term();

  return nRet;
}
