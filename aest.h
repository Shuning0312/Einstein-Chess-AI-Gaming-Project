#ifndef AEST_H_INCLUDED
#define AEST_H_INCLUDED

#include<stdio.h>
#include<time.h>
#include<stdlib.h>
#include<winbgim.h>
#include<graphics.h>
#include<windows.h>
#include<math.h>
#include<vector>
#include<ctime>

using namespace std;
struct Position
{
    int xFrom;
    int yFrom;
    int xTo;
    int yTo;
    int num;
    int total;//记录模拟随机下棋的次数
    int win;//记录模拟随机下棋获胜的次数
    double prob;//记录模拟随机下棋的胜率
};
Position bestPosition;


#define RED1     1
#define RED2     2
#define RED3     3
#define RED4     4
#define RED5     5
#define RED6     6

#define BLUE1    -1
#define BLUE2    -2
#define BLUE3    -3
#define BLUE4    -4
#define BLUE5    -5
#define BLUE6    -6

void displayBoard(int board[][5]);//控制台画棋盘
int get_playchess_side(); //判断先后手
int get_playchess_players(int i);  //确定玩家双方
void initBoard(int board[][5]);//初始化棋盘
void drawBoard(int board[][5],int number[][6]);//绘制棋盘
int randNum(int number[][6]);//骰子环节
void playChessByMouse();//下棋
void isRightNumRed(int board[][5],int num,int n[2]);//判断红棋骰子可走的值
void isRightNumBlue(int board[][5],int num,int n[]);//判断蓝棋骰子可走的值
int isWin(int board[][5]);//判断输赢
vector<Position> genMove(int board[][5],int num,int n[],int side);//确定结构体中所有可下位置的各个值


//蒙特卡洛算法
void defineWinStart(int board[5][5]);//选择一组胜率较高的开局来进行随机模拟下棋
vector<Position> genMoveForProb(int board[][5]);//生成用于随机下棋的下棋位置
int randomPlay(int board[5][5],int num);//随机下棋
void getBestRandPosition();//获取随机下棋的最佳位置
void checkGetBestRandPosition();//测试获得随机下棋位置

//估值函数
double Value(int board[][5],int side);//估值函数的总值_负极大极小使用
double value();//估值函数的总值_极大极小和AlphaBeta方法使用
double eatValue(int board[][5],int side);//吃子值
double distanceValue(int board[][5],int side);//距离角点的值
void getCountForValue(int board[][5],int side,int c[6]);//获得每个棋子的概率次数

//负极大极小_极大极小_AlphaBeta
double minMaxSearch(int depth);//极大极小算法
double maxSearch(int depth);//极大算法
double minSearch(int depth);//极小算法
double negaMax(int depth,int side);//负极大极小
double Beta(int depth,int side);//beta剪枝
double Alpha(int depth,int side);//alpha剪枝
double alphaBeta(int depth,int moveSide);//Alpha-Beta算法

//测试函数
void checkGenMove();//测试全部可下位置
void checkIsRightNum();//测试由骰子值确定的棋子
void checkDistanceValue();//测试到角点的距离
void checkEatValue();//测试吃子估值
void checkValue();//测试估值函数
void checkRandomPlay();//测试随机下棋
void checkGetBestRandPosition();//测试随机下棋获得最佳位置
void checkMinMaxSearch();//测试极大极小算法
void checkNegaMax();//测试负极大极小
void checkGetCountForValue();//测试每个骰子的概率次数
void checkAlphaBeta();//测试AlphaBeta


//定义全局变量
int board[5][5];
int n[2];
int side = 1;
int num;
int number[1][6]={1,2,3,4,5,6};
int depth;
int c[6]={0,0,0,0,0,0};
int play_side;
FILE *fp;
int No = 0;//保存棋谱使用


//控制台绘制棋盘
void displayBoard(int board[][5])
{
    int i;
    int j;
    for(i=-1;i<5;i++)
    {
        printf("%2d",i);
    }
    printf("\n");

    for(i=0;i<5;i++)
    {
        printf("%2d",i);
        for(j=0;j<5;j++)
        {
            if(board[i][j]>=1)
            {
                printf(" O");
            }
            else if(board[i][j]<=-1)
            {
                printf(" X");
            }
            else
            {
                printf(" _");
            }

        }
        printf("\n");

    }
}

//画棋盘
void drawBoard(int board[][5],int number[][6])
{
    int i;
    int j;
    setbkcolor(8);
    cleardevice();
    for(i=0;i<6;i++)
    {
        setcolor(WHITE);
        line(30,30+80*i,430,30+80*i);
        line(30+80*i,30,30+80*i,430);
    }
    for(i=1;i<6;i++)  //表示坐标
    {
        settextstyle(SMALL_FONT,HORIZ_DIR,8);
        if(i==1)
        {
            outtextxy(5,80*0+55,"5");
            outtextxy(70*i,4,"A");
        }
        if(i==2)
        {
            outtextxy(5,80*1+55,"4");
            outtextxy(70*i+10,4,"B");
        }
        if(i==3)
        {
            outtextxy(5,80*2+55,"3");
            outtextxy(70*i+20,4,"C");
        }
        if(i==4)
        {
            outtextxy(5,80*3+55,"2");
            outtextxy(70*i+30,4,"D");
        }
        if(i==5)
        {
            outtextxy(5,80*4+55,"1");
            outtextxy(70*i+40,4,"E");
        }
    }
    for(i=0;i<5;i++)
    {
        for(j=0;j<5;j++)
        {
            if(board[i][j]>=1)
            {
                //setfillstyle(1,RED);
                setfillstyle(SOLID_FILL,RED);
                rectangle(80*j+30,80*i+30,80*j+110,80*i+110);
                floodfill(70+80*j,70+80*i,getcolor());
                //fillellipse(80*j+70,80*i+70,40,40);
                //setcolor(WHITE);
                setbkcolor(RED);
                settextstyle(SMALL_FONT,HORIZ_DIR,10);
                if(board[i][j]==1)
                {
                    outtextxy(80*j+57,80*i+54,"1");
                }
                else if(board[i][j]==2)
                {
                    outtextxy(80*j+57,80*i+54,"2");
                }
                else if(board[i][j]==3)
                {
                    outtextxy(80*j+57,80*i+54,"3");
                }
                else if(board[i][j]==4)
                {
                    outtextxy(80*j+57,80*i+54,"4");
                }
                else if(board[i][j]==5)
                {
                    outtextxy(80*j+57,80*i+54,"5");
                }
                else if(board[i][j]==6)
                {
                    outtextxy(80*j+57,80*i+54,"6");
                }

            }
            else if(board[i][j] <= -1)
            {
                /*setfillstyle(1,BLUE);
                fillellipse(80*j+70,80*i+70,40,40);*/
                setfillstyle(SOLID_FILL,BLUE);
                rectangle(80*j+30,80*i+30,80*j+110,80*i+110);
                floodfill(70+80*j,70+80*i,getcolor());
                setcolor(WHITE);
                setbkcolor(BLUE);
                settextstyle(SMALL_FONT,HORIZ_DIR,10);
                if(board[i][j]==-1)
                {
                    outtextxy(80*j+57,80*i+54,"1");
                }
                else if(board[i][j]==-2)
                {
                    outtextxy(80*j+57,80*i+54,"2");
                }
                else if(board[i][j]==-3)
                {
                    outtextxy(80*j+57,80*i+54,"3");
                }
                else if(board[i][j]==-4)
                {
                    outtextxy(80*j+57,80*i+54,"4");
                }
                else if(board[i][j]==-5)
                {
                    outtextxy(80*j+57,80*i+54,"5");
                }
                else if(board[i][j]==-6)
                {
                    outtextxy(80*j+57,80*i+54,"6");
                }
            }
            else
            {

            }
        }
    }
    for(i=0;i<6;i++)//画骰子
    {
        setfillstyle(1,9);
        fillellipse(70*i+55,80*5+70,35,35);
        setcolor(WHITE);
        setbkcolor(9);
        settextstyle(SMALL_FONT,HORIZ_DIR,10);
        if(number[0][i]==1)
        {
            outtextxy(70*i+37,80*5+55,"1");
        }
        else if(number[0][i]==2)
        {
            outtextxy(70*i+37,80*5+55,"2");
        }
        else if(number[0][i]==3)
        {
            outtextxy(70*i+37,80*5+55,"3");
        }
        else if(number[0][i]==4)
        {
            outtextxy(70*i+37,80*5+55,"4");
        }
        else if(number[0][i]==5)
        {
            outtextxy(70*i+37,80*5+55,"5");
        }
        else if(number[0][i]==6)
        {
            outtextxy(70*i+37,80*5+55,"6");
        }
    }

}

