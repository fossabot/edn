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

#include <tools_debug.h>
#include <tools_globals.h>
#include <CTagsManager.h>
#include <ClipBoard.h>
#include <BufferManager.h>



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
	/*
	if(0 != m_historyList.Size()) {
		for (int32_t iii=0; iii< m_historyList.Size(); iii++) {
			delete(m_historyList[iii]);
		}
		m_historyList.Clear();
	}
	*/
}

etk::String CTagsManager::GetFolder(etk::String &inputString)
{
	/*
	char tmpVal[4096];
	strncpy(tmpVal, inputString.c_str(), 4096);
	tmpVal[4096-1] = '\0';
	char *ptr = strrchr(tmpVal, '/');
	if (NULL == ptr) {
		ptr = strrchr(tmpVal, '\\');
	}
	etk::String out = "./";
	if (NULL != ptr) {
		*ptr = '\0';
		out = tmpVal;
		out+= '/';
	}
	return out;
	*/
	etk::String out = "./";
	return out;
}

void CTagsManager::OnMessage(int32_t id, int32_t dataID)
{
	/*
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
			if (m_historyList.Size() > 0) {
				BufferManager *myBufferManager = BufferManager::getInstance();
				int32_t id = m_historyList.Size()-1;
				if (false == myBufferManager->Exist(*m_historyList[id]) ) {
					// need to open the file : 
					int32_t openID = myBufferManager->Open(*m_historyList[id]);
					SendMessage(EDN_MSG__CURRENT_CHANGE_BUFFER_ID, openID);
				} else {
					SendMessage(EDN_MSG__CURRENT_CHANGE_BUFFER_ID, myBufferManager->GetId(*m_historyList[id]));
				}
				SendMessage(EDN_MSG__CURRENT_GOTO_LINE, m_historyList[id]->GetLineNumber());
				// Remove element ....
				delete(m_historyList[id]);
				m_historyList[id]=NULL;
				m_historyList.PopBack();
			}
			break;
	}
	*/
}


void CTagsManager::LoadTagFile(void)
{
	/*
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
	*/
}

void CTagsManager::AddToHistory(int32_t bufferID)
{
	/*
	// check tho history position : remove if needed
	if (m_historyPos < edn_max(m_historyList.Size()-1, 0) ) {
		for(int32_t iii= m_historyPos; iii < m_historyList.Size(); iii++) {
			delete(m_historyList[iii]);
		}
		m_historyList.EraseLen(m_historyPos, m_historyList.Size() - m_historyPos);
	}
	// add the current element
	BufferManager *myBufferManager = BufferManager::getInstance();
	etk::File currentFilename = myBufferManager->Get(bufferID)->GetFileName();
	*/
}

/*
enum
{
	CTAGS_COL_FILE = 0,
	CTAGS_COL_LINE_NUMBER,
	CTAGS_NUM_COLS
};
void CTagsManager::cb_row(GtkTreeView *p_treeview,
                          GtkTreePath * p_path,
                          GtkTreeViewColumn * p_column,
                          gpointer data)
{
	EDN_DEBUG("event");
	CTagsManager * self = reinterpret_cast<CTagsManager*>(data);
	
	gchar * p_file=NULL;
	gint lineNumber;
	GtkTreeIter iter;
	
	
	if (gtk_tree_model_get_iter( GTK_TREE_MODEL(self->m_listStore), &iter, p_path))
	{
		gtk_tree_model_get( GTK_TREE_MODEL(self->m_listStore),
		                    &iter,
		                    CTAGS_COL_FILE, &p_file,
		                    CTAGS_COL_LINE_NUMBER, &lineNumber,
		                    -1 );
		EDN_DEBUG("find : " << p_file << ":" << lineNumber);
		for (int32_t iii = 0; iii < self->m_currentList.Size() ; iii++) {
			if(    self->m_currentList[iii].lineID == lineNumber
			    && strcmp(self->m_currentList[iii].filename, p_file)==0)
			{
				g_object_unref( GTK_TREE_MODEL(self->m_listStore));
				// Remove dialogue
				gtk_widget_destroy(self->m_Dialog);
				// Jump ...
				self->JumpAtID(iii);
				return;
			}
		}
	}
}
*/

/*
GtkWidget * CTagsManager::CreateViewAndModel(void)
{
	
	GtkCellRenderer *   renderer;
	GtkWidget *         view;
	view = gtk_tree_view_new();
	
	// Column 1
	renderer = gtk_cell_renderer_text_new();
	gtk_tree_view_insert_column_with_attributes(GTK_TREE_VIEW(view),
	                                            -1,
	                                            "File",
	                                            renderer,
	                                            "text", CTAGS_COL_FILE,
	                                            NULL);
	
	// Column 2
	renderer = gtk_cell_renderer_text_new();
	gtk_tree_view_insert_column_with_attributes(GTK_TREE_VIEW(view),
	                                            -1,
	                                            "lineNumber",
	                                            renderer,
	                                            "text", CTAGS_COL_LINE_NUMBER,
	                                            NULL);
	
	// Set data in the list : 
	GtkTreeIter      iter;
	m_listStore = gtk_list_store_new(CTAGS_NUM_COLS, G_TYPE_STRING, G_TYPE_UINT);
	// Append a row and fill in some data
	for (int32_t iii=0; iii<m_currentList.Size() ; iii++) {
		gtk_list_store_append(m_listStore, &iter);
		gtk_list_store_set(m_listStore, &iter,
		                   CTAGS_COL_FILE, m_currentList[iii].filename,
		                   CTAGS_COL_LINE_NUMBER, m_currentList[iii].lineID,
		                   -1);
	}
	
	gtk_tree_view_set_model( GTK_TREE_VIEW(view), GTK_TREE_MODEL(m_listStore) );
	g_signal_connect( G_OBJECT(view), "row-activated", G_CALLBACK(cb_row), this );
	//g_object_unref(GTK_TREE_MODEL(m_listStore));
	
	return view;
}

*/



