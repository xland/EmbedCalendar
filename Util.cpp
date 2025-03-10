
#include "Util.h"

QFont* Util::getIconFont(const int& fontSize)
{
    static QFont font = []() {
        int fontId = QFontDatabase::addApplicationFont(":/Res/iconfont.ttf");
        QString fontName = QFontDatabase::applicationFontFamilies(fontId).at(0);
        QFont iconFont(fontName);
        iconFont.setStyleStrategy(QFont::PreferAntialias);
        iconFont.setHintingPreference(QFont::PreferNoHinting);
        return iconFont;
    }();
    font.setPixelSize(fontSize);
    return &font;
}

QFont* Util::getTextFont(const int& fontSize)
{
    static QFont font = []() {
        QFont font("Microsoft YaHei");
        font.setStyleStrategy(QFont::PreferAntialias);
        font.setHintingPreference(QFont::PreferNoHinting);
        return font;
        }();
    font.setPixelSize(fontSize);
    return &font;
}

HWND Util::getWorkerW()
{
    static HWND workerW;
    if (!workerW) {
        HWND progman = FindWindow(L"Progman", NULL);
        SendMessage(progman, 0x052C, 0xD, 0);
        SendMessage(progman, 0x052C, 0xD, 1);
        EnumWindows([](HWND hwnd, LPARAM lParam) -> BOOL {
            HWND defView = FindWindowEx(hwnd, NULL, L"SHELLDLL_DefView", NULL);
            if (defView != NULL) {
                auto tar = (HWND*)lParam;
                *tar = FindWindowEx(NULL, hwnd, L"WorkerW", NULL);
            }
            return TRUE;
            }, (LPARAM)&workerW);
    }
    return workerW;
}
