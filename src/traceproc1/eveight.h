/* eveight */


/* revision history:

	= 1998-11-01, David Morano

	Originally written for Audix Database Processor (DBP) work.


*/

/* Copyright � 1998 David A�D� Morano.  All rights reserved. */

#ifndef	EVEIGHT_INCLUDE
#define	EVEIGHT_INCLUDE	1


#include	<envstandards.h>

#include	<sys/types.h>
#include	<time.h>

#include	<localmisc.h>


/* object defines */

#define	EVEIGHT			struct eveight_head
#define	EVEIGHT_STATS		struct eveight_stats

/* more important defines */

#define	EVEIGHT_COUNTBITS	2	/* counter bits */


/* statistics */
struct eveight_stats {
	uint			tlen ;
	uint			bits ;
} ;

struct eveight_banks {
	uint	bim : 2 ;
	uint	g0 : 2 ;
	uint	g1 : 2 ;
	uint	meta : 2 ;
} ;

struct eveight_head {
	unsigned long		magic ;
	struct eveight_stats	s ;
	struct eveight_banks	*table ;
	uint			bhistory ;	/* global branch history */
	uint			tlen ;
	uint			tmask ;
} ;


#if	(! defined(EVEIGHT_MASTER)) || (EVEIGHT_MASTER == 0)

#ifdef	__cplusplus
extern "C" {
#endif

extern int	eveight_init(EVEIGHT *,int,int,int,int) ;
extern int	eveight_lookup(EVEIGHT *,uint) ;
extern int	eveight_confidence(EVEIGHT *,uint) ;
extern int	eveight_update(EVEIGHT *,uint,int) ;
extern int	eveight_zerostats(EVEIGHT *) ;
extern int	eveight_stats(EVEIGHT *,EVEIGHT_STATS *) ;
extern int	eveight_free(EVEIGHT *) ;

#ifdef	__cplusplus
}
#endif

#endif /* EVEIGHT_MASTER */

#endif /* EVEIGHT_INCLUDE */


