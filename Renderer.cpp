#include "Renderer.h"
#include <fstream>
#include <memory.h>
#include <vector>
#define PAD 20 //이미 지정되어 있는 COLOR_XXXX값과 겹치지 않게 컬러를 설정해주기 위해 임의의 padding값 설정

void Renderer::Init(){
    initscr(); //ncurses 초기화

    wresize(stdscr, 200, 200 ); //스크린버퍼사이즈를 충분히 키움. 터미널크기는 제어 안됨.
	keypad(stdscr, true); // 방향키 사용
	noecho(); // 사용자로부터 받은 입력 문자를 출력X
	curs_set(0); // 커서 표시X

    start_color(); //color적용시 꼭 필요,터미널에서 지원되는 모든 색들을 초기화해서 준비
    init_color((int)TileType::Size + 1, 1000,1000,1000);
    init_color((short)TileType::Item_Growth + PAD, 200, 800, 200);
    init_color((short)TileType::Item_Poison + PAD, 1000, 400, 200);
    init_pair((int)TileType::Wall, COLOR_RED, (int)TileType::Size + 1); //팔레트 넘버3, 전경색 RED, 배경색 WHITE
    init_pair((int)TileType::ImmuneWall, COLOR_BLUE, (int)TileType::Size + 1);
    init_pair((int)TileType::Snake_Head, COLOR_CYAN, (int)TileType::Size + 1);
    init_pair((int)TileType::Snake_Tail, COLOR_BLACK, (int)TileType::Size + 1);
    init_pair((int)TileType::Snake_Body, COLOR_GREEN, (int)TileType::Size + 1);
    init_pair((int)TileType::Item_Growth, (short)TileType::Item_Growth + PAD, (int)TileType::Size);
    init_pair((int)TileType::Item_Poison, COLOR_RED, (short)TileType::Item_Poison + PAD );
    init_pair((int)TileType::Gate, COLOR_CYAN, COLOR_BLACK);
    init_pair((int)TileType::Gate2, COLOR_MAGENTA, COLOR_BLACK);
    init_pair((int)TileType::Size, COLOR_BLUE, COLOR_BLACK);
    bkgd(COLOR_PAIR((int)TileType::Size)); //background 지정

    //윈도우 속성 받아오기
    std::ifstream inStream("data/screen/Screen.txt");
    if(!inStream.is_open()){
        printw("Error\n");
        endwin();
        return;
    }
    for(int i = 0; i < (int)WindowType::SIZE; i++){
        for(int j = 0; j < 4; j++)
            inStream >> subWinProperty[i][j];
    }
    inStream.close();

    //SubWindow 생성
    WINDOW* subscrs[(int)WindowType::SIZE];
    for(int i = 0; i < (int)WindowType::SIZE; i++){

        //서브윈도우 생성
        subscrs[i] = subwin(stdscr, subWinProperty[i][0], subWinProperty[i][1], subWinProperty[i][2], subWinProperty[i][3]);

        windows[i] = subscrs[i];
    }
}

void Renderer::DrawUI(){
    DrawBox(windows[(int)WindowType::SCORE]);
    DrawBox(windows[(int)WindowType::MISSION]);
}

