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
    // Network stuff
    Node_Type = NT_END_DEVICE;
    Cluster_Level = CLUSTER_LEVEL_UNKNOWN;
    Tx_Range = 150;

    // Visual stuff
    Node_Body = new TShape(owner);
    Node_Body->Shape = stCircle;
    Node_Body->Width = DEFAULT_NODE_SIZE;
    Node_Body->Height = DEFAULT_NODE_SIZE;
    Node_Body->OnMouseDown = formMain->shNodeMouseDown;
    Node_Body->OnMouseMove = formMain->shNodeMouseMove;
    Node_Body->OnMouseUp = formMain->shNodeMouseUp;

    Node_Range = new TShape(owner);
    Node_Range->Shape = stCircle;
    Node_Range->Brush->Style = bsClear;
    Node_Range->Width = Tx_Range;
    Node_Range->Height = Tx_Range;
    Node_Range->Visible = false;

    Node_Label = new TLabel(owner);
    Node_Label->Font->Name = "Arial";
    Node_Label->Font->Size = 12;
    Node_Label->Font->Color = clWhite;
    Node_Label->Font->Style = TFontStyles() << fsBold;
    Node_Label->OnMouseDown = formMain->shNodeMouseDown;
    Node_Label->OnMouseMove = formMain->shNodeMouseMove;
    Node_Label->OnMouseUp = formMain->shNodeMouseUp;
} // End of constructor


void TRfd::DrawNode(sint32 x, sint32 y)
{

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


