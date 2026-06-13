#ifndef JUEGO_H
#define JUEGO_H

//Medidas del mapa y lo que se muestra en la pantalla
#define FILAS 60
#define COLUMNAS 60
#define FILASVIS 20
#define COLUMNASVIS 20

//Objetos del juego
#define PARED '#'
#define CAMINOLIBRE '.'
#define JUGADOR 'P'
#define MONEDA 'M'
#define LLAVE 'K'
#define PUERTA 'D'
#define SALIDA 'E'
#define ENEMIGO 'X'

#define MAXENEMIGOS 5

typedef struct {
     int fila;
     int col;
     int vivo;          //1= vivo 0= eliminado
     int pausa;          //contador de pausa para el movimiento del enemigo
     int velocidad;      //cada cuantos movimientos se mueve (1= rapido, 2= medio, 3= lento)
} Enemigo;

typedef struct {
     char mapa[FILAS][COLUMNAS];
     int jugadorFila;
     int jugadorCol;
     int monedasRec;
     int monedasTotal;
     int pasos;
     int nivelActual;
     Enemigo enemigos[MAXENEMIGOS];
     int numEnemigos;         //por nivel
     int jugadorVivo;        //1= vivo 0= muerto
     int tieneLlave;        //1= tiene llave 0= no tiene llave
} Juego;

//prototipos de NASM
extern long contar_caracter(char *mapa, int totalCeldas, char caracter);
extern long validar_movimiento(char *mapa, int columnas, int fila, int col);
extern long calcular_puntaje(int monedasTotal, int pasos, int nivelActual);
extern long detectar_objeto(char *mapa, int columnas, int fila, int col, char caracter);
extern long contar_celdas_libres(char *mapa, int totalCeldas);

void inicializarJuego(Juego *juego, int num);

void limpiarPantalla();
void mostrarMapa(Juego *juego);
void mostrarInstrucciones(Juego *juego);

//input
char leerLetra();

//jugador
void moverJugador(Juego *juego, char tecla);

// enemigos
void moverEnemigos(Juego *juego);
void verificarColisionEnemigos(Juego *juego);

int nivelTerminado(Juego *juego);

void mostrarResumen(Juego *juego);

//bucle de un nivel
int ejecutarJuego(Juego *juego, int num);

#endif //JUEGO_H

//el ifndef y e, endif son  para evitar que se incluya el mismo header varias veces