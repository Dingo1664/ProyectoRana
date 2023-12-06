#include <LCD.h>
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27, 2, 1, 0, 4, 5, 6, 7);
int C25p = 0;
int C50p = 0;
int C75p = 0;
int C100p = 0;
int C200p = 0;
int Array_Tipo_Casilla[] = { C25p, C50p, C75p, C100p, C200p };

int indiceJugadores = 0;
int x = 0;
int nJugadores = 0;// se selccionan al meter fiches en la rana;
unsigned long PuntuacionJugador[8]{ 0, 0, 0, 0, 0, 0, 0, 0 };
unsigned long TotalJugador[8]{ 0, 0, 0, 0, 0, 0, 0, 0 };
String Jugador[8]{ "Jugador 1", "Jugador 2", "Jugador 3", "Jugador 4", "Jugador 5", "Jugador 6", "Jugador 7", "Jugador 8" };

byte Sensortapa = A0;

void lecturaPines() {
  Serial.print(" PIN 2 ==> ");
  Serial.print(digitalRead(2));
  Serial.print(" | PIN 3 ==> ");
  Serial.print(digitalRead(3));
  Serial.print(" | PIN 4 ==> ");
  Serial.print(digitalRead(4));
  Serial.print(" | PIN 5 ==> ");
  Serial.print(digitalRead(5));
  Serial.print(" | PIN 6 ==> ");
  Serial.print(digitalRead(6));
  Serial.print(" | PIN 7 ==> ");
  Serial.print(digitalRead(7));
  Serial.print("  PIN 8 ==> ");
  Serial.print(digitalRead(8));
  Serial.print(" | PIN 9 ==> ");
  Serial.print(digitalRead(9));
  Serial.print(" | PIN 10 ==> ");
  Serial.println(digitalRead(10));
  delay(200);
}

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
  PuntuacionJugador[indiceJugadores] = (Array_Tipo_Casilla[0] * 25) + (Array_Tipo_Casilla[1] * 50) + (Array_Tipo_Casilla[2] * 75) + (Array_Tipo_Casilla[3] * 100) + (Array_Tipo_Casilla[4] * 200);
  //-------

  Serial.print(Jugador[indiceJugadores]);
  Serial.print(" ==> ");
  Serial.println(PuntuacionJugador[indiceJugadores]);
  lcd.setCursor(0, 0);
  lcd.print(Jugador[indiceJugadores]);
  lcd.setCursor(0, 1);
  lcd.print(PuntuacionJugador[indiceJugadores]);
  lcd.setCursor(5,1);
  lcd.print("|| ");lcd.print(TotalJugador[indiceJugadores]);
}
void LecturasensoresCasillas() {
  const int Retardo_sensor = 300;
  const bool Sensor_25p_ON = digitalRead(8) || digitalRead(10);
  const bool Sensor_50p_ON = digitalRead(6);
  const bool Sensor_75p_ON = digitalRead(5) || digitalRead(7);
  const bool Sensor_100p_ON = digitalRead(2) || digitalRead(3) || digitalRead(4);
  const bool Sensor_200p_ON = digitalRead(9);
  const bool Esta_Activo_Sensor[] = { Sensor_25p_ON, Sensor_50p_ON, Sensor_75p_ON, Sensor_100p_ON, Sensor_200p_ON };  //Almacena el estado de los sensores y si estan activos devuelven true
  const int arrayLength = sizeof(Array_Tipo_Casilla) / sizeof(int);
  int Indice_Array_Tipo_Cas[] = { 0, 1, 2, 3, 4 };

  for (int indice = 0; indice < arrayLength; indice++) {

    if (Esta_Activo_Sensor[indice]) {
      delay(Retardo_sensor);
      Array_Tipo_Casilla[Indice_Array_Tipo_Cas[indice]]++;
    }
  }
}
void seleccionJugadores(){
 while(analogRead(Sensortapa) >= 900){
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Inserta Jugador");
      lcd.setCursor(0, 1);
      lcd.print(nJugadores);lcd.print(" Jugadores");
      if(digitalRead(9)==HIGH){
        delay(200);
      nJugadores++;
      }
      //Serial.print(" N jugadores ");Serial.println(nJugadores);
 } 
 while(analogRead(Sensortapa) < 900){

 }
}

void setup() {
  lcd.setBacklightPin(3, POSITIVE);
  lcd.setBacklight(HIGH);
  lcd.begin(16, 2);
  Serial.begin(115200);
  //Declaracion sensores
  for (int x = 2; x == 10; x++) {
    pinMode(x, INPUT);
  }
  pinMode(Sensortapa, INPUT);
  seleccionJugadores();
}

void loop() {
  Enviar();
  
  if (analogRead(Sensortapa) < 900) {
    
    delay(100);
   
    if (x == 1) {
      TotalJugador[indiceJugadores] =TotalJugador[indiceJugadores] + PuntuacionJugador[indiceJugadores];
      for (int y = 0; y < 5; y++) {Array_Tipo_Casilla[y] = 0;};
      PuntuacionJugador[indiceJugadores] = 0;
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print(Jugador[indiceJugadores]);
      lcd.setCursor(0, 1);
      lcd.print(PuntuacionJugador[indiceJugadores]);
       indiceJugadores++;
    }
    lcd.clear();
 x++;
    if (indiceJugadores >=nJugadores /* 8*/) { indiceJugadores = 0; };
  };
  if (analogRead(Sensortapa) >= 900) {
    x = 0;
    LecturasensoresCasillas();
    Serial.print(" || Total jugador ==> ");
    Serial.println(TotalJugador[indiceJugadores]);
  }
  //MonitoringAll();
  CalculoPuntos();
  //lecturaPines();
}
