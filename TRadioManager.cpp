/*---------------*/
/* Include Files */
/*---------------*/

#pragma hdrstop

#include "TformMain.h"
#include "TRadioManager.h"

#pragma package(smart_init)


TRadioManager radioManager;


void TRadioManager::TransmitMsg(TRadioMsg * msg, TRfd * /*sender*/)
{
    TListItem * new_item = formMain->lvLog->Items->Add();
    new_item->Caption = IntToHex(msg->Sender, 4);
    new_item->SubItems->Add(IntToHex(msg->Recipient, 4));
    AnsiString data;
    for ( sint32 i = 0; i < msg->Msg_Length; i++ )
    {
        data += IntToHex(msg->Msg_Data[i], 2) + " ";
    }
    new_item->SubItems->Add(data);
    // TODO: Fire the receive methods in a random order

    // Transmit the message to all nodes that match the recipient address and are within transmission range
    for (sint32 i = 0; i < formMain->Node_List->Count; i++ )
    {
        TRfd * rx_node = (TRfd *)formMain->Node_List->Items[i];
        if ( (rx_node->MAC_Address != msg->Sender) &&
             ((msg->Recipient == rx_node->MAC_Address) || (msg->Recipient == MSG_RECIPIENT_ALL_NODES)) )
        {
            // The address matches. Now check the range


            rx_node->ReceiveRadioMsg(msg);
        }
    }
} // End of TransmitMsg

