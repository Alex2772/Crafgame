#ifndef SERVER
#include "GuiTextField.h"
#include "GameEngine.h"
#include "Clipboard.h"

using namespace std;

GuiTextField::GuiTextField() {

}
void GuiTextField::render(int rf, int mouseX, int mouseY) {
	static CFont* f = CGE::instance->fontRegistry->getCFont(_R("craftgame:default"));
	Gui::render(rf, mouseX, mouseY);

	CGE::instance->maskFB->begin();
	CGE::instance->guiShader = CGE::instance->maskShader;
	CGE::instance->guiShader->start();
	CGE::instance->renderer->drawRect(0, 0, width, height);
	CGE::instance->guiShader = CGE::instance->standartGuiShader;
	CGE::instance->guiShader->start();
	CGE::instance->maskFB->end();
	CGE::instance->renderer->setTexturingMode(0);
	if (Keyboard::isKeyDown(Keyboard::Key::LButton) && isFocused()) {
		int mX = mouseX;
		size_t x = 0;
		mX += 5 + scroll;
		size_t i;
		for (i = 0; i < text.size(); i++) {
			char* c = new char[2];
			c[0] = text[i];
			c[1] = '\0';
			x += f->length(c, textsize);
			delete[] c;
			if (mX < x) {
				break;
			}
		}
		if (selection == cursor)
			selection = -1;
		else
			selection = i;
	}
	glm::vec4 color = (isFocused() && isEnabled() ? glm::vec4(1, 1, 1, 0.9) : glm::vec4(1, 1, 1, 0.6));
	if (displayBackground) {
		CGE::instance->renderer->setColor(color);
		CGE::instance->renderer->drawRect(0, height - 2, width, 2);
	}
	CGE::instance->renderer->setColor(color * glm::vec4(1, 1, 1, 0.4));
	CGE::instance->renderer->setTexturingMode(0);
	if (selection >= 0 && isEnabled() && isFocused()) {
		if (selection > cursor) {
			CGE::instance->renderer->drawRect(f->length(text.substr(0, cursor), textsize) - scroll, 0, f->length(text.substr(cursor, selection - cursor), textsize), height);
		}
		else {
			CGE::instance->renderer->drawRect(f->length(text.substr(0, selection), textsize) - scroll, 0, f->length(text.substr(selection, cursor - selection), textsize), height);
		}
	}
	CGE::instance->renderer->setTexturingMode(1);
	CGE::instance->renderer->drawString(1 - scroll, 6, text, Align::LEFT, TextStyle::SIMPLE, color, textsize);
	if (isFocused() && isEnabled()) {
		std::string s = text.substr(0, cursor);
		float alpha = (cos(blink * blink / 400.f) + 1.f) / 2.f;
		CGE::instance->renderer->setColor(glm::vec4(1, 1, 1, alpha) * getColor());
		CGE::instance->renderer->setTexturingMode(0);
		CGE::instance->renderer->drawRect(f->length(s, textsize) - scroll, 5, 2, 20);
		CGE::instance->renderer->setTexturingMode(1);
	}
	if (blink >= 49)
		blink = 0;
	blink += CGE::instance->millis * 50;
	CGE::instance->renderer->setColor(glm::vec4(1, 1, 1, 1));
}

