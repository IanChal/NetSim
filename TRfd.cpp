/*------------------------------------------------------------------------------------*/
/* This file contains the class definitions etc. for a Reduced Function Device (RFD). */
/* An RFD is the most basic type of node than can be part of the network. It cannot   */
/* act as a data router.                                                              */
/*------------------------------------------------------------------------------------*/

/*---------------*/
/* Include Files */
/*---------------*/

#pragma hdrstop

#include "TRfd.h"
#include "TformMain.h"
#include "TRadioMsg.h"

#pragma package(smart_init)


/*-------------------*/
/* Macro Definitions */
/*-------------------*/

#define RFD_COLOUR                          (clRed)


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
    Parent_Node = NULL;
    Node_State = NS_NODE_IDLE;
    Retry_Counter = 0;

    // Visual stuff
    Node_Body = new TShape(owner);
    Node_Body->Shape = stCircle;
    Node_Body->Width = DEFAULT_NODE_SIZE;
    Node_Body->Height = DEFAULT_NODE_SIZE;
    Node_Body->Pen->Color = RFD_COLOUR;
    Node_Body->Brush->Color = RFD_COLOUR;
    Node_Body->OnMouseDown = formMain->shNodeMouseDown;
    Node_Body->OnMouseMove = formMain->shNodeMouseMove;
    Node_Body->OnMouseUp = formMain->shNodeMouseUp;

    Node_Range = new TShape(owner);
    Node_Range->Shape = stCircle;
    Node_Range->Brush->Style = bsClear;
    Node_Range->Pen->Color = RFD_COLOUR;
    Node_Range->Width = Tx_Range * 2;
    Node_Range->Height = Tx_Range * 2;
    Node_Range->Visible = false;

    Node_Label = new TLabel(owner);
    Node_Label->Color = RFD_COLOUR;
    Node_Label->Caption = "00";
    Node_Label->Font->Name = "Arial";
    Node_Label->Font->Size = 8;
    Node_Label->Font->Color = clBlack;
    Node_Label->Font->Style = TFontStyles() << fsBold;
    Node_Label->OnMouseDown = formMain->shNodeMouseDown;
    Node_Label->OnMouseMove = formMain->shNodeMouseMove;
    Node_Label->OnMouseUp = formMain->shNodeMouseUp;
    Node_Label->PopupMenu = formMain->menuContextNode;

    Msg_Timer = new TTimer(owner);
    Msg_Timer->Enabled = false;
    Msg_Timer->OnTimer = Msg_TimerTimer;

    Msg_Timeout_Timer = new TTimer(owner);
    Msg_Timeout_Timer->Enabled = false;
    Msg_Timeout_Timer->OnTimer = Msg_Timeout_TimerTimer;

    Msg_Buffer = new TList;
} // End of CommonConstructor


TRfd::~TRfd(void)
{
    delete Node_Body;
    delete Node_Range;
    delete Node_Label;
    ClearMessageBuffer();
    delete Msg_Buffer;
} // End of destructor


void TRfd::ClearMessageBuffer(void)
{
    while ( Msg_Buffer->Count > 0 )
    {
        TRadioMsg * msg = (TRadioMsg *)Msg_Buffer->First();
        delete msg;
        Msg_Buffer->Delete(0);
    }
} // End of ClearMessageBuffer


/*-----------------------------------------------------------------------------------*/
/* This timer emulates a periodic(?) reading of a message buffer.                    */
/* Each time this timer triggers, all messages in the 'buffer' are read and executed */
/*-----------------------------------------------------------------------------------*/
void __fastcall TRfd::Msg_TimerTimer(TObject * /*Sender*/)
{
    Msg_Timer->Enabled = false;
    while ( Msg_Buffer->Count > 0 )
    {
        TRadioMsg * rx_msg = (TRadioMsg *)Msg_Buffer->First();
        if ( rx_msg->Msg_Length > 0 )
        {
            switch ( rx_msg->Msg_Data[0] )
            {
                case MT_JOIN_MY_NETWORK_REQ:
                    HandleJoinMyNetworkRequest(rx_msg);
                    break;


                case MT_JOIN_MY_NETWORK_ACK:
                    HandleJoinMyNetworkAck(rx_msg);
                    break;


                case MT_SET_CLUSTER_LEVEL_CMD:
                    HandleSetClusterLevelCommand(rx_msg);
                    break;


                case MT_SET_CLUSTER_LEVEL_ACK:
                    HandleSetClusterLevelAck(rx_msg);
                    break;


                case MT_DISCOVER_DESCENDANTS_CMD:
                    HandleDiscoverDescendantsCommand(rx_msg);
                    break;


                case MT_DISCOVER_DESCENDANTS_ACK:
                    // No action necessary - it's just to complete the reqyest / responsse pair
                    break;


                case MT_DESCENDANT_DISCOVERY_DONE_MSG:
                    HandleDescendantDiscoveryDoneMsg(rx_msg);


                default:
                    break;
                // End of default case
            } // End of switch statement
        } // End of message length check

        // Delete the read message
        delete rx_msg;
        Msg_Buffer->Delete(0);
    } // End of message buffer scan loop
} // End of Msg_TimerTimer


/*---------------------------------------------------------------------*/
/* The Msg Timeout Timer event means that a message has timed out,     */
/* possibly without being answered, though this is not always the case */
/*---------------------------------------------------------------------*/
void __fastcall TRfd::Msg_Timeout_TimerTimer(TObject * /*Sender*/)
{
    Msg_Timeout_Timer->Enabled = false;
} // End of Msg_Timeout_TimerTimer


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


/*-----------------------------------------------------------------------------------*/
/* Message Transmission Methods (some of these may be overridden by derived classes) */
/*-----------------------------------------------------------------------------------*/

