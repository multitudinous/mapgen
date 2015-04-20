#ifndef BATCHCFG_H
#define BATCHCFG_H

#include <memory>

class GisSys;

class BatchCfg
{
public:
    BatchCfg() {}

    virtual bool init() { return true; }
    virtual bool process( GisSys* ) { return false; } // return false when done
};

typedef std::shared_ptr<BatchCfg> PBatchCfg;

#endif
