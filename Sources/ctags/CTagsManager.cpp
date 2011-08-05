/**
 *******************************************************************************
 * @file CTagsManager.cpp
 * @brief Editeur De N'ours : Ctags manager : acces to the ctags file (Sources)
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

#include "tools_debug.h"
#include "tools_globals.h"
#include "Edn.h"
#include "CTagsManager.h"
#include "ClipBoard.h"
#include "BufferManager.h"



#undef  __class__
#define __class__    "CTagsManager"

/**
 * @brief
 *
 * @param[in,out] ---
 *
 * @return ---
 *
 */
CTagsManager::CTagsManager(void) : MsgBroadcast("C-Tags Manager", EDN_CAT_CTAGS)
{
	m_tagFilename = "";
	m_tagFolderBase = "";
	m_ctagFile = NULL;
	m_historyPos = 0;
}

/**
 * @brief
 *
 * @param[in,out] ---
 *
 * @return ---
 *
 */
CTagsManager::~CTagsManager(void)
{
	if(0 != m_historyList.Size()) {
		for (int32_t iii=0; iii< m_historyList.Size(); iii++) {
			delete(m_historyList[iii]);
		}
		m_historyList.Clear();
	}
}

Edn::String CTagsManager::GetFolder(Edn::String &inputString)
{
	char tmpVal[4096];
	strncpy(tmpVal, inputString.c_str(), 4096);
	tmpVal[4096-1] = '\0';
	char *ptr = strrchr(tmpVal, '/');
	if (NULL == ptr) {
		ptr = strrchr(tmpVal, '\\');
	}
	Edn::String out = "./";
	if (NULL != ptr) {
		*ptr = '\0';
		out = tmpVal;
		out+= '/';
	}
	return out;
}

void CTagsManager::OnMessage(int32_t id, int32_t dataID)
{
	switch (id)
	{
		case EDN_MSG__BUFFER_CHANGE_CURRENT:
			m_currentSelectedID = dataID;
			break;
		case EDN_MSG__OPEN_CTAGS:
			EDN_INFO("Request opening ctag file");
			{
				GtkWidget *dialog = gtk_file_chooser_dialog_new( "Open Exuberant Ctags File", NULL,
				                                                 GTK_FILE_CHOOSER_ACTION_OPEN,
				                                                 GTK_STOCK_CANCEL, GTK_RESPONSE_CANCEL,
				                                                 GTK_STOCK_OPEN,   GTK_RESPONSE_ACCEPT,
				                                                 NULL); // end button/response list
				if (gtk_dialog_run(GTK_DIALOG (dialog)) == GTK_RESPONSE_ACCEPT)
				{
					// open the new one : 
					m_tagFilename = gtk_file_chooser_get_filename(GTK_FILE_CHOOSER (dialog));
					m_tagFolderBase = GetFolder(m_tagFilename);
					LoadTagFile();
				}
				gtk_widget_destroy(dialog);
			}
			break;
		case EDN_MSG__RELOAD_CTAGS:
			LoadTagFile();
			break;
		case EDN_MSG__JUMP_TO_CURRENT_SELECTION:
			JumpTo();
			break;
		case EDN_MSG__JUMP_BACK:
			EDN_INFO("TODO .... jump back");
			break;
			
	}
}


void CTagsManager::LoadTagFile(void)
{
	tagFileInfo info;
	
	// close previous tag file
	if (NULL != m_ctagFile) {
		tagsClose(m_ctagFile);
		m_ctagFile = NULL;
	}
	if (m_tagFilename == "") {
		return;
	}
	// load (open) the tag file : 
	EDN_INFO("try to open tag file : " << m_tagFilename);
	m_ctagFile = tagsOpen(m_tagFilename.c_str(), &info);
	if (NULL != m_ctagFile) {
		EDN_INFO("open exuberant Ctags file is OK ...");
	} else {
		EDN_INFO("Error to open ctags file ...");
	}
}

void CTagsManager::AddToHistory(int32_t bufferID)
{
	// check tho history position : remove if needed
	if (m_historyPos < edn_max(m_historyList.Size()-1, 0) ) {
		for(int32_t iii= m_historyPos; iii < m_historyList.Size(); iii++) {
			delete(m_historyList[iii]);
		}
		m_historyList.Erase(m_historyPos, m_historyList.Size() - m_historyPos);
	}
	// add the current element
	BufferManager *myBufferManager = BufferManager::getInstance();
	Edn::File currentFilename = myBufferManager->Get(bufferID)->GetFileName();
	
}

enum
{
	CTAGS_COL_FILE = 0,
	CTAGS_COL_REGEXP,
	CTAGS_NUM_COLS
};

static GtkTreeModel * create_and_fill_model(void)
{
	GtkListStore *   store;
	GtkTreeIter      iter;
	
	store = gtk_list_store_new(CTAGS_NUM_COLS, G_TYPE_STRING, G_TYPE_STRING);
	
	// Append a row and fill in some data
	gtk_list_store_append(store, &iter);
	gtk_list_store_set(store, &iter,
	                   CTAGS_COL_FILE, "file1.c",
	                   CTAGS_COL_REGEXP, "void function1(void);",
	                   -1);
	
	gtk_list_store_append(store, &iter);
	gtk_list_store_set(store, &iter,
	                   CTAGS_COL_FILE, "file2.c",
	                   CTAGS_COL_REGEXP, "void function2(void);",
	                   -1);
	
	gtk_list_store_append(store, &iter);
	gtk_list_store_set(store, &iter,
	                   CTAGS_COL_FILE, "file3.c",
	                   CTAGS_COL_REGEXP, "void function3(void);",
	                   -1);
	
	return GTK_TREE_MODEL(store);
}

