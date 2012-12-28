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
    Tx_Range = 80;
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
    Node_Range->Width = Tx_Range * 2;
    Node_Range->Height = Tx_Range * 2;
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
    Node_Label->PopupMenu = formMain->menuContextNode;

    Msg_Timer = new TTimer(owner);
    Msg_Timer->Enabled = false;
    Msg_Timer->OnTimer = Msg_TimerTimer;

    Msg_Buffer = new TList;
} // End of CommonConstructor


TRfd::~TRfd(void)
{
    delete Node_Body;
    delete Node_Range;
    delete Node_Label;
    while ( Msg_Buffer->Count > 0 )
    {
        TRadioMsg * msg = (TRadioMsg *)Msg_Buffer->First();
        delete msg;
        Msg_Buffer->Delete(0);
    }
    delete Msg_Buffer;
} // End of destructor


void TRfd::DrawNode(sint32 x, sint32 y)
{
    // x,y marks the midpoint of the Node Body circle and the Node Range circle
    Node_Body->Left = x - (Node_Body->Width / 2);
    Node_Body->Top = y - (Node_Body->Height / 2);
    Node_Range->Width = Tx_Range * 2;
    Node_Range->Height = Tx_Range * 2;
    Node_Range->Left = x - (Node_Range->Width / 2);
    Node_Range->Top = y - (Node_Range->Height / 2);
    Node_Label->Left = Node_Body->Left + ((Node_Body->Width - Node_Label->Width) / 2);
    Node_Label->Top = Node_Body->Top + ((Node_Body->Height - Node_Label->Height) / 2);
} // End of DrawNode


void __fastcall TRfd::Msg_TimerTimer(TObject * /*Sender*/)
{
    Msg_Timer->Enabled = false;

    while ( Msg_Buffer->Count > 0 )
    {
        // Handle the received message(s)
        TRadioMsg * rx_msg = (TRadioMsg *)Msg_Buffer->First();
        switch(rx_msg->Msg_Data[0])
        {
            case MT_JOIN_MY_NETWORK_REQ:
            {
                if ( Cluster_Level == CLUSTER_LEVEL_UNKNOWN )
                {
                    // This node is currently not connected to the network.
                    // So respond with the expected response
                    TRadioMsg resp_msg(1);
                    resp_msg.Timestamp = 0;                      // Put timestamp here
                    resp_msg.Recipient = rx_msg->Sender;
                    resp_msg.Sender = MAC_Address;
                    resp_msg.Msg_Data[0] = MT_JOIN_MY_NETWORK_RESP;
	                radioManager.TransmitMsg(&resp_msg, this);
                }
                break;
            }


            case MT_JOIN_MY_NETWORK_RESP:
            {
                // A node wants to join as a child
                // So respond back with a request to set the node's cluster level
                TRadioMsg resp_msg(2);
                resp_msg.Timestamp = 0;                      // Put timestamp here
                resp_msg.Recipient = rx_msg->Sender;
                resp_msg.Sender = MAC_Address;
                resp_msg.Msg_Data[0] = MT_SET_CLUSTER_LEVEL_REQ;
                resp_msg.Msg_Data[1] = Cluster_Level + 1;
                radioManager.TransmitMsg(&resp_msg, this);
                break;
            }


            case MT_SET_CLUSTER_LEVEL_REQ:
            {
                // Set the node's cluster level as instructed
                if ( Cluster_Level == CLUSTER_LEVEL_UNKNOWN )
                {
                    Cluster_Level = rx_msg->Msg_Data[1];

                    // Now send the response to say this node is now in the network
                    TRadioMsg resp_msg(1);
                    resp_msg.Timestamp = 0;                      // Put timestamp here
                    resp_msg.Recipient = rx_msg->Sender;
                    resp_msg.Sender = MAC_Address;
                    resp_msg.Msg_Data[0] = MT_SET_CLUSTER_LEVEL_RESP;
	                radioManager.TransmitMsg(&resp_msg, this);
                }
                break;
            }


            case MT_SET_CLUSTER_LEVEL_RESP:
            {
                break;
            }


            default:
                // Just ignore unrecognised messages
                break;
        } // End of switch statement

        // Remove the massage from the buffer
        delete rx_msg;
        Msg_Buffer->Delete(0);
    } // End of Message Buffer empty check
} // End of Msg_TimerTimer

