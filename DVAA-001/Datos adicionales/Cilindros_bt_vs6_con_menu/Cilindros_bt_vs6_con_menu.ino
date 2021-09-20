#include <SoftwareSerial.h>
SoftwareSerial BT (10,11);

//variables menu
  String str;
  int temp=0,
      aux=0;

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
 /* delay(500);
  digitalWrite(8, HIGH);*/

   //iniciar puerto serial y bt
  Serial.begin(9600);
  Serial.println("Bluetooth iniciado");
  Serial.println("M para abrir el menu");
  BT.begin(9600);
  BT.println("Bluetooth iniciado");
  BT.println("M para abrir el menu");
  
  //configuracion de electrovalvulas en estado inicial neutro
  /*digitalWrite(2, LOW);
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
  accionD();*/
  int estado[3] = {0 , 0 , 0};
}
//////////////////////////////
void loop(){
        
  val = Serial.read();
  val = BT.read();
    
//Puerta Central
  if (val=='a') {
    r1 = 2;
    r2 = 3;
    n = 0;
    amort1 = am11;
    amort2 = am12;
    accion();
  }

//Puerta Lateral Derecha
  if (val=='b') {
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
    accionD();
    r1 = 6;
    r2 = 7;
    n = 2;
    amort1 = am31;
    amort2 = am32;
    accion();
  }
  
//Menu
    if (val=='m') {
    menu();
    }
} 
//////////////////////////////////////////////
  
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
///////////////////////////////////////

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
////////////////////////////////////////////

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
////////////////////////////////////////////////////////////////////
    
 void menu(){
      Serial.println("Menu");
      BT.println("Menu");
      Serial.print("a.Temp. Amortiguacion puerta central 1: ");
      Serial.println(am11);
      BT.print("a.Temp. Amortiguacion puerta central 1: ");
      BT.println(am11);
      Serial.print("b.Temp. Amortiguacion puerta central 2: "); 
      Serial.println(am22);
      BT.print("b.Temp. Amortiguacion puerta central 2: "); 
      BT.println(am22);
      Serial.print("c.Temp. Amortiguacion puerta L.Izq 1: "); 
      Serial.println(am21);
      BT.print("c.Temp. Amortiguacion puerta L.Izq 1: "); 
      BT.println(am21);
      Serial.print("d.Temp. Amortiguacion puerta L.Izq 2: "); 
      Serial.println(am22);
      BT.print("d.Temp. Amortiguacion puerta L.Izq 2: "); 
      BT.println(am22);
      Serial.print("e.Temp. Amortiguacion puerta L.Der 1: "); 
      Serial.println(am32);
      BT.print("e.Temp. Amortiguacion puerta L.Der 1: "); 
      BT.println(am32);
      Serial.print("f.Temp. Amortiguacion puerta L.Der 2: "); 
      Serial.println(am32);
      BT.print("f.Temp. Amortiguacion puerta L.Der 2: "); 
      BT.println(am32);
      Serial.println("S.Salir");
      BT.println("S.Salir"); 
   
      while (val !='s' && val!='S' ){
        val = Serial.read();
        val = BT.read();
        
        switch (val){
        case 'a':
          Serial.print("Temp. Amortiguacion puerta central 1: ");
          Serial.println(am11);
          BT.print("Temp. Amortiguacion puerta central 1: ");
          BT.println(am11);
          asignarValor();
          am11 = aux;
          menu();
          break;
        
        case 'b':
          Serial.print("Temp. Amortiguacion puerta central 2: ");
          Serial.println(am12);
          BT.print("Temp. Amortiguacion puerta central 2: ");
          BT.println(am12);
          asignarValor();
          am12 = aux;
          menu();
          break;
     
        case 'c':
          Serial.print("Temp. Amortiguacion puerta L.Izq 1: ");
          Serial.println(am21);
          BT.print("Temp. Amortiguacion puerta L.Izq 1: ");
          BT.println(am21);
          asignarValor();
          am21 = aux;
          menu();
          break;
      
       case 'd':
          Serial.print("Temp. Amortiguacion puerta L.Izq 2: ");
          Serial.println(am22);
          BT.print("Temp. Amortiguacion puerta L.Izq 2: ");
          BT.println(am22);
          asignarValor();
          am22 = aux;
          menu();
          break;
      
       case 'e':
          Serial.print("Temp. Amortiguacion puerta L.Der 1: ");
          Serial.println(am31);
          BT.print("Temp. Amortiguacion puerta L.Der 1: ");
          BT.println(am31);
          asignarValor();
          am31 = aux;
          menu();
          break;
      
        case 'f':
          Serial.print("Temp. Amortiguacion puerta L.Der 2: ");
          Serial.println(am32);
          BT.print("Temp. Amortiguacion puerta L.Der 2: ");
          BT.println(am32);
          asignarValor();
          am32 = aux;
          menu();
          break;
       }
    }
     Serial.println("Saliendo..");
     BT.println("Saliendo..");
     Serial.println("M para abrir el menu");
     BT.println("M para abrir el menu");
     loop ();
  }
////////////////////////////////////////////////////////////

  void asignarValor(){
        
        Serial.println("Ingrese valor entre 1 a 2000" );
        BT.println("Ingrese valor entre 1 a 2000" ); 
        temp = 0;
                       
        while (temp==0) {
          str = Serial.readStringUntil('\n');
          str = BT.readStringUntil('\n');
          temp = str.toInt();   
        }
          
        if (temp >0 && temp < 2001){
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
