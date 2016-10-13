#include "coordinate.h"



//Извлекает координтату из json
//return - есть ли координата
/* Исопльзуется простой конечный
автомат, чтобы вытащить из json коордианты вида
"x":dddd.dddd,
"y":dddd.dddd,

Плюсы решения:
 1. Меньший размер, чем у ArduinoJson
 2. Минимальный объем потребляемой ОЗУ
*/
uint8_t GetCoord(Coordinate* coord)
{
  HttpClient client;
  client.get(REQUEST);


  String buff = "";
  //String y_s = "";
  float x=0, y=0;
  uint8_t parse_state = 0;

  //Читаем ответ сервера по-символьно
  //Конечный авомат, чтобы вытащить "x", "y"
  while (client.available()) {
    char c = client.read();
    /*Serial.print("c=");
    Serial.print(c);
    Serial.print(' ');
    Serial.println(parse_state);*/

    if(parse_state==0 && c=='[')
      parse_state = 100;
    else if(parse_state==100 && c==']')
    {
      //Пустой response
      Serial.println("empty request");
      return 0;
    }
    //Поиск координаты х
    else if(parse_state==100 && c == '\"')
      parse_state = 1;
    else if(parse_state==1 && c=='x')
      parse_state = 2;
    else if(parse_state==2 && c=='\"')
      parse_state = 3;
    else if(parse_state==3 && c==':')
      parse_state = 5;

    //Координата х
    else if(parse_state==5 && (isDigit(c) || c=='.'))  // coord
    {
      parse_state = 6;
      buff+=c;
    }
    else if(parse_state==6 && (isDigit(c) || c=='.'))
    {
      parse_state=6;
      buff+=c;
    }

    //Поиск координаты y
    else if((parse_state == 6 || parse_state==5) && c==',')
    {
      x = buff.toFloat();
      buff="";
      parse_state = 7;
    }
    else if(parse_state == 7 && c=='\"')
      parse_state = 8;
    else if(parse_state == 8 && c=='y')
      parse_state = 9;
    else if(parse_state == 9 && c=='\"')
      parse_state = 10;
    else if(parse_state == 10 && c==':')
      parse_state = 11;

    //Координата y
    else if(parse_state==11 && (isDigit(c) || c=='.'))
    {
      parse_state = 12;
      buff += c;
    }
    else if(parse_state == 12 && (isDigit(c) || c=='.'))
    {
      parse_state = 12;
      buff += c;
    }

    //Координаты получены
    else if((parse_state == 12 || parse_state==11) && c==',')
    {
      //Конец
      y = buff.toFloat();

      //Переводим в сантиметры
      coord->x=(x-TARGET_X)*SM_PER_PARROT;
      coord->y=(y-TARGET_Y)*SM_PER_PARROT;

      //Serial.print(x);Serial.print(' ');Serial.println(y);


      //Serial.print("xp=");Serial.print(x);
      //Serial.print(" yp=");Serial.println(y);
      //Serial.print("x=");Serial.print(coord->x);
      //Serial.print(" y=");Serial.println(coord->y);
      return true;
    }
  }

  return false;
}
