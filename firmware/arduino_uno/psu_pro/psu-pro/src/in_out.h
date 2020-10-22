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
	
	Gestion de las entradas y salidas

*******************************************************************************/



#ifndef __IN_OUT_H
#define __IN_OUT_H



/*** Includes ***/
// Arduino
#include <Arduino.h>
// Proyecto




/*** Definicion de la clase ***/
class InOut {
	
	public:
	
		// Constructor
		InOut();
		
		// Destructor
		~InOut();

        // Codigo inicial
        void Boot();
		
		// Al iniciar el programa
		void Start();
		
		// Ejecuta regularmente
		void Update();

		// Estado del rele principal (PSU)
		bool psu_state;					// Estado del rele principal
		void PsuOn();					// Conecta las fuentes de alimentacion
		void PsuOff();					// Apaga las fuentes de alimentacion

		// Ventilador interno de refrigeracion
		bool fan_state;					// Estado del Ventilador
		void FanOn();					// Enciende el Ventilador
		void FanOff();					// Apaga el ventilador

		// Reles de salida
		bool output_a_state;			// Estado de la salida A		(+5V)
		bool output_b_state;			// Estado de la salida B		(+12V)
		bool output_c_state;			// Estado de la salida C		(-12V)	
		bool output_d_state;			// Estado de la salida D		(9V AC)
		bool output_state;
		void EnableOutput();			// Habilita las salidas
		void DisableOutput();			// Deshabilita las salidas

		// Led indicador
		bool power_led_state;			// Estado del LED del boton de encendido
		void PowerLedOn();				// Enciende el LED
		void PowerLedOff();				// Apaga el LED

		// Pulsador de Power
		struct {
			bool held;
			bool down;
			bool up;
			bool last;
		} power_switch;					// Estado del pulsador de Power



	
	private:

		const uint8_t PIN_MAIN_RELAY = 2;			// Rele principal
		const uint8_t PIN_POWER_SWITCH = 4;			// Pulsador de encendido
		const uint8_t PIN_POWER_LED = 5;			// LED indicador de encendido
		const uint8_t PIN_FAN = 6;					// Ventilador

		const uint8_t PIN_A_RELAY = 8;				// Rele de Salida A (+5V)
		const uint8_t PIN_B_RELAY = 9;				// Rele de Salida B (+12V)
		const uint8_t PIN_C_RELAY = 10;				// Rele de Salida C (-12V)
		const uint8_t PIN_D_RELAY = 11;				// Rele de Salida D (~9V)
					
};



#endif