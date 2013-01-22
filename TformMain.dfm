object formMain: TformMain
  Left = 0
  Top = 0
  ClientHeight = 690
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
  OnClose = FormClose
  OnCreate = FormCreate
  OnPaint = FormPaint
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
      Left = 337
      Top = 8
      Width = 96
      Height = 24
      Caption = 'Power On'
      TabOrder = 0
      OnClick = btnPwrClick
    end
    object cbShowRange: TCheckBox
      Left = 450
      Top = 15
      Width = 113
      Height = 17
      Caption = 'Show Tx Range'
      TabOrder = 1
      OnClick = cbShowRangeClick
    end
    object btnNewRouter: TButton
      Left = 113
      Top = 8
      Width = 96
      Height = 24
      Caption = 'New Router ...'
      TabOrder = 2
      OnClick = btnNewRouterClick
    end
    object btnNewZed: TButton
      Left = 218
      Top = 8
      Width = 96
      Height = 24
      Caption = 'New ZED ...'
      TabOrder = 3
      OnClick = btnNewZedClick
    end
    object btnClearLog: TButton
      Left = 8
      Top = 8
      Width = 96
      Height = 24
      Caption = 'Clear Log'
      TabOrder = 4
      OnClick = btnClearLogClick
    end
  end
  object lvLog: TListView
    Left = 0
    Top = 41
    Width = 177
    Height = 649
    Align = alLeft
    BorderWidth = 1
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
    Font.Charset = ANSI_CHARSET
    Font.Color = clWindowText
    Font.Height = -12
    Font.Name = 'Arial'
    Font.Style = []
    GridLines = True
    ReadOnly = True
    RowSelect = True
    ParentFont = False
    TabOrder = 1
    ViewStyle = vsReport
    ExplicitHeight = 629
  end
  object menuMain: TMainMenu
    Left = 232
    Top = 64
    object menuFile: TMenuItem
      Caption = 'File'
      object menuNewNetwork: TMenuItem
        Caption = 'New Network'
        OnClick = menuNewNetworkClick
      end
      object menuLoad: TMenuItem
        Caption = 'Load Network File ...'
        OnClick = menuLoadClick
      end
      object menuSave: TMenuItem
        Caption = 'Save Network File'
        OnClick = menuSaveClick
      end
      object menuSaveAs: TMenuItem
        Caption = 'Save Network As ...'
        OnClick = menuSaveAsClick
      end
      object N1: TMenuItem
        Caption = '-'
      end
      object menuExit: TMenuItem
        Caption = 'Exit'
        OnClick = menuExitClick
      end
    end
    object menuNetwork: TMenuItem
      Caption = 'Network'
      object menuAddRouter: TMenuItem
        Caption = 'Add Router ...'
        OnClick = btnNewRouterClick
      end
      object menuAddEndDevice: TMenuItem
        Caption = 'Add End Device ...'
        OnClick = btnNewZedClick
      end
    end
    object menuHelp: TMenuItem
      Caption = 'Help'
      object About1: TMenuItem
        Caption = 'About ...'
        Enabled = False
      end
    end
  end
  object menuContextNode: TPopupMenu
    OnPopup = menuContextNodePopup
    Left = 240
    Top = 128
    object menuDeleteNode: TMenuItem
      Caption = 'Delete Node'
      OnClick = menuDeleteNodeClick
    end
    object menuBar: TMenuItem
      Caption = '-'
    end
    object menuDummy: TMenuItem
      Caption = 'Dummy'
    end
    object menuDummy2: TMenuItem
      Caption = 'Dummy'
    end
  end
  object dialogOpen: TOpenDialog
    DefaultExt = '.nwk'
    Filter = 'Network Files (*.nwk)|*.nwk|All Files (*.*)|*.*'
    Title = 'Load Network'
    Left = 352
    Top = 72
  end
  object dialogSave: TSaveDialog
    DefaultExt = '.nwk'
    Filter = 'Network Files (*.nwk)|*.nwk|All Files (*.*)|*.*'
    Options = [ofOverwritePrompt, ofHideReadOnly, ofEnableSizing]
    Title = 'Save Network As'
    Left = 352
    Top = 136
  end
end
