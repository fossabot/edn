/**
 *******************************************************************************
 * @file HighlightManager.h
 * @brief Editeur De N'ours : Hightlining Manager (header)
 * @author Edouard DUPIN
 * @date 14/12/2010
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
 
#ifndef __HIGHLIGHT_MANAGER_H__
#define __HIGHLIGHT_MANAGER_H__

class HighlightManager;

#include <etk/Singleton.h>
#include <MsgBroadcast.h>
#include <Highlight.h>


class HighlightManager: public etk::Singleton<HighlightManager>, public MsgBroadcast
{
	friend class etk::Singleton<HighlightManager>;
	// specific for sigleton system...
	private:
		// Constructeur
		HighlightManager(void);
		~HighlightManager(void);

	public:
		void    OnMessage(int32_t id, int32_t dataID);
	public:
		void loadLanguages(void);
		Highlight	* Get(etk::File &fileName);
		bool		  Exist(etk::File &fileName);

	private:
		etk::VectorType<Highlight*> listHighlight;		//!< List of ALL hightlight modules
};

#endif

