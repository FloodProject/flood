@echo off
goto menu

:menu
echo Build Project Generator:
echo.
echo [0] Clean
echo [1] Visual C++ 2013
echo [2] Visual C++ 2013 (deps only)
echo [3] Flood
echo [4] GNU Make
echo.

:choice
set /P C="Choice: "
if "%C%"=="4" goto gmake
if "%C%"=="3" goto flood
if "%C%"=="2" goto vs2012deps
if "%C%"=="1" goto vs2012
if "%C%"=="0" goto clean

:clean
"../tools/premake5" --file=premake/premake4.lua clean
"../tools/premake5" --file=premake/premake4-deps.lua clean
goto quit

:vs2012
"../tools/premake5" --file=premake/premake4.lua vs2013
"../tools/premake5" --file=premake/premake4-deps.lua vs2013
goto quit

:vs2012deps
"../tools/premake5" --file=premake/premake4-deps.lua vs2013
goto quit

:flood
"../tools/premake5" --file=premake/premake4.lua flood
"../tools/premake5" --file=premake/premake4-deps.lua flood
goto quit

:gmake
"../tools/premake5" --file=premake/premake4.lua gmake
"../tools/premake5" --file=premake/premake4-deps.lua gmake
goto quit

:quit
pause
:end