#include "IntelWeb.h"
#include "InteractionTuple.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <map>
#include <set>
#include <string>
#include <cstdlib>
using namespace std;

bool getLinesFromFile(string filename, vector<string>& data)
{
    ifstream inf(filename);
    if (!inf)
        return false;
    string line;
    while (getline(inf, line))
        data.push_back(line);
    return true;
}

bool createDB(string databasePrefix, unsigned int expectedMaxNumberOfItems)
{
    IntelWeb iw;
    if ( ! iw.createNew(databasePrefix, expectedMaxNumberOfItems))
    {
        cout << "Error: Cannot create database with prefix " << databasePrefix
             << " with " << expectedMaxNumberOfItems << " items expected." << endl;
    }
    return true;
}

bool ingest(string databasePrefix, string telemetryLogFile)
{
    IntelWeb iw;
    if ( ! iw.openExisting(databasePrefix))
    {
        cout << "Error: Cannot open existing database with prefix " << databasePrefix << endl;
        return false;
    }
    if ( ! iw.ingest(telemetryLogFile))
    {
        cout << "Error: Ingesting telemetry data from " << telemetryLogFile << " failed." << endl;
        return false;
    }
    return true;
}

bool crawl(string databasePrefix, string indicatorFile, unsigned int minGoodPrevalence, string resultsFile)
{
    if (minGoodPrevalence <= 1)
    {
        cout << "Error: minGoodPrevalence is " << minGoodPrevalence
             << ", but must be greater than 1" << endl;
        return false;
    }
    IntelWeb iw;
    if ( ! iw.openExisting(databasePrefix))
    {
        cout << "Error: Cannot open existing database with prefix " << databasePrefix << endl;
        return false;
    }

    vector<string> indicators;
    if ( ! getLinesFromFile(indicatorFile, indicators))
    {
        cout << "Error: Cannot open indicators file " << indicatorFile << endl;
        return false;
    }
    if (indicators.empty())
    {
        cout << "Error: Indicators file " << indicatorFile << " is empty." << endl;
        return false;
    }

    vector<string> badEntitiesFound;
    vector<InteractionTuple> badInteractions;

    iw.crawl(indicators, minGoodPrevalence, badEntitiesFound, badInteractions);

    ofstream resultf(resultsFile);
    if ( ! resultf)
    {
        cout << "Error: Cannot write results file " << resultsFile << endl;
        return false;
    }

    for (auto bad : badEntitiesFound)
        resultf << bad << endl;

    resultf << endl;

    for (const auto& i : badInteractions)
        resultf << i.context << ' ' << i.from << ' ' << i.to << endl;

    return true;
}

bool purge(string databasePrefix, string purgeFile)
{
    IntelWeb iw;
    if ( ! iw.openExisting(databasePrefix))
    {
        cout << "Error: Cannot open existing database with prefix " << databasePrefix << endl;
        return false;
    }

    vector<string> purgeList;

    if ( ! getLinesFromFile(purgeFile, purgeList))
    {
        cout << "Error: Cannot open purge file " << purgeFile << endl;
        return false;
    }

    for (auto itemToPurge : purgeList)
        iw.purge(itemToPurge);
    return true;
}

string generateCode(string machine, string& entity, const set<string>& badEntities)
{
    const string HTTP_STRING = "http://";

    bool isBad = (badEntities.find(entity) != badEntities.end());

    string badColor;
    if (entity.substr(0, HTTP_STRING.size()) == HTTP_STRING)
        badColor = "orange";
    else
    {
        entity = entity + " (" + machine + ")";
        badColor = "red";
    }
    return "nodes[\"" + entity + "\"] = {name: \"" + entity + "\", color: \"" +
            (isBad ? badColor : "green") + "\"};";
}

void writeGraph(ostream& htmlf, const vector<string>& results)
{
    set<string> badEntities;
    set<string> nodes;
    vector<string> edges;

    for (auto resultLine : results)
    {
        istringstream iss(resultLine);
        vector<string> tokens;
        string s;
        while (iss >> s)
            tokens.push_back(s);

        if (tokens.size() == 1)  // first part of result file: bad entities
            badEntities.insert(tokens[0]);
        else if (tokens.size() == 3)  // second part:  bad interactions
        {
            string machine = tokens[0];
            string from = tokens[1];
            string to = tokens[2];

              // Generate JavaScript statement like
              //   nodes["blah.exe"] = {name: "blah.exe (m1066)", color: "red"};
            nodes.insert(generateCode(machine, from, badEntities));
            nodes.insert(generateCode(machine, to, badEntities));

              // Generate JavaScript expression like
              //   {source: nodes["blah.exe"], target: nodes["www.a.com"], type: "defaultedge"},
            string edge = "{source: nodes[\"" + from + "\"], target: nodes[\"" + to + "\"], type: \"defaultedge\"},";
            edges.push_back(edge);
        } 
    }

    if ( ! edges.empty())
        edges.back().pop_back(); // remove final comma from last string

    htmlf << "var nodes = { };\n";
    for (const auto& n : nodes)
        htmlf << n << "\n";

    htmlf << "\nvar links = [\n";
    for (const auto& e : edges)
        htmlf << e << "\n";
    htmlf << "];\n";
}

bool convertToJavaScript(string resultsFile, string templateFile, string htmlFile)
{
    vector<string> results;
    if ( ! getLinesFromFile(resultsFile, results))
    {
        cout << "Error: Cannot open results file " << resultsFile << endl;
        return false;
    }

    vector<string> templateLines;
    if ( ! getLinesFromFile(templateFile, templateLines))
    {
        cout << "Error: Cannot open graph template file " << templateFile << endl;
        return false;
    }
    
    ofstream htmlf(htmlFile);
    if ( ! htmlf)
    {
        cout << "Error: Cannot write result graph file " << htmlFile << endl;
        return false;
    }

    const string PLACEHOLDER_STRING = "<PLACEHOLDER/>";
    for (auto line : templateLines)
    {
        if (line.find(PLACEHOLDER_STRING) == string::npos)
            htmlf << line << endl;
        else
            writeGraph(htmlf, results);
    }
    return true;
}

void printUsageAndExit()
{
    cout << "Usage:" << endl;
    cout << "  p4tester -b databasePrefix expectedNumberOfItems" << endl;
    cout << "  p4tester -i databasePrefix telemetryLogfile" << endl;
    cout << "  p4tester -s databasePrefix indicators minGoodPrevalence results" << endl;
    cout << "  p4tester -p databasePrefix purgeFile" << endl;
    cout << "  p4tester -w results graphtemplate.html resultgraph.html" << endl;
    exit(1);
}

int main(int argc, char *argv[])
{
    if (argc < 2  ||  argv[1][0] != '-')
        printUsageAndExit();
    switch (argv[1][1])
    {
      case 'b':
        if (argc != 4)
            printUsageAndExit();
        if ( ! createDB(argv[2], atoi(argv[3])) )
            return 1;
        break;
      case 'i':
        if (argc != 4)
            printUsageAndExit();
        if ( ! ingest(argv[2], argv[3]) )
            return 1;
        break;
      case 's':
        if (argc != 6)
            printUsageAndExit();
        if ( ! crawl(argv[2], argv[3], atoi(argv[4]), argv[5]) )
            return 1;
        break;
      case 'p':
        if (argc != 4)
            printUsageAndExit();
        if ( ! purge(argv[2], argv[3]) )
            return 1;
        break;
      case 'w':
        if (argc != 5)
            printUsageAndExit();
        if ( ! convertToJavaScript(argv[2], argv[3], argv[4]) )
            return 1;
        break;
      default:
        printUsageAndExit();
    }
}
