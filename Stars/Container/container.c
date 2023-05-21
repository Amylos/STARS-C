/**
 * @file container.c
 * @author your name (you@domain.com)
 * @brief 
 * @version 1.0
 * @date 2022-02-09
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "container.h"

/**
 * @brief type definition of node structure
 * 		  Node component: Hidden structure
 */
typedef struct s_node{
	struct s_node *m_pPrev;		/* Pointer to the previous node in the current list */
	struct s_node *m_pNext;		/* Pointer to the next node in the current list		*/
			 void *m_pItem;		/* Pointer to the item stored at this node in the current list */
}t_node;

/* Node component: Hidden functions---------------------------------------------------------*/
/**
 * @brief Node "constructor".
 * 
 * @param [in] pPrev pointer to the previous node in the linked list.
 * @param [in] pNext pointer to the next node in the linked list.
 * @param [in] pItem pointer to the item to be linked to the node.
 * @return t_node* pointer to the mewly created node.
 */
t_node*NodeNew(t_node*pPrev, t_node*pNext, void*pItem){
	/**
	 * @todo Implementing node "constructor".
	 * 
	 */
	t_node*pNode = (t_node*)malloc(sizeof(t_node));
	assert(pNode);

	*pNode=(t_node){
		.m_pPrev = pPrev,
		.m_pNext = pNext,
		.m_pItem = pItem,
	};

	if(pPrev) pPrev->m_pNext = pNode;
	if(pNext) pNext->m_pPrev = pNode;

	return pNode;
}

/**
 * @brief Node "destructor".
 * 		  This version will return the next node to the deleted current node.
 * 
 * @param [in] pNodeToDel pointer to the node to delete.
 * @param [in] pDeleteFunc pointer to the item destruction function.
 * @return t_node* pointer to the next node to this one.
 */
t_node*NodeDelReturnNext(t_node*pNodeToDel, t_ptfV pDeleteFunc){
	/**
	 * @todo Implementing node "destructor".
	 * 
	 */

	if(pDeleteFunc){
		pDeleteFunc(pNodeToDel->m_pItem);
	}
	else{
		free(pNodeToDel->m_pItem);
	}

	t_node*pNodeNext = pNodeToDel->m_pNext;


	if(pNodeToDel->m_pNext) pNodeToDel->m_pNext->m_pPrev = pNodeToDel->m_pPrev;
	if(pNodeToDel->m_pPrev) pNodeToDel->m_pPrev->m_pNext = pNodeToDel->m_pNext;

	free(pNodeToDel);
	return pNodeNext;
}


/**
 * @brief type definition of container structure.
 * 		  Container component: Hidden structure.
 */
struct s_container{
	t_node *m_pHead;			/* Pointer to the first node of the container list: also known as HEAD of list.	*/
	t_node *m_pTail;			/* Pointer to the  last node of the container list: also known as TAIL of list.	*/
	size_t  m_szCard;			/* Number of total nodes in container: also known as CARDINAL of list.			*/
	t_ptfV  m_pDeleteItemFunc;	/* Function pointer to the function responsible for item destruction.			*/
};

/* Container component: Public functions implementation---------------------------------------------------------*/

/**
 * @brief Container "constructor".
 * 
 * @param [in] pDeleteItemFunc 	function pointer to the function responsible of 
 * 								properly deleting items in stored in container.
 * @return t_container* pointer on the newly created container.
 */
t_container* ContainerNew(t_ptfV pDeleteItemFunc){
	/**
	 * @todo Implementing container "constructor".
	 * 
	 */


	t_container*pContainer = (t_container*)malloc(sizeof(t_container));
	assert(pContainer);

	*pContainer = (t_container){
		.m_pDeleteItemFunc = pDeleteItemFunc,
	};


	return pContainer;
}

/**
 * @brief  Container "destructor".
 * 
 * @param [in] pContainer pointer to the container to destroy.
 * @return t_container* NULL.
 */
t_container* ContainerDel(t_container *pContainer){
	assert(pContainer);
	/**
	 * @todo Implementing container "destructor".
	 * 
	 */

	free(ContainerFlush(pContainer));

	return NULL;
}

/**
 * @brief 	Flushing the container by deleting all items and nodes.
 * 			The container structure is not destroyed !
 * 
 * @param [in] pContainer pointer to the container to flush.
 * @return t_container* the flushed container.
 */
t_container*ContainerFlush(t_container *pContainer){
	assert(pContainer);
	/**
	 * @todo Implementing container flush.
	 * 
	 */

	while(pContainer->m_pHead){
		pContainer->m_pHead = NodeDelReturnNext(pContainer->m_pHead, pContainer->m_pDeleteItemFunc);
		pContainer->m_szCard--;
	}
	assert(pContainer->m_szCard == 0);
	pContainer->m_pTail = NULL;

	return pContainer;
}

/**
 * @brief Returns the number of elements in container.
 * 
 * @param pContainer pointer to the container to get cardinal.
 * @return size_t number of elements.
 */
size_t ContainerCard(const t_container*pContainer){
	assert(pContainer);
	/**
	 * @todo Implementing container cardinal.
	 * 
	 */
	return pContainer->m_szCard;
}

