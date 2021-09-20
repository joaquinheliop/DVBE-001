#include <SoftwareSerial.h>
#include <EEPROM.h>
SoftwareSerial BT (9, 8);

//variables menu
String str, strBt;
int temp = 0,
    aux = 0;

//variables para determinar boton que se pulsó
char val;
char valBt;
int bot1, bot2, bot3;

//variables para asignar numero a mi funcion accion, determinan que rele se activa
int r1, r2;

//auxiliares para evitar accion si el pulsador se mantiene
int control, val1;

//guarda los estados y posicionamiento de los cilindros
int estado[3] = {0, 1, 1}, n;

//auxiliares para configurar los amortiguamientos individuales
int amort1, amort2;

//amortiguamientos individuales en orden
int am11 ,
    am12 ,
    am21 ,
    am22 ,
    am31 ,
    am32 ;

//tiempo de amortiguamiento
int amortT = 300 ;

//////////////////////////////////////////

void setup() {
  //relays cilindro 1
  pinMode(2, OUTPUT);
  pinMode(3, OUTPUT);
  //relays cilindro 2
  pinMode(4, OUTPUT);
  pinMode(5, OUTPUT);
  //relays cilindro 3
  pinMode(6, OUTPUT);
  pinMode(7, OUTPUT);
  //alimentacion
  pinMode(8, OUTPUT);

  //configuracion salidas
  digitalWrite(2, HIGH);
  digitalWrite(3, HIGH);
  digitalWrite(4, HIGH);
  digitalWrite(5, HIGH);
  digitalWrite(6, HIGH);
  digitalWrite(7, HIGH);

  //iniciar puerto serial y bt
  Serial.begin(9600);
  Serial.println("Serial iniciado");
  BT.begin(9600);
  BT.println("BT iniciado");

  //grabar EEPROM por primera vez con valores por defecto
  int eepr;
  EEPROM.get(12, eepr);

  if (eepr == -1) {
    EEPROM.put(12, 1);
    EEPROM.put(0, 450);
    EEPROM.put(2, 1220);
    EEPROM.put(4, 1800);
    EEPROM.put(6, 1050);
    EEPROM.put(8, 1050);
    EEPROM.put(10, 1500);
    Serial.println("Valores por defecto establecidos");
    BT.println("Valores por defecto establecidos");
  }

  //obtener valores guardados en la EEPROM
  EEPROM.get(0, am11);
  EEPROM.get(2, am12);
  EEPROM.get(4, am21);
  EEPROM.get(6, am22);
  EEPROM.get(8, am31);
  EEPROM.get(10, am32);

  //alimentacion de los relays despues de 0,5[s]
  /* delay(500);
    digitalWrite(8, HIGH);*/

  //mensaje de menu
  BT.println("M para abrir el menu");
  BT.println("");
  Serial.println("M para abrir el menu");
  Serial.println("");


  //configuracion de electrovalvulas en estado inicial neutro
  Serial.println("Moviendo puerta Central...");
  BT.println("Moviendo puerta Central...");
  digitalWrite(2, LOW);
  delay(200);
  digitalWrite(2, HIGH);

  delay(am12);

  digitalWrite(1, LOW);
  delay(200);
  digitalWrite(1, HIGH);

  delay(amortT);

  digitalWrite(2, LOW);
  delay(200);
  digitalWrite(2, HIGH);
  Serial.println("Listo");
  BT.println("Listo");

  accionI();
  accionD();
  int estado[3] = {0 , 0 , 0};
}

void loop() {

  val = Serial.read();
  valBt = BT.read();

  //Puerta Central
  if (val == 'a' || valBt == 'a') {
    Serial.println("Moviendo puerta Central...");
    BT.println("Moviendo puerta Central...");
    r1 = 2;
    r2 = 3;
    n = 0;
    amort1 = am11;
    amort2 = am12;
    accion();
    Serial.println("Listo");
    BT.println("Listo");
    return;
  }

  //Puerta Lateral Derecha
  if (val == 'b' || valBt == 'b') {
    accionI();
    Serial.println("Moviendo puerta Derecha...");
    BT.println("Moviendo puerta Derecha...");
    r1 = 4;
    r2 = 5;
    n = 1;
    amort1 = am21;
    amort2 = am22;
    accion();
    Serial.println("Listo");
    BT.println("Listo");
    return;
  }

  //Puerta Lateral Izquierda
  if (val == 'c' || valBt == 'c') {
    accionD();
    Serial.println("Moviendo puerta Izquierda...");
    BT.println("Moviendo puerta Izquierda...");
    r1 = 6;
    r2 = 7;
    n = 2;
    amort1 = am31;
    amort2 = am32;
    accion();
    Serial.println("Listo");
    BT.println("Listo");
    return;
  }

  //Menu
  if (val == 'm' || val == 'M' || valBt == 'm' || valBt == 'M' ) {
    menu();
  }
}

