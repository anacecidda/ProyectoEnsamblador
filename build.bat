@echo off
REM ==================================================
REM build.bat - BitQuest: Laberinto
REM Compilacion para Windows con NASM + GCC de 64 bits
REM
REM Requisitos:
REM     - NASM 2.x en el PATH
REM     - GCC 64 bits en el PATH  (MinGW-w64 o MSYS2)
REM ==================================================

echo [1/3] Compilando rutinas.asm con NASM
nasm -f win64 rutinas.asm -o rutinas.obj -DWIN64
if errorlevel 1(
    echo Error: fallo la compilacion del NASM
    pasue
    exit /b 1
)

echo [2/3] Compilando y linkeando con GCC
gcc main.c juego.c rutinas.obj -o BitQuest.exe
if errorlevel 1 (
    echo Error: fallo la compilacion de GCC
    pause
    exit /b 1
)

echo [3/3] Ejecutando BitQuest.exe
BitQuest.exe