#include <iostream>
#include <string>
#include "List/TLinkedList.h"
#include <list>
#include <algorithm>

#define NUM_LETTERS 26

using namespace std;

class Patron;

class Book
{
public:
	Book()
		: patron{nullptr}
	{}
	bool operator== (const Book& bk) const
	{
		return strcmp(title, bk.title) == 0;
	}

	char* title;
	Patron* patron;
private:
	ostream& print(ostream& stream) const;
	friend ostream& operator<< (ostream& out, const Book& bk)
	{
		return bk.print(out);
	}
	friend class CheckedOutBook;
	friend Patron;
	friend void includeBook();
	friend void checkOutBook();
	friend void returnBook();
};

class Author
{
public:
	Author() {}

	bool operator== (const Author& ar) const
	{
		return strcmp(name, ar.name) == 0;
	}
	char* name;
	TLinkedList<Book> books;

private:

	ostream& print(ostream& stream) const;
	friend ostream& operator<< (ostream& out, const Author& ar)
	{
		return ar.print(out);
	}

	friend void includeBook();
	friend void checkOutBook();
	friend void returnBook();
	friend class CheckedOutBook;
	friend Patron;
};

class CheckedOutBook
{
public:
	CheckedOutBook()
	{}

	CheckedOutBook(	TLinkedListNode<Author> * ar, TLinkedListNode<Book> * bk)
		: author{ar}
		, book{bk}
	{}

	bool operator== (const CheckedOutBook& bk) const
	{
		return strcmp(author->Info.name, bk.author->Info.name) == 0 &&
			strcmp(book->Info.title, bk.book->Info.title) == 0;
	}


	TLinkedListNode<Author>* author;
	TLinkedListNode<Book>* book;

	friend void checkOutBook();
	friend void returnBook();
	friend Patron;
};

class Patron
{
public:
	Patron()
	{}

	bool operator== (const Patron& pn) const
	{
		return strcmp(name, pn.name) == 0;
	}
	char* name;
	TLinkedList<CheckedOutBook> books;

private:
	ostream& print(ostream& stream) const;
	friend ostream& operator<<(ostream& out, const Patron& pn)
	{
		return pn.print(out);
	}
	friend void checkOutBook();
	friend void returnBook();
	friend Book;
};

TLinkedList<Author> catalog['Z' + 1];
TLinkedList<Patron> people['Z' + 1];

ostream& Author::print(ostream& stream) const
{
	stream << name << endl;
	TLinkedListNode<Book> * ref = books.GetHead();
	for (; ref != books.GetTail(); ref = ref->Next)
		stream << ref->Info;

	return stream;
}

ostream& Book::print(ostream& stream) const
{
	stream << "\t*" << title;
	if(patron)
		stream << " - checked out to " << patron->name;

	stream << endl;
	return stream;
}

int getIndexFromLetter(const char ch)
{
	return ch;
}

ostream& Patron::print(ostream& stream) const
{
	stream << name;
	if(!books.IsEmpty())
	{
		stream << " has the following books:\n";
		TLinkedListNode<CheckedOutBook>* bk = books.GetHead();
		for (; bk != nullptr; bk = bk->Next)
			stream << "\t* " << bk->Info.author->Info.name << ", " << bk->Info.book->Info.title << endl;
	}
	else
		stream << " has no books" << endl;

	return stream;
}

template<class T>
ostream& operator<< (ostream& out, const TLinkedList<T>& lst)
{
	for (TLinkedListNode<T> * ref = lst.GetHead(); ref != nullptr;
	ref = ref->Next)
	out << ref->Info; // overloaded <<
	return out;
}

char* getString(const char *msg)
{
	char s[82], i, * destin;

	cout << msg;
	cin.get(s, 80);
	while (cin.get(s[81]) && s[81] != '\n');
	destin = new char[strlen(s) + 1];
	for (i = 0; destin[i] = toupper(s[i]); ++i);
	return destin;
}

