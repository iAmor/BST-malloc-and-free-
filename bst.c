#include<stdio.h>
#include<stdlib.h>
#include "bst.h"
#include "memory.h"
#include "llist.h"

/* malloc a new node and assign the data
 * h
 * pointer to its data field
 */
node* node_new(void* data){
	node *newnode = (node *) malloc(sizeof(node));
	newnode->data = data;
	newnode->left = NULL;
	newnode->right = NULL; 
  return newnode;
}

/* create a new bst, initialize its root to be NULL
 */
bst* bst_new(int (*cmp)(const void* x, const void* y)){
	bst *newbst = (bst *) malloc(sizeof(bst));
	newbst->root = NULL;
	newbst->cmp = cmp;
  return newbst;
}

/* Insert a node to a subtree with a root node as parameter
 * Insertion is in sorted order. 
 * Return the new root of the modified subtree.  
 */
node* node_insert(node* root, void* data, 
    int (*cmp)(const void* x, const void* y)){
	//decides which subtree to visit, if any, otherwise places the node
	if (root == NULL) {
		node *newnode = node_new(data);
		root = newnode;
		return root;
	}
	else if (cmp(root->data, data) == 1) {
		node *tmp = root;
		root->left = node_insert(root->left, data, cmp);
		return tmp;
	}
	else {
		node *tmp = root;
		root->right = node_insert(root->right, data, cmp);
		return tmp;
	}

}

/* Insert a new node to the bst
 */
void bst_insert(bst* b, void* data) {
	node *tmp = node_insert(b->root, data, b->cmp);	
	b->root = tmp;
}

//finds the rightmost node, called on the left subtree when deleting
node* rightmost(node* top)
{
	node *tmp = malloc(sizeof(node));
	//takes the tmp pointer to the rightmost node of the given tree
	for (tmp = top; tmp->right != NULL; tmp = tmp->right) {
		;
	}
	return tmp;
}

/* delete a node from a subtree with a given root node
 * use the comparison function to search the node and delete 
 * it when a matching node is found. This function only
 * deletes the first occurrence of the node, i.e, if multiple 
 * nodes contain the data we are looking for, only the first node 
 * we found is deleted. 
 * Return the new root node after deletion.
 */
node* node_delete(node* root, void* data, 
    int (*cmp)(const void* x, const void* y)){
	//if the root is null, returns the root immediately
	if (root == NULL) {
		return root;
	}
	//checks to see if the root is greater or less than the given data
	if (cmp(root->data, data) == 1) {
		root->left = node_delete(root->left, data, cmp);
	}
	else if (cmp(root->data, data) == -1) {
		root->right = node_delete(root->right, data, cmp);
	}
	if (cmp(root->data, data) == 0) {
		//replaces with right subtree, or NULL if node has no children
		if (root->left == NULL) {
			node *tmp = root->right;
			free(root);
			return tmp;
		}
		else if (root->right == NULL) {
			node *tmp = root->left;
			free(root);
			return tmp;
		}
		//in the case where the node has 2 children, find suitable node
		else {
		node *tmp = rightmost(root->left);
		root->data = tmp->data;
		root->left = node_delete(root->left, tmp->data, cmp);
		}
	}
	else if (cmp(root->data, data) == 1) {
		root->left = node_delete(root->left, data, cmp);
	}
	else {
		root->right = node_delete(root->right, data, cmp);
	}
	return root;
}

/* delete a node containing data from the bst
 */
void bst_delete(bst* b, void* data){
	b->root = node_delete(b->root, data, b->cmp);
	return;
}

/* Search for a node containing data in a subtree with
 * a given root node. Use recursion to search that node. 
 * Return the first occurrence of node. 
 */
void* node_search(node* root, void* data, 
    int (*cmp)(const void* x, const void* y)){
	node *tmp = root;
	//iterates through the tree, deciding which path to follow using cmp
	while (1) {
		if (tmp == NULL) {
			printf("data not found\n");
			return NULL;
		}
		if (cmp(tmp->data, data) == 0) {
			return tmp->data;
		}
		if (cmp(tmp->data, data) == 1) {
			//precaution to not segfault by sending tmp to NULL
			if (tmp->left != NULL) {
				tmp = tmp->left;
			}
			else {
				printf("data not found\n");
				return NULL;
			}
		}
		else if (cmp(tmp->data, data) == -1) {
			
			if (tmp->right != NULL) {
				tmp = tmp->right;
			}
			else {
				printf("data not found\n");
				return NULL;
			}
		}
	}
}

/* Search a node with data in a bst. 
 */
void* bst_search(bst* b, void* data) {
	void *tmp = node_search(b->root, data, b->cmp);
	return tmp;
}

/* traverse a subtree with root in ascending order. 
 * Apply func to the data of each node. 
 */
void inorder_traversal(node* root, void(*func)(void* data)){
	if (root == NULL) {
		return;
	}
		inorder_traversal(root->left, func);
		func(root->data);
		inorder_traversal(root->right, func); 
}



/* traverse a bst in ascending order. 
 * Apply func to the data of each node. 
 */
void bst_inorder_traversal(bst* b, void(*func)(void* data)){
	inorder_traversal(b->root, func);
}
/*
void node_traversal(node* root, void(*func)(llist* list, void* data)){
	if (root == NULL) {
		return;
	}
		node_traversal(root->left, func);
		func(root->data);
		node_traversal(root->right, func); 
}

void tree_traversal(bst* b, void(*func)(llist* list, void* data)){
	node_traversal(b->root, func);
}

void* bst_iterate(bst* tree)
{
	node *list = create_llist();
	tree_traversal(tree, insert_tail);
	iterate(list);   	
}
*/

llist* tree_to_list(node* root, llist* list)
{
	if (root != NULL) {
	tree_to_list(root->left, list);
	insert_tail(list, root->data);
	tree_to_list(root->right, list);
	}
	return list;
}

llist* bst_to_list(bst* b, llist* list)
{
	return tree_to_list(b->root, list);
}

void* bst_iterate(bst* tree)
{
	llist *list = create_llist();
	list = bst_to_list(tree, list);
	return iterate(list);   	
}

//helper function to free the tree
void node_free(node* root)
{
	if (root == NULL) {
		return;
	}
	node_free(root->left);
	node_free(root->right);
	free(root);
}

/* free the bst with 
 */
void bst_free(bst* b){
	node_free(b->root);
	free(b);
}

void* node_item_or_successor(node *n, void *item, 
    	int (*cmp)(const void* x, const void* y))
{
	if (cmp(n->data, item) == -1) {
		printf("node_item_or_successor: no valid memory blocks\n");
		return NULL;
	}
	if (cmp(n->data, item) == 0) {
		return n->data;
	}
	node *tmp = n;
	while ((tmp->left != NULL) && 
	(cmp(tmp->data, item) == 1) && 
	(cmp(tmp->left->data, item) >= 0)) {
		tmp = tmp->left;
	}
	return tmp->data;
}



void* bst_item_or_successor(bst *b, void *item)
{
	return node_item_or_successor(b->root, item, b->cmp);
}





