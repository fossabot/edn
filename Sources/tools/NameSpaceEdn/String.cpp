/**
 *******************************************************************************
 * @file EdnString.cpp
 * @brief Editeur De N'ours : normal sting management... (sources)
 * @author Edouard DUPIN
 * @date 26/01/2011
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

#include "Edn.h"
#include "toolsMemory.h"


#undef __class__
#define __class__	"EdnString"


/**
 * @brief 
 *
 * @param[in,out] 
 *
 * @return 
 *
 */
Edn::String::~String(void)
{
	m_data.Clear();
}


/**
 * @brief 
 *
 * @param[in,out] 
 *
 * @return 
 *
 */
Edn::String::String(void)
{
	//EDN_INFO("new Edn::String()");
	m_data.Clear();
	m_data.PushBack('\0');
}


/**
 * @brief 
 *
 * @param[in,out] 
 *
 * @return 
 *
 */
Edn::String::String(const char myInput)
{
	m_data.Clear();
	m_data.PushBack(myInput);
	m_data.PushBack('\0');
}


/**
 * @brief 
 *
 * @param[in,out] 
 *
 * @return 
 *
 */
Edn::String::String(const char* inputData, int32_t len)
{
	m_data.Clear();
	m_data.PushBack('\0');
	Set(inputData, len);
}

void Edn::String::Set(const char * inputData, int32_t len)
{
	// overwrite the len if needed : 
	if ((-1) == len) {
		len = strlen(inputData);
	}
	
	if (len != 0) {
		// remove the last '\0'
		m_data.PopBack();
		// copy the data ...
		m_data.PushBack((int8_t*)inputData, len);
		// add the last '\0'
		m_data.PushBack('\0');
	}
}

/**
 * @brief 
 *
 * @param[in,out] 
 *
 * @return 
 *
 */
Edn::String::String(int inputData)
{
	char tmpVal[256];
	// generate the string : 
	sprintf(tmpVal, "%d", inputData);
	// set the internal data : 
	m_data.Clear();
	m_data.PushBack('\0');
	Set(tmpVal);
}


/**
 * @brief 
 *
 * @param[in,out] 
 *
 * @return 
 *
 */
Edn::String::String(unsigned int inputData)
{
	char tmpVal[256];
	// generate the string : 
	sprintf(tmpVal, "%d", inputData);
	// set the internal data : 
	m_data.Clear();
	m_data.PushBack('\0');
	Set(tmpVal);
}

Edn::String::String(const Edn::String &ednS)
{
	//EDN_INFO("Constructeur de recopie");
	m_data = ednS.m_data;
}


/**
 * @brief 
 *
 * @param[in,out] 
 *
 * @return 
 *
 */
const Edn::String& Edn::String::operator= (const Edn::String &ednS )
{
	//EDN_INFO("OPERATOR de recopie");
	if( this != &ednS ) // avoid copy to itself
	{
		m_data = ednS.m_data;
	}
	return *this;
}


/**
 * @brief 
 *
 * @param[in,out] 
 *
 * @return 
 *
 */
const Edn::String& Edn::String::operator= (const char * inputData)
{
	m_data.Clear();
	m_data.PushBack('\0');
	// calculate the size : 
	uint32_t len = strlen(inputData);
	// check the new size ...
	if (len > 0 ) {
		// copy all data : 
		Set(inputData, len);
	}
	return *this;
}


/**
 * @brief 
 *
 * @param[in,out] 
 *
 * @return 
 *
 */
const Edn::String& Edn::String::operator= (Edn::VectorType<int8_t> inputData)
{
	m_data = inputData;
	if (m_data.Size()>0) {
		if (m_data[m_data.Size()-1] != '\0') {
			m_data.PushBack('\0');
		}
	}
	//EDN_DEBUG("m_dataLen="<<m_dataLen << " m_dataLenUTF8="<<m_dataLenUTF8 << " description=" << m_data);
	return *this;
}


/**
 * @brief 
 *
 * @param[in,out] 
 *
 * @return 
 *
 */
