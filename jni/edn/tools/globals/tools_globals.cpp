/**
 *******************************************************************************
 * @file tools_Globals.cpp
 * @brief Editeur De N'ours : Globals Values
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

#include <tools_globals.h>
#include <ColorizeManager.h>
#include <MsgBroadcast.h>
#include <ewol/WidgetMessageMultiCast.h>

#undef __class__
#define __class__	"globals"




erreurCode_te globals::init(void)
{
	erreurCode_te ret = ERR_NONE;
	

	return ret;
}



// -----------------------------------------------------------
static bool displayEOL = false;
bool globals::IsSetDisplayEndOfLine(void)
{
	return displayEOL;
}

void globals::SetDisplayEndOfLine(bool newVal)
{
	EDN_INFO("Set EndOfLine " << newVal);
	displayEOL = newVal;
	ewol::widgetMessageMultiCast::Send(-1, ednMsgUserDisplayChange);
}

// -----------------------------------------------------------
static bool displaySpaceChar = true;
bool globals::IsSetDisplaySpaceChar(void)
{
	return displaySpaceChar;
}

void globals::SetDisplaySpaceChar(bool newVal)
{
	EDN_INFO("Set SpaceChar " << newVal);
	displaySpaceChar = newVal;
	ewol::widgetMessageMultiCast::Send(-1, ednMsgUserDisplayChange);
}


// -----------------------------------------------------------
static bool AutoIndent = true;
bool globals::IsSetAutoIndent(void)
{
	return AutoIndent;
}

void globals::SetAutoIndent(bool newVal)
{
	EDN_INFO("Set AutoIndent " << newVal);
	AutoIndent = newVal;
}

// -----------------------------------------------------------




int32_t globals::getNbColoneBorder(void)
{
	return 6;
}

int32_t globals::getNbLineBorder(void)
{
	return 3;
}




