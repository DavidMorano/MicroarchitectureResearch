/* ldecode_instr_class.h  */



#ifndef	LDECODEINSTRCLASS_INCLUDE
#define	LDECODEINSTRCLASS_INCLUDE	1



#include	"opclass.h"



/*    Class of instructions */

#define INSTR_UNUSED	OPCLASS_UNUSED
#define INSTR_BREL	OPCLASS_BREL
#define INSTR_BIND	OPCLASS_BIND
#define INSTR_LOAD	OPCLASS_LOAD
#define INSTR_STORE	OPCLASS_STORE
#define INSTR_ALU	OPCLASS_ALU
#define INSTR_FPALU	OPCLASS_FPALU
#define INSTR_SPECIAL	OPCLASS_SPECIAL
#define INSTR_JREL	OPCLASS_JREL
#define INSTR_JIND	OPCLASS_JIND
#define INSTR_EXCEP	OPCLASS_EXCEP
#define	INSTR_UNIMPL	OPCLASS_UNIMPL



#endif /* LDECODEINSTRCLASS_INCLUDE */



