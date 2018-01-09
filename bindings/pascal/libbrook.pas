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

unit libbrook;

{$IFDEF FPC}
 {$MODE DELPHI}
 {$IFDEF VER3_0}
  {$MACRO ON}
  {$DEFINE MarshaledAString := PAnsiChar}
 {$ENDIF}
{$ENDIF}

interface

uses
  SysUtils,
{$IFDEF FPC}
 {$IFDEF UNIX}
  BaseUnix,
 {$ENDIF}
  DynLibs,
{$ELSE}
 {$IF DEFINED(MSWINDOWS)}
  Winapi.Windows
 {$ELSEIF DEFINED(POSIX)}
  Posix.Dlfcn,
  Posix.SysTypes
 {$ENDIF},
{$ENDIF}
  SyncObjs;

{$IFDEF FPC}
 {$PACKRECORDS C}
 {$IFDEF VER3_0}
type
 TLibHandle = DynLibs.TLibHandle;
 {$ENDIF}
{$ELSE}
type
  TLibHandle = HMODULE;
{$ENDIF}

const
{$IF DEFINED(VER3_0) OR (NOT DEFINED(FPC))}
  SharedSuffix =
 {$IF DEFINED(MSWINDOWS)}
    'dll'
 {$ELSEIF DEFINED(MACOS)}
    'dylib'
 {$ELSE}
    'so'
 {$ENDIF};
{$ENDIF}
{$IFDEF FPC}
 {$IFDEF VER3_0}
  NilHandle = DynLibs.NilHandle;
 {$ENDIF}
{$ELSE}
  NilHandle = HMODULE(0);
{$ENDIF}
  B4R_LIB_NAME = Concat(
{$IFDEF MSWINDOWS}
    'libbrook-4'
{$ELSE}
    'libbrook'
{$ENDIF}, '.', SharedSuffix);

resourcestring
  SB4RLibraryNotLoaded = 'Brook library ''%s'' not loaded.';

type
  EB4RLibraryNotLoaded = class(Exception);

type
  Pcchar = MarshaledAString;
  PPcchar = ^MarshaledAString;
  cbool = Boolean;
  Pcbool = PBoolean;
  cuint16 = UInt16;
  cuint64 = UInt64;
  cint = Integer;
  cuint = Cardinal;
  csize =
{$IFDEF FPC}
 {$IFDEF UNIX}
    BaseUnix
 {$ELSE}
    System
 {$ENDIF}
{$ELSE}
 {$IFDEF POSIX}
    Posix.SysTypes
 {$ELSE}
    Winapi.Windows
 {$ENDIF}
{$ENDIF}.size_t;
  cenum = cint;
  Pcvoid = Pointer;
  PPcvoid = PPointer;
  cva_list = Pointer;

const
  B4R_UUID_STR_LEN = 36;

type
  Pb4r_hs = ^b4r_hs;
  PPb4r_hs = ^Pb4r_hs;
  b4r_hs = record
  end;

  b4r_uuid_func = function(uuid: Pcchar): cbool; cdecl;

  b4r_hs_iter_cb = function(cls: Pcvoid; hs: Pb4r_hs): cbool; cdecl;

  b4r_hs_sort_cb = function(cls: Pcvoid; a: Pb4r_hs; b: Pb4r_hs): cint; cdecl;

var
  b4r_hs_name: function(hs: Pb4r_hs): Pcchar; cdecl;

  b4r_hs_val: function(hs: Pb4r_hs): Pcchar; cdecl;

  b4r_hs_name_val: function(hs: Pb4r_hs; const name: PPcchar;
    const val: PPcchar): cbool; cdecl;

  b4r_hs_add: function(hsl: PPb4r_hs; const name: Pcchar;
    const val: Pcchar): cbool; cdecl;

  b4r_hs_set: function(hsl: PPb4r_hs; const name: Pcchar;
    const val: Pcchar): cbool; cdecl;

  b4r_hs_rm: function(hsl: PPb4r_hs; const name: Pcchar): cbool; cdecl;

  b4r_hs_find: function(hsl: Pb4r_hs; const name: Pcchar): Pb4r_hs; cdecl;

  b4r_hs_get: function(hsl: Pb4r_hs; const name: Pcchar): Pcchar; cdecl;

  b4r_hs_try: function(hsl: Pb4r_hs; const name: Pcchar;
    const val: PPcchar): cbool; cdecl;

  b4r_hs_has: function(hsl: Pb4r_hs; const name: Pcchar): cbool; cdecl;

  b4r_hs_iter: function(hsl: Pb4r_hs;
    iter_cb: b4r_hs_iter_cb; iter_cls: Pcvoid): cbool; cdecl;

  b4r_hs_next: function(hs: PPb4r_hs): cbool; cdecl;

  b4r_hs_count: function(hsl: Pb4r_hs): cuint; cdecl;

  b4r_hs_sort: function(hsl: PPb4r_hs; cmp_cb: b4r_hs_sort_cb;
    cmp_cls: Pcvoid): Boolean; cdecl;

  b4r_hs_cleanup: procedure(hsl: PPb4r_hs); cdecl;