int get_playchess_frist() //判断先后手
{
    int i;
    printf("red is player, blue is computer.\n");
    printf("red or blue first? input 1 or 2:\n");
    scanf("%d",&i);
    printf("\n");
    if(i==1)
    {
        printf("red frist:\n");
        return 1;
    }
    if(i==2)
    {
        printf("blue frist:\n");
        return -1;
    }
}

int get_playchess_players(int i)  //确定玩家双方
{
    return i;
}


//初始化棋盘
void initBoard(int board[][5])
{
	int i;
	int j;
	for (i = 0; i < 5; i++)
	{
		for (j = 0; j < 5; j++)
		{
			board[i][j] = 0;
		}
	}
	drawBoard(board,number);
	int countClick = 1;
	int x;
	int y;

    fp = fopen("E:\\100.txt","w+");
    fprintf(fp,"#[双方都][地方][先手胜][2018.03.01 16:01 士大夫][萨芬];");
    fprintf(fp,"\n");

    printf("1 : player and computer;\n");
    printf("2 : computer and player;\n");
    printf("3 : computer and computer;\n");
    scanf("%d",&play_side);
    if(play_side==1)
    {
        printf("player and computer:\n");
    }
    if(play_side==2)
    {
        printf("computer and player:\n");
    }
    if(play_side==3)
    {
        printf("computer and computer:\n");
    }

	if(get_playchess_players(play_side)==3)  //两方电脑对战，放置高胜率开局
    {
        defineWinStart(board);
    }

    side = get_playchess_frist();  //确定先后手

	while (true)
	{
		if(get_playchess_players(play_side)==1||get_playchess_players(play_side)==2) //玩家和电脑
        {
            if ((ismouseclick(WM_LBUTTONDOWN)) && countClick < 7 && countClick>0)
            {
                y = mousex();
                x = mousey();
                x = (x + 50) / 80 - 1;
                y = (y + 50) / 80 - 1;
                clearmouseclick(WM_LBUTTONDOWN);
                if((x==0&&y==0)||(x==0&&y==1)||(x==0&&y==2)||(x==1&&y==0)||(x==1&&y==1)||(x==2&&y==0))
                {
                    if(board[x][y]==0)
                    {
                        board[x][y] = countClick;
                        printf("%d ", countClick);
                        countClick++;
                        drawBoard(board,number);
                    }
                }
                else
                {
                    continue;
                }

            }

            if ((ismouseclick(WM_LBUTTONDOWN)) && countClick<0 && countClick>-7)
            {
                y = mousex();
                x = mousey();
                x = (x + 50) / 80 - 1;
                y = (y + 50) / 80-1;
                clearmouseclick(WM_LBUTTONDOWN);
                if((x==2&&y==4)||(x==3&&y==3)||(x==3&&y==4)||(x==4&&y==3)||(x==4&&y==4)||(x==4&&y==2))
                {
                    if(board[x][y]==0)
                    {
                        board[x][y] = countClick;
                        printf("%d ", countClick);
                        if (countClick == -6)
                        {
                            fprintf(fp,"B:E3-%d;",board[2][4]);
                            fprintf(fp,"E2-%d;",board[3][4]);
                            fprintf(fp,"E1-%d;",board[4][4]);
                            fprintf(fp,"D2-%d;",board[3][3]);
                            fprintf(fp,"D1-%d;",board[4][3]);
                            fprintf(fp,"C1-%d",board[4][2]);
                            fprintf(fp,"\n");
                        }
                        countClick--;
                        drawBoard(board,number);
                    }

                }
                else
                {
                    continue;
                }

            }
            if (countClick == 7)
            {
                fprintf(fp,"R:A5-%d;",board[0][0]);
                fprintf(fp,"A4-%d;",board[1][0]);
                fprintf(fp,"A3-%d;",board[2][0]);
                fprintf(fp,"B5-%d;",board[0][1]);
                fprintf(fp,"B4-%d;",board[1][1]);
                fprintf(fp,"C5-%d",board[0][2]);
                fprintf(fp,"\n");

                countClick = -countClick + 6;
            }

            if (countClick == -7)
            {

                if(isWin(board)==-1)
                {
                    MessageBox(NULL,"Blue is winner!","aest",MB_OK);
                    break;
                }
                if(isWin(board)==1)
                {
                    MessageBox(NULL,"Red is winner!","aest",MB_OK);
                    break;
                }
                playChessByMouse();
            }
        }
        if(get_playchess_players(play_side)==3)
        {
            if(countClick==1)
            {
                fprintf(fp,"R:A5-%d;",board[0][0]);
                fprintf(fp,"A4-%d;",board[1][0]);
                fprintf(fp,"A3-%d;",board[2][0]);
                fprintf(fp,"B5-%d;",board[0][1]);
                fprintf(fp,"B4-%d;",board[1][1]);
                fprintf(fp,"C5-%d",board[0][2]);
                fprintf(fp,"\n");

                fprintf(fp,"B:E3-%d;",board[2][4]);
                fprintf(fp,"E2-%d;",board[3][4]);
                fprintf(fp,"E1-%d;",board[4][4]);
                fprintf(fp,"D2-%d;",board[3][3]);
                fprintf(fp,"D1-%d;",board[4][3]);
                fprintf(fp,"C1-%d",board[4][2]);
                fprintf(fp,"\n");
                countClick++;
            }

            if(isWin(board)==-1)
                {
                    MessageBox(NULL,"Blue is winner!","aest",MB_OK);
                    break;
                }
                if(isWin(board)==1)
                {
                    MessageBox(NULL,"Red is winner!","aest",MB_OK);
                    break;
                }
            playChessByMouse();
        }
	}
}

//通过鼠标点击传递骰子的值
int randNum(int number[][6])
{
    int x;
    int y;
    int N = 0;
    while(N==0)
    {
        if (ismouseclick(WM_LBUTTONDOWN))
        {
            y = mousex();
            x = mousey();
            y = (y+50) / 70 ;
            clearmouseclick(WM_LBUTTONDOWN);
            num = number[0][y-1];
            if(x<505&&x>435&&(y>0&&y<7))
            {
                printf("num = %d\n", num);
                N = N+1;
            }
            else
            {
                N = 0;
            }
        }
   }
   return num;

}

