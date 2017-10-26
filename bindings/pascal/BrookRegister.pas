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

unit BrookRegister;

{$I Brook.inc}

interface

uses
  SysUtils,
  Classes,
  Dialogs,
{$IFDEF LCL}
  PropEdits,
  ComponentEditors
{$ELSE}
  DesignIntf,
  DesignEditors
{$ENDIF},
  libbrook;

resourcestring
  SBrookSelectLibrary = 'Select Brook library';
  SBrookLibraryFiles = 'Library files (%s)|%s|All files (*.*)|*.*';

type

  { TBrookLibraryFileNamePropertyEditor }

  TBrookLibraryFileNamePropertyEditor = class(
{$IFDEF LCL}TFileNamePropertyEditor{$ELSE}TStringProperty{$ENDIF})
  public
{$IFDEF LCL}
    function GetVerbCount: Integer; override;
    function GetVerb(AIndex: Integer): string; override;
    procedure ExecuteVerb(AIndex: Integer); override;
{$ENDIF}
    function GetFilter: string; {$IFDEF LCL}override{$ELSE}virtual{$ENDIF};
    function GetDialogTitle: string; {$IFDEF LCL}override{$ELSE}virtual{$ENDIF};
{$IFNDEF LCL}
    function CreateFileDialog: TOpenDialog; virtual;
    procedure Edit; override;
    function GetAttributes: TPropertyAttributes; override;
    function GetDialogOptions: TOpenOptions; virtual;
    function GetInitialDirectory: string; virtual;
    procedure SetFileName(const AFileName: string); virtual;
{$ENDIF}
  end;

  { TBrookLibraryFileNameComponentEditor }

  TBrookLibraryFileNameComponentEditor = class(TDefaultEditor)
  public
    procedure Edit; override;
    function GetVerb(AIndex: Integer): string; override;
{$IFNDEF LCL}
    function GetVerbCount: Integer; override;
{$ENDIF}
    procedure ExecuteVerb(AIndex: Integer); override;
  end;

  { TBrookOnRequestComponentEditor }

  TBrookOnRequestComponentEditor = class(TDefaultEditor)
  protected
    procedure EditProperty(const AProperty:
      {$IFDEF LCL}TPropertyEditor{$ELSE}IProperty{$ENDIF};
      var AContinue: Boolean); override;
  end;

procedure Register;

implementation

uses
  BrookLibraryLoader,
  BrookHTTPServerConfiguration,
  BrookHTTPServer;

procedure Register;
begin
  RegisterComponents('Brook', [
    TBrookLibraryLoader,
    TBrookHTTPServerConfiguration,
    TBrookHTTPServer
  ]);
  RegisterPropertyEditor(TypeInfo(TFileName), TBrookLibraryLoader,
    'FileName', TBrookLibraryFileNamePropertyEditor);
  RegisterComponentEditor(TBrookLibraryLoader,
    TBrookLibraryFileNameComponentEditor);
  RegisterComponentEditor(TBrookHTTPServer, TBrookOnRequestComponentEditor);
end;

{$IFDEF LCL}

{ TBrookLibraryFileNamePropertyEditor }

function TBrookLibraryFileNamePropertyEditor.GetVerbCount: Integer;
begin
  Result := Succ(inherited GetVerbCount);
end;

function TBrookLibraryFileNamePropertyEditor.GetVerb(AIndex: Integer): string;
begin
  if AIndex = 0 then
    Result := LoadResString(@SBrookSelectLibrary)
  else
    Result := inherited GetVerb(AIndex);
end;

procedure TBrookLibraryFileNamePropertyEditor.ExecuteVerb(AIndex: Integer);
begin
  if AIndex = 0 then
    Edit
  else
    inherited ExecuteVerb(AIndex);
end;

{$ENDIF}

function TBrookLibraryFileNamePropertyEditor.GetFilter: string;
var
  VSharedSuffix: string;
