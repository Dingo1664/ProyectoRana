#include <LCD.h>
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27, 2, 1, 0, 4, 5, 6, 7);
int C25p = 0;
int C50p = 0;
int C75p = 0;
int C100p = 0;
int C200p = 0;
int Array_Tipo_Casilla[] = { C25p, C50p, C75p, C100p, C200p };

unsigned long TotalP1 = 0;
unsigned long TotalP2 = 0;
unsigned long TotalP3 = 0;
byte Sensortapa = A0;

void lecturaPines(){Serial.print(" PIN 2 ==> ");Serial.print(digitalRead(2));Serial.print(" | PIN 3 ==> ");Serial.print(digitalRead(3));Serial.print(" | PIN 4 ==> ");Serial.print(digitalRead(4));
Serial.print(" | PIN 5 ==> ");Serial.print(digitalRead(5));Serial.print(" | PIN 6 ==> ");Serial.print(digitalRead(6));Serial.print(" | PIN 7 ==> ");Serial.print(digitalRead(7));
Serial.print("  PIN 8 ==> ");Serial.print(digitalRead(8));Serial.print(" | PIN 9 ==> ");Serial.print(digitalRead(9));Serial.print(" | PIN 10 ==> ");Serial.println(digitalRead(10));delay(200);}

void Enviar() {
  int reset = 'r';
  if (Serial.available() > 0) {
    char Tecla = Serial.read();
    if (Tecla == reset) {
      Serial.println("reset");
      delay(200);
      asm volatile("jmp 0x00");
    }
    if (Tecla == 'a') { Array_Tipo_Casilla[0]++; }
    if (Tecla == 'b') { Array_Tipo_Casilla[1]++; }
    if (Tecla == 'c') { Array_Tipo_Casilla[2]++; }
    if (Tecla == 'd') { Array_Tipo_Casilla[3]++; }
    if (Tecla == 'e') { Array_Tipo_Casilla[4]++; }
  }
}

void MonitoringAll() {
  Serial.print("25p==>");
  Serial.print(Array_Tipo_Casilla[0]);
  Serial.print(" | 50p==> ");
  Serial.print(Array_Tipo_Casilla[1]);
  Serial.print(" | 75p==> ");
  Serial.print(Array_Tipo_Casilla[2]);
  Serial.print(" | 100p==> ");
  Serial.print(Array_Tipo_Casilla[3]);
  Serial.print(" | 200p==> ");
  Serial.print(Array_Tipo_Casilla[4]);
}

void CalculoPuntos() {
  TotalP1 = (Array_Tipo_Casilla[0] * 25) + (Array_Tipo_Casilla[1] * 50) + (Array_Tipo_Casilla[2] * 75) + (Array_Tipo_Casilla[3] * 100) + (Array_Tipo_Casilla[4] * 200);
  Serial.print(" || TOTAL ==> ");
  Serial.println(TotalP1);
  lcd.setCursor(0, 0);
  lcd.print("TotalP1");
  lcd.setCursor(0, 1);
  lcd.print(TotalP1);
}
void LecturasensoresCasillas() {
  const int Retardo_sensor = 300;
  const bool Sensor_25p_ON = digitalRead(8) || digitalRead(10) ;
  const bool Sensor_50p_ON = digitalRead(6);
  const bool Sensor_75p_ON = digitalRead(5) || digitalRead(7);
  const bool Sensor_100p_ON = digitalRead(2)|| digitalRead(3) || digitalRead(4);
  const bool Sensor_200p_ON = digitalRead(9);
  const bool Esta_Activo_Sensor[] = { Sensor_25p_ON, Sensor_50p_ON, Sensor_75p_ON, Sensor_100p_ON, Sensor_200p_ON };  //Almacena el estado de los sensores y si estan activos devuelven true
  const int arrayLength = sizeof(Array_Tipo_Casilla) / sizeof(int);
  int Indice_Array_Tipo_Cas[] = { 0, 1, 2, 3, 4 };
  if(analogRead(Sensortap)>=900){
  for (int indice = 0; indice < arrayLength; indice++) {
    
    if (Esta_Activo_Sensor[indice]) {
      delay(Retardo_sensor);
      Array_Tipo_Casilla[Indice_Array_Tipo_Cas[indice]]++;
    }
  }
}  
}

void setup() {
  lcd.setBacklightPin(3,POSITIVE);
  lcd.setBacklight(HIGH);
  lcd.begin (16,2); 
  Serial.begin(11500);
  //Declaracion sensores
  for (int x = 2; x == 10; x++) {
    pinMode(x, INPUT);
  }
  pinMode(Sensortapa, INPUT);
}

void loop() {
  Enviar();
  LecturasensoresCasillas();
  //MonitoringAll();
  CalculoPuntos();
  //lecturaPines();
}
