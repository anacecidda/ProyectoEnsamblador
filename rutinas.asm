section .text

;exporta los simbolos para que puedan ser usados en otros archivos
global contar_caracter
global validar_movimiento
global calcular_puntaje
global detectar_objeto
global contar_celdas_libres

;recorre un bloque de memoria y cuenta cuantas veces aparece un caracter especifico
contar_caracter:        
    ;rcx= char *mapa
    ;rdx= int totalCEldas
    ;r8= char caracter
    ;rax: cantidad de veces que aparece el caracter
    
    push rbp   
    mov rbp, rsp

%ifdef WIN64      
    mov rdi, rcx       
    mov rsi, rdx    
    movzx rdx, r8b  
%else
    movzx rdx, dl   ;extiende el byte objetivo a 64 bits
%endif

    xor rax, rax     ;inicializa el contador a 0   
    xor rcx, rcx      ;indice=0

.loop_contar:
    cmp rcx, rsi         ;indice >= total
    jge .fin_contar      ; si, termina

    movzx r10, byte [rdi + rcx]     ;lee un byte del mapa
    cmp r10, rdx        ; compara si es el caracter que se esta buscando
    jne .sig_contar     ;no: siguiente celda
    inc rax             ;si: incrementa el contador

.sig_contar:
    inc rcx
    jmp .loop_contar

.fin_contar:
    pop rbp
    ret

;verifica si una posicion del mapa puede ser ocupada por el jugador o el enemigo
validar_movimiento: 
    ;rcx= char *mapa
    ;rdx= int columnas
    ;r8= int filas
    ;r9= int cols

    ;rax: 1(movimiento valido, no es pared), 0(movimiento bloqueado, es pared)

    push rbp
    mov rbp, rsp

%ifdef WIN64
    mov rdi, rcx    ;mapa
    mov rsi, rdx    ;cols
    mov rdx, r8     ;nueva_f
    mov rcx, r9     ;nueva_c
%endif

    ;calcula indice lineal: nueva_f * cols + nueva_c
    imul rdx, rsi   ;rdx= nueva_f * cols
    add rdx, rcx    ;rdx= nueva_f * cols + nueva_c

    movzx rax, byte [rdi + rdx]     ;lee el caracter en esa posicion

    cmp al, '#'     ;compara con '#' (ASCII 0x23)
    je .bloqueado

    mov rax, 1      ;cualquier otro caracter:  movimiento valido
    jmp .fin_validar

.bloqueado:
    xor rax, rax  ;retorna 0  

.fin_validar:
    pop rbp
    ret

;calcula el puntaje final del jugdor con una formula
;puntaje= (monedas * 100)- (pasos*2) + (niveles*500)
calcular_puntaje:
    ;rcx= int monedas
    ;rdx= int pasos
    ;r8= int niveles

    push rbp
    mov rbp, rsp

%ifdef WIN64
    mov rdi, rcx        ;monedas
    mov rsi, rdx        ;pasos
    mov rdx, r8         ;niveles
%endif

    ;multiplicar monedas * 100
    imul rdi, rdi, 100      ;rsi= monedas * 100

    ;multiplicar pasos * 2
    imul rsi, rsi, 2       ;rsi= pasos * 2

    ;multiplicar niveles * 500
    imul rdx, rdx, 500      ;rdx= niveles * 500

    ;puntaje= (monedas*100)-(pasos*2)+(niveles*500)
    mov rax, rdi
    sub rax, rsi
    add rax, rdx

    pop rbp
    ret

;verifica si una celda especifica del mapa contiene un determinado caracter
;(moneda, llave, puerta, salida, etc)

detectar_objeto:
    ;rcx= char *mapa
    ;rdx= int cols
    ;r8= int jugadorFila 
    ;r9= int jugadorCol

    ;rax: 1(objeto encontrado), 0(objeto no encontrado)

    push rbp
    mov rbp, rsp

%ifdef WIN64

    mov rdi, rcx    ;mapa
    mov rsi, rdx    ;cols
    mov rdx, r8     ;fila
    mov rcx, r9     ;columna 
    movzx r10, byte [rbp + 48]     ;objeti (5to arg en pila)
%else
    movzx r10, r8b    ; objeto(byte)
%endif

    ;calcula indice: fila * cols + col
    imul rdx, rsi       ;rdx= fila * cols
    add rdx, rcx        ;rdx += col

    movzx rax, byte [rdi + rdx]     ;lee el caracter en la celda

    ;compara con el objeto buscado
    cmp al, r10b
    je .encontrado

    xor rax, rax
    jmp .fin_detectar

.encontrado:
    mov rax, 1      ;encontrado: retorna 1

.fin_detectar:
    pop rbp
    ret

;cuenta cuantas celdas de camino libre '.' existen en el mapa.
contar_celdas_libres:
    push rbp
    mov rbp, rsp

%ifdef WIN64
    mov rdi, rcx        ;mapa
    mov rsi, rdx        ;total
%endif

    xor rax, rax        ;contador = 0
    xor rcx, rcx        ;indice = 0

.loop_libres:
    cmp rcx, rsi
    jge .fin_libres

    movzx r10, byte [rdi + rcx]
    cmp r10b, '.'       ; compara es camino libre
    jne .sig_libres
    inc rax             ;si: incrementa contador

.sig_libres:
    inc rcx
    jmp .loop_libres

.fin_libres:
    pop rbp
    ret


