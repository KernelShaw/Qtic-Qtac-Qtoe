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
    void cpu_move();
    void cpu_place_piece(int);
    int random_cpu_move();

    void swap_2p();

    void load_player_wins();
    void save_player_wins();
    bool win_check();
    QString board_state();
    void winners_ceremony(QString);

    void on_slot1_clicked();
    void on_slot2_clicked();
    void on_slot3_clicked();
    void on_slot4_clicked();
    void on_slot5_clicked();
    void on_slot6_clicked();
    void on_slot7_clicked();
    void on_slot8_clicked();
    void on_slot9_clicked();

    void swap_pieces();
    void on_reset_clicked();

    void on_easy_button_clicked();
    void on_medium_button_clicked();
    void on_hard_button_clicked();
    void on_two_player_mode_clicked();

    void button_changer(QString);
    void gif_change(int&);

    void on_volume_valueChanged(int);
    void jukebox(int&);

    void on_theme1_clicked();
    void on_theme2_clicked();
    void on_theme5_clicked();
    void on_theme4_clicked();
    void on_theme6_clicked();
    void on_theme7_clicked();
    void on_theme3_clicked();

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
