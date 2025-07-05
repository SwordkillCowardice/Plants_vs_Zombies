// OrionZhang写的植物大战僵尸
#include <graphics.h> // 图形库
#include <mmsystem.h> // 多媒体播放
#include <stdio.h> 
#include <time.h> // 随机数种子
#include "tools.h" // Rock老师写的辅助工具
#include "vector2.h" // 同上
#pragma comment(lib, "MSIMG32.LIB") // 透明贴图需要
#pragma comment(lib, "winmm.lib") // 多媒体播放

// 定义常量
#define LENGTH 64 // 最大长度:文件名/文字内容
#define FLUSH_TIME 10 // 主界面刷新间隔
#define WIN_WIDTH 900 // 游戏主界面宽
#define WIN_HEIGHT 600 // 主界面高
#define FINAL_PAINTX -112 // 主界面绘制起点X
#define BUTTON_STARTX 474 // 开始按钮绘制起始X
#define BUTTON_STARTY 75 // 开始按钮绘制起始Y
#define BAR_STARTX 148 // 植物卡牌栏绘制起点X
#define CARD_WIDTH 65 // 植物卡牌宽度
#define CARD_HEIGHT 90 // 植物卡牌高度
#define CARDS_STARTX 236 // 植物卡牌绘制起点X
#define CARDS_STARTY 6 // 植物卡牌绘制起点Y
#define GRASS_STARTX 140 // 草坪块起始X
#define GRASS_STARTY 172 // 草坪块起始Y
#define GRASSBLOCK_WIDTH 82 // 草坪块宽度
#define GRASSBLOCK_HEIGHT 104 // 草坪块高度
#define GRASSROW 3 // 草坪总行数
#define GRASSCOL 9 // 草坪总列数
#define SUNSHINE_COUNT 15 // 阳光池中阳光总数。池:相当于提前创建好一定量的目标对象，要用时直接从中取，使用完再放回;而不是需要用时再创建
#define SUNSHINE_VALUE 25 // 收集单个阳光所得阳光值
#define TEXT_STARTX 161 // 阳光数值绘制起始X
#define TEXT_STARTY 72 // 阳光值绘制起始Y
#define ZOMBIE_COUNT 12 // 僵尸池中僵尸总数
#define ZOMBIE_HURT 2 // 僵尸伤害
#define BULLET_COUNT 30 // 子弹池中子弹数
#define BULLET_HURT 10 // 子弹伤害

// 定义植物种植结构
typedef struct
{
	int type; // 植物类型，0代表未种植，1-8代表植物序号
	int x, y; // 对应草坪块的起始x和y
	int frameIndex; // 植物的渲染帧
	int blood; // 植物耐久度
	bool isEaten; // 是否已经在被吃
	int timer; // 用于控制向日葵生产阳光
	int shoottime; // 用于控制豌豆发射子弹
} Plants;
Plants plants[GRASSROW][GRASSCOL]; // 植物种植数组，辅助植物渲染
enum{ZERO, XRK, WD, DZH, YT, LJ, JG, WG, TD}; // 植物种类枚举

// 定义大嘴花
typedef struct
{
	bool used; // 是否在使用
	int pos; // 种植位置
	int frameIndex; // 帧
	int state; // 处于什么状态
	int eatTime; // 吃了多久
} DaZuiHua;
DaZuiHua dzh[27]; // 大嘴花池
enum {NONE, NORMAL, ATTACK, EAT}; // 大嘴花四态：未定义态、常态、攻击、咀嚼态
IMAGE normalDZH[8]; // 大嘴花普通帧
IMAGE attackDZH[8]; // 大嘴花攻击帧
IMAGE eatDZH[5]; // 大嘴花咀嚼帧

// 定义樱桃炸弹
typedef struct
{
	bool used; // 是否在用
	int pos; // 种在哪
	int frameIndex; // 帧
	int state; // 状态
} CherryBomb;
CherryBomb crb[27]; // 樱桃炸弹池
enum{CNONE, BOMB, REST}; // 樱桃炸弹三状态：未定义、爆炸态、灰烬态
IMAGE bombCB[7]; // 樱桃炸弹爆炸帧
IMAGE restCB[12]; // 樱桃炸弹爆炸余烬帧

// 定义火爆辣椒
typedef struct
{
	bool used; // 是否在用
	int pos; // 种在哪
	int frameIndex; // 帧
	int state; // 状态
} HUOBLJ;
HUOBLJ hblj[27]; // 火爆辣椒池
enum {HNONE, HBOMB, HREST }; // 火爆辣椒三状态：未定义、爆炸态、灰烬态
IMAGE bombHB[8]; // 火爆辣椒爆炸帧
IMAGE restHB[8]; // 火爆辣椒爆炸余烬帧

// 定义土豆地雷
typedef struct
{
	bool used; // 是否在用
	int pos; // 种在哪
	int frameIndex; // 帧
	int state; // 状态
} PotatoMine;
PotatoMine pm[27]; // 土豆地雷池
enum {PNONE, PHIDE, PNORMAL, PREST }; // 土豆地雷三状态：未定义、隐藏态、普通态、灰烬态
IMAGE hidePM[20]; // 埋在土里
IMAGE normalPM[8]; // 土豆地雷普通帧
IMAGE restPM[6]; // 土豆地雷爆炸灰烬帧

// 定义窝瓜
typedef struct
{
	bool used; // 是否在用
	int pos; // 种在哪
	int frameIndex; // 帧
	int state; // 状态
} WOGUA;
WOGUA wg[27]; // 窝瓜池
enum {WNONE, WNORMAL, WATTACK}; //窝瓜三状态：未定义、正常态、攻击态
IMAGE normalWG[17]; // 窝瓜普通帧
IMAGE attackWG[4]; // 窝瓜攻击帧

// 定义阳光
typedef struct SUNSHIEN
{
	bool used; // 是否在使用中(与池相关)
	int frameIndex; // 渲染帧
	float t; // 贝塞尔曲线的参数，取值0-1，控制阳光球位置
	vector2 p1, p2, p3, p4; // 贝塞尔曲线的四个控制点
	vector2 pCur; // 阳光沿贝塞尔曲线运动的实时位置
	float speed; // 阳光球运动速度，控制t值变化
	int state; // 阳光球当前状态
	int time; // 到达目的地后滞留时间
} sunShine;
sunShine sunshine[SUNSHINE_COUNT]; // 阳光池
IMAGE imgSunshine[29]; // 阳光帧数组
enum {SUNSHINE_DOWN, SUNSHINE_STAY, SUNSHINE_COL, SUNSHINE_GEN}; // 阳光的四个状态：垂直下降、滞留、被收集、向日葵生产
int sunshineCost[8] = { 50, 100, 150, 150, 125, 50, 50, 25 }; // 每个植物的阳光消耗

// 定义僵尸
typedef struct Zombie
{
	bool used; // 是否在使用
	int x, y; // 僵尸实时位置
	int row; // 僵尸所在行号
	int col; // 僵尸所在列号
	int frameIndex; // 渲染帧
	int speed; // 速度
	int blood; // 血量
	bool dead; // 僵尸是否死亡
	bool eat; // 僵尸是否在吃植物
	int eatwho; // 僵尸在吃哪一个
} Zombie;
Zombie zombies[ZOMBIE_COUNT]; // 僵尸池
IMAGE imgWalkZombies[22]; // 僵尸行走帧
IMAGE imgDeadZombies[20]; // 僵尸死亡帧
IMAGE imgEatZombies[21]; // 僵尸吃帧
IMAGE imgStandZombies[11]; // 僵尸站立帧

