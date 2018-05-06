#include "stdafx.h"
#include "GObject.h"

using namespace std;

GMap *GObject::pStage = NULL;       

int GObject::GetRow()         //��ȡ��
{
	return m_nRow;
}


int GObject::GetArray()      //���������׵�ַ
{
	return m_nArray;
}

int GObject::PtTransform(int k)//����ת������
{
	return (k - (pStage->LD) / 2) / pStage->LD;
}

//�ж������Ƿ񵽴��߼�����λ��
bool GObject::Achive()
{
	int n = (m_ptCenter.x - pStage->LD / 2) % pStage->LD;//����x������߼�λ��
	int k = (m_ptCenter.y - pStage->LD / 2) % pStage->LD;//����y������߼�λ��
	bool l = (n == 0 && k == 0);                         //�������������Ϊ0.˵������ÿ���߼����������λ��
	return l;
}

//�����߼�����������
void GObject::AchiverCtrl()
{
	if (Achive())            //��������߼�����
	{
		m_nArray = PtTransform(m_ptCenter.x);//������
		m_nRow = PtTransform(m_ptCenter.y);  //������
	} 
}

void GObject::DrawBlank(HDC &hdc)
{
	//������Դ������������ָ�봦��
	HBRUSH hbr = ::CreateSolidBrush(RGB(255,255,255));//������ˢ�����ƾ��κ���Ҫ��ʹ��
	//shared_ptr<HBRUSH> phbr(&hbr);//����Դ��������ָ�봦�����Զ��ͷ�

	RECT rect;
	rect.top = m_ny - RD;
	rect.left = m_nx - RD;
	rect.right = m_nx + RD;
	rect.bottom = m_ny + RD;
	FillRect(hdc,&rect,hbr);//���ƾ���
	DeleteObject(hbr);
}

//��ײ���
bool GObject::Collision()
{
	bool b = false;

	//�����С��е��������Ǵ���,���ִ��PacMan��д��AchiveCtrl������������
	AchiverCtrl();
	//�ж�ָ�����Ч��
	if (m_nArray < 0 || m_nRow < 0 || m_nArray > MAPLENTH - 1
		|| m_nRow > MAPLENTH - 1) {
		b = true;
	}
	else if (Achive()) {
		//switch (m_cmd) {  //�ж��н��ķ���
		//case LEFT:
		//	//�ж���һ�������Ƿ��ܹ�ͨ��
		//	if (m_nArray > 0 &&
		//		!pStage->mapData[m_nRow][m_nArray - 1]) {
		//		b = true;//ָ����Ч
		//	}
		//	break;
		//	//���·�����ж�ԭ����ͬ
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
			m_dir = m_cmd; //ûײǽ,ָ��ɹ�
		//}
	}
	//������ʵ�ķ���λ��
	m_nx = m_ptCenter.x;
	m_ny = m_ptCenter.y;
	int MAX = pStage->LD * MAPLENTH + pStage->LD / 2;
	int MIN = pStage->LD / 2;
	switch (m_dir) {  //�ж��н��ķ���
	case LEFT:
		//�ж���һ�������Ƿ��ܹ�ͨ��
		if (m_nArray > 0 &&
			!pStage->mapData[m_nRow][m_nArray - 1]) {
			b = true;
			break;//"ײǽ��"
		}
		m_ptCenter.x -= m_nSpeed;
		if (m_ptCenter.x < MIN) {
			m_ptCenter.x = MAX;
		}

		break;
		//���·�����ж�ԭ����ͬ
	case RIGHT:
		if (m_nArray < MAPLENTH - 1 &&
			!pStage->mapData[m_nRow][m_nArray + 1]) {
			b = true;
			break;//"ײǽ��"
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
			break;//"ײǽ��"
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
			break;//"ײǽ��"
		}
		m_ptCenter.y += m_nSpeed;
		if (m_ptCenter.y > MAX) {
			m_ptCenter.y = MIN;
		}
		break;
	}
	return b;
}

//��������λ��
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




//PacMan�ĳ�Ա���壺

void PacMan::AchiverCtrl()
{
	GObject::AchiverCtrl();
	if (Achive())
	{
		if (m_nArray >= 0 && m_nArray < MAPLENTH    //��ֹ����Խ��
			&&m_nRow >= 0 && m_nRow < MAPLENTH)
		{
			if (pStage->peaMapData[m_nRow][m_nArray])//�жϸ�λ���Ƿ��ж��ӣ�����ж��Ӿ�ȡ�����ӵ���ʾ
			{
				pStage->peaMapData[m_nRow][m_nArray] = false;
			}
		}
	}
}