//下棋
void playChessByMouse()
{
    if(side==1)
    {
        printf("\nRed play chess:\n");
    }
    if(side==-1)
    {
        printf("\nBlue play chess:\n");
    }
	int countClick = 0;
	int chessKind;
	int x;
	int y;

	if(get_playchess_players(play_side)==1||get_playchess_players(play_side)==2)  //根据双方玩家决定 骰子出场方式
	{
	    num = randNum(number);
	    No++;
	    fprintf(fp,"%d:",No);
	}
	if(get_playchess_players(play_side)==3)
    {
        srand((unsigned)time(NULL));//随机种子
        num = rand()%6+1;//电脑随机骰子
        printf("num = %d\n", num);
        No++;
	    fprintf(fp,"%d:",No);
        system("pause");
    }

    while (1)
	{
	    if(side==1)
        {
            //红方为玩家时，鼠标点击下棋
            if(get_playchess_players(play_side)==1)
            {
                int s;//临时存放棋子坐标
                int t;
                if ((ismouseclick(WM_LBUTTONDOWN)) && (countClick == 0))
                {
                    y = mousex();
                    x = mousey();
                    x = (x + 50) / 80 - 1;
                    y = (y + 50) / 80 - 1;
                    clearmouseclick(WM_LBUTTONDOWN);
                    isRightNumRed(board,num,n);
                    if(board[x][y]==n[0]||board[x][y]==n[1])//确保选择的棋子符合规定
                    {
                        chessKind = board[x][y];
                        s = x;
                        t = y;
                        fprintf(fp,"%d;(R%d,",num,chessKind);
                        printf("%d\n", chessKind);
                        printf("(%d,%d)\n",x,y);
                        countClick++;
                    }
                    else
                    {
                        continue;
                    }
                }
                else  if ((ismouseclick(WM_LBUTTONDOWN)) && (countClick == 1))
                {
                    board[x][y] = 0;
                    y = mousex();
                    x = mousey();
                    x = (x + 50) / 80 - 1;
                    y = (y + 50) / 80 - 1;
                    clearmouseclick(WM_LBUTTONDOWN);
                    if(chessKind>0)
                    {
                        if((x==s+1&&y==t+1)||(x==s&&y==t+1)||(x==s+1&&y==t))//确保下棋位置合理  红棋
                        {
                            printf("(%d,%d)\n",x,y);
                            board[x][y] = chessKind;
                            if(y==0)
                            {
                                fprintf(fp,"A%d)",5-x);
                            }
                            if(y==1)
                            {
                                fprintf(fp,"B%d)",5-x);
                            }
                            if(y==2)
                            {
                                fprintf(fp,"C%d)",5-x);
                            }
                            if(y==3)
                            {
                                fprintf(fp,"D%d)",5-x);
                            }
                            if(y==4)
                            {
                                fprintf(fp,"E%d)",5-x);
                            }
                            fprintf(fp,"\n");
                            drawBoard(board,number);
                            countClick++;
                        }
                        else
                        {
                            continue;
                        }
                    }
                    if(chessKind<0)
                    {
                        if((x==s-1&&y==t-1)||(x==s&&y==t-1)||(x==s-1&&y==t))//蓝棋
                        {
                            printf("(%d,%d)\n",x,y);
                            board[x][y] = chessKind;
                            countClick++;
                            drawBoard(board,number);
                        }
                        else
                        {
                            continue;
                        }
                    }
                    side = -side;
                    break;
                }
            }
            if(get_playchess_players(play_side)==2||get_playchess_players(play_side)==3)  //红方为电脑
            {
                double t = minMaxSearch(30);//极大极小算法
                board[bestPosition.xTo][bestPosition.yTo] = bestPosition.num;
                board[bestPosition.xFrom][bestPosition.yFrom] = 0;
                printf("%d\n",bestPosition.num);
                printf("(%d,%d)\n",bestPosition.xFrom,bestPosition.yFrom);
                printf("(%d,%d)\n",bestPosition.xTo,bestPosition.yTo);

                fprintf(fp,"%d;(R%d,",num,bestPosition.num);

                if(bestPosition.yTo==0)
                {
                    fprintf(fp,"A%d)",5-bestPosition.xTo);
                }
                if(bestPosition.yTo==1)
                {
                    fprintf(fp,"B%d)",5-bestPosition.xTo);
                }
                if(bestPosition.yTo==2)
                {
                    fprintf(fp,"C%d)",5-bestPosition.xTo);
                }
                if(bestPosition.yTo==3)
                {
                    fprintf(fp,"D%d)",5-bestPosition.xTo);
                }
                if(bestPosition.yTo==4)
                {
                    fprintf(fp,"E%d)",5-bestPosition.xTo);
                }
                fprintf(fp,"\n");

                drawBoard(board,number);
                side = -side;
                break;
            }
        }
        if(side==-1)
        {
            if(get_playchess_players(play_side)==1||get_playchess_players(play_side)==3)
            {
                //随机索引下棋
                /*vector<Position> vecMove;
                vecMove.clear();
                vecMove = genMove(board,num,n,side);//得到所有可下位置
                int index;
                index = rand()%vecMove.size();
                x = vecMove[index].xTo;
                y = vecMove[index].yTo;
                board[x][y] = vecMove[index].num;
                board[vecMove[index].xFrom][vecMove[index].yFrom] = 0;
                printf("(%d,%d)\n",vecMove[index].xFrom,vecMove[index].yFrom);
                printf("%2d, (%d,%d)\n",vecMove[index].num,x,y);*/

                //getBestRandPosition();//蒙特卡洛算法
                double k = minMaxSearch(6);//极大极小算法
                //double k = negaMax(3,side);//负极大算法
                //double k = alphaBeta(3,side);
                board[bestPosition.xTo][bestPosition.yTo] = bestPosition.num;
                board[bestPosition.xFrom][bestPosition.yFrom] = 0;
                printf("%d\n",bestPosition.num);
                printf("(%d,%d)\n",bestPosition.xFrom,bestPosition.yFrom);
                printf("(%d,%d)\n",bestPosition.xTo,bestPosition.yTo);

                fprintf(fp,"%d;(B%d,",num,-bestPosition.num);

                if(bestPosition.yTo==0)
                {
                    fprintf(fp,"A%d)",5-bestPosition.xTo);
                }
                if(bestPosition.yTo==1)
                {
                    fprintf(fp,"B%d)",5-bestPosition.xTo);
                }
                if(bestPosition.yTo==2)
                {
                    fprintf(fp,"C%d)",5-bestPosition.xTo);
                }
                if(bestPosition.yTo==3)
                {
                    fprintf(fp,"D%d)",5-bestPosition.xTo);
                }
                if(bestPosition.yTo==4)
                {
                    fprintf(fp,"E%d)",5-bestPosition.xTo);
                }
                fprintf(fp,"\n");

                drawBoard(board,number);
                side = -side;
                break;
            }
            if(get_playchess_players(play_side)==2)
            {
                int s;//临时存放棋子坐标
                int t;
                if ((ismouseclick(WM_LBUTTONDOWN)) && (countClick == 0))
                {
                    y = mousex();
                    x = mousey();
                    x = (x + 50) / 80 - 1;
                    y = (y + 50) / 80 - 1;
                    clearmouseclick(WM_LBUTTONDOWN);
                    isRightNumBlue(board,num,n);
                    if(board[x][y]==n[0]||board[x][y]==n[1])//确保选择的棋子符合规定
                    {
                        chessKind = board[x][y];
                        s = x;
                        t = y;
                        printf("%d\n", chessKind);
                        printf("(%d,%d)\n",x,y);

                        fprintf(fp,"%d;(B%d,",num,-chessKind);

                        countClick++;
                    }
                    else
                    {
                        continue;
                    }
                }
                else  if ((ismouseclick(WM_LBUTTONDOWN)) && (countClick == 1))
                {
                    board[x][y] = 0;
                    y = mousex();
                    x = mousey();
                    x = (x + 50) / 80 - 1;
                    y = (y + 50) / 80 - 1;
                    clearmouseclick(WM_LBUTTONDOWN);
                    if(chessKind>0)
                    {
                        if((x==s+1&&y==t+1)||(x==s&&y==t+1)||(x==s+1&&y==t))//确保下棋位置合理  红棋
                        {
                            printf("(%d,%d)\n",x,y);
                            board[x][y] = chessKind;
                            drawBoard(board,number);
                            countClick++;
                        }
                        else
                        {
                            continue;
                        }
                    }
                    if(chessKind<0)
                    {
                        if((x==s-1&&y==t-1)||(x==s&&y==t-1)||(x==s-1&&y==t))//蓝棋
                        {
                            printf("(%d,%d)\n",x,y);
                            board[x][y] = chessKind;
                            countClick++;
                            drawBoard(board,number);

                            if(y==0)
                            {
                                fprintf(fp,"A%d)",5-x);
                            }
                            if(y==1)
                            {
                                fprintf(fp,"B%d)",5-x);
                            }
                            if(y==2)
                            {
                                fprintf(fp,"C%d)",5-x);
                            }
                            if(y==3)
                            {
                                fprintf(fp,"D%d)",5-x);
                            }
                            if(y==4)
                            {
                                fprintf(fp,"E%d)",5-x);
                            }
                            fprintf(fp,"\n");

                        }
                        else
                        {
                            continue;
                        }
                    }
                    side = -side;
                    break;
                }
            }
        }
	}
	//system("pause");

}

//根据骰子值确定红棋可走的棋子
void isRightNumRed(int board[][5],int num,int n[2])
{
    int i;
    int j;
    int min1 = 6;
    int min2 = 6;
    int a;
    int b;
    for(i=0;i<5;i++)
    {
        for(j=0;j<5;j++)
        {
            if(board[i][j]<=num)//利用做差找到值最近的棋子
            {
                a = num - board[i][j];
                if(a<min1)
                {
                    min1 = a;
                    n[0] = board[i][j];
                }
            }
            if(board[i][j]>num)
            {
                b = board[i][j] - num;
                if(b<min2)
                {
                    min2 = b;
                    n[1] = board[i][j];
                }
            }
        }
    }
    //骰子值与棋子值相等
    if(min1==0)
    {
        n[1] = n[0];
    }
    //边界情况
    if(n[1]==0)
    {
        n[1] = n[0];
    }
    if(n[0]==0)
    {
        n[0] = n[1];
    }

}

