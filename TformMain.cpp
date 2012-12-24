/*---------------*/
/* Include Files */
/*---------------*/

#include <vcl.h>
#pragma hdrstop

#include "TCoordinator.h"
#include "TformMain.h"

#pragma package(smart_init)
#pragma resource "*.dfm"

TformMain *formMain;


/*--------------------------------*/
/* Methods of the TformMain class */
/*--------------------------------*/

__fastcall TformMain::TformMain(TComponent* Owner): TForm(Owner)
{
	Node_List = new TList;
    Node_Being_Dragged = NULL;

	Coordinator = new TCoordinator(this);
	Coordinator->Node_Type = NT_COORDINATOR;
    Coordinator->Cluster_Level = 0;
    Coordinator->MAC_Address = 0;
    Node_List->Add(Coordinator);







    // Add the Nodes already on the screen to the list
	R1 = new TRouter(this);
	R1->Node_Type = NT_ROUTER;
    R1->Cluster_Level = CLUSTER_LEVEL_UNKNOWN;
    R1->MAC_Address = 1;
    R1->Node_Body = shNode1;
    R1->Tx_Range = 200;
    Node_List->Add(R1);

	R2 = new TRouter(this);
	R2->Node_Type = NT_ROUTER;
    R2->Cluster_Level = CLUSTER_LEVEL_UNKNOWN;
    R2->MAC_Address = 2;
    R2->Node_Body = shNode2;
    R2->Tx_Range = 200;
    Node_List->Add(R2);

	R3 = new TRouter(this);
	R3->Node_Type = NT_ROUTER;
    R3->Cluster_Level = CLUSTER_LEVEL_UNKNOWN;
    R3->MAC_Address = 3;
    R3->Node_Body = shNode3;
    R3->Tx_Range = 200;
    Node_List->Add(R3);

	R4 = new TRouter(this);
	R4->Node_Type = NT_ROUTER;
    R4->Cluster_Level = CLUSTER_LEVEL_UNKNOWN;
    R4->MAC_Address = 4;
    R4->Node_Body = shNode4;
    R4->Tx_Range = 200;
    Node_List->Add(R4);

	R5 = new TRouter(this);
	R5->Node_Type = NT_ROUTER;
    R5->Cluster_Level = CLUSTER_LEVEL_UNKNOWN;
    R5->MAC_Address = 5;
    R5->Node_Body = shNode5;
    R5->Tx_Range = 200;
    Node_List->Add(R5);

	R6 = new TRouter(this);
	R6->Node_Type = NT_ROUTER;
    R6->Cluster_Level = CLUSTER_LEVEL_UNKNOWN;
    R6->MAC_Address = 6;
    R6->Node_Body = shNode6;
    R6->Tx_Range = 200;
    Node_List->Add(R6);

	R7 = new TRouter(this);
	R7->Node_Type = NT_ROUTER;
    R7->Cluster_Level = CLUSTER_LEVEL_UNKNOWN;
    R7->MAC_Address = 7;
    R7->Node_Body = shNode7;
    R7->Tx_Range = 200;
    Node_List->Add(R7);
} // End of constructor


__fastcall TformMain::~TformMain(void)
{
    // Delete the Node List and its contents
    while ( Node_List->Count > 0 )
    {
        TRfd * node = (TRfd *)Node_List->First();
        delete node;
        Node_List->Delete(0);
    }
    delete Node_List;
} // End of constructor

void __fastcall TformMain::btnPwrClick(TObject * /*Sender*/)
{
	// At power-on, start the network formation.
	Coordinator->DiscoverChildren();
}
//---------------------------------------------------------------------------



void __fastcall TformMain::shCoordMouseDown(TObject *Sender, TMouseButton /*Button*/, TShiftState /*Shift*/, int X, int Y)
{
    // Find which shape caused the event
    TRfd * node = NULL;
    TShape * sender = (TShape *)Sender;
    bool found = false;
    sint32 i = 0;
    while ( ! found && (i < Node_List->Count) )
    {
        node = (TRfd *)Node_List->Items[i++];
        found = ( node->Node_Body == sender );
    } // End of Node LIst scan loop

    if ( found  )
    {
        Node_Being_Dragged = node;
        Drag_Start_X = X;
        Drag_Start_Y = Y;

        // Show the extent of the node's transmit range
        sint32 delta = (node->Tx_Range - DEFAULT_NODE_SIZE) / 2;
        shTxRange->Width = node->Tx_Range;
        shTxRange->Height = node->Tx_Range;
        shTxRange->Left = sender->Left - delta;
        shTxRange->Top = sender->Top - delta;
        shTxRange->Visible = true;
        shTxRange->SendToBack();
    } // End of Node found in list
} // End of shCoordMouseDown


void __fastcall TformMain::shCoordMouseMove(TObject * /*Sender*/, TShiftState /*Shift*/, int X, int Y)
{
    if ( Node_Being_Dragged != NULL )
    {
        sint32 dx = X - Drag_Start_X;
        sint32 dy = Y - Drag_Start_Y;
        Node_Being_Dragged->Node_Body->Left += dx;
        Node_Being_Dragged->Node_Body->Top += dy;
        if ( Node_Being_Dragged->Node_Body->Left < lvLog->Width )
        {
            Node_Being_Dragged->Node_Body->Left = lvLog->Width;
        }
        if ( Node_Being_Dragged->Node_Body->Left > formMain->ClientWidth - Node_Being_Dragged->Node_Body->Width )
        {
            Node_Being_Dragged->Node_Body->Left = formMain->ClientWidth - Node_Being_Dragged->Node_Body->Width;
        }
        if ( Node_Being_Dragged->Node_Body->Top < panelTools->Height )
        {
            Node_Being_Dragged->Node_Body->Top = panelTools->Height;
        }
        if ( Node_Being_Dragged->Node_Body->Top > formMain->ClientHeight - Node_Being_Dragged->Node_Body->Height )
        {
            Node_Being_Dragged->Node_Body->Top = formMain->ClientHeight - Node_Being_Dragged->Node_Body->Height;
        }
        shTxRange->Left = Node_Being_Dragged->Node_Body->Left - (Node_Being_Dragged->Tx_Range - DEFAULT_NODE_SIZE) / 2;
        shTxRange->Top = Node_Being_Dragged->Node_Body->Top - (Node_Being_Dragged->Tx_Range - DEFAULT_NODE_SIZE) / 2;
    } // End of null pointer check
} // End of shCoordMouseMove


void __fastcall TformMain::shCoordMouseUp(TObject * /*Sender*/, TMouseButton /*Button*/, TShiftState /*Shift*/, int /*X*/, int /*Y*/)
{
    Node_Being_Dragged = NULL;
    shTxRange->Visible = false;
} // End of shCoordMouseUp

