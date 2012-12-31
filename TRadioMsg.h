#ifndef TRadioMsgH
#define TRadioMsgH

/*---------------*/
/* Include Files */
/*---------------*/

#include "globals.h"
#include "TRfd.h"

/*-------------------*/
/* Macro definitions */
/*-------------------*/

#define MSG_RECIPIENT_ALL_NODES             (NULL)


/*-----------------*/
/* Data Structures */
/*-----------------*/

class TRfd;                                 // Forward reference

/*-----------------------------------*/
/* Definition of the TRadioMsg class */
/*-----------------------------------*/
class TRadioMsg
{
public:
    // Public data
    uint32 Timestamp;
    TRfd * Sender_Node;
    TRfd * Recipient_Node;
    uint8 Msg_Length;
    uint8 * Msg_Data;

    // Public Methods
    TRadioMsg(uint32 msg_len);
    ~TRadioMsg(void);
    TRadioMsg(TRadioMsg &ref);
}; // End of TRadioMsg class definition

#endif
