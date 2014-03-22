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

#ifndef TimerWind_H
#define TimerWind_H

#include <QDialog>
#include <QTimer>
#include <QElapsedTimer>

#ifdef Q_OS_WIN
#   include <QtWinExtras>
#endif

namespace Ui {
class TimerWind;
}

class TimerWind : public QDialog
{
    Q_OBJECT

public:
    explicit TimerWind(QWidget *parent = 0);
    ~TimerWind();

protected:
    bool event(QEvent *e);

private slots:
    void on_startBtn_clicked();
    void time();
    void on_pauseBtn_clicked();
    void on_resetBtn_clicked();

private:
    /* Timer state */
    QTimer triggerTimer;
    QElapsedTimer accurateTimer;

    bool countingUp;
    bool showWarn;

    bool didDoOvrTenMbox;
    bool didDoOvrFiftMbox;
    bool didDoOvrFiveMbox;
    bool didDoFiveMbox;
    bool didDo30sMbox;

    int origMins;
    int minLeft;
    int secLeft;

    /* Regular stuff */
    Ui::TimerWind *ui;
    void modalMsgBox(QString tagline, QString text, bool critical = false);
    void updateTimeLabel();

#ifdef Q_OS_WIN
    QWinTaskbarButton* taskbarButton;
    QWinTaskbarProgress* taskbarProgress;
#endif
};

#endif // TimerWind_H
