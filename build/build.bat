@echo off
goto menu

:menu
echo Build Project Generator:
echo.
echo [0] Clean
echo [1] Visual C++ 2010
echo [2] Visual C++ 2008
echo [3] CodeLite
echo [4] GNU Make
echo.

:choice
set /P C="Choice: "
if "%C%"=="4" goto GNU Make
if "%C%"=="3" goto codelite
if "%C%"=="2" goto vs2008
if "%C%"=="1" goto vs2010
if "%C%"=="0" goto clean

:clean
"../tools/premake4" --file=premake4.lua clean
goto quit

:vs2010
"../tools/premake4" --file=premake4.lua vs2010
goto quit

:vs2008
"../tools/premake4" --file=premake4.lua vs2008
goto quit

:codelite
"../tools/premake4" --file=premake4.lua codelite
goto quit

:codelite
"../tools/premake4" --file=premake4.lua gmake
goto quit

:quit
pause
:end