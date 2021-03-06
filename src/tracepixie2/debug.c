/* debug */

/* debugging stubs */


#define	CF_DEBUGS	0		/* compile-time debugging */


/* revision history:

	= 1998-01-15, David Morano

	This was written to debug the REXEC program.


*/

/* Copyright � 1998 David A�D� Morano.  All rights reserved. */

/*******************************************************************************

	This modeule provides debugging support for the REXEC program.


*******************************************************************************/


#include	<envstandards.h>

#include	<sys/types.h>
#include	<sys/param.h>
#include	<sys/stat.h>
#include	<unistd.h>
#include	<fcntl.h>
#include	<signal.h>
#include	<stropts.h>
#include	<poll.h>
#include	<stdlib.h>
#include	<string.h>
#include	<ctype.h>

#include	<vsystem.h>
#include	<storebuf.h>
#include	<localmisc.h>


/* local defines */

#ifndef	COLUMNS
#define	COLUMNS		80
#endif

#define	PRINTBUFLEN	(COLUMNS + 2)
#define	HEXBUFLEN	100

#ifndef	TIMEBUFLEN
#define	TIMEBUFLEN	80
#endif

#ifndef	XDEBFILE
#define	XDEBFILE	"/var/tmp/pcspoll.deb"
#endif


/* external subroutines */

extern int	snopenflags(char *,int,int) ;
extern int	bufprintf(char *,int,const char *,...) ;
extern int	nprintf(const char *,const char *,...) ;
extern int	debugprintf(const char *,...) ;
extern int	debugprint(const char *,int) ;

extern char	*strwcpy(char *,const char *,int) ;


/* local structures */

struct debug_oflags {
	int	m ;
	char	*s ;
} ;


/* forward subroutines */

int		mkhexstr(char *,int,const void *,int) ;

static int	checkbasebounds(const char *,int,void *) ;


/* external variables */


/* local variables */

static const char	hextable[] = {
	'0', '1', '2', '3', '4', '5', '6', '7',
	'8', '9', 'a', 'b', 'c', 'd', 'e', 'f'
} ;


/* exported subroutines */


char *d_reventstr(revents,buf,buflen)
int	revents ;
char	buf[] ;
int	buflen ;
{


#if	CF_DEBUGS
	debugprintf("d_reventstr: ent\n") ;
#endif

	buf[0] = '\0' ;
	bufprintf(buf,buflen,"%s %s %s %s %s %s %s %s %s",
	    (revents & POLLIN) ? "I " : "  ",
	    (revents & POLLRDNORM) ? "IN" : "  ",
	    (revents & POLLRDBAND) ? "IB" : "  ",
	    (revents & POLLPRI) ? "PR" : "  ",
	    (revents & POLLWRNORM) ? "WN" : "  ",
	    (revents & POLLWRBAND) ? "WB" : "  ",
	    (revents & POLLERR) ? "ER" : "  ",
	    (revents & POLLHUP) ? "HU" : "  ",
	    (revents & POLLNVAL) ? "NV" : "  ") ;

#if	CF_DEBUGS
	debugprintf("d_reventstr: %s\n",buf) ;
#endif

	return buf ;
}
/* end subroutine (d_reventstr) */


/* who is open? */
void d_whoopen(s)
const char	s[] ;
{
	int	rs ;
	int	i ;


	if (s != NULL)
	    debugprintf("d_whoopen: %s\n",s) ;

	for (i = 0 ; i < 20 ; i += 1) {

	    if ((rs = u_fcntl(i,F_GETFL,0)) >= 0)
	        debugprintf("d_whoopen: open on %d accmod=%08x\n",
	            i,(rs & O_ACCMODE)) ;

	}

}
/* end subroutine (d_whoopen) */


/* return a count of the number of open files */
int d_openfiles()
{
	struct ustat	sb ;

	int	i ;
	int	count = 0 ;


	for (i = 0 ; i < 2048 ; i += 1) {

	    if (u_fstat(i,&sb) >= 0)
	        count += 1 ;

	} /* end for */

	return count ;
}
/* end subroutine (d_openfiles) */


