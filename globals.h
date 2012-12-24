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
#define DEFAULT_NODE_SIZE                   (0x14u)

#define MAX(x,y)                            ((x)<(y)?(y):(x))
#define MIN(x,y)                            ((x)<(y)?(x):(y))


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
    MT_JOIN_NETWORK_REQ,
    MT_JOIN_NETWORK_RESP,
    MT_FIND_CHILDREN_REQ,
    MT_FIND_CHILDREN_RESP
} TRadioMsgType;


#endif
