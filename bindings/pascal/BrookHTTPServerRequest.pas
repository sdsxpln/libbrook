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

unit BrookHTTPServerRequest;

{$I Brook.inc}

interface

uses
  SysUtils,
  Classes,
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
    FUp: Boolean;
    FUploads: TBrookHTTPServerRequestUploads;
    FURI: string;
    FVersion: string;
  protected
    function CreateHeaders(AHandle: Pointer): TBrookHashStrings; virtual;
    procedure FreeHeaders(AHeaders: TBrookHashStrings); virtual;
    function CreateUploads(
      AHandle: Pointer): TBrookHTTPServerRequestUploads; virtual;
    procedure FreeUploads(AUploads: TBrookHTTPServerRequestUploads); virtual;
    function CreateFields(AHandle: Pointer): TBrookHashStrings; virtual;
    procedure FreeFields(AFields: TBrookHashStrings); virtual;
    function CreateParams(AHandle: Pointer): TBrookHashStrings; virtual;
    procedure FreeParams(AParams: TBrookHashStrings); virtual;
    function GetHandle: Pointer; override;
  public
    constructor Create(AApp: TObject; AHandle: Pointer); virtual;
    destructor Destroy; override;
    procedure Assign(ASource: TPersistent); override;
    function TryParam(const AName: string; out AValue: string): Boolean;
    property App: TObject read FApp;
    property Version: string read FVersion;
    property Method: string read FMethod;
    property ContentType: string read FContentType;
    property URI: string read FURI;
    property ID: string read FID;
    property Path: string read FPath;
    property Payload: TBytes read FPayload;
    property Headers: TBrookHashStrings read FHeaders;
    property Fields: TBrookHashStrings read FFields;
    property Params: TBrookHashStrings read FParams;
    property Uploads: TBrookHTTPServerRequestUploads read FUploads
      write FUploads;
    property Paths: TArray<string> read FPaths;
    property Up: Boolean read FUp;
  end;

implementation

{ TBrookHTTPServerRequest }

constructor TBrookHTTPServerRequest.Create(AApp: TObject; AHandle: Pointer);
begin
  inherited Create;
  BFCheckLibrary;
  FHeaders := CreateHeaders(b4r_httpsrv_req_headers_ref(AHandle));
  FFields := CreateFields(b4r_httpsrv_req_fields_ref(AHandle));
  FParams := CreateParams(b4r_httpsrv_req_params_ref(AHandle));
  FUploads := CreateUploads(AHandle);
  Freq := AHandle;
  FApp := AApp;
  FVersion := C2S(b4r_httpsrv_req_version(Freq));
  FMethod := C2S(b4r_httpsrv_req_method(Freq));
  FContentType := C2S(b4r_httpsrv_req_content_type(Freq));
  FURI := C2S(b4r_httpsrv_req_uri(Freq));
  FID := C2S(b4r_httpsrv_req_id(Freq));
  FPath := C2S(b4r_httpsrv_req_path(Freq));
  FPayload := C2B(b4r_httpsrv_req_payld(AHandle));
  FUp := b4r_httpsrv_req_up(Freq);
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
    FUp := VReq.FUp;
    FPayload := VReq.FPayload;
    FPaths := VReq.FPaths;
  end
  else
    inherited Assign(ASource);
end;

function TBrookHTTPServerRequest.CreateHeaders(
  AHandle: Pointer): TBrookHashStrings;
begin
  Result := TBrookHashStrings.Create(AHandle);
end;

procedure TBrookHTTPServerRequest.FreeHeaders(AHeaders: TBrookHashStrings);
begin
  AHeaders.Free;
end;

function TBrookHTTPServerRequest.CreateUploads(
  AHandle: Pointer): TBrookHTTPServerRequestUploads;
begin
  Result := TBrookHTTPServerRequestUploads.Create(AHandle);
end;

procedure TBrookHTTPServerRequest.FreeUploads(
  AUploads: TBrookHTTPServerRequestUploads);
begin
  AUploads.Free;
end;

function TBrookHTTPServerRequest.CreateFields(
  AHandle: Pointer): TBrookHashStrings;
begin
  Result := TBrookHashStrings.Create(AHandle);
end;

procedure TBrookHTTPServerRequest.FreeFields(AFields: TBrookHashStrings);
begin
  AFields.Free;
end;

function TBrookHTTPServerRequest.CreateParams(
  AHandle: Pointer): TBrookHashStrings;
begin
  Result := TBrookHashStrings.Create(AHandle);
end;

procedure TBrookHTTPServerRequest.FreeParams(AParams: TBrookHashStrings);
begin
  AParams.Free;
end;

function TBrookHTTPServerRequest.TryParam(const AName: string;
  out AValue: string): Boolean;
var
  VVal: Pcchar;
begin
  BFCheckLibrary;
  Result := b4r_httpsrv_req_try_param(Freq, S2C(AName), @VVal);
  if Result then
    AValue := C2S(VVal);
end;

function TBrookHTTPServerRequest.GetHandle: Pointer;
begin
  Result := Freq;
end;

end.
