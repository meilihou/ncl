C
C	$Id: gzxid.f,v 1.1 1993-01-09 02:04:43 fred Exp $
C
      SUBROUTINE GZXID(WKID,XID,IER)
C
C  Return the local X window identifier associated with the
C  WKID supplied.  IER is 0 if success.
C
      include 'gkscom.h'
C
      INTEGER WKID,XID,IER
C
      IER = 0
C
C  Check to make sure that WKID flags an X workstation type.
C
      IF (WKID .LT. 0) THEN
        IER = 20
        RETURN
      ENDIF
C
      CALL GQWKC(WKID,IER,ICONID,ITYPE)
      IF (IER .NE. 0) RETURN
      CALL GQWKCA(ITYPE,IER,ICAT)
      IF (IER .NE. 0) RETURN
C
      DO 10 I=1,NOPWK
        IF (WKID .EQ. SOPWK(I)) THEN
          XID = LXWKID(I)
          RETURN
        ENDIF
   10 CONTINUE
      IER = 25
C
      RETURN
      END
