#include <cstdio>
#include <iostream>
#include <vector>
#include <string>
#include <stack>
#include <unordered_map>
#include <unordered_set>
#include <queue>
#include <memory>
#include <algorithm>

#define INT_MAX 0x7fffffff
#define INT_MIN 0x80000000

using namespace std;

int maxTreeHeight = 0;
int nodeNum = 0;

struct TreeNode {
	vector<vector<char>> table;
	vector<int> colIndex;
	vector<shared_ptr<TreeNode>> children;
	int fatherPartition;
	char fatherPartitionVal;
	int curHeight;
	TreeNode(vector<vector<char>>
			_table,
			vector<int>_colIndex,vector<shared_ptr<TreeNode>>
			_children,int _fatherPartition,char
			_fatherPartitionVal,int _curHeight) :
		table(_table) ,colIndex(_colIndex), children(_children)
		,fatherPartition(_fatherPartition),fatherPartitionVal(_fatherPartitionVal),curHeight(_curHeight) {}
};

void printNode(shared_ptr<TreeNode> node){
	cout << "Tabel: " << endl;
	for(auto x : node->table){
		for(auto y : x)	{
			cout << " " << y; 	
		}
		cout << endl;
	}
	cout << "Column index: " <<endl;
	for(auto x : node->colIndex){
		cout << " " << x; 
	}
	cout << endl << "Father partition: " << node->fatherPartition << endl;
	cout << endl << "Father partition value: " << node->fatherPartitionVal << endl;
	cout << endl << "Cur height: " << node->curHeight << endl;
	cout << "========================" << endl;
}

void partitionNode(shared_ptr<TreeNode> node,int partitionCol){	
	auto nodeTable = node->table;
	int curHeight = node->curHeight + 1;
	maxTreeHeight = max(maxTreeHeight,curHeight);
	while(nodeTable.size()!=0){
		char curVal = nodeTable[0][partitionCol];
		auto newColIndex = node->colIndex;
		newColIndex.erase(newColIndex.begin()+partitionCol);
		int fatherPartitionVal = curVal;
		vector<vector<char>> newTable;
		vector<shared_ptr<TreeNode>> newChildren;
		for(int i=0;i<nodeTable.size();i++){
			if(nodeTable[i][partitionCol] == curVal){
				auto curRow = nodeTable[i];	
				curRow.erase(curRow.begin()+partitionCol);
				newTable.push_back(curRow);
				nodeTable.erase(nodeTable.begin()+i);
				i--;
			}	
		}
		auto newNode
			=make_shared<TreeNode>(newTable,newColIndex,newChildren,partitionCol,curVal,curHeight);
		nodeNum ++;	
		node->children.push_back(newNode);
	}
}

shared_ptr<TreeNode> inputData(){
	string oneline;
	vector<vector<char>> inputTable;
	vector<int> colIndex;
	vector<shared_ptr<TreeNode>> children;
	while(getline(cin,oneline)){
		vector<char> onelineTable;
		for(int i=0;i<oneline.length();i++){
			if(oneline[i] != ','){
				onelineTable.push_back(oneline[i]);	
			}	
		}
		onelineTable.pop_back();
		inputTable.push_back(onelineTable);
	}
	for(int i=0;i<inputTable[0].size();i++){
		colIndex.push_back(i);	
	}
	auto root =make_shared<TreeNode>(inputTable,colIndex,children,-1,'-',0);
	return root;
}

float calEntropy(shared_ptr<TreeNode> node,int col){
	vector<vector<char>> nodeTable = node->table;
	int tableRowSize = node->table.size();
	float entropy = 0;
	while(nodeTable.size() != 0){
		char curVal = nodeTable[0][col]; 	
		int curValPosNum = 0;
		int curValNegNum = 0;
		for(int i=0;i<nodeTable.size();i++){
			if(nodeTable[i][col] == curVal){
				if(nodeTable[i][0] == 'p')
					curValPosNum++;
				if(nodeTable[i][0] == 'e')
					curValNegNum++;
				nodeTable.erase(nodeTable.begin()+i);
				i--;	
			}	
		}
		int curValNum = curValPosNum + curValNegNum;

		if(curValPosNum != 0 && curValNegNum != 0 && col != 0){
		entropy += - ((float)curValNum/tableRowSize) * 
			( ((float)curValPosNum/curValNum * (float)log((float)curValPosNum/curValNum) / log(2))
			+
			((float)curValNegNum/curValNum * (float)log((float)curValNegNum/curValNum) / log(2)) );
		}
		if(col == 0){
		entropy += - 
			( ((float)curValNum/tableRowSize *
			   (float)log((float)curValNum/tableRowSize) / log(2))
			);
		}
	}	
	return entropy;
}

int calPartitionCol(shared_ptr<TreeNode> node){
	float info = calEntropy(node,0);
	int maxGainCol = -1;
	float maxGain = -1;
	for(int i=1;i<node->table[0].size();i++){
		float gain = info - calEntropy(node,i);	
		if(gain > maxGain){
			maxGain = gain;	
			maxGainCol = i;
		}
	}
	return maxGainCol;
}

bool isPure(shared_ptr<TreeNode> node){
	if(node->table.size() == 0) return true;
	if(node->table[0].size() == 2) return true;
	char sign = node->table[0][0];
	for(int i=1;i<node->table.size();i++){
		if(node->table[i][0] != sign )	
			return false;
	}
	return true;
}

bool buildTree(shared_ptr<TreeNode> node){
	if(isPure(node))
		return true;
	cout << isPure(node) << endl;
	int partitionCol = calPartitionCol(node);
	partitionNode(node,partitionCol);
	for(auto x : node->children){
		printNode(x);
		buildTree(x);
	}
	return true;
}

int main(){
	auto root = inputData();
	printNode(root);
	if(buildTree(root))
	cout << "Tree build finished! " << endl;
	cout << "Max tree height: " << maxTreeHeight << endl;
}
