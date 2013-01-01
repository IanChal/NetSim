#ifndef TRadioManagerH
#define TRadioManagerH

/*---------------*/
/* Include Files */
/*---------------*/

#include <vcl.h>

#include "TRfd.h"

/*-----------------*/
/* Data Structures */
/*-----------------*/

/*---------------------------------------*/
/* Definition of the TRadioManager class */
/*---------------------------------------*/
class TRadioMsg;
class TRfd;
class TRadioManager
{
public:
    // Public Data

    // Public Methods
	void TransmitMsg(TRadioMsg * msg, TRfd * sender);
    
}; // End of TRadioManager class definition


/*------------------------------*/
/* Global data for external use */
/*------------------------------*/

extern TRadioManager radioManager;


#endif
