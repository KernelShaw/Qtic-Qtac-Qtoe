#include "mainwindow.h"
#include "ticTacToeAI.h"
#include "ui_mainwindow.h"
#include <fstream>
#include <QtMultimedia>
#include <QGraphicsView>
#include <QStringBuilder>

#include <iostream>
#include <math.h>
#include <vector>

int difficulty_level = 0;
int theme = 0;
QAudioOutput *volume = new QAudioOutput;

QChar default_text = '_';
QChar player_piece = 'X';
QChar cpu_piece = 'O';
QChar twop_mode_current_player = 'X';
int wins[3];

QString winner = "";

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->volume->setValue(50);
    volume->setVolume(powf((static_cast<float>(ui->volume->value()) / 100), 4));
    load_player_wins();

    this->setWindowTitle("Tic-Tac-Toe: The Intellectual's Game");
}

MainWindow::~MainWindow()
{
    save_player_wins();
    delete ui;
}

void MainWindow::swap_pieces(){
    if (player_piece == 'X'){
        ui->status_message->setText("PLAYER: O");
        player_piece = 'O';
        cpu_piece = 'X';
    }
    else {
        ui->status_message->setText("PLAYER: X");
        player_piece = 'X';
        cpu_piece = 'O';
    }

    //ai->swap();
}

void MainWindow::save_player_wins(){
    std::ofstream write_to_save;

    write_to_save.open("easywins.txt");
    write_to_save << wins[0];
    write_to_save.close();

    write_to_save.open("medwins.txt");
    write_to_save << wins[1];
    write_to_save.close();

    write_to_save.open("hardwins.txt");
    write_to_save << wins[2];
    write_to_save.close();

}

void MainWindow::load_player_wins(){
    std::ifstream easy("easywins.txt");
    easy >> wins[0];
    ui->easy_wins->display(wins[0]);
    easy.close();

    std::ifstream med("medwins.txt");
    med >> wins[1];
    ui->medium_wins->display(wins[1]);
    med.close();

    std::ifstream hard("hardwins.txt");
    hard >> wins[2];
    ui->hard_wins->display(wins[2]);
    hard.close();
}

void MainWindow::swap_2p(){
    if (twop_mode_current_player == 'X')
        twop_mode_current_player = 'O';
    else
        twop_mode_current_player = 'X';
    ui->status_message->setText("TWO-PLAYER MODE. TURN: " % twop_mode_current_player);
}

void MainWindow::cpu_move(){
    if (difficulty_level == 0){ // Easy: Random CPU movement.
        cpu_place_piece(random_cpu_move());
    }
    else if (difficulty_level == 1){ // Medium: CPU has a 50/50 chance of thinking.
        if (rand() % 2 == 0){
            cpu_place_piece(random_cpu_move());
        }
        else {
            QChar board[3][3] =
            {
                { ui->slot1->text().at(0), ui->slot2->text().at(0), ui->slot3->text().at(0) },
                { ui->slot4->text().at(0), ui->slot5->text().at(0), ui->slot6->text().at(0) },
                { ui->slot7->text().at(0), ui->slot8->text().at(0), ui->slot9->text().at(0) }
            };

            cpu_place_piece(findBestMove(board, cpu_piece, player_piece));
        }
    }
    else if (difficulty_level == 2){ // Hard: CPU has a 1/9 chance of thinking.

        QChar board[3][3] =
        {
            { ui->slot1->text().at(0), ui->slot2->text().at(0), ui->slot3->text().at(0) },
            { ui->slot4->text().at(0), ui->slot5->text().at(0), ui->slot6->text().at(0) },
            { ui->slot7->text().at(0), ui->slot8->text().at(0), ui->slot9->text().at(0) }
        };

        cpu_place_piece(findBestMove(board, cpu_piece, player_piece));
    }

    if (board_state() == "WIN")
        winners_ceremony(board_state());
}

void MainWindow::cpu_place_piece(int num){
    switch(num) {
        case 0:
            ui->slot1->setText(cpu_piece);
            break;
        case 1:
            ui->slot2->setText(cpu_piece);
            break;
        case 2:
            ui->slot3->setText(cpu_piece);
            break;
        case 3:
            ui->slot4->setText(cpu_piece);
            break;
        case 4:
            ui->slot5->setText(cpu_piece);
            break;
        case 5:
            ui->slot6->setText(cpu_piece);
            break;
        case 6:
            ui->slot7->setText(cpu_piece);
            break;
        case 7:
            ui->slot8->setText(cpu_piece);
            break;
        case 8:
            ui->slot9->setText(cpu_piece);
            break;
    }

}

