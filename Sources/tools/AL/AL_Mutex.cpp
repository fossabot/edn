/**
 *******************************************************************************
 * @file AL_Mutex.c
 * @brief Editeur De N'ours : Abstraction Layer Mutex
 * @author Edouard DUPIN
 * @date 04/12/2010
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


#include "tools_debug.h"
#include "AL_Mutex.h"
// /usr/include/pthread.h


/**
 * @brief initialize the curent Mutex
 *
 * @param[in,out]  pointerMutex    Pointer on the mutex that might be init
 * @param[in]      recursive       Enable the possibility that one thread can lock multiple time the same Mutex
 *
 * @return an standard Error Code (ERR_NONE / ERR_FAIL)
 *
 */
erreurCode_te AL_mutex_init(AL_MUTEX * pointerMutex,bool recursive)
{
	int systemRet;
	pthread_mutexattr_t mutexattr;
	erreurCode_te myError= ERR_NONE;
	EDN_CHECK_INOUT(pointerMutex);
	// init mutex attributes
	systemRet = pthread_mutexattr_init(&mutexattr);
	if (0 == systemRet) {
		if (true == recursive) {
			systemRet = pthread_mutexattr_settype(&mutexattr, PTHREAD_MUTEX_RECURSIVE_NP);
			EDN_ASSERT(!systemRet, "pthread_mutexattr_settype Error");
		}
		else {
			systemRet = pthread_mutexattr_settype(&mutexattr, PTHREAD_MUTEX_ERRORCHECK_NP);
			EDN_ASSERT(!systemRet, "pthread_mutexattr_settype Error");
		}
	}
	if (0 == systemRet) {
		systemRet = pthread_mutex_init(pointerMutex, &mutexattr);
		EDN_ASSERT(!systemRet, "pthread_mutex_init Error Mutex Init");
		if (systemRet) {
			myError = ERR_FAIL;
		}
	}
	systemRet = pthread_mutexattr_destroy(&mutexattr);
	EDN_ASSERT(0 == systemRet, "pthread_mutexattr_destroy Error");
	return myError;
}


/**
 * @brief Destroy the current Mutex
 *
 * @param[in,out]  pointerMutex    Pointer on the mutex that might be init
 *
 * @return an standard Error Code (ERR_NONE / ERR_FAIL)
 *
 */
erreurCode_te AL_mutex_destroy(AL_MUTEX * pointerMutex)
{
	int systemRet;
	EDN_CHECK_INOUT(pointerMutex);
	systemRet = pthread_mutex_destroy(pointerMutex);
	EDN_ASSERT(!systemRet, "pthread_mutex_destroy Error Mutex Destroy");
	if (systemRet) {
		return ERR_FAIL;
	}
	return ERR_NONE;
}


/**
 * @brief Lock the curent Mutex. Lock call
 *
 * @param[in,out]  pointerMutex    Pointer on the mutex that might be init
 *
 * @return ---
 *
 */
void AL_mutex_lock(AL_MUTEX * pointerMutex)
{
	int systemRet;
	EDN_CHECK_INOUT(pointerMutex);
	systemRet = pthread_mutex_lock(pointerMutex);
	EDN_ASSERT(!systemRet, "pthread_mutex_lock Error Mutex lock");
}


/**
 * @brief Unlock the current Mutex
 *
 * @param[in,out]  pointerMutex    Pointer on the mutex that might be init
 *
 * @return ---
 *
 */
void AL_mutex_unlock(AL_MUTEX * pointerMutex)
{
	int systemRet;
	EDN_CHECK_INOUT(pointerMutex);
	systemRet = pthread_mutex_unlock(pointerMutex);
	EDN_ASSERT(!systemRet, "pthread_mutex_unlock Error Mutex unlock");
}


/**
 * @brief Try the lock of the curent Mutex
 *
 * @param[in,out]  pointerMutex    Pointer on the mutex that might be init
 *
 * @return an standard Error Code (ERR_NONE / ERR_BUSY)
 *
 */
erreurCode_te AL_mutex_trylock(AL_MUTEX * pointerMutex)
{
	int systemRet;
	EDN_CHECK_INOUT(pointerMutex);
	systemRet = pthread_mutex_trylock(pointerMutex);
	EDN_ASSERT(0==systemRet || EBUSY==systemRet, "pthread_mutex_trylock Error Mutex unlock");
	if (EBUSY==systemRet) {
		return ERR_BUSY;
	}
	return ERR_NONE;
}


/**
 * @brief try lock in a periode of time
 *
 * @param[in,out]  pointerMutex    Pointer on the mutex that might be init
 *
 * @return an standard Error Code (ERR_NONE / ERR_TIMEOUT)
 *
 */
erreurCode_te AL_mutex_timedlock(AL_MUTEX * pointerMutex, int32_t delay)
{
/*
	int systemRet;
	EDN_CHECK_INOUT(pointerMutex);
	if (0 == delay) {
		return ERR_NONE;
	}
	// TODO ... check is it OK...
	systemRet = pthread_mutex_timedlock(pointerMutex, delay);
	EDN_ASSERT(0 == systemRet || ETIMEDOUT == systemRet, "pthread_mutex_timedlock Error");
	if (ETIMEDOUT == systemRet) {
		return ERR_TIMEOUT;
	}
*/
	return ERR_NONE;
}







