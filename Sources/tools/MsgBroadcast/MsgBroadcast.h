/**
 *******************************************************************************
 * @file MsgBroadcast.h
 * @brief Editeur De N'ours : message beetween thread and GUI elements ... (header)
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
 
#ifndef __MSG_BROADCAST_H__
#define __MSG_BROADCAST_H__

#include "tools_debug.h"
#include "Edn.h"
#include "Singleton.h"



// broadCast Message

// Message to prevent the curent thread that Buffer has changed
// the ID we'll use to identify our event
typedef enum {
	EDN_MSG__NONE = 0,
	// Programm is Quitting... close all if needed ...
	EDN_MSG__QUIT,
	EDN_MSG__BUFFER_CHANGE_CURRENT,  // set the new current BUFFER ...
	EDN_MSG__USER_DISPLAY_CHANGE,    // User change the display ==> need to reload all the display depending on color internal
	
	// DESTINATION : GUI_MANAGER
	MSG_TO_GUI_MANAGER__START,
		// GUI windows openning and closing
		EDN_MSG__GUI_SHOW_MAIN_WINDOWS,
		EDN_MSG__GUI_SHOW_SEARCH,
		EDN_MSG__GUI_SHOW_PREFERENCE,
		EDN_MSG__GUI_SHOW_REPLACE,
		EDN_MSG__GUI_SHOW_OPEN_FILE,
		EDN_MSG__GUI_SHOW_SAVE_AS,
		EDN_MSG__GUI_SHOW_GOTO_LINE,
		EDN_MSG__GUI_SHOW_ABOUT,
	MSG_TO_GUI_MANAGER__STOP,
	
	// DESTINATION : GUI
	// generate by the current buffer to said the buffer has changing
	MSG_TO_GUI__START,
		EDN_MSG__BUFFER_REMOVE,
		EDN_MSG__BUFFER_REMOVE_ALL,
		EDN_MSG__BUFFER_ADD,
		EDN_MSG__BUFFER_CHANGE_STATE,
		EDN_MSG__BUFFER_CHANGE_NAME,
		EDN_MSG__BUFFER_CHANGE_MODIFY,
		EDN_MSG__BUFFER_CHANGE_HAS_HISTORY,
		EDN_MSG__BUFFER_CHANGE_HAS_NOT_HISTORY,
		EDN_MSG__BUFFER_CHANGE_HAS_FUTURE_HISTORY,
		EDN_MSG__BUFFER_CHANGE_HAS_NOT_FUTURE_HISTORY,
	MSG_TO_GUI__STOP,
	
	// DESTINATION : Buffer MANAGER
	MSG_TO_BUFFER_MANAGER__START,
		// create a new buffer
		EDN_MSG__NEW,
		// Event For All buffer ==> goto the buffer MANAGER
		EDN_MSG__BUFF_ALL_SAVE,
		EDN_MSG__BUFF_ALL_CLOSE,
		// Event For Specific Buffer ID :  ==> GOTO the buffer MANAGER
		EDN_MSG__BUFF_ID_CLOSE,
		EDN_MSG__BUFF_ID_SAVE,
	MSG_TO_BUFFER_MANAGER__STOP,

	// DESTINATION : Working AREA
	MSG_TO_WORKING_AREA__START,
		// GUI event for the selected buffer
		EDN_MSG__CURRENT_CHANGE_BUFFER_ID,
		EDN_MSG__CURRENT_SAVE,
		EDN_MSG__CURRENT_SAVE_AS,
		EDN_MSG__CURRENT_SELECT_ALL,
		EDN_MSG__CURRENT_REMOVE_LINE,
		EDN_MSG__CURRENT_UN_SELECT,
		EDN_MSG__CURRENT_COPY,
		EDN_MSG__CURRENT_CUT,
		EDN_MSG__CURRENT_PASTE,
		EDN_MSG__CURRENT_FIND_PREVIOUS,
		EDN_MSG__CURRENT_FIND_OLD_PREVIOUS,
		EDN_MSG__CURRENT_FIND_NEXT,
		EDN_MSG__CURRENT_FIND_OLD_NEXT,
		EDN_MSG__CURRENT_REPLACE,
		EDN_MSG__CURRENT_REPLACE_ALL,
		EDN_MSG__CURRENT_CLOSE,
		EDN_MSG__CURRENT_UNDO,
		EDN_MSG__CURRENT_REDO,
		EDN_MSG__CURRENT_GOTO_LINE,
		EDN_MSG__REFRESH_DISPLAY,
		EDN_MSG__CURRENT_SET_CHARSET,
	MSG_TO_WORKING_AREA__STOP,
	
	// DESTINATION : SYSTEM ...
	MSG_TO_SYSTEM__START,
	MSG_TO_SYSTEM__STOP,
	
	
	// DESTINATION : CTAGS ...
	MSG_TO_CTAGS__START,
		EDN_MSG__OPEN_CTAGS,
		EDN_MSG__RELOAD_CTAGS,
		EDN_MSG__JUMP_TO_CURRENT_SELECTION,
		EDN_MSG__JUMP_BACK,
	MSG_TO_CTAGS__STOP,

	// DESTINATION : context popup ...
	MSG_TO_CONTEXT__START,
	MSG_TO_CONTEXT__STOP,

	// DESTINATION : highlight system ...
	MSG_TO_HL__START,
		EDN_MSG__COLOR_HAS_CHANGE,
	MSG_TO_HL__STOP,

	// DESTINATION : Color System ...
	MSG_TO_COLOR__START,
		EDN_MSG__RELOAD_COLOR_FILE,
	MSG_TO_COLOR__STOP,


}messageType_te;

typedef enum {
	EDN_CAT_NONE,
	EDN_CAT_GUI,
	EDN_CAT_WORK_AREA,
	EDN_CAT_SYSTEM,
	EDN_CAT_BUFFER_MANAGER,
	EDN_CAT_GUI_MANAGER,
	EDN_CAT_CTAGS,
	EDN_CAT_MENU_CONTEXT,
	EDN_CAT_HL,
	EDN_CAT_COLOR,
}messageCat_te;


typedef struct {
	messageType_te  msgId;
	int32_t         dataId;
}messageData_ts;


class MsgBroadcastCore;

class MsgBroadcast
{
	private:
		Edn::String             m_className;
		MsgBroadcastCore *      m_messageSystem;
		messageCat_te           m_cat;
	public:
		MsgBroadcast(const char * className, messageCat_te cat);
		virtual ~MsgBroadcast(void);
		// caul when a message is send
		virtual void     OnMessage(int32_t id, int32_t dataID);
		Edn::String&     GetName(void) { return m_className; };
		messageCat_te    GetCat(void) { return m_cat; };
	protected :
		void         SendMessage(messageType_te id, int32_t dataID = -1);
};


typedef struct {
	int32_t         localMessageID;
	messageCat_te   msgCatDest;
	messageType_te  msgId;
	int32_t         data;
}messageElement_ts;

// need to create a syngleton ...
class MsgBroadcastCore: public Singleton<MsgBroadcastCore>
{
	friend class Singleton<MsgBroadcastCore>;
	// specific for sigleton system...
	private:
		// Constructeur
		MsgBroadcastCore(void);
		~MsgBroadcastCore(void);
	public:
		void SendMessage(MsgBroadcast * pointerOnSender, messageType_te id, int32_t dataID = -1);
		void AddReceiver(MsgBroadcast * pointerOnReceiver);
		void RmReceiver(MsgBroadcast * pointerOnReceiver);

	private:
		Edn::VectorType<MsgBroadcast*>         m_listMessage;
		uint32_t                            m_messageID;
		Edn::VectorType<messageElement_ts>     m_listOfMessage;
};


void GeneralSendMessage(messageType_te id, int32_t dataID = -1);





#endif

