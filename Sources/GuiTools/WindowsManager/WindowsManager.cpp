/**
 *******************************************************************************
 * @file WindowsManager.cpp
 * @brief Editeur De N'ours : Windows creation, pop up, destruction ... (Sources)
 * @author Edouard DUPIN
 * @date 20/06/2011
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
#include "WindowsManager.h"
#include "MainWindows.h"
#include "Edn.h"
#include "Search.h"


#undef  __class__
#define __class__    "WindowsManager"

/**
 * @brief
 *
 * @param[in,out] ---
 *
 * @return ---
 *
 */
WindowsManager::WindowsManager(void) : MsgBroadcast("Windows Manager", EDN_CAT_GUI_MANAGER)
{
	m_currentBufferID = -1;
}

/**
 * @brief
 *
 * @param[in,out] ---
 *
 * @return ---
 *
 */
WindowsManager::~WindowsManager(void)
{

}

void WindowsManager::OnMessage(int32_t id, int32_t dataID)
{
	switch (id)
	{
		case EDN_MSG__BUFFER_CHANGE_CURRENT:
			m_currentBufferID = dataID;
			break;
		case EDN_MSG__GUI_SHOW_MAIN_WINDOWS:
			EDN_INFO("Request opening MAIN_WINDOWS");
			m_mainWindow = MainWindows::getInstance();
			break;
		case EDN_MSG__GUI_SHOW_SEARCH:
			EDN_INFO("Request opening SEARCH");
			{
				Search *myInstance = Search::getInstance();
				myInstance->Display();
			}
			break;
		case EDN_MSG__GUI_SHOW_PREFERENCE:
			EDN_INFO("Request opening PREFERENCE");
			break;
		case EDN_MSG__GUI_SHOW_REPLACE:
			EDN_INFO("Request opening REPLACE");
			break;
		case EDN_MSG__GUI_SHOW_OPEN_FILE:
			EDN_INFO("Request opening OPEN_FILE");
			{
				BufferManager *myBufferManager = NULL;
				myBufferManager = BufferManager::getInstance();
				
				GtkWidget *dialog = gtk_file_chooser_dialog_new( "Open File", NULL,
				                                                 GTK_FILE_CHOOSER_ACTION_OPEN,
				                                                 GTK_STOCK_CANCEL, // button text
				                                                 GTK_RESPONSE_CANCEL, // response id
				                                                 GTK_STOCK_OPEN, // button text
				                                                 GTK_RESPONSE_ACCEPT, // response id
				                                                 NULL); // end button/response list
				if(    -1   != m_currentBufferID
				    && true == myBufferManager->Exist(m_currentBufferID) )
				{
					Edn::String fileFolder = myBufferManager->Get(m_currentBufferID)->GetFolder();
					gtk_file_chooser_set_current_folder (GTK_FILE_CHOOSER(dialog), fileFolder.c_str());
					//gtk_file_chooser_set_current_name (GTK_FILE_CHOOSER(dialog), "Untitled document");
				}
				if (gtk_dialog_run(GTK_DIALOG (dialog)) == GTK_RESPONSE_ACCEPT)
				{
					Edn::String myfilename;
					myfilename = gtk_file_chooser_get_filename(GTK_FILE_CHOOSER (dialog));

					if (false == myBufferManager->Exist(myfilename) ) {
						int32_t openID = myBufferManager->Open(myfilename);
						SendMessage(EDN_MSG__CURRENT_CHANGE_BUFFER_ID, openID);
					} else {
						SendMessage(EDN_MSG__CURRENT_CHANGE_BUFFER_ID, myBufferManager->GetId(myfilename));
					}
				}
				gtk_widget_destroy(dialog);
			}
			break;
		case EDN_MSG__GUI_SHOW_SAVE_AS:
			EDN_INFO("Request opening SAVE_AS");
			// save only if one element is selected
			if (BufferManager::getInstance()->Size() > 0) {
				int32_t idSelected;
				if(-1 == dataID) {
					idSelected = BufferManager::getInstance()->GetSelected();
				} else {
					idSelected = dataID;
				}
				Buffer *myBuffer = BufferManager::getInstance()->Get(idSelected);
				Edn::String tmpString = "Save as file : ";
				tmpString += myBuffer->GetShortName().c_str();
				GtkWidget *dialog = gtk_file_chooser_dialog_new( tmpString.c_str(), NULL,
				                                                 GTK_FILE_CHOOSER_ACTION_SAVE,
				                                                 GTK_STOCK_CANCEL, // button text
				                                                 GTK_RESPONSE_CANCEL, // response id
				                                                 GTK_STOCK_SAVE, // button text
				                                                 GTK_RESPONSE_ACCEPT, // response id
				                                                 NULL); // end button/response list
				if (gtk_dialog_run(GTK_DIALOG (dialog)) == GTK_RESPONSE_ACCEPT)
				{
					Edn::String myfilename;
					myfilename = gtk_file_chooser_get_filename(GTK_FILE_CHOOSER (dialog));
					
					myBuffer->SetName(myfilename);
					myBuffer->Save();
					
					SendMessage(EDN_MSG__CURRENT_CHANGE_BUFFER_ID, idSelected);
				}
				gtk_widget_destroy(dialog); // implicitly hides dialog
			}
			break;
	}
}