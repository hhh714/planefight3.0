#include"Airp.h"

ExMessage msg = { 0 };
int difficulty = 1;
bool GetDifficulty()
{
	TCHAR szBuffer[10] = { 0 };
	const TCHAR* prompt = _T("��ѡ���Ѷȣ�\n1. ��\n2. ��ͨ\n3. ����");
	if (InputBox(szBuffer, 10, prompt, _T("�Ѷ�����"), _T("2"), 0, 0, true) == 0)
		return false;

		difficulty = _ttoi(szBuffer);
		if (difficulty < 1 || difficulty > 3) 
		{
			MessageBox(GetForegroundWindow(), _T("������1-3֮������֣�"), _T("����"), MB_OK);
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
	// ���ش�͸��ͨ�����淨����ͼ����ȷ��ͼƬ�ߴ��봰��ƥ�䣩
	loadimage(&img_instruction_bg, _T("res/introduction.png"), BGWEIGHT - 100, BGHEIGHT - 150);

	// ���ư�ɫ���ֲ㣨�޸���ɫ����ؼ���
	setfillcolor(0xDF1A1A1A); // ���ɫ��͸��
	solidrectangle(0, 0, BGWEIGHT, BGHEIGHT);

	// ������ʾ�淨����ͼ���Զ����ţ�
	int x = (BGWEIGHT - img_instruction_bg.getwidth()) / 2;
	int y = (BGHEIGHT - img_instruction_bg.getheight()) / 2;

	// ���Ʊ�����Ӱ
	setfillcolor(0x80000000);
	fillroundrect(x - 10, y - 10,
		x + img_instruction_bg.getwidth() + 10,
		y + img_instruction_bg.getheight() + 10,
		15, 15);

	// ���ƴ�͸��ͨ������Ϸ˵��ͼ
	putimage_withalpha(NULL, img_instruction_bg, x, y);

	// �������֣���ɫ����Ӱ��
	settextstyle(36, 0, _T("΢���ź�"));
	settextcolor(0x222222);
	outtextxy(x + 18, y + 18, _T("��Ϸ˵��"));
	settextcolor(0xFFFFFF);
	outtextxy(x + 15, y + 15, _T("��Ϸ˵��"));

	// �������ݣ�����ɫ��
	settextstyle(22, 0, _T("΢���ź�"));
	
	settextcolor(BLUE);
	const TCHAR* instructions =
		_T("������˵����\n")
		_T("�ƶ����ƣ�W/A/S/D �����\n")
		_T("������ո��\n\n")
		_T("����ϷĿ�꡿\n")
		_T("1. ����л���÷���\n")
		_T("2. ��ܵл�������ײ\n")
		_T("3. ���Ŀ�������ӭսBOSS");

	RECT textRect = { x + 40, y + 80,
					 x + img_instruction_bg.getwidth() - 40,
					 y + img_instruction_bg.getheight() - 40 };
	drawtext(instructions, &textRect, DT_WORDBREAK | DT_LEFT);

	// �رհ�ť����ɫ���
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
		// �����ͣʱ����ת��ӰЧ��
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


	//����ͼƬ
	IMAGE img_start;
	loadimage(&img_start, "res/start.jpg", BGWEIGHT, BGHEIGHT);
	const int btnx = 100, btny = 400, btnw = 150, btnh = 70;
	const int menubtny = btny + btnh + 20;

	bool isClicked = false;
	bool showmenu = false;
	while (true)
	{
		// ��Ϣ����
		while (peekmessage(&msg, EX_MOUSE))
		{
			if (msg.message == WM_LBUTTONDOWN)
			{
				if (showmenu)
				{
					// �����水ť���
					if (inarea(msg.x, msg.y, BGWEIGHT - 50 - 20 - 40, 110, 40, 40))
					{
						showmenu = false;
					}
				}
				else
				{
					// �����水ť���
					if (inarea(msg.x, msg.y, btnx, btny, btnw, btnh)) {

						if (!GetTargetScore() || !GetDifficulty()) { 
							MessageBox(GetForegroundWindow(), _T("��������Ч���ã�"), _T("����"), MB_OK);
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

		// ���ƿ�ʼ��ť
		bool isStartHover = inarea(msg.x, msg.y, btnx, btny, btnw, btnh);
		draw3DButton(btnx, btny, btnw, btnh, isStartHover ? BLUE : RED, isStartHover);

		// ���Ʋ˵���ť����ͬ��ʽ��
		bool isMenuHover = inarea(msg.x, msg.y, btnx, menubtny, btnw, btnh);
		draw3DButton(btnx, menubtny, btnw, btnh, isMenuHover ? BLUE : RED, isMenuHover);

		// ��ť����
		settextstyle(30, 0, _T("����"));
		setbkmode(TRANSPARENT);
		settextcolor(BLACK);
		outtextxy(btnx + 20, btny + 20, _T("��ʼ��Ϸ"));
		outtextxy(btnx + 20, menubtny + 20, _T("��Ϸ�淨"));

		// ��ʾ�淨�˵�
		if (showmenu)
		{
			showgameinstructions();
		}
		EndBatchDraw();
		Sleep(8);
	}

	

	//��Ϸ��ʼ�ĺ���
	
	
	return 0;




}