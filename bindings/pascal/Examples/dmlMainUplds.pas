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
  //TODO: fazer download do arquivo assim que o método AResponse.Download estiver pronto.
  if ARequest.Up then
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

