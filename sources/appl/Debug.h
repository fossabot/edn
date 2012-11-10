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

extern const char * applLog;

#define APPL_CRITICAL(data)			ETK_CRITICAL(applLog, data)
#define APPL_WARNING(data)			ETK_WARNING(applLog, data)
#define APPL_ERROR(data)			ETK_ERROR(applLog, data)
#define APPL_INFO(data)				ETK_INFO(applLog, data)
#define APPL_DEBUG(data)			ETK_DEBUG(applLog, data)
#define APPL_VERBOSE(data)			ETK_VERBOSE(applLog, data)
#define APPL_ASSERT(cond, data)		ETK_ASSERT(applLog, cond, data)
#define APPL_CHECK_INOUT(cond)		ETK_CHECK_INOUT(applLog, cond)
#define APPL_TODO(cond)				ETK_TODO(applLog, cond)

#endif
