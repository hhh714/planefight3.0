//1、打开窗口，加载图片，链表结构、变量的定义
// 2、生成我方飞机、我方飞机的移动
//3、子弹的生成、发射、释放
//4、敌机生成移动和销毁
//5、子弹消灭敌机
//6、敌机与我方飞机的碰撞
//7、得分的显示
//8、音乐的播放
#include<easyx.h>
#include"list.h"
#include<conio.h>
#include<stdlib.h>
#include<time.h>
#include<math.h>
#include<mmsystem.h>
#include<windows.h>
#include<graphics.h>
#pragma once
#pragma comment(lib,"MSIMG32.LIB")
#pragma warning(disable.4996)
#pragma comment(lib,"winmm.lib")//把音乐包含进来

#define BGWEIGHT 400
#define BGHEIGHT 600
//我方飞机大小
#define myairweight 60
#define myairheight 80

//敌机大小
#define enemyweight 60
#define enemyheight 60
//子弹大小
#define zidanweight 30
#define zidanheight 30

//boss敌机
#define bossweight 100
#define bossheight 120
#define bossHP 15

// 定义boss敌机节点和血量
Node boss;
int bossHPLeft;
bool isBossAlive;
bool canSpawnBoss = true;



//定义变量
IMAGE img[4];//图片数组
IMAGE img_boss;
Node myplane;//我方飞机
LL mybullut;//我方子弹链表
LL enemyplane;//敌方飞机链表
int isend;
int count;


static int targetscore = 0;
extern int difficulty ;
bool GetDiffculty();

IMAGE img_bg[2];
int bg_offset = 0;
bool GetTargetScore()
{
	TCHAR szBuffer[10] = { 0 };
	if (InputBox(szBuffer, 10, _T("请输入目标分数（1-1000）："), _T("目标设置"), _T("100"), 0, 0, true) == 0)
		return false;

	targetscore = _ttoi(szBuffer);
	return (targetscore > 0 && targetscore <= 1000);
}
void init()
{
//加载图片
	//1、绝对路径：带盘符的路径
	//2、相对路径
	loadimage(&img_bg[0], "res/bkground.jpg",BGWEIGHT,BGHEIGHT*2);
	loadimage(&img_bg[1], "res/bkground.jpg", BGWEIGHT, BGHEIGHT * 2);
	loadimage(&img[1], "res/myplane.png", myairweight, myairheight);
	setbkcolor(WHITE);
	setcolor(WHITE);

	loadimage(&img[2], "res/enemyplane.png", enemyweight, enemyheight);
	setbkcolor(WHITE);
	setcolor(WHITE);
	loadimage(&img[3], "res/mybullut.png", zidanweight, zidanheight);
	setbkcolor(WHITE);
	setcolor(WHITE);
	
	loadimage(&img_boss, "res/boss.png", bossweight, bossheight);
	setbkcolor(WHITE);
	setcolor(WHITE);

	//我方飞机的初始化
	myplane.x = BGWEIGHT / 2 - myairweight / 2;
	myplane.y = BGHEIGHT - myairheight - 10;
	myplane.speed=10 ;
	myplane.next = NULL;

	//子单链表的初始化
	mybullut.head = NULL;
	mybullut.end = NULL;

	//敌方飞机的初始化
	enemyplane.head = NULL;
	enemyplane.end = NULL;

	// boss敌机的初始化
	boss.x = BGWEIGHT / 2 - bossweight / 2;
	boss.y = -bossheight;
	boss.speed = 2;
	boss.next = NULL;
	bossHPLeft = bossHP;
	isBossAlive = false;
	canSpawnBoss = true;
	

	//播下随机生成的种子
	srand((unsigned)time(NULL));
	isend = 0;//为1时，游戏结束
	count = 0;

	//加载音乐
	mciSendString("open res/music.mp3", NULL, 0, 0);
	mciSendString("play res/music.mp3", NULL, 0, 0);
}
//把图片贴在图形界面上
void draw()
{
	static int offset = 0;
//开始批量绘图
	BeginBatchDraw();
	putimage(0, offset, &img_bg[0]);
	putimage(0, offset - BGHEIGHT * 2, &img_bg[1]);
	offset += 1;
	if (offset >= BGHEIGHT * 2) offset = 0;
	putimage(myplane.x, myplane.y, &img[1],SRCAND);
	//putimage(100, 100, &img[2]);
	for (Node* temp = enemyplane.head; temp != NULL; temp = temp->next)
	{
		putimage(temp->x, temp->y, &img[2],SRCAND);
	}
	//putimage (200,200, &img[3]);
	for (Node* temp = mybullut.head; temp != NULL; temp = temp->next)
	{
		putimage(temp->x, temp->y, &img[3],SRCAND);
	}
	//显示得分
	TCHAR str1[30];
	wsprintf(str1, "当前得分 ：%d", count);
	outtextxy(0, 0, str1);
	setbkcolor(0xcccccc);//设置文本背景颜色
	settextcolor(BLACK);

	TCHAR targetStr[50];
	wsprintf(targetStr, "目标: %d", targetscore);
	outtextxy(BGWEIGHT - 120, 10, targetStr);

	if (isBossAlive) {
		
		putimage(boss.x, boss.y, &img_boss, SRCAND);

		// 绘制boss血条
		int barWidth = 100;
		int barHeight = 10;
		int barX = boss.x + (bossweight - barWidth) / 2;
		int barY = boss.y - barHeight - 5;
		setfillcolor(RED);
		solidrectangle(barX, barY, barX + barWidth * (bossHPLeft / (float)bossHP), barY + barHeight);
		setlinecolor(WHITE);
		rectangle(barX, barY, barX + barWidth, barY + barHeight);
	}

	//结束批量绘图
	EndBatchDraw();


}

