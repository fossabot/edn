/**
 *******************************************************************************
 * @file EdnEdnVectorBuf.h
 * @brief Editeur De N'ours : Basic EdnVectorBuf for direct data insertion (template)
 * @author Edouard DUPIN
 * @date 07/04/2011
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

#ifndef __EDN_VECTOR_BUF_H__
#define __EDN_VECTOR_BUF_H__

#include <toolsMemory.h>
#include <etk/VectorType.h>

#undef __class__
#define __class__	"EdnVectorBuf"

// minimum gapSize when allocated
#define GAP_SIZE_MIN		(80)
// maximum gap that is automaticly resize
#define GAP_SIZE_MAX		(GAP_SIZE_MIN*4)


/*
                                                                                                                                      
                                                                                                                                      
                                                                                                                                      
              ______________________________________________________________________________________                                  
              | 																				   |								  
              | 																				   |								  
              | 				<GapStart														   |								  
              | 				*******************************************************************|								  
              |******************************************										   |								  
              | 								Gap Stop >                                         |								  
              | 																				   |								  
              | 																				   |								  
              | 																				   |								  
              | 																				   |								  
              | 																				   |								  
              | 																				   |								  
              | 																				   |								  
              | 																				   |								  
              | 																				   |								  
              | 																				   |								  
              | 																				   |								  
              | 																				   |								  
              | 																				   |								  
              | 																				   |								  
              | 																				   |								  
              | 																				   |								  
              | 																				   |								  
              | 																				   |								  
              | 																				   |								  
              | 																				   |								  
              | 																				   |								  
              | 																				   |								  
              | 																				   |								  
              | 																				   |								  
              | 																				   |								  
              | 																				   |								  
              |____________________________________________________________________________________|								  
                                                                                                                                      
                                                                                                                                      
                                                                                                                                      
                                                                                                                                      
                                                                                                                                      
                                                                                                                                      
                                                                                                                                      
                                                                                                                                      
                                                                                                                                      
                                                                                                                                      
*/











/**
 * @brief EdnVectorBuf classes ...
 */
class EdnVectorBuf
{
	public:
		class Iterator
		{
			// Private data : 
			private:
				int32_t				  m_current;		// curent Id on the vector
				EdnVectorBuf		* m_EdnVectorBuf;	// Pointer on the curent element of the vectorBin
			public:
				/**
				 * @brief Basic itarator constructor with no link with an EdnVector
				 */
				Iterator(void):
					m_current(-1),
					m_EdnVectorBuf(NULL)
				{
					// nothing to do ...
				}
				/**
				 * @brief Recopy constructor on a specific EdnVector.
				 * @param[in] otherIterator		The Iterator that might be copy
				 */
				Iterator(const Iterator & otherIterator):
					m_current(otherIterator.m_current),
					m_EdnVectorBuf(otherIterator.m_EdnVectorBuf)
				{
					// nothing to do ...
				}
				/**
				 * @brief Asignation operator.
				 * @param[in] otherIterator		The Iterator that might be copy
				 * @return reference on the curent Iterator
				 */
				Iterator& operator=(const Iterator & otherIterator)
				{
					m_current = otherIterator.m_current;
					m_EdnVectorBuf = otherIterator.m_EdnVectorBuf;
					return *this;
				}
				/**
				 * @brief Basic destructor
				 */
				~Iterator(void)
				{
					m_current = -1;
					m_EdnVectorBuf = NULL;
				}
				/**
				 * @brief gaet element position in the system
				 */
				int32_t Position(void)
				{
					if(0 > m_current) {
						return 0;
					} else if (m_EdnVectorBuf->Size() <= m_current) {
						return m_EdnVectorBuf->Size();
					} else {
						return m_current;
					}
				}
				/**
				 * @brief basic boolean cast
				 * @return true if the element is present in the EdnVector size
				 */
				operator bool ()
				{
					if(		0 <= m_current
						&&	m_current < m_EdnVectorBuf->Size() )
					{
						return true;
					} else {
						return false;
					}
				}
				/**
				 * @brief Incremental operator
				 * @return Reference on the current iterator incremented
				 */
				Iterator& operator++ ()
				{
					if(		NULL != m_EdnVectorBuf
						&&	m_current < m_EdnVectorBuf->Size() )
					{
						m_current++;
					}
					return *this;
				}
				/**
				 * @brief Decremental operator
				 * @return Reference on the current iterator decremented
				 */
				Iterator& operator-- ()
				{
					if (m_current >= 0) {
						m_current--;
					}
					return *this;
				}
				/**
				 * @brief Incremental operator
				 * @return Reference on a new iterator and increment the other one
				 */
				Iterator operator++ (int32_t)
				{
					Iterator it(*this);
					++(*this);
					return it;
				}
				/**
				 * @brief Decremental operator
				 * @return Reference on a new iterator and decrement the other one
				 */
				Iterator operator-- (int32_t)
				{
					Iterator it(*this);
					--(*this);
					return it;
				}
				/**
				 * @brief Get reference on the current Element
				 * @return the reference on the current Element 
				 */
				int8_t & operator-> () const
				{
					EDN_CHECK_INOUT(m_current >= 0 && m_current < m_EdnVectorBuf->Size());
					return m_EdnVectorBuf->Get(m_current);
				}
				/**
				 * @brief Get reference on the current Element
				 * @return the reference on the current Element 
				 */
				int8_t & operator* () const
				{
					EDN_CHECK_INOUT(m_current >= 0 && m_current < m_EdnVectorBuf->Size());
					return m_EdnVectorBuf->Get(m_current);
				}
			private:
				/**
				 * @brief 
				 *
				 * @param[in,out] ---
				 *
				 * @return ---
				 *
				 */
				Iterator(EdnVectorBuf * Evb, int32_t pos):
					m_current(pos),
					m_EdnVectorBuf(Evb)
				{
					// nothing to do ...
				}
				friend class EdnVectorBuf;
		};
	private:
		int8_t		* m_data;		//!< pointer on the curetn table of Data
		int32_t		  m_allocated;	//!< Current allocated size
		// empty part of the buffer data
		int32_t		  m_gapStart;	//!< points to the first character of the gap
		int32_t		  m_gapEnd;		//!< points to the first char after the gap
	public:
										EdnVectorBuf(int32_t count = 0);
										EdnVectorBuf(const EdnVectorBuf & Evb);
										~EdnVectorBuf();
		
