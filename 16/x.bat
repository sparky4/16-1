wpp /d1 /0 /ml dos_gfx lib\x\modex.lib
wlink file dos_gfx,lib\x\*.obj name dos_gfx
copy /y dos_gfx.exe c:\z\bakapi.exe 
