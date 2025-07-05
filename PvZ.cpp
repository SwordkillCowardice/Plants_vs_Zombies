// OrionZhangд��ֲ���ս��ʬ
#include <graphics.h> // ͼ�ο�
#include <mmsystem.h> // ��ý�岥��
#include <stdio.h> 
#include <time.h> // ���������
#include "tools.h" // Rock��ʦд�ĸ�������
#include "vector2.h" // ͬ��
#pragma comment(lib, "MSIMG32.LIB") // ͸����ͼ��Ҫ
#pragma comment(lib, "winmm.lib") // ��ý�岥��

// ���峣��
#define LENGTH 64 // ��󳤶�:�ļ���/��������
#define FLUSH_TIME 10 // ������ˢ�¼��
#define WIN_WIDTH 900 // ��Ϸ�������
#define WIN_HEIGHT 600 // �������
#define FINAL_PAINTX -112 // ������������X
#define BUTTON_STARTX 474 // ��ʼ��ť������ʼX
#define BUTTON_STARTY 75 // ��ʼ��ť������ʼY
#define BAR_STARTX 148 // ֲ�￨�����������X
#define CARD_WIDTH 65 // ֲ�￨�ƿ��
#define CARD_HEIGHT 90 // ֲ�￨�Ƹ߶�
#define CARDS_STARTX 236 // ֲ�￨�ƻ������X
#define CARDS_STARTY 6 // ֲ�￨�ƻ������Y
#define GRASS_STARTX 140 // ��ƺ����ʼX
#define GRASS_STARTY 172 // ��ƺ����ʼY
#define GRASSBLOCK_WIDTH 82 // ��ƺ����
#define GRASSBLOCK_HEIGHT 104 // ��ƺ��߶�
#define GRASSROW 3 // ��ƺ������
#define GRASSCOL 9 // ��ƺ������
#define SUNSHINE_COUNT 15 // �������������������:�൱����ǰ������һ������Ŀ�����Ҫ��ʱֱ�Ӵ���ȡ��ʹ�����ٷŻ�;��������Ҫ��ʱ�ٴ���
#define SUNSHINE_VALUE 25 // �ռ�����������������ֵ
#define TEXT_STARTX 161 // ������ֵ������ʼX
#define TEXT_STARTY 72 // ����ֵ������ʼY
#define ZOMBIE_COUNT 12 // ��ʬ���н�ʬ����
#define ZOMBIE_HURT 2 // ��ʬ�˺�
#define BULLET_COUNT 30 // �ӵ������ӵ���
#define BULLET_HURT 10 // �ӵ��˺�

// ����ֲ����ֲ�ṹ
typedef struct
{
	int type; // ֲ�����ͣ�0����δ��ֲ��1-8����ֲ�����
	int x, y; // ��Ӧ��ƺ�����ʼx��y
	int frameIndex; // ֲ�����Ⱦ֡
	int blood; // ֲ���;ö�
	bool isEaten; // �Ƿ��Ѿ��ڱ���
	int timer; // ���ڿ������տ���������
	int shoottime; // ���ڿ����㶹�����ӵ�
} Plants;
Plants plants[GRASSROW][GRASSCOL]; // ֲ����ֲ���飬����ֲ����Ⱦ
enum{ZERO, XRK, WD, DZH, YT, LJ, JG, WG, TD}; // ֲ������ö��

// ������컨
typedef struct
{
	bool used; // �Ƿ���ʹ��
	int pos; // ��ֲλ��
	int frameIndex; // ֡
	int state; // ����ʲô״̬
	int eatTime; // ���˶��
} DaZuiHua;
DaZuiHua dzh[27]; // ���컨��
enum {NONE, NORMAL, ATTACK, EAT}; // ���컨��̬��δ����̬����̬���������׽�̬
IMAGE normalDZH[8]; // ���컨��ͨ֡
IMAGE attackDZH[8]; // ���컨����֡
IMAGE eatDZH[5]; // ���컨�׽�֡

// ����ӣ��ը��
typedef struct
{
	bool used; // �Ƿ�����
	int pos; // ������
	int frameIndex; // ֡
	int state; // ״̬
} CherryBomb;
CherryBomb crb[27]; // ӣ��ը����
enum{CNONE, BOMB, REST}; // ӣ��ը����״̬��δ���塢��ը̬���ҽ�̬
IMAGE bombCB[7]; // ӣ��ը����ը֡
IMAGE restCB[12]; // ӣ��ը����ը���֡

// ���������
typedef struct
{
	bool used; // �Ƿ�����
	int pos; // ������
	int frameIndex; // ֡
	int state; // ״̬
} HUOBLJ;
HUOBLJ hblj[27]; // ��������
enum {HNONE, HBOMB, HREST }; // ��������״̬��δ���塢��ը̬���ҽ�̬
IMAGE bombHB[8]; // ��������ը֡
IMAGE restHB[8]; // ��������ը���֡

// ������������
typedef struct
{
	bool used; // �Ƿ�����
	int pos; // ������
	int frameIndex; // ֡
	int state; // ״̬
} PotatoMine;
PotatoMine pm[27]; // �������׳�
enum {PNONE, PHIDE, PNORMAL, PREST }; // ����������״̬��δ���塢����̬����̬ͨ���ҽ�̬
IMAGE hidePM[20]; // ��������
IMAGE normalPM[8]; // ����������ͨ֡
IMAGE restPM[6]; // �������ױ�ը�ҽ�֡

// �����ѹ�
typedef struct
{
	bool used; // �Ƿ�����
	int pos; // ������
	int frameIndex; // ֡
	int state; // ״̬
} WOGUA;
WOGUA wg[27]; // �ѹϳ�
enum {WNONE, WNORMAL, WATTACK}; //�ѹ���״̬��δ���塢����̬������̬
IMAGE normalWG[17]; // �ѹ���ͨ֡
IMAGE attackWG[4]; // �ѹϹ���֡

// ��������
typedef struct SUNSHIEN
{
	bool used; // �Ƿ���ʹ����(������)
	int frameIndex; // ��Ⱦ֡
	float t; // ���������ߵĲ�����ȡֵ0-1������������λ��
	vector2 p1, p2, p3, p4; // ���������ߵ��ĸ����Ƶ�
	vector2 pCur; // �����ر����������˶���ʵʱλ��
	float speed; // �������˶��ٶȣ�����tֵ�仯
	int state; // ������ǰ״̬
	int time; // ����Ŀ�ĵغ�����ʱ��
} sunShine;
sunShine sunshine[SUNSHINE_COUNT]; // �����
IMAGE imgSunshine[29]; // ����֡����
enum {SUNSHINE_DOWN, SUNSHINE_STAY, SUNSHINE_COL, SUNSHINE_GEN}; // ������ĸ�״̬����ֱ�½������������ռ������տ�����
int sunshineCost[8] = { 50, 100, 150, 150, 125, 50, 50, 25 }; // ÿ��ֲ�����������

// ���彩ʬ
typedef struct Zombie
{
	bool used; // �Ƿ���ʹ��
	int x, y; // ��ʬʵʱλ��
	int row; // ��ʬ�����к�
	int col; // ��ʬ�����к�
	int frameIndex; // ��Ⱦ֡
	int speed; // �ٶ�
	int blood; // Ѫ��
	bool dead; // ��ʬ�Ƿ�����
	bool eat; // ��ʬ�Ƿ��ڳ�ֲ��
	int eatwho; // ��ʬ�ڳ���һ��
} Zombie;
Zombie zombies[ZOMBIE_COUNT]; // ��ʬ��
IMAGE imgWalkZombies[22]; // ��ʬ����֡
IMAGE imgDeadZombies[20]; // ��ʬ����֡
IMAGE imgEatZombies[21]; // ��ʬ��֡
IMAGE imgStandZombies[11]; // ��ʬվ��֡

// �����ӵ�
typedef struct bo
{
	bool used; // �Ƿ���ʹ��
	int x, y; // �ӵ�λ��
	int row; // �ӵ������к�
	int speed; // �ӵ������ٶ�
	bool blast; // �Ƿ��ڱ�ը״̬
	int blastFrame; // ��ը֡
} Bullet;
Bullet bullets[BULLET_COUNT]; // �ӵ���
IMAGE bulletNormal; // �ӵ�����
IMAGE bulletBlast[4]; // �ӵ���ը֡

// ��Ϸ����״̬
enum {WIN, LOSE, RUN};

// ȫ�ֱ���������
IMAGE imgBk; // ����������ͼ
IMAGE imgBr; // ֲ�￨����
IMAGE imgCards[8]; // ֲ�￨��ͼ��
IMAGE* imgPlants[3][8]; // ֲ��ͼƬָ�����飬[i][j]��Ӧ��i+1��ֲ��ĵ�j+1֡

int gameState = RUN; // ��Ϸ��ǰ״̬
int curX, curY; // ��굱ǰλ��
int selIndex = 0; // ��ǰ��ѡ�е�ֲ�����(1-8)��0����δѡ��
int sunshineValue = 50; // ��ʼ����ֵ
int killCount = 0; // ��ǰ��ɱ�Ľ�ʬ��
int zmCount = 0; // ��ǰ�����ɵĽ�ʬ����

