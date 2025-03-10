#pragma once

#include <QtWidgets/QMainWindow>

class TitleBar;
class YearBar;
class WeekBar;
class DayBtn;
class SwitchBar;
class ListBar;
class ListContent;
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
public:    
    TitleBar* titleBar;
    YearBar* yearBar;
    WeekBar* weekBar;
    QList<DayBtn*> dayBtns;
    ListBar* listBar;
    ListContent* listContent;
    SwitchBar* switchBar;
protected:
    void paintEvent(QPaintEvent* event) override;
private:
    void embed();
    HWND getWorkerW();
private:

};