//生成子弹的函数
void create_mybullut()
{
	LinkList_insert(&mybullut, myplane.x + myairweight / 2 - zidanweight, myplane.y, 5);

}

void play()
{
//玩游戏函数：控制飞机的移动
const float frameSpeed = 0.8f; // 控制移动速度（按帧率平滑移动）

// 上移
if (GetAsyncKeyState('W') & 0x8000 || GetAsyncKeyState(VK_UP) & 0x8000) 
{
	myplane.y -= (int)(myplane.speed * frameSpeed);
	if (myplane.y < 0) myplane.y = 0;
}

// 下移
if (GetAsyncKeyState('S') & 0x8000 || GetAsyncKeyState(VK_DOWN) & 0x8000) 
{
	myplane.y += (int)(myplane.speed * frameSpeed);
	if (myplane.y > BGHEIGHT - myairheight)
		myplane.y = BGHEIGHT - myairheight;
}

// 左移
if (GetAsyncKeyState('A') & 0x8000 || GetAsyncKeyState(VK_LEFT) & 0x8000) 
{
	myplane.x -= (int)(myplane.speed * frameSpeed);
	if (myplane.x < 0) myplane.x = 0;
}

// 右移
if (GetAsyncKeyState('D') & 0x8000 || GetAsyncKeyState(VK_RIGHT) & 0x8000) 
{
	myplane.x += (int)(myplane.speed * frameSpeed);
	if (myplane.x > BGWEIGHT - myairweight)
		myplane.x = BGWEIGHT - myairweight;
}

// 发射子弹
static int shootcooldown = 0;
if (GetAsyncKeyState(VK_SPACE) & 0x8000)
{
	if (shootcooldown <= 0)
	{
		create_mybullut();
		shootcooldown = 10;
	}
	
}
if (shootcooldown > 0) shootcooldown--;

}


//我方子弹的移动 敌机的移动
void move()
{
	for (Node* temp = mybullut.head; temp != NULL; temp = temp->next)
	{
		temp->y -= temp->speed;

	}
	for (Node* tempenemy = enemyplane.head; tempenemy != NULL; tempenemy = tempenemy->next)
	{
		tempenemy->y += tempenemy->speed;
	}
	if (isBossAlive) {
		boss.y += boss.speed;
		if (boss.y > BGHEIGHT) {
			isBossAlive = false;
			canSpawnBoss = true;
		}
	}
}

//检测子弹与敌机是否发生碰撞
int blt_isconflict(Node*p,Node*q)  //p子弹q敌机
{
	//以中心点来判断
	int px = p->x + zidanweight / 2;
	int py = p->y + zidanheight / 2;
	int qx = q->x + enemyweight / 2;
	int qy = q->y + enemyheight / 2;

	//判断是否重叠
	if (abs(px - qx) < enemyheight / 2 + zidanweight / 2 && abs(py - qy) < enemyheight / 2 + zidanheight / 2)
	{
		return 1;
	}
	else
	{
		return 0;
	}

}

//检测我放飞机是否与敌机发生碰撞
int myplane_conflict(Node*p,Node*q)//p飞机q敌机
{
	//以中心点检测
	int px = p->x + myairweight / 2;
	int py = p->y + myairheight / 2;
	int qx = q->x + enemyweight / 2;
	int qy = q->y + enemyheight / 2;
	if (abs(px - qx) < enemyweight / 2 + myairweight / 2 && abs(py - qy) < enemyheight / 2 + myairheight / 2)
	{
		return 1;
	}
	else
	{
		return 0;
	}


}

//检测我放飞机是否与boss发生碰撞
int myplane_conflict_with_boss(Node* p) {
	int px = p->x + myairweight / 2;
	int py = p->y + myairheight / 2;
	int qx = boss.x + bossweight / 2;
	int qy = boss.y + bossheight / 2;
	if (abs(px - qx) < bossweight / 2 + myairweight / 2 && abs(py - qy) < bossheight / 2 + myairheight / 2) {
		return 1;
	}
	return 0;
}