///////////////////////////////////////////////////

//Acciona la puierta segun su estado anterior
void accion() {

  estado[n] = 1 - estado[n]; //guarda el estado anterior

  delay(500);

  switch (estado[n]) {
    case 1:
      digitalWrite(r1, LOW);
      delay(200);
      digitalWrite(r1, HIGH);

      delay(amort1);

      digitalWrite(r2, LOW);
      delay(200);
      digitalWrite(r2, HIGH);

      delay(amortT);

      digitalWrite(r1, LOW);
      delay(200);
      digitalWrite(r1, HIGH);
      break;
    case 0:
      digitalWrite(r2, LOW);
      delay(200);
      digitalWrite(r2, HIGH);

      delay(amort2);

      digitalWrite(r1, LOW);
      delay(200);
      digitalWrite(r1, HIGH);

      delay(amortT);

      digitalWrite(r2, LOW);
      delay(200);
      digitalWrite(r2, HIGH);
      break;
  }
  return;
}

////////////////////////////////////////////

//Puesta en neutro puerta Izquierda
void accionI() {
  amort2 = am22;
  r1 = 6;
  r2 = 7;
  n = 2;

  delay(500);

  switch (estado[n]) {
    case 1:
      Serial.println("Moviendo puerta Izquierda...");
      BT.println("Moviendo puerta Izquierda...");

      digitalWrite(r2, LOW);
      delay(200);
      digitalWrite(r2, HIGH);

      delay(amort2);

      digitalWrite(r1, LOW);
      delay(200);
      digitalWrite(r1, HIGH);

      delay(amortT);

      digitalWrite(r2, LOW);
      delay(200);
      digitalWrite(r2, HIGH);

      estado[n] = 1 - estado[n];

      Serial.println("Listo");
      BT.println("Listo");
      break;
    case 0:
      break;
  }
}

/////////////////////////////////////////////////

//Puesta en neutro puesta Derecha
void accionD() {
  amort2 = am32;
  r1 = 4;
  r2 = 5;
  n = 1;

  delay(500);

  switch (estado[n]) {
    case 1:
      Serial.println("Moviendo puerta Derecha...");
      BT.println("Moviendo puerta Derecha...");
      digitalWrite(r2, LOW);
      delay(200);
      digitalWrite(r2, HIGH);

      delay(amort2);

      digitalWrite(r1, LOW);
      delay(200);
      digitalWrite(r1, HIGH);

      delay(amortT);

      digitalWrite(r2, LOW);
      delay(200);
      digitalWrite(r2, HIGH);

      estado[n] = 1 - estado[n];
      Serial.println("Listo");
      BT.println("Listo");
      break;
    case 0:
      break;
  }
}

/////////////////////////////////////////////////////////////

