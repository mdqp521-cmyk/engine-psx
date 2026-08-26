#include <sys/types.h>
#include <stdio.h>
#include <libgte.h>
#include <libgpu.h>
#include <libetc.h>
#include <libpad.h>

#define SCREEN_WIDTH  320
#define SCREEN_HEIGHT 240

// Estructuras de doble buffer para evitar el parpadeo en la pantalla
DISPENV disp[2];
DRAWENV draw[2];
int db = 0;

// Variables para el control y las coordenadas de Leon (Movimiento de Tanque)
u_long pad_state;
int leon_x = 0;
int leon_z = 0;
int leon_angulo = 0; // Controla la rotacion de la camara al hombro

void init_game_system() {
    // Reseteamos la tarjeta grafica de la PS1
    ResetGraph(0);
    
    // Configuramos los dos entornos de dibujo y muestra en la VRAM
    SetDefDispEnv(&disp[0], 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
    SetDefDrawEnv(&draw[0], 0, 240, SCREEN_WIDTH, SCREEN_HEIGHT);
    
    SetDefDispEnv(&disp[1], 0, 240, SCREEN_WIDTH, SCREEN_HEIGHT);
    SetDefDrawEnv(&draw[1], 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
    
    // Color de fondo negro puro para ambiente de Survival Horror
    setRGB0(&draw[0], 0, 0, 0);
    setRGB0(&draw[1], 0, 0, 0);
    
    // Aplicamos la primera configuracion a la pantalla
    PutDispEnv(&disp[db]);
    PutDrawEnv(&draw[db]);
    
    // Activamos la salida de video hacia el emulador/consola
    SetDispMask(1);
    
    // Inicializamos los puertos de los joysticks
    PadInit(0);
}

void procesar_input() {
    // Leemos los botones presionados en el Puerto 1
    pad_state = PadRead(0);
    
    // Control de tanque identico a Resident Evil 4 Mobile
    if (pad_state & PADLup) {
        leon_z += 4; // Camina hacia adelante en el escenario 3D
    }
    if (pad_state & PADLdown) {
        leon_z -= 4; // Camina hacia atras
    }
    if (pad_state & PADLleft) {
        leon_angulo -= 16; // Gira sobre su propio eje a la izquierda
    }
    if (pad_state & PADLright) {
        leon_angulo += 16; // Gira sobre su propio eje a la derecha
    }
}

int main() {
    // Encendemos el hardware de la PS1
    init_game_system();
    
    // Bucle principal (Game Loop)
    while (1) {
        procesar_input();
        
        // Esperamos la sincronizacion vertical de la tele (V-Sync)
        DrawSync(0);
        VSync(0);
        
        // Intercambiamos los buffers de renderizado (Doble Buffer)
        db = !db;
        PutDispEnv(&disp[db]);
        PutDrawEnv(&draw[db]);
    }
    
    return 0;
}
