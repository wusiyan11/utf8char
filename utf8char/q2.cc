#include "utf8char.h"
#include <iostream>
#include <fstream>
#include <iomanip>
using namespace std;

/*
int main(int argc, char *argv[]) handle the command-line arguments
and calls read until end-of-file is raised, and does all necessary printing
of valid UTF-8 characters or errors. Prints the bytes of the UTF-8 character in hexadecimal.

Usage : [ filename ]
*/
int main( int argc, char *argv[] ) {
	istream *infile = &cin;
	infile->exceptions(ios_base::failbit);

	try{
		if (argc > 2) {
			cerr << "Usage: " << argv[0] << " [ filename ]" <<endl;
		}
		if (argc == 2) {
			infile = new ifstream (argv[1]);
		}
		while (!infile->fail()) {
			character one;
			try {
				wchar_t lol = read(*infile, one);
				cout << "0x";
				for (unsigned int i=0; i < one.length; i++){
					cout << hex << setfill('0') << setw(2) << right << (unsigned int)(unsigned char)one.data[i].ch;
				}//for
				cout << " : valid value 0x" << hex << (unsigned int)lol << endl;
			} catch(UTF8err x) {
				string y = string(x.msg);
				if (y == "EOF") {
					break;
				}
				cout << "0x";
				for (unsigned int i=0; i < one.length; i++){
					cout << hex << setfill('0') << setw(2) << right << (unsigned int)(unsigned char)one.data[i].ch;	
				}//for
				cout << " : invalid " << x.msg << endl;
			}
		}//while
	}//try
	 catch (ios_base::failure err) {
		cerr<< err.what() <<endl;
	}//catch
	//handles changed infile
	if (infile != &cin) delete infile;
}