void menu() {
menu1:
  Serial.println("-------------------------  MENU  -------------------------------");
  BT.println("-------------------------  MENU  -------------------------------");

  Serial.println("----------------------------------------------------------------");
  BT.println("----------------------------------------------------------------");

  Serial.print("a. Temp. Amortiguacion puerta central 1: ");
  Serial.println(am11);
  BT.print("a. Temp. Amortiguacion puerta central 1: ");
  BT.println(am11);

  Serial.print("b. Temp. Amortiguacion puerta central 2: ");
  Serial.println(am12);
  BT.print("b. Temp. Amortiguacion puerta central 2: ");
  BT.println(am12);

  Serial.println("");
  BT.println("");

  Serial.print("c. Temp. Amortiguacion puerta L.Der 1: ");
  Serial.println(am21);
  BT.print("c. Temp. Amortiguacion puerta L.Der 1: ");
  BT.println(am21);

  Serial.print("d. Temp. Amortiguacion puerta L.Der 2: ");
  Serial.println(am22);
  BT.print("d. Temp. Amortiguacion puerta L.Der 2: ");
  BT.println(am22);

  Serial.println("");
  BT.println("");

  Serial.print("e. Temp. Amortiguacion puerta L.Izq 1: ");
  Serial.println(am31);
  BT.print("e. Temp. Amortiguacion puerta L.Izq 1: ");
  BT.println(am31);

  Serial.print("f. Temp. Amortiguacion puerta L.Izq 2: ");
  Serial.println(am32);
  BT.print("f. Temp. Amortiguacion puerta L.Izq 2: ");
  BT.println(am32);

  Serial.println("----------------------------------------------------------------");
  BT.println("----------------------------------------------------------------");

  Serial.println("r. Reset valores");
  BT.println("r. Reset valores");

  Serial.println("g. Guardar valores en EEPROM");
  BT.println("g. Guardar valores en EEPROM");

  Serial.println("----------------------------------------------------------------");
  BT.println("----------------------------------------------------------------");

  Serial.println("s. Salir");
  BT.println("s. Salir");

  while (val != 's' && val != 'S' && valBt != 's' && valBt != 'S') {
    val = Serial.read();
    valBt = BT.read();

    switch (val) {
      case 'a':
        Serial.print("Temp. Amortiguacion puerta central 1: ");
        Serial.println(am11);
        BT.print("Temp. Amortiguacion puerta central 1: ");
        BT.println(am11);
        asignarValor();
        am11 = aux;
        goto menu1;
        break;

      case 'b':
        Serial.print("Temp. Amortiguacion puerta central 2: ");
        Serial.println(am12);
        BT.print("Temp. Amortiguacion puerta central 2: ");
        BT.println(am12);
        asignarValor();
        am12 = aux;
        goto menu1;
        break;

      case 'c':
        Serial.print("Temp. Amortiguacion puerta L.Izq 1: ");
        Serial.println(am21);
        BT.print("Temp. Amortiguacion puerta L.Izq 1: ");
        BT.println(am21);
        asignarValor();
        am21 = aux;
        goto menu1;
        break;

      case 'd':
        Serial.print("Temp. Amortiguacion puerta L.Izq 2: ");
        Serial.println(am22);
        BT.print("Temp. Amortiguacion puerta L.Izq 2: ");
        BT.println(am22);
        asignarValor();
        am22 = aux;
        goto menu1;
        break;

      case 'e':
        Serial.print("Temp. Amortiguacion puerta L.Der 1: ");
        Serial.println(am31);
        BT.print("Temp. Amortiguacion puerta L.Der 1: ");
        BT.println(am31);
        asignarValor();
        am31 = aux;
        goto menu1;
        break;

      case 'f':
        Serial.print("Temp. Amortiguacion puerta L.Der 2: ");
        Serial.println(am32);
        BT.print("Temp. Amortiguacion puerta L.Der 2: ");
        BT.println(am32);
        asignarValor();
        am32 = aux;
        goto menu1;
        break;

      case 'r':
        char confirmacion;
        Serial.println(" ¿Desea resetear los valores a los de fabrica? S | N");
        BT.print(" ¿Desea resetear los valores a los de fabrica? S | N");

        while (confirmacion != '&' && confirmacion != '&') {
          confirmacion = Serial.read();

          switch (confirmacion) {
            case 's':
              EEPROM.put(0, 450);
              EEPROM.put(2, 1220);
              EEPROM.put(4, 1800);
              EEPROM.put(6, 1050);
              EEPROM.put(8, 1050);
              EEPROM.put(10, 1500);
              Serial.println("Valores reseteados");
              BT.print("Valores resteados");
              valoresEeprom();
              goto menu1;
              break;

            case 'n':
              Serial.println("Valores no reseteados");
              BT.println("Valores no reseteados");
              delay(2000);
              goto menu1;
              break;
          }
        }

      case 'g':
        char confirmacion1;
        Serial.println(" ¿Desea guardar valores en la EEPROM? S | N");
        BT.print(" ¿Desea guardar valores en la EEPROM? S | N");

        while (confirmacion != '&' && confirmacion != '&') {
          confirmacion = Serial.read();

          switch (confirmacion) {
            case 's':
              EEPROM.put(0, am11);
              EEPROM.put(2, am12);
              EEPROM.put(4, am21);
              EEPROM.put(6, am22);
              EEPROM.put(8, am31);
              EEPROM.put(10, am32);
              Serial.println("Valores guardados");
              BT.println("Valores guardados");
              delay(2000);
              valoresEeprom();
              goto menu1;
              break;

            case 'n':
              Serial.println("Valores no guardados");
              BT.println("Valores no guardados");
              delay(2000);
              goto menu1;
              break;

          }
        }
        break;
    }

    /////MENU BLUETOOTH/////
    switch (valBt) {
      case 'a':
        Serial.print("Temp. Amortiguacion puerta central 1: ");
        Serial.println(am11);
        BT.print("Temp. Amortiguacion puerta central 1: ");
        BT.println(am11);
        asignarValorBt();
        am11 = aux;
        goto menu1;
        break;

      case 'b':
        Serial.print("Temp. Amortiguacion puerta central 2: ");
        Serial.println(am12);
        BT.print("Temp. Amortiguacion puerta central 2: ");
        BT.println(am12);
        asignarValorBt();
        am12 = aux;
        goto menu1;
        break;

      case 'c':
        Serial.print("Temp. Amortiguacion puerta L.Izq 1: ");
        Serial.println(am21);
        BT.print("Temp. Amortiguacion puerta L.Izq 1: ");
        BT.println(am21);
        asignarValorBt();
        am21 = aux;
        goto menu1;
        break;

      case 'd':
        Serial.print("Temp. Amortiguacion puerta L.Izq 2: ");
        Serial.println(am22);
        BT.print("Temp. Amortiguacion puerta L.Izq 2: ");
        BT.println(am22);
        asignarValorBt();
        am22 = aux;
        goto menu1;
        break;

      case 'e':
        Serial.print("Temp. Amortiguacion puerta L.Der 1: ");
        Serial.println(am31);
        BT.print("Temp. Amortiguacion puerta L.Der 1: ");
        BT.println(am31);
        asignarValorBt();
        am31 = aux;
        goto menu1;
        break;

      case 'f':
        Serial.print("Temp. Amortiguacion puerta L.Der 2: ");
        Serial.println(am32);
        BT.print("Temp. Amortiguacion puerta L.Der 2: ");
        BT.println(am32);
        asignarValorBt();
        am32 = aux;
        goto menu1;
        break;

      case 'r':
        char confirmacion;
        Serial.println(" ¿Desea resetear los valores a los de fabrica? S | N");
        BT.println(" ¿Desea resetear los valores a los de fabrica? S | N");

        while (confirmacion != '&' && confirmacion != '&') {
          confirmacion = BT.read();

          switch (confirmacion) {
            case 's':
              EEPROM.put(0, 450);
              EEPROM.put(2, 1220);
              EEPROM.put(4, 1800);
              EEPROM.put(6, 1050);
              EEPROM.put(8, 1050);
              EEPROM.put(10, 1500);
              Serial.println("Valores reseteados");
              BT.println("Valores resteados");
              valoresEeprom();
              goto menu1;
              break;

            case 'n':
              Serial.println("Valores no reseteados");
              BT.println("Valores no reseteados");
              delay(2000);
              goto menu1;
              break;
          }
        }

      case 'g':
        char confirmacion1;
        Serial.println(" ¿Desea guardar valores en la EEPROM? S | N");
        BT.println(" ¿Desea guardar valores en la EEPROM? S | N");

        while (confirmacion != '&' && confirmacion != '&') {
          confirmacion = BT.read();

          switch (confirmacion) {
            case 's':
              EEPROM.put(0, am11);
              EEPROM.put(2, am12);
              EEPROM.put(4, am21);
              EEPROM.put(6, am22);
              EEPROM.put(8, am31);
              EEPROM.put(10, am32);
              Serial.println("Valores guardados");
              BT.println("Valores guardados");
              delay(2000);
              valoresEeprom();
              goto menu1;
              break;

            case 'n':
              Serial.println("Valores no guardados");
              BT.println("Valores no guardados");
              delay(2000);
              goto menu1;
              break;
          }
        }
        break;
    }
  }
  Serial.println("Saliendo..");
  BT.println("Saliendo..");
  BT.println("M para abrir el menu");
  Serial.println("M para abrir el menu");
}

