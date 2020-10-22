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
	
	Nucleo de Ejecucion

*******************************************************************************/



/*** Includes ***/
// Arduino
#include <Arduino.h>
// Includes del proyecto
#include "core.h"




/*** Constructor ***/
Core::Core() {	
}



/*** Destructor ***/
Core::~Core() {
}



/*** Al iniciar la placa ***/
void Core::Boot() {

	// I/O
	io.Boot();

	// Voltimetro
	voltmeter.Boot();

	// LCD
	lcd.Boot();			// Inicia la pantalla LCD

	// Texto de bienvenida
	lcd.Cls();
	for (uint8_t n = 0; n < 4; n ++) lcd.Print(0, n, WELCOME_TEXT[n]);

	// Inicia el sensor ambiental DHT
	dht.Boot();			// Inicia el sensor DHT
	delay(3000);		// Espera a que termine el bootup de los sensores

}



/*** Al iniciar el programa ***/
void Core::Start() {

	// Inicializa el sistema I/O
	io.Start();
	// Inicializa el voltimetro
	voltmeter.Start();
	// Inicializa el sistema DHT
	dht.Start();
	temp_state = TEMP_NORMAL;

	// Maquina de estados
	main_st = next_main_st = MAIN_ST_START_UP;

	// Prepara el Start Up...
	startup_st = next_startup_st = STARTUP_ST_START;
	time_counter = millis();
	pw_led_timer = millis();

	// Control de ejecucion
	delta_time = millis();

	// Variables
	standby_timer = 0xFFFFFF;
	error_code = 0x00;
	error_count = 0;
	fan_anim_frame = fan_first_frame;
			
}



/*** Ejecuta el programa ***/
void Core::Run() {

	// Control de ejecucion
	if (millis() < delta_time) return;

	// I/O
	io.Update();

	// Lectura de sensores
	DhtRead();
	VoltRead();

	// Maquina de estados
	int8_t r = 0;
	switch (main_st) {

		// Start Up de la fuente
		case MAIN_ST_START_UP:
			r = StStartUp();
			switch (r) {
				case 1:
					// Pasa al estado "Idle"
					lcd_delta = millis();				// Control del LCD
					io.PowerLedOff();					// Apaga el LED
					GuiLayout();						// Dibuja la GUI
					lcd.Print(16, 3, "IDLE");			// Estado
					next_main_st = MAIN_ST_IDLE;		// Estado al que saltas
					break;
			}
			break;

		// Fuente en reposo
		case MAIN_ST_IDLE:
			r = StIdle();
			switch (r) {
				case 1:
					// Habilita la salida
					lcd_delta = millis();				// Control del LCD
					io.EnableOutput();					// Habilita la salida
					io.PowerLedOn();					// Enciende el LED
					lcd.Print(16, 3, "  ON");			// Estado
					next_main_st = MAIN_ST_POWER_ON;	// Estado al que saltas
					break;
				case 2:
					// Stand By
					standby_st = next_standby_st = STANDBY_ST_START;
					next_main_st = MAIN_ST_STANDBY;
					break;
				default:
					// Error trap
					if (r > ERR_CODE_ID) {
						ErrorHandler(r);
						next_main_st = MAIN_ST_ERROR;
					}
					break;
			}
			break;

		// Fuente encendida
		case MAIN_ST_POWER_ON:
			r = StPowerOn();
			switch (r) {
				case 1:
					// Deshabilita la salida
					lcd_delta = millis();				// Control del LCD
					io.DisableOutput();					// Deshabilita la salida
					io.PowerLedOff();					// Apaga el LED
					lcd.Print(16, 3, "IDLE");			// Estado
					next_main_st = MAIN_ST_IDLE;		// Estado al que saltas
					break;
				case 2:
					// Stand By
					standby_st = next_standby_st = STANDBY_ST_START;
					next_main_st = MAIN_ST_STANDBY;
				default:
					// Error trap
					if (r > ERR_CODE_ID) {
						ErrorHandler(r);
						next_main_st = MAIN_ST_ERROR;
					}
					break;
			}
			break;

		// Modo de espera
		case MAIN_ST_STANDBY:
			r = StStandBy();
			switch (r) {
				case 1:
					next_main_st = MAIN_ST_START_UP;
					break;
			}
			break;

		// Gestion de errores
		case MAIN_ST_ERROR:
			r = StError();
			switch (r) {
				case 1:
					lcd_delta = millis();				// Control del LCD
					io.PowerLedOff();					// Apaga el LED
					GuiLayout();						// Dibuja la GUI
					lcd.Print(16, 3, "IDLE");			// Estado
					next_main_st = MAIN_ST_IDLE;		// Estado al que saltas
					break;
				case 2:
					// Stand By
					standby_st = next_standby_st = STANDBY_ST_START;
					next_main_st = MAIN_ST_STANDBY;
					break;
			}
			break;

	}


	// Control del ventilador
	FanControl();

	// Siguiente estado
	main_st = next_main_st;

	// Siguiente tiempo de ejecucion
	delta_time = millis() + UPDATE_RATE;

}



