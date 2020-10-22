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
	
	Sensor DHT

    --- Requiere ---
	"DHT sensor library" de Adafruit

*******************************************************************************/



#ifndef __DHT_SENSOR_H
#define __DHT_SENSOR_H



/*** Includes ***/
// Arduino
#include <Arduino.h>
// Arduino Libs
#include "DHT.h"
// Proyecto
#include "lcd_routines.h"



/*** Propiedades estaticas de la clase ***/
static const uint8_t DHT_TYPE = 22;			// Tipo de sensor: 11 (DHT11), 12 (DHT12), 21 (DHT21), 22 (DHT22)
static const uint8_t DHT_PIN = 3;			// Pin de datos
static DHT dht(DHT_PIN, DHT_TYPE);



/*** Definicion de la clase ***/
class DhtSensor {
	
	public:
	
		// Constructor
		DhtSensor();
		
		// Destructor
		~DhtSensor();

        // Codigo inicial
        void Boot();
		
		// Al iniciar el programa
		void Start();
		
		// Ejecuta regularmente
		void Update();

		// Propiedades
		float temperature;			// Temperatura en º celsius
		float humidity;				// Humedad relativa
		bool sensor_state;			// Estado del sensor
		bool sensor_error;			// Error critico
		
	
	private:

		const uint8_t DHT_ERROR_COUNT = 3;		// Numero maximo de reintentos
		uint8_t error_count;					// Conteo de errores

		// Lee el sensor
		bool Read();

		// Control de tiempo entre lecturas
		uint32_t delta_time;
		const uint32_t UPDATE_RATE = 2000;

				
};



#endif