int d_ispath(p)
const char	*p ;
{


	if (p == NULL)
	    return FALSE ;

#ifdef	DEBFILE
	nprintf(DEBFILE,"d_ispath: PATH=>%W<\n",
	    p,strnlen(p,30)) ;
#endif

	return ((*p == '/') || (*p == ':')) ;
}
/* end subroutine (d_ispath) */


int gdb()
{

	return 0 ;
}
/* end subroutine (gdb) */


int mkhexstr(dbuf,dlen,vp,vl)
char		dbuf[] ;
int		dlen ;
const void	*vp ;
int		vl ;
{
	int	sl = vl ;
	int	i ;
	int	ch ;
	int	j = 0 ;

	const uchar	*sp = (const uchar *) vp ;


	if (sl < 0) sl = strlen(sp) ;

	for (i = 0 ; (dlen >= 3) && (i < sl) ; i += 1) {
	    ch = sp[i] ;

	    if (i > 0) dbuf[j++] = ' ' ;
	    dbuf[j++] = hextable[(ch>>4)&15] ;
	    dbuf[j++] = hextable[(ch>>0)&15] ;

	    dlen -= ((i > 0) ? 3 : 2) ;
	} /* end for */
	dbuf[j] = '\0' ;

	return j ;
}
/* end subroutine (mkhexstr) */


int mkhexnstr(hbuf,hlen,maxcols,sbuf,slen)
char		hbuf[] ;
int		hlen ;
const char	sbuf[] ;
int		slen ;
int		maxcols ;
{
	int	n = 0 ;


	if (maxcols < 0) maxcols = COLUMNS ;

	if (slen < 0) slen = strlen(sbuf) ;
	n = MIN((maxcols / 3),slen) ;
	mkhexstr(hbuf,hlen,sbuf,n) ;

	return n ;
}
/* end subroutine (mkhexnstr) */


int debugprinthex(ids,maxcols,sp,sl)
const char	*ids ;
int		maxcols ;
const char	*sp ;
int		sl ;
{
	int	rs ;
	int	idlen = 0 ;
	int	wlen = 0 ;

	char	printbuf[PRINTBUFLEN + 1] ;


	if (ids != NULL) idlen = strlen(ids) ;

	if (maxcols < 0) maxcols = COLUMNS ;

	if (idlen > 0) maxcols -= (idlen + 1) ;

	rs = mkhexnstr(printbuf,PRINTBUFLEN,maxcols,sp,sl) ;

	if (rs >= 0) {
	    if (idlen > 0) {
	        rs = debugprintf("%t %s\n",ids,idlen,printbuf) ;
	    } else
	        rs = debugprintf("%s\n",printbuf) ;
	    wlen = rs ;
	}

	return (rs >= 0) ? wlen : rs ;
}
/* end subroutine (debugprinthex) */


int debugprinthexblock(ids,maxcols,vp,vl)
const char	*ids ;
int		maxcols ;
const void	*vp ;
int		vl ;
{
	int	rs = SR_OK ;
	int	idlen = 0 ;
	int	n, i ;
	int	pbl ;
	int	cslen ;
	int	cols ;
	int	len ;
	int	sl = vl ;
	int	wlen = 0 ;

	const char	*sp = (const char *) vp ;

	char	printbuf[PRINTBUFLEN + 1] ;
	char	*pbp ;


	if (ids != NULL) idlen = strlen(ids) ;

	if (maxcols < 0) maxcols = COLUMNS ;

	if (sl < 0) sl = strlen(sp) ;

	i = 0 ;
	while ((rs >= 0) && (sl > 0)) {

	    pbp = printbuf ;
	    pbl = PRINTBUFLEN ;
	    cols = maxcols ;

	    if (ids != NULL) {
	        if ((idlen+2) < pbl) {
	            i = strwcpy(pbp,ids,idlen) - pbp ;
		    pbp[i++] = ':' ;
		    pbp[i++] = ' ' ;
		    pbp += i ;
		    pbl -= i ;
		    cols -= i ;
		} else
		    rs = SR_OVERFLOW ;
	    }

	    if (rs >= 0) {
	        n = (cols / 3) ;
	            cslen = MIN(n,sl) ;
	            rs = mkhexstr(pbp,pbl,sp,cslen) ;
	            sp += cslen ;
	            sl -= cslen ;
	    }

	    if (rs >= 0) {
	        len = debugprint(printbuf,-1) ;
	        wlen += len ;
	    }

	} /* end while */

	return (rs >= 0) ? wlen : rs ;
}
/* end subroutine (debugprinthexblock) */


