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

unit frmMainUplds;

{$I Examples.inc}

interface

uses
  Classes,
{$IFDEF FPC}
  LCLIntf,
{$ELSE}
  Controls,
  Windows,
  ShellAPI,
  UITypes,
{$ENDIF}
  Graphics,
  StdCtrls,
  Forms,
  dmlMainUplds;

type

  { TfrMainUplds }

  TfrMainUplds = class(TForm)
    lbInfo: TLabel;
    lbDirectory: TLabel;
    lbLink: TLabel;
    procedure FormShow(Sender: TObject);
    procedure lbLinkClick(Sender: TObject);
    procedure lbLinkMouseEnter(Sender: TObject);
    procedure lbLinkMouseLeave(Sender: TObject);
  end;

var
  frMainUplds: TfrMainUplds;

implementation

{$IFDEF FPC}
{$R *.lfm}
{$ELSE}
{$R *.dfm}
{$ENDIF}

{ TfrMainUplds }

procedure TfrMainUplds.FormShow(Sender: TObject);
begin
  with dmMainUplds do
  begin
    lbDirectory.Caption := BrookHTTPServerConfiguration1.UploadsDir;
    BrookHTTPServer1.Open;
    lbLink.Enabled := True;
  end;
end;

procedure TfrMainUplds.lbLinkMouseEnter(Sender: TObject);
begin
  lbLink.Font.Style := lbLink.Font.Style + [fsUnderline];
end;

procedure TfrMainUplds.lbLinkMouseLeave(Sender: TObject);
begin
  lbLink.Font.Style := lbLink.Font.Style - [fsUnderline];
end;

procedure TfrMainUplds.lbLinkClick(Sender: TObject);
begin
{$IFDEF FPC}
  OpenURL(lbLink.Caption);
{$ELSE}
  ShellExecute(0, 'open', PChar(lbLink.Caption), nil, nil, SW_SHOWNORMAL);
{$ENDIF}
end;

end.
