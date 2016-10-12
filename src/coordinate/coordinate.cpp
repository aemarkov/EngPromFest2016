#include "coordinate.h"

uint8_t parse_state = 0;

//Извлекает координтату из json
//return - есть ли координата
uint8_t GetCoord(Coordinate* coord)
{
  //HttpClient client;
  //client.get(REQUEST);


  String x_s = "";
  String y_s = "";


  //Читаем ответ сервера по-символьно
  //Конечный авомат, чтобы вытащить "x", "y"
  String json = "{\"response\":[{\"mac\":\"B0B448D4E382\",\"ts\":1476208702257,\"coord\":{\"geoloc\":{\"lat\":61.786820445942645,\"lon\":34.35401144212008,\"alt\":80.0},\"loc\":{\"x\":3824271.059910759,\"y\":8808770.314990286,\"z\":80.0}},\"motion\":{\"type\":[]},\"raw\":{\"type\":[\"UWB\"]}}]}";
  //while (client.available()) {
    //char c = client.read();

    Serial.println(json);

  for(int i = 0; i<json.length(); i++)
  {
    char c=json[i];

    Serial.print("c=");
    Serial.print(c);
    Serial.print(' ');
    Serial.println(parse_state);

    //Serial.println(c!='[');
    //Serial.println(parse_state==0 && (c!='['));

    /*if(parse_state==0 && c!='[')
      parse_state=0;
    else if(parse_state==0 && c=='[')
      parse_state = 100;
    else if(parse_state==100 && c==']')
    {
      //Пустой response
      Serial.println("empty request");
      return 0;
    }*/
    //Автомат
    if(parse_state==0 && c == '\"')
      parse_state = 1;
    else if(parse_state==1 && c=='x')
      parse_state = 2;
    else if(parse_state==2 && c=='\"')
      parse_state = 3;
    else if(parse_state==3 && c==' ') //!
      parse_state = 3;
    else if(parse_state==3 && c==':')
      parse_state = 5;
    else if(parse_state==5 && c==' ') //!
      parse_state = 5;
    else if(parse_state==5 && (isDigit(c) || c=='.'))  // coord
    {
      parse_state = 6;
      x_s+=c;
    }
    else if(parse_state==6 && (isDigit(c) || c=='.'))
    {
      parse_state=6;
      x_s+=c;
    }
    else if(parse_state == 6 && c==',')
      parse_state = 7;
    else if(parse_state == 7 && c!='\"')
      parse_state = 7;
    else if(parse_state == 7 && c=='\"')
      parse_state = 8;
    else if(parse_state == 8 && c=='y')
      parse_state = 9;
    else if(parse_state == 9 && c=='\"')
      parse_state = 10;
    else if(parse_state == 10 && c==' ')
      parse_state = 10;
    else if(parse_state == 10 && c==':')
      parse_state = 11;
    else if(parse_state == 11 && c==' ')
      parse_state = 11;
    else if(parse_state==11 && (isDigit(c) || c=='.'))
    {
      parse_state = 12;
      y_s += c;
    }
    else if(parse_state == 12 && (isDigit(c) || c=='.'))
    {
      parse_state = 12;
      y_s += c;
    }
    else if(parse_state == 12 && c==',')
    {
      //Конец
      Serial.print("x=");Serial.println(x_s);
      Serial.print("y=");Serial.println(y_s);
      return true;
    }
    /*else if(parse_state=100 && c!=']')
      parse_state=100;*/
    else
    {
      //Ошибка парсинга
      //Serial.println("parse error");
      //return 0;
    }
  }
}
