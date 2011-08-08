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


#undef __class__
#define __class__	"EdnBuf"

static void		  addPadding(char *string, int32_t startIndent, int32_t toIndent, int32_t tabDist, int32_t useTabs, int32_t *charsAdded);
static int32_t	  textWidth(EdnVectorBin<int8_t> &text, int32_t tabDist);


/**
 *  @brief convertion table for non printable control caracters
 */
static const char *ControlCodeTable[32] = {
	"NUL", "soh", "stx", "etx", "eot", "enq", "ack", "bel", "bs",  "ht", "nl",  "vt",  "np", "cr", "so", "si",
	"dle", "dc1", "dc2", "dc3", "dc4", "nak", "syn", "etb", "can", "em", "sub", "esc", "fs", "gs", "rs", "us"};

#endif

/**
 * @brief Constructor of the Edn buffer Text :
 * 
 * Create an empty text buffer of a pre-determined size
 * 
 * @param[in] requestedSize		use this to avoid unnecessary re-allocation if you know exactly how much the buffer will need to hold
 * 
 */
EdnBuf::EdnBuf(void)
{
	m_tabDist = 4;
	m_useTabs = true;

	// Current selection
	m_selectionList[SELECTION_PRIMARY].selected = false;
	m_selectionList[SELECTION_PRIMARY].zeroWidth = false;
	m_selectionList[SELECTION_PRIMARY].rectangular = false;
	m_selectionList[SELECTION_PRIMARY].start = m_selectionList[SELECTION_PRIMARY].end = 0;
	m_selectionList[SELECTION_SECONDARY].selected = false;
	m_selectionList[SELECTION_SECONDARY].zeroWidth = false;
	m_selectionList[SELECTION_SECONDARY].rectangular = false;
	m_selectionList[SELECTION_SECONDARY].start = m_selectionList[SELECTION_SECONDARY].end = 0;
	m_selectionList[SELECTION_HIGHTLIGHT].selected = false;
	m_selectionList[SELECTION_HIGHTLIGHT].zeroWidth = false;
	m_selectionList[SELECTION_HIGHTLIGHT].rectangular = false;
	m_selectionList[SELECTION_HIGHTLIGHT].start = m_selectionList[SELECTION_HIGHTLIGHT].end = 0;

	// charset : 
	m_isUtf8 = false;
	m_charsetType = EDN_CHARSET_ISO_8859_1;
	
	m_isUndoProcessing = false;
	m_isRedoProcessing = false;
	
	// Load highlight system : 
	HighlightManager *myHighlightManager = HighlightManager::getInstance();
	EdnString plop = ".c";
	m_Highlight = myHighlightManager->Get(plop);
	//m_Highlight = NULL;
	m_nbLine = 1;
	m_HLDataSequence = 0;
}
