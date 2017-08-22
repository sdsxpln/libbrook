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

unit BrookHTTPServer;

{$I Brook.inc}

interface

uses
  SysUtils,
  Classes,
  libbrook,
  BrookHandledClasses,
  BrookHTTPServerRequest,
  BrookHTTPServerResponse,
  BrookHTTPServerConfiguration;

resourcestring
  SBrookCannotCreateServer = 'Cannot create server.';
  SBrookServerCannotListen = 'Server cannot listen.';
  SBrookServerCannotShutdown = 'Server cannot shutdown.';
  SBrookServerCannotSetOption = 'Cannot set server option: %s.';
  SBrookConfigurationNotAssigned = 'Configuration not assigned.';
  SBrookServerNotActived = 'Server not actived.';

type

  { TBrookHTTPServerConnectionEvent }

  TBrookHTTPServerConnectionEvent = procedure(ASender: TObject;
    const AID: string; AClosed: Boolean) of object;

  { TBrookHTTPServerBeforeRequestEvent }

  TBrookHTTPServerBeforeRequestEvent = procedure(ASender: TObject; const AID,
    AURI: string; var AAborted: Boolean) of object;

  { TBrookHTTPServerAfterRequestEvent }

  TBrookHTTPServerAfterRequestEvent = procedure(ASender: TObject;
    AFinished: Boolean) of object;

  { TBrookHTTPServerErrorEvent }

  TBrookHTTPServerErrorEvent = procedure(ASender: TObject;
    const AErrorMsg: string) of object;

  { TBrookHTTPServerRequestEvent }

  TBrookHTTPServerRequestEvent = procedure(ASender: TObject;
    ARequest: TBrookHTTPServerRequest; AResponse: TBrookHTTPServerResponse;
    var ADone: Boolean) of object;

  { TBrookHTTPServerRequestErrorEvent }

  TBrookHTTPServerRequestErrorEvent = procedure(ASender: TObject;
    ARequest: TBrookHTTPServerRequest; AResponse: TBrookHTTPServerResponse;
    const AErrorMsg: string; var ADone: Boolean) of object;

  { TBrookHTTPServer }

  TBrookHTTPServer = class(TBrookHandledComponent)
  private
    FAfterClose: TNotifyEvent;
    FAfterOpen: TNotifyEvent;
    FBeforeClose: TNotifyEvent;
    FBeforeOpen: TNotifyEvent;
    FForcedShutdown: Boolean;
    FOnConnection: TBrookHTTPServerConnectionEvent;
    FBeforeRequest: TBrookHTTPServerBeforeRequestEvent;
    FAfterRequest: TBrookHTTPServerAfterRequestEvent;
    FOnError: TBrookHTTPServerErrorEvent;
    FOnRequestError: TBrookHTTPServerRequestErrorEvent;
    FShutdownAttempts: Byte;
    Fsrv: Pb4r_httpsrv;
    FActive: Boolean;
    FConfiguration: TBrookHTTPServerConfiguration;
    FOnRequest: TBrookHTTPServerRequestEvent;
    FPort: UInt16;
    FStreamedActive: Boolean;
    function GetActiveConnections: Cardinal;
    function IsPortStored: Boolean;
    function IsShutdownAttemptsStored: Boolean;
    procedure SetPort(AValue: UInt16);
    procedure Createserver;
    procedure FreeServer;
    procedure SetupServer;
  protected
    procedure Loaded; override;
    procedure Notification(AComponent: TComponent;
      AOperation: TOperation); override;
    class procedure DoConCb(Acls: Pcvoid; const Aid: Pcchar;
      Aclosed: cbool); cdecl; static;
    class procedure DoReqInitCb(Acls: Pcvoid; const Aid: Pcchar;
      const Auri: Pcchar; Aaborted: Pcbool); cdecl; static;
    class procedure DoReqFiniCb(Acls: Pcvoid; Afinished: cbool); cdecl; static;
    class procedure DoReqCb(Acls: Pcvoid; Areq: Pb4r_httpsrv_req;
      Ares: Pb4r_httpsrv_res; Adone: Pcbool); cdecl; static;
    class procedure DoReqErrCb(Acls: Pcvoid; Areq: Pb4r_httpsrv_req;
      Ares: Pb4r_httpsrv_res; Adone: Pcbool; const Afmt: Pcchar;
      Ava: cva_list); cdecl; static;
    class procedure DoErrCb(Acls: Pcvoid; const Afmt: Pcchar;
      Ava: cva_list); cdecl; static;
    function CreateRequest(ASrv: TBrookHTTPServer;
      AHandle: Pointer): TBrookHTTPServerRequest; virtual;
    procedure FreeRequest(AReq: TBrookHTTPServerRequest); virtual;
    function CreateResponse(ASrv: TBrookHTTPServer;
      AHandle: Pointer): TBrookHTTPServerResponse; virtual;
    procedure FreeResponse(ARes: TBrookHTTPServerResponse); virtual;
    function GetHandle: Pointer; override;
    procedure DoOpen; virtual;
    procedure DoClose; virtual;
    procedure DoException; virtual;
    procedure DoConnection(ASender: TObject; const AID: string;
      AClosed: Boolean); virtual;
    procedure DoBeforeRequest(ASender: TObject; const AID, AURI: string;
      var AAborted: Boolean); virtual;
    procedure DoAfterRequest(ASender: TObject; AFinished: Boolean); virtual;
    procedure DoError(ASender: TObject; const AErrorMsg: string); virtual;
    procedure DoRequest(ASender: TObject; ARequest: TBrookHTTPServerRequest;
      AResponse: TBrookHTTPServerResponse; var ADone: Boolean); virtual;
    procedure DoRequestError(ASender: TObject; ARequest: TBrookHTTPServerRequest;
      AResponse: TBrookHTTPServerResponse; const AErrorMsg: string;
      var ADone: Boolean); virtual;
    procedure SetConfiguration(AValue: TBrookHTTPServerConfiguration); virtual;
    procedure SetActive(AValue: Boolean); virtual;
    procedure CheckConfiguration; inline;
    procedure CheckActive; inline;
    property StreamedActive: Boolean read FStreamedActive write FStreamedActive;
  public
    constructor Create(AOwner: TComponent); override;
    destructor Destroy; override;
    procedure Open;
    procedure Close;
    property ActiveConnections: Cardinal read GetActiveConnections;
  published
    property Active: Boolean read FActive write SetActive default False;
    property Configuration: TBrookHTTPServerConfiguration read FConfiguration
      write SetConfiguration;
    property Port: UInt16 read FPort write SetPort stored IsPortStored
      default B4R_HTTPSRV_PORT;
    property ShutdownAttempts: Byte read FShutdownAttempts
      write FShutdownAttempts stored IsShutdownAttemptsStored;
    property ForcedShutdown: Boolean read FForcedShutdown write FForcedShutdown
      default True;
    property BeforeOpen: TNotifyEvent read FBeforeOpen write FBeforeOpen;
    property AfterOpen: TNotifyEvent read FAfterOpen write FAfterOpen;
    property BeforeClose: TNotifyEvent read FBeforeClose write FBeforeClose;
    property AfterClose: TNotifyEvent read FAfterClose write FAfterClose;
    property OnConnection: TBrookHTTPServerConnectionEvent read FOnConnection
      write FOnConnection;
    property BeforeRequest: TBrookHTTPServerBeforeRequestEvent
      read FBeforeRequest write FBeforeRequest;
    property AfterRequest: TBrookHTTPServerAfterRequestEvent read FAfterRequest
      write FAfterRequest;
    property OnRequest: TBrookHTTPServerRequestEvent read FOnRequest
      write FOnRequest;
    property OnRequestError: TBrookHTTPServerRequestErrorEvent
      read FOnRequestError write FOnRequestError;
    property OnError: TBrookHTTPServerErrorEvent read FOnError write FOnError;
  end;