int MainWindow::random_cpu_move(){
    std::vector<int> open_spaces;

    if (ui->slot1->text() == '_')
        open_spaces.push_back(0);
    if (ui->slot2->text() == '_')
        open_spaces.push_back(1);
    if (ui->slot3->text() == '_')
        open_spaces.push_back(2);
    if (ui->slot4->text() == '_')
        open_spaces.push_back(3);
    if (ui->slot5->text() == '_')
        open_spaces.push_back(4);
    if (ui->slot6->text() == '_')
        open_spaces.push_back(5);
    if (ui->slot7->text() == '_')
        open_spaces.push_back(6);
    if (ui->slot8->text() == '_')
        open_spaces.push_back(7);
    if (ui->slot9->text() == '_')
        open_spaces.push_back(8);

    return open_spaces.at(rand() % open_spaces.size());
}

void MainWindow::on_slot1_clicked()
{
    if (ui->slot1->text() == default_text
            && board_state() != "DRAW"
            && board_state() != "WIN"){
        if (ui->reset->text() == "SWAP PIECES")
            ui->reset->setText("RESET");

        if (difficulty_level < 3){
            ui->slot1->setText(player_piece);
            if (board_state() == "WIN" || board_state() == "DRAW")
                winners_ceremony(board_state());
            else
                cpu_move();
        }
        else{
            ui->slot1->setText(twop_mode_current_player);
            if (board_state() == "WIN" || board_state() == "DRAW")
                winners_ceremony(board_state());
            else
                swap_2p();
        }
    }
}

void MainWindow::on_slot2_clicked()
{
    if (ui->slot2->text() == default_text
            && board_state() != "DRAW"
            && board_state() != "WIN"){
        if (ui->reset->text() == "SWAP PIECES")
            ui->reset->setText("RESET");

        if (difficulty_level < 3){
            ui->slot2->setText(player_piece);
            if (board_state() == "WIN" || board_state() == "DRAW")
                winners_ceremony(board_state());
            else
                cpu_move();
        }
        else{
            ui->slot2->setText(twop_mode_current_player);
            if (board_state() == "WIN" || board_state() == "DRAW")
                winners_ceremony(board_state());
            else
                swap_2p();
        }
    }
}

void MainWindow::on_slot3_clicked()
{
    if (ui->slot3->text() == default_text
            && board_state() != "DRAW"
            && board_state() != "WIN"){
        if (ui->reset->text() == "SWAP PIECES")
            ui->reset->setText("RESET");

        if (difficulty_level < 3){
            ui->slot3->setText(player_piece);
            if (board_state() == "WIN" || board_state() == "DRAW")
                winners_ceremony(board_state());
            else
                cpu_move();
        }
        else{
            ui->slot3->setText(twop_mode_current_player);
            if (board_state() == "WIN" || board_state() == "DRAW")
                winners_ceremony(board_state());
            else
                swap_2p();
        }
    }
}

void MainWindow::on_slot4_clicked()
{
    if (ui->slot4->text() == default_text
            && board_state() != "DRAW"
            && board_state() != "WIN"){
        if (ui->reset->text() == "SWAP PIECES")
            ui->reset->setText("RESET");

        if (difficulty_level < 3){
            ui->slot4->setText(player_piece);
            if (board_state() == "WIN" || board_state() == "DRAW")
                winners_ceremony(board_state());
            else
                cpu_move();
        }
        else{
            ui->slot4->setText(twop_mode_current_player);
            if (board_state() == "WIN" || board_state() == "DRAW")
                winners_ceremony(board_state());
            else
                swap_2p();
        }
    }
}

void MainWindow::on_slot5_clicked()
{
    if (ui->slot5->text() == default_text
            && board_state() != "DRAW"
            && board_state() != "WIN"){
        if (ui->reset->text() == "SWAP PIECES")
            ui->reset->setText("RESET");

        if (difficulty_level < 3){
            ui->slot5->setText(player_piece);
            if (board_state() == "WIN" || board_state() == "DRAW")
                winners_ceremony(board_state());
            else
                cpu_move();
        }
        else{
            ui->slot5->setText(twop_mode_current_player);
            if (board_state() == "WIN" || board_state() == "DRAW")
                winners_ceremony(board_state());
            else
                swap_2p();
        }
    }
}

