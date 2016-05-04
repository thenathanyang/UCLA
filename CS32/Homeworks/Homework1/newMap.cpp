// newMap.cpp

#include "newMap.h"

#include <iostream>
#include <cstdlib>
using namespace std;

Map::Map(int arraySize)
{	
	if (arraySize < 0)
	{
		cout << "Number of key/value pairs cannot be negative." << endl;
		exit(1);
	}
	m_pairCounter = 0;
	m_arraySize = arraySize;
	m_pair = new mapPair[arraySize];
}

Map::Map(const Map& other)						// Copy constructor
{
	m_pairCounter = other.m_pairCounter;
	m_arraySize = other.m_arraySize;
	m_pair = new mapPair[other.m_arraySize];
	for (int k = 0; k < other.size(); k++)
		m_pair[k] = other.m_pair[k];
}

Map& Map::operator=(const Map& rhs)				// Assignment operator
{
	if (this != &rhs)
	{
		delete [] m_pair;
		m_pairCounter = rhs.m_pairCounter;
		m_arraySize = rhs.m_arraySize;
		m_pair = new mapPair[rhs.m_arraySize];
		for (int k = 0; k < rhs.size(); k++)
			m_pair[k] = rhs.m_pair[k];

		// Map temp = rhs;
		// swap(temp);
	}
	return *this;
}

Map::~Map()										// Destructor
{
	delete [] m_pair;
}

bool Map::empty() const
{
	if (size() == 0)
		return true;
	return false;
}

int Map::size() const
{
	return m_pairCounter;
}

bool Map::insert(const KeyType& key, const ValueType& value)
{
	if (size() >= m_arraySize || contains(key))
		return false;

	m_pair[size()].key = key;
	m_pair[size()].value = value;
	m_pairCounter++;

	return true;
}

bool Map::update(const KeyType& key, const ValueType& value)
{
	for (int k = 0; k < size(); k++)
		if (key == m_pair[k].key)
		{
			m_pair[k].value = value;
			return true;
		}

	return false;
}

bool Map::insertOrUpdate(const KeyType& key, const ValueType& value)		// CORRECT?!?!
{
	if (update(key, value))
		return true;
	return insert(key, value);
}

bool Map::erase(const KeyType& key)
{
	for (int k = 0; k < size(); k++)
		if (key == m_pair[k].key)
		{
			m_pair[k] = m_pair[size()-1];
			m_pairCounter--;
			return true;
		}
	return false;
}

bool Map::contains(const KeyType& key) const
{
	for (int k = 0; k < size(); k++)
		if (key == m_pair[k].key)
			return true;
	return false;
}

bool Map::get(const KeyType& key, ValueType& value) const
{
	for (int k = 0; k < size(); k++)
		if (key == m_pair[k].key)
		{
			value = m_pair[k].value;
			return true;
		}

	return false;
}

bool Map::get(int i, KeyType& key, ValueType& value) const
{
	if (i >= 0 && i < size())
	{
		key = m_pair[i].key;
		value = m_pair[i].value;
		return true;
	}

	return false;
}

void Map::swap(Map& other)
{
	if (&other == this)
		return;

	mapPair* tempPair = m_pair;
	m_pair = other.m_pair;
	other.m_pair = tempPair;

	int tempPairCounter = m_pairCounter;
	m_pairCounter = other.m_pairCounter;
	other.m_pairCounter = tempPairCounter;

	int tempArraySize = m_arraySize;
	m_arraySize = other.m_arraySize;
	other.m_arraySize = tempArraySize;
}
