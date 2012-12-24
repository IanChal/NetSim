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
    // Network Stuff
    Node_Type = NT_COORDINATOR;
    Cluster_Level = 0;
    MAC_Address = 0;

    // Visual Stuff
    this->Node_Body->Pen->Color = clGreen;
    this->Node_Body->Brush->Color = clGreen;
    this->Node_Range->Pen->Color = clGreen;
    this->Node_Label->Color = clGreen;
} // End of constructor

