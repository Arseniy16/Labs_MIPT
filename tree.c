#include <stdio.h>
#include <stdlib.h>

typedef struct _tree {
	int element;
	int h;
	struct _tree* left;
	struct _tree* right;
} Tree;

#define MAX(a, b)  ((a)<(b)?(b):(a))
#define H(x) ((x)?(x)->h:0)

int _bal(Tree* r) {
	return  (H(r->left) - H(r->right));
}

Tree* _node(Tree*l,  int x, Tree* r) {
	Tree* n;
	int lh = H(l);
	int rh = H(r);
	if ( lh - rh == -2) {
		if (_bal(r) < 0) {

			n = _node(_node(l, x, r->left), r->element, r->right);
			free(r);
		} else {

			Tree* rl = r->left;
			n = _node( _node(l,  x, rl->left), rl->element, _node(rl->right, r->element, r->right));
			free(r);
			free(rl);
		}
	} else if ( lh - rh == 2) {
		if (_bal(l) > 0) {
			n = _node(l->left , l->element, _node(l->right, x, r));
			free(l);
		} else {
			Tree* lr = l->right;
			n = _node(_node(l->left, l->element, lr->left), lr->element, _node(lr->right, x, r));
			free(l);
			free(lr);
		}
	} else {
		n  = malloc(sizeof(Tree));
		*n = (Tree){x, 1+MAX(lh, rh), l, r};
	}
	return n;
}

Tree* tree_insert(Tree* root, int x) {
	Tree* n;
	if (!root ) {
		return _node(NULL, x, NULL);
	} else if ( x <= root->element )
		n = _node(tree_insert(root->left, x), root->element, root->right);
	else
		n = _node(root->left, root->element, tree_insert(root->right, x));
	free(root);
	return n;
}

int tree_search(Tree* root, int x, int found(int, void*), void* ctx) {
	if ( !root ) return 0;
	if ( x == root->element) {
		found(root->element, ctx);
		return tree_search(root->left, x, found, ctx);
	} else if ( x < root->element )
		return tree_search(root->left, x, found, ctx);
	else 
		return tree_search(root->right, x, found, ctx);
}

void tree_free(Tree* root) {
	Tree* left, *right;
	if ( root ) {
		left = root->left;
		right = root->right;
		free(root);
		tree_free(left);
		tree_free(right);
	}
}

void tree_walk(Tree* root, int level, int cb(int, int, void*), void* ctx) {
	
	if ( !root ) return;
	tree_walk(root->left, level+1, cb, ctx);
	cb(root->element, level, ctx);
	tree_walk(root->right, level+1, cb, ctx);
}

int count(Tree * root)
{
	
	if(!root)	return 1;
	int left, right ;
	if(root->left != NULL)	left = count(root->left);
	else left = 0;

	if(root->right != NULL)	right = count(root->right);
	else right = 0;
	
	return left+right+1;	
}


int print_element(int x, int level, void* data) {
	char* ctx = (char*)data;
	printf("%s: ", ctx);
	while(level--) printf("  ");
	printf("%d\n", x);
	return 0;
}


int main() {
	int x;
	Tree* root = NULL;
	while ( scanf("%d", &x) == 1 ) 
		root = tree_insert(root, x);
	
	if(!root) printf("0");
	else 
	{
		if(root->left == NULL) printf("0 ");
		else	printf("%d ", count(root->left));
		if(root->right == NULL) printf("0");
		else printf("%d", count(root->right));
	}
	tree_free(root);
}