//根据骰子值确定蓝棋可走的棋子
void isRightNumBlue(int board[][5],int num,int n[2])
{
    int i;
    int j;
    int min1 = 6;
    int min2 = 6;
    int a;
    int b;
    for(i=0;i<5;i++)
    {
        for(j=0;j<5;j++)
        {
            if((-board[i][j])<=num)
            {
                a = num + board[i][j];
                if(a<min1)
                {
                    min1 = a;
                    n[0] = board[i][j];
                }
            }
            if((-board[i][j])>num)
            {
                b = (-board[i][j]) - num;
                if(b<min2)
                {
                    min2 = b;
                    n[1] = board[i][j];
                }
            }
        }
    }
    if(min1==0)
    {
        n[1] = n[0];
    }
    if(n[1]==0)
    {
        n[1] = n[0];
    }
    if(n[0]==0)
    {
        n[0] = n[1];
    }

}

//生成所有可下位置的向量
vector<Position> genMove(int board[][5],int num,int n[],int side)
{
    struct Position move;
    vector<Position> vecMove;
    int i;
    int j;
    if(side==1)
    {
        isRightNumRed(board,num,n);
        for(i=0;i<5;i++)
        {
            for(j=0;j<5;j++)
            {
                if(board[i][j]==n[0])//对第一个可走的棋
                {
                    move.xFrom = i;
                    move.yFrom = j;
                    move.num = n[0];

                    vecMove.clear();

                    if(move.xFrom!=4&&move.yFrom!=4)
                    {
                        move.xTo = move.xFrom+1;
                        move.yTo = move.yFrom+1;
                        move.num = n[0];
                        vecMove.push_back(move);
                    }

                    if(move.xFrom!=4)
                    {
                        move.xTo = move.xFrom+1;
                        move.yTo = move.yFrom;
                        move.num = n[0];
                        vecMove.push_back(move);
                    }

                    if(move.yFrom!=4)
                    {
                        move.xTo = move.xFrom;
                        move.yTo = move.yFrom+1;
                        move.num = n[0];
                        vecMove.push_back(move);
                    }

                }
                if(board[i][j]==n[1]&&n[0]!=n[1])//对第二个可走的棋
                {
                    move.xFrom = i;
                    move.yFrom = j;
                    move.num = n[1];

                    if(move.xFrom!=4&&move.yFrom!=4)
                    {
                        move.xTo = move.xFrom+1;
                        move.yTo = move.yFrom+1;
                        move.num = n[1];
                        vecMove.push_back(move);
                    }

                    if(move.xFrom!=4)
                    {
                        move.xTo = move.xFrom+1;
                        move.yTo = move.yFrom;
                        move.num = n[1];
                        vecMove.push_back(move);
                    }

                    if(move.yFrom!=4)
                    {
                        move.xTo = move.xFrom;
                        move.yTo = move.yFrom+1;
                        move.num = n[1];
                        vecMove.push_back(move);
                    }

                }
            }
        }
    }

    if(side==-1)
    {
        isRightNumBlue(board,num,n);
        for(i=0;i<5;i++)
        {
            for(j=0;j<5;j++)
            {
                if(board[i][j]==n[0])
                {
                    move.xFrom = i;
                    move.yFrom = j;
                    move.num = n[0];

                    vecMove.clear();

                    if(move.xFrom!=0&&move.yFrom!=0)
                    {
                        move.xTo = move.xFrom-1;
                        move.yTo = move.yFrom-1;
                        move.num = n[0];
                        vecMove.push_back(move);
                    }

                    if(move.xFrom!=0)
                    {
                        move.xTo = move.xFrom-1;
                        move.yTo = move.yFrom;
                        move.num = n[0];
                        vecMove.push_back(move);
                    }

                    if(move.yFrom!=0)
                    {
                        move.xTo = move.xFrom;
                        move.yTo = move.yFrom-1;
                        move.num = n[0];
                        vecMove.push_back(move);
                    }

                }
                if(board[i][j]==n[1]&&n[0]!=n[1])
                {
                    move.xFrom = i;
                    move.yFrom = j;
                    move.num = n[1];

                    if(move.xFrom!=0&&move.yFrom!=0)
                    {
                        move.xTo = move.xFrom-1;
                        move.yTo = move.yFrom-1;
                        move.num = n[1];
                        vecMove.push_back(move);
                    }

                    if(move.xFrom!=0)
                    {
                        move.xTo = move.xFrom-1;
                        move.yTo = move.yFrom;
                        move.num = n[1];
                        vecMove.push_back(move);
                    }

                    if(move.yFrom!=0)
                    {
                        move.xTo = move.xFrom;
                        move.yTo = move.yFrom-1;
                        move.num = n[1];
                        vecMove.push_back(move);
                    }

                }
            }
        }
    }
    return vecMove;
}



//判断输赢
int isWin(int board[][5])
{
    int i;
    int j;
    int red = 0;
    int blue = 0;
    for(i=0;i<5;i++)
    {
        for(j=0;j<5;j++)
        {
            if(board[0][0]<0)//蓝棋到达对方角点
            {
                return -1;
            }
            if(board[i][j]<0)
            {
                blue = blue+1;
            }
            if(board[i][j]>0)
            {
                red = red+1;
            }
            if(board[4][4]>0)//红棋到达对方角点
            {
                return 1;
            }
        }
    }
    if(red==0)//棋盘上无红棋
    {
        return -1;
    }
    if(blue==0)//棋盘上无蓝棋
    {
        return 1;
    }
}

//选择一组胜率较高的开局来进行随机模拟下棋
void defineWinStart(int board[5][5])
{
    board[0][0] = 6;
    board[0][1] = 2;
    board[0][2] = 4;
    board[0][3] = 0;
    board[0][4] = 0;

    board[1][0] = 1;
    board[1][1] = 5;
    board[1][2] = 0;
    board[1][3] = 0;
    board[1][4] = 0;

    board[2][0] = 3;
    board[2][1] = 0;
    board[2][2] = 0;
    board[2][3] = 0;
    board[2][4] = -3;

    board[3][0] = 0;
    board[3][1] = 0;
    board[3][2] = 0;
    board[3][3] = -5;
    board[3][4] = -1;

    board[4][0] = 0;
    board[4][1] = 0;
    board[4][2] = -4;
    board[4][3] = -2;
    board[4][4] = -6;
}

