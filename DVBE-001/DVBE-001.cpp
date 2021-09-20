#include <SoftwareSerial.h>
#include <EEPROM.h>
#include "HX711.h"

//HX711
#define pinDt A0
#define pinSck A1
HX711 balanza;

//Bluetooth
#define pinTx 3
#define pinRx 4
#define pinState 5
SoftwareSerial BT (pinTx, pinRx);

//Indicadores
#define pinLed 2

//Valores por defecto
#define varDefault 1.0
#define escDefault 1000.0
#define corDefault 1.0
#define pesoMinDefault 50
#define tiempo 10

//Parametros de configuracion
float variacion;
float escala;
float correccion;
int pesoMin;

//Auxiliares
float peso, pesoAnt, pesoSig, pesoEst;
byte condicion;

void setup() {

  //Inicio de puertos
  Serial.begin(9600);
  BT.begin(9600);
  balanza.begin (pinDt, pinSck);

  //E/S
  pinMode(pinState, INPUT);
  pinMode(pinLed, OUTPUT);

  //Configuracion inicial de pinLed
  digitalWrite(pinLed, LOW);

  //Recoleccion de datos EXCEL
  /*Serial.println("CLEARDATA");
    Serial.println("LABEL,HORA,TIEMPO,PESO,DIFERENCIA DE PESO, PESO ESTABLE");
    Serial.println("RESETTIMER");*/


  //Grabar EEPROM por primera vez con valores por defecto
  int eepr;
  EEPROM.get(14, eepr);
  if (eepr == -1) {
    EEPROM.put(0, varDefault);//variacion
    EEPROM.put(4, escDefault);//escala
    EEPROM.put(8, corDefault);//correccion
    EEPROM.put(12, pesoMinDefault);//peso minimo estable
    EEPROM.put(14, 1);//estado eeprom
  }

  //Obtener valores guardados en la EEPROM
  EEPROM.get(0, variacion);
  EEPROM.get(4, escala);
  EEPROM.get(8, correccion);
  EEPROM.get(12, pesoMin);

  //Configuraciones iniciales de escala y tara
  balanza.set_scale(escala);
  delay(100);
  balanza.tare(10);

}

void loop() {

  //Recoleccion de datos EXCEL
  /* Serial.print("DATA,TIME,TIMER,");
    Serial.print(peso);
    Serial.print(",");
    Serial.print(peso - pesoAnt);
    Serial.print(",");
    Serial.print("0");
    Serial.println("");*/

  //Pulsar led en el caso de estar buscando conexion BT (Solo para HM-10,HC-05)
  while (digitalRead(pinState) == 0) {
    condicion = 1;
    digitalWrite(pinLed, LOW);
    delay(1000);
    digitalWrite(pinLed, HIGH);
    delay(1000);
  }
  digitalWrite(pinLed, LOW);

  // Mensaje para configurador serial (solo se ve una vez cuando establesco conexion)
  if (digitalRead(pinState) == 1 && condicion == 1) {
    condicion = 0;
    BT.println("Escriba 'm' + 'Enviar' para ingresar al menu de configuracion de balanza");
    delay(2000);   
  }

  pesoAnt = balanza.get_units(2);
  peso = balanza.get_units(2);
  pesoSig = balanza.get_units(2);

  //Imprime datos en el puerto BT
  BT.println("P" + (String)round(pesoSig));
  delay(tiempo);

  estable();

  //Lee datos del puerto BT
  if (BT.available()) {
    char lectura = BT.read();
    switch (lectura) {
      case 'T':
        Serial.println("Destarando");
        balanza.tare(10);
        break;

      case 'm':
        menu();
        break;
    }
  }

}

void estable() {

  if (abs(pesoAnt - peso) <= variacion && abs(pesoSig - peso) <= variacion && abs(peso) > pesoMin) {

    pesoEst = peso;

    while (peso > pesoMin) {

      //Recoleccion de datos EXCEL
      /*Serial.print("DATA,TIME,TIMER,");
        Serial.print(peso);
        Serial.print(",");
        Serial.print("0");
        Serial.print(",");
        Serial.print(pesoEst);
        Serial.println(" ,");*/

      //Imprime datos en puerto BT
      BT.println("E" + (String)round(pesoEst));
      delay (tiempo);

      //Lee balanza para saber si todavia hay carga en la balanza
      peso = balanza.get_units(5);

      //Lee datos del puerto BT para escuchar la tara
      if (BT.available()) {
        char lectura = BT.read();

        switch (lectura) {
          case 'T':
            repesado();
            estable();
            break;

          case 'm':
            menu();
            break;
        }
      }
    }
  }
}

void menu() {

  char dato;

  BT.println("MENU\n\na. Establecer escala \nb. Peso minimo estable \nc. Velocidad Estabilizacion \ng. Guardar valores en EEPROM  \nr. Reestablecer valores por defecto \ns. Salir");


  BT.read(); //Limpiar Buffer
  dato = BT.read();

  while (!BT.available()) {
  }

  dato = BT.read();

  switch (dato) {

    case 'a':
      BT.println("Escala actual: " + (String)escala );
      BT.println("Ingrese el peso conocido en Kg 's' para salir");
      correccionPesoF();
      break;

    case 'b':
      BT.println("Peso minimo estable actual: " + (String)pesoMin);
      BT.println("Ingrese un valor de 10 a 500 o 's' para salir");
      pesoMinF();
      break;

    case 'c':
      BT.println("Velocidad de Estabilizacion: " + (String)variacion);
      BT.println("Ingrese un valor de 0,1 a 5 o 's' para salir");
      variacionF();
      break;

    case 'g':
      eepromGuardar();
      break;

    case 'r':
      eepromReset();
      break;

    case 's':
      BT.println("Saliendo...\nListo!");
      loop();
      break;

    default:
      BT.println("Comando incorrecto");
      menu();
      break;
  }
}

