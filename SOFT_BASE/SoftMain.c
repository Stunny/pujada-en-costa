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
				LS_LCD_Printf(3,2, "XYZ= %+03d,%+03d,%+03d", X, Y, Z);
				// El valor dels senyals de Stop0, Stop1 i Stop2
				//LS_LCD_Printf(6,3, "%c%c%c", GetS(MT_STOP_1),GetS(MT_STOP_0), GetS(MT_STOP_2));
				// El robot est? adquirint?
			}
			break;
	}
}

void AV50(void) { int Stop; int temps;
	temps = LS_MT_GetTimeFromDistance(50, 60); // 50 cm a velocitat 60
	LS_MT_Lineal(temps, 60, 0, &Stop);
}

void RE50(void) { int Stop; int temps;
	temps = LS_MT_GetTimeFromDistance(50, 60); // 50 cm a velocitat -60
	LS_MT_Lineal(temps, -60, 0, &Stop);
}

//---------------------------------------END SOFTWARE CALIBRACIO----------------




//------------PUJADA EN COSTA-------------------------------------------------//

/**
 * Gir a la dreta del robot
 * @param angle Angle de gir desitjat
 * @param speed Velocitat de gir desitjada
 * @param radius Radi de gir desitjat. 0 per girar sobre si mateix
 */
void GD(int angle, int speed, int radius) { 
    int Stop;
	int angleGir = LS_MT_GetTimeFromAngle(angle, speed); // Girar a dreta $('angle') graus 
	LS_MT_TurnRight(angleGir, speed, radius, 0, &Stop);       // a $('speed') 
}

/**
 * Gir a l'esquerra del robot
 * @param angle Angle de gir dessitjat
 * @param speed Velocitat de gir desitjada
 * @param radius Radi de gir desitjat. 0 per girar sobre si mateix
 */
void GE(int angle, int speed, int radius) { 
    int Stop; 
	int angleGir = LS_MT_GetTimeFromAngle(angle, speed); // Girar Esquerra $('angle') graus 
	LS_MT_TurnLeft(angleGir, speed, radius, 0, &Stop);        // a $('speed')
}

/**
 * Procedimient que implementa el comportamient de pujasda del robot
 * @param X Component X del acelerometre
 * @param Y Component Y del acelerometre
 * @param Z Component Z del acelerometre
 */
void upYouGo(int X, int Y, int Z){
    
    int stopReason;
    
     //Subir recto
    if(Y > -5 && Y < 5){
        LS_MT_Lineal(LS_MT_GetTimeFromDistance(10, 100), 100, 0, &stopReason);
        LS_LCD_Printf(1,2, "Subir");
        return;
    }
        
    //Subir girando a derecha
    if(Y > -10 && Y < -5){
        LS_MT_TurnRight(LS_MT_GetTimeFromDistance(10, 100), 100, 7, 0, &stopReason);
        LS_LCD_Printf(1,2, "Subir Der");
        return;
    }
        
    //Subir girando a izquierda
    if(Y < 10 && Y > 5){
        LS_MT_TurnLeft(LS_MT_GetTimeFromDistance(10, 100), 100, 7, 0, &stopReason);
        LS_LCD_Printf(1,2, "Subir Izq");
        return;
    }
    
    if(Y > 10){
        LS_MT_TurnLeft(LS_MT_GetTimeFromDistance(10, 100), 100, 10, 0, &stopReason);
        LS_LCD_Printf(1,2, "Subir Izq");
        return;
    }
    
    if(Y < -10){
        LS_MT_TurnRight(LS_MT_GetTimeFromDistance(10, 100), 100, 10, 0, &stopReason);
        LS_LCD_Printf(1,2, "Subir Der");
        return;
    }
}

/**
 * Procediment principal de la funcionalitat de pujada en costa.
 * Aquesta funció esta pensada per funcionar dins del bucle infinit 
 * establert al main(void).
 */
void Main(){ 

    int X, Y, Z; float Xf, Yf, Zf;
    
    AcGetXYZ(&Xf, &Yf, &Zf); X = Xf; Y = Yf; Z = Zf;
    
    //Cerca de la posició del robot que el deixi direccio amunt.
    while(X >= 10){
        if(Y > 5){
            //Cerca dreta
            GE(10, 100, 0);
            LS_LCD_Printf(1,2, "End Der");
        }
        if (Y < 5){
            //Cerca esquerra
            GD(10, 100, 0);
            LS_LCD_Printf(1,2, "End Izq");
        }
        AcGetXYZ(&Xf, &Yf, &Zf); X = Xf; Y = Yf; Z = Zf;
    }
    
    // Avanç de pujada
    upYouGo(X, Y, Z);
	return;
}


/**
 * Entrada al encendre.
 * @return Exit code.
 */
int main(void) { 
    
    //Flag que s'activa quan es pulsa GPButton i que dona inici a la
    //pujada en costa
    int start = 0;
	
    //Carga de la LSAPI
    LS_Init();
    
    // Mostrara constantment les components de l'acceleroetre per l'LCD
	LS_SYS_AddStateMachine(MotorInfo, 250);
    
	while (1) {
        
        if (LS_IO_GpButtonPress()) 
            start = 1;
        
        if(start)
            Main(); 
 	}
	return 0;
}

