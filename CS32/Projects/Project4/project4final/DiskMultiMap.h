#ifndef DISKMULTIMAP_H_
#define DISKMULTIMAP_H_

#include <string>
#include <list>
#include "MultiMapTuple.h"
#include "BinaryFile.h"

class DiskMultiMap
{
public:

	class Iterator
	{
	public:
		Iterator();
		// You may add additional constructors
		Iterator(std::list<MultiMapTuple> tupleList);
		bool isValid() const;
		Iterator& operator++();
		MultiMapTuple operator*();

	private:
		// Your private member declarations will go here
		std::list<MultiMapTuple> m_tupleList;
	};

	DiskMultiMap();
	~DiskMultiMap();
	bool createNew(const std::string& filename, unsigned int numBuckets);
	bool openExisting(const std::string& filename);
	void close();
	bool insert(const std::string& key, const std::string& value, const std::string& context);
	Iterator search(const std::string& key);
	int erase(const std::string& key, const std::string& value, const std::string& context);

private:
	// Your private member declarations will go here
	BinaryFile bf;
	unsigned long hash(const std::string& str);

	struct DiskMultiMapHeader
	{
		int numBuckets;
		int numDelNodes;
		BinaryFile::Offset firstDelNode;
		BinaryFile::Offset lastDelNode;
	};

	struct DiskMultiMapBucket
	{
		BinaryFile::Offset bucketPos;
		int numNodes;
		BinaryFile::Offset firstNodePos;
		BinaryFile::Offset lastNodePos;
	};

	struct DiskMultiMapNode
	{
		char key[121];
		char value[121];
		char context[121];
		BinaryFile::Offset nodePos;
		BinaryFile::Offset nextNode;
	};

	DiskMultiMapHeader header;

	DiskMultiMapBucket readBucket(BinaryFile::Offset pos);
	DiskMultiMapNode readNode(BinaryFile::Offset pos);

	bool updateHeader();
	bool updateBucket(DiskMultiMapBucket bucket);
	bool updateNode(DiskMultiMapNode node);
};

#endif // DISKMULTIMAP_H_