implementation

uses
  BrookLibraryLoader;

{ TBrookHTTPServer }

constructor TBrookHTTPServer.Create(AOwner: TComponent);
begin
  inherited Create(AOwner);
  FForcedShutdown := True;
  FPort := B4R_HTTPSRV_PORT;
end;

destructor TBrookHTTPServer.Destroy;
begin
  Configuration := nil;
  inherited Destroy;
end;

procedure TBrookHTTPServer.Loaded;
begin
  inherited Loaded;
  try
    if FStreamedActive then
      SetActive(True);
  except
    if csDesigning in ComponentState then
      DoException
    else
      raise;
  end;
end;

function TBrookHTTPServer.CreateRequest(ASrv: TBrookHTTPServer;
  AHandle: Pointer): TBrookHTTPServerRequest;
begin
  Result := TBrookHTTPServerRequest.Create(ASrv, AHandle);
end;

procedure TBrookHTTPServer.FreeRequest(AReq: TBrookHTTPServerRequest);
begin
  AReq.Free;
end;

function TBrookHTTPServer.CreateResponse(ASrv: TBrookHTTPServer;
  AHandle: Pointer): TBrookHTTPServerResponse;
begin
  Result := TBrookHTTPServerResponse.Create(ASrv, AHandle);
end;

procedure TBrookHTTPServer.FreeResponse(ARes: TBrookHTTPServerResponse);
begin
  ARes.Free;