// ��������
void getImage(); // ����ͼƬ
void initialize(); // ��Ŀ��ʼ��
void transparentimage(IMAGE* dstimg, int x, int y, IMAGE* srcimg); // ͸����ͼ
void start(); // ��ʼ����
void patrol(); // ����Ѳ��
void bardown(); // ����������
void collectSunshine(ExMessage* msg); // �ռ�����
void createDZH(int pos); // �������컨
void createCB(int pos); // ����ӣ��ը��
void createHB(int pos); // ����������
void createPM(int pos); // ������������
void createWG(int pos); // �����ѹ�
void userControl(); // ��ҿ���
void drawDZH(int x, int y); // ���ƴ��컨
void drawCB(int x, int y); // ����ӣ��ը��
void drawHB(int x, int y); // ���ƻ�����
void drawPM(int x, int y); // ������������
void drawWG(int x, int y); // �����ѹ�
void drawPlants(); // ����ֲ�����
void drawZombies(); // ���ƽ�ʬ
void drawSunshine(); // �����������
void drawBullets(); // �����ӵ�
void drawScr(); // ��������Ⱦ
void updateDZH(int x, int y); // ���´��컨 !!! һ��Ҫ�ô�����x,y�ĸ��£��漰���µ��߼�
void updateCB(int x, int y); // ����ӣ��ը��
void updateHB(int x, int y); // ���»�����
void updatePM(int x, int y); // ������������
void updateWG(int x, int y); // �����ѹ�
void createSunshine(); // ��������
void updateSunshine(); // ��������
void createZombies(); // ������ʬ
void updateZombies(); // ���½�ʬ
void shoot(); // �ӵ�����
void updateBullets(); // �����ӵ�
void checkZombiestoBullets(); // ��⽩ʬ���ӵ�
void checkZombiestoPlants(); // ��⽩ʬ��ֲ��
void checkZombiestoDZH(); // ��⽩ʬ�ʹ��컨
void cbBomb(); // ӣ��ը����ը
void hbBomb(); // ��������ը
void pmBomb(); // �������ױ�ը
void wgAttack(); // �ѹϹ���
void collisionCheck(); // �������
void updateGame(); // ������Ϸ����
bool checkOver(); // �����Ϸ���

// ������
int main(void)
{
	initialize(); // ��Ŀ��ʼ��
	start(); // ��ʼ����
	patrol(); // Ѳ��
	bardown(); // ����������

	int mainTime = 0; // �����ʱ����,���ڻ������
	
	while (true) // ��������
	{
		mainTime += getDelay(); // �Գ���������ʱ������ۼ�
		userControl(); // ��ҿ���

		if (mainTime > FLUSH_TIME) // ����ˢ��ʱ��󣬼�ʱ����
		{
			mainTime = 0;
			drawScr(); // ��������Ⱦ
			updateGame(); // �޸���Ϸ�������(��������)
			if (checkOver()) // �ж���Ϸ����
				break;
		}
	}
}

// ����ͼƬ
void getImage()
{
	// ��������ͼƬ
	loadimage(&imgBk, "res\\bg.jpg"); // ������ͼ
	loadimage(&imgBr, "res\\bar5.png"); // ֲ�￨����

	char path[LENGTH]; // �����ļ�·��
	for (int i = 0; i < 8; i++) // ����ֲ�￨��
	{
		snprintf(path, sizeof(path), "res\\Cards\\card_%d.png", i + 1);
		loadimage(&imgCards[i], path);
	}

	for (int i = 0; i < 2; i++) // ������������տ�
	{
		for (int j = 0; j < 8; j++)
		{
			snprintf(path, sizeof(path), "res\\zhiwu\\%d\\%d.png", i, j + 1);
			imgPlants[i][j] = new IMAGE;
			loadimage(imgPlants[i][j], path);
		}
	}
	
	for (int i = 0; i < 8; i++) // ���ؼ��
	{
		snprintf(path, sizeof(path), "res\\zhiwu\\5\\%d.png", i + 1);
		imgPlants[2][i] = new IMAGE;
		loadimage(imgPlants[2][i], path);
	}

	for (int i = 0; i < 8; i++) // ���س�̬���컨
	{
		snprintf(path, sizeof(path), "res\\zhiwu\\2\\%d.png", i + 1);
		loadimage(&normalDZH[i], path);
	}

	for (int i = 0; i < 8; i++) // ���ع���̬���컨
	{
		snprintf(path, sizeof(path), "res\\Plants\\1Chomper\\ChomperAttack.png\\%d.png", i);
		loadimage(&attackDZH[i], path);
	}

	for (int i = 0; i < 5; i++) // ���ؾ׽�̬���컨
	{
		snprintf(path, sizeof(path), "res\\Plants\\1Chomper\\ChomperDigest.png\\%d.png", i);
		loadimage(&eatDZH[i], path);
	}

	for (int i = 0; i < 7; i++) // ���ر�ը̬ӣ��ը��
	{
		snprintf(path, sizeof(path), "res\\Plants\\2CherryBomb\\1.png\\%d.png", i + 1);
		loadimage(&bombCB[i], path);
	}

	for (int i = 0; i < 12; i++) // �������̬ӣ��ը��
	{
		snprintf(path, sizeof(path), "res\\Plants\\2CherryBomb\\Boom.png\\%d.png", i + 1);
		loadimage(&restCB[i], path);
	}

	for (int i = 0; i < 8; i++) // ���ر�ը̬������
	{
		snprintf(path, sizeof(path), "res\\zhiwu\\4\\%d.png", i + 1);
		loadimage(&bombHB[i], path);
	}

	for (int i = 0; i < 8; i++) // �������̬������
	{
		snprintf(path, sizeof(path), "res\\Plants\\3Jalapeno\\JalapenoAttack.png\\%d.png", i + 1);
		loadimage(&restHB[i], path);
	}

	for (int i = 0; i < 20; i++)  // ��������̬
		loadimage(&hidePM[i], "res\\Plants\\4PotatoMine\\1.png");

	for (int i = 0; i < 8; i++) // ���س�̬��������
	{
		snprintf(path, sizeof(path), "res\\zhiwu\\7\\%d.png", i + 1);
		loadimage(&normalPM[i], path);
	}

	for (int i = 0; i < 3; i++) // �������̬��������
	{
		loadimage(&restPM[i], "res\\Plants\\4PotatoMine\\2.png"); 
		loadimage(&restPM[i + 3], "res\\Plants\\4PotatoMine\\3.png");
	}

	for (int i = 0; i < 17; i++) // ���س�̬�ѹ�
	{
		snprintf(path, sizeof(path), "res\\zhiwu\\6\\%d.png", i + 1);
		loadimage(&normalWG[i], path);
	}

	for (int i = 0; i < 4; i++) // ���ع���̬�ѹ�
	{
		snprintf(path, sizeof(path), "res\\Plants\\5Squash\\SquashAttack.png\\%d.png", i + 1);
		loadimage(&attackWG[i], path);
	}

	for (int i = 0; i < 29; i++) // ��������֡
	{
		snprintf(path, sizeof(path), "res\\sunshine\\%d.png", i + 1);
		loadimage(&imgSunshine[i], path);
	}

	for (int i = 0; i < 22; i++) // ���ؽ�ʬ����֡
	{
		snprintf(path, sizeof(path), "res\\zm\\%d.png", i + 1);
		loadimage(&imgWalkZombies[i], path);
	}

	for (int i = 0; i < 20; i++) // ���ؽ�ʬ����֡
	{
		snprintf(path, sizeof(path), "res\\zm_dead\\%d.png", i + 1);
		loadimage(&imgDeadZombies[i], path);
	}

	for (int i = 0; i < 21; i++) // ���ؽ�ʬ��֡
	{
		snprintf(path, sizeof(path), "res\\zm_eat\\%d.png", i + 1);
		loadimage(&imgEatZombies[i], path);
	}

	for (int i = 0; i < 11; i++) // ���ؽ�ʬվ��֡
	{
		snprintf(path, sizeof(path), "res\\zm_stand\\%d.png", i + 1);
		loadimage(&imgStandZombies[i], path);
	}

	loadimage(&bulletNormal, "res\\bullets\\bullet_normal.png"); // ���س�̬�ӵ�

	loadimage(&bulletBlast[3], "res\\bullets\\bullet_blast.png"); // ���ر�ը�ӵ�,��������ʵ�ֱ�ըЧ��
	for (int i = 0; i < 3; i++)
	{
		float factor = (i + 1) * 0.25;
		loadimage(&bulletBlast[i], "res\\bullets\\bullet_blast.png", bulletBlast[0].getwidth() * factor, bulletBlast[0].getheight() * factor, true);
	}
}

// ��Ŀ��ʼ��
void initialize()
{
	// �ڴ��ʼ��
	memset(imgPlants, 0, sizeof(imgPlants)); // ֲ��ָ��
	memset(plants, 0, sizeof(plants)); // ֲ����ֲ
	memset(dzh, 0, sizeof(dzh)); // ���컨��
	memset(crb, 0, sizeof(crb)); // ӣ��ը����
	memset(hblj, 0, sizeof(hblj)); // ��������
	memset(pm, 0, sizeof(pm)); // �������׳�
	memset(wg, 0, sizeof(wg)); // �ѹϳ�
	memset(sunshine, 0, sizeof(sunshine)); // �����
	memset(zombies, 0, sizeof(zombies)); // ��ʬ��
	memset(bullets, 0, sizeof(bullets)); // �ӵ��س�ʼ��

	// ����ͼƬ
	getImage();

	// ��ʼ�����������
	srand(time(NULL));

	// ���ڳ�ʼ��
	initgraph(WIN_WIDTH, WIN_HEIGHT);

	// ��������
	setbkmode(TRANSPARENT);
	settextstyle(30, 0, "Segoe UI Black");
	settextcolor(BLACK);
}

