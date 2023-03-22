#pragma once

#include <cmath>
#include <iostream>

struct V2 
{
	float x, y;

	V2(float _x, float _y) { x = _x; y = _y; }

	float norm() { return sqrt(x*x + y*y); }
	void  normalize() { float n = norm();     x /= n;    y /= n;    }
};

struct V2Rect
{
    V2 p1, p2, p3, p4;

    V2Rect(V2 _p1, V2 _p4): p1(_p1.x, _p1.y), p4(_p4.x,_p4.y), p2(_p4.x, _p1.y), p3(_p1.x, _p4.y) {}
};

struct Bumper
{
	V2 C;
	int r;
	float flash;
	Bumper(V2 _C, int _r, float _flash = 0) :C(_C.x, _C.y), r(_r) , flash(_flash) {}
};

struct Target
{
	V2 p1, p2;
	bool state;
	Target(V2 _p1, V2 _p2, bool _state = true): p1(_p1), p2(_p2), state(_state) {}

	V2 sideP1Right() { return V2(p1.x + 1, p1.y + 1); }
	V2 sideP2Right() { return V2(p2.x + 1, p2.y + 1); }

	V2 sideP1Left() { return V2(p1.x - 1, p1.y + 1); }
	V2 sideP2Left() { return V2(p2.x - 1, p2.y + 1); }
};

// comparaison sur des flottants... traitement sp�cial

bool operator == (const V2 & a, const V2 & b);

// red�finition des op�rateurs standards

V2 operator + (const V2 & a, const V2 & b);
V2 operator - (const V2 & a, const V2 & b);
V2 operator * (float      a, const V2 & b);
V2 operator * (const V2 & a, float      b);
V2 operator / (const V2 & a, float      b);

//-------------------------------------------------------------------------------------------------------------
//--------------------------------------------  PART 1 : Operators  -------------------------------------------
//-------------------------------------------------------------------------------------------------------------
float operator * (const V2 & a, const V2 & b);
float operator ^ (const V2& a, const V2& b);
std::ostream & operator << (std::ostream& os, V2& t);

//-------------------------------------------------------------------------------------------------------------
//--------------------------------------------  PART 2 : Collisions  ------------------------------------------
//-------------------------------------------------------------------------------------------------------------
float SameDirection(const V2& a, const V2& b);	
bool SameSide(const V2& a, const V2& b, const V2& m, const V2& n);
bool OppositeSide(const V2& a, const V2& b, const V2& m, const V2& n);
V2 Rot90(const V2& a);
float Dist(const V2& a, const V2& b, const V2& m);
bool PointInRectangle(const V2& a, const V2& b, const V2& m);
bool PointInCercle(const V2& a, const int r, const V2& b);
bool PointInPolConv(const V2 L[], int t, const V2& m);
bool InterSegmentSegment(const V2& a, const V2& b, const V2& m, const V2& n);
bool InterSegmentPoly(const V2& a, const V2& b, const V2 L[], int t);
bool InterSegmentCercle(const V2& a, const int r, const V2& m, const V2& n);
bool InterCercleCercle(const V2& a, const int r1, const V2& b, const int r2);
bool InterCerclePoly(const V2& a, const int r, const V2 L[], int t);
bool InterRectRect(const V2Rect& R1, const V2Rect& R2);
bool InterPolPol(const V2 L1[], int t1, const V2 L2[], int t2);


//-------------------------------------------------------------------------------------------------------------
//--------------------------------------------  PART 3 : Rebounds ---------------------------------------------
//-------------------------------------------------------------------------------------------------------------

V2 Rebound(const V2& v, V2& n);
V2 normalizator3000(const V2& v);
V2 calculator2000(const V2& c, const V2& d, const int r);
void AllongeSegment(V2& a, V2& b, const int r);