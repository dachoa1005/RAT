#include <iostream>
#include <cstdio>
#include <string>
#include <cstring>

using namespace std;

int main(int argc, char const *argv[])
{
	char str[50] = "GeeksForGeeks is for programming geeks.";
	printf("Before memset(): %s\n", str);

	// Fill 8 characters starting from str[13] with '.'
	memset(&str,0,sizeof(str));

	cout << "After memset(): " << str << "a" << endl;
	cout << sizeof(str) << endl;
	return 0;
}
