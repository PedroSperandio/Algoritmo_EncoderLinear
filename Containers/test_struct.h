
//Estrutura que com um campo de dado e o tempo que foi adquirido
//Nesse exemplo os dados são os de um contador de loops

#ifndef TEST_STRUCT
#define TEST_STRUCT
#include <time.h>
#include "../Utils/road_time.h"
typedef struct
{
    //int velocidade;
    uint64_t contador;
}TEST_DATA;


typedef struct
{
    
    double velocidade;
    double time1;
    double t5, t4;
    road_time_t time;
    TEST_DATA data;
}TIMESTAMPED_TEST_DATA;



#endif