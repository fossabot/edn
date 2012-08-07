/**
 *******************************************************************************
 * @file EdnBufHistory.h
 * @brief Editeur De N'ours : history of buffer modification (header)
 * @author Edouard DUPIN
 * @date 24/03/2011
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
 
#ifndef __EDN_BUFFER_HISTORY_H__
#define __EDN_BUFFER_HISTORY_H__


#include <vector>

class EdnBufHistory{
	public:
					 EdnBufHistory(void);
					 EdnBufHistory(int32_t pos, int32_t nInserted, std::vector<int8_t> &deletedText);
					~EdnBufHistory(void);
		void		 Set(int32_t pos, int32_t nInserted, std::vector<int8_t> &deletedText);
		int32_t		 getPos(void);
		int32_t		 getnbDeleted(void);
		int32_t		 getnbInserted(void);
		void		 getData(std::vector<int8_t> &deletedText);
	private:
		int32_t m_pos;
		int32_t m_nInserted;
		std::vector<int8_t> m_deletedText;
};

#endif

