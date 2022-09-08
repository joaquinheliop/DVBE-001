/*En esta version la balanza puede recibir configuraciones
mediante un mensaje con el siguiente formato: Ax0000.00 (A x 0000.00)
El caracter A indica autoconfig, el x indica la opcion (a, b, c)
y el 0000.00 es el valor de config. Si la config esta correcta, se envia
'OK' si no 'ER'. Además, si envio G o R en el loop puedo guardar en eeprom o resetear valores
por defecto*/

#include <HX711.h>
#include <SoftwareSerial.h>
#include <EEPROM.h>

//HX711
#define pinDt A0
#define pinSck A1
HX711 balanza;

//Bluetooth
#define pinTx 3
#define pinRx 4
#define pinState 5
SoftwareSerial BT(pinTx, pinRx);

//Indicadores
#define pinLed 2

//Valores por defecto
#define nDefault 1.0
#define escDefault 1000.0
//#define corDefault 1.0
#define pesoMinDefault 50
#define tiempo 1

//Valores por defecto correccion de peso
#define PESOMIN 1
#define PESOMAX 5000

//Valores por defecto peso minimo estable
#define PESOESTMIN 5
#define PESOESTMAX 500

//Valores por defecto numero de muestras
#define NMIN 20
#define NMAX 150

//Parametros de configuracion
int muestras;
float escala;
int n=100;
int pesoMin;

//Auxiliares
float peso, pesoAnt, pesoSig, pesoEst;
byte condicion;

void setup()
{

  //Inicio de puertos
  //Serial.begin(9600);
  Serial.begin(9600);
  balanza.begin(pinDt, pinSck);

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
  if (eepr == -1)
  {
    EEPROM.put(0, nDefault);      //muestras
    EEPROM.put(4, escDefault);      //escala
    //EEPROM.put(8, corDefault);      //correccion
    EEPROM.put(12, pesoMinDefault); //peso minimo estable
    EEPROM.put(14, 1);              //estado eeprom
  }

  //Obtener valores guardados en la EEPROM
  EEPROM.get(0, muestras);
  EEPROM.get(4, escala);
  EEPROM.get(8, n);
  EEPROM.get(12, pesoMin);


  //Configuraciones iniciales de escala y tara
  balanza.set_scale(escala);
  delay(100);
  balanza.tare(10);
}

void loop() 
{
  //Recoleccion de datos EXCEL
  /*Serial.print("DATA,TIME,TIMER,");
    Serial.print(peso);
    Serial.print(",");
    Serial.print(peso - pesoAnt);
    Serial.print(",");
    Serial.print("0");
    Serial.println("");*/

  //Pulsar led en el caso de estar buscando conexion BT (Solo para HM-10,HC-05)
  busquedaConexion();

  //Devuelve el peso por BT y verifica el algoritmo de estabilizacion
  pesar();

  //Lee datos del puerto BT
  verificarBt();
}

void pesar()
{  
  peso = balanza.get_units(1);
  //Imprime datos en el puerto BT
  Serial.println("P" + (String)round(peso));
  delay(tiempo);
  estable();
}

void estable()
{
  /*Una vez superado el peso min estable se va a disparar el muestreo
  todos los valores se guardan dentro de un vector con x tamaño mientras mas grande
  el vector mas demora en estabilizar pero mayor es el n del muestreo y mayo presicion*/

  //Configuracion inicial de muestreo
  float muestreo[muestras];
  float suma= 0;
  if (abs(peso) > pesoMin){
    for (int i=0; i<muestras; i++){
        peso = balanza.get_units(1);
    
        //Imprime datos en puerto BT
        //Serial.print("Bloque estable");
        Serial.println("P" + (String)round(peso));
        
        //delay(tiempo);

      //Asigno muestra al array de muestreo
        muestreo[i]=peso;

        if (abs(peso) < pesoMin){
          break;
      }  
    }
    //Calculo de promedio del array
    for (int i=(0.2*muestras); i<muestras; i++){
      suma= suma+muestreo[i];
    }

    while (abs(peso) > pesoMin){
    //Serial.println("Peso real: " + (String)(peso));
    float promedio= suma/(muestras*(1-0.2));
    Serial.println("E" + (String)round(promedio));
    delay(tiempo);
    peso = balanza.get_units(1);
    verificarBtEstable();

    /*Serial.print("DATA,TIME,TIMER,");
    Serial.print("0");
    Serial.print(",");
    Serial.print(peso - pesoAnt);
    Serial.print(",");
    Serial.print(peso);
    Serial.println("");*/
    }
  }
}

void repesado()
{
  /*funcion creeada para que cuando recibe varias taras en la reestabilizacion 
  del peso no me haga un a tara a cero se logro quitando la capacidad de lectura
  mientras hace el repesaje*/

  Serial.read(); //Limpiar Buffer

  pesoAnt = balanza.get_units(2);
  peso = balanza.get_units(2);
  pesoSig = balanza.get_units(2);

  //Imprime datos en el puerto BT
  Serial.println("P" + (String)round(pesoSig));
  delay(tiempo);

  while (Serial.available())
  {
    Serial.read();
  }
}

