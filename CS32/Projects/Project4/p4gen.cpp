#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cstring>
#include <cstdio>
#include <cstdlib>
#include <ctime>
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

string generateFilename()
{
    const int MAX_FILENAME_LEN = 4;
    const int length = rand() % MAX_FILENAME_LEN + 1;

    string filename;
    for (int i = 0; i < length; i++)
        filename += static_cast<char>('a' + rand() % 26);

    return filename;
}

string randomPath()
{
    int depthCategory = rand() % 10;
    int depth = depthCategory < 5 ? 1 :  // 50% depth 1
                depthCategory < 8 ? 2 :  // 30% depth 2
                depthCategory < 9 ? 3 :  // 10% depth 3
                         3 + rand() % 4; // 2.5% each 3, 4, 5, 6

    string path;
    while (depth-- > 0)
        path += "/" + generateFilename();
    return path + "/";
}

string generateSource(const vector<string>& goodSources)
{
    size_t r = rand() % goodSources.size();
    string src = goodSources[r];

    if (src.find("http://") != string::npos)
        return src + randomPath();  // URL
    else
        return src;         // file name
}

string generateTarget(const vector<string>& goodSources)
{
    size_t r = rand() % goodSources.size();
    for (size_t k = 0; k < goodSources.size(); k++)
    {
        if (goodSources[r].find("http://") != string::npos)
            return goodSources[r] + randomPath();
        if (++r == goodSources.size())
            r = 0;
    } 
    return "http://ucla.edu" + randomPath();
}

string generateMachineName(int numMachines)
{
    return "m" + to_string(rand() % numMachines);
}

void generateEvent(const vector<string>& goodSources, int numMachines, vector<string>& outputLogs)
{
    string baseExeName = generateFilename() + ".exe";
    string line;
    vector<string> exeFiles;

    string curMachine = generateMachineName(numMachines);

    line = curMachine + " " + generateSource(goodSources) + " " + baseExeName;
    exeFiles.push_back(baseExeName);    

    outputLogs.push_back(line); // download event

    int depth = rand() % 5;
    if (rand() % 10 == 0)
        depth *= 2;
    while (depth-- > 0)
    {
        string newFile = generateFilename() + ".exe";
        line = curMachine + " " + exeFiles[rand() % exeFiles.size()] + " " + newFile;           // create event
        exeFiles.push_back(newFile);

        outputLogs.push_back(line);

        if (rand() % 2 == 0)
        {
            line = curMachine + " " + exeFiles[rand() % exeFiles.size()] + " " + generateTarget(goodSources);
            outputLogs.push_back(line);

        }
    }
}

bool generateLogs(const vector<string>& goodSources, const vector<string>& maliciousLogs, int numEvents, int numMachines, const string& outputfile)
{
    ofstream outf(outputfile);
    if ( ! outf)
        return false;

    if (maliciousLogs.size() == 0)
        return false;

    vector<string> outputLogs;
    for (int i = 0; i < numEvents; i++)
        generateEvent(goodSources, numMachines, outputLogs);

    size_t curMaliciousLogLine = 0;
    size_t odds = outputLogs.size() / maliciousLogs.size();
    if (odds == 0)
        odds = 1;

    for (size_t c = 0; c < outputLogs.size(); c++)
    {
        if (rand() % odds == 0 && curMaliciousLogLine < maliciousLogs.size())
            outf << maliciousLogs[curMaliciousLogLine++] << endl;
        outf << outputLogs[c] << endl;
    }

    // drain malicious logs
    while (curMaliciousLogLine < maliciousLogs.size())
        outf << maliciousLogs[curMaliciousLogLine++] << endl;

    return true;
}

int main(int argc, char *argv[])
{
    srand(time(NULL));

    if (argc != 6)
    {
        cout << "Usage: p4gen sources.txt malicious.txt numEvents numMachines outputlog.txt" << endl;
        return 1;
    }

    vector<string> goodSources;

    if ( ! getLinesFromFile(argv[1], goodSources))
    {
        cout << "Error: can't open source file: " <<  argv[1] << endl;
        return 1;
    }

    vector<string> maliciousLogs;

    if ( ! getLinesFromFile(argv[2], maliciousLogs))
    {
        cout << "Error: can't open source file: " << argv[2] << endl;
        return 1;
    }

    int numEvents = atoi(argv[3]);
    if (numEvents <= 0)
    {
        cout << "Error: invalid number of events: " << argv[3] << endl;
        return 1;
    }

    int numMachines = atoi(argv[4]);
    if (numMachines <= 0)
    {
        cout << "Error: invalid number of machines: " << argv[4] << endl;
        return 1;
    }

    if ( ! generateLogs(goodSources, maliciousLogs, numEvents,numMachines, argv[5]))
    {
        cout << "Error: problem generating logs" << endl;
        return 1;
    }
}