end;

procedure TBrookHTTPServer.Createserver;
begin
  if not Assigned(Fsrv) then
  begin
    Fsrv := b4r_httpsrv_new(FConfiguration.Handle,
{$IFNDEF VER3_0}@{$ENDIF}DoReqCb, Self,
{$IFNDEF VER3_0}@{$ENDIF}DoReqErrCb, Self,
{$IFNDEF VER3_0}@{$ENDIF}DoErrCb, Self);
    if not b4r_httpsrv_setopt(Fsrv, B4R_HTTPSRV_OPT_CON_CB,
      @DoConCb, Pointer(Self)) then
      raise EInvalidOpException.CreateResFmt(@SBrookServerCannotSetOption,
        ['B4R_HTTPSRV_OPT_CON_CB']);
    if not b4r_httpsrv_setopt(Fsrv, B4R_HTTPSRV_OPT_REQ_INIT_CB,
      @DoReqInitCb, Pointer(Self)) then
      raise EInvalidOpException.CreateResFmt(@SBrookServerCannotSetOption,
        ['B4R_HTTPSRV_OPT_REQ_INIT_CB']);
    if not b4r_httpsrv_setopt(Fsrv, B4R_HTTPSRV_OPT_REQ_FINI_CB,
      @DoReqFiniCb, Pointer(Self)) then
      raise EInvalidOpException.CreateResFmt(@SBrookServerCannotSetOption,
        ['B4R_HTTPSRV_OPT_REQ_FINI_CB']);
    if not Assigned(Fsrv) then
      raise EInvalidPointer.CreateRes(@SBrookCannotCreateServer);
  end;
  SetupServer;
  if not b4r_httpsrv_listen(Fsrv) then
    raise EInvalidOpException.CreateRes(@SBrookServerCannotListen);
end;

procedure TBrookHTTPServer.FreeServer;
begin
  if not b4r_httpsrv_shutdown(Fsrv) then
    raise EInvalidOpException.CreateRes(@SBrookServerCannotShutdown);
  b4r_httpsrv_free(Fsrv);
  Fsrv := nil;
end;

procedure TBrookHTTPServer.SetupServer;
begin
  if FPort > 0 then
    b4r_httpsrv_setopt(Fsrv, B4R_HTTPSRV_OPT_PORT, FPort);
  b4r_httpsrv_setopt(Fsrv, B4R_HTTPSRV_OPT_SHUTDOWN_ATTEMPTS, FShutdownAttempts);
  b4r_httpsrv_setopt(Fsrv, B4R_HTTPSRV_OPT_FORCED_SHUTDOWN, FForcedShutdown);
end;

class procedure TBrookHTTPServer.DoConCb(Acls: Pcvoid; const Aid: Pcchar;
  Aclosed: cbool);
var
  VSrv: TBrookHTTPServer absolute Acls;
begin
  (VSrv as TBrookHTTPServer).DoConnection(VSrv, C2S(Aid), Aclosed);
end;

