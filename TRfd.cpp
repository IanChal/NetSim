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
    Parent_Node = NULL;
    Node_State = NS_NODE_IDLE;

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


/*-----------------------------------------------------------------------------------*/
/* This timer emulates a periodic(?) reading of a message buffer.                    */
/* Each time this timer triggers, all messages in the 'buffer' are read and executed */
/*-----------------------------------------------------------------------------------*/
void __fastcall TRfd::Msg_TimerTimer(TObject * /*Sender*/)
{
    Msg_Timer->Enabled = false;

} // End of Msg_TimerTimer


void TRfd::DiscoverDescendants(void)
{
    // An RFD cannot have any descendants, so just send back acknowledgments to say 'done'
} // End of DiscoverDescendants


void TRfd::SendJoinMyNetworkRequest(TRfd * target_node)
{
    // The node is sending a request (should be to all nodes) to join the network as a child of this node
    TRadioMsg msg(2);
    msg.Recipient_Node = target_node;
    msg.Sender_Node = this;
    msg.Msg_Data[0] = MT_JOIN_MY_NETWORK_REQ;
    msg.Msg_Data[1] = this->Cluster_Level + 1;
    radioManager.TransmitMsg(&msg, this);
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


void TRfd::SendSetClusterLevelCommand(TRfd * target_node)
{
    // Send the target node a command to set its cluster level
    TRadioMsg msg(2);
    msg.Recipient_Node = target_node;
    msg.Sender_Node = this;
    msg.Msg_Data[0] = MT_SET_CLUSTER_LEVEL_CMD;
    msg.Msg_Data[1] = Cluster_Level + 1;
    radioManager.TransmitMsg(&msg, this);
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


void TRfd::SendDiscoverDescendantsCommand(TRfd * target_node)
{
    // Send the target node a command to disciver all of its Descendant nodes
    TRadioMsg msg(1);
    msg.Recipient_Node = target_node;
    msg.Sender_Node = this;
    msg.Msg_Data[0] = MT_DISCOVER_DESCENDANTS_CMD;
    radioManager.TransmitMsg(&msg, this);
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


void TRfd::SendDescendantDiscoveryCompleteMsg(TRfd * target_node)
{
    // Tell the target node (should be parent) that this node has finished discovering all it's descendants
    TRadioMsg msg(1);
    msg.Recipient_Node = target_node;
    msg.Sender_Node = this;
    msg.Msg_Data[0] = MT_DESCENDANT_DISCOVERY_DONE_MSG;
    radioManager.TransmitMsg(&msg, this);
} // End of SendDescendantDiscoveryCompleteMsg


void TRfd::SendDescendantDiscoveryCompleteAck(TRfd * target_node)
{
    // Acknowledge the Descendant Discovery Complete message
    TRadioMsg msg(1);
    msg.Recipient_Node = target_node;
    msg.Sender_Node = this;
    msg.Msg_Data[0] = MT_DESCENDANT_DISCOVERY_DONE_ACK;
    radioManager.TransmitMsg(&msg, this);
} // End of SendDescendantDiscoveryCompleteAck