int32_t CTagsManager::MultipleJump(void)
{
	/*
	// dlg to confirm the quit event : 
	m_Dialog = gtk_dialog_new_with_buttons("C-Tags jump...",
	                                       NULL,
	                                       GTK_DIALOG_MODAL,
	                                       //"Jump", GTK_RESPONSE_YES,
	                                       GTK_STOCK_QUIT, GTK_RESPONSE_NO,
	                                       NULL);
	// Set over main windows
	//gtk_window_set_transient_for(GTK_WINDOW(myDialog), GTK_WINDOW(m_mainWindow->GetWidget()));
	// add writting area
	GtkWidget *myContentArea = gtk_dialog_get_content_area( GTK_DIALOG(m_Dialog));
	GtkWidget *listView = CreateViewAndModel();
	gtk_box_pack_start(GTK_BOX(myContentArea), listView, TRUE, TRUE, 0);
	// Display it
	gtk_widget_show_all(myContentArea);
	int32_t result = gtk_dialog_run(GTK_DIALOG(m_Dialog));
	// Get data from the gtk entry
	if (result == GTK_RESPONSE_NO) {
		g_object_unref(GTK_TREE_MODEL(m_listStore));
		// Remove dialogue
		gtk_widget_destroy(m_Dialog);
	}
	*/
	return 0;
}


void CTagsManager::JumpAtID(int32_t selectID)
{
/*
	BufferManager *myBufferManager = BufferManager::getInstance();
	etk::File myFile = m_currentList[selectID].filename;
	EDN_INFO("save curent filename and position : ");
	int32_t currentSelected = myBufferManager->GetSelected();
	Buffer* tmpBuf = myBufferManager->Get(currentSelected);
	if (NULL != tmpBuf) {
		etk::File * bufferFilename = new etk::File();
		*bufferFilename = tmpBuf->GetFileName();
		bufferFilename->SetLineNumber(tmpBuf->GetCurrentLine());
		m_historyList.PushBack(bufferFilename);
	}
	EDN_INFO(" OPEN the TAG file Destination : " << myFile );
	if (false == myBufferManager->Exist(myFile) ) {
		// need to open the file : 
		int32_t openID = myBufferManager->Open(myFile);
		SendMessage(EDN_MSG__CURRENT_CHANGE_BUFFER_ID, openID);
	} else {
		SendMessage(EDN_MSG__CURRENT_CHANGE_BUFFER_ID, myBufferManager->GetId(myFile));
	}
	SendMessage(EDN_MSG__CURRENT_GOTO_LINE, m_currentList[selectID].lineID - 1);
*/
}


void CTagsManager::JumpTo(void)
{
/*
	m_currentList.Clear();
	if (NULL != m_ctagFile) {
		etk::VectorType<int8_t> data;
		// get the middle button of the clipboard ==> represent the current selection ...
		ClipBoard::Get(COPY_MIDDLE_BUTTON, data);
		if (data.Size() == 0) {
			EDN_INFO("No current Sélection");
		}
		tagEntry entry;
		data.PushBack('\0');
		EDN_INFO("try to find the tag : " << (const char *)&data[0]);
		if (tagsFind (m_ctagFile, &entry, (const char *)&data[0], 0) == TagSuccess) {
			tagEntry entrySave = entry;
			int32_t numberOfTags = 0;
			
			// For all tags : Save in an internal Structure :
			do {
				etk::String destinationFilename = m_tagFolderBase;
				destinationFilename += entry.file;
				etk::File myfile = destinationFilename;
				TagListFind_ts myStruct;
				strncpy(myStruct.filename, myfile.GetCompleateName().c_str(), MAX_FILE_NAME);
				myStruct.filename[MAX_FILE_NAME-1] = '\0';
				strncpy(myStruct.RegExp, entry.address.pattern, MAX_REG_EXP_SEARCH);
				myStruct.RegExp[MAX_REG_EXP_SEARCH-1] = '\0';
				myStruct.lineID = entry.address.lineNumber;
				// at at the corect position
				m_currentList.PushBack(myStruct);
				PrintTag(&entry, true);
			} while (tagsFindNext (m_ctagFile, &entry) == TagSuccess);
			
			
			if (1==m_currentList.Size() ) {
				JumpAtID(0);
				
			} else {
				// Open a choice windows...
				int32_t SelectID = MultipleJump();
			}
		} else {
			EDN_INFO("no tag find ...");
		}
	}
*/
}
/*
void CTagsManager::PrintTag (const tagEntry *entry, bool small)
{
	if (small==true) {
		EDN_INFO("find Tag file : name=\"" << entry->name << "\" in file=\"" << entry->file 
			<< "\" at line="<< entry->address.lineNumber);
	} else {
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
}
*/