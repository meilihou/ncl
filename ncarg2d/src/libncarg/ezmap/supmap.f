C
C $Id: supmap.f,v 1.13 2001-08-16 23:09:46 kennison Exp $
C
C                Copyright (C)  2000
C        University Corporation for Atmospheric Research
C                All Rights Reserved
C
C This file is free software; you can redistribute it and/or modify
C it under the terms of the GNU General Public License as published
C by the Free Software Foundation; either version 2 of the License, or
C (at your option) any later version.
C
C This software is distributed in the hope that it will be useful, but
C WITHOUT ANY WARRANTY; without even the implied warranty of
C MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
C General Public License for more details.
C
C You should have received a copy of the GNU General Public License
C along with this software; if not, write to the Free Software
C Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307
C USA.
C
      SUBROUTINE SUPMAP (JPRJ,PLAT,PLON,ROTA,PLM1,PLM2,PLM3,PLM4,JLTS,
     +                   JGRD,IOUT,IDOT,IERR)
C
      INTEGER JPRJ,JLTS,JGRD,IOUT,IDOT,IERR
      REAL    PLAT,PLON,ROTA,PLM1(2),PLM2(2),PLM3(2),PLM4(2)
C
C Declare required common blocks.  See MAPBD for descriptions of these
C common blocks and the variables in them.
C
        COMMON /MAPCM5/  DDCT(5),DDCL(5),LDCT(6),LDCL(6),PDCT(12),
     +                   PDCL(12)
        CHARACTER*2      DDCT,DDCL,LDCT,LDCL,PDCT,PDCL
        SAVE   /MAPCM5/
C
C Declare local variables.
C
        INTEGER          I,INTF,LLTS(6),LPRJ(11)
C
        DATA LPRJ / 2,3,1,4,5,6,11,7,8,9,10 /
        DATA LLTS / 1,2,5,4,3,6 /
C
C Set the error flag to indicate an error; if all goes well, this flag
C will be cleared.
C
        IERR=1
C
C Check for an uncleared prior error.
C
        IF (ICFELL('SUPMAP - UNCLEARED PRIOR ERROR',1).NE.0) RETURN
C
C Set EZMAP's grid-spacing parameter.
C
        CALL MDSETI ('GR',MOD(ABS(JGRD),1000))
        IF (ICFELL('SUPMAP',2).NE.0) RETURN
C
C Set EZMAP's outline-selection parameter.
C
        IF (ABS(IOUT).EQ.0.OR.ABS(IOUT).EQ.1) THEN
          I=1+2*ABS(IOUT)+(1+ISIGN(1,JPRJ))/2
        ELSE
          I=MAX(1,MIN(6,IOUT))
        END IF
C
        IF (I.LE.5) THEN
          CALL MDSETC ('OU',DDCT(I))
          IF (ICFELL('SUPMAP',3).NE.0) RETURN
        END IF
C
C Set EZMAP's perimeter-drawing flag.
C
        CALL MDSETL ('PE',JGRD.GE.0)
        IF (ICFELL('SUPMAP',4).NE.0) RETURN
C
C Set EZMAP's grid-line-labelling flag.
C
        CALL MDSETL ('LA',MOD(ABS(JGRD),1000).NE.0)
        IF (ICFELL('SUPMAP',5).NE.0) RETURN
C
C Set EZMAP's dotted-outline flag.
C
        CALL MDSETI ('DO',MAX(0,MIN(1,IDOT)))
        IF (ICFELL('SUPMAP',6).NE.0) RETURN
C
C Set EZMAP's projection-selection parameters.
C
        I=MAX(1,MIN(11,ABS(JPRJ)))
        CALL MAPROJ (PDCT(LPRJ(I)+1),PLAT,PLON,ROTA)
        IF (ICFELL('SUPMAP',7).NE.0) RETURN
C
C Set EZMAP's rectangular-limits-selection parameters.
C
        I=LLTS(MAX(1,MIN(6,ABS(JLTS))))
        CALL MAPSET (LDCT(I),PLM1,PLM2,PLM3,PLM4)
        IF (ICFELL('SUPMAP',8).NE.0) RETURN
C
C Draw the map.
C
        CALL MDGETI ('IN',INTF)
C
        IF (INTF.NE.0) THEN
          CALL MDPINT
          IF (ICFELL('SUPMAP',9).NE.0) RETURN
        END IF
C
        CALL MDPGRD
        IF (ICFELL('SUPMAP',10).NE.0) RETURN
C
        CALL MDPLBL
        IF (ICFELL('SUPMAP',11).NE.0) RETURN
C
        IF (IOUT.LT.100) THEN
          CALL MDPLOT
          IF (ICFELL('SUPMAP',12).NE.0) RETURN
        ELSE
          CALL MDLNDR ('Earth..1',MOD(IOUT,100))
          IF (ICFELL('SUPMAP',13).NE.0) RETURN
        END IF
C
C All seems to have gone well - turn off the error flag.
C
        IERR=0
C
C Done.
C
        RETURN
C
      END