const
  B4R_HTTPSRV_PORT = 8080;

  B4R_HTTPSRV_POST_BUF_SIZE =
{$IF DEFINED(ANDROID)}
    1024 // 1 kB
{$ELSEIF DEFINED(WIN32)}
    32768 // 32 kB
{$ELSE}
    65536 // 64 kB
{$ENDIF};

  B4R_HTTPSRV_MAX_BODY_SIZE = 2097152; // 2 MB

  B4R_HTTPSRV_MAX_PAYLD_SIZE = 2097152; // 2 MB

  B4R_HTTPSRV_MAX_UPLD_SIZE = 15728640; // 15 MB

  B4R_HTTPSRV_CONTENT_TYPE  = 'text/html';

  B4R_HTTPSRV_CFG_PORT = 'b4r_httpsrv_cfg_port';

  B4R_HTTPSRV_CFG_UPLDS_DIR = 'b4r_httpsrv_cfg_upld_dir';

  B4R_HTTPSRV_CFG_MAX_BODY_SIZE = 'b4r_httpsrv_cfg_max_body_size';

  B4R_HTTPSRV_CFG_POST_BUF_SIZE = 'b4r_httpsrv_cfg_post_buf_size';

  B4R_HTTPSRV_CFG_MAX_PAYLD_SIZE = 'b4r_httpsrv_cfg_max_payld_size';

  B4R_HTTPSRV_CFG_MAX_UPLD_SIZE = 'b4r_httpsrv_cfg_max_upld_size';

  B4R_HTTPSRV_CFG_CONTENT_TYPE = 'b4r_httpsrv_cfg_content_type';

  B4R_HTTPSRV_CFG_UUID_FUNC = 'b4r_httpsrv_cfg_uuid_func';

type
  B4R_HTTPSRV_AUTH_TYPE = cenum;
const
  B4R_HTTPSRV_AUTH_TYPE_BASIC = 1;
  B4R_HTTPSRV_AUTH_TYPE_DIGEST = 2;

type
  B4R_HTTPSRV_OPT = cenum;
const
  B4R_HTTPSRV_OPT_CON_CB = 0;
  B4R_HTTPSRV_OPT_REQ_INIT_CB = 1;
  B4R_HTTPSRV_OPT_REQ_FINI_CB = 2;
  B4R_HTTPSRV_OPT_REQ_PREP_CB = 3;
  B4R_HTTPSRV_OPT_REQ_UPLD_DATA_CB = 4;
  B4R_HTTPSRV_OPT_REQ_UPLD_FILE_CBS = 5;
  B4R_HTTPSRV_OPT_REQ_ERR_CB = 6;
  B4R_HTTPSRV_OPT_REQ_ALLOWED_POST = 7;
  B4R_HTTPSRV_OPT_ERR_CB = 8;
  B4R_HTTPSRV_OPT_PORT = 9;
  B4R_HTTPSRV_OPT_SHUTDOWN_ATTEMPTS = 10;
  B4R_HTTPSRV_OPT_FORCED_SHUTDOWN = 11;
  B4R_HTTPSRV_OPT_AUTH = 12;
  B4R_HTTPSRV_OPT_ENABLED_LOG = 13;

