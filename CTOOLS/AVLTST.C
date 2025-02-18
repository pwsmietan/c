/* Simple program to test the AVL tree routines */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <malloc.h>
#include "debug.h"
#include "avl.h"

AVL_TREE	*tree;

typedef struct {
	int		key;
	} REC;

int my_cmp(REC *r1,REC *r2)
{
	return r1->key - r2->key;
}

void my_prnt(FILE* out,REC *r)
{
	fprintf(out, r ? "%3d" : "   ",r->key);
}

void my_visit(void *params,REC *r)
{
	printf("%d\n",r->key);
}

void docmd(int cmd,int v1,int v2)
{
	REC		*p,*p2;
	REC		r,r2;

	switch (cmd) {
		case 'd':
		case 'D':
			r.key = v1;
			if ((p = avl_delete(tree,&r)) == NULL)
				printf("Node is NOT in tree\n");
			else {
				printf("Deleted node (%d).\n",p->key);
				avl_freenode(p);
				}
			break;
		case 'n':
		case 'N':
			if ((p = avl_delmin(tree)) == NULL)
				printf("Tree is empty\n");
			else {
				printf("Removed node (%d).\n",p->key);
				avl_freenode(p);
				}
			break;
		case 'm':
		case 'M':
			if ((p = avl_delmax(tree)) == NULL)
				printf("Tree is empty\n");
			else {
				printf("Removed node (%d).\n",p->key);
				avl_freenode(p);
				}
			break;
		case 'f':
		case 'F':
			r.key = v1;
			if ((p = avl_findsym(tree,&r)) != NULL)
				printf("Node %d found\n", r.key);
			else
				printf("Node NOT found\n");
			break;
		case 'r':
		case 'R':
			r.key = v1;	r2.key = v2;
			printf("Range searching [%d,%d]:\n",v1,v2);
			avl_range(tree,&r,&r2,my_visit,NULL);
			break;
		case 'i':
		case 'I':
			if(!(p = avl_newnode(sizeof(REC))))
				printf("Out of memory\n");
			else {
				p->key = v1;
				if ((p2 = avl_insert(tree,p)) != NULL) {
					fprintf(stderr,"%d is already in the tree\n",p2->key);
					avl_freenode(p);
					}
				}
			break;
		case 'a':
		case 'A':
			avl_empty(tree,avl_freenode);
			break;
		case 'q':
		case 'Q':
			exit(0);
		default:
			printf("Invalid command...\n");
		}

	avl_print(stdout,tree,my_prnt,false);
	printf("\n");
}

void main(int argc,char *argv[])
{
	char		buf[255];
	int			v1,v2;

	tree = avl_init(my_cmp);
	for(++argv; --argc > 0; ++argv) {
		sscanf(*argv+1,"%d,%d",&v1,&v2);
		docmd(**argv,v1,v2);
		}

	printf("commands are: iN   - Insert node N into tree\n");
	printf("              dN   - Delete node N\n");
	printf("              m    - Delete maximum valued node\n");
	printf("              n    - Delete minimum valued node\n");
	printf("              fN   - Find node N\n");
	printf("              rL,U - Range search the range [L,U]\n");
	printf("              a    - Delete entire tree\n");
	printf("              q    - Quit (exit)\n");

	for(; gets(buf); printf("i/d/m/n/f/r/a/q: ") ) {
		sscanf(buf+1,"%d,%d",&v1,&v2);
		docmd(*buf,v1,v2);
		}
}
