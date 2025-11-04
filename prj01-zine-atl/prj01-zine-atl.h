#pragma once

/*
* CMyWindowクラスはCWindowImplクラスから派生していますが、
* CWindowImplの第1テンプレート引数にもCMyWindowという名前を渡します
* （第2、3テンプレート引数は省略可能です。ここでは省略しています）
*/
class CMyWindow : public CWindowImpl<CMyWindow>
{
public:
  /*
  * ウィンドウクラス名をDECLARE_WND_CLASSマクロによって登録し、
  * メッセージマップによってメッセージとそれに対するハンドラを結びつけています。
  * 今回の例では、ハンドラ内の処理はSDKスタイルとほぼ同等です。
  */
  /*
  * SDKスタイルのWindowsプログラミングでは、ウィンドウクラス名や背景色、
  * カーソルなどの属性を指定するためにWNDCLASSEX構造体を埋めていきますが、
  * ATLにはその作業を単純化するマクロや構造体が用意されています。
  *  DECLARE_WND_CLASS(ウィンドウクラス名)
  *  DECLARE_WND_CLASS_EX(ウィンドウクラス名, スタイル, 背景色)
  */
  /*
  * CWndClassInfo構造体を使うとウィンドウクラス名マクロよりもウィンドウの属性を細かく指定できる
  */
  // ウィンドウクラス名を登録
  DECLARE_WND_CLASS(_T("Hello"));

private:
  /*
  * ウィンドウがメッセージを受け取ると、メッセージマップの先頭から順に検索されるので、
  * 頻繁に使用されるメッセージを最初の方に記述しておくと良いでしょう。
  * メッセージマップ中に対応するメッセージマクロが見つからなければ、
  * メッセージはデフォルトウィンドウプロシージャに渡されます。
  * 
　* ATLが用意するメッセージマクロは大きく分けて3種類あります。
  * それは汎用メッセージハンドラマクロ、コマンドメッセージハンドラマクロ、通知メッセージハンドラマクロの3つです。
  */
  /*
  * 汎用メッセージハンドラマクロ
　* すべてのメッセージを対象とします。汎用メッセージハンドラマクロには次の2つがあります。
  *  MESSAGE_HANDLER(メッセージ名, ハンドラ名)
  *  MESSAGE_RANGE_HANDLER(開始位置のメッセージ名, 終了位置のメッセージ名, ハンドラ名)
  * 汎用メッセージハンドラ関数のプロトタイプ
  *  LRESULT MessageHandler(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
　* uMsgはメッセージを識別し、wParamとlParamはメッセージパラメータです。
  * メッセージパラメータの内容はメッセージの種類によって変わります。 
  * bHandledはメッセージの処理を終えたかどうかを示すフラグです。 
  * bHandledがハンドラ関数の中でFALSEに設定されていると、メッセージマップの残りの部分で、
  * そのメッセージのためのハンドラが別にないかどうかが検索されます。 
  * bHandledはハンドラ関数の呼び出し前にTRUEに設定されるので、
  * ハンドラ関数がbHandledを明示的にFALSEに設定しない限り、それ以上のハンドラ検索処理は行われません。
  */
  /*
  * コマンドメッセージハンドラマクロ
　* コマンドメッセージ（WM_COMMAND）を対象とします。コマンドメッセージハンドラマクロには次の4つがあります。
  *  COMMAND_HANDLER(コントロールID, 通知コード, ハンドラ名)
  *  COMMAND_ID_HANDLER(コントロールID, ハンドラ名)
  *  COMMAND_CODE_HANDLER(通知コード, ハンドラ名)
  *  COMMAND_RANGE_HANDLER(開始位置のメッセージ名, 終了位置のメッセージ名, ハンドラ名)
  * コマンドメッセージハンドラ関数のプロトタイプ
  *  LRESULT CommandHandler(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
　* wNotifyCodeは通知コード、wIDはコマンドを送信しているコントロールの識別子、
  * hWndCtlはコマンドを送信しているコントロールのハンドル、bHandledは前述のフラグです。
  */
  /*
  * 通知メッセージハンドラマクロ
　* 通知メッセージ（WM_NOTIFY）を対象とします。通知メッセージハンドラマクロには次の4つがあります。
  *  NOTIFY_HANDLER(コントロールID, 通知コード, ハンドラ名)
  *  NOTIFY_ID_HANDLER(コントロールID, ハンドラ名)
  *  NOTIFY_CODE_HANDLER(通知コード, ハンドラ名)
  *  NOTIFY_RANGE_HANDLER(開始位置のメッセージ名, 終了位置のメッセージ名, ハンドラ名)
  * 通知メッセージハンドラ関数のプロトタイプ
  *  LRESULT NotifyHandler(int idCtrl, LPNMHDR pnmh, BOOL& bHandled);
　* idCtrlは通知を送信しているコントロールの識別子、pnmhはNMHDR構造体へのポインタ、 bHandledは前述のフラグです。
  */
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
    
    DrawText(hDC, _T("Hello, ATL/WTL"), -1, &rect, DT_SINGLELINE | DT_CENTER | DT_VCENTER);
    
    EndPaint(&ps);
    return 0;
  }

  LRESULT OnDestroy(UINT, WPARAM, LPARAM, BOOL&) {
    PostQuitMessage(0);
    return 0;
  }
};