// newMap.cpp

#include "Map.h"

#include <iostream>
using namespace std;

Map::Map()										// Constructor			
{	
	head = nullptr;								// Initializes head, tail, and m_pairCounter (size of the linked list)
	tail = nullptr;
	m_pairCounter = 0;
}

Map::Map(const Map& other)						// Copy constructor		
{
	m_pairCounter = 0;							// Reset pairCounter, head, and tail
	head = nullptr;
	tail = nullptr;

	for (int k = 0; k < other.size(); k++)
	{
		KeyType tempKey;
		ValueType tempValue;

		other.get(k, tempKey, tempValue);		// Get keys and values from the other map
		insert(tempKey, tempValue);				// Insert those keys and values into the this map
	}
}

Map& Map::operator=(const Map& rhs)				// Assignment operator
{
	if (this != &rhs)							// Makes sure this map is not the same as the map on the right hand side (rhs)
	{
		Map temp = rhs;							// Make a copy of rhs
		swap(temp);								// Swap this map with the temp map, setting this map to rhs
	}
	return *this;
}

Map::~Map()										// Destructor
{
	Node* p;									// Creates two pointers and initializes one of them to the head	 
	Node* n;
	p = head;
	while (p != nullptr)						// Goes through the list and destructs each node until it reaches the end, which is a nullptr
	{
		n = p->m_next;							
		delete p;								
		p = n;									
	}
}

bool Map::empty() const 		
{
	return size() == 0;							// Checks to see whether or not the size of the list is 0
}

int Map::size() const 			
{
	return m_pairCounter;						// Returns the number of key/value pairs in the list
}

bool Map::insert(const KeyType& key, const ValueType& value)		// Inserts a new Node 
{
	if (contains(key))							// If a key is already in the map, stops and returns false
		return false;

	Node* newNode = new Node;					// Created a new Node
	newNode->m_key = key;						// Set the key in Node to the key passed through the parameter
	newNode->m_value = value;					// Set the value in Node to the value passed through the parameter
	newNode->m_next = nullptr;					
	newNode->m_prev = tail;
	m_pairCounter++;							// Increases size of list by 1
	if (size() == 1)
	{					
		head = newNode;							// If first node in a list, set head and tail to that node
		tail = newNode;
	}
	else
	{
		tail->m_next = newNode;					// Set the old tail's next pointer to the new node
		tail = newNode;							// Set tail to the new node
	}
	return true;
}

bool Map::update(const KeyType& key, const ValueType& value)		
{
	if (contains(key))							// Checks to see if this map contains the desired key
	{
		Node* p = locateNode(key);				
		p->m_value = value;						// Update that key's value with the new value
		return true;
	}

	return false;
}

bool Map::insertOrUpdate(const KeyType& key, const ValueType& value)		
{
	if (update(key, value))						// Attempts to update the value of a desired key
		return true;
	return insert(key, value);					// If can't update, attempts to insert the key and value
}

bool Map::erase(const KeyType& key)				
{
	if (size() > 0 && contains(key))			// Checks to make sure Map isn't empty and contains the desired key
	{
		Node* curr = locateNode(key);			// Create a pointer to the deired key

		if (curr != nullptr && key == curr->m_key)
		{
			Node* prev = curr->m_prev;
			Node* next = curr->m_next;

			if (size() == 1)					// Case for one-element list
			{
				head = nullptr;					// Sets head and tail to nullptr			 
				tail = nullptr;
			}
			else if (curr == head)				// Case for when the pointer points to the front/head
			{
				head = curr->m_next;			// Set head to the next node
				next->m_prev = nullptr;			// Set the next node's previous pointer to nullptr
			} 
			else if (curr == tail)				// Case for when the pointer points to the back/tail
			{
				tail = curr->m_prev;			// Set tail to previous node
				prev->m_next = nullptr;			// Set previous node's next pointer to nullptr
			}
			else								// Case for when the pointer points to an element in the middle of the list
			{
				prev->m_next = curr->m_next;	// Set previous node's next pointer to the next node
				next->m_prev = curr->m_prev;	// Set the next node's previous pointer to the previous node
			}

			delete curr;						// Delete the desired node
			m_pairCounter--;					// Because the size of the list by 1
			return true;
		}
	}
	return false;
}

