C
C	$Id: ginst.f,v 1.1 1993-01-09 01:58:51 fred Exp $
C
      SUBROUTINE GINST (WKID,STDNR,LSTR,ISTR,PET,XMIN,XMAX,YMIN,
     +                  YMAX,BUFLEN,INIPOS,LDR,DATREC)
C
C  INITIALISE STRING
C
      INTEGER EINST
      PARAMETER (EINST=74)
C
      INTEGER WKID,STDNR,LSTR,PET,BUFLEN,INIPOS,LDR
      REAL XMIN,XMAX,YMIN,YMAX
      CHARACTER*(*) ISTR
      CHARACTER*80 DATREC(LDR)
C
C  The only reason this subroutine is in the NCAR GKS package is
C  to support the pause feature of FRAME and NBPICT in a standard
C  manner so that those two subroutines can work with a any level
C  2B GKS package.
C
C  Check if GKS is in the proper state.
C
      CALL GZCKST(7,EINST,IER)
      IF (IER .NE. 0) RETURN
C
C  Check if the workstation identifier is valid.
C
      CALL GZCKWK(20,EINST,WKID,IDUM,IER)
      IF (IER .NE. 0) RETURN
C
C  Check if the specified workstation is open.
C
      CALL GZCKWK(25,EINST,WKID,IDUM,IER)
      IF (IER .NE. 0) RETURN
C
C  Other checks should be added if this subroutine is ever fully
C  implemented for NCAR GKS.
C
      RETURN
      END