//funcion creeada para que cuando recibe varias taras en la reestabilizacion del peso no me haga un a tara a cero se logro quitando la capacidad de lectura mientras hace el repesaje
void repesado() {
  BT.read(); //Limpiar Buffer
  //Recoleccion de datos EXCEL
  /* Serial.print("DATA,TIME,TIMER,");
    Serial.print(peso);
    Serial.print(",");
    Serial.print(peso - pesoAnt);
    Serial.print(",");
    Serial.print("0");
    Serial.println("");*/

  pesoAnt = balanza.get_units(2);
  peso = balanza.get_units(2);
  pesoSig = balanza.get_units(2);

  //Imprime datos en el puerto BT
  BT.println("P" + (String)round(pesoSig));
  delay(tiempo);

  while (BT.available()) {
    BT.read();
  }
}

void correccionPesoF() {

  String str;
  float correccionAux = 0;
  float lecturaAux;
  float promedio = 0;


  BT.read(); //limpiar buffer
  BT.read();
  BT.read();

  while (!BT.available()) {
  }

  str = BT.readStringUntil('\n');
  char strAux = str.charAt(0);
  correccionAux = str.toFloat();

  if (strAux == 's') {
    menu();
  } else if (correccionAux >= 1 && correccionAux <= 5000) {

    correccion = correccionAux;

    balanza.set_scale(1);
    delay(300);
    lecturaAux = balanza.get_units(10);

    float nuevaEscala = lecturaAux / correccion;

    balanza.set_scale(nuevaEscala);
    escala = nuevaEscala;

    BT.println("Nueva escala establecida: " + (String)nuevaEscala + "\n");
    delay(1500);
    menu();

  } else {
    BT.println("Valor incorrecto\nIngrese un valor de 1 a 5000");
    correccionPesoF();
  }
}

void pesoMinF() {

  String str;
  float pesoMinAux = 0;

  BT.read(); //limpiar buffer
  BT.read();
  BT.read();

  while (!BT.available() && digitalRead(pinState) == 1) {
  }

  str = BT.readStringUntil('\n');
  char strAux = str.charAt(0);
  pesoMinAux = str.toInt();

  if (strAux == 'S' || strAux == 's') {
    menu();

  } else if (pesoMinAux >= 10  && pesoMinAux <= 500) {
    pesoMin = pesoMinAux;
    BT.println("Cambio correcto\nPeso minimo estable: " + (String)pesoMin + "\n");
    menu();

  } else {
    BT.println("Valor incorrecto\nIngrese un valor de 10 a 500");
    pesoMinF();
  }
}

void variacionF() {

  String str;
  float variacionAux = 0;

  BT.read(); //limpiar buffer
  BT.read();
  BT.read();

  while (!BT.available()) {
  }

  str = BT.readStringUntil('\n');
  char strAux = str.charAt(0);
  variacionAux = str.toFloat();

  if (strAux == 'S' || strAux == 's') {
    menu();

  } else if (variacionAux >= 0.1  && variacionAux <= 5) {
    variacion = variacionAux;
    BT.println("Cambio correcto\nVariacion: " + (String)variacion + "\n");
    menu();

  } else {
    BT.println("Valor incorrecto\nIngrese un valor de 0,1 a 5");
    variacionF();
  }
}

void eepromGuardar() {

  char confirmacion;


  BT.println("¿Desea guardar valores en la EEPROM s|n?");
  
  BT.read(); //limpiar buffer
  delay(2000);
  BT.read();


  while (!BT.available()) {
  }

  confirmacion = BT.read();

  switch (confirmacion) {
    case 's':

      EEPROM.put(0, variacion);
      EEPROM.put(4, escala);
      EEPROM.put(8, correccion);
      EEPROM.put(12, pesoMin);

      BT.println("Valores guardados");
      delay(1000);
      menu();
      break;

    case 'n':
      BT.println("Valores no guardados");
      menu();
      break;

    default:
      BT.println("Comando incorrecto");
      eepromGuardar();
      break;
  }
}

void eepromReset() {

  char confirmacion;

  BT.println("¿Desea reestablecer valores por defecto s|n?");
  
  BT.read(); //limpiar buffer
  delay(2000);
  BT.read();
 
  while (!BT.available()) {
  }

  confirmacion = BT.read();

  switch (confirmacion) {
    case 's':
      EEPROM.put(0, varDefault);//variacion
      EEPROM.put(4, escDefault);//escala
      EEPROM.put(8, corDefault);//correccion
      EEPROM.put(12, pesoMinDefault);//peso minimo estable

      EEPROM.get(0, variacion);
      EEPROM.get(4, escala);
      EEPROM.get(8, correccion);
      EEPROM.get(12, pesoMin);


      BT.println("Valores reestablecidos");
      delay(1000);
      menu();
      break;

    case 'n':
      BT.println("Valores no reestablecidos");
      menu();
      break;

    default:
      BT.println("Comando incorrecto");
      eepromReset();
      break;
  }
}