type
  Pb4r_httpsrv_cfg = ^b4r_httpsrv_cfg;
  b4r_httpsrv_cfg = record
  end;

  Pb4r_httpsrv = ^b4r_httpsrv;
  b4r_httpsrv = record
  end;

  Pb4r_httpsrv_req = ^b4r_httpsrv_req;
  b4r_httpsrv_req = record
  end;

  Pb4r_httpsrv_res = ^b4r_httpsrv_res;
  b4r_httpsrv_res = record
  end;

  PPb4r_httpsrv_req_upld = ^Pb4r_httpsrv_req_upld;
  Pb4r_httpsrv_req_upld = ^b4r_httpsrv_req_upld;
  b4r_httpsrv_req_upld = record
  end;

  b4r_httpsrv_con_cb = procedure(cls: Pcvoid; const id: Pcchar;
    closed: cbool); cdecl;

  b4r_httpsrv_req_init_cb = procedure(cls: Pcvoid; const id: Pcchar;
    const uri: Pcchar; aborted: Pcbool); cdecl;

  b4r_httpsrv_req_fini_cb = procedure(cls: Pcvoid; finished: cbool); cdecl;

  b4r_httpsrv_req_prep_cb = procedure(cls: Pcvoid; req: Pb4r_httpsrv_req;
    res: Pb4r_httpsrv_res; done: Pcbool); cdecl;

  b4r_httpsrv_req_cb = procedure(cls: Pcvoid; req: Pb4r_httpsrv_req;
    res: Pb4r_httpsrv_res; done: Pcbool); cdecl;

  b4r_httpsrv_req_upld_data_cb = procedure(cls: Pcvoid; req: Pb4r_httpsrv_req;
    const buf: Pcchar; size: csize; const err: Pcchar); cdecl;

  b4r_httpsrv_req_upld_file_prepare_cb = function(cls: Pcvoid;
    upld: Pb4r_httpsrv_req_upld): Pcvoid; cdecl;

  b4r_httpsrv_req_upld_file_save_cb = function(cls: Pcvoid;
    upld: Pb4r_httpsrv_req_upld; overwritten: cbool): cbool; cdecl;

  b4r_httpsrv_req_upld_file_write_cb = function(cls: Pcvoid;
    upld: Pb4r_httpsrv_req_upld; const buf: Pcchar; size: csize): cbool; cdecl;

  b4r_httpsrv_req_err_cb = procedure(cls: Pcvoid; req: Pb4r_httpsrv_req;
    res: Pb4r_httpsrv_res; done: Pcbool; const err: Pcchar); cdecl;

  b4r_httpsrv_err_cb = procedure(cls: Pcvoid; const err: Pcchar); cdecl;

  b4r_httpsrv_req_uplds_iter_cb = procedure(cls: Pcvoid;
    upld: Pb4r_httpsrv_req_upld); cdecl;

