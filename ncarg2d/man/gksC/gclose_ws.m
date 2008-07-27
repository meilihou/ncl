.\"
.\"	$Id: gclose_ws.m,v 1.15 2008-07-27 03:34:03 haley Exp $
.\"
.TH GCLOSE_WS 3NCARG "March 1993" UNIX "NCAR GRAPHICS"
.SH NAME
gclose_ws (Close workstation) - closes a workstation.
.SH SYNOPSIS
#include <ncarg/gks.h>
.sp
void gclose_ws(Gint ws_id);
.SH DESCRIPTION
.IP ws_id 12
(Input) - A number identifying the workstation to be
closed.  ws_id must be the same as that used in some previous gopen_ws call.
.SH USAGE
Close workstation updates the workstation and removes its identifier
from the set of open workstations.  The connection to the workstation
is released and the associated connection ID is available for re-use.
.SH ACCESS
To use the GKS C-binding routines, load the ncarg_gks and
ncarg_c libraries.
.SH SEE ALSO
Online: 
.BR gopen_gks(3NCARG),
.BR gopen_ws(3NCARG),
.BR gactivate_ws(3NCARG),
.BR gdeactivate_ws(3NCARG),
.BR gclose_gks(3NCARG),
.BR opngks(3NCARG),
.BR clsgks(3NCARG),
.BR gks(3NCARG),
.BR ncarg_gks_cbind(3NCARG)
.sp
Hardcopy: 
User's Guide for NCAR GKS-0A Graphics;
NCAR Graphics Fundamentals, UNIX Version
.SH COPYRIGHT
Copyright (C) 1987-2008
.br
University Corporation for Atmospheric Research
.br
The use of this Software is governed by a License Agreement.