//释放函数
void  Delete()
{
	//释放越界子弹
	for (Node* tempbullut = mybullut.head; tempbullut != NULL; tempbullut = tempbullut->next)
	{
		if (tempbullut->y < 0)
		{
			Node_delete(&mybullut, tempbullut);//一次删除一个节点
			return;
		}
	}
	//释放越界敌机

	for (Node* tempenenmy = enemyplane.head; tempenenmy != NULL; tempenenmy = tempenenmy->next)
	{
		if (tempenenmy->y >= BGHEIGHT)
		{
			Node_delete(&enemyplane, tempenenmy);
			return;
        }

	}

	//子弹消灭敌机
	//先遍历每一个子弹，在每一个子弹节点遍历所有敌机
	for (Node* tempbullut = mybullut.head; tempbullut != NULL; tempbullut = tempbullut->next)
	{
		for (Node* tempenemy = enemyplane.head; tempenemy != NULL; tempenemy = tempenemy->next)
		{
			if (blt_isconflict(tempbullut, tempenemy))
			{
				Node_delete(&mybullut, tempbullut);
				Node_delete(&enemyplane, tempenemy);
				count++;//得分加1
				return;
				
			}
			
		}
	}
	if (isBossAlive) {
		for (Node* tempbullut = mybullut.head; tempbullut != NULL; tempbullut = tempbullut->next) 
		{
			if (blt_isconflict(tempbullut, &boss))
			{
				Node_delete(&mybullut, tempbullut);
				bossHPLeft--;
				if (bossHPLeft <= 0) 
				{
					isBossAlive = false;
					canSpawnBoss = true;
					count += 50; // 击败boss获得50分
				}
				return;
			}
		}
	}

	for (Node* temp = enemyplane.head; temp != NULL; temp = temp->next)
	{

		//敌机与我方飞机是否发生冲突
		if (myplane_conflict(&myplane, temp))
		{
			isend = 1;
		
		}

	}
	if (isBossAlive && myplane_conflict_with_boss(&myplane)) {
		isend = 1;
	}


}
//敌机的生成
void enemy_create()
{
	static int val = 0;
	if (val >= 100) {
		if (count >= targetscore && canSpawnBoss && !isBossAlive) {
			// 以较低的概率生成 BOSS
			int randomNum = rand() % 10; // 生成 0 - 9 的随机数
			if (randomNum == 0) { // 这里设置 BOSS 出现的概率为 10%
				// 生成boss敌机
				boss.x = rand() % (BGWEIGHT - bossweight);
				boss.y = -bossheight;
				bossHPLeft = bossHP;
				isBossAlive = true;
				canSpawnBoss = false;
			}
			else {
				int baseSpeed = 1;
				switch (difficulty) {
				case 1: baseSpeed = 1; break;   // 简单：1-3
				case 2: baseSpeed = 2; break;   // 普通：2-4
				case 3: baseSpeed = 3; break;   // 困难：3-5
				}
				LinkList_insert(&enemyplane, rand() % (BGWEIGHT - enemyweight),
					-enemyheight, baseSpeed + rand() % 2);
			}
		}
		else if (!isBossAlive) {
			int baseSpeed = 1;
			switch (difficulty) {
			case 1: baseSpeed = 1; break;   // 简单：1-3
			case 2: baseSpeed = 2; break;   // 普通：2-4
			case 3: baseSpeed = 3; break;   // 困难：3-5
			}
			LinkList_insert(&enemyplane, rand() % (BGWEIGHT - enemyweight),
				-enemyheight, baseSpeed + rand() % 2);
		}
		val = 0;
	}
	val++;
}






//所有功能从这个函数开始
void start()
{
	initgraph(BGWEIGHT, BGHEIGHT);
	init();
	draw();
	while (1)
	{
		ExMessage msg;
		while (peekmessage(&msg, EX_KEY | EX_MOUSE)) {
			// 可在此处理其他消息（如关闭窗口事件）
			if (msg.message == WM_KEYDOWN) {
				// 预留处理其他按键事件
			}
		}
		play();
		draw();
		move();
		Sleep(8);
		Delete();
		enemy_create();
		
		if (isend == 1)
		{
			mciSendString("stop res/music.mp3", NULL, 0, 0);


			
			LinkList_all(&mybullut);//释放所有子弹节点
			LinkList_all(&enemyplane);

			TCHAR resultMsg[100];
			if (count >= targetscore) {
				mciSendString("close res/vicmusic.mp3", NULL, 0, 0);
				mciSendString("open res/vicmusic.mp3", NULL, 0, 0);
				mciSendString("play res/vicmusic.mp3", NULL, 0, 0);
				wsprintf(resultMsg, _T("难度： %d级\n恭喜达成目标 %d / %d!\n是否重新开始？"),difficulty, count, targetscore);
				
			}
			else {
				mciSendString("close res/endingmusic.mp3", NULL, 0, 0);
				mciSendString("open res/endingmusic.mp3", NULL, 0, 0);
				mciSendString("play res/endingmusic.mp3", NULL, 0, 0);
				wsprintf(resultMsg, _T("难度： %d级\n当前得分  %d / %d\n再接再厉！是否重试？"),difficulty, count, targetscore);
			}

			//跳出弹窗：提示失败
			
			

			
			int temp= MessageBox(GetForegroundWindow(), resultMsg, _T("游戏结束"), MB_YESNO);
			if (temp == IDYES)
			{
				
				init();//初始化游戏
				
			}
			else
			{
			
				closegraph();
				exit(0);

			}
		}

	}
	closegraph();
}