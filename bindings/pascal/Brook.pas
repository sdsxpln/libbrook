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

unit Brook;

{$I Brook.inc}

interface

uses
  SysUtils;

resourcestring
  SBrookSubjectIsEmpty = '''%s'' cannot be empty';

type
  { Raised for indicating that a string is empty. }
  EBrookSubjectException = class(Exception);

  { Contains static methods for value checking. }
  BrookChecker = record
  public
    { Raises an exception indicating a nil argument.

      @param(AName The name specified for the argument.)
      @raises(EArgumentNilException Raised indicating the nil argument.)
    }
    class procedure RaiseNotNil(const AName: string); static; inline;
    { Raises an exception indicating an empty argument.

      @param(AName The name specified for the argument.)
      @raises(EArgumentEmptyException Raised indicating the empty argument.)
    }
    class procedure RaiseNotEmpty(const AName: string); static; inline;
    { Checks if an argument is nil and raises an exception if it is.

      @param(AValue The argument reference.)
      @param(AName The name specified for the checked argument.)
      @raises(EArgumentNilException Raised indicating the nil argument.)
    }
    class procedure CheckNotNil(AValue: Pointer;
      const AName: string); static; inline;
    { Checks if an argument is empty and raises an exception if it is.

      @param(AValue The argument value.)
      @param(AName The name specified for the checked argument.)
      @raises(EArgumentEmptyException Raised indicating the empty argument.)
    }
    class procedure CheckNotEmpty(const AValue: string;
      const AName: string); static; inline;
  end;

implementation

{$IFDEF FPC}

function ReturnAddress: CodePointer; inline;
begin
  Result := CodePointer(get_caller_addr(get_frame));
end;

{$ENDIF}

{ BrookChecker }

class procedure BrookChecker.RaiseNotNil(const AName: string);
begin
  raise EArgumentNilException.CreateResFmt(
    @SBrookSubjectIsEmpty, [AName]) at ReturnAddress;
end;

class procedure BrookChecker.RaiseNotEmpty(const AName: string);
begin
  raise EBrookSubjectException.CreateResFmt(
    @SBrookSubjectIsEmpty, [AName]) at ReturnAddress;
end;

class procedure BrookChecker.CheckNotNil(AValue: Pointer; const AName: string);
begin
  if not Assigned(AValue) then
    RaiseNotNil(AName);
end;

class procedure BrookChecker.CheckNotEmpty(const AValue: string;
  const AName: string);
begin
  if AValue = '' then
    RaiseNotEmpty(AName);
end;

end.

