#include <iostream>
#include <bits/stdc++.h>
#include <curses.h>
#include <fstream>
#include <ctime>
#include <chrono>
using namespace std;

int yMax, xMax;

WINDOW* init(){
    initscr();
    start_color();
    init_pair(1, COLOR_GREEN, COLOR_BLACK);
    init_pair(2, COLOR_RED, COLOR_BLACK);
    init_pair(3, COLOR_WHITE, COLOR_BLACK);

    getmaxyx(stdscr, yMax, xMax);
    WINDOW * win = newwin(yMax, xMax, 0, 0);
    box(win, 0 ,0);
    refresh();
    wrefresh(win);
    return win;
}

bool shouldContinue(WINDOW * win)
{
    mvwprintw(win, yMax / 2, (xMax / 2) - 14, "do you wish to continue ?: [Y/N]");
    wrefresh(win);
    refresh();
    int answer = wgetch(win);
    if ((answer == 'Y') || (answer == 'y'))
    {
        return true;
    }
    else
    {
        return false;
    }
}

void checkingChar(string sentence, WINDOW * win){
    int cur_chr = 0;
    int cur_x = 1;

    wmove(win, 2, 1);
    while (cur_chr < sentence.size())
    {
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

int main(int argc, char ** argv){
    WINDOW *curwin =  init();
    bool curstate = true;

    while (curstate)
    {
        // try to create a function thaty take random phraise to from a text file 
        string sentence = randomSentence();
        int numchar = sentence.size();
        mvwprintw(curwin, 1, 1, "%s", sentence.c_str());
        // create a funtion that check type per min (haredest)

        // create a funtuntion to wrap this check if correct word
        auto start = chrono::high_resolution_clock::now();
        checkingChar(sentence, curwin);
        auto end = chrono::high_resolution_clock::now();

        // careful about double (if want double  must divide by a double or it will round up)
        double duration = (chrono::duration_cast<chrono::seconds>(end - start).count()) / 60.0;

        // create a end loop that tell should player continue or stop
        int wpm = round(numchar / duration);
        string result = "your word per min is : " + to_string(wpm);
        mvwprintw(curwin, (yMax / 2) + 3, (xMax / 2) - 11, "%s" , result.c_str()); 
        curstate = shouldContinue(curwin);

        wclear(curwin);
        box(curwin, 0 ,0);
    }
    getch();
    
    // delocate tehe memory and ends ncurses
    endwin();
    return 0;
}