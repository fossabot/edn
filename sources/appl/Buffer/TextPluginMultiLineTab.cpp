if (_event.getType() == ewol::keyEvent::keyboardChar) {
		//APPL_DEBUG("KB EVENT : \"" << UTF8_data << "\" size=" << strlen(UTF8_data) << "type=" << (int32_t)typeEvent);
		if (_event.getStatus() != ewol::keyEvent::statusDown) {
			return false;
		}
		etk::UChar localValue = _event.getChar();
		if (localValue == etk::UChar::Tabulation) {
			if (hasTextSelected()) {
				// TODO : Special tabulation multiline indentation ...
				/*
				int32_t nbSelectedLines = m_EdnBuf.CountLines(SelectionStart, SelectionEnd);
				if (1 < nbSelectedLines) {
					if (true == _event.getSpecialKey().isSetShift() ) {
						m_cursorPos = m_EdnBuf.UnIndent();
					} else {
						m_cursorPos = m_EdnBuf.Indent();
					}
				}
				*/
				markToRedraw();
				return true;
			}