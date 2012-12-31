#ifndef TRfdH
#define TRfdH

/*---------------*/
/* Include Files */
/*---------------*/

#include <vcl.h>
#include "globals.h"
#include "TRadioMsg.h"


/*-------------------*/
/* Macro Definitions */
/*-------------------*/


/*-----------------*/
/* Data Structures */
/*-----------------*/

typedef enum
{
    NS_NODE_IDLE,
    NS_NODE_WAITING_JOINERS
} TNodeStatus;


/*------------------------------*/
/* Definition of the TRfd class */
/*------------------------------*/

class TRfd
{

protected:
    // Protected events
    void __fastcall Msg_TimerTimer(TObject *Sender);

private:
    // Private Data

    // Private Methods
    void CommonConstructor(TComponent * owner);

public:
    // Public Data
    // Visual stuff
    TShape * Node_Body;
    TShape * Node_Range;
    TLabel * Node_Label;
    TTimer * Msg_Timer;                     // Emulates a periodic read of a receive buffer
    TTimer * Msg_Timeout_Timer;             // Used to retry messages if they've not been acknowledged
    TList * Msg_Buffer;                     // List of TRadioMsg pointers

    // Network stuff
    sint64 MAC_Address;
    TRfd * Parent_Node;
    TNodeType Node_Type;
    uint8 Cluster_Level;
    sint32 Tx_Range;
    TNodeStatus Node_State;

    // Public Methods
    TRfd(TComponent * owner);
    ~TRfd(void);
    void DrawNode(sint32 x, sint32 y);
    virtual void DiscoverDescendants(void);

    // Message Transmission Methods
    virtual void SendJoinMyNetworkRequest(TRfd * target_node);
    virtual void SendJoinMyNetworkAck(TRfd * target_node);
    virtual void SendSetClusterLevelCommand(TRfd * target_node);
    virtual void SendSetClusterLevelAck(TRfd * target_node);
    virtual void SendDiscoverDescendantsCommand(TRfd * target_node);
    virtual void SendDiscoverDescendantsAck(TRfd * target_node);
    virtual void SendDescendantDiscoveryCompleteMsg(TRfd * target_node);
    virtual void SendDescendantDiscoveryCompleteAck(TRfd * target_node);

    // Public Events

}; // End of the TRfd class definition


#endif
