/**
 * @author Edouard DUPIN
 * 
 * @copyright 2010, Edouard DUPIN, all right reserved
 * 
 * @license GPL v3 (see license file)
 */


#include <appl/Buffer/Buffer.h>
#include <appl/Debug.h>


bool appl::Buffer::LoadFile(const etk::UString& _name)
{
	APPL_DEBUG("Load file : '" << _name << "'");
	m_fileName = _name;
	etk::FSNode file(m_fileName);
	if (file.Exist() == false) {
		return false;
	}
	if (true == m_data.DumpFrom(file) ) {
		return true;
	}
	return false;
}


void appl::Buffer::SetFileName(const etk::UString& _name)
{
	// TODO : ...
}

