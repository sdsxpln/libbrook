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

unit BrookHTTPServerRequestUploads;

{$I Brook.inc}

interface

uses
  SysUtils,
  libbrook,
  BrookHandledClasses;

type
  TBrookHTTPServerRequestUploads = class;

  { TBrookHTTPServerRequestUpload }

  TBrookHTTPServerRequestUpload = record
  private
    FDestinationName: string;
    FDirectory: string;
    FEncoding: string;
    FFieldName: string;
    FMimeType: string;
    FName: string;
    FOriginalName: string;
    FSize: UInt64;
    FStream: Pcvoid;
    Fupld: Pb4r_httpsrv_req_upld;
    Freq: Pb4r_httpsrv_req;
    function GetError: string;
    function GetHandle: Pb4r_httpsrv_req_upld;
    function GetReqHandle: Pb4r_httpsrv_req;
  public
    constructor Create(AReqHandle: Pb4r_httpsrv_req;
      AHandle: Pb4r_httpsrv_req_upld; AStream: Pcvoid; ASize: UInt64;
      const ADirectory, AName, AOriginalName, ADestinationName, AFieldName,
      AMimeType, AEncoding: string);
    function Save(AOverwritten: Boolean): Boolean; overload;
    function Save: Boolean; overload;
    function SaveAs(const AName: TFileName;
      AOverwritten: Boolean): Boolean; overload;
    function SaveAs(const AName: TFileName): Boolean; overload;
    procedure Fail(const S: string); overload;
    procedure Fail(const AFmt: string; const AArgs: array of const); overload;
    property ReqHandle: Pb4r_httpsrv_req read GetReqHandle;
    property Handle: Pb4r_httpsrv_req_upld read GetHandle;
    property Stream: Pcvoid read FStream;
    property Size: UInt64 read FSize;
    property Directory: string read FDirectory;
    property Name: string read FName;
    property OriginalName: string read FOriginalName;
    property DestinationName: string read FDestinationName;
    property FieldName: string read FFieldName;
    property MimeType: string read FMimeType;
    property Encoding: string read FEncoding;
    property Error: string read GetError;
  end;

  { TBrookHTTPServerRequestUploadsEnumerator }

  TBrookHTTPServerRequestUploadsEnumerator = class
  private
    FUploads: TBrookHTTPServerRequestUploads;
    FCurr: TBrookHTTPServerRequestUpload;
    FBOF: Boolean;
  public
    constructor Create(AUploads: TBrookHTTPServerRequestUploads);
    function GetCurrent: TBrookHTTPServerRequestUpload;
    function MoveNext: Boolean;
    property Current: TBrookHTTPServerRequestUpload read GetCurrent;
  end;

  { TBrookHTTPServerRequestUploads }

  TBrookHTTPServerRequestUploads = class(TBrookHandledPersistent)
  private
    Fupld: Pb4r_httpsrv_req_upld;
    Freq: Pb4r_httpsrv_req;
  protected
    class function CreateItem(
      AUplds: TBrookHTTPServerRequestUploads; Areq: Pb4r_httpsrv_req;
      Aupld: Pb4r_httpsrv_req_upld): TBrookHTTPServerRequestUpload; inline;
    function CreateUpload(AReqHandle: Pb4r_httpsrv_req;
      AHandle: Pb4r_httpsrv_req_upld; AStream: Pcvoid; ASize: UInt64;
      const ADirectory, AName, AOriginalName, ADestinationName, AFieldName,
      AMimeType, AEncoding: string): TBrookHTTPServerRequestUpload; virtual;
    function GetHandle: Pointer; override;
    function IsEOF: Boolean; virtual;
    function GetCount: Integer; virtual;
  public
    constructor Create(AHandle: Pb4r_httpsrv_req); virtual;
    function GetEnumerator: TBrookHTTPServerRequestUploadsEnumerator;
    function First(out AUpload: TBrookHTTPServerRequestUpload): Boolean; virtual;
    function Next(out AUpload: TBrookHTTPServerRequestUpload): Boolean; virtual;
    property EOF: Boolean read IsEOF;
    property Count: Integer read GetCount;
  end;

implementation

{ TBrookHTTPServerRequestUploadsEnumerator }

constructor TBrookHTTPServerRequestUploadsEnumerator.Create(
  AUploads: TBrookHTTPServerRequestUploads);
begin
  inherited Create;
  FUploads := AUploads;
  FUploads.First(FCurr);
  FBOF := True;
end;

function TBrookHTTPServerRequestUploadsEnumerator.GetCurrent: TBrookHTTPServerRequestUpload;
begin
  Result := FCurr;
end;

function TBrookHTTPServerRequestUploadsEnumerator.MoveNext: Boolean;
begin
  if FBOF then
    FBOF := False
  else
    FUploads.Next(FCurr);
  Result := not FUploads.EOF;
end;

{ TBrookHTTPServerRequestUpload }

constructor TBrookHTTPServerRequestUpload.Create(AReqHandle: Pb4r_httpsrv_req;
  AHandle: Pb4r_httpsrv_req_upld; AStream: Pcvoid; ASize: UInt64;
  const ADirectory, AName, AOriginalName, ADestinationName, AFieldName,
  AMimeType, AEncoding: string);
