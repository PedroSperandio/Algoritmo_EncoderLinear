#include "ReadMem.h"
#include "../test_struct.h"
#include <fstream>
#include <iostream>
#include <wiringPi.h>
#define PINO_PWM
#include "../general_defines.h"

using namespace std;

wiringPiSetup();

ReadMem::ReadMem()///Construtor
{
    this->data = new PosixShMem("SH_MEM",sizeof(TIMESTAMPED_TEST_DATA));//é o que a thread vai ler
    this->startActivity();  /// Inicio a thread
}

ReadMem::~ReadMem() ///Destrutor
{
    this->stopActivity(); ///Finalizo a thread
    delete this->data;/// deleto o espaço alocado
}

void ReadMem::startActivity()
{
    ThreadBase::startActivity();
}

void ReadMem::stopActivity()
{
    ThreadBase::stopActivity();
    std::cout << "READ" << std::endl;
}

int ReadMem::run()
{
    this->is_running = 1;
    this->is_alive = 1;
    this->tim1.tv_sec = 0;
    this->tim1.tv_nsec = 100000000L;//10Hz
    
    TIMESTAMPED_TEST_DATA my_data;

    
   double required_speed= 25;                   //Uma velocidade de Teste para ver o funcionamento do algoritmo.
   double My_speed;                            //Variável que será salvo o valor da velocidade linear que está na memória compartilhada. 
   double assistant;                                    //Salva a diferença entre a velocidade_requerida e a Minha_velocidade.
   double value_pwm;
   double value_brake;

    ofstream arquivo1;
    arquivo1.open("Velocidade_saida",ios::app);       //Cria e abre um arquivo para salvar o valor da velocidade de saída.


    void pinMode(PINO_MOTORPRINCIPAL, OUTPUT);
    void pinMode(PINO_MOTORFREIO, OUTPUT);

    while(this->is_alive)
    {
        //std::cout<<" teste"<<std::endl;
        this->data->read(&my_data, sizeof(TIMESTAMPED_TEST_DATA));         //busca na posição de memoria da máquina os valores de velocidade salvos na memoria compartilhada.

        std::cout<<" Velocidade: "<<my_data.velocidade<<std::endl;
        arquivo1 << my_data.velocidade<<endl;

        std::cout <<  "TEMPO: " << my_data.time1 << std::endl;

       My_speed = my_data.velocidade;                               // Inicio da lógica de controle. 
       assistant = required_speed-My_speed;

      if(assistant > 0){   
                                                  // Compara os valor da diferença entre a velocidade_requerida e a Minha_velocidade e toma a decisão necessária.
            std::cout<< "ACELERAR" <<std::endl;
            analogWrite(PINO_MOTORFREIO,0);
            
			if(assistant > MAX_ACC)
			{
				value_pwm = (MAX_ACC/10.0 +My_speed)*CONST_PWM;
			}
			else
			{
				value_pwm = (assistant/10.0 +My_speed)*CONST_PWM;
			}
			analogWrite(PINO_MOTORPRINCIPAL,value_pwm);
			
        }
      else if(assistant< 0){
            
			if(assistant < MAX_DCC)
			{
				value_brake = ((MAX_DCC/10 -My_speed)*CONST_PWM_FREIO)*-1;
			}
			else
			{
				value_brake = ((assistant/10.0 -My_speed)*CONST_PWM_FREIO)*-1;
			}

			//analogWrite(PINO_MOTORPRINCIPAL,valor_pwm);
            std::cout<<"FREAR"<< std::endl;
            value_brake = assistant*CONST_PWM_FREIO;
            analogWrite(PINO_MOTORFREIO,value_brake);
            
        }
      else if(assistant == 0){
            std::cout<<"MANTER"<< std::endl;
            analogWrite(PINO_PWM,value_pwm);
            analogWrite(PINO_MOTORFREIO,0);
        }

        //Realizará a leitura da memória e apresentará os dados no terminal
        //"Cont: " << my_data.data.contador << "
        nanosleep(&this->tim1, &this->tim2);
    }
    arquivo1.close();
    this->is_running = 0;
    pthread_exit(NULL);
    return EXIT_SUCCESS;
}