var
  b4r_httpsrv_cfg_new: function(const filename: Pcchar): Pb4r_httpsrv_cfg; cdecl;

  b4r_httpsrv_cfg_free: procedure(cfg: Pb4r_httpsrv_cfg); cdecl;

  b4r_httpsrv_cfg_set_va: function(cfg: Pb4r_httpsrv_cfg; const name: Pcchar;
    va: cva_list): cbool; cdecl;

  b4r_httpsrv_cfg_set: function(cfg: Pb4r_httpsrv_cfg;
    const name: Pcchar): cbool; cdecl varargs;

  b4r_httpsrv_cfg_get: function(cfg: Pb4r_httpsrv_cfg;
    const name: Pcchar): cbool; cdecl varargs;

  b4r_httpsrv_new: function(cfg: Pb4r_httpsrv_cfg;
    req_cb: b4r_httpsrv_req_cb; req_cls: Pcvoid;
    req_err_cb: b4r_httpsrv_req_err_cb; req_err_cls: Pcvoid;
    err_cb: b4r_httpsrv_err_cb; err_cls: Pcvoid): Pb4r_httpsrv; cdecl;

  b4r_httpsrv_free: procedure(srv: Pb4r_httpsrv);  cdecl;

  b4r_httpsrv_listen: function(srv: Pb4r_httpsrv): cbool; cdecl;

  b4r_httpsrv_listening: function(srv: Pb4r_httpsrv): cbool; cdecl;

  b4r_httpsrv_shutdown: function(srv: Pb4r_httpsrv): cbool; cdecl;

  b4r_httpsrv_setopt: function(srv: Pb4r_httpsrv;
    opt: B4R_HTTPSRV_OPT): cbool; cdecl varargs;

  b4r_httpsrv_active_conns: function(srv: Pb4r_httpsrv): cuint; cdecl;

  b4r_httpsrv_req_version: function(req: Pb4r_httpsrv_req): Pcchar; cdecl;

  b4r_httpsrv_req_uri: function(req: Pb4r_httpsrv_req): Pcchar; cdecl;

  b4r_httpsrv_req_id: function(req: Pb4r_httpsrv_req): Pcchar; cdecl;

  b4r_httpsrv_req_path: function(req: Pb4r_httpsrv_req): Pcchar; cdecl;

  b4r_httpsrv_req_method: function(req: Pb4r_httpsrv_req): Pcchar; cdecl;

  b4r_httpsrv_req_content_type: function(req: Pb4r_httpsrv_req): Pcchar; cdecl;

  b4r_httpsrv_req_is_post: function(req: Pb4r_httpsrv_req): cbool; cdecl;

  b4r_httpsrv_req_headers: function(req: Pb4r_httpsrv_req): PPb4r_hs; cdecl;

  b4r_httpsrv_req_params: function(req: Pb4r_httpsrv_req): PPb4r_hs; cdecl;

  b4r_httpsrv_req_fields: function(req: Pb4r_httpsrv_req): PPb4r_hs; cdecl;

  b4r_httpsrv_req_payld: function(req: Pb4r_httpsrv_req): Pcchar; cdecl;

  b4r_httpsrv_req_payld_size: function(req: Pb4r_httpsrv_req): csize; cdecl;

  b4r_httpsrv_req_iter_uplds: function(req: Pb4r_httpsrv_req;
     iter_cb: b4r_httpsrv_req_uplds_iter_cb; iter_cls: Pcvoid): cbool; cdecl;

  b4r_httpsrv_req_uplds_first: function(req: Pb4r_httpsrv_req;
     upld: PPb4r_httpsrv_req_upld): cbool; cdecl;

  b4r_httpsrv_req_uplds_next: function(
     upld: PPb4r_httpsrv_req_upld): cbool; cdecl;

  b4r_httpsrv_req_uplds_count: function(req: Pb4r_httpsrv_req): cint; cdecl;

  b4r_httpsrv_req_upld_save: function(req: Pb4r_httpsrv_req;
     upld: Pb4r_httpsrv_req_upld; overwritten: cbool): cbool; cdecl;

  b4r_httpsrv_req_upld_save_as: function(req: Pb4r_httpsrv_req;
     upld: Pb4r_httpsrv_req_upld; const name: Pcchar;
     overwritten: cbool): cbool; cdecl;

  b4r_httpsrv_req_upld_stream: function(
     upld: Pb4r_httpsrv_req_upld): Pcvoid; cdecl;

  b4r_httpsrv_req_upld_size: function(
     upld: Pb4r_httpsrv_req_upld): cuint64; cdecl;

  b4r_httpsrv_req_upld_name: function(upld: Pb4r_httpsrv_req_upld): Pcchar; cdecl;

  b4r_httpsrv_req_upld_orig_name: function(
     upld: Pb4r_httpsrv_req_upld): Pcchar; cdecl;

  b4r_httpsrv_req_upld_dir: function(upld: Pb4r_httpsrv_req_upld): Pcchar; cdecl;

  b4r_httpsrv_req_upld_dest_name: function(
     upld: Pb4r_httpsrv_req_upld): Pcchar; cdecl;

  b4r_httpsrv_req_upld_field: function(
     upld: Pb4r_httpsrv_req_upld): Pcchar; cdecl;

  b4r_httpsrv_req_upld_mime: function(upld: Pb4r_httpsrv_req_upld): Pcchar; cdecl;

  b4r_httpsrv_req_upld_encoding: function(
     upld: Pb4r_httpsrv_req_upld): Pcchar; cdecl;

  b4r_httpsrv_req_upld_err: function(upld: Pb4r_httpsrv_req_upld): Pcchar; cdecl;

  b4r_httpsrv_req_upld_failf_va: function(upld: Pb4r_httpsrv_req_upld;
     const fmt: Pcchar; va: cva_list): cint; cdecl;

  b4r_httpsrv_req_upld_failf: function(upld: Pb4r_httpsrv_req_upld;
     const fmt: Pcchar): cint; cdecl varargs;

  b4r_httpsrv_res_headers: function(res: Pb4r_httpsrv_res): PPb4r_hs; cdecl;

  b4r_httpsrv_res_status: function(res: Pb4r_httpsrv_res;
    code: cuint16): cbool; cdecl;

  b4r_httpsrv_res_content_type: function(res: Pb4r_httpsrv_res;
    const content_type: Pcchar): cbool; cdecl;

  b4r_httpsrv_res_write_raw: function(res: Pb4r_httpsrv_res; const data: Pcvoid;
    size: csize): cbool; cdecl varargs;

  b4r_httpsrv_res_write_va: function(res: Pb4r_httpsrv_res; const fmt: Pcchar;
    va: cva_list): cbool; cdecl;

  b4r_httpsrv_res_write: function(res: Pb4r_httpsrv_res;
    const fmt: Pcchar): cbool; cdecl varargs;

  b4r_httpsrv_res_send_va: function(res: Pb4r_httpsrv_res; const fmt: Pcchar;
    va: cva_list): cbool; cdecl;

  b4r_httpsrv_res_send: function(res: Pb4r_httpsrv_res;
    const fmt: Pcchar): cbool; cdecl varargs;

  b4r_httpsrv_res_send_file: function(res: Pb4r_httpsrv_res;
    filename: Pcchar): cbool; cdecl;

  b4r_httpsrv_res_json: function(res: Pb4r_httpsrv_res;
    const json: Pcchar): cbool; cdecl;

  b4r_alloc: function(size: csize): Pcvoid; cdecl;

  b4r_free: procedure(ptr: Pcvoid); cdecl;

  b4r_is_empty: function(const str: Pcchar): cbool; cdecl;

  b4r_asprintf_va: function(const fmt: Pcchar; va: cva_list): Pcchar; cdecl;

  b4r_asprintf: function(const fmt: Pcchar): Pcchar; cdecl varargs;

  b4r_tmp_dir: function: pcchar; cdecl;

  b4r_uuid: function(uuid: Pcchar): cbool; cdecl;

