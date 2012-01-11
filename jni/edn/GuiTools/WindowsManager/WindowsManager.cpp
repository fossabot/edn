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
				myInstance->Display(GTK_WINDOW(m_mainWindow->GetWidget()));
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
				                                                 GTK_STOCK_CANCEL, GTK_RESPONSE_CANCEL,
				                                                 GTK_STOCK_OPEN,   GTK_RESPONSE_ACCEPT,
				                                                 NULL); // end button/response list
				// this element did not apear in the miniature of the windows
				gtk_window_set_skip_pager_hint(GTK_WINDOW(dialog), TRUE);
				if(    -1   != m_currentBufferID
				    && true == myBufferManager->Exist(m_currentBufferID) )
				{
					Edn::File fileName = myBufferManager->Get(m_currentBufferID)->GetFileName();
					gtk_file_chooser_set_current_folder(GTK_FILE_CHOOSER(dialog), fileName.GetFolder().c_str());
					gtk_file_chooser_set_current_name(GTK_FILE_CHOOSER(dialog), fileName.GetShortFilename().c_str());
				}
				if (gtk_dialog_run(GTK_DIALOG (dialog)) == GTK_RESPONSE_ACCEPT)
				{
					Edn::File myfilename;
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
				tmpString += myBuffer->GetFileName().GetShortFilename().c_str();
				GtkWidget *dialog = gtk_file_chooser_dialog_new( tmpString.c_str(), NULL,
				                                                 GTK_FILE_CHOOSER_ACTION_SAVE,
				                                                 GTK_STOCK_CANCEL, GTK_RESPONSE_CANCEL,
				                                                 GTK_STOCK_SAVE,   GTK_RESPONSE_ACCEPT,
				                                                 NULL); // end button/response list
				// this element did not apear in the miniature of the windows
				gtk_window_set_skip_pager_hint(GTK_WINDOW(dialog), TRUE);
				if (gtk_dialog_run(GTK_DIALOG (dialog)) == GTK_RESPONSE_ACCEPT)
				{
					Edn::String myfilename;
					myfilename = gtk_file_chooser_get_filename(GTK_FILE_CHOOSER (dialog));
					
					myBuffer->SetFileName(myfilename);
					myBuffer->Save();
					
					SendMessage(EDN_MSG__CURRENT_CHANGE_BUFFER_ID, idSelected);
				}
				gtk_widget_destroy(dialog); // implicitly hides dialog
			}
			break;
		case EDN_MSG__GUI_SHOW_ABOUT:
			{
				GtkWidget  *myDialog = gtk_about_dialog_new();
				gtk_about_dialog_set_program_name(GTK_ABOUT_DIALOG(myDialog), "Edn");
				gtk_about_dialog_set_version(     GTK_ABOUT_DIALOG(myDialog), VERSION_TAG_NAME);
				gtk_about_dialog_set_comments(    GTK_ABOUT_DIALOG(myDialog), "Editeur De N'ours\n"
				                                                              "L'Editeur Desoxyribo-Nucleique.\n"
				                                                              "Source Code Editor\n"
				                                                              "Build Time : " VERSION_BUILD_TIME);
				gtk_about_dialog_set_copyright(   GTK_ABOUT_DIALOG(myDialog), "Copyright 2010 Edouard DUPIN, all right reserved");
				gtk_about_dialog_set_license(     GTK_ABOUT_DIALOG(myDialog), "This software is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY\n\n"
				                                                              "You can:\n"
				                                                              "    * Redistribute the sources code and binaries.\n"
				                                                              "    * Modify the Sources code.\n"
				                                                              "    * Use a part of the sources (less than 50%) in an other software, just write somewhere \"Edn is great\" visible by the user (on your product or on your website with a link to my page).\n"
				                                                              "    * Redistribute the modification only if you want.\n"
				                                                              "    * Send me the bug-fix (it could be great).\n"
				                                                              "    * Pay me a beer or some other things.\n"
				                                                              "    * Print the source code on WC paper ...\n\n"
				                                                              "You can NOT:\n"
				                                                              "    * Earn money with this Software (But I can).\n"
				                                                              "    * Add malware in the Sources.\n"
				                                                              "    * Do something bad with the sources.\n"
				                                                              "    * Use it to travel in the space with a toaster.\n\n"
				                                                              "I reserve the right to change this licence. If it change the version of the copy you have keep its own license.");
				gtk_about_dialog_set_wrap_license(GTK_ABOUT_DIALOG(myDialog), true);
#				if USE_GTK_VERSION_3_0
				gtk_about_dialog_set_license_type(GTK_ABOUT_DIALOG(myDialog), GTK_LICENSE_CUSTOM);
#				endif
				gtk_about_dialog_set_website(GTK_ABOUT_DIALOG(myDialog), "http://HeeroYui.github.com/edn");
				gtk_about_dialog_set_website_label(GTK_ABOUT_DIALOG(myDialog), "Edn on github");
				const char * listAutor[] = {"Edouard DUPIN", NULL};
				gtk_about_dialog_set_authors(GTK_ABOUT_DIALOG(myDialog), listAutor);
				// Display About
				gtk_dialog_run(GTK_DIALOG(myDialog));
				gtk_widget_destroy(myDialog);
			}
			break;
		case EDN_MSG__GUI_SHOW_GOTO_LINE:
			{
				// dlg to confirm the quit event : 
				GtkWidget *myDialog = gtk_dialog_new_with_buttons("Goto Line",
				                                                  GTK_WINDOW(m_mainWindow->GetWidget()),
				                                                  GTK_DIALOG_MODAL,
				                                                  "Jump", GTK_RESPONSE_YES,
				                                                  GTK_STOCK_QUIT, GTK_RESPONSE_NO,
				                                                  NULL);
				// this element did not apear in the miniature of the windows
				gtk_window_set_skip_pager_hint(GTK_WINDOW(myDialog), TRUE);
				// Set over main windows
				gtk_window_set_transient_for(GTK_WINDOW(myDialog), GTK_WINDOW(m_mainWindow->GetWidget()));
				// add writting area
				GtkWidget *myContentArea = gtk_dialog_get_content_area( GTK_DIALOG(myDialog));
				GtkWidget *myEntry =  gtk_entry_new();
				gtk_box_pack_start(GTK_BOX(myContentArea), myEntry, TRUE, TRUE, 0);
				// Display it
				gtk_widget_show_all(myContentArea);
				int32_t result = gtk_dialog_run (GTK_DIALOG (myDialog));
				// Get data from the gtk entry
				const char *myData = gtk_entry_get_text(GTK_ENTRY(myEntry));
				if (NULL != myData) {
					int32_t lineID=0;
					if (1==sscanf(myData, "%d",&lineID)) {
						EDN_DEBUG("find in : \"" << myData << "\" = " << lineID);
						if(GTK_RESPONSE_YES == result) {
							SendMessage(EDN_MSG__CURRENT_GOTO_LINE, lineID-1);
						}
					} else {
						if (GTK_RESPONSE_YES == result) {
							EDN_WARNING("find in : \"" << myData << "\" line Number is not correct ");
						}
					}
				} else {
					EDN_DEBUG("no line Writen ...");
				}
				// Remove dialogue
				gtk_widget_destroy(myDialog);
			}
			break;
		case EDN_MSG__GUI_SHOW_EXIT_CONFIRMATION:
			{
				EDN_INFO("quit requested");
				// dlg to confirm the quit event : 
				GtkWidget *p_dialog = gtk_dialog_new_with_buttons("Exit",
				                                                  GTK_WINDOW(m_mainWindow->GetWidget()),
				                                                  GTK_DIALOG_MODAL,
				                                                  GTK_STOCK_YES, GTK_RESPONSE_YES,
				                                                  GTK_STOCK_NO, GTK_RESPONSE_NO,
				                                                  NULL);
				GtkWidget *p_label =  gtk_label_new ("Do you want exit Edn ?");
				gtk_box_pack_start(GTK_BOX(gtk_dialog_get_content_area( GTK_DIALOG(p_dialog) )), p_label, TRUE, TRUE, 0);
				gtk_widget_show(p_label);
				
				switch (gtk_dialog_run (GTK_DIALOG (p_dialog)))
				{
					case GTK_RESPONSE_YES:
						gtk_widget_destroy (p_dialog);
						p_dialog = NULL;
						{
							BufferManager * myBufferMng = BufferManager::getInstance();
							for (int32_t iii=0 ; iii<myBufferMng->Size() ; iii++) {
								if (true==myBufferMng->Exist(iii) ) {
									Buffer * myBuffer = myBufferMng->Get(iii);
									if (NULL != myBuffer) {
										if (true == myBuffer->IsModify()) {
											if (true == myBuffer->HaveName()) {
												p_dialog = gtk_dialog_new_with_buttons("Save Before Exit",
												                                       GTK_WINDOW(m_mainWindow->GetWidget()),
												                                       GTK_DIALOG_MODAL,
												                                       GTK_STOCK_SAVE,    GTK_RESPONSE_YES,
												                                       GTK_STOCK_SAVE_AS, GTK_RESPONSE_ACCEPT,
												                                       GTK_STOCK_NO,      GTK_RESPONSE_NO,
												                                       GTK_STOCK_CANCEL,  GTK_RESPONSE_CANCEL,
												                                       NULL);
											} else {
												p_dialog = gtk_dialog_new_with_buttons("Save Before Exit",
												                                       GTK_WINDOW(m_mainWindow->GetWidget()),
												                                       GTK_DIALOG_MODAL,
												                                       GTK_STOCK_SAVE_AS, GTK_RESPONSE_ACCEPT,
												                                       GTK_STOCK_NO,      GTK_RESPONSE_NO,
												                                       GTK_STOCK_CANCEL,  GTK_RESPONSE_CANCEL,
												                                       NULL);
											}
											char tmpName[1024];
											sprintf(tmpName, "Save file \"%s\" ?", myBuffer->GetFileName().GetCompleateName().c_str());
											p_label = gtk_label_new(tmpName);
											gtk_box_pack_start(GTK_BOX(gtk_dialog_get_content_area( GTK_DIALOG(p_dialog) )), p_label, TRUE, TRUE, 0);
											gtk_widget_show(p_label);
											
											switch (gtk_dialog_run (GTK_DIALOG (p_dialog)))
											{
												case GTK_RESPONSE_YES:
													myBuffer->Save();
													break;
												case GTK_RESPONSE_ACCEPT:
													OnMessage(EDN_MSG__GUI_SHOW_SAVE_AS, iii);
													break;
												case GTK_RESPONSE_NO:
													// nothing to do ...
													break;
												case GTK_RESPONSE_CANCEL:
													gtk_widget_destroy (p_dialog);
													return;
													break;
											}
											gtk_widget_destroy (p_dialog);
											p_dialog = NULL;
										}
									}
								}
							}
						}
						gtk_main_quit();
						break;
					case GTK_RESPONSE_NO:
						gtk_widget_destroy (p_dialog);
						break;
				}
				
				
			}
			break;
	
	}
}