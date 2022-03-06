#define RIGHT_MOTOR_PWM 7            // обязательно должен быть ШИМ
#define LEFT_MOTOR_PWM 3              // обязательно должен быть ШИМ
#define LEFT_MOTOR_D 2
#define RIGHT_MOTOR_D 6
#define VAC_CLEANER 25 

char _bluetoothValue;

void setup()
{
  Serial.begin(9600);

  pinMode(RIGHT_MOTOR_PWM, OUTPUT);
  pinMode(LEFT_MOTOR_PWM, OUTPUT);
  pinMode(RIGHT_MOTOR_D, OUTPUT);
  pinMode(LEFT_MOTOR_D, OUTPUT);

  pinMode(VAC_CLEANER, OUTPUT);
}

void loop()
{
  if(Serial.available())
  {
    _bluetoothValue = Serial.read();
    switch(_bluetoothValue)
    {
      case '1':   
      Forward();
      break;
  
      case  '2':
      Left();
      break;

      case '3':
      Right();
      break;
  
      case '4':
      Backward();
      break;
  
      case '5':
      CleaningOn();
      break;
  
      case '6':
      CleaningOff();
      break;
  
      case '7':
      Stop();
      break;
    }
  }
}

void CleaningOn() {digitalWrite(VAC_CLEANER, HIGH);}

void CleaningOff() {digitalWrite(VAC_CLEANER, LOW);}

void Forward() 
{
  digitalWrite(RIGHT_MOTOR_PWM, LOW);
  digitalWrite(RIGHT_MOTOR_D, HIGH);
  digitalWrite(LEFT_MOTOR_D, HIGH);
  digitalWrite(LEFT_MOTOR_PWM, LOW);
}
  
void Backward() 
{
  digitalWrite(RIGHT_MOTOR_PWM, HIGH);
  digitalWrite(RIGHT_MOTOR_D, LOW);
  digitalWrite(LEFT_MOTOR_PWM, HIGH);
  digitalWrite(LEFT_MOTOR_D, LOW);
}

void Right() 
{
  analogWrite(RIGHT_MOTOR_PWM, 40);
  analogWrite(RIGHT_MOTOR_D, 0);
  analogWrite(LEFT_MOTOR_D, 0);
  analogWrite(LEFT_MOTOR_PWM, 120); 
}

void Left() 
{
  analogWrite(RIGHT_MOTOR_PWM, 120);
  analogWrite(RIGHT_MOTOR_D, 0);
  analogWrite(LEFT_MOTOR_D, 0);
  analogWrite(LEFT_MOTOR_PWM, 40); 
}

void Stop()
{
  digitalWrite(RIGHT_MOTOR_PWM, LOW);
  digitalWrite(RIGHT_MOTOR_D, LOW);
  digitalWrite(LEFT_MOTOR_D, LOW);
  digitalWrite(LEFT_MOTOR_PWM, LOW);
}
