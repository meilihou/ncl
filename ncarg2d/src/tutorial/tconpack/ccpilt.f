	PROGRAM CCPILT

        PARAMETER (M=40,N=40,LRWK=3500,LIWK=4000)
	REAL Z(M,N), RWRK(LRWK)
	INTEGER IWRK(LIWK)

	CALL GETDAT (Z, M, M, N)
C Open GKS
	CALL OPNGKS
	CALL GSCLIP (0)
C Set up label options
	CALL CPSETC('ILT - INFORMATION LABEL TEXT',
     +	'Modified Sine Function Contoured from $CMN$ to $CMX$ by $CIU$')
	CALL CPSETR('ILX - INFORMATION LABEL X COORDINATE',.5)
	CALL CPSETR('ILP - INFORMATION LABEL POSITION',0)
C Initialize Conpack
	CALL CPRECT(Z, M, M, N, RWRK, LRWK, IWRK, LIWK)
C Draw Perimeter
	CALL CPBACK(Z, RWRK, IWRK)
C Draw Contours
	CALL CPLBDR(Z,RWRK,IWRK)
	CALL CPCLDR(Z,RWRK,IWRK)

C Close frame and close GKS
	CALL FRAME
	CALL CLSGKS

	STOP
	END

	SUBROUTINE GETDAT (Z, K, M, N)

	REAL Z(K,N)
	INTEGER I,J,K,M,N

	OPEN (10,FILE='ccpex.dat',STATUS='OLD')
	L=K
	DO 10, I=1,L
  	  READ (10,*) (Z(I,J),J=1,N)
  10	CONTINUE

	RETURN
	END