bool Edn::String::operator== (const Edn::String& ednS) const
{
	if( this != &ednS ) {
		if (ednS.m_data.Size() != m_data.Size()) {
			//EDN_DEBUG(" not the same size : " << ednS.m_data.Size() << "!=" << m_data.Size());
			return false;
		}
		for (int32_t iii= 0; iii<m_data.Size(); iii++) {
			//EDN_DEBUG("     check : " << ednS.m_data[iii] << "!=" << m_data[iii]);
			if (ednS.m_data[iii]!= m_data[iii]){
				return false;
			}
		}
		return true;
	}
	return true;
}


/**
 * @brief 
 *
 * @param[in,out] 
 *
 * @return 
 *
 */
bool Edn::String::operator== (const char * inputData) const
{

	// calculate the size : 
	int32_t len = strlen(inputData);
	if (len+1 != m_data.Size()) {
		return false;
	}
	for (int32_t iii= 0; iii<m_data.Size(); iii++) {
		if (inputData[iii]!= m_data[iii]){
			return false;
		}
	}
	return true;
}


/**
 * @brief 
 *
 * @param[in,out] 
 *
 * @return 
 *
 */
bool Edn::String::operator!= (const Edn::String& ednS) const
{
	return !(*this == ednS);
}


/**
 * @brief 
 *
 * @param[in,out] 
 *
 * @return 
 *
 */
bool Edn::String::operator!= (const char * inputData) const
{
	return !(*this == inputData);
}


/**
 * @brief 
 *
 * @param[in,out] 
 *
 * @return 
 *
 */
const Edn::String& Edn::String::operator+= (const Edn::String &ednS)
{
	if (0 < ednS.Size()) {
		// remove the last '\0'
		m_data.PopBack();
		// copy the data ...
		m_data += ednS.m_data;
		// This previous include the \0 in case of the 2 string are different...
		if( this == &ednS ) {
			// add the removed end string
			m_data.PushBack('\0');
		}
	}
	return *this;
}


/**
 * @brief 
 *
 * @param[in,out] 
 *
 * @return 
 *
 */
const Edn::String& Edn::String::operator+= (const char * inputData)
{
	//EDN_INFO("        string(arg) : \"" << inputData << "\"");
	//EDN_INFO("        string(direct) : \"" << m_data << "\"");
	int32_t len = strlen(inputData);
	
	if (len != 0) {
		// remove the last '\0'
		m_data.PopBack();
		// copy the data ...
		m_data.PushBack((int8_t*)inputData, len+1 );
	}
	return *this;
}


/**
 * @brief 
 *
 * @param[in,out] 
 *
 * @return 
 *
 */
Edn::String Edn::String::operator+ (const Edn::String &ednS)
{
	Edn::String temp;
	//EDN_INFO("        string(arg) : \"" << ednS.m_data << "\"");
	//EDN_INFO("        string(direct) : \"" << m_data << "\"");
	temp += *this;
	temp += ednS;
	return temp;
}


/**
 * @brief 
 *
 * @param[in,out] 
 *
 * @return 
 *
 */
Edn::String Edn::String::operator+ (const char * inputData)
{
	Edn::String temp;
	//EDN_INFO("        string(arg) : \"" << inputData << "\"");
	//EDN_INFO("        string(direct) : \"" << m_data << "\"");
	temp += *this;
	temp += inputData;
	return temp;
}





/**
 * @brief 
 *
 * @param[in,out] 
 *
 * @return 
 *
 */
bool Edn::String::IsEmpty(void) const
{
	if(1 >= m_data.Size() ) {
		return true;
	} else {
		return false;
	}
}


/**
 * @brief 
 *
 * @param[in,out] 
 *
 * @return 
 *
 */
int32_t Edn::String::Size(void) const
{
	if (m_data.Size() == 0) {
		return 0;
	} else {
		return m_data.Size() - 1;
	}
}


/**
 * @brief 
 *
 * @param[in,out] 
 *
 * @return 
 *
 */
