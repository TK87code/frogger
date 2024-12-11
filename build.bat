@echo off

set files=..\code\main.c

set libs=C:\libs\raylib-5.0_win64_msvc16\lib\raylib.lib opengl32.lib kernel32.lib user32.lib shell32.lib gdi32.lib winmm.lib msvcrt.lib

set incpath=C:\libs\raylib-5.0_win64_msvc16\include

pushd ..\build
cl /Zi /Wall /wd4820 /wd5045 %files% /I %incpath% /link %libs% /OUT:"main.exe" /NODEFAULTLIB:LIBCMT /subsystem:windows /ENTRY:"mainCRTStartup"
popd

REM ..\release
REM cl %files% /I %incpath% /link %libs% /OUT:"main.exe" /NODEFAULTLIB:LIBCMT /subsystem:windows /ENTRY:"mainCRTStartup"
REM popd