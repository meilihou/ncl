      SUBROUTINE CSA3S (NI,XI,UI,KNOTS,NXO,NYO,NZO,
     +                  XO,YO,ZO,UO,NWRK,WORK,IER)
C
      DIMENSION XI(3,NI),UI(NI),KNOTS(3),XO(NXO),YO(NYO),
     +          ZO(NZO),UO(NXO,NYO,NZO),WORK(NWRK),NDERIV(3)
      DATA NDERIV/0,0,0/
C
C  Check on the number of knots.
C
      NTOT = KNOTS(1)*KNOTS(2)*KNOTS(3)
      IF (NTOT .GT. NI) THEN
        CALL CFAERR (201,' CSA3S - cannot have more knots than input dat
     +a points',54)
      ENDIF
C
      DO 20 I=1,3
        IF (KNOTS(I) .LT. 4) THEN
          CALL CFAERR (202,' CSA3S - must have at least four knots in ev
     +ery coordinate direction',68)       
        ENDIF
   20 CONTINUE
C
C  Check on the size of the workspace.
C
      IF (NWRK .LT. NTOT*(NTOT+3)) THEN
        CALL CFAERR (203,' CSA3S - workspace too small',28)
      ENDIF
C
C  Invoke the expanded function.
C
      WTS = -1.
      SSMTH = 0.
      CALL CSA3XS (NI,XI,UI,WTS,KNOTS,SSMTH,NDERIV,NXO,NYO,NZO,
     +             XO,YO,ZO,UO,NWRK,WORK,IER)
      IF (IERR .NE. 0) RETURN
C
      RETURN
      END
