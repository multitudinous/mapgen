#ifndef DATAVALIDATE_H
#define DATAVALIDATE_H

#include "sysdefs.h"

class DataValidate
{
public:
    DataValidate();
    virtual ~DataValidate();

    bool init(const char *filepath);

    bool validate() { return _validate;  }
    void validate(bool v) { _validate = v; }

    FILE* file() { return _fp; }

protected:
    bool _validate;
    FILE *_fp;
};
typedef shared_ptr<DataValidate > PDataValidate;

#endif