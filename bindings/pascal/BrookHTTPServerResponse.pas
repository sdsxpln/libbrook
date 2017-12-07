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

unit BrookHTTPServerResponse;

{$I Brook.inc}

interface

uses
  SysUtils,
  Classes,
  libbrook,
  BrookHandledClasses,
  BrookHashStrings;

type

  { TBrookHTTPServerResponse }

  TBrookHTTPServerResponse = class(TBrookHandledPersistent)
  private
    FContentType: string;
    FHeaders: TBrookHashStrings;
    Fres: Pb4r_httpsrv_res;
    FApp: TObject;
    FStatus: UInt16;
    procedure SetContentType(const AValue: string);
    procedure SetStatus(AValue: UInt16);
  protected
    function CreateHeaders(AHandle: PPb4r_hs): TBrookHashStrings; virtual;
    procedure FreeHeaders(AHeaders: TBrookHashStrings); virtual;
    function GetHandle: Pointer; override;
  public
    constructor Create(AApp: TObject; AHandle: Pb4r_httpsrv_res); virtual;
    destructor Destroy; override;
    procedure Assign(ASource: TPersistent); override;
    procedure WriteBuffer(const ABuffer; ASize: LongInt);
    procedure WriteBytes(const ABytes: TBytes; ASize: LongInt);
    procedure Write(const AString: string; AEncoding: TEncoding); overload;
    procedure Write(const AString: string); overload;
    procedure Write(const AFmt: string; const AArgs: array of const;
      AEncoding: TEncoding); overload;
    procedure Write(const AFmt: string; const AArgs: array of const); overload;
    procedure Send(const AFmt: string; const AArgs: array of const); overload;
    procedure Send(const AString: string); overload;
    procedure SendFile(const AFileName: TFileName);
    procedure Json(const AJson: string);
    property Headers: TBrookHashStrings read FHeaders;
    property Status: Word read FStatus write SetStatus;
    property ContentType: string read FContentType write SetContentType;
  end;

implementation

{ TBrookHTTPServerResponse }

constructor TBrookHTTPServerResponse.Create(AApp: TObject;
  AHandle: Pb4r_httpsrv_res);
begin
  inherited Create;
  B4RCheckLibrary;
  FHeaders := CreateHeaders(b4r_httpsrv_res_headers(AHandle));
  FApp := AApp;
  Fres := AHandle;
end;

destructor TBrookHTTPServerResponse.Destroy;
begin
  FreeHeaders(FHeaders);
  inherited Destroy;
end;

procedure TBrookHTTPServerResponse.Assign(ASource: TPersistent);
var
  VRes: TBrookHTTPServerResponse;
begin
  if ASource is TBrookHTTPServerResponse then
  begin
    VRes := ASource as TBrookHTTPServerResponse;
    FStatus := VRes.FStatus;
    FContentType := VRes.FContentType;
  end
  else
    inherited Assign(ASource);
end;

function TBrookHTTPServerResponse.CreateHeaders(
  AHandle: PPb4r_hs): TBrookHashStrings;
begin
  Result := TBrookHashStrings.Create(AHandle);
end;

procedure TBrookHTTPServerResponse.FreeHeaders(AHeaders: TBrookHashStrings);
begin
  AHeaders.Free;
end;

function TBrookHTTPServerResponse.GetHandle: Pointer;
begin
  Result := Fres;
end;

procedure TBrookHTTPServerResponse.SetStatus(AValue: UInt16);
begin
  if FStatus = AValue then
    Exit;
  B4RCheckLibrary;
  FStatus := AValue;
  b4r_httpsrv_res_status(Fres, FStatus);
end;

procedure TBrookHTTPServerResponse.SetContentType(const AValue: string);
begin
  if FContentType = AValue then
    Exit;
  B4RCheckLibrary;
  FContentType := AValue;
  b4r_httpsrv_res_content_type(Fres, S2C(FContentType));
end;

procedure TBrookHTTPServerResponse.WriteBuffer(const ABuffer; ASize: LongInt);
begin
  B4RCheckLibrary;
  b4r_httpsrv_res_write_raw(Fres, @ABuffer, csize(ASize));
end;

procedure TBrookHTTPServerResponse.WriteBytes(const ABytes: TBytes;
  ASize: LongInt);
begin
  B4RCheckLibrary;
  b4r_httpsrv_res_write_raw(Fres, @ABytes[0], csize(ASize));
end;

procedure TBrookHTTPServerResponse.Write(const AString: string;
  AEncoding: TEncoding);
var
  VBytes: TBytes;
begin
  B4RCheckLibrary;
  if Assigned(AEncoding) then
    VBytes := AEncoding.GetBytes(UnicodeString(AString))
  else
    VBytes := TEncoding.UTF8.GetBytes(UnicodeString(AString));
  b4r_httpsrv_res_write_raw(Fres, @VBytes[0], csize(Length(VBytes)));
end;

procedure TBrookHTTPServerResponse.Write(const AString: string);
var
  S: AnsiString;
begin
  B4RCheckLibrary;
  S := AnsiString(AString);
  b4r_httpsrv_res_write_raw(Fres, @S[1], csize(Length(S)));
end;

procedure TBrookHTTPServerResponse.Write(const AFmt: string;
  const AArgs: array of const; AEncoding: TEncoding);
begin
  Write(Format(AFmt, AArgs), AEncoding);
end;

procedure TBrookHTTPServerResponse.Write(const AFmt: string;
  const AArgs: array of const);
begin
  Write(Format(AFmt, AArgs));
end;

procedure TBrookHTTPServerResponse.Send(const AFmt: string;
  const AArgs: array of const);
begin
  B4RCheckLibrary;
  b4r_httpsrv_res_send(Fres, '%s', S2C(Format(AFmt, AArgs)));
end;

procedure TBrookHTTPServerResponse.Send(const AString: string);
begin
  B4RCheckLibrary;
  b4r_httpsrv_res_send(Fres, '%s', S2C(AString));
end;

procedure TBrookHTTPServerResponse.SendFile(const AFileName: TFileName);
begin
  B4RCheckLibrary;
  b4r_httpsrv_res_send_file(Fres, S2C(AFileName));
end;

procedure TBrookHTTPServerResponse.Json(const AJson: string);
begin
  B4RCheckLibrary;
  b4r_httpsrv_res_json(Fres, S2C(AJson));
end;

end.
