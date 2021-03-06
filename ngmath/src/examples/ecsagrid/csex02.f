
      PROGRAM CSEX02
C
C  Show the effects of changing the weighting array.
C
C  The number of input data points.
C
      PARAMETER (NDATA=6)
C
C  The number of output data points.
C
      PARAMETER (NX=101)
C
C  The maximum number of knots used in any call.
C
      PARAMETER (NCF=5)
C
C  The size of the workspace.
C
      PARAMETER (NWRK=NCF*(NCF+3))
C
C  Array dimensions.
C
      DIMENSION XDATA(1,NDATA),YDATA(NDATA),XDATAT(NDATA)
      DIMENSION WTS(NDATA),WORK(NWRK)
      DIMENSION XO(NX),Y1(NX),Y2(NX),Y3(NX)
C
C  Initialize the weight array so that all input points are
C  weighted equally.
C
      DATA WTS/1.0, 1.0, 1.0, 1.0, 1.0, 1.0/
C
C  Specify the input data.
C
      DATA XDATA(1,1),XDATA(1,2),XDATA(1,3)/0.0, 0.2, 0.4/
      DATA XDATA(1,4),XDATA(1,5),XDATA(1,6)/0.6, 0.8, 1.0/
      DATA YDATA/0.0, 1.0, -0.7, -0.2, -0.1, 0.0/
C
C  Define the output coordinates.
C
      XINC = 1./REAL(NX-1)
      DO 10 I=1,NX
        XO(I) = (I-1)*XINC
   10 CONTINUE
C
C  Calculate the approximating curve with all weights equal.
C
      SSMTH = 0.
      NDERIV = 0
      CALL CSA1XS (NDATA,XDATA,YDATA,WTS,NCF,
     +             SSMTH,NDERIV,NX,XO,Y1,NWRK,WORK,IER)       
      IF (IER .NE. 0) THEN
        WRITE(6,520) IER
  520   FORMAT(' Error ',I3,' returned from CSA1XS')
        STOP
      ENDIF
C
C  Calculate the approximating curve with the second input coordinate
C  weighted as half the other weights.
C
      WTS(2) = 0.5
      CALL CSA1XS (NDATA,XDATA,YDATA,WTS,NCF,
     +             SSMTH,NDERIV,NX,XO,Y2,NWRK,WORK,IER)       
      IF (IER .NE. 0) THEN
        WRITE(6,520) IER
        STOP
      ENDIF
C
C  Calculate the approximating curve with the second input coordinate
C  ignored (i.e. with a weight of zero).
C
      WTS(2) = 0.0
      CALL CSA1XS (NDATA,XDATA,YDATA,WTS,NCF,
     +             SSMTH,NDERIV,NX,XO,Y3,NWRK,WORK,IER)       
      IF (IER .NE. 0) THEN
        WRITE(6,520) IER
        STOP
      ENDIF
C
C  Draw a plot of the approximation curves and mark the original points.
C
      DO 30 I=1,NDATA
        XDATAT(I) = XDATA(1,I)
   30 CONTINUE
      CALL DRWFT1(NDATA,XDATAT,YDATA,NX,XO,Y1,Y2,Y3)
C
      STOP
      END
      SUBROUTINE DRWFT1(NUMIN,X,Y,NUMOUT,XO,Y1,Y2,Y3)
C
C  This subroutine uses NCAR Graphics to plot curves.
C
C Define the error file, the Fortran unit number, the workstation type,
C and the workstation ID to be used in calls to GKS routines.
C
C     PARAMETER (IERRF=6, LUNIT=2, IWTYPE=1,  IWKID=1)   ! NCGM
C     PARAMETER (IERRF=6, LUNIT=2, IWTYPE=8,  IWKID=1)   ! X Windows
C     PARAMETER (IERRF=6, LUNIT=2, IWTYPE=11, IWKID=1)   ! PDF
C     PARAMETER (IERRF=6, LUNIT=2, IWTYPE=20, IWKID=1)   ! PostScript
C
      PARAMETER (IERRF=6, LUNIT=2, IWTYPE=1,  IWKID=1)
