#include <SoftwareSerial.h>    //para mudar a porta serial para receber
#include <math.h>
#include <stdio.h>
SoftwareSerial mySerial(7, 6);   // RX do bluetooth


#include <Servo.h>

#define SERVO 5 // Porta Digital 6 PWM

Servo s; // Variável Servo
int dir = 1;
int pos = 0; // Posição Servo



char Temp_String[50];
int cmd = 0;
unsigned long lastCheck = 0;
long int timestamp = 0;

boolean Get_Comando() {
  String veet;
  char caracter[] = "c";
  memset(Temp_String, 0, sizeof(Temp_String));
  if (mySerial.available()) {
    while (1) {
      if (mySerial.available()) {
        caracter[0] = mySerial.read();
        veet.concat(caracter);
        lastCheck = millis();
      }
      if (millis() - lastCheck > 5000) {
        Serial.println(Temp_String);
        memset(Temp_String, 0, sizeof(Temp_String));
        Serial.println("ERRO");
        break;
      }
      if (caracter[0] == '\n') {
        veet.toCharArray(Temp_String, veet.length() + 1);
        break;
      }
    }
  } else memset(Temp_String, 0, sizeof(Temp_String));

  return veet.length() > 1;
}

void desliga() {

  noTone(11);

}

void ruido() {
  float z1, z2, r1, r2, gauss;

  r1 = (rand () % 100);
  r2 = (rand () % 100);

  z1 = cos(2 * M_PI * r1 / 100);
  z2 = sqrt(-2 * log(r2 / 100));

  gauss = z1 * z2;


  tone(11, gauss * 10000 + 10000);

}

void start() {

  for (int i = 0; i < 10; i++) {
    ruido();
  }
  if ((pos == 0 && dir == -1) || (pos == 90 && dir == 1)) {
    dir *= -1;
  }
  pos += dir;
  s.write(pos);
  Serial.println("start");
}

void balanca() {

  delay (8);
  if ((pos == 0 && dir == -1) || (pos == 90 && dir == 1)) {
    dir *= -1;
  }
  pos += dir;
  s.write(pos);
  Serial.println("balanca");
}

void setup() {
  s.attach(SERVO);
  s.write(0); // Inicia motor posição zero

  Serial.begin(9600);
  mySerial.begin(9600);
}

void loop() {


  if (Get_Comando()) {
    Serial.println(Temp_String);
    if (!strcmp(Temp_String, "dorme\n")) {
      cmd = 1;
      timestamp = millis();
    }
    else if (!strcmp(Temp_String, "desliga\n")) {
      cmd = 2;
    }
    else if (!strcmp(Temp_String, "balanca\n")) {
      cmd = 3;
      timestamp = millis();
    }
    else if (!strcmp(Temp_String, "ruido\n")) {
      cmd = 4;
      timestamp = millis();

    }
  }

  switch (cmd) {
    case 1 : start();
      break;

    case 2 : desliga();
      break;

    case 3 : balanca();
      break;

    case 4 : ruido();
      break;

    default : Serial.println("sem comando");
  }

  if (millis() - timestamp > 10000) {
    cmd = 2;
  }

}
