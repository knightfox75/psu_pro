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
	
	Display LCD

    --- Requiere ---
    "LiquidCrystal_I2C" de Frank de Brabander

*******************************************************************************/



#ifndef __LCD_ROUTINES_H
#define __LCD_ROUTINES_H



/*** Includes ***/
// Arduino
#include <Arduino.h>
// Arduino libs
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>



/*** Propiedades estaticas de la clase ***/
static LiquidCrystal_I2C lcd(0x27, 20, 4); 



/*** Definicion de la clase ***/
class LcdRoutines {
	
	public:
	
		// Constructor
		LcdRoutines();
		
		// Destructor
		~LcdRoutines();

        // Codigo inicial
        void Boot();
		
		// Imprime un texto
		void Print(uint8_t x, uint8_t y, String text);
		void Print(const String text);
		void PrintCustom(uint8_t chr);

		// Desplaza el cabezal de escritura
		void Locate(uint8_t x, uint8_t y);

		// Borra la pantalla
		void Cls();

		// Control del backlight
		void BackLightOn();
		void BackLightOff();

		// Convierte un numero a String
		String IntToString(int32_t value);
		String FloatToString(float value);
		String IntToString(int32_t value, uint8_t width);
		String FloatToString(float value, uint8_t width);
	
	private:

		// Caracteres personalizados
		uint8_t c_degree[8] = {0x04, 0x0A, 0x04, 0x00, 0x00, 0x00, 0x00, 0x00};		//	º
		uint8_t c_sine[8] = {0x00, 0x00, 0x08, 0x15, 0x02, 0x00, 0x00, 0x00};		//	~
		// Animacion del ventilador
		uint8_t c_fan[4][8] {
			{0x00, 0x00, 0x00, 0x1F, 0x00, 0x00, 0x00, 0x00},
			{0x00, 0x10, 0x08, 0x04, 0x02, 0x01, 0x00, 0x00},		
			{0x00, 0x04, 0x04, 0x04, 0x04, 0x04, 0x00, 0x00},
			{0x00, 0x01, 0x02, 0x04, 0x08, 0x10, 0x00, 0x00}
		};


				
};



#endif