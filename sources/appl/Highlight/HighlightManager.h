/**
 * @author Edouard DUPIN
 * 
 * @copyright 2010, Edouard DUPIN, all right reserved
 * 
 * @license GPL v3 (see license file)
 */

#ifndef __HIGHLIGHT_MANAGER_H__
#define __HIGHLIGHT_MANAGER_H__


#include <etk/UString.h>
#include <appl/globalMsg.h>
#include <Highlight.h>
#include <ewol/widget/Widget.h>

namespace HighlightManager{
	void       init(void);
	void       UnInit(void);
	void       loadLanguages(void);
	Highlight* get(etk::FSNode &fileName);
	bool       Exist(etk::FSNode &fileName);
};


#endif

