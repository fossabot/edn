/**
 *******************************************************************************
 * @file Debug.h
 * @brief Application log implementation
 * @author Edouard DUPIN
 * @date 10/04/2012
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
 
#ifndef __DEBUG_H__
#define __DEBUG_H__

#include <etk/Types.h>
#include <etk/Debug.h>

extern const char * ednLog;

#define APPL_CRITICAL(data)         ETK_CRITICAL(ednLog, data)
// General 
#if APPL_DEBUG_LEVEL > 0
#	define APPL_WARNING(data)       ETK_WARNING(ednLog, data)
#	define APPL_ERROR(data)         ETK_ERROR(ednLog, data)
#else
#	define APPL_WARNING(data)       do {}while(0)
#	define APPL_ERROR(data)         do {}while(0)
#endif

#if APPL_DEBUG_LEVEL > 1
#	define APPL_INFO(data)          ETK_INFO(ednLog, data)
#else
#	define APPL_INFO(data)          do {}while(0)
#endif

#if APPL_DEBUG_LEVEL > 2
#	define APPL_DEBUG(data)         ETK_DEBUG(ednLog, data)
#else
#	define APPL_DEBUG(data)         do {}while(0)
#endif

#if APPL_DEBUG_LEVEL > 3
#	define APPL_VERBOSE(data)         ETK_VERBOSE(ednLog, data)
#else
#	define APPL_VERBOSE(data)         do {}while(0)
#endif

#define APPL_TODO(data)             ETK_WARNING(ednLog, "TODO : " << data)

#define APPL_ASSERT(cond, data)     ETK_ASSERT(ednLog, cond, data)

#if APPL_DEBUG_LEVEL > 1
#	define APPL_CHECK_INOUT(cond)   ETK_CHECK_INOUT_ASSERT(ednLog, cond)
#elif APPL_DEBUG_LEVEL > 0
#	define APPL_CHECK_INOUT(cond)   ETK_CHECK_INOUT_WARNING(ednLog, cond)
#else
#	define APPL_CHECK_INOUT(cond)   do { } while (0)
#endif





#endif
