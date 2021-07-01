object Form8: TForm8
  Left = 0
  Top = 0
  Caption = 'Select Network Interface'
  ClientHeight = 120
  ClientWidth = 497
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'Tahoma'
  Font.Style = []
  OldCreateOrder = False
  OnCloseQuery = FormCloseQuery
  PixelsPerInch = 96
  TextHeight = 13
  object Label1: TLabel
    Left = 104
    Top = 8
    Width = 282
    Height = 13
    Caption = 'Multiple network interfaces found on this system. '
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -11
    Font.Name = 'Tahoma'
    Font.Style = [fsBold]
    ParentFont = False
  end
  object Label2: TLabel
    Left = 16
    Top = 27
    Width = 465
    Height = 13
    Caption = 
      'Please select the approbiate one, which should be used for commu' +
      'nication with the HTPA device.'
  end
  object ComboBox1: TComboBox
    Left = 152
    Top = 56
    Width = 193
    Height = 21
    TabOrder = 0
    Text = '???'
    OnKeyPress = ComboBox1KeyPress
    OnKeyUp = ComboBox1KeyUp
  end
  object Button1: TButton
    Left = 208
    Top = 83
    Width = 75
    Height = 25
    Caption = 'OK'
    TabOrder = 1
    OnClick = Button1Click
  end
end
