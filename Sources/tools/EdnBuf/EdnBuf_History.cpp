/**
 *******************************************************************************
 * @file EdnBuf_History.cpp
 * @brief Editeur De N'ours : Buffer for internal Data - section history (Sources)
 * @author Edouard DUPIN
 * @date 23/03/2011
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
#include "EdnBuf.h"


#undef __class__
#define __class__	"EdnBuf{History}"




int32_t EdnBuf::Undo(void)
{
	int32_t nbElement = m_historyUndo.Size();
	//EDN_DEBUG("EdnBuf::Undo Request id="<<nbElement);
	int32_t posDest = -1;
	if (0 == nbElement) {
		// nothing to do ...
		EDN_ERROR("EdnBuf::Undo No more History");
		return -1;
	}
	nbElement--;
	if (m_historyUndo[nbElement] == NULL) {
		EDN_ERROR("EdnBuf::Undo Find empty history ==> remove it");
		m_historyUndo.PopBack();
		return -1;
	}
	int32_t pos        = m_historyUndo[nbElement]->getPos();
	int32_t nbDeleted  = m_historyUndo[nbElement]->getnbDeleted();
	int32_t nbInserted = m_historyUndo[nbElement]->getnbInserted();
	EdnVectorBin<int8_t> deletedText;
	m_historyUndo[nbElement]->getData(deletedText);
	m_isUndoProcessing = true;
	if (0 == nbInserted) {
		// just add data at position ...
		if (0 == nbDeleted) {
			EDN_ERROR("EdnBuf::Undo nothing to do in UNDO");
		} else {
			Insert(pos, deletedText);
			posDest = pos + nbDeleted;
		}
	} else {
		if (0 == nbDeleted) {
			// only remove data
			Remove(pos, pos+nbInserted);
			posDest = pos;
		} else {
			// replace data
			Replace(pos, pos+nbInserted, deletedText);
			posDest = pos + nbDeleted;
		}
	}
	// remove element in the list :
	delete(m_historyUndo[nbElement]);
	m_historyUndo.PopBack();
	m_isUndoProcessing = false;
	return posDest;
}

int32_t EdnBuf::Redo(void)
{
	int32_t nbElement = m_historyRedo.Size();
	//EDN_DEBUG("EdnBuf::Redo Request id="<<nbElement);
	int32_t posDest = -1;
	if (0 == nbElement) {
		// nothing to do ...
		EDN_ERROR("EdnBuf::Redo No more History");
		return -1;
	}
	nbElement--;
	if (m_historyRedo[nbElement] == NULL) {
		EDN_ERROR("EdnBuf::Redo Find empty history ==> remove it");
		m_historyRedo.PopBack();
		return -1;
	}
	int32_t pos        = m_historyRedo[nbElement]->getPos();
	int32_t nbDeleted  = m_historyRedo[nbElement]->getnbDeleted();
	int32_t nbInserted = m_historyRedo[nbElement]->getnbInserted();
	EdnVectorBin<int8_t> deletedText;
	m_historyRedo[nbElement]->getData(deletedText);
	m_isRedoProcessing = true;
	if (0 == nbInserted) {
		// just add data at position ...
		if (0 == nbDeleted) {
			EDN_ERROR("EdnBuf::Redo nothing to do in REDO");
		} else {
			Insert(pos, deletedText);
			posDest = pos + nbDeleted;
		}
	} else {
		if (0 == nbDeleted) {
			// only remove data
			Remove(pos, pos+nbInserted);
			posDest = pos;
		} else {
			// replace data
			Replace(pos, pos+nbInserted, deletedText);
			posDest = pos + nbDeleted;
		}
	}
	// remove element in the list :
	delete(m_historyRedo[nbElement]);
	m_historyRedo.PopBack();
	m_isRedoProcessing = false;
	return posDest;
}


