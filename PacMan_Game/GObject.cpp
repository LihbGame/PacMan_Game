#include "stdafx.h"
#include "GObject.h"

using namespace std;

GMap *GObject::pStage = NULL;       

int GObject::GetRow()         //获取行
{
	return m_nRow;
}


int GObject::GetArray()      //返回数组首地址
{
	return m_nArray;
}

int GObject::PtTransform(int k)//坐标转换函数
{
	return (k - (pStage->LD) / 2) / pStage->LD;
}

//判断物体是否到达逻辑坐标位置
bool GObject::Achive()
{
	int n = (m_ptCenter.x - pStage->LD / 2) % pStage->LD;//计算x坐标的逻辑位置
	int k = (m_ptCenter.y - pStage->LD / 2) % pStage->LD;//计算y坐标的逻辑位置
	bool l = (n == 0 && k == 0);                         //如果两个余数都为0.说明到达每个逻辑方块的中心位置
	return l;
}

//到达逻辑点后更新数据
void GObject::AchiverCtrl()
{
	if (Achive())            //如果到达逻辑坐标
	{
		m_nArray = PtTransform(m_ptCenter.x);//更新列
		m_nRow = PtTransform(m_ptCenter.y);  //更新行
	} 
}

void GObject::DrawBlank(HDC &hdc)
{
	//申请资源，并交给智能指针处理
	HBRUSH hbr = ::CreateSolidBrush(RGB(255,255,255));//创建画刷，绘制矩形函数要求使用
	//shared_ptr<HBRUSH> phbr(&hbr);//把资源交给智能指针处理，并自动释放

	RECT rect;
	rect.top = m_ny - RD;
	rect.left = m_nx - RD;
	rect.right = m_nx + RD;
	rect.bottom = m_ny + RD;
	FillRect(hdc,&rect,hbr);//绘制矩形
	DeleteObject(hbr);
}

//碰撞检测
bool GObject::Collision()
{
	bool b = false;

	//更新行、列的数据若是大嘴,则会执行PacMan重写的AchiveCtrl函数消除豆子
	AchiverCtrl();
	//判断指令的有效性
	if (m_nArray < 0 || m_nRow < 0 || m_nArray > MAPLENTH - 1
		|| m_nRow > MAPLENTH - 1) {
		b = true;
	}
	else if (Achive()) {
		//switch (m_cmd) {  //判断行进的方向
		//case LEFT:
		//	//判断下一个格子是否能够通行
		//	if (m_nArray > 0 &&
		//		!pStage->mapData[m_nRow][m_nArray - 1]) {
		//		b = true;//指令无效
		//	}
		//	break;
		//	//以下方向的判断原理相同
		//case RIGHT:
		//	if (m_nArray < MAPLENTH - 1 &&
		//		!pStage->mapData[m_nRow][m_nArray + 1]) {
		//		b = true;
		//	}
		//	break;
		//case UP:
		//	if (m_nRow > 0 &&
		//		!pStage->mapData[m_nRow - 1][m_nArray]) {
		//		b = true;
		//	}
		//	break;
		//case DOWN:
		//	if (m_nRow < MAPLENTH - 1 &&
		//		!pStage->mapData[m_nRow + 1][m_nArray]) {
		//		b = true;
		//	}
		//	break;
		//}
		//if (!b) {
			m_dir = m_cmd; //没撞墙,指令成功
		//}
	}
	//依照真实的方向位移
	m_nx = m_ptCenter.x;
	m_ny = m_ptCenter.y;
	int MAX = pStage->LD * MAPLENTH + pStage->LD / 2;
	int MIN = pStage->LD / 2;
	switch (m_dir) {  //判断行进的方向
	case LEFT:
		//判断下一个格子是否能够通行
		if (m_nArray > 0 &&
			!pStage->mapData[m_nRow][m_nArray - 1]) {
			b = true;
			break;//"撞墙了"
		}
		m_ptCenter.x -= m_nSpeed;
		if (m_ptCenter.x < MIN) {
			m_ptCenter.x = MAX;
		}

		break;
		//以下方向的判断原理相同
	case RIGHT:
		if (m_nArray < MAPLENTH - 1 &&
			!pStage->mapData[m_nRow][m_nArray + 1]) {
			b = true;
			break;//"撞墙了"
		}
		m_ptCenter.x += m_nSpeed;
		if (m_ptCenter.x > MAX) {
			m_ptCenter.x = MIN;
		}

		break;
	case UP:
		if (m_nRow > 0 &&
			!pStage->mapData[m_nRow - 1][m_nArray]) {
			b = true;
			break;//"撞墙了"
		}
		m_ptCenter.y -= m_nSpeed;
		if (m_ptCenter.y < MIN) {
			m_ptCenter.y = MAX;
		}
		break;
	case DOWN:
		if (m_nRow < MAPLENTH - 1 &&
			!pStage->mapData[m_nRow + 1][m_nArray]) {
			b = true;
			break;//"撞墙了"
		}
		m_ptCenter.y += m_nSpeed;
		if (m_ptCenter.y > MAX) {
			m_ptCenter.y = MIN;
		}
		break;
	}
	return b;
}

