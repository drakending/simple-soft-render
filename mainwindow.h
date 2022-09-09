#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "string"
QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    std::string OBJFile,TexFile;
    void setFile();
    void loadFile();

private slots:
    void on_horizontalSlider_valueChanged(int value);

    void on_comboBox_activated(int index);

    void on_kaS_valueChanged(int value);

    void on_ksS_valueChanged(int value);

    void on_loadBtn_clicked();

    void on_sizeDSB_valueChanged(double arg1);

    void on_upBtn_clicked();

    void on_downBtn_clicked();

    void on_leftBtn_clicked();

    void on_rightBtn_clicked();

    void on_xAngelhS_valueChanged(int value);

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
