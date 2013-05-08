form Display mean F1 and F2
   sentence filename
endform
filename_noext$ = replace$ (filename$, ".wav", "", 0)

Read from file... 'filename$'
objName$ = selected$ ("Sound")
select Sound 'objName$'
To Formant (burg)... 0 5 5500 0.0025 50
f1 = Get mean... 1 0 0 Hertz
f2 = Get mean... 2 0 0 Hertz
f3 = Get mean... 3 0 0 Hertz
clearinfo
f1small = f1/600
f2small = f2/1000
f3small = f3/3000
print 'f1small' 'f2small' 'f3small''newline$'