begin
  VSharedSuffix := Concat('*.', SharedSuffix);
  Result := Format(LoadResString(@SBrookLibraryFiles), [VSharedSuffix,
{$IFDEF LINUX}Concat({$ENDIF}VSharedSuffix
{$IFDEF LINUX}, ';', VSharedSuffix, '.*'){$ENDIF}]);
end;

function TBrookLibraryFileNamePropertyEditor.GetDialogTitle: string;
begin
  Result := LoadResString(@SBrookSelectLibrary);
end;

{$IFNDEF LCL}

function TBrookLibraryFileNamePropertyEditor.CreateFileDialog: TOpenDialog;
begin
  Result := TOpenDialog.Create(nil);
end;

procedure TBrookLibraryFileNamePropertyEditor.Edit;
begin
  with CreateFileDialog do
  try
    Filter := GetFilter;
    Options := GetDialogOptions;
    FileName := GetStrValue;
    InitialDir := GetInitialDirectory;
    Title := GetDialogTitle;
    if Execute then
      SetFileName(FileName);
  finally
    Free;
  end;
end;

function TBrookLibraryFileNamePropertyEditor.GetAttributes: TPropertyAttributes;
begin
  Result := [paDialog, paRevertable];
end;

function TBrookLibraryFileNamePropertyEditor.GetDialogOptions: TOpenOptions;
begin
  Result := [ofEnableSizing];
end;

function TBrookLibraryFileNamePropertyEditor.GetInitialDirectory: string;
begin
  Result := '';
end;

procedure TBrookLibraryFileNamePropertyEditor.SetFileName(const AFileName: string);
begin
  SetStrValue(AFileName);
end;

{$ENDIF}

{ TBrookLibraryFileNameComponentEditor }

procedure TBrookLibraryFileNameComponentEditor.Edit;
var
  VDialog: TOpenDialog;
  VLibraryLoader: TBrookLibraryLoader;
  VPropertyEditor: TBrookLibraryFileNamePropertyEditor;
begin
  VLibraryLoader := Component as TBrookLibraryLoader;
  if not Assigned(VLibraryLoader) then Exit;
  VPropertyEditor := TBrookLibraryFileNamePropertyEditor.Create(nil, 0);
  VDialog := VPropertyEditor.CreateFileDialog;
  try
    VDialog.Filter := VPropertyEditor.GetFilter;
    VDialog.Options := VPropertyEditor.GetDialogOptions;
    VDialog.InitialDir := VPropertyEditor.GetInitialDirectory;
    VDialog.Title := VPropertyEditor.GetDialogTitle;
    VDialog.FileName := VLibraryLoader.FileName;
    if VDialog.Execute then
    begin
      VLibraryLoader.FileName := VDialog.FileName;
      Designer.Modified;
    end;
  finally
    VPropertyEditor.Free;
  end;
end;

function TBrookLibraryFileNameComponentEditor.GetVerb(AIndex: Integer): string;
begin
  if AIndex = 0 then
    Result := LoadResString(@SBrookSelectLibrary)
  else
    Result := inherited GetVerb(AIndex);
end;

{$IFNDEF LCL}

function TBrookLibraryFileNameComponentEditor.GetVerbCount: Integer;
begin
  Result := Succ(inherited GetVerbCount);
end;

{$ENDIF}

procedure TBrookLibraryFileNameComponentEditor.ExecuteVerb(AIndex: Integer);
begin
  if AIndex = 0 then
    Edit
  else
    inherited ExecuteVerb(AIndex);
end;

{ TBrookOnRequestComponentEditor }

procedure TBrookOnRequestComponentEditor.EditProperty(const AProperty:
{$IFDEF LCL}TPropertyEditor{$ELSE}IProperty{$ENDIF}; var AContinue: Boolean);
begin
  if SameText(AProperty.GetName, 'OnRequest') then
    inherited EditProperty(AProperty, AContinue);
end;

end.
