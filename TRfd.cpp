/*------------------------------------------------------------------------------------*/
/* This file contains the class definitions etc. for a Reduced Function Device (RFD). */
/* An RFD is the most basic type of node than can be part of the network. It cannot   */
/* act as a data router.                                                              */
/*------------------------------------------------------------------------------------*/

/*---------------*/
/* Include Files */
/*---------------*/

#pragma hdrstop

#include "TRouter.h"
#include "TformMain.h"
#include "TRfd.h"

#pragma package(smart_init)


/*---------------------------*/
/* Methods of the TRfd class */
/*---------------------------*/

TRfd::TRfd(TComponent * owner)
{
    CommonConstructor(owner);
} // End of constructor


void TRfd::CommonConstructor(TComponent * owner)
{
    // Network stuff
    Node_Type = NT_END_DEVICE;
    Cluster_Level = CLUSTER_LEVEL_UNKNOWN;
    Tx_Range = 150;
    MAC_Address = 0;

    // Visual stuff
    Node_Body = new TShape(owner);
    Node_Body->Shape = stCircle;
    Node_Body->Width = DEFAULT_NODE_SIZE;
    Node_Body->Height = DEFAULT_NODE_SIZE;
    Node_Body->Pen->Color = clRed;
    Node_Body->Brush->Color = clRed;
    Node_Body->OnMouseDown = formMain->shNodeMouseDown;
    Node_Body->OnMouseMove = formMain->shNodeMouseMove;
    Node_Body->OnMouseUp = formMain->shNodeMouseUp;

    Node_Range = new TShape(owner);
    Node_Range->Shape = stCircle;
    Node_Range->Brush->Style = bsClear;
    Node_Range->Pen->Color = clRed;
    Node_Range->Width = Tx_Range;
    Node_Range->Height = Tx_Range;
    Node_Range->Visible = false;

    Node_Label = new TLabel(owner);
    Node_Label->Color = clRed;
    Node_Label->Caption = "00";
    Node_Label->Font->Name = "Arial";
    Node_Label->Font->Size = 8;
    Node_Label->Font->Color = clWhite;
    Node_Label->Font->Style = TFontStyles() << fsBold;
    Node_Label->OnMouseDown = formMain->shNodeMouseDown;
    Node_Label->OnMouseMove = formMain->shNodeMouseMove;
    Node_Label->OnMouseUp = formMain->shNodeMouseUp;
} // End of CommonConstructor

void TRfd::DrawNode(sint32 x, sint32 y)
{
    // x,y marks the midpoint of the Node Body circle and the Node Range circle
    Node_Body->Left = x - (Node_Body->Width / 2);
    Node_Body->Top = y - (Node_Body->Height / 2);
    Node_Range->Left = x - (Node_Range->Width / 2);
    Node_Range->Top = y - (Node_Range->Height / 2);
    Node_Label->Left = Node_Body->Left + ((Node_Body->Width - Node_Label->Width) / 2);
    Node_Label->Top = Node_Body->Top + ((Node_Body->Height - Node_Label->Height) / 2);
} // End of DrawNode


bool TRfd::ReceiveRadioMsg(TRadioMsg * msg)
{
bool handled;

    switch(msg->Msg_Data[0])
    {
        case MT_JOIN_NETWORK_REQ:
            if ( (this->Node_Type == NT_ROUTER) && (this->Cluster_Level == CLUSTER_LEVEL_UNKNOWN) )
            {
                // This node is currently not connected to the network, so accept the request
                this->Cluster_Level = msg->Msg_Data[1];
            }
            break;

        default:
            handled = false;
            break;
    } // End of switch statement

    return handled;
} // End of ReceiveRadioMsg


