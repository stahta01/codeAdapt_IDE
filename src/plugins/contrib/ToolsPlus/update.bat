@echo off
md ..\..\..\devel\share\codeadapt\images\settings > nul 2>&1
md ..\..\..\output\share\codeadapt\images\settings > nul 2>&1
copy .\Resources\*.png ..\..\..\devel\share\codeadapt\images\settings\ > nul 2>&1
copy .\Resources\*.png ..\..\..\output\share\codeadapt\images\settings\ > nul 2>&1
exit 0
