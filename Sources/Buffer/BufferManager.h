/**
 *******************************************************************************
 * @file BufferManager.h
 * @brief Editeur De N'ours : Text Buffer (header)
 * @author Edouard DUPIN
 * @date 08/12/2010
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
 
#ifndef __BUFFER_MANAGER_H__
#define __BUFFER_MANAGER_H__

#include "Buffer.h"
#include "BufferText.h"
#include "BufferEmpty.h"
#include "Singleton.h"
#include "MsgBroadcast.h"
#include <vector>
#include <string>

class BufferManager: public Singleton<BufferManager>, public MsgBroadcast
{
	friend class Singleton<BufferManager>;
	// specific for sigleton system...
	private:
		// Constructeur
		BufferManager(void);
		~BufferManager(void);

	public:
		void OnMessage(int32_t id, int32_t dataID);
		
		// return the ID of the buffer allocated
		// create a buffer with no element
		int32_t		Create(void);
		// open curent filename
		int32_t		Open(Edn::String &filename);
		int32_t     GetSelected(void) { return m_idSelected;};
		void        SetSelected(int32_t id) {m_idSelected = id;};
		Buffer		* Get(int32_t BufferID);
		bool		Exist(int32_t BufferID);
		bool		Exist(Edn::String &filename);
		int32_t		GetId(Edn::String &filename);
		// return the number of buffer (open in the past) if 5 buffer open and 4 close ==> return 5
		uint32_t	Size(void);
		int32_t		WitchBuffer(int32_t iEmeElement);
		
		bool		Remove(int32_t BufferID);

	private:
		
		std::vector<Buffer*> listBuffer;						//!< element List of the char Elements
		
		void	RemoveAll(void);						//!< remove all buffer
		int32_t m_idSelected;
		Buffer * BufferNotExiste;						//!< When an error arrive in get buffer we return the Error buffer (not writable)
};

#endif

