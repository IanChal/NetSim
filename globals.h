#ifndef _TDEF_H_
#define _TDEF_H_

/*---------------*/
/* Include Files */
/*---------------*/


/*----------*/
/* Typedefs */
/*----------*/

typedef unsigned char                       uint8;
typedef signed char                         sint8;
typedef char                                int8;
typedef unsigned short int                  uint16;
typedef signed short int                    sint16;
typedef unsigned int                        uint32;
typedef signed int                          sint32;
typedef float                               fl32;
typedef __int64                             sint64;


/*-------------------*/
/* Macro Definitions */
/*-------------------*/

#define CLUSTER_LEVEL_UNKNOWN               ((uint8)0xFFu)
#define DEFAULT_NODE_SIZE                   (0x12u)

#define MAX(x,y)                            ((x)<(y)?(y):(x))
#define MIN(x,y)                            ((x)<(y)?(x):(y))
#define SHAPE_CENTRE_X(s)                   ((s)->Left + ((s)->Width/2))
#define SHAPE_CENTRE_Y(s)                   ((s)->Top + ((s)->Height/2))
#define INI_FILENAME                        (Application->ExeName + ".ini")
#define INI_SECTION_NAME                    ("Nodes")


/*-----------------*/
/* Data Structures */
/*-----------------*/

typedef enum
{
    NT_UNKNOWN,
    NT_COORDINATOR,
    NT_ROUTER,
    NT_END_DEVICE
} TNodeType;

typedef enum
{
    MT_JOIN_MY_NETWORK_REQ,
    MT_JOIN_MY_NETWORK_ACK,
    MT_SET_CLUSTER_LEVEL_CMD,
    MT_SET_CLUSTER_LEVEL_ACK,
    MT_DISCOVER_DESCENDANTS_CMD,
    MT_DISCOVER_DESCENDANTS_ACK,
    MT_DESCENDANT_DISCOVERY_DONE_MSG,
    MT_DESCENDANT_DISCOVERY_DONE_ACK
} TRadioMsgType;


#endif
