#ifndef FORM_CPP_1_H
#define FORM_CPP_1_H

#include <QWidget>
#include <QTimer>
#include "blackjack.h"

namespace Ui {
class Form_cpp_1;
}

class Form_cpp_1 : public QWidget
{
    Q_OBJECT

public:
    explicit Form_cpp_1(QWidget *parent = nullptr);
    ~Form_cpp_1();
    static Form_cpp_1& getForm_cpp_1_ptr();
    void output(const QString& str);
    void outputTime();
    static void outputS(const QString& str);
    static void outputTimeS();

private slots:
    void on_pushButton_clicked();

    void on_pushButton_3_clicked();

    void on_pushButton_4_clicked();

    void on_pushButton_2_clicked();
    void checkObjectHeight();

    void on_pushButton_5_clicked();

    void on_pushButton_6_clicked();

    void on_pushButton_7_clicked();

    void on_pushButton_8_clicked();

    void on_pushButton_9_clicked();

    void on_pushButton_10_clicked();

    void on_pushButton_11_clicked();

    void on_pushButton_8Start_clicked();

    void on_pushButton_8Get_clicked();

    void on_pushButton_8Stop_clicked();

    void on_pushButton_chapter9_clicked();

private:
    Ui::Form_cpp_1 *ui;
    static Form_cpp_1* form_cpp_1_ptr;
    double m_currentObjectHeight;
    double m_currentObjectSpeed;
    QTimer m_timerObjectHeight;
    Deck m_deck{};
    Player m_player{"<Player>"};
    Player m_dealer{"<Dealer>"};
};

#endif // FORM_CPP_1_H
