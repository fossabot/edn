/**
 *******************************************************************************
 * @file EdnBuf.cpp
 * @brief Editeur De N'ours : Buffer for internal Data (Sources)
 * @author Edouard DUPIN
 * @date 23/03/2011
 * @par Projectqsdfqsdfq
 * Edn
 *
 *******************************************************************************
 */
 
/*
#if 0
*/


//#include "tools_debug.h"
#include "tools_globals.h"
#include "EdnBuf.h"

	m_isUndoProcessing = false;
	m_isRedoProcessing = false;
	
	// Load highlight system : 
	HighlightManager *myHighlightManager = HighlightManager::getInstance();
	EdnString plop = ".c";
	m_Highlight = myHighlightManager->Get(plop);
	//m_Highlight = NULL;
	m_nbLine = 1;
	m_HLDataSequence = 0;