//设置中心位置
void GObject::SetPosition(int Row, int Array)
{
	m_nRow = Row;
	m_nArray = Array;
	this->m_ptCenter.y = m_nRow * pStage->LD + pStage->LD / 2;
	this->m_ptCenter.x = m_nArray * pStage->LD + pStage->LD / 2;
}

GObject::~GObject()
{
}




//PacMan的成员定义：

void PacMan::AchiverCtrl()
{
	GObject::AchiverCtrl();
	if (Achive())
	{
		if (m_nArray >= 0 && m_nArray < MAPLENTH    //防止数组越界
			&&m_nRow >= 0 && m_nRow < MAPLENTH)
		{
			if (pStage->peaMapData[m_nRow][m_nArray])//判断该位置是否有豆子，如果有豆子就取消豆子的显示
			{
				pStage->peaMapData[m_nRow][m_nArray] = false;
			}
		}
	}
}


void PacMan::action() //大嘴的移动
{
	Collision();      //进行碰撞检测（包括移动的功能）
}

void PacMan::SetTwCommand(TWARDS command)
{
	m_cmd = command; //设置移动方向
}

bool PacMan::IsOver()
{
	return m_dir == OVER;   //判断游戏是否结束
}

bool PacMan::IsWin()       //判断玩家是否取得胜利
{
	for (int i = 0; i < MAPLENTH; i++)
	{
		for (int j = 0; j < MAPLENTH; j++)
		{
			if (pStage->peaMapData[i][j] == true)
			{
				return FALSE;           //如果存在任何一个豆子，就没取得胜利
			}
		}
	}
	return true;                        //没有豆子玩家取得胜利
}


POINT PacMan::GetPos()
{
	return m_ptCenter;                  //返回对象的中心位置
}


void PacMan::SetOver()
{
	m_dir = OVER;                       //设置游戏结束
}

