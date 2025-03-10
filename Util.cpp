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
