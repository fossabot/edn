/**
 *******************************************************************************
 * @file CTagsManager.h
 * @brief Editeur De N'ours : Ctags manager : acces to the ctags file (header)
 * @author Edouard DUPIN
 * @date 15/07/2011
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
 
#ifndef __C_TAGS_MANAGER_H__
#define __C_TAGS_MANAGER_H__


#include "Singleton.h"
#include "MsgBroadcast.h"
#include "readtags.h"
#include "Edn.h"


class CTagsManager: public Singleton<CTagsManager>, public MsgBroadcast
{
	friend class Singleton<CTagsManager>;
	// specific for sigleton system...
	private:
		// Constructeur
		CTagsManager(void);
		~CTagsManager(void);

	public:
		void                       OnMessage(int32_t id, int32_t dataID);
	private:
		int32_t                    m_currentSelectedID;
		void                       LoadTagFile(void);
		void                       JumpTo(void);
		void                       PrintTag(const tagEntry *entry);
		Edn::String                GetFolder(Edn::String &inputString);
		Edn::String                m_tagFolderBase;
		Edn::String                m_tagFilename;
		tagFile *                  m_ctagFile;
		// history system
		void                       AddToHistory(int32_t bufferID);
		int32_t                    m_historyPos;
		EdnVectorBin<Edn::File*>   m_historyList;
};

#endif
