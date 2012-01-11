/**
 *******************************************************************************
 * @file AccelKey.h
 * @brief Editeur De N'ours : Basic Gui Accelerator Key (common for ALL) or nearly (header)
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
 
#ifndef __ACCEL_KEY_H__
#define __ACCEL_KEY_H__

#include "tools_debug.h"
#include "Singleton.h"

// need to create a syngleton ...
class AccelKey: public Singleton<AccelKey>
{
	friend class Singleton<AccelKey>;
	// specific for sigleton system...
	private:
		// Constructeur
		AccelKey(void);
		~AccelKey(void);
	public:
		// for internal Parsing
		void SetAccel(GtkWidget * widget, char * accelKey);
		// For external parsing
		void SetAccel(GtkWidget * widget, GtkAccelGroup * accel, char * accelKey);
		void LinkCommonAccel(GtkWidget * widget);
		void LinkCommonAccel(GtkWindow * widget);
		GtkAccelGroup * GetAccel(void) { return m_accelGroup; };

	private:
		GtkAccelGroup * m_accelGroup;
};


#endif