C
C  Vertical position for initial curve.
C
      DATA YPOS_TOP/0.88/
C
C  Open GKS, open and activate a workstation.
C
      CALL GOPKS (IERRF, ISZDM)
      CALL GOPWK (IWKID, LUNIT, IWTYPE)
      CALL GACWK (IWKID)
C
C  Define a color table.
C
      CALL GSCR(IWKID, 0, 1.0, 1.0, 1.0)
      CALL GSCR(IWKID, 1, 0.0, 0.0, 0.0)
      CALL GSCR(IWKID, 2, 1.0, 0.0, 0.0)
      CALL GSCR(IWKID, 3, 0.0, 1.0, 0.0)
      CALL GSCR(IWKID, 4, 0.0, 0.0, 1.0)
      CALL GSCLIP(0)
C
C  Plot the main title.
C
      CALL PLCHHQ(0.50,0.95,':F25:Effect of data weights',
     +            0.030,0.,0.)
C
C  Draw a background grid for the first curve.
C
      YB = -1.0
      YT =  1.0
      CALL BKGFT1(YPOS_TOP,'Weights = (1., 1., 1., 1., 1., 1.)',YB,YT)
      CALL GRIDAL(5,5,4,1,1,1,10,0.0,YB)
C
C  Graph the approximation curve where all input coordinates were
C  wieghted equally.
C
      CALL GPL(NUMOUT,XO,Y1)
C
C  Mark the original data points.
C
      CALL GSMKSC(2.)
      CALL GSPMCI(4)
      CALL GPM(NUMIN,X,Y)
C
C  Graph the approximation curve where the second input coordinate was
C  given a weight of 0.5.
C
      CALL BKGFT1(YPOS_TOP-0.3,
     +            'Weights = (1., .5, 1., 1., 1., 1.)',YB,YT)
      CALL GRIDAL(5,5,4,1,1,1,10,0.0,YB)
      CALL GPL(NUMOUT,XO,Y2)
      CALL GPM(NUMIN,X,Y)
C
C  Graph the approximation curve where the second input coordinate was
C  given a weight of 0.0 .
C
      CALL BKGFT1(YPOS_TOP-0.6,
     +            'Weights = (1., 0., 1., 1., 1., 1.)',YB,YT)
      CALL GRIDAL(5,5,4,1,1,1,10,0.0,YB)
      CALL GPL(NUMOUT,XO,Y3)
      CALL GPM(NUMIN,X,Y)
      CALL FRAME
C
      CALL GDAWK(IWKID)
      CALL GCLWK(IWKID)
      CALL GCLKS
C
      RETURN
      END
      SUBROUTINE BKGFT1(YPOS,LABEL,YB,YT)
C
C  This subroutine draws a background grid.
C
      DIMENSION XX(2),YY(2)
      CHARACTER*(*) LABEL
C
      CALL SET(0.,1.,0.,1.,0.,1.,0.,1.,1)
C
C  Plot the curve label using font 21 (Helvetica).
C
      CALL PCSETI('FN',21)
      CALL PLCHHQ(0.65,YPOS - 0.03,LABEL,0.023,0.,0.)
      CALL SET(0.13,0.93,YPOS-0.2,YPOS,0.0,1., YB, YT, 1)
C
C  Draw a horizontal line at Y=0. using color index 2.
C
      XX(1) = 0.
      XX(2) = 1.
      YY(1) = 0.
      YY(2) = 0.
      CALL GSPLCI(2)
      CALL GPL(2,XX,YY)
      CALL GSPLCI(1)
C
C  Set Gridal parameters.
C
C
C   Set LTY to indicate that the Plotchar routine PLCHHQ should be used.
C
      CALL GASETI('LTY',1)
C
C   Size and format for X axis labels.
C
      CALL GASETR('XLS',0.02)
      CALL GASETC('XLF','(F3.1)')
C
C   Size and format for Y axis labels.
C
      CALL GASETR('YLS',0.02)
      CALL GASETC('YLF','(F5.1)')
C
C   Length of major tick marks for the X and Y axes.
C
      CALL GASETR('XMJ',0.02)
      CALL GASETR('YMJ',0.02)
C
      RETURN
      END
