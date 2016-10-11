//Парсинг координат
//Для уменьшения объема программы и потребляемой
//памяти, полноценный парсинг json заменен на
//достаточный костыль

#ifndef __COORDINATE_H__
#define __COORDINATE_H__

#include <inttypes.h>
#include <HttpClient.h>

#define COORD_TYPE double
#define REQUEST "http://192.168.99.5:9002/rtlscp/location/B0B448D4E382/get"

//Структура координат
struct Coordinate
{
  COORD_TYPE x;
  COORD_TYPE y;
};

//Извлекает координтату из json
//return - есть ли координата
uint8_t GetCoord(Coordinate* coord);

#endif
