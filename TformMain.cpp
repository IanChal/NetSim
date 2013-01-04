/*---------------*/
/* Include Files */
/*---------------*/

#include <vcl.h>
#include <new.h>
#include <inifiles.hpp>
#pragma hdrstop

#include "TformMain.h"
#include "TCoordinator.h"
#include "TRadioMsg.h"

#pragma package(smart_init)
#pragma resource "*.dfm"

TformMain *formMain;


/*-------------------*/
/* Macro definitions */
/*-------------------*/

#define APPLICATION_NAME                    "Network Simulator"


/*--------------------------------*/
/* Methods of the TformMain class */
/*--------------------------------*/

__fastcall TformMain::TformMain(TComponent* Owner): TForm(Owner)
{
	Node_List = new TList;
    Node_Being_Dragged = NULL;
    Total_Node_Count = 0;
    Power_Is_On = false;
    Network_Filename = "";
    Network_Changed = false;
    Caption = (AnsiString)APPLICATION_NAME + " - (Unnamed Network)";
} // End of constructor


void __fastcall TformMain::FormCreate(TObject * /*Sender*/)
{
    LoadNetworkConfig(INI_FILENAME);
} // End of FormCreate


void __fastcall TformMain::FormClose(TObject * /*Sender*/, TCloseAction &Action)
{
    if ( !Network_Filename.IsEmpty() && Network_Changed )
    {
        AnsiString msg = "Changes to the network configuration have not been changed.\nSave Changes now?";
        TModalResult result = MessageDlg(msg, mtConfirmation, TMsgDlgButtons() << mbYes << mbNo << mbCancel, 0);
        if ( result == mrYes )
        {
            menuSave->Click();
        }
        else if ( result == mrCancel )
        {
            Action = caNone;
        }
    } // End of Network changed check

    // Regardless of changes, save the current configuratino for restore on next startup
    SaveNetworkConfig(INI_FILENAME);
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


void __fastcall TformMain::btnClearLogClick(TObject * /*Sender*/)
{
    lvLog->Clear();
} // End of btnClearLogClick


void __fastcall TformMain::btnNewRouterClick(TObject * /*Sender*/)
{
    TRouter * r = new TRouter(this);
    r->Node_Body->Parent = this;
    r->Node_Range->Parent = this;
    r->Node_Label->Parent = this;
    r->MAC_Address = Total_Node_Count;
    r->Node_Label->Caption = FormatFloat("00", r->MAC_Address);
    r->Node_Label->BringToFront();
    r->DrawNode(lvLog->Width + (r->Node_Body->Width / 2), panelTools->Height + (r->Node_Body->Height / 2));
    Node_List->Add(r);
    Total_Node_Count++;
    if ( ! Network_Changed )
    {
        Network_Changed = true;
        Caption = Caption + "*";
    }
} // End of btnNewRouterClick


void __fastcall TformMain::btnNewZedClick(TObject * /*Sender*/)
{
    TRfd * r = new TRfd(this);
    r->Node_Body->Parent = this;
    r->Node_Range->Parent = this;
    r->Node_Label->Parent = this;
    r->MAC_Address = Total_Node_Count;
    r->Node_Label->Caption = FormatFloat("00", r->MAC_Address);
    r->Node_Label->BringToFront();
    r->DrawNode(lvLog->Width + (r->Node_Body->Width / 2), panelTools->Height + (r->Node_Body->Height / 2));
    Node_List->Add(r);
    Total_Node_Count++;
    if ( ! Network_Changed )
    {
        Network_Changed = true;
        Caption = Caption + "*";
    }
} // End of btnNewZedClick


void __fastcall TformMain::cbShowRangeClick(TObject * /*Sender*/)
{
    for ( sint32 i = 0; i < Node_List->Count; i++ )
    {
        TRfd * node = (TRfd *)Node_List->Items[i];
        node->DrawNode(node->Node_Body->Left + (node->Node_Body->Width / 2),
                       node->Node_Body->Top + (node->Node_Body->Height / 2));
        node->Node_Range->Visible = cbShowRange->Checked;
    }
} // End of cbShowRangeClick


void __fastcall TformMain::btnPwrClick(TObject * /*Sender*/)
{
    if ( Power_Is_On )
    {
        // Switch off the power to the network
        Power_Is_On = false;
        btnPwr->Caption = "Power On";
        for ( sint32 i = 0; i < Node_List->Count; i++ )
        {
            TRfd * node = (TRfd *)Node_List->Items[i];
            if ( i > 0 )
            {
                // Unassign the cluster level of each node, except the coordinator (always 0)
                node->Cluster_Level = CLUSTER_LEVEL_UNKNOWN;
            }
            node->Parent_Node = NULL;
            node->Msg_Timeout_Timer->Enabled = false;
            node->Msg_Timer->Enabled = false;
            node->ClearMessageBuffer();
            if ( (node->Node_Type == NT_ROUTER) || (node->Node_Type == NT_COORDINATOR) )
            {
                TRouter * router_node = (TRouter *)node;
                router_node->Child_List->Clear();
            }
        }
        formMain->Repaint();
    } // End of power is currently on
    else
    {
        // Switch on the power to the network
        if ( Node_List->Count > 0 )
        {
            // At power-on, start the network formation
            TCoordinator * coord = (TCoordinator *)Node_List->First();
            Power_Is_On = true;
            btnPwr->Caption = "Power Off";

            // Start the network build by sending a Deiscover Descendants command from
            // the coordinator to all nodes (that are within transmissino range)
            coord->Retry_Counter = 0;
            coord->SendJoinMyNetworkRequest(MSG_RECIPIENT_ALL_NODES);
        } // End of empty Node List check
    } // End of power is currently off
    btnNewRouter->Enabled = ! Power_Is_On;
    btnNewZed->Enabled = ! Power_Is_On;
} // End of btnPwrClick


void __fastcall TformMain::shNodeMouseDown(TObject *Sender, TMouseButton Button, TShiftState /*Shift*/, int X, int Y)
{
    if ( ! Power_Is_On && (Button == mbLeft) )
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
    } // End of left-button check
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
        n->DrawNode(n->Node_Body->Left + (n->Node_Body->Width / 2), n->Node_Body->Top + (n->Node_Body->Height / 2));
    } // End of null pointer check
} // End of shNodeMouseMove


