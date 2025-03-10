#pragma once
#include "Windows.h"
#include <QtWidgets/QMainWindow>

class TitleBar;
class YearBar;
class WeekBar;
class DayBtn;
class SwitchBar;
class ListBar;
class ListContent;
class TipInfo;
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(bool isEmbeded,QWidget *parent = nullptr);
    ~MainWindow();
    void switchEmbed();

    static LRESULT CALLBACK mouseProc(int nCode, WPARAM wParam, LPARAM lParam);
    static RAWINPUT* getRawInput(HRAWINPUT lParam);
public:    
    TitleBar* titleBar;
    YearBar* yearBar;
    WeekBar* weekBar;
    QList<DayBtn*> dayBtns;
    ListBar* listBar;
    ListContent* listContent;
    SwitchBar* switchBar;
    TipInfo* tipInfo;
protected:
    void paintEvent(QPaintEvent* event) override;
    void closeEvent(QCloseEvent* event);
private:
    void embed();
    static LRESULT CALLBACK processMsg(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
private:
    bool isEmbeded;
};
