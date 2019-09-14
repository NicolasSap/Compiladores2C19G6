cls
c:\GnuWin32\bin\flex Lexico.l
echo "Se genero el Flex"
pause
cls
c:\GnuWin32\bin\bison -dyv Sintactico.y
echo "Se genero el Sintactico"
pause
cls
echo "Se ejecutara el compilador"
c:\MinGW\bin\gcc.exe lex.yy.c y.tab.c -o TP_1.exe
pause
cls
TP_1.exe %1
pause
cls
del lex.yy.c
del y.tab.c
del y.output
del y.tab.h
del TP_1.exe
pause
cls
