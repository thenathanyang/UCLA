// IntelWeb.cpp

#include "IntelWeb.h"
#include "DiskMultiMap.h"
#include <iostream> // needed for any I/O
#include <fstream>  // needed in addition to <iostream> for file I/O
#include <sstream>  // needed in addition to <iostream> for string stream I/O
#include <string>
#include <unordered_map>
#include <set>
#include <vector>
#include <queue>

// === START DEBUGGING TOOLS
#include <libunwind.h>
#include <cstdlib>
#include <cstdio>
#ifndef DEBUG
#define DEBUG 1
#define STACK_TRACE 1
#define BREAK_ON_ERROR 1
#define check(__a) if (DEBUG && !(__a)) __CHECK_FAIL__(__a, __FILE__, __FUNCTION__, __LINE__);
#define checkv(__a,__f,...) if (DEBUG && !(__a)) __CHECK_FAIL__VB(__a, __FILE__, __FUNCTION__, __LINE__,__f,__VA_ARGS__)
#define __PRINT_STACK_TRACE__() { printf("---STACK TRACE---\n"); unw_cursor_t cursor; unw_context_t context; unw_getcontext(&context); unw_init_local(&cursor, &context); unw_word_t offset, pc = 1; while (unw_step(&cursor) > 0 && pc) { unw_get_reg(&cursor, UNW_REG_IP, &pc); printf(" > 0x%llx:", pc); char sym[256]; if (unw_get_proc_name(&cursor, sym, sizeof(sym), &offset) == 0) { printf(" (%s [+0x%llx])\n", sym, offset); } } }
#define __CHECK_FAIL__(__a,__b,__c,__d) { printf("---CHECK FAILED---\n > FILE: %s\n > FUNC: %s\n > LINE: %d\n > RESULT: %s\n",__b,__c,__d,(__a ? "TRUE" : "FALSE")); if (STACK_TRACE) { __PRINT_STACK_TRACE__() } if (BREAK_ON_ERROR) { printf("---STOPPING RUNTIME---\n"); exit(EXIT_FAILURE); } }
#define __CHECK_FAIL__VB(__a,__b,__c,__d,__e,...) { printf("---CHECK FAILED---\n > FILE: %s\n > FUNC: %s\n > LINE: %d\n > RESULT: %s\n > ARGS: ",__b,__c,__d,(__a ? "TRUE" : "FALSE")); { printf(__e,__VA_ARGS__); } printf("\n"); if (STACK_TRACE) { __PRINT_STACK_TRACE__() } if (BREAK_ON_ERROR) { printf("---STOPPING RUNTIME---\n"); exit(EXIT_FAILURE); } }
#endif
// === END DEBUGGING TOOLS

using namespace std;

IntelWeb::IntelWeb() {}

IntelWeb::~IntelWeb()
{
	close();
}

bool IntelWeb::createNew(const std::string& filePrefix, unsigned int maxDataItems)
{
	int numBuckets = maxDataItems / 0.6;
	//close();
	if (!dmm_forward.createNew(filePrefix + "_forward.dat", numBuckets)) return false;
	if (!dmm_backward.createNew(filePrefix + "_backward.dat", numBuckets)) return false;
	if (!dmm_prevalence.createNew(filePrefix + "_prevalence.dat", numBuckets)) return false;
	return true;
}

bool IntelWeb::openExisting(const std::string& filePrefix)
{
	// close();
	if (!dmm_forward.openExisting(filePrefix + "_forward.dat")) return false;
	if (!dmm_backward.openExisting(filePrefix + "_backward.dat")) return false;
	if (!dmm_prevalence.openExisting(filePrefix + "_prevalence.dat")) return false;
	return true;
}

void IntelWeb::close()
{
	dmm_forward.close();
	dmm_backward.close();
	dmm_prevalence.close();
}

bool IntelWeb::ingest(const std::string& telemetryFile)
{
	// Open the file for input
	ifstream inf(telemetryFile);
	if (!inf) return false;

	string line;
	while (getline(inf, line))
	{
		istringstream iss(line);

		string context;
		string key;
		string value;

		if (!(iss >> context >> key >> value)) continue;

		char dummy;
		iss >> dummy;

		dmm_forward.insert(key, value, context);
		dmm_backward.insert(value, key, context);
		
		insertPrevalence(key);
		insertPrevalence(value);
	}
	return true;
}

