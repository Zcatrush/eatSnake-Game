#include <curses.h>
#include<iostream>
#include<cstdlib>
#include<ctime>
#include<ncurses.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <termios.h>
#include <fcntl.h>
#define width 50
#define length 25
using namespace std;
int direction_x = 0, direction_y = 1;//由这两个变量控制蛇的下一步移动操作
int score = 0;
static int star_i, star_j;

int _kbhit(void) {
    struct termios oldt, newt;
    int ch;
    int oldf;

    tcgetattr(STDIN_FILENO, &oldt);  // 保存当前终端设置
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO); // 关闭缓冲区和回显
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);
    oldf = fcntl(STDIN_FILENO, F_GETFL, 0);
    fcntl(STDIN_FILENO, F_SETFL, oldf | O_NONBLOCK);

    ch = getchar();

    tcsetattr(STDIN_FILENO, TCSANOW, &oldt); // 恢复终端设置
    fcntl(STDIN_FILENO, F_SETFL, oldf);

    if (ch != EOF) {
        ungetc(ch, stdin);  // 将字符放回输入缓冲区
        return 1;
    }

    return 0;
}
char _getch(void) {
    struct termios oldt, newt;
    char ch;
    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);
    ch = getchar();
    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    return ch;
}

void generate_star() {
    srand((unsigned int)time(NULL));
    star_i = rand() % (length/5*3) + 3;
    star_j = rand() % (width/5*3) + 3;
}//随机生成star的位置
class snake {
public:
    //构造函数
    snake(int a, int b) {
        snake_i = a;
        snake_j = b;
        next = NULL;
    }
    int snake_i, snake_j;
    snake* next;
};
snake* init_snake() {
    snake* head = new snake(0, 0);
    snake* top = new snake(5, 6);
    snake* mid = new snake(5, 5);
    snake* tail = new snake(5, 4);
    head->next = top;
    top->next = mid;
    mid->next = tail;
    tail->next = NULL;
    return(head);
}//初始化一条长度为3的蛇，以链形式
bool isfalse(snake* nTop) {
    if (nTop->snake_i == 0 || nTop->snake_i == length-1 || nTop->snake_j == 0 || nTop->snake_j == width-1)
    {
        return true;//撞到墙壁算失败
    }
    snake* m = nTop->next;
    while (m) {
        
        if (nTop->snake_i == m->snake_i && nTop->snake_j == m->snake_j)return true;//碰到自己的身体为失败
        m = m->next;
    } 
    return false;
}//判定游戏是否结束
bool eatstar(snake* ntop) {
    if (star_i == ntop->snake_i && star_j == ntop->snake_j)return true;
    
    return false;
}
void movesnake(snake* head) {
    void drawNewMap(snake * head);
    if (!head)return;//防止空指针
    if (_kbhit()) {
        char cmd = _getch();
        char cmd_1 = '0';
       if(cmd_1!=cmd ){
        switch (cmd_1) {
        case 'w': 
            if (direction_x ==0) { direction_x = -1; direction_y = 0; }
            
        break;
        case 'a': 
            if (direction_y == 0) { direction_x = 0; direction_y = -1; }
           
        break;
        case 's': 
            if (direction_x == 0) { direction_x = 1; direction_y = 0; }
            
        break;
        case 'd': 
            if (direction_y == 0) { direction_x = 0; direction_y = 1; }
            
        break;
        default: break;
        }
    }//若按键，更改移动的方向
    }
    else{
        direction_x=direction_x%2+direction_x;
        direction_y=direction_y%2+direction_y;
    }
    snake* newTop=new snake(0,0);
    newTop->snake_i = head->next->snake_i + direction_x;
    newTop->snake_j = head->next->snake_j + direction_y;
    newTop->next = head->next;
    head->next = newTop;//插入新的结点
    if (isfalse(newTop)) return;
        
    else if (eatstar(newTop)) {
        score++; generate_star();
    }//吃掉星星，重新生成一个，尾结点保留（长度加了1）
    
    else {
        snake* p = head->next;
        while (p->next->next) {
            p = p->next;
        }//此时p指向倒数第2个结点


        delete(p->next);
        p->next= NULL;//删除尾结点
    }
    drawNewMap(head);
}




void drawNewMap(snake* head) {
    

    int a[length][width] = {0 };
    int i, j;
    snake* p2 = head;
    while (p2) {
        a[p2->snake_i][p2->snake_j] = 1;
        p2 = p2->next;
    }
    a[star_i][star_j] = 2;
    for (i = 0; i < length; i++) {
        for (j = 0; j < width; j++) {
            if (i == 0 || i == length - 1 || j == 0 || j == width - 1)
                a[i][j] = 3;
        }
    }
    cout << "当前分数为：" << score << endl;
    cout << "当前等级为：" << score/5 << endl;
    for (i = 0; i < length; i++) {
        for (j = 0; j < width; j++) {
            if (a[i][j] == 1)cout << "●";
            else if (a[i][j] == (2))cout << "*";
            else if (a[i][j] == (3))cout << "■";
            else cout << " ";
        }
        cout << endl;
    }
                
                
                
 

}//打印地图以及蛇还有星星

void showMenu() {
    int i, j;
    for (i = 0; i < 25; i++) {
        for (j = 0; j < 50; j++) {
            if (i == 0 || i == 24 || j == 0 || j == 49)
                cout << "■";
            else if (i == 10 && j == 20) { cout << "1.START"; j += 6; }
            else if (i == 15 && j == 20) {
                cout << "2.EXIT"; j += 5;
            }
            else cout << " ";

        }
        cout << "\n";
    }
}
void game() {
    generate_star();//初始化星星   
    snake* Head;
    Head = init_snake();//初始化蛇
    drawNewMap(Head);
    do {

        usleep(100000-10000*score%50000);
        system("clear");
        movesnake(Head);
        
    } while (!isfalse(Head->next));//若蛇头未碰壁，循环进行 清屏->移动蛇->打印地图 的操作
    system("clear");
    cout << endl;
    cout << endl;
    cout << endl;
    cout << endl;
    cout << "\tGAME OVER!" << endl;//蛇头碰壁，结束游戏
}
int main(){
    
    int choice = 0;
   
    
       
        
        //显示初始菜单
        
        
        system("clear");      
        showMenu();
       while(1){
       if (_kbhit()) {

           choice = _getch() - '0';//根据按键选择选项
           
           switch (choice) {
           case 1:
           system("clear");game();system("pause");break;
            
           case 2:
               system("clear");system("pause"); return 0; break;
           default:
               break;
           }
           }
        }
       
      
       
           
       
    
    
         return 0;
        
    
}