//生成用于随机下棋的下棋位置
vector<Position> genMoveForProb(int board[][5],int num,int n[],int side)
{
    int i;
    int j;
    Position posCanMove;
    vector<Position> vecPosCanMoveForProb;
    if(side==1)
    {
        isRightNumRed(board,num,n);
        for(i=0;i<5;i++)
        {
            for(j=0;j<5;j++)
            {
                if(board[i][j]==n[0])//对第一个可走的棋
                {
                    posCanMove.xFrom = i;
                    posCanMove.yFrom = j;
                    posCanMove.num = n[0];

                    vecPosCanMoveForProb.clear();

                    if(posCanMove.xFrom!=4)
                    {
                        posCanMove.xTo = posCanMove.xFrom+1;
                        posCanMove.yTo = posCanMove.yFrom;
                        posCanMove.num = n[0];
                        posCanMove.total = 0;
                        posCanMove.win = 0;
                        posCanMove.prob = 0;
                        vecPosCanMoveForProb.push_back(posCanMove);
                    }

                    if(posCanMove.yFrom!=4)
                    {
                        posCanMove.xTo = posCanMove.xFrom;
                        posCanMove.yTo = posCanMove.yFrom+1;
                        posCanMove.num = n[0];
                        posCanMove.total = 0;
                        posCanMove.win = 0;
                        posCanMove.prob = 0;
                        vecPosCanMoveForProb.push_back(posCanMove);
                    }

                    if(posCanMove.xFrom!=4&&posCanMove.yFrom!=4)
                    {
                        posCanMove.xTo = posCanMove.xFrom+1;
                        posCanMove.yTo = posCanMove.yFrom+1;
                        posCanMove.num = n[0];
                        posCanMove.total = 0;
                        posCanMove.win = 0;
                        posCanMove.prob = 0;
                        vecPosCanMoveForProb.push_back(posCanMove);
                    }
                }
                if(board[i][j]==n[1]&&n[0]!=n[1])//对第二个可走的棋
                {
                    posCanMove.xFrom = i;
                    posCanMove.yFrom = j;
                    posCanMove.num = n[1];

                    if(posCanMove.xFrom!=4)
                    {
                        posCanMove.xTo = posCanMove.xFrom+1;
                        posCanMove.yTo = posCanMove.yFrom;
                        posCanMove.num = n[1];
                        posCanMove.total = 0;
                        posCanMove.win = 0;
                        posCanMove.prob = 0;
                        vecPosCanMoveForProb.push_back(posCanMove);
                    }

                    if(posCanMove.yFrom!=4)
                    {
                        posCanMove.xTo = posCanMove.xFrom;
                        posCanMove.yTo = posCanMove.yFrom+1;
                        posCanMove.num = n[1];
                        posCanMove.total = 0;
                        posCanMove.win = 0;
                        posCanMove.prob = 0;
                        vecPosCanMoveForProb.push_back(posCanMove);
                    }

                    if(posCanMove.xFrom!=4&&posCanMove.yFrom!=4)
                    {
                        posCanMove.xTo = posCanMove.xFrom+1;
                        posCanMove.yTo = posCanMove.yFrom+1;
                        posCanMove.num = n[1];
                        posCanMove.total = 0;
                        posCanMove.win = 0;
                        posCanMove.prob = 0;
                        vecPosCanMoveForProb.push_back(posCanMove);
                    }
                }
            }
        }
    }

    if(side==-1)
    {
        isRightNumBlue(board,num,n);
        for(i=0;i<5;i++)
        {
            for(j=0;j<5;j++)
            {
                if(board[i][j]==n[0])
                {
                    posCanMove.xFrom = i;
                    posCanMove.yFrom = j;
                    posCanMove.num = n[0];

                    vecPosCanMoveForProb.clear();

                    if(posCanMove.xFrom!=0)
                    {
                        posCanMove.xTo = posCanMove.xFrom-1;
                        posCanMove.yTo = posCanMove.yFrom;
                        posCanMove.num = n[0];
                        posCanMove.total = 0;
                        posCanMove.win = 0;
                        posCanMove.prob = 0;
                        vecPosCanMoveForProb.push_back(posCanMove);
                    }

                    if(posCanMove.yFrom!=0)
                    {
                        posCanMove.xTo = posCanMove.xFrom;
                        posCanMove.yTo = posCanMove.yFrom-1;
                        posCanMove.num = n[0];
                        posCanMove.total = 0;
                        posCanMove.win = 0;
                        posCanMove.prob = 0;
                        vecPosCanMoveForProb.push_back(posCanMove);
                    }

                    if(posCanMove.xFrom!=0&&posCanMove.yFrom!=0)
                    {
                        posCanMove.xTo = posCanMove.xFrom-1;
                        posCanMove.yTo = posCanMove.yFrom-1;
                        posCanMove.num = n[0];
                        posCanMove.total = 0;
                        posCanMove.win = 0;
                        posCanMove.prob = 0;
                        vecPosCanMoveForProb.push_back(posCanMove);
                    }
                }
                if(board[i][j]==n[1]&&n[0]!=n[1])
                {
                    posCanMove.xFrom = i;
                    posCanMove.yFrom = j;
                    posCanMove.num = n[1];

                    if(posCanMove.xFrom!=0)
                    {
                        posCanMove.xTo = posCanMove.xFrom-1;
                        posCanMove.yTo = posCanMove.yFrom;
                        posCanMove.num = n[1];
                        posCanMove.total = 0;
                        posCanMove.win = 0;
                        posCanMove.prob = 0;
                        vecPosCanMoveForProb.push_back(posCanMove);
                    }

                    if(posCanMove.yFrom!=0)
                    {
                        posCanMove.xTo = posCanMove.xFrom;
                        posCanMove.yTo = posCanMove.yFrom-1;
                        posCanMove.num = n[1];
                        posCanMove.total = 0;
                        posCanMove.win = 0;
                        posCanMove.prob = 0;
                        vecPosCanMoveForProb.push_back(posCanMove);
                    }

                    if(posCanMove.xFrom!=0&&posCanMove.yFrom!=0)
                    {
                        posCanMove.xTo = posCanMove.xFrom-1;
                        posCanMove.yTo = posCanMove.yFrom-1;
                        posCanMove.num = n[1];
                        posCanMove.total = 0;
                        posCanMove.win = 0;
                        posCanMove.prob = 0;
                        vecPosCanMoveForProb.push_back(posCanMove);
                    }

                }
            }
        }
    }
    return vecPosCanMoveForProb;
}

//随机下棋
int randomPlay(int board[][5],int num)
{
    vector<Position> vecPos;
    int tempSide = side;
    int tempBoard[5][5];
    int index;//随机选择的位置
    int i;
    int j;
    for(i=0;i<5;i++)
    {
        for(j=0;j<5;j++)
        {
            tempBoard[i][j] = board[i][j];
        }
    }
    //drawBoard(tempBoard,number);
    while(1)
    {
        //得到骰子值
        srand(time(NULL));
        num = rand()%6+1;//电脑随机骰子
        //根据骰子值确定可走的值
        if(tempSide==1)
        {
            isRightNumRed(tempBoard,num,n);
        }
        if(tempSide==-1)
        {
            isRightNumBlue(tempBoard,num,n);
        }
        //生成所有可下位置
        vecPos.clear();
        vecPos = genMove(tempBoard,num,n,tempSide);
        if(vecPos.size()==0)
        {
            return 0;
        }
        index = rand()%vecPos.size();
        tempBoard[vecPos[index].xTo][vecPos[index].yTo] = vecPos[index].num;
        tempBoard[vecPos[index].xFrom][vecPos[index].yFrom] = 0;
        index = rand()%vecPos.size();

        if(isWin(tempBoard)==1)
        {
            return 1;
        }
        if(isWin(tempBoard)==-1)
        {
            return -1;
        }
        if(isWin(tempBoard)==0)
        {
            return 0;
        }
        tempSide = -tempSide;
        //drawBoard(tempBoard,number);
        //system("pause");
    }
}

//获取随机下棋的最佳位置
void getBestRandPosition()
{
    int i;
    int j;
    //得到骰子值
    //num = randNum(number);//鼠标点击获得骰子的值
    //num = rand()%6+1;//电脑随机骰子
    //根据骰子值确定可走的值
    if(side==1)
    {
        isRightNumRed(board,num,n);
    }
    if(side==-1)
    {
        isRightNumBlue(board,num,n);
    }
    vector<Position> vecPosPro = genMoveForProb(board,num,n,side);
    //对每一个位置进行随机下棋
    for(i=0;i<(int)vecPosPro.size();i++)
    {
        int temp1 = board[vecPosPro[i].xFrom][vecPosPro[i].yFrom];
        int temp2 = board[vecPosPro[i].xTo][vecPosPro[i].yTo];
        board[vecPosPro[i].xTo][vecPosPro[i].yTo] = vecPosPro[i].num;   //下棋位置
        board[vecPosPro[i].xFrom][vecPosPro[i].yFrom] = 0;
        for(j=0;j<100000;j++)//每个位置下模拟若干盘
        {
            vecPosPro[i].total++;
            vecPosPro[i].win += randomPlay(board,num);
            vecPosPro[i].prob = (double)vecPosPro[i].win/vecPosPro[i].total;
        }
        board[vecPosPro[i].xFrom][vecPosPro[i].yFrom] = temp1;//还原棋盘
        board[vecPosPro[i].xTo][vecPosPro[i].yTo] = temp2;
    }
    //获得最佳下棋位置
    double bestProb = vecPosPro[0].prob;
    int bestIndex = 0;
    for(i=0;i<(int)vecPosPro.size();i++)
    {
        if(vecPosPro[i].prob>bestProb)
        {
            bestProb = vecPosPro[i].prob;
            bestIndex = i;
        }
    }
    bestPosition.xFrom = vecPosPro[bestIndex].xFrom;
    bestPosition.yFrom = vecPosPro[bestIndex].yFrom;
    bestPosition.xTo = vecPosPro[bestIndex].xTo;
    bestPosition.yTo = vecPosPro[bestIndex].yTo;
    bestPosition.num = vecPosPro[bestIndex].num;
}

//获得由估值函数求出的最大值的最佳位置
double Value(int board[][5],int side)
{
    double value;
    if(side==1)
    {
        if(isWin(board)==1)
        {
            return 1000;
        }
        else
        {
            value = distanceValue(board,side)+2*eatValue(board,side);
            return value;
        }
    }
    if(side==-1)
    {
        if(isWin(board)==-1)
        {
            return 1000;
        }
        else
        {
            value = distanceValue(board,side)+2*eatValue(board,side);
            return value;
        }
    }
}


