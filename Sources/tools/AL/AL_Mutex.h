/**
 *******************************************************************************
 * @file AL_Mutex.h
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
 
#ifndef __AL_MUTEX_H__
#define __AL_MUTEX_H__

//basic mutex with pthread system
#include <pthread.h>
#include <errno.h>


typedef pthread_mutex_t     AL_MUTEX;

erreurCode_te AL_mutex_init(AL_MUTEX * pointerMutex,bool recursive);
erreurCode_te AL_mutex_destroy(AL_MUTEX * pointerMutex);
void          AL_mutex_lock(AL_MUTEX * pointerMutex);
void          AL_mutex_unlock(AL_MUTEX * pointerMutex);
erreurCode_te AL_mutex_trylock(AL_MUTEX * pointerMutex);
//erreurCode_te AL_mutex_timedlock(AL_MUTEX * pointerMutex, int32_t delay);

#endif


