#include <AsyncTaskLib.h>
#include <TM1637.h>

uint8_t btnInicioParar = 4;
uint8_t btnAumenta = 3;
uint8_t btnDisminuye = 2;

uint8_t dioTM = 9;
uint8_t clkTM = 8;
uint8_t inReg = 7;
uint8_t clkReg = 6;
uint8_t resetReg = 5;

// uint8_t led1 = A0;
// uint8_t led2 = A2;
// uint8_t led3 = A4;
// uint8_t led4 = A6;
// int potenciometro = A1;
int velocidad = 500;
// uint8_t clkReg = 6;
// uint8_t resetReg = 5;

// Variables
uint8_t estado = 1;
uint8_t espera = 200;
uint8_t atrInicio = 0;
uint8_t atrParar = 0;

TM1637 tm(clkTM, dioTM);

uint8_t tick = 0;
uint8_t limite = 5;
uint8_t nivelActivo = 1;

bool flag = false;
bool estaActiva = true;
bool reiniciado = false;

void displayNumber(int num);
void inicializarReloj();

AsyncTask relojVelocidad(velocidad, true, []() {
  flag = !flag;
  if (flag) {
    digitalWrite(clkReg, 1);
    tick++;
    displayNumber(tick - 1);
  } else digitalWrite(clkReg, 0);

  if (tick == 2) digitalWrite(inReg, 0);
  if ((tick - 1) == limite) {
    // digitalWrite(led1, 1);
    inicializarReloj();
    estado = 1;
  }
  Serial.println(velocidad);
});


void setup() {
  pinMode(btnInicioParar, INPUT);
  pinMode(btnAumenta, INPUT);
  pinMode(btnDisminuye, INPUT);

  pinMode(inReg, OUTPUT);
  pinMode(clkReg, OUTPUT);
  pinMode(resetReg, OUTPUT);

  // pinMode(led1, OUTPUT);
  // pinMode(led2, OUTPUT);
  // pinMode(led3, OUTPUT);
  // pinMode(led4, OUTPUT);

  digitalWrite(inReg, 0);
  digitalWrite(clkReg, 1);
  digitalWrite(resetReg, 1);

  // digitalWrite(led1, 0);
  // digitalWrite(led2, 0);
  // digitalWrite(led3, 0);
  // digitalWrite(led4, 0);

  tm.init();
  tm.set(3);

  Serial.begin(9600);
}

void loop() {
  switch (estado) {
    case 1:
      if (digitalRead(btnAumenta) == nivelActivo) {
        delay(espera);
        limite++;
        if (limite > 64) limite = 0;

        if ((tick - 1) > limite) tick = 1;
      }

      if (digitalRead(btnDisminuye) == nivelActivo) {
        delay(espera);
        limite--;
        if (limite < 0) limite = 64;
      }

      displayLimite(limite);

      if (digitalRead(btnInicioParar) == nivelActivo) {
        delay(espera);
        inicializarReloj();
        relojVelocidad.Start();
      }
      // digitalWrite(led2, 1);
      break;
    case 2:
      if (digitalRead(btnAumenta) == nivelActivo || digitalRead(btnDisminuye) == nivelActivo) {
        delay(espera);
        estaActiva = false;
        relojVelocidad.Stop();
        estado = 1;
      }
      // digitalWrite(led2, 0);
      break;
  }
  // relojVelocidad.Update();
  if (estaActiva) relojVelocidad.Update();
  velocidad = analogRead(A1);
  delay(1);
}

void displayNumber(int num) {
  tm.display(3, num % 10);
  tm.display(2, num / 10 % 10);
  // tm.display(1, num / 100 % 10);
  // tm.display(0, num / 1000 % 10);
}

void displayLimite(int num) {
  tm.display(1, num % 10);
  tm.display(0, num / 10 % 10);
  // tm.display(1, num / 100 % 10);
  // tm.display(0, num / 1000 % 10);
}

void inicializarReloj() {
  digitalWrite(resetReg, 0);
  digitalWrite(resetReg, 1);
  digitalWrite(inReg, 1);
  estaActiva = true;
  tick = 0;
  flag = false;
  estado = 2;
}