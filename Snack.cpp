#include<stdio.h>
#include<string.h>
#include<Windows.h>
#include<time.h>
#include<conio.h>
 

#define up 'w'
#define down 's'
#define left 'a'
#define right 'd'
#define stop 'e'
 
void welcome();               //55开始界面
void Finish();                //291结束界面
void creatgraph();            //69围墙打印
void gotoxy(int x, int y);    //111光标跳转，横为X 0,1,2..
void gotoprint(int x, int y); //121跳转打印
void gotodelete(int x, int y);//127跳转删除
void creatfood();             //133食物产生
int ClickControl();           //157获取键盘信号
int Judge();                  //270游戏结束判断
void MovingBody(); 	    	  //172蛇的移动 
void Eating();                //223蛇吃到东西后的操作（伸长）
void ChangeBody(int a,int b); //245蛇的坐标变换,后一个复制前一个STRUCT,a,b为head之前坐标 
 
/*全局变量 + 预处理：*/
typedef struct Snakes
{
	int x;
	int y;
	struct Snakes *next;
}snake;
 
snake *head,*tail;
 
struct Food
{
	int x;
	int y;
}food;

int score = 0;
char click = 1;
int speed;
 
/************************************************************/
 
int main()
{
	system("color 7");//改变控制台颜色 
	welcome();//生成欢迎界面 
	creatgraph();//蛇和游戏界面初始化 
	creatfood();//食物初始化 
	
	if (ClickControl() == 0) return 0;
	return 0;
}
 
