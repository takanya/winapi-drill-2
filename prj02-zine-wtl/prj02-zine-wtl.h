#pragma once

/*
* メッセージループにCMessageLoopクラスを使用すると、
* CMessageFilterクラスを使ってメッセージフィルタを追加することができます。
* メッセージフィルタを追加すると、CMessageLoopクラス内のメッセージループで
* ::TranslateMessage()が呼び出される前に何か処理をさせることができます。
* 
* CMessageFilterクラスを利用するためには、メインウィンドウであるCMyWindowクラスを
* 次のように書き換える必要があります。 
* 
* まずはCMyWindowクラスの基底クラスにCMessageFilterクラスを追加します。
* CMessageFilterクラスにはPreTranslateMessage()という純粋仮想関数が一つあるだけです。
* 
* CMyWindowクラスではこのPreTranslateMessage()を実装します。
* 今回の例ではFALSEを返しているだけですが、これは何もせずに通常の処理を続けるということを意味します。
*/
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

  /*
  * WTLには各メッセージ専用のメッセージマクロがあり、メッセージハンドラ関数には不要な引数がありません。
  * WTLにはATLが用意する汎用メッセージハンドラマクロ、コマンドメッセージハンドラマクロ、
  * 通知メッセージハンドラマクロの拡張版が用意されています。 
  */
  // メッセージマップ
  BEGIN_MSG_MAP_EX(CMyWindow)
    MSG_WM_PAINT(OnPaint)
    MSG_WM_CREATE(OnCreate)
    MSG_WM_DESTROY(OnDestroy)
  END_MSG_MAP()

  /* SDKスタイルのWM_PAINTメッセージハンドラ
    PAINTSTRUCT ps;
    HDC hDC = BeginPaint(&ps);
    RECT rect;
    GetClientRect(&rect);
    DrawText(hDC, _T("Hello, ATL/WTL"), -1, &rect, DT_SINGLELINE | DT_CENTER | DT_VCENTER);
    EndPaint(&ps);
  */
  void OnPaint(HDC /*hDC*/) {
    CPaintDC dc(m_hWnd);

    /*
    * なお、この例ではWTLのCRectクラスを使用するために「atlmisc.h」ヘッダをインクルードする必要があります。
    * WTLにはGDIサポートクラスとしてCPen/CBrush/CFont/CPalette/CBitmap/CRgnなどのクラスが用意されています。
    */
    CRect rect;
    GetClientRect(rect);
    dc.DrawText(_T("Hello, ATL/WTL"), -1,
      rect, DT_SINGLELINE | DT_CENTER | DT_VCENTER);
  }

  /*
  * WTLには、メッセージループをカプセル化するためにCMessageLoopというクラスが用意されています。
  * このクラスを利用するためにはWTLの「atlapp.h」ヘッダをインクルードする必要があります。
  * また、CMessageLoopクラスはCAppModuleクラスによって管理されるため、
  * これまで記述してきたCComModuleクラスをCAppModuleクラスに置き換えます。
  * CAppModuleクラスも「atlapp.h」ヘッダに宣言されているため、CAppModuleクラスを使用する前に「atlapp.h」ヘッダをインクルードします。
  */
  LRESULT OnCreate(LPCREATESTRUCT lpcs) {
    // メッセージループにメッセージフィルタとアイドルハンドラを追加
    CMessageLoop* pLoop = _Module.GetMessageLoop();

    /*
    * 最後にWM_CREATEメッセージハンドラを追加し、CMessageLoopのメンバ関数であるAddMessageFilter()を呼び出して
    * メッセージループにメッセージフィルタを追加します。
    * これにより、CMessageLoopクラス内のメッセージループで::TranslateMessage()が呼び出される前に
    * CMyWindowクラスで実装したPreTranslateMessage()が呼び出されることになります。
    */
    pLoop->AddMessageFilter(this);

    /*
    * メッセージループにCMessageLoopクラスを使用すると、CIdleHandlerクラスを使って
    * アイドルハンドラを追加することができます。
    * アイドルハンドラを追加すると、CMessageLoopクラス内のメッセージループで処理するメッセージが無い時に、
    * アイドル処理をさせることができます。
    */
    pLoop->AddIdleHandler(this);
    return 0;
  }

  void OnDestroy() {
    PostQuitMessage(0);
  }
};