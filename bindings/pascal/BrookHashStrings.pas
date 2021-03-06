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

unit BrookHashStrings;

{$I Brook.inc}

interface

uses
  RTLConsts,
  SysUtils,
{$IFNDEF FPC}
 {$IFDEF MSWINDOWS}
  Windows,
 {$ENDIF}
{$ENDIF}
  libbrook,
  BrookHandledClasses;

type
  TBrookHashStrings = class;

  { TBrookHashStringsItem }

  TBrookHashStringsItem = record
  private
    FName: string;
    FValue: string;
  public
    constructor Create(const AName, AValue: string);
    property Name: string read FName;
    property Value: string read FValue;
  end;

  { TBrookHashStringsIteratorEvent }

  TBrookHashStringsIteratorEvent = function(AData: Pointer;
    AItem: TBrookHashStringsItem): Boolean;

  { TBrookHashStringsSortEvent }

  TBrookHashStringsSortEvent = function(AData: Pointer;
    AItemA, AItemB: TBrookHashStringsItem): Integer;

  { TBrookHashStringsEnumerator }

  TBrookHashStringsEnumerator = class
  private
    FHS: TBrookHashStrings;
    FCurr: TBrookHashStringsItem;
    FBOF: Boolean;
  public
    constructor Create(AHS: TBrookHashStrings);
    function GetCurrent: TBrookHashStringsItem;
    function MoveNext: Boolean;
    property Current: TBrookHashStringsItem read GetCurrent;
  end;

  { TBrookHashStrings }

  TBrookHashStrings = class(TBrookHandledPersistent)
  private
    Fhs: Pb4r_hs;
    Fhsl: PPb4r_hs;
    FExHandle: Boolean;
  protected
    class function DoIterCb(Acls: Pcvoid; Ahs: Pb4r_hs): cbool; cdecl; static;
    class function DoSortCb(Acls: Pcvoid; Aa, Ab: Pb4r_hs): cint; cdecl; static;
    class function CreateItem(Ahs: Pb4r_hs): TBrookHashStringsItem; inline;
    function GetValue(const AName: string): string; virtual;
    procedure SetValue(const AName: string; const AValue: string); virtual;
    function GetHandle: Pointer; override;
    function GetCount: Integer; virtual;
    function IsEOF: Boolean; virtual;
    property hs: Pb4r_hs read Fhs;
    property ExHandle: Boolean read FExHandle;
  public
    constructor Create(AHandle: PPb4r_hs); virtual;
    destructor Destroy; override;
    function GetEnumerator: TBrookHashStringsEnumerator;
    function Add(const AName, AValue: string): Boolean; virtual;
    function &Set(const AName, AValue: string): Boolean; virtual;
    function Remove(const AName: string): Boolean; virtual;
    function Get(const AName: string): string; virtual;
    function TryValue(const AName: string;
      out AValue: string): Boolean; virtual;
    function Has(const AName: string): Boolean; virtual;
    function ForEach(ACb: TBrookHashStringsIteratorEvent;
      AData: Pointer): Boolean; virtual;
    function First(out AItem: TBrookHashStringsItem): Boolean; virtual;
    function Next(out AItem: TBrookHashStringsItem): Boolean; virtual;
    procedure Clear; virtual;
    procedure Sort(ACmpCb: TBrookHashStringsSortEvent; AData: Pointer); virtual;
    property EOF: Boolean read IsEOF;
    property Count: Integer read GetCount;
    property Values[const AName: string]: string read GetValue
      write SetValue; default;
  end;

implementation

{ TBrookHashStringsItem }

constructor TBrookHashStringsItem.Create(const AName, AValue: string);
begin
  FName := AName;
  FValue := AValue;
end;

{ TBrookHashStringsEnumerator }

constructor TBrookHashStringsEnumerator.Create(AHS: TBrookHashStrings);
begin
  inherited Create;
  FHS := AHS;
  FHS.First(FCurr);
  FBOF := True;
end;

function TBrookHashStringsEnumerator.GetCurrent: TBrookHashStringsItem;
begin
  Result := FCurr;
end;

function TBrookHashStringsEnumerator.MoveNext: Boolean;
begin
  if FBOF then
    FBOF := False
  else
    FHS.Next(FCurr);
  Result := not FHS.EOF;
end;

{ TBrookHashStrings }

constructor TBrookHashStrings.Create(AHandle: PPb4r_hs);
begin
  inherited Create;
  if not Assigned(AHandle) then
    raise EArgumentNilException.CreateResFmt(@SParamIsNil, ['AHandle']);
  Fhsl := AHandle;
  FExHandle := Assigned(Fhsl);
end;

destructor TBrookHashStrings.Destroy;
begin
  try
    if not FExHandle then
    begin
      B4RCheckLibrary;
      b4r_hs_cleanup(Fhsl);
    end;
  finally
    inherited Destroy;
  end;
end;