void PacMan::Draw(HDC &memDC)
{
	if (m_dir == OVER)                 //游戏结束，什么也不做
	{
	}
	else if (m_nFrame % 4 != 0)        //第四帧动画和第二动画：张嘴形状
	{
		int x1 = 0, x2 = 0, y1 = 0, y2 = 0;
		int offsetx = DISTANCE / 2 + D_OFFSET;//弧弦交点x
		int offsety = DISTANCE / 2 + D_OFFSET;//弧线交点y
		switch (m_dir)
		{
		case UP:              //向上移动
			x1 = m_ptCenter.x - offsetx;
			x2 = m_ptCenter.x + offsetx;
			y1 = y2 = m_ptCenter.y - offsety;
			break;
		case DOWN:           //向下移动
			x1 = m_ptCenter.x + offsetx;
			x2 = m_ptCenter.x - offsetx;
			y1 = y2 = m_ptCenter.y + offsety;
			break;
		case LEFT:          //向左移动
			x2 = x1 = m_ptCenter.x - offsetx;
			y1 = m_ptCenter.y + offsety;
			y2 = m_ptCenter.y - offsetx;
			break;
		case RIGHT:         //向右移动
			x1 = x2 = m_ptCenter.x + offsetx;
			y1 = m_ptCenter.y - offsety;
			y2 = m_ptCenter.y + offsety;
			break;
		}
		//画出弧线部分
		Arc(memDC, m_ptCenter.x - DISTANCE, m_ptCenter.y - DISTANCE
			, m_ptCenter.x + DISTANCE, m_ptCenter.y + DISTANCE
			, x1, y1, x2, y2);
		//画直线部分，组合成玩家对象
		MoveToEx(memDC, x1, y1, NULL);
		LineTo(memDC,m_ptCenter.x,m_ptCenter.y);
		LineTo(memDC, x2, y2);
	}
	else if (m_nFrame%5!=0)         //第三帧动画，画出圆形
	{
		Ellipse(memDC,m_ptCenter.x-DISTANCE,m_ptCenter.y-DISTANCE
		,m_ptCenter.x+DISTANCE,m_ptCenter.y+DISTANCE);

		MoveToEx(memDC,m_ptCenter.x,m_ptCenter.y,NULL);
		if (m_dir == LEFT)
			LineTo(memDC, m_ptCenter.x - DISTANCE, m_ptCenter.y);
		else if(m_dir == RIGHT)
			LineTo(memDC, m_ptCenter.x + DISTANCE, m_ptCenter.y);
		else if(m_dir==UP)
			LineTo(memDC, m_ptCenter.x, m_ptCenter.y - DISTANCE);
		else
			LineTo(memDC, m_ptCenter.x , m_ptCenter.y+ DISTANCE);
	}
	else                           //嘴完全张开的状态
	{
		int x1 = 0, x2 = 0, y1 = 0, y2 = 0;
		switch (m_dir)
		{
		case UP:
			x1 = m_ptCenter.x - DISTANCE;
			x2 = m_ptCenter.x + DISTANCE;
			y1 = y2 = m_ptCenter.y;
			break;
		case DOWN:
			x1 = m_ptCenter.x + DISTANCE;
			x2 = m_ptCenter.x + DISTANCE;
			y2 = y1 = m_ptCenter.y;
			break;
		case LEFT:
			x1 = x2 = m_ptCenter.x;
			y1 = m_ptCenter.y + DISTANCE;
			y2 = m_ptCenter.y - DISTANCE;
			break;
		case RIGHT:
			x1 = x2 = m_ptCenter.x;
			y1 = m_ptCenter.y - DISTANCE;
			y2 = m_ptCenter.y + DISTANCE;
			break;
		}
		Arc(memDC, m_ptCenter.x - DISTANCE, m_ptCenter.y - DISTANCE
			, m_ptCenter.x + DISTANCE,  m_ptCenter.y + DISTANCE
			, x1, y1, x2, y2);
		MoveToEx(memDC,x1,y1,NULL);
		LineTo(memDC,x2,y2);
	}
	m_nFrame++;      //准备绘制下一帧
}


/*enermy成员定义*/
shared_ptr<PacMan>Enermy::player = nullptr;

//抓住玩家，游戏结束
void Enermy::Catch()
{
	int DX = m_ptCenter.x - player->GetPos().x;
	int DY = m_ptCenter.y - player->GetPos().y;
	if ((-RD < DX&&DX < RD) && (-RD < DY&&DY < RD))
	{
		player->SetOver();
	}
}

