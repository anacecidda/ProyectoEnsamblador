#include <stdio.h>
#include <string.h>
#include <conio.h>

#include "mapa.h"
#include "juego.h"

#define ANSI_COLOR_RED    "\x1b[31m"    //ENEMIGO
#define ANSI_COLOR_GREEN "\033[32m"     //PARED
#define ANSI_COLOR_YELLOW "\033[33m"    //MONEDA
#define ANSI_COLOR_LIGHTBLUE "\033[94m"      //LLAVE
#define ANSI_COLOR_CIAN "\033[36m"      //PUERTA
#define ANSI_COLOR_BLUE "\033[34m"
#define ANSI_RESET "\033[0m"
//CAMINO LIBRE BLANCO

void inicializarJuego(Juego *juego, int num);
void limpiarPantalla();
void mostrarMapa(Juego *juego);
static int enemigoEnCelda(Juego *juego, int fila, int col);
void mostrarInstrucciones(Juego *juego);
char leerLetra();
void moverJugador(Juego *juego, char tecla);
void moverEnemigos(Juego *juego);
void verificarColisionEnemigos(Juego *juego);
int nivelTerminado(Juego *juego);
void mostrarResumen(Juego *juego);
int ejecutarJuego(Juego *juego, int num);

void inicializarJuego(Juego *juego, int num){
    int f, c;
    int totalCeldas= FILAS * COLUMNAS;
    long celdasLibres;

    memcpy(juego->mapa, mapas[num], totalCeldas);
    juego->nivelActual= num + 1;
    
    juego->jugadorFila= 1;
    juego->jugadorCol= 1;
    juego->jugadorVivo= 1;
    for (f=0; f<FILAS; f++){
        for (c=0; c<COLUMNAS; c++){
            if (juego->mapa[f][c]== JUGADOR){
                juego->jugadorFila= f;
                juego->jugadorCol= c;
                juego->mapa[f][c]= CAMINOLIBRE;
            }
        }
    }
    juego->monedasTotal = (int)contar_caracter((char *)juego->mapa, totalCeldas, MONEDA);
    celdasLibres = contar_celdas_libres((char *)juego->mapa, totalCeldas);

    //resetea contadores de nivel
    juego->monedasRec = 0;
    juego->pasos= 0;
    juego->tieneLlave= 0;

    //enemigos por nivel
    memset(juego->enemigos, 0, sizeof(juego->enemigos));
    if (num == 0){
        
        //nivel 1
        juego->numEnemigos= 1;
        juego->enemigos[0].fila= FILAS - 10;
        juego->enemigos[0].col= COLUMNAS - 10;
        juego->enemigos[0].pausa = 0;
        juego->enemigos[0].velocidad = 3;
        juego->enemigos[0].vivo= 1;
    } else if (num == 1){
        //nivel 2
        
       
        juego->numEnemigos= 2; 

        //enemigo 1
        juego->enemigos[0].fila= FILAS - 10;
        juego->enemigos[0].col= COLUMNAS - 10;
        juego->enemigos[0].vivo= 1;
        juego->enemigos[0].velocidad= 2;
        juego->enemigos[0].pausa= 0;

        //enemigo 2
        juego->enemigos[1].fila= FILAS - 10;
        juego->enemigos[1].col= COLUMNAS;
        juego->enemigos[1].vivo= 1;
        juego->enemigos[1].velocidad= 2;
        juego->enemigos[1].pausa= 1;         //desfase para que no se muevan juntos
    } else {
        //nivel 3
        juego->numEnemigos= 3;

        //enemigo 1
        juego->enemigos[0].fila= FILAS - 5;
        juego->enemigos[0].col= COLUMNAS - 5;
        juego->enemigos[0].vivo= 1;
        juego->enemigos[0].velocidad= 1;
        juego->enemigos[0].pausa= 0;

        //enemigo 2
        juego->enemigos[1].fila= FILAS / 2;
        juego->enemigos[1].col= COLUMNAS - 3;
        juego->enemigos[1].vivo= 1;
        juego->enemigos[1].velocidad= 1;
        juego->enemigos[1].pausa= 0;

        //enemigo 3
        juego->enemigos[2].fila= FILAS - 3;
        juego->enemigos[2].col= COLUMNAS / 2;
        juego->enemigos[2].vivo= 1;
        juego->enemigos[2].velocidad= 1;
        juego->enemigos[2].pausa= 0;
    }

    //ajuste de posiciones de enemigos a celdas libres
    for (int i=0; i< juego->numEnemigos; i++){
        int ef= juego->enemigos[i].fila;
        int ec= juego->enemigos[i].col;
        //si cae en pared busca la celda libre mas cercana
        while (juego->mapa[ef][ec] != CAMINOLIBRE && ef > 1){
            ef--;
        }
        juego->enemigos[i].fila= ef;
        juego->enemigos[i].col= ec;
    }

    //mensaje de inicio de nivel
    printf("\nNIVEL: %d\n", juego->nivelActual);
    printf("MONEDAS: %d\n", juego->monedasTotal);
    printf("ENEMIGOS: %d\n", juego->numEnemigos);
    printf ("Presiona cualquier letra para comenzar\n");
    leerLetra();
}

