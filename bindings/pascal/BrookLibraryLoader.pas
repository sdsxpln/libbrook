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

unit BrookLibraryLoader;

{$I Brook.inc}

interface

uses
  SysUtils,
  Classes,
  libbrook,
  BrookHandledClasses;

type

  { TBrookLibraryLoader }

  TBrookLibraryLoader = class(TBrookHandledComponent)
  private
    FLibLoaded: Boolean;
    FFileName: TFileName;
    FHandle: TLibHandle;
    FVersion: string;
    function IsFileNameStored: Boolean;
    procedure SetFileName(AValue: TFileName);
  protected
    function GetHandle: Pointer; override;
    procedure DoLoad; virtual;
  public
    constructor Create(AOwner: TComponent); override;
    procedure DefineProperties(AFiler: TFiler); override;
  published
    property FileName: TFileName read FFileName write SetFileName
      stored IsFileNameStored;
    property Version: string read FVersion stored False;
  end;

implementation

{ TBrookLibraryLoader }

constructor TBrookLibraryLoader.Create(AOwner: TComponent);
begin
  inherited Create(AOwner);
  FFileName := B4R_LIB_NAME;
end;

procedure TBrookLibraryLoader.DefineProperties(AFiler: TFiler);
begin
  inherited DefineProperties(AFiler);
  DoLoad;
end;

function TBrookLibraryLoader.IsFileNameStored: Boolean;
begin
  Result := FFileName <> B4R_LIB_NAME;
end;

procedure TBrookLibraryLoader.SetFileName(AValue: TFileName);
begin
  if FFileName = AValue then
    Exit;
  FLibLoaded := False;
  FFileName := AValue;
  DoLoad;
end;

function TBrookLibraryLoader.GetHandle: Pointer;
begin
  Result := @FHandle;
end;

procedure TBrookLibraryLoader.DoLoad;
begin
  if FLibLoaded then
    Exit;
  BFUnloadLibrary;
  FHandle := NilHandle;
  FLibLoaded := False;
  if (FFileName = '') or (not FileExists(FFileName)) then
    Exit;
  FHandle := BFLoadLibrary(FFileName);
  FLibLoaded := FHandle <> NilHandle;
  if FLibLoaded then
    { TODO: get version. }
  else
    FVersion := '';
end;

end.
