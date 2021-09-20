#include <SoftwareSerial.h>
SoftwareSerial BT (9,8);

//variables para determinar boton que se pulsó
  char val;
  int bot1, bot2, bot3; 

//variables para asignar numero a mi funcion accion, determinan que rele se activa
  int r1, r2;

//auxiliares para evitar accion si el pulsador se mantiene
  int control, val1;

//guarda los estados y posicionamiento de los cilindros
  int estado[3] = {0,1,1}, n;

//auxiliares para configurar los amortiguamientos individuales
  int amort1, amort2;
//amortiguamientos individuales en orden
  int am11= 450 ,
      am12= 1220 ,
      am21= 1800 ,
      am22= 1050 ,
      am31= 1050 ,
      am32= 1500 ;
//tiempo de amortiguamiento
  int amortT = 300 ;
 
void setup(){
    //relays cilindro 1
  pinMode(2,OUTPUT);
  pinMode(3,OUTPUT);
    //relays cilindro 2
  pinMode(4,OUTPUT);
  pinMode(5,OUTPUT);
    //relays cilindro 3
  pinMode(6,OUTPUT);
  pinMode(7,OUTPUT);
    //alimentacion
  pinMode(8,OUTPUT);

   //configuracion salidas
  digitalWrite(2, HIGH);
  digitalWrite(3, HIGH);
  digitalWrite(4, HIGH);
  digitalWrite(5, HIGH);
  digitalWrite(6, HIGH);
  digitalWrite(7, HIGH);
    
   //alimentacion de los relays despues de 0,5[s]
  delay(500);
  digitalWrite(8, HIGH);

   //iniciar puerto serial
  Serial.begin(9600); 
  BT.begin(9600);
  BT.println("Bluetooth iniciado");

  //configuracion de electrovalvulas en estado inicial neutro
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

  accionI();
  accionD();
  int estado[3] = {0 , 0 , 0};
}

void loop(){
        
  val = BT.read();
    
//Puerta Central
  if (val=='a') {
    BT.println("Puerta 1");
    r1 = 2;
    r2 = 3;
    n = 0;
    amort1 = am11;
    amort2 = am12;
    accion();
  }

//Puerta Lateral Derecha
  if (val=='b') {
    BT.println("Puerta 2");
    accionI();
    r1 = 4;
    r2 = 5;
    n = 1;
    amort1 = am21;
    amort2 = am22;
    accion();    
  }

//Puerta Lateral Izquierda  
  if (val=='c') {
     BT.println("Puerta 3");
    accionD();
    r1 = 6;
    r2 = 7;
    n = 2;
    amort1 = am31;
    amort2 = am32;
    accion();
  }
 
}

//Acciona la puierta segun su estado anterior
void accion(){
  
  estado[n]=1-estado[n];//guarda el estado anterior
  
  delay(500);
  
    switch (estado[n]){
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
  loop();
}

//Puesta en neutro puerta Izquierda
void accionI(){
  amort2 = am22;
  r1 = 6;
  r2 = 7;
  n = 2;
  
  delay(500);
  
    switch (estado[n]){
    case 1:
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

      estado[n]=1-estado[n];
          break;
    case 0:
          break;
  }
}

//Puesta en neutro puesta Derecha
  void accionD(){
  amort2 = am32; 
  r1 = 4;
  r2 = 5;
  n = 1;
  
  delay(500);
  
    switch (estado[n]){
    case 1:
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

      estado[n]=1-estado[n];
          break;
    case 0:
          break;
  }
} 
 
