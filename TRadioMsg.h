#ifndef TRadioMsgH
#define TRadioMsgH

/*---------------*/
/* Include Files */
/*---------------*/

#include "globals.h"


/*-------------------*/
/* Macro definitions */
/*-------------------*/

#define MSG_RECIPIENT_ALL_NODES             (0xFFFF)


/*-----------------*/
/* Data Structures */
/*-----------------*/

/*-----------------------------------*/
/* Definition of the TRadioMsg class */
/*-----------------------------------*/
class TRadioMsg
{
public:
    // Public data
    uint32 Timestamp;
    sint64 Sender;
    sint64 Recipient;
    uint8 Msg_Length;
    uint8 * Msg_Data;

    // Public Methods
    TRadioMsg(uint32 msg_len);
    ~TRadioMsg(void);
}; // End of TRadioMsg class definition

#endif
