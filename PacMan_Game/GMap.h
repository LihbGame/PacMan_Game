#pragma once
#include <list>
#define MAPLENTH 19       //�߼���ͼ��С
#define P_ROW 10          //�ҷ���λ������
#define P_ARRAY 9         //�ҷ���λ������
#define E_ROW 8           //�з���λ������
#define E_ARRAY 9         //�з���λ������

using std::list;

//������CMap
class GMap
{
protected:
	static int LD;       //�谭��ߴ�
	static int PD;       //���ӵİ뾶
	void InitOP();       //����˫�����ֵ�λ��û�ж��ӳ���
	bool mapData[MAPLENTH][MAPLENTH];   //�谭���߼���ͼ����
	bool peaMapData[MAPLENTH][MAPLENTH];//�����߼���ͼ����
	COLORREF color;      //��ͼ��ǽ����ɫ
public:
	void DrawMap(HDC &hdc);        //���Ƶ�ͼ
	void DrawPeas(HDC &hdc);       //���ƶ���
	GMap();
	virtual ~GMap();

	friend class GObject;          //����������ʹ��ֱ�ߵ������յ����Ϣ����ײ���
	friend class PacMan;
};

//��һ��
class Stage_1 :public GMap
{
private:
	bool static initData[MAPLENTH][MAPLENTH]; //��ͼ����
public:
	Stage_1();
};

//�ڶ���
class Stage_2 :public GMap
{
private:
	bool static initData[MAPLENTH][MAPLENTH]; //��ͼ����
public:
	Stage_2();
};

//������
class Stage_3 :public GMap
{
private:
	bool static initData[MAPLENTH][MAPLENTH]; //��ͼ����
public:
	Stage_3();
};
