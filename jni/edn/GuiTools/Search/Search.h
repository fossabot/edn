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


class Search: public Singleton<Search>
{
	friend class Singleton<Search>;
	// specific for sigleton system...
	private:
		// Constructeur
		Search(void);
		~Search(void);

	public:
		void Destroy(void);
		void Display(GtkWindow *parent);
		void Hide(void);

	private:
		GtkWidget * m_localDialog;          //!< local dialog element
		// entry
		GtkWidget * m_searchLabel;          //!< gtk label of the search section
		GtkWidget * m_searchEntry;          //!< gtk entry of the search section
		GtkWidget * m_replaceLabel;         //!< gtk label of the replace section
		GtkWidget * m_replaceEntry;         //!< gtk entry of the replace section
		// checkbox
		GtkWidget * m_CkMatchCase;          //!< tick of the case matching
		GtkWidget * m_CkWrapAround;         //!< tick of the wrap the file
		GtkWidget * m_CkRegularExpression;  //!< the test is a regular expression
		// button
		GtkWidget * m_BtPrevious;           //!< Button Previous
		GtkWidget * m_BtNext;               //!< Button Next
		GtkWidget * m_BtReplace;            //!< Button Replace
		GtkWidget * m_BtReplaceAndNext;     //!< Button Replace and find next
		GtkWidget * m_BtQuit;               //!< Button Quit
		bool m_haveSearchData;
		bool m_haveReplaceData;
	// CallBack for GTK+ gui
	private :
		static void OnButtonPrevious(GtkWidget *widget, gpointer user_data);
		static void OnButtonNext(GtkWidget *widget, gpointer user_data);
		static void OnButtonReplace(GtkWidget *widget, gpointer user_data);
		static void OnButtonReplaceAndNext(GtkWidget *widget, gpointer user_data);
		static void OnButtonQuit(GtkWidget *widget, gpointer user_data);
		static void OnCheckBoxEventCase(GtkWidget *widget, gpointer user_data);
		static void OnCheckBoxEventWrap(GtkWidget *widget, gpointer user_data);
		static void OnCheckBoxEventRegExp(GtkWidget *widget, gpointer user_data);
		static void OnEntrySearchChange(GtkWidget *widget, gpointer user_data);
		static void OnEntryReplaceChange(GtkWidget *widget, gpointer user_data);
		
};


#endif


