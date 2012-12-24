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
  OldCreateOrder = False
  Position = poScreenCenter
  PixelsPerInch = 96
  TextHeight = 16
  object shNode2: TShape
    Left = 622
    Top = 344
    Width = 20
    Height = 20
    Hint = 'Router'
    Brush.Color = clNavy
    ParentShowHint = False
    Pen.Color = clNavy
    Pen.Width = 2
    Shape = stCircle
    ShowHint = True
    OnMouseDown = shCoordMouseDown
    OnMouseMove = shCoordMouseMove
    OnMouseUp = shCoordMouseUp
  end
  object shNode3: TShape
    Left = 650
    Top = 144
    Width = 20
    Height = 20
    Hint = 'Router'
    Brush.Color = clNavy
    ParentShowHint = False
    Pen.Color = clNavy
    Pen.Width = 2
    Shape = stCircle
    ShowHint = True
    OnMouseDown = shCoordMouseDown
    OnMouseMove = shCoordMouseMove
    OnMouseUp = shCoordMouseUp
  end
  object shNode1: TShape
    Left = 602
    Top = 248
    Width = 20
    Height = 20
    Hint = 'Router'
    Brush.Color = clNavy
    ParentShowHint = False
    Pen.Color = clNavy
    Pen.Width = 2
    Shape = stCircle
    ShowHint = True
    OnMouseDown = shCoordMouseDown
    OnMouseMove = shCoordMouseMove
    OnMouseUp = shCoordMouseUp
  end
  object shNode4: TShape
    Left = 552
    Top = 416
    Width = 20
    Height = 20
    Hint = 'Router'
    Brush.Color = clNavy
    ParentShowHint = False
    Pen.Color = clNavy
    Pen.Width = 2
    Shape = stCircle
    ShowHint = True
    OnMouseDown = shCoordMouseDown
    OnMouseMove = shCoordMouseMove
    OnMouseUp = shCoordMouseUp
  end
  object shNode5: TShape
    Left = 690
    Top = 198
    Width = 20
    Height = 20
    Hint = 'Router'
    Brush.Color = clNavy
    ParentShowHint = False
    Pen.Color = clNavy
    Pen.Width = 2
    Shape = stCircle
    ShowHint = True
    OnMouseDown = shCoordMouseDown
    OnMouseMove = shCoordMouseMove
    OnMouseUp = shCoordMouseUp
  end
  object shNode6: TShape
    Left = 712
    Top = 280
    Width = 20
    Height = 20
    Hint = 'Router'
    Brush.Color = clNavy
    ParentShowHint = False
    Pen.Color = clNavy
    Pen.Width = 2
    Shape = stCircle
    ShowHint = True
    OnMouseDown = shCoordMouseDown
    OnMouseMove = shCoordMouseMove
    OnMouseUp = shCoordMouseUp
  end
  object shNode7: TShape
    Left = 576
    Top = 160
    Width = 20
    Height = 20
    Hint = 'Router'
    Brush.Color = clNavy
    ParentShowHint = False
    Pen.Color = clNavy
    Pen.Width = 2
    Shape = stCircle
    ShowHint = True
    OnMouseDown = shCoordMouseDown
    OnMouseMove = shCoordMouseMove
    OnMouseUp = shCoordMouseUp
  end
  object shCoord: TShape
    Left = 352
    Top = 248
    Width = 20
    Height = 20
    Hint = 'Gateway'
    Brush.Color = clGreen
    ParentShowHint = False
    Pen.Color = clGreen
    Pen.Width = 2
    Shape = stCircle
    ShowHint = True
    OnMouseDown = shCoordMouseDown
    OnMouseMove = shCoordMouseMove
    OnMouseUp = shCoordMouseUp
  end
  object shTxRange: TShape
    Left = 298
    Top = 47
    Width = 100
    Height = 100
    Hint = 'Gateway'
    Brush.Style = bsClear
    ParentShowHint = False
    Pen.Color = clGreen
    Shape = stCircle
    ShowHint = True
    Visible = False
  end
  object Label1: TLabel
    Left = 345
    Top = 88
    Width = 16
    Height = 19
    Caption = '0'
    Font.Charset = ANSI_CHARSET
    Font.Color = clWhite
    Font.Height = -16
    Font.Name = 'Arial'
    Font.Style = [fsBold]
    ParentFont = False
  end
  object panelTools: TPanel
    Left = 0
    Top = 0
    Width = 1025
    Height = 41
    Align = alTop
    TabOrder = 0
    object btnPwr: TButton
      Left = 8
      Top = 8
      Width = 80
      Height = 24
      Caption = 'Power On'
      TabOrder = 0
      OnClick = btnPwrClick
    end
    object CheckBox1: TCheckBox
      Left = 104
      Top = 12
      Width = 113
      Height = 17
      Caption = 'Show Tx Range'
      TabOrder = 1
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
    GridLines = True
    TabOrder = 1
    ViewStyle = vsReport
  end
end
