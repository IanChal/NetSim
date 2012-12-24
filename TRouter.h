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
protected:
    // Protected Data
    TList * Child_List;                     // List of child nodes. Type can be of any type derived from TRfd

    // Protected Methods
    

public:
    // Public Data

    // Public Methods
    TRouter(TComponent * owner);
    ~TRouter(void);
    void DiscoverChildren(void);

}; // End of the TRouter class definition



#endif
