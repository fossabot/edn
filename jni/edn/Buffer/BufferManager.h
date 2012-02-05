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

#include <Buffer.h>
#include <BufferText.h>
#include <BufferEmpty.h>
#include <etk/Singleton.h>
#include <MsgBroadcast.h>
#include <ewol/Widget.h>

class BufferManager: public etk::Singleton<BufferManager>, public ewol::Widget
{
	friend class etk::Singleton<BufferManager>;
	// specific for sigleton system...
	private:
		// Constructeur
		BufferManager(void);
		~BufferManager(void);

	public:
		bool OnEventAreaExternal(int32_t widgetID, const char * generateEventId, const char * eventExternId, etkFloat_t x, etkFloat_t y);
	private:
		// return the ID of the buffer allocated
		// create a buffer with no element
		int32_t     Create(void);
		// open curent filename
		int32_t     Open(etk::File &myFile);
		bool        Remove(int32_t BufferID);
	public:
		int32_t     GetSelected(void) { return m_idSelected;};
		void        SetSelected(int32_t id) {m_idSelected = id;};
		Buffer *    Get(int32_t BufferID);
		bool        Exist(int32_t BufferID);
		bool        Exist(etk::File &myFile);
		int32_t     GetId(etk::File &myFile);
		// return the number of buffer (open in the past) if 5 buffer open and 4 close ==> return 5
		uint32_t    Size(void);
		int32_t     WitchBuffer(int32_t iEmeElement);
		

	private:
		
		etk::VectorType<Buffer*> listBuffer;  //!< element List of the char Elements
		
		void        RemoveAll(void);          //!< remove all buffer
		int32_t     m_idSelected;
		Buffer *    BufferNotExiste;          //!< When an error arrive in get buffer we return the Error buffer (not writable)
};

#endif

