object HTTPSRVForm: THTTPSRVForm
  Left = 412
  Top = 380
  Width = 1627
  Height = 838
  Caption = 'WEB srv'
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -13
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = False
  OnClose = FormClose
  OnCreate = FormCreate
  PixelsPerInch = 120
  TextHeight = 16
  object Panel1: TPanel
    Left = 0
    Top = 0
    Width = 1609
    Height = 99
    Align = alTop
    TabOrder = 0
    object SpeedButton1: TSpeedButton
      Left = 16
      Top = 16
      Width = 33
      Height = 33
      Glyph.Data = {
        76010000424D7601000000000000760000002800000020000000100000000100
        04000000000000010000120B0000120B00001000000000000000000000000000
        800000800000008080008000000080008000808000007F7F7F00BFBFBF000000
        FF0000FF000000FFFF00FF000000FF00FF00FFFF0000FFFFFF003FFFFFFFFFFF
        FFFF33333333333FFFFF3FFFFFFFFF00000F333333333377777F33FFFFFFFF09
        990F33333333337F337F333FFFFFFF09990F33333333337F337F3333FFFFFF09
        990F33333333337FFF7F33333FFFFF00000F3333333333777773333333FFFFFF
        FFFF3FFFFF3333333F330000033FFFFF0FFF77777F3333337FF30EEE0333FFF0
        00FF7F337FFF333777FF0EEE00033F00000F7F33777F3777777F0EEE0E033000
        00007FFF7F7FF777777700000E00033000FF777773777F3777F3330EEE0E0330
        00FF337FFF7F7F3777F33300000E033000FF337777737F37773333330EEE0300
        03FF33337FFF77777333333300000333333F3333777773333333}
      NumGlyphs = 2
      OnClick = SpeedButton1Click
    end
    object URLED: TEdit
      Left = 8
      Top = 64
      Width = 625
      Height = 24
      TabOrder = 0
      Text = 'http://nucloweb.jinr.ru/kgu/Cache/get_data.php?callback=?'
    end
    object kgu1freqtxt: TStaticText
      Left = 653
      Top = 62
      Width = 250
      Height = 28
      AutoSize = False
      BevelKind = bkSoft
      BorderStyle = sbsSunken
      Caption = 'kgu1freqtxt'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clBlack
      Font.Height = -17
      Font.Name = 'MS Sans Serif'
      Font.Style = []
      ParentFont = False
      TabOrder = 1
    end
    object kgu2freqtxt: TStaticText
      Left = 910
      Top = 62
      Width = 250
      Height = 28
      AutoSize = False
      BevelKind = bkSoft
      BorderStyle = sbsSunken
      Caption = 'kgu2freqtxt'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clBlack
      Font.Height = -17
      Font.Name = 'MS Sans Serif'
      Font.Style = []
      ParentFont = False
      TabOrder = 2
    end
    object kgu1stdtxt: TStaticText
      Left = 654
      Top = 32
      Width = 250
      Height = 28
      AutoSize = False
      BevelKind = bkSoft
      BorderStyle = sbsSunken
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clBlack
      Font.Height = -17
      Font.Name = 'MS Sans Serif'
      Font.Style = []
      ParentFont = False
      TabOrder = 3
    end
    object kgu2stdtxt: TStaticText
      Left = 911
      Top = 32
      Width = 250
      Height = 28
      AutoSize = False
      BevelKind = bkSoft
      BorderStyle = sbsSunken
      Caption = 'kgu2freqtxt'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clBlack
      Font.Height = -17
      Font.Name = 'MS Sans Serif'
      Font.Style = []
      ParentFont = False
      TabOrder = 4
    end
  end
  object Memo2: TMemo
    Left = 0
    Top = 99
    Width = 1609
    Height = 351
    Align = alTop
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -17
    Font.Name = 'MS Sans Serif'
    Font.Style = []
    ParentFont = False
    TabOrder = 1
  end
  object Memobody: TMemo
    Left = 0
    Top = 450
    Width = 1609
    Height = 343
    Align = alClient
    Lines.Strings = (
      'Memobody')
    TabOrder = 2
  end
  object Timer1: TTimer
    Enabled = False
    Interval = 10000
    OnTimer = SpeedButton1Click
    Left = 592
    Top = 160
  end
  object ApplicationEvents1: TApplicationEvents
    OnMinimize = ApplicationEvents1Minimize
    Left = 352
    Top = 16
  end
  object PopupMenu1: TPopupMenu
    Left = 416
    Top = 16
    object Restore1: TMenuItem
      Caption = 'Restore'
      OnClick = Restore1Click
    end
    object Minimize1: TMenuItem
      Caption = 'Minimize'
      OnClick = Minimize1Click
    end
    object quit1: TMenuItem
      Caption = 'quit'
      OnClick = quit1Click
    end
    object Restart1: TMenuItem
      Caption = 'Restart'
      OnClick = Restart1Click
    end
  end
end
