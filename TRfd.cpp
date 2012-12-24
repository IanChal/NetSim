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
#include "TRouter.h"


/*---------------------------*/
/* Methods of the TRfd class */
/*---------------------------*/

TRfd::TRfd(TComponent * owner)
{
    /* Generate the visual components */
    Node_Body = new TShape(owner);
    Node_Range = new TShape(owner);
    Node_Label = new TLabel(owner);

    Node_Type = NT_END_DEVICE;
    Cluster_Level = CLUSTER_LEVEL_UNKNOWN;
    Tx_Range = 200;

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







#pragma package(smart_init)