////////////////////////////////////////////////////////

void asignarValor() {

  Serial.println("Ingrese valor entre 1 a 2000" );
  temp = 0;

  while (temp == 0) {
    str = Serial.readStringUntil('\n');
    temp = str.toInt();
  }

  if (temp > 0 && temp < 2001) {
    Serial.println("Cambio guardado");
    BT.println("Cambio guardado");
    aux = temp;
    delay(2000);
  }
  else {
    Serial.println("Valor incorrecto");
    BT.println("Valor incorrecto");
    temp = 0;
    asignarValor();
  }
}

///////////////////////////////////////////////////////////

void asignarValorBt() {

  BT.println("Ingrese valor entre 1 a 2000" );
  temp = 0;

  while (temp == 0) {
    strBt = BT.readStringUntil('\n');
    temp = strBt.toInt();
  }

  if (temp > 0 && temp < 2001) {
    Serial.println("Cambio guardado");
    BT.println("Cambio guardado");
    aux = temp;
    delay(2000);
  }
  else {
    Serial.println("Valor incorrecto");
    BT.println("Valor incorrecto");
    temp = 0;
    asignarValorBt();
  }
}

/////////////////////////////////////////////////////////////////

void valoresEeprom() {
  EEPROM.get(0, am11);
  EEPROM.get(2, am12);
  EEPROM.get(4, am21);
  EEPROM.get(6, am22);
  EEPROM.get(8, am31);
  EEPROM.get(10, am32);
  return;
}
