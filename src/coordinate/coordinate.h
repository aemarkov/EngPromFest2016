//Парсинг координат
//Для уменьшения объема программы и потребляемой
//памяти, полноценный парсинг json заменен на
//достаточный костыль

#ifndef __COORDINATE_H__
#define __COORDINATE_H__

#include <inttypes.h>
#include <HttpClient.h>
#include <consts.h>

#define COORD_TYPE float
#define REQUEST "http://192.168.173.1:9002/rtlscp/location/B0B448D5DE82/get"

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