// ͸����ͼ
void transparentimage(IMAGE* dstimg, int x, int y, IMAGE* srcimg) 
{
	HDC dstDC = GetImageHDC(dstimg);
	HDC srcDC = GetImageHDC(srcimg);
	int w = srcimg->getwidth();
	int h = srcimg->getheight();
	BLENDFUNCTION bf = { AC_SRC_OVER, 0, 255, AC_SRC_ALPHA };
	AlphaBlend(dstDC, x, y, w, h, srcDC, 0, 0, w, h, bf);
}

// ʵ�ֿ�ʼ����:ע��Ҫ���������ֿ����ƣ������������Ĳ���
void start()
{
	// ���忪ʼҳ�汳��ͼ��������ʼ��ť����ť���±�־�����ֲ��ű�־
	IMAGE bkMn, bkStart1, bkStart2;
	bool isPressed = false;
	bool isPlayed = false;

	// ��������ͼƬ
	loadimage(&bkMn, "res\\menu.png");
	loadimage(&bkStart1, "res\\menu1.png");
	loadimage(&bkStart2, "res\\menu2.png");

	// ���ϻ�ȡ��Ϣ�����л�����ƣ�ֱ����ʼ��ť����
	while (1)
	{
		// ����
		BeginBatchDraw();
		transparentimage(NULL, 0, 0, &bkMn);
		transparentimage(NULL, BUTTON_STARTX, BUTTON_STARTY, isPressed ? &bkStart1 : &bkStart2);
		EndBatchDraw();

		// ��������
		if (isPlayed == false)
		{
			mciSendString("play res\\bg.mp3 repeat", NULL, 0, NULL);
			isPlayed = true;
		}

		// ��Ϣ��ȡ
		ExMessage msg;
		if (peekmessage(&msg))
		{
			// ������°�ť���ɿ�����������Ϸ
			if (msg.message == WM_LBUTTONDOWN && msg.x >= BUTTON_STARTX && msg.x < BUTTON_STARTX + bkStart1.getwidth() && msg.y >= BUTTON_STARTY && msg.y < BUTTON_STARTY + bkStart2.getheight())
				isPressed = true;
			else if (msg.message == WM_LBUTTONUP && isPressed)
			{
				mciSendString("play res\\zz_audio\\evillaugh.mp3", NULL, 0, NULL);
				Sleep(3100); // �����ַ�����
				return;
			}
		}
	}
}

// ������Ѳ��
void patrol()
{
	 // ����Ѳ��ʬ��,��ʬѲ��������λ,���Ե�֡����
	const int count = 9; // C++֧�֣���C��֧����const������ʼ������
	vector2 points[count] = {{550, 80}, {530, 160}, {630, 170}, {530, 200}, {515, 270}, {565, 370}, {605, 340}, {690, 340}, {705, 280}};
	int index[count];
	for (int i = 0; i < count; i++) // ���ȷ��ĳ��ʬ����֡
		index[i] = rand() % 11;

	int times = 0; // ��������������֡�л�Ƶ��
	int changeFrame = 5; // ֡�л�Ƶ��
	int widthdif = -500; // ���ӿڿ��뻭���ܿ�֮��
	int wspeed = 2; // �����ƶ��ٶ�

	// ���λ���
	BeginBatchDraw();
	putimage(0, 0, &imgBk); // ���Ʊ���ͼ
	EndBatchDraw();
	mciSendString("stop res\\bg.mp3", NULL, 0, NULL); // ֹͣ���֣�׼��Ѳ��
	mciSendString("play res\\LookupattheSky.mp3", NULL, 0, NULL); // ������һ��
	Sleep(1000);

	for (int i = 0; i >= widthdif; i -= wspeed) // Ѳ��
	{
		times++; // ���¼�������
		BeginBatchDraw(); 
		putimage(i, 0, &imgBk); // ���Ʊ���ͼ
		for (int k = 0; k < count; k++) // ���ƽ�ʬ
		{
			transparentimage(NULL, points[k].x - widthdif + i, points[k].y, &imgStandZombies[index[k]]);
			if (times >= changeFrame) // ����֡��ע�ⲻ�������ｫtimes���㣬�����еĽ�ʬ֡���£����е�û��
				index[k] = (index[k] + 1) % 11;
		}
		if (times >= changeFrame)
			times = 0;
		EndBatchDraw();
		Sleep(5); // �����������
	}

	int stayFrame = 30; // פ��֡
	for (int i = 0; i < stayFrame; i++) // Ѳ����е���ʬ��·�󣬻������פ��
	{
		BeginBatchDraw();
		putimage(widthdif, 0, &imgBk);
		for (int k = 0; k < count; k++)
		{
			transparentimage(NULL, points[k].x, points[k].y, &imgStandZombies[index[k]]);
			index[k] = (index[k] + 1) % 11;
		}
		EndBatchDraw();
		Sleep(50);
	}

	times = 0; // ������������
	for (int i = widthdif; i <= FINAL_PAINTX; i += wspeed) // Ѳ������
	{
		times++;
		BeginBatchDraw();
		putimage(i, 0, &imgBk);
		for (int k = 0; k < count; k++)
		{
			transparentimage(NULL, points[k].x - widthdif + i, points[k].y, &imgStandZombies[index[k]]);
			if (times >= changeFrame)
				index[k] = (index[k] + 1) % 11;
		}
		if (times >= changeFrame)
			times = 0;
		EndBatchDraw();
		Sleep(5);
	}
}

// ����������
void bardown()
{
	// ���忨�����߶Ⱥ��»��ٶ�
	int barHeight = 100;
	int speed = 2;

	for (int y = -barHeight; y <= 0; y += speed)
	{
		BeginBatchDraw();
		putimage(FINAL_PAINTX, 0, &imgBk); // ����������ͼ
		transparentimage(NULL, BAR_STARTX, y, &imgBr); // ����ֲ����
		for (int i = 0; i < 8; i++) // ���ƿ���
			transparentimage(NULL, CARDS_STARTX + i * CARD_WIDTH, CARDS_STARTY + y, &imgCards[i]);
		EndBatchDraw();
		Sleep(5);
	}
}

// �ռ�����
void collectSunshine(ExMessage* msg)
{
	// ��������أ��ж�����Ƿ�����ĳ������ʹ�õ�������
	for (int i = 0; i < SUNSHINE_COUNT; i++)
	{
		if (sunshine[i].used && !(sunshine[i].state == SUNSHINE_COL))
		{
			if (msg->x >= sunshine[i].pCur.x && msg->y >= sunshine[i].pCur.y && msg->x < sunshine[i].pCur.x + imgSunshine[i].getwidth() && msg->y < sunshine[i].pCur.y + imgSunshine[i].getheight())
			{
				sunshine[i].state = SUNSHINE_COL;
				sunshine[i].p1 = sunshine[i].pCur;
				sunshine[i].p4 = vector2(BAR_STARTX, 0);
				sunshine[i].t = 0;
				float v = 10;
				sunshine[i].speed = v / dis(sunshine[i].p1 - sunshine[i].p4);
				PlaySound("res\\sunshine.wav", NULL, SND_FILENAME | SND_ASYNC);
				break; // ���ε�����ռ�һ������
			}
		}
	}
}

// �������컨
void createDZH(int pos)
{
	// �������컨��, ��һ��δ��ʹ�õ�
	int i;
	for (i = 0; i < 27 && dzh[i].used; i++);
	if (i < 27)
	{
		dzh[i].used = true;
		dzh[i].state = NORMAL;
		dzh[i].eatTime = 0;
		dzh[i].pos = pos;
		dzh[i].frameIndex = 0;
	}
	else
		return;
}

// ����ӣ��ը��
void createCB(int pos)
{
	// ����ӣ��ը����, ��һ��δ��ʹ�õ�
	int i;
	for (i = 0; i < 27 && crb[i].used; i++);
	if (i < 27)
	{
		crb[i].used = true;
		crb[i].state = BOMB;
		crb[i].pos = pos;
		crb[i].frameIndex = 0;
	}
	else
		return;
}

// ����������
void createHB(int pos)
{
	// ������������, ��һ��δ��ʹ�õ�
	int i;
	for (i = 0; i < 27 && hblj[i].used; i++);
	if (i < 27)
	{
		hblj[i].used = true;
		hblj[i].state = HBOMB;
		hblj[i].pos = pos;
		hblj[i].frameIndex = 0;
	}
	else
		return;
}

// ������������
void createPM(int pos)
{
	// �����������׳�, ��һ��δ��ʹ�õ�
	int i;
	for (i = 0; i < 27 && pm[i].used; i++);
	if (i < 27)
	{
		pm[i].used = true;
		pm[i].state = PHIDE;
		pm[i].pos = pos;
		pm[i].frameIndex = 0;
	}
	else
		return;
}

