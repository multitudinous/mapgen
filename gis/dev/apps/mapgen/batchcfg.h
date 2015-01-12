#ifndef BATCHCFG_H
#define BATCHCFG_H

#include <memory>

class GisSys;

class BatchCfg
{
public:
    BatchCfg() {}

    virtual bool init() { return true; }
    virtual bool process(GisSys *pgis) { return false; } // return false when done
};

typedef std::tr1::shared_ptr<BatchCfg> PBatchCfg;

#endif