begin
  Freq := AReqHandle;
  Fupld := AHandle;
  FStream := AStream;
  FSize := ASize;
  FDirectory := ADirectory;
  FName := AName;
  FOriginalName := AOriginalName;
  FDestinationName := ADestinationName;
  FFieldName := AFieldName;
  FMimeType := AMimeType;
  FEncoding := AEncoding;
end;

function TBrookHTTPServerRequestUpload.GetReqHandle: Pb4r_httpsrv_req;
begin
  Result := Freq;
end;

function TBrookHTTPServerRequestUpload.GetHandle: Pb4r_httpsrv_req_upld;
begin
  Result := Fupld;
end;

function TBrookHTTPServerRequestUpload.Save(AOverwritten: Boolean): Boolean;
begin
  B4RCheckLibrary;
  Result := b4r_httpsrv_req_upld_save(Freq, Fupld, AOverwritten);
end;

function TBrookHTTPServerRequestUpload.Save: Boolean;
begin
  Result := Save(True);
end;

function TBrookHTTPServerRequestUpload.SaveAs(const AName: TFileName;
  AOverwritten: Boolean): Boolean;
begin
  B4RCheckLibrary;
  Result := b4r_httpsrv_req_upld_save_as(Freq, Fupld, S2C(AName), AOverwritten);
end;

function TBrookHTTPServerRequestUpload.SaveAs(const AName: TFileName): Boolean;
begin
  Result := SaveAs(AName, True);
end;

procedure TBrookHTTPServerRequestUpload.Fail(const S: string);
begin
  B4RCheckLibrary;
  b4r_httpsrv_req_upld_failf(Fupld, '%s', S2C(S));
end;

procedure TBrookHTTPServerRequestUpload.Fail(const AFmt: string;
  const AArgs: array of const);
begin
  Fail(Format(AFmt, AArgs));
end;

function TBrookHTTPServerRequestUpload.GetError: string;
begin
  B4RCheckLibrary;
  Result := C2S(b4r_httpsrv_req_upld_err(Fupld));
end;

{ TBrookHTTPServerRequestUploads }

constructor TBrookHTTPServerRequestUploads.Create(AHandle: Pb4r_httpsrv_req);
begin
  inherited Create;
  Freq := AHandle;
end;

function TBrookHTTPServerRequestUploads.GetEnumerator: TBrookHTTPServerRequestUploadsEnumerator;
begin
  Result := TBrookHTTPServerRequestUploadsEnumerator.Create(Self);
end;

class function TBrookHTTPServerRequestUploads.CreateItem(
  AUplds: TBrookHTTPServerRequestUploads; Areq: Pb4r_httpsrv_req;
  Aupld: Pb4r_httpsrv_req_upld): TBrookHTTPServerRequestUpload;
begin
  Result := AUplds.CreateUpload(Areq, Aupld,
    b4r_httpsrv_req_upld_stream(Aupld),
    b4r_httpsrv_req_upld_size(Aupld),
    C2S(b4r_httpsrv_req_upld_dir(Aupld)),
    C2S(b4r_httpsrv_req_upld_name(Aupld)),
    C2S(b4r_httpsrv_req_upld_orig_name(Aupld)),
    C2S(b4r_httpsrv_req_upld_dest_name(Aupld)),
    C2S(b4r_httpsrv_req_upld_field(Aupld)),
    C2S(b4r_httpsrv_req_upld_mime(Aupld)),
    C2S(b4r_httpsrv_req_upld_encoding(Aupld)));
end;

function TBrookHTTPServerRequestUploads.CreateUpload(
  AReqHandle: Pb4r_httpsrv_req; AHandle: Pb4r_httpsrv_req_upld; AStream: Pcvoid;
  ASize: UInt64; const ADirectory, AName, AOriginalName, ADestinationName,
  AFieldName, AMimeType, AEncoding: string): TBrookHTTPServerRequestUpload;
begin
  Result := TBrookHTTPServerRequestUpload.Create(AReqHandle, AHandle, AStream,
    ASize, ADirectory, AName, AOriginalName, ADestinationName, AFieldName,
    AMimeType, AEncoding);
end;

function TBrookHTTPServerRequestUploads.GetHandle: Pointer;
begin
  Result := Freq;
end;

function TBrookHTTPServerRequestUploads.IsEOF: Boolean;
begin
  Result := not Assigned(Fupld);
end;

function TBrookHTTPServerRequestUploads.First(
  out AUpload: TBrookHTTPServerRequestUpload): Boolean;
begin
  B4RCheckLibrary;
  Result := b4r_httpsrv_req_uplds_first(Freq, @Fupld);
  if Result and Assigned(Fupld) then
    AUpload := CreateItem(Self, Freq, Fupld);
end;

function TBrookHTTPServerRequestUploads.Next(
  out AUpload: TBrookHTTPServerRequestUpload): Boolean;
begin
  B4RCheckLibrary;
  Result := b4r_httpsrv_req_uplds_next(@Fupld);
  if Result and Assigned(Fupld) then
    AUpload := CreateItem(Self, Freq, Fupld);
end;

function TBrookHTTPServerRequestUploads.GetCount: Integer;
begin
  B4RCheckLibrary;
  Result := b4r_httpsrv_req_uplds_count(Freq);
end;

end.

