/**
 *******************************************************************************
 * @file types_generique.h
 * @brief Editeur De N'ours : generique define type
 * @author Edouard DUPIN
 * @date 08/06/2010
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
 
#ifndef __TOOLS_TYPES_GENERIQUE_H__
#define __TOOLS_TYPES_GENERIQUE_H__

// includes system, malloc, EXIT_SUCCESS
#include <stdlib.h>
// includes fopen, fwrite, fseek, ftell
#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <assert.h>

#include <ewol/ewol.h>

typedef enum {
    ERR_NONE = 0,   //!< No error, luckily everything went fine
    ERR_FAIL,       //!< Miscellaneous failure
    ERR_INVAL,      //!< Invalid entry parameter
    ERR_MEM,        //!< Dynamic memory allocation failure
    ERR_TIMEOUT,    //!< Request time out
    ERR_BUSY,       //!< Element curently Busy
}erreurCode_te;


#define edn_min(elemA, elemB)					((elemA)<(elemB)) ? (elemA) : (elemB)
#define edn_max(elemA, elemB)					((elemA)<(elemB)) ? (elemB) : (elemA)
#define edn_average(minimim, elem, maximum)		((minimim)>(elem)) ? (minimim) : ((maximum)<(elem)) ? (maximum) : (elem)

#endif