/*** Estado "Start Up" ***/
int8_t Core::StStartUp() {

	// Resultado de la Ejecucion
	int8_t r = 0;

	// Maquina de estados
	if (millis() >= time_counter) {
		
		switch (startup_st) {

			// Prepara el inicio
			case STARTUP_ST_START:
				lcd.Cls();												// Borra la pantalla
				// Prepara el test del sensor ambiental
				lcd.Print(0, 0, "DHT TEST: ");
				// Prepara la pausa
				time_counter = millis() + 1000;
				next_startup_st = STARTUP_ST_DHT_TEST;					// Siguiente estado
				break;

			// Test del sensor DHT
			case STARTUP_ST_DHT_TEST:
				// Lee el sensor
				dht.Update();
				// Si el sensor responde correctamente o a informado de un error...
				if (dht.sensor_state || dht.sensor_error) {
					if (dht.sensor_state) {
						lcd.Print("OK");
					} else {
						lcd.Print("ERROR");
					}
					// Prepara la pausa
					time_counter = millis() + 1000;
					// Y el siguiente estado
					next_startup_st = STARTUP_ST_FAN_ON;
				}
				break;

			// Enciende el ventilador 2 segundos
			case STARTUP_ST_FAN_ON:
				lcd.Print(0, 1, "SPINNING FAN");
				// Enciende el ventilador
				io.FanOn();
				// Prepara la pausa
				time_counter = millis() + 3000;
				// Y el siguiente estado
				next_startup_st = STARTUP_ST_FAN_OFF;
				break;

			// Apaga el ventilador y prepara el siguiente estado
			case STARTUP_ST_FAN_OFF:
				// Enciende el ventilador
				io.FanOff();
				// Y el siguiente estado
				next_startup_st = STARTUP_ST_PSU_ON;
				break;

			// Enciende la fuente de alimentacion y espera 2 segundos
			case STARTUP_ST_PSU_ON:
				lcd.Print(0, 2, "POWERING UNITS");
				// Enciende la fuente
				io.PsuOn();
				// Prepara la pausa
				time_counter = millis() + 2000;
				// Y el siguiente estado
				next_startup_st = STARTUP_ST_VOLTAGE_TEST;
				break;

			// Prepara el test de voltaje
			case STARTUP_ST_VOLTAGE_TEST:
				lcd.Print(0, 3, "VOLTAGE TEST: ");
				// Prepara la pausa
				time_counter = millis() + 500;
				// Y el siguiente estado
				error_count = 0;
				next_startup_st = STARTUP_ST_VOLTAGE_CHECK;
				break;

			// Realiza el test de voltaje
			case STARTUP_ST_VOLTAGE_CHECK:
				// Lee el voltimetro
				VoltRead();
				if (error_count >= 3) {
					// Si se ha superado el numero de reintentos, indicalo
					lcd.Print("ERROR");
					time_counter = millis() + 1000;
					next_startup_st = STARTUP_ST_EXIT;
				} else if (!voltmeter.state) {
					// Si algun valor, esta fuera de rango, reintentalo
					time_counter = millis() + 100;
					error_count ++;
				} else {
					// Si se ha superado el numero de reintentos, indicalo
					lcd.Print("OK");
					time_counter = millis() + 1000;
					next_startup_st = STARTUP_ST_EXIT;
				}				
				break;

			// Finaliza el proceso de arranque
			case STARTUP_ST_EXIT:
				// Borra la pantalla
				lcd.Cls();
				// Informa que has finalizado sin errores
				r = 1;
				break;

		}

		// Sigueinte estado
		startup_st = next_startup_st;


	}



	// Led del boton de encendido
	if (millis() >= pw_led_timer) {
		
		if (io.power_led_state) {
			io.PowerLedOff();
		} else {
			io.PowerLedOn();
		}
		pw_led_timer = millis() + 250;

	}


	// Resultado de la ejecucion
	return r;

}



