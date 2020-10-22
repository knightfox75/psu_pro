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



#ifndef __VOLTMETER_H
#define __VOLTMETER_H



/*** Includes ***/
// Arduino
#include <Arduino.h>
// Proyecto




/*** Definicion de la clase ***/
class Voltmeter {
	
	public:
	
		// Constructor
		Voltmeter();
		
		// Destructor
		~Voltmeter();

        // Codigo inicial
        void Boot();
		
		// Al iniciar el programa
		void Start();
		
		// Actualizacion de los datos
		void Update();

		// Valores
		uint16_t dc5;			//  +5V DC (A0)		Formato entero, valor REAL X10
		uint16_t dc12;			// +12v DC (A1) 	Formato entero, valor REAL X10
		uint16_t dc12n;			// -12V DC (A2)		Formato entero, valor REAL X10
		uint16_t ac9;			//   9V AC (A3)		Formato entero, valor REAL X10
		uint16_t ac9_display;	//   9V AC (A3)		Formato entero, valor REAL X10, media para mostrar

		// Rango de valores de trabajo maximos
		const uint8_t MIN_DC5 = 45;					// Valor minimo (x10) 4.5V
		const uint8_t MAX_DC5 = 55;					// Valor maximo (x10) 5.5V
		const uint8_t MIN_DC12 = 110;				// Valor minimo (x10) 11.0V
		const uint8_t MAX_DC12 = 130;				// Valor maximo (x10) 13.0V
		const uint8_t MIN_DC12N = 110;				// Valor minimo (x10) -11.0V
		const uint8_t MAX_DC12N = 130;				// Valor maximo (x10) -13.0V
		const uint8_t MIN_AC9 = 80;					// Valor minimo (x10) 8.0V
		const uint8_t MAX_AC9 = 140;				// Valor maximo (x10) 14.0V
		// Estado de los valores de salida
		bool state;
		bool dc5_state, dc12_state, dc12n_state, ac9_state;
		bool dc5_low, dc12_low, dc12n_low, ac9_low;
		bool dc5_high, dc12_high, dc12n_high, ac9_high;
		
	
	private:

		// Constantes
		const uint8_t PIN_DC5 = 0;
		const uint8_t PIN_DC12 = 1;
		const uint8_t PIN_DC12N = 2;
		const uint8_t PIN_AC9 = 3;
		const uint8_t LAST_PIN = 3;


		// Control de tiempo entre ejecuciones
		uint32_t delta_time;
		const uint8_t UPDATE_RATE = 50;		
		const float MAX_VOLTAGE = 41.66f;	// Voltios maximos (Valor REAL x1)
		const uint32_t MAX_ADC = 1023;
		const float AC_RMS = 1.4142f;		// Compensacio conversion AC/DC

		// Opten y calcula los Valores
		void ReadValues();
		static const uint32_t AC_VALUES_LENGTH = 20;		// Numero de muestras para el calculo del valor medio AC
		uint16_t ac_values[AC_VALUES_LENGTH];

		// Analisis de los resultados
		void Analyze();
		
};



#endif