void limpiarPantalla(){
    printf ("\033[H\033[J");        //Secuencia de escape ANSI para limpiar pantalla y mover cursor a (0,0)
}

static void calcularVentana(Juego *juego, int *of, int *oc) {
    *of = juego->jugadorFila - FILASVIS / 2;
    *oc = juego->jugadorCol - COLUMNASVIS / 2;
 
    if (*of < 0)           *of = 0;
    if (*oc < 0)           *oc = 0;
    if (*of > FILAS-FILASVIS) *of = FILAS - FILASVIS;
    if (*oc > COLUMNAS-COLUMNASVIS) *oc = COLUMNAS  - COLUMNASVIS;
}

void mostrarMapa(Juego *juego){
    int of, oc, f, c;
    calcularVentana(juego, &of, &oc);

    for (f = of; f < of + FILASVIS; f++){
        for (c = oc; c < oc + COLUMNASVIS; c++){
            const char *color = "";
            char simbolo = juego->mapa[f][c];

            if (f == juego->jugadorFila && c == juego->jugadorCol) {
                simbolo = JUGADOR;
                color = ANSI_COLOR_BLUE;
            } else if (enemigoEnCelda(juego, f, c) >= 0) {
                simbolo = ENEMIGO;
                color = ANSI_COLOR_RED;
            } else {
                switch (juego->mapa[f][c]) {
                    case PARED:
                        color = ANSI_COLOR_GREEN;
                        break;
                    case MONEDA:
                        color = ANSI_COLOR_YELLOW;
                        break;
                    case LLAVE:
                        color = ANSI_COLOR_LIGHTBLUE;
                        break;
                    case PUERTA:
                        color = ANSI_COLOR_CIAN;
                        break;
                    default:
                        color = "";
                        break;
                }
            }

            printf("%s%c%s", color, simbolo, ANSI_RESET);
        }
        putchar('\n');
    }

    mostrarInstrucciones(juego);
}

static int enemigoEnCelda(Juego *juego, int fila, int col){
    for (int i=0; i< juego->numEnemigos; i++){
        if (juego->enemigos[i].vivo && juego->enemigos[i].fila == fila && juego->enemigos[i].col == col){
            return i;
        }
    }
    return -1;
}

void mostrarInstrucciones(Juego *juego){
    printf ("---------------------------------------------------------\n");
    printf ("Nivel: %d  |  Llave: %s  |  Monedas: %d/%d  |  Pasos: %d\n", juego->nivelActual, juego->tieneLlave ? "Si" : "No", juego->monedasRec, juego->monedasTotal, juego->pasos);
    printf ("----------------------------------------------------------\n");
    printf ("W= arriba  S= abajo  A= izquierda  D= derecha  Q= salir\n");

}

char leerLetra(){
    return (char)_getch();
}

void moverJugador(Juego *juego, char tecla){
    int nf= juego->jugadorFila;
    int nc= juego->jugadorCol;

    //movimiento
    switch (tecla){
        case 'w': case 'W': nf--; break;
        case 's': case 'S': nf++; break;
        case 'a': case 'A': nc--; break;
        case 'd': case 'D': nc++; break;
        default: return;
    }

    //limites del mapa
    if (nf < 0 || nf >= FILAS || nc < 0 || nc >= COLUMNAS) return;

    if (!validar_movimiento((char *)juego->mapa, COLUMNAS, nf, nc)) return;      //validar con nasm
    
    //validar puerta abierta con llave
    if (detectar_objeto((char *)juego->mapa, COLUMNAS, nf, nc, PUERTA)){
        if (!juego->tieneLlave){
            printf ("\007"); //beep: puerta bloqueada
            return;
        }
        juego->mapa[nf][nc]= CAMINOLIBRE;    //se abre la puerta
    }

    //recoger moneda o llave
    if (detectar_objeto((char *)juego->mapa, COLUMNAS, nf, nc, MONEDA)){
        juego->monedasRec++;
        juego->mapa[nf][nc]= CAMINOLIBRE;
    }

    if (detectar_objeto((char *)juego->mapa, COLUMNAS, nf, nc, LLAVE)){
        juego->tieneLlave= 1;
        juego->mapa[nf][nc]= CAMINOLIBRE;
    }

    juego->jugadorFila= nf;
    juego->jugadorCol= nc;
    juego->pasos++;

}

