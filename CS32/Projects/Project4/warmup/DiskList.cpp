#include <iostream>
#include <cstring>
#include "BinaryFile.h"

class DiskList
{
  public:
    DiskList(const std::string& filename);
    bool push_front(const char* data);
    bool remove(const char* data);
    void printAll();
  private:
  	BinaryFile bf;
  	int fileLen;

};

DiskList::DiskList(const std::string& filename)
{
	bf.createNew(filename);
	fileLen = 0;
}

bool DiskList::push_front(const char* data)
{
	if (strlen(data) >= 256) return false;

	char arr[fileLen];
	bf.read(arr, fileLen, 0);
	bf.write(data, strlen(data) + 1, 0);
	bf.write(arr, fileLen, strlen(data) + 1);
	fileLen += strlen(data) + 1;

	return true;
}

bool DiskList::remove(const char* data)
{
	int pos = 0;
	bool isDeleted = false;
	while (pos != fileLen)
	{
		char arr[257];
		int arrLen = 0;
		
		do
		{
			bf.read(arr + arrLen, 1, pos + arrLen);
		}
		while (arr[arrLen++] != '\0');		//-1
		
		//cout << arr << endl;

		if (!strcmp(arr, data))
		{
			char newArr[fileLen - pos - arrLen];
			bf.read(newArr, fileLen - pos - arrLen, pos + arrLen);
			bf.write(newArr, fileLen - pos - arrLen, pos);
			fileLen -= arrLen;
		}
		else pos += arrLen;
	}

	// int pos = 0;
	// bool isDeleted = false;
	// while (pos != fileLen)
	// {
	// 	char arr[257];
	// 	int arrLen = 0;
		
	// 	do
	// 	{
	// 		bf.read(arr + arrLen, 1, pos + arrLen);
	// 	}
	// 	while (arr[arrLen++] != '\0');		//-1
	// 	pos += arrLen;
	// 	isDeleted = false;
	// 	//cout << arr << endl;

	// 	if (!strcmp(arr, data))
	// 	{
	// 		char newArr[fileLen - pos];
	// 		bf.read(newArr, fileLen - pos, pos);
	// 		bf.write(newArr, fileLen - pos, pos - arrLen);
	// 		fileLen -= arrLen;
	// 		isDeleted = true;
	// 	}

	// }
}

void DiskList::printAll()
{
	int pos = 0;
	while (pos != fileLen)
	{
		char arr[257];
		int arrLen = 0;
		do
		{
			bf.read(arr + arrLen, 1, pos + arrLen);
		}
		while (arr[arrLen++] != '\0');		//-1
		pos += arrLen;
		cout << arr << endl;
	}
}

int main()
{
	DiskList x("mylist.dat");
	x.push_front("Fred");
	x.push_front("Lucy");
	x.push_front("Ethel");
	x.push_front("Ethel");
	x.push_front("Lucy");
	x.push_front("Lucy");
	x.push_front("Fred");
	x.push_front("Ethel");
	x.push_front("Ricky");
	x.push_front("Lucy");
	x.remove("Lucy");
	x.push_front("Fred");
	x.push_front("Ricky");
	x.printAll();  // writes, one per line
	  // Ricky  Fred  Ricky  Ethel  Fred  Ethel  Ethel  Fred
}
