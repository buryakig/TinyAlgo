#include <iostream>
#include <string>
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
	list<Book> books;

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

	CheckedOutBook(	list<Author>::iterator ar, list<Book>::iterator bk)
		: author{ar}
		, book{bk}
	{}

	bool operator== (const CheckedOutBook& bk) const
	{
		return strcmp(author->name, bk.author->name) == 0 &&
			strcmp(book->title, bk.book->title) == 0;
	}


	list<Author>::iterator author;
	list<Book>::iterator book;

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
	list<CheckedOutBook> books;

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

list<Author> catalog['Z' + 1];
list<Patron> people['Z' + 1];

ostream& Author::print(ostream& stream) const
{
	stream << name << endl;
	list<Book>::const_iterator ref = books.begin();
	for (; ref != books.end(); ++ref)
		stream << *ref;

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
	if(!books.empty())
	{
		stream << " has the following books:\n";
		list<CheckedOutBook>::const_iterator bk = books.begin();
		for (; bk != books.end(); ++bk)
			stream << "\t* " << bk->author->name << ", " << bk->book->title << endl;
	}
	else
		stream << " has no books" << endl;

	return stream;
}

template<class T>
ostream& operator<< (ostream& out, const list<T>& lst)
{
	for (typename list<T>::const_iterator ref = lst.begin(); ref != lst.end();
	ref++)
	out << *ref; // overloaded <<
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
	register int i;
	cout << "Library has the following books:\n\n";
	for (i = getIndexFromLetter('A'); i <= getIndexFromLetter('Z'); ++i)
		if (!catalog[i].empty())
			cout << catalog[i];
	cout << "\nThe following people are using the library:\n\n";
	for(i = getIndexFromLetter('A'); i <= getIndexFromLetter('Z'); ++i)
		if (!people[i].empty())
			cout << people[i];
}

void includeBook()
{
	Author newAuthor;
	Book newBook;
	newAuthor.name = getString("Enter author's name: ");
	newBook.title = getString("Enter the title of the book: ");
	list<Author>::iterator oldAuthor = find(catalog[getIndexFromLetter(newAuthor.name[0])].begin(),
											catalog[getIndexFromLetter(newAuthor.name[0])].end(),
											newAuthor);
	if (oldAuthor == catalog[getIndexFromLetter(newAuthor.name[0])].end())
	{
		newAuthor.books.push_front(newBook);
		catalog[getIndexFromLetter(newAuthor.name[0])].push_front(newAuthor);
	}
	else
		(*oldAuthor).books.push_front(newBook);
}

void checkoutBook()
{
	Patron patron;
	Author author;
	Book book;

	list<Author>::iterator authorRef;
	list<Book>::iterator bookRef;

	patron.name = getString("Enter patron's name: ");

	while (true)
	{
		author.name = getString("Enter author's name: ");
		authorRef = find(catalog[getIndexFromLetter(author.name[0])].begin(),
			catalog[getIndexFromLetter(author.name[0])].end(),
			author);
		if (authorRef == catalog[getIndexFromLetter(author.name[0])].end())
			cout << "No such author\n";
		else break;
	}
	while(true)
	{
		book.title = getString("Enter the title of the book: ");
		bookRef = find(authorRef->books.begin(),
						authorRef->books.end(),
						book);
		if (bookRef == authorRef->books.end())
			cout << "Misspelled title\n";
		else break;
	}
	list<Patron>::iterator patronRef;
	patronRef = find(people[getIndexFromLetter(patron.name[0])].begin(),
					people[getIndexFromLetter(patron.name[0])].end(),
					patron);
	CheckedOutBook checkedOutBook(authorRef, bookRef);

	if(patronRef == people[getIndexFromLetter(patron.name[0])].end())
	{
		patron.books.push_front(checkedOutBook);
		people[getIndexFromLetter(patron.name[0])].push_front(patron);
		bookRef->patron = &*people[getIndexFromLetter(patron.name[0])].begin();
	}
	else
	{
		patronRef->books.push_front(checkedOutBook);
		bookRef->patron = &*patronRef;
	}
}

void returnBook()
{
	Patron patron;
	Book book;
	Author author;

	list<Patron>::iterator patronRef;
	list<Book>::iterator bookRef;
	list<Author>::iterator authorRef;

	while(true)
	{
		patron.name = getString("Enter patron's name: ");
		patronRef = find(	people[getIndexFromLetter(patron.name[0])].begin(),
							people[getIndexFromLetter(patron.name[0])].end(),
				patron);
		if (patronRef == people[getIndexFromLetter(patron.name[0])].end())
			cout << "No such patron\n";
		else
			break;
	}
	while(true)
	{
		author.name = getString("Enter author's name: ");
		authorRef = find(catalog[getIndexFromLetter(author.name[0])].begin(),
						catalog[getIndexFromLetter(author.name[0])].end(),
				author);
		if (authorRef == catalog[getIndexFromLetter(author.name[0])].end())
			cout << "No such author\n";
		else break;
	}
	while(true)
	{
		book.title = getString("Enter the title of the book: ");
		bookRef = find((*authorRef).books.begin(),
			(*authorRef).books.end(), book);
		if (bookRef == (*authorRef).books.end())
			cout << "Misspelled title\n";
		else break;
	}
	CheckedOutBook checkedOutBook(authorRef, bookRef);
	bookRef->patron = nullptr;
	patronRef->books.remove(checkedOutBook);
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