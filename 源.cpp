#include"Airp.h"

ExMessage msg = { 0 };
int difficulty = 1;
bool GetDifficulty()
{
	TCHAR szBuffer[10] = { 0 };
	const TCHAR* prompt = _T("请选择难度：\n1. 简单\n2. 普通\n3. 困难");
	if (InputBox(szBuffer, 10, prompt, _T("难度设置"), _T("2"), 0, 0, true) == 0)
		return false;

		difficulty = _ttoi(szBuffer);
		if (difficulty < 1 || difficulty > 3) 
		{
			MessageBox(GetForegroundWindow(), _T("请输入1-3之间的数字！"), _T("错误"), MB_OK);
			return false;
		}

		return true;
}
inline void putimage_withalpha(IMAGE* dstimg, IMAGE& img, int x, int y) {
	HDC dstDC = GetImageHDC(dstimg);
	HDC srcDC = GetImageHDC(&img);
	int w = img.getwidth();
	int h = img.getheight();
	BLENDFUNCTION bf = { AC_SRC_OVER, 0, 255, AC_SRC_ALPHA };
	AlphaBlend(dstDC, x, y, w, h, srcDC, 0, 0, w, h, bf);
}



bool inarea(int mx, int my, int x, int y, int w, int h)
{
	if (mx > x && mx<x + w && my>y && my < y + h)
	{
		return true;
	}
	return false;
}

void showgameinstructions()
{
	IMAGE img_instruction_bg;
	// 加载带透明通道的玩法介绍图（需确保图片尺寸与窗口匹配）
	loadimage(&img_instruction_bg, _T("res/introduction.png"), BGWEIGHT - 100, BGHEIGHT - 150);

	// 绘制暗色遮罩层（修复黄色问题关键）
	setfillcolor(0xDF1A1A1A); // 深灰色半透明
	solidrectangle(0, 0, BGWEIGHT, BGHEIGHT);

	// 居中显示玩法介绍图（自动缩放）
	int x = (BGWEIGHT - img_instruction_bg.getwidth()) / 2;
	int y = (BGHEIGHT - img_instruction_bg.getheight()) / 2;

	// 绘制背景阴影
	setfillcolor(0x80000000);
	fillroundrect(x - 10, y - 10,
		x + img_instruction_bg.getwidth() + 10,
		y + img_instruction_bg.getheight() + 10,
		15, 15);

	// 绘制带透明通道的游戏说明图
	putimage_withalpha(NULL, img_instruction_bg, x, y);

	// 标题文字（白色带阴影）
	settextstyle(36, 0, _T("微软雅黑"));
	settextcolor(0x222222);
	outtextxy(x + 18, y + 18, _T("游戏说明"));
	settextcolor(0xFFFFFF);
	outtextxy(x + 15, y + 15, _T("游戏说明"));

	// 正文内容（亮灰色）
	settextstyle(22, 0, _T("微软雅黑"));
	
	settextcolor(BLUE);
	const TCHAR* instructions =
		_T("【操作说明】\n")
		_T("移动控制：W/A/S/D 或方向键\n")
		_T("射击：空格键\n\n")
		_T("【游戏目标】\n")
		_T("1. 击落敌机获得分数\n")
		_T("2. 躲避敌机避免碰撞\n")
		_T("3. 达成目标分数后迎战BOSS");

	RECT textRect = { x + 40, y + 80,
					 x + img_instruction_bg.getwidth() - 40,
					 y + img_instruction_bg.getheight() - 40 };
	drawtext(instructions, &textRect, DT_WORDBREAK | DT_LEFT);

	// 关闭按钮（白色风格）
	const int closeBtnSize = 32;
	const int closeBtnx = x + img_instruction_bg.getwidth() - closeBtnSize - 20;
	const int closeBtny = y + 20;
	bool isHover = inarea(msg.x, msg.y, closeBtnx, closeBtny, closeBtnSize, closeBtnSize);

	setlinestyle(PS_SOLID, 2);
	setlinecolor(isHover ? 0xFF4444 : 0xAAAAAA);
	line(closeBtnx + 8, closeBtny + 8, closeBtnx + closeBtnSize - 8, closeBtny + closeBtnSize - 8);
	line(closeBtnx + 8, closeBtny + closeBtnSize - 8, closeBtnx + closeBtnSize - 8, closeBtny + 8);
}
void draw3DButton(int x, int y, int w, int h, COLORREF color, bool isHover)
{
	COLORREF lightColor = RGB(GetRValue(color) + 30, GetGValue(color) + 30, GetBValue(color) + 30);
	COLORREF darkColor = RGB(GetRValue(color) - 30, GetGValue(color) - 30, GetBValue(color) - 30);

	if (isHover)
	{
		// 鼠标悬停时，反转光影效果
		setfillcolor(darkColor);
		solidrectangle(x, y, x + w, y + h);
		setfillcolor(lightColor);
		solidrectangle(x + 3, y + 3, x + w - 3, y + h - 3);
	}
	else
	{
		setfillcolor(lightColor);
		solidrectangle(x, y, x + w, y + h);
		setfillcolor(darkColor);
		solidrectangle(x + 3, y + 3, x + w - 3, y + h - 3);
	}
}








