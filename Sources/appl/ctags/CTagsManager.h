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


#include <ewol/widget/Widget.h>
#include "appl/globalMsg.h"
#include "readtags.h"

#define MAX_REG_EXP_SEARCH	(1024)

typedef struct{
	char    filename[MAX_FILE_NAME];
	char    RegExp[MAX_REG_EXP_SEARCH];
	int32_t lineID;
} TagListFind_ts;

/*
class CTagsManager: public etk::Singleton<CTagsManager>, public ewol::Widget
{
	friend class etk::Singleton<CTagsManager>;
	// specific for sigleton system...
	private:
		// Constructeur
		CTagsManager(void);
		~CTagsManager(void);

	public:
		virtual bool OnEventAreaExternal(int32_t widgetID, const char * generateEventId, const char * eventExternId, float x, float y);
	private:
		int32_t                    m_currentSelectedID;
		void                       LoadTagFile(void);
		int32_t                    MultipleJump(void);
		void                       JumpTo(void);
		void                       PrintTag(const tagEntry *entry, bool small);
		etk::UString               GetFolder(etk::UString &inputString);
		etk::UString               m_tagFolderBase;
		etk::UString               m_tagFilename;
		tagFile *                  m_ctagFile;
		// history system
		void                       AddToHistory(int32_t bufferID);
		int32_t                    m_historyPos;
		etk::Vector<etk::File*>   m_historyList;
		etk::Vector<TagListFind_ts> m_currentList;
		void                       JumpAtID(int32_t selectID);
};
*/
#endif