void Edn::String::Add(int32_t currentID, const char* inputData)
{
	// get the input lenght
	int32_t len = strlen(inputData);
	if (0 == len) {
		EDN_WARNING("no data to add on the current string");
		return;
	} else if (currentID < 0) {
		EDN_WARNING("Curent ID(" << currentID << ") < 0   ==> Add at the start");
		currentID = 0;
	} else if (currentID > Size() ) {
		EDN_ERROR("Curent ID(" << currentID << ") > maxSize ... (" << Size() << ")  ==> add at the end ...");
		m_data.PushBack((int8_t*)inputData, len);
		return;
	}
	m_data.Insert(currentID, (int8_t*)inputData, len);
}


/**
 * @brief 
 *
 * @param[in,out] 
 *
 * @return 
 *
 */
void Edn::String::Remove(int32_t currentID, int32_t len)
{
	if (0 >= len) {
		EDN_ERROR("no data to remove on the current string");
		return;
	}
	// TODO : check the size of the data
	m_data.Erase(currentID, len);
}


/**
 * @brief Remove all element in the string
 *
 * @param ---
 *
 * @return ---
 *
 */
void Edn::String::Clear(void)
{
	m_data.Clear();
	m_data.PushBack('\0');
}



/**
 * @brief find the first accurence after the position indicated
 *
 * @param[in] element Element that might be find in the string
 * @param[in] startPos Stert position to begin the search
 *
 * @return the position of the first occurence or -1 if not find...
 *
 */
int32_t Edn::String::FindForward(const char element, int32_t startPos)
{
	if (startPos < 0) {
		startPos = 0;
	} else if (startPos >= Size() ) {
		return -1;
	}
	for (int32_t iii=startPos; iii< Size(); iii++) {
		if (m_data[iii] == element) {
			return iii;
		}
	}
	return -1;
}


/**
 * @brief find the first accurence before the position indicated.
 *
 * @param[in] element Element that might be find in the string
 * @param[in] startPos Stert position to begin the search
 *
 * @return the position of the first occurence begining by the end or -1 if not find...
 *
 */
int32_t Edn::String::FindBack(const char element, int32_t startPos)
{
	if (startPos < 0) {
		return -1;
	} else if (startPos >= Size() ) {
		startPos = Size();
	}
	for (int32_t iii=startPos; iii>=0; iii--) {
		if (m_data[iii] == element) {
			return iii;
		}
	}
	return -1;
}


/**
 * @brief Extract data from the data between two position
 *
 * @param[in] posStart Start position where to extract data
 * @param[in] posEnd End position where to extract data
 *
 * @return the extracted string
 *
 */
Edn::String Edn::String::Extract(int32_t posStart, int32_t posEnd)
{
	Edn::String out;
	if (posStart < 0) {
		posStart = 0;
	} else if (posStart >= Size() ) {
		return out;
	}
	if (posEnd < 0) {
		return out;
	} else if (posEnd >= Size() ) {
		posEnd = Size();
	}
	out.m_data = m_data.Extract(posStart, posEnd);
	out.m_data.PushBack('\0');
	return out;
}


/**
 * @brief Get a basic vector in int8 data with no \0 at the end of the string
 *
 * @param ---
 *
 * @return The desired vector with data
 *
 */
Edn::VectorType<int8_t> Edn::String::GetVector(void)
{
	Edn::VectorType<int8_t> out = m_data;
	out.PopBack();
	return out;
}

/**
 * @brief Unitary test for the string system
 *
 * @param ---
 *
 * @return ---
 *
 */