//计算到角点距离的值
double distanceValue(int board[][5],int side)
{
    int range = 0;
    int i;
    int j;
    int count = 0;
    double countValue = 0;
    int tempBoard[5][5];
    for(i=0;i<5;i++)
    {
        for(j=0;j<5;j++)
        {
            tempBoard[i][j] = board[i][j];
        }
    }
    if(side==1)  //红棋的距离计算
    {
        for(i=0;i<5;i++)
        {
            for(j=0;j<5;j++)
            {
                if(tempBoard[i][j]>0)
                {
                    range = range+max((4-i),(4-j));  //计算棋子距离角点的位置
                    count = count+1;  //记录棋盘上剩余的棋子个数
                }
            }
        }
        getCountForValue(tempBoard,side,c);
        for(i=0;i<6;i++)
        {
            countValue = countValue+c[i];
        }
        countValue = countValue/count;
    }
    if(side==-1)
    {
        for(i=0;i<5;i++)
        {
            for(j=0;j<5;j++)
            {
                if(tempBoard[i][j]<0)
                {
                    range = range+max(i,j);  //计算棋子距离角点的位置
                    count = count+1;  //记录棋盘上剩余的棋子个数
                }
            }
        }
        getCountForValue(tempBoard,side,c);
        for(i=0;i<6;i++)
        {
            countValue = countValue+c[i];
        }
        countValue = countValue/count;
    }
    return (double)range*countValue;
}

//计算当前下棋方吃子的值
double eatValue(int board[][5],int side)
{
    double value = 0;
    int i;
    int j;
    int tempBoard[5][5];
    int countBlue = 0;
    int countRed = 0;
    for(i=0;i<5;i++)
    {
        for(j=0;j<5;j++)
        {
            tempBoard[i][j] = board[i][j];
            if(tempBoard[i][j]>0)
            {
                countRed = countRed+1;
            }
            if(tempBoard[i][j]<0)
            {
                countBlue = countBlue+1;
            }
        }
    }
    if(side==1)
    {
        for(i=0;i<5;i++)
        {
            for(j=0;j<5;j++)
            {
                if(tempBoard[i][j]>0)
                {
                    if(tempBoard[i+1][j+1]<0)//对方对己方产生威胁
                    {
                        value = value+1;
                    }
                    if(tempBoard[i+1][j]<0)
                    {
                        value = value+1;
                    }
                    if(tempBoard[i][j+1]<0)
                    {
                        value = value+1;
                    }
                }
            }
        }
        value = 3*(countRed-countBlue)-value;
    }

    if(side==-1)
    {
        for(i=0;i<5;i++)
        {
            for(j=0;j<5;j++)
            {
                if(tempBoard[i][j]<0)
                {
                    if(tempBoard[i-1][j-1]>0)
                    {
                        value = value+1;
                    }
                    if(tempBoard[i-1][j]>0)
                    {
                        value = value+1;
                    }
                    if(tempBoard[i][j-1]>0)
                    {
                        value = value+1;
                    }
                }
            }
        }
        value = 3*(countBlue-countRed)-value;
    }
    return value;
}

//获得每个棋子的概率次数
void getCountForValue(int board[][5],int side,int c[6])
{
    int i;
    int j;
    int minn = 6;
    int t = 0;
    int t1 = 0;//左边量容器
    int minn1 = 6;
    int s1 = 0;
    int s2 = 0;
    int s3 = 0;
    int s4 = 0;
    int s5 = 0;
    int s6 = 0;
    for(i=0;i<6;i++)
    {
        c[i] = 0;
    }
    if(side==1)
    {
        //存在即有可能
        for(i=0;i<5;i++)
        {
            for(j=0;j<5;j++)
            {
                if(board[i][j]>0)
                {
                    c[board[i][j]-1]++;
                }
            }
        }
        for(i=0;i<5;i++)
        {
            for(j=0;j<5;j++)
            {
                if(board[i][j]==1)
                {
                    s1 = 1;
                }
            }
        }
        if(s1==0)
        {
            for(i=0;i<5;i++)
            {
                for(j=0;j<5;j++)
                {
                    if(board[i][j]>0&&(board[i][j]-1)<minn)
                    {
                        minn = board[i][j]-1;
                        t = board[i][j];
                    }
                }
            }
            c[t-1] = c[t-1]+1;
            minn = 6;
        }
        for(i=0;i<5;i++)
        {
            for(j=0;j<5;j++)
            {
                if(board[i][j]==2)
                {
                    s2 = 1;
                }
            }
        }
        if(s2==0)
        {
            for(i=0;i<5;i++)
            {
                for(j=0;j<5;j++)
                {
                    if(board[i][j]==1)
                    {
                        c[0] = c[0]+1;
                    }
                    if(board[i][j]>2&&(board[i][j]-2)<minn)
                    {
                        minn = board[i][j]-2;
                        t = board[i][j];
                    }
                }
            }
            c[t-1] = c[t-1]+1;
            minn = 6;
        }
        for(i=0;i<5;i++)
        {
            for(j=0;j<5;j++)
            {
                if(board[i][j]==3)
                {
                    s3 = 1;
                }
            }
        }
        if(s3==0)
        {
            for(i=0;i<5;i++)
            {
                for(j<0;j<5;j++)
                {
                    if(board[i][j]<3&&board[i][j]>0)
                    {
                        if((3-board[i][j])<minn1)
                        {
                            minn1 = 3-board[i][j];
                            t1 = board[i][j];
                        }
                    }
                    if(board[i][j]>3)
                    {
                        if((board[i][j]-3)<minn)
                        {
                            minn = board[i][j]-3;
                            t = board[i][j];
                        }
                    }
                }
            }
            c[t1-1]++;
            c[t-1]++;
            minn = 6;
            minn1 = 6;
        }
        for(i=0;i<5;i++)
        {
            for(j=0;j<5;j++)
            {
                if(board[i][j]==4)
                {
                    s4 = 1;
                }
            }
        }
        if(s4==0)
        {
            for(i=0;i<5;i++)
            {
                for(j=0;j<5;j++)
                {
                    if(board[i][j]<4&&board[i][j]>0)
                    {
                        if((4-board[i][j])<minn1)
                        {
                            minn1 = 4-board[i][j];
                            t1 = board[i][j];
                        }
                    }
                    if(board[i][j]>4)
                    {
                        if((board[i][j]-4)<minn)
                        {
                            minn = board[i][j]-4;
                            t = board[i][j];
                        }
                    }
                }
            }
            c[t1-1]++;
            c[t-1]++;
            minn = 6;
            minn1 =6;
        }
        for(i=0;i<5;i++)
        {
            for(j=0;j<5;j++)
            {
                if(board[i][j]==5)
                {
                    s5 = 1;
                }
            }
        }
        if(s5==0)
        {
            for(i=0;i<5;i++)
            {
                for(j=0;j<5;j++)
                {
                    if(board[i][j]==6)
                    {
                        c[5]++;
                    }
                    if(board[i][j]<5&&board[i][j]>0)
                    {
                        if((5-board[i][j])<minn1)
                        {
                            minn1 = 5-board[i][j];
                            t1 = board[i][j];
                        }
                    }
                }
            }
            c[t1-1]++;
            minn1 = 6;
        }
        for(i=0;i<5;i++)
        {
            for(j=0;j<5;j++)
            {
                if(board[i][j]==6)
                {
                    s6 = 1;
                }
            }
        }
        if(s6==0)
        {
            for(i=0;i<5;i++)
            {
                for(j=0;j<5;j++)
                {
                    if((6-board[i][j])<minn1)
                    {
                        minn1 = 6-board[i][j];
                        t1 = board[i][j];
                    }
                }
            }
            c[t1-1]++;
            minn1 = 6;
        }
    }

    if(side==-1)
    {
        //存在即有可能
        for(i=0;i<5;i++)
        {
            for(j=0;j<5;j++)
            {
                if(board[i][j]<0)
                {
                    c[(-board[i][j])-1]++;
                }
            }
        }
        for(i=0;i<5;i++)
        {
            for(j=0;j<5;j++)
            {
                if(board[i][j]==-1)
                {
                    s1 = 1;
                }
            }
        }
        if(s1==0)
        {
            for(i=0;i<5;i++)
            {
                for(j=0;j<5;j++)
                {
                    if(board[i][j]<0&&((-board[i][j])-1)<minn)
                    {
                        minn = (-board[i][j])-1;
                        t = -board[i][j];
                    }
                }
            }
            c[t-1] = c[t-1]+1;
            minn = 6;
        }
        for(i=0;i<5;i++)
        {
            for(j=0;j<5;j++)
            {
                if(-board[i][j]==2)
                {
                    s2 = 1;
                }
            }
        }
        if(s2==0)
        {
            for(i=0;i<5;i++)
            {
                for(j=0;j<5;j++)
                {
                    if(-board[i][j]==1)
                    {
                        c[0] = c[0]+1;
                    }
                    if(((-board[i][j])>2)&&((-board[i][j])-2)<minn)
                    {
                        minn = -board[i][j]-2;
                        t = -board[i][j];
                    }
                }
            }
            c[t-1] = c[t-1]+1;
            minn = 6;
        }
        for(i=0;i<5;i++)
        {
            for(j=0;j<5;j++)
            {
                if(-board[i][j]==3)
                {
                    s3 = 1;
                }
            }
        }
        if(s3==0)
        {
            for(i=0;i<5;i++)
            {
                for(j<0;j<5;j++)
                {
                    if(-board[i][j]<3&&-board[i][j]>0)
                    {
                        if((3+board[i][j])<minn1)
                        {
                            minn1 = 3+board[i][j];
                            t1 = board[i][j];
                        }
                    }
                    if(-board[i][j]>3)
                    {
                        if((-board[i][j]-3)<minn)
                        {
                            minn = board[i][j]-3;
                            t = -board[i][j];
                        }
                    }
                }
            }
            c[t1-1]++;
            c[t-1]++;
            minn = 6;
            minn1 = 6;
        }
        for(i=0;i<5;i++)
        {
            for(j=0;j<5;j++)
            {
                if(-board[i][j]==4)
                {
                    s4 = 1;
                }
            }
        }
        if(s4==0)
        {
            for(i=0;i<5;i++)
            {
                for(j=0;j<5;j++)
                {
                    if(((-board[i][j])<4)&&(-board[i][j]>0))
                    {
                        if((4+board[i][j])<minn1)
                        {
                            minn1 = 4+board[i][j];
                            t1 = -board[i][j];
                        }
                    }
                    if(-board[i][j]>4)
                    {
                        if((-board[i][j]-4)<minn)
                        {
                            minn = -board[i][j]-4;
                            t = -board[i][j];
                        }
                    }
                }
            }
            c[t1-1]++;
            c[t-1]++;
        }
        for(i=0;i<5;i++)
        {
            for(j=0;j<5;j++)
            {
                if(-board[i][j]==5)
                {
                    s5 = 1;
                }
            }
        }
        if(s5==0)
        {
            for(i=0;i<5;i++)
            {
                for(j=0;j<5;j++)
                {
                    if(-board[i][j]==6)
                    {
                        c[5]++;
                    }
                    if(-board[i][j]<5&&-board[i][j]>0)
                    {
                        if((5+board[i][j])<minn1)
                        {
                            minn1 = 5+board[i][j];
                            t1 = -board[i][j];
                        }
                    }
                }
            }
            c[t1-1]++;
        }
        for(i=0;i<5;i++)
        {
            for(j=0;j<5;j++)
            {
                if(-board[i][j]==6)
                {
                    s6 = 1;
                }
            }
        }
        if(s6==0)
        {
            for(i=0;i<5;i++)
            {
                for(j=0;j<5;j++)
                {
                    if((6+board[i][j])<minn1)
                    {
                        minn1 = 6+board[i][j];
                        t1 = -board[i][j];
                    }
                }
            }
            c[t-1]++;
        }
    }

}

