/**
 *******************************************************************************
 * @file MsgBroadcast.cpp
 * @brief Editeur De N'ours : message beetween thread and GUI elements ... (Souces)
 * @author Edouard DUPIN
 * @date 04/02/2011
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
#include <MsgBroadcast.h>

#undef __class__
#define __class__	"MsgBroadcast"


MsgBroadcast::MsgBroadcast(const char * className, messageCat_te cat)
{
	m_messageSystem = MsgBroadcastCore::Get();
	m_className = className;
	m_cat = cat;
	// add on listner
	m_messageSystem->AddReceiver(this);
}

MsgBroadcast::~MsgBroadcast(void)
{
	m_messageSystem->RmReceiver(this);
	m_messageSystem = NULL;
}

void MsgBroadcast::OnMessage(int32_t id, int32_t dataID)
{
	// nothing to do here
}

void MsgBroadcast::SendMessage(messageType_te id, int32_t dataID)
{
	m_messageSystem->SendMessage(this, id, dataID);
}


#undef __class__
#define __class__	"MsgBroadcastCore"


// need to create a syngleton ...
MsgBroadcastCore::MsgBroadcastCore(void)
{
	EDN_INFO("Init broadcast message System : ");
	m_messageID = 0;
}

MsgBroadcastCore::~MsgBroadcastCore(void)
{
	EDN_INFO("Un-Init broadcast message System : ");
}

#define MACRO_DISPLAY_MSG(data)         case data: return (char*)#data ; break;

static char * GetMessageChar(messageType_te Id)
{
	switch(Id)
	{
		MACRO_DISPLAY_MSG(EDN_MSG__BUFFER_CHANGE_CURRENT)
		MACRO_DISPLAY_MSG(EDN_MSG__USER_DISPLAY_CHANGE)

		MACRO_DISPLAY_MSG(EDN_MSG__GUI_SHOW_MAIN_WINDOWS)
		MACRO_DISPLAY_MSG(EDN_MSG__GUI_SHOW_SEARCH)
		MACRO_DISPLAY_MSG(EDN_MSG__GUI_SHOW_PREFERENCE)
		MACRO_DISPLAY_MSG(EDN_MSG__GUI_SHOW_REPLACE)
		MACRO_DISPLAY_MSG(EDN_MSG__GUI_SHOW_OPEN_FILE)
		MACRO_DISPLAY_MSG(EDN_MSG__GUI_SHOW_SAVE_AS)
		MACRO_DISPLAY_MSG(EDN_MSG__GUI_SHOW_GOTO_LINE)
		MACRO_DISPLAY_MSG(EDN_MSG__GUI_SHOW_ABOUT)
		MACRO_DISPLAY_MSG(EDN_MSG__GUI_SHOW_EXIT_CONFIRMATION)
	
		MACRO_DISPLAY_MSG(EDN_MSG__BUFFER_REMOVE)
		MACRO_DISPLAY_MSG(EDN_MSG__BUFFER_REMOVE_ALL)
		MACRO_DISPLAY_MSG(EDN_MSG__BUFFER_ADD)
		MACRO_DISPLAY_MSG(EDN_MSG__BUFFER_CHANGE_STATE)
		MACRO_DISPLAY_MSG(EDN_MSG__BUFFER_CHANGE_NAME)
		MACRO_DISPLAY_MSG(EDN_MSG__BUFFER_CHANGE_MODIFY)
		MACRO_DISPLAY_MSG(EDN_MSG__BUFFER_CHANGE_HAS_HISTORY)
		MACRO_DISPLAY_MSG(EDN_MSG__BUFFER_CHANGE_HAS_NOT_HISTORY)
		MACRO_DISPLAY_MSG(EDN_MSG__BUFFER_CHANGE_HAS_FUTURE_HISTORY)
		MACRO_DISPLAY_MSG(EDN_MSG__BUFFER_CHANGE_HAS_NOT_FUTURE_HISTORY)
	
		// create a new buffer
		MACRO_DISPLAY_MSG(EDN_MSG__NEW)
		MACRO_DISPLAY_MSG(EDN_MSG__BUFF_ALL_SAVE)
		MACRO_DISPLAY_MSG(EDN_MSG__BUFF_ALL_CLOSE)
		MACRO_DISPLAY_MSG(EDN_MSG__BUFF_ID_CLOSE)
		MACRO_DISPLAY_MSG(EDN_MSG__BUFF_ID_SAVE)
	
		// GUI event for the selected buffer
		MACRO_DISPLAY_MSG(EDN_MSG__CURRENT_CHANGE_BUFFER_ID)
		MACRO_DISPLAY_MSG(EDN_MSG__CURRENT_SAVE)
		MACRO_DISPLAY_MSG(EDN_MSG__CURRENT_SAVE_AS)
		MACRO_DISPLAY_MSG(EDN_MSG__CURRENT_REMOVE_LINE)
		MACRO_DISPLAY_MSG(EDN_MSG__CURRENT_SELECT_ALL)
		MACRO_DISPLAY_MSG(EDN_MSG__CURRENT_UN_SELECT)
		MACRO_DISPLAY_MSG(EDN_MSG__CURRENT_COPY)
		MACRO_DISPLAY_MSG(EDN_MSG__CURRENT_CUT)
		MACRO_DISPLAY_MSG(EDN_MSG__CURRENT_PASTE)
		MACRO_DISPLAY_MSG(EDN_MSG__CURRENT_FIND_PREVIOUS)
		MACRO_DISPLAY_MSG(EDN_MSG__CURRENT_FIND_OLD_PREVIOUS)
		MACRO_DISPLAY_MSG(EDN_MSG__CURRENT_FIND_NEXT)
		MACRO_DISPLAY_MSG(EDN_MSG__CURRENT_FIND_OLD_NEXT)
		MACRO_DISPLAY_MSG(EDN_MSG__CURRENT_REPLACE)
		MACRO_DISPLAY_MSG(EDN_MSG__CURRENT_REPLACE_ALL)
		MACRO_DISPLAY_MSG(EDN_MSG__CURRENT_CLOSE)
		MACRO_DISPLAY_MSG(EDN_MSG__CURRENT_UNDO)
		MACRO_DISPLAY_MSG(EDN_MSG__CURRENT_REDO)
		MACRO_DISPLAY_MSG(EDN_MSG__CURRENT_GOTO_LINE)
		MACRO_DISPLAY_MSG(EDN_MSG__REFRESH_DISPLAY)
		MACRO_DISPLAY_MSG(EDN_MSG__CURRENT_SET_CHARSET)
	
		// Ctags MESSAGE :
		MACRO_DISPLAY_MSG(EDN_MSG__OPEN_CTAGS)
		MACRO_DISPLAY_MSG(EDN_MSG__RELOAD_CTAGS)
		MACRO_DISPLAY_MSG(EDN_MSG__JUMP_TO_CURRENT_SELECTION)
		MACRO_DISPLAY_MSG(EDN_MSG__JUMP_BACK)
	
		// HL message :
		MACRO_DISPLAY_MSG(EDN_MSG__COLOR_HAS_CHANGE)
	
		// Color message :
		MACRO_DISPLAY_MSG(EDN_MSG__RELOAD_COLOR_FILE)
	
		default:
			return (char*)"??";
	}
}

static char * GetMessageTypeChar(messageCat_te Id)
{
	switch(Id)
	{
		case EDN_CAT_NONE:
			return (char*)"NONE";
		case EDN_CAT_GUI:
			return (char*)"GUI";
		case EDN_CAT_WORK_AREA:
			return (char*)"WORK_AREA";
		case EDN_CAT_SYSTEM:
			return (char*)"SYSTEM";
		case EDN_CAT_BUFFER_MANAGER:
			return (char*)"BUFFER_MANAGER";
		case EDN_CAT_GUI_MANAGER:
			return (char*)"GUI_MANAGER";
		case EDN_CAT_CTAGS:
			return (char*)"C-TAGS_MANAGER";
		case EDN_CAT_MENU_CONTEXT:
			return (char*)"MENU CONTEXT";
		case EDN_CAT_HL:
			return (char*)"HIGHT-LIGHT";
		case EDN_CAT_COLOR:
			return (char*)"COLOR";
		default:
			return (char*)"??";
	}
}

void MsgBroadcastCore::SendMessage(MsgBroadcast * pointerOnSender, messageType_te id, int32_t dataID)
{
	// Add message on the list :
	messageElement_ts myStructMessage;
	messageCat_te catDest = EDN_CAT_NONE;
	
	
	// DESTINATION : GUI_MANAGER
	if(    MSG_TO_GUI_MANAGER__START <= id
	    && MSG_TO_GUI_MANAGER__STOP  >= id )
	{
		catDest = EDN_CAT_GUI_MANAGER;
	} else if(    MSG_TO_GUI__START <= id
	           && MSG_TO_GUI__STOP  >= id )
	{
		catDest = EDN_CAT_GUI;
	} else if(    MSG_TO_BUFFER_MANAGER__START <= id
	           && MSG_TO_BUFFER_MANAGER__STOP  >= id )
	{
		catDest = EDN_CAT_BUFFER_MANAGER;
	} else if(    MSG_TO_WORKING_AREA__START <= id
	           && MSG_TO_WORKING_AREA__STOP  >= id )
	{
		catDest = EDN_CAT_WORK_AREA;
	} else if(    MSG_TO_SYSTEM__START <= id
	           && MSG_TO_SYSTEM__STOP  >= id )
	{
		catDest = EDN_CAT_SYSTEM;
	} else if(    MSG_TO_CTAGS__START <= id
	           && MSG_TO_CTAGS__STOP  >= id )
	{
		catDest = EDN_CAT_CTAGS;
	} else if(    MSG_TO_CONTEXT__START <= id
	           && MSG_TO_CONTEXT__STOP  >= id )
	{
		catDest = EDN_CAT_MENU_CONTEXT;
	} else if(    MSG_TO_HL__START <= id
	           && MSG_TO_HL__STOP  >= id )
	{
		catDest = EDN_CAT_HL;
	} else if(    MSG_TO_COLOR__START <= id
	           && MSG_TO_COLOR__STOP  >= id )
	{
		catDest = EDN_CAT_COLOR;
	}
	
	myStructMessage.localMessageID = m_messageID++;
	if (NULL == pointerOnSender) {
		EDN_INFO("#" << myStructMessage.localMessageID << " From \"NULL\" CAT="                                         << GetMessageTypeChar(catDest) << " id=" << id << "=\"" << GetMessageChar(id) << "\" dataID=" << dataID);
	} else {
		EDN_INFO("#" << myStructMessage.localMessageID << " From \"" << pointerOnSender->GetName().c_str() << "\" CAT=" << GetMessageTypeChar(catDest) << " id=" << id << "=\"" << GetMessageChar(id) << "\" dataID=" << dataID);
	}
	myStructMessage.msgCatDest = catDest;
	myStructMessage.msgId      = id;
	myStructMessage.data       = dataID;
	m_listOfMessage.PushBack(myStructMessage);
	
	
	if (m_listOfMessage.Size() > 1 ) {
		// we are curently in message processing ==> wait end to process this message
		return;
	}
	// send message on system :
	while (m_listOfMessage.Size() > 0) {
		for (int32_t i=0 ; i<m_listMessage.Size() ; i++) {
			if(		EDN_CAT_NONE == m_listOfMessage[0].msgCatDest
				||	m_listOfMessage[0].msgCatDest == m_listMessage[i]->GetCat())
			{
				EDN_INFO("        #" << m_listOfMessage[0].localMessageID << " ==> process In :\"" << m_listMessage[i]->GetName().c_str() << "\" ");
				m_listMessage[i]->OnMessage(m_listOfMessage[0].msgId, m_listOfMessage[0].data);
			}
		}
		m_listOfMessage.Erase(0);
	}
}

void MsgBroadcastCore::AddReceiver(MsgBroadcast * pointerOnReceiver)
{
	for (int32_t i=0 ; i<m_listMessage.Size() ; i++) {
		if (m_listMessage[i] == pointerOnReceiver) {
			// nothing to do ...
			return;
		}
	}
	m_listMessage.PushBack(pointerOnReceiver);
	EDN_INFO("Add a listner for the broadCast messages : \"" << pointerOnReceiver->GetName().c_str() << "\"");
}


void MsgBroadcastCore::RmReceiver(MsgBroadcast * pointerOnReceiver)
{
	for (int32_t i=0 ; i<m_listMessage.Size() ; i++) {
		if (m_listMessage[i] == pointerOnReceiver) {
			m_listMessage.Erase(i);
			return;
		}
	}
}



void GeneralSendMessage(messageType_te id, int32_t dataID)
{
	MsgBroadcastCore::Get()->SendMessage(NULL, id, dataID);
}
