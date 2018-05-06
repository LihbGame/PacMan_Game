#pragma once
#include <time.h>
#include "GMap.h"


#define PLAYERSPEED 6      //����ٶ�
#define ENERMYSPEED 4      //�����ٶ�
#define LEGCOUNTS   5      //�����ȵ�����
#define DISTANCE    10     //ͼ�η�Χ
#define BLUE_ALTER  8      //��ɫ���䷶Χ
#define D_OFFSET    2      //��ͼ���
#define RD (DISTANCE+D_OFFSET)     //��ͼ��Χ


enum   TWARDS       //ָ��ö��
{
	UP,             //��
	DOWN,           //��
	LEFT,           //��
	RIGHT,          //��
	OVER,           //��Ϸ����
};

class GObject                     //�����ࣺ��Һ͵��˵ĸ���
{
public:
	GObject(int Row, int Array)
	{
		m_nFrame = 1;            //֡��
		pStage = NULL;           //��ǰ�ؿ�
		this->m_nRow = Row;      //��
		this->m_nArray = Array;  //����
		//����λ��
		this->m_ptCenter.y = m_nRow * pStage->LD + pStage->LD / 2;
		this->m_ptCenter.x = m_nArray * pStage->LD + pStage->LD / 2;

		this->m_nx = m_ptCenter.x;
		this->m_ny = m_ptCenter.y;

	}
	~GObject();

	void SetPosition(int Row, int Array);   //����λ��
	void DrawBlank(HDC &hdc);               //���հ�
	virtual void Draw(HDC &hdc) = 0;        //���ƶ���
	virtual void action() = 0;              //���ݱ���ı���

	int GetRow();
	int GetArray();

	static GMap *pStage;       //ָ���ͼ���ָ�룬����Ϊ��̬��ʹ���е����඼��ʹ����ͬ�ĵ�ͼ
protected:
	int m_nx;
	int m_ny;
	TWARDS m_cmd;         //ָ��ö��
	POINT m_ptCenter;     //��������
	int m_nRow;           //�߼�������
	int m_nArray;         //�߼�������
	int m_nSpeed;         //�ٶ�
	TWARDS m_dir;         //����
	int m_nFrame;         //֡��
	
	bool Achive();        //�ж������Ƿ񵽴��߼�����λ��
	
	bool Collision();      //�߼���ײ��⣬������ڷŵ������λ��
	
	int PtTransform(int k);//��ʵ������ת�����߼�����

	virtual void AchiverCtrl(); //�����߼�����������
};


class PacMan :public GObject        //��Ҷ���
{
protected:
	virtual void AchiverCtrl();      //��д�麯��

public:
	POINT GetPos();       
	bool IsOver();        //��Ϸ�Ƿ����
	bool IsWin();         //����Ƿ�Ӯ����Ϸ
	void Draw(HDC &hdc);  //�����Լ�
	void SetTwCommand(TWARDS command);    //���������һ��ָ��
	PacMan(int x, int y) :GObject(x, y)
	{
		this->m_nSpeed = PLAYERSPEED;     //��������ٶ�
		m_cmd = m_dir = RIGHT;            //���ó��� ��
	}
	void action();  //��Ҷ�������
	void SetOver(); //������Ϸ��������
};

class Enermy :public GObject     //�о�����
{
protected:
	void Catch();        //�Ƿ�ץס����
	
	virtual void MakeDecision(bool b) = 0;//AIʵ�֣�ȷ������
	COLORREF color;
public:
	static std::shared_ptr<PacMan> player;
	virtual void Draw(HDC& hdc);     //��������Լ�
	Enermy(int x, int y) :GObject(x, y)
	{
		this->m_nSpeed = ENERMYSPEED;   //�����ٶ�
		m_dir = LEFT;                   //���ó���
		m_cmd = UP;                     //�����ƶ�����
	}
	virtual void action();              //������Ϊ
};



/*���ֵ���*/
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








