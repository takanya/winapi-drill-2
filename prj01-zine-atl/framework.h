// header.h : 標準のシステム インクルード ファイルのインクルード ファイル、
// またはプロジェクト専用のインクルード ファイル
//

#pragma once

#include "targetver.h"
#define WIN32_LEAN_AND_MEAN             // Windows ヘッダーからほとんど使用されていない部分を除外する
// Windows ヘッダー ファイル
#include <windows.h>
// C ランタイム ヘッダー ファイル
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>

// added
#include <atlbase.h>

/*
* まず、ATLを使用するためのヘッダをインクルードしますが、
* CComModuleクラスのインスタンスである _ModuleはATLの各ヘッダから参照されるので
* グローバルに宣言しておきます。
* この_Moduleは、_tWinMain()の最初と最後で初期化と後始末をしています。
*/
extern CComModule _Module;
#include <atlwin.h>
