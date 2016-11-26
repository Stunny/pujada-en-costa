//
//-------------------------PROGRAMA DE TEST DE l'LSMAKER--------------------
//
// Aquest �s el programa que porta carregat "de f�brica" l'LSMaker.
// Nom�s fa que moure els motors endavant i endarrera i enviar una
// trama per l'enlla� de radiofreq�encia. Per la pantalla de l'LCD
// mostra la tensi� de la bateria, l'adre�a mac, el nom d'usuari 
// que hagis configurat amb el programa LSLoader, la inclinaci� XYZ i
// el valor dels senyals STOP0, STOP1 i STOP2 (Blanc o Negre)
//
// Per crear la teva propia aplicaci�, nom�s cal que baixis el directori
// LS_API i el directori Proves i ja pots engegar el MPLAB i obrir el projecte
// TestMain. El projecte ja te tots els tads i l'API ben configurada, nom�s
// cal que afegeixis els teus propis TADS i la teva aplicaci� en lloc de 
// la "ProvaDeCalibracio"
// 

//------------------------------LLIBRERIES-------------------------
#include "../LS_API/LSApi.h"
//-------------------------END--LLIBRERIES-------------------------

//------------------------------CONSTANTS--------------------------
// Opcions de configuraci� del microprocessador PIC16F64
_CONFIG2(OSCIOFNC_ON);
_CONFIG1(JTAGEN_OFF & ICS_PGx1);
//-------------------------END--CONSTANTS--------------------------

//------------------------------TIPUS------------------------------
//-------------------------END--TIPUS------------------------------

void inicialitza() {
    LS_Init();
}

int main(void) {
    
    inicialitza();
    while (1) {
        LS_Executiu();
    }
    return 0;
}
