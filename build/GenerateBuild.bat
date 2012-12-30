@echo off
goto menu

:menu
echo Build Project Generator:
echo.
echo [0] Clean
echo [1] Visual C++ 2012
echo [2] Visual C++ 2010
echo [3] CodeLite
echo [4] GNU Make
echo.

:choice
set /P C="Choice: "
if "%C%"=="4" goto gmake
if "%C%"=="3" goto codelite
if "%C%"=="2" goto vs2010
if "%C%"=="1" goto vs2012
if "%C%"=="0" goto clean

:clean
"../tools/premake4" --file=premake4.lua clean
"../tools/premake4" --file=premake4-deps.lua clean
goto quit

:vs2012
"../tools/premake4" --file=premake4.lua vs2012
"../tools/premake4" --file=premake4-deps.lua vs2012
goto quit

:vs2010
"../tools/premake4" --file=premake4.lua vs2010
"../tools/premake4" --file=premake4-deps.lua vs2010
goto quit

:codelite
"../tools/premake4" --file=premake4.lua codelite
"../tools/premake4" --file=premake4-deps.lua codelite
goto quit

:gmake
"../tools/premake4" --file=premake4.lua gmake
"../tools/premake4" --file=premake4-deps.lua gmake
goto quit

:quit
pause
:end