bool isSymbol(char t) {
	return string("!@#$%^&*()_-+=;':\"\\|/?/.,<> ").find(t) != string::npos;
}
using namespace Keyboard;
void GuiTextField::onKeyDown(Key key, byte data) {
	static CFont* font = CGE::instance->fontRegistry->getCFont(_R("craftgame:default"));
	if (!enabled)
		return;
	if (isKeyDown(Key::LControl)) {
		switch (key) {
		case Key::A:
			if (text.size()) {
				selection = 0;
				cursor = text.size();
			}
			break;
		case Key::C:
		{
			std::string s = getSelectedText();
			if (s.size()) {
				Clipboard::copyToClipboard(s.c_str());
			}
		}
			break;
		case Key::X:
		{
			std::string s = getSelectedText();
			if (s.size()) {
				Clipboard::copyToClipboard(s.c_str());
			}
			if (selection >= 0) {
				if (selection > cursor) {
					std::string t = text.substr(0, cursor);
					t += text.substr(selection, text.size() - selection);
					text = t;
				}
				else {
					std::string t = text.substr(0, selection);
					t += text.substr(cursor, text.size() - cursor);
					text = t;
					cursor = selection;
				}
			}
			selection = -1;
			
		}
		break;
		case Key::V:
		{
			char* s = Clipboard::copyFromClipboard();
			if (s) {
				if (selection >= 0) {
					if (selection > cursor) {
						std::string t = text.substr(0, cursor);
						t += text.substr(selection, text.size() - selection);
						text = t;
					}
					else {
						std::string t = text.substr(0, selection);
						if (!t.empty()) {
							cursor %= t.length();
							t += text.substr(cursor, text.size() - cursor);
							text = t;
							cursor = selection;
						}
					}
				}
				std::string t = text.substr(0, cursor);
				t += s;
				t += text.substr(cursor, text.size() - cursor);
				text = t;
				cursor += strlen(s);
				selection = -1;
			}
			delete[] s;
		}
		break;
		}
	}
	else {
		switch (key) {
		case Key::Left:
			if (isKeyDown(Key::LShift) && selection < 0) {
				selection = cursor;
			}
			else if (!isKeyDown(Key::LShift)) {
				selection = -1;
			}
			if ((isKeyDown(Key::LControl) || isKeyDown(Key::RControl)) && cursor) {
				size_t i;
				for (i = cursor - 1; i > 0; i--) {
					char t = text[i - 1];
					if (isSymbol(t)) {
						break;
					}
				}
				cursor = i;
			}
			else {
				if (cursor)
					cursor--;
			}
			break;
		case Key::Right:
			if (isKeyDown(Key::LShift) && selection < 0) {
				selection = cursor;
			}
			else if (!isKeyDown(Key::LShift)) {
				selection = -1;
			}
			if ((isKeyDown(Key::LControl) || isKeyDown(Key::RControl)) && cursor < text.size()) {
				size_t i;
				for (i = cursor + 1; i < text.size(); i++) {
					char t = text[i];
					if (isSymbol(t)) {
						break;
					}
				}
				cursor = i;
			}
			else {
				if (cursor < text.size())
					cursor++;
			}
			break;
		case Key::Delete:
			if (selection >= 0) {
				onText(fromKey(Key::BackSpace));
			}
			else if (cursor < text.size()) {
				std::string s = text.substr(0, cursor);
				s += text.substr(cursor + 1, text.size() - cursor - 1);
				text = s;
			}
			break;
#ifndef WINDOWS
		case Key::BackSpace:
			if (cursor) {
				if (selection < 0) {
					cursor--;
					std::string t = text.substr(0, cursor);
					t += text.substr(cursor + 1, text.size() - cursor);
					text = t;
					break;
				}
			}
#endif
		}
	}
	size_t l = font->length(text.substr(0, cursor), textsize);
	if (l < scroll) {
		scroll = l;
	}
	else if (l > width && l - width > scroll + 2) {
		scroll = l - width + 2;
	}
	blink = 0;
}
void GuiTextField::onText(char c) {
	static CFont* font = CGE::instance->fontRegistry->getCFont(_R("craftgame:default"));
	if (!enabled)
		return;
	if (!isKeyDown(Key::LControl)) {
		switch (toKey(c)) {
		case Key::Escape:
			break;
		case Key::Return:
			break;
		case Key::BackSpace:
#ifdef WINDOWS
			if (cursor) {
				if (selection < 0) {
					cursor--;
					std::string t = text.substr(0, cursor);
					t += text.substr(cursor + 1, text.size() - cursor);
					text = t;
					break;
				}
			}
#endif
		default:
			if (selection >= 0) {
				if (selection > cursor) {
					std::string t = text.substr(0, cursor);
					t += text.substr(selection, text.size() - selection);
					text = t;
				}
				else {
					std::string t = text.substr(0, selection);
					t += text.substr(cursor, text.size() - cursor);
					text = t;
					cursor = selection;
				}
			}
			if (toKey(c) != Key::BackSpace) {
				std::string t = text.substr(0, cursor);
				t += c;
				t += text.substr(cursor, text.size() - cursor);
				if (t.size() < 256)
					text = t;
				cursor++;
			}
		}
		selection = -1;
	}
	size_t l = font->length(text.substr(0, cursor), textsize);
	if (l < scroll) {
		scroll = l;
	}
	else if (l > width && l - width > scroll + 2) {
		scroll = l - width + 2;
	}
	blink = 0;
}
void GuiTextField::onMouseClick(int mX, int mY, int button) {
	static CFont* font = CGE::instance->fontRegistry->getCFont(_R("craftgame:default"));
	if (!enabled)
		return;
	Gui::onMouseClick(mX, mY, button);
	size_t x = 0;
	mX += 5 + scroll;
	size_t i;
	for (i = 0; i < text.size(); i++) {
		char* c = new char[2];
		c[0] = text[i];
		c[1] = '\0';
		x += font->length(c, textsize);
		delete[] c;
		if (mX < x) {
			break;
		}
	}
	if (selection >= 0)
		selection = -1;
	cursor = i;
}

std::string GuiTextField::getText() {
	return text;
}
std::string GuiTextField::getSelectedText() {
	if (selection >= 0) {
		if (selection > cursor) {
			return text.substr(cursor, selection - cursor);
		}
		else {
			return text.substr(selection, cursor - selection);
		}
	}
	return "";
}
void GuiTextField::setText(std::string s) {
	selection = 0;
	text = s;
	cursor = s.size();
}
void GuiTextField::setEnabled(bool e) {
	enabled = e;
}
bool GuiTextField::isEnabled() {
	return enabled;
}
#endif