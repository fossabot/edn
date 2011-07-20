/**
 *******************************************************************************
 * @file EdnString.h
 * @brief Editeur De N'ours : normal sting management... (header)
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

#ifndef _IN_NAMESPACE_EDN_
#	error This will be include only in "edn.h"
#else 

	class String
	{
		public:
			// Constructeurs
			String(void);
			String(const char myInput);
			String(const char* inputData, int32_t len = -1);//, bool noAllocation=false);
			void Set(const char* inputData, int32_t len=-1);
			// basic convertion integer en string
			String(int inputData);
			String(unsigned int inputData);
			//String(const wchar_t *inputData);
			String(const Edn::String &ednS);
			// destructor : 
			~String(void);
			
			const Edn::String& operator= (const Edn::String &ednS );	// assigment
			const Edn::String& operator= (const char * inputData);
			const Edn::String& operator= (EdnVectorBin<int8_t> inputData);
			bool operator== (const Edn::String& ednS) const;			// == operator
			bool operator== (const char * inputData) const;
			bool operator!= (const Edn::String& ednS) const;			// != operator
			bool operator!= (const char * inputData) const;
			const Edn::String& operator+= (const Edn::String &ednS);	// += operator
			const Edn::String& operator+= (const char * inputData);
			Edn::String operator+ (const Edn::String &ednS);			// + operator
			Edn::String operator+ (const char * inputData);
			//operator const char *()
	
	
			bool          IsEmpty(void) const;
			int32_t       Size(void) const;
	
			void          Add(int32_t currentID, const char* inputData);
			void          Remove(int32_t currentID, int32_t len);
			void          Clear(void);
	
			EdnVectorBin<int8_t>    GetVector(void);
			char *                  c_str(void) { return (char*)&m_data[0]; };
	
			// Sting operation :
			int32_t       FindForward(const char element, int32_t startPos=0);
			int32_t       FindBack(const char element, int32_t startPos=0x7FFFFFFF);
			Edn::String   Extract(int32_t posStart=0, int32_t posEnd=0x7FFFFFFF);
	
		private :
			EdnVectorBin<int8_t> m_data;
	};

	void TestUntaire_String(void);

#endif