void MainWindow::on_slot6_clicked()
{
    if (ui->slot6->text() == default_text
            && board_state() != "DRAW"
            && board_state() != "WIN"){
        if (ui->reset->text() == "SWAP PIECES")
            ui->reset->setText("RESET");

        if (difficulty_level < 3){
            ui->slot6->setText(player_piece);
            if (board_state() == "WIN" || board_state() == "DRAW")
                winners_ceremony(board_state());
            else
                cpu_move();
        }
        else{
            ui->slot6->setText(twop_mode_current_player);
            if (board_state() == "WIN" || board_state() == "DRAW")
                winners_ceremony(board_state());
            else
                swap_2p();
        }
    }
}

void MainWindow::on_slot7_clicked()
{
    if (ui->slot7->text() == default_text
            && board_state() != "DRAW"
            && board_state() != "WIN"){
        if (ui->reset->text() == "SWAP PIECES")
            ui->reset->setText("RESET");

        if (difficulty_level < 3){
            ui->slot7->setText(player_piece);
            if (board_state() == "WIN" || board_state() == "DRAW")
                winners_ceremony(board_state());
            else
                cpu_move();
        }
        else{
            ui->slot7->setText(twop_mode_current_player);
            if (board_state() == "WIN" || board_state() == "DRAW")
                winners_ceremony(board_state());
            else
                swap_2p();
        }
    }
}

void MainWindow::on_slot8_clicked()
{
    if (ui->slot8->text() == default_text
            && board_state() != "DRAW"
            && board_state() != "WIN"){
        if (ui->reset->text() == "SWAP PIECES")
            ui->reset->setText("RESET");

        if (difficulty_level < 3){
            ui->slot8->setText(player_piece);
            if (board_state() == "WIN" || board_state() == "DRAW")
                winners_ceremony(board_state());
            else
                cpu_move();
        }
        else{
            ui->slot8->setText(twop_mode_current_player);
            if (board_state() == "WIN" || board_state() == "DRAW")
                winners_ceremony(board_state());
            else
                swap_2p();
        }
    }
}

void MainWindow::on_slot9_clicked()
{
    if (ui->slot9->text() == default_text
            && board_state() != "DRAW"
            && board_state() != "WIN"){
        if (ui->reset->text() == "SWAP PIECES")
            ui->reset->setText("RESET");

        if (difficulty_level < 3){
            ui->slot9->setText(player_piece);
            if (board_state() == "WIN" || board_state() == "DRAW")
                winners_ceremony(board_state());
            else
                cpu_move();
        }
        else{
            ui->slot9->setText(twop_mode_current_player);
            if (board_state() == "WIN" || board_state() == "DRAW")
                winners_ceremony(board_state());
            else
                swap_2p();
        }
    }
}

QString MainWindow::board_state(){

    //This is a horrendous way to do this. I really don't care!

    if (ui->slot1->text() == default_text &&
            ui->slot2->text() == default_text &&
            ui->slot3->text() == default_text &&
            ui->slot4->text() == default_text &&
            ui->slot5->text() == default_text &&
            ui->slot6->text() == default_text &&
            ui->slot7->text() == default_text &&
            ui->slot8->text() == default_text &&
            ui->slot9->text() == default_text)
        return "CLEAR";

    if (win_check())
        return "WIN";

    if (win_check() == false &&
            (ui->slot1->text() != default_text &&
             ui->slot2->text() != default_text &&
             ui->slot3->text() != default_text &&
             ui->slot4->text() != default_text &&
             ui->slot5->text() != default_text &&
             ui->slot6->text() != default_text &&
             ui->slot7->text() != default_text &&
             ui->slot8->text() != default_text &&
             ui->slot9->text() != default_text))
        return "DRAW";

    else
        return "PLAYING";
}

