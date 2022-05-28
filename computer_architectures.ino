/*AQUAFFICIENT*/
#include <MD_MAX72xx.h>
#include <Stepper.h>
#include <MatrizLed.h>
#define STEPS_PER_REV 513
#define HARDWARE_TYPE MD_MAX72XX::DR0CR0RR1_HW
#define NUM_OF_MATRIX 1
#define CLK_PIN 4
#define DATA_PIN 2
#define CS_PIN 3
#define DELAY_ANIMATION 200
int ledPin = 13,estado_nuevo=0,ledPin_stand_by=A4;
boolean muestra_cartel =false,esta_en_stand_by =false;
MatrizLed pantalla;
const byte pulgar_abajo[] = {B00000000,B00111010,B01111010,B01111010,B01111010,B01111010,B00011000,B00010000};
const byte pulgar_arriba[] = {B00001000,B00001100,B11011110,B11011110,B11011110,B11011110,B11011100,B00000000};
const byte lleno[] = {B00111100,B01100110,B10000001,B11111111,B11111111,B11111111,B11111111,B11111111};
const byte medio[] = {B00111100,B01100110,B10000001,B10000001,B11111111,B11111111,B11111111,B11111111};
const byte vacio[] = {B00111100,B01100110,B10000001,B10000001,B10000001,B10000001,B10000001,B11111111};
const byte bajo[] = {B00111100,B01100110,B10000001,B10000001,B10000001,B10000001,B11111111,B11111111};//bajo
/*crea el objeto de motor */
Stepper motor(STEPS_PER_REV, 5, 6, 7, 8);
const int stepsPerRevolution = 200;
MD_MAX72XX mx = MD_MAX72XX(HARDWARE_TYPE, DATA_PIN, CLK_PIN, CS_PIN, NUM_OF_MATRIX);
int sensorPin_1 = A0,sensorPin_2 = A1,sensorPin_3 = A2,sensor_Stand_by = A3;
int sensorValue_1,sensorValue_2,sensorValue_3,sensor_value_Stand_by = 0;
void setup() {
pantalla.begin(2,4,3,1);
motor.setSpeed(10);
// inicializar el objeto mx
pinMode(ledPin, OUTPUT);
/*ledPin_stand_by*/
pinMode(ledPin_stand_by, OUTPUT);
mx.begin();
// poniendo la matriz en modo de prueba
mx.control(MD_MAX72XX::TEST, true);
delay(2000);
// desactivando el modo de prueba
mx.control(MD_MAX72XX::TEST, false);
// Establecer intencidad a un valor de 5
mx.control(MD_MAX72XX::INTENSITY, 5);
}
/**
* Recibe un arreglo con la configuración de
* las filas y las dibuja en la matriz
**/
void drawRows( const byte fig[] ){
for( int i = 0; i < 8; i++ ){
mx.setRow(0, i, fig[i]);
}
}
void Stand_by_function(){
}
void muestra_estado_nuevo_tinaco(int param1){
if(param1==1){
motor.setSpeed(10);
motor.step(128); //da 4 vueltas
drawRows(lleno);
delay(2*DELAY_ANIMATION);
}else if(param1==2){
motor.setSpeed(10);
motor.step(96); //da 3 vueltas
drawRows(lleno);
delay(2*DELAY_ANIMATION);
}else if(param1==3){
motor.setSpeed(10);
motor.step(64); //da 2 vueltas
drawRows(lleno);
delay(2*DELAY_ANIMATION);
}else if(param1==4){
motor.setSpeed(10);
motor.step(000); //da 0 vueltas
drawRows(lleno);
delay(2*DELAY_ANIMATION);
drawRows(pulgar_arriba);
delay(2*DELAY_ANIMATION);
}
}
void muestra_tinaco(int param1,int param2,int param3){
/*sensorPin_1 sensorPin_2 sensorPin_3*/
/*sensorValue_1 sensorValue_2 sensorValue_3 */
mx.clear();estado_nuevo=0;
sensorValue_1 = analogRead(sensorPin_1);
sensorValue_2 = analogRead(sensorPin_2);
sensorValue_3 = analogRead(sensorPin_3);
/*checa sensores*/
if(sensorValue_1==0 && sensorValue_2 == 0 && sensorValue_3== 0 ){
/* cuando esta vaci muestra tinaco vacio y sube agua*/
mx.clear();
drawRows(vacio);
delay(2*DELAY_ANIMATION);
estado_nuevo=1;
muestra_estado_nuevo_tinaco(estado_nuevo);
}else if(sensorValue_1!= 0 && sensorValue_2== 0 && sensorValue_3== 0 ){
/* cuando el nivel del agua es bajo muestra tinaco bajo y sube agua*/
mx.clear();
drawRows(bajo);
delay(2*DELAY_ANIMATION);
estado_nuevo=2;
muestra_estado_nuevo_tinaco(estado_nuevo);
}else if(sensorValue_1!= 0 &&sensorValue_2!= 0 &&sensorValue_3== 0 ) {
/* cuando el nivel del agua es medio muestra tinaco medio y sube agua*/
mx.clear();
drawRows(medio);
delay(2*DELAY_ANIMATION);
estado_nuevo=3;
muestra_estado_nuevo_tinaco(estado_nuevo);
}else if (sensorValue_1!= 0 &&sensorValue_2!= 0 &&sensorValue_3!= 0 ) {
/* cuando el nivel del agua es alto muestra tinaco lleno y no sube agua*/
mx.clear();
drawRows(lleno);
delay(2*DELAY_ANIMATION);
estado_nuevo=4;
muestra_estado_nuevo_tinaco(estado_nuevo);
}
delay(2*DELAY_ANIMATION);
delay(2*DELAY_ANIMATION);
delay(2*DELAY_ANIMATION);
loop();
}
void misc(){
// limpiar la matriz
//mx.clear();
pantalla.escribirFraseScroll("Aquafficient",50);
// encender los LEDs de las esquinas
}
void loop() { //se repite
sensor_value_Stand_by = analogRead(sensor_Stand_by);
if(muestra_cartel==false){
misc();//limpia matriz y hace delay
muestra_cartel=true;
esta_en_stand_by=false;
}
if(sensor_value_Stand_by==0){
digitalWrite(ledPin_stand_by, LOW);
esta_en_stand_by=false;
muestra_tinaco(sensorPin_1,sensorPin_2,sensorPin_3);
delay(5*DELAY_ANIMATION);
}else if(sensor_value_Stand_by!=0&&esta_en_stand_by==false){/* mostrar que estamos en standby escribiendo en el led matrix y encendiendo un led*/
esta_en_stand_by=true;
pantalla.escribirFraseScroll("Stand by",50);
/*que muestre el foquito*/
digitalWrite(ledPin_stand_by, HIGH);
//sensor_Stand_by=0;
loop();
}else if(sensor_value_Stand_by!=0&&esta_en_stand_by==true){
digitalWrite(ledPin_stand_by, HIGH);
//sensor_Stand_by=0;
loop();
/*solo muestre el foquito*/
}
}