function C2B(const S: Pcchar; const Z: csize): TBytes; inline;

function C2S(const S: pcchar): string; inline;

function S2C(const S: string): pcchar; inline;

function B4RLoadLibrary(const AFileName: TFileName): TLibHandle;

function B4RUnloadLibrary: TLibHandle;

procedure B4RCheckLibrary;

implementation

var
  GLock: TCriticalSection = nil;
  GLibHandle: TLibHandle = NilHandle;
  GLastLibName: TFileName = B4R_LIB_NAME;

function C2B(const S: Pcchar; const Z: csize): TBytes;
begin
  if (not Assigned(S)) or (Z = 0) then
    Exit(nil);
  SetLength(Result, Z);
  Move(S^, Result[0], Z);
end;

function C2S(const S: pcchar): string;
var
  B: pcchar;
  R: RawByteString;
begin
  if not Assigned(S) then
    Exit('');
  B := pcchar(@S[0]);
  SetString(R, B, Length(B));
  SetCodePage(R, CP_UTF8, False);
  Result := string(R);
end;

function S2C(const S: string): pcchar;
{$IFNDEF FPC}
var
  M: TMarshaller;
{$ENDIF}
begin
  Result := pcchar({$IFDEF FPC}S{$ELSE}M.AsUtf8(S){$ENDIF});
end;

