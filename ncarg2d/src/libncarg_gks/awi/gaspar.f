C
C	$Id: gaspar.f,v 1.1 1993-01-09 01:57:42 fred Exp $
C
      SUBROUTINE GASPAR(IOS,STATUS)
C
C  Parse the ASPECT SOURCE FLAG elements.
C
      include 'trstat.h'
      include 'trpars.h'
      include 'trinst.h'
C
      INTEGER IOS, STATUS
      PARAMETER (ICVMAX=15)
      INTEGER TMPASF(2), ASFCNT, II, ICONV(ICVMAX)
      DATA ICONV/1,2,3,4,5,6,7,7,8,9,10,11,13,12,12/
C
C  Compute the number of aspect source flags defined.
C
      ASFCNT = (LEN / (MENCPR/BYTSIZ)) / 2
C
C  Extract the ASF pointer and value.
C
      DO 10 II = 1,ASFCNT
        CALL GOPDEC(TMPASF,MENCPR,2,IOS,STATUS)
        IF (TMPASF(1).GE.0 .AND. TMPASF(1).LT.ASFMAX) THEN
          IF(II .LE. ICVMAX) GASFSF(ICONV(II)) = TMPASF(2)
        END IF
 10   CONTINUE
      CALL GSASF(GASFSF)
C
      RETURN
      END
