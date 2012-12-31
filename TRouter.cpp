/*---------------*/
/* Include Files */
/*---------------*/

#pragma hdrstop

#include "TRadioManager.h"
#include "TRouter.h"

#pragma package(smart_init)


/*------------------------------*/
/* Methods of the TRouter class */
/*------------------------------*/

TRouter::TRouter(TComponent * owner) : TRfd(owner)
{
    CommonConstructorTasks(owner);
} // End of constructor


void TRouter::CommonConstructorTasks(TComponent * owner)
{
    // Network Stuff
    Node_Type = NT_ROUTER;

    // Visual Stuff
    Node_Body->Pen->Color = clBlue;
    Node_Body->Brush->Color = clBlue;
    Node_Range->Pen->Color = clBlue;
    Node_Label->Color = clBlue;
    Msg_Timeout_Timer = new TTimer(owner);
    Msg_Timeout_Timer->Enabled = false;
    Msg_Timeout_Timer->OnTimer = Msg_Timeout_TimerTimer;

    // Initialise the child list
    Child_List = new TList;
} // End of CommonConstructorTasks


TRouter::~TRouter(void)
{
	// Delete the child list and its contents (but *not* the object to which they point)
	while ( Child_List->Count > 0 )
	{
		Child_List->Delete(0);
	}
	delete Child_List;
} // End of destructor


void __fastcall TRouter::Msg_Timeout_TimerTimer(TObject * /*Sender*/)
{
    Msg_Timeout_Timer->Enabled = false;
    if ( Child_Added )
    {
        Retry_Counter = 0;
    }
    else
    {
        Retry_Counter++;
    }

    if ( Retry_Counter < 2 )
    {
        // Retries have not yet expired, so send the 'Join My Network' message again
        SendJoinMyNetworkRequest(NULL);
    }
    else
    {
        // Retries have expired. All possible children have now been discovered.
        // Report back to the parent node that the child Discovery is complete
        if ( Parent_Node != NULL )
        {
            TRadioMsg resp_msg(1);
            resp_msg.Timestamp = 0;                      // Put timestamp here
            resp_msg.Recipient_Node = Parent_Node;
            resp_msg.Sender_Node = this;
            resp_msg.Msg_Data[0] = MT_DESCENDANT_DISCOVERY_DONE_MSG;
            radioManager.TransmitMsg(&resp_msg, this);
        } // End of node has parent

        // Kick off the child discovery of each of the children of this node
        if ( Child_List->Count > 0 )
        {
            Child_List_Index = 0;
            while ( (Child_List_Index < Child_List->Count) &&
                    ((TRfd *)(Child_List->Items[Child_List_Index]))->Node_Type != NT_ROUTER )
            {
                Child_List_Index++;
            }
//            SendDiscoverChildrenCmd((TRouter *)(Child_List->Items[Child_List_Index]));
        }
/*
        else if ((Node_Type == NT_COORDINATOR) || (Node_Type == NT_ROUTER))
        {
            // The coordinator has found all of its direct connections.
            // Now instruct all of the children to find their children
            for ( sint32 i = 0; i < Child_List->Count; i++ )
            {
                TRfd * child = (TRfd *)Child_List->Items[i];
                if ( child->Node_Type == NT_ROUTER )
                {
                    // Send a message to each child router to go and find its children
                    TRouter * child_router = (TRouter *)child;
                    TRadioMsg msg(1);
                    msg.Timestamp = 0;                      // Put timestamp here
                    msg.Recipient_Node = child_router;
                    msg.Sender_Node = this;
                    msg.Msg_Data[0] = MT_DISCOVER_CHILDREN_CMD;
                    Msg_Timeout_Timer->Interval = 1000;
                    Msg_Timeout_Timer->Enabled = true;
	                radioManager.TransmitMsg(&msg, this);
                } // End of node is router
            } // End of child list scan loop
        } // End of node is a coordinator
*/
    } // End of message retries expired
} // End of Msg_Timeout_TimerTimer