//极大极小算法
double minMaxSearch(int depth)
{
    double best = -1000;
    double val;
    vector<Position> vecMove;
    vector<Position> vecPos = genMove(board,num,n,side);
    int i;
    if(depth<=0)
    {
        return Value(board,side);
    }
    if(side==1)
    {
        if(isWin(board)==1)
        {
            return Value(board,side);
        }
    }
    if(side==-1)
    {
        if(isWin(board)==-1)
        {
            return Value(board,side);
        }
    }
    for(i=0;i<(int)vecPos.size();i++)
    {
        int temp1 = board[vecPos[i].xFrom][vecPos[i].yFrom];
        int temp2 = board[vecPos[i].xTo][vecPos[i].yTo];
        board[vecPos[i].xTo][vecPos[i].yTo] = vecPos[i].num;
        board[vecPos[i].xFrom][vecPos[i].yFrom] = 0;

        val = minSearch(depth-1);

        if(val>best)
        {
            best = val;
            bestPosition.xFrom = vecPos[i].xFrom;
            bestPosition.yFrom = vecPos[i].yFrom;
            bestPosition.xTo = vecPos[i].xTo;
            bestPosition.yTo = vecPos[i].yTo;
            bestPosition.num = vecPos[i].num;
        }

        board[vecPos[i].xFrom][vecPos[i].yFrom] = temp1;
        board[vecPos[i].xTo][vecPos[i].yTo] = temp2;
    }
    return best;
}

//极大算法
double maxSearch(int depth)
{
    srand(time(NULL));
    num = rand()%6+1;
    double best = -1000;
    vector<Position> vecMove;
    vector<Position> vecPos = genMove(board,num,n,side);
    int i;
    if(depth <= 0)
    {
        return Value(board,side);
    }
    if(side==1)
    {
        if(isWin(board)==1)
        {
            return Value(board,side);
        }
    }
    if(side==-1)
    {
        if(isWin(board)==-1)
        {
            return Value(board,side);
        }
    }
    double val;
    for(i=0;i<(int)vecPos.size();i++)
    {
        int temp1 = board[vecPos[i].xFrom][vecPos[i].yFrom];
        int temp2 = board[vecPos[i].xTo][vecPos[i].yTo];
        board[vecPos[i].xTo][vecPos[i].yTo] = vecPos[i].num;
        board[vecPos[i].xFrom][vecPos[i].yFrom] = 0;

        val = minSearch(depth-1);

        if(val>best)
        {
            best = val;
        }

        board[vecPos[i].xFrom][vecPos[i].yFrom] = temp1;
        board[vecPos[i].xTo][vecPos[i].yTo] = temp2;
    }
    return best;
}

//极小算法
double minSearch(int depth)
{
    double best = 1000;
    int i;
    vector<Position> vecMove;

    srand(time(NULL));
    num = rand()%6+1;

    vector<Position> vecPos = genMove(board,num,n,side);
    if(depth <= 0)
    {
        return Value(board,side);
    }
    if(side==1)
    {
        if(isWin(board)==1)
        {
            return Value(board,side);
        }
    }
    if(side==-1)
    {
        if(isWin(board)==-1)
        {
            return Value(board,side);
        }
    }
    double val;
    for(i=0;i<(int)vecPos.size();i++)
    {
        int temp1 = board[vecPos[i].xFrom][vecPos[i].yFrom];
        int temp2 = board[vecPos[i].xTo][vecPos[i].yTo];
        board[vecPos[i].xTo][vecPos[i].yTo] = vecPos[i].num;
        board[vecPos[i].xFrom][vecPos[i].yFrom] = 0;

        val = maxSearch(depth-1);

        if(val<best)
        {
            best = val;
        }
        board[vecPos[i].xFrom][vecPos[i].yFrom] = temp1;
        board[vecPos[i].xTo][vecPos[i].yTo] = temp2;

    }
    return best;
}

//负极大极小
double negaMax(int depth,int side)
{
    double val;
    int i;
    double best = -1000;
    if(depth<=0)
    {
        return Value(board,side);
    }
    if(side==1)
    {
        if(isWin(board)==1)
        {
            return Value(board,side);
        }
    }
    if(side==-1)
    {
        if(isWin(board)==-1)
        {
            return Value(board,side);
        }
    }
    vector<Position> vecPos;
    vecPos = genMove(board,num,n,side);
    if(vecPos.size()==0)
    {
        return 0;
    }
    for(i=0;i<int(vecPos.size());i++)
    {
        int temp;
        temp = board[vecPos[i].xTo][vecPos[i].yTo];
        board[vecPos[i].xTo][vecPos[i].yTo] = vecPos[i].num;
        board[vecPos[i].xFrom][vecPos[i].yFrom] = 0;
        srand(time(NULL));
        num = rand()%6+1;
        val = -negaMax(depth-1,side);
        if(val>=best)
        {
            best = val;
            if(depth==3)//根据搜索深度的大小而改变
            {
                bestPosition.xFrom = vecPos[i].xFrom;
                bestPosition.yFrom = vecPos[i].yFrom;
                bestPosition.xTo = vecPos[i].xTo;
                bestPosition.yTo = vecPos[i].yTo;
                bestPosition.num = vecPos[i].num;
            }
        }
        board[vecPos[i].xFrom][vecPos[i].yFrom] = vecPos[i].num;
        board[vecPos[i].xTo][vecPos[i].yTo] = temp;
    }
    return best;

}

