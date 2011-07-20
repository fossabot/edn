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

extern "C" {
	// includes GDK_q
	#include <gdk/gdkkeysyms.h>
	#include <gtk/gtk.h>
}



#ifndef __int8_t_defined
#   define __int8_t_defined
    typedef   signed char                    int8_t;
    typedef   signed short int               int16_t;
    typedef   int                            int32_t;
    typedef   signed long long int           int64_t;
#endif

typedef unsigned char                   uint8_t;
typedef unsigned short int              uint16_t;
typedef unsigned long int               uint32_t;
typedef unsigned long long int          uint64_t;

typedef  bool     BOOL;

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





typedef struct {
	int32_t x;
	int32_t y;
} position_ts;



#endif


