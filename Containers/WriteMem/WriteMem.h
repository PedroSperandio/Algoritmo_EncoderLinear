#ifndef WRITEMEM_H
#define WRITEMEM_H

#include "../../Utils/PosixShMem/PosixShMem.h"
#include "../../Utils/ThreadBase/ThreadBase.h"
#include <fstream>
#include <string>
#include <stdio.h>
#include <iostream>
using namespace std;
class WriteMem: public ThreadBase
{
public:
    WriteMem();
    ~WriteMem();

private:

    PosixShMem *data;
    void startActivity() override;
    void stopActivity() override;
    int run() override;
    
    
};

#endif