//alpha剪枝
double Alpha(int depth,int side)
{
    int i;
    double beta = -1000;
    double alpha;
    if(depth<=0)
    {
        return Value(board,side);
    }
    if(side==1)
    {
        if(isWin(board)==1)
        {
            return Value(board,side);
        }
    }
    if(side==-1)
    {
        if(isWin(board)==-1)
        {
            return Value(board,side);
        }
    }
    srand(time(NULL));
    num = rand()%6+1;
    vector<Position> vecPos;
    vecPos = genMove(board,num,n,side);
    for(i=0;i<(int)vecPos.size();i++)
    {
        int temp1 = board[vecPos[i].xFrom][vecPos[i].yFrom];
        int temp2 = board[vecPos[i].xTo][vecPos[i].yTo];
        board[vecPos[i].xTo][vecPos[i].yTo] = vecPos[i].num;
        board[vecPos[i].xFrom][vecPos[i].yFrom] = 0;

        alpha = Beta(depth-1,side);

        if(alpha>=beta)
        {
            beta = alpha;
        }

        board[vecPos[i].xFrom][vecPos[i].yFrom] = temp1;
        board[vecPos[i].xTo][vecPos[i].yTo] = temp2;
    }
    return beta;
}

//beta剪枝
double Beta(int depth,int side)
{
    int i;
    double beta;
    double alpha = -1000;
    if(depth<=0)
    {
        return Value(board,side);
    }
    if(side==1)
    {
        if(isWin(board)==1)
        {
            return Value(board,side);
        }
    }
    if(side==-1)
    {
        if(isWin(board)==-1)
        {
            return Value(board,side);
        }
    }
    srand(time(NULL));
    num = rand()%6+1;
    vector<Position> vecPos;
    vecPos = genMove(board,num,n,side);
    for(i=0;i<(int)vecPos.size();i++)
    {
        int temp1 = board[vecPos[i].xFrom][vecPos[i].yFrom];
        int temp2 = board[vecPos[i].xTo][vecPos[i].yTo];
        board[vecPos[i].xTo][vecPos[i].yTo] = vecPos[i].num;
        board[vecPos[i].xFrom][vecPos[i].yFrom] = 0;

        beta = Alpha(depth-1,side);

        if(alpha<=beta)
        {
            alpha = beta;
        }

        board[vecPos[i].xFrom][vecPos[i].yFrom] = temp1;
        board[vecPos[i].xTo][vecPos[i].yTo] = temp2;
    }
    return alpha;
}

//Alpha-Beta算法
double alphaBeta(int depth,int side)
{
    if(depth<=0)
    {
        return Value(board,side);
    }
    if(side==1)
    {
        if(isWin(board)==1)
        {
            return Value(board,side);
        }
    }
    if(side==-1)
    {
        if(isWin(board)==-1)
        {
            return Value(board,side);
        }
    }
    vector<Position> vecPos;
    vecPos = genMove(board,num,n,side);
    if(vecPos.size()==0)
    {
        return 0;
    }
    int i;
    int val;
    int best = -1000;
    for(i=0;i<int(vecPos.size());i++)
    {
        int temp;
        temp = board[vecPos[i].xTo][vecPos[i].yTo];
        board[vecPos[i].xTo][vecPos[i].yTo] = vecPos[i].num;
        board[vecPos[i].xFrom][vecPos[i].yFrom] = 0;

        val = Beta(depth-1,side);

        if(val>best)
        {
            bestPosition.xFrom = vecPos[i].xFrom;
            bestPosition.yFrom = vecPos[i].yFrom;
            bestPosition.xTo = vecPos[i].xTo;
            bestPosition.yTo = vecPos[i].yTo;
            bestPosition.num = vecPos[i].num;
        }
        board[vecPos[i].xFrom][vecPos[i].yFrom] = vecPos[i].num;
        board[vecPos[i].xTo][vecPos[i].yTo] = temp;
    }
    return val;
}

//测试由骰子值确定的棋子
void checkIsRightNum()
{
    defineWinStart(board);
    side = 1;
    num = 3;
    isRightNumRed(board,num,n);
    printf("num = %d\n",num);
    printf("n1 = %d\n",n[0]);
    printf("n2 = %d\n",n[1]);
    drawBoard(board,number);
}

//测试全部可下位置
void checkGenMove()
{
    defineWinStart(board);
    side = 1;
    num = 3;
    vector<Position> vecPos;
    vecPos = genMove(board,num,n,side);
    int i;
    for(i=0;i<int(vecPos.size());i++)
    {
        printf("num = %d\n",vecPos[i].num);
        printf("(%d,%d)\n",vecPos[i].xFrom,vecPos[i].yFrom);
        printf("(%d,%d)\n",vecPos[i].xTo,vecPos[i].yTo);
        printf("\n");
    }
    drawBoard(board,number);

}

//测试每个骰子的概率次数
void checkGetCountForValue()
{
    defineWinStart(board);
    side = -1;
    int i;
    getCountForValue(board,side,c);
    for(i=0;i<6;i++)
    {
        printf("c = %d\n",c[i]);
    }
    drawBoard(board,number);
}

//测试到角点的距离
void checkDistanceValue()
{
    defineWinStart(board);
    side = 1;
    num = 3;
    double m = distanceValue(board,side);
    printf("distanceValue = %f\n",m);
    drawBoard(board,number);
}

//测试吃子估值
void checkEatValue()
{
    defineWinStart(board);
    side = 1;
    num = 3;
    double m = eatValue(board,side);
    printf("eatValue = %f\n",m);
    drawBoard(board,number);
}

//测试随机下棋获得最佳位置
void checkGetBestRandPosition()
{
    defineWinStart(board);
    side = 1;
    num = 3;
    getBestRandPosition();
    printf("%d\n",bestPosition.num);
    printf("(%d,%d)\n",bestPosition.xFrom,bestPosition.yFrom);
    printf("(%d,%d)\n",bestPosition.xTo,bestPosition.yTo);
    printf("prob = %f\n",bestPosition.prob);
    printf("\n");
    drawBoard(board,number);

}

//测试随机下棋
void checkRandomPlay()
{
    defineWinStart(board);
    side= 1;
    num = 3;
    double win = randomPlay(board,num);
    printf("%f\n",win);
    drawBoard(board,number);
}

//测试估值
void checkValue()
{
    defineWinStart(board);
    /*side = 1;
    num = 3;
    double cValue = Value(board,side);
    printf("Value = %f\n",cValue);*/

    side = -1;
    num = 4;
    double cValue = Value(board,side);
    printf("Value = %f\n",cValue);
    drawBoard(board,number);
}

//测试极大极小算法
void checkMinMaxSearch()
{
    defineWinStart(board);
    drawBoard(board,number);
    side = -1;
    num = 4;
    double x = minMaxSearch(1);
    printf("minMaxSearch = %f\n",x);
    printf("%d\n",bestPosition.num);
    printf("(%d,%d)\n",bestPosition.xFrom,bestPosition.yFrom);
    printf("(%d,%d)\n",bestPosition.xTo,bestPosition.yTo);
}

//测试负极大极小
void checkNegaMax()
{
    defineWinStart(board);
    drawBoard(board,number);
    side = -1;
    num = 4;
    double x = negaMax(3,side);
    printf("negaMax = %f\n",x);
    printf("%d\n",bestPosition.num);
    printf("(%d,%d)\n",bestPosition.xFrom,bestPosition.yFrom);
    printf("(%d,%d)\n",bestPosition.xTo,bestPosition.yTo);
}

//测试AlphaBeta
void checkAlphaBeta()
{
    defineWinStart(board);
    drawBoard(board,number);
    side = -1;
    num = 4;
    double x = alphaBeta(1,side);
    printf("alphaBeta = %f\n",x);
    printf("%d\n",bestPosition.num);
    printf("(%d,%d)\n",bestPosition.xFrom,bestPosition.yFrom);
    printf("(%d,%d)\n",bestPosition.xTo,bestPosition.yTo);
}



#endif // AEST_H_INCLUDED
