C
C $Id: mapaci.f,v 1.6 1994-03-17 02:42:49 kennison Exp $
C
      FUNCTION MAPACI (IAI)
C
C Given an integer area identifier IAI generated by MAPBLA, the value
C of MAPACI is an appropriate color index for the area.
C
C
C Define the array of color indices.
C
      DIMENSION ICI(1361)
C
      DATA (ICI(I),I=   1, 100) /
     +    2,1,2,2,2 , 2,2,2,2,2 , 2,2,2,2,2 , 2,2,2,2,2 , 2,2,2,2,2 ,
     +    1,2,2,2,2 , 1,2,2,2,1 , 2,2,2,2,2 , 1,2,1,1,2 , 1,2,2,2,2 ,
     +    1,2,2,2,1 , 2,2,2,1,2 , 2,2,2,2,2 , 1,2,2,2,2 , 2,2,2,2,2 ,
     +    2,2,2,2,2 , 2,2,2,2,2 , 2,2,2,2,2 , 2,2,2,2,2 , 2,2,2,2,2 /
      DATA (ICI(I),I= 101, 200) /
     +    2,2,2,2,2 , 2,2,2,2,2 , 2,2,2,2,2 , 2,1,2,2,2 , 2,2,1,2,1 ,
     +    1,1,2,1,1 , 1,2,2,1,2 , 2,2,2,2,2 , 2,2,1,2,2 , 2,1,1,2,2 ,
     +    2,2,2,2,1 , 2,2,2,2,2 , 2,2,2,2,2 , 2,2,2,2,2 , 2,2,2,2,2 ,
     +    2,2,2,2,2 , 2,2,2,2,2 , 2,2,2,2,2 , 2,2,2,2,2 , 2,2,2,2,2 /
      DATA (ICI(I),I= 201, 300) /
     +    2,2,2,2,2 , 2,2,2,2,2 , 2,2,2,2,2 , 2,2,2,2,2 , 2,2,7,2,3 ,
     +    4,2,2,2,2 , 2,2,2,4,4 , 1,5,4,4,4 , 4,6,5,1,2 , 1,3,1,4,5 ,
     +    6,2,5,2,3 , 3,4,2,2,4 , 1,6,2,1,1 , 4,2,4,4,5 , 4,4,1,1,1 ,
     +    1,3,1,4,1 , 1,4,2,4,4 , 2,4,2,2,1 , 4,2,2,1,3 , 4,4,4,6,4 /
      DATA (ICI(I),I= 301, 400) /
     +    4,4,4,4,4 , 4,2,2,4,5 , 5,1,4,2,2 , 2,2,2,1,6 , 2,4,1,4,2 ,
     +    2,1,4,1,2 , 2,2,1,2,2 , 1,2,3,1,4 , 4,2,4,4,4 , 4,1,4,4,4 ,
     +    4,6,4,2,2 , 3,1,4,4,4 , 4,4,4,4,4 , 4,4,4,4,4 , 2,4,4,4,2 ,
     +    4,2,4,2,4 , 4,2,4,4,2 , 1,2,4,2,2 , 4,4,5,4,4 , 4,4,4,4,2 /
      DATA (ICI(I),I= 401, 500) /
     +    2,1,4,4,2 , 3,1,3,5,4 , 1,2,2,1,1 , 1,1,6,3,3 , 6,6,6,6,6 ,
     +    1,5,4,4,1 , 5,5,5,5,5 , 5,5,5,5,7 , 1,6,6,3,6 , 3,3,3,3,3 ,
     +    3,3,7,3,3 , 3,3,3,3,7 , 3,6,6,6,6 , 1,2,3,4,6 , 2,2,2,2,2 ,
     +    2,2,4,4,1 , 5,4,4,6,6 , 4,4,6,6,1 , 6,5,6,1,2 , 1,3,6,6,1 /
      DATA (ICI(I),I= 501, 600) /
     +    1,4,6,5,6 , 2,6,6,6,6 , 5,2,3,1,3 , 6,4,1,1,6 , 1,2,2,4,6 ,
     +    1,6,6,6,2 , 1,1,4,2,6 , 4,4,5,4,4 , 1,1,1,1,3 , 1,4,1,3,1 ,
     +    4,2,3,4,4 , 2,4,2,2,1 , 4,6,2,2,1 , 3,4,6,4,4 , 4,5,6,4,4 ,
     +    4,4,4,4,4 , 2,2,2,4,5 , 5,1,6,4,6 , 2,2,2,2,2 , 2,1,6,2,4 /
      DATA (ICI(I),I= 601, 700) /
     +    1,4,4,2,2 , 1,4,1,2,6 , 2,2,1,2,2 , 4,5,1,2,3 , 1,4,4,6,2 ,
     +    4,4,4,4,5 , 1,3,4,6,4 , 4,4,6,4,2 , 2,6,6,3,1 , 4,2,4,4,5 ,
     +    4,4,4,4,4 , 4,4,4,4,4 , 6,2,4,4,4 , 7,2,4,7,2 , 4,2,4,4,2 ,
     +    4,4,2,1,2 , 4,2,2,4,4 , 5,4,4,4,4 , 4,4,2,2,2 , 1,4,4,2,6 /
      DATA (ICI(I),I= 701, 800) /
     +    2,2,3,1,6 , 3,5,4,4,3 , 1,2,3,2,1 , 1,1,2,1,3 , 3,3,6,3,6 ,
     +    6,6,6,1,5 , 4,4,1,4,3 , 5,5,5,3,5 , 5,5,5,5,5 , 3,6,7,6,7 ,
     +    5,6,3,3,4 , 5,3,7,7,6 , 2,5,7,3,7 , 5,7,2,3,6 , 4,2,4,2,2 ,
     +    5,2,2,6,5 , 6,3,4,4,2 , 2,4,5,2,2 , 2,5,2,3,3 , 5,6,5,3,4 /
      DATA (ICI(I),I= 801, 900) /
     +    2,6,4,3,4 , 5,5,4,6,5 , 2,6,3,3,3 , 5,6,4,4,3 , 2,3,2,4,2 ,
     +    5,2,2,4,4 , 2,6,6,1,4 , 2,5,5,5,5 , 2,3,6,2,6 , 3,3,2,6,3 ,
     +    5,6,5,2,6 , 3,3,6,1,2 , 5,1,4,2,3 , 1,4,6,1,3 , 2,4,5,1,2 ,
     +    1,1,3,5,3 , 4,6,2,3,5 , 3,6,5,6,4 , 1,6,2,4,6 , 4,6,6,6,3 /
      DATA (ICI(I),I= 901,1000) /
     +    6,4,3,1,6 , 3,4,6,2,3 , 1,5,1,4,5 , 3,3,4,3,6 , 3,5,5,5,5 ,
     +    6,6,2,4,1 , 5,5,5,5,5 , 6,6,5,3,3 , 5,5,4,5,5 , 5,4,2,4,4 ,
     +    4,5,5,5,4 , 2,5,5,4,5 , 5,5,3,3,5 , 5,3,5,3,5 , 5,3,6,6,3 ,
     +    6,6,3,6,3 , 6,6,3,6,3 , 3,3,6,3,3 , 6,3,7,6,3 , 3,6,3,6,6 /
      DATA (ICI(I),I=1001,1100) /
     +    3,3,2,7,1 , 6,6,3,6,3 , 3,3,3,3,3 , 3,7,3,3,3 , 3,3,3,7,3 ,
     +    6,6,6,6,1 , 3,6,6,6,6 , 1,6,6,6,6 , 1,7,6,6,6 , 6,1,6,1,1 ,
     +    6,1,6,6,6 , 6,3,1,3,6 , 4,6,5,2,1 , 6,6,2,4,6 , 1,6,4,5,6 ,
     +    5,3,6,6,6 , 1,2,6,6,5 , 6,6,7,7,2 , 2,6,2,6,3 , 4,3,3,2,3 /
      DATA (ICI(I),I=1101,1200) /
     +    4,3,3,3,6 , 7,6,7,5,6 , 3,3,4,5,3 , 7,7,6,2,5 , 7,3,7,5,7 ,
     +    2,3,6,4,2 , 4,2,2,5,2 , 2,6,5,6,3 , 4,4,2,2,4 , 5,2,2,2,5 ,
     +    2,3,3,5,6 , 5,3,4,2,6 , 4,3,4,5,5 , 4,6,5,2,6 , 3,3,3,5,6 ,
     +    4,4,3,2,3 , 2,4,2,5,2 , 2,4,4,2,6 , 6,1,4,2,5 , 5,5,5,2,3 /
      DATA (ICI(I),I=1201,1300) /
     +    6,2,6,3,3 , 2,6,3,5,6 , 5,2,6,3,3 , 6,1,2,5,1 , 4,2,3,1,4 ,
     +    6,1,3,2,4 , 5,1,2,1,1 , 3,5,3,4,6 , 2,3,5,3,6 , 5,6,4,1,6 ,
     +    2,4,6,4,6 , 6,6,3,6,4 , 3,1,6,3,4 , 6,2,3,1,5 , 1,4,5,3,3 ,
     +    4,3,6,3,5 , 5,5,5,6,6 , 2,4,1,5,5 , 5,5,5,6,6 , 5,3,3,5,5 /
      DATA (ICI(I),I=1301,1361) /
     +    4,5,5,5,4 , 2,4,4,4,5 , 5,5,4,2,5 , 5,4,5,5,5 , 3,3,5,5,3 ,
     +    5,3,5,5,3 , 6,6,3,6,6 , 3,6,3,6,6 , 3,6,3,3,3 , 6,3,3,6,3 ,
     +    7,6,3,3,6 , 3,6,6,3,3 , 2 /
C
C Check for an uncleared prior error.
C
      IF (.NOT.(ICFELL('MAPACI - UNCLEARED PRIOR ERROR',1).NE.0))
     +GO TO 10000
      MAPACI=1
      RETURN
10000 CONTINUE
C
C Pull out the appropriate color index, taking precautions to prevent
C an out-of-array reference.
C
      IF (.NOT.(IAI.GE.1.AND.IAI.LE.1361)) GO TO 10001
      MAPACI=ICI(IAI)
      GO TO 10002
10001 CONTINUE
      MAPACI=1
10002 CONTINUE
C
C Done.
C
      RETURN
C
      END