void verificarBt()
{
  if (Serial.available())
  {
    String lecturaString = Serial.readStringUntil('\n');
    char lectura = lecturaString.charAt(0);

    switch (lectura)
    {
    case 'A':
      autoConfig(lecturaString);
      break;

    case 'G':
      eepromGuardar(true);
      break;

    case 'R':
      eepromReset(true);
      break;

    case 'T':
      balanza.tare(10);
      break;

    case 'm':
      menu();
      break;
    }
  }
}

void verificarBtEstable()
{
  if (Serial.available())
  {
    String lecturaString = Serial.readStringUntil('\n');
    char lectura = lecturaString.charAt(0);

    switch (lectura)
    {
    case 'A':
      autoConfig(lecturaString);
      break;

    case 'G':
      eepromGuardar(true);
      break;

    case 'R':
      eepromReset(true);
      break;

    case 'T':
      pesar();
      break;

    case 'm':
      menu();
      break;
    }
  }
}

void menu()
{
  /*Esta funcion muestra el meno para hacer configuraciones por algun
  monitor serial Bluetooth. La opcion false que aparece como parametro de algunos
  llamados a funciones es para indicarle que no esta ejecutando la funcion en modo 
  autoconfig*/

  char dato;

  Serial.println("MENU\n\na. Establecer escala \nb. Peso minimo estable \nc. Muestras \ng. Guardar valores en EEPROM  \nr. Reestablecer valores por defecto \ns. Salir");

  Serial.read(); //Limpiar Buffer
  dato = Serial.read();

  while (!Serial.available())
  {
  }

  dato = Serial.read();

  switch (dato)
  {

  case 'a':
    Serial.println("Escala actual: " + (String)escala);
    Serial.println("Ingrese el peso conocido en Kg 's' para salir");
    correccionPesoF(0, false);
    break;

  case 'b':
    Serial.println("Peso minimo estable actual: " + (String)pesoMin);
    Serial.println("Ingrese un valor de " + String(PESOESTMIN) + " a " + String(PESOESTMAX) + " o 's' para salir");
    pesoMinF(0, false);
    break;

  case 'c':
    Serial.println("Muestras " + (String)muestras);
    Serial.println("Ingrese un valor de " + String(NMIN) + " a " + String(NMAX) + " o 's' para salir");
    muestrasF(0, false);
    break;

  case 'g':
    eepromGuardar(false);
    break;

  case 'r':
    eepromReset(false);
    break;

  case 's':
    Serial.println("Saliendo...\nListo!");
    loop();
    break;

  default:
    Serial.println("Comando incorrecto");
    menu();
    break;
  }
}

void correccionPesoF(float valor, bool autoconfig)
{

  String str;
  char strAux;
  float correccionAux = 0;
  float lecturaAux;

  Serial.read(); //limpiar buffer
  Serial.read();
  Serial.read();

  while (!Serial.available() && !autoconfig)
  {
  }

  if (autoconfig)
  {
    correccionAux = valor;
  }
  else
  {
    str = Serial.readStringUntil('\n');
    strAux = str.charAt(0);
    correccionAux = str.toFloat();
  }

  if (strAux == 's')
  {
    menu();
  }
  else if (correccionAux >= PESOMIN && correccionAux <= PESOMAX)
  {
    n = correccionAux;

    balanza.set_scale(1);
    delay(300);
    lecturaAux = balanza.get_units(10);

    float nuevaEscala = lecturaAux / n;

    balanza.set_scale(nuevaEscala);
    escala = nuevaEscala;

    if (autoconfig)
    {
      Serial.println("OK");
      loop();
    }
    else
    {
      Serial.println("Nueva escala establecida: " + (String)nuevaEscala + "\n");
      delay(1500);
      menu();
    }
  }
  else
  {
    if (autoconfig)
    {
      Serial.println("ER");
      loop();
    }
    else
    {
      Serial.println("Valor incorrecto\nIngrese un valor de " + String(PESOMIN) + " a " + String(PESOMAX));
      correccionPesoF(0, false);
    }
  }
}

void pesoMinF(float valor, bool autoconfig)
{

  String str;
  char strAux;
  float pesoMinAux = 0;

  Serial.read(); //limpiar buffer
  Serial.read();
  Serial.read();

  while (!Serial.available() && digitalRead(pinState) == 1 && !autoconfig)
  {
  }

  if (autoconfig)
  {
    pesoMinAux = valor;
  }
  else
  {
    str = Serial.readStringUntil('\n');
    strAux = str.charAt(0);
    pesoMinAux = str.toInt();
  }

  if (strAux == 'S' || strAux == 's')
  {
    menu();
  }
  else if (pesoMinAux >= 10 && pesoMinAux <= 500)
  {
    pesoMin = pesoMinAux;

    if (autoconfig)
    {
      Serial.println("OK");
      loop();
    }
    else
    {
      Serial.println("Cambio correcto\nPeso minimo estable: " + (String)pesoMin + "\n");
      menu();
    }
  }
  else
  {
    if (autoconfig)
    {
      Serial.println("ER");
      loop();
    }
    else
    {
      Serial.println("Valor incorrecto\nIngrese un valor de " + String(PESOESTMIN) + " a " + String(PESOESTMAX));
      pesoMinF(0, false);
    }
  }
}

