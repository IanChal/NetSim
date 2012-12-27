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
    CommonConstructorTasks();
} // End of constructor


void TRouter::CommonConstructorTasks(void)
{
    // Network Stuff
    Node_Type = NT_ROUTER;

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
	// Delete the child list and its contents (but *not* the object to which they point)
	while ( Child_List->Count > 0 )
	{
		Child_List->Delete(0);
	}
	delete Child_List;
} // End of destructor


void TRouter::DiscoverChildren(void)
{
TRadioMsg msg(2);

    msg.Timestamp = 0;                      // Put timestamp here
    msg.Recipient = MSG_RECIPIENT_ALL_NODES;
    msg.Sender = MAC_Address;
    msg.Msg_Data[0] = MT_JOIN_NETWORK_REQ;
    msg.Msg_Data[1] = Cluster_Level + 1;
	radioManager.TransmitMsg(&msg, this);
} // End of DiscoverChildren

