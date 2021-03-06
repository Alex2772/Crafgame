#ifndef SERVER
#include "GuiScreen.h"

#include "gl.h"
#include "GameEngine.h"

GuiScreen::GuiScreen() {

}
GuiScreen::~GuiScreen() {
}
void GuiScreen::render(int flags, int mouseX, int mouseY) {
}
void GuiScreen::renderWidgets(int _rf, int mouseX, int mouseY) {
	GuiContainer::renderWidgets(_rf, mouseX, mouseY);
}

void GuiScreen::onMouseClick(int mX, int mY, int button) {
	GuiContainer::onMouseClick(mX, mY, button);
}

void GuiScreen::onKeyDown(Keyboard::Key key, byte data) {
	GuiContainer::onKeyDown(key, data);
}
void GuiScreen::close() {
	CGE::instance->removeGuiScreen(this);
}
void GuiScreen::animatedClose(Animation* a) {
	if (a->frame == 0)
	{
		a->frame = a->maxFrames;
	}
	a->state = State::inverse;
	a->onAnimationEnd = std::function<void(State)>([&](State s) {
		if (s == State::inverse) {
			CGE::instance->uiThread.push(std::function<void()>([&, s]() {
				CGE::instance->removeGuiScreen(this);
			}));
		}
	});
}
#endif