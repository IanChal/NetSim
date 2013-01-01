/*---------------*/
/* Include Files */
/*---------------*/

#pragma hdrstop

#include "TRouter.h"
#include "TRadioMsg.h"
#include "TRadioManager.h"

#pragma package(smart_init)


/*------------------------------*/
/* Methods of the TRouter class */
/*------------------------------*/

TRouter::TRouter(TComponent * owner) : TRfd(owner)
{
    CommonConstructorTasks(owner);
} // End of constructor


void TRouter::CommonConstructorTasks(TComponent * /*owner*/)
{
    // Network Stuff
    Node_Type = NT_ROUTER;
    Msg_Timeout_Timer->OnTimer = Msg_Timeout_TimerTimer;

    // Visual Stuff
    Node_Body->Pen->Color = clBlue;
    Node_Body->Brush->Color = clBlue;
    Node_Range->Pen->Color = clBlue;
    Node_Label->Color = clBlue;

    // Initialise the child list
    Child_List = new TList;
} // End of CommonConstructorTasks


TRouter::~TRouter(void)
{
    Child_List->Clear();
	delete Child_List;
} // End of destructor


/*---------------------------------------------------------------------*/
/* The Msg Timeout Timer event means that a message has timed out,     */
/* possibly without being answered, though this is not always the case */
/*---------------------------------------------------------------------*/
void __fastcall TRouter::Msg_Timeout_TimerTimer(TObject * /*Sender*/)
{
    Msg_Timeout_Timer->Enabled = false;
    switch( Node_State )
    {
        case NS_NODE_WAITING_JOINERS:
        {
            if ( Child_Added )
            {
                Retry_Counter = 0;
            }
            else
            {
                Retry_Counter++;
            }

            if ( Retry_Counter < 1 )
            {
                // Send the Joint My Network message again
                SendJoinMyNetworkRequest(MSG_RECIPIENT_ALL_NODES);
            } // End of retries not yet expired
            else
            {
                // Retries have expires while asking nodes to join the network as our child.
                // So detection of our immediate descendants is complete.
                // The next step of finding descendants is to get each of these children to
                // detect their descendants, and report back to this node when complete
                Node_State = NS_UNSPECIFIED;
                if ( Child_List->Count > 0 )
                {
                    Child_List_Index = 0;
                    TRfd * first_child = (TRfd *)Child_List->First();
                    first_child->Retry_Counter = 0;
                    SendDiscoverDescendantsCommand(first_child);
                } // End of child count check
                else
                {
                    // There are no more children to discover, so the process is complete
                    if ( Parent_Node != NULL )
                    {
                        SendDescendantDiscoveryCompleteMsg(Parent_Node);
                    }
                }
            } // End of retries expired
            break;
        } // End of NS_NODE_WAITING_JOINERS case


        default:
            // Should never get here
            throw(Exception("Unhandled Message Timeout"));
    } // End of switch statement
} // End of Msg_Timeout_TimerTimer


/*-----------------------------------------------------------------------------------*/
/* Message Transmission Methods (some of these may be overridden by derived classes) */
/*-----------------------------------------------------------------------------------*/

void TRouter::SendJoinMyNetworkRequest(TRfd * target_node)
{
    // The node is sending a request (should be to all nodes) to join the network as a child of this node
    TRadioMsg msg(2);
    msg.Recipient_Node = target_node;
    msg.Sender_Node = this;
    msg.Msg_Data[0] = MT_JOIN_MY_NETWORK_REQ;
    msg.Msg_Data[1] = Cluster_Level + 1;
    radioManager.TransmitMsg(&msg, this);

    // The Join My Network command needs a message timeout
    Node_State = NS_NODE_WAITING_JOINERS;
    Child_Added = false;
    if ( Msg_Timeout_Timer->Enabled == false )
    {
        Msg_Timeout_Timer->Interval = 500;
        Msg_Timeout_Timer->Enabled = true;
    }
} // End of SendJoinMyNetworkRequest


void TRouter::SendSetClusterLevelCommand(TRfd * target_node)
{
    // Send the target node a command to set its cluster level
    TRadioMsg msg(2);
    msg.Recipient_Node = target_node;
    msg.Sender_Node = this;
    msg.Msg_Data[0] = MT_SET_CLUSTER_LEVEL_CMD;
    msg.Msg_Data[1] = Cluster_Level + 1;
    radioManager.TransmitMsg(&msg, this);
} // End of SendSetClusterLevelCommand


void TRouter::SendDiscoverDescendantsCommand(TRfd * target_node)
{
    // Send the target node a command to disciver all of its Descendant nodes
    TRadioMsg msg(1);
    msg.Recipient_Node = target_node;
    msg.Sender_Node = this;
    msg.Msg_Data[0] = MT_DISCOVER_DESCENDANTS_CMD;
    radioManager.TransmitMsg(&msg, this);
} // End of SendDiscoverDescendantsCommand


void TRouter::SendDescendantDiscoveryCompleteAck(TRfd * target_node)
{
    // Acknowledge the Descendant Discovery Complete message
    TRadioMsg msg(1);
    msg.Recipient_Node = target_node;
    msg.Sender_Node = this;
    msg.Msg_Data[0] = MT_DESCENDANT_DISCOVERY_DONE_ACK;
    radioManager.TransmitMsg(&msg, this);
} // End of SendDescendantDiscoveryCompleteAck


/*-------------------------------------------------------------------------------*/
/* Message handling methods (some of these may be overridden by derived classes) */
/*-------------------------------------------------------------------------------*/

void TRouter::HandleJoinMyNetworkAck(TRadioMsg * msg)
{
    // A node has accepted our Network Join request, so set it's cluster level
    SendSetClusterLevelCommand(msg->Sender_Node);
} // End of HandleJoinMyNetworkAck


void TRouter::HandleSetClusterLevelAck(TRadioMsg * msg)
{
    // A node has joined the network as our child, so add it to the list
    Child_List->Add(msg->Sender_Node);
    Child_Added = true;

    // Repainting the screen will show the new association
    Node_Body->Parent->Repaint();

} // End of HandleSetClusterLevelAck


void TRouter::HandleDiscoverDescendantsCommand(TRadioMsg * msg)
{
    // The router node has been asked to discover its descendants.
    // Start by acknowledging the command.
    SendDiscoverDescendantsAck(msg->Sender_Node);

    // Now start of the discover process by broadcasting a Join My Network request
    SendJoinMyNetworkRequest(MSG_RECIPIENT_ALL_NODES);
} // End of HandleDiscoverDescendantsCommand


void TRouter::HandleDescendantDiscoveryDoneMsg(TRadioMsg * msg)
{
    // A child node has completed its descendant discovery,
    // so acknowledge and moveon to the next, if there is one
    SendDescendantDiscoveryCompleteAck(msg->Sender_Node);
    if ( ++Child_List_Index < Child_List->Count )
    {
        TRfd * next_child = (TRfd *)Child_List->Items[Child_List_Index];
        next_child->Retry_Counter = 0;
        SendDiscoverDescendantsCommand(next_child);
    }
    else
    {
        // There are no more children, so our descendant discovery is complete
        if ( Parent_Node != NULL )
        {
            SendDescendantDiscoveryCompleteMsg(Parent_Node);
        }
    }
} // End of HandleDescendantDiscoveryDoneMsg

