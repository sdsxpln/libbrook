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
  BFCheckLibrary;
  Result := b4r_alloc(ASize);
end;

procedure BrookFree(APtr: Pointer);
begin
  BFCheckLibrary;
  b4r_free(APtr);
end;

function BrookIsEmpty(const AStr: string): Boolean;
begin
  BFCheckLibrary;
  Result := b4r_is_empty(S2C(AStr));
end;

function BrookASprintfVA(const AFmt: string; AVA: Pointer): string;
var
  VRes: Pcchar;
begin
  BFCheckLibrary;
  VRes := b4r_asprintf_va(S2C(AFmt), AVA);
  Result := C2S(VRes);
  b4r_free(VRes);
end;

function BrookTmpDir: string;
begin
  BFCheckLibrary;
  Result := C2S(b4r_tmp_dir);
end;

function BrookUuid(out AUuid: string): Boolean;
var
  Vuuid: array[0..B4R_UUID_STR_LEN] of cchar;
begin
  BFCheckLibrary;
  Result := b4r_uuid(Vuuid);
  if Result then
    AUuid := C2S(Vuuid);
end;

end.
