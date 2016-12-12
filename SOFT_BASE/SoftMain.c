//
//----------------------------------------PROGRAMA DE TEST DE l'LSMAKER-----
//
// Aquest ?s el programa que porta carregat "de f?brica" l'LSMaker.
// Nom?s fa que moure els motors endavant i endarrera i enviar una
// trama per l'enlla? de radiofreq?encia. Per la pantalla de l'LCD
// mostra la tensi? de la bateria, l'adre?a mac, el nom d'usuari 
// que hagis configurat amb el programa LSLoader, la inclinaci? XYZ i
// el valor dels senyals STOP0, STOP1 i STOP2 (Blanc o Negre)
//
// Per crear la teva propia aplicaci?, nom?s cal que baixis el directori
// LS_API i el directori Proves i ja pots engegar el MPLAB i obrir el projecte
// TestMain. El projecte ja te tots els tads i l'API ben configurada, nom?s
// cal que afegeixis els teus propis TADS i la teva aplicaci? en lloc de 
// la "ProvaDeCalibracio"
// 

//
//----------------------------------------PROTOTIPUS---------------
//

//
//----------------------------------------PROTOTIPUS---------------
//
#include "../LS_API/LSApi.h"
//
//------------------------------END-------PROTOTIPUS---------------
//
//
//----------------------------------------CONSTANTS----------------
//

// Opcions de configuraci? del microprocessador PIC16F64
_CONFIG2(OSCIOFNC_ON);
_CONFIG1(JTAGEN_OFF & ICS_PGx1);

//
//------------------------------END-------CONSTANTS----------------
//

//
//----------------------------------------VARIABLES----------------
//
//
//------------------------------END-------VARIABLES----------------
//

char GetS(unsigned int MaskStop) {
	// Retorna Blanc o Negre segons el que valgui aquest senyal d'stop
	if (MtGetStop(MaskStop)) return 'N';
	return 'B';
}

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
				//LS_LCD_Printf(15,0, " ");
				// Estat de la bateria
				//LS_LCD_Printf(0,3, "B=%03d", LS_SYS_GetBatteryVoltage());
				// Els angles XYZ de l'acceler?metre
				AcGetXYZ(&Xf, &Yf, &Zf); X = Xf; Y = Yf; Z = Zf;
				LS_LCD_Printf(0,2, "XYZ= %+03d,%+03d,%+03d", X, Y, Z);
				// El valor dels senyals de Stop0, Stop1 i Stop2
				//LS_LCD_Printf(6,3, "%c%c%c", GetS(MT_STOP_1),GetS(MT_STOP_0), GetS(MT_STOP_2));
				// El robot est? adquirint?
			}
			break;
	}
}

// Subtrutines de moviment
void AV50(void) { int Stop; int temps;
	temps = LS_MT_GetTimeFromDistance(50, 60); // 50 cm a velocitat 60
	LS_MT_Lineal(temps, 60, 0, &Stop);
}

void RE50(void) { int Stop; int temps;
	temps = LS_MT_GetTimeFromDistance(50, 60); // 50 cm a velocitat -60
	LS_MT_Lineal(temps, -60, 0, &Stop);
}


void GD90(void) { int Stop; int angle;
	angle = LS_MT_GetTimeFromAngle(90, 40); // 90 graus a 40
	LS_MT_TurnRight(angle, 40, 0, 0, &Stop);
}

void GE90(void) { int Stop; int angle;
	angle = LS_MT_GetTimeFromAngle(90, 40); // 90 graus a 40
	LS_MT_TurnLeft(angle, 40, 0, 0, &Stop);
}

int ProvaDeCalibracio(int enderezado){ 
	
    //AV50();
    int X, Y, Z; float Xf, Yf, Zf, stopReason;
    AcGetXYZ(&Xf, &Yf, &Zf); X = Xf; Y = Yf; Z = Zf;
    
    //Enderezar robot hasta estar cuesta arriba (hasta que y = 0 & x < 10)
    while(!(X < 10) && enderezado == 0){
        if(Y > 0){
            //Enderezar derecha
            LS_MT_TurnRight(LS_MT_GetTimeFromDistance(5, 100), 100, 0, 0, &stopReason);
            LS_LCD_Printf(1,2, "E Derecha");
        }
        if (Y < 0){
            //Enderezar izquierda
            LS_MT_TurnLeft(LS_MT_GetTimeFromDistance(5, 100), 100, 0, 0, &stopReason);
            LS_LCD_Printf(1,2, "E Izq");
            //Velocitats diferents (50 -> 10) en els dos enderezar
        }
    }
    
    if(enderezado == 0) enderezado = 1;
    
    //Diferentes situaciones de subir de frente
    if(X < 10){
        
        //Subir recto
        if(Y > -5 && Y < 5){
            LS_MT_Lineal(LS_MT_GetTimeFromDistance(15, 100), 100, 0, &stopReason);
            LS_LCD_Printf(1,2, "Subir");
        }
        
        //Subir girando a derecha
        if(Y < -10){
            LS_MT_TurnRight(LS_MT_GetTimeFromDistance(3, 100), 100, 0, 0, &stopReason);
            LS_LCD_Printf(1,2, "Subir Der");
        }
        
        //Subir girando a izquierda
        if(Y > 10){
            LS_MT_TurnLeft(LS_MT_GetTimeFromDistance(3, 100), 100, 0, 0, &stopReason);
            LS_LCD_Printf(1,2, "Subir Izq");
        }
    }
    
    //Diferentes situaciones de subir marcha atras
    /*if(X > 10){
        
        //Subir recto
        if(Y > -5 && Y < 5){
            LS_MT_Lineal(LS_MT_GetTimeFromDistance(15, -100), -100, 0, &stopReason);
            LS_LCD_Printf(1,2, "Bajar");
        }
        
        //Subir girando a derecha
        if(Y < -10){
            LS_MT_TurnLeft(LS_MT_GetTimeFromDistance(3, -100), -100, 0, 0, &stopReason);
            LS_LCD_Printf(1,2, "Bajar Der");
        }
        
        //Subir girando a izquierda
        if(Y > 10){
            LS_MT_TurnRight(LS_MT_GetTimeFromDistance(3, -100), -100, 0, 0, &stopReason);
            LS_LCD_Printf(1,2, "Bajar Izq");
        }
    }*/
	
    return enderezado;
}

void Inicialitza(void) {
	LS_Init();	// Inicialitza la API de l'LSMaker
	// Aqu? vindrien les inicialitzacions pr?pies de l'aplicaci?
}

int main(void) { 
	// Aqu? comen?a el programa.
    
    int start = 0, enderezado = 0;
    
	Inicialitza();
	LS_SYS_AddStateMachine(MotorInfo, 250);	// Una m?quina d'estats per refrescar l'LCD cada 250ms
	while (1) {	// El bucle principal
        
        if (LS_IO_GpButtonPress()) start = 1;
        
        if(start)
            enderezado = ProvaDeCalibracio(enderezado); 
 	}
	return 0;
}

