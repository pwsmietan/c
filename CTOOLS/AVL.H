/****************************************************************************
*
*					Copyright (C) 1991 Kendall Bennett.
*							All rights reserved.
*
* Filename:		$RCSfile: avl.h $
* Version:		$Revision: 1.3 $
*
* Language:		ANSI C
* Environment:	any
*
* Description:	Header file for AVL tree module.
*
* $Id: avl.h 1.3 91/12/31 19:41:40 kjb Exp $
*
* Revision History:
* -----------------
*
* $Log:	avl.h $
* Revision 1.3  91/12/31  19:41:40  kjb
* Modified include file directories.
* 
* Revision 1.2  91/09/27  03:11:19  kjb
* Added compatibility with C++.
* 
* Revision 1.1  91/09/27  02:50:21  kjb
* Initial revision
* 
****************************************************************************/

#ifndef	__AVL_H
#define	__AVL_H

#ifndef	__DEBUG_H
#include "debug.h"
#endif

/*---------------------- Macros and type definitions ----------------------*/

typedef struct AVL_BUCKET {
	struct AVL_BUCKET	*left;		/* Pointer to left subtree			*/
	struct AVL_BUCKET	*right;		/* Pointer to right subtree			*/
	short				bal;		/* Balance factor					*/
	} AVL_BUCKET;

typedef struct {
	int			count;			/* Number of elements currently in tree	*/
	int			(*cmp)(void*,void*);	/* Compare two nodes			*/
	AVL_BUCKET	*root;			/* Pointer to root node of AVL tree		*/
	} AVL_TREE;

/* The three traversal orders supported	*/

#define	PREORDER	0
#define	INORDER		1
#define	POSTORDER	2

#define	MAXLEVEL	64			/* Maximum levels for avl_print			*/

/* The three balance factors stored in each subtree						*/

#define	LH		0				/* Left subtree is larger				*/
#define	BAL		1				/* Subtree is balanced					*/
#define	RH		2				/* Right subtree is balanced			*/

/* Return a pointer to the user space given the address of the header of
 * a node.
 */

#define	AVL_USERSPACE(h)	((void*)((AVL_BUCKET*)(h) + 1))

/* Return a pointer to the header of a node, given the address of the
 * user space.
 */

#define	AVL_HEADER(n)		((AVL_BUCKET*)(n) - 1)

#define	AVL_EMPTY(t)		((t)->count == 0)

/*-------------------------- Function Prototypes --------------------------*/

#ifdef __cplusplus
extern "C" {
#endif

void *avl_newnode(int size);
void avl_freenode(void *node);
AVL_TREE *avl_init(int (*cmp_function)());
void avl_empty(AVL_TREE *t,void (*freeNode)());
void *avl_insert(AVL_TREE *tree,void *node);
void *avl_delete(AVL_TREE *tree,void *node);
void avl_traverse(AVL_TREE *tree,int order,void (*visit)(),void *params);
void avl_print(FILE *out,AVL_TREE *tree,void (*prnt)(),bool graph_chars);
void *avl_findsym(AVL_TREE *tree,void *node);
void avl_range(AVL_TREE *tree,void *lower,void *upper,void (*visit)(),
	void *params);
void *avl_delmin(AVL_TREE *tree);
void *avl_delmax(AVL_TREE *tree);

#ifdef __cplusplus
}
#endif

#endif