// �����ѹ�
void createWG(int pos)
{
	// �����ѹϳ�, ��һ��δ��ʹ�õ�
	int i;
	for (i = 0; i < 27 && wg[i].used; i++);
	if (i < 27)
	{
		wg[i].used = true;
		wg[i].state = WNORMAL;
		wg[i].pos = pos;
		wg[i].frameIndex = 0;
	}
	else
		return;
}

// ��ҿ���
void userControl()
{
	// ������Ϣ�Լ����״̬
	ExMessage msg;
	static int state = 0; // ʹ�þֲ���̬��������Ϊ��������᲻�ϵ��ã�����һ�ֵ�״̬��Ӱ����һ�ֵ��ж�������֮����״̬

	// ���Ի�ȡ��Ϣ,������Ϣ����ִ�в���
	if (peekmessage(&msg))
	{
		// �������
		if (msg.message == WM_LBUTTONDOWN)
		{
			// �ж��Ƿ���ֲ�￨��������:����ҿ����ϱ��¿�
			if (msg.x >= CARDS_STARTX && msg.x < CARDS_STARTX + CARD_WIDTH * 8 && msg.y >= CARDS_STARTY && msg.y < CARDS_STARTY + CARD_HEIGHT)
			{
				selIndex = (msg.x - CARDS_STARTX) / CARD_WIDTH + 1; // ��ѡ�е�ֲ�����
				if (sunshineValue >= sunshineCost[selIndex - 1]) // �ж�����ֵ�Ƿ���
				{
					state = 1; // ����״̬
					curX = msg.x;
					curY = msg.y;
				}
				else
					selIndex = 0;
			}
			// �ռ�����
			else
				collectSunshine(&msg);
		}

		// ����ƶ�
		else if (msg.message == WM_MOUSEMOVE)
		{
			// �϶�״̬,�������λ��
			if (state == 1)
			{
				curX = msg.x;
				curY = msg.y;
			}
		}

		// ���̧��
		else if (msg.message == WM_LBUTTONUP)
		{
			// �ж��Ƿ����ڲ�ƺ�������ڣ����жϾ���λ�ã����޸�ָ��λ��ֲ����ֲ���
			if (msg.x >= GRASS_STARTX && msg.x < GRASS_STARTX + GRASSCOL * GRASSBLOCK_WIDTH && msg.y >= GRASS_STARTY && msg.y < GRASS_STARTY + GRASSROW * GRASSBLOCK_HEIGHT)
			{
				// ����������к�
				int row = (msg.y - GRASS_STARTY) / GRASSBLOCK_HEIGHT;
				int col = (msg.x - GRASS_STARTX) / GRASSBLOCK_WIDTH;

				// ��õ�ǰ��ƺ����ʼx,yֵ
				plants[row][col].x = GRASS_STARTX + col * GRASSBLOCK_WIDTH;
				plants[row][col].y = GRASS_STARTY + row * GRASSBLOCK_HEIGHT;

				// ��λ����ֲ����ֲʱ����ֲ������
				if (plants[row][col].type == ZERO)
				{
					if (selIndex > 0)
					{
						// ���и���
						plants[row][col].type = selIndex;
						plants[row][col].blood = (selIndex == JG ? 2000 : 400);
						sunshineValue = sunshineValue - sunshineCost[selIndex - 1];

						// ����
						if (selIndex == DZH)
							createDZH(row * 10 + col);
						else if (selIndex == YT)
							createCB(row * 10 + col);
						else if (selIndex == LJ)
							createHB(row * 10 + col);
						else if (selIndex == TD)
							createPM(row * 10 + col);
						else if (selIndex == WG)
							createWG(row * 10 + col);
						
						// ������Ч
						PlaySound("res\\plant2.wav", NULL, SND_FILENAME | SND_ASYNC);
					}
				}
			}

			// �����϶�״̬�����㱻ѡ�е�ֲ�����
			state = 0;
			selIndex = 0;
		}
	}
}

// ���ƴ��컨
void drawDZH(int x, int y)
{
	for (int i = 0; i < 27; i++)
	{
		if (dzh[i].used && dzh[i].pos == x * 10 + y)
		{
			// ���λ��
			int xpos = plants[x][y].x + GRASSBLOCK_WIDTH / 2 - normalDZH[0].getwidth() / 2;
			int ypos = plants[x][y].y + GRASSBLOCK_HEIGHT / 2 - normalDZH[0].getheight() / 2;

			// ���ݲ�ͬ״̬���л���
			if (dzh[i].state == NORMAL)
				transparentimage(NULL, xpos, ypos, &normalDZH[dzh[i].frameIndex]);
			else if (dzh[i].state == ATTACK)
				transparentimage(NULL, xpos, ypos, &attackDZH[dzh[i].frameIndex]);
			else if (dzh[i].state == EAT)
				transparentimage(NULL, xpos, ypos, &eatDZH[dzh[i].frameIndex]);
			break; // �ҵ�ȷ��λ��ȥ��ֲ
		}
	}
}

// ����ӣ��ը��
void drawCB(int x, int y)
{
	for (int i = 0; i < 27; i++)
	{
		if (crb[i].used && crb[i].pos == x * 10 + y)
		{
			// ���λ��
			int xpos = plants[x][y].x + GRASSBLOCK_WIDTH / 2 - bombCB[0].getwidth() / 2;
			int ypos = plants[x][y].y + GRASSBLOCK_HEIGHT / 2 - bombCB[0].getheight() / 2;

			// ���ݲ�ͬ״̬���л���
			if (crb[i].state == BOMB)
				transparentimage(NULL, xpos, ypos, &bombCB[crb[i].frameIndex]);
			else if (crb[i].state == REST)
				transparentimage(NULL, xpos - 50, ypos - 50, &restCB[crb[i].frameIndex]);
			break;
		}
	}
}

// ���ƻ�����
void drawHB(int x, int y)
{
	for (int i = 0; i < 27; i++)
	{
		if (hblj[i].used && hblj[i].pos == x * 10 + y)
		{
			// ���λ��
			int xpos = plants[x][y].x + GRASSBLOCK_WIDTH / 2 - bombHB[0].getwidth() / 2;
			int ypos = plants[x][y].y + GRASSBLOCK_HEIGHT / 2 - bombHB[0].getheight() / 2;

			// ���ݲ�ͬ״̬���л���
			if (hblj[i].state == HBOMB)
				transparentimage(NULL, xpos, ypos, &bombHB[hblj[i].frameIndex]);
			else if (hblj[i].state == HREST)
				transparentimage(NULL, GRASS_STARTX, ypos - 60, &restHB[hblj[i].frameIndex]);
			break;
		}
	}
}

// ������������
void drawPM(int x, int y)
{
	for (int i = 0; i < 27; i++)
	{
		if (pm[i].used && pm[i].pos == x * 10 + y)
		{
			// ���λ��
			int xpos = plants[x][y].x + GRASSBLOCK_WIDTH / 2 - normalPM[0].getwidth() / 2;
			int ypos = plants[x][y].y + GRASSBLOCK_HEIGHT / 2 - normalPM[0].getheight() / 2;

			// ���ݲ�ͬ״̬���л���
			if (pm[i].state == PHIDE)
				transparentimage(NULL, xpos, ypos, &hidePM[pm[i].frameIndex]);
			else if (pm[i].state == PNORMAL)
				transparentimage(NULL, xpos, ypos, &normalPM[pm[i].frameIndex]);
			else if (pm[i].state == PREST)
				transparentimage(NULL, xpos - 15, ypos, &restPM[pm[i].frameIndex]);
			break;
		}
	}
}

// �����ѹ�
void drawWG(int x, int y)
{
	for (int i = 0; i < 27; i++)
	{
		if (wg[i].used && wg[i].pos == x * 10 + y)
		{
			// ���λ��
			int xpos = plants[x][y].x + GRASSBLOCK_WIDTH / 2 - normalWG[0].getwidth() / 2;
			int ypos = plants[x][y].y + GRASSBLOCK_HEIGHT / 2 - normalWG[0].getheight() / 2;

			// ���ݲ�ͬ״̬���л���
			if (wg[i].state == WNORMAL)
				transparentimage(NULL, xpos, ypos, &normalWG[wg[i].frameIndex]);
			else if (wg[i].state == WATTACK)
				transparentimage(NULL, xpos + 15, ypos - 100, &attackWG[wg[i].frameIndex]);
			break;
		}
	}
}