/*** Estado "Idle" ***/
int8_t Core::StIdle() {

	// Resultado
	int8_t r = 0;

	// Actualiza el panel LCD
	UpdateLcd();

	if (CheckStandByPush() > 0) {
		// Modo "StandBy"
		r = 2;
	} else if (io.power_switch.up) {
		if (dht.sensor_error) {
			// Error en el sensor DHT
			r = ERR_CODE_DHT;
		} else if (temp_state == TEMP_CRITICAL) {
			// Error de temperatura, sobrecalentamiento
			r = ERR_CODE_TEMP_CRITICAL;
		} else if (temp_state == TEMP_OVERHEAT) {
			// Error de temperatura, sobrecalentamiento
			r = ERR_CODE_TEMP_OVERHEAT;
		} else if (!voltmeter.state) {
			// Error en el voltaje de salida
			r = ERR_CODE_VOLT_IN;
		} else {
			// Enciende la salida
			r = 1;
		}
	}

	// Devuelve el resultado
	return r;

}



/*** Estado "POWER ON" ***/
int8_t Core::StPowerOn() {

	// Resultado
	int8_t r = 0;

	// Actualiza el panel LCD
	UpdateLcd();

	if (dht.sensor_error) {
		// Error en el sensor DHT
		r = ERR_CODE_DHT;
	} else if (temp_state == TEMP_CRITICAL) {
		// Error de temperatura, sobrecalentamiento
		r = ERR_CODE_TEMP_CRITICAL;
	} else if (temp_state == TEMP_OVERHEAT) {
		// Error de temperatura, sobrecalentamiento
		r = ERR_CODE_TEMP_OVERHEAT;
	} else if (!voltmeter.state) {
		// Desconecta las salidas
		io.DisableOutput();
		io.PowerLedOff();
		// Error en el voltaje de salida
		r = ERR_CODE_VOLT_EXT;
	} else if (CheckStandByPush() > 0) {
		// Modo "StandBy"
		r = 2;
	} else if (io.power_switch.up) {
		// Modo "Idle"
		r = 1;
	}

	// Devuelve el resultado
	return r;

}



/*** Estado "STAND BY" ***/
int8_t Core::StStandBy() {

	// Resultado
	int8_t r = 0;

	// Maquina de estados
	switch (standby_st) {

		// Prepara el apagado
		case STANDBY_ST_START:
			io.DisableOutput();
			io.PsuOff();
			io.PowerLedOff();
			lcd.Cls();
			lcd.Print(6, 1, "SHUTDOWN");
			time_counter = millis() + 2000;
			next_standby_st = STANDBY_ST_SHUTDOWN;
			break;

		// Espera 2 segundo y apaga la fuente
		case STANDBY_ST_SHUTDOWN:
			if (millis() >= time_counter) {
				lcd.BackLightOff();
				lcd.Cls();
				next_standby_st = STANDBY_ST_SLEEP;
			}
			break;

		// Modo Reposo
		case STANDBY_ST_SLEEP:
			if (io.power_switch.down) {
				next_standby_st = STANDBY_ST_POWER_ON;
			}
			break;

		// Prepara el Power On
		case STANDBY_ST_POWER_ON:
			lcd.BackLightOn();
			lcd.Cls();
			lcd.Print(6, 1, "POWER ON");
			time_counter = millis() + 2000;
			next_standby_st = STANDBY_ST_EXIT;
		break;

		// Sal del modo Stand By
		case STANDBY_ST_EXIT:
			// Si se supera el tiempo marcado
			if (millis() >= time_counter) {
				// Prepara el Start Up...
				startup_st = next_startup_st = STARTUP_ST_START;
				time_counter = millis();
				pw_led_timer = millis();
				r = 1;
			}
			break;

	}

	standby_st = next_standby_st;

	// Devuelve el resultado
	return r;

}



/*** Gestion de errores ***/
int8_t Core::StError() {

	// Resultado
	int8_t r = 0;

	// Al pulsar el boton...
	if (CheckStandByPush() > 0) {
		// Modo "StandBy"
		r = 2;
	} else if (io.power_switch.up) {
		switch (error_code) {
			case ERR_CODE_DHT:
				// Modo "StandBy"
				r = 1;
				break;
			case ERR_CODE_TEMP_OVERHEAT:
				// Modo "Idle"
				r = 1;
				break;
			case ERR_CODE_TEMP_CRITICAL:
				// Modo "StandBy"
				r = 2;
				break;
			case ERR_CODE_VOLT_IN:
				// Modo "StandBy"
				r = 1;
				break;
			case ERR_CODE_VOLT_EXT:
				// Modo "Idle"
				r = 1;
				break;
		}
	}


	// Led del boton de encendido
	if (millis() >= pw_led_timer) {
		
		if (io.power_led_state) {
			io.PowerLedOff();
			pw_led_timer = millis() + 1000;
		} else {
			io.PowerLedOn();
			pw_led_timer = millis() + 50;
		}

	}

	// Devuelve el resultado
	return r;

}