function TBrookHashStrings.GetEnumerator: TBrookHashStringsEnumerator;
begin
  Result := TBrookHashStringsEnumerator.Create(Self);
end;

class function TBrookHashStrings.CreateItem(Ahs: Pb4r_hs): TBrookHashStringsItem;
begin
  Result := TBrookHashStringsItem.Create(TMarshal.ToString(b4r_hs_name(Ahs)),
    TMarshal.ToString(b4r_hs_val(Ahs)));
end;

function TBrookHashStrings.GetHandle: Pointer;
begin
  Result := Fhsl;
end;

function TBrookHashStrings.GetValue(const AName: string): string;
begin
  Result := Get(AName);
end;

procedure TBrookHashStrings.SetValue(const AName: string; const AValue: string);
begin
  &Set(AName, AValue);
end;

function TBrookHashStrings.Add(const AName, AValue: string): Boolean;
var
  M: TMarshaller;
begin
  B4RCheckLibrary;
  Result := b4r_hs_add(Fhsl, M.ToCString(AName), M.ToCString(AValue));
end;

function TBrookHashStrings.&Set(const AName, AValue: string): Boolean;
var
  M: TMarshaller;
begin
  B4RCheckLibrary;
  Result := b4r_hs_set(Fhsl, M.ToCString(AName), M.ToCString(AValue));
end;

function TBrookHashStrings.Remove(const AName: string): Boolean;
var
  M: TMarshaller;
begin
  B4RCheckLibrary;
  Result := b4r_hs_rm(Fhsl, M.ToCString(AName));
end;

function TBrookHashStrings.Get(const AName: string): string;
var
  M: TMarshaller;
begin
  B4RCheckLibrary;
  Result := TMarshal.ToString(b4r_hs_get(Fhsl^, M.ToCString(AName)));
end;

function TBrookHashStrings.TryValue(const AName: string;
  out AValue: string): Boolean;
var
  V: Pcchar;
  M: TMarshaller;
begin
  B4RCheckLibrary;
  Result := b4r_hs_try(Fhsl^, M.ToCString(AName), @V);
  if Result then
    AValue := TMarshal.ToString(V);
end;

function TBrookHashStrings.Has(const AName: string): Boolean;
var
  M: TMarshaller;
begin
  B4RCheckLibrary;
  Result := b4r_hs_has(Fhsl^, M.ToCString(AName));
end;

class function TBrookHashStrings.DoIterCb(Acls: Pcvoid; Ahs: Pb4r_hs): cbool;
var
  VMethod: PMethod absolute Acls;
begin
  if not Assigned(VMethod.Code) then
    Exit(False);
  Result := TBrookHashStringsIteratorEvent(VMethod.Code)(VMethod.Data,
    CreateItem(Ahs));
end;

function TBrookHashStrings.ForEach(ACb: TBrookHashStringsIteratorEvent;
  AData: Pointer): Boolean;
var
  VMethod: TMethod;
begin
  B4RCheckLibrary;
  VMethod.Code := @ACb;
  VMethod.Data := AData;
  Result := b4r_hs_iter(Fhsl^,{$IFNDEF VER3_0}@{$ENDIF}DoIterCb, @VMethod);
end;

function TBrookHashStrings.First(out AItem: TBrookHashStringsItem): Boolean;
begin
  B4RCheckLibrary;
  Fhs := Fhsl^;
  Result := Assigned(Fhs);
  if Result then
    AItem := CreateItem(Fhs);
end;

function TBrookHashStrings.Next(out AItem: TBrookHashStringsItem): Boolean;
begin
  B4RCheckLibrary;
  Result := b4r_hs_next(@Fhs);
  if Result and Assigned(Fhs) then
    AItem := CreateItem(Fhs);
end;

function TBrookHashStrings.GetCount: Integer;
begin
  B4RCheckLibrary;
  Result := b4r_hs_count(Fhsl^);
end;

function TBrookHashStrings.IsEOF: Boolean;
begin
  Result := not Assigned(Fhs);
end;

procedure TBrookHashStrings.Clear;
begin
  B4RCheckLibrary;
  b4r_hs_cleanup(Fhsl);
end;

class function TBrookHashStrings.DoSortCb(Acls: Pcvoid; Aa, Ab: Pb4r_hs): cint;
var
  VMethod: PMethod absolute Acls;
begin
  if not Assigned(VMethod.Code) then
    Exit(0);
  Result := TBrookHashStringsSortEvent(VMethod.Code)(VMethod.Data,
    CreateItem(Aa), CreateItem(Ab));
end;

procedure TBrookHashStrings.Sort(ACmpCb: TBrookHashStringsSortEvent;
  AData: Pointer);
var
  VMethod: TMethod;
begin
  B4RCheckLibrary;
  VMethod.Code := @ACmpCb;
  VMethod.Data := AData;
  b4r_hs_sort(Fhsl, {$IFNDEF VER3_0}@{$ENDIF}DoSortCb, @VMethod);
end;

end.
