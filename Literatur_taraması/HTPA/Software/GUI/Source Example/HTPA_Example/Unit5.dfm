object Form5: TForm5
  Left = 173
  Top = 71
  Caption = 'Options'
  ClientHeight = 329
  ClientWidth = 269
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'Tahoma'
  Font.Style = []
  OldCreateOrder = False
  Position = poDesigned
  OnShow = FormShow
  PixelsPerInch = 96
  TextHeight = 13
  object Label1: TLabel
    Left = 104
    Top = 8
    Width = 62
    Height = 20
    Caption = 'Options'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = 20
    Font.Name = 'Tahoma'
    Font.Style = [fsBold, fsUnderline]
    ParentFont = False
  end
  object Label11: TLabel
    Left = 8
    Top = 232
    Width = 80
    Height = 13
    Caption = 'Selected Device:'
  end
  object GroupBox1: TGroupBox
    Left = 8
    Top = 55
    Width = 249
    Height = 169
    Caption = 'Set static IP to Cam'
    TabOrder = 0
    object Label2: TLabel
      Left = 7
      Top = 27
      Width = 10
      Height = 13
      Caption = 'IP'
    end
    object Label3: TLabel
      Left = 78
      Top = 32
      Width = 4
      Height = 13
      Caption = '.'
    end
    object Label4: TLabel
      Left = 133
      Top = 32
      Width = 4
      Height = 13
      Caption = '.'
    end
    object Label5: TLabel
      Left = 189
      Top = 32
      Width = 4
      Height = 13
      Caption = '.'
    end
    object Label6: TLabel
      Left = 7
      Top = 60
      Width = 22
      Height = 13
      Caption = 'Sub-'
    end
    object Label7: TLabel
      Left = 7
      Top = 74
      Width = 16
      Height = 13
      Caption = 'net'
    end
    object Label8: TLabel
      Left = 78
      Top = 72
      Width = 4
      Height = 13
      Caption = '.'
    end
    object Label9: TLabel
      Left = 133
      Top = 72
      Width = 4
      Height = 13
      Caption = '.'
    end
    object Label10: TLabel
      Left = 189
      Top = 72
      Width = 4
      Height = 13
      Caption = '.'
    end
    object Edit1: TEdit
      Left = 35
      Top = 24
      Width = 33
      Height = 21
      TabOrder = 0
      Text = '192'
      OnKeyPress = Edit1KeyPress
      OnKeyUp = Edit1KeyUp
    end
    object Edit2: TEdit
      Left = 90
      Top = 24
      Width = 33
      Height = 21
      TabOrder = 1
      Text = '168'
      OnKeyPress = Edit2KeyPress
      OnKeyUp = Edit2KeyUp
    end
    object Edit4: TEdit
      Left = 200
      Top = 24
      Width = 33
      Height = 21
      TabOrder = 2
      Text = '122'
      OnKeyPress = Edit4KeyPress
      OnKeyUp = Edit4KeyUp
    end
    object Edit3: TEdit
      Left = 143
      Top = 24
      Width = 33
      Height = 21
      TabOrder = 3
      Text = '240'
      OnKeyPress = Edit3KeyPress
      OnKeyUp = Edit3KeyUp
    end
    object Edit5: TEdit
      Left = 35
      Top = 64
      Width = 33
      Height = 21
      TabOrder = 4
      Text = '255'
      OnKeyPress = Edit5KeyPress
      OnKeyUp = Edit5KeyUp
    end
    object Edit6: TEdit
      Left = 90
      Top = 63
      Width = 33
      Height = 21
      TabOrder = 5
      Text = '255'
      OnKeyPress = Edit6KeyPress
      OnKeyUp = Edit6KeyUp
    end
    object Edit7: TEdit
      Left = 145
      Top = 64
      Width = 33
      Height = 21
      TabOrder = 6
      Text = '255'
      OnKeyPress = Edit7KeyPress
      OnKeyUp = Edit7KeyUp
    end
    object Edit8: TEdit
      Left = 200
      Top = 64
      Width = 33
      Height = 21
      TabOrder = 7
      Text = '000'
      OnKeyPress = Edit8KeyPress
      OnKeyUp = Edit8KeyUp
    end
    object Button1: TButton
      Left = 16
      Top = 104
      Width = 217
      Height = 25
      Caption = 'Try to reach Cam / set default IP'
      TabOrder = 8
      OnClick = Button1Click
    end
    object Edit9: TEdit
      Left = 16
      Top = 138
      Width = 217
      Height = 21
      TabOrder = 9
      Text = 'UDP-Interface must be initialized.'
    end
  end
  object ComboBox1: TComboBox
    Left = 8
    Top = 251
    Width = 249
    Height = 21
    DropDownCount = 64
    TabOrder = 1
    Text = 'no device found yet.'
    OnChange = ComboBox1Change
  end
  object Button2: TButton
    Left = 8
    Top = 286
    Width = 249
    Height = 25
    Caption = 'Release selected Device '
    TabOrder = 2
    OnClick = Button2Click
  end
  object Button3: TButton
    Left = 8
    Top = 8
    Width = 23
    Height = 20
    Caption = '!'
    TabOrder = 3
    OnClick = Button3Click
  end
  object Timer1: TTimer
    Enabled = False
    OnTimer = Timer1Timer
    Left = 192
    Top = 8
  end
  object Timer2: TTimer
    Enabled = False
    Interval = 1
    Left = 224
    Top = 8
  end
end
