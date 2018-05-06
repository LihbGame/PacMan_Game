#pragma once
#include <time.h>
#include "GMap.h"


#define PLAYERSPEED 6      //玩家速度
#define ENERMYSPEED 4      //敌人速度
#define LEGCOUNTS   5      //敌人腿的数量
#define DISTANCE    10     //图形范围
#define BLUE_ALTER  8      //蓝色警戒范围
#define D_OFFSET    2      //绘图误差
#define RD (DISTANCE+D_OFFSET)     //绘图范围


enum   TWARDS       //指令枚举
{
	UP,             //上
	DOWN,           //下
	LEFT,           //左
	RIGHT,          //右
	OVER,           //游戏结束
};

class GObject                     //物体类：玩家和敌人的父类
{
public:
	GObject(int Row, int Array)
	{
		m_nFrame = 1;            //帧数
		pStage = NULL;           //当前关卡
		this->m_nRow = Row;      //行
		this->m_nArray = Array;  //数组
		//中心位置
		this->m_ptCenter.y = m_nRow * pStage->LD + pStage->LD / 2;
		this->m_ptCenter.x = m_nArray * pStage->LD + pStage->LD / 2;

		this->m_nx = m_ptCenter.x;
		this->m_ny = m_ptCenter.y;

	}
	~GObject();

	void SetPosition(int Row, int Array);   //设置位置
	void DrawBlank(HDC &hdc);               //画空白
	virtual void Draw(HDC &hdc) = 0;        //绘制对象
	virtual void action() = 0;              //数据变更的表现

	int GetRow();
	int GetArray();

	static GMap *pStage;       //指向地图类的指针，设置为静态，使所有的子类都能使用相同的地图
protected:
	int m_nx;
	int m_ny;
	TWARDS m_cmd;         //指令枚举
	POINT m_ptCenter;     //中心坐标
	int m_nRow;           //逻辑横坐标
	int m_nArray;         //逻辑纵坐标
	int m_nSpeed;         //速度
	TWARDS m_dir;         //方向
	int m_nFrame;         //帧数
	
	bool Achive();        //判断物体是否到达逻辑坐标位置
	
	bool Collision();      //逻辑碰撞检测，将物体摆放到合理的位置
	
	int PtTransform(int k);//将实际坐标转换成逻辑坐标

	virtual void AchiverCtrl(); //到达逻辑点后更新数据
};


class PacMan :public GObject        //玩家对象
{
protected:
	virtual void AchiverCtrl();      //重写虚函数

public:
	POINT GetPos();       
	bool IsOver();        //游戏是否结束
	bool IsWin();         //玩家是否赢得游戏
	void Draw(HDC &hdc);  //绘制自己
	void SetTwCommand(TWARDS command);    //设置玩家下一步指令
	PacMan(int x, int y) :GObject(x, y)
	{
		this->m_nSpeed = PLAYERSPEED;     //设置玩家速度
		m_cmd = m_dir = RIGHT;            //设置朝向 ·
	}
	void action();  //玩家动作函数
	void SetOver(); //设置游戏结束函数
};

class Enermy :public GObject     //敌军对象
{
protected:
	void Catch();        //是否抓住对象
	
	virtual void MakeDecision(bool b) = 0;//AI实现，确定方向
	COLORREF color;
public:
	static std::shared_ptr<PacMan> player;
	virtual void Draw(HDC& hdc);     //负责绘制自己
	Enermy(int x, int y) :GObject(x, y)
	{
		this->m_nSpeed = ENERMYSPEED;   //设置速度
		m_dir = LEFT;                   //设置朝向
		m_cmd = UP;                     //设置移动方向
	}
	virtual void action();              //负责行为
};



/*三种敌人*/
class RedOne :public Enermy
{
protected:
	virtual void MakeDecision(bool b);
public:
	void Draw(HDC& hdc);
	RedOne(int x, int y) :Enermy(x, y)
	{
		color = RGB(255, 0, 0);
	}

};

class BlueOne :public RedOne
{
protected:
	virtual void MakeDecision(bool b);
public:
	void Draw(HDC& hdc);
	BlueOne(int x, int y) :RedOne(x, y)
	{
		color = RGB(0, 0, 255);
	}
};

class YellowOne :public RedOne
{
protected:
	virtual void MakeDecision(bool b);
public:
	void Draw(HDC& hdc);
	YellowOne(int x, int y) :RedOne(x, y)
	{
		color = RGB(200, 200, 200);
	}
};








