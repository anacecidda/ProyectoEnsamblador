# ProyectoEnsamblador
Proyecto Final ensamblador

Herramientas:
- gcc
- nasm

- Para compilar (en Windows)
    - Doble click en build.bat o desde la terminal "build.bat"

Controles: 
W: mover arriba
A: moverizquierda
S: mover abajo
D: mover derecha
Q: salir del juego

Estructura del proyecto:
- main.c: punto de entarda, menu y resumen final.
- juego.c: logica del juego
- juego.h: constantes, structs y prototipos
- mapas.h: mapas de 60x60
- rutinas.asm: rutinas en nasm
- build.bat: compilacion en windows
- README: este archivo

Funciones nasm implementadas:
- contar_caracter: cuenta ocurrencias de un carater en el mapa
- validar_movimiento: valida si una celda es transitable
- calcular_puntaje: utiliza la formula (monedas * 100) - (pasos *2) + (niveles * 500)
- contar_celdas_libres: cuenta los '.' del mapa