// ����ֲ�����
void drawPlants()
{
	transparentimage(NULL, BAR_STARTX, 0, &imgBr); // ����ֲ�￨����

	for (int i = 0; i < 8; i++) // ����ֲ�￨��
		transparentimage(NULL, CARDS_STARTX + i * CARD_WIDTH, CARDS_STARTY, &imgCards[i]);

	for (int i = 0; i < GRASSROW; i++) // ��������ֲֲ��
	{
		for (int j = 0; j < GRASSCOL; j++)
		{
			if (plants[i][j].type > 0)
			{
				if (plants[i][j].type == DZH)
					drawDZH(i, j);
				else if (plants[i][j].type == YT)
					drawCB(i, j);
				else if (plants[i][j].type == LJ)
					drawHB(i, j);
				else if (plants[i][j].type == TD)
					drawPM(i, j);
				else if (plants[i][j].type == WG)
					drawWG(i, j);
				else if (plants[i][j].type == JG)
					transparentimage(NULL, plants[i][j].x + GRASSBLOCK_WIDTH / 2 - imgPlants[2][0]->getwidth() / 2,
						plants[i][j].y + GRASSBLOCK_HEIGHT / 2 - imgPlants[2][0]->getheight() / 2, imgPlants[2][plants[i][j].frameIndex]);
				else
					transparentimage(NULL, plants[i][j].x + GRASSBLOCK_WIDTH / 2 - imgPlants[plants[i][j].type - 1][0]->getwidth() / 2,
						plants[i][j].y + GRASSBLOCK_HEIGHT / 2 - imgPlants[plants[i][j].type - 1][0]->getheight() / 2, imgPlants[plants[i][j].type - 1][plants[i][j].frameIndex]);
			}
		}
	}

	if (selIndex > 0) // ��ֲ�ﱻѡ��ʱ���������λ�ý����϶��л���
	{
		if (selIndex == DZH)
			transparentimage(NULL, curX - normalDZH[0].getwidth() / 2, curY - normalDZH[0].getheight() / 2, &normalDZH[0]);
		else if (selIndex == YT)
			transparentimage(NULL, curX - bombCB[0].getwidth() / 2, curY - bombCB[0].getheight() / 2, &bombCB[0]);
		else if (selIndex == LJ)
			transparentimage(NULL, curX - bombHB[0].getwidth() / 2, curY - bombHB[0].getheight() / 2, &bombHB[0]);
		else if (selIndex == TD)
			transparentimage(NULL, curX - normalPM[0].getwidth() / 2, curY - normalPM[0].getheight() / 2, &normalPM[0]);
		else if (selIndex == WG)
			transparentimage(NULL, curX - normalWG[0].getwidth() / 2, curY - normalWG[0].getheight() / 2, &normalWG[0]);
		else if (selIndex == JG)
			transparentimage(NULL, curX - imgPlants[2][0]->getwidth() / 2, curY - imgPlants[2][0]->getheight() / 2, imgPlants[2][0]);
		else
			transparentimage(NULL, curX - imgPlants[selIndex - 1][0]->getwidth() / 2, curY - imgPlants[selIndex - 1][0]->getheight() / 2, imgPlants[selIndex - 1][0]);
	}
}

// ���ƽ�ʬ
void drawZombies()
{
	for (int i = 0; i < ZOMBIE_COUNT; i++) // ���ƽ�ʬ���б�ѡ�еĽ�ʬ
	{
		if (zombies[i].used)
		{
			if (zombies[i].dead)
				transparentimage(NULL, zombies[i].x, zombies[i].y - imgDeadZombies[zombies[i].frameIndex].getheight(), &imgDeadZombies[zombies[i].frameIndex]);
			else if (zombies[i].eat)
				transparentimage(NULL, zombies[i].x, zombies[i].y - imgEatZombies[zombies[i].frameIndex].getheight(), &imgEatZombies[zombies[i].frameIndex]);
			else
				transparentimage(NULL, zombies[i].x, zombies[i].y - imgWalkZombies[zombies[i].frameIndex].getheight(), &imgWalkZombies[zombies[i].frameIndex]);
		}
	}
}

// �����������
void drawSunshine()
{
	char sunshine_text[LENGTH]; // ��������ֵ
	snprintf(sunshine_text, sizeof(sunshine_text), "%d", sunshineValue);
	outtextxy(TEXT_STARTX + 33 - textwidth(sunshine_text) / 2, TEXT_STARTY + 12 - textheight(sunshine_text) / 2, sunshine_text);

	for (int i = 0; i < SUNSHINE_COUNT; i++) // ����������б�ʹ�õ�����
	{
		if (sunshine[i].used)
			transparentimage(NULL, sunshine[i].pCur.x, sunshine[i].pCur.y, &imgSunshine[sunshine[i].frameIndex]);
	}
}

// �����ӵ�
void drawBullets()
{
	for (int i = 0; i < BULLET_COUNT; i++)
	{
		if (!bullets[i].used)
			continue;
		else
		{
			if (bullets[i].blast)
				transparentimage(NULL, bullets[i].x, bullets[i].y, &bulletBlast[bullets[i].blastFrame]);
			else
				transparentimage(NULL, bullets[i].x, bullets[i].y, &bulletNormal);
		}
	}
}

// ��������Ⱦ
void drawScr()
{
	// ʹ��˫������л���
	BeginBatchDraw();
	putimage(-112, 0, &imgBk); // ����������ͼ
	drawPlants(); // ����ֲ�����
	drawZombies(); // ���ƽ�ʬ
	drawSunshine(); // �����������
	drawBullets(); // �����ӵ�
	EndBatchDraw();
}

// ���´��컨
void updateDZH(int x, int y)
{
	static int eatEdge = 300; // ���컨��ÿ��Գ��꽩ʬ

	for (int i = 0; i < 27; i++)
	{
		if (dzh[i].used && dzh[i].pos == x * 10 + y) // �ҵ���Ӧ�Ĵ��컨
		{
			if (dzh[i].state == NORMAL) // ��̬
				dzh[i].frameIndex = (dzh[i].frameIndex + 1) % 8;
			else if (dzh[i].state == ATTACK) // ����̬
			{
				if (dzh[i].frameIndex < 8)
					dzh[i].frameIndex = (dzh[i].frameIndex + 1);
				if (dzh[i].frameIndex == 8) // ������Ч
					PlaySound("res\\chomp.wav", NULL, SND_FILENAME | SND_ASYNC);
			}
			else if (dzh[i].state == EAT) // �׽�̬
			{
				dzh[i].frameIndex = (dzh[i].frameIndex + 1) % 5;
				dzh[i].eatTime++;
				if (dzh[i].eatTime >= eatEdge)
				{
					dzh[i].frameIndex = 0;
					dzh[i].state = NORMAL;
					dzh[i].eatTime = 0;
				}
			}
			break;
		}
	}
}

// ����ӣ��ը��
void updateCB(int x, int y)
{
	for (int i = 0; i < 27; i++)
	{
		if (crb[i].used && crb[i].pos == x * 10 + y) // �ҵ���Ӧ��ӣ��ը��
		{
			if (crb[i].state == BOMB) // ��ը̬
			{
				if (crb[i].frameIndex < 7)
					crb[i].frameIndex = crb[i].frameIndex + 1;
			}
			else if (crb[i].state == REST) // ���̬
			{
				if (crb[i].frameIndex == 0) // ���ű�ը��Ч
					PlaySound("res\\cherrybomb.wav", NULL, SND_FILENAME | SND_ASYNC);
				if (crb[i].frameIndex < 12)
					crb[i].frameIndex = crb[i].frameIndex + 1;
			}
			break;
		}
	}
}

// ���»�����
void updateHB(int x, int y)
{
	for (int i = 0; i < 27; i++)
	{
		if (hblj[i].used && hblj[i].pos == x * 10 + y) // �ҵ���Ӧ�Ļ�����
		{
			if (hblj[i].state == HBOMB) // ��ը̬
			{
				if (hblj[i].frameIndex < 8)
					hblj[i].frameIndex = hblj[i].frameIndex + 1;
			}
			else if (hblj[i].state == HREST) // ���̬
			{
				if (hblj[i].frameIndex == 0) // ������Ч
					PlaySound("res\\jalapeno.wav", NULL, SND_FILENAME | SND_ASYNC);
				if (hblj[i].frameIndex < 8)
					hblj[i].frameIndex = hblj[i].frameIndex + 1;
			}
			break;
		}
	}
}

// ������������
void updatePM(int x, int y)
{
	for (int i = 0; i < 27; i++)
	{
		if (pm[i].used && pm[i].pos == x * 10 + y) // �ҵ���Ӧ����������
		{
			if (pm[i].state == PHIDE) // ����̬
			{
				if (pm[i].frameIndex < 20)
					pm[i].frameIndex = pm[i].frameIndex + 1;
			}
			else if (pm[i].state == PNORMAL) // ��̬ͨ
				pm[i].frameIndex = (pm[i].frameIndex + 1) % 8;
			else if (pm[i].state == PREST) // ���̬
			{
				if (pm[i].frameIndex == 0) // ������Ч
					PlaySound("res\\potato_mine.wav", NULL, SND_FILENAME | SND_ASYNC);
				if (pm[i].frameIndex < 6)
					pm[i].frameIndex = pm[i].frameIndex + 1;
			}
			break;
		}
	}
}

// �����ѹ�
void updateWG(int x, int y)
{
	for (int i = 0; i < 27; i++)
	{
		if (wg[i].used && wg[i].pos == x * 10 + y) // �ҵ���Ӧ���ѹ�
		{
			if (wg[i].state == WNORMAL) // ��̬ͨ
				wg[i].frameIndex = (wg[i].frameIndex + 1) % 17;
			else if (wg[i].state == WATTACK) // ����̬
			{
				if (wg[i].frameIndex < 4)
					wg[i].frameIndex = wg[i].frameIndex + 1;
			}
			break;
		}
	}
}

