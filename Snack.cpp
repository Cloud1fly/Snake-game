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
 
void welcome();               //55��ʼ����
void Finish();                //291��������
void creatgraph();            //69Χǽ��ӡ
void gotoxy(int x, int y);    //111�����ת����ΪX 0,1,2..
void gotoprint(int x, int y); //121��ת��ӡ
void gotodelete(int x, int y);//127��תɾ��
void creatfood();             //133ʳ�����
int ClickControl();           //157��ȡ�����ź�
int Judge();                  //270��Ϸ�����ж�
void MovingBody(); 	    	  //172�ߵ��ƶ� 
void Eating();                //223�߳Ե�������Ĳ������쳤��
void ChangeBody(int a,int b); //245�ߵ�����任,��һ������ǰһ��STRUCT,a,bΪhead֮ǰ���� 
 
/*ȫ�ֱ��� + Ԥ����*/
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
	system("color 7");//�ı����̨��ɫ 
	welcome();//���ɻ�ӭ���� 
	creatgraph();//�ߺ���Ϸ�����ʼ�� 
	creatfood();//ʳ���ʼ�� 
	
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
	printf("����Ӣ�����뷨�в�����");
	gotoxy(20, 16);
	printf("WASD���Ʒ���E��ͣ");
	gotoxy(20, 18);
	system("pause");
	system("cls");//����ϵͳ�����У�cmd���е�cls���������յ�ǰ�ն˴����е����ݡ�
}
/**********************************************************/
void creatgraph()             
{
	for (int i = 0; i < 58; i += 2)//��ӡ���±߿�
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
	printf("��WASD�е�����һ����ʼ��Ϸ\n");
	gotoxy(63, 15);
	printf("��ķ�����:%d   ",score);
	gotoxy(63, 20);
	printf("��E��ͣ���ٴΰ�WASD���е�����һ��������Ϸ\n");
	gotoxy(63, 25);
	printf("Ҫ��Ӣ�����뷨��WASD�����Ʒ���");
	gotoxy(63, 26);
	printf("Ҫ��Ӣ�����뷨��WASD�����Ʒ���");
	gotoxy(63, 27);
	printf("Ҫ��Ӣ�����뷨��WASD�����Ʒ���!");
	
	head = (snake*)malloc(sizeof(snake));//��ͷ 
	//��ʼ��λ�� 
	head->x = 16;
	head->y = 15;
	
	//gotoprint(head->x, head->y);
	tail = (snake*)malloc(sizeof(snake));//��β����Ϊ�ڱ���㣬����ֵ 
	//��p->next == null , ˵���Ѿ������������� 
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
	hOutput = GetStdHandle(STD_OUTPUT_HANDLE);//��׼������ 
	SetConsoleCursorPosition(hOutput, pos);//��λ���λ��
}
/**********************************************************/
void gotoprint(int x, int y)//��궨λ����x��y������� ��
{
	gotoxy(x, y);
	printf("��");
}
/**********************************************************/
void gotodelete(int x, int y)//��궨λ����x��y������ӡ���ַ� 
{
	gotoxy(x, y);
	printf("  ");
}
/**********************************************************/
void creatfood()
{
	//�������������������������ֵ���������ֵ��Ӱ�����ʹ�� rand() �������ɵ���������С�
	srand((int)time(NULL));
	lable://�Զ����ǩ 
	food.y = rand() % (25 - 1 + 1) + 1;//����1��25������� 
	food.x = rand() % (54 - 2 + 1) + 2;//����2��54�������
	if (food.x % 2 != 0)
	{
		food.x = food.x+1;// �������Ϊ�����������1����ȷ�� x ����Ϊż��
	}
	
	// ���������ų�ʳ���������ص������
	snake *judge = head;
	while (judge->next != NULL)  //�����ų������ظ�
	{
		if (food.x == judge->x && food.y == judge->y)
		{
			goto lable;// ���ʳ��������λ���ص����򷵻� lable �������������λ�õ�ʳ��
		}
		judge = judge->next;
	}
	gotoxy(food.x, food.y); // ������ƶ���ʳ���λ��
	printf("��");// �ڸ�λ�ô�ӡ "��" ����
}
/**********************************************************/
int ClickControl()
{
	while (true)
	{
		if (!Judge()) return 0;
		
		if (_kbhit())//ʵʱ�жϼ����Ƿ��а��������£� ���û�з���0 
		{
			click = _getch();
			//�ӿ���̨���ն˴����л�ȡ�û�����ĵ����ַ����������û����»س�����
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
	int a = head->x, b = head->y;//��ʱ�����洢��ͷ�ı�ǰ��λ�� 
	snake *p = head;
	//ͨ������պ��ӡʵ�ֶ���Ч��
	while (p->next != NULL)
	{
		gotodelete(p->x, p->y);//��ӡ���ַ�ģ����� 
		count++;
		p = p->next;
	}
	switch (click)
	{
		case up:
			head->y -= 1;//�ı���ͷλ�� 
			ChangeBody(a,b);//�ı����� 
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
			head->x += 2;//+2 ��Ϊ����ͼ����������߼��ϻ���+1
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
	Sleep(speed);//countԽ�٣�speedԽ�� ���ܵ�Խ�� 
}
/**********************************************************/
void Eating()
{
	if (head->x == food.x && head->y == food.y)
	{
		creatfood();
		snake *_new = (snake*)malloc(sizeof(snake));
		snake *p = head;
		//��λ�����һ�����(�ڱ������⣩ 
		while (p->next->next != NULL)
		{
			p = p->next;
		}
		p->next = _new;
		_new->next = tail;//�ڱ����ĺô� 
	
		score += 10;
		gotoxy(77, 15);
		printf("%d", score);
	}
}
/**********************************************************/
//�������ڵ����θ�����ͷ��ǰ�ƶ�һλ��
//��ͷ���֮��Ľ��������ǰ���Ǿ����� 
void ChangeBody(int a,int b)
{
	snake *p = head->next;//��ʼ��Ϊͷ�ڵ����һ����� 
	
	//tΪ��Ҫ���ǵĵ�����꣬_t����ʱ����������һ��������� 
	int t1, t2,_t1,_t2; 
	t1 = p->x;
	t2 = p->y;
	//p->nextΪ��Ҫ���µĵ����� 
	//p�������һ������ͣ�£� �����ڱ������������ p->next->next
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
	
	//��������ͷ����һ���ڵ� 
	p = head->next;
	p->x = a;
	p->y = b;	
}
/**********************************************************/
int Judge()
{
	//�ж���ͷ��û�������߽� 
	if (head->x == 0|| head->x == 56 || head->y == 0 || head->y == 26)
	{
		Finish();
		return 0;
	}
	snake *p = head->next;
	while (p->next != NULL)
	{
		if (head->x == p->x && head->y == p->y)//�ж���ͷ��û������������ 
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
	printf("��ķ����ǡ� %d ��", score);
	gotoxy(18, 18);
	printf("�����Ŭ����");
	gotoxy(0, 27);
	system("pause");
}
