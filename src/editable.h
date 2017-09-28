#ifndef EDITABLE_H
#define EDITABLE_H

#include "vector.h"

class Editable {
public:
    virtual bool select(Vec)=0;
    
    virtual void mouse(Vec,Vec)=0;
    virtual void cancel()=0;
    virtual void confirm()=0;
    virtual bool key(char)=0;
};

#endif