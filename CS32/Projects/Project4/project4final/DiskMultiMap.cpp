// DiskMultiMap.cpp

// CHECK TO SEE WHEN STUFF FAILS

#include <string>
#include <list>
#include <vector>
#include <cstdlib>
#include <cstring>
#include "DiskMultiMap.h"
#include "MultiMapTuple.h"
using namespace std;

DiskMultiMap::DiskMultiMap() {}

DiskMultiMap::~DiskMultiMap()
{
	close();
}

bool DiskMultiMap::createNew(const std::string& filename, unsigned int numBuckets)
{
	bf.close();

	if (!bf.createNew(filename)) return false;

	header.numBuckets = numBuckets;
	header.numDelNodes = 0;
	header.firstDelNode = 0;
	header.lastDelNode = 0;
	if (!bf.write(header, 0)) 
	{
		//cout << "Failed to write header" << endl;
		return false;
	}

	DiskMultiMapBucket bucket;
	bucket.numNodes = 0;
	bucket.firstNodePos = 0;
	bucket.lastNodePos = 0;
	for (int k = 0; k < numBuckets; k++)
	{
		bucket.bucketPos = bf.fileLength();
		if (!bf.write(bucket, bf.fileLength())) return false;
	}

	//cout << "createNew: size of header is: " << sizeof(header) << endl;
	//cout << "createNew: bucket info: " << header.numBuckets << "*" << sizeof(bucket) << endl;
	// cout << bf.fileLength() << endl;
	return true;
}

bool DiskMultiMap::openExisting(const std::string& filename)
{
	bf.close();

	// bf.openExisting(filename);
	// bf.read(header, 0);
	// cout << filename << endl;
	// cout << "Number of Buckets: " <<  header.numBuckets << endl;
	// cout << "Number of deleted nodes: " << header.numDelNodes << endl;
	// cout <<  "First deleted node: " << header.firstDelNode << endl;
	// cout << "Last deleted node: " << header.lastDelNode << endl;
	// cout << "----------------" << endl;

	return bf.openExisting(filename) && bf.read(header, 0);
}

void DiskMultiMap::close()
{
	bf.close();
}

bool DiskMultiMap::insert(const std::string& key, const std::string& value, const std::string& context)
{
	// cout << endl;
	// cout << "-----" << endl;
	// cout << "HIHIHI Key: " << key << " Value: " << value << " Conext: " << context << endl;
	// cout << "-----" << endl;
	// cout << endl;
	
	if (key.size() > 120 || value.size() > 120 || context.size() > 120) return false;

	BinaryFile::Offset hashKey = hash(key.c_str()) % header.numBuckets;
	DiskMultiMapBucket bucket = readBucket(hashKey);

	if (header.numBuckets == 0) return false;		// CORRECT?!?!?

	DiskMultiMapNode node;
	strcpy(node.key, key.c_str());
	strcpy(node.value, value.c_str());
	strcpy(node.context, context.c_str());

	//if (header.numInDelPos > 0) node.nodePos = header.delPos[header.numInDelPos-1];
	if (header.numDelNodes > 0) 
	{
		node.nodePos = header.firstDelNode;
		// cout << node.nodePos << endl;
		DiskMultiMapNode oldFirstDelNode = readNode(header.firstDelNode);
		header.firstDelNode = oldFirstDelNode.nextNode;		// CORRECT?!?!?!
		header.numDelNodes--;
	}
	else node.nodePos = bf.fileLength();

	//cout << "NEW INSERT POSITION: " << node.nodePos << endl;

	if (bucket.numNodes == 0) bucket.firstNodePos = node.nodePos;
	else
	{	
		DiskMultiMapNode prevNode = readNode(bucket.lastNodePos);
		prevNode.nextNode = node.nodePos;
		updateNode(prevNode);
	}

	node.nextNode = 0;
	bucket.lastNodePos = node.nodePos;

	//cout << "bucket.numNodes before " << bucket.numNodes << endl;

	bf.write(node, node.nodePos);
	bucket.numNodes++;
	updateBucket(bucket);
	updateHeader();

	//cout << "bucket.numNodes after " << bucket.numNodes << endl;

	return true;
}

DiskMultiMap::Iterator DiskMultiMap::search(const std::string& key)
{
	Iterator it;
	MultiMapTuple tuple;
	std::list<MultiMapTuple> tupleList;

	BinaryFile::Offset hashKey = hash(key.c_str()) % header.numBuckets;
	DiskMultiMapBucket bucket = readBucket(hashKey);

	if (bucket.numNodes == 0) return Iterator();

	DiskMultiMapNode currNode = readNode(bucket.firstNodePos);
	while (currNode.nodePos != bucket.lastNodePos)
	{
		//cout << "search " << endl;
		if (!strcmp(currNode.key, key.c_str()))
		{
			tuple.key = currNode.key;
			tuple.value = currNode.value;
			tuple.context = currNode.context;
			tupleList.push_back(tuple);
			//cout << "FOUND" << endl;
		}

		currNode = readNode(currNode.nextNode);
	}
	
	if (currNode.nodePos == bucket.lastNodePos)
	{
		if (!strcmp(currNode.key, key.c_str()))
		{
			tuple.key = currNode.key;
			tuple.value = currNode.value;
			tuple.context = currNode.context;
			tupleList.push_back(tuple);
			//cout << "FOUND" << endl;
		}
	}

	it = Iterator(tupleList);
	return it;
}