void Enermy::Draw(HDC &hdc)
{
	HPEN pen = ::CreatePen(0, 0, color);
	HPEN oldPen = (HPEN)SelectObject(hdc, pen);//加载画笔

	Arc(hdc,m_ptCenter.x-DISTANCE,m_ptCenter.y-DISTANCE   //绘制敌军的头
	,m_ptCenter.x+DISTANCE,m_ptCenter.y+DISTANCE
	,m_ptCenter.x-DISTANCE,m_ptCenter.y
	,m_ptCenter.x-DISTANCE,m_ptCenter.y);

	int const LEGLENTH = (DISTANCE) / (LEGCOUNTS);
	//用帧数来绘制身体和腿
	if (m_nFrame%2==0)
	{
		//矩形的身体
		MoveToEx(hdc, m_ptCenter.x - DISTANCE, m_ptCenter.y, NULL);
		LineTo(hdc, m_ptCenter.x - DISTANCE, m_ptCenter.y + DISTANCE - LEGLENTH);
		MoveToEx(hdc, m_ptCenter.x + DISTANCE, m_ptCenter.y,NULL);
		LineTo(hdc, m_ptCenter.x - DISTANCE, m_ptCenter.y + DISTANCE - LEGLENTH);

		for (int i = 0; i < LEGCOUNTS; i++)//从左向右绘制脚
		{
			Arc(hdc,
				m_ptCenter.x - DISTANCE + i * 2 * LEGLENTH,
				m_ptCenter.y + DISTANCE - 2 * LEGLENTH,
				m_ptCenter.x - DISTANCE + (i + 1) * 2 * LEGLENTH,
				m_ptCenter.y + DISTANCE,
				m_ptCenter.x - DISTANCE + i * 2 * LEGLENTH,
				m_ptCenter.y + DISTANCE - LEGLENTH,
				m_ptCenter.x - DISTANCE + (i + 1) * 2 * LEGLENTH,
				m_ptCenter.y + DISTANCE - LEGLENTH);
		}
	}
	else
	{
		MoveToEx(hdc, m_ptCenter.x - DISTANCE, m_ptCenter.y, NULL);
		LineTo(hdc, m_ptCenter.x - DISTANCE, m_ptCenter.y + DISTANCE);
		MoveToEx(hdc, m_ptCenter.x + DISTANCE, m_ptCenter.y, NULL);
		LineTo(hdc, m_ptCenter.x - DISTANCE, m_ptCenter.y + DISTANCE);
		
		MoveToEx(hdc,m_ptCenter.x-DISTANCE,m_ptCenter.y+DISTANCE,NULL );
		LineTo(hdc,m_ptCenter.x-DISTANCE+LEGLENTH,m_ptCenter.y+DISTANCE-LEGLENTH);

		for (int i = 0; i < LEGCOUNTS-1; i++)   //从左向右绘制脚,与前面的绘图方式不同实现一种动态效果
		{
			Arc(hdc,
				m_ptCenter.x - DISTANCE + (1+i * 2) * LEGLENTH,
				m_ptCenter.y + DISTANCE - 2 * LEGLENTH,
				m_ptCenter.x - DISTANCE + (3+i*2) * LEGLENTH,
				m_ptCenter.y + DISTANCE,
				m_ptCenter.x - DISTANCE + (i * 2 +1)* LEGLENTH,
				m_ptCenter.y + DISTANCE - LEGLENTH,
				m_ptCenter.x - DISTANCE + (i*2 + 3) * LEGLENTH,
				m_ptCenter.y + DISTANCE - LEGLENTH);
		}
		MoveToEx(hdc,m_ptCenter.x+DISTANCE,m_ptCenter.y+DISTANCE,NULL);
		LineTo(hdc,m_ptCenter.x+DISTANCE-LEGLENTH,m_ptCenter.y+DISTANCE-LEGLENTH);
	}
	//根据方向绘制眼睛
	int R = DISTANCE / 5;
	switch (m_dir)
	{
	case UP:
		Ellipse(hdc, m_ptCenter.x - 2 * R, m_ptCenter.y - 2 * R,
			m_ptCenter.x, m_ptCenter.y);         //画左眼
		Ellipse(hdc,m_ptCenter.x,m_ptCenter.y-2*R,
			m_ptCenter.x+2*R,m_ptCenter.y);      //画右眼
		break;
	case DOWN:
		Ellipse(hdc,m_ptCenter.x-2*R,m_ptCenter.y,
			m_ptCenter.x,m_ptCenter.y+2*R);      //画左眼
		Ellipse(hdc,m_ptCenter.x,m_ptCenter.y,
			m_ptCenter.x+2*R,m_ptCenter.y+2*R);  //画右眼
		break;
	case LEFT:
		Ellipse(hdc,m_ptCenter.x-3*R,m_ptCenter.y-R,
			m_ptCenter.x-R,m_ptCenter.y+R);      //画左眼
		Ellipse(hdc,m_ptCenter.x-R,m_ptCenter.y-R,
			m_ptCenter.x+R,m_ptCenter.y+R);      //画右眼
		break;
	case RIGHT:
		Ellipse(hdc,m_ptCenter.x-R,m_ptCenter.y-R,
			m_ptCenter.x+R,m_ptCenter.y+R);      //画左眼
		Ellipse(hdc,m_ptCenter.x+R,m_ptCenter.y-R,
			m_ptCenter.x+3*R,m_ptCenter.y+R);    //画右眼
		break;
	}
	m_nFrame++;                 //准备画下一帧
	SelectObject(hdc, oldPen);  //还原画笔
	DeleteObject(pen);          //删除画笔对象
	return;
}


