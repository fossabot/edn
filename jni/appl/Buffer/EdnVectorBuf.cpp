/**
 *******************************************************************************
 * @file EdnEdnVectorBuf.cpp
 * @brief Editeur De N'ours : Basic EdnVectorBuf Basic binary vector for all type of storage
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

#include <appl/Debug.h>
#include <appl/global.h>
#include <EdnVectorBuf.h>

#undef __class__
#define __class__	"EdnEdnVectorBuf"


/**
 * @brief Create an empty vector
 * 
 * @param[in] count		Minimum request size of the Buffer
 * 
 * @return ---
 * 
 */
EdnVectorBuf::EdnVectorBuf(int32_t count)
{
	m_data = NULL;
	m_allocated = 0;
	m_gapStart = 0;
	m_gapEnd = GAP_SIZE_MIN;
	ChangeAllocation(count+GAP_SIZE_MIN);
}


/**
 * @brief Re-copy constructor (copy all needed data)
 * 
 * @param[in] Evb	Vector that might be copy
 * 
 * @return ---
 * 
 */
EdnVectorBuf::EdnVectorBuf(const EdnVectorBuf & Evb)
{
	m_allocated = Evb.m_allocated;
	m_data = NULL;
	m_gapStart = Evb.m_gapStart;
	m_gapEnd = Evb.m_gapEnd;
	
	// allocate all same data
	m_data = (int8_t *)malloc( m_allocated * sizeof(int8_t) );
	APPL_ASSERT(NULL!=m_data, "Error in data allocation");
	// Copy all data ...
	memcpy(m_data, Evb.m_data, m_allocated * sizeof(int8_t) );
}


/**
 * @brief Destructor of the current Class
 * 
 * @param ---
 * 
 * @return ---
 * 
 */
EdnVectorBuf::~EdnVectorBuf()
{
	if (NULL!=m_data) {
		free(m_data);
		m_data = NULL;
		m_allocated = 0;
		m_gapStart = 0;
		m_gapEnd = 0;
	}
}


static int32_t getFileSize(FILE *myFile)
{
	if (NULL == myFile) {
		return 0;
	}
	int32_t size = 0;

	fseek(myFile, 0, SEEK_END);
	size = ftell(myFile);
	fseek(myFile, 0, SEEK_SET);
	return size;
}


/**
 * @brief Save in the current file open
 * 
 * @param[in,out] myFile	pointer on the file where data might be writed
 *
 * @return true if OK / false if an error occured
 *
 */
bool EdnVectorBuf::DumpIn(FILE *myFile)
{
	bool ret = true;
	// write Data
	(void)fwrite(m_data,            sizeof(int8_t), m_gapStart, myFile);
	(void)fwrite(&m_data[m_gapEnd], sizeof(int8_t), m_allocated - m_gapEnd, myFile);
	return ret;
}


/**
 * @brief Load in the current file open
 * 
 * @param[in,out] myFile	pointer on the file where data might be read
 *
 * @return true if OK / false if an error occured
 *
 */
bool EdnVectorBuf::DumpFrom(FILE *myFile)
{
	bool ret = true;
	int32_t length = getFileSize(myFile);
	// error case ...
	if (length < 0) {
		length = 0;
	}
	// allocate the current buffer : 
	ChangeAllocation(length + GAP_SIZE_MIN);
	
	// insert Data
	int32_t nbReadData = fread(&m_data[GAP_SIZE_MIN], sizeof(int8_t), length, myFile);
	APPL_INFO("load data : filesize=" << length << ", readData=" << nbReadData);
	// check ERROR
	if (nbReadData != length) {
		APPL_ERROR("load data pb : filesize=" << length << ", readData=" << nbReadData);
		ret = false;
	}
	// set the gapsize at the end ...
	m_gapStart = 0;
	m_gapEnd = GAP_SIZE_MIN;
	return ret;
}



/**
 * @brief Re-copy operator
 * 
 * @param[in] Evb	Vector that might be copy
 * 
 * @return reference on the curent re-copy vector
 * 
 */
EdnVectorBuf& EdnVectorBuf::operator=(const EdnVectorBuf & Evb)
{
	if( this != &Evb ) // avoid copy to itself
	{
		if (NULL!=m_data) {
			free(m_data);
			m_data = NULL;
		}
		// Set the new value
		m_allocated = Evb.m_allocated;
		m_gapStart  = Evb.m_gapStart;
		m_gapEnd    = Evb.m_gapEnd;
		// allocate all same data
		m_data = (int8_t *)malloc( m_allocated * sizeof(int8_t) );
		APPL_ASSERT(NULL!=m_data, "Error in data allocation");
		// Copy all data ...
		memcpy(m_data, Evb.m_data, m_allocated * sizeof(int8_t) );
	}
	// Return the curent pointer
	return *this;
}

