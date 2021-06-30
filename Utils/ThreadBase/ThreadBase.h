#ifndef THREADBASE_H
#define THREADBASE_H

#include<pthread.h>
#include<time.h>

class ThreadBase
{
private:
    pthread_t thread;                  //Ponteiro da thread
    struct timespec tim3, tim4; //Tempos utilizados para parar o loop
    static void * start(void *param);//ponteiro para função que inicia a thread
protected:
    int is_running; //Indicador que a thread esta rodando
    int is_alive;//Indicador que a thread esta viva
    struct timespec tim1, tim2;// Tempo de sleep da thread

    ThreadBase();//construtor da classe
    virtual ~ThreadBase();//Destrutor da classe
    virtual int run() = 0;//Função da Thread
public:
    virtual void startActivity();//Função ao iniciar uma thread
    virtual void stopActivity();//Função ao terminar uma thread
};

#endif