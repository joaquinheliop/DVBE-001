#include <SoftwareSerial.h>
SoftwareSerial BT (10,11);

//variable para determinar que boton se pulsó
char val;
int bot1, bot2, bot3; 

//variables para asignar numero a mi funcion accion
int r1, r2;

//auxiliares para evitar accion si el pulsador se mantiene
int control, val1;

//guarda los estados y posicionamiento de los cilindros
int estado[3], n;

//tiempo de amortiguamiento
 int amort1, amort2;
 

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

   //Configuracion salidas
  digitalWrite(2, HIGH);
  digitalWrite(3, HIGH);
  digitalWrite(4, HIGH);
  digitalWrite(5, HIGH);
  digitalWrite(6, HIGH);
  digitalWrite(7, HIGH);
    
   //alimentacion de los relays despues de 0,5[s]
  delay(500);
  digitalWrite(8, HIGH);

   // Iniciar puerto serial
  Serial.begin(9600);
  Serial.println("Bluetooth iniciado");
  BT.begin(9600);
  BT.println("Bluetooth iniciado");

  //configuracion de electrovalvulas en estado inicial neutro
  for (int i= 1; i < 8; i = i+2) {
    digitalWrite(i, LOW);
    delay(200); 
    digitalWrite(i, HIGH);  
 }

}

void loop(){
        
    val = BT.read();
    

  //con los if veo que boton se pulsó
  if (val=='a') {
    r1 = 2;
    r2 = 3;
    n = 0;
    amort1=450;
    amort2=1220;
    accion();
  }
  
  if (val=='b') {
    accionA();
    r1 = 4;
    r2 = 5;
    n = 1;
    amort1=1800;
    amort2=1050;
    accion();    
  }
  
  if (val=='c') { 
    accionB();
    r1 = 6;
    r2 = 7;
    n = 2;
    amort1=1050;
    amort2=1500;
    accion();
  }
 
}

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

      delay(300);
      
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

      delay(200);

      digitalWrite(r2, LOW);
      delay(200); 
      digitalWrite(r2, HIGH);
          break;
  }
  loop();
}
  
void accionA(){
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

      delay(200);
      
      digitalWrite(r2, LOW);
      delay(200); 
      digitalWrite(r2, HIGH);

      estado[n]=1-estado[n];
          break;
    case 0:
          break;
  }
}
  
  void accionB(){
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

      delay(200);
      
      digitalWrite(r2, LOW);
      delay(200); 
      digitalWrite(r2, HIGH);

      estado[n]=1-estado[n];
          break;
    case 0:
          break;
  }
} 
 
