#include "Window.h"

#ifdef Q_OS_WIN
#include <windows.h>
#endif

namespace Clash::Meta::GUI {
bool Window::stayTop() const
{
    return stayTop_;
}

void Window::setStayTop(bool state)
{
    if (stayTop_ == state)
        return;

#ifdef Q_OS_WIN
    HWND hwnd = reinterpret_cast<HWND>(this->winId());
    if (state) {
        ::SetWindowPos(hwnd, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
    } else {
        ::SetWindowPos(hwnd, HWND_NOTOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
    }
#else
    this->setFlag(Qt::WindowStaysOnTopHint, state);
#endif

    stayTop_ = state;
    Q_EMIT stayTopChanged(stayTop_);
}

} // namespace Clash::Meta::GUI
