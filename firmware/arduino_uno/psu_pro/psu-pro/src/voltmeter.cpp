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
	
	Rutinas para la medicion del voltaje

*******************************************************************************/



/*** Includes ***/
// Arduino
#include <Arduino.h>
// Includes del proyecto
#include "voltmeter.h"




/*** Constructor ***/
Voltmeter::Voltmeter() {	
}



/*** Destructor ***/
Voltmeter::~Voltmeter() {
}



/*** Al iniciar la placa ***/
void Voltmeter::Boot() {

	uint16_t v = 0;
	for (uint8_t n = 0; n < 10; n ++) {
		for (uint8_t i = 0; i <= LAST_PIN; i ++) v = analogRead(i);
		delay(50);
	}
	

}



/*** Al iniciar el programa ***/
void Voltmeter::Start() {

	dc5 = 0;
	dc12 = 0;
	dc12n = 0;
	ac9 = 0;
	ac9_display = 0;

	for (uint8_t i = 0; i < AC_VALUES_LENGTH; i ++) ac_values[i] = 0;

	state = false;

	dc5_state = false;
	dc12_state = false;
	dc12n_state = false;
	ac9_state = false;

	dc5_low = false;
	dc12_low = false;
	dc12n_low = false;
	ac9_low = false;

	dc5_high = false;
	dc12_high = false;
	dc12n_high = false;
	ac9_high = false;
			
}



/*** Actualiza los datos ***/
void Voltmeter::Update() {

	// Control de ejecucion
	if (millis() < delta_time) return;

	// Lectura de los voltajes
	ReadValues();
	// Analiza los resultados
	Analyze();

	// Siguiente gap de lectura
	delta_time = millis() + UPDATE_RATE;

}



/*** Opten y calcula los valores ***/
void Voltmeter::ReadValues() {

	uint32_t v = 0;

	// +5V DC
	v = analogRead(PIN_DC5);
	dc5 = round((((float)v * MAX_VOLTAGE) / (float)MAX_ADC) * 10.0f);
	if (dc5 < 10) dc5 = 0;

	// +12V DC
	v = analogRead(PIN_DC12);
	dc12 = round((((float)v * MAX_VOLTAGE) / (float)MAX_ADC) * 10.0f);
	if (dc12 < 10) dc12 = 0;

	// -12V DC
	v = analogRead(PIN_DC12N);
	dc12n = round((((float)v * MAX_VOLTAGE) / (float)MAX_ADC) * 10.0f);
	if (dc12n < 10) dc12n = 0;

	// 9V AC
	v = analogRead(PIN_AC9);
	ac9 = round(((((float)v * MAX_VOLTAGE) / (float)MAX_ADC) / AC_RMS) * 10.0f);
	for (uint8_t i = 0; i < (AC_VALUES_LENGTH - 1); i ++) ac_values[i] = ac_values[(i + 1)];
	ac_values[(AC_VALUES_LENGTH - 1)] = ac9;
	v = 0;
	for (uint8_t i = 0; i < AC_VALUES_LENGTH; i ++) v += ac_values[i];
	ac9_display = round(v / AC_VALUES_LENGTH);
	if (ac9 < 10) ac9 = 0;
	if (ac9_display < 10) ac9_display = 0;

}



/*** Analiza los valores del voltimetro ***/
void Voltmeter::Analyze() {

	// Flags de estado
	dc5_low = false;
	dc12_low = false;
	dc12n_low = false;
	ac9_low = false;
	dc5_high = false;
	dc12_high = false;
	dc12n_high = false;
	ac9_high = false;

	// Analisis de la linea de +5V
	if (dc5 < MIN_DC5) dc5_low = true;
	if (dc5 > MAX_DC5) dc5_high = true;
	// Analisis de la linea de +12V
	if (dc12 < MIN_DC12) dc12_low = true;
	if (dc12 > MAX_DC12) dc12_high = true;
	// Analisis de la linea de -12V
	if (dc12n < MIN_DC12N) dc12n_low = true;
	if (dc12n > MAX_DC12N) dc12n_high = true;
	// Analisis de la linea de ~9V
	if (ac9 < MIN_AC9) ac9_low = true;
	if (ac9 > MAX_AC9) ac9_high = true;

	// Flags de error de salida generales
	dc5_state = !dc5_low && !dc5_high;
	dc12_state = !dc12_low && !dc12_high;
	dc12n_state = !dc12n_low && !dc12n_high;
	ac9_state = !ac9_low && !ac9_high;
	state = dc5_state && dc12_state && dc12n_state && ac9_state;

}