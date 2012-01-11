/**
 *******************************************************************************
 * @file tools_debug.h
 * @brief Editeur De N'ours : log implementation
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
 
#ifndef __TOOLS_DEBUG_H__
#define __TOOLS_DEBUG_H__

#include <iostream>
#include "types_generique.h"

// Log Message System For EDN
void TOOLS_DisplayFuncName(int32_t ligne, const char* className, const char* funcName);
void TOOLS_DisplayTime(void);

//regular colors
#define COLOR_BLACK				"\e[0;30m"
#define COLOR_RED				"\e[0;31m"
#define COLOR_GREEN				"\e[0;32m"
#define COLOR_YELLOW			"\e[0;33m"
#define COLOR_BLUE				"\e[0;34m"
#define COLOR_MAGENTA			"\e[0;35m"
#define COLOR_CYAN				"\e[0;36m"
#define COLOR_WHITE				"\e[0;37m"
//emphasized (bolded) colors
#define COLOR_BOLD_BLACK		"\e[1;30m"
#define COLOR_BOLD_RED			"\e[1;31m"
#define COLOR_BOLD_GREEN		"\e[1;32m"
#define COLOR_BOLD_YELLOW		"\e[1;33m"
#define COLOR_BOLD_BLUE			"\e[1;34m"
#define COLOR_BOLD_MAGENTA		"\e[1;35m"
#define COLOR_BOLD_CYAN			"\e[1;36m"
#define COLOR_BOLD_WHITE		"\e[1;37m"
//background colors
#define COLOR_BG_BLACK			"\e[40m"
#define COLOR_BG_RED			"\e[41m"
#define COLOR_BG_GREEN			"\e[42m"
#define COLOR_BG_YELLOW			"\e[43m"
#define COLOR_BG_BLUE			"\e[44m"
#define COLOR_BG_MAGENTA		"\e[45m"
#define COLOR_BG_CYAN			"\e[46m"
#define COLOR_BG_WHITE			"\e[47m"
// Return to the normal color setings
#define COLOR_NORMAL			"\e[0m"
//go to the Top of bash
#define GO_TOP					"\e[0;0f"

#undef  __class__
#define __class__				(NULL)

/*
#define DEFINE_CLASS_NAME(name) 		#undef __class__		\
										#define __class__	(#name)
at the start of the class : 
#undef __class__
#define __class__	""
*/

#define EDN_DBG_COMMON(color, info, data)	do {													\
												std::cout << color;									\
												TOOLS_DisplayTime();								\
												TOOLS_DisplayFuncName(__LINE__, __class__, __func__);			\
												std::cout << "[" << info << "] " << data;			\
												std::cout << COLOR_NORMAL <<std::endl;				\
											}while(0)

#define EDN_CRITICAL(data)				EDN_DBG_COMMON(COLOR_BOLD_RED, "CC", data)
// General 
#if EDN_DEBUG_LEVEL > 0
#   define EDN_WARNING(data)			EDN_DBG_COMMON(COLOR_MAGENTA, "WW", data)
#   define EDN_ERROR(data)				EDN_DBG_COMMON(COLOR_BOLD_RED, "EE", data)
#else
#	define EDN_WARNING(data)      do {}while(0)
#	define EDN_ERROR(data)      do {}while(0)
#endif

#if EDN_DEBUG_LEVEL > 1
#	define EDN_INFO(data)				EDN_DBG_COMMON(COLOR_CYAN, "II", data)
#else
#	define EDN_INFO(data)      do {}while(0)
#endif

#if EDN_DEBUG_LEVEL > 2
#   define EDN_DEBUG(data)				EDN_DBG_COMMON(COLOR_YELLOW, "DD", data)
#else
#	define EDN_DEBUG(data)      do {}while(0)
#endif



#if EDN_DEBUG_LEVEL > 0
#   define EDN_ASSERT(cond, format, ...)                                                    \
        do {                                                                                  \
            if (!(cond)) {                                                                    \
                EDN_CRITICAL(format, ##__VA_ARGS__);   \
                assert(!#cond);                                                               \
            }                                                                                 \
        } while (0)
#else
#   define EDN_ASSERT(cond, format, ...)            \
        do {                                        \
            assert(cond);                           \
        } while (0)
#endif

#if EDN_DEBUG_LEVEL > 1
#   define EDN_CHECK_INOUT(cond) EDN_ASSERT((cond), "Internal input error : "#cond)
#elif EDN_DEBUG_LEVEL > 0
//  Critical warning mode
#   define EDN_CHECK_INOUT(cond)                             \
        do {                                                   \
            if (!(cond)) {                                     \
                EDN_CRITICAL("Internal input error : "#cond);\
            }                                                  \
        } while (0)
#else
//  Default : No check
#   define EDN_CHECK_INOUT(cond) do { } while (0)
#endif


// Enable or disable the magic element checking...
#if EDN_DEBUG_LEVEL > 0
    #define CHECK_MAGIC(cond)            EDN_ASSERT((cond), "MAGIC check error : "#cond)
    #define EDN_ENABLE_CHECK_MAGIC     (1)
#else
    #define CHECK_MAGIC(cond)            do {}while(0)
#endif






#endif
