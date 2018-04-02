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
#include "tree.h"
#include <sstream>



CodeTable createCode(unsigned int code, unsigned int shift, unsigned int data) {

	CodeTable c;
	c.code = code;
	c.data = data;
	c.shift = shift;


	return c;
}

//Node Operators
p_TreeNode createNode(p_TreeNode left, p_TreeNode right)
{
	p_TreeNode node = (p_TreeNode)malloc(sizeof(struct TreeNode));
	node->leaf = 0;
	node->frequency = left->frequency + right->frequency; 
	node->data = 0;
	node->leftC = left;
	node->rightC = right;

	return node;
}
p_TreeNode createNode(int frequency, unsigned char data)
{
	p_TreeNode node = (p_TreeNode)malloc(sizeof(struct TreeNode));
	node->leaf = 1;
	node->frequency = frequency;
	node->data = data;
	node->leftC = 0;
	node->rightC = 0;

	return node;
}


// Tree Operators

#define writeBit(x) { \
	if (x) \
		code |= bitpos;\
	bitpos >>= 1;\
	shift++;\
}



void getCode(p_TreeNode  root, vector<CodeTable> & table, bool right, unsigned int code, unsigned int bitpos, unsigned int shift) {
	if (root->leaf)
	{
		writeBit(right);
		table[root->data] = createCode(code, shift, root->data);

	}
	else
	{
		writeBit(right);
		getCode(root->leftC, table, 0, code, bitpos, shift);
		getCode(root->rightC, table, 1, code, bitpos, shift);
	}

}

void getCodes(p_TreeNode root, vector<CodeTable> & table){

	getCode(root->leftC, table, 0, 0, 0b10000000000000000000000000000000, 0);
	getCode(root->rightC, table, 1, 0, 0b10000000000000000000000000000000, 0);

}

//int storeTree(p_TreeNode root, unsigned char * result) {
//	int skip = 1;
//	if (root->leaf) {
//
//	/*	Log("1");
//
//		std::stringstream streama;
//		streama << std::dec << "(" << (int)root->data << ")";
//		std::string resulta(streama.str());
//		Log((char *)&resulta);*/
//
//		result[0] = 1;
//		result[1] = root->data;
//		skip++;
//	}
//	else
//	{
//		Log("0");
//		result[0] = 0;
//		skip += storeTree(root->leftC, result + 1);
//		skip += storeTree(root->rightC, result + skip);
//	}
//	return skip;
//}
//
//int restoreTree(p_TreeNode * root, const unsigned char * in) {
//	int skip = 0;
//	if (*in == 0)
//	{
//		Log("0");
//		skip++;
//		p_TreeNode left = createNode(1, 0);
//		p_TreeNode right = createNode(1, 0);
//		skip += restoreTree(&left, in + skip);
//		skip += restoreTree(&right, in + skip);
//		*root = createNode(left, right);
//		return skip;
//	}
//	else if(*in == 1)
//	{
//		Log("1");
//
//		std::stringstream streama;
//		streama << std::dec << "(" << (int)in[1] << ")";
//		std::string resulta(streama.str());
//		Log((char *)&resulta);
//
//		skip += 2;
//		char data = in[1];
//		*root = createNode(0, data);
//		return skip;
//	}
//	else {
//
//		std::stringstream streama;
//		streama << std::dec << "(" << (int)in[0] << ")";
//		std::string resulta(streama.str());
//		Log((char *)&resulta);
//
//		Log("ERROR\n");
//		return 0;
//	}
//}


//void storeTree(p_TreeNode root, unsigned char * result, int * skip) {
//	if (root->leaf) {
//		//Log("1");
//		*(result + skip[0]) = 1;
//		skip[0]++;
//		*(result + skip[0]) = root->data;
//		skip[0]++;
//		
//
//	}
//	else
//	{
//		//Log("0");
//		*(result + skip[0]) = 0;
//		skip[0]++;
//		storeTree(root->leftC, result, skip);
//		storeTree(root->rightC, result, skip);
//	}
//}
//
//void restoreTreeRekursiv(p_TreeNode * root, const unsigned char * in, int * skip) {
//	if (*(in + skip[0]) == 0)
//	{
//		//Log("0");
//		skip[0]++;
//		p_TreeNode left = createNode(1, 0);
//		p_TreeNode right = createNode(1, 0);
//		restoreTreeRekursiv(&left, in, skip);
//		restoreTreeRekursiv(&right, in, skip);
//		*root = createNode(left, right);
//	}
//	else if(*(in + skip[0]) == 1)
//	{
//		//Log("1");
//		char data = *((in + skip[0]) + 1);
//		skip[0] += 2;
//		*root = createNode(0, data);
//	}
//	else
//	{
//		Log("Error");
//	}
//
//}
//
//bool restoreTree(p_TreeNode * root, const unsigned char * in, int * skip) {
//	//Log("Start Restore");
//	restoreTreeRekursiv(root, in, skip);
//	//Log("End Restore");
//	return true;
//}


bool storeTree(p_TreeNode root, unsigned char * result, int * skip) {
	if (root->leaf) {
		/*Log("1");
		
		std::stringstream streama;
		streama << std::dec << "(" << (int)root->data << ")";
		std::string resulta(streama.str());
		Log((char *)&resulta);*/

		*(result + skip[0]) = 1;
		skip[0]++;
		*(result + skip[0]) = root->data;
		skip[0]++;

	}
	else
	{
		//Log("0");
		*(result + skip[0]) = 0;
		skip[0]++;
		storeTree(root->leftC, result, skip);
		storeTree(root->rightC, result, skip);
	}
	return true;
}


