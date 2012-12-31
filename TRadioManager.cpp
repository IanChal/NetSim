/*---------------*/
/* Include Files */
/*---------------*/

#pragma hdrstop

#include "TformMain.h"
#include "TRadioManager.h"

#pragma package(smart_init)


TRadioManager radioManager;


void TRadioManager::TransmitMsg(TRadioMsg * msg, TRfd * sender)
{
    TListItem * new_item = formMain->lvLog->Items->Add();
    new_item->Caption = IntToHex(msg->Sender_Node->MAC_Address, 4);

    new_item->SubItems->Add( (msg->Recipient_Node == MSG_RECIPIENT_ALL_NODES) ?
                              (AnsiString)"<ALL>" : (AnsiString)IntToHex(msg->Recipient_Node->MAC_Address, 4) );
    AnsiString data;
    for ( sint32 i = 0; i < msg->Msg_Length; i++ )
    {
        data += IntToHex(msg->Msg_Data[i], 2) + " ";
    }
    new_item->SubItems->Add(data);

    // Transmit the message to all nodes that match the recipient address and are within transmission range
    const sint32 sender_pos_x = sender->Node_Body->Left + (sender->Node_Body->Width / 2);
    const sint32 sender_pos_y = sender->Node_Body->Top + (sender->Node_Body->Height / 2);
    for (sint32 i = 0; i < formMain->Node_List->Count; i++ )
    {
        TRfd * rx_node = (TRfd *)formMain->Node_List->Items[i];
        if ( (rx_node->MAC_Address != msg->Sender_Node->MAC_Address) &&
             ((msg->Recipient_Node == rx_node) || (msg->Recipient_Node == MSG_RECIPIENT_ALL_NODES)) )
        {
            // The address matches. Now check the range
            const sint32 rx_node_pos_x = rx_node->Node_Body->Left + (rx_node->Node_Body->Width / 2);
            const sint32 rx_node_pos_y = rx_node->Node_Body->Top + (rx_node->Node_Body->Height / 2);
            const sint32 dx = rx_node_pos_x - sender_pos_x;
            const sint32 dy = rx_node_pos_y - sender_pos_y;
            const sint32 dx2 = dx * dx;
            const sint32 dy2 = dy * dy;
            const sint32 node_dist_sq = dx2 + dy2;
            if ( node_dist_sq <= (sender->Tx_Range * sender->Tx_Range) )
            {
                // Create a copy of the message to send to each recipient
                TRadioMsg * msg_copy = new TRadioMsg(*msg);
                rx_node->Msg_Buffer->Add(msg_copy);

                // Leave a random time before the node reads the message.
                // This simulates latency and allows different nodes to answer braodcast messages in different orders
                if ( ! rx_node->Msg_Timer->Enabled )
                {
                    rx_node->Msg_Timer->Interval = Random(80) + 20;
                    rx_node->Msg_Timer->Enabled = true;
                }
            } // end of Tx range check
        } // End of address check
    } // End of node list scan loop
} // End of TransmitMsg

