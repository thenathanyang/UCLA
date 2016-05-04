#ifndef INTELWEB_H_
#define INTELWEB_H_

#include "InteractionTuple.h"
#include "DiskMultiMap.h"
#include <string>
#include <vector>

class IntelWeb
{
public:
    IntelWeb();
    ~IntelWeb();
    bool createNew(const std::string& filePrefix, unsigned int maxDataItems);
    bool openExisting(const std::string& filePrefix);
    void close();
    bool ingest(const std::string& telemetryFile);
    unsigned int crawl(const std::vector<std::string>& indicators,
                   unsigned int minPrevalenceToBeGood,
                   std::vector<std::string>& badEntitiesFound,
                   std::vector<InteractionTuple>& interactions
                  );
    bool purge(const std::string& entity);

private:
    // Your private member declarations will go here
    DiskMultiMap dmm_forward;
    DiskMultiMap dmm_backward;
    DiskMultiMap dmm_prevalence;

    void insertPrevalence(const string& item);
};

inline
bool operator<(const InteractionTuple& lhs, const InteractionTuple& rhs)
{
    return (lhs.context < rhs.context) || 
        (lhs.context == rhs.context && lhs.from < rhs.from) || 
        (lhs.context == rhs.context && lhs.from == rhs.from && lhs.to < rhs.to);

    // if (lhs.conext < rhs.context) return true;
    // if (lhs.conext == rhs.context && lhs.from < rhs.from) return true;
    // if (lhs.conext == rhs.context && lhs.from == rhs.from && lhs.to < rhs.to) return true;
    // return false;
}
   
inline 
bool operator==(const InteractionTuple& lhs, const InteractionTuple& rhs)
{
    return lhs.from == rhs.from && lhs.to == rhs.to && lhs.context == rhs.context;
}

#endif // INTELWEB_H_
