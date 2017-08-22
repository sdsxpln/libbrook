object dmMainUplds: TdmMainUplds
  OldCreateOrder = False
  OnCreate = DataModuleCreate
  Height = 154
  Width = 242
  object BrookHTTPServerConfiguration1: TBrookHTTPServerConfiguration
    Left = 96
    Top = 24
  end
  object BrookHTTPServer1: TBrookHTTPServer
    Configuration = BrookHTTPServerConfiguration1
    OnRequest = BrookHTTPServer1Request
    OnRequestError = BrookHTTPServer1RequestError
    Left = 96
    Top = 88
  end
end
