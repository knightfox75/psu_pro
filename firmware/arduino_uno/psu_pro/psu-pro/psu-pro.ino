
/*******************************************************************************

	Power Supply PRO for C64, Spectrum and Amstrad CPC
	Ver 1.0.0
	
	Proyecto iniciado el 17-08-2020
	
	(cc) 2020 por Cesar Rincon
	
	nightfoxandco.com
	contact@nightfoxandco.com

	Power Supply PRO se distribuye bajo la licencia CREATIVE COMMONS
	"Attribution-NonCommercial 4.0 International"
	https://creativecommons.org/licenses/by-nc/4.0/
	
	Archivo principal

    --- Requiere ---
    "LiquidCrystal_I2C" de Frank de Brabander
	"DHT sensor library" de Adafruit

*******************************************************************************/




/*** Includes ***/
// Arduino
#include <Arduino.h>
/*** Includes del proyecto ***/
#include "src/core.h"




/*** Propiedades principales ***/
Core core;



/*** Al iniciar la placa arduino ***/
void setup() {

	core.Boot();
	core.Start();

}



/*** Bucle de ejecucion ***/
void loop() {

	core.Run();

}