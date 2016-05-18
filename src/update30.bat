@echo off

REM SETLOCAL assures environment variables created in a batch file are not exported to its calling environment
setlocal

echo Creating output directory tree

set CA_DEVEL_RESDIR=devel30\share\codeadapt
set CA_OUTPUT_RESDIR=output30\share\codeadapt

if not exist output md output\
if not exist output\share md output\share\
if not exist %CA_OUTPUT_RESDIR% md %CA_OUTPUT_RESDIR%\
if not exist %CA_OUTPUT_RESDIR%\lexers md %CA_OUTPUT_RESDIR%\lexers\
if not exist %CA_OUTPUT_RESDIR%\images md %CA_OUTPUT_RESDIR%\images\
if not exist %CA_OUTPUT_RESDIR%\images\settings md %CA_OUTPUT_RESDIR%\images\settings\
if not exist %CA_OUTPUT_RESDIR%\images\16x16 md %CA_OUTPUT_RESDIR%\images\16x16\
if not exist %CA_OUTPUT_RESDIR%\plugins md %CA_OUTPUT_RESDIR%\plugins\
if not exist %CA_OUTPUT_RESDIR%\templates md %CA_OUTPUT_RESDIR%\templates\
if not exist %CA_OUTPUT_RESDIR%\templates\wizard md %CA_OUTPUT_RESDIR%\templates\wizard\
if not exist %CA_OUTPUT_RESDIR%\scripts md %CA_OUTPUT_RESDIR%\scripts\
if not exist devel md devel\
if not exist devel\share md devel\share\
if not exist %CA_DEVEL_RESDIR% md %CA_DEVEL_RESDIR%\
if not exist %CA_DEVEL_RESDIR%\lexers md %CA_DEVEL_RESDIR%\lexers\
if not exist %CA_DEVEL_RESDIR%\images md %CA_DEVEL_RESDIR%\images\
if not exist %CA_DEVEL_RESDIR%\images\settings md %CA_DEVEL_RESDIR%\images\settings\
if not exist %CA_DEVEL_RESDIR%\images\16x16 md %CA_DEVEL_RESDIR%\images\16x16\
if not exist %CA_DEVEL_RESDIR%\plugins md %CA_DEVEL_RESDIR%\plugins\
if not exist %CA_DEVEL_RESDIR%\templates md %CA_DEVEL_RESDIR%\templates\
if not exist %CA_DEVEL_RESDIR%\templates\wizard md %CA_DEVEL_RESDIR%\templates\wizard\
if not exist %CA_DEVEL_RESDIR%\scripts md %CA_DEVEL_RESDIR%\scripts\

set ZIPCMD=zip

echo Packing core UI resources
%ZIPCMD% -j9 %CA_DEVEL_RESDIR%\resources.zip src\resources\*.xrc > nul
%ZIPCMD% -j9 %CA_DEVEL_RESDIR%\manager_resources.zip sdk\resources\*.xrc sdk\resources\images\*.png > nul
%ZIPCMD% -j9 %CA_DEVEL_RESDIR%\start_here.zip src\resources\start_here\*.* > nul
echo Packing plugins UI resources
%ZIPCMD% -j9 %CA_DEVEL_RESDIR%\astyle.zip plugins\astyle\resources\manifest.xml plugins\astyle\resources\*.xrc > nul
%ZIPCMD% -j9 %CA_DEVEL_RESDIR%\autosave.zip plugins\autosave\manifest.xml plugins\autosave\*.xrc > nul
%ZIPCMD% -j9 %CA_DEVEL_RESDIR%\classwizard.zip plugins\classwizard\resources\manifest.xml plugins\classwizard\resources\*.xrc > nul
%ZIPCMD% -j9 %CA_DEVEL_RESDIR%\compiler.zip plugins\compilergcc\resources\manifest.xml plugins\compilergcc\resources\*.xrc > nul
%ZIPCMD% -j9 %CA_DEVEL_RESDIR%\defaultmimehandler.zip plugins\defaultmimehandler\resources\manifest.xml plugins\defaultmimehandler\resources\*.xrc > nul
%ZIPCMD% -j9 %CA_DEVEL_RESDIR%\openfileslist.zip plugins\openfileslist\manifest.xml > nul
%ZIPCMD% -j9 %CA_DEVEL_RESDIR%\projectsimporter.zip plugins\projectsimporter\resources\manifest.xml plugins\projectsimporter\resources\*.xrc > nul
%ZIPCMD% -j9 %CA_DEVEL_RESDIR%\scriptedwizard.zip plugins\scriptedwizard\resources\manifest.xml > nul
%ZIPCMD% -j9 %CA_DEVEL_RESDIR%\todo.zip plugins\todo\resources\manifest.xml plugins\todo\resources\*.xrc > nul
%ZIPCMD% -j9 %CA_DEVEL_RESDIR%\xpmanifest.zip plugins\xpmanifest\manifest.xml > nul
echo Packing core UI bitmaps
cd src\resources
%ZIPCMD% -0 -q ..\..\%CA_DEVEL_RESDIR%\resources.zip images\*.png images\16x16\*.png > nul
cd ..\..\sdk\resources
%ZIPCMD% -0 -q ..\..\%CA_DEVEL_RESDIR%\manager_resources.zip images\*.png > nul
echo Packing plugins UI bitmaps
cd ..\..\plugins\compilergcc\resources
%ZIPCMD% -0 -q ..\..\..\%CA_DEVEL_RESDIR%\compiler.zip images\*.png images\16x16\*.png > nul
cd ..\..\..

echo Copying files
copy /y %CA_DEVEL_RESDIR%\*.zip %CA_OUTPUT_RESDIR% > nul
copy /y sdk\resources\lexers\lexer_* %CA_DEVEL_RESDIR%\lexers > nul
copy /y sdk\resources\lexers\lexer_* %CA_OUTPUT_RESDIR%\lexers > nul
copy /y src\resources\images\*.png %CA_DEVEL_RESDIR%\images > nul
copy /y src\resources\images\settings\*.png %CA_DEVEL_RESDIR%\images\settings > nul
copy /y src\resources\images\*.png %CA_OUTPUT_RESDIR%\images > nul
copy /y src\resources\images\settings\*.png %CA_OUTPUT_RESDIR%\images\settings > nul
copy /y src\resources\images\16x16\*.png %CA_DEVEL_RESDIR%\images\16x16 > nul
copy /y src\resources\images\16x16\*.png %CA_OUTPUT_RESDIR%\images\16x16 > nul
echo Makefile.am > excludes.txt
echo \.svn\ >> excludes.txt
xcopy /y /s plugins\scriptedwizard\resources\* %CA_DEVEL_RESDIR%\templates\wizard /EXCLUDE:excludes.txt >nul
xcopy /y /s plugins\scriptedwizard\resources\* %CA_OUTPUT_RESDIR%\templates\wizard /EXCLUDE:excludes.txt >nul
xcopy /y scripts\* %CA_DEVEL_RESDIR%\scripts /EXCLUDE:excludes.txt > nul
xcopy /y scripts\* %CA_OUTPUT_RESDIR%\scripts /EXCLUDE:excludes.txt > nul
del excludes.txt
copy /y tips.txt %CA_DEVEL_RESDIR% > nul
copy /y tips.txt %CA_OUTPUT_RESDIR% > nul
copy /y devel\*.exe output > nul
copy /y devel\*.dll output > nul
copy /y %CA_DEVEL_RESDIR%\plugins\*.dll %CA_OUTPUT_RESDIR%\plugins > nul