int8_t EdnVectorBuf::operator[] (int32_t pos)
{
	APPL_ASSERT(0 <= pos || pos < Size(), "try to read an element non existing");
	if (pos < m_gapStart) {
		return m_data[pos];
	}
	return m_data[pos + m_gapEnd-m_gapStart];
}


/**
 * @brief Get a current element in the vector
 * 
 * @param[in] pos Desired position read
 * 
 * @return Reference on the Element
 * 
 */
int8_t& EdnVectorBuf::Get(int32_t pos)
{
	APPL_ASSERT(0 <= pos || pos < Size(), "try to read an element non existing");
	if (pos < m_gapStart) {
		return m_data[pos];
	}
	return m_data[pos + m_gapEnd-m_gapStart];
}


/**
 * @brief 
 * 
 * @param[in,out] ---
 * 
 * @return ---
 * 
 */
void EdnVectorBuf::Get(int32_t pos, int32_t nbElement, etk::VectorType<int8_t> &tmpBuffer)
{
	tmpBuffer.Clear();
	if (pos < m_gapStart) {
		if (pos + nbElement < m_gapStart) {
			tmpBuffer.PushBack(&m_data[pos], nbElement);
		} else {
			tmpBuffer.PushBack(&m_data[pos], m_gapStart - pos);
			tmpBuffer.PushBack(&m_data[m_gapEnd], nbElement - (m_gapStart - pos) );
		}
	} else {
		tmpBuffer.PushBack(&m_data[pos+(m_gapEnd-m_gapStart)], nbElement);
	}
}


/**
 * @brief Add at the Last position of the Vector
 * 
 * @param[in] item	Element to add at the end of vector
 * 
 * @return ---
 * 
 */
void EdnVectorBuf::PushBack(const int8_t& item)
{
	Insert( Size(), item);
}

/**
 * @brief Remove the last element of the vector
 * 
 * @param ---
 * 
 * @return ---
 * 
 */
void EdnVectorBuf::PopBack(void)
{
	if (Size()>0) {
		Remove( Size() );
	}
}


/**
 * @brief Remove data in the buffer
 * 
 * @param[in] 
 * 
 * @return ---
 * 
 */
void EdnVectorBuf::Remove(int32_t pos, int32_t nbRemoveElement)
{
	if(		pos > Size()
		||	pos < 0 ) {
		APPL_ERROR("Request higher than buffer size : pos="<<pos<< " bufferSize="<<Size());
		return;
	}
	if( pos+nbRemoveElement > Size() ) {
		APPL_ERROR("Request remove more element than expected in the buffer pos+nbRemoveElement="<<pos+nbRemoveElement<< " bufferSize="<<Size());
		return;
	}
	if (false == GapMove(pos) ) {
		return;
	}
	// Remove elements :
	if (m_allocated==m_gapEnd) {
		m_gapStart -= nbRemoveElement;
	} else {
		m_gapEnd += nbRemoveElement;
	}
	// Resize buffer if needed...
	GapCheckMaxSize();
}

/**
 * @brief Change the current allocation to the corect one (depend on the current size)
 * 
 * @param[in] newSize Minimum number of element needed
 * 
 * @return ---
 * 
 */
void EdnVectorBuf::Clear(void)
{
	// Remove all element in the buffer
	Remove(0, Size() );
}


/**
 * @brief Change the current allocation to the corect one (depend on the current size)
 * 
 * @param[in] newSize Minimum number of element needed
 * 
 * @return ---
 * 
 */
void EdnVectorBuf::ChangeAllocation(int32_t newSize)
{
	// set the minimal size to 1
	if(newSize <= 0) {
		newSize = 1;
	}
	// set the size with the corect chose type : 
	if (newSize == m_allocated) {
		return;
	}
	APPL_DEBUG("Change Allocation : " << m_allocated << " ==> " << newSize);
	// check if something is allocated : 
	if (NULL == m_data) {
		// no data allocated ==> request an allocation (might be the first)
		m_data = (int8_t *)malloc( newSize * sizeof(int8_t) );
	} else {
		// move datas
		m_data = (int8_t *)realloc( m_data, newSize* sizeof(int8_t) );
	}
	// Check result with assert : 
	APPL_ASSERT(NULL!=m_data, "Error in data allocation");
	// set the new allocation size
	m_allocated = newSize;
}


/**
 * @brief 
 * 
 * @param[in,out] ---
 * 
 * @return ---
 * 
 */
