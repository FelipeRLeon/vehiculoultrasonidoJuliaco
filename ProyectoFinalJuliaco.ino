#include <ESP32Servo.h>

Servo myservo;
int servoPin = 26;

const int Trigger = 34;   //Pin digital 2 para el Trigger del sensor
const int Echo = 35;   //Pin digital 3 para el Echo del sensor

const int grados[]={180,90, 0};


void setup() {
  Serial.begin(9600);//iniciailzamos la comunicación

  //Ultrasonido
    pinMode(Trigger, OUTPUT); //pin como salida
    pinMode(Echo, INPUT);  //pin como entrada
    digitalWrite(Trigger, LOW);//Inicializamos el pin con 0

    //Servomotor
    ESP32PWM::allocateTimer(0);
    ESP32PWM::allocateTimer(1);
    ESP32PWM::allocateTimer(2);
    ESP32PWM::allocateTimer(3);

    myservo.setPeriodHertz(50);    // standard 50 hz servo
    myservo.attach(servoPin, 500, 2500);// attaches the servo on pin 18 to the servo object

    Serial.print("Inicio ");
}

void loop() {
  Serial.print(distanciasLados());
  Serial.println();
  delay(1000);
  // put your main code here, to run repeatedly:

}

int distanciasLados(){
  float lados[3];
  Serial.print("medida");
  Serial.println();
  float mayor = 0;
  int mayorindex = 0;
  for(int i = 0; i<3; i++){
    myservo.write(grados[i]);
    delay(400);
    lados[i] = calcularDistancia();
    if(lados[i] > mayor){
      mayor = lados[i];
      mayorindex = i;
    }
  }

  for(int i = 0; i<3; i++){
     Serial.print(lados[i]);
    Serial.print("cm");
    Serial.println();
  }
  myservo.write(90);
  delay(1000);
  return mayorindex;
}


float calcularDistancia(){
  float  t; //timepo que demora en llegar el eco
  float distancia; //distancia en centimetros

  digitalWrite(Trigger, HIGH);
  delayMicroseconds(10);          //Enviamos un pulso de 10us
  digitalWrite(Trigger, LOW);
  
  t = pulseIn(Echo, HIGH); //obtenemos el ancho del pulso
  distancia = t*0.01715;             //escalamos el tiempo a una distancia en cm
  delay(1000);          //Hacemos una pausa de 100ms
  //Serial.print("Distancia: ");
  //Serial.print(distancia);      //Enviamos serialmente el valor de la distancia
  //Serial.print("cm");
  //Serial.println();
  return distancia;
}