// ��������
void createSunshine()
{
	// ������ֱ���������
	static int count = 0; // ����������������Ƶ��
	static int highFre = 500;
	static int lowFre = 300;
	static int feq = 300;
	count++;

	if (count >= feq) // �������������
	{
		// ��������,ͬʱ�������ⴴ��Ƶ��(ʹ����ˢ�¾��������)
		count = 0;
		feq = lowFre + rand() % (highFre - lowFre + 1); // Ƶ��������300-500

		// ��������أ��ҵ�����ʹ�õ����⣬���д���
		int index;
		for (index = 0; index < SUNSHINE_COUNT && sunshine[index].used; index++);

		if (index == SUNSHINE_COUNT) // ������ȫ��ʹ�ã��޷�����������
			return;
		else // �����µ�����
		{
			static int yStart = 60;
			static double v = 2.;
			sunshine[index].used = true;
			sunshine[index].state = SUNSHINE_DOWN;
			sunshine[index].frameIndex = 0;
			sunshine[index].time = 0;
			sunshine[index].t = 0;
			sunshine[index].pCur.x = BAR_STARTX + rand() % (WIN_WIDTH - BAR_STARTX + FINAL_PAINTX + 1); // ����ˢ��λ��X
			sunshine[index].pCur.y = yStart;
			sunshine[index].p1 = vector2(sunshine[index].pCur.x, sunshine[index].pCur.y);
			sunshine[index].p4 = vector2(sunshine[index].pCur.x, GRASS_STARTY + rand() % (GRASSROW + 1) * GRASSBLOCK_HEIGHT);
			sunshine[index].speed = v / dis(sunshine[index].p1 - sunshine[index].p4);
		}
	}

	// ���տ���������
	static int fre = 300; // �������ʱ�����
	for (int i = 0; i < GRASSROW; i++)
	{
		for (int j = 0; j < GRASSCOL; j++)
		{
			if (plants[i][j].type == XRK) // �����տ�������¼�ʱ��
			{
				plants[i][j].timer++;
				if (plants[i][j].timer >= fre) // ��ʱ��������ޣ����������
				{
					plants[i][j].timer = 0; // �����ʱ��

					int index; // �ҿ�������
					for (index = 0; index < SUNSHINE_COUNT && sunshine[index].used; index++);

					if (index == SUNSHINE_COUNT)
						return;
					else // ��������
					{
						static int left = 120; // ��������λ�������
						static int right = 10; // ��������λ���ҽ���
						int off = (left + rand() % right + 1) * (rand() % 2 ? 1 : -1); // ��������ƫ����

						sunshine[index].used = true; 
						sunshine[index].state = SUNSHINE_GEN;
						sunshine[index].frameIndex = 0;
						sunshine[index].pCur = vector2(plants[i][j].x, plants[i][j].y);
						sunshine[index].p1 = vector2(plants[i][j].x, plants[i][j].y);
						sunshine[index].p4 = vector2(plants[i][j].x + off, plants[i][j].y + imgPlants[plants[i][j].type - 1][0]->getheight() - imgSunshine[0].getheight());
						sunshine[index].p2 = vector2(plants[i][j].x + 0.33 * off, plants[i][j].y - 100);
						sunshine[index].p3 = vector2(plants[i][j].x + 0.7 * off, plants[i][j].y - 50);
						sunshine[index].speed = 0.1;
						sunshine[index].time = 0;
						sunshine[index].t = 0;
					}
				}
			}
		}
	}
}

// ��������
void updateSunshine()
{
	 // ��������أ��ҵ�����ʹ�õ����⣬�޸�������
	 static int timeEdge = 130; // ��������ʱ�����
	 for (int i = 0; i < SUNSHINE_COUNT; i++)
	 {
		 if (sunshine[i].used)
		 {
			 sunshine[i].frameIndex = (++sunshine[i].frameIndex) % 29; // ���Ļ���֡

			 if (sunshine[i].state == SUNSHINE_DOWN) // ���⴦������״̬
			 {
				 sunshine[i].t += sunshine[i].speed; // ��ϸ����ʵʱλ��
				 sunshine[i].pCur = sunshine[i].p1 + sunshine[i].t * (sunshine[i].p4 - sunshine[i].p1);
				 if (sunshine[i].t >= 1) // �����յ��
				 {
					 sunshine[i].t = 0;
					 sunshine[i].state = SUNSHINE_STAY;
				 }
			 }
			 else if (sunshine[i].state == SUNSHINE_STAY) // ���⴦������״̬
			 {
				 sunshine[i].time++;
				 if (sunshine[i].time > timeEdge)
				 {
					 sunshine[i].time = 0;
					 sunshine[i].used = false;
				 }
			 }
			 else if (sunshine[i].state == SUNSHINE_GEN) // ���տ���������
			 {
				 sunshine[i].t += sunshine[i].speed;
				 sunshine[i].pCur = calcBezierPoint(sunshine[i].t, sunshine[i].p1, sunshine[i].p2, sunshine[i].p3, sunshine[i].p4);
				 if (sunshine[i].t >= 1)
				 {
					 sunshine[i].state = SUNSHINE_STAY;
					 sunshine[i].t = 0;
				 }
			 }
			 else if (sunshine[i].state == SUNSHINE_COL) // ���ⱻ�ռ�
			 {
				 sunshine[i].t += sunshine[i].speed;
				 sunshine[i].pCur = sunshine[i].p1 + sunshine[i].t * (sunshine[i].p4 - sunshine[i].p1);
				 if (sunshine[i].t >= 1)
				 {
					 sunshine[i].t = 0;
					 sunshine[i].used = false;
					 sunshineValue += SUNSHINE_VALUE; // ��������ֵ
				 }
			 }
		 }
	 }
}

// ������ʬ
void createZombies()
{
	// �ж��Ƿ��Ѿ��ﵽ���ʬ������
	static int maxZombies = 60;
	if (zmCount == maxZombies)
		return;

	// ���������ƴ���Ƶ��
	static int count = 0;
	static int lowF = 250;
	static int highF = 350;
	static int fre = 250;
	count++;

	if (count > fre) // �������
	{
		count = 0; // ���¼������ʹ���Ƶ��
		fre = lowF + rand() % (highF - lowF + 1); 

		int index; // �ҵ�δʹ�õĽ�ʬ
		for (index = 0; index < ZOMBIE_COUNT && zombies[index].used; index++);

		if (index < ZOMBIE_COUNT) // ������ʬ
		{
			zombies[index].used = true;
			zombies[index].dead = false;
			zombies[index].eat = false;
			zombies[index].frameIndex = 0;
			zombies[index].x = WIN_WIDTH;
			zombies[index].row = rand() % GRASSROW + 1;
			zombies[index].y = GRASS_STARTY + zombies[index].row * GRASSBLOCK_HEIGHT;
			zombies[index].col = (zombies[index].x + 100 - GRASS_STARTX) / GRASSBLOCK_WIDTH;
			zombies[index].speed = 1;
			zombies[index].blood = 100;
			zombies[index].eatwho = 0;
			zmCount++;
			if (zmCount == 1) // ������Ч
			{
				mciSendString("play res\\awooga.mp3", NULL, 0, NULL); // ��ʾ��
				mciSendString("play res\\zz_audio\\UraniwaNi.mp3 repeat", NULL, 0, NULL); // ��ʼ���ű�����
				mciSendString("stop res\\LookupattheSky.mp3", NULL, 0, NULL); // ֹͣ��һ��
			}
		}
		else
			return;
	}
}

// ���½�ʬ
void updateZombies()
{
	// ���ü�����ʽ�Խ�ʬ���н���
	static int count = 0;
	static int limit = 4;
	count++;

	if (count > limit) // ���½�ʬ
	{
		count = 0;
		for (int i = 0; i < ZOMBIE_COUNT; i++)
		{
			// �ҵ�ʹ���еĽ�ʬ������Ϸδʧ�ܣ�����½�ʬ
			if (zombies[i].used)
			{
				static int housePos = 23;
				if (zombies[i].x <= housePos) // ��Ϸ�и�
					gameState = LOSE;
				else
				{
					zombies[i].x -= zombies[i].speed;
					zombies[i].col = (zombies[i].x + 100 - GRASS_STARTX) / GRASSBLOCK_WIDTH;
					if (zombies[i].dead) // ��ʬ����
					{
						zombies[i].frameIndex++;
						if (zombies[i].frameIndex == 20) // ������������
						{
							zombies[i].used = false;
							killCount++; // ���»�ɱ��
							if (killCount == 60)
								gameState = WIN;
						}
					}
					else if (zombies[i].eat)
					{
						zombies[i].frameIndex = (zombies[i].frameIndex + 1) % 21;
						mciSendString("play res\\zombieEaT.mp3", NULL, 0, NULL);
					}
					else
						zombies[i].frameIndex = (zombies[i].frameIndex + 1) % 22;
				}
			}
		} 
	}
}

