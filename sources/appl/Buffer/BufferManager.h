/**
 * @author Edouard DUPIN
 * 
 * @copyright 2010, Edouard DUPIN, all right reserved
 * 
 * @license GPL v3 (see license file)
 */

#ifndef __BUFFER_MANAGER_H__
#define __BUFFER_MANAGER_H__

#include <BufferText.h>
#include <appl/globalMsg.h>
#include <ewol/widget/Widget.h>

namespace BufferManager
{
	void        init(void);
	void        UnInit(void);
	int32_t     getSelected(void);
	BufferText* get(int32_t BufferID);
	bool        Exist(int32_t BufferID);
	bool        Exist(etk::FSNode &myFile);
	int32_t     getId(etk::FSNode &myFile);
	// return the number of buffer (open in the past) if 5 buffer open and 4 close  == > return 5
	uint32_t    size(void);
	uint32_t    sizeOpen(void);
	int32_t     WitchBuffer(int32_t iEmeElement);
};

#endif