void PacMan::action() //������ƶ�
{
	Collision();      //������ײ��⣨�����ƶ��Ĺ��ܣ�
}

void PacMan::SetTwCommand(TWARDS command)
{
	m_cmd = command; //�����ƶ�����
}

bool PacMan::IsOver()
{
	return m_dir == OVER;   //�ж���Ϸ�Ƿ����
}

bool PacMan::IsWin()       //�ж�����Ƿ�ȡ��ʤ��
{
	for (int i = 0; i < MAPLENTH; i++)
	{
		for (int j = 0; j < MAPLENTH; j++)
		{
			if (pStage->peaMapData[i][j] == true)
			{
				return FALSE;           //��������κ�һ�����ӣ���ûȡ��ʤ��
			}
		}
	}
	return true;                        //û�ж������ȡ��ʤ��
}


POINT PacMan::GetPos()
{
	return m_ptCenter;                  //���ض��������λ��
}


void PacMan::SetOver()
{
	m_dir = OVER;                       //������Ϸ����
}

void PacMan::Draw(HDC &memDC)
{
	if (m_dir == OVER)                 //��Ϸ������ʲôҲ����
	{
	}
	else if (m_nFrame % 4 != 0)        //����֡�����͵ڶ�������������״
	{
		int x1 = 0, x2 = 0, y1 = 0, y2 = 0;
		int offsetx = DISTANCE / 2 + D_OFFSET;//���ҽ���x
		int offsety = DISTANCE / 2 + D_OFFSET;//���߽���y
		switch (m_dir)
		{
		case UP:              //�����ƶ�
			x1 = m_ptCenter.x - offsetx;
			x2 = m_ptCenter.x + offsetx;
			y1 = y2 = m_ptCenter.y - offsety;
			break;
		case DOWN:           //�����ƶ�
			x1 = m_ptCenter.x + offsetx;
			x2 = m_ptCenter.x - offsetx;
			y1 = y2 = m_ptCenter.y + offsety;
			break;
		case LEFT:          //�����ƶ�
			x2 = x1 = m_ptCenter.x - offsetx;
			y1 = m_ptCenter.y + offsety;
			y2 = m_ptCenter.y - offsetx;
			break;
		case RIGHT:         //�����ƶ�
			x1 = x2 = m_ptCenter.x + offsetx;
			y1 = m_ptCenter.y - offsety;
			y2 = m_ptCenter.y + offsety;
			break;
		}
		//�������߲���
		Arc(memDC, m_ptCenter.x - DISTANCE, m_ptCenter.y - DISTANCE
			, m_ptCenter.x + DISTANCE, m_ptCenter.y + DISTANCE
			, x1, y1, x2, y2);
		//��ֱ�߲��֣���ϳ���Ҷ���
		MoveToEx(memDC, x1, y1, NULL);
		LineTo(memDC,m_ptCenter.x,m_ptCenter.y);
		LineTo(memDC, x2, y2);
	}
	else if (m_nFrame%5!=0)         //����֡����������Բ��
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
	else                           //����ȫ�ſ���״̬
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
	m_nFrame++;      //׼��������һ֡
}


/*enermy��Ա����*/
shared_ptr<PacMan>Enermy::player = nullptr;

