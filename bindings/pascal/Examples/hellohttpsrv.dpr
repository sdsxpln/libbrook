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

program hellohttpsrv;

{$I Examples.inc}

uses
{$IFDEF FPC}
 {$IFDEF UNIX}
  CThreads,
 {$ENDIF}
  Interfaces,
{$ENDIF}
  Forms,
  frmMain in 'frmMain.pas' {frMain};

{$R *.res}

begin
{$IFDEF FPC}
  RequireDerivedFormResource := True;
{$ELSE}
  Application.MainFormOnTaskbar := True;
{$ENDIF}
  Application.Initialize;
  Application.CreateForm(TfrMain, frMain);
  Application.Run;
end.
