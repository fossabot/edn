/**
 * @author Edouard DUPIN
 * 
 * @copyright 2010, Edouard DUPIN, all right reserved
 * 
 * @license GPL v3 (see license file)
 */

#ifndef __SEARCH_H__
#define __SEARCH_H__

#include <appl/debug.h>
#include <ewol/widget/Sizer.h>
#include <ewol/widget/Entry.h>

class Search : public widget::Sizer
{
	private:
		bool m_forward;
		widget::Entry * m_searchEntry;
		widget::Entry * m_replaceEntry;
	public:
		// Constructeur
		Search(void);
		~Search(void);
	public: // derived function
		virtual const char * const getObjectType(void) { return "ApplSearch"; };
		virtual void onReceiveMessage(const ewol::EMessage& _msg);
		virtual void onObjectRemove(ewol::EObject * _removeObject);
};

#endif


