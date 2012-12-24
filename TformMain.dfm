object formMain: TformMain
  Left = 0
  Top = 0
  ClientHeight = 636
  ClientWidth = 1025
  Color = clBtnFace
  Font.Charset = ANSI_CHARSET
  Font.Color = clWindowText
  Font.Height = -13
  Font.Name = 'Arial'
  Font.Style = []
  Menu = menuMain
  OldCreateOrder = False
  Position = poScreenCenter
  PixelsPerInch = 96
  TextHeight = 16
  object panelTools: TPanel
    Left = 0
    Top = 0
    Width = 1025
    Height = 41
    Align = alTop
    TabOrder = 0
    object btnPwr: TButton
      Left = 232
      Top = 8
      Width = 96
      Height = 24
      Caption = 'Power On'
      TabOrder = 0
      OnClick = btnPwrClick
    end
    object CheckBox1: TCheckBox
      Left = 345
      Top = 15
      Width = 113
      Height = 17
      Caption = 'Show Tx Range'
      TabOrder = 1
    end
    object Button1: TButton
      Left = 8
      Top = 8
      Width = 96
      Height = 24
      Caption = 'New Router ...'
      TabOrder = 2
    end
    object Button2: TButton
      Left = 113
      Top = 8
      Width = 96
      Height = 24
      Caption = 'New ZED ...'
      TabOrder = 3
    end
  end
  object lvLog: TListView
    Left = 0
    Top = 41
    Width = 177
    Height = 595
    Align = alLeft
    Columns = <
      item
        Caption = 'Tx'
      end
      item
        Caption = 'Rx'
      end
      item
        Caption = 'Msg'
        Width = 500
      end>
    ColumnClick = False
    GridLines = True
    ReadOnly = True
    RowSelect = True
    TabOrder = 1
    ViewStyle = vsReport
  end
  object menuMain: TMainMenu
    Left = 232
    Top = 64
    object menuFile: TMenuItem
      Caption = 'File'
      object menuLoad: TMenuItem
        Caption = 'Load Network ...'
      end
      object menuSave: TMenuItem
        Caption = 'Save Network'
      end
      object menuSaveAs: TMenuItem
        Caption = 'Save Network As ...'
      end
      object N1: TMenuItem
        Caption = '-'
      end
      object menuExit: TMenuItem
        Caption = 'Exit'
      end
    end
    object menuNetwork: TMenuItem
      Caption = 'Network'
      object menuAddRouter: TMenuItem
        Caption = 'Add Router ...'
      end
      object menuRemoveRouter: TMenuItem
        Caption = 'Remove Router'
      end
      object menuAddEndDevice: TMenuItem
        Caption = 'Add End Device ...'
      end
      object menuRemoveEndDevice: TMenuItem
        Caption = 'Remove End Device'
      end
    end
    object menuHelp: TMenuItem
      Caption = 'Help'
      object About1: TMenuItem
        Caption = 'About ...'
      end
    end
  end
end
