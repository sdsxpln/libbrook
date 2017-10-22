object frMain: TfrMain
  Left = 248
  Top = 137
  BorderIcons = [biSystemMenu, biMinimize]
  BorderStyle = bsSingle
  Caption = 'HTTP server'
  ClientHeight = 220
  ClientWidth = 347
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'Tahoma'
  Font.Style = []
  OldCreateOrder = True
  Position = poDesktopCenter
  PixelsPerInch = 96
  TextHeight = 13
  object lbPort: TLabel
    Left = 20
    Top = 20
    Width = 20
    Height = 13
    Caption = 'Port'
    Color = clBtnFace
    ParentColor = False
  end
  object lbLink: TLabel
    Left = 20
    Top = 64
    Width = 102
    Height = 13
    Cursor = crHandPoint
    Caption = 'http://localhost:8080'
    Color = clBtnFace
    Enabled = False
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clBlue
    Font.Height = -11
    Font.Name = 'Tahoma'
    Font.Style = []
    ParentColor = False
    ParentFont = False
    OnClick = lbLinkClick
    OnMouseEnter = lbLinkMouseEnter
    OnMouseLeave = lbLinkMouseLeave
  end
  object btStart: TButton
    Left = 121
    Top = 16
    Width = 75
    Height = 25
    Action = acStart
    TabOrder = 0
  end
  object btStop: TButton
    Left = 202
    Top = 16
    Width = 75
    Height = 25
    Action = acStop
    TabOrder = 1
  end
  object edPort: TSpinEdit
    Left = 49
    Top = 17
    Width = 66
    Height = 22
    MaxValue = 65535
    MinValue = 1
    TabOrder = 2
    Value = 8080
  end
  object pnErrors: TPanel
    Left = 0
    Top = 96
    Width = 347
    Height = 124
    Align = alBottom
    Anchors = [akLeft, akTop, akRight, akBottom]
    BevelOuter = bvNone
    TabOrder = 3
    Visible = False
    object pnErrorsCap: TPanel
      Left = 0
      Top = 0
      Width = 347
      Height = 24
      Align = alTop
      AutoSize = True
      BevelOuter = bvNone
      Caption = 'Errors'
      Color = clSilver
      Constraints.MaxHeight = 24
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -11
      Font.Name = 'Tahoma'
      Font.Style = [fsBold]
      ParentFont = False
      TabOrder = 0
      object btErrorsClose: TButton
        Left = 327
        Top = 0
        Width = 20
        Height = 24
        Align = alRight
        Caption = 'X'
        TabOrder = 0
        OnClick = btErrorsCloseClick
      end
    end
    object txErrors: TMemo
      Left = 0
      Top = 24
      Width = 347
      Height = 100
      Align = alClient
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clMaroon
      Font.Height = -11
      Font.Name = 'Tahoma'
      Font.Style = []
      ParentFont = False
      ReadOnly = True
      TabOrder = 1
      WordWrap = False
    end
  end
  object alMain: TActionList
    Left = 32
    Top = 48
    object acStart: TAction
      Category = 'Server'
      Caption = '&Start'
      ShortCut = 120
      OnExecute = acStartExecute
    end
    object acStop: TAction
      Category = 'Server'
      Caption = 'Sto&p'
      Enabled = False
      ShortCut = 16497
      OnExecute = acStopExecute
    end
  end
  object BrookHTTPServerConfiguration1: TBrookHTTPServerConfiguration
    Left = 200
    Top = 56
  end
  object BrookHTTPServer1: TBrookHTTPServer
    Configuration = BrookHTTPServerConfiguration1
    AfterOpen = BrookHTTPServer1AfterOpen
    AfterClose = BrookHTTPServer1AfterClose
    OnRequest = BrookHTTPServer1Request
    OnRequestError = BrookHTTPServer1RequestError
    OnError = BrookHTTPServer1Error
    Left = 96
    Top = 112
  end
end