		bool	 DumpIn(				FILE *myFile);
		bool	 DumpFrom(				FILE *myFile);
		
		EdnVectorBuf &					operator=(	const EdnVectorBuf & Evb);
		int8_t							operator[] (int32_t pos);
		int8_t &						Get(		int32_t pos);
		void							Get(		int32_t pos, int32_t nbElement, etk::VectorType<int8_t> &tmpBuffer);
		// insert functions
		void							PushBack(	const int8_t& item);
		void							Insert(		int32_t pos, const int8_t& item);
		void							Insert(		int32_t pos, etk::VectorType<int8_t>& items);
		// Remove and insert functions
		void							Replace(	int32_t pos, const int8_t& item);
		void							Replace(	int32_t pos, int32_t nbRemoveElement, etk::VectorType<int8_t>& items);
		// Revove fonctions
		void							Remove(		int32_t pos, int32_t nbRemoveElement = 1);
		void							PopBack(	void);
		void							Clear(		void);
		
		
		void							Fit(void);
		
		void							Display(void);
		
		/**
		 * @brief Get a current element in the vector (iterator system)
		 * @param[in] RealElementPosition Real position in the buffer (only use in the ITERATOR)
		 * @return Reference on the Element
		 */
		int8_t &	GetDirect(int32_t RealElementPosition){	return m_data[RealElementPosition];	};
		/**
		 * @brief Get the number of element in the vector
		 * @return The number requested
		 */
		int32_t		Size(void) { return m_allocated - GapSize(); };

		Iterator Position(int32_t pos)
		{
			return Iterator(this, pos);
		}

		/**
		 * @brief Get an Iterator on the start position of the Vector
		 * @return The Iterator
		 */
		Iterator Begin()
		{
			return Position(0);
		}

		/**
		 * @brief Get an Iterator on the end position of the Vector
		 * @return The Iterator
		 */
		Iterator End()
		{
			return Position( Size()-1 );
		}
	private:
		// TODO : Set a boolean at the return value to prevent internal error ...
		void				ChangeAllocation(	int32_t newSize);
		bool				GapMove(			int32_t pos);
		bool				GapResize(			int32_t pos, int32_t newGapLen);
		// get current gap Size
		int32_t				GapSize(			void) { return m_gapEnd - m_gapStart; };
		void				GapCheckMaxSize(	void);
};

#undef __class__
#define __class__	NULL

void TestEdnVectorBuf(void);

#endif