static void moverUnEnemigo(Juego *juego, int idx) {
    Enemigo *en = &juego->enemigos[idx];
    int df = juego->jugadorFila - juego->enemigos[idx].fila;  //diferencia de filas
    int dc = juego->jugadorCol - juego->enemigos[idx].col;      //diferencia de columnas
    int nf = en->fila;        //posicion tentativa de movimiento en filas
    int nc = en->col;         //posicion tentativa de movimiento en columnas
    int nf2, nc2;
    char celda;
 
    int dir_f = (df > 0) - (df < 0);   
    int dir_c = (dc > 0) - (dc < 0);   
 
    // si el jugador esta en la misma posicion no se mueve
    int abs_df = df < 0 ? -df : df;
    int abs_dc = dc < 0 ? -dc : dc;
 
    //intento 1: eje principal
    if (abs_df >= abs_dc) {
        nf = en->fila + dir_f;  
        nc = en->col;
    } else {
        nf = en->fila;
        nc = en->col + dir_c; 
    }

    if (nf < 1 || nf >= FILAS-1 || nc < 1 || nc >= COLUMNAS-1) {
        nf = en->fila; nc = en->col;
        goto intento2;
    }

    if (validar_movimiento((char *)juego->mapa, COLUMNAS, nf, nc)) {
        celda = juego->mapa[nf][nc];
        if (celda != SALIDA && celda != PUERTA) {
            en->fila = nf;
            en->col  = nc;
            return;
        }
    }
 
intento2:
    if (abs_df >= abs_dc) {
        nf2 = en->fila;
        nc2 = en->col + dir_c;
    } else {
        nf2 = en->fila + dir_f;
        nc2 = en->col;
    }
 
    if (nf2 < 1 || nf2 >= FILAS-1 || nc2 < 1 || nc2 >= COLUMNAS-1) return;
 
    if (validar_movimiento((char *)juego->mapa, COLUMNAS, nf2, nc2)) {
        celda = juego->mapa[nf2][nc2];
        if (celda != SALIDA && celda != PUERTA) {
            en->fila = nf2;
            en->col  = nc2;
        }
    }
    //si nunguno funciona el enemigo se queda en su lugar
}

//mover al enemigo
void moverEnemigos(Juego *juego){
    for (int i = 0; i < juego->numEnemigos; i++){
        if (!juego->enemigos[i].vivo) continue;

        juego->enemigos[i].pausa++;
        if (juego->enemigos[i].pausa >= juego->enemigos[i].velocidad){
            juego->enemigos[i].pausa= 0;
            moverUnEnemigo(juego, i);
        }
    }
}

void verificarColisionEnemigos(Juego *juego){
    for (int i=0; i < juego->numEnemigos;i++){
        if (!juego->enemigos[i].vivo) continue;
        if (juego->enemigos[i].fila == juego->jugadorFila && juego->enemigos[i].col == juego->jugadorCol){
            juego->jugadorVivo= 0;
            return;
        }
    }
}

int nivelTerminado(Juego *juego){
    return (int)detectar_objeto((char *) juego->mapa, COLUMNAS, juego->jugadorFila, juego->jugadorCol, SALIDA);
}

void mostrarResumen(Juego *juego){
    printf("\n");
    printf("---------------------------------------------------------\n");
    printf("Nivel: %d  |  Llave %s  |  Monedas: %d/%d  |  Pasos: %d\n", juego->nivelActual, juego->tieneLlave ? "Si" : "No", juego->monedasRec, juego->monedasTotal, juego->pasos);
    printf("---------------------------------------------------------\n");
    printf("Presione cualquier tecla para continuar\n");
    leerLetra();
}
int ejecutarJuego(Juego *juego, int num){
    char tecla;

    inicializarJuego(juego, num);

    while (1){
        limpiarPantalla();
        
        if (!juego->jugadorVivo){
            printf("Has sido atrapado por un enemigo! Fin del juego.\n");
            printf ("Presiona cualquier tecla para salir\n");
            leerLetra();
            return -1;
        }

        mostrarMapa(juego);

        tecla= leerLetra();
        if (tecla == 'q' || tecla == 'Q'){
            printf("Saliendo del juego\n");
            return 0;
        }

        //mueve al jugador
        moverJugador(juego, tecla);

        //mueve enemigos
        moverEnemigos(juego);

        //verifica si el enemigo atrapo al jugador
        verificarColisionEnemigos(juego);
        if (!juego->jugadorVivo) continue;

        if (nivelTerminado(juego)){
            limpiarPantalla();
            mostrarMapa(juego);
            mostrarResumen(juego);
            return 1;
        }
    }
}