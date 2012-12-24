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
    Coordinator->Node_Body->Parent = this;
    Coordinator->Node_Body->Left = lvLog->Width;
    Coordinator->Node_Body->Top = panelTools->Height;

    Coordinator->Node_Range->Parent = this;
    Coordinator->Node_Range->Left =
        Coordinator->Node_Body->Left - ((Coordinator->Node_Range->Width - Coordinator->Node_Body->Width) / 2);
    Coordinator->Node_Range->Top =
        Coordinator->Node_Body->Top - ((Coordinator->Node_Range->Height - Coordinator->Node_Body->Height) / 2);

    Coordinator->Node_Label->Caption = "0";
    Coordinator->Node_Label->Left =
        Coordinator->Node_Body->Left + ((Coordinator->Node_Body->Width - Coordinator->Node_Label->Width) / 2);
    Coordinator->Node_Label->Top =
        Coordinator->Node_Body->Top + ((Coordinator->Node_Body->Height - Coordinator->Node_Label->Height) / 2);;
    Coordinator->Node_Label->Parent = this;
    Coordinator->Node_Label->BringToFront();
    Node_List->Add(Coordinator);







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


void __fastcall TformMain::shNodeMouseDown(TObject *Sender, TMouseButton /*Button*/, TShiftState /*Shift*/, int X, int Y)
{
    // Find which shape caused the event
    TRfd * node = NULL;
    bool found = false;
    sint32 i = 0;
    while ( ! found && (i < Node_List->Count) )
    {
        node = (TRfd *)Node_List->Items[i++];
        found = ( (node->Node_Body == Sender) || (node->Node_Label == Sender) );
    } // End of Node LIst scan loop

    if ( found  )
    {
        Node_Being_Dragged = node;
        Drag_Start_X = X;
        Drag_Start_Y = Y;

        // Show the extent of the node's transmit range
        node->Node_Range->Visible = true;
        node->Node_Range->SendToBack();
    } // End of Node found in list
} // End of shNodeMouseDown


void __fastcall TformMain::shNodeMouseMove(TObject * /*Sender*/, TShiftState /*Shift*/, int X, int Y)
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
        Node_Being_Dragged->Node_Range->Left =
            Node_Being_Dragged->Node_Body->Left - (Node_Being_Dragged->Tx_Range - DEFAULT_NODE_SIZE) / 2;
        Node_Being_Dragged->Node_Range->Top =
            Node_Being_Dragged->Node_Body->Top - (Node_Being_Dragged->Tx_Range - DEFAULT_NODE_SIZE) / 2;
        Node_Being_Dragged->Node_Label->Left = Node_Being_Dragged->Node_Body->Left +
            ((Node_Being_Dragged->Node_Body->Width - Node_Being_Dragged->Node_Label->Width) / 2);
        Node_Being_Dragged->Node_Label->Top = Node_Being_Dragged->Node_Body->Top +
            ((Node_Being_Dragged->Node_Body->Height - Coordinator->Node_Label->Height) / 2);;
    } // End of null pointer check
} // End of shNodeMouseMove


void __fastcall TformMain::shNodeMouseUp(TObject * /*Sender*/, TMouseButton /*Button*/, TShiftState /*Shift*/, int /*X*/, int /*Y*/)
{
    if ( Node_Being_Dragged != NULL )
    {
        Node_Being_Dragged->Node_Range->Visible = false;
        Node_Being_Dragged = NULL;
    }
} // End of shNodeMouseUp

