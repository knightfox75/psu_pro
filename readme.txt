PSU Pro
Para Commodore C64, Spectrum, Amstrad CPC y MSX.
PSU Pro es una fuente de alimentación universal orientada al diagnostico y reparación de microordenadores clásicos, como el Commodore C64, Spectrum +2 y +3, Amstrad CPC 464 y algunos modelos de MSX. 

1. Voltajes suministrados.
PSU Pro suministra los siguientes voltajes:
C.C. (DC) de +5 V con una intensidad máxima de 5 A.
C.C. (DC) de +12 V con una intensidad máxima de 2.5 A.
C.C. (DC) de -12V con una intensidad máxima de 0.5 A.
C.A. (AC) de 9 V con una intensidad máxima de 1.5 A.
Estos voltajes están disponibles a través de las diferentes bananas de conexión y del conector DIN-7 del frontal.

2. Sistema de diagnóstico y control inteligente.
Una placa Arduino UNO se encarga de la supervisión y control de las diferentes salidas de la fuente de alimentación, así como del encendido de los diferentes módulos de alimentación, incluyendo el control de la temperatura interior de la fuente y la activación del sistema de ventilación.

3. La seguridad, ante todo.
La placa Arduino, conjuntamente con componentes de electrónica discreta, supervisan el proceso de encendido de la fuente, verifican el funcionamiento de los sensores, prueban los ventiladores a cada encendido y supervisan los voltajes de salida de los diferentes módulos. Si alguno de los voltajes esta fuera de rango, se impide la habilitación de la salida, lo que protege al equipo conectado de cualquier daño. Un subida o caída de tensión o un cortocircuito en el aparato conectado a la salida de la fuente, provoca la desconexión automática de la misma. Si el interior de la fuente alcanza la temperatura programada, se activa el sistema de ventilación automáticamente. En caso de no poder reducirse la temperatura, se desconectará la salida para proteger los módulos de la fuente. Si la temperatura alcanza niveles críticos, la fuente desconectara la alimentación de todos los módulos, entrando en modo de suspensión.

4. Control de lo que ocurre.
Una pantalla LCD de 4 líneas informa del proceso de encendido, estado de las salidas, voltajes, temperatura y humedad relativas del interior de la fuente y activación del sistema de ventilación. El LED en el botón de encendido también nos indica la actividad de ciertos procesos. Una pulsación breve en este botón, conectará o desconectará las salidas de la fuente, devolverá la fuente al estado de espera en caso de error o de encontrarse en modo de suspensión la reactivará. Si en cualquier momento se mantiene pulsado durante unos segundos, forzaremos la fuente a entrar en modo de suspensión.

5. Construcción interna.
Exceptuando la electrónica necesaria para la alimentación de la placa Arduino y la electrónica para algunos circuitos de control y medida del voltaje, se han usado siempre que ha sido posible módulos de alimentación o control estandarizados, usados habitualmente en montajes relacionados con Arduino.

6. ¿Que se suministra?
Todos los diagramas, código fuente y serigrafías de muestra para la construcción de la fuente de alimentación.

7. Descargo de responsabilidad.
El material aquí presente se cede con el espíritu de compartir el conocimiento para su estudio y/o difusión. Aunque toda la información aquí suministrada se ha verificado con la construcción de un prototipo funcional, el autor no puede hacerse ni se hace responsable de la construcción de cualquier dispositivo fabricado en base a la información aquí suministrada, ni de los posibles daños a bienes o personas que el uso de la misma pueda ocasionar.

8. Licencia.
Toda la documentación e información aquí suministrada se distribuye bajo el amparo de la licencia CREATIVE COMMONS "Attribution-NonCommercial 4.0 International"
https://creativecommons.org/licenses/by-nc/4.0/

9. Contacto.
e-mail: contact@nightfoxandco.com
web: https://nightfoxandco.com

