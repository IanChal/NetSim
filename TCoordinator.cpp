/*---------------*/
/* Include Files */
/*---------------*/

#pragma hdrstop

#include "TCoordinator.h"


#pragma package(smart_init)


/*-----------------------------------*/
/* Methods of the TCoordinator class */
/*-----------------------------------*/

TCoordinator::TCoordinator(TComponent * owner) : TRouter(owner)
{
    Node_Type = NT_COORDINATOR;
} // End of constructor

