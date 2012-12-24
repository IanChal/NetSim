#ifndef TRfdH
#define TRfdH

/*---------------*/
/* Include Files */
/*---------------*/

#include <vcl.h>
#include "globals.h"
#include "TRadioMsg.h"


/*-------------------*/
/* Macro Definitions */
/*-------------------*/


/*-----------------*/
/* Data Structures */
/*-----------------*/

/*------------------------------*/
/* Definition of the TRfd class */
/*------------------------------*/
class TRfd
{
public:
    // Public Data
    // Visual stuff
    TShape * Node_Body;
    TShape * Node_Range;
    TLabel * Node_Label;

    // Network stuff
    sint64 MAC_Address;
    TNodeType Node_Type;
    uint8 Cluster_Level;
    uint32 Tx_Range;

    // Public Methods
    TRfd(TComponent * owner);
    void DrawNode(sint32 x, sint32 y);

    virtual bool ReceiveRadioMsg(TRadioMsg * msg);


}; // End of the TRfd class definition


#endif