class procedure TBrookHTTPServer.DoReqInitCb(Acls: Pcvoid; const Aid: Pcchar;
  const Auri: Pcchar; Aaborted: Pcbool);
var
  VSrv: TBrookHTTPServer absolute Acls;
begin
  (VSrv as TBrookHTTPServer).DoBeforeRequest(VSrv, C2S(Aid),
    C2S(Auri), Aaborted^);
end;

class procedure TBrookHTTPServer.DoReqFiniCb(Acls: Pcvoid; Afinished: cbool);
var
  VSrv: TBrookHTTPServer absolute Acls;
begin
  (VSrv as TBrookHTTPServer).DoAfterRequest(VSrv, Afinished);
end;

class procedure TBrookHTTPServer.DoReqCb(Acls: Pcvoid; Areq: Pb4r_httpsrv_req;
  Ares: Pb4r_httpsrv_res; Adone: Pcbool);
var
  VSrv: TBrookHTTPServer absolute Acls;
  VReq: TBrookHTTPServerRequest;
  VRes: TBrookHTTPServerResponse;
begin
  VReq := VSrv.CreateRequest(VSrv, Areq);
  VRes := VSrv.CreateResponse(VSrv, Ares);
  try
    try
      VSrv.DoRequest(VSrv, VReq, VRes, Adone^);
    except
      on E: Exception do
        VSrv.DoRequestError(VSrv, VReq, VRes, E.Message, Adone^);
    end;
  finally
    VSrv.FreeResponse(VRes);
    VSrv.FreeRequest(VReq);
  end;
end;

class procedure TBrookHTTPServer.DoReqErrCb(Acls: Pcvoid;
  Areq: Pb4r_httpsrv_req; Ares: Pb4r_httpsrv_res; Adone: Pcbool;
  const Afmt: Pcchar; Ava: cva_list);
var
  VSrv: TBrookHTTPServer absolute Acls;
  VReq: TBrookHTTPServerRequest;
  VRes: TBrookHTTPServerResponse;
  VErr: Pcchar;
  VMsg: string;
begin
  VReq := VSrv.CreateRequest(VSrv, Areq);
  VRes := VSrv.CreateResponse(VSrv, Ares);
  try
    BFCheckLibrary;
    VErr := b4r_asprintf_va(Afmt, Ava);
    VMsg := C2S(VErr);
    b4r_free(VErr);
    VSrv.DoRequestError(VSrv, VReq, VRes, VMsg, Adone^);
  finally
    VSrv.FreeResponse(VRes);
    VSrv.FreeRequest(VReq);
  end;
end;

procedure TBrookHTTPServer.DoRequest(ASender: TObject;
  ARequest: TBrookHTTPServerRequest; AResponse: TBrookHTTPServerResponse;
  var ADone: Boolean);
begin
  if Assigned(FOnRequest) then
    FOnRequest(ASender, ARequest, AResponse, ADone);
end;

procedure TBrookHTTPServer.DoConnection(ASender: TObject; const AID: string;
  AClosed: Boolean);
begin
  if Assigned(FOnConnection) then
    FOnConnection(ASender, AID, AClosed);
end;

procedure TBrookHTTPServer.DoBeforeRequest(ASender: TObject; const AID,
  AURI: string; var AAborted: Boolean);
begin
  if Assigned(FBeforeRequest) then
    FBeforeRequest(ASender, AID, AURI, AAborted);
end;

procedure TBrookHTTPServer.DoAfterRequest(ASender: TObject; AFinished: Boolean);
begin
  if Assigned(FAfterRequest) then
    FAfterRequest(ASender, AFinished);
end;

procedure TBrookHTTPServer.DoRequestError(ASender: TObject;
  ARequest: TBrookHTTPServerRequest; AResponse: TBrookHTTPServerResponse;
  const AErrorMsg: string; var ADone: Boolean);
begin
  if Assigned(FOnRequestError) then
    FOnRequestError(ASender, ARequest, AResponse, AErrorMsg, ADone);
end;

class procedure TBrookHTTPServer.DoErrCb(Acls: Pcvoid; const Afmt: Pcchar;
  Ava: cva_list);