// 定义子弹
typedef struct bo
{
	bool used; // 是否在使用
	int x, y; // 子弹位置
	int row; // 子弹所在行号
	int speed; // 子弹飞行速度
	bool blast; // 是否处于爆炸状态
	int blastFrame; // 爆炸帧
} Bullet;
Bullet bullets[BULLET_COUNT]; // 子弹池
IMAGE bulletNormal; // 子弹飞行
IMAGE bulletBlast[4]; // 子弹爆炸帧

// 游戏运行状态
enum {WIN, LOSE, RUN};

// 全局变量和数组
IMAGE imgBk; // 主场景背景图
IMAGE imgBr; // 植物卡牌栏
IMAGE imgCards[8]; // 植物卡牌图组
IMAGE* imgPlants[3][8]; // 植物图片指针数组，[i][j]对应第i+1个植物的第j+1帧

int gameState = RUN; // 游戏当前状态
int curX, curY; // 鼠标当前位置
int selIndex = 0; // 当前被选中的植物序号(1-8)，0代表未选中
int sunshineValue = 50; // 初始阳光值
int killCount = 0; // 当前击杀的僵尸数
int zmCount = 0; // 当前已生成的僵尸总数

// 函数声明
void getImage(); // 加载图片
void initialize(); // 项目初始化
void transparentimage(IMAGE* dstimg, int x, int y, IMAGE* srcimg); // 透明贴图
void start(); // 开始界面
void patrol(); // 开场巡礼
void bardown(); // 卡牌栏滑下
void collectSunshine(ExMessage* msg); // 收集阳光
void createDZH(int pos); // 创建大嘴花
void createCB(int pos); // 创建樱桃炸弹
void createHB(int pos); // 创建火爆辣椒
void createPM(int pos); // 创建土豆地雷
void createWG(int pos); // 创建窝瓜
void userControl(); // 玩家控制
void drawDZH(int x, int y); // 绘制大嘴花
void drawCB(int x, int y); // 绘制樱桃炸弹
void drawHB(int x, int y); // 绘制火爆辣椒
void drawPM(int x, int y); // 绘制土豆地雷
void drawWG(int x, int y); // 绘制窝瓜
void drawPlants(); // 绘制植物相关
void drawZombies(); // 绘制僵尸
void drawSunshine(); // 绘制阳光相关
void drawBullets(); // 绘制子弹
void drawScr(); // 主场景渲染
void updateDZH(int x, int y); // 更新大嘴花 !!! 一定要用带参数x,y的更新：涉及更新的逻辑
void updateCB(int x, int y); // 更新樱桃炸弹
void updateHB(int x, int y); // 更新火爆辣椒
void updatePM(int x, int y); // 更新土豆地雷
void updateWG(int x, int y); // 更新窝瓜
void createSunshine(); // 创建阳光
void updateSunshine(); // 更新阳光
void createZombies(); // 创建僵尸
void updateZombies(); // 更新僵尸
void shoot(); // 子弹发射
void updateBullets(); // 更新子弹
void checkZombiestoBullets(); // 检测僵尸和子弹
void checkZombiestoPlants(); // 检测僵尸和植物
void checkZombiestoDZH(); // 检测僵尸和大嘴花
void cbBomb(); // 樱桃炸弹爆炸
void hbBomb(); // 火爆辣椒爆炸
void pmBomb(); // 土豆地雷爆炸
void wgAttack(); // 窝瓜攻击
void collisionCheck(); // 交互检测
void updateGame(); // 更新游戏数据
bool checkOver(); // 检查游戏情况

// 主函数
int main(void)
{
	initialize(); // 项目初始化
	start(); // 开始界面
	patrol(); // 巡礼
	bardown(); // 卡牌栏滑下

	int mainTime = 0; // 定义计时变量,用于画面更新
	
	while (true) // 程序主体
	{
		mainTime += getDelay(); // 对程序已运行时间进行累加
		userControl(); // 玩家控制

		if (mainTime > FLUSH_TIME) // 超过刷新时间后，计时清零
		{
			mainTime = 0;
			drawScr(); // 主场景渲染
			updateGame(); // 修改游戏相关数据(绘制内容)
			if (checkOver()) // 判断游戏结束
				break;
		}
	}
}

// 加载图片
void getImage()
{
	// 加载所需图片
	loadimage(&imgBk, "res\\bg.jpg"); // 主背景图
	loadimage(&imgBr, "res\\bar5.png"); // 植物卡牌栏

	char path[LENGTH]; // 定义文件路径
	for (int i = 0; i < 8; i++) // 加载植物卡牌
	{
		snprintf(path, sizeof(path), "res\\Cards\\card_%d.png", i + 1);
		loadimage(&imgCards[i], path);
	}

	for (int i = 0; i < 2; i++) // 加载阳光和向日葵
	{
		for (int j = 0; j < 8; j++)
		{
			snprintf(path, sizeof(path), "res\\zhiwu\\%d\\%d.png", i, j + 1);
			imgPlants[i][j] = new IMAGE;
			loadimage(imgPlants[i][j], path);
		}
	}
	
	for (int i = 0; i < 8; i++) // 加载坚果
	{
		snprintf(path, sizeof(path), "res\\zhiwu\\5\\%d.png", i + 1);
		imgPlants[2][i] = new IMAGE;
		loadimage(imgPlants[2][i], path);
	}

	for (int i = 0; i < 8; i++) // 加载常态大嘴花
	{
		snprintf(path, sizeof(path), "res\\zhiwu\\2\\%d.png", i + 1);
		loadimage(&normalDZH[i], path);
	}

	for (int i = 0; i < 8; i++) // 加载攻击态大嘴花
	{
		snprintf(path, sizeof(path), "res\\Plants\\1Chomper\\ChomperAttack.png\\%d.png", i);
		loadimage(&attackDZH[i], path);
	}

	for (int i = 0; i < 5; i++) // 加载咀嚼态大嘴花
	{
		snprintf(path, sizeof(path), "res\\Plants\\1Chomper\\ChomperDigest.png\\%d.png", i);
		loadimage(&eatDZH[i], path);
	}

	for (int i = 0; i < 7; i++) // 加载爆炸态樱桃炸弹
	{
		snprintf(path, sizeof(path), "res\\Plants\\2CherryBomb\\1.png\\%d.png", i + 1);
		loadimage(&bombCB[i], path);
	}

	for (int i = 0; i < 12; i++) // 加载余烬态樱桃炸弹
	{
		snprintf(path, sizeof(path), "res\\Plants\\2CherryBomb\\Boom.png\\%d.png", i + 1);
		loadimage(&restCB[i], path);
	}

	for (int i = 0; i < 8; i++) // 加载爆炸态火爆辣椒
	{
		snprintf(path, sizeof(path), "res\\zhiwu\\4\\%d.png", i + 1);
		loadimage(&bombHB[i], path);
	}

	for (int i = 0; i < 8; i++) // 加载余烬态火爆辣椒
	{
		snprintf(path, sizeof(path), "res\\Plants\\3Jalapeno\\JalapenoAttack.png\\%d.png", i + 1);
		loadimage(&restHB[i], path);
	}

	for (int i = 0; i < 20; i++)  // 加载土中态
		loadimage(&hidePM[i], "res\\Plants\\4PotatoMine\\1.png");

	for (int i = 0; i < 8; i++) // 加载常态土豆地雷
	{
		snprintf(path, sizeof(path), "res\\zhiwu\\7\\%d.png", i + 1);
		loadimage(&normalPM[i], path);
	}

	for (int i = 0; i < 3; i++) // 加载余烬态土豆地雷
	{
		loadimage(&restPM[i], "res\\Plants\\4PotatoMine\\2.png"); 
		loadimage(&restPM[i + 3], "res\\Plants\\4PotatoMine\\3.png");
	}

	for (int i = 0; i < 17; i++) // 加载常态窝瓜
	{
		snprintf(path, sizeof(path), "res\\zhiwu\\6\\%d.png", i + 1);
		loadimage(&normalWG[i], path);
	}

	for (int i = 0; i < 4; i++) // 加载攻击态窝瓜
	{
		snprintf(path, sizeof(path), "res\\Plants\\5Squash\\SquashAttack.png\\%d.png", i + 1);
		loadimage(&attackWG[i], path);
	}

	for (int i = 0; i < 29; i++) // 加载阳光帧
	{
		snprintf(path, sizeof(path), "res\\sunshine\\%d.png", i + 1);
		loadimage(&imgSunshine[i], path);
	}

	for (int i = 0; i < 22; i++) // 加载僵尸行走帧
	{
		snprintf(path, sizeof(path), "res\\zm\\%d.png", i + 1);
		loadimage(&imgWalkZombies[i], path);
	}

	for (int i = 0; i < 20; i++) // 加载僵尸死亡帧
	{
		snprintf(path, sizeof(path), "res\\zm_dead\\%d.png", i + 1);
		loadimage(&imgDeadZombies[i], path);
	}

	for (int i = 0; i < 21; i++) // 加载僵尸吃帧
	{
		snprintf(path, sizeof(path), "res\\zm_eat\\%d.png", i + 1);
		loadimage(&imgEatZombies[i], path);
	}

	for (int i = 0; i < 11; i++) // 加载僵尸站立帧
	{
		snprintf(path, sizeof(path), "res\\zm_stand\\%d.png", i + 1);
		loadimage(&imgStandZombies[i], path);
	}

	loadimage(&bulletNormal, "res\\bullets\\bullet_normal.png"); // 加载常态子弹

	loadimage(&bulletBlast[3], "res\\bullets\\bullet_blast.png"); // 加载爆炸子弹,利用缩放实现爆炸效果
	for (int i = 0; i < 3; i++)
	{
		float factor = (i + 1) * 0.25;
		loadimage(&bulletBlast[i], "res\\bullets\\bullet_blast.png", bulletBlast[0].getwidth() * factor, bulletBlast[0].getheight() * factor, true);
	}
}

