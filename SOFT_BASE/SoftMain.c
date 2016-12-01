//
//-------------------------PROGRAMA DE TEST DE l'LSMAKER--------------------
//
// Aquest és el programa que porta carregat "de fàbrica" l'LSMaker.
// Només fa que moure els motors endavant i endarrera i enviar una
// trama per l'enllaç de radiofreqüencia. Per la pantalla de l'LCD
// mostra la tensió de la bateria, l'adreça mac, el nom d'usuari 
// que hagis configurat amb el programa LSLoader, la inclinació XYZ i
// el valor dels senyals STOP0, STOP1 i STOP2 (Blanc o Negre)
//
// Per crear la teva propia aplicació, només cal que baixis el directori
// LS_API i el directori Proves i ja pots engegar el MPLAB i obrir el projecte
// TestMain. El projecte ja te tots els tads i l'API ben configurada, només
// cal que afegeixis els teus propis TADS i la teva aplicació en lloc de 
// la "ProvaDeCalibracio"
// 

//------------------------------LLIBRERIES-------------------------
#include "../LS_API/LSApi.h"
//-------------------------END--LLIBRERIES-------------------------

//------------------------------CONSTANTS--------------------------
// Opcions de configuració del microprocessador PIC16F64
_CONFIG2(OSCIOFNC_ON);
_CONFIG1(JTAGEN_OFF & ICS_PGx1);
//-------------------------END--CONSTANTS--------------------------

//------------------------------TIPUS------------------------------
//-------------------------END--TIPUS------------------------------

void inicialitza() {
    LS_Init();
}

void LS_Main(){
    // Comprobacion de estado
    int x, y, z;
    
    LS_ACC_GetPosXYZf(&x, &y, &z);
    
    
    // Correcciones de trayectoria
    
}

int main(void) {
    
    inicialitza();
    while (1) {
        LS_Executiu();
        LS_Main();
    }
    return 0;
}
