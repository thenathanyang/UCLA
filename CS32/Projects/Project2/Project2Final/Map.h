// Map.h

#ifndef MAP_INCLUDED
#define MAP_INCLUDED

#include <string>

typedef std::string KeyType;
typedef double ValueType;

// typedef int KeyType;
// typedef std::string ValueType;

class Map
{
  public:
    Map();      // Create an empty map (i.e., one with no key/value pairs)

    ~Map();     // Destructor

    Map(const Map& other);    // Copy constructor

    Map& operator=(const Map& rhs);   // Assignment operator

    bool empty() const;  // Return true if the map is empty, otherwise false.

    int size() const;    // Return the number of key/value pairs in the map.

    bool insert(const KeyType& key, const ValueType& value);
      // If key is not equal to any key currently in the map, and if the
      // key/value pair can be added to the map, then do so and return true.
      // Otherwise, make no change to the map and return false (indicating
      // that either the key is already in the map, or the map has a fixed
      // capacity and is full.

    bool update(const KeyType& key, const ValueType& value);
      // If key is equal to a key currently in the map, then make that key no
      // longer map to the value it currently maps to, but instead map to
      // the value of the second parameter; return true in this case.
      // Otherwise, make no change to the map and return false.

    bool insertOrUpdate(const KeyType& key, const ValueType& value);
      // If key is equal to a key currently in the map, then make that key no
      // longer map to the value it currently maps to, but instead map to
      // the value of the second parameter; return true in this case.
      // If key is not equal to any key currently in the map, and if the
      // key/value pair can be added to the map, then do so and return true.
      // Otherwise, make no change to the map and return false.

    bool erase(const KeyType& key);
      // If key is equal to a key currently in the map, remove the key/value
      // pair with that key from the map and return true.  Otherwise, make
      // no change to the map and return false.
     
    bool contains(const KeyType& key) const;
      // Return true if key is equal to a key currently in the map, otherwise
      // false.
     
    bool get(const KeyType& key, ValueType& value) const;
      // If key is equal to a key currently in the map, set value to the
      // value in the map that that key maps to and return true.  Otherwise,
      // make no change to the value parameter of this function and return
      // false.
     
    bool get(int i, KeyType& key, ValueType& value) const;
      // If 0 <= i < size(), copy into the key and value parameters the
      // key and value of one of the key/value pairs in the map and return
      // true.  Otherwise, leave the key and value parameters unchanged and
      // return false.

    void swap(Map& other);
      // Exchange the contents of this map with the other one.

    void dump() const;
      // Prints out all the key/value pairs in the map

  private:
    struct Node
    {
        KeyType m_key;
        ValueType m_value;
        Node* m_next;
        Node* m_prev;
    };
    Node* head;
    Node* tail;
    int m_pairCounter; 
    Node* locateNode(const KeyType& key) const;         // Returns a pointer to a desired key
};


bool combine(const Map& m1, const Map& m2, Map& result);        // Combines two maps and puts it into result
void subtract(const Map& m1, const Map& m2, Map& result);       // Subtracts two maps (contains pairs in m1 whose keys don't appear in m2)


#endif // MAP_INCLUDED

