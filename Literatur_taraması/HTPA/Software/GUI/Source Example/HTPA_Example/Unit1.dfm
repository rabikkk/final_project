object Form1: TForm1
  Left = 0
  Top = 0
  Caption = 'Form1'
  ClientHeight = 290
  ClientWidth = 554
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'Tahoma'
  Font.Style = []
  OldCreateOrder = False
  PixelsPerInch = 96
  TextHeight = 13
  object Label1: TLabel
    Left = 112
    Top = 16
    Width = 59
    Height = 13
    Caption = 'Local IP: ???'
  end
  object Label2: TLabel
    Left = 112
    Top = 35
    Width = 49
    Height = 13
    Caption = 'Device IP:'
  end
  object Button1: TButton
    Left = 24
    Top = 16
    Width = 75
    Height = 25
    Caption = 'Connect'
    TabOrder = 0
    OnClick = Button1Click
  end
  object Edit1: TEdit
    Left = 24
    Top = 80
    Width = 465
    Height = 21
    TabOrder = 1
    Text = 'Edit1'
  end
  object Button2: TButton
    Left = 440
    Top = 24
    Width = 75
    Height = 25
    Caption = 'Options'
    TabOrder = 2
    OnClick = Button2Click
  end
  object Button3: TButton
    Left = 24
    Top = 120
    Width = 75
    Height = 25
    Caption = 'Get T-Frame'
    TabOrder = 3
    OnClick = Button3Click
  end
  object IdUDPServer1: TIdUDPServer
    Bindings = <>
    DefaultPort = 30444
    Left = 320
    Top = 40
  end
end
