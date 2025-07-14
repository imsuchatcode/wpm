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

int main(int argc, char ** argv){
    WINDOW *curwin =  init();
    string sentence = "please try to write like this sentence";
    mvwprintw(curwin, 1, 1, sentence.c_str());
    bool state = true;
    int cur_chr = 0;
    int cur_x = 1;
    while(state){
        if(cur_chr >= sentence.size()){
            state = false;
        }
        int ch = wgetch(curwin);
        if (ch == sentence[cur_chr]){
            wattron(curwin,COLOR_PAIR(1));
            mvwprintw(curwin, 2, cur_x, "%c", sentence[cur_chr]); 
            wattroff(curwin, COLOR_PAIR(1));
        }
        else{
            wattron(curwin, COLOR_PAIR(2));
            mvwprintw(curwin, 2, cur_x, "%c", sentence[cur_chr]);
            wattroff(curwin, COLOR_PAIR(2));
        }
        wrefresh(curwin);
        cur_chr++;
        cur_x++;
    }
    getch();
    
    // delocate tehe memory and ends ncurses
    endwin();
    return 0;
}