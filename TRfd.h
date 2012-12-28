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

protected:
    void __fastcall Msg_TimerTimer(TObject *Sender);

private:
    // Private Data

    // Private Methods
    void CommonConstructor(TComponent * owner);

public:
    // Public Data
    // Visual stuff
    TShape * Node_Body;
    TShape * Node_Range;
    TLabel * Node_Label;
    TTimer * Msg_Timer;
    TList * Msg_Buffer;                     // List of TRadioMsg pointers

    // Network stuff
    sint64 MAC_Address;
    TNodeType Node_Type;
    uint8 Cluster_Level;
    sint32 Tx_Range;

    // Public Methods
    TRfd(TComponent * owner);
    ~TRfd(void);
    void DrawNode(sint32 x, sint32 y);

    // Public Events
    void __fastcall shNodeMouseDown(TObject *Sender, TMouseButton Button, TShiftState Shift, int X, int Y);
    void __fastcall shNodeMouseUp(TObject *Sender, TMouseButton Button, TShiftState Shift, int X, int Y);
    void __fastcall shNodeMouseMove(TObject *Sender, TShiftState Shift, int X, int Y);

}; // End of the TRfd class definition


#endif
