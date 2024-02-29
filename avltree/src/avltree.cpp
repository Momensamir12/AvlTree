#include <iostream>
#include <cassert>
#include <vector>
#include <algorithm>
using namespace std;

class AVLTree {
private:
	struct BinaryNode {
		string data { };
		int height { };

		BinaryNode* left { };
		BinaryNode* right { };

		BinaryNode(string data) :
				data(data) {
		}

		int ch_height(BinaryNode* node) {	// child height
			if (!node)
				return -1;			// -1 for null
			return node->height;	// 0 for leaf
		}


		void update() {	// call in end of insert function
			height = 1 + max(ch_height(left), ch_height(right));

		}

		int balance_factor() {
			return ch_height(left) - ch_height(right);
		}
	};


	BinaryNode *root { };

	///////////////////////////
	bool search(string target, BinaryNode* node) {
		if (!node)
			return false;

		if (target == node->data)
			return true;

		if (target < node->data)
			return search(target, node->left);

		return search(target, node->right);
	}
	bool search_prefix(string prefix,BinaryNode * node,bool found=true){
		if(!node)
			return false;
		for(int i=0;i<node->data.size();i++){
			if(node->data[i]==prefix[i]){
				for(int j=1;j<prefix.size();j++){
						if(node->data[j]!=prefix[j])
							found=false;

				}
			}
		}
		if(found)
			return true;

		search_prefix(prefix,node->left);
		search_prefix(prefix,node->right);
		return false;
	}

	BinaryNode* right_rotation(BinaryNode* Q) {
		BinaryNode* P = Q->left;
		Q->left = P->right;
		P->right = Q;
		Q->update();
		P->update();
		return P;
	}

	BinaryNode* left_rotation(BinaryNode* P) {
		BinaryNode* Q = P->right;
		P->right = Q->left;
		Q->left = P;
		P->update();
		Q->update();
		return Q;
	}

	BinaryNode* balance(BinaryNode* node) {
		if (node->balance_factor() == 2) { 			// Left
			if (node->left->balance_factor() == -1)	// Left Right?
				node->left = left_rotation(node->left);	// To Left Left

			node = right_rotation(node);	// Balance Left Left
		} else if (node->balance_factor() == -2) {
			if (node->right->balance_factor() == 1)
				node->right = right_rotation(node->right);

			node = left_rotation(node);
		}
		return node;
	}

	BinaryNode* insert_node(string target, BinaryNode* node) {
		if (target < node->data) {
			if (!node->left)
				node->left = new BinaryNode(target);
			else
				// change left. update left as it might be balanced
				node->left = insert_node(target, node->left);
		} else if (target > node->data) {
			if (!node->right)
				node->right = new BinaryNode(target);
			else
				node->right = insert_node(target, node->right);
		}
		node->update();
		return balance(node);
	}

	BinaryNode* min_node(BinaryNode* cur) {
		while (cur && cur->left)
			cur = cur->left;
		return cur;
	}

	BinaryNode* delete_node(string target, BinaryNode* node) {
		if (!node)
			return nullptr;

		if (target < node->data)
			node->left = delete_node(target, node->left);
		else if (target > node->data)
			node->right = delete_node(target, node->right);
		else {
			BinaryNode* tmp = node;

			if (!node->left && !node->right)	// case 1: no child
				node = nullptr;
			else if (!node->right) 	// case 2: has left only
				node = node->left;		// connect with child
			else if (!node->left)	// case 2: has right only
				node = node->right;
			else {	// 2 children: Use successor
				BinaryNode* mn = min_node(node->right);
				node->data = mn->data;	// copy & go delete
				node->right = delete_node(node->data, node->right);
				tmp = nullptr;	// Don't delete me. Successor will be deleted
			}
			if (tmp)
				delete tmp;
		}
		if (node) {
			node->update();
			node = balance(node);
		}
		return node;
	}



public:
	void insert_value(string target) {
		if (!root)
			root = new BinaryNode(target);
		else
			root = insert_node(target, root);
	}

	void delete_value(string target) {
		if (root) {
			root = delete_node(target, root);
		}
	}

	bool word_exist(string node){
		return search(node,root);
	}
	bool prefix_exist(string prefix){

		return search_prefix(prefix,root);
	}

};

int main() {

	AVLTree tree;
	tree.insert_value("abcd");
	tree.insert_value("xyz");
	cout<<tree.word_exist("abcd")<<"\n";
	cout<<tree.word_exist("ab")<<"\n";
	cout<<tree.prefix_exist("ab")<<"\n";
	cout<<tree.prefix_exist("cd")<<"\n";

	return 0;
}
