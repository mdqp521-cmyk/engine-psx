re4-ps1-engine <sys/types.h>
#include <libgte.h>
#include <libgpu.h>
#include <libetc.h>
#include <libpad.h>

#define SCREEN_WIDTH  320
#define SCREEN_HEIGHT 240

DISPENV disp;
DRAWENV draw;
int db = 0;

// Variables para el control y las coordenadas de Leon
u_long pad_state;
int leon_x = 0;
int leon_z = 0;
int leon_angulo = 0;

void init_game_system() {
    ResetGraph(0);
    SetDefDispEnv(&disp, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
    SetDefDrawEnv(&draw, 0, 240, SCREEN_WIDTH, SCREEN_HEIGHT);
    SetDefDispEnv(&disp, 0, 240, SCREEN_WIDTH, SCREEN_HEIGHT);
    SetDefDrawEnv(&draw, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
    setRGB0(&draw, 0, 0, 0);
    PutDispEnv(&disp[db]);
    PutDrawEnv(&draw[draw]);
    SetDispMask(1);
    
    // Inicializamos el sistema de controles de la PS1
    PadInit(0);
}

void procesar_input() {
    // Leemos el estado del control en el puerto 1
    pad_state = PadRead(0);
    
    // Movimiento de tanque estilo RE4 Mobile
    if (pad_state & PADLup) {
        leon_z += 4; // Camina hacia adelante
    }
    if (pad_state & PADLdown) {
        leon_z -= 4; // Camina hacia atras
    }
    if (pad_state & PADLleft) {
        leon_angulo -= 2; // Gira sobre su eje a la izquierda
    }
    if (pad_state & PADLright) {
        leon_angulo += 2; // Gira sobre su eje a la derecha
    }
}

int main() {
    init_game_system();
    
    // Bucle principal del juego
    while (1) {
        procesar_input();
        
        DrawSync(0);
        VSync(0);
        
        db = !db;
        PutDispEnv(&disp[db]);
        PutDrawEnv(&draw[db]);
    }
    
    return 0;
}
