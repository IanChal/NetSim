#ifndef TformMainH
#define TformMainH


/*---------------*/
/* Include Files */
/*---------------*/

#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ExtCtrls.hpp>

#include "TRadioManager.h"
#include "TRouter.h"
#include "TCoordinator.h"
#include <ComCtrls.hpp>
#include <Menus.hpp>


/*-------------------*/
/* Macro Definitions */
/*-------------------*/

#define NODE_SIZE_STD                       (24)
#define NODE_SIZE_EXT                       (128)


/*-----------------*/
/* Data Structures */
/*-----------------*/

/*-----------------------------------*/
/* Definition of the TformMain class */
/*-----------------------------------*/
class TformMain : public TForm
{
__published:	// IDE-managed Components
    TPanel *panelTools;
    TButton *btnPwr;
    TCheckBox *cbShowRange;
    TListView *lvLog;
    TButton *btnNewRouter;
    TButton *btnNewZed;
    TMainMenu *menuMain;
    TMenuItem *menuFile;
    TMenuItem *N1;
    TMenuItem *menuExit;
    TMenuItem *menuSave;
    TMenuItem *menuSaveAs;
    TMenuItem *menuLoad;
    TMenuItem *menuNetwork;
    TMenuItem *menuAddRouter;
    TMenuItem *menuAddEndDevice;
    TMenuItem *menuRemoveEndDevice;
    TMenuItem *menuRemoveRouter;
    TMenuItem *menuHelp;
    TMenuItem *About1;
    TPopupMenu *menuContextNode;
    TMenuItem *menuDeleteNode;
    TTimer *Timer1;
    TButton *btnClearLog;
    void __fastcall btnPwrClick(TObject *Sender);
    void __fastcall shNodeMouseDown(TObject *Sender, TMouseButton Button, TShiftState Shift, int X, int Y);
    void __fastcall shNodeMouseUp(TObject *Sender, TMouseButton Button, TShiftState Shift, int X, int Y);
    void __fastcall shNodeMouseMove(TObject *Sender, TShiftState Shift, int X, int Y);
    void __fastcall btnNewRouterClick(TObject *Sender);
    void __fastcall cbShowRangeClick(TObject *Sender);
    void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
    void __fastcall FormCreate(TObject *Sender);
    void __fastcall btnNewZedClick(TObject *Sender);
    void __fastcall menuContextNodePopup(TObject *Sender);
    void __fastcall menuDeleteNodeClick(TObject *Sender);
    void __fastcall btnClearLogClick(TObject *Sender);

private:
    // Private Data
    sint32 Total_Node_Count;

    // Node-dragging stuff
    TRfd * Node_Being_Dragged;
    sint32 Drag_Start_X;
    sint32 Drag_Start_Y;

	// Private Methods
    sint32 FindNodeIndex(TObject * sender);

public:
	// Public Data
	TRadioManager Radio_Manager;
    TList * Node_List;                      // List of Node - type can be any class derived from TRfd

	// Public Methods

	__fastcall TformMain(TComponent* Owner);
	__fastcall ~TformMain(void);
};


extern PACKAGE TformMain *formMain;


#endif
