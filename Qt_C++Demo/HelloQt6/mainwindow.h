#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_selectButton_clicked();

    void on_regnizeButton_clicked();

    void on_copyButton_clicked();

private:
    Ui::MainWindow *ui;
    QString fileUrl;

    void refreshImage(QString filePath);
    void recognizeImage(QString filePath);
    void showMessage(QString msg);
};
#endif // MAINWINDOW_H
