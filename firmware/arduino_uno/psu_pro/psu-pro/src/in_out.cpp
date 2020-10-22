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



/*** Includes ***/
// Arduino
#include <Arduino.h>
// Includes del proyecto
#include "in_out.h"




/*** Constructor ***/
InOut::InOut() {	
}



/*** Destructor ***/
InOut::~InOut() {
}



/*** Al iniciar la placa ***/
void InOut::Boot() {

	// Configuracion de los pines
	pinMode(PIN_MAIN_RELAY, OUTPUT);
	pinMode(PIN_POWER_SWITCH, INPUT);
	pinMode(PIN_POWER_LED, OUTPUT);
	pinMode(PIN_FAN, OUTPUT);

	pinMode(PIN_A_RELAY, OUTPUT);
	pinMode(PIN_B_RELAY, OUTPUT);
	pinMode(PIN_C_RELAY, OUTPUT);
	pinMode(PIN_D_RELAY, OUTPUT);

	PsuOff();
	DisableOutput();
	FanOff();
	PowerLedOff();


}



/*** Al iniciar el programa ***/
void InOut::Start() {

	// Estado inicial del rele maestro
	psu_state = false;
	// Estado inicial del ventilador
	fan_state = false;
	// Estado inicial de los reles de salida
	output_a_state = false;
	output_b_state = false;
	output_c_state = false;
	output_d_state = false;
	output_state = false;

	// Pulsador de POWER
	power_switch.held = false;
	power_switch.down = false;
	power_switch.up = false;
	power_switch.last = false;

}



/*** Actualiza la interfaz I/O ***/
void InOut::Update() {

	// Lectura del pulsador de Power
	power_switch.held = digitalRead(PIN_POWER_SWITCH);
    power_switch.down = power_switch.held & !power_switch.last;
    power_switch.up = !power_switch.held & power_switch.last;
    power_switch.last = power_switch.held;

}



/*** Enciende el rele principal (unidades PSU) ***/
void InOut::PsuOn() {

	digitalWrite(PIN_MAIN_RELAY, true);
	psu_state = true;

}
/*** Apaga el rele principal (unidades PSU) ***/
void InOut::PsuOff() {

	digitalWrite(PIN_MAIN_RELAY, false);
	psu_state = false;

}



/*** Enciende el ventilador ***/
void InOut::FanOn() {

	digitalWrite(PIN_FAN, true);
	fan_state = true;

}
/*** Apaga el ventilador ***/
void InOut::FanOff() {

	digitalWrite(PIN_FAN, false);
	fan_state = false;

}



/*** Enciende el LED de power ***/
void InOut::PowerLedOn() {

	digitalWrite(PIN_POWER_LED, true);
	power_led_state = true;

}
/*** Apaga el LED de power ***/
void InOut::PowerLedOff() {

	digitalWrite(PIN_POWER_LED, false);
	power_led_state = false;

}



/*** Habilita la salida ***/
void InOut::EnableOutput() {

	digitalWrite(PIN_A_RELAY, false);
	digitalWrite(PIN_B_RELAY, false);
	digitalWrite(PIN_C_RELAY, false);
	digitalWrite(PIN_D_RELAY, false);

	output_a_state = true;
	output_b_state = true;
	output_c_state = true;
	output_d_state = true;

	output_state = true;

}
/*** Deshabilita la salida ***/
void InOut::DisableOutput() {

	digitalWrite(PIN_A_RELAY, true);
	digitalWrite(PIN_B_RELAY, true);
	digitalWrite(PIN_C_RELAY, true);
	digitalWrite(PIN_D_RELAY, true);

	output_a_state = false;
	output_b_state = false;
	output_c_state = false;
	output_d_state = false;

	output_state = false;

}
