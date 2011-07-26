/**
 *******************************************************************************
 * @file RegExp.h
 * @brief Editeur De N'ours : Regular expression annalyser (header)
 * @author Edouard DUPIN
 * @date 04/04/2011
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
#ifndef __EDN_REG_EXP_H__
#define __EDN_REG_EXP_H__

class EdnRegExp;

#include "EdnTree.h"
#include "EdnBuf.h"
#include "EdnVectorBin.h"
#include "EdnVectorBuf.h"


/*
normal mode :
	(...)				sub element is separate with |
	\d					Digits									[0-9]
	\D					NOT a digit								[^0-9]
	\l					Letters									[a-zA-Z]
	\L					NOT a Letter							[^a-zA-Z]
	\s					Whitespace								[ \t\n\r\f\v]
	\S					NOT Whitespace							[^ \t\n\r\f\v]
	\w					"Word" character						[a-zA-Z0-9_]
	\W					NOT a "Word" character					[^a-zA-Z0-9_]
	\@					at the start or the end					not in the parsing of element ==> check if \w is not present   (other regExp will be <> ...)
	[anjdi] or [a-gt-j]	range
	.					dot										[^\x00-\x08\x0A-\x1F\x7F]
==> TODO :
	$					End / Start of line of line 			==> ce sera un truc suplémentaire comme le \@
	^in the []			invertion of the range element

multiplicity :
	*     ==> {0, 2147483647}
	?     ==> {0, 1}
	+     ==> {1, 2147483647}
	{x}   ==> {x, x}
	{x,y} ==> {x, y}
*/

class RegExpNode;

/**
 *	@brief Node Elements for every-one
 */
class RegExpNode{
	public :
		                        RegExpNode(void);
		virtual                 ~RegExpNode(void) { };
		virtual int32_t         Generate(EdnVectorBin<int16_t> &data, int32_t startPos, int32_t nbElement);
		virtual bool            Parse(EdnVectorBuf &data, int32_t currentPos, int32_t lenMax, int32_t &findLen);
		virtual void            Display(int32_t level);
		void                    SetMult(int32_t min, int32_t max);
	protected:
		int32_t                 GetMultMin(void) { return m_multipleMin; };
		int32_t                 GetMultMax(void) { return m_multipleMax; };
	protected :
		int32_t                 m_multipleMin;      //!< minimum repetition (included)
		int32_t                 m_multipleMax;      //!< maximum repetition (included)
		// Data Section ... (can have no data...)
		EdnVectorBin<int16_t>   m_RegExpData;       //!< data to parse and compare in some case ...
};


class RegExpNodePThese : public RegExpNode {
	public :
		        RegExpNodePThese(void) { };
		        ~RegExpNodePThese(void);
		int32_t Generate(EdnVectorBin<int16_t> &data);
		bool    Parse(EdnVectorBuf &data, int32_t currentPos, int32_t lenMax, int32_t &findLen);
		void    Display(int32_t level);
	protected :
		// SubNodes :
		EdnVectorBin<RegExpNode*> m_subNode;
		//int32_t						m_posPthese;		//!< position of the element is detected in the output element
};

class RegExpNodePTheseElem : public RegExpNode {
	public :
		        RegExpNodePTheseElem(void) { };
		        ~RegExpNodePTheseElem(void);
		int32_t Generate(EdnVectorBin<int16_t> &data);
		bool    Parse(EdnVectorBuf &data, int32_t currentPos, int32_t lenMax, int32_t &findLen);
		void    Display(int32_t level);
	protected :
		// SubNodes :
		EdnVectorBin<RegExpNode*> m_subNode;
	private :
		bool    SetMultiplicityOnLastNode(int32_t min, int32_t max);
};


class RegExpNodeValue : public RegExpNode {
	public :
		        RegExpNodeValue(void) { };
		        ~RegExpNodeValue(void) { };
		int32_t Generate(EdnVectorBin<int16_t> &data);
		bool    Parse(EdnVectorBuf &data, int32_t currentPos, int32_t lenMax, int32_t &findLen);
		void    Display(int32_t level);
	protected :
		// SubNodes :
		EdnVectorBin<char> m_data;
};

class RegExpNodeBracket : public RegExpNode {
	public :
		        RegExpNodeBracket(void) { };
		        ~RegExpNodeBracket(void) { };
		int32_t Generate(EdnVectorBin<int16_t> &data);
		bool    Parse(EdnVectorBuf &data, int32_t currentPos, int32_t lenMax, int32_t &findLen);
		void    Display(int32_t level);
	protected :
		// SubNodes :
		EdnVectorBin<char> m_data;
};

