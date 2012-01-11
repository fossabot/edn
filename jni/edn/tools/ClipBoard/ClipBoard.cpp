/**
 *******************************************************************************
 * @file ClipBoard.cpp
 * @brief Editeur De N'ours : copy / past main system
 * @author Edouard DUPIN
 * @date 05/12/2010
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

#include "tools_globals.h"
#include "ClipBoard.h"

#undef __class__
#define __class__	"ClipBoard"

/*
note: la copy dans le :
        0 : copy standard
   [1..9] : copy interne
       10 : bouton du milieux
*/
static Edn::VectorType<int8_t> mesCopy[TOTAL_OF_CLICKBOARD];


void ClipBoard::Init(void)
{
	EDN_INFO("Initialyse ClipBoards");
	for(int32_t i=0; i<TOTAL_OF_CLICKBOARD; i++) {
		mesCopy[i].Clear();
	}
}


void ClipBoard::Set(uint8_t clipboardID, Edn::VectorType<int8_t> &data)
{
	// check if ID is correct
	if(clipboardID >= TOTAL_OF_CLICKBOARD) {
		EDN_WARNING("request ClickBoard id error");
	} else if(0 == data.Size()) {
		EDN_INFO("request a copy of nothing");
	} else if (COPY_STD == clipboardID) {
		GtkClipboard * clipboard = gtk_clipboard_get(GDK_SELECTION_CLIPBOARD);
		gtk_clipboard_set_text(clipboard, (const gchar*)&data[0], data.Size() );
	} else if (COPY_MIDDLE_BUTTON == clipboardID) {
		GtkClipboard * clipboard = gtk_clipboard_get(GDK_SELECTION_PRIMARY);
		gtk_clipboard_set_text(clipboard, (const gchar*)&data[0], data.Size() );
	}
	// Copy datas ...
	mesCopy[clipboardID] = data;
}


void ClipBoard::Get(uint8_t clipboardID, Edn::VectorType<int8_t> &data)
{
	if(clipboardID >= TOTAL_OF_CLICKBOARD) {
		EDN_WARNING("request ClickBoard id error");
	} else if (COPY_STD == clipboardID) {
		GtkClipboard * clipboard = gtk_clipboard_get(GDK_SELECTION_CLIPBOARD );
		gchar *text = gtk_clipboard_wait_for_text(clipboard);
		if (text != NULL) {
			mesCopy[COPY_STD].Clear();
			mesCopy[COPY_STD].PushBack((int8_t*)text, strlen(text) );
		}
	} else if (COPY_MIDDLE_BUTTON == clipboardID) {
		GtkClipboard * clipboard = gtk_clipboard_get(GDK_SELECTION_PRIMARY );
		gchar *text = gtk_clipboard_wait_for_text(clipboard);
		if (text != NULL) {
			mesCopy[COPY_MIDDLE_BUTTON].Clear();
			mesCopy[COPY_MIDDLE_BUTTON].PushBack((int8_t*)text, strlen(text) );
		}
	}
	// Copy datas ...
	data = mesCopy[clipboardID];
}





