#include  <stdio.h>
#include "juego.h"

//mostrar pantalla de bienvenida y menu
static int mostrarMenu(){
    char opcion;
    printf("------------------------------------------------------------------\n");
    printf("BIENVENIDO AL BITQUEST!\n");
    printf("Simbolos:\n");
    printf("P: Jugador\tX: Enemigo\tK: LLave\t#: Pared\n");
    printf(".: Camino libre\tE: Salida\tD: Puerta\tM: Moneda\n");
    printf("1. Jugar\n");
    printf("2. Salir\n");
    printf("-------------------------------------------------------------------\n");
    printf("Ingrese su opcion: ");
    do{
        opcion = leerLetra();
    } while (opcion != '1' && opcion != '2');

    return (opcion == '1') ? 1 : 0;
}

//mostrar pantalla de victoria
static void mostrarVictoria(){
    limpiarPantalla();
    printf("\nFELICIDADES! HAS COMPLETADO TODOS LOS NIVELES!\n");
}

//Mostrar resumen final d el juego
static void mostrarResumenFinal (int monedasTotal, int monedasRec, int pasos, int nivelActual){
    long puntaje;
    puntaje = calcular_puntaje(monedasTotal, pasos, nivelActual);
    printf("---------------------------------------------------------\n");
    printf("Juego completado!\n");
    printf("Monedas totales: %d/%d\n", monedasRec, monedasTotal);
    printf("Pasos totales: %d\n", pasos);
    printf("Niveles completados: %d\n", nivelActual);
    printf("Puntaje final: %ld\n", puntaje);
    printf("---------------------------------------------------------\n");
    printf("Presione cualquier tecla para salir\n");
    leerLetra();
}

int main(){
    Juego juego;
    int monedasRec= 0;
    int monedasTotal= 0;
    int pasos= 0;
    int nivelCompletado= 0;
    int resultado;
    int nivelActual;
    char respuesta;

    //Menu de inicio
    if (!mostrarMenu()){
        printf ("Saliendo del juego...\n");
        return 0;
    }

    for (nivelActual = 0 ; nivelActual < 3 ; nivelActual++){

        reintentarNivel:
        resultado = ejecutarJuego(&juego, nivelActual);

        if (resultado == 0){
            printf ("\n Partida cancelada\n");  //el jugador presiono Q
            goto fin;
        }

        if (resultado == -1){   // el jugador perdio
            printf ("\nHas perdido :(, quieres reintentar el nivel %d? (s/n)): ", nivelActual + 1);
            do {
                respuesta= leerLetra();
            }while (respuesta != 's' && respuesta != 'n' && respuesta != 'S' && respuesta != 'N');

            if (respuesta == 's' || respuesta == 'S'){
                goto reintentarNivel;
            } else {
                    printf ("\nPartida terminada\n");
                    goto fin;
            }
        }

        //resultado == 1: nivel completado normalmente
        monedasRec += juego.monedasRec;
        monedasTotal += juego.monedasTotal;
        pasos += juego.pasos;
        nivelCompletado++;
    }

    //pantalla de victoria
    if (nivelCompletado == 3){
        mostrarVictoria();
    }

    fin:
    //el resumen final siempre se muestra, inlcuso si no termino
    if (nivelCompletado > 0){
        limpiarPantalla();
        printf("\n");
        mostrarResumenFinal(monedasTotal, monedasRec, pasos, nivelCompletado);
    }

    return 0;
}