void EdnVectorBuf::Insert(int32_t pos, const int8_t& item)
{
	if(		pos > Size()
		||	pos < 0 ) {
		APPL_ERROR("Request higher than buffer size : pos="<<pos<< " bufferSize="<<Size());
		return;
	}
	if( 0 == GapSize() ) {
		if (false == GapResize(pos, GAP_SIZE_MIN + 1) ) {
			return;
		}
	} else if(		pos == m_gapStart
				&&	pos == m_gapEnd-1 )
	{
		// mothing to do ...
	} else {
		if (false == GapMove(pos)) {
			return;
		}
	}
	if(pos == m_gapStart) {
		m_data[m_gapStart] = item;
		m_gapStart++;
	} else {
		m_data[m_gapEnd-1] = item;
		m_gapEnd--;
	}
}


/**
 * @brief 
 * 
 * @param[in,out] ---
 * 
 * @return ---
 * 
 */
void EdnVectorBuf::Insert(int32_t pos, etk::VectorType<int8_t>& items)
{
	if(		pos > Size()
		||	pos < 0 ) {
		APPL_ERROR("Request higher than buffer size : pos="<<pos<< " bufferSize="<<Size());
		return;
	}
	if( items.Size() > GapSize() ) {
		if (false == GapResize(pos, GAP_SIZE_MIN + items.Size()) ) {
			return;
		}
	} else {
		if (false == GapMove(pos) ) {
			return;
		}
	}
	int32_t i;
	for(i=0; i<items.Size(); i++) {
		m_data[m_gapStart+i] = items[i];
	}
	m_gapStart += items.Size();
}


/**
 * @brief 
 * 
 * @param[in,out] ---
 * 
 * @return ---
 * 
 */
void EdnVectorBuf::Replace(int32_t pos, const int8_t& item)
{
	if(		pos > Size()
		||	pos < 0 ) {
		APPL_ERROR("Request higher than buffer size : pos="<<pos<< " bufferSize="<<Size());
		return;
	}
	// just replace the element, not update Gap position
	if (pos < m_gapStart) {
		m_data[pos] = item;
	} else {
		m_data[pos+GapSize()] = item;
	}
}


/**
 * @brief 
 * 
 * @param[in,out] ---
 * 
 * @return ---
 * 
 */
void EdnVectorBuf::Replace(int32_t pos, int32_t nbRemoveElement, etk::VectorType<int8_t>& items)
{
	if(		pos > Size()
		||	pos < 0 ) {
		APPL_ERROR("Request higher than buffer size : pos="<<pos<< " bufferSize="<<Size());
		return;
	}
	if( pos+nbRemoveElement > Size() ) {
		APPL_ERROR("Request remove more element than expected in the buffer pos+nbRemoveElement="<<pos+nbRemoveElement<< " bufferSize="<<Size());
		return;
	}
	if (false == GapMove(pos)) {
		return;
	}
	// Remove elements :
	m_gapEnd += nbRemoveElement;
	//Display();
	// insert elements
	Insert(pos, items);
	// Resize buffer if needed...
	GapCheckMaxSize();
}


/**
 * @brief 
 * 
 * @param[in,out] ---
 * 
 * @return ---
 * 
 */
bool EdnVectorBuf::GapMove(int32_t pos)
{
	if(		pos > Size()
		||	pos < 0 ) {
		APPL_ERROR("Request higher than buffer size : pos="<<pos<< " bufferSize="<<Size());
		return false;
	}
	int32_t gapLen = m_gapEnd - m_gapStart;
	if (pos > m_gapStart) {
		memmove(&m_data[m_gapStart], &m_data[m_gapEnd], pos - m_gapStart);
	} else {
		memmove(&m_data[pos + gapLen], &m_data[pos], m_gapStart - pos);
	}
	m_gapEnd += pos - m_gapStart;
	m_gapStart += pos - m_gapStart;
	return true;
}


/**
 * @brief 
 * 
 * @param[in,out] ---
 * 
 * @return ---
 * 
 */
