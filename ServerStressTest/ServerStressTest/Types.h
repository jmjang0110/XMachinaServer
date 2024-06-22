
// ����ü�� ����� �����մϴ�. 


#pragma once

// �ػ� ( ȭ�� ũ�� )
typedef struct _tagResolution
{
	unsigned int Width;
	unsigned int Height;

}RESOLUTION, * PRESOLUTION;

// DeltaTime ���� �Ǽ��� �̿��� ������ �� �ְ��ϱ� ���� ����������ϴ�. 
typedef struct _tagPosition
{
	// �Ǽ��� ������� : �Ҽ����� �����Ǹ� ���߿� �ȼ� ���̰� ���Եȴ�.
	float x, y;

	_tagPosition() :
		x(0.f), y(0.f)
	{

	}

	_tagPosition(float _x, float _y) :
		x(_x), y(_y)
	{

	}

	// ���������
	_tagPosition(const _tagPosition& pos)
	{

		x = pos.x;
		y = pos.y;
	}
	_tagPosition(const POINT& pt)
	{
		x = static_cast<float>(pt.x);
		y = static_cast<float>(pt.y);
	}
	//������ ����� ������
	void operator=(const _tagPosition& pos)
	{
		x = pos.x;
		y = pos.y;
	}

	void operator = (const POINT& pt)
	{
		x = static_cast<float>(pt.x);
		y = static_cast<float>(pt.y);
	}

	void operator=(float f[2])
	{
		x = f[0];
		y = f[1];
	}
	// �ڿ� const �� �ٿ����� ��� ��ü���� ȣ���� �����ϴ�.

	// =========================    +     =====================
	_tagPosition operator+(const _tagPosition& pos)const
	{
		_tagPosition tpos;
		tpos.x = x + pos.x;
		tpos.y = y + pos.y;
		return tpos;
	}
	_tagPosition operator+(const POINT& pos)const
	{
		_tagPosition tpos;
		tpos.x = x + pos.x;
		tpos.y = y + pos.y;
		return tpos;
	}
	_tagPosition operator+(float f[2])const
	{
		_tagPosition tpos;
		tpos.x = x + f[0];
		tpos.y = y + f[1];
		return tpos;
	}
	_tagPosition operator+(float f)const
	{
		_tagPosition tpos;
		tpos.x = x + f;
		tpos.y = y + f;
		return tpos;
	}

	void operator += (const _tagPosition& pos)
	{
		x += pos.x;
		y += pos.y;
	}

	// =========================    -    =====================
	_tagPosition operator-(const _tagPosition& pos)const
	{
		_tagPosition tpos;
		tpos.x = x - pos.x;
		tpos.y = y - pos.y;
		return tpos;
	}
	_tagPosition operator-(const POINT& pos)const
	{
		_tagPosition tpos;
		tpos.x = x - pos.x;
		tpos.y = y - pos.y;
		return tpos;
	}
	_tagPosition operator-(float f[2])const
	{
		_tagPosition tpos;
		tpos.x = x - f[0];
		tpos.y = y - f[1];
		return tpos;
	}
	_tagPosition operator-(float f)const
	{
		_tagPosition tpos;
		tpos.x = x - f;
		tpos.y = y - f;
		return tpos;
	}


	// =========================    *    =====================
	_tagPosition operator*(const _tagPosition& pos)const
	{
		_tagPosition tpos;
		tpos.x = x * pos.x;
		tpos.y = y * pos.y;
		return tpos;
	}
	_tagPosition operator*(const POINT& pos)const
	{
		_tagPosition tpos;
		tpos.x = x * pos.x;
		tpos.y = y * pos.y;
		return tpos;
	}
	_tagPosition operator*(float f[2]) const
	{
		_tagPosition tpos;
		tpos.x = x * f[0];
		tpos.y = y * f[1];
		return tpos;
	}
	_tagPosition operator*(float f) const
	{
		_tagPosition tpos;
		tpos.x = x * f;
		tpos.y = y * f;
		return tpos;
	}


	// =========================    /    =====================
	_tagPosition operator/(const _tagPosition& pos)const
	{
		_tagPosition tpos;
		tpos.x = x / pos.x;
		tpos.y = y / pos.y;
		return tpos;
	}
	_tagPosition operator/(const POINT& pos)const
	{
		_tagPosition tpos;
		tpos.x = x / pos.x;
		tpos.y = y / pos.y;
		return tpos;
	}
	_tagPosition operator/(float f[2])const
	{
		_tagPosition tpos;
		tpos.x = x / f[0];
		tpos.y = y / f[1];
		return tpos;
	}
	_tagPosition operator/(float f)const
	{
		_tagPosition tpos;
		tpos.x = x / f;
		tpos.y = y / f;
		return tpos;
	}
}VECTOR2, * PVECTOR2, POSITION, * PPOSITION, _SIZE, * _PSIZE, VECTOR, * PVECTOR;