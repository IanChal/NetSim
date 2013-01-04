#ifndef TRfdH
#define TRfdH

/*---------------*/
/* Include Files */
/*---------------*/

#include <vcl.h>
#include "globals.h"


/*-------------------*/
/* Macro Definitions */
/*-------------------*/


/*-----------------*/
/* Data Structures */
/*-----------------*/

typedef enum _TNodeStatus
{
    NS_NODE_IDLE,
    NS_NODE_WAITING_JOINERS,
    NS_UNSPECIFIED
} TNodeStatus;


/*------------------------------*/
/* Definition of the TRfd class */
/*------------------------------*/
class TRouter;                              // Forward references
class TRadioMsg;
class TRfd
{
protected:
    // Protected events
    virtual void __fastcall Msg_TimerTimer(TObject *Sender);
    virtual void __fastcall Msg_Timeout_TimerTimer(TObject *Sender);

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
    TRouter * Parent_Node;
    TNodeType Node_Type;
    uint8 Cluster_Level;
    sint32 Tx_Range;
    TNodeStatus Node_State;
    uint8 Retry_Counter;

    // Public Methods
    TRfd(TComponent * owner);
    ~TRfd(void);
    void ClearMessageBuffer(void);
    void DrawNode(sint32 x, sint32 y);


    // Message Transmission Methods
    virtual void SendJoinMyNetworkRequest(TRfd * target_node);
    virtual void SendJoinMyNetworkAck(TRfd * target_node);
    virtual void SendSetClusterLevelCommand(TRfd * target_node);
    virtual void SendSetClusterLevelAck(TRfd * target_node);
    virtual void SendDiscoverDescendantsCommand(TRfd * target_node);
    virtual void SendDiscoverDescendantsAck(TRfd * target_node);
    virtual void SendDescendantDiscoveryCompleteMsg(TRfd * target_node, sint32 count);
    virtual void SendDescendantDiscoveryCompleteAck(TRfd * target_node);

    // Message handling methods
    virtual void HandleJoinMyNetworkRequest(TRadioMsg * msg);
    virtual void HandleJoinMyNetworkAck(TRadioMsg * msg);
    virtual void HandleSetClusterLevelCommand(TRadioMsg * msg);
    virtual void HandleSetClusterLevelAck(TRadioMsg * msg);
    virtual void HandleDiscoverDescendantsCommand(TRadioMsg * msg);
    virtual void HandleDescendantDiscoveryDoneMsg(TRadioMsg * msg);

    // Public Events

}; // End of the TRfd class definition


// Will not compile unless it's in this module ??? :-(

class TRouter : public TRfd
{
private:
    // Private Data

    // Private Methods
    void CommonConstructorTasks(TComponent * owner);


protected:
    // Protected Data

    // Protected Methods

    // Protected Events
    void __fastcall Msg_Timeout_TimerTimer(TObject *Sender);

public:
    // Public Data
    bool Child_Added;
    TList * Child_List;                     // List of child nodes. Type can be of any type derived from TRfd
    sint32 Child_List_Index;

    // Public Methods
    TRouter(TComponent * owner);
    ~TRouter(void);

    // Message transmission methods
    virtual void SendJoinMyNetworkRequest(TRfd * target_node);
    virtual void SendSetClusterLevelCommand(TRfd * target_node);
    virtual void SendDiscoverDescendantsCommand(TRfd * target_node);
    virtual void SendDescendantDiscoveryCompleteAck(TRfd * target_node);

    // Message handler methods
    virtual void HandleJoinMyNetworkAck(TRadioMsg * msg);
    virtual void HandleSetClusterLevelAck(TRadioMsg * msg);
    virtual void HandleDiscoverDescendantsCommand(TRadioMsg * msg);
    virtual void HandleDescendantDiscoveryDoneMsg(TRadioMsg * msg);
}; // End of the TRouter class definition


#endif
