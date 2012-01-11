/**
 *******************************************************************************
 * @file AccelKey.cpp
 * @brief Editeur De N'ours : Basic Gui Accelerator Key (common for ALL) or nearly (Sources)
 * @author Edouard DUPIN
 * @date 17/06/2011
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
#include "AccelKey.h"

AccelKey::AccelKey(void)
{
	m_accelGroup = gtk_accel_group_new();
}

AccelKey::~AccelKey(void)
{
	
}

void AccelKey::SetAccel(GtkWidget * widget, char * accelKey)
{
	SetAccel(widget, m_accelGroup, accelKey);
}

void AccelKey::SetAccel(GtkWidget * widget, GtkAccelGroup * accel, char * accelKey)
{
	guint accel_key = 0;
	int32_t accel_mods = 0;
	if(		NULL==accelKey
		||	0==strlen(accelKey))
	{
		return;
	}
	// parsing of the string :
	//"ctrl+shift+alt+pomme+s"
	//EDN_DEBUG("Parse acxel string : \"" << accelKey << "\"");
	char * tmp = strstr(accelKey, "ctrl");
	if(NULL != tmp) {
		accel_mods |= GDK_CONTROL_MASK;
		//EDN_DEBUG("   => find CTRL");
	}
	tmp = strstr(accelKey, "shift");
	if(NULL != tmp) {
		accel_mods |= GDK_SHIFT_MASK;
		//EDN_DEBUG("   => find SHIFT");
	}
	tmp = strstr(accelKey, "alt");
	if(NULL != tmp) {
		accel_mods |= GDK_MOD1_MASK;
		//EDN_DEBUG("   => find ALT");
	}
	tmp = strstr(accelKey, "pomme");
	if(NULL != tmp) {
		accel_mods |= GDK_MOD2_MASK;
		//EDN_DEBUG("   => find POMME");
	}
	accel_key = accelKey[strlen(accelKey) -1];
	//char plop = accel_key;
	//EDN_DEBUG("   => find letter : '" << plop << "'");

	// Ajout du racourcis clavier :
	gtk_widget_add_accelerator(	widget, "activate", accel,
									accel_key, // key
									(GdkModifierType)accel_mods, // modifier keys
									GTK_ACCEL_VISIBLE);
}

void AccelKey::LinkCommonAccel(GtkWidget * widget)
{
	gtk_window_add_accel_group(GTK_WINDOW(widget), m_accelGroup);
}

void AccelKey::LinkCommonAccel(GtkWindow * widget)
{
	gtk_window_add_accel_group(GTK_WINDOW(widget), m_accelGroup);
}


