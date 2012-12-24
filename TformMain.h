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
    TShape *shNode2;
    TShape *shNode3;
    TShape *shNode1;
    TShape *shNode4;
    TShape *shNode5;
    TShape *shNode6;
    TShape *shNode7;
    TPanel *panelTools;
    TButton *btnPwr;
    TShape *shCoord;
    TShape *shTxRange;
    TCheckBox *CheckBox1;
    TListView *lvLog;
    TLabel *Label1;
    void __fastcall btnPwrClick(TObject *Sender);
    void __fastcall shCoordMouseDown(TObject *Sender, TMouseButton Button, TShiftState Shift, int X, int Y);
    void __fastcall shCoordMouseUp(TObject *Sender, TMouseButton Button, TShiftState Shift, int X, int Y);
    void __fastcall shCoordMouseMove(TObject *Sender, TShiftState Shift, int X, int Y);

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