class RegExpNodeDigit : public RegExpNode {
	public :
		        RegExpNodeDigit(void) { };
		        ~RegExpNodeDigit(void) { };
		bool    Parse(EdnVectorBuf &data, int32_t currentPos, int32_t lenMax, int32_t &findLen);
		void    Display(int32_t level);
};
class RegExpNodeDigitNot : public RegExpNode {
	public :
		        RegExpNodeDigitNot(void) { };
		        ~RegExpNodeDigitNot(void) { };
		bool    Parse(EdnVectorBuf &data, int32_t currentPos, int32_t lenMax, int32_t &findLen);
		void    Display(int32_t level);
};
class RegExpNodeLetter : public RegExpNode {
	public :
		        RegExpNodeLetter(void) { };
		        ~RegExpNodeLetter(void) { };
		bool    Parse(EdnVectorBuf &data, int32_t currentPos, int32_t lenMax, int32_t &findLen);
		void    Display(int32_t level);
};
class RegExpNodeLetterNot : public RegExpNode {
	public :
		        RegExpNodeLetterNot(void) { };
		        ~RegExpNodeLetterNot(void) { };
		bool    Parse(EdnVectorBuf &data, int32_t currentPos, int32_t lenMax, int32_t &findLen);
		void    Display(int32_t level);
};
class RegExpNodeWhiteSpace : public RegExpNode {
	public :
		        RegExpNodeWhiteSpace(void) { };
		        ~RegExpNodeWhiteSpace(void) { };
		bool    Parse(EdnVectorBuf &data, int32_t currentPos, int32_t lenMax, int32_t &findLen);
		void    Display(int32_t level);
};
class RegExpNodeWhiteSpaceNot : public RegExpNode {
	public :
		        RegExpNodeWhiteSpaceNot(void) { };
		        ~RegExpNodeWhiteSpaceNot(void) { };
		bool    Parse(EdnVectorBuf &data, int32_t currentPos, int32_t lenMax, int32_t &findLen);
		void    Display(int32_t level);
};
class RegExpNodeWordChar : public RegExpNode {
	public :
		        RegExpNodeWordChar(void) { };
		        ~RegExpNodeWordChar(void) { };
		bool    Parse(EdnVectorBuf &data, int32_t currentPos, int32_t lenMax, int32_t &findLen);
		void    Display(int32_t level);
};
class RegExpNodeWordCharNot : public RegExpNode {
	public :
		        RegExpNodeWordCharNot(void) { };
		        ~RegExpNodeWordCharNot(void) { };
		bool    Parse(EdnVectorBuf &data, int32_t currentPos, int32_t lenMax, int32_t &findLen);
		void    Display(int32_t level);
};
class RegExpNodeDot : public RegExpNode {
	public :
		        RegExpNodeDot(void) { };
		        ~RegExpNodeDot(void) { };
		bool    Parse(EdnVectorBuf &data, int32_t currentPos, int32_t lenMax, int32_t &findLen);
		void    Display(int32_t level);
};

class RegExpNodeSOL : public RegExpNode {
	public :
		        RegExpNodeSOL(void) { };
		        ~RegExpNodeSOL(void) { };
		bool    Parse(EdnVectorBuf &data, int32_t currentPos, int32_t lenMax, int32_t &findLen);
		void    Display(int32_t level);
};

class RegExpNodeEOL : public RegExpNode {
	public :
		        RegExpNodeEOL(void) { };
		        ~RegExpNodeEOL(void) { };
		bool    Parse(EdnVectorBuf &data, int32_t currentPos, int32_t lenMax, int32_t &findLen);
		void    Display(int32_t level);
};

typedef struct {
	int32_t start;
	int32_t stop;
}elementPos_ts;


// Regular expression manager
class EdnRegExp {
	// public API : 
	public:
		// create the regular expression
		EdnRegExp(const char *exp);
		EdnRegExp(Edn::String &exp);
		EdnRegExp(void);
		~EdnRegExp(void);
		void		SetRegExp(const char *exp);
		void		SetRegExp(Edn::String &exp);
		Edn::String	GetRegExp(void) { return m_expressionRequested;};
		bool		GetStatus(void)  { return m_isOk;};
		// process the regular expression
		bool	Process(	EdnVectorBuf			&SearchIn,
							int32_t					startPos,
							int32_t					endPos,
							char					escapeChar=0);
		int32_t	Start(void)	{ return m_areaFind.start; };
		int32_t	Stop(void)	{ return m_areaFind.stop; };
		void	Display(void);
	// internal parameters
	private:
		Edn::String					m_expressionRequested;	// TODO : Remove ...
		elementPos_ts				m_areaFind;					//!< position around selection
		RegExpNodePThese			m_exprRootNode;				//!< The tree where data is set
		bool						m_isOk;						//!< Known if we can process with this regExp
		bool						m_notBeginWithChar;			//!< The regular expression must not have previously a char [a-zA-Z0-9_]
		bool						m_notEndWithChar;			//!< The regular expression must not have after the end a char [a-zA-Z0-9_]
	// internal access
	private:
		bool CheckGoodPosition(EdnVectorBin<int16_t> tmpExp, int32_t &pos);
		bool CheckGoodPosition(EdnVectorBin<int16_t> tmpExp);


};

#endif
