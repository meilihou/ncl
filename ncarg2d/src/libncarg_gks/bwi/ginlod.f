C
C	$Id: ginlod.f,v 1.2 1993-01-09 02:07:12 fred Exp $
C
      SUBROUTINE GINLOD(GKSERR)
C
C  This routine loads the current CGM element into the metafile.
C  The current CGM element includes the opcode CLASS and ID,
C  and the size which may be short or long.
C
C  OUTPUT
C    GKSERR -- An error status flag.
C
C  All data is type integer unless otherwise indicated.
C
      IMPLICIT INTEGER (A-Z)
C
C  Common for communication of instruction and length.
C
      include 'g01ins.h'
      include 'g01io.h'
C
C  Define the ALLOK status and the opcode CLASS and ID lengths.
C
      DATA ALLOK,OPCLLN,OPIDLN/0,4,7/
C
C  Define the short format length, short format count, long format flag,
C  continue flag on, continue flag off, continue length, long format
C  length.
C
      DATA SHFMLN,SHTFMT,LFMFLG,CONON,CONOFF,CFMLNG,LFMLNG
     +          /5,30,31,1,0,1,15/
      DATA  SHORT/0/, LONG/1/
C
C  Initialize error status.
C
      GKSERR = ALLOK
C
C  Make sure the element starts on a 16 bit boundry.
C
      TEMP = MOD(MBFPOS,16)
      IF (TEMP .NE. 0) MBFPOS = MBFPOS + (16-TEMP)
C
C  Load the opcode CLASS and ID into the metafile.
C
      CALL GMFLOD(MCOPCL,OPCLLN,1,GKSERR)
      IF (GKSERR .NE. ALLOK) RETURN
      CALL GMFLOD(MCOPID,OPIDLN,1,GKSERR)
      IF (GKSERR .NE. ALLOK) RETURN
C
C  Determine if a long format or short format element.
C
      IF (MCCBYT .LE. SHTFMT) THEN
C
C  Short format.
C
        MSLFMT = SHORT
        CALL GMFLOD (MCCBYT,SHFMLN,1,GKSERR)
        IF (GKSERR .NE. ALLOK) RETURN
      ELSE
C
C  Long format.
C
        MSLFMT = LONG
        CALL GMFLOD (LFMFLG,SHFMLN,1,GKSERR)
        IF (GKSERR .NE. ALLOK) RETURN
C
C  Set the continue flag.
C
        IF (MCNBYT .NE. 0) THEN
C
C  There is another partition.
C
          CALL GMFLOD(CONON,CFMLNG,1,GKSERR)
        ELSE
C
C  Last partition.
C
          CALL GMFLOD(CONOFF,CFMLNG,1,GKSERR)
        END IF
C
        IF (GKSERR .NE. ALLOK) RETURN
C
C  Set the long format operand list size.
C
        CALL GMFLOD(MCCBYT,LFMLNG,1,GKSERR)
      END IF
C
      RETURN
      END