unsigned int IntelWeb::crawl(const std::vector<std::string>& indicators, unsigned int minPrevalenceToBeGood, std::vector<std::string>& badEntitiesFound, std::vector<InteractionTuple>& badInteractions)
{
	std::set<std::string> badEntitiesSet;
	std::unordered_map<std::string, bool> visitedMap;
	std::queue<std::string> visitQueue;
	std::set<InteractionTuple> badInteractionsSet;
	std::unordered_map<std::string, unsigned int> prevalCache;

	DiskMultiMap::Iterator dmmIt;
	DiskMultiMap::Iterator prevalIt;

	// for (std::vector<std::string>iterator it = indicators.begin(); it != indicators.end(); k++)
	for (const std::string& s : indicators)
		visitQueue.push(s);

	while(!visitQueue.empty())
	{
		std::string currKey = visitQueue.front();
		visitQueue.pop();
		
		if (visitedMap.find(currKey) == visitedMap.end())
		{
			visitedMap[currKey] = true;


			dmmIt = dmm_forward.search(currKey);
			while(dmmIt.isValid())
			{
				if (prevalCache.find(currKey) == prevalCache.end())
				{
					prevalIt = dmm_prevalence.search(currKey);
					prevalCache[currKey] = stoi((*prevalIt).value);
				}

				if (prevalCache.find((*dmmIt).value) == prevalCache.end())
				{
					prevalIt = dmm_prevalence.search((*dmmIt).value);
					prevalCache[(*dmmIt).value] = stoi((*prevalIt).value);
				}

				// if (stoi((*prevalIt).value) < minPrevalenceToBeGood)
				if (prevalCache[currKey] < minPrevalenceToBeGood && prevalCache[(*dmmIt).value] < minPrevalenceToBeGood)
				{
					visitQueue.push((*dmmIt).value);
					badEntitiesSet.insert((*dmmIt).key);
					badEntitiesSet.insert((*dmmIt).value);
					//badInteractionsSet.insert(InteractionTuple((*dmmIt).key, (*dmmIt).value, (*dmmIt).context));
				}

				if (prevalCache[currKey] < minPrevalenceToBeGood || prevalCache[(*dmmIt).value] < minPrevalenceToBeGood)
					badInteractionsSet.insert(InteractionTuple((*dmmIt).key, (*dmmIt).value, (*dmmIt).context));

				++dmmIt;
			}

			dmmIt = dmm_backward.search(currKey);
			while(dmmIt.isValid())
			{
				if (prevalCache.find(currKey) == prevalCache.end())
				{
					prevalIt = dmm_prevalence.search(currKey);
					prevalCache[currKey] = stoi((*prevalIt).value);
				}

				if (prevalCache.find((*dmmIt).value) == prevalCache.end())
				{
					prevalIt = dmm_prevalence.search((*dmmIt).value);
					prevalCache[(*dmmIt).value] = stoi((*prevalIt).value);
				}

				//prevalIt = dmm_prevalence.search(currKey);
				if (prevalCache[currKey] < minPrevalenceToBeGood && prevalCache[(*dmmIt).value] < minPrevalenceToBeGood)
				{
					visitQueue.push((*dmmIt).value);
					badEntitiesSet.insert((*dmmIt).key);
					badEntitiesSet.insert((*dmmIt).value);
				}

				// insert interaction if either the key or value has prev < minPrev
				if (prevalCache[currKey] < minPrevalenceToBeGood || prevalCache[(*dmmIt).value] < minPrevalenceToBeGood)
					badInteractionsSet.insert(InteractionTuple((*dmmIt).value, (*dmmIt).key, (*dmmIt).context));

				++dmmIt;
			}
		}
	}

	std::vector<std::string> badEntitiesFoundTemp (badEntitiesSet.begin(), badEntitiesSet.end());
	std::vector<InteractionTuple> badInteractionsTemp (badInteractionsSet.begin(), badInteractionsSet.end());

	badEntitiesFound = badEntitiesFoundTemp;
	badInteractions = badInteractionsTemp;

	return badEntitiesFoundTemp.size();
}

bool IntelWeb::purge(const std::string& entity)
{
	bool hasPurged = false;

	DiskMultiMap::Iterator dmmIt;
	std::queue<std::string> toDeleteQueue;

	toDeleteQueue.push(entity);

	while(!toDeleteQueue.empty())
	{
		std::string currKey = toDeleteQueue.front();
		toDeleteQueue.pop();

		dmmIt = dmm_forward.search(currKey);
		while (dmmIt.isValid())
		{
			dmm_forward.erase((*dmmIt).key, (*dmmIt).value, (*dmmIt).context);
			hasPurged = true;
			toDeleteQueue.push((*dmmIt).value);
			++dmmIt;
		}

		dmmIt = dmm_backward.search(currKey);
		while (dmmIt.isValid())
		{
			dmm_backward.erase((*dmmIt).key, (*dmmIt).value, (*dmmIt).context);
			hasPurged = true;
			toDeleteQueue.push((*dmmIt).value);
			++dmmIt;
		}
	}
	return hasPurged;
}

void IntelWeb::insertPrevalence(const string& item)
{
	DiskMultiMap::Iterator prevalIt = dmm_prevalence.search(item);
	string prevalStrValue = (*prevalIt).value;
	int prevalIntValue;
	if (prevalStrValue == "") 
	{
		prevalStrValue = "1";
		dmm_prevalence.erase(item, "", "");
		dmm_prevalence.insert(item, prevalStrValue, "");
	}
	else
	{
		prevalIntValue = stoi(prevalStrValue);
		prevalIntValue++;
		dmm_prevalence.erase(item, prevalStrValue, "");
		prevalStrValue = to_string(prevalIntValue);
		dmm_prevalence.insert(item, prevalStrValue, "");
	}
}





