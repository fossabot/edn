/**
 * @author Edouard DUPIN
 * 
 * @copyright 2010, Edouard DUPIN, all right reserved
 * 
 * @license GPL v3 (see license file)
 */

#include <appl/debug.h>
#include <appl/global.h>
#include <EdnBuf.h>


#undef __class__
#define __class__	"EdnBuf{History}"




int32_t EdnBuf::Undo(void)
{
	int32_t nbElement = m_historyUndo.size();
	//APPL_DEBUG("EdnBuf::Undo Request id="<<nbElement);
	int32_t posDest = -1;
	if (0 == nbElement) {
		// nothing to do ...
		APPL_ERROR("EdnBuf::Undo No more History");
		return -1;
	}
	nbElement--;
	if (m_historyUndo[nbElement] == NULL) {
		APPL_ERROR("EdnBuf::Undo find empty history  == > remove it");
		m_historyUndo.popBack();
		return -1;
	}
	int32_t pos        = m_historyUndo[nbElement]->getPos();
	int32_t nbDeleted  = m_historyUndo[nbElement]->getnbDeleted();
	int32_t nbInserted = m_historyUndo[nbElement]->getnbInserted();
	etk::Vector<int8_t> deletedText;
	m_historyUndo[nbElement]->getData(deletedText);
	m_isUndoProcessing = true;
	if (0 == nbInserted) {
		// just add data at position ...
		if (0 == nbDeleted) {
			APPL_DEBUG("EdnBuf::Undo nothing to do in UNDO");
		} else {
			insert(pos, deletedText);
			posDest = pos + nbDeleted;
		}
	} else {
		if (0 == nbDeleted) {
			// only remove data
			remove(pos, pos+nbInserted);
			posDest = pos;
		} else {
			// replace data
			Replace(pos, pos+nbInserted, deletedText);
			posDest = pos + nbDeleted;
		}
	}
	// remove element in the list :
	delete(m_historyUndo[nbElement]);
	m_historyUndo.popBack();
	m_isUndoProcessing = false;
	return posDest;
}

int32_t EdnBuf::Redo(void)
{
	int32_t nbElement = m_historyRedo.size();
	//APPL_DEBUG("EdnBuf::Redo Request id="<<nbElement);
	int32_t posDest = -1;
	if (0 == nbElement) {
		// nothing to do ...
		APPL_DEBUG("EdnBuf::Redo No more History");
		return -1;
	}
	nbElement--;
	if (m_historyRedo[nbElement] == NULL) {
		APPL_ERROR("EdnBuf::Redo find empty history  == > remove it");
		m_historyRedo.popBack();
		return -1;
	}
	int32_t pos        = m_historyRedo[nbElement]->getPos();
	int32_t nbDeleted  = m_historyRedo[nbElement]->getnbDeleted();
	int32_t nbInserted = m_historyRedo[nbElement]->getnbInserted();
	etk::Vector<int8_t> deletedText;
	m_historyRedo[nbElement]->getData(deletedText);
	m_isRedoProcessing = true;
	if (0 == nbInserted) {
		// just add data at position ...
		if (0 == nbDeleted) {
			APPL_ERROR("EdnBuf::Redo nothing to do in REDO");
		} else {
			insert(pos, deletedText);
			posDest = pos + nbDeleted;
		}
	} else {
		if (0 == nbDeleted) {
			// only remove data
			remove(pos, pos+nbInserted);
			posDest = pos;
		} else {
			// replace data
			Replace(pos, pos+nbInserted, deletedText);
			posDest = pos + nbDeleted;
		}
	}
	// remove element in the list :
	delete(m_historyRedo[nbElement]);
	m_historyRedo.popBack();
	m_isRedoProcessing = false;
	return posDest;
}