// �ӵ�����
void shoot()
{
	// �ж�ĳ���Ƿ��н�ʬ
	int lines[GRASSROW] = { 0 }; // �б�־
	int dangerx = WIN_WIDTH - imgWalkZombies[0].getwidth() + GRASSBLOCK_WIDTH / 2;

	for (int i = 0; i < ZOMBIE_COUNT; i++) // ������ʬ�������б�־
	{
		if (zombies[i].used && zombies[i].x < dangerx)
			lines[zombies[i].row - 1] = 1;
	}

	// ����ֲ����ֲ����,�ҳ��㶹
	static int shootEdge = 80; // �ӵ��������
	for (int i = 0; i < GRASSROW; i++)
	{
		for (int j = 0; j < GRASSCOL; j++)
		{
			// ĳλ����ֲ���㶹���Ҹ����н�ʬ
			if (plants[i][j].type == WD && lines[i] == 1)
			{
				// �����ӵ������ʱ��
				plants[i][j].shoottime++;

				if (plants[i][j].shoottime >= shootEdge)
				{
					plants[i][j].shoottime = 0; 

					int k; // �ҿ����ӵ�
					for (k = 0; k < BULLET_COUNT && bullets[k].used; k++);

					if (k == BULLET_COUNT)
						return;
					else
					{
						bullets[k].used = true;
						bullets[k].blast = false;
						bullets[k].blastFrame = 0;
						bullets[k].x = GRASS_STARTX + j * GRASSBLOCK_WIDTH + GRASSBLOCK_WIDTH / 2  + 10;
						bullets[k].y = GRASS_STARTY + i * GRASSBLOCK_HEIGHT + GRASSBLOCK_HEIGHT / 2 - imgPlants[plants[i][j].type - 1][plants[i][j].frameIndex]->getheight() / 2 + 5;
						bullets[k].row = i + 1;
						bullets[k].speed = 2;
					}
				}
			}
		}
	}
}

// �����ӵ�
void updateBullets()
{
	// �����ӵ��أ��ҵ�����ʹ�õ��ӵ�
	for (int i = 0; i < BULLET_COUNT; i++)
	{
		if (bullets[i].used)
		{
			// �ӵ��ƶ�
			bullets[i].x += bullets[i].speed;
			if (bullets[i].x > WIN_WIDTH)
				bullets[i].used = false;
			
			// �ӵ�����
			if (bullets[i].blast)
			{
				bullets[i].blastFrame++;
				if (bullets[i].blastFrame == 4)
					bullets[i].used = false;
			}
		}
	}
}

// ��⽩ʬ���ӵ�
void checkZombiestoBullets()
{
	for (int i = 0; i < BULLET_COUNT; i++)
	{
		// �ӵ����ڷ�ʹ��״̬/����״ֱ̬������
		if (!bullets[i].used || bullets[i].blast)
			continue;
		else
		{
			for (int j = 0; j < ZOMBIE_COUNT; j++)
			{
				if (!zombies[j].used)
					continue;
				else
				{
					// ���м��:�������Ҽ��߽粢���
					int left_check = zombies[j].x + 80;
					int right_check = zombies[j].x + 115;

					if (bullets[i].row == zombies[j].row && !zombies[j].dead && bullets[i].x >= left_check && bullets[i].x <= right_check)
					{
						// ���Ż�����Ч
						PlaySound("res\\hitzombie.wav", NULL, SND_FILENAME | SND_ASYNC);
		
						// ���½�ʬѪ���Լ��ӵ�״̬
						zombies[j].blood -= BULLET_HURT;
						bullets[i].blast = true;
						bullets[i].speed = 0;

						// �жϽ�ʬ�Ƿ�����
						if (zombies[j].blood <= 0)
						{
							zombies[j].dead = true;
							zombies[j].speed = 0;
							zombies[j].frameIndex = 0;
						}

						break; // һ���ӵ�ֻ�ܴ�һ����ʬ
					}
			}
				}
		}
	}
}

// ��⽩ʬ��ֲ��
void checkZombiestoPlants()
{
	for (int i = 0; i < ZOMBIE_COUNT; i++)
	{
		// ��ʬ���������������ж�
		if (zombies[i].dead)
			continue;
		else
		{
			// �жϽ�ʬ�Ƿ񹻵õ�ͬһ�е�ֲ��
			for (int j = 0; j < GRASSCOL; j++)
			{
				if (plants[zombies[i].row - 1][j].type != ZERO)
				{
					// ������߽�
					int leftCheck = GRASS_STARTX + j * GRASSBLOCK_WIDTH + 10;
					int rightCheck = GRASS_STARTX + j * GRASSBLOCK_WIDTH + 60;

					if (zombies[i].x + 80 >= leftCheck && zombies[i].x + 80 <= rightCheck)
					{
						if (plants[zombies[i].row - 1][j].isEaten) // ��ǰֲ���Ƿ��Ѿ���ĳֻ��ʬ��
						{
							plants[zombies[i].row - 1][j].blood -= ZOMBIE_HURT; // ����Ѫ��
							if (!zombies[i].eat) // �жϵ�ǰ��ʬ�Ƿ��ڳ�
							{
								zombies[i].eat = true;
								zombies[i].eatwho = (zombies[i].row - 1) * 10 + j;
								zombies[i].speed = 0;
								zombies[i].frameIndex = 0;
							}

						}
						else
						{
							plants[zombies[i].row - 1][j].isEaten = true;
							zombies[i].eat = true;
							zombies[i].eatwho = (zombies[i].row - 1) * 10 + j;
							zombies[i].speed = 0;
							zombies[i].frameIndex = 0;
						}

						if (plants[zombies[i].row - 1][j].blood <= 0) // ��ǰֲ��ûѪ��
						{
							// ����ֲ��
							if (plants[zombies[i].row - 1][j].type == DZH) // ����Ǵ��컨
							{
								for (int m = 0; m < 27; m++)
								{
									if (dzh[m].pos == (zombies[i].row - 1) * 10 + j)
									{
										dzh[m].used = false;
										dzh[m].frameIndex = 0;
										dzh[m].pos = -1;
										dzh[m].eatTime = 0;
										dzh[m].state = NONE;
										break;
									}
								}
							}
							else if (plants[zombies[i].row - 1][j].type == YT) // �����ӣ��
							{
								for (int m = 0; m < 27; m++)
								{
									if (crb[m].pos == (zombies[i].row - 1) * 10 + j)
									{
										crb[m].used = false;
										crb[m].frameIndex = 0;
										crb[m].pos = -1;
										crb[m].state = CNONE;
										break;
									}
								}
							}
							else if (plants[zombies[i].row - 1][j].type == LJ) // ����ǻ�����
							{
								for (int m = 0; m < 27; m++)
								{
									if (hblj[m].pos == (zombies[i].row - 1) * 10 + j)
									{
										hblj[m].used = false;
										hblj[m].frameIndex = 0;
										hblj[m].pos = -1;
										hblj[m].state = HNONE;
										break;
									}
								}
							}
							else if (plants[zombies[i].row - 1][j].type == TD) // �������������
							{
								for (int m = 0; m < 27; m++)
								{
									if (pm[m].pos == (zombies[i].row - 1) * 10 + j)
									{
										pm[m].used = false;
										pm[m].frameIndex = 0;
										pm[m].pos = -1;
										pm[m].state = PNONE;
										break;
									}
								}
							}
							else if (plants[zombies[i].row - 1][j].type == WG) // ������ѹ�
							{
								for (int m = 0; m < 27; m++)
								{
									if (wg[m].pos == (zombies[i].row - 1) * 10 + j)
									{
										wg[m].used = false;
										wg[m].frameIndex = 0;
										wg[m].pos = -1;
										wg[m].state = WNONE;
										break;
									}
								}
							}
							plants[zombies[i].row - 1][j].type = ZERO;
							plants[zombies[i].row - 1][j].isEaten = false;
							plants[zombies[i].row - 1][j].frameIndex = 0;
							plants[zombies[i].row - 1][j].shoottime = 0;
							plants[zombies[i].row - 1][j].timer = 0;
							
							// ���½�ʬ
							for (int k = 0; k < ZOMBIE_COUNT; k++)
							{
								if (zombies[k].eatwho == (zombies[i].row - 1) * 10 + j) // ÿһ���ڳԵ�ǰֲ��Ķ�Ҫ����
								{
									zombies[k].eat = false;
									zombies[k].eatwho = 0;
									zombies[k].speed = 1;
									zombies[k].frameIndex = 0;
								}
							}	
						}
					}
				}
			}
		}
	}
}

// ��⽩ʬ�ʹ��컨
void checkZombiestoDZH()
{
	for (int i = 0; i < 27; i++)
	{
		if (dzh[i].used) // ���컨������
		{
			int dzhCol = dzh[i].pos % 10; // ���컨����
			int dzhRow = dzh[i].pos / 10; // ���컨����

			for (int j = 0; j < ZOMBIE_COUNT; j++)
			{
				if (zombies[j].dead || !(dzhRow == zombies[j].row - 1)) // ��ʬ����/��ʬ�ʹ��컨����ͬһ��
					continue;
				else
				{
					// �������ұ߽�,�����
					int leftCheck = GRASS_STARTX + dzhCol * GRASSBLOCK_WIDTH + 10;
					int rightCheck = GRASS_STARTX + dzhCol * GRASSBLOCK_WIDTH + 60;

					if (zombies[j].x + 80 >= leftCheck && zombies[j].x + 80 <= rightCheck)
					{
						if (dzh[i].state == NORMAL) // ���ڳ�̬
						{
							dzh[i].state = ATTACK;
							dzh[i].frameIndex = 0;
						}
						else if (dzh[i].state == ATTACK) // ���𹥻�
						{
							if (dzh[i].frameIndex == 8)
							{
								dzh[i].frameIndex = 0;
								dzh[i].eatTime = 0;
								dzh[i].state = EAT;
								zombies[j].used = false;
								zombies[j].dead = true;
								killCount++;
								if (killCount == 60)
									gameState = WIN;
								break; // ֻ����һֻ��ʬ
							}
						}
					}
				}
			}
		}
	}
}