bool Map::contains(const KeyType& key) const 	
{
	Node* p;
	for (p = head; p != nullptr && p->m_key != key; p = p->m_next);
	if (p != nullptr && p->m_key == key)
		return true;							// Returns true if desired key is found in the map
	return false;								// Otherwise, return false
}

bool Map::get(const KeyType& key, ValueType& value) const 			
{
	if (contains(key))
	{
		Node* p = locateNode(key);				// Finds the node with the desired key
		value = p->m_value;						// Gets and returns the value in that key
		return true;
	}
	return false;
}

bool Map::get(int i, KeyType& key, ValueType& value) const 			
{
	if (i >= 0 && i < size())
	{
		Node* p = head;
		if (p != nullptr)
		{
			for (int k = 0; k < i; k++)			// Goes through each node until the entered integer i is reached
			{	
				p = p->m_next;
			}

			key = p->m_key;						// Sets the key and value parameters to the gathered key and value from the map
			value = p->m_value;
			return true;
		}
	}
	return false;
}

void Map::swap(Map& other)							
{
	if (&other == this)
		return;

	Node* tempHead = head;						// Exchange the head pointers between the two maps
	head = other.head;
	other.head = tempHead;

	Node* tempTail = tail;						// Exchange the tail pointers between the two maps
	tail = other.tail;
	other.tail = tempTail;

	int tempPairCounter = m_pairCounter;		// Exchange the key/value pair counter between the two maps
	m_pairCounter = other.m_pairCounter;
	other.m_pairCounter = tempPairCounter;
}


Map::Node* Map::locateNode(const KeyType& key) const 			
{
    Node* p;
    for (p = head; p != nullptr && p->m_key != key; p = p->m_next);			// Finds and returns address of the desired key
    return p;
}


bool combine(const Map& m1, const Map& m2, Map& result)
{
	Map tempMap;								// Create a temp map to store the result

	bool returnValue = true;

	KeyType tempKey;		
	ValueType tempValue;

	for (int j = 0; j < m1.size(); j++)			// Inserts m1 into the temp map
	{
		m1.get(j, tempKey, tempValue);
		tempMap.insert(tempKey, tempValue);
	}

	for (int k = 0; k < m2.size(); k++)	
	{
		m2.get(k, tempKey, tempValue);			// Gets keys and values from m2 

		if (tempMap.contains(tempKey))			// Executes only if m1 contains a key from m2
		{
			ValueType tempValue2;
			tempMap.get(tempKey, tempValue2);

			if (tempValue == tempValue2)		// If the values are the same, do nothing
				continue;
			else
			{
				returnValue = false;			// If the values are different, don't include that key in temp map
				tempMap.erase(tempKey);			// Erase the node with that key from temp map
				continue;
			}
		}
		tempMap.insert(tempKey, tempValue);		// Otherwise, insert the key and value
	}

	result = tempMap;							// Set result to the temp map
	return returnValue;
}

void subtract(const Map& m1, const Map& m2, Map& result)
{
	Map tempMap;								// Create a temp map to store the result

	KeyType tempKey;		
	ValueType tempValue;

	for (int k = 0; k < m1.size(); k++)
	{
		m1.get(k, tempKey, tempValue);			// Get m1 keys and values
		if (! m2.contains(tempKey))				// If a key in m2 is not in m1, insert that key and value into the temp map
			tempMap.insert(tempKey, tempValue);
	}
	result = tempMap;							// Set result to the temp map
}

void Map::dump() const
{
	KeyType key;
	ValueType value;

	for (int k = 0; k < size(); k++)			// Prints out all the nodes in the linked list
	{
		get(k, key, value);
		cerr << "Key: " << key << " --- Value: " << value << endl;
	}
}












