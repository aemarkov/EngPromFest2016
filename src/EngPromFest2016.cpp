#include <Arduino.h>
#include <Bridge.h>
#include <HttpClient.h>
#include <inttypes.h>

#include "shieldbot\Shieldbot.h" //????
#include "coordinate\coordinate.h"
#include <consts.h>
/*
Все расстояния в сантиметрах. Перевод - в файле coordinate.h
Все расстояния в целых градусах
*/

//Параметры движенния
#define TIME_PER_1_DEGREE 11 //ms
#define TIME_PER_SM 30       //ms

#define MOTOR_MAX 127
#define MOTOR_HALF 64
#define MOTOR_TURN 20

#define PI_2 1.5707963

Coordinate oldCoord, coord;
Coordinate targetCoord;

Coordinate ourDirection;
//long last_update;

Shieldbot shieldbot;

// повернуться на месте на угол angle (deg)
void rotateToAngle(int angle);

//ЕЗда вперед на определенное расстояние
void move_sm(int16_t dist);

//Полуить новые координаты
void updateCoord();

Coordinate vectorDifference(Coordinate a, Coordinate b);
float distanceBetweenPoints(Coordinate a, Coordinate b);
float vectorLength(Coordinate a);
float angleBetweenVectors(Coordinate a, Coordinate b);

void setup()
{
  pinMode(13, OUTPUT);
  digitalWrite(13, LOW);
  Bridge.begin();
  digitalWrite(13, HIGH);

  Serial.begin(9600);
  Serial.println("Hello!");

  //Определение первоначального угла
  updateCoord();
  move_sm(10);
  updateCoord();
  //last_update = millis();

  targetCoord.x =  0 ;
  targetCoord.y =  0 ;
}

void loop()
{

  /*Serial.println(45);
  rotateToAngle(45);
  delay(1000);
  Serial.println(-45);
  rotateToAngle(-45);
  delay(1000);
  Serial.println(135);
  rotateToAngle(135);
  delay(1000);
  Serial.println(-135);
  rotateToAngle(-135);
  delay(4000);
  return;*/

  //Смещение с прошлого момоента
  int16_t movement = distanceBetweenPoints(oldCoord, coord);

  //Если сместились - обновляем текущий курс
  if(movement>20)
    ourDirection = vectorDifference(coord, oldCoord);

  //Вычисляем расстояние и курс на цель
  int16_t dist_to_target = distanceBetweenPoints(targetCoord, coord);

  Coordinate targetDirection = vectorDifference(targetCoord, coord);
  float angle_to_target = angleBetweenVectors(ourDirection,targetDirection);


  Serial.print("x=");Serial.print(coord.x);Serial.print(" y=");Serial.print(coord.y);
  //Serial.print(" angle="); Serial.print(our_angle);
  Serial.print(" angle_to_target="); Serial.print(angle_to_target);
  Serial.print(" dist_to_target=");  Serial.println(dist_to_target);

  if(dist_to_target<MIN_DIST)
  {
    rotateToAngle(360);

    //Fin
    while(1)
    {
      digitalWrite(13, HIGH);
      delay(100);
      digitalWrite(13, LOW);
      delay(13);
    }
  }

  int16_t abs_ang = abs(angle_to_target);

  if(abs_ang>10)
  {
    //Поворот на месте
    rotateToAngle(angle_to_target);
    ourDirection = targetDirection;
  }
  else
  {
    //Прямо
    move_sm(10);
  }


  updateCoord();
  delay(1000);
}


// повернуться на угол angle
//+ - направо, - -налево
void rotateToAngle(int angle)
{
  Serial.print("Rotate ");
  Serial.println(angle);
  if(angle < 0) // поврот налево
  {
    shieldbot.drive(-MOTOR_TURN, MOTOR_TURN);
  }
  else // поворот направо
  {
    shieldbot.drive(MOTOR_TURN, -MOTOR_TURN);
  }
  // ждем время для поворота
  delay(abs(angle) * TIME_PER_1_DEGREE);
  // отключаем двигатели
  shieldbot.drive(0, 0 );
}


//ЕЗда вперед на определенное расстояние
void move_sm(int16_t dist)
{
  Serial.print("move ");
  Serial.println(dist);
  shieldbot.drive(MOTOR_HALF, MOTOR_HALF);
  delay(abs(dist) * TIME_PER_SM);
  shieldbot.drive(0, 0);

}

//Полуить новые координаты
void updateCoord()
{
  oldCoord.x = coord.x;
  oldCoord.y = coord.y;
  GetCoord(&coord);
}







//Расстояние между точками
float distanceBetweenPoints(Coordinate a, Coordinate b)
{
    return sqrt((a.x-b.x)*(a.x-b.x) + (a.y-b.y)*(a.y-b.y));
}

//Вычитание координат (векторов)
Coordinate vectorDifference(Coordinate a, Coordinate b)
{
  Coordinate c;
  c.x = a.x-b.x;
  c.y = a.y-b.y;
  return c;
}

float vectorLength(Coordinate a)
{
  return sqrt(a.x*a.x+a.y*a.y);
}

float  angleBetweenVectors(Coordinate a, Coordinate b)
{
  /*float dist1 = vectorLength(a);
  float dist2 = vectorLength(b);
  return (a.x*b.x + a.y*b.y)/(dist1*dist2);*/
  //return degrees(atan2(a.y-b.y, a.x-b.x));
  return degrees(atan2(a.x*b.y-a.y*b.x,a.x*b.x+a.y*b.y));
}