static GtkWidget * create_view_and_model(void)
{
	GtkCellRenderer *   renderer;
	GtkTreeModel *      model;
	GtkWidget *         view;
	
	view = gtk_tree_view_new();
	
	// Column 1
	renderer = gtk_cell_renderer_text_new();
	gtk_tree_view_insert_column_with_attributes(GTK_TREE_VIEW (view),
	                                            -1,
	                                            "File",
	                                            renderer,
	                                            "text", CTAGS_COL_FILE,
	                                            NULL);
	
	// Column 2
	renderer = gtk_cell_renderer_text_new();
	gtk_tree_view_insert_column_with_attributes(GTK_TREE_VIEW (view),
	                                            -1,
	                                            "Regular Expression",
	                                            renderer,
	                                            "text", CTAGS_COL_REGEXP,
	                                            NULL);
	
	model = create_and_fill_model();
	
	gtk_tree_view_set_model(GTK_TREE_VIEW (view), model);
	
	g_object_unref(model);
	
	return view;
}





void CTagsManager::MultipleJump(void)
{
	// dlg to confirm the quit event : 
	GtkWidget *myDialog = gtk_dialog_new_with_buttons("C-Tags jump...",
	                                                  NULL,
	                                                  GTK_DIALOG_MODAL,
	                                                  "Jump", GTK_RESPONSE_YES,
	                                                  GTK_STOCK_QUIT, GTK_RESPONSE_NO,
	                                                  NULL);
	// Set over main windows
	//gtk_window_set_transient_for(GTK_WINDOW(myDialog), GTK_WINDOW(m_mainWindow->GetWidget()));
	// add writting area
	GtkWidget *myContentArea = gtk_dialog_get_content_area( GTK_DIALOG(myDialog));
	GtkWidget *listView = create_view_and_model ();
	gtk_box_pack_start(GTK_BOX(myContentArea), listView, TRUE, TRUE, 0);
	// Display it
	gtk_widget_show_all(myContentArea);
	int32_t result = gtk_dialog_run (GTK_DIALOG (myDialog));
	// Get data from the gtk entry
	
	// Remove dialogue
	gtk_widget_destroy(myDialog);
}

void CTagsManager::JumpTo(void)
{
	MultipleJump();
	if (NULL != m_ctagFile) {
		Edn::VectorType<int8_t> data;
		// get the middle button of the clipboard ==> represent the current selection ...
		ClipBoard::Get(COPY_MIDDLE_BUTTON, data);
		if (data.Size() == 0) {
			EDN_INFO("No current Sélection");
		}
		tagEntry entry;
		data.PushBack('\0');
		EDN_INFO("try to find the tag : " << (const char *)&data[0]);
		if (tagsFind (m_ctagFile, &entry, (const char *)&data[0], 0) == TagSuccess) {
			//EDN_INFO("find the tag");
			BufferManager *myBufferManager = BufferManager::getInstance();
			Edn::String destinationFilename = m_tagFolderBase;
			destinationFilename += entry.file;
			Edn::File myfile = destinationFilename;
			EDN_INFO(" OPEN the TAG file Destination : " << myfile );
			if (false == myBufferManager->Exist(myfile) ) {
				// need to open the file : 
				int32_t openID = myBufferManager->Open(myfile);
				SendMessage(EDN_MSG__CURRENT_CHANGE_BUFFER_ID, openID);
			} else {
				SendMessage(EDN_MSG__CURRENT_CHANGE_BUFFER_ID, myBufferManager->GetId(myfile));
			}
			int32_t localId = myBufferManager->GetId(myfile);
			Edn::String pattern = entry.address.pattern;
			EDN_DEBUG("try to find line with : \"" << pattern << "\"" );
			if (pattern.Size() > 4) {
				pattern.Remove(0,2);
				pattern.Remove(pattern.Size()-1,2);
			}
			// TODO : remove '\' char when needed ...
			EDN_DEBUG("try to find line with : \"" << pattern << "\"" );
			int32_t destLine = myBufferManager->Get(localId)->FindLine(pattern);
			SendMessage(EDN_MSG__CURRENT_GOTO_LINE, destLine);
			/*
			do {
				PrintTag (&entry);
			} while (tagsFindNext (m_ctagFile, &entry) == TagSuccess);
			*/
		} else {
			EDN_INFO("no tag find ...");
		}
	}
}

void CTagsManager::PrintTag (const tagEntry *entry)
{
	int i;
	EDN_INFO("find Tag file : name=\"" << entry->name << "\" in file=\"" << entry->file 
	<< "\" pattern=\"" <<entry->address.pattern 
	<< "\" at line="<<entry->address.lineNumber);
	
	EDN_INFO("Extention field : ");
	if (entry->kind != NULL  &&  entry->kind [0] != '\0') {
		EDN_INFO("        kind : " << entry->kind);
	}
	if (entry->fileScope) {
		EDN_INFO("        file : ");
	}
	for (i = 0  ;  i < entry->fields.count  ;  ++i) {
		EDN_INFO("               " << entry->fields.list[i].key << ":" << entry->fields.list[i].value );
	}
}