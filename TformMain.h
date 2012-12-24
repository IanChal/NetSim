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
    TCheckBox *CheckBox1;
    TListView *lvLog;
    TButton *Button1;
    TButton *Button2;
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
    void __fastcall btnPwrClick(TObject *Sender);
    void __fastcall shNodeMouseDown(TObject *Sender, TMouseButton Button, TShiftState Shift, int X, int Y);
    void __fastcall shNodeMouseUp(TObject *Sender, TMouseButton Button, TShiftState Shift, int X, int Y);
    void __fastcall shNodeMouseMove(TObject *Sender, TShiftState Shift, int X, int Y);

private:
    // Private Data

	// Add some fixed nodes as a temporary measure
	TRouter * R1;
	TRouter * R2;
	TRouter * R3;
	TRouter * R4;
	TRouter * R5;
	TRouter * R6;
	TRouter * R7;

    // Node-dragging stuff
    TRfd * Node_Being_Dragged;
    sint32 Drag_Start_X;
    sint32 Drag_Start_Y;

	// Private Methods

public:
	// Public Data
	TRadioManager Radio_Manager;
    TList * Node_List;                      // List of Node - type can be any class derived from TRfd
	TCoordinator * Coordinator;             // The network must have a coordinator

	// Public Methods

	__fastcall TformMain(TComponent* Owner);
	__fastcall ~TformMain(void);
};


extern PACKAGE TformMain *formMain;


#endif