function B4RLoadLibrary(const AFileName: TFileName): TLibHandle;
begin
  GLock.Acquire;
  try
    if Trim(AFileName) = '' then
      Exit(NilHandle);
    if GLibHandle <> NilHandle then
      Exit(GLibHandle);
    //TODO: check library version.
    GLastLibName := ExtractFileName(AFileName);
    GLibHandle := SafeLoadLibrary(AFileName);
    if GLibHandle = NilHandle then
      Exit(NilHandle);
    b4r_hs_name := GetProcAddress(GLibHandle, 'b4r_hs_name');
    b4r_hs_val := GetProcAddress(GLibHandle, 'b4r_hs_val');
    b4r_hs_name_val := GetProcAddress(GLibHandle, 'b4r_hs_name_val');
    b4r_hs_add := GetProcAddress(GLibHandle, 'b4r_hs_add');
    b4r_hs_set := GetProcAddress(GLibHandle, 'b4r_hs_set');
    b4r_hs_rm := GetProcAddress(GLibHandle, 'b4r_hs_rm');
    b4r_hs_find := GetProcAddress(GLibHandle, 'b4r_hs_find');
    b4r_hs_get := GetProcAddress(GLibHandle, 'b4r_hs_get');
    b4r_hs_try := GetProcAddress(GLibHandle, 'b4r_hs_try');
    b4r_hs_has := GetProcAddress(GLibHandle, 'b4r_hs_has');
    b4r_hs_iter := GetProcAddress(GLibHandle, 'b4r_hs_iter');
    b4r_hs_next := GetProcAddress(GLibHandle, 'b4r_hs_next');
    b4r_hs_count := GetProcAddress(GLibHandle, 'b4r_hs_count');
    b4r_hs_sort := GetProcAddress(GLibHandle, 'b4r_hs_sort');
    b4r_hs_cleanup := GetProcAddress(GLibHandle, 'b4r_hs_cleanup');
    b4r_httpsrv_cfg_new := GetProcAddress(GLibHandle, 'b4r_httpsrv_cfg_new');
    b4r_httpsrv_cfg_free := GetProcAddress(GLibHandle, 'b4r_httpsrv_cfg_free');
    b4r_httpsrv_cfg_set_va := GetProcAddress(GLibHandle, 'b4r_httpsrv_cfg_set_va');
    b4r_httpsrv_cfg_set := GetProcAddress(GLibHandle, 'b4r_httpsrv_cfg_set');
    b4r_httpsrv_cfg_get := GetProcAddress(GLibHandle, 'b4r_httpsrv_cfg_get');
    b4r_httpsrv_new := GetProcAddress(GLibHandle, 'b4r_httpsrv_new');
    b4r_httpsrv_free := GetProcAddress(GLibHandle, 'b4r_httpsrv_free');
    b4r_httpsrv_listen := GetProcAddress(GLibHandle, 'b4r_httpsrv_listen');
    b4r_httpsrv_listening := GetProcAddress(GLibHandle, 'b4r_httpsrv_listening');
    b4r_httpsrv_shutdown := GetProcAddress(GLibHandle, 'b4r_httpsrv_shutdown');
    b4r_httpsrv_setopt := GetProcAddress(GLibHandle, 'b4r_httpsrv_setopt');
    b4r_httpsrv_active_conns := GetProcAddress(GLibHandle, 'b4r_httpsrv_active_conns');
    b4r_httpsrv_req_version := GetProcAddress(GLibHandle, 'b4r_httpsrv_req_version');
    b4r_httpsrv_req_uri := GetProcAddress(GLibHandle, 'b4r_httpsrv_req_uri');
    b4r_httpsrv_req_id := GetProcAddress(GLibHandle, 'b4r_httpsrv_req_id');
    b4r_httpsrv_req_path := GetProcAddress(GLibHandle, 'b4r_httpsrv_req_path');
    b4r_httpsrv_req_method := GetProcAddress(GLibHandle, 'b4r_httpsrv_req_method');
    b4r_httpsrv_req_content_type := GetProcAddress(GLibHandle, 'b4r_httpsrv_req_content_type');
    b4r_httpsrv_req_is_post := GetProcAddress(GLibHandle, 'b4r_httpsrv_req_is_post');
    b4r_httpsrv_req_headers := GetProcAddress(GLibHandle, 'b4r_httpsrv_req_headers');
    b4r_httpsrv_req_params := GetProcAddress(GLibHandle, 'b4r_httpsrv_req_params');
    b4r_httpsrv_req_fields := GetProcAddress(GLibHandle, 'b4r_httpsrv_req_fields');
    b4r_httpsrv_req_payld := GetProcAddress(GLibHandle, 'b4r_httpsrv_req_payld');
    b4r_httpsrv_req_payld_size := GetProcAddress(GLibHandle, 'b4r_httpsrv_req_payld_size');
    b4r_httpsrv_req_iter_uplds := GetProcAddress(GLibHandle, 'b4r_httpsrv_req_iter_uplds');
    b4r_httpsrv_req_uplds_first := GetProcAddress(GLibHandle, 'b4r_httpsrv_req_uplds_first');
    b4r_httpsrv_req_uplds_next := GetProcAddress(GLibHandle, 'b4r_httpsrv_req_uplds_next');
    b4r_httpsrv_req_uplds_count := GetProcAddress(GLibHandle, 'b4r_httpsrv_req_uplds_count');
    b4r_httpsrv_req_upld_save := GetProcAddress(GLibHandle, 'b4r_httpsrv_req_upld_save');
    b4r_httpsrv_req_upld_save_as := GetProcAddress(GLibHandle, 'b4r_httpsrv_req_upld_save_as');
    b4r_httpsrv_req_upld_stream := GetProcAddress(GLibHandle, 'b4r_httpsrv_req_upld_stream');
    b4r_httpsrv_req_upld_size := GetProcAddress(GLibHandle, 'b4r_httpsrv_req_upld_size');
    b4r_httpsrv_req_upld_name := GetProcAddress(GLibHandle, 'b4r_httpsrv_req_upld_name');
    b4r_httpsrv_req_upld_orig_name := GetProcAddress(GLibHandle, 'b4r_httpsrv_req_upld_orig_name');
    b4r_httpsrv_req_upld_dir := GetProcAddress(GLibHandle, 'b4r_httpsrv_req_upld_dir');
    b4r_httpsrv_req_upld_dest_name := GetProcAddress(GLibHandle, 'b4r_httpsrv_req_upld_dest_name');
    b4r_httpsrv_req_upld_field := GetProcAddress(GLibHandle, 'b4r_httpsrv_req_upld_field');
    b4r_httpsrv_req_upld_mime := GetProcAddress(GLibHandle, 'b4r_httpsrv_req_upld_mime');
    b4r_httpsrv_req_upld_encoding := GetProcAddress(GLibHandle, 'b4r_httpsrv_req_upld_encoding');
    b4r_httpsrv_req_upld_err := GetProcAddress(GLibHandle, 'b4r_httpsrv_req_upld_err');
    b4r_httpsrv_req_upld_failf_va := GetProcAddress(GLibHandle, 'b4r_httpsrv_req_upld_failf_va');
    b4r_httpsrv_req_upld_failf := GetProcAddress(GLibHandle, 'b4r_httpsrv_req_upld_failf');
    b4r_httpsrv_res_headers := GetProcAddress(GLibHandle, 'b4r_httpsrv_res_headers');
    b4r_httpsrv_res_status := GetProcAddress(GLibHandle, 'b4r_httpsrv_res_status');
    b4r_httpsrv_res_content_type := GetProcAddress(GLibHandle, 'b4r_httpsrv_res_content_type');
    b4r_httpsrv_res_write_raw := GetProcAddress(GLibHandle, 'b4r_httpsrv_res_write_raw');
    b4r_httpsrv_res_write_va := GetProcAddress(GLibHandle, 'b4r_httpsrv_res_write_va');
    b4r_httpsrv_res_write := GetProcAddress(GLibHandle, 'b4r_httpsrv_res_write');
    b4r_httpsrv_res_send_va := GetProcAddress(GLibHandle, 'b4r_httpsrv_res_send_va');
    b4r_httpsrv_res_send := GetProcAddress(GLibHandle, 'b4r_httpsrv_res_send');
    b4r_httpsrv_res_send_file := GetProcAddress(GLibHandle, 'b4r_httpsrv_res_send_file');
    b4r_httpsrv_res_json := GetProcAddress(GLibHandle, 'b4r_httpsrv_res_json');
    b4r_alloc := GetProcAddress(GLibHandle, 'b4r_alloc');
    b4r_free := GetProcAddress(GLibHandle, 'b4r_free');
    b4r_is_empty := GetProcAddress(GLibHandle, 'b4r_is_empty');
    b4r_asprintf_va := GetProcAddress(GLibHandle, 'b4r_asprintf_va');
    b4r_asprintf := GetProcAddress(GLibHandle, 'b4r_asprintf');
    b4r_tmp_dir := GetProcAddress(GLibHandle, 'b4r_tmp_dir');
    b4r_uuid := GetProcAddress(GLibHandle, 'b4r_uuid');
    Result := GLibHandle;
  finally
    GLock.Release;
  end;
