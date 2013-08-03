/*
 Shifter Analog-Clock
 for two 74HC595 shift registers

 This sketch turns on each of the LEDs attached to two 74HC595 shift registers,
 in sequence from output 0 to output 15.

 Hardware:
 * 2 74HC595 shift register attached to pins 8, 11, and 12 of the Arduino,
 as detailed below.
 * LEDs attached to each of the outputs of the shift register

 Created 02 Jul 2013
 Modified 01 Aug 2013
 by Omar Miranda & Sebastian Ferrada

 */

// Entradas de Arduino
const int latchPin = 8;
const int clockPin = 12;
const int dataPin = 11;
// Variables utilizadas para el manejo del tiempo
unsigned int minutes = 1;
unsigned int hours = 1;

void setup() {
  	pinMode(latchPin, OUTPUT);
	pinMode(dataPin, OUTPUT);
	pinMode(clockPin, OUTPUT);
	Serial.begin(9600);
}

void loop() {

	// Iteramos sobre los outputs de los 2 shift registers
	for (int thisLed = 0; thisLed < 16; thisLed++) {
		// Escribimos en los shifters
    		registerWrite(thisLed, HIGH, minutes);
		// Apagamos el led anterior; si es el primero apagamos el último
    		if (thisLed > 0)
			registerWrite(thisLed - 1, LOW, minutes);
		else 
			registerWrite(15, LOW, minutes);
	}

	// Un par de comparaciones matemáticas para mantener/calcular los minutos
	// y horas acorde al sistema métrico (Aumentamos minutos y horas, de acuerdo
	// a la cantidad de segundos actuales
	if(minutes >= 32768) {
		minutes = 1;
		if(hours >= 32768)
			hours = 1;
		else	hours = hours*2;
	}
	else	minutes=minutes*2;
}

// This method sends bits to the shift registers:

void registerWrite(int whichPin, int whichState, int minutes) {
	
	// Esta función en teoría es la que envia informacion a los shift

	// Usamos unsigned int, para poder ocupar 16 bits
	unsigned int bitsToSend = 0;    

	// Apagamos los leds para que no se prendan mientras "shifteamos"
	digitalWrite(latchPin, LOW);

	// prendemos el siguiente bit mas alto
	bitWrite(bitsToSend, whichPin, whichState);

	//Hacemos un OR bitwise para prender el minutero y horario
	bitsToSend = bitsToSend | minutes;
	bitsToSend = bitsToSend | hours;
	
	// separamos los bits para cada shifter (SH y SL, si S fuera un registro assembler)
	byte registerOne = highByte(bitsToSend);
	byte registerTwo = lowByte(bitsToSend);
	
	// "shift the bytes out", sigue como un misterio para nosotros esta funcion
	// pero estamos conciente que es la que realmente "shiftea" los datos del shift register
	shiftOut(dataPin, clockPin, MSBFIRST, registerOne);
	shiftOut(dataPin, clockPin, MSBFIRST, registerTwo);

	// prendemos finalmente los Leds
	digitalWrite(latchPin, HIGH);
	
	// hacemos el delay correspondiente a 1 segundo	
	if(whichState == HIGH)
		delay(1000);

}
