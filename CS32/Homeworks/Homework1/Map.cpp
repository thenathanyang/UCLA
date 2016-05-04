// Map.cpp

#include "Map.h"

#include <iostream>
using namespace std;

Map::Map()
{	
	m_pairCounter = 0;
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
	if (size() >= DEFAULT_MAX_ITEMS || contains(key))
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

bool Map::insertOrUpdate(const KeyType& key, const ValueType& value)
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

	int largerSize = size();
	if (other.size() > size())
		largerSize = other.size();

	for (int k = 0; k < largerSize; k++)
	{
		mapPair tempPair = m_pair[k];
		m_pair[k] = other.m_pair[k];
		other.m_pair[k] = tempPair;
	}

	int tempPairCounter = m_pairCounter;
	m_pairCounter = other.m_pairCounter;
	other.m_pairCounter = tempPairCounter;
}
