/* bpalpha */


/* revision history:

	= 1998-11-01, David Morano

	Originally written for Audix Database Processor (DBP) work.


*/

/* Copyright � 1998 David A�D� Morano.  All rights reserved. */

#ifndef	BPALPHA_INCLUDE
#define	BPALPHA_INCLUDE	1


#include	<envstandards.h>

#include	<sys/types.h>
#include	<time.h>

#include	<localmisc.h>


/* object defines */

#define	BPALPHA			struct bpalpha_head
#define	BPALPHA_STATS		struct bpalpha_stats


/* statistics */
struct bpalpha_stats {
	uint			gpht ;		/* global length */
	uint			lbht ;		/* local history length */
	uint			lpht ;		/* local pattern length */
	uint			bits ;
} ;

struct bpalpha_head {
	unsigned long		magic ;
	struct bpalpha_stats	s ;
	uint			*lbht ;		/* local BHT */
	uchar			*lpht ;		/* local PHT */
	uchar			*cpht ;		/* choice PHT */
	uchar			*gpht ;		/* global PHT */
	uint			bhistory ;	/* global branch history */
	uint			lhlen ;		/* local history length */
	uint			lplen ;		/* local pattern length */
	uint			glen ;		/* global length */
	uint			historymask ;
} ;


#if	(! defined(BPALPHA_MASTER)) || (BPALPHA_MASTER == 0)

#ifdef	__cplusplus
extern "C" {
#endif

extern int	bpalpha_init(BPALPHA *,int,int,int) ;
extern int	bpalpha_lookup(BPALPHA *,uint) ;
extern int	bpalpha_confidence(BPALPHA *,uint) ;
extern int	bpalpha_update(BPALPHA *,uint,int) ;
extern int	bpalpha_zerostats(BPALPHA *) ;
extern int	bpalpha_stats(BPALPHA *,BPALPHA_STATS *) ;
extern int	bpalpha_free(BPALPHA *) ;

#ifdef	__cplusplus
}
#endif

#endif /* BPALPHA_MASTER */

#endif /* BPALPHA_INCLUDE */