var
  VSrv: TBrookHTTPServer absolute Acls;
  VErr: Pcchar;
  VMsg: string;
begin
  BFCheckLibrary;
  VErr := b4r_asprintf_va(Afmt, Ava);
  VMsg := C2S(VErr);
  b4r_free(VErr);
  VSrv.DoError(VSrv, VMsg);
end;

procedure TBrookHTTPServer.DoError(ASender: TObject; const AErrorMsg: string);
begin
  if Assigned(FOnError) then
    FOnError(ASender, AErrorMsg);
end;

procedure TBrookHTTPServer.Notification(AComponent: TComponent;
  AOperation: TOperation);
begin
  inherited Notification(AComponent, AOperation);
  if AOperation <> opRemove then
    Exit;
  if AComponent = FConfiguration then
    Configuration := nil
  else
    if FActive and ((AComponent = Owner) or
      (AComponent is TBrookLibraryLoader)) then
      Active := False;
end;

procedure TBrookHTTPServer.Open;
begin
  Active := True;
end;

procedure TBrookHTTPServer.Close;
begin
  Active := False;
end;

function TBrookHTTPServer.GetHandle: Pointer;
begin
  Result := Fsrv;
end;

procedure TBrookHTTPServer.SetPort(AValue: UInt16);
begin
  if FPort = AValue then
    Exit;
  Close;
  FPort := AValue;
end;

procedure TBrookHTTPServer.DoOpen;
begin
  CheckConfiguration;
  FConfiguration.Prepare;
  BFCheckLibrary;
  CreateServer;
end;

procedure TBrookHTTPServer.DoClose;
begin
  BFCheckLibrary;
  FreeServer;
end;

procedure TBrookHTTPServer.DoException;
begin
  if Assigned(Classes.ApplicationHandleException) then
    Classes.ApplicationHandleException(Self)
  else
    ShowException(ExceptObject, ExceptAddr);
end;

procedure TBrookHTTPServer.CheckConfiguration;
begin
  if not Assigned(FConfiguration) then
    raise EInvalidOpException.CreateRes(@SBrookConfigurationNotAssigned);
end;

procedure TBrookHTTPServer.CheckActive;
begin
  if not FActive then
    raise EInvalidOpException.CreateRes(@SBrookServerNotActived);
end;

procedure TBrookHTTPServer.SetConfiguration(
  AValue: TBrookHTTPServerConfiguration);
begin
  if FConfiguration = AValue then
    Exit;
  Active := False;
  if Assigned(FConfiguration) then
    FConfiguration.RemoveFreeNotification(Self);
  FConfiguration := AValue;
  if Assigned(FConfiguration) then
    FConfiguration.FreeNotification(Self);
  if (FPort = 0) and Assigned(FConfiguration) and (FConfiguration.Port > 0) then
    FPort := FConfiguration.Port;
end;

procedure TBrookHTTPServer.SetActive(AValue: Boolean);
begin
  if AValue = FActive then
    Exit;
  if csDesigning in ComponentState then
  begin
    CheckConfiguration;
    BFCheckLibrary;
  end
  else
    if AValue then
    begin
      if csReading in ComponentState then
      begin
        FStreamedActive := True;
        Exit;
      end
      else
      begin
        if Assigned(FBeforeOpen) then
          FBeforeOpen(Self);
        DoOpen;
        if Assigned(FAfterOpen) then
          FAfterOpen(Self);
      end;
    end
    else
    begin
      if Assigned(FBeforeClose) then
        FBeforeClose(Self);
      DoClose;
      if Assigned(FAfterClose) then
        FAfterClose(Self);
    end;
  FActive := AValue;
end;

function TBrookHTTPServer.GetActiveConnections: Cardinal;
begin
  CheckActive;
  BFCheckLibrary;
  Result := b4r_httpsrv_active_conns(Fsrv);
end;

function TBrookHTTPServer.IsPortStored: Boolean;
begin
  Result := FPort <> B4R_HTTPSRV_PORT; { TODO: prioritize the config port. }
end;

function TBrookHTTPServer.IsShutdownAttemptsStored: Boolean;
begin
  Result := FShutdownAttempts > 0;
end;

end.
