*ndvPlotStyleName : vs. longitude (zero contour emphasized)
*ndvPlotName      : time_plot
*ndvData          : (/ cndata : 2 /)
*ndvFuncFiles     : (/ ../csm_utils.ncl /)

;
; Define the objects we created.
;
*ndvObjects : (/ \
  sf             : scalarFieldClass, \
  contour        : contourPlotClass, \
  left_title     : textItemClass, \
  center_title   : textItemClass, \
  right_title    : textItemClass \
/)   

;
; Describe the data.
;
*cndata@Pattern     : (/ /.*T.*/i , /.*/ /)
*cndata!-1@Pattern  : /.*lon.*/i
*cndata!-2@Pattern  : /.*time.*/i
*cndata@Description : contour data
*cndata@Required    : True

;
; Scalar field setup.
;
*sf@sfDataArray         : $cndata$
*sf@sfDataArray%Profile : (/ Name : Primary Data Var /)
*sf@sfXArray            : $cndata$&-1
*sf@sfXArray%Profile    : (/ Name : Longitude /)

;
; contour resources
;
*contour@cnScalarFieldData          : $sf$
*contour@tiYAxisString              : $cndata$&-2@long_name
*contour*cnLineLabelBackgroundColor : transparent
*contour@pmAnnoViews    : (/ $left_title$, $center_title$, $right_title$ /)
*contour@ndvUpdateFunc  : LabelLon($contour$,60,20)
*contour@ndvUpdateFunc%Profile : (/ Name : Longitude Tickmarks on X Axis /)
*contour@ndvUpdateFunc2 : PlotTitles($contour$, \
                                      $left_title$,$cndata$@long_name, \
                                      $center_title$, "", \
                                      $right_title$, $cndata$@units, \
                                      0.024,1)
*contour@ndvUpdateFunc2%Profile : (/ Name : Plot Titles /)
*contour@ndvUpdateFunc3 : SetContourLevels($contour$,0.,0.,0.)
*contour@ndvUpdateFunc3%Profile : (/ Name : Contour Level Control /)
*contour@ndvUpdateFunc4 : LtGtContourDashPattern($contour$,0.,1,0.,-2)
*contour@ndvUpdateFunc4%Profile : (/ Name : Contour Dash Patterns /)
*contour@ndvUpdateFunc5 : SetContourLevelThickness($contour$,0.,2.)
*contour@ndvUpdateFunc5%Profile : (/ Name : Contour Level Thickness /)

;
; Title resources
;
*left_title*amZone             : 3
*left_title*amSide             : Top
*left_title*amParallelPosF     : 0.0
*left_title*amOrthogonalPosF   : 0.05
*left_title*amJust             : BottomLeft
*left_title*amResizeNotify     : True

*center_title*amZone           : 3
*center_title*amSide           : Top
*center_title*amParallelPosF   : 0.5
*center_title*amOrthogonalPosF : 0.05
*center_title*amJust           : BottomCenter
*center_title*amResizeNotify   : True

*right_title*amZone            : 3
*right_title*amSide            : Top
*right_title*amParallelPosF    : 1.0
*right_title*amOrthogonalPosF  : 0.05
*right_title*amJust            : BottomRight
*right_title*amResizeNotify    : True

;
; Other resources.
;
*vpXF                 : 0.20
*vpYF                 : 0.85
*vpWidthF             : 0.70
*vpHeightF            : 0.70
*vpUseSegments        : True
*MaxLevelCount        : 20
*Font                 : helvetica-bold
*TextFuncCode         : ~