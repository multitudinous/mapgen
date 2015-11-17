#include "datavalidate.h"

//============================================================================
//============================================================================
DataValidate::DataValidate() :
    _validate(false),
    _fp(NULL)
{
}

//============================================================================
//============================================================================
DataValidate::~DataValidate()
{
    if (_fp) fclose(_fp);
}

//============================================================================
//============================================================================
bool DataValidate::init(const char *filepath)
{
    if (_fp) fclose(_fp);

    _fp = fopen(filepath, "wt");
    if (!_fp)
    {
        LogError("DataValidate::init - failed to open file %s", filepath);
        return false;
    }

    return true;
}