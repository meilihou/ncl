C
C	$Id: gselnt.f,v 1.2 1993-01-09 02:02:36 fred Exp $
C
      SUBROUTINE GSELNT (TNR)
C
C  SELECT NORMALIZATION TRANSFORMATION
C
      INTEGER ESELNT
      PARAMETER (ESELNT=52)
C
      include 'gkscom.h'
C
      INTEGER TNR
C
C  Check if GKS is in the proper state.
C
      CALL GZCKST(8,ESELNT,IER)
      IF (IER .NE. 0) RETURN
C
C  Check that the normalization transformation number is valid.
C
      IF (TNR.LT.0 .OR. TNR.GT.MNT) THEN
        ERS = 1
        CALL GERHND(50,ESELNT,ERF)
        ERS = 0
        RETURN
      ENDIF
C
C  Set the current normalization transformation variable in
C  the GKS state list.
C
      CNT = TNR
C
C  Reestablish character height and up vector, and
C  pattern size and reference point, and clipping rectangle.
C
      CALL GSCHH(CCHH)
      CALL GSCHUP(CCHUP(1),CCHUP(2))
C
C  Suppress issuing warning of unsupported elements.
C
      IFLGO  = CUFLAG
      CUFLAG = -2
      CALL GSPA(CPA(1),CPA(2))
      CALL GSPARF(CPARF(1),CPARF(2))
      CUFLAG = IFLGO
      CALL GSCLIP(CCLIP)
C
      RETURN
      END