int hexblock(ids,ap,n)
const char	ids[] ;
const char	*ap ;
int		n ;
{
	int	i, sl ;

	char	hexbuf[HEXBUFLEN + 3] ;


	if (ids != NULL)
	    debugprint(ids,-1) ;

	for (i = 0 ; i < n ; i += 1) {

	    sl = mkhexstr(hexbuf,HEXBUFLEN,ap,4) ;

	    hexbuf[sl++] = '\n' ;
	    hexbuf[sl] = '\0' ;

	    ap += 4 ;

	    debugprint(hexbuf,-1) ;

	} /* end for */

	return n ;
}
/* end subroutine (hexblock) */


/* audit a HOSTENT structure */
int heaudit(hep,buf,buflen)
struct hostent	*hep ;
const char	buf[] ;
int		buflen ;
{
	int	rs = SR_OK ;
	int	i ;

	char	**cpp ;


	if (hep == NULL)
	    return SR_FAULT ;

	if (buf == NULL)
	    return SR_FAULT ;

	if (buflen < 0)
	    return SR_INVALID ;

	if (rs >= 0)
	    rs = checkbasebounds(buf,buflen,hep->h_name) ;

	if (rs >= 0) {
	    cpp = hep->h_aliases ;
	    if (cpp != NULL) {
	        rs = checkbasebounds(buf,buflen,cpp) ;

	        if (rs >= 0) {
	            for (i = 0 ; cpp[i] != NULL ; i += 1) {
	                rs = checkbasebounds(buf,buflen,(cpp + i)) ;
	                if (rs >= 0)
	                    rs = checkbasebounds(buf,buflen,cpp[i]) ;
	                if (rs < 0)
	                    break ;
	            }
	        }
	    }
	}

	if (rs >= 0) {
	    cpp = hep->h_aliases ;
	    if (cpp != NULL) {
	        rs = checkbasebounds(buf,buflen,cpp) ;

	        if (rs >= 0) {
	            for (i = 0 ; cpp[i] != NULL ; i += 1) {
	                rs = checkbasebounds(buf,buflen,(cpp + i)) ;
	                if (rs >= 0)
	                    rs = checkbasebounds(buf,buflen,cpp[i]) ;
	                if (rs < 0)
	                    break ;
	            }
	        }
	    }
	}

	return rs ;
}
/* end subroutine (heaudit) */


char *stroflags(buf,oflags)
char		buf[] ;
int		oflags ;
{
	int	rs = snopenflags(buf,TIMEBUFLEN,oflags) ;
	return (rs >= 0) ? buf : NULL ;
}
/* end subroutine (stroflags) */


/* local subroutines */


static int checkbasebounds(bbuf,blen,vp)
const char	bbuf[] ;
int		blen ;
void		*vp ;
{
	int	rs = SR_OK ;

	const char	*tp = (const char *) vp ;


	if ((rs >= 0) && (tp < bbuf))
	    rs = SR_BADFMT ;

	if ((rs >= 0) && (tp >= (bbuf + blen)))
	    rs = SR_BADFMT ;

	return rs ;
}
/* end subroutine (checkbasebounds) */



