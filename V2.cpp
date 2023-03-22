#include "V2.h"

// comparaison sur des flottants... traitement sp�cial

bool operator == (const V2 & a, const V2 & b)
{
	V2 t = a - b;
	float epsilon = 0.001f;
	return t.norm() < epsilon;
}

// red�finition des op�rateurs standards

V2 operator + (const V2 & a, const V2 & b) { return V2(a.x + b.x, a.y + b.y); }
V2 operator - (const V2 & a, const V2 & b) { return V2(a.x - b.x, a.y - b.y); }
V2 operator * (float      a, const V2 & b) { return V2(a   * b.x, a   * b.y); }
V2 operator * (const V2 & a, float      b) { return V2(a.x * b, a.y * b); }
V2 operator / (const V2 & a, float      b) { return V2(a.x / b, a.y / b); }

//-------------------------------------------------------------------------------------------------------------
//--------------------------------------------  PART 1 : Operators  -------------------------------------------
//-------------------------------------------------------------------------------------------------------------
float operator * (const V2 & a, const V2 & b) { return a.x * b.x + a.y * b.y; }
float operator ^ (const V2& a, const V2& b) { return a.x * b.y - a.y * b.x; }
std::ostream& operator << (std::ostream& os, V2& t) { return os << "(" << t.x << "," << t.y << ")"; }

//-------------------------------------------------------------------------------------------------------------
//--------------------------------------------  PART 2 : Collision  -------------------------------------------
//-------------------------------------------------------------------------------------------------------------
float SameDirection(const V2& a, const V2& b) { return a*b; }

//bool SameSide(const V2& a, const V2& b, const V2& m, const V2& n) { return ((b-a) ^ (m-a)) * ((b-a) ^ (n-a)) >= 0; }
bool SameSide(const V2& a, const V2& b, const V2& m, const V2& n) 
{ 
	float r1 = (b - a) ^ (m - a);
	float r2 = (b - a) ^ (n - a);
	return (r1 >= 0 && r2 >= 0) || (r1<= 0 && r2 <= 0);
}
bool OppositeSide(const V2& a, const V2& b, const V2& m, const V2& n) { return !SameSide(a, b, m, n); }

V2 Rot90(const V2& a) { V2 u(-a.x, a.y);  return u; }

float Dist(const V2& a,const V2& b, const V2& m)
{
	V2 T = a - b;
	V2 n(T.y, -T.x);
	n.normalize();
	return fabs((a + m) * n);
}

float Dist(const V2& a, const V2& b) { return (float)sqrt(pow(b.x - a.x, 2) + pow(b.y - a.y, 2)); }

bool PointInRectangle(const V2& a, const V2& b, const V2& m)
{
	bool x_test = a.x <= m.x && m.x <= b.x;
	bool y_test = a.y <= m.y && m.y <= b.y;
	return x_test && y_test;
}

bool PointInCercle(const V2& a, const int r, const V2& b) { return Dist(a, b) <= r; }

bool PointInPolConv(const V2 L[], int t, const V2& m)
{
	for (int i = 0; i < t-1; i++)
	{
		V2 a = L[i];
		V2 b = L[(i + 1) % t];
		V2 n = L[(i + 2) % t];
		if (!SameSide(a, b, n, m))
			return false;
	}
	return true;
}

bool InterSegmentSegment(const V2& a, const V2& b, const V2& m, const V2& n)
{
	return OppositeSide(a, b, m, n) && OppositeSide(m, n, a, b);
}

bool InterSegmentPoly(const V2& a, const V2& b, const V2 L[], int t)
{
	for (int i = 0; i < t-1; i++)
		if (InterSegmentSegment(a, b, L[i], L[(i + 1) % t]))
			return true;

	return false;
}

bool InterSegmentCercle(const V2& a, const int r, const V2& m, const V2& n)
{
	if (Dist(a, m) <= r || Dist(a, n) <= r)
		return true;

	if (SameDirection(m - n, m - a) * SameDirection(n - m, n - a) > 0)
		if (Dist(m, n, a) <= r)
			return true;

	return false;
}

bool InterCercleCercle(const V2& a, const int r1, const V2& b, const int r2)
{
	return Dist(a, b) < r1 + r2;
}

bool InterCerclePoly(const V2& a, const int r, const V2 L[], int t)
{
	for(int i = 0; i < t; i++)
		if (InterSegmentCercle(a, r, L[i], L[(i+1)%t]))
			return true;

	return false;
}

bool InterRectRect(const V2Rect& R1, const V2Rect& R2){
	// const V2& a, const V2& b, const V2& v, const V2& m
    //	V2Rect R1 = V2Rect(a,b);
    //	V2Rect R2 = V2Rect(v,m);

	if ((R1.p4.y < R2.p1.y) || (R1.p1.y > R2.p4.y) || (R1.p1.x > R2.p4.x) || (R1.p4.x > R2.p1.x))
		return false;

	//if R1.ymax < R2.ymin : return false  // (1)
    //if R1.ymin > R2.ymax : return false  // (2)
    //if R1.xmin > R2.xmax : return false  // (3)
    //if R1.xmax < R2.xmin : return false  // (4)
	return true;  // non empty intersection
}

bool InterPolPol(const V2 L1[], int t1, const V2 L2[], int t2)
{
	for (int i = 0; i < t1; i++)
		if (InterSegmentPoly(L1[i], L1[(i + 1) % t1], L2, t2))
			return true;

	// Could be erased ... but not checked yet
	for (int i = 0; i < t2; i++)
		if (InterSegmentPoly(L2[i], L2[(i + 1) % t2], L1, t1))
			return true;
	return false;
}

//-------------------------------------------------------------------------------------------------------------
//--------------------------------------------  PART 3 : Rebound  ---------------------------------------------
//-------------------------------------------------------------------------------------------------------------

V2 Rebound(const V2& v, V2& n)
{
	n.normalize();
	V2 t(n.y, -n.x);
	float vt = v * t;
	float vn = v * n;
	return vt * t - vn * n;
}

V2 normalizator3000(const V2& v)
{
	V2 t(v.y, -v.x);
	return t;
}

V2 calculator2000(const V2& c, const V2& d, const int r)
{
	V2 t = c + d * r;
	return t;
}
//-------------------------------------------------------------------------------------------------------------
//--------------------------------------------  PART 3 : Bumper  ----------------------------------------------
//-------------------------------------------------------------------------------------------------------------

void AllongeSegment(V2& a, V2& b, const int r)
{
	V2 T = b - a;
	T.normalize();
	a = a - r * T;
	b = b + r * T;
}