// ӣ��ը����ը
void cbBomb()
{
	for (int i = 0; i < 27; i++)
	{
		if (crb[i].used) // ����ӣ�ҳ�,�ҵ�ʹ���е�ӣ��
		{
			int ytCol = crb[i].pos % 10; // ӣ�ҵ���
			int ytRow = crb[i].pos / 10; // ӣ�ҵ���

			if (crb[i].state == BOMB) // ���ڱ�ը̬
			{
				if (crb[i].frameIndex == 7)
				{
					crb[i].state = REST; // ����ӣ��
					crb[i].frameIndex = 0;
					
					for (int k = 0; k < ZOMBIE_COUNT; k++) // ���½�ʬ
					{
						if (zombies[k].dead) continue;
						if ((zombies[k].row - 1 == ytRow || zombies[k].row - 1 == ytRow - 1 || zombies[k].row - 1 == ytRow + 1) // �Ź���Χ�ڽ�ʬ
							&& (zombies[k].col == ytCol || zombies[k].col  == ytCol - 1 || zombies[k].col  == ytCol + 1))
						{
							zombies[k].dead = true;
							zombies[k].frameIndex = 0;
							zombies[k].speed = 0;
						}
					}
				}
			}
			else if (crb[i].state == REST) // �������̬
			{
				if (crb[i].frameIndex == 12)
				{
					plants[ytRow][ytCol].type = ZERO;
					crb[i].used = false;
					crb[i].state = CNONE;
					crb[i].frameIndex = 0;
					crb[i].pos = -1;
				}
			}
		}
	}
}

// ��������ը
void hbBomb()
{
	for (int i = 0; i < 27; i++)
	{
		if (hblj[i].used) // �ҵ�ʹ���еĻ�����
		{
			int hbCol = hblj[i].pos % 10; // ����������
			int hbRow = hblj[i].pos / 10; // ����������

			if (hblj[i].state == HBOMB) // ���ڱ�ը̬
			{
				if (hblj[i].frameIndex == 8)
				{
					hblj[i].state = HREST; // ���»�����
					hblj[i].frameIndex = 0;

					for (int k = 0; k < ZOMBIE_COUNT; k++) // ���½�ʬ
					{
						if (zombies[k].dead) 
							continue;
						if (zombies[k].row - 1 == hbRow) // ͬһ�еĽ�ʬ
						{
							zombies[k].dead = true;
							zombies[k].frameIndex = 0;
							zombies[k].speed = 0;
						}
					}
				}
			}
			else if (hblj[i].state == HREST) // �������̬
			{
				if (hblj[i].frameIndex == 8)
				{
					plants[hbRow][hbCol].type = ZERO;
					hblj[i].used = false;
					hblj[i].state = HNONE;
					hblj[i].frameIndex = 0;
					hblj[i].pos = -1;
				}
			}
		}
	}
}

// �������ױ�ը
void pmBomb()
{
	for (int i = 0; i < 27; i++)
	{
		if (pm[i].used) // �ҵ�ʹ���е���������
		{
			int pmCol = pm[i].pos % 10; // �������׵���
			int pmRow = pm[i].pos / 10; // �������׵���
			int leftCheck = GRASS_STARTX + pmCol * GRASSBLOCK_WIDTH + 10; // �������ұ߽�,�����
			int rightCheck = GRASS_STARTX + pmCol * GRASSBLOCK_WIDTH + 60;
			bool bob = false; // �Ƿ��н�ʬ���ڱ�ը��Χ

			if (pm[i].state == PHIDE) // ����
			{
				if (pm[i].frameIndex == 20)
				{
					pm[i].frameIndex = 0;
					pm[i].state = PNORMAL;
				}
			}
			else if (pm[i].state == PNORMAL) // ��������̬
			{
				for (int j = 0; j < ZOMBIE_COUNT; j++) // ������ʬ�أ����Ƿ��в��׵�
				{
					if (zombies[j].dead || !(pmRow == zombies[j].row - 1)) // ��ʬ����/��ʬ���������ײ���ͬһ��
						continue;
					else
					{
						if (zombies[j].x + 80 >= leftCheck && zombies[j].x + 80 <= rightCheck) // ���빥����Χ
						{
							zombies[j].dead = true;
							zombies[j].speed = 0;
							zombies[j].frameIndex = 0;
							bob = true;
						}
					}
				}
				if (bob == true) // ��ը״̬
				{
					pm[i].state = PREST;
					pm[i].frameIndex = 0;
				}
			}
			else if (pm[i].state == PREST) // �������̬
			{
				if (pm[i].frameIndex == 6)
				{
					plants[pmRow][pmCol].type = ZERO;
					pm[i].used = false;
					pm[i].frameIndex = 0;
					pm[i].pos = -1;
					pm[i].state = PNONE;
				}
			}
		}
	}
}

// �ѹϹ���
void wgAttack()
{
	for (int i = 0; i < 27; i++)
	{
		if (wg[i].used) // �ҵ�ʹ���е��ѹ�
		{
			bool attack = false; // �ж��Ƿ���Ŀ��
			int wgCol = wg[i].pos % 10; // �ѹϵ���
			int wgRow = wg[i].pos / 10; // �ѹϵ���

			int leftCheck = GRASS_STARTX + wgCol * GRASSBLOCK_WIDTH + 10; // �������ұ߽�,�����
			int rightCheck = GRASS_STARTX + wgCol * GRASSBLOCK_WIDTH + 60;

			if (wg[i].state == WNORMAL) // ��������̬
			{
				for (int j = 0; j < ZOMBIE_COUNT; j++) // ������ʬ��
				{
					if (zombies[j].dead || !(wgRow == zombies[j].row - 1)) // ��ʬ����/��ʬ���ѹϲ���ͬһ��
						continue;
					else if (zombies[j].x + 80 >= leftCheck && zombies[j].x + 80 <= rightCheck) // ���빥����Χ
					{
						attack = true;
						zombies[j].used = false; // ���½�ʬ
						zombies[j].dead = true;
						zombies[j].speed = 0;
						zombies[j].frameIndex = 0;
						killCount++;
						if (killCount == 60)
							gameState = WIN;
					}
				}
				if (attack == true)
				{
					wg[i].state = WATTACK;
					wg[i].frameIndex = 0;
				}
			}
			else if (wg[i].state == WATTACK) // ���ڹ���̬
			{
				if (wg[i].frameIndex == 4)
				{
					plants[wgRow][wgCol].type = ZERO; // ����ֲ��
					wg[i].used = false;
					wg[i].frameIndex = 0;
					wg[i].pos = -1;
					wg[i].state = WNONE;
				}
			}
		}
	}
}

// �������
void collisionCheck()
{
	// ��ʬ���ӵ�
	checkZombiestoBullets();

	// ��ʬ��ֲ��
	checkZombiestoPlants();

	// ���컨������ʬ
	checkZombiestoDZH();

	// ��ը
	cbBomb(); // ӣ��ը����ը
	hbBomb(); // ��������ը
	pmBomb(); // �������ױ�ը

	// �ѹϹ�����ʬ
	wgAttack();
}

// ������Ϸ����
void updateGame()
{
	// �޸�ֲ����Ⱦ֡
	static int times = 0; // �����������ڿ���֡�޸�
	static int fre = 6; // ����
	times++;

	if (times >= fre) // �������ﵽ����
	{	
		times = 0;
		for (int i = 0; i < GRASSROW; i++)
		{
			for (int j = 0; j < GRASSCOL; j++)
			{
				if (plants[i][j].type > 0) // ��������ֲֲ��޸���֡
				{
					if (plants[i][j].type == DZH) // ���컨
						updateDZH(i, j);
					else if (plants[i][j].type == YT) // ӣ��ը��
						updateCB(i, j);
					else if (plants[i][j].type == LJ) // ������
						updateHB(i, j);
					else if (plants[i][j].type == TD) // ��������
						updatePM(i, j);
					else if (plants[i][j].type == WG) // �ѹ�
						updateWG(i, j);
					else
						plants[i][j].frameIndex = (++plants[i][j].frameIndex) % 8;
				}	
			}
		}
	}
	
	// �������
	createSunshine(); // ��������
	updateSunshine(); // ��������

	// ��ʬ���
	createZombies(); // ������ʬ
	updateZombies(); // ���½�ʬ

	// �ӵ����
	shoot(); // ����
	updateBullets(); // �����ӵ�

	// �������
	collisionCheck(); 
}

// �����Ϸ���
bool checkOver()
{
	bool ret = false; // ����ֵ��ʼΪ��
	IMAGE img; // �������ͼ

	if (gameState == WIN) // ��ʤ
	{
		loadimage(&img, "res\\win2.png");
		transparentimage(NULL, 0, 0, &img);
		mciSendString("stop res\\zz_audio\\UraniwaNi.mp3", NULL, 0, NULL); // ��ֹͣ������
		PlaySound("res\\win.wav", NULL, SND_FILENAME | SND_ASYNC);
		Sleep(4000); // �ȴ�4S
		ret = true;
	}
	else if (gameState == LOSE) // ����
	{
		loadimage(&img, "res\\fail2.png");
		transparentimage(NULL, 0, 0, &img);
		mciSendString("stop res\\zz_audio\\UraniwaNi.mp3", NULL, 0, NULL);
		PlaySound("res\\lose.wav", NULL, SND_FILENAME | SND_ASYNC);
		Sleep(5000); // �ȴ�5S
		ret = true;
	}
	return ret;
}