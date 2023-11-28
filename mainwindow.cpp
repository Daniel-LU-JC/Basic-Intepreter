#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include <fstream>
#include <QMessageBox>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    connect(ui->btnLoadCode, &QPushButton::clicked, this, &MainWindow::LOAD);
    connect(ui->btnRunCode, &QPushButton::clicked, this, &MainWindow::RUN);
    connect(ui->btnClearCode, &QPushButton::clicked, this, &MainWindow::CLEAR);

    CLEAR();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_cmdLineEdit_editingFinished()
{
    QString cmd = ui->cmdLineEdit->text();
    ui->cmdLineEdit->clear();

    if (cmd.isEmpty()) return;

    std::string c = cmd.toStdString();

    if (program->isSuspended()) {  // waiting for the input of command INPUT
        input(c);
        return;
    }

    std::stringstream ss(c);
    int n = ss.peek();  // get the first char from stringstream
    std::string num;

    if (n >= '0' && n <= '9') {  // command line with number
        ss >> num;  // stop at the first space
        n = QBasic::Program::stoi(num);
        c.clear();
        getline(ss, c);  // convert the remaining ss into std::string
        program->appendCMD(n, c);
        showCode();  // render lines of codes again
        return;
    }

    std::string consoleCmd;  // execute the command line immediately
    ss >> consoleCmd;  // get the command type(trim)
    QString str = QString::fromStdString(consoleCmd);  // from std::string to QString
    qDebug() << "Command Type: " << str;  // QDebug!!!

    if (QBasic::validCommand.count(consoleCmd)) {
        program->execOne(c);
        showOutput();  // display the result immediately
        return;
    }

    if (consoleCmd == "LOAD") {
        LOAD();
        return;
    }

    if (consoleCmd == "RUN") {
        RUN();
        return;
    }

    if (consoleCmd == "CLEAR") {
        CLEAR();
        return;
    }

    if (consoleCmd == "HELP") {
        QMessageBox::information(this, "HELP", "Please enter line number and command line!");
        return;
    }

    if (consoleCmd == "QUIT") {
        exit(0);
    }

    // deal with the erroneous input from user
    QMessageBox::warning(this, "ERROR", "Please check your input based on HELP!");
}

void MainWindow::LOAD()
{
    QString filePath = QFileDialog::getOpenFileName(this, "Basic Programs");
    if (filePath.isEmpty()) return;

    CLEAR();  // erase the existing input

    // reset the unique pointer to a new program object
    program.reset(new QBasic::Program(filePath.toStdString()));

    showCode();  // display the code after loading
}

// program is being suspended, waiting for the input from user
void MainWindow::input(std::string n)
{
    program->continueExec(n);
    showOutput();  // continue the execution flow and display the input value
}

void MainWindow::RUN()
{
    ui->textBrowser->clear();
    ui->treeDisplay->clear();  // clear the result and the syntax tree

    if (!program) {
        QMessageBox::warning(this, "ERROR", "Please load a basic program first!");
        return;
    }

    program->exec();

    showOutput();
    showTree();
}

void MainWindow::CLEAR()
{
    ui->CodeDisplay->clear();
    ui->textBrowser->clear();
    ui->treeDisplay->clear();
    // reset the unique pointer to a new blank program object
    program.reset(new QBasic::Program());
}

void MainWindow::showCode()
{
    ui->CodeDisplay->clear();

    for (auto & cmd : program->rawCommands)
        ui->CodeDisplay->append(QString::number(cmd.first) +
            " " + QString::fromStdString(cmd.second));
}

void MainWindow::showOutput()
{
    ui->textBrowser->clear();

    for (auto & o : program->getOutput())
        ui->textBrowser->append(QString::fromStdString(o));
}

void MainWindow::showTree() // display the syntax tree
{
    bool first;
    for (auto & t : program->tree) {
        first = true;
        for (auto & line : t.second) {
            if (first) {  // first row in the syntax tree
                ui->treeDisplay->append(QString::number(t.first) + " " + QString::fromStdString(line));
                first = false;
            } else {
                ui->treeDisplay->append(QString::fromStdString(line));
            }
        }
    }
}
