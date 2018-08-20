#ifndef VERSION_H
#define VERSION_H
#include "basemodel.h"

class Version : public BaseModel
{
public:
    Version();
    int version;
    int currentDbVersion();
    void upgrade();
};

#endif // VERSION_H