// 项目初始化
void initialize()
{
	// 内存初始化
	memset(imgPlants, 0, sizeof(imgPlants)); // 植物指针
	memset(plants, 0, sizeof(plants)); // 植物种植
	memset(dzh, 0, sizeof(dzh)); // 大嘴花池
	memset(crb, 0, sizeof(crb)); // 樱桃炸弹池
	memset(hblj, 0, sizeof(hblj)); // 火爆辣椒池
	memset(pm, 0, sizeof(pm)); // 土豆地雷池
	memset(wg, 0, sizeof(wg)); // 窝瓜池
	memset(sunshine, 0, sizeof(sunshine)); // 阳光池
	memset(zombies, 0, sizeof(zombies)); // 僵尸池
	memset(bullets, 0, sizeof(bullets)); // 子弹池初始化

	// 加载图片
	getImage();

	// 初始化随机数种子
	srand(time(NULL));

	// 窗口初始化
	initgraph(WIN_WIDTH, WIN_HEIGHT);

	// 字体设置
	setbkmode(TRANSPARENT);
	settextstyle(30, 0, "Segoe UI Black");
	settextcolor(BLACK);
}

// 透明贴图
void transparentimage(IMAGE* dstimg, int x, int y, IMAGE* srcimg) 
{
	HDC dstDC = GetImageHDC(dstimg);
	HDC srcDC = GetImageHDC(srcimg);
	int w = srcimg->getwidth();
	int h = srcimg->getheight();
	BLENDFUNCTION bf = { AC_SRC_OVER, 0, 255, AC_SRC_ALPHA };
	AlphaBlend(dstDC, x, y, w, h, srcDC, 0, 0, w, h, bf);
}

// 实现开始界面:注意要与主场景分开绘制，是两个独立的部分
void start()
{
	// 定义开始页面背景图，两个开始按钮，按钮按下标志，音乐播放标志
	IMAGE bkMn, bkStart1, bkStart2;
	bool isPressed = false;
	bool isPlayed = false;

	// 加载所需图片
	loadimage(&bkMn, "res\\menu.png");
	loadimage(&bkStart1, "res\\menu1.png");
	loadimage(&bkStart2, "res\\menu2.png");

	// 不断获取消息并进行画面绘制，直到开始按钮按下
	while (1)
	{
		// 绘制
		BeginBatchDraw();
		transparentimage(NULL, 0, 0, &bkMn);
		transparentimage(NULL, BUTTON_STARTX, BUTTON_STARTY, isPressed ? &bkStart1 : &bkStart2);
		EndBatchDraw();

		// 播放音乐
		if (isPlayed == false)
		{
			mciSendString("play res\\bg.mp3 repeat", NULL, 0, NULL);
			isPlayed = true;
		}

		// 消息获取
		ExMessage msg;
		if (peekmessage(&msg))
		{
			// 左键按下按钮并松开，即进入游戏
			if (msg.message == WM_LBUTTONDOWN && msg.x >= BUTTON_STARTX && msg.x < BUTTON_STARTX + bkStart1.getwidth() && msg.y >= BUTTON_STARTY && msg.y < BUTTON_STARTY + bkStart2.getheight())
				isPressed = true;
			else if (msg.message == WM_LBUTTONUP && isPressed)
			{
				mciSendString("play res\\zz_audio\\evillaugh.mp3", NULL, 0, NULL);
				Sleep(3100); // 等音乐放完啦
				return;
			}
		}
	}
}

