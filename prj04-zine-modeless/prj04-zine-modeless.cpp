// prj04-zine-modeless.cpp : main source file for prj04-zine-modeless.exe
//

#include "stdafx.h"

#include <atlframe.h>
#include <atlctrls.h>
#include <atldlgs.h>

#include "resource.h"

#include "MainDlg.h"

CAppModule _Module;

/*
　モードレスダイアログとモーダルダイアログでは、主に以下の点が異なります。

ダイアログの作成
　モーダルダイアログでは_tWinMain()内でDoModal()を呼び出しましたが、
  モードレスダイアログはCreate()を呼び出して作成します。

メッセージループの管理
　モードレスダイアログでは、メッセージループをCMessageLoopクラスで管理できるため、
  CMessageFilterクラスやCIdleHandlerクラスを使ってメッセージフィルタやアイドルハンドラを利用できます。
	今回の例ではCMainDlgクラスの基底クラスにCMessageFilterクラスやCIdleHandlerクラスを追加し、
	CMainDlgクラス内でメッセージフィルタハンドラ関数（PreTranslateMessage）と
	アイドルハンドラ関数（OnIdle）を定義しています。

ダイアログの閉じ方
　モーダルダイアログでは、［OK］ボタンハンドラと［キャンセル］ボタンハンドラで
  EndDialog()を呼び出しましたが、モードレスダイアログはDestroyWindow()と
	 ::PostQuitMessage()を呼び出して閉じます。
*/
int Run(LPTSTR /*lpstrCmdLine*/ = NULL, int nCmdShow = SW_SHOWDEFAULT)
{
	CMessageLoop theLoop;
	_Module.AddMessageLoop(&theLoop);

	// モードレスダイアログ表示
	CMainDlg dlgMain;

	if(dlgMain.Create(NULL) == NULL)
	{
		ATLTRACE(_T("Main dialog creation failed!\n"));
		return 0;
	}

	dlgMain.ShowWindow(nCmdShow);

	int nRet = theLoop.Run();

	_Module.RemoveMessageLoop();
	return nRet;
}

int WINAPI _tWinMain(HINSTANCE hInstance, HINSTANCE /*hPrevInstance*/, LPTSTR lpstrCmdLine, int nCmdShow)
{
	HRESULT hRes = ::CoInitialize(NULL);
	ATLASSERT(SUCCEEDED(hRes));

	AtlInitCommonControls(ICC_BAR_CLASSES);	// add flags to support other controls

	hRes = _Module.Init(NULL, hInstance);
	ATLASSERT(SUCCEEDED(hRes));

	int nRet = Run(lpstrCmdLine, nCmdShow);

	_Module.Term();
	::CoUninitialize();

	return nRet;
}
