

#include <GyverMotor.h>

#define PUMP_DIG 1                   //пин сосущей части
#define MOTOR_MAX 255                 // максимальный сигнал на мотор (max 255)
#define JOY_MAX 40                    // рабочий ход джойстика (из приложения)
#define minDuty 0                     // скорость, при которой мотор должен начинать крутится (обычно 25-50)
#define RIGHT_MOTOR_DIRECTION NORM  // напрваление правого мотора (NORMAL или REVERSE)
#define LEFT_MOTOR_DIRECTION  NORM  // напрваление левого мотора (NORMAL или REVERSE)
#define RIGHT_MOTOR_MODE HIGH         // смени HIGH на LOW если мотор включает тормоз
#define LEFT_MOTOR_MODE HIGH          // смени HIGH на LOW если мотор включает тормоз
#define PUMP_STATE LOW



#define RIGHT_MOTOR_PWM 7            // обязательно должен быть ШИМ
#define LEFT_MOTOR_PWM 3              // обязательно должен быть ШИМ
#define LEFT_MOTOR_D 2
#define RIGHT_MOTOR_D 6         // обязательно должен быть ШИМ пином!!!
#define BT_TX 0
#define BT_RX 1
#define DRIVER2WIRE 0
#define VAC_CLEANER 25

#include "GyverMotor.h"
GMotor motorR( RIGHT_MOTOR_D, RIGHT_MOTOR_PWM);
GMotor motorL( LEFT_MOTOR_D, LEFT_MOTOR_PWM);

#include <SoftwareSerial.h>
SoftwareSerial BTserial(BT_TX, BT_RX); // TX, RX

boolean doneParsing, startParsing;
int dataX, dataY;
String string_convert;

void setup() {

  //команды, начинающиеся с '#' - команды прекомпилятору, т. е. обрабатіваются ПК, не Ардуино
  #if (LEFT_MOTOR_PWM == 3 || LEFT_MOTOR_PWM == 11 || RIGHT_MOTOR_PWM == 3 || RIGHT_MOTOR_PWM == 11)
    //D3 и D11 62.5 kHz PWM 
    TCCR2B = 0b00000001;
    TCCR2A = 0b00000011;
  #elif (LEFT_MOTOR_PWM == 9 || LEFT_MOTOR_PWM == 10 || RIGHT_MOTOR_PWM == 9 || RIGHT_MOTOR_PWM == 10)
    //D9 и D10 62.5 kHz PWM 
    TCCR1A = 0b00000001;
    TCCR1B = 0b00001001;
  #endif

  BTserial.begin(9600);
  pinMode(VAC_CLEANER, OUTPUT); 
  motorR.setMode(minDuty);
  motorL.setMode(minDuty);
//motorR.setMinDuty(minDuty);
// motorL.setMinDuty(minDuty);
  motorR.setDirection(RIGHT_MOTOR_DIRECTION);
  motorL.setDirection(LEFT_MOTOR_DIRECTION);
}

void loop() {
  
  parsing();         // функция парсинга
  if (doneParsing) { // если получены данные
    doneParsing = false;

    int joystickX = map((dataX), -JOY_MAX, JOY_MAX, -MOTOR_MAX / 2, MOTOR_MAX / 2); // сигнал по Х
    int joystickY = map((dataY), -JOY_MAX, JOY_MAX, -MOTOR_MAX, MOTOR_MAX);         // сигнал по Y

    int dutyR = joystickY + joystickX; // считаем сигнал для правого мотора
    int dutyL = joystickY - joystickX; // считаем сигнал для левого мотора

    dutyR = constrain(dutyR, -MOTOR_MAX, MOTOR_MAX); // ограничиваем значение для dutyR, на случай если dutyR > 255
    dutyL = constrain(dutyL, -MOTOR_MAX, MOTOR_MAX); // ограничиваем значение для dutyL, на случай если dutyL > 255
    
//motorR.smoothTick(dutyR); // плавно крутим правый мотор
//motorL.smoothTick(dutyL); // плавно крутим левый мотор
  }
}
void parsing() {
  if (BTserial.available() > 0) {       // если в буфере есть данные

    char incomingChar = BTserial.read();// читаем из буфера
    if (startParsing) {                 // начать принятие пакетов
      if (incomingChar == ' ') {        // принят пакет dataX
        dataX = string_convert.toInt(); // ковертируем принятый пакет в переменную
        string_convert = "";            // очищаем переменную пакета
      }
      else if (incomingChar == ';') {   // принят пакет dataY
        dataY = string_convert.toInt(); // ковертируем принятый пакет в переменную
        string_convert = "";            // очищаем переменную пакета
        startParsing = false;           // закончить принятие пакетов
        doneParsing = true;             // парсинг окончен, можно переходить к движению
      } else {
        string_convert += incomingChar; // записываем  принятые данные в переменную
      }
    }
    if (incomingChar == '$') {          // начало парсинга
      startParsing = true;              // начать принятие пакетов
    }
  }
}

void CleaningOn() {digitalWrite(VAC_CLEANER, HIGH);}

void CleaningOff() {digitalWrite(VAC_CLEANER, LOW);}