void Edn::TestUntaire_String(void)
{
	EDN_WARNING("*********************************************************");
	EDN_WARNING("**  Test Unitaire 'EdnString'  (START)");
	EDN_WARNING("*********************************************************");
	
	int32_t iddd = 0;
	Edn::String * monString = new Edn::String();
	EDN_INFO("phase : " << iddd++ << " : \"" << monString->c_str() << "\"");
	delete(monString);
	
	monString = new Edn::String("test de direct data");
	EDN_INFO("phase : " << iddd++ << " : \"" << monString->c_str() << "\"");
	delete(monString);
	
	monString = new Edn::String("test de direct data", 7);
	EDN_INFO("phase : " << iddd++ << " : \"" << monString->c_str() << "\"");
	delete(monString);
	
	int32_t testId = -6789;
	monString = new Edn::String(testId);
	EDN_INFO("phase : " << iddd++ << " : \"" << monString->c_str() << "\"");
	delete(monString);
	
	uint32_t testId2 = 12345;
	monString = new Edn::String((unsigned int)testId2);
	EDN_INFO("phase : " << iddd++ << " : \"" << monString->c_str() << "\"");
	delete(monString);
	
	Edn::String plop = "otherString";
	monString = new Edn::String(plop);
	EDN_INFO("phase : " << iddd++ << " : \"" << monString->c_str() << "\"");
	delete(monString);
	
	
	Edn::String s1 = "test de base ...";
	s1 += s1;
	EDN_INFO("phase : " << iddd++ << " : \"" << s1.c_str() << "\"");
	s1 += " plop 2 ";
	EDN_INFO("phase : " << iddd++ << " : \"" << s1.c_str() << "\"");
	s1 += plop;
	EDN_INFO("phase : " << iddd++ << " : \"" << s1.c_str() << "\"");
	s1 = plop;
	EDN_INFO("phase : " << iddd++ << " : \"" << s1.c_str() << "\"");
	s1 = "test direct 44";
	EDN_INFO("phase : " << iddd++ << " : \"" << s1.c_str() << "\"");
	Edn::VectorType<int8_t> vb1;
	vb1.PushBack('v');
	vb1.PushBack('b');
	vb1.PushBack('1');
	s1 = vb1;
	EDN_INFO("phase : " << iddd++ << " : \"" << s1.c_str() << "\"");
	vb1.Clear();
	vb1.PushBack('v');
	vb1.PushBack('b');
	vb1.PushBack('2');
	vb1.PushBack('\0');
	s1 = vb1;
	EDN_INFO("phase : " << iddd++ << " : \"" << s1.c_str() << "\"");
	
	if (s1 == "vb2") {
		EDN_INFO("phase : " << iddd++ << " : == OK");
	} else {
		EDN_ERROR("phase : " << iddd++ << " : == ERROR");
	}
	
	
	if (s1 == "vb3") {
		EDN_ERROR("phase : " << iddd++ << " : == ERROR");
	} else {
		EDN_INFO("phase : " << iddd++ << " : == OK");
	}
	
	
	if (s1 != "vb3") {
		EDN_INFO("phase : " << iddd++ << " : == OK");
	} else {
		EDN_ERROR("phase : " << iddd++ << " : == ERROR");
	}
	
	
	if (s1 != "vb2") {
		EDN_ERROR("phase : " << iddd++ << " : == ERROR");
	} else {
		EDN_INFO("phase : " << iddd++ << " : == OK");
	}
	
	
	Edn::String s2 = "vb2";
	Edn::String s3 = "vb3";
	
	if (s1 == s2) {
		EDN_INFO("phase : " << iddd++ << " : == OK");
	} else {
		EDN_ERROR("phase : " << iddd++ << " : == ERROR");
	}
	
	
	if (s1 == s3) {
		EDN_ERROR("phase : " << iddd++ << " : == ERROR");
	} else {
		EDN_INFO("phase : " << iddd++ << " : == OK");
	}
	
	
	if (s1 != s3) {
		EDN_INFO("phase : " << iddd++ << " : == OK");
	} else {
		EDN_ERROR("phase : " << iddd++ << " : == ERROR");
	}
	
	
	if (s1 != s2) {
		EDN_ERROR("phase : " << iddd++ << " : == ERROR");
	} else {
		EDN_INFO("phase : " << iddd++ << " : == OK");
	}
	EDN_WARNING("*********************************************************");
	EDN_WARNING("**  Test Unitaire 'EdnString'  (STOP)");
	EDN_WARNING("*********************************************************");
}