bool MainWindow::win_check(){
    if ((ui->slot1->text() == player_piece && ui->slot2->text() == player_piece && ui->slot3->text() == player_piece) ||
            (ui->slot1->text() == cpu_piece && ui->slot2->text() == cpu_piece && ui->slot3->text() == cpu_piece)) { // Checks 123
        winner = ui->slot1->text();
        return true;
    }
    if ((ui->slot4->text() == player_piece && ui->slot5->text() == player_piece && ui->slot6->text() == player_piece) ||
            (ui->slot4->text() == cpu_piece && ui->slot5->text() == cpu_piece && ui->slot6->text() == cpu_piece)) { // Checks 456
        winner = ui->slot4->text();
        return true;
    }
    if ((ui->slot7->text() == player_piece && ui->slot8->text() == player_piece && ui->slot9->text() == player_piece) ||
            (ui->slot7->text() == cpu_piece && ui->slot8->text() == cpu_piece && ui->slot9->text() == cpu_piece)) { // Checks 789
        winner = ui->slot7->text();
        return true;
    }
    if ((ui->slot1->text() == player_piece && ui->slot4->text() == player_piece && ui->slot7->text() == player_piece) ||
            (ui->slot1->text() == cpu_piece && ui->slot4->text() == cpu_piece && ui->slot7->text() == cpu_piece)) { // Checks 147
        winner = ui->slot1->text();
        return true;
    }
    if ((ui->slot2->text() == player_piece && ui->slot5->text() == player_piece && ui->slot8->text() == player_piece) ||
            (ui->slot2->text() == cpu_piece && ui->slot5->text() == cpu_piece && ui->slot8->text() == cpu_piece)) { // Checks 258
        winner = ui->slot2->text();
        return true;
    }
    if ((ui->slot9->text() == player_piece && ui->slot6->text() == player_piece && ui->slot3->text() == player_piece) ||
            (ui->slot9->text() == cpu_piece && ui->slot6->text() == cpu_piece && ui->slot3->text() == cpu_piece)) { // Checks 369
        winner = ui->slot3->text();
        return true;
    }
    if ((ui->slot7->text() == player_piece && ui->slot5->text() == player_piece && ui->slot3->text() == player_piece) ||
            (ui->slot7->text() == cpu_piece && ui->slot5->text() == cpu_piece && ui->slot3->text() == cpu_piece)) { // Checks 357
        winner = ui->slot3->text();
        return true;
    }
    if ((ui->slot1->text() == player_piece && ui->slot5->text() == player_piece && ui->slot9->text() == player_piece) ||
            (ui->slot1->text() == cpu_piece && ui->slot5->text() == cpu_piece && ui->slot9->text() == cpu_piece)){ // Checks 159
        winner = ui->slot1->text();
        return true;
    }
    return false;
}

void MainWindow::winners_ceremony(QString result){
    if (result == "WIN"){
        ui->status_message->setText("WINNER: " + winner + "  (RESET TO PLAY AGAIN)");
        if (difficulty_level == 0 && winner == player_piece)
            ui->easy_wins->display(ui->easy_wins->intValue() + 1);
        else if (difficulty_level == 1 && winner == player_piece)
            ui->medium_wins->display(ui->medium_wins->intValue() + 1);
        else if (difficulty_level == 2 && winner == player_piece)
            ui->hard_wins->display(ui->hard_wins->intValue() + 1);

        save_player_wins();
    }
    else
        ui->status_message->setText("DRAW!!!!  (RESET TO PLAY AGAIN)");
}

void MainWindow::on_reset_clicked()
{
    if (board_state() == "WIN" || board_state() == "DRAW"){
        ui->reset->setText("NOPE, NO RAGE-QUITTING.");
        ui->slot1->setText(default_text);
        ui->slot2->setText(default_text);
        ui->slot3->setText(default_text);
        ui->slot4->setText(default_text);
        ui->slot5->setText(default_text);
        ui->slot6->setText(default_text);
        ui->slot7->setText(default_text);
        ui->slot8->setText(default_text);
        ui->slot9->setText(default_text);

        if (difficulty_level == 0)
            ui->status_message->setText("DIFFICULTY: EASY");
        else if (difficulty_level == 1)
            ui->status_message->setText("DIFFICULTY: MEDIUM");
        else if (difficulty_level == 2)
            ui->status_message->setText("DIFFICULTY: HARD");
        else if (difficulty_level == 3)
            ui->status_message->setText("TWO-PLAYER MODE. TURN: " % twop_mode_current_player);

        ui->reset->setText("SWAP PIECES");
    }
    else if (board_state() == "PLAYING"){
        ui->reset->setText("NOPE, NO RAGE-QUITTING.");
    }
    else if (board_state() == "CLEAR"){
        swap_pieces();
    }
}

void MainWindow::on_easy_button_clicked()
{
    if (board_state() == "CLEAR"){
        difficulty_level = 0;
        ui->status_message->setText("DIFFICULTY: EASY");
    }
}

void MainWindow::on_medium_button_clicked()
{
    if (board_state() == "CLEAR"){
        difficulty_level = 1;
        ui->status_message->setText("DIFFICULTY: MEDIUM");
    }
}