/**
 * @brief Append a item at the end of the container.
 * 
 * @param [in] pContainer pointer to the container to append to.
 * @param [in] pItem pointer to the item to append. 
 * @return void* pointer to the item that was appended.
 */
void*ContainerPushback(t_container*pContainer, void*pItem){
	assert(pContainer);
	/**
	 * @todo Implementing container pushback.
	 * 
	 */

	if(pContainer->m_szCard==0){
		assert(pContainer->m_pHead==NULL);
		assert(pContainer->m_pTail==NULL);
		pContainer->m_pHead=pContainer->m_pTail=NodeNew(NULL, NULL, pItem);
	}
	else{
		pContainer->m_pTail=NodeNew(pContainer->m_pTail, NULL, pItem);
	}
	pContainer->m_szCard++;
	assert(pContainer->m_pTail->m_pItem==pItem);
	return pContainer->m_pTail->m_pItem;

	return NULL;
}

/**
 * @brief Parsing the container from front to back, and for each item, calling
 * 		  the parsing function with parameters the pointer to the corresponding
 * 		  item and the pParam parameter.
 * 		  The parsing is stopped if the parsing function returns a non null value,
 * 		  and in this case, the function returns the corresponding item to the caller.
 * 
 * @param [in] pContainer pointer to the container to parse.
 * @param [in] pParseFunc pointer to the called function for each parsed item.
 * @param [in] pParam parameter directly passed to the called function during parsing.
 * @return void* NULL in case of a complete parsing,
 * 				 pointer to the item in case of interrupted parsing.
 */
void*ContainerParse(const t_container*pContainer, t_ptfVV pParseFunc, void*pParam){
	assert(pContainer);
	assert(pParseFunc);
	/**
	 * @todo Implementing container parse.
	 * 
	 */

	t_node*pParse=pContainer->m_pHead;
	while(pParse){
		if(pParseFunc(pParse->m_pItem, pParam)) return pParse->m_pItem;
		pParse=pParse->m_pNext;
	}
	return NULL;
}

/**
 * @brief Parsing the container from front to back, and for each item, calling
 * 		  the parsing function with parameters the pointer to the corresponding
 * 		  item and the pParam parameter.
 * 		  In case of a non null return value from called function, the item and the 
 * 		  corresponding node are destroyed. The parsing is then resumed to the next
 * 		  item and the same scenario takes place, until container back is reached.
 * 
 * @param [in] pContainer pointer to the container to parse.
 * @param [in] pParseFunc pointer to the called function for each parsed item.
 * @param [in] pParam parameter directly passed to the called function during parsing.
 * @return void* NULL (complete parsing).
 */
void*ContainerParseDelIf(t_container*pContainer, t_ptfVV pParseFunc, void*pParam){
	assert(pContainer);
	assert(pParseFunc);
	/**
	 * @todo Implementing container parse and delete if condition matches.
	 * 
	 */

	t_node*pParse=pContainer->m_pHead;
	while(pParse){
		if(pParseFunc(pParse->m_pItem, pParam)){
			if(pContainer->m_pHead==pParse) pContainer->m_pHead=pParse->m_pNext;
			if(pContainer->m_pTail==pParse) pContainer->m_pTail=pParse->m_pPrev;
			pParse=NodeDelReturnNext(pParse, pContainer->m_pDeleteItemFunc);
			pContainer->m_szCard--;
		}
		else{
			pParse=pParse->m_pNext;
		}
	}
	return NULL;
}


void*ContainerIntersectDelIf(t_container*pContainerA, t_container*pContainerB, t_ptfVV pIntersectFunc, void*pParam){

	assert(pContainerA);
	assert(pContainerB);
	assert(pIntersectFunc);

	t_node*pParseA = pContainerA->m_pHead;
	t_node*pParseB = pContainerB->m_pHead;

	int iHasIntersect;

	while(pParseA && pParseB){
		iHasIntersect = 0;
		while(pParseA && pParseB){
			if(pIntersectFunc(pParseA->m_pItem,pParseB->m_pItem)){
				iHasIntersect = 1;

				if(pParseA == pContainerA->m_pHead) pContainerA->m_pHead = pParseA->m_pNext;
				if(pParseA == pContainerA->m_pTail) pContainerA->m_pTail = pParseB->m_pPrev;
				pParseA = NodeDelReturnNext(pParseA, pContainerA->m_pDeleteItemFunc);
				pContainerA->m_szCard--;

				if(pParseB == pContainerB->m_pHead) pContainerB->m_pHead = pParseB->m_pNext;
				if(pParseB == pContainerB->m_pTail) pContainerB->m_pTail = pParseB->m_pPrev;
				pParseB = NodeDelReturnNext(pParseB, pContainerB->m_pDeleteItemFunc);
				pContainerB->m_szCard--;
			}
			else{
				pParseB = pParseB->m_pNext;
			}
		}
		if(iHasIntersect){
			pParseA = pParseA->m_pNext;
		}
		pParseB = pParseB->m_pNext;
	}
	return NULL;
}

