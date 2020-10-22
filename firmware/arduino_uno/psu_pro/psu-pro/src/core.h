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



#ifndef __CORE_H
#define __CORE_H



/*** Includes ***/
// Arduino
#include <Arduino.h>
// Proyecto
#include "in_out.h"
#include "voltmeter.h"
#include "lcd_routines.h"
#include "dht_sensor.h"



/*** Definicion de la clase ***/
class Core {
	
	public:
	
		// Constructor
		Core();
		
		// Destructor
		~Core();

        // Codigo inicial
        void Boot();
		
		// Al iniciar el programa
		void Start();
		
		// Ejecuta regularmente
		void Run();
		
	
	private:

		// Constantes
		const String WELCOME_TEXT[4] = {
			"PSU PRO",
			"FIRMWARE V.1.0.0.",
			"(CC) 2020 NIGHTFOX",
			"NIGHTFOXANDCO.COM"
		};

		// Control de tiempo entre ejecuciones
		uint32_t delta_time;
		const uint32_t UPDATE_RATE = 50;

		// Maquina de estados principal
		uint8_t main_st, next_main_st;
		static const uint8_t MAIN_ST_START_UP = 0;
		static const uint8_t MAIN_ST_IDLE = 1;
		static const uint8_t MAIN_ST_POWER_ON = 2;
		static const uint8_t MAIN_ST_STANDBY = 3;
		static const uint8_t MAIN_ST_ERROR = 255;

		// Variables generales
		uint32_t time_counter;			// Contador general
		uint32_t pw_led_timer;			// Cronometro para el LED



		// Estado de arranque (Start Up)
		int8_t StStartUp();									// Estado de arranque
		uint8_t startup_st, next_startup_st;
		static const uint8_t STARTUP_ST_START = 0;			// Pasos previos
		static const uint8_t STARTUP_ST_DHT_TEST = 1;		// Test del sensor DHT
		static const uint8_t STARTUP_ST_FAN_ON = 2;			// Enciende el ventilador
		static const uint8_t STARTUP_ST_FAN_OFF = 3;		// Apaga el ventilador
		static const uint8_t STARTUP_ST_PSU_ON = 4;			// Enciende la fuente de alimentacion
		static const uint8_t STARTUP_ST_VOLTAGE_TEST = 5;	// Test de voltaje
		static const uint8_t STARTUP_ST_VOLTAGE_CHECK = 6;	// Verificacion del voltaje
		static const uint8_t STARTUP_ST_EXIT = 100;			// Fin del start up



		// Estado de espera (Idle)
		int8_t StIdle();



		// Estado de fuente encendida
		int8_t StPowerOn();



		// Estado Stand By
		int8_t StStandBy();
		uint8_t standby_st, next_standby_st;
		static const uint8_t STANDBY_ST_START = 0;				// Pasos previos
		static const uint8_t STANDBY_ST_SHUTDOWN = 1;			// Espera y apagado
		static const uint8_t STANDBY_ST_SLEEP = 2;				// Modo reposo
		static const uint8_t STANDBY_ST_POWER_ON = 3;			// Encendido y espera
		static const uint8_t STANDBY_ST_EXIT = 4;				// Fin del StandBy



		// Gestion de errores
		int8_t StError();



		// Gestion de la entradas/salidas
		InOut io;

		// Voltimetro digital
		Voltmeter voltmeter;
		void VoltRead();			// Lee y analiza los valores del voltimetro
		void VoltInfo();			// Representa en pantalla los valores del voltimetro
	
		// Gestion del panel LCD
		LcdRoutines lcd;
		void UpdateLcd();								// Actualiza el panel LCD
		uint32_t lcd_delta;								// Control de actualizacion del display
		const uint8_t LCD_DELTA_TIME = 200;				// Tiempo entre actualizaciones
		void GuiLayout();								// Diseño de la interfaz
		const uint8_t fan_first_frame = 4;				// Animacion del ventilador
		const uint8_t fan_last_frame = 7;
		uint8_t fan_anim_frame;

		// Acceso al sensor ambiental DHT
		DhtSensor dht;
		void DhtRead();									// Lee y analiza los valores del sensor DHT
		void DhtInfo();									// Representa en pantalla la informacion del DHT
		const uint8_t FAN_START_TEMP = 35;				// Temperatura para encender el ventilador
		const uint8_t FAN_STOP_TEMP = 30;				// Temperatura para apagar el ventilador
		const uint8_t OVERHEAT_TEMP = 45;				// Sobrecalentamiento, pasa automaticamente a modo "IDLE"
		const uint8_t CRITICAL_TEMP = 50;				// Temperatura critica, realiza un apagado de emergencia
		uint8_t temp_state;								// Estado de la temperatura
		const uint8_t TEMP_NORMAL = 0;					// Temperatura normal
		const uint8_t TEMP_WARM = 1;					// Temperatura eleveda, activar el ventilador
		const uint8_t TEMP_OVERHEAT = 2;				// Temperatura por encima del limite, apagar la salida
		const uint8_t TEMP_CRITICAL = 3;				// Temperatura critica, desconectar las fuentes de alimientacion

		// Gestion de la temperatura y el ventilador
		void FanControl();

		// Gestion del StandBy mode
		int8_t CheckStandByPush();						// Verifica si hay cambio de modo
		uint32_t standby_timer;							// Contador
		const uint32_t STANDBY_PUSH_TIME = 2000;		// Tiempo a mantener pulsado el boton para pasar de modo

		// Gestion de errores
		void ErrorHandler(uint8_t err_code);
		const uint8_t ERR_CODE_ID = 0x10;
		static const uint8_t ERR_CODE_DHT = 0x11;				// Error en el sensor DHT
		static const uint8_t ERR_CODE_TEMP_OVERHEAT = 0x12;		// Error de temperatura, sobrecalentamiento
		static const uint8_t ERR_CODE_TEMP_CRITICAL = 0x13;		// Error de temperatura, CRITICA
		static const uint8_t ERR_CODE_VOLT_IN = 0x14;			// Error de voltaje (interno, idle)
		static const uint8_t ERR_CODE_VOLT_EXT = 0x15;			// Error de voltaje (externo, power ON)
		uint8_t error_code;
		uint8_t error_count;
		
};



#endif