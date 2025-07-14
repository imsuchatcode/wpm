#include <iostream>
#include <bits/stdc++.h>
#include <curses.h>
using namespace std;

WINDOW* init(){
    initscr();
    start_color();
    init_pair(1, COLOR_GREEN, COLOR_BLACK);
    init_pair(2, COLOR_RED, COLOR_BLACK);
    init_pair(3, COLOR_WHITE, COLOR_BLACK);

    int yMax, xMax;
    getmaxyx(stdscr, yMax, xMax);
    WINDOW * win = newwin(yMax, xMax, 0, 0);
    box(win, 0 ,0);
    refresh();
    wrefresh(win);
    return win;
}

bool checkingChar(bool state, string sentence, WINDOW * win){
    int cur_chr = 0;
    int cur_x = 1;
    while (state)
    {
        if (cur_chr >= sentence.size())
        {
            state = false;
        }

        int ch = wgetch(win);
        if (ch == sentence[cur_chr])
        {
            wattron(win, COLOR_PAIR(1));
            mvwprintw(win, 2, cur_x, "%c", sentence[cur_chr]);
            wattroff(win, COLOR_PAIR(1));
        }
        else
        {
            wattron(win, COLOR_PAIR(2));
            mvwprintw(win, 2, cur_x, "%c", sentence[cur_chr]);
            wattroff(win, COLOR_PAIR(2));
        }
        wrefresh(win);
        cur_chr++;
        cur_x++;
    }
    return false;
}

int main(int argc, char ** argv){
    WINDOW *curwin =  init();
    bool curstate = true;

    while (curstate)
    {
        // try to create a function thaty take random phraise to from a text file (would be great to learn wtf is api)
        string sentence = "please try to write like this sentence";
        mvwprintw(curwin, 1, 1, sentence.c_str());

        // create a funtion that check type per min (haredest)

        // create a funtuntion to wrap this check if correct word
        curstate = checkingChar(curstate, sentence, curwin);
        // create a end loop that tell should player continue or stop
    }
    getch();
    
    // delocate tehe memory and ends ncurses
    endwin();
    return 0;
}