void restoreTreeRekursiv(p_TreeNode * root, const unsigned char * in, int * skip) {
	if (*(in + skip[0]) == 0)
	{
		//Log("0");
		skip[0]++;
		p_TreeNode left = createNode(1, 0);
		p_TreeNode right = createNode(1, 0);
		restoreTreeRekursiv(&left, in, skip);
		restoreTreeRekursiv(&right, in, skip);
		*root = createNode(left, right);
	}
	else
	{
		//Log("1");

		unsigned char data = *((in + skip[0]) + 1);

		//std::stringstream streama;
		//streama << std::dec << "(" << (int)data << ")";
		//std::string resulta(streama.str());
		//Log((char *)&resulta);
		
		skip[0] += 2;
		*root = createNode(0, data);
	}

}

bool restoreTree(p_TreeNode * root, const unsigned char * in, int * skip) {
	restoreTreeRekursiv(root, in, skip);
	return true;
}

p_TreeNode buildTree(int * frequency, unsigned char * data, int n)
{
	priority_queue<p_TreeNode, vector<p_TreeNode>, nodeComparatorMIN> minHeap;

	for (int i = 0; i < n; i++)
	{
		p_TreeNode node = createNode(frequency[i], data[i]);
		minHeap.push(node);
	}

	while (minHeap.size() > 1)
	{
		p_TreeNode node1 = minHeap.top();
		minHeap.pop();
		p_TreeNode node2 = minHeap.top();
		minHeap.pop();
		p_TreeNode new_node = createNode(node1, node2);
		minHeap.push(new_node);
	}
	return minHeap.top();
}

void deleteTree(p_TreeNode root)
{
	if (root != NULL)
	{
		deleteTree(root->leftC);
		deleteTree(root->rightC);
		if (root->leftC != NULL)
			root->leftC = NULL;
		if (root->rightC != NULL)
			root->rightC = NULL;
		root = NULL;
		delete(root);
	}
}

// SHANNON .......................................


void ShannonRekursive(p_TreeNodeArray nArray) {
	splitArray(nArray);
	if (nArray->leftsize > 1)
		ShannonRekursive(nArray->leftnodes);
	if (nArray->rightsize > 1)
		ShannonRekursive(nArray->rightnodes);
}

void CreateTreefromNodeArray(p_TreeNodeArray nArray, p_TreeNode * root) {
	if (nArray->size == 1) {
		root[0] = nArray->nodes[0];
	}	
	else {
		p_TreeNode lnode = createNode(0, 0);;
		p_TreeNode rnode = createNode(0, 0);;
		CreateTreefromNodeArray(nArray->leftnodes, &lnode);
		CreateTreefromNodeArray(nArray->rightnodes, &rnode);
		*root = createNode(lnode, rnode);

		/*nArray->leftnodes = NULL;
		nArray->rightnodes = NULL;
		nArray = NULL;
		delete(nArray);*/
	}
}

p_TreeNode buildTreeShannon(vector<unsigned int> & frequency, vector<unsigned char> & data, int n, p_TreeNodeArray * nArray)
{
	priority_queue<p_TreeNode, vector<p_TreeNode>, nodeComparatorMAX> maxHeap;

	p_TreeNode * nodes = new p_TreeNode[n];
	int sumfreq = 0;

	for (int i = 0; i < n; i++)
	{
		p_TreeNode node = createNode(frequency[i], data[i]);
		maxHeap.push(node);
		sumfreq += frequency[i];
	}

	for (int i = 0; i < n; i++)
	{
		nodes[i] = maxHeap.top();
		maxHeap.pop();
	}

	nArray[0] = createArray(n, nodes);
	ShannonRekursive(nArray[0]);

	p_TreeNode root = createNode(0, 0);
	CreateTreefromNodeArray(nArray[0], &root);

	return  root;
}

p_TreeNodeArray createArray(int size, p_TreeNode * nodes) {
	p_TreeNodeArray narray = (p_TreeNodeArray)malloc(sizeof(struct TreeNodeArray));
	narray->size = size;
	narray->leftsize = 0;
	narray->rightsize = 0;
	narray->nodes = nodes;
	narray->leftnodes = (p_TreeNodeArray)malloc(sizeof(struct TreeNodeArray));
	narray->rightnodes = (p_TreeNodeArray)malloc(sizeof(struct TreeNodeArray));
	return narray;
}

void splitArray(p_TreeNodeArray narray) {
	int freq = 0;
	for (int i = 0; i < narray->size; i++) {
		freq += narray->nodes[i]->frequency;
	}

	p_TreeNode * lnodes = new p_TreeNode[narray->size];
	p_TreeNode * rnodes = new p_TreeNode[narray->size];
	int sum = 0;
	int hsum = freq / 2;
	int right = 0;
	int countleft = 0;
	int countright = 0;

	for (int i = 0; i < narray->size; i++) {

		if (abs(sum - hsum) < abs(sum + narray->nodes[i]->frequency - hsum)) {
			right = 1;
		}
		else
		{
			sum += narray->nodes[i]->frequency;
		}

		if (!right) {
			lnodes[countleft] = narray->nodes[i];
			countleft++;
		}
		else
		{
			rnodes[countright] = narray->nodes[i];
			countright++;
		}

	}

	narray->leftnodes = createArray(countleft, lnodes);
	narray->leftsize = countleft;
	narray->rightnodes = createArray(countright, rnodes);
	narray->rightsize = countright;

}

void deleteTree(p_TreeNodeArray root)
{
	if (root != NULL)
	{	
		
		if (root->leftnodes != NULL) {
			deleteTree(root->leftnodes);
			root->leftnodes = NULL;
		}
			if (root->rightnodes != NULL) {
				deleteTree(root->rightnodes);
				root->rightnodes = NULL;
			}
		root = NULL;
		delete(root);
	}
}