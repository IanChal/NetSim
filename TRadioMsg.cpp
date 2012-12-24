/*---------------*/
/* Include Files */
/*---------------*/

#pragma hdrstop

#include "TRadioMsg.h"


TRadioMsg::TRadioMsg(uint32 msg_len)
{
    Msg_Length = msg_len;
    Msg_Data = new uint8[msg_len];
} // End of constructor


TRadioMsg::~TRadioMsg(void)
{
    delete Msg_Data;
} // End of destructor



#pragma package(smart_init)
