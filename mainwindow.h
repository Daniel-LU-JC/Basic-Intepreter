#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <program.h>

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
    // dealing with the input command from user
    void on_cmdLineEdit_editingFinished();
    // usage of private slots is as follows:
    // on + name of the QWidget(cmdLineEdit) + signal(editingFinished)
    // pros: avoid the usage of connect()

private:
    void LOAD();  // button: load basic program from an existing file
    void RUN();  // button: run the current program
    void CLEAR();  // button: clear the current program
    void input(std::string n);  // enter a new command line from console
    void showCode();  // display the current program
    void showOutput();  // display the result when running the program
    void showTree();  // display the syntax tree

private:
    Ui::MainWindow *ui;
    std::unique_ptr<QBasic::Program> program;  // unique pointer to the basic program
};

#endif // MAINWINDOW_H
