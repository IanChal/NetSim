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

    // Visual Stuff
    Node_Body->Pen->Color = clGreen;
    Node_Body->Brush->Color = clGreen;
    Node_Range->Pen->Color = clGreen;
    Node_Label->Color = clGreen;
} // End of constructor