void __fastcall TformMain::shNodeMouseUp(TObject * /*Sender*/, TMouseButton /*Button*/, TShiftState /*Shift*/, int /*X*/, int /*Y*/)
{
    if ( Node_Being_Dragged != NULL )
    {
        Node_Being_Dragged->Node_Range->Visible = cbShowRange->Checked;
        Node_Being_Dragged = NULL;
    }
    if ( ! Network_Changed )
    {
        Network_Changed = true;
        Caption = Caption + "*";
    }
} // End of shNodeMouseUp


void __fastcall TformMain::menuContextNodePopup(TObject * /*Sender*/)
{
    sint32 i = FindNodeIndex(menuContextNode->PopupComponent);
    menuDeleteNode->Enabled = ( i > 0 );
} // End of menuContextNodePopup


void __fastcall TformMain::menuDeleteNodeClick(TObject */*Sender*/)
{
    sint32 i = FindNodeIndex(menuContextNode->PopupComponent);
    if ( i > 0 )
    {
        TRfd * node = (TRfd *)Node_List->Items[i];
        delete node;
        Node_List->Delete(i);
        if ( ! Network_Changed )
        {
            Network_Changed = true;
            Caption = Caption + "*";
        }
    }
} // End of menuDeleteNodeClick


sint32 TformMain::FindNodeIndex(TObject * sender)
{
    sint32 node_index = -1;
    sint32 i = 0;
    while ( (node_index < 0) && (i < Node_List->Count) )
    {
        TRfd * node = (TRfd *)Node_List->Items[i];
        if ( node->Node_Label == sender )
        {
            node_index = i;
        }
        else
        {
            i++;
        }
    } // End of Node List scan loop

    return node_index;
} // End of FindSender



void __fastcall TformMain::FormPaint(TObject * /*Sender*/)
{
    for ( sint32 i = 0; i < Node_List->Count; i++ )
    {
        TRfd * node = (TRfd *)Node_List->Items[i];
        if ( node->Parent_Node != NULL )
        {
            // Draw a line from the child to the parent to vosualise the relatinoship
            sint32 child_x = SHAPE_CENTRE_X(node->Node_Body);
            sint32 child_y = SHAPE_CENTRE_Y(node->Node_Body);
            sint32 parent_x = SHAPE_CENTRE_X(node->Parent_Node->Node_Body);
            sint32 parent_y = SHAPE_CENTRE_Y(node->Parent_Node->Node_Body);
            formMain->Canvas->Pen->Width = 1;
            formMain->Canvas->Pen->Color = node->Node_Body->Pen->Color;
            formMain->Canvas->Pen->Style = psDot;
            formMain->Canvas->MoveTo(child_x, child_y);
            formMain->Canvas->LineTo(parent_x, parent_y);
        }
    } // End of node list scan loop
} // End of FormPaint


