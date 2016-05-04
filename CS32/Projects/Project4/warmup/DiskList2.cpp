#include <iostream>
#include <cstring>
#include "BinaryFile.h"

// sizeof returns in bytes
// return nullptr if something cant be found

class DiskList
{
  public:
    DiskList(const std::string& filename);
    bool push_front(const char* data);
    bool remove(const char* data);
    void printAll();
  private:
  	struct DiskNode
  	{
  		char info[256];
  		BinaryFile::Offset next, location;
  	};
  	BinaryFile bf;
  	int fileLen;
  	BinaryFile::Offset offsetFirst, offsetLast;
  	std::string m_filename;


  	void newBinaryFile();

};

DiskList::DiskList(const std::string& filename)
{
	bf.createNew(filename);
	offsetFirst = 0;
	offsetLast = 0;
	fileLen = 0;
	m_filename = filename;
	
}

void DiskList::newBinaryFile()
{
	bf.createNew(m_filename);
	offsetFirst = 0;
	offsetLast = 0;
	fileLen = 0;
}

bool DiskList::push_front(const char* data)
{
	if (strlen(data) >= 256) return false;

	DiskNode newNode;
	strcpy(newNode.info, data);
	newNode.next = offsetFirst;
	offsetFirst = fileLen;

	if (fileLen > 0)
	{
		DiskNode lastNode;
		bf.read(lastNode, offsetLast);
		lastNode.next = offsetFirst;
		bf.write(lastNode, offsetLast);
	}

	newNode.location = fileLen;
	bf.write(newNode, fileLen);
	fileLen += sizeof(newNode);
	return true;

	// DiskNode newNode;
	// strcpy(newNode.data, data);
	// BinaryFile::Offset nodeOffset = sizeof(data);

	// char oldData[fileLen];
	// bf.read(oldData, fileLen, 0);
	// bf.write(newNode, 0);
	// //bf.write(oldData, fileLen, sizeof(DiskNode) + 1);
	// fileLen += sizeof(newNode) + 1;

	// return true;

	// DiskNode newNode;
	// strcpy(newNode.data, data);
	// newNode.next = offsetFirstNode;
	// offsetFirstNode = fileLen;
	// bf.write(newNode, fileLen);
	// fileLen += sizeof(newNode);



}

// bool DiskList::remove(const char* data)
// {
// 	int pos = 0;
// 	bool isDeleted = false;
// 	while (pos != fileLen)
// 	{
// 		char arr[257];
// 		int arrLen = 0;
		
// 		do
// 		{
// 			bf.read(arr + arrLen, 1, pos + arrLen);
// 		}
// 		while (arr[arrLen++] != '\0');		// -1
		
// 		//cout << arr << endl;

// 		if (!strcmp(arr, data))
// 		{
// 			char newArr[fileLen - pos - arrLen];
// 			bf.read(newArr, fileLen - pos - arrLen, pos + arrLen);
// 			bf.write(newArr, fileLen - pos - arrLen, pos);
// 			fileLen -= arrLen;
// 		}
// 		else pos += arrLen;
// 	}
// }

bool DiskList::remove(const char* data)
{
	bool hasRemoved = false;

	DiskNode curr;
	bf.read(curr, offsetFirst);

	if (curr.location == offsetLast && !strcmp(curr.info, data))
	{
		newBinaryFile();
		hasRemoved = true;
	}

	while (curr.next != offsetFirst)
	{
		DiskNode nextDisk;
		bf.read(nextDisk, curr.next);
		if (!strcmp(nextDisk.info, data))
		{
			curr.next = nextDisk.next;
			bf.write(curr, curr.location);
			//fileLen -= sizeof(nextDisk);
			bf.read(curr, curr.location);
			hasRemoved = true;
		}
		else
			bf.read(curr, curr.next);
	}

	if (curr.next == offsetFirst)
	{
		DiskNode nextDisk;
		bf.read(nextDisk, curr.next);
		if (!strcmp(nextDisk.info, data))
		{
			curr.next = nextDisk.next;
			bf.write(curr, curr.location);
			offsetFirst = nextDisk.next;
			hasRemoved = true;
		}
	}

	return hasRemoved;
}

void DiskList::printAll()
{
	if (fileLen <= 0) return;
	DiskNode printNode;
	bf.read(printNode, offsetFirst);
	while (printNode.next != offsetFirst)
	{
		cout << printNode.info << endl;
		bf.read(printNode, printNode.next);
	}
	cout << printNode.info << endl;
}

int main()
{
	DiskList x("mylist.dat");
	x.push_front("Fred");
	x.push_front("Lucy");
	x.push_front("Ethel");
	//cout << x.remove("Lucy") << endl;
	x.push_front("Ethel");
	x.push_front("Lucy");
	x.push_front("Lucy");
	x.push_front("Fred");
	x.push_front("Ethel");
	x.push_front("Ricky");
	x.push_front("Lucy");
	x.remove("Lucy");
	// x.push_front("Fred");
	// x.push_front("Ricky");
	x.printAll();  // writes, one per line
	  // Ricky  Fred  Ricky  Ethel  Fred  Ethel  Ethel  Fred
}

