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

unit BrookHTTPServerConfiguration;

{$I Brook.inc}

interface

uses
  RtlConsts,
  SysUtils,
  Classes,
{$IFNDEF FPC}
  IOUtils,
 {$IFDEF MSWINDOWS}
  Windows,
 {$ENDIF}
{$ENDIF}
  libbrook,
  BrookHandledClasses;

resourcestring
  SBrookPreparedConfiguration =
    'Operation cannot be performed on a prepared configuration.';

type

  { TBrookHTTPServerConfiguration }

  TBrookHTTPServerConfiguration = class(TBrookHandledComponent)
  private
    Fcfg: Pb4r_httpsrv_cfg;
    FContentType: string;
    FFileName: TFileName;
    FMaxBodySize: LongWord;
    FMaxPayloadSize: LongWord;
    FMaxUploadSize: UInt64;
    FPort: UInt16;
    FPostBufferSize: NativeUInt;
    FUploadsDir: string;
    FUUIDFunc: b4r_uuid_func;
    FPrepared: Boolean;
    function IsContentTypeStored: Boolean;
    function IsMaxBodySizeStored: Boolean;
    function IsMaxPayloadSizeStored: Boolean;
    function IsMaxUploadSizeStored: Boolean;
    function IsPortStored: Boolean;
    function IsPostBufferSizeStored: Boolean;
    function IsUploadsDirStored: Boolean;
    procedure SetContentType(const AValue: string);
    procedure SetFileName(const AValue: TFileName);
    procedure SetMaxBodySize(AValue: LongWord);
    procedure SetMaxPayloadSize(AValue: LongWord);
    procedure SetMaxUploadSize(AValue: UInt64);
    procedure SetPort(AValue: UInt16);
    procedure SetPostBufferSize(AValue: NativeUInt);
    procedure SetUploadsDir(const AValue: string);
    procedure SetUUIDFunc(AValue: b4r_uuid_func);
  protected
    class function GetTempDir: string; static; inline;
    function GetHandle: Pointer; override;
    procedure CheckPrepared; inline;
  public
    constructor Create(AOwner: TComponent); override;
    destructor Destroy; override;
    procedure Prepare; virtual;
    procedure Unprepare; virtual;
    property Prepared: Boolean read FPrepared;
    property UUIDFunc: b4r_uuid_func read FUUIDFunc write SetUUIDFunc;
  published
    property FileName: TFileName read FFileName write SetFileName;
    property PostBufferSize: NativeUInt read FPostBufferSize
      write SetPostBufferSize stored IsPostBufferSizeStored;
    property MaxBodySize: LongWord read FMaxBodySize write SetMaxBodySize
      stored IsMaxBodySizeStored;
    property MaxPayloadSize: LongWord read FMaxPayloadSize
      write SetMaxPayloadSize stored IsMaxPayloadSizeStored;
    property MaxUploadSize: UInt64 read FMaxUploadSize write SetMaxUploadSize
      stored IsMaxUploadSizeStored;
    property ContentType: string read FContentType write SetContentType
      stored IsContentTypeStored;
    property UploadsDir: string read FUploadsDir write SetUploadsDir
      stored IsUploadsDirStored;
    property Port: UInt16 read FPort write SetPort stored IsPortStored
      default B4R_HTTPSRV_PORT;
  end;

implementation

{ TBrookHTTPServerConfiguration }

constructor TBrookHTTPServerConfiguration.Create(AOwner: TComponent);
begin
  inherited Create(AOwner);
  FPort := B4R_HTTPSRV_PORT;
  FMaxBodySize := B4R_HTTPSRV_MAX_BODY_SIZE;
  FPostBufferSize := B4R_HTTPSRV_POST_BUF_SIZE;
  FMaxPayloadSize := B4R_HTTPSRV_MAX_PAYLD_SIZE;
  FMaxUploadSize := B4R_HTTPSRV_MAX_UPLD_SIZE;
  FContentType := B4R_HTTPSRV_CONTENT_TYPE;
  FUploadsDir := TBrookHTTPServerConfiguration.GetTempDir;
  FUUIDFunc := b4r_uuid;
end;

destructor TBrookHTTPServerConfiguration.Destroy;
begin
  Unprepare;
  inherited Destroy;
end;

class function TBrookHTTPServerConfiguration.GetTempDir: string;
begin
  Result := IncludeTrailingPathDelimiter(
    {$IFDEF FPC}SysUtils.GetTempDir(True){$ELSE}TPath.GetTempPath{$ENDIF});
end;

function TBrookHTTPServerConfiguration.GetHandle: Pointer;
begin
  Result := Fcfg;
end;

procedure TBrookHTTPServerConfiguration.Prepare;
var
  M: TMarshaller;
