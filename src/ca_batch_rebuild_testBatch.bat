@echo off
rem ----------------------------------------
rem Setup C::B root folder of *binaries* (!)
rem ----------------------------------------
if not defined CB_ROOT set CB_ROOT=C:\Program Files\CodeBlocks
rem ------------------------------------------
rem Setup GCC root folder with "bin" subfolder
rem ------------------------------------------
if not defined GCC_ROOT set GCC_ROOT=C:\Apps32\MSys2\mingw32

rem -------------------------------------------
rem Usually below here no changes are required.
rem -------------------------------------------
if not exist "%CB_ROOT%"  goto ErrNoCB
if not exist "%GCC_ROOT%" goto ErrNoGCC
set PATH=%CB_ROOT%;%GCC_ROOT%;%PATH%


if not defined START_CMD set START_CMD=start "codeAdapt testBatch Build (wx 3.0.x)" /D"%~dp0" /min
set CB_EXE="%CB_ROOT%\codeblocks.exe"
if not defined CB_PARAMS set CB_PARAMS=-p msys2_clang --batch-build-notify --no-batch-window-close
set CB_CMD="%~dp0codeAdapt_testBatch_wx30-win32.cbp"

set CB_TARGET=--target=All
REM %START_CMD% %CB_EXE% %CB_PARAMS% %CB_TARGET% --clean %CB_CMD%

%START_CMD% %CB_EXE% %CB_PARAMS% %CB_TARGET% --rebuild %CB_CMD%
goto TheEnd

:ErrNoCB
echo Error: C::B root folder not found. Adjust batch file or supply parameter accordingly
goto TheEnd

:ErrNoGCC
echo Error: GCC root folder not found. Adjust batch file or supply parameter accordingly
goto TheEnd

:TheEnd