void status()
{
	int i;
	cout << "Library has the following books:\n\n";
	for (i = getIndexFromLetter('A'); i <= getIndexFromLetter('Z'); ++i)
		if (!catalog[i].IsEmpty())
			cout << catalog[i];
	cout << "\nThe following people are using the library:\n\n";
	for(i = getIndexFromLetter('A'); i <= getIndexFromLetter('Z'); ++i)
		if (!people[i].IsEmpty())
			cout << people[i];
}

void includeBook()
{
	Author newAuthor;
	Book newBook;
	newAuthor.name = getString("Enter author's name: ");
	newBook.title = getString("Enter the title of the book: ");
	TLinkedListNode<Author>* oldAuthor = catalog[getIndexFromLetter(newAuthor.name[0])].FindInList(newAuthor);
	if (!oldAuthor)
	{
		newAuthor.books.AddToHead(newBook);
		catalog[getIndexFromLetter(newAuthor.name[0])].AddToHead(newAuthor);
	}
	else
		(*oldAuthor).Info.books.AddToHead(newBook);
}

void checkoutBook()
{
	Patron patron;
	Author author;
	Book book;

	TLinkedListNode<Author>* authorRef;
	TLinkedListNode<Book>* bookRef;

	patron.name = getString("Enter patron's name: ");

	while (true)
	{
		author.name = getString("Enter author's name: ");
		authorRef = catalog[getIndexFromLetter(author.name[0])].FindInList(author);
		if (authorRef)
			cout << "No such author\n";
		else break;
	}
	while(true)
	{
		book.title = getString("Enter the title of the book: ");
		bookRef = authorRef->Info.books.FindInList(book);
		if (!bookRef)
			cout << "Misspelled title\n";
		else break;
	}
	TLinkedListNode<Patron> * patronRef;
	patronRef = people[getIndexFromLetter(patron.name[0])].FindInList(patron);
	CheckedOutBook checkedOutBook(authorRef, bookRef);

	if(!patronRef)
	{
		patron.books.AddToHead(checkedOutBook);
		people[getIndexFromLetter(patron.name[0])].AddToHead(patron);
		bookRef->Info.patron = &people[getIndexFromLetter(patron.name[0])].GetHead()->Info;
	}
	else
	{
		patronRef->Info.books.AddToHead(checkedOutBook);
		bookRef->Info.patron = &patronRef->Info;
	}
}

void returnBook()
{
	Patron patron;
	Book book;
	Author author;

	TLinkedListNode<Patron> * patronRef;
	TLinkedListNode<Book>* bookRef;
	TLinkedListNode<Author>* authorRef;

	while(true)
	{
		patron.name = getString("Enter patron's name: ");
		patronRef = people[getIndexFromLetter(patron.name[0])].FindInList(patron);
		if (!patronRef)
			cout << "No such patron\n";
		else
			break;
	}
	while(true)
	{
		author.name = getString("Enter author's name: ");
		authorRef = catalog[getIndexFromLetter(author.name[0])].FindInList(author);
		if (!authorRef)
			cout << "No such author\n";
		else break;
	}
	while(true)
	{
		book.title = getString("Enter the title of the book: ");
		bookRef = authorRef->Info.books.FindInList( book);
		if (!bookRef)
			cout << "Misspelled title\n";
		else break;
	}
	CheckedOutBook checkedOutBook(authorRef, bookRef);
	bookRef->Info.patron = nullptr;
	patronRef->Info.books.DeleteNode(checkedOutBook);
}

int menu()
{
	int option;
	cout << "\nEnter one of the following options:\n"
		<< "1. Include a book in the catalog\n2. Check out a book\n"
		<< "3. Return a book\n4. Status\n5. Exit\n"
		<< "Your option? ";

	cin >> option;

	cin.get(); // discard '\n';
	return option;
}