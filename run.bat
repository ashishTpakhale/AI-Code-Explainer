@echo off
setlocal
set PATH=C:\msys64\ucrt64\bin;C:\msys64\usr\bin;%PATH%
"%~dp0ai_codebase_explainer.exe" %*
endlocal
