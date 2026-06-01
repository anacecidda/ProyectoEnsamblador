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
#define MONDEDA 'M'
#define LLAVE 'K'
#define PUERTA 'D'
#define SALIDA 'E'
#define ENEMIGO 'X'

#define MAXENEMIGOS 5

typedef struct {
     int fila;
     int col;
} Pos;

typedef struct {
     bool vivo;          //1= vivo 0= eliminado
     int pausa;          //contador de pausa para el movimiento del enemigo
     int velocidad;      //cada cuantos movimientos se mueve (1= rapido, 2= medio, 3= lento)
} Enemigo;

typedef struct {
     char mapa[FILAS][COLUMNAS];
     Pos jugador;
     Pos enemigo;
     int monedasRec;
     int monedasTotal;
     int pasos;
     int nivelActual;
     bool tieneLlave;
     Enemigo enemigos[MAXENEMIGOS];
     int numEnemigos;         //por nivel
     bool jugadorVivo;        //1= vivo 0= muerto
} Juego;

//prototipos de nasm

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
void ejecutarJuego(Juego *juego, int num);

#endif //JUEGO_H