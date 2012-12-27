/*---------------*/
/* Include Files */
/*---------------*/

#include <vcl.h>
#include <new.h>
#include <inifiles.hpp>
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
} // End of constructor


void __fastcall TformMain::FormCreate(TObject * /*Sender*/)
{
TIniFile *ini = new(nothrow) TIniFile(INI_FILENAME);

    if ( (ini != NULL) && FileExists(ini->FileName) )
    {
        sint32 node_count = ini->ReadInteger(INI_SECTION_NAME, "Count", 0);
        for ( sint32 i = 0; i < node_count; i++ )
        {
            TRfd * new_node;
            try
            {
                TNodeType type = TNodeType(ini->ReadInteger(INI_SECTION_NAME, "Type"+IntToStr(i), NT_UNKNOWN));
                sint32 x_pos = ini->ReadInteger(INI_SECTION_NAME, "PosX"+IntToStr(i), lvLog->Width + (DEFAULT_NODE_SIZE / 2));
                sint32 y_pos = ini->ReadInteger(INI_SECTION_NAME, "PosY"+IntToStr(i), panelTools->Height + (DEFAULT_NODE_SIZE / 2));
                switch (type)
                {
                    case NT_COORDINATOR:
                    {
                        TCoordinator * c = new TCoordinator(this);
                        c->Node_Body->Parent = this;
                        c->Node_Range->Parent = this;
                        c->Node_Label->Parent = this;
                        c->DrawNode(x_pos, y_pos);
                        c->Node_Label->BringToFront();
                        Node_List->Add(c);
                        break;
                    }

                    case NT_ROUTER:
                    {
                        TRouter * r = new TRouter(this);
                        r->Node_Body->Parent = this;
                        r->Node_Range->Parent = this;
                        r->Node_Label->Parent = this;
                        r->MAC_Address = Node_List->Count;
                        r->Node_Label->Caption = FormatFloat("00", r->MAC_Address);
                        r->Node_Label->BringToFront();
                        r->DrawNode(x_pos, y_pos);
                        Node_List->Add(r);
                        break;
                    }

                    case NT_END_DEVICE:
                    {
                        TRfd * r = new TRfd(this);
                        r->Node_Body->Parent = this;
                        r->Node_Range->Parent = this;
                        r->Node_Label->Parent = this;
                        r->MAC_Address = Node_List->Count;
                        r->Node_Label->Caption = FormatFloat("00", r->MAC_Address);
                        r->Node_Label->BringToFront();
                        r->DrawNode(x_pos, y_pos);
                        Node_List->Add(r);
                        break;
                    }

                    default:
                        // Just ignore any erroneous entries
                        break;
                } // End of switch statement
            }
            catch (...)
            {
            }
        } // End of node list scan loop
        delete ini;
    } // End of ini file checks

    if ( Node_List->Count == 0 )
    {
        // If there's a problem reading the .ini file, just create a coordinator
    	TCoordinator * c = new TCoordinator(this);
        c->Node_Body->Parent = this;
        c->Node_Range->Parent = this;
        c->Node_Label->Parent = this;
        c->DrawNode(lvLog->Width + (c->Node_Body->Width / 2), panelTools->Height + (c->Node_Body->Width / 2));
        c->Node_Label->BringToFront();
        Node_List->Add(c);
    } // End of empty Node List check
} // End of FormCreate


void __fastcall TformMain::FormClose(TObject * /*Sender*/, TCloseAction &/*Action*/)
{
TIniFile *ini = new(nothrow) TIniFile(INI_FILENAME);
    if ( ini != NULL )
    {
        if ( ini->SectionExists(INI_SECTION_NAME) )
        {
            ini->EraseSection(INI_SECTION_NAME);
        }
        ini->WriteInteger(INI_SECTION_NAME, "Count", Node_List->Count);
        for ( sint32 i = 0; i < Node_List->Count; i++ )
        {
            TRfd * node = (TRfd *)Node_List->Items[i];
            ini->WriteInteger(INI_SECTION_NAME, "Type"+IntToStr(i), node->Node_Type);
            ini->WriteInteger(INI_SECTION_NAME, "PosX"+IntToStr(i), node->Node_Body->Left);
            ini->WriteInteger(INI_SECTION_NAME, "PosY"+IntToStr(i), node->Node_Body->Top);

        } // End of node list scan loop
        delete ini;
    } // End of null pointer check
} // End of FormClose


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
} // End of destructor


