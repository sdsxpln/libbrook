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
