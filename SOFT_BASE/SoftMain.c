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

char GetS(unsigned int MaskStop) {
	// Retorna Blanc o Negre segons el que valgui aquest senyal d'stop
	if (MtGetStop(MaskStop)) return 'N';
	return 'B';
}

/**
 * Muestra por el LCD los valores de los ejes X, Y, Z
 */
void MotorInfo(void) {	
	// Refresca la pantalla LCD cada 250ms amb el valor de la bateria i els
	// angles de l'accelerometre
	// No cridis a funcions que comen?en per LS a dins d'un motor o tindr?s 
	// problemes. Nom?s es permet usar LS_LCD_Printf i LS_USB_printf
	int X, Y, Z; float Xf, Yf, Zf;
	static int Estat = 0;
	switch (Estat) {
		case 0:
			++Estat;
			break;
		case 1:
			if (NvAdquirint()) LS_LCD_Printf(15,0, "A"); 
			else {
				// Mentre adquireix, millor no fer servir l'LCD ja que fa perdre
				// precisi? al motor de calcul de posici?
				LS_LCD_Printf(15,0, " ");
				// Estat de la bateria
				LS_LCD_Printf(0,3, "B=%03d", LS_SYS_GetBatteryVoltage());
				// Els angles XYZ de l'acceler?metre
				AcGetXYZ(&Xf, &Yf, &Zf); X = Xf; Y = Yf; Z = Zf;
				LS_LCD_Printf(0,2, "XYZ= %+03d,%+03d,%+03d", X, Y, Z);
				// El valor dels senyals de Stop0, Stop1 i Stop2
				LS_LCD_Printf(6,3, "%c%c%c", GetS(MT_STOP_0),GetS(MT_STOP_1), GetS(MT_STOP_2));
				// El robot est? adquirint?
			}
			break;
	}
}

void inicialitza() {
    LS_Init();
}

/** Modificado el fichero RftRadio.c para proteger de tramas de 
 *  broadcast ajenas el dispositivo. 
 *  @Funcion: RfEsBroadcast(...)
 */

void LS_Main(){
    // *****************************************Comprobacion de estado
    // Cada 250ms Actualiza los valores X, Y, Z de pantalla
        //LS_SYS_AddStateMachine(MotorInfo, 250);
    MotorInfo();
    
    
    //Parametros que decuelve el acelerometro    
    
    
    // ******************************************Correcciones de trayectoria
    
}

int main(void) {
    
    inicialitza();
    
    while(!LS_IO_GpButtonPress()){}
    
    while (1) {
        LS_Executiu();
        LS_Main();
    }
    return 0;
}