// 开场的巡礼
void patrol()
{
	 // 定义巡礼僵尸数,僵尸巡礼最终落位,各自的帧索引
	const int count = 9; // C++支持，而C不支持用const变量初始化数组
	vector2 points[count] = {{550, 80}, {530, 160}, {630, 170}, {530, 200}, {515, 270}, {565, 370}, {605, 340}, {690, 340}, {705, 280}};
	int index[count];
	for (int i = 0; i < count; i++) // 随机确定某僵尸所在帧
		index[i] = rand() % 11;

	int times = 0; // 计数变量，控制帧切换频率
	int changeFrame = 5; // 帧切换频率
	int widthdif = -500; // 主视口宽与画面总宽之差
	int wspeed = 2; // 画面移动速度

	// 初次绘制
	BeginBatchDraw();
	putimage(0, 0, &imgBk); // 绘制背景图
	EndBatchDraw();
	mciSendString("stop res\\bg.mp3", NULL, 0, NULL); // 停止音乐，准备巡礼
	mciSendString("play res\\LookupattheSky.mp3", NULL, 0, NULL); // 播放下一曲
	Sleep(1000);

	for (int i = 0; i >= widthdif; i -= wspeed) // 巡礼
	{
		times++; // 更新计数变量
		BeginBatchDraw(); 
		putimage(i, 0, &imgBk); // 绘制背景图
		for (int k = 0; k < count; k++) // 绘制僵尸
		{
			transparentimage(NULL, points[k].x - widthdif + i, points[k].y, &imgStandZombies[index[k]]);
			if (times >= changeFrame) // 更新帧：注意不能在这里将times清零，否则有的僵尸帧更新，而有的没有
				index[k] = (index[k] + 1) % 11;
		}
		if (times >= changeFrame)
			times = 0;
		EndBatchDraw();
		Sleep(5); // 阻塞画面更新
	}

	int stayFrame = 30; // 驻留帧
	for (int i = 0; i < stayFrame; i++) // 巡礼进行到僵尸公路后，画面进行驻留
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

	times = 0; // 计数变量清零
	for (int i = widthdif; i <= FINAL_PAINTX; i += wspeed) // 巡礼拉回
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

// 卡牌栏滑下
void bardown()
{
	// 定义卡牌栏高度和下滑速度
	int barHeight = 100;
	int speed = 2;

	for (int y = -barHeight; y <= 0; y += speed)
	{
		BeginBatchDraw();
		putimage(FINAL_PAINTX, 0, &imgBk); // 绘制主背景图
		transparentimage(NULL, BAR_STARTX, y, &imgBr); // 绘制植物栏
		for (int i = 0; i < 8; i++) // 绘制卡牌
			transparentimage(NULL, CARDS_STARTX + i * CARD_WIDTH, CARDS_STARTY + y, &imgCards[i]);
		EndBatchDraw();
		Sleep(5);
	}
}

// 收集阳光
void collectSunshine(ExMessage* msg)
{
	// 遍历阳光池，判断鼠标是否落在某个正在使用的阳光上
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
				break; // 单次点击仅收集一个阳光
			}
		}
	}
}

