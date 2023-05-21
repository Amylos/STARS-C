/**
 * @file container.h
 * @author your name (you@domain.com)
 * @brief C Generic Container based on linked list
 * @version 1.0
 * @date 2022-02-09
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#ifndef CONTAINER_H_
#define CONTAINER_H_
#endif

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Function pointers definition. 
 */
typedef void*(*t_ptfV)(void*);			// For functions like:  void*()(void*)
typedef void*(*t_ptfVV)(void*, void*);	// For functions like:  void*()(void*, void*)


/**
 * @brief Container type definition: "hidden structure" or "incomplete structure" definition.
 */
typedef struct s_container t_container;

/**
 * @brief  Container public functions declaration.
 */

/**
 * @brief Container "constructor".
 * 
 * @param [in] pDeleteItemFunc 	function pointer to the function responsible of 
 * 								properly deleting items in stored in container.
 * @return t_container* pointer on the newly created container.
 */
t_container*ContainerNew(t_ptfV pDeleteItemFunc);

/**
 * @brief  Container "destructor".
 * 
 * @param [in] pContainer pointer to the container to destroy.
 * @return t_container* NULL.
 */
t_container*ContainerDel(t_container *pContainer);

/**
 * @brief 	Flushing the container by deleting all items and nodes.
 * 			The container structure is not destroyed !
 * 
 * @param [in] pContainer pointer to the container to flush.
 * @return t_container* the flushed container.
 */
t_container*ContainerFlush(t_container *pContainer);

/**
 * @brief Returns the number of elements in container.
 * 
 * @param pContainer pointer to the container to get cardinal.
 * @return size_t number of elements.
 */
size_t ContainerCard(const t_container*pContainer);

/**
 * @brief Append a item at the end of the container.
 * 
 * @param [in] pContainer pointer to the container to append to.
 * @param [in] pItem pointer to the item to append.
 * @return void* pointer to the item that was appended.
 */
void*ContainerPushback(t_container*pContainer, void*pItem);

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
void*ContainerParse(const t_container*pContainer, t_ptfVV pParseFunc, void*pParam);

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
void*ContainerParseDelIf(t_container*pContainer, t_ptfVV pParseFunc, void*pParam);

void*ContainerIntersectDelIf(t_container*pContainerA, t_container*pContainerB, t_ptfVV pIntersectFunc, void*pParam);

#ifdef __cplusplus
}
#endif
