#ifndef TRouterH
#define TRouterH

/*---------------*/
/* Include Files */
/*---------------*/

#include "TRfd.h"
#include "globals.h"


/*-------------------*/
/* Macro Definitions */
/*-------------------*/


/*-----------------*/
/* Data Structures */
/*-----------------*/


/*---------------------------------*/
/* Definition of the TRouter class */
/*---------------------------------*/
class TRouter : public TRfd
{
private:
    // Private Data

    // Private Methods
    void CommonConstructorTasks(TComponent * owner);


protected:
    // Protected Data
    TTimer * Msg_Timeout_Timer;
    uint8 Retry_Counter;

    // Protected Methods
    void __fastcall Msg_Timeout_TimerTimer(TObject *Sender);

public:
    // Public Data
    bool Child_Added;
    TList * Child_List;                     // List of child nodes. Type can be of any type derived from TRfd
    sint32 Child_List_Index;

    // Public Methods
    TRouter(TComponent * owner);
    ~TRouter(void);
}; // End of the TRouter class definition



#endif