// 创建大嘴花
void createDZH(int pos)
{
	// 遍历大嘴花池, 找一个未被使用的
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

// 创建樱桃炸弹
void createCB(int pos)
{
	// 遍历樱桃炸弹池, 找一个未被使用的
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

// 创建火爆辣椒
void createHB(int pos)
{
	// 遍历火爆辣椒池, 找一个未被使用的
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

// 创建土豆地雷
void createPM(int pos)
{
	// 遍历土豆地雷池, 找一个未被使用的
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

// 创建窝瓜
void createWG(int pos)
{
	// 遍历窝瓜池, 找一个未被使用的
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

// 玩家控制
void userControl()
{
	// 定义消息以及鼠标状态
	ExMessage msg;
	static int state = 0; // 使用局部静态变量。因为这个函数会不断调用，而上一轮的状态会影响下一轮的判定，故用之保存状态

	// 尝试获取消息,根据消息类型执行操作
	if (peekmessage(&msg))
	{
		// 左键按下
		if (msg.message == WM_LBUTTONDOWN)
		{
			// 判断是否在植物卡牌栏按下:左闭右开，上闭下开
			if (msg.x >= CARDS_STARTX && msg.x < CARDS_STARTX + CARD_WIDTH * 8 && msg.y >= CARDS_STARTY && msg.y < CARDS_STARTY + CARD_HEIGHT)
			{
				selIndex = (msg.x - CARDS_STARTX) / CARD_WIDTH + 1; // 被选中的植物序号
				if (sunshineValue >= sunshineCost[selIndex - 1]) // 判断阳光值是否够用
				{
					state = 1; // 按下状态
					curX = msg.x;
					curY = msg.y;
				}
				else
					selIndex = 0;
			}
			// 收集阳光
			else
				collectSunshine(&msg);
		}

		// 鼠标移动
		else if (msg.message == WM_MOUSEMOVE)
		{
			// 拖动状态,返回鼠标位置
			if (state == 1)
			{
				curX = msg.x;
				curY = msg.y;
			}
		}

		// 左键抬起
		else if (msg.message == WM_LBUTTONUP)
		{
			// 判断是否落在草坪区。若在，则判断具体位置，并修改指定位置植物种植情况
			if (msg.x >= GRASS_STARTX && msg.x < GRASS_STARTX + GRASSCOL * GRASSBLOCK_WIDTH && msg.y >= GRASS_STARTY && msg.y < GRASS_STARTY + GRASSROW * GRASSBLOCK_HEIGHT)
			{
				// 计算具体行列号
				int row = (msg.y - GRASS_STARTY) / GRASSBLOCK_HEIGHT;
				int col = (msg.x - GRASS_STARTX) / GRASSBLOCK_WIDTH;

				// 获得当前草坪块起始x,y值
				plants[row][col].x = GRASS_STARTX + col * GRASSBLOCK_WIDTH;
				plants[row][col].y = GRASS_STARTY + row * GRASSBLOCK_HEIGHT;

				// 该位置无植物种植时，将植物种下
				if (plants[row][col].type == ZERO)
				{
					if (selIndex > 0)
					{
						// 共有更新
						plants[row][col].type = selIndex;
						plants[row][col].blood = (selIndex == JG ? 2000 : 400);
						sunshineValue = sunshineValue - sunshineCost[selIndex - 1];

						// 创造
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
						
						// 播放音效
						PlaySound("res\\plant2.wav", NULL, SND_FILENAME | SND_ASYNC);
					}
				}
			}

			// 结束拖动状态，清零被选中的植物序号
			state = 0;
			selIndex = 0;
		}
	}
}

// 绘制大嘴花
void drawDZH(int x, int y)
{
	for (int i = 0; i < 27; i++)
	{
		if (dzh[i].used && dzh[i].pos == x * 10 + y)
		{
			// 求出位置
			int xpos = plants[x][y].x + GRASSBLOCK_WIDTH / 2 - normalDZH[0].getwidth() / 2;
			int ypos = plants[x][y].y + GRASSBLOCK_HEIGHT / 2 - normalDZH[0].getheight() / 2;

			// 根据不同状态进行绘制
			if (dzh[i].state == NORMAL)
				transparentimage(NULL, xpos, ypos, &normalDZH[dzh[i].frameIndex]);
			else if (dzh[i].state == ATTACK)
				transparentimage(NULL, xpos, ypos, &attackDZH[dzh[i].frameIndex]);
			else if (dzh[i].state == EAT)
				transparentimage(NULL, xpos, ypos, &eatDZH[dzh[i].frameIndex]);
			break; // 找到确切位置去种植
		}
	}
}

// 绘制樱桃炸弹
void drawCB(int x, int y)
{
	for (int i = 0; i < 27; i++)
	{
		if (crb[i].used && crb[i].pos == x * 10 + y)
		{
			// 求出位置
			int xpos = plants[x][y].x + GRASSBLOCK_WIDTH / 2 - bombCB[0].getwidth() / 2;
			int ypos = plants[x][y].y + GRASSBLOCK_HEIGHT / 2 - bombCB[0].getheight() / 2;

			// 根据不同状态进行绘制
			if (crb[i].state == BOMB)
				transparentimage(NULL, xpos, ypos, &bombCB[crb[i].frameIndex]);
			else if (crb[i].state == REST)
				transparentimage(NULL, xpos - 50, ypos - 50, &restCB[crb[i].frameIndex]);
			break;
		}
	}
}

// 绘制火爆辣椒
void drawHB(int x, int y)
{
	for (int i = 0; i < 27; i++)
	{
		if (hblj[i].used && hblj[i].pos == x * 10 + y)
		{
			// 求出位置
			int xpos = plants[x][y].x + GRASSBLOCK_WIDTH / 2 - bombHB[0].getwidth() / 2;
			int ypos = plants[x][y].y + GRASSBLOCK_HEIGHT / 2 - bombHB[0].getheight() / 2;

			// 根据不同状态进行绘制
			if (hblj[i].state == HBOMB)
				transparentimage(NULL, xpos, ypos, &bombHB[hblj[i].frameIndex]);
			else if (hblj[i].state == HREST)
				transparentimage(NULL, GRASS_STARTX, ypos - 60, &restHB[hblj[i].frameIndex]);
			break;
		}
	}
}

// 绘制土豆地雷
void drawPM(int x, int y)
{
	for (int i = 0; i < 27; i++)
	{
		if (pm[i].used && pm[i].pos == x * 10 + y)
		{
			// 求出位置
			int xpos = plants[x][y].x + GRASSBLOCK_WIDTH / 2 - normalPM[0].getwidth() / 2;
			int ypos = plants[x][y].y + GRASSBLOCK_HEIGHT / 2 - normalPM[0].getheight() / 2;

			// 根据不同状态进行绘制
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

// 绘制窝瓜
void drawWG(int x, int y)
{
	for (int i = 0; i < 27; i++)
	{
		if (wg[i].used && wg[i].pos == x * 10 + y)
		{
			// 求出位置
			int xpos = plants[x][y].x + GRASSBLOCK_WIDTH / 2 - normalWG[0].getwidth() / 2;
			int ypos = plants[x][y].y + GRASSBLOCK_HEIGHT / 2 - normalWG[0].getheight() / 2;

			// 根据不同状态进行绘制
			if (wg[i].state == WNORMAL)
				transparentimage(NULL, xpos, ypos, &normalWG[wg[i].frameIndex]);
			else if (wg[i].state == WATTACK)
				transparentimage(NULL, xpos + 15, ypos - 100, &attackWG[wg[i].frameIndex]);
			break;
		}
	}
}

// 绘制植物相关
void drawPlants()
{
	transparentimage(NULL, BAR_STARTX, 0, &imgBr); // 绘制植物卡牌栏

	for (int i = 0; i < 8; i++) // 绘制植物卡牌
		transparentimage(NULL, CARDS_STARTX + i * CARD_WIDTH, CARDS_STARTY, &imgCards[i]);

	for (int i = 0; i < GRASSROW; i++) // 绘制已种植植物
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

	if (selIndex > 0) // 有植物被选中时，根据鼠标位置进行拖动中绘制
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

// 绘制僵尸
void drawZombies()
{
	for (int i = 0; i < ZOMBIE_COUNT; i++) // 绘制僵尸池中被选中的僵尸
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

// 绘制阳光相关
void drawSunshine()
{
	char sunshine_text[LENGTH]; // 绘制阳光值
	snprintf(sunshine_text, sizeof(sunshine_text), "%d", sunshineValue);
	outtextxy(TEXT_STARTX + 33 - textwidth(sunshine_text) / 2, TEXT_STARTY + 12 - textheight(sunshine_text) / 2, sunshine_text);

	for (int i = 0; i < SUNSHINE_COUNT; i++) // 绘制阳光池中被使用的阳光
	{
		if (sunshine[i].used)
			transparentimage(NULL, sunshine[i].pCur.x, sunshine[i].pCur.y, &imgSunshine[sunshine[i].frameIndex]);
	}
}

// 绘制子弹
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

// 主场景渲染
void drawScr()
{
	// 使用双缓冲进行绘制
	BeginBatchDraw();
	putimage(-112, 0, &imgBk); // 绘制主背景图
	drawPlants(); // 绘制植物相关
	drawZombies(); // 绘制僵尸
	drawSunshine(); // 绘制阳光相关
	drawBullets(); // 绘制子弹
	EndBatchDraw();
}

// 更新大嘴花
void updateDZH(int x, int y)
{
	static int eatEdge = 300; // 大嘴花多久可以吃完僵尸

	for (int i = 0; i < 27; i++)
	{
		if (dzh[i].used && dzh[i].pos == x * 10 + y) // 找到对应的大嘴花
		{
			if (dzh[i].state == NORMAL) // 常态
				dzh[i].frameIndex = (dzh[i].frameIndex + 1) % 8;
			else if (dzh[i].state == ATTACK) // 攻击态
			{
				if (dzh[i].frameIndex < 8)
					dzh[i].frameIndex = (dzh[i].frameIndex + 1);
				if (dzh[i].frameIndex == 8) // 播放音效
					PlaySound("res\\chomp.wav", NULL, SND_FILENAME | SND_ASYNC);
			}
			else if (dzh[i].state == EAT) // 咀嚼态
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

// 更新樱桃炸弹
void updateCB(int x, int y)
{
	for (int i = 0; i < 27; i++)
	{
		if (crb[i].used && crb[i].pos == x * 10 + y) // 找到对应的樱桃炸弹
		{
			if (crb[i].state == BOMB) // 爆炸态
			{
				if (crb[i].frameIndex < 7)
					crb[i].frameIndex = crb[i].frameIndex + 1;
			}
			else if (crb[i].state == REST) // 余烬态
			{
				if (crb[i].frameIndex == 0) // 播放爆炸音效
					PlaySound("res\\cherrybomb.wav", NULL, SND_FILENAME | SND_ASYNC);
				if (crb[i].frameIndex < 12)
					crb[i].frameIndex = crb[i].frameIndex + 1;
			}
			break;
		}
	}
}

// 更新火爆辣椒
void updateHB(int x, int y)
{
	for (int i = 0; i < 27; i++)
	{
		if (hblj[i].used && hblj[i].pos == x * 10 + y) // 找到对应的火爆辣椒
		{
			if (hblj[i].state == HBOMB) // 爆炸态
			{
				if (hblj[i].frameIndex < 8)
					hblj[i].frameIndex = hblj[i].frameIndex + 1;
			}
			else if (hblj[i].state == HREST) // 余烬态
			{
				if (hblj[i].frameIndex == 0) // 播放音效
					PlaySound("res\\jalapeno.wav", NULL, SND_FILENAME | SND_ASYNC);
				if (hblj[i].frameIndex < 8)
					hblj[i].frameIndex = hblj[i].frameIndex + 1;
			}
			break;
		}
	}
}

// 更新土豆地雷
void updatePM(int x, int y)
{
	for (int i = 0; i < 27; i++)
	{
		if (pm[i].used && pm[i].pos == x * 10 + y) // 找到对应的土豆地雷
		{
			if (pm[i].state == PHIDE) // 隐藏态
			{
				if (pm[i].frameIndex < 20)
					pm[i].frameIndex = pm[i].frameIndex + 1;
			}
			else if (pm[i].state == PNORMAL) // 普通态
				pm[i].frameIndex = (pm[i].frameIndex + 1) % 8;
			else if (pm[i].state == PREST) // 余烬态
			{
				if (pm[i].frameIndex == 0) // 播放音效
					PlaySound("res\\potato_mine.wav", NULL, SND_FILENAME | SND_ASYNC);
				if (pm[i].frameIndex < 6)
					pm[i].frameIndex = pm[i].frameIndex + 1;
			}
			break;
		}
	}
}

// 更新窝瓜
void updateWG(int x, int y)
{
	for (int i = 0; i < 27; i++)
	{
		if (wg[i].used && wg[i].pos == x * 10 + y) // 找到对应的窝瓜
		{
			if (wg[i].state == WNORMAL) // 普通态
				wg[i].frameIndex = (wg[i].frameIndex + 1) % 17;
			else if (wg[i].state == WATTACK) // 攻击态
			{
				if (wg[i].frameIndex < 4)
					wg[i].frameIndex = wg[i].frameIndex + 1;
			}
			break;
		}
	}
}

// 创建阳光
void createSunshine()
{
	// 创建垂直下落的阳光
	static int count = 0; // 计数器，控制生成频率
	static int highFre = 500;
	static int lowFre = 300;
	static int feq = 300;
	count++;

	if (count >= feq) // 计数器到达界限
	{
		// 计数清零,同时更新阳光创建频数(使阳光刷新具有随机性)
		count = 0;
		feq = lowFre + rand() % (highFre - lowFre + 1); // 频数控制在300-500

		// 遍历阳光池，找到可以使用的阳光，进行创建
		int index;
		for (index = 0; index < SUNSHINE_COUNT && sunshine[index].used; index++);

		if (index == SUNSHINE_COUNT) // 阳光已全在使用，无法创建新阳光
			return;
		else // 创建新的阳光
		{
			static int yStart = 60;
			static double v = 2.;
			sunshine[index].used = true;
			sunshine[index].state = SUNSHINE_DOWN;
			sunshine[index].frameIndex = 0;
			sunshine[index].time = 0;
			sunshine[index].t = 0;
			sunshine[index].pCur.x = BAR_STARTX + rand() % (WIN_WIDTH - BAR_STARTX + FINAL_PAINTX + 1); // 阳光刷新位置X
			sunshine[index].pCur.y = yStart;
			sunshine[index].p1 = vector2(sunshine[index].pCur.x, sunshine[index].pCur.y);
			sunshine[index].p4 = vector2(sunshine[index].pCur.x, GRASS_STARTY + rand() % (GRASSROW + 1) * GRASSBLOCK_HEIGHT);
			sunshine[index].speed = v / dis(sunshine[index].p1 - sunshine[index].p4);
		}
	}

	// 向日葵产生阳光
	static int fre = 300; // 阳光产生时间界限
	for (int i = 0; i < GRASSROW; i++)
	{
		for (int j = 0; j < GRASSCOL; j++)
		{
			if (plants[i][j].type == XRK) // 是向日葵，则更新计时器
			{
				plants[i][j].timer++;
				if (plants[i][j].timer >= fre) // 计时器到达界限，则产生阳光
				{
					plants[i][j].timer = 0; // 清零计时器

					int index; // 找可用阳光
					for (index = 0; index < SUNSHINE_COUNT && sunshine[index].used; index++);

					if (index == SUNSHINE_COUNT)
						return;
					else // 产生阳光
					{
						static int left = 120; // 阳光落下位置左界限
						static int right = 10; // 阳光落下位置右界限
						int off = (left + rand() % right + 1) * (rand() % 2 ? 1 : -1); // 阳光落下偏移量

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

// 更新阳光
void updateSunshine()
{
	 // 遍历阳光池，找到正在使用的阳光，修改其数据
	 static int timeEdge = 130; // 阳光滞留时间界限
	 for (int i = 0; i < SUNSHINE_COUNT; i++)
	 {
		 if (sunshine[i].used)
		 {
			 sunshine[i].frameIndex = (++sunshine[i].frameIndex) % 29; // 更改绘制帧

			 if (sunshine[i].state == SUNSHINE_DOWN) // 阳光处于下落状态
			 {
				 sunshine[i].t += sunshine[i].speed; // 更细阳光实时位置
				 sunshine[i].pCur = sunshine[i].p1 + sunshine[i].t * (sunshine[i].p4 - sunshine[i].p1);
				 if (sunshine[i].t >= 1) // 到达终点后
				 {
					 sunshine[i].t = 0;
					 sunshine[i].state = SUNSHINE_STAY;
				 }
			 }
			 else if (sunshine[i].state == SUNSHINE_STAY) // 阳光处于滞留状态
			 {
				 sunshine[i].time++;
				 if (sunshine[i].time > timeEdge)
				 {
					 sunshine[i].time = 0;
					 sunshine[i].used = false;
				 }
			 }
			 else if (sunshine[i].state == SUNSHINE_GEN) // 向日葵产生阳光
			 {
				 sunshine[i].t += sunshine[i].speed;
				 sunshine[i].pCur = calcBezierPoint(sunshine[i].t, sunshine[i].p1, sunshine[i].p2, sunshine[i].p3, sunshine[i].p4);
				 if (sunshine[i].t >= 1)
				 {
					 sunshine[i].state = SUNSHINE_STAY;
					 sunshine[i].t = 0;
				 }
			 }
			 else if (sunshine[i].state == SUNSHINE_COL) // 阳光被收集
			 {
				 sunshine[i].t += sunshine[i].speed;
				 sunshine[i].pCur = sunshine[i].p1 + sunshine[i].t * (sunshine[i].p4 - sunshine[i].p1);
				 if (sunshine[i].t >= 1)
				 {
					 sunshine[i].t = 0;
					 sunshine[i].used = false;
					 sunshineValue += SUNSHINE_VALUE; // 更新阳光值
				 }
			 }
		 }
	 }
}

// 创建僵尸
void createZombies()
{
	// 判断是否已经达到最大僵尸创建数
	static int maxZombies = 60;
	if (zmCount == maxZombies)
		return;

	// 计数器控制创建频率
	static int count = 0;
	static int lowF = 250;
	static int highF = 350;
	static int fre = 250;
	count++;

	if (count > fre) // 到达界限
	{
		count = 0; // 更新计数器和创建频率
		fre = lowF + rand() % (highF - lowF + 1); 

		int index; // 找到未使用的僵尸
		for (index = 0; index < ZOMBIE_COUNT && zombies[index].used; index++);

		if (index < ZOMBIE_COUNT) // 创建僵尸
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
			if (zmCount == 1) // 播放音效
			{
				mciSendString("play res\\awooga.mp3", NULL, 0, NULL); // 提示音
				mciSendString("play res\\zz_audio\\UraniwaNi.mp3 repeat", NULL, 0, NULL); // 开始播放背景音
				mciSendString("stop res\\LookupattheSky.mp3", NULL, 0, NULL); // 停止上一曲
			}
		}
		else
			return;
	}
}

// 更新僵尸
void updateZombies()
{
	// 采用计数方式对僵尸进行降速
	static int count = 0;
	static int limit = 4;
	count++;

	if (count > limit) // 更新僵尸
	{
		count = 0;
		for (int i = 0; i < ZOMBIE_COUNT; i++)
		{
			// 找到使用中的僵尸，若游戏未失败，则更新僵尸
			if (zombies[i].used)
			{
				static int housePos = 23;
				if (zombies[i].x <= housePos) // 游戏判负
					gameState = LOSE;
				else
				{
					zombies[i].x -= zombies[i].speed;
					zombies[i].col = (zombies[i].x + 100 - GRASS_STARTX) / GRASSBLOCK_WIDTH;
					if (zombies[i].dead) // 僵尸死了
					{
						zombies[i].frameIndex++;
						if (zombies[i].frameIndex == 20) // 死亡动画结束
						{
							zombies[i].used = false;
							killCount++; // 更新击杀数
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

// 子弹发射
void shoot()
{
	// 判断某行是否有僵尸
	int lines[GRASSROW] = { 0 }; // 行标志
	int dangerx = WIN_WIDTH - imgWalkZombies[0].getwidth() + GRASSBLOCK_WIDTH / 2;

	for (int i = 0; i < ZOMBIE_COUNT; i++) // 遍历僵尸，更新行标志
	{
		if (zombies[i].used && zombies[i].x < dangerx)
			lines[zombies[i].row - 1] = 1;
	}

	// 遍历植物种植数组,找出豌豆
	static int shootEdge = 80; // 子弹发射界限
	for (int i = 0; i < GRASSROW; i++)
	{
		for (int j = 0; j < GRASSCOL; j++)
		{
			// 某位置种植了豌豆，且该行有僵尸
			if (plants[i][j].type == WD && lines[i] == 1)
			{
				// 更新子弹发射计时器
				plants[i][j].shoottime++;

				if (plants[i][j].shoottime >= shootEdge)
				{
					plants[i][j].shoottime = 0; 

					int k; // 找可用子弹
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

// 更新子弹
void updateBullets()
{
	// 遍历子弹池，找到正在使用的子弹
	for (int i = 0; i < BULLET_COUNT; i++)
	{
		if (bullets[i].used)
		{
			// 子弹移动
			bullets[i].x += bullets[i].speed;
			if (bullets[i].x > WIN_WIDTH)
				bullets[i].used = false;
			
			// 子弹爆裂
			if (bullets[i].blast)
			{
				bullets[i].blastFrame++;
				if (bullets[i].blastFrame == 4)
					bullets[i].used = false;
			}
		}
	}
}

// 检测僵尸和子弹
void checkZombiestoBullets()
{
	for (int i = 0; i < BULLET_COUNT; i++)
	{
		// 子弹处于非使用状态/爆裂状态直接跳过
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
					// 进行检测:定义左右检测边界并检查
					int left_check = zombies[j].x + 80;
					int right_check = zombies[j].x + 115;

					if (bullets[i].row == zombies[j].row && !zombies[j].dead && bullets[i].x >= left_check && bullets[i].x <= right_check)
					{
						// 播放击中音效
						PlaySound("res\\hitzombie.wav", NULL, SND_FILENAME | SND_ASYNC);
		
						// 更新僵尸血量以及子弹状态
						zombies[j].blood -= BULLET_HURT;
						bullets[i].blast = true;
						bullets[i].speed = 0;

						// 判断僵尸是否死亡
						if (zombies[j].blood <= 0)
						{
							zombies[j].dead = true;
							zombies[j].speed = 0;
							zombies[j].frameIndex = 0;
						}

						break; // 一发子弹只能打一个僵尸
					}
			}
				}
		}
	}
}

// 检测僵尸和植物
void checkZombiestoPlants()
{
	for (int i = 0; i < ZOMBIE_COUNT; i++)
	{
		// 僵尸若死亡，则无需判断
		if (zombies[i].dead)
			continue;
		else
		{
			// 判断僵尸是否够得到同一行的植物
			for (int j = 0; j < GRASSCOL; j++)
			{
				if (plants[zombies[i].row - 1][j].type != ZERO)
				{
					// 定义检测边界
					int leftCheck = GRASS_STARTX + j * GRASSBLOCK_WIDTH + 10;
					int rightCheck = GRASS_STARTX + j * GRASSBLOCK_WIDTH + 60;

					if (zombies[i].x + 80 >= leftCheck && zombies[i].x + 80 <= rightCheck)
					{
						if (plants[zombies[i].row - 1][j].isEaten) // 当前植物是否已经被某只僵尸吃
						{
							plants[zombies[i].row - 1][j].blood -= ZOMBIE_HURT; // 降低血量
							if (!zombies[i].eat) // 判断当前僵尸是否在吃
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

						if (plants[zombies[i].row - 1][j].blood <= 0) // 当前植物没血了
						{
							// 更新植物
							if (plants[zombies[i].row - 1][j].type == DZH) // 如果是大嘴花
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
							else if (plants[zombies[i].row - 1][j].type == YT) // 如果是樱桃
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
							else if (plants[zombies[i].row - 1][j].type == LJ) // 如果是火爆辣椒
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
							else if (plants[zombies[i].row - 1][j].type == TD) // 如果是土豆地雷
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
							else if (plants[zombies[i].row - 1][j].type == WG) // 如果是窝瓜
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
							
							// 更新僵尸
							for (int k = 0; k < ZOMBIE_COUNT; k++)
							{
								if (zombies[k].eatwho == (zombies[i].row - 1) * 10 + j) // 每一个在吃当前植物的都要更新
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

// 检测僵尸和大嘴花
void checkZombiestoDZH()
{
	for (int i = 0; i < 27; i++)
	{
		if (dzh[i].used) // 大嘴花可以用
		{
			int dzhCol = dzh[i].pos % 10; // 大嘴花的列
			int dzhRow = dzh[i].pos / 10; // 大嘴花的行

			for (int j = 0; j < ZOMBIE_COUNT; j++)
			{
				if (zombies[j].dead || !(dzhRow == zombies[j].row - 1)) // 僵尸死了/僵尸和大嘴花不在同一行
					continue;
				else
				{
					// 定义左右边界,并检测
					int leftCheck = GRASS_STARTX + dzhCol * GRASSBLOCK_WIDTH + 10;
					int rightCheck = GRASS_STARTX + dzhCol * GRASSBLOCK_WIDTH + 60;

					if (zombies[j].x + 80 >= leftCheck && zombies[j].x + 80 <= rightCheck)
					{
						if (dzh[i].state == NORMAL) // 处于常态
						{
							dzh[i].state = ATTACK;
							dzh[i].frameIndex = 0;
						}
						else if (dzh[i].state == ATTACK) // 发起攻击
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
								break; // 只攻击一只僵尸
							}
						}
					}
				}
			}
		}
	}
}

// 樱桃炸弹爆炸
void cbBomb()
{
	for (int i = 0; i < 27; i++)
	{
		if (crb[i].used) // 遍历樱桃池,找到使用中的樱桃
		{
			int ytCol = crb[i].pos % 10; // 樱桃的列
			int ytRow = crb[i].pos / 10; // 樱桃的行

			if (crb[i].state == BOMB) // 处于爆炸态
			{
				if (crb[i].frameIndex == 7)
				{
					crb[i].state = REST; // 更新樱桃
					crb[i].frameIndex = 0;
					
					for (int k = 0; k < ZOMBIE_COUNT; k++) // 更新僵尸
					{
						if (zombies[k].dead) continue;
						if ((zombies[k].row - 1 == ytRow || zombies[k].row - 1 == ytRow - 1 || zombies[k].row - 1 == ytRow + 1) // 九宫格范围内僵尸
							&& (zombies[k].col == ytCol || zombies[k].col  == ytCol - 1 || zombies[k].col  == ytCol + 1))
						{
							zombies[k].dead = true;
							zombies[k].frameIndex = 0;
							zombies[k].speed = 0;
						}
					}
				}
			}
			else if (crb[i].state == REST) // 处于余烬态
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

// 火爆辣椒爆炸
void hbBomb()
{
	for (int i = 0; i < 27; i++)
	{
		if (hblj[i].used) // 找到使用中的火爆辣椒
		{
			int hbCol = hblj[i].pos % 10; // 火爆辣椒的列
			int hbRow = hblj[i].pos / 10; // 火爆辣椒的行

			if (hblj[i].state == HBOMB) // 处于爆炸态
			{
				if (hblj[i].frameIndex == 8)
				{
					hblj[i].state = HREST; // 更新火爆辣椒
					hblj[i].frameIndex = 0;

					for (int k = 0; k < ZOMBIE_COUNT; k++) // 更新僵尸
					{
						if (zombies[k].dead) 
							continue;
						if (zombies[k].row - 1 == hbRow) // 同一行的僵尸
						{
							zombies[k].dead = true;
							zombies[k].frameIndex = 0;
							zombies[k].speed = 0;
						}
					}
				}
			}
			else if (hblj[i].state == HREST) // 处于余烬态
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

// 土豆地雷爆炸
void pmBomb()
{
	for (int i = 0; i < 27; i++)
	{
		if (pm[i].used) // 找到使用中的土豆地雷
		{
			int pmCol = pm[i].pos % 10; // 土豆地雷的列
			int pmRow = pm[i].pos / 10; // 土豆地雷的行
			int leftCheck = GRASS_STARTX + pmCol * GRASSBLOCK_WIDTH + 10; // 定义左右边界,并检测
			int rightCheck = GRASS_STARTX + pmCol * GRASSBLOCK_WIDTH + 60;
			bool bob = false; // 是否有僵尸处于爆炸范围

			if (pm[i].state == PHIDE) // 隐藏
			{
				if (pm[i].frameIndex == 20)
				{
					pm[i].frameIndex = 0;
					pm[i].state = PNORMAL;
				}
			}
			else if (pm[i].state == PNORMAL) // 处于正常态
			{
				for (int j = 0; j < ZOMBIE_COUNT; j++) // 遍历僵尸池，看是否有踩雷的
				{
					if (zombies[j].dead || !(pmRow == zombies[j].row - 1)) // 僵尸死了/僵尸和土豆地雷不在同一行
						continue;
					else
					{
						if (zombies[j].x + 80 >= leftCheck && zombies[j].x + 80 <= rightCheck) // 进入攻击范围
						{
							zombies[j].dead = true;
							zombies[j].speed = 0;
							zombies[j].frameIndex = 0;
							bob = true;
						}
					}
				}
				if (bob == true) // 爆炸状态
				{
					pm[i].state = PREST;
					pm[i].frameIndex = 0;
				}
			}
			else if (pm[i].state == PREST) // 处于余烬态
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

// 窝瓜攻击
void wgAttack()
{
	for (int i = 0; i < 27; i++)
	{
		if (wg[i].used) // 找到使用中的窝瓜
		{
			bool attack = false; // 判断是否有目标
			int wgCol = wg[i].pos % 10; // 窝瓜的列
			int wgRow = wg[i].pos / 10; // 窝瓜的行

			int leftCheck = GRASS_STARTX + wgCol * GRASSBLOCK_WIDTH + 10; // 定义左右边界,并检测
			int rightCheck = GRASS_STARTX + wgCol * GRASSBLOCK_WIDTH + 60;

			if (wg[i].state == WNORMAL) // 处于正常态
			{
				for (int j = 0; j < ZOMBIE_COUNT; j++) // 遍历僵尸池
				{
					if (zombies[j].dead || !(wgRow == zombies[j].row - 1)) // 僵尸死了/僵尸和窝瓜不在同一行
						continue;
					else if (zombies[j].x + 80 >= leftCheck && zombies[j].x + 80 <= rightCheck) // 进入攻击范围
					{
						attack = true;
						zombies[j].used = false; // 更新僵尸
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
			else if (wg[i].state == WATTACK) // 处于攻击态
			{
				if (wg[i].frameIndex == 4)
				{
					plants[wgRow][wgCol].type = ZERO; // 更新植物
					wg[i].used = false;
					wg[i].frameIndex = 0;
					wg[i].pos = -1;
					wg[i].state = WNONE;
				}
			}
		}
	}
}

// 交互检测
void collisionCheck()
{
	// 僵尸和子弹
	checkZombiestoBullets();

	// 僵尸吃植物
	checkZombiestoPlants();

	// 大嘴花攻击僵尸
	checkZombiestoDZH();

	// 爆炸
	cbBomb(); // 樱桃炸弹爆炸
	hbBomb(); // 火爆辣椒爆炸
	pmBomb(); // 土豆地雷爆炸

	// 窝瓜攻击僵尸
	wgAttack();
}

// 更新游戏数据
void updateGame()
{
	// 修改植物渲染帧
	static int times = 0; // 计数器，用于控制帧修改
	static int fre = 6; // 界限
	times++;

	if (times >= fre) // 计数器达到界限
	{	
		times = 0;
		for (int i = 0; i < GRASSROW; i++)
		{
			for (int j = 0; j < GRASSCOL; j++)
			{
				if (plants[i][j].type > 0) // 对于已种植植物，修改其帧
				{
					if (plants[i][j].type == DZH) // 大嘴花
						updateDZH(i, j);
					else if (plants[i][j].type == YT) // 樱桃炸弹
						updateCB(i, j);
					else if (plants[i][j].type == LJ) // 火爆辣椒
						updateHB(i, j);
					else if (plants[i][j].type == TD) // 土豆地雷
						updatePM(i, j);
					else if (plants[i][j].type == WG) // 窝瓜
						updateWG(i, j);
					else
						plants[i][j].frameIndex = (++plants[i][j].frameIndex) % 8;
				}	
			}
		}
	}
	
	// 阳光相关
	createSunshine(); // 创建阳光
	updateSunshine(); // 更新阳光

	// 僵尸相关
	createZombies(); // 创建僵尸
	updateZombies(); // 更新僵尸

	// 子弹相关
	shoot(); // 发射
	updateBullets(); // 更新子弹

	// 交互检测
	collisionCheck(); 
}

// 检查游戏情况
bool checkOver()
{
	bool ret = false; // 返回值初始为假
	IMAGE img; // 定义结算图

	if (gameState == WIN) // 获胜
	{
		loadimage(&img, "res\\win2.png");
		transparentimage(NULL, 0, 0, &img);
		mciSendString("stop res\\zz_audio\\UraniwaNi.mp3", NULL, 0, NULL); // 先停止背景音
		PlaySound("res\\win.wav", NULL, SND_FILENAME | SND_ASYNC);
		Sleep(4000); // 等待4S
		ret = true;
	}
	else if (gameState == LOSE) // 输了
	{
		loadimage(&img, "res\\fail2.png");
		transparentimage(NULL, 0, 0, &img);
		mciSendString("stop res\\zz_audio\\UraniwaNi.mp3", NULL, 0, NULL);
		PlaySound("res\\lose.wav", NULL, SND_FILENAME | SND_ASYNC);
		Sleep(5000); // 等待5S
		ret = true;
	}
	return ret;
}