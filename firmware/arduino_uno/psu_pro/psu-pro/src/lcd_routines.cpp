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



/*** Includes ***/
// Arduino
#include <Arduino.h>
// Includes del proyecto
#include "lcd_routines.h"



/*** Constructor ***/
LcdRoutines::LcdRoutines() {	
}



/*** Destructor ***/
LcdRoutines::~LcdRoutines() {
}



/*** Al iniciar la placa ***/
void LcdRoutines::Boot() {

  lcd.init();
  lcd.setBacklight(1);

  // Caracteres personalizados
  lcd.createChar(0, c_degree);		//	º
  lcd.createChar(1, c_sine);		//	~

  // Animacion del ventilador
  lcd.createChar(4, c_fan[0]);
  lcd.createChar(5, c_fan[1]);
  lcd.createChar(6, c_fan[2]);
  lcd.createChar(7, c_fan[3]);

}



/*** Imprime un texto (sobrecarga 1) ***/
void LcdRoutines::Print(uint8_t x, uint8_t y, String text) {

	lcd.setCursor(x, y);
	lcd.print(text);

}



/*** Imprime un texto (sobrecarga 2) ***/
void LcdRoutines::Print(String text) {

	lcd.print(text);

}



/*** Imprime un caracter especial ***/
void LcdRoutines::PrintCustom(uint8_t chr) {

	lcd.write(chr);

}



/*** Desplaza el cabezal de escritura ***/
void LcdRoutines::Locate(uint8_t x, uint8_t y) {

	lcd.setCursor(x, y);

}



/*** Borra la pantalla ***/
void LcdRoutines::Cls() {

	lcd.clear();
	lcd.home();

}



/*** Control del backlight ***/
void LcdRoutines::BackLightOn() {
	lcd.setBacklight(1);
}
void LcdRoutines::BackLightOff() {
	lcd.setBacklight(0);
}



/*** Convierte un numero entero a String (sobrecarga 1) ***/
String LcdRoutines::IntToString(int32_t value) {

	return String(value);

}



/*** Convierte un numero decimal a String (sobrecarga 1) ***/
String LcdRoutines::FloatToString(float value) {

	return String(value);

}



/*** Convierte un numero entero a String (sobrecarga 2) ***/
String LcdRoutines::IntToString(int32_t value, uint8_t width) {

	// Si se ha especificado solo 1 digito, conversion directa
	if (width <= 1) return String(value);
	
	// Variables de control
	String text = "";
	uint8_t digits = width;
	int32_t number = value;
	
	// Si se trata de un numero negativo...
	if (number < 0) {
		text += "-";
		number = abs(number);
		digits --;
	}
	
	// Añade el numero de ceros necesarios
	for (uint8_t i = 1; i < digits; i ++) {
		if (number < pow(10, i)) text += "0";
	}
	
	// Añadele el numero convertido
	text += String(number);
	
	// Devuelve el resultado
	return text;

}



/*** Convierte un numero decimal a String (sobrecarga 2) ***/
String LcdRoutines::FloatToString(float value, uint8_t width) {

	// Si se ha especificado solo 1 digito, conversion directa
	if (width <= 1) return String(value);
	
	// Variables de control
	String text = "";
	uint8_t digits = width;
	float number = value;
	
	// Si se trata de un numero negativo...
	if (number < 0) {
		text += "-";
		number = abs(number);
		digits --;
	}
	
	// Añade el numero de ceros necesarios
	for (uint8_t i = 1; i < digits; i ++) {
		if (number < pow(10, i)) text += "0";
	}
	
	// Añadele el numero convertido
	text += String(number);
	
	// Devuelve el resultado
	return text;

}

