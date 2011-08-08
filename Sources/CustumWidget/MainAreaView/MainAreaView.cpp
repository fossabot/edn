/**
 *******************************************************************************
 * @file Search.h
 * @brief Editeur De N'ours : Search system (header)
 * @author Edouard DUPIN
 * @date 03/01/2011
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
 
#ifndef __SEARCH_H__
#define __SEARCH_H__

#include "tools_debug.h"
#include "Singleton.h"



class MainAreaView: public Singleton<MainAreaView>
{
	friend class Singleton<MainAreaView>;
	// specific for sigleton system...
	private:
		// Constructeur
		MainAreaView(void);
		~MainAreaView(void);

	public:
		GtkWidget	* GetWidget(void);
		void		  SplitV(void);			// split current window vertically (max 1 by section)
		void		  SplitH(void);			// split current window horizontaly (max 3)
		void		  Remove(void);			// remove current selected element
		//ViewArea	* GetCurent(void);		// Get a pointer on the current displayed area


	private:
		GtkWidget		* m_localDialog;				//!< local dialog element
		ViewArea		* m_EntrySearch[6];
		uint8_t			  m_CurentSelected
};


#endif

