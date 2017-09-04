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
{$IFDEF FPC}
  PropEdits,
  ComponentEditors
{$ELSE}
  Dialogs,
  DesignIntf,
  DesignEditors
{$ENDIF};

type

  { TBrookLibraryFileNamePropertyEditor }

  TBrookLibraryFileNamePropertyEditor = class(
{$IFDEF FPC}TFileNamePropertyEditor{$ELSE}TStringProperty{$ENDIF})
  public
    function GetFilter: string; {$IFDEF FPC}override{$ELSE}virtual{$ENDIF};
    function GetDialogTitle: string; {$IFDEF FPC}override{$ELSE}virtual{$ENDIF};
{$IFNDEF FPC}
    function CreateFileDialog: TOpenDialog; virtual;
    procedure Edit; override;
    function GetAttributes: TPropertyAttributes; override;
    function GetDialogOptions: TOpenOptions; virtual;
    function GetInitialDirectory: string; virtual;
    procedure SetFileName(const AFileName: string); virtual;
{$ENDIF}
  end;

  { TBrookOnRequestComponentEditor }

  TBrookOnRequestComponentEditor = class(TDefaultEditor)
  protected
    procedure EditProperty(const AProperty:
      {$IFDEF FPC}TPropertyEditor{$ELSE}IProperty{$ENDIF};
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
  RegisterComponentEditor(TBrookHTTPServer, TBrookOnRequestComponentEditor);
end;

{ TBrookLibraryFileNamePropertyEditor }

function TBrookLibraryFileNamePropertyEditor.GetFilter: string;
begin
  Result := 'Library files (*.dll;*.so)|*.dll;*.so';
end;

function TBrookLibraryFileNamePropertyEditor.GetDialogTitle: string;
begin
  Result := 'Select a library';
end;

{$IFNDEF FPC}

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

{ TBrookOnRequestComponentEditor }

procedure TBrookOnRequestComponentEditor.EditProperty(const AProperty:
{$IFDEF FPC}TPropertyEditor{$ELSE}IProperty{$ENDIF}; var AContinue: Boolean);
begin
  if SameText(AProperty.GetName, 'OnRequest') then
    inherited EditProperty(AProperty, AContinue);
end;

end.