int DiskMultiMap::erase(const std::string& key, const std::string& value, const std::string& context)
{
	int numRemoved = 0;
	BinaryFile::Offset hashKey = hash(key.c_str()) % header.numBuckets;
	DiskMultiMapBucket bucket = readBucket(hashKey);
	//bf.read(currNode, bucket.firstNodePos);

	if (bucket.numNodes == 0) return 0;

	DiskMultiMapNode currNode = readNode(bucket.firstNodePos);
	std::vector<DiskMultiMapNode> bucketNodes;

	for (int k = 0; k < bucket.numNodes; k++)
	{
		bucketNodes.push_back(currNode);
		currNode = readNode(currNode.nextNode);
	}

	//cout << "bucketNodes size: " << bucketNodes.size() << endl;

	for (std::vector<DiskMultiMapNode>::iterator it = bucketNodes.begin(); it != bucketNodes.end(); ++it)
		if (!strcmp((*it).key, key.c_str()) && !strcmp((*it).value, value.c_str()) && !strcmp((*it).context, context.c_str()))
		{
			// cout << (*it).key << " " << (*it).value << " " << (*it).context << endl;
			bucket.numNodes--;
			header.numDelNodes++;
			//cout << "Erase " << header.numDelNodes << endl;
			if (header.numDelNodes == 1)
			{
				//cout << "erase 1 node" << endl;
				header.firstDelNode = (*it).nodePos;
				header.lastDelNode = (*it).nodePos;
				(*it).nextNode = 0;
			}
			else 
			{	
				//cout << "erase more than 1" << endl;
				DiskMultiMapNode prevNode = readNode(header.lastDelNode);
				prevNode.nextNode = (*it).nodePos;
				updateNode(prevNode);
				header.lastDelNode = (*it).nodePos;
			}
			//cout << "NEW DELETED POSITION: " << header.lastDelNode << endl;
			numRemoved++;
			it = --bucketNodes.erase(it);	
		}

	//cout << "New bucketNodes size: " << bucketNodes.size() << endl;

	if (bucketNodes.size() > 0)
	{
		for (int k = 0; k < bucketNodes.size() - 1; k++)	// Relink the nodes
		{
			if (k == 0) bucket.firstNodePos = bucketNodes[k].nodePos;
			bucketNodes[k].nextNode = bucketNodes[k+1].nodePos;
		}
		// bucketNodes[bucketNodes.size() - 1].nextNode = 0;
		if (bucketNodes.size() - 1 == 0) bucket.firstNodePos = bucketNodes[bucketNodes.size() - 1].nodePos;
		bucketNodes[bucketNodes.size() - 1].nextNode = 0;
		bucket.lastNodePos = bucketNodes[bucketNodes.size() - 1].nodePos;
	}

	for (int j = 0; j < bucketNodes.size(); j++)		// Update the nodes back into BinaryFile
		updateNode(bucketNodes[j]);

	updateBucket(bucket);
	updateHeader();
	return numRemoved;
}

unsigned long DiskMultiMap::hash(const std::string& str)
{
    unsigned long hash = 5381;

    for (int k = 0; k < str.size(); k++)
        hash = ((hash << 5) + hash) + str[k]; /* hash * 33 + c */

    return hash;
}

DiskMultiMap::DiskMultiMapBucket DiskMultiMap::readBucket(BinaryFile::Offset pos)
{
	DiskMultiMapBucket bucket;
	BinaryFile::Offset bucketOffset = sizeof(DiskMultiMapHeader) + (pos * sizeof(bucket));
	bf.read(bucket, bucketOffset);
	return bucket;
}

DiskMultiMap::DiskMultiMapNode DiskMultiMap::readNode(BinaryFile::Offset pos)
{
	DiskMultiMapNode node;
	BinaryFile::Offset nodeOffset = pos;
	bf.read(node, nodeOffset);
	return node;
}

bool DiskMultiMap::updateHeader()
{
	return bf.write(header, 0);
}

bool DiskMultiMap::updateBucket(DiskMultiMapBucket bucket)
{
	return bf.write(bucket, bucket.bucketPos);
}

bool DiskMultiMap::updateNode(DiskMultiMapNode node)
{
	return bf.write(node, node.nodePos);
}

DiskMultiMap::Iterator::Iterator() {}

DiskMultiMap::Iterator::Iterator(std::list<MultiMapTuple> tupleList)
{
	m_tupleList = tupleList;
}

bool DiskMultiMap::Iterator::isValid() const
{
	return m_tupleList.size();
}

DiskMultiMap::Iterator& DiskMultiMap::Iterator::operator++()
{
	if (!isValid()) return *this;
	m_tupleList.pop_front();
	return *this;
}

MultiMapTuple DiskMultiMap::Iterator::operator*()
{
	MultiMapTuple tuple;	
	if (!isValid()) return tuple;
	tuple = m_tupleList.front();
	return tuple;
}
