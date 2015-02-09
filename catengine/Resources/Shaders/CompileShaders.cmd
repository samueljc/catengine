@echo off

set error=0

call :CompileShader Basic VSBasic vs
call :CompileShader Basic PSBasic ps

if %error% == 0 (
  echo compilation success
) else (
  echo compilation error
)

exit /b

:CompileShader
set fxc="C:\Program Files (x86)\Windows Kits\8.1\bin\x64\fxc.exe" /Zi /nologo /T%3_4_0 /E%2 /FhCompiled\%1_%2.inc /Vn%1_%2 %1.hlsl
echo %fxc%
%fxc% || set error=1
exit /b

:CleanCompileShader
set fxc="C:\Program Files (x86)\Windows Kits\8.1\bin\x64\fxc.exe" /nologo /T%3_4_0 /Qstrip_reflect /Qstrip_debug /FhCompiled\%1_%2.inc /Vn%1_%2 %1.hlsl
echo %fxc%
%fxc% || set error=1
exit /b