void TformMain::LoadNetworkConfig(AnsiString filename)
{
TIniFile *ini = new(nothrow) TIniFile(filename);

    if ( (ini != NULL) && FileExists(ini->FileName) )
    {
        sint32 node_count = ini->ReadInteger(INI_SECTION_NAME, "Count", 0);
        for ( sint32 i = 0; i < node_count; i++ )
        {
            TRfd * new_node;
            try
            {
                TNodeType type = TNodeType(ini->ReadInteger(INI_SECTION_NAME, "Type"+IntToStr(i), NT_UNKNOWN));
                sint32 x_pos = ini->ReadInteger(INI_SECTION_NAME, "PosX"+IntToStr(i), lvLog->Width) + (DEFAULT_NODE_SIZE / 2);
                sint32 y_pos = ini->ReadInteger(INI_SECTION_NAME, "PosY"+IntToStr(i), panelTools->Height) + (DEFAULT_NODE_SIZE / 2);
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
                        Total_Node_Count++;
                        break;
                    }

                    case NT_ROUTER:
                    {
                        TRouter * r = new TRouter(this);
                        r->Node_Body->Parent = this;
                        r->Node_Range->Parent = this;
                        r->Node_Label->Parent = this;
                        r->MAC_Address = Total_Node_Count;
                        r->Node_Label->Caption = FormatFloat("00", r->MAC_Address);
                        r->Node_Label->BringToFront();
                        r->DrawNode(x_pos, y_pos);
                        Node_List->Add(r);
                        Total_Node_Count++;
                        break;
                    }

                    case NT_END_DEVICE:
                    {
                        TRfd * r = new TRfd(this);
                        r->Node_Body->Parent = this;
                        r->Node_Range->Parent = this;
                        r->Node_Label->Parent = this;
                        r->MAC_Address = Total_Node_Count;
                        r->Node_Label->Caption = FormatFloat("00", r->MAC_Address);
                        r->Node_Label->BringToFront();
                        r->DrawNode(x_pos, y_pos);
                        Node_List->Add(r);
                        Total_Node_Count++;
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
        Total_Node_Count++;
    } // End of empty Node List check
} // End of LoadNetworkConfig


void TformMain::SaveNetworkConfig(AnsiString filename)
{
TIniFile *ini = new(nothrow) TIniFile(filename);
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
} // End of SaveNetworkConfig

void __fastcall TformMain::menuExitClick(TObject * /*Sender*/)
{
    Close();
} // End of menuExitClick


void __fastcall TformMain::menuLoadClick(TObject * /*Sender*/)
{
    TModalResult result = mrNone;

    if ( !Network_Filename.IsEmpty() && Network_Changed )
    {
        AnsiString msg = "Changes to the network configuration have not been changed.\nSave Changes now?";
        result = MessageDlg(msg, mtConfirmation, TMsgDlgButtons() << mbYes << mbNo << mbCancel, 0);
        if ( result == mrYes )
        {
            menuSave->Click();
        }
    } // End of Network changed check

    if ( (result != mrCancel) && dialogOpen->Execute() )
    {
        // Delete all nodes in the list before loading new ones
        while ( Node_List->Count > 0 )
        {
            TRfd * node = (TRfd *)Node_List->First();
            delete node;
            Node_List->Delete(0);
        }
        Total_Node_Count = 0;
        LoadNetworkConfig(dialogOpen->FileName);
        Network_Filename = dialogOpen->FileName;
        formMain->Caption = (AnsiString)APPLICATION_NAME + " - " + Network_Filename;
        Network_Changed = false;
    } // End of ok clicked
} // End of menuLoadClick


void __fastcall TformMain::menuSaveClick(TObject *Sender)
{
    if ( Network_Filename.IsEmpty() )
    {
        // File does not yet have a name, so prompt for one
        menuSaveAsClick(Sender);
    }
    else
    {
        // File already has a name, so use it
        SaveNetworkConfig(Network_Filename);
    }
} // End of menuSaveClick


void __fastcall TformMain::menuSaveAsClick(TObject * /*Sender*/)
{
    if ( dialogSave->Execute() )
    {
        SaveNetworkConfig(dialogSave->FileName);
        Network_Filename = dialogSave->FileName;
        formMain->Caption = (AnsiString)APPLICATION_NAME + " - " + Network_Filename;
    } // End of OK button pressed
} // End of menuSaveAsClick

