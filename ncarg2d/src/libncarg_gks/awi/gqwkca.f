C
C	$Id: gqwkca.f,v 1.2 1993-01-09 02:01:58 fred Exp $
C
      SUBROUTINE GQWKCA(WTYPE,ERRIND,WKCAT)
C
C  INQUIRE WORKSTATION CATEGORY
C
      include 'gkscom.h'
C
      INTEGER WTYPE,ERRIND,WKCAT
C
C  Check if GKS is in the proper state.
C
      CALL GZCKST(8,-1,ERRIND)
      IF (ERRIND .NE. 0) GOTO 100
C
C  Check that the workstation type is valid.
C
      CALL GZCKWK(22,-1,IDUM,WTYPE,ERRIND)
      IF (ERRIND .NE. 0) GO TO 100
C
C  Provide the requested information.
C
      IF (WTYPE .EQ. GWSS) THEN
        WKCAT = GWISS
      ELSE IF (WTYPE.EQ.GCGM) THEN
        WKCAT = GMO
      ELSE IF (WTYPE.EQ.GXWC .OR. WTYPE.EQ.GXWE .OR. WTYPE.EQ.GDMP) THEN       
        WKCAT = GOUTPT
      ELSE
        WKCAT = -1
      ENDIF
      RETURN
C
  100 CONTINUE
      WKCAT = -1
      RETURN
      END
