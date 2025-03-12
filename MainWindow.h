#pragma once
#include "Windows.h"
#include <QtWidgets/QMainWindow>
#include <QJsonObject>

class TitleBar;
class YearBar;
class WeekBar;
class DayBtn;
class SwitchBtn;
class ListBar;
class ListContent;
class TipInfo;
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:    
    ~MainWindow();
    void switchEmbed();
    static RAWINPUT* getRawInput(HRAWINPUT lParam);
    static void init();
    static MainWindow* get();
    void updateData();
public:    
    TitleBar* titleBar;
    YearBar* yearBar;
    WeekBar* weekBar;
    QList<DayBtn*> dayBtns;
    ListBar* listBar;
    ListContent* listContent;
    SwitchBtn* switchBtn;
    TipInfo* tipInfo;
    bool isEnter{ false };
protected:
    void paintEvent(QPaintEvent* event) override;
    void closeEvent(QCloseEvent* event);
private:
    MainWindow(bool isEmbeded, QWidget* parent = nullptr);
    void onEmbedMouseMove();
    void onEmbedMousePress();
    void onEmbedMouseWheel(const int& wheelData);
    void onEmbedLeaveWindow();
    void embed();
    static LRESULT CALLBACK processMsg(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
private:
    bool isEmbeded;
};
