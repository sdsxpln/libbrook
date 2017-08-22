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
 * This software is licensed as described in the file COPYING, which
 * you should have received as part of this distribution. The terms
 * are also available at https://brookframework.org/docs/copyright.html.
 *
 * You may opt to use, copy, modify, merge, publish, distribute and/or sell
 * copies of the Software, and permit persons to whom the Software is
 * furnished to do so, under the terms of the COPYING file.
 *
 * This software is distributed on an "AS IS" basis, WITHOUT WARRANTY OF ANY
 * KIND, either express or implied.
 *)

unit frmMain;

{$I Examples.inc}

interface

uses
  SysUtils,
  Classes,
{$IFDEF FPC}
  LCLIntf,
{$ELSE}
  Windows,
  ShellAPI,
  Actions,
  Controls,
  UITypes,
{$ENDIF}
  Graphics,
  StdCtrls,
  ActnList,
  Spin,
  Forms,
  BrookHTTPServerConfiguration,
  BrookHTTPServerRequest,
  BrookHTTPServerResponse,
  BrookHTTPServer;

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
    lbLink: TLabel;
    lbPort: TLabel;
    edPort: TSpinEdit;
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
    procedure lbLinkClick(Sender: TObject);
    procedure lbLinkMouseEnter(Sender: TObject);
    procedure lbLinkMouseLeave(Sender: TObject);
  protected
    procedure DoUpdateActions(AEnabled: Boolean);
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

procedure TfrMain.BrookHTTPServer1Error(ASender: TObject;
  const AErrorMsg: string);
begin
  raise EInvalidOpException.Create(AErrorMsg);
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
