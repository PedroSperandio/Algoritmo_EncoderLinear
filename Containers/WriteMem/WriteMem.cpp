#include "WriteMem.h"
#include "../test_struct.h"
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <iomanip>
#include <string>
#include <sstream>
#include <utility>  
#include <math.h>
#include <ctime>
#include <wiringPi.h>
#include <vector>
#include "../Utils/road_time.h"
#include "../general_defines.h"


using namespace std;

wiringPiSetup();

WriteMem::WriteMem()
{
    
    
    this->data = new PosixShMem("SH_MEM",sizeof(TIMESTAMPED_TEST_DATA));
    this->startActivity();
}

WriteMem::~WriteMem()
{
    this->stopActivity();
}

void WriteMem::startActivity()
{
    ThreadBase::startActivity();
}

void WriteMem::stopActivity()
{
    ThreadBase::stopActivity();
    std::cout << "WRITE" << std::endl;
}

int WriteMem::run()
{
  
	this->is_running = 1;
	this->is_alive = 1;
	this->tim1.tv_sec = 0;
	this->tim1.tv_nsec = 500000000L;//20Hz

	TIMESTAMPED_TEST_DATA my_data;
    int my_cont = -1;                           //contador de processos na maquina.
    int cont=0;                                 //controle do if.
    double velocity_angular;                    //Salva o valor da velocidade angular.

    void pinMode(PINOSENSOR, INPUT);

    clock_t t1, t0,t3;                          //Variáveis para manipulação de tempo.
    double t2;                                      
    int parametro=1;                            //parametro de permissão para calcular o tempo e a velocidade angular.
    t0=clock();
    t1=clock();

	road_time_t previous_time = road_time(); // microseconds ex. 100287238913721
	
	road_time_t current_time;
	
	int previous_state = digitalRead(PINOSENSOR);
	int current_state;
	
	long long unsigned int enc_counter = 0;

    while(this->is_alive)
    {    
		current_state = digitalRead(PINOSENSOR);
		
        if(previous_state != current_state)
		{
			++enc_counter;
			
			previous_state = current_state;
			
			current_time = road_time();
			
			velocity_angular = 2*PI*MIN_ANGLE/((double)(current_time - previous_time)/1000000.0);		//rad/s

            previous_time = current_time;
			
			my_data.velocidade = velocity_angular*RADIUS;
			
			my_data.time1 = current_time;
			
			my_data.data.contador = enc_counter;
        }
        
		
		this->data->write(&my_data, sizeof(TIMESTAMPED_TEST_DATA));             //Escreve os valores de velocidade linear na memoria compartilhada.
		//std:: cout<< "Escrita"<< std:: endl;
		
		nanosleep(&this->tim1, &this->tim2);
       
    }
    this->is_running = 0;
    pthread_exit(NULL);
    
    return EXIT_SUCCESS;
}
