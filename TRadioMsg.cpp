/*---------------*/
/* Include Files */
/*---------------*/

#pragma hdrstop

#include "TRadioMsg.h"


TRadioMsg::TRadioMsg(uint16 msg_len)
{
    Msg_Length = msg_len;
    Msg_Data = new uint8[msg_len];
    Timestamp = 0;                          // TODO: Put correct timestamp here
} // End of constructor


TRadioMsg::~TRadioMsg(void)
{
    // Get rid of any unprocessed messages
    delete[] Msg_Data;
} // End of destructor


TRadioMsg::TRadioMsg(TRadioMsg &ref)
{
    Timestamp = ref.Timestamp;
    Sender_Node = ref.Sender_Node;
    Recipient_Node = ref.Recipient_Node;
    Msg_Length = ref.Msg_Length;
    Msg_Data = new uint8[Msg_Length];
    for ( sint32 i = 0; i < Msg_Length; i++ )
    {
        Msg_Data[i] = ref.Msg_Data[i];
    }
} // End of copy constructor


#pragma package(smart_init)
