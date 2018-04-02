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


#pragma once
#include <iostream>
#include <stdlib.h>
#include <queue>
#include <vector>
using namespace std;


struct CodeTable
{
	unsigned int code;
	unsigned int shift;
	unsigned char data;
};

typedef CodeTable * p_CodeTable;

CodeTable createCode(unsigned int code, unsigned int shift, unsigned int data);

struct TreeNode
{
	bool leaf;
	int frequency;
	unsigned char data;
	TreeNode * leftC;
	TreeNode * rightC;
};
typedef TreeNode *  p_TreeNode;

p_TreeNode createNode(p_TreeNode left, p_TreeNode right);
p_TreeNode createNode(int frequency, unsigned char data);
p_TreeNode buildTree(int * frequency, unsigned char * data, int n);

void deleteTree(p_TreeNode root);

void getCodes(p_TreeNode root, vector<CodeTable> & table); 

bool storeTree(p_TreeNode root, unsigned char * result, int * skip);
bool restoreTree(p_TreeNode * root, const unsigned char * in, int * skip);

struct TreeNodeArray {
	int size;
	int leftsize;
	int rightsize;
	p_TreeNode * nodes;
	TreeNodeArray * leftnodes;
	TreeNodeArray * rightnodes;
};

typedef TreeNodeArray *  p_TreeNodeArray;

p_TreeNodeArray createArray(int size, p_TreeNode * nodes);
p_TreeNode buildTreeShannon(vector<unsigned int> & frequency, vector<unsigned char> & data, int n, p_TreeNodeArray * nArray);
void splitArray(p_TreeNodeArray narray);
void deleteTree(p_TreeNodeArray root);


struct nodeComparatorMIN {
	bool operator()(const p_TreeNode  &i, const p_TreeNode  &j) {
		return i->frequency > j->frequency;
	}
};

struct nodeComparatorMAX {
	bool operator()(const p_TreeNode  &i, const p_TreeNode  &j) {
		return i->frequency < j->frequency;
	}
};