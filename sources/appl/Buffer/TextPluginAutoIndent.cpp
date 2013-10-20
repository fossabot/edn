
	// just forward event  == > manage directly in the buffer
	if (_event.getType() == ewol::keyEvent::keyboardChar) {
		//APPL_DEBUG("KB EVENT : \"" << UTF8_data << "\" size=" << strlen(UTF8_data) << "type=" << (int32_t)typeEvent);
		if (_event.getStatus() != ewol::keyEvent::statusDown) {
			return false;
		}
		etk::UChar localValue = _event.getChar();
		if (localValue == etk::UChar::Return) {
			if (true == _event.getSpecialKey().isSetShift()) {
				localValue = etk::UChar::CarrierReturn;
			} else {
				/*
				m_data.insert(m_cursorPos, '\n');
				if (true == globals::isSetAutoIndent() ) {
					int32_t l_lineStart;
					// get the begin of the line or the begin of the line befor selection
					if (false == haveSelectionActive) {
						l_lineStart = m_EdnBuf.StartOfLine(m_cursorPos);
					} else {
						l_lineStart = m_EdnBuf.StartOfLine(SelectionStart);
					}
					// add same characters in the temporar buffer
					for (int32_t kk=l_lineStart; kk<m_cursorPos; kk++) {
						if (' ' == m_EdnBuf[kk]) {
							tmpVect.pushBack(' ');
						} else if('\t' == m_EdnBuf[kk]) {
							tmpVect.pushBack('\t');
						} else {
							break;
						}
					}
				}
				m_selectMode = false;
				moveCursor(m_cursorPos + 1);
				return true;
				*/
			}