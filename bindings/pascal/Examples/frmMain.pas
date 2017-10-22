(*    _____   _____    _____   _____   _   __
 *   |  _  \ |  _  \  /  _  \ /  _  \ | | / /
 *   | |_) | | |_) |  | | | | | | | | | |/ /
 *   |  _ <  |  _ <   | | | | | | | | |   (
 *   | |_) | | | \ \  | |_| | | |_| | | |\ \
 *   |_____/ |_|  \_\ \_____/ \_____/ |_| \_\ 4-REST.
 *
 *   –– a small tool which helps you write quickly REST APIs.
 *
 * Copyright (c) 2012-2017 Silvio Clecio, et al.
 *
 * This file is part of Brook4-REST.
 *
 * Brook4-REST is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Brook4-REST is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with Brook4-REST.  If not, see <http://www.gnu.org/licenses/>.
 *)

unit frmMain;

{$I Examples.inc}

interface

uses
  SysUtils,
  Classes,
{$IFDEF FPC}
  LMessages,
  LCLIntf,
{$ELSE}
  Windows,
  Messages,
  ShellAPI,
  Actions,
  Controls,
  UITypes,
{$ENDIF}
  Graphics,
  StdCtrls,
  ActnList,
  ExtCtrls,
  Spin,
  Forms,
  BrookHTTPServerConfiguration,
  BrookHTTPServerRequest,
  BrookHTTPServerResponse,
  BrookHTTPServer;

const
  WM_SHOW_ERROR_MSG = WM_USER + $0001;

type

  { TfrMain }

  TfrMain = class(TForm)
    acStart: TAction;
    acStop: TAction;
    alMain: TActionList;
    BrookHTTPServer1: TBrookHTTPServer;
    BrookHTTPServerConfiguration1: TBrookHTTPServerConfiguration;
    btStart: TButton;
    btStop: TButton;
    btErrorsClose: TButton;
    lbLink: TLabel;
    lbPort: TLabel;
    edPort: TSpinEdit;
    txErrors: TMemo;
    pnErrorsCap: TPanel;
    pnErrors: TPanel;
    procedure acStartExecute(Sender: TObject);
    procedure acStopExecute(Sender: TObject);
    procedure BrookHTTPServer1AfterClose(Sender: TObject);
    procedure BrookHTTPServer1AfterOpen(Sender: TObject);
    procedure BrookHTTPServer1Error(ASender: TObject; const AErrorMsg: string);
    procedure BrookHTTPServer1Request(ASender: TObject;
      ARequest: TBrookHTTPServerRequest; AResponse: TBrookHTTPServerResponse;
      var ADone: Boolean);
    procedure BrookHTTPServer1RequestError(ASender: TObject;
      ARequest: TBrookHTTPServerRequest; AResponse: TBrookHTTPServerResponse;
      const AErrorMsg: string; var ADone: Boolean);
    procedure btErrorsCloseClick(Sender: TObject);
    procedure lbLinkClick(Sender: TObject);
    procedure lbLinkMouseEnter(Sender: TObject);
    procedure lbLinkMouseLeave(Sender: TObject);
  protected
    procedure DoUpdateActions(AEnabled: Boolean);
    procedure WMShowErrorMsg(var AMessage: TMessage); message WM_SHOW_ERROR_MSG;
  end;

var
  frMain: TfrMain;

implementation

{$IFDEF FPC}
{$R *.lfm}
{$ELSE}
{$R *.dfm}
{$ENDIF}

{ TfrMain }

procedure TfrMain.acStartExecute(Sender: TObject);
begin
  BrookHTTPServer1.Port := edPort.Value;
  BrookHTTPServer1.Open;
end;

procedure TfrMain.acStopExecute(Sender: TObject);
begin
  BrookHTTPServer1.Close;
end;

procedure TfrMain.BrookHTTPServer1AfterOpen(Sender: TObject);
begin
  DoUpdateActions(BrookHTTPServer1.Active);
end;

procedure TfrMain.BrookHTTPServer1AfterClose(Sender: TObject);
begin
  DoUpdateActions(BrookHTTPServer1.Active);
end;

procedure TfrMain.DoUpdateActions(AEnabled: Boolean);
begin
  acStart.Enabled := AEnabled;
  acStop.Enabled := not acStart.Enabled;
  edPort.Enabled := acStart.Enabled;
  lbLink.Enabled := acStop.Enabled;
  lbLink.Caption := Concat('http://localhost:', IntToStr(BrookHTTPServer1.Port));
end;

procedure TfrMain.WMShowErrorMsg(var AMessage: TMessage);
begin
  with TStringStream(AMessage.WParam) do
  try
    txErrors.Lines.BeginUpdate;
    try
      if txErrors.Lines.Count < 10 then
        txErrors.Lines.Add(Concat('[', DateTimeToStr(Now), ']: ', DataString));
      pnErrors.Show;
    finally
      txErrors.Lines.EndUpdate;
    end;
  finally
    Free;
  end;
end;

procedure TfrMain.BrookHTTPServer1Request(ASender: TObject;
  ARequest: TBrookHTTPServerRequest; AResponse: TBrookHTTPServerResponse;
  var ADone: Boolean);
begin
  AResponse.Write('<html><body>Hello world</body></html>');
end;

procedure TfrMain.BrookHTTPServer1RequestError(ASender: TObject;
  ARequest: TBrookHTTPServerRequest; AResponse: TBrookHTTPServerResponse;
  const AErrorMsg: string; var ADone: Boolean);
begin
  AResponse.Write(
    '<html><body><font color="red">%s</font></body></html>', [AErrorMsg]);
end;

procedure TfrMain.btErrorsCloseClick(Sender: TObject);
begin
  txErrors.Clear;
  pnErrors.Hide;
end;

procedure TfrMain.BrookHTTPServer1Error(ASender: TObject;
  const AErrorMsg: string);
begin
  PostMessage(Handle, WM_SHOW_ERROR_MSG,
    NativeInt(TStringStream.Create(AErrorMsg)), 0);
end;

procedure TfrMain.lbLinkMouseEnter(Sender: TObject);
begin
  lbLink.Font.Style := lbLink.Font.Style + [fsUnderline];
end;

procedure TfrMain.lbLinkMouseLeave(Sender: TObject);
begin
  lbLink.Font.Style := lbLink.Font.Style - [fsUnderline];
end;

procedure TfrMain.lbLinkClick(Sender: TObject);
begin
{$IFDEF FPC}
  OpenURL(lbLink.Caption);
{$ELSE}
  ShellExecute(0, 'open', PChar(lbLink.Caption), nil, nil, SW_SHOWNORMAL);
{$ENDIF}
end;

end.