//ץס��ң���Ϸ����
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
	HPEN oldPen = (HPEN)SelectObject(hdc, pen);//���ػ���

	Arc(hdc,m_ptCenter.x-DISTANCE,m_ptCenter.y-DISTANCE   //���Ƶо���ͷ
	,m_ptCenter.x+DISTANCE,m_ptCenter.y+DISTANCE
	,m_ptCenter.x-DISTANCE,m_ptCenter.y
	,m_ptCenter.x-DISTANCE,m_ptCenter.y);

	int const LEGLENTH = (DISTANCE) / (LEGCOUNTS);
	//��֡���������������
	if (m_nFrame%2==0)
	{
		//���ε�����
		MoveToEx(hdc, m_ptCenter.x - DISTANCE, m_ptCenter.y, NULL);
		LineTo(hdc, m_ptCenter.x - DISTANCE, m_ptCenter.y + DISTANCE - LEGLENTH);
		MoveToEx(hdc, m_ptCenter.x + DISTANCE, m_ptCenter.y,NULL);
		LineTo(hdc, m_ptCenter.x - DISTANCE, m_ptCenter.y + DISTANCE - LEGLENTH);

		for (int i = 0; i < LEGCOUNTS; i++)//�������һ��ƽ�
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

		for (int i = 0; i < LEGCOUNTS-1; i++)   //�������һ��ƽ�,��ǰ��Ļ�ͼ��ʽ��ͬʵ��һ�ֶ�̬Ч��
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
	//���ݷ�������۾�
	int R = DISTANCE / 5;
	switch (m_dir)
	{
	case UP:
		Ellipse(hdc, m_ptCenter.x - 2 * R, m_ptCenter.y - 2 * R,
			m_ptCenter.x, m_ptCenter.y);         //������
		Ellipse(hdc,m_ptCenter.x,m_ptCenter.y-2*R,
			m_ptCenter.x+2*R,m_ptCenter.y);      //������
		break;
	case DOWN:
		Ellipse(hdc,m_ptCenter.x-2*R,m_ptCenter.y,
			m_ptCenter.x,m_ptCenter.y+2*R);      //������
		Ellipse(hdc,m_ptCenter.x,m_ptCenter.y,
			m_ptCenter.x+2*R,m_ptCenter.y+2*R);  //������
		break;
	case LEFT:
		Ellipse(hdc,m_ptCenter.x-3*R,m_ptCenter.y-R,
			m_ptCenter.x-R,m_ptCenter.y+R);      //������
		Ellipse(hdc,m_ptCenter.x-R,m_ptCenter.y-R,
			m_ptCenter.x+R,m_ptCenter.y+R);      //������
		break;
	case RIGHT:
		Ellipse(hdc,m_ptCenter.x-R,m_ptCenter.y-R,
			m_ptCenter.x+R,m_ptCenter.y+R);      //������
		Ellipse(hdc,m_ptCenter.x+R,m_ptCenter.y-R,
			m_ptCenter.x+3*R,m_ptCenter.y+R);    //������
		break;
	}
	m_nFrame++;                 //׼������һ֡
	SelectObject(hdc, oldPen);  //��ԭ����
	DeleteObject(pen);          //ɾ�����ʶ���
	return;
}


void Enermy::action()
{
	bool b = Collision();//�ж��Ƿ�����ײ
	MakeDecision(b);     //�趨����
	Catch();             //��ʼץ�����
}

//RedOne ��Ա
void RedOne::Draw(HDC &hdc)
{
	Enermy::Draw(hdc);//���ø���Ļ�ͼ����
}

void RedOne::MakeDecision(bool b)
{
	int i = rand();
	if(b)                  //ײ��ǽ�ڣ��ı䷽��
	{
		if (i % 4 == 0)    //��ʱ����ת
		{
			m_dir == UP ? m_cmd = LEFT : m_cmd = UP;   //�����ϣ������
		}
		else if (i%3==0)
		{
			m_dir == DOWN ? m_cmd = RIGHT : m_cmd = DOWN;//�����£����ҹ�
		}
		else if (i % 2 == 0)
		{
			m_dir == RIGHT ? m_cmd = UP : m_cmd = RIGHT;//�����ң����Ϲ�
		}
		else
		{
			m_dir == LEFT ? m_cmd = DOWN: m_cmd = LEFT;//���������¹�
		}
		return;     //������ǰ����
	}
	//�������е����˵��û��ײǽ����������
	if (i % 4 == 0)
	{
		m_dir != UP ? m_cmd = DOWN : m_cmd = UP;//�������˶�ʹ֮�����£�����������
	}
	else if (i % 3 == 0)
	{
		m_dir != DOWN ? m_cmd = UP : m_cmd = DOWN;//�������ƶ�ʹ֮�����ϣ�����������
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

//BlueOne ��Ա����
void BlueOne::Draw(HDC &hdc)
{
	Enermy::Draw(hdc);
}

void BlueOne::MakeDecision(bool b)
{
	const int DR = this->m_nRow - player->GetRow();
	const int DA = this->m_nArray - player->GetArray();
	
	if (!b&&DR == 0)//û����ײ�����������ͬһ���ϡ�
	{
		if (DA<=BLUE_ALTER&&DA>0)            //�������ྯ�䷶Χ
		{
			m_cmd = LEFT;                    //�����ƶ�
			return;
		}
		if (DA < 0 && DA >= -BLUE_ALTER)    //������Ҳྯ�䷶Χ
		{
			m_cmd = RIGHT;                   //�����ƶ�
			return;
		}
	}
	if (!b&&DA == 0)//û�з�����ײ�������ͬһ����
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
	RedOne::MakeDecision(b);       //����׷��ģʽʱ��Red����Ϊ��ͬ
}


//YellowOne��Ա����
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
	RedOne::MakeDecision(b);    //����׷��ģʽʱ��Red����Ϊ��ͬ
}

void YellowOne::Draw(HDC &hdc)
{
	Enermy::Draw(hdc);
}
















