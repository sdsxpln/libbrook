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

unit dmlMainUplds;

{$I Examples.inc}

interface

uses
  SysUtils,
  Classes,
  BrookHTTPServerConfiguration,
  BrookHTTPServerRequestUploads,
  BrookHTTPServerResponse,
  BrookHTTPServerRequest,
  BrookHTTPServer;

type

  { TdmMainUplds }

  TdmMainUplds = class(TDataModule)
    BrookHTTPServer1: TBrookHTTPServer;
    BrookHTTPServerConfiguration1: TBrookHTTPServerConfiguration;
    procedure BrookHTTPServer1Request(ASender: TObject;
      ARequest: TBrookHTTPServerRequest; AResponse: TBrookHTTPServerResponse;
      var ADone: Boolean);
    procedure BrookHTTPServer1RequestError(ASender: TObject;
      ARequest: TBrookHTTPServerRequest; AResponse: TBrookHTTPServerResponse;
      const AErrorMsg: string; var ADone: Boolean);
    procedure DataModuleCreate(Sender: TObject);
  end;

const
  UPLOAD_PAGE =
    '<html>'+
    '<body>'+
    '<form action="" method="post" enctype="multipart/form-data">'+
    '<fieldset>'+
    '<legend>Choose the files:</legend>'+
    'File 1: <input type="file" name="file1"/><br>'+
    'File 2: <input type="file" name="file2"/><br>'+
    '<input type="submit"/>'+
    '</fieldset>'+
    '</form>'+
    '</body>'+
    '</html>';

var
  dmMainUplds: TdmMainUplds;

implementation

{$IFDEF FPC}
{$R *.lfm}
{$ELSE}
{$R *.dfm}
{$ENDIF}

{ TdmMainUplds }

procedure TdmMainUplds.DataModuleCreate(Sender: TObject);
begin
  BrookHTTPServerConfiguration1.UploadsDir :=
    Concat(ExtractFilePath(ParamStr(0)), 'uploads');
  ForceDirectories(BrookHTTPServerConfiguration1.UploadsDir);
end;

procedure TdmMainUplds.BrookHTTPServer1Request(ASender: TObject;
  ARequest: TBrookHTTPServerRequest; AResponse: TBrookHTTPServerResponse;
  var ADone: Boolean);
var
  VSavedOL, VErrOL: string;
  VUpload: TBrookHTTPServerRequestUpload;
begin
  //TODO: download file as soon as the AResponse.Download() method was done.
  if ARequest.IsPost then
  begin
    VSavedOL := '<ol>';
    VErrOL := '<ol style="color: red;">';
    for VUpload in ARequest.Uploads do
    begin
      if VUpload.Save then
        VSavedOL := Concat(VSavedOL, '<li><a href="?file=',
          VUpload.OriginalName, '">', VUpload.OriginalName, '</a></li>')
      else
        VErrOL := Concat(VErrOL, '<li><font color="red">', VUpload.Error,
          '</font></li>');
    end;
    VSavedOL := Concat(VSavedOL, '</ol>');
    VErrOL := Concat(VErrOL, '</ol>');
    AResponse.Send(Concat(
      '<html><body>Uploads:', VSavedOL, ' ', VErrOL, '</body></html>'));
  end
  else
    AResponse.Send(UPLOAD_PAGE);
end;

procedure TdmMainUplds.BrookHTTPServer1RequestError(ASender: TObject;
  ARequest: TBrookHTTPServerRequest; AResponse: TBrookHTTPServerResponse;
  const AErrorMsg: string; var ADone: Boolean);
begin
  AResponse.Send(
    '<html><body><font color="red">%s</font></body></html>', [AErrorMsg]);
end;

end.

