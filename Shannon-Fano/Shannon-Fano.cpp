// Vuong-DCP Codec, copyright 2018 by Samuel Wolf
//
//This program is free software : you can redistribute it and/or modify
//it under the terms of the GNU General Public License as published by
//the Free Software Foundation, either version 3 of the License, or
//(at your option) any later version.
//
//This program is distributed in the hope that it will be useful,
//but WITHOUT ANY WARRANTY; without even the implied warranty of
//MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.See the
//GNU General Public License for more details.
//
//You should have received a copy of the GNU General Public License
//along with this program.If not, see <http://www.gnu.org/licenses/>.

#include "stdafx.h"
#include "Shannon-Fano.h"
#include "tree.h"

#define readBit() { \
	bit = bitpos & *in;\
	bitpos >>=1;\
	if ( !bitpos ){\
		in++;\
		bitpos = 0x80;\
	}\
}

#define writeBit(x) { \
	if ( x) \
		*out|=bitpos;\
	bitpos>>=1;\
	if ( !bitpos){ \
		bitpos=0x80;\
		out++;\
		*out=0;\
	}\
}

void Countchar(const unsigned char * in, unsigned int length, vector<unsigned int> & table) {
	const unsigned char * input = in;
	const unsigned char * const ending = in + length;
	unsigned int a = 0;

	do
	{
		a = (unsigned int)table[*input];
		a++;
		table[*input] = (unsigned int)a;
		input++;

	} while (input < ending);

}

DllExport unsigned int Encode(const unsigned char * __restrict in, unsigned char * __restrict out, const unsigned int length)
{
	// Define input end
	const unsigned char * const ending = in + length;
	// save the out pointers start position
	unsigned char * const start = out;

	unsigned char bitpos = 0b10000000;

	vector<unsigned int> table;	
	vector<unsigned char> data;
	vector<unsigned int> freq;	
	vector<CodeTable> codeTable;

	table.resize(256);
	data.resize(256);
	freq.resize(256);
	codeTable.resize(256);



	// Count the chars
	Countchar(in, length, table);

	// Remove the empty counts
	int count = 0;
	for (int i = 0; i < 256; i++) {
		if ((int)table[i] > 0) {
			data[count] = i;
			freq[count] = (int)table[i];
			count++;
		}

	}


	//Build Huffman tree
	p_TreeNodeArray nArray = createArray(0, 0);
	p_TreeNode root = buildTreeShannon(freq, data, count, &nArray);

	int skip = 0;
	storeTree(root, out, &skip);
	out += skip;

	getCodes(root, codeTable);

	*out = 0;
	do {
		// getting input data
		int in_val = in[0];

		bool bit = 0;
		//unsigned int readpos = 0b10000000000000000000000000000000;
		unsigned int readpos = 0b10000000000000000000000000000000;
		unsigned int  shift = codeTable[in_val].shift;
		unsigned int  code = codeTable[in_val].code;

		for (unsigned int i = 0; i < shift; i++) {
			bit = code & readpos;
			readpos >>= 1;
			writeBit(bit);
		}

		in++;

	} while (in < ending);

	//deleteTree(root);
	//deleteTree(nArray);


	return (unsigned int)(out - start) + 1;
}

DllExport void Decode(const unsigned char * __restrict in, unsigned char * __restrict out, const unsigned int length)
{
	const unsigned char * const ending = out + length;
	bool bit = 0;
	unsigned char bitpos = 0x80;

	p_TreeNode root = createNode(0, 0);

	// read and restore Tree from input
	int skip = 0;
	restoreTree(&root, in, &skip);
	in += skip;

	p_TreeNode currentNode;
	currentNode = root;
	do {

		readBit();
		// if 0 go to left child, if 1 go to right child
		if (bit)
			currentNode = currentNode->rightC;
		else
			currentNode = currentNode->leftC;

		// if leaf write data, go back to root
		if (currentNode->leaf) {
			*out = currentNode->data;
			out++;
			currentNode = root;
		}

	} while (out < ending);
}