void Renderer::Draw(int** scrBuffer){
    for(int i = 0; i < MAXROW; i++){
        move(i, 1);
        for(int k = 0; k < MAXCOL; k++){
            switch(scrBuffer[i][k]){
                case (int)TileType::Blank:
                // attron(COLOR_PAIR(1));
                addch(ACS_CKBOARD);
                // attroff(COLOR_PAIR(1));
                break;

                case (int)TileType::Wall:
                attron(COLOR_PAIR((int)TileType::Wall));
                addch(ACS_CKBOARD);
                attroff(COLOR_PAIR((int)TileType::Wall));
                break;

                case (int)TileType::ImmuneWall:
                attron(COLOR_PAIR((int)TileType::ImmuneWall));
                addch(ACS_CKBOARD);
                attroff(COLOR_PAIR((int)TileType::ImmuneWall));
                break;

                case (int)TileType::Snake_Head:
                attron(COLOR_PAIR((int)TileType::Snake_Head));
                // addch(ACS_CKBOARD);
                addch(ACS_BULLET);
                attroff(COLOR_PAIR((int)TileType::Snake_Head));
                break;

                case (int)TileType::Snake_Body:
                attron(COLOR_PAIR((int)TileType::Snake_Body));
                addch(ACS_CKBOARD);
                attroff(COLOR_PAIR((int)TileType::Snake_Body));
                break;

                case (int)TileType::Snake_Tail:
                attron(COLOR_PAIR((int)TileType::Snake_Tail));
                addch(ACS_CKBOARD);
                attroff(COLOR_PAIR((int)TileType::Snake_Tail));
                break;

                case (int)TileType::Item_Growth:
                attron(COLOR_PAIR((int)TileType::Item_Growth));
                addch(ACS_BLOCK);
                attroff(COLOR_PAIR((int)TileType::Item_Growth));
                break;

                case (int)TileType::Item_Poison:
                attron(COLOR_PAIR((int)TileType::Item_Poison));
                addch(ACS_BLOCK);
                attroff(COLOR_PAIR((int)TileType::Item_Poison));
                break;

                case (int)TileType::Gate:
                attron(COLOR_PAIR((int)TileType::Gate));
                addch(ACS_BLOCK);
                attroff(COLOR_PAIR((int)TileType::Gate));
                break;

                case (int)TileType::Gate2:
                attron(COLOR_PAIR((int)TileType::Gate2));
                addch(ACS_BLOCK);
                attroff(COLOR_PAIR((int)TileType::Gate2));
                break;
            }
        }
    }
}

//
void Renderer::Refresh(){
    refresh();
    for(int i = 0; i < (int)WindowType::SIZE; i++){
        wrefresh(windows[i]);
    }

    ClearWindow();

}

void Renderer::DrawBox(WINDOW* win){
    box(win, 0, 0);
    wbkgd(win, COLOR_PAIR(1)); //백그라운드 컬러도 지정
}

void Renderer::PrintSystemMessage(const char* str){
    std::vector<std::string> strs;
    std::string tempStr;
    int maxLen=0;
    for(int i=0;  ;i++)
    {
        if(str[i]=='\n')
        {
            strs.push_back(tempStr);
            if(maxLen < tempStr.length())
                maxLen = tempStr.length();
            tempStr.clear();
        }
        else if (str[i]=='\0')
        {
            strs.push_back(tempStr);
            if(maxLen < tempStr.length())
                maxLen = tempStr.length();
            tempStr.clear();
            break;
        }
        else
        {
            tempStr.push_back(str[i]);
        }
    }

    WINDOW* sysWin = subwin(stdscr, 10, maxLen, 10, maxLen<MAXCOL?MAXCOL/2-maxLen/2 : 3);
    wattron(sysWin, COLOR_PAIR(3));
    for(int i=0; i<strs.size(); i++)
    {
        wmove(sysWin, i, (maxLen/2 - strs[i].length()/2));
        wprintw(sysWin, strs[i].c_str());
    }
    wattroff(sysWin, COLOR_PAIR(3));
    wrefresh(sysWin);
    delwin(sysWin);
}

void Renderer::ClearWindow(){
    for(int i = 0; i < (int)WindowType::SIZE; i++){
        werase(windows[i]);
    }
}
void Renderer::PrintScoreMessage(const char* str){
    wmove(windows[(int)WindowType::SCORE], 0, 0);
    wprintw(windows[(int)WindowType::SCORE], str);
}
void Renderer::PrintScoreMessageXY(int x, int y, const char* str){
    wmove(windows[(int)WindowType::SCORE], y, x);
    wprintw(windows[(int)WindowType::SCORE], str);
}

void Renderer::PrintMissionMessage(const char* str){
    wmove(windows[(int)WindowType::MISSION], 0, 0);
    wprintw(windows[(int)WindowType::MISSION], str);
}

void Renderer::PrintMissionMessageXY(int x, int y, const char* str){
    wmove(windows[(int)WindowType::MISSION], y, x);
    wprintw(windows[(int)WindowType::MISSION], str);

}

void Renderer::End(){
    endwin();
}