/*** Diseño de la interfaz ***/
void Core::GuiLayout() {

	lcd.Cls();
	lcd.Print(0, 0, "DC +5  DC +12");
	lcd.Print(0, 2, "AC 9   DC -12");
	lcd.Print(17, 2, "[ ]");

}



/*** Actualiza el panel LCD ***/
void Core::UpdateLcd() {

	if (millis() < lcd_delta) return;

	// Muestra la informacion ambiental
	DhtInfo();

	// Voltimetro
	VoltInfo();

	// Animacion del ventilador
	if (io.fan_state) {
		lcd.Locate(18, 2);
		lcd.PrintCustom(fan_anim_frame);
		fan_anim_frame ++;
		if (fan_anim_frame > fan_last_frame) fan_anim_frame = fan_first_frame;
	} else {
		lcd.Print(18, 2, " ");
	}

	// Nueva marca de tiempo
	lcd_delta = millis() + LCD_DELTA_TIME;

}



/*** Lee y analiza los valores del voltimetro ***/
void Core::VoltRead() {

	// Lectura del voltimetro
	voltmeter.Update();

}



/*** Muestra los voltajes ***/
void Core::VoltInfo() {

	// Variables para el muestreo
	int16_t integer = 0;
	int16_t decimal = 0;

	// +5V
	integer = (int32_t)(voltmeter.dc5 / 10);
	decimal = (voltmeter.dc5 % 10);
	lcd.Print(0, 1, "+" + lcd.IntToString(integer, 2) + "." + lcd.IntToString(decimal, 1) + "V");

	// +12V
	integer = (int32_t)(voltmeter.dc12 / 10);
	decimal = (voltmeter.dc12 % 10);
	lcd.Print(7, 1, "+" + lcd.IntToString(integer, 2) + "." + lcd.IntToString(decimal, 1) + "V");

	// 9AC
	integer = (int32_t)(voltmeter.ac9_display / 10);
	decimal = (voltmeter.ac9_display % 10);
	lcd.Locate(0, 3);
	lcd.PrintCustom(1);
	lcd.Print(1, 3, lcd.IntToString(integer, 2) + "." + lcd.IntToString(decimal, 1) + "V");

	// -12V
	integer = (int32_t)(voltmeter.dc12n / 10);
	decimal = (voltmeter.dc12n % 10);
	lcd.Print(7, 3, "-" + lcd.IntToString(integer, 2) + "." + lcd.IntToString(decimal, 1) + "V");

}



/*** Lee y analiza los valores del DHT ***/
void Core::DhtRead() {

	// Actualiza el estado del sensor DHT
	dht.Update();

	// Estado del sensor
	if (dht.sensor_error) return;

	// Analisis del nivel de temperatura
	if (dht.temperature > CRITICAL_TEMP) {
		temp_state = TEMP_CRITICAL;
	} else if (dht.temperature > OVERHEAT_TEMP) {
		temp_state = TEMP_OVERHEAT;
	} else if (dht.temperature > FAN_START_TEMP) {
		temp_state = TEMP_WARM;
	} else {
		temp_state = TEMP_NORMAL;
	}


}



/*** Muestra la informacion ambiental ***/
void Core::DhtInfo() {

	if (dht.sensor_state) {
		lcd.Print(14, 0, " " + lcd.IntToString(dht.temperature, 3));
		lcd.PrintCustom(0);
		lcd.Print("C");
		lcd.Print(14, 1, " " + lcd.IntToString(dht.humidity, 3) + "%H");
	} else if (!dht.sensor_error) {
		lcd.Print(14, 0, "  WAIT");
		lcd.Print(14, 1, "   DHT");
	} else {
		lcd.Print(14, 0, "   DHT");
		lcd.Print(14, 1, " ERROR");
	}

}