void Enermy::action()
{
	bool b = Collision();//判断是否发生碰撞
	MakeDecision(b);     //设定方向
	Catch();             //开始抓捕玩家
}

//RedOne 成员
void RedOne::Draw(HDC &hdc)
{
	Enermy::Draw(hdc);//调用父类的绘图函数
}

void RedOne::MakeDecision(bool b)
{
	int i = rand();
	if(b)                  //撞到墙壁，改变方向
	{
		if (i % 4 == 0)    //逆时针旋转
		{
			m_dir == UP ? m_cmd = LEFT : m_cmd = UP;   //面向上，向左拐
		}
		else if (i%3==0)
		{
			m_dir == DOWN ? m_cmd = RIGHT : m_cmd = DOWN;//面向下，向右拐
		}
		else if (i % 2 == 0)
		{
			m_dir == RIGHT ? m_cmd = UP : m_cmd = RIGHT;//面向右，向上拐
		}
		else
		{
			m_dir == LEFT ? m_cmd = DOWN: m_cmd = LEFT;//面向左，向下拐
		}
		return;     //程序提前结束
	}
	//程序运行到这里，说明没有撞墙，继续处理
	if (i % 4 == 0)
	{
		m_dir != UP ? m_cmd = DOWN : m_cmd = UP;//非向上运动使之面向下，否则面向上
	}
	else if (i % 3 == 0)
	{
		m_dir != DOWN ? m_cmd = UP : m_cmd = DOWN;//非向下移动使之面向上，否则面向下
	}
	else if (i % 2 == 0)
	{
		m_dir != RIGHT ? m_cmd = LEFT : m_cmd = RIGHT;
	}
	else
	{
		m_dir != LEFT ? m_cmd = RIGHT : m_cmd = LEFT;
	}
}

//BlueOne 成员定义
void BlueOne::Draw(HDC &hdc)
{
	Enermy::Draw(hdc);
}

void BlueOne::MakeDecision(bool b)
{
	const int DR = this->m_nRow - player->GetRow();
	const int DA = this->m_nArray - player->GetArray();
	
	if (!b&&DR == 0)//没有碰撞发生，玩家在同一行上。
	{
		if (DA<=BLUE_ALTER&&DA>0)            //玩家在左侧警戒范围
		{
			m_cmd = LEFT;                    //向左移动
			return;
		}
		if (DA < 0 && DA >= -BLUE_ALTER)    //玩家在右侧警戒范围
		{
			m_cmd = RIGHT;                   //向右移动
			return;
		}
	}
	if (!b&&DA == 0)//没有发生碰撞，玩家在同一列上
	{
		if (DR <= BLUE_ALTER && DR > 0)
		{
			m_cmd = UP;
			return;
		}
		if (DR >= -BLUE_ALTER && DR < 0)
		{
			m_cmd = DOWN;
			return;
		}
	}
	RedOne::MakeDecision(b);       //不在追踪模式时与Red的行为相同
}


//YellowOne成员定义
void YellowOne::MakeDecision(bool b)
{
	const int DR = this->m_nRow - player->GetRow();
	const int DA = this->m_nArray - player->GetArray();
	if (!b)
	{
		if (DR*DR > DA*DA)
		{
			if (DA > 0)
			{
				m_cmd = LEFT;
				return;
			}
			else
			{
				m_cmd = RIGHT;
				return;
			}
		}
		else 
		{
			if (DR > 0)
			{
				m_cmd = UP;
				return;
			}
			else
			{
				m_cmd = DOWN;
				return;
			}
		}
	}
	RedOne::MakeDecision(b);    //不在追踪模式时与Red的行为相同
}

void YellowOne::Draw(HDC &hdc)
{
	Enermy::Draw(hdc);
}
















