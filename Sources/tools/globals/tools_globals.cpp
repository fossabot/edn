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

#include "tools_globals.h"
#include "ColorizeManager.h"
#include "MsgBroadcast.h"
#include <string>

#undef __class__
#define __class__	"globals"

// Variables privé du namespace

static std::string curentFileName = "???";


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
	GeneralSendMessage(EDN_MSG__REFRESH_DISPLAY);
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
	GeneralSendMessage(EDN_MSG__REFRESH_DISPLAY);
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



static bool shiftIsSet = false;
void globals::SetShift(void)
{
	shiftIsSet = true;
	EDN_INFO(" SHIFT => Set");
}

void globals::UnSetShift(void)
{
	shiftIsSet = false;
	EDN_INFO(" SHIFT => UnSet");
}

bool globals::IsSetShift(void)
{
	return shiftIsSet;
}



static bool altIsSet = false;
void globals::SetAlt(void)
{
	altIsSet = true;
}

void globals::UnSetAlt(void)
{
	altIsSet = false;
}

bool globals::IsSetAlt(void)
{
	return altIsSet;
}


static bool ctrlIsSet = false;
void globals::SetCtrl(void)
{
	ctrlIsSet = true;
}

void globals::UnSetCtrl(void)
{
	ctrlIsSet = false;
}

bool globals::IsSetCtrl(void)
{
	return ctrlIsSet;
}


static bool pommeIsSet = false;
void globals::SetPomme(void)
{
	pommeIsSet = true;
}

void globals::UnSetPomme(void)
{
	pommeIsSet = false;
}

bool globals::IsSetPomme(void)
{
	return pommeIsSet;
}

static bool insertIsSet = false;
void globals::SetInsert(void)
{
	insertIsSet = true;
}

void globals::UnSetInsert(void)
{
	insertIsSet = false;
}
void globals::ToggleInsert(void)
{
	if (true==insertIsSet) {
		insertIsSet = false;
	} else {
		insertIsSet = true;
	}
}

bool globals::IsSetInsert(void)
{
	return insertIsSet;
}




/*
 * Basic GUI system : 
 *
 *
 *
 *
 */



void globals::DisplaySystemString(std::vector<int32_t> &data)
{
	// Display the copyed data ... 
	uint32_t i;
	EDN_INFO("Display Data : ");
	printf(" ========================================================\n");
	for(i=0; i<data.size(); i++) {
#		ifdef USE_GTK_VERSION_2_0
		if (GDK_Return == data[i]) {
#       elif USE_GTK_VERSION_3_0
		if (GDK_KEY_Return == data[i]) {
#       endif
			printf("\n = ");
		} else {
			printf("%c", (char)data[i]);
		}
	}
	printf("\n ========================================================\n");
}



