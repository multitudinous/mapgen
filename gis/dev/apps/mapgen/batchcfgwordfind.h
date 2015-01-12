#ifndef BATCHCFGWORDFIND_H
#define BATCHCFGWORDFIND_H

#include "batchcfg.h"
#include <string>
#include <list>
#include "drawattr.h"

class BatchCfgWordFind : public BatchCfg
{
protected:
    struct BatchItem
    {
        std::string jsonFile;
        std::string wordFile;
        std::string outImgFile;
        std::string resultsFile;
        std::string validationFile;
        std::string name;
    };
    typedef std::tr1::shared_ptr<BatchItem> PBatchItem;

public:
    BatchCfgWordFind(const char *jsonFolder, const char *wordFolder, const char *outFolder);

    virtual bool init();
    virtual bool process(GisSys *pgis);

protected:

protected:
    std::string _jsonFolder;
    std::string _wordFolder;
    std::string _outFolder;
    std::list<PBatchItem> _batchItems;
    PDrawAttr _mapStyle;
};

#endif
