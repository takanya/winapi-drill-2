/*
ウィンドウを作成するためにはATLのCWindowImplクラスを使用しましたが、
ダイアログを作成するためにはATLのCDialogImplクラスを使用します。
CMainDlgクラス内でWM_INITDIALOGと各ボタンメッセージへの応答を定義しています。
*/
class CMainDlg : public CDialogImpl<CMainDlg>
{
public:
  enum { IDD = IDD_DIALOG1 };

  // メッセージマップ
  BEGIN_MSG_MAP_EX(CMainDlg)
    MSG_WM_INITDIALOG(OnInitDialog)
    COMMAND_ID_HANDLER_EX(IDC_BUTTON_HELLO, OnHello)
    COMMAND_ID_HANDLER_EX(IDOK, OnOK)
    COMMAND_ID_HANDLER_EX(IDCANCEL, OnCancel)
    END_MSG_MAP()

  LRESULT OnInitDialog(HWND hWnd, LPARAM lParam) {
    // スクリーンの中央に配置
    CenterWindow();

    // 大きいアイコン設定
    HICON hIcon = AtlLoadIconImage(IDI_ICON1, LR_DEFAULTCOLOR,
      ::GetSystemMetrics(SM_CXICON),
      ::GetSystemMetrics(SM_CYICON));
    SetIcon(hIcon, TRUE);

    // 小さいアイコン設定
    HICON hIconSmall = AtlLoadIconImage(IDI_ICON1, LR_DEFAULTCOLOR,
      ::GetSystemMetrics(SM_CXSMICON),
      ::GetSystemMetrics(SM_CYSMICON));
    SetIcon(hIconSmall, FALSE);

    return TRUE;
  }

  // ［Hello］ボタンハンドラ
  void OnHello(UINT uNotifyCode, int nID, HWND hWndCtl) {
    MessageBox(_T("Hello, ATL/WTL"));
  }

  // ［OK］ボタンハンドラ
  void OnOK(UINT uNotifyCode, int nID, HWND hWndCtl) {
    EndDialog(nID);
  }

  // ［キャンセル］ボタンハンドラ
  void OnCancel(UINT uNotifyCode, int nID, HWND hWndCtl) {
    EndDialog(nID);
  }
};