/**********************************************************/
void welcome()
{
	gotoxy(15,10);
	printf("/**********************************************/");
	gotoxy(15, 20);
	printf("/**********************************************/");
	gotoxy(20, 13);
	printf("请在英文输入法中操作！");
	gotoxy(20, 16);
	printf("WASD控制方向，E暂停");
	gotoxy(20, 18);
	system("pause");
	system("cls");//调用系统命令行（cmd）中的cls命令，用于清空当前终端窗口中的内容。
}
/**********************************************************/
void creatgraph()             
{
	for (int i = 0; i < 58; i += 2)//打印上下边框
	{
		gotoprint(i, 0);
		gotoprint(i, 26);
	}
	for (int i = 1; i < 26; i++)
	{
		gotoprint(0, i);
		gotoprint(56, i);
	}
	
	gotoxy(63, 10);
	printf("按WASD中的任意一键开始游戏\n");
	gotoxy(63, 15);
	printf("你的分数是:%d   ",score);
	gotoxy(63, 20);
	printf("按E暂停，再次按WASD其中的任意一键继续游戏\n");
	gotoxy(63, 25);
	printf("要用英文输入法的WASD来控制方向！");
	gotoxy(63, 26);
	printf("要用英文输入法的WASD来控制方向！");
	gotoxy(63, 27);
	printf("要用英文输入法的WASD来控制方向!");
	
	head = (snake*)malloc(sizeof(snake));//蛇头 
	//初始化位置 
	head->x = 16;
	head->y = 15;
	
	//gotoprint(head->x, head->y);
	tail = (snake*)malloc(sizeof(snake));//蛇尾，作为哨兵结点，不赋值 
	//当p->next == null , 说明已经遍历完蛇身了 
	snake *p = (snake*)malloc(sizeof(snake));
	snake *q = (snake*)malloc(sizeof(snake));
	p->x = 16;
	p->y = 16;
	q->x = 16;
	q->y = 17;
	head->next = p;
	p->next = q;
	q->next = tail; 
	//gotoprint(p->x, p->y);
	//gotoprint(q->x, q->y);
	tail->next = NULL;
}
/**********************************************************/
void gotoxy(int x, int y)
{
	COORD pos;
	HANDLE hOutput; 
	pos.X = x;
	pos.Y = y;
	hOutput = GetStdHandle(STD_OUTPUT_HANDLE);//标准输入句柄 
	SetConsoleCursorPosition(hOutput, pos);//定位光标位置
}
/**********************************************************/
void gotoprint(int x, int y)//光标定位到（x，y）并输出 ■
{
	gotoxy(x, y);
	printf("■");
}
/**********************************************************/
void gotodelete(int x, int y)//光标定位到（x，y）并打印空字符 
{
	gotoxy(x, y);
	printf("  ");
}
/**********************************************************/
void creatfood()
{
	//用于设置随机数生成器的种子值，这个种子值将影响后续使用 rand() 函数生成的随机数序列。
	srand((int)time(NULL));
	lable://自定义标签 
	food.y = rand() % (25 - 1 + 1) + 1;//生成1到25的随机数 
	food.x = rand() % (54 - 2 + 1) + 2;//生成2到54的随机数
	if (food.x % 2 != 0)
	{
		food.x = food.x+1;// 如果列数为奇数，则将其加1，以确保 x 坐标为偶数
	}
	
	// 遍历蛇身，排除食物与蛇身重叠的情况
	snake *judge = head;
	while (judge->next != NULL)  //遍历排除蛇身重复
	{
		if (food.x == judge->x && food.y == judge->y)
		{
			goto lable;// 如果食物与蛇身位置重叠，则返回 lable 处重新生成随机位置的食物
		}
		judge = judge->next;
	}
	gotoxy(food.x, food.y); // 将光标移动到食物的位置
	printf("☆");// 在该位置打印 "☆" 符号
}
/**********************************************************/
int ClickControl()
{
	while (true)
	{
		if (!Judge()) return 0;
		
		if (_kbhit())//实时判断键盘是否有按键被按下， 如果没有返回0 
		{
			click = _getch();
			//从控制台或终端窗口中获取用户输入的单个字符，而无需用户按下回车键。
		}
		MovingBody();
		Eating();
	}
	return 1;
}
/**********************************************************/
void MovingBody()
{
	int count = 0;
	int a = head->x, b = head->y;//临时变量存储蛇头改变前的位置 
	snake *p = head;
	//通过先清空后打印实现动画效果
	while (p->next != NULL)
	{
		gotodelete(p->x, p->y);//打印空字符模拟清空 
		count++;
		p = p->next;
	}
	switch (click)
	{
		case up:
			head->y -= 1;//改变蛇头位置 
			ChangeBody(a,b);//改变蛇身 
			break;
		case down:
			head->y += 1;
			ChangeBody(a,b);
			break;
		case left:
			head->x -= 2; 
			ChangeBody(a,b);
			break;
		case right:
			head->x += 2;//+2 是为了让图像更清晰，逻辑上还是+1
			ChangeBody(a,b);
			break;
		case stop:
			break;
		default: 
			break;
	}
	
	p = head;
	while (p->next != NULL)
	{
		gotoprint(p->x, p->y);
		p = p->next;
	}
	p = head;
	gotoxy(0, 28);
	if (count <= 10) speed = 150;
	else if (count > 10 && count <= 20) speed = 100;
	else if (count > 20 && count <= 40) speed = 50;
	else speed = 10;
	Sleep(speed);//count越少，speed越大 ，跑的越慢 
}
/**********************************************************/
void Eating()
{
	if (head->x == food.x && head->y == food.y)
	{
		creatfood();
		snake *_new = (snake*)malloc(sizeof(snake));
		snake *p = head;
		//定位到最后一个结点(哨兵结点除外） 
		while (p->next->next != NULL)
		{
			p = p->next;
		}
		p->next = _new;
		_new->next = tail;//哨兵结点的好处 
	
		score += 10;
		gotoxy(77, 15);
		printf("%d", score);
	}
}
/**********************************************************/
//将后续节点依次根据蛇头向前移动一位。
//让头结点之后的结点依次向前覆盖就行了 
void ChangeBody(int a,int b)
{
	snake *p = head->next;//初始化为头节点的下一个结点 
	
	//t为需要覆盖的点的坐标，_t作临时变量保存下一个结点坐标 
	int t1, t2,_t1,_t2; 
	t1 = p->x;
	t2 = p->y;
	//p->next为需要更新的点坐标 
	//p到达最后一个结点就停下， 由于哨兵结点存在因此是 p->next->next
	while (p->next->next != NULL)
	{
		_t1 = p->next->x;
		_t2 = p->next->y;
		p->next->x = t1;
		p->next->y = t2;
		t1 = _t1;
		t2 = _t2; 
		p = p->next;
	}
	
	//最后更新蛇头的下一个节点 
	p = head->next;
	p->x = a;
	p->y = b;	
}
/**********************************************************/
int Judge()
{
	//判断蛇头有没有碰到边界 
	if (head->x == 0|| head->x == 56 || head->y == 0 || head->y == 26)
	{
		Finish();
		return 0;
	}
	snake *p = head->next;
	while (p->next != NULL)
	{
		if (head->x == p->x && head->y == p->y)//判断蛇头有没有碰到蛇身体 
		{
			Finish();
			return 0;
		}
		p = p->next;
	}
	return 1;
}
/**********************************************************/
void Finish()
{
	system("cls");
	gotoxy(15, 10);
	printf("/**********************************************/");
	gotoxy(15, 20);
	printf("/**********************************************/");
	gotoxy(18, 14);
	printf("GAME   OVER");
	gotoxy(18, 16);
	printf("你的分数是【 %d 】", score);
	gotoxy(18, 18);
	printf("请继续努力！");
	gotoxy(0, 27);
	system("pause");
}
