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



/*** Includes ***/
// Arduino
#include <Arduino.h>
// Includes del proyecto
#include "dht_sensor.h"




/*** Constructor ***/
DhtSensor::DhtSensor() {	
}



/*** Destructor ***/
DhtSensor::~DhtSensor() {
}



/*** Al iniciar la placa ***/
void DhtSensor::Boot() {

	dht.begin();

}



/*** Al iniciar el programa ***/
void DhtSensor::Start() {

	delta_time = millis();				// Define el tiempo entre lecturas
	error_count = 0;					// Conteo de errores
	sensor_state = false;				// Estado del sensor
	sensor_error = false;				// Error critico

	temperature = 0.0f;					// Valores iniciales de la lectura
	humidity = 0.0f;

	Update();							// Primera lectura

}



/*** Actualizacion del sensor ***/
void DhtSensor::Update() {

	// Si no se alcanza la marca de tiempo...
	if (millis() < delta_time) return;

	// Intenta la lectura del sensor
	if (Read()) {
		error_count = 0;
		sensor_state = true;
		sensor_error = false;
	} else {
		error_count ++;
		sensor_state = false;
		if (error_count >= DHT_ERROR_COUNT) {
			error_count = DHT_ERROR_COUNT;
			sensor_error = true;
		}
	}

	// Siguiente gap de lectura
	delta_time = millis() + UPDATE_RATE;
		
}



/*** Lectura del sensor ***/
bool DhtSensor::Read() {

	// Lectura de temperatura y humedad
	float t = dht.readTemperature();
	float h = dht.readHumidity();

	// Si falla la lectura...
	if (isnan(t) || isnan(h)) return false;

	// Debug
	//t = 56.0f;

	// Si no falla, actualiza los datos
	temperature = t;
	humidity = h;
	return true;

}
