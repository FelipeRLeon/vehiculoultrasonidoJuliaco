#include <ESP32Servo.h>

Servo myservo;
int servoPin = 13;

const int Trigger = 34;   //Pin digital 2 para el Trigger del sensor
const int Echo = 35;   //Pin digital 3 para el Echo del sensor

const int grados[]={180,90, 0};

const int motorPin1  = 25;  // Pin 14 of L293
const int motorPin2  = 33;  // Pin 10 of L293
//Motor B
const int motorPin3  = 27; // Pin  7 of L293
const int motorPin4  = 26;  // Pin  2 of L293


void setup() {
  Serial.begin(9600);//iniciailzamos la comunicación

  //Motores
    pinMode(motorPin1, OUTPUT);
    pinMode(motorPin2, OUTPUT);
    pinMode(motorPin3, OUTPUT);
    pinMode(motorPin4, OUTPUT);

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
  float distancia=calcularDistancia();
  int direccion;
  if(distancia>10){
    digitalWrite(motorPin1, HIGH);
    digitalWrite(motorPin2, LOW);
    digitalWrite(motorPin3, HIGH);
    digitalWrite(motorPin4, LOW);
  }else{
    digitalWrite(motorPin1, LOW);
    digitalWrite(motorPin2, LOW);
    digitalWrite(motorPin3, LOW);
    digitalWrite(motorPin4, LOW);
    direccion = distanciasLados();
    if(direccion == 0){
      digitalWrite(motorPin1, LOW);
      digitalWrite(motorPin2, LOW);
      digitalWrite(motorPin3, HIGH);
      digitalWrite(motorPin4, LOW);
      delay(700); 
      digitalWrite(motorPin1, LOW);
      digitalWrite(motorPin2, LOW);
      digitalWrite(motorPin3, LOW);
      digitalWrite(motorPin4, LOW);
    }else if(direccion == 1){
      digitalWrite(motorPin1, LOW);
      digitalWrite(motorPin2, LOW);
      digitalWrite(motorPin3, LOW);
      digitalWrite(motorPin4, LOW);
    }else if(direccion == 2){
      digitalWrite(motorPin1, HIGH);
      digitalWrite(motorPin2, LOW);
      digitalWrite(motorPin3, LOW);
      digitalWrite(motorPin4, LOW);
      delay(700); 
      digitalWrite(motorPin1, LOW);
      digitalWrite(motorPin2, LOW);
      digitalWrite(motorPin3, LOW);
      digitalWrite(motorPin4, LOW);
    }
    
  }
  //Serial.print(distanciasLados());
  //Serial.println();
  //delay(1000);
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
  digitalWrite(motorPin1, LOW);
  digitalWrite(motorPin2, HIGH);
  digitalWrite(motorPin3, LOW);
  digitalWrite(motorPin4, HIGH);
  delay(500);
  digitalWrite(motorPin1, LOW);
  digitalWrite(motorPin2, LOW);
  digitalWrite(motorPin3, LOW);
  digitalWrite(motorPin4, LOW);
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
  delay(25);          //Hacemos una pausa de 100ms
  //Serial.print("Distancia: ");
  //Serial.print(distancia);      //Enviamos serialmente el valor de la distancia
  //Serial.print("cm");
  //Serial.println();
  return distancia;
}
