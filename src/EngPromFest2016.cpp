#include <Arduino.h>
#include "coordinate\coordinate.h"

#define TIME_PER_5_DEGREE 15
#define TIME_PER_SM 30
#define MIN_DIST 100

#define MOTOR_MAX 127
#define MOTOR_HALF 64

#define PI 3.1415926
#define PI_2 1.5707963

Coordinate oldCoord, coord;
Coordinate targetCoord;
long last_update;

// повернуться на месте на угол angle
void rotateToAngle(int angle);

//Езда вперед с корректировкой
void move(int8_t speed, float angle);

//ЕЗда вперед на определенное расстояние
void move_for_dist(int8_t dist);

//Полуить новые координаты
void updateCoord();

float getDist(Coordinate a, Coordinate b);
float getVectorAngle(Coordinate a, Coordinate b);
float getBearing(Coordinate cur, Coordinate old, Coordinate target);

void setup()
{
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(13,OUTPUT);

  //Определение первоначального угла
  updateCoord();
  move_for_dist(100);
  updateCoord();
  last_update = millis();
}

void loop()
{
  float dist_to_target = getDist(coord, targetCoord);
  if(dist_to_target<MIN_DIST)
  {
    //Fin
    while(1)
    {
      digitalWrite(13, HIGH);
      delay(100);
      digitalWrite(13, LOW);
      delay(13);
    }
  }

  float angle_to_target = getBearing(coord, oldCoord, targetCoord);
  float abs_ang = abs(angle_to_target);

  Serial.print(angle_to_target);Serial.print(' ');
  Serial.println(dist_to_target);

  if(abs_ang>45)
  {
    //Поворот на месте
    rotateToAngle(angle_to_target);
  }
  else if(abs_ang>10)
  {
    //Подруливание
    move(MOTOR_HALF, angle_to_target);
  }
  else
  {
    //Прямо
    move(MOTOR_HALF, 0);
  }

  if(millis()-last_update>1000)
  {
    updateCoord();
    last_update = millis();
  }
}


// повернуться на угол angle
void rotateToAngle(int angle)
{
  if(angle < 0) // поврот налево
  {
    //shieldbot.drive(-leftDriveSpeed, rightDriveSpeed);
  }
  else // поворот направо
  {
    //shieldbot.drive(leftDriveSpeed, -rightDriveSpeed);
  }
  // ждем время для поворота
  delay(TIME_PER_5_DEGREE * (angle / 5));
  // отключаем двигатели
  //shieldbot.drive(0, 0 );
}

//Езда вперед с корректировкой
void move(int8_t speed, float angle)
{
  int8_t l_speed, r_speed;
  if(speed>0)
  {
    //Forwrd
    l_speed = speed + angle/PI_2*MOTOR_HALF;
    r_speed = speed - angle/PI_2*MOTOR_HALF;
  }
  else
  {
    l_speed = speed - angle/PI_2*MOTOR_HALF;
    r_speed = speed + angle/PI_2*MOTOR_HALF;
  }

  //shieldbot.drive(l_speed, r_speed);
}

//ЕЗда вперед на определенное расстояние
void move_for_dist(int8_t dist)
{
  //shieldbot.drive(MOTOR_MAX, MOTOR_MAX);
  delay(dist * TIME_PER_SM);
  //shieldbot.drive(0, 0);
}

//Полуить новые координаты
void updateCoord()
{
  oldCoord.x = coord.x;
  oldCoord.y = coord.y;
  GetCoord(&coord);
}

float getDist(Coordinate a, Coordinate b)
{
    return sqrt((a.x-b.x)*(a.x-b.x) + (a.y-b.y)*(a.y-b.y));
}

float getVectorAngle(Coordinate a, Coordinate b)
{
  return atan2(b.y-a.y, b.x-a.x);
}

float  getBearing(Coordinate cur, Coordinate old, Coordinate target)
{
  float my_angle = getVectorAngle(cur, old);
  float angle_to_target = getVectorAngle(target, cur);
  return my_angle - angle_to_target;
}
