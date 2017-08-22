object frMainUplds: TfrMainUplds
  Left = 248
  Top = 137
  Caption = 'frMainUplds'
  ClientHeight = 200
  ClientWidth = 400
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'Tahoma'
  Font.Style = []
  OldCreateOrder = True
  Position = poDesktopCenter
  OnShow = FormShow
  DesignSize = (
    400
    200)
  PixelsPerInch = 96
  TextHeight = 13
  object lbInfo: TLabel
    Left = 16
    Top = 8
    Width = 103
    Height = 13
    Caption = '&Uploads directory:'
    Color = clBtnFace
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -11
    Font.Name = 'Tahoma'
    Font.Style = [fsBold]
    ParentColor = False
    ParentFont = False
  end
  object lbDirectory: TLabel
    Left = 16
    Top = 32
    Width = 368
    Height = 153
    Anchors = [akLeft, akTop, akRight, akBottom]
    AutoSize = False
    BiDiMode = bdLeftToRight
    Caption = 'lbDirectory'
    Color = clBtnFace
    ParentBiDiMode = False
    ParentColor = False
    WordWrap = True
  end
  object lbLink: TLabel
    Left = 282
    Top = 8
    Width = 102
    Height = 13
    Cursor = crHandPoint
    Anchors = [akTop, akRight]
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
end
