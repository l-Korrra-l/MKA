@ECHO OFF
cd ../..
cd Generator
ml /c /coff /Zi asm.asm
link /OPT:NOREF /DEBUG asm.obj  /SUBSYSTEM:CONSOLE
call asm.exe			
