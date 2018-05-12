@echo off

set CFLAGS=-Od -nologo -fp:fast -fp:except- -Gm- -GR- -EHa- -Zo -Oi -WX -W4 -wd4201 -wd4100 -wd4189 -wd4505 -wd4127 -FC -Z7 -GS- -Gs9999999
set LDFLAGS=-incremental:no -opt:ref user32.lib gdi32.lib winmm.lib opengl32.lib kernel32.lib

set RENDER_EXPORTS=/EXPORT:Win32SetupRenderContext /EXPORT:Win32SelectRenderContext /EXPORT:InitializeRenderer /EXPORT:ResizeWindow /EXPORT:RenderFrame

IF NOT EXIST ..\build mkdir ..\build
pushd ..\build

del *.pdb > NUL 2> NUL

echo "building" > lock.tmp
cl %CFLAGS% ..\code\game.cpp -Fmgame.map -LD /link -PDB:game_%random%.pdb %LDFLAGS% glu32.lib
del lock.tmp
echo "building" > render_lock.tmp
cl %CFLAGS% ..\code\win32_opengl.cpp -Fmwin32_opengl.map -LD /link -PDB:win32_opengl_%random%.pdb %LDFLAGS% %RENDER_EXPORTS%
del render_lock.tmp
cl %CFLAGS% ..\code\win32_client.cpp -Fmwin32_game.map /link /SUBSYSTEM:windows %LDFLAGS%

popd
