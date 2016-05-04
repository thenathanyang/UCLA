// WeightMap.cpp

#include "WeightMap.h"

#include <iostream>
#include <string>
using namespace std;

WeightMap::WeightMap()
{
	Map m_weightMap;
}

bool WeightMap::enroll(std::string name, double startWeight)
{
	if (m_weightMap.contains(name) || size() >= DEFAULT_MAX_ITEMS || startWeight < 0)
		return false;
	m_weightMap.insert(name, startWeight);
	return true;
}

double WeightMap::weight(std::string name) const
{
	double weightNum = -1;
	m_weightMap.get(name, weightNum);
	return weightNum;
}

bool WeightMap::adjustWeight(std::string name, double amt)
{
	if (! m_weightMap.contains(name) || weight(name) + amt < 0)
		return false;

	double newWeight = weight(name) + amt;
	m_weightMap.update(name, newWeight);
	return true;
}

int WeightMap::size() const
{
	return m_weightMap.size();
}

void WeightMap::print() const
{
	string name;
	double weightNum;
	for (int k = 0; k < size(); k++)
	{
		m_weightMap.get(k, name, weightNum);
		cout << name << " " << weightNum << endl;
	}
}
