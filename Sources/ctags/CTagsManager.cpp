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
				                                                 GTK_STOCK_CANCEL, // button text
				                                                 GTK_RESPONSE_CANCEL, // response id
				                                                 GTK_STOCK_OPEN, // button text
				                                                 GTK_RESPONSE_ACCEPT, // response id
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
	EDN_INFO("try to open tag file : " << m_tagFilename.c_str());
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
	Edn::String currentFilename = myBufferManager->Get(bufferID)->GetName();
	int32_t currentLineId = 0;
	Edn::File * currentFile = new Edn::File(currentFilename);
	
}


void CTagsManager::JumpTo(void)
{
	if (NULL != m_ctagFile) {
		EdnVectorBin<int8_t> data;
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
			EDN_INFO(" OPEN the TAG file Destination : " << destinationFilename.c_str() );
			if (false == myBufferManager->Exist(destinationFilename) ) {
				// need to open the file : 
				int32_t openID = myBufferManager->Open(destinationFilename);
				SendMessage(EDN_MSG__CURRENT_CHANGE_BUFFER_ID, openID);
			} else {
				SendMessage(EDN_MSG__CURRENT_CHANGE_BUFFER_ID, myBufferManager->GetId(destinationFilename));
			}
			int32_t localId = myBufferManager->GetId(destinationFilename);
			Edn::String pattern = entry.address.pattern;
			EDN_DEBUG("try to find line with : \"" << pattern.c_str() << "\"" );
			if (pattern.Size() > 4) {
				pattern.Remove(0,2);
				pattern.Remove(pattern.Size()-1,2);
			}
			// TODO : remove '\' char when needed ...
			EDN_DEBUG("try to find line with : \"" << pattern.c_str() << "\"" );
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