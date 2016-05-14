#include "utf8char.h"
#include <iostream>
#include <fstream>
using namespace std;

wchar_t read(istream & infile, character & ch) {
	wchar_t result = 0x00000000;
	unsigned int &range = ch.length;

	infile.read((char *)&ch.data[0],sizeof(character::UTF8));

	//handles the last duplicated character
	if (infile.fail()) {
		throw UTF8err("EOF");
	}
	//check if UTF-8 start with 0
	if (ch.data[0].t1.ck ^ 0x01) {
		range = 1;
		goto rest;
		}	
	//check if UTF-8 start with 110
	if (!(ch.data[0].t2.ck ^ 0x06)) {
		range = 2;
		goto rest;
	}
	//check if UTF-8 start with 1110
	if (!(ch.data[0].t3.ck ^ 0x0e)) {
		range = 3;
		goto rest;
	}
	//check if UTF-8 start with 11110
	if (!(ch.data[0].t4.ck ^ 0x1e)) {
		range = 4;
		goto rest;
	} else { //handles invalid length
		range = 1;
		throw UTF8err("length");
	}

	rest: {//handles the rest of UTF-8 encoding
		for (unsigned int i = 1; i < range; i ++){
			infile.read((char *)&ch.data[i].ch,sizeof(character::UTF8));
			//check if the extra data starts with 0
			if(ch.data[i].dt.ck ^ 0x02) {
				range = i + 1;
				throw UTF8err("padding");
			}
		}//for
		//000000-00007F case
		if (range == 1) {
			result = ch.data[0].t1.dt;
			}
		//000080-0007FF case
		if (range == 2) {
			result = ch.data[0].t2.dt;
			}
		//000800-00FFFF case
		if (range == 3) {
			result = ch.data[0].t3.dt;
			} 
		//010000-10FFFF case
		if (range == 4) {
			result = ch.data[0].t4.dt;
			}
		//handles the 10xxxxxx
		for (unsigned int i = 1; i < range; i++){
			result = (result << 6);
			result |= ch.data[i].dt.dt;
			}//for
		//check if the result is within the proper range
		if ((range == 1 && (result < 0x00000000 || result > 0x0000007f)) ||
			(range == 2 && (result < 0x00000080 || result > 0x000007ff)) ||
			(range == 3 && (result < 0x00000800 || result > 0x0000ffff)) ||
			(range == 4 && (result < 0x00010000 || result > 0x0010ffff))) {
			throw UTF8err("range");
		}
		return result;
	}
}
