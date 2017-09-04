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

program httpsrvuploads;

{$I Examples.inc}

uses
{$IFDEF FPC}
 {$IFDEF UNIX}
  CThreads,
 {$ENDIF}
  Interfaces,
{$ENDIF}
  Forms,
  frmMainUplds in 'frmMainUplds.pas' {frMainUplds},
  dmlMainUplds in 'dmlMainUplds.pas' {dmMainUplds};

{$R *.res}

begin
{$IFDEF FPC}
  RequireDerivedFormResource := True;
{$ELSE}
  Application.MainFormOnTaskbar := True;
{$ENDIF}
  Application.Initialize;
  Application.CreateForm(TfrMainUplds, frMainUplds);
  Application.CreateForm(TdmMainUplds, dmMainUplds);
  Application.Run;
end.
