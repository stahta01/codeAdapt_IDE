@echo off
mkdir ..\..\..\devel\share\codeadapt\lib_finder > nul 2>&1
mkdir ..\..\..\output\share\codeadapt\lib_finder > nul 2>&1
copy /Y lib_finder\*.xml ..\..\..\devel\share\codeadapt\lib_finder > nul 2>&1
copy /Y lib_finder\*.xml ..\..\..\output\share\codeadapt\lib_finder > nul 2>&1
zip -j9 ..\..\..\devel\share\codeadapt\lib_finder.zip manifest.xml
