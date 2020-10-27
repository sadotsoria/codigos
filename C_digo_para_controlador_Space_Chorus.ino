/*
   Declaramos pines de entrada y hacemos variables
   para alojar los valores leídos de los pines.
*/

// ------------- Botones
// entradas digitales
int dPinIn1 = 2;
int dPinIn2 = 3;
int dPinIn3 = 4;
int dPinIn4 = 5;
int dPinInSt = 7;
int dPinIn[4] =  {dPinIn1, dPinIn2, dPinIn3, dPinIn4};

// variables para guardar los valores de los pines de entrada
int dIn1Val = 0;
int dIn2Val = 0;
int dIn3Val = 0;
int dIn4Val = 0;
//int dInStVal = 0;
int dInVal[4] = {dIn1Val, dIn2Val, dIn3Val, dIn4Val};

// variables de secuencia
boolean on1 = false;
boolean on2 = false;
boolean on3 = false;
boolean on4 = false;
boolean ons[4] = {on1, on2, on3, on4};
int botCtrlN[4] = {6, 7, 8, 9};

// variable para indicar el estado del controlador
//int state = 0;

// Banderas para lectura de pines digitales
boolean dIn1ReadFlag = true;
boolean dIn2ReadFlag = true;
boolean dIn3ReadFlag = true;
boolean dIn4ReadFlag = true;
//boolean dInStReadFlag = true;
boolean dInReadFlags[4] = {dIn1ReadFlag, dIn2ReadFlag, dIn3ReadFlag, dIn4ReadFlag};

// entradas analógicas
// Potenciometros
int pot1Pin = A0;
int pot1Val = 0;
int pot2Pin = A1;
int pot2Val = 0;
int pot3Pin = A3;
int pot3Val = 0;
int pot4Pin = A4;
int pot4Val = 0;
int potPin[4] = {pot1Pin, pot2Pin, pot3Pin, pot4Pin};
int potVal[4] = {pot1Val, pot2Val, pot3Val, pot4Val};

// fotoresistencia
int fotoRPin = A5;
int fotoRVal = 0;

// numero de control
int pot1 = 1;
int pot2 = 2;
int pot3 = 3;
int pot4 = 4;
int potN[4] = {pot1, pot2, pot3, pot4};

int foto = 5;


// salidas
int dPinOut1 = 8;
int dPinOut2 = 9;
int dPinOut3 = 10;
int dPinOut4 = 11;
//int dPinOutSt = 13;
int dPinOut[4] = {dPinOut1, dPinOut2, dPinOut3, dPinOut4};

// ---------------------------------------------------------------------
void setup() {
  // inicializamos el puerto en serie
  // Configuramos el baud rate a 115200
  // ya que es el valor con el cual viene
  // configurado Hairless MIDI por omisión
  Serial.begin(115200);
  // Configuramos los pines GPIO de entrada para implementar una
  // resistencia tipo pullup:
  // 0 -> apretado, 1-> abierto

  for (int i = 0; i < 4; i++) {
    // Configuramos los pines GPIO de entrada
    pinMode(dPinIn[i], INPUT_PULLUP);
    // Configuramos los pines GPIO de salida
    pinMode(dPinOut[i], OUTPUT);
  }

}

// ---------------------------------------------------------------------
void loop() {
  


  // Lee la fotoresistencia
  fotoRVal = map(analogRead(fotoRPin), 0, 1023, 0, 127);

  // Lee las cuatro entradas digitales y las 4 entradas analógicas una por una
  for (int i = 0; i < 4; i++) {
    // lee las entradas digitales (Botones);
    dInVal[i] = digitalRead(dPinIn[i]);

    // lee las entradas analógicas (Potenciómetros) y
    // mapea los valores que van a representar valores de nota MIDI
      potVal[i] = map(analogRead(potPin[i]), 0, 1023, 48, 72);

    // Checa el estado de cada botón...
    // Asígnale a cada paso un estado:[true, false].
    if ( dInVal[i] == 0 && ons[i] == false && dInReadFlags[i] == true ) {
      ons[i] = true;
      dInReadFlags[i] = false;
    } else if (dInVal[i] == 0 && ons[i] == true && dInReadFlags[i] == true) {
      ons[i] = false;
      dInReadFlags[i] = false;
    }

    // Si está prendido indicalo prendiendo el LED que le corresponde
    if (ons[i]) {
      digitalWrite(dPinOut[i], HIGH);
      cc(0, botCtrlN[i], 127);

    }
    // Si no está prendido indicalo apagando el LED que le corresponde
    else {
      digitalWrite(dPinOut[i], LOW);
      cc(0, botCtrlN[i], 0);

    }

// Transmite los valores de los potenciómetros 
      cc(0, potN[0], potVal[0]);
      cc(0, potN[1], potVal[1]);
      cc(0, potN[2], potVal[2]);
      cc(0, potN[3], potVal[3]);

    // Transmite el mensaje de control que se produce de la fotoresistencia.
    cc(0, foto, fotoRVal);
    
    // Re establece el valor de dInReadFlag si el interruptor (botón)
    // esta abierto, recuerda que utilizamos una entrada pullup.
    if (dInVal[i] == 1 && dInReadFlags[i] == false) {
      dInReadFlags[i] = true;
    }

  }
  // Aquí termina el bucle de control de la estructura "for"


  // Implementa un ligero delay entre cada ejecución.
  delay(50);
}

// ---------------------------------------------------------------------

// Envía el comando noteOn
void noteOn(int chan, int nota, int vel) {
  int cmd = 0x90 | chan;
  Serial.write(cmd);
  Serial.write(nota);
  Serial.write(vel);
};

// Envía el comando noteOff
void noteOff(int chan, int nota, int vel) {
  int cmd = 0x80 | chan;
  Serial.write(cmd);
  Serial.write(nota);
  Serial.write(vel);
};

// Envía el comando Control Change
void cc(int chan, int num, int val) {
  int cmd = 0xb0 | chan;
  Serial.write(cmd);
  Serial.write(num);
  Serial.write(val);
};
