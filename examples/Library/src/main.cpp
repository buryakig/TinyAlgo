// This case study is a program that can be used in a small library to include new books
// in the library, to check out books to people, and to return them.
// As this program is a practice in the use of linked lists, almost everything is implemented in terms of such lists.But to make the program more interesting, it uses
// linked lists of linked lists that also contain cross - references

#include <iostream>
#include "Library.h"

using namespace std;

int main()
{
	while (true)
		switch (menu()) {
		case 1: includeBook(); break;
		case 2: checkoutBook(); break;
		case 3: returnBook(); break;
		case 4: status(); break;
		case 5: return 0;
		default: cout << "Wrong option, try again: ";
		}
}