void muestrasF(int valor, bool autoconfig)
{

  String str;
  char strAux;
  int muestrasAux = 0;

  Serial.read(); //limpiar buffer
  Serial.read();
  Serial.read();

  while (!Serial.available() && !autoconfig)
  {
  }

  if (autoconfig)
  {
    muestrasAux = valor;
  }
  else
  {

    str = Serial.readStringUntil('\n');
    strAux = str.charAt(0);
    muestrasAux = str.toFloat();
  }

  if (strAux == 'S' || strAux == 's')
  {
    menu();
  }
  else if (muestrasAux >= NMIN && muestrasAux <= NMAX)
  {
    muestras = muestrasAux;

    if (autoconfig)
    {
      Serial.println("OK");
      loop();
    }
    else
    {

      Serial.println("Cambio correcto\nmuestras: " + (String)muestras + "\n");
      menu();
    }
  }
  else
  {

    if (autoconfig)
    {
      Serial.println("ER");
      loop();
    }
    else
    {
      Serial.println("Valor incorrecto\nIngrese un valor de " + String(NMIN) + " a " + String(NMAX));
      muestrasF(0, false);
    }
  }
}

void eepromGuardar(bool autoconfig)
{

  char confirmacion;

  if (!autoconfig)
  {
    Serial.println("¿Desea guardar los valores establecidos s|n?");

    Serial.read(); //limpiar buffer
    delay(2000);
    Serial.read();

    while (!Serial.available())
    {
    }

    confirmacion = Serial.read();

    switch (confirmacion)
    {
    case 's':
      EEPROM.put(0, muestras);
      EEPROM.put(4, escala);
      EEPROM.put(8, n);
      EEPROM.put(12, pesoMin);

      Serial.println("Valores guardados");
      delay(1000);
      break;

    case 'n':
      Serial.println("Valores no guardados");
      menu();
      break;

    default:
      Serial.println("Comando incorrecto");
      eepromReset(false);
      break;
    }
  }
  else
  {
    EEPROM.put(0, muestras);
    EEPROM.put(4, escala);
    EEPROM.put(8, n);
    EEPROM.put(12, pesoMin);

    Serial.println("OK");
    delay(1000);
    loop();
  }
}

void eepromReset(bool autoconfig)
{

  char confirmacion;

  if (!autoconfig)
  {

    Serial.println("¿Desea reestablecer valores por defecto s|n?");

    Serial.read(); //limpiar buffer
    delay(2000);
    Serial.read();

    while (!Serial.available())
    {
    }

    confirmacion = Serial.read();

    switch (confirmacion)
    {
    case 's':
      EEPROM.put(0, nDefault);      //muestras
      EEPROM.put(4, escDefault);      //escala
//      EEPROM.put(8, corDefault);      //correccion
      EEPROM.put(12, pesoMinDefault); //peso minimo estable

      EEPROM.get(0, muestras);
      EEPROM.get(4, escala);
      EEPROM.get(8, n);
      EEPROM.get(12, pesoMin);

      Serial.println("Valores reestablecidos");
      delay(1000);
      menu();
      break;

    case 'n':
      Serial.println("Valores no reestablecidos");
      menu();
      break;

    default:
      Serial.println("Comando incorrecto");
      eepromReset(false);
      break;
    }
  }
  else
  {
    EEPROM.put(0, nDefault);      //muestras
    EEPROM.put(4, escDefault);      //escala
  //  EEPROM.put(8, corDefault);      //correccion
    EEPROM.put(12, pesoMinDefault); //peso minimo estable

    EEPROM.get(0, muestras);
    EEPROM.get(4, escala);
    EEPROM.get(8, n);
    EEPROM.get(12, pesoMin);

    Serial.println("OK");
    delay(1000);
    loop();
  }
}

void autoConfig(String lecturaString)
{
  /*Esta funcion toma como parametro el string leido por bt Aa0000,00
    lee el segundo valor de la cadena para saber a que config tiene que ir
    y lee 7 digitos para tener el valor de config. Cuando tiene esos dos datos
    llama a la funcion que corresponde enviandole el valor de config por parametro
    y el valor true para decirle que la autoconfig esta activada*/

  float valor = 0;

  String lectura = lecturaString;
  char opcion = lectura.charAt(1);
  String valorStr = lectura.substring(2, 9);

  switch (opcion)
  {
  case 'a':
    valor = valorStr.toFloat();
    correccionPesoF(valor, true);
    break;

  case 'b':
    valor = valorStr.toFloat();
    pesoMinF(valor, true);
    break;

  case 'c':
    valor = valorStr.toInt();
    muestrasF(valor, true);
    break;
  }
}

void busquedaConexion()
{
  while (digitalRead(pinState) == 0)
  {
    condicion = 1;
    digitalWrite(pinLed, LOW);
    delay(1000);
    digitalWrite(pinLed, HIGH);
    delay(1000);
  }
  digitalWrite(pinLed, LOW);
}