void __fastcall TformMain::btnPwrClick(TObject * /*Sender*/)
{
    if ( Node_List->Count > 0 )
    {
        // At power-on, start the network formation
        TCoordinator * coord = (TCoordinator *)Node_List->First();
	    coord->DiscoverChildren();
    }
} // End of btnPwrClick


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
        TRfd * n = Node_Being_Dragged;
        sint32 dx = X - Drag_Start_X;
        sint32 dy = Y - Drag_Start_Y;
        n->Node_Body->Left += dx;
        n->Node_Body->Top += dy;
        if ( n->Node_Body->Left < lvLog->Width )
        {
            n->Node_Body->Left = lvLog->Width;
        }
        if ( n->Node_Body->Left > formMain->ClientWidth - n->Node_Body->Width )
        {
            n->Node_Body->Left = formMain->ClientWidth - n->Node_Body->Width;
        }
        if ( n->Node_Body->Top < panelTools->Height )
        {
            n->Node_Body->Top = panelTools->Height;
        }
        if ( n->Node_Body->Top > formMain->ClientHeight - n->Node_Body->Height )
        {
            n->Node_Body->Top = formMain->ClientHeight - n->Node_Body->Height;
        }
        n->Node_Range->Left = n->Node_Body->Left - (n->Tx_Range - DEFAULT_NODE_SIZE) / 2;
        n->Node_Range->Top = n->Node_Body->Top - (n->Tx_Range - DEFAULT_NODE_SIZE) / 2;
        n->Node_Label->Left = n->Node_Body->Left + ((n->Node_Body->Width - n->Node_Label->Width) / 2);
        n->Node_Label->Top = n->Node_Body->Top + ((n->Node_Body->Height - n->Node_Label->Height) / 2);
    } // End of null pointer check
} // End of shNodeMouseMove


void __fastcall TformMain::shNodeMouseUp(TObject * /*Sender*/, TMouseButton /*Button*/, TShiftState /*Shift*/, int /*X*/, int /*Y*/)
{
    if ( Node_Being_Dragged != NULL )
    {
        Node_Being_Dragged->Node_Range->Visible = cbShowRange->Checked;
        Node_Being_Dragged = NULL;
    }
} // End of shNodeMouseUp

void __fastcall TformMain::btnNewRouterClick(TObject * /*Sender*/)
{
    TRouter * r = new TRouter(this);
    r->Node_Body->Parent = this;
    r->Node_Range->Parent = this;
    r->Node_Label->Parent = this;
    r->MAC_Address = Node_List->Count;
    r->Node_Label->Caption = FormatFloat("00", r->MAC_Address);
    r->Node_Label->BringToFront();
    r->DrawNode(lvLog->Width + (r->Node_Body->Width / 2), panelTools->Height + (r->Node_Body->Height / 2));
    Node_List->Add(r);
} // End of btnNewRouterClick


void __fastcall TformMain::cbShowRangeClick(TObject * /*Sender*/)
{
    for ( sint32 i = 0; i < Node_List->Count; i++ )
    {
        TRfd * node = (TRfd *)Node_List->Items[i];
        node->Node_Range->Visible = cbShowRange->Checked;
    }
} // End of cbShowRangeClick

void __fastcall TformMain::btnNewZedClick(TObject *Sender)
{
    TRfd * r = new TRfd(this);
    r->Node_Body->Parent = this;
    r->Node_Range->Parent = this;
    r->Node_Label->Parent = this;
    r->MAC_Address = Node_List->Count;
    r->Node_Label->Caption = FormatFloat("00", r->MAC_Address);
    r->Node_Label->BringToFront();
    r->DrawNode(lvLog->Width + (r->Node_Body->Width / 2), panelTools->Height + (r->Node_Body->Height / 2));
    Node_List->Add(r);
} // End of btnNewZedClick

