/*---------------*/
/* Include Files */
/*---------------*/

#pragma hdrstop

#include "TCoordinator.h"

#pragma package(smart_init)


/*-------------------*/
/* Macro Definitions */
/*-------------------*/

#define COORDINATOR_COLOUR                  (clBlue)


/*-----------------------------------*/
/* Methods of the TCoordinator class */
/*-----------------------------------*/

TCoordinator::TCoordinator(TComponent * owner) : TRouter(owner)
{
    // Network Stuff
    Node_Type = NT_COORDINATOR;
    Cluster_Level = 0;
    Tx_Range = 100;

    // Visual Stuff
    Node_Body->Pen->Color = COORDINATOR_COLOUR;
    Node_Body->Brush->Color = COORDINATOR_COLOUR;
    Node_Range->Pen->Color = COORDINATOR_COLOUR;
    Node_Label->Color = COORDINATOR_COLOUR;
    Node_Label->Font->Color = clWhite;

    // The coordinator does not need a pop-up menu
    Node_Label->PopupMenu = NULL;
} // End of constructor


void TCoordinator::DiscoverChildren(void)
{
    Retry_Counter = 0;

} // End of DiscoverChildren