begin
  if FPrepared then
    Exit;
  B4RCheckLibrary;
  if (FFileName <> '') and (not FileExists(FFileName)) then
    raise EFileNotFoundException.CreateResFmt(@SFOpenError, [FFileName]);
  Fcfg := b4r_httpsrv_cfg_new(M.ToCString(FFileName));
  FPrepared := Assigned(Fcfg);
  if FPrepared then
  begin
    b4r_httpsrv_cfg_set(Fcfg, M.ToCString(B4R_HTTPSRV_CFG_PORT), FPort);
    b4r_httpsrv_cfg_set(Fcfg, M.ToCString(B4R_HTTPSRV_CFG_MAX_BODY_SIZE),
      FMaxBodySize);
    b4r_httpsrv_cfg_set(Fcfg, M.ToCString(B4R_HTTPSRV_CFG_POST_BUF_SIZE),
      FPostBufferSize);
    b4r_httpsrv_cfg_set(Fcfg, M.ToCString(B4R_HTTPSRV_CFG_MAX_PAYLD_SIZE),
      FMaxPayloadSize);
    b4r_httpsrv_cfg_set(Fcfg, M.ToCString(B4R_HTTPSRV_CFG_MAX_UPLD_SIZE),
      FMaxUploadSize);
    b4r_httpsrv_cfg_set(Fcfg, M.ToCString(B4R_HTTPSRV_CFG_CONTENT_TYPE),
      M.ToCString(FContentType));
    b4r_httpsrv_cfg_set(Fcfg, M.ToCString(B4R_HTTPSRV_CFG_UPLDS_DIR),
      M.ToCString(FUploadsDir));
    b4r_httpsrv_cfg_set(Fcfg, M.ToCString(B4R_HTTPSRV_CFG_UUID_FUNC),
      @FUUIDFunc);
  end;
end;

procedure TBrookHTTPServerConfiguration.Unprepare;
begin
  if not FPrepared then
    Exit;
  B4RCheckLibrary;
  b4r_httpsrv_cfg_free(Fcfg);
  FPrepared := False;
end;

procedure TBrookHTTPServerConfiguration.CheckPrepared;
begin
  if FPrepared then
    raise EInvalidOpException.CreateRes(@SBrookPreparedConfiguration);
end;

procedure TBrookHTTPServerConfiguration.SetUUIDFunc(AValue: b4r_uuid_func);
begin
  if @AValue = @FUUIDFunc then
    Exit;
  CheckPrepared;
  FUUIDFunc := AValue;
end;

procedure TBrookHTTPServerConfiguration.SetFileName(const AValue: TFileName);
begin
  if AValue = FFileName then
    Exit;
  CheckPrepared;
  FFileName := AValue;
end;

procedure TBrookHTTPServerConfiguration.SetContentType(const AValue: string);
begin
  if AValue = FContentType then
    Exit;
  CheckPrepared;
  FContentType := AValue;
end;

function TBrookHTTPServerConfiguration.IsPostBufferSizeStored: Boolean;
begin
  Result := FPostBufferSize <> B4R_HTTPSRV_POST_BUF_SIZE;
end;

function TBrookHTTPServerConfiguration.IsUploadsDirStored: Boolean;
begin
  Result := not SameText(FUploadsDir, TBrookHTTPServerConfiguration.GetTempDir);
end;

function TBrookHTTPServerConfiguration.IsMaxBodySizeStored: Boolean;
begin
  Result := FMaxBodySize <> B4R_HTTPSRV_MAX_BODY_SIZE;
end;

function TBrookHTTPServerConfiguration.IsContentTypeStored: Boolean;
begin
  Result := not SameText(FContentType, B4R_HTTPSRV_CONTENT_TYPE);
end;

function TBrookHTTPServerConfiguration.IsMaxPayloadSizeStored: Boolean;
begin
  Result := FMaxPayloadSize <> B4R_HTTPSRV_MAX_PAYLD_SIZE;
end;

function TBrookHTTPServerConfiguration.IsMaxUploadSizeStored: Boolean;
begin
  Result := FMaxUploadSize <> B4R_HTTPSRV_MAX_UPLD_SIZE;
end;

function TBrookHTTPServerConfiguration.IsPortStored: Boolean;
begin
  Result := FPort <> B4R_HTTPSRV_PORT; { TODO: prioritize the config port. }
end;

procedure TBrookHTTPServerConfiguration.SetMaxBodySize(AValue: LongWord);
begin
  if AValue = FMaxBodySize then
    Exit;
  CheckPrepared;
  FMaxBodySize := AValue;
end;

procedure TBrookHTTPServerConfiguration.SetMaxPayloadSize(AValue: LongWord);
begin
  if AValue = FMaxPayloadSize then
    Exit;
  CheckPrepared;
  FMaxPayloadSize := AValue;
end;

procedure TBrookHTTPServerConfiguration.SetMaxUploadSize(AValue: UInt64);
begin
  if AValue = FMaxUploadSize then
    Exit;
  CheckPrepared;
  FMaxUploadSize := AValue;
end;

procedure TBrookHTTPServerConfiguration.SetPort(AValue: UInt16);
begin
  if AValue = FPort then
    Exit;
  CheckPrepared;
  FPort := AValue;
end;

procedure TBrookHTTPServerConfiguration.SetPostBufferSize(AValue: NativeUInt);
begin
  if AValue = FPostBufferSize then
    Exit;
  CheckPrepared;
  FPostBufferSize := AValue;
end;

procedure TBrookHTTPServerConfiguration.SetUploadsDir(const AValue: string);
begin
  if AValue = FUploadsDir then
    Exit;
  CheckPrepared;
  FUploadsDir := IncludeTrailingPathDelimiter(AValue);
end;

end.
