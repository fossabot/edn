/**
 * @author Edouard DUPIN
 * 
 * @copyright 2010, Edouard DUPIN, all right reserved
 * 
 * @license GPL v3 (see license file)
 */

#ifndef __EDN_BUFFER_HISTORY_H__
#define __EDN_BUFFER_HISTORY_H__


#include <etk/Vector.h>

class EdnBufHistory{
	public:
					 EdnBufHistory(void);
					 EdnBufHistory(int32_t pos, int32_t nInserted, etk::Vector<int8_t> &deletedText);
					~EdnBufHistory(void);
		void		 set(int32_t pos, int32_t nInserted, etk::Vector<int8_t> &deletedText);
		int32_t		 getPos(void);
		int32_t		 getnbDeleted(void);
		int32_t		 getnbInserted(void);
		void		 getData(etk::Vector<int8_t> &deletedText);
	private:
		int32_t m_pos;
		int32_t m_nInserted;
		etk::Vector<int8_t> m_deletedText;
};

#endif

