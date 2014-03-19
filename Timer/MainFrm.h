/*
 * Copyright (c) 2009-2012 WaddleSplash
 *
 * Permission is hereby granted, free of charge, to any person obtaining
 * a copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the Software
 * is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall
 * be included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
 * OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 * IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM,
 * DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
 * ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 * DEALINGS IN THE SOFTWARE.
 *
*/

#ifndef MAINFRM_H
#define MAINFRM_H

#include <QDialog>
#include <QString>

#ifdef Q_OS_WIN
#   include "WinUtils.h"
#endif

namespace Ui {
    class MainFrm;
}

class MainFrm : public QDialog
{
    Q_OBJECT

public:
    explicit MainFrm(QWidget *parent = 0);
    ~MainFrm();

private slots:
    void on_startBtn_clicked();
    void on_aboutQtBtn_clicked();
    void on_aboutSTBtn_clicked();
    void time();
    void on_pauseBtn_clicked();
    void on_resetBtn_clicked();

#ifdef Q_OS_WIN
protected:
    bool winEvent(MSG * message, long * result);
#endif

private:
    Ui::MainFrm *ui;
    int stMsgBox(QString txt, QString infoTxt, QString windTitle, bool isCritical = false);

#ifdef Q_OS_WIN
    void updateProgBarState(int state);
    void updateProgBarVal(int value);
    void initTaskbarButton();

    unsigned int m_IDTaskbarButtonCreated;
    ITaskbarList3* m_taskbarInterface;
#endif
};

#endif // MAINFRM_H
