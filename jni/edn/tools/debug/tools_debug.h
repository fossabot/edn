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

#include <etk/Types.h>
#include <etk/Debug.h>

extern const char * ednLog;

#define EDN_CRITICAL(data)         ETK_CRITICAL(ednLog, data)
// General 
#if EDN_DEBUG_LEVEL > 0
#	define EDN_WARNING(data)       ETK_WARNING(ednLog, data)
#	define EDN_ERROR(data)         ETK_ERROR(ednLog, data)
#else
#	define EDN_WARNING(data)       do {}while(0)
#	define EDN_ERROR(data)         do {}while(0)
#endif

#if EDN_DEBUG_LEVEL > 1
#	define EDN_INFO(data)          ETK_INFO(ednLog, data)
#else
#	define EDN_INFO(data)          do {}while(0)
#endif

#if EDN_DEBUG_LEVEL > 2
#	define EDN_DEBUG(data)         ETK_DEBUG(ednLog, data)
#else
#	define EDN_DEBUG(data)         do {}while(0)
#endif

#if EDN_DEBUG_LEVEL > 3
#	define EDN_VERBOSE(data)         ETK_VERBOSE(ednLog, data)
#else
#	define EDN_VERBOSE(data)         do {}while(0)
#endif

#define EDN_TODO(data)             ETK_WARNING(ednLog, "TODO : " << data)

#define EDN_ASSERT(cond, data)     ETK_ASSERT(ednLog, cond, data)

#if EDN_DEBUG_LEVEL > 1
#	define EDN_CHECK_INOUT(cond)   ETK_CHECK_INOUT_ASSERT(ednLog, cond)
#elif EDN_DEBUG_LEVEL > 0
#	define EDN_CHECK_INOUT(cond)   ETK_CHECK_INOUT_WARNING(ednLog, cond)
#else
#	define EDN_CHECK_INOUT(cond)   do { } while (0)
#endif





#endif
