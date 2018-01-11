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

unit BrookHTTPServerRequest;

{$I Brook.inc}

interface

uses
  SysUtils,
  Classes,
{$IFNDEF FPC}
 {$IFDEF MSWINDOWS}
  Windows,
 {$ENDIF}
{$ENDIF}
  libbrook,
  BrookHashStrings,
  BrookHandledClasses,
  BrookHTTPServerRequestUploads;

type

  { TBrookHTTPServerRequest }

  TBrookHTTPServerRequest = class(TBrookHandledPersistent)
  private
    FContentType: string;
    FHeaders: TBrookHashStrings;
    FFields: TBrookHashStrings;
    FID: string;
    FMethod: string;
    FPath: string;
    Freq: Pb4r_httpsrv_req;
    FParams: TBrookHashStrings;
    FPaths: TArray<string>;
    FApp: TObject;
    FPayload: TBytes;
    FPayloadSize: NativeUInt;
    FIsPost: Boolean;
    FUploads: TBrookHTTPServerRequestUploads;
    FURI: string;
    FVersion: string;
  protected
    function CreateHeaders(AHandle: PPb4r_hs): TBrookHashStrings; virtual;
    procedure FreeHeaders(AHeaders: TBrookHashStrings); virtual;
    function CreateUploads(
      AHandle: Pb4r_httpsrv_req): TBrookHTTPServerRequestUploads; virtual;
    procedure FreeUploads(AUploads: TBrookHTTPServerRequestUploads); virtual;
    function CreateFields(AHandle: PPb4r_hs): TBrookHashStrings; virtual;
    procedure FreeFields(AFields: TBrookHashStrings); virtual;
    function CreateParams(AHandle: PPb4r_hs): TBrookHashStrings; virtual;
    procedure FreeParams(AParams: TBrookHashStrings); virtual;
    function GetHandle: Pointer; override;
  public
    constructor Create(AApp: TObject; AHandle: Pb4r_httpsrv_req); virtual;
    destructor Destroy; override;
    procedure Assign(ASource: TPersistent); override;
    property App: TObject read FApp;
    property Version: string read FVersion;
    property Method: string read FMethod;
    property ContentType: string read FContentType;
    property URI: string read FURI;
    property ID: string read FID;
    property Path: string read FPath;
    property Payload: TBytes read FPayload;
    property PayloadSize: NativeUInt read FPayloadSize;
    property Headers: TBrookHashStrings read FHeaders;
    property Fields: TBrookHashStrings read FFields;
    property Params: TBrookHashStrings read FParams;
    property Uploads: TBrookHTTPServerRequestUploads read FUploads
      write FUploads;
    property Paths: TArray<string> read FPaths;
    property IsPost: Boolean read FIsPost;
  end;

implementation

{ TBrookHTTPServerRequest }

constructor TBrookHTTPServerRequest.Create(AApp: TObject;
  AHandle: Pb4r_httpsrv_req);
begin
  inherited Create;
  B4RCheckLibrary;
  FHeaders := CreateHeaders(b4r_httpsrv_req_headers(AHandle));
  FFields := CreateFields(b4r_httpsrv_req_fields(AHandle));
  FParams := CreateParams(b4r_httpsrv_req_params(AHandle));
  FUploads := CreateUploads(AHandle);
  Freq := AHandle;
  FApp := AApp;
  FVersion := C2S(b4r_httpsrv_req_version(Freq));
  FMethod := C2S(b4r_httpsrv_req_method(Freq));
  FContentType := C2S(b4r_httpsrv_req_content_type(Freq));
  FURI := C2S(b4r_httpsrv_req_uri(Freq));
  FID := C2S(b4r_httpsrv_req_id(Freq));
  FPath := C2S(b4r_httpsrv_req_path(Freq));
  FPayloadSize := b4r_httpsrv_req_payld_size(AHandle);
  FPayload := TMarshal.ToBytes(b4r_httpsrv_req_payld(AHandle), FPayloadSize);
  FIsPost := b4r_httpsrv_req_is_post(Freq);
  FPaths := C2S(b4r_httpsrv_req_path(AHandle)).Split(['/'],
    TStringSplitOptions.ExcludeEmpty);
end;

destructor TBrookHTTPServerRequest.Destroy;
begin
  FreeUploads(FUploads);
  FreeParams(FParams);
  FreeFields(FFields);
  FreeHeaders(FHeaders);
  inherited Destroy;
end;

procedure TBrookHTTPServerRequest.Assign(ASource: TPersistent);
var
  VReq: TBrookHTTPServerRequest;
begin
  if ASource is TBrookHTTPServerRequest then
  begin
    VReq := ASource as TBrookHTTPServerRequest;
    FVersion := VReq.FVersion;
    FMethod := VReq.FMethod;
    FContentType := VReq.FContentType;
    FURI := VReq.FURI;
    FID := VReq.FID;
    FPath := VReq.FPath;
    FIsPost := VReq.FIsPost;
    FPayload := VReq.FPayload;
    FPaths := VReq.FPaths;
  end
  else
    inherited Assign(ASource);
end;

function TBrookHTTPServerRequest.CreateHeaders(
  AHandle: PPb4r_hs): TBrookHashStrings;
begin
  Result := TBrookHashStrings.Create(AHandle);
end;

procedure TBrookHTTPServerRequest.FreeHeaders(AHeaders: TBrookHashStrings);
begin
  AHeaders.Free;
end;

function TBrookHTTPServerRequest.CreateUploads(
  AHandle: Pb4r_httpsrv_req): TBrookHTTPServerRequestUploads;
begin
  Result := TBrookHTTPServerRequestUploads.Create(AHandle);
end;

procedure TBrookHTTPServerRequest.FreeUploads(
  AUploads: TBrookHTTPServerRequestUploads);
begin
  AUploads.Free;
end;

function TBrookHTTPServerRequest.CreateFields(
  AHandle: PPb4r_hs): TBrookHashStrings;
begin
  Result := TBrookHashStrings.Create(AHandle);
end;

procedure TBrookHTTPServerRequest.FreeFields(AFields: TBrookHashStrings);
begin
  AFields.Free;
end;

function TBrookHTTPServerRequest.CreateParams(
  AHandle: PPb4r_hs): TBrookHashStrings;
begin
  Result := TBrookHashStrings.Create(AHandle);
end;

procedure TBrookHTTPServerRequest.FreeParams(AParams: TBrookHashStrings);
begin
  AParams.Free;
end;

function TBrookHTTPServerRequest.GetHandle: Pointer;
begin
  Result := Freq;
end;

end.
