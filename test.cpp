#include <iostream>
#include <string>
#include <ctime>
#include <vector>
#include <cmath>
#include <curses.h>
#include <fstream>
#include <ctime>
#include <chrono>

using namespace std;
using namespace chrono;

int yMax, xMax;

WINDOW* init(){
    initscr();
    start_color();
    noecho();
    init_pair(1, COLOR_GREEN, COLOR_BLACK);
    init_pair(2, COLOR_RED, COLOR_BLACK);
    init_pair(3, COLOR_WHITE, COLOR_BLACK);

    getmaxyx(stdscr, yMax, xMax);
    WINDOW * win = newwin(yMax, xMax, 0, 0);
    mvwprintw(win, (yMax) / 2 , ((xMax) / 2) - 29, "%s", "this is a word per minite test press any where to continue");
    wgetch(win);
    wclear(win);
    box(win, 0, 0);
    refresh();
    wrefresh(win);
    return win;
}

int charCount(string sentence){
    int charcount = 0;
    for (int i = 0 ; i < sentence.size(); i++){
        if (sentence[i] == ' '){
            continue;
        }
        charcount++;
    }
    return charcount;
}

bool shouldContinue(WINDOW * win)
{
    nodelay(win, false);
    mvwprintw(win, yMax / 2, (xMax / 2) - 14, "do you wish to continue ?: [Y/N]");
    wrefresh(win);
    refresh();
    int answer = wgetch(win);
    return ((answer == 'Y') || (answer == 'y'));
}

void updateWin(WINDOW * win, int total , int correct_char, high_resolution_clock::time_point start_time){
    auto now = high_resolution_clock::now();
    double duration = duration_cast<milliseconds>(now - start_time).count() / 1000.0;

    if (duration <= 0) return;
    int wpm = round((correct_char / 5) / (duration / 60));

    double accuracy = total > 0 ? ((double)correct_char / total) * 100 : 0.0;

    char buf[120];
    snprintf(buf, sizeof(buf), "WPM : %d | accuracy : %.1f%% | duration : %.1fs", wpm, accuracy, duration);

    wmove(win, 3, 1);
    wclrtoeol(win);
    mvwprintw(win, 3, 1, "%s", buf);
    wrefresh(win);
}

void checkingChar(string sentence, WINDOW *win)
{
    int cur_chr = 0;
    int cur_x = 1;
    int numchar = charCount(sentence);
    int corrchar = 0;
    int total_attemp = 0;    

    auto start = high_resolution_clock::now();
    auto last_update = start;  
    wmove(win, 2, 1);

    nodelay(win, false);
    while (cur_chr < sentence.size())
    {
        int ch = wgetch(win);

        auto now = high_resolution_clock::now();
        if (duration_cast<milliseconds>(now - last_update).count() >= 100){
            updateWin(win, total_attemp, corrchar, start);
            last_update = now;
        }

        if (ch == ERR){
            napms(10);
            continue;
        }

        if ((sentence[cur_chr] == ' ') && (ch != ' ')){
            mvwprintw(win, 2, cur_x, "%c" ,' ');
            continue;
        }
        else if (ch == sentence[cur_chr])
        {
            wattron(win, COLOR_PAIR(1));
            mvwprintw(win, 2, cur_x, "%c", sentence[cur_chr]);
            wattroff(win, COLOR_PAIR(1));
            corrchar++;
            total_attemp++;    
        }
        else
        {
            wattron(win, COLOR_PAIR(2));
            mvwprintw(win, 2, cur_x, "%c", sentence[cur_chr]);
            wattroff(win, COLOR_PAIR(2));
            total_attemp++;
        }
        cur_chr++;
        cur_x++;
        wrefresh(win);
        wmove(win, 2, cur_x);
    }
    nodelay(win, true);
}

string randomSentence()
{
    vector<string> sentences;
    string sentence;
    ifstream textfile("sentence.txt");
    
    srand(time(0));
    while (getline(textfile, sentence)){
        sentences.push_back(sentence);
    }
    textfile.close();

    return sentences[rand() % sentences.size()];
}

int main(){
    WINDOW *curwin =  init();
    bool curstate = true;
    while (curstate)
    {
        // try to create a function thaty take random phraise to from a text file 
        string sentence = randomSentence();
        mvwprintw(curwin, 1, 1, "%s", sentence.c_str());

        // create a funtion that check type per min (haredest)
        // done inside checkingChar function

        // create a funtuntion to wrap this check if correct word
        checkingChar(sentence, curwin);

        // create a end loop that tell should player continue or stop
        curstate = shouldContinue(curwin);

        wclear(curwin);
        box(curwin, 0 ,0);
    }
    getch();
    
    // delocate tehe memory and ends ncurses
    endwin();
    return 0;
}