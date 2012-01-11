/**
 *******************************************************************************
 * @file toolsmemory.h
 * @brief Editeur De N'ours : Memory implementation (headers)
 * @author Edouard DUPIN
 * @date 12/01/2011
 * @par Project
 * Edn
 *
 * @par Copyright
 * Copyright 2010 Edouard DUPIN, all right reserved
 *
 * This software is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY.
 *
 * Licence summary : 
 *    You can modify and redistribute the sources code and binaries.
 *    You can send me the bug-fix
 *    You can not earn money with this Software (if the source extract from Edn
 *        represent less than 50% of original Sources)
 * Term of the licence in in the file licence.txt.
 *
 *******************************************************************************
 */
 
#ifndef __TOOLS_MEMORY_H__
#define __TOOLS_MEMORY_H__



// General 
#if EDN_MEMORY_CHECKER > 0
    void EDN_MemFree( void * pointerData, const char * variableName, const char * functionName, int32_t line, const char * fileName );
    void * EDN_MemMalloc( size_t num, size_t size, uint8_t init, const char * variableName, const char * functionName, int32_t line, const char * fileName );
    void EDN_MemShowLogs( void );
#   define EDN_MALLOC(pointerData, nbElements, dataType)              do {                                                                                                                                \
                                                                            pointerData = (dataType *)EDN_MemMalloc( (nbElements), sizeof(dataType), 0, #pointerData, __func__, __LINE__, __FILE__);    \
                                                                        }while(0)
#   define EDN_MALLOC_CAST(pointerData, nbElements, dataType, cast)   do {                                                                                                                                \
                                                                            pointerData = (cast)EDN_MemMalloc( (nbElements), sizeof(dataType), 0, #pointerData, __func__, __LINE__, __FILE__);            \
                                                                        }while(0)
#   define EDN_CALLOC(pointerData, nbElements, dataType)              do {                                                                                                                                \
                                                                            pointerData = (dataType *)EDN_MemMalloc( (nbElements), sizeof(dataType), 1, #pointerData, __func__, __LINE__, __FILE__);    \
                                                                        }while(0)
#   define EDN_CALLOC_CAST(pointerData, nbElements, dataType, cast)   do {                                                                                                                                \
                                                                            pointerData = (cast)EDN_MemMalloc( (nbElements), sizeof(dataType), 1, #pointerData, __func__, __LINE__, __FILE__);            \
                                                                        }while(0)
#   define EDN_FREE(pointerData)                                      do {                                                                            \
                                                                            EDN_MemFree( (pointerData) , #pointerData, __func__, __LINE__, __FILE__); \
                                                                            (pointerData) = NULL;                                                       \
                                                                        }while(0)
#   define EDN_MEM_SHOW_LOG()                                         do {                                                                            \
                                                                            EDN_MemShowLogs();                                                        \
                                                                        }while(0)
#else

#	define EDN_MALLOC(pointerData, nbElements, dataType)					do {																		\
																				(pointerData) = (dataType *)malloc( (nbElements) * sizeof(dataType) );	\
																			}while(0)

#	define EDN_MALLOC_CAST(pointerData, nbElements, dataType, cast)		do {																	\
																				(pointerData) = (cast)malloc( (nbElements) * sizeof(dataType) );	\
																			}while(0)

#	define EDN_CALLOC(pointerData, nbElements, dataType)					do {																		\
																				(pointerData) = (dataType *)calloc( (nbElements), sizeof(dataType) );	\
																			}while(0)

#	define EDN_CALLOC_CAST(pointerData, nbElements, dataType, cast)		do {																\
																				(pointerData) = (cast)calloc( (nbElements), sizeof(dataType) );	\
																			}while(0)

#	define EDN_REALLOC(pointerData, nbElements, dataType)					do {																						\
																				(pointerData) = (dataType *)realloc( (pointerData), (nbElements)* sizeof(dataType) );	\
																			}while(0)

#	define EDN_REALLOC_CAST(pointerData, nbElements, dataType, cast)		do {																					\
																				(pointerData) = (cast)realloc( (pointerData), (nbElements) * sizeof(dataType) );	\
																			}while(0)

#	define EDN_FREE(pointerData)	do {						\
										free( pointerData );	\
										(pointerData) = NULL;	\
									}while(0)

#	define EDN_MEM_SHOW_LOG()		do {													\
										sup_system_diag("No Memory check availlable");		\
									}while(0)
#endif


#endif