void TRfd::SendJoinMyNetworkRequest(TRfd * /*target_node*/)
{
    // An RFD should never send this message
    throw(Exception("Unexpected Message Transmission"));
} // End of SendJoinMyNetworkRequest


void TRfd::SendJoinMyNetworkAck(TRfd * target_node)
{
    // This node is agreeing to join the network as a child of the node that sent the request message
    TRadioMsg msg(1);
    msg.Recipient_Node = target_node;
    msg.Sender_Node = this;
    msg.Msg_Data[0] = MT_JOIN_MY_NETWORK_ACK;
    radioManager.TransmitMsg(&msg, this);
} // End of SendJoinMyNetworkAck


void TRfd::SendSetClusterLevelCommand(TRfd * /*target_node*/)
{
    // An RFD should never send this message
    throw(Exception("Unexpected Message Transmission"));
} // End of SendSetClusterLevelCommand


void TRfd::SendSetClusterLevelAck(TRfd * target_node)
{
    // Acknowledge the Set Cluster Level command
    TRadioMsg msg(1);
    msg.Recipient_Node = target_node;
    msg.Sender_Node = this;
    msg.Msg_Data[0] = MT_SET_CLUSTER_LEVEL_ACK;
    radioManager.TransmitMsg(&msg, this);
} // End of SendSetClusterLevelAck


void TRfd::SendDiscoverDescendantsCommand(TRfd * /*target_node*/)
{
    // An RFD should never send this message
    throw(Exception("Unexpected Message Transmission"));
} // End of SendDiscoverDescendantsCommand


void TRfd::SendDiscoverDescendantsAck(TRfd * target_node)
{
    // Send an Ack to tell the target node (should be parent) that the Discover Descendants message has been accepted
    TRadioMsg msg(1);
    msg.Recipient_Node = target_node;
    msg.Sender_Node = this;
    msg.Msg_Data[0] = MT_DISCOVER_DESCENDANTS_ACK;
    radioManager.TransmitMsg(&msg, this);
} // End of SendDiscoverDescendantsAck


void TRfd::SendDescendantDiscoveryCompleteMsg(TRfd * target_node, uint16 count)
{
    // Tell the target node (should be parent) that this node has finished discovering all it's descendants
    TRadioMsg msg(3);
    msg.Recipient_Node = target_node;
    msg.Sender_Node = this;
    msg.Msg_Data[0] = MT_DESCENDANT_DISCOVERY_DONE_MSG;
    msg.Msg_Data[1] = (uint8)(count >> 8);
    msg.Msg_Data[2] = (uint8)count;
    radioManager.TransmitMsg(&msg, this);
} // End of SendDescendantDiscoveryCompleteMsg


void TRfd::SendDescendantDiscoveryCompleteAck(TRfd * /*target_node*/)
{
    // An RFD should never send this message
    throw(Exception("Unexpected Message Transmission"));
} // End of SendDescendantDiscoveryCompleteAck


/*-------------------------------------------------------------------------------*/
/* Message handling methods (some of these may be overridden by derived classes) */
/*-------------------------------------------------------------------------------*/

void TRfd::HandleJoinMyNetworkRequest(TRadioMsg * msg)
{
    // Check the cluster level to decide whether to join the network
//    if ( msg->Msg_Data[1] < Cluster_Level )
    if ( (Cluster_Level == CLUSTER_LEVEL_UNKNOWN) && (msg->Msg_Data[1] < 11) )
//    if ( (msg->Msg_Data[1] < Cluster_Level) && (msg->Msg_Data[1] < 11) )
    {
        // The cluster level offered is less than the current value (less hops to coordinator),
        // so acknowledge the request. No action is taken at this point in case the sender node
        // cannot received our transmissions (too weak). If this happened, this node would falsely
        // flag that it is connected to the network!
        SendJoinMyNetworkAck(msg->Sender_Node);
    } // End of cluster level check
} // End of HandleJoinMyNetworkRequest


void TRfd::HandleJoinMyNetworkAck(TRadioMsg * /*msg*/)
{
    // An RFD should never receive this message
    throw(Exception("Unexpected Message Transmission"));
} // End of HandleJoinMyNetworkAck


void TRfd::HandleSetClusterLevelCommand(TRadioMsg * msg)
{
    if ( Parent_Node != NULL )
    {
        // Delete this node from the child list of the current parent
        // TODO: How would this be achieved in reality??
        if ( Parent_Node->Child_List->IndexOf(this) != -1 )
        {
            Parent_Node->Child_List->Remove(this);
        }
        else
        {
            throw(Exception("Unexpected Orphan Node Found!"));
        }
    } // End of parent node already assigned

    // Make the parent-child association
    Parent_Node = (TRouter *)msg->Sender_Node;
    Cluster_Level = msg->Msg_Data[1];
    SendSetClusterLevelAck(msg->Sender_Node);
} // End of HandleSetClusterLevelCommand


void TRfd::HandleSetClusterLevelAck(TRadioMsg * /*msg*/)
{
    // An RFD should never receive this message
    throw(Exception("Unexpected Message Transmission"));
} // End of HandleSetClusterLevelAck


void TRfd::HandleDiscoverDescendantsCommand(TRadioMsg * msg)
{
    // An RFD does not support descendants, so just send back positive
    // responses as if no descendants had been found
    SendDiscoverDescendantsAck(msg->Sender_Node);
    SendDescendantDiscoveryCompleteMsg(msg->Sender_Node, 0);
} // End of HandleDiscoverDescendantsCommand


void TRfd::HandleDescendantDiscoveryDoneMsg(TRadioMsg * /*msg*/)
{
    // An RFD should never receive this message
    throw(Exception("Unexpected Message Transmission"));
} // End of HandleDescendantDiscoveryDoneMsg