end;

function B4RUnloadLibrary: TLibHandle;
begin
  GLock.Acquire;
  try
    if GLibHandle = NilHandle then
      Exit(NilHandle);
    if not FreeLibrary(GLibHandle) then
      Exit(GLibHandle);
    GLibHandle := NilHandle;
    b4r_hs_name := nil;
    b4r_hs_val := nil;
    b4r_hs_name_val := nil;
    b4r_hs_add := nil;
    b4r_hs_set := nil;
    b4r_hs_rm := nil;
    b4r_hs_find := nil;
    b4r_hs_get := nil;
    b4r_hs_try := nil;
    b4r_hs_has := nil;
    b4r_hs_iter := nil;
    b4r_hs_next := nil;
    b4r_hs_count := nil;
    b4r_hs_sort := nil;
    b4r_hs_cleanup := nil;
    b4r_httpsrv_cfg_new := nil;
    b4r_httpsrv_cfg_free := nil;
    b4r_httpsrv_cfg_set_va := nil;
    b4r_httpsrv_cfg_set := nil;
    b4r_httpsrv_cfg_get := nil;
    b4r_httpsrv_new := nil;
    b4r_httpsrv_free := nil;
    b4r_httpsrv_listen := nil;
    b4r_httpsrv_listening := nil;
    b4r_httpsrv_shutdown := nil;
    b4r_httpsrv_setopt := nil;
    b4r_httpsrv_active_conns := nil;
    b4r_httpsrv_req_version := nil;
    b4r_httpsrv_req_uri := nil;
    b4r_httpsrv_req_id := nil;
    b4r_httpsrv_req_path := nil;
    b4r_httpsrv_req_method := nil;
    b4r_httpsrv_req_content_type := nil;
    b4r_httpsrv_req_is_post := nil;
    b4r_httpsrv_req_headers := nil;
    b4r_httpsrv_req_params := nil;
    b4r_httpsrv_req_fields := nil;
    b4r_httpsrv_req_payld := nil;
    b4r_httpsrv_req_payld_size := nil;
    b4r_httpsrv_req_iter_uplds := nil;
    b4r_httpsrv_req_uplds_first := nil;
    b4r_httpsrv_req_uplds_next := nil;
    b4r_httpsrv_req_uplds_count := nil;
    b4r_httpsrv_req_upld_save := nil;
    b4r_httpsrv_req_upld_save_as := nil;
    b4r_httpsrv_req_upld_stream := nil;
    b4r_httpsrv_req_upld_size := nil;
    b4r_httpsrv_req_upld_name := nil;
    b4r_httpsrv_req_upld_orig_name := nil;
    b4r_httpsrv_req_upld_dir := nil;
    b4r_httpsrv_req_upld_dest_name := nil;
    b4r_httpsrv_req_upld_field := nil;
    b4r_httpsrv_req_upld_mime := nil;
    b4r_httpsrv_req_upld_encoding := nil;
    b4r_httpsrv_req_upld_err := nil;
    b4r_httpsrv_req_upld_failf_va := nil;
    b4r_httpsrv_req_upld_failf := nil;
    b4r_httpsrv_res_write_va := nil;
    b4r_httpsrv_res_headers := nil;
    b4r_httpsrv_res_status := nil;
    b4r_httpsrv_res_content_type := nil;
    b4r_httpsrv_res_write_raw := nil;
    b4r_httpsrv_res_write := nil;
    b4r_httpsrv_res_send_va := nil;
    b4r_httpsrv_res_send := nil;
    b4r_httpsrv_res_send_file := nil;
    b4r_httpsrv_res_json := nil;
    b4r_alloc := nil;
    b4r_free := nil;
    b4r_is_empty := nil;
    b4r_asprintf_va := nil;
    b4r_asprintf := nil;
    b4r_tmp_dir := nil;
    b4r_uuid := nil;
    Result := GLibHandle;
  finally
    GLock.Release;
  end;
end;

procedure B4RCheckLibrary;
begin
  if GLibHandle = NilHandle then
    raise EB4RLibraryNotLoaded.CreateResFmt(@SB4RLibraryNotLoaded, [GLastLibName]);
end;

initialization
  GLock := TCriticalSection.Create;
  B4RLoadLibrary(B4R_LIB_NAME);

finalization
  B4RUnloadLibrary;
  FreeAndNil(GLock);

end.
