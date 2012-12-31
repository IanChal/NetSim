#ifndef TCoordinatorH
#define TCoordinatorH

/*---------------*/
/* Include Files */
/*---------------*/

#include "TRouter.h"
#include "globals.h"


/*-------------------*/
/* Macro Definitions */
/*-------------------*/


/*-----------------*/
/* Data Structures */
/*-----------------*/

/*--------------------------------------*/
/* Definition of the TCoordinator class */
/*--------------------------------------*/
class TCoordinator : public TRouter
{

public:
    // Public Data

    // Public Methods
    TCoordinator(TComponent * owner);
    void DiscoverChildren(void);
}; // End of TCoordinator class definition





#endif
