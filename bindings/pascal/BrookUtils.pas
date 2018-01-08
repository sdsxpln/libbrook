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

unit BrookUtils;

{$I Brook.inc}

interface

uses
{$IFNDEF FPC}
  SysUtils,
{$ENDIF}
  libbrook;

function BrookAlloc(ASize: NativeUInt): Pointer;
procedure BrookFree(APtr: Pointer);
function BrookIsEmpty(const AStr: string): Boolean;
function BrookASprintfVA(const AFmt: string; AVA: Pointer): string;
function BrookTmpDir: string;
function BrookUuid(out AUuid: string): Boolean;

implementation

function BrookAlloc(ASize: NativeUInt): Pointer;
begin
  B4RCheckLibrary;
  Result := b4r_alloc(ASize);
end;

procedure BrookFree(APtr: Pointer);
begin
  B4RCheckLibrary;
  b4r_free(APtr);
end;

function BrookIsEmpty(const AStr: string): Boolean;
begin
  B4RCheckLibrary;
  Result := b4r_is_empty(S2C(AStr));
end;

function BrookASprintfVA(const AFmt: string; AVA: Pointer): string;
var
  VRes: Pcchar;
begin
  B4RCheckLibrary;
  VRes := b4r_asprintf_va(S2C(AFmt), AVA);
  Result := C2S(VRes);
  b4r_free(VRes);
end;

function BrookTmpDir: string;
begin
  B4RCheckLibrary;
  Result := C2S(b4r_tmp_dir);
end;

function BrookUuid(out AUuid: string): Boolean;
var
  Vuuid: array[0..B4R_UUID_STR_LEN] of cchar;
begin
  B4RCheckLibrary;
  Result := b4r_uuid(@Vuuid);
  if Result then
    AUuid := C2S(@Vuuid);
end;

end.