void MainWindow::on_hard_button_clicked()
{
    if (board_state() == "CLEAR"){
        difficulty_level = 2;
        ui->status_message->setText("DIFFICULTY: HARD");
    }
}

void MainWindow::on_two_player_mode_clicked()
{
    if (board_state() == "CLEAR"){
        difficulty_level = 3;
        ui->status_message->setText("TWO-PLAYER MODE. TURN: " % twop_mode_current_player);
    }
}

void MainWindow::button_changer(QString style){
    ui->slot1->setStyleSheet(style);
    ui->slot2->setStyleSheet(style);
    ui->slot3->setStyleSheet(style);
    ui->slot4->setStyleSheet(style);
    ui->slot5->setStyleSheet(style);
    ui->slot6->setStyleSheet(style);
    ui->slot7->setStyleSheet(style);
    ui->slot8->setStyleSheet(style);
    ui->slot9->setStyleSheet(style);

    ui->easy_button->setStyleSheet(style);
    ui->easy_wins->setStyleSheet(style);
    ui->medium_button->setStyleSheet(style);
    ui->medium_wins->setStyleSheet(style);
    ui->hard_button->setStyleSheet(style);
    ui->hard_wins->setStyleSheet(style);

    ui->two_player_mode->setStyleSheet(style);
    ui->reset->setStyleSheet(style);

    ui->theme1->setStyleSheet(style);
    ui->theme2->setStyleSheet(style);
    ui->theme3->setStyleSheet(style);
    ui->theme4->setStyleSheet(style);
    ui->theme5->setStyleSheet(style);
    ui->theme6->setStyleSheet(style);
    ui->theme7->setStyleSheet(style);
}

void MainWindow::on_volume_valueChanged(int value)
{
    volume->setVolume(powf((static_cast<float>(value) / 100), 4));
}

void MainWindow::on_theme1_clicked()
{
    if (theme != 1){
        theme = 1;

        ui->centralwidget->setStyleSheet("QWidget#centralwidget{"
                                         "background-image: url(:/img/img/sopranos.png);"
                                         "}");
        button_changer("border: 1px solid red; "
                       "color: red; "
                       "background-color: rgb(0, 0, 0); "
                       "font: 900 25pt 'Impact';");
        ui->status_message->setStyleSheet("border: 1px solid red; "
                                          "background-color: rgb(0, 0, 0); "
                                          "font: 900 20pt 'Impact'; "
                                          "color: red;");

        jukebox(theme);
        gif_change(theme);

        this->setWindowTitle("Never had the makings of a varisty athlete.");
    }
}


void MainWindow::on_theme2_clicked()
{
    if (theme != 2){
        theme = 2;

        ui->centralwidget->setStyleSheet("QWidget#centralwidget{"
                                         "background-image: url(:/img/img/eva.png);"
                                         "}");
        button_changer("border: 1px solid orange; "
                       "font: 900 22pt 'Bodoni 72';");
        ui->status_message->setStyleSheet("border: 1px solid orange; "
                                          "background-color: rgb(0, 0, 0); "
                                          "font: 900 16pt 'Bodoni 72'; "
                                          "color: white;");

        jukebox(theme);
        gif_change(theme);

        this->setWindowTitle("It all comes tumblin' down, tumblin' down, tumblin' down~");
    }
}


void MainWindow::on_theme3_clicked()
{
    if (theme != 3){
        theme = 3;

        ui->centralwidget->setStyleSheet("QWidget#centralwidget{"
                                         "background-image: url(:/img/img/simp.png);"
                                         "}");
        button_changer("border: 1px solid white; "
                       "background-color: pink; "
                       "color: white; "
                       "font: 900 18pt 'Chalkduster';");
        ui->status_message->setStyleSheet("border: 1px solid white; "
                                          "background-color: rgb(0, 0, 0); "
                                          "font: 900 12pt 'Chalkduster'; "
                                          "color: white;");

        jukebox(theme);
        gif_change(theme);

        this->setWindowTitle("Hey, don't have a cow, man!");
    }
}