/*** Control de la temperatura y el ventilador ***/
void Core::FanControl() {

	if (temp_state == TEMP_CRITICAL) {
		// Gestion de la temperatura critica
		if (io.output_state) {
			io.DisableOutput();
			io.PowerLedOff();
		}
		if (io.psu_state) io.PsuOff();
		if (!io.fan_state) io.FanOn();
		if ((main_st == MAIN_ST_IDLE) || (main_st == MAIN_ST_POWER_ON)) {
			ErrorHandler(ERR_CODE_TEMP_CRITICAL);
			next_main_st = MAIN_ST_ERROR;
		}
		return;
	} else if (temp_state == TEMP_OVERHEAT) {
		// Gestion del sobrecalentamiento
		if (io.output_state) {
			io.DisableOutput();
			io.PowerLedOff();
		}
		if (!io.fan_state) io.FanOn();
		return;		
	} else if (main_st == MAIN_ST_START_UP) {
		// Si esta en modo "Start up", vuelve
		return;
	}

	// Control de temperatura
	if ((dht.temperature > FAN_START_TEMP) && !io.fan_state) io.FanOn();
	if ((dht.temperature < FAN_STOP_TEMP) && io.fan_state) io.FanOff();

}



/*** Pulsacion para el cambio de modo del STAND BY ***/
int8_t Core::CheckStandByPush() {

	int8_t r = 0;

	if (io.power_switch.down) {
		standby_timer = millis() + STANDBY_PUSH_TIME;
	} else if (io.power_switch.held) {
		if (millis() >= standby_timer) r = 1;
	} else {
		standby_timer = 0xFFFFFF;
	}

	return r;
	
}



/*** Manejador de errores ***/
void Core::ErrorHandler(uint8_t err_code) {

	// Borra la pantalla
	lcd.Cls();

	// Guarda el error
	error_code = err_code;

	// Variables de texto
	String txt = "", z_text = "";
	const String low_t = "LOW";
	const String high_t = "HIGH";
	const String norm_t = "GOOD";
	const String zero_t = "ZERO";
	const String short_t = "SHORT";

	// Gestion de errores
	switch (error_code) {

		case ERR_CODE_DHT:
			lcd.Print(0, 0, "DHT SENSOR ERROR");
			lcd.Print(0, 1, "REPLACE THE SENSOR");
			break;

		case ERR_CODE_TEMP_OVERHEAT:
			lcd.Print(0, 0, "PSU OVERHEAT");
			lcd.Print(0, 1, "WAIT FOR COLDDOWN");
			break;

		case ERR_CODE_TEMP_CRITICAL:
			lcd.Print(0, 0, "TEMPERATURE CRITICAL");
			lcd.Print(0, 1, "WAIT FOR COLDDOWN");
			lcd.Print(0, 2, "CHECK FAN");
			break;

		case ERR_CODE_VOLT_IN:
		case ERR_CODE_VOLT_EXT:
			GuiLayout();
			if (error_code == ERR_CODE_VOLT_IN) {
				lcd.Print(17, 0, "PSU");
				lcd.Print(15, 1, "ERROR");
				z_text = zero_t;
			} else {
				lcd.Print(14, 0, "OUTPUT");
				lcd.Print(15, 1, "ERROR");
				z_text = short_t;
			}
			// DC +5V
			txt = norm_t;
			if (voltmeter.dc5 == 0) {
				txt = z_text;
			} else if (voltmeter.dc5_low) {
				txt = low_t;
			} else if (voltmeter.dc5_high) {
				txt = high_t;
			}
			lcd.Print(0, 1, txt);
			// DC +12V
			txt = norm_t;
			if (voltmeter.dc12 == 0) {
				txt = z_text;
			} else if (voltmeter.dc12_low) {
				txt = low_t;
			} else if (voltmeter.dc12_high) {
				txt = high_t;
			}
			lcd.Print(7, 1, txt);
			// AC 9V
			txt = norm_t;
			if (voltmeter.ac9 == 0) {
				if (error_code == ERR_CODE_VOLT_IN) {
					lcd.Print(15, 2, "CHECK");
					lcd.Print(16, 3, "FUSE");
				}
				txt = z_text;
			} else if (voltmeter.ac9_low) {
				txt = low_t;
			} else if (voltmeter.ac9_high) {
				txt = high_t;
			}
			lcd.Print(0, 3, txt);
			// DC -12V
			txt = norm_t;
			if (voltmeter.dc12n == 0) {
				txt = z_text;
			} else if (voltmeter.dc12n_low) {
				txt = low_t;
			} else if (voltmeter.dc12n_high) {
				txt = high_t;
			}
			lcd.Print(7, 3, txt);
			break;

	}

	// Control del LED
	pw_led_timer = millis();

}