bool EdnVectorBuf::GapResize(int32_t pos, int32_t newGapLen)
{
	if(		pos > Size()
		||	pos < 0 ) {
		APPL_ERROR("Request higher than buffer size : pos="<<pos<< " bufferSize="<<Size());
		return false;
	}
	int32_t previousSize = Size();
	if (newGapLen == GapSize() ) {
		// nothing to do ...
		return true;
	} else {
		if (newGapLen > GapSize() ) {
			// reallocation
			ChangeAllocation( previousSize + newGapLen);
		}
		// move Data
		if (pos <= m_gapStart) {
			// just move the end of the gap
			memmove(&m_data[m_gapStart + newGapLen], &m_data[m_gapEnd], previousSize - m_gapStart);
			// update gap end position
			m_gapEnd = m_gapStart + newGapLen;
			if (pos < m_gapStart) {
				if (false == GapMove(pos)) {
					return false;
				}
			}
			// no else
		} else {
			if (false == GapMove(pos) ) {
				return false;
			}
			memmove(&m_data[m_gapStart + newGapLen], &m_data[m_gapEnd], previousSize - m_gapStart);
		}
		if (newGapLen < GapSize() ) {
			// rellocation
			ChangeAllocation(previousSize + newGapLen);
		}
	}
	// update gap position
	m_gapStart = pos;
	m_gapEnd = pos + newGapLen;
	return true;
}


/**
 * @brief 
 * 
 * @param[in,out] ---
 * 
 * @return ---
 * 
 */
void EdnVectorBuf::GapCheckMaxSize(void)
{
	if(GapSize() > GAP_SIZE_MAX) {
		int32_t currentSize = Size();
		// Change the gap Size
		if (false == GapResize(m_gapStart, GAP_SIZE_MAX) ) {
			return;
		}
		// remove deprecated elements at the end of the buffer ...
		ChangeAllocation(currentSize + GAP_SIZE_MAX);
	}
}

/**
 * @brief 
 * 
 * @param[in,out] ---
 * 
 * @return ---
 * 
 */
void EdnVectorBuf::Display(void)
{
	APPL_INFO(" Display Buffer : Size="<<Size()<<" m_allocated="<<m_allocated<<" m_gapStart="<<m_gapStart<<" m_gapEnd="<<m_gapEnd);
	for(int32_t i=0; i<m_allocated; i++) {
		if (i>= m_gapStart && i< m_gapEnd) {
			APPL_INFO( "Element " << i << " : GAP");
		} else {
			APPL_INFO( "Element " << i << " : " << m_data[i]);
		}
	}
}



/**
 * @brief 
 * 
 * @param[in,out] ---
 * 
 * @return ---
 * 
 */
void TestEdnVectorBuf(void)
{
	EdnVectorBuf myBufferTmp;
	int32_t i;
	
	//invert data
	for (i=0; i<50; i++) {
		myBufferTmp.Insert(0, 'a' + i%26);
	}
	myBufferTmp.Display();
	myBufferTmp.Clear();
	myBufferTmp.Display();
	
	myBufferTmp.Remove(2, 300);
	/*
	char plop='a';
	myBufferTmp.Insert(0, plop);
	myBufferTmp.Display();
	plop='b';
	myBufferTmp.Insert(0, plop);
	myBufferTmp.Display();
	plop='c';
	myBufferTmp.Insert(0, plop);
	myBufferTmp.Display();
	plop='d';
	myBufferTmp.Insert(0, plop);
	myBufferTmp.Display();
	plop='e';
	myBufferTmp.Insert(0, plop);
	myBufferTmp.Display();
	plop='f';
	myBufferTmp.Insert(0, plop);
	myBufferTmp.Display();
	plop='g';
	myBufferTmp.Insert(0, plop);
	myBufferTmp.Display();
	plop='h';
	myBufferTmp.Insert(0, plop);
	myBufferTmp.Display();
	plop='m';
	
	etk::VectorType<int8_t> items;
	items.PushBack('i');
	items.PushBack('j');
	items.PushBack('k');
	items.PushBack('l');
	items.PushBack('m');
	items.PushBack('n');
	items.PushBack('o');
	items.PushBack('p');
	
	
	
	myBufferTmp.Insert(3, items);
	myBufferTmp.Display();
	
	
	
	plop='7';
	myBufferTmp.Insert(7, plop);
	myBufferTmp.Display();
	
	myBufferTmp.Replace(8, 'z');
	myBufferTmp.Display();
	
	items.Clear();
	items.PushBack('1');
	items.PushBack('2');
	items.PushBack('3');
	myBufferTmp.Replace(10, 4, items);
	myBufferTmp.Display();

	
	myBufferTmp.PushBack('a');
	myBufferTmp.PushBack('a');
	myBufferTmp.PushBack('a');
	myBufferTmp.PushBack('a');
	myBufferTmp.Display();
	
	
	myBufferTmp.PopBack();
	myBufferTmp.PopBack();
	myBufferTmp.PopBack();
	myBufferTmp.PopBack();
	myBufferTmp.Display();
	
	myBufferTmp.Remove(2, 3);
	myBufferTmp.Display();
	*/
	
}