void MainWindow::on_theme4_clicked()
{
    if (theme != 4){
        theme = 4;

        ui->centralwidget->setStyleSheet("QWidget#centralwidget{"
                                         "background-image: url(:/img/img/shrek.png);"
                                         "}");
        button_changer("border: 1px solid saddlebrown; "
                       "background-color: olive; "
                       "color: yellow; "
                       "font: 900 18pt 'Luminari';");
        ui->status_message->setStyleSheet("border: 1px solid brown; "
                                          "background-color: rgb(0, 0, 0); "
                                          "font: 900 14pt 'Luminari'; "
                                          "color: yellow;");

        jukebox(theme);
        gif_change(theme);

        this->setWindowTitle("WHAT are ya DOIN in MY SWAMP??????");
    }
}


void MainWindow::on_theme5_clicked()
{
    if (theme != 5){
        theme = 5;

        ui->centralwidget->setStyleSheet("QWidget#centralwidget{"
                                         "background-image: url(:/img/img/sonic.png);"
                                         "}");
        button_changer("border: 1px solid red; "
                       "color: yellow; "
                       "background-color: deepskyblue; "
                       "font: 900 18pt 'Comic Sans MS';");
        ui->status_message->setStyleSheet("border: 1px solid maroon; "
                                          "color: yellow; "
                                          "background-color: rgb(0, 0, 0); "
                                          "font: 900 14pt 'Comic Sans MS';");

        jukebox(theme);
        gif_change(theme);

        this->setWindowTitle("Gotta go fast! You're too slow! Take me sweet death!");
    }
}


void MainWindow::on_theme6_clicked()
{
    if (theme != 6){
        theme = 6;

        ui->centralwidget->setStyleSheet("QWidget#centralwidget{"
                                         "background-image: url(:/img/img/thanos.png);"
                                         "}");
        button_changer("border: 1px solid white; "
                       "color: white; "
                       "font: 900 18pt 'Futura';");
        ui->status_message->setStyleSheet("border: 1px solid purple; "
                                          "background-color: black; "
                                          "font: 900 12pt 'Futura';");

        jukebox(theme);
        gif_change(theme);

        this->setWindowTitle("Perfectly drippy, as all things should be.");
    }
}


void MainWindow::on_theme7_clicked()
{
    if (theme != 7){
        theme = 7;
        ui->centralwidget->setStyleSheet("QWidget#centralwidget{"
                                         "background-image: url(:/img/img/retsuko.png);"
                                         "}");
        button_changer("border: 1px solid red; "
                       "background-color: pink; "
                       "font: 900 18pt 'Rockwell'; "
                       "color: white");
        ui->status_message->setStyleSheet("border: 1px solid pink; "
                                          "background-color: rgb(0, 0, 0); "
                                          "font: 900 14pt 'Rockwell';");

        jukebox(theme);
        gif_change(theme);

        this->setWindowTitle("RAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAWRGH");
    }
}

void MainWindow::jukebox(int& selection){
    static QMediaPlayer *music = new QMediaPlayer();
    music->setLoops(QMediaPlayer::Infinite);

    switch (selection){
    case 1:
        music->setSource(QUrl("qrc:/music/sopranos.mp3"));
        break;
    case 2:
        music->setSource(QUrl("qrc:/music/eva.mp3"));
        break;
    case 3:
        music->setSource(QUrl("qrc:/music/simp.mp3"));
        break;
    case 4:
        music->setSource(QUrl("qrc:/music/shrek.mp3"));
        break;
    case 5:
        music->setSource(QUrl("qrc:/music/sonic.mp3"));
        break;
    case 6:
        music->setSource(QUrl("qrc:/music/thanos.mp3"));
        break;
    case 7:
        music->setSource(QUrl("qrc:/music/rage.mp3"));
        break;
    }

    music->setAudioOutput(volume);
    music->play();

}

void MainWindow::gif_change(int& selection){
    static QMovie *movie = new QMovie;
    movie->stop();

    switch (selection){
    case 1:
        movie->setFileName(":/gifs/gifs/tony.gif");
        break;
    case 2:
        movie->setFileName(":/gifs/gifs/asuka.gif");
        break;
    case 3:
        movie->setFileName(":/gifs/gifs/bart.gif");
        break;
    case 4:
        movie->setFileName(":/gifs/gifs/shrek.gif");
        break;
    case 5:
        movie->setFileName(":/gifs/gifs/sonic.gif");
        break;
    case 6:
        movie->setFileName(":/gifs/gifs/thanos.gif");
        break;
    case 7:
        movie->setFileName(":/gifs/gifs/retsuko.gif");
        break;
    }

    ui->img_display->setMovie(movie);
    ui->img_display->setScaledContents(true);
    movie->start();
}

