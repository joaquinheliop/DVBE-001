#include <SoftwareSerial.h>
SoftwareSerial BT (0,1);

//variable para determinar que boton se pulsó


void setup(){
    //relays cilindro 1
  pinMode(13,OUTPUT);
    // Iniciar puerto serial
  BT.begin(9600);
  BT.println("Bluetooth iniciado");

}

void loop(){
        
char    val = BT.read();
    digitalWrite(13,LOW);

  if (val=='a') {
   digitalWrite(13, HIGH);
   delay(1000);
   digitalWrite(13,LOW);
   loop();
  }
  
  if (val=='b') {
    BT.println("Respuesta desde mi bluetooth");
    loop();   
  }
} 
 