int main()
{
	
	initgraph(BGWEIGHT, BGHEIGHT);


	//加载图片
	IMAGE img_start;
	loadimage(&img_start, "res/start.jpg", BGWEIGHT, BGHEIGHT);
	const int btnx = 100, btny = 400, btnw = 150, btnh = 70;
	const int menubtny = btny + btnh + 20;

	bool isClicked = false;
	bool showmenu = false;
	while (true)
	{
		// 消息处理
		while (peekmessage(&msg, EX_MOUSE))
		{
			if (msg.message == WM_LBUTTONDOWN)
			{
				if (showmenu)
				{
					// 主界面按钮检测
					if (inarea(msg.x, msg.y, BGWEIGHT - 50 - 20 - 40, 110, 40, 40))
					{
						showmenu = false;
					}
				}
				else
				{
					// 主界面按钮检测
					if (inarea(msg.x, msg.y, btnx, btny, btnw, btnh)) {

						if (!GetTargetScore() || !GetDifficulty()) { 
							MessageBox(GetForegroundWindow(), _T("请输入有效设置！"), _T("错误"), MB_OK);
							continue;
						}
						closegraph();
						isClicked = true;
						start();
						return 0;
					}
					if (inarea(msg.x, msg.y, btnx, menubtny, btnw, btnh)) {
						showmenu = true;
					}
				}
			}
		}

		if (isClicked) break;

		BeginBatchDraw();
		cleardevice();
		putimage(0, 0, &img_start);

		// 绘制开始按钮
		bool isStartHover = inarea(msg.x, msg.y, btnx, btny, btnw, btnh);
		draw3DButton(btnx, btny, btnw, btnh, isStartHover ? BLUE : RED, isStartHover);

		// 绘制菜单按钮（不同样式）
		bool isMenuHover = inarea(msg.x, msg.y, btnx, menubtny, btnw, btnh);
		draw3DButton(btnx, menubtny, btnw, btnh, isMenuHover ? BLUE : RED, isMenuHover);

		// 按钮文字
		settextstyle(30, 0, _T("宋体"));
		setbkmode(TRANSPARENT);
		settextcolor(BLACK);
		outtextxy(btnx + 20, btny + 20, _T("开始游戏"));
		outtextxy(btnx + 20, menubtny + 20, _T("游戏玩法"));

		// 显示玩法菜单
		if (showmenu)
		{
			showgameinstructions();
		}
		EndBatchDraw();
		Sleep(8);
	}

	

	//游戏开始的函数
	
	
	return 0;




}