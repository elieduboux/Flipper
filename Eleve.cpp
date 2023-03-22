#pragma warning( disable : 4996 ) 

#include <cstdlib>
#include <vector>
#include <iostream>
#include <string>
#include "G2D.h"

using namespace std;

// information de partie
struct GameData
{
	int idFrame   = 0;
	float HeighPix  = 800;
	float WidthPix  = 600;
	int Score = 0;
	int BallRadius = 15;
	float Impact = 0;
	int skippedFrames = 0;
	V2 BallPos    = V2(550, 30);
	V2 BallSpeed  = V2(200, 200);
	V2 BallDirecton = V2(-1, 1);

	Bumper BumperLeft   = Bumper(V2(200, 400), 40);
	Bumper BumperMiddle = Bumper(V2(300, 550), 40);
	Bumper BumperRight  = Bumper(V2(400, 400), 40);
	
	V2 LP[11] = { V2(600, 550), V2(585, 596), V2(542, 638), V2(476, 671), V2(392, 692), V2(300, 700), V2(207, 692),
		V2(123, 671), V2(57, 638), V2(14, 596), V2(0, 550) };

	Target target1 = Target(V2(450, 570), V2(480, 550));
	Target target2 = Target(V2(490, 545), V2(520, 525));
	Target target3 = Target(V2(530, 520), V2(560, 500));

	Target target4 = Target(V2(150, 570), V2(120, 550));
	Target target5 = Target(V2(110, 545), V2(80 , 525));
	Target target6 = Target(V2(70 , 520), V2(40 , 500));
	Target TP[6] = { target1, target2, target3, target4, target5, target6 };
};

GameData G;

void render()
{
	G2D::ClearScreen(Color::Black);
	 
	G2D::DrawStringFontMono(V2(80, G.HeighPix - 70), string("Super Flipper"), 50, 5, Color::Blue);

	G2D::DrawStringFontMono(V2(10, G.HeighPix - 100), string("Score : "), 20, 3, Color::White);
	G2D::DrawStringFontMono(V2(120, G.HeighPix - 100), to_string(G.Score), 20, 3, Color::White);

	G2D::DrawRectangle(V2(0, 0), V2(G.WidthPix,G.HeighPix) , Color::Green );

	G2D::DrawCircle(G.BallPos, G.BallRadius , Color::Red, true);

	G2D::DrawCircle(V2(200, 400), 40, Color::Blue, true);
	G2D::DrawCircle(V2(300, 550), 40, Color::Blue, true);
	G2D::DrawCircle(V2(400, 400), 40, Color::Blue, true);

	if (G.BumperLeft.flash <= 2 && G.BumperLeft.flash > 0)
	{
		G.BumperLeft.flash += G2D::ElapsedTimeFromLastCallbackSeconds();
		G2D::DrawCircle(G.BumperLeft.C, G.BumperLeft.r + 15 - G.BumperLeft.flash*5 , Color(1, 0, 0, 0.2), true);
	}
	if (G.BumperMiddle.flash <= 2 && G.BumperMiddle.flash > 0)
	{
		G.BumperMiddle.flash += G2D::ElapsedTimeFromLastCallbackSeconds();
		G2D::DrawCircle(G.BumperMiddle.C, G.BumperMiddle.r + 15 - G.BumperMiddle.flash * 5, Color(1, 0, 0, 0.2), true);
	}
	if (G.BumperRight.flash <= 2 && G.BumperRight.flash > 0)
	{
		G.BumperRight.flash += G2D::ElapsedTimeFromLastCallbackSeconds();
		G2D::DrawCircle(G.BumperRight.C, G.BumperRight.r + 15 - G.BumperRight.flash * 5, Color(1, 0, 0, 0.2), true);
	}


	for (int i = 0; i < 10; i++)
	   G2D::DrawLine(G.LP[i],G.LP[i+1], Color::Green);

	for (int i = 0; i < 3; i++)
	{		
		G2D::DrawLine(G.TP[i].p1		  , G.TP[i].p2			, (G.TP[i].state) ? Color::Red : Color::Green);
		G2D::DrawLine(G.TP[i].sideP1Left(), G.TP[i].sideP2Left(), (G.TP[i].state) ? Color::Red : Color::Green);
	}
	for (int i = 3; i < 6; i++)
	{
		G2D::DrawLine(G.TP[i].p1		   , G.TP[i].p2			  , (G.TP[i].state) ? Color::Red : Color::Green);
		G2D::DrawLine(G.TP[i].sideP1Right(), G.TP[i].sideP2Right(), (G.TP[i].state) ? Color::Red : Color::Green);
	}
	 
	G2D::Show();
}


void Logic()
{
	V2 p1(0,0), p2(0,0);
	G.idFrame += 1;
	float dt = (float)G2D::ElapsedTimeFromLastCallbackSeconds();

	if (G.BallSpeed.x > 200)
	{
		G.BallSpeed = G.BallSpeed * 0.997;
	}
	
	if (G.Impact <= 0.1 && G.Impact > 0)
	{
		G.Impact += G2D::ElapsedTimeFromLastCallbackSeconds();
		G.BallPos = G.BallPos + V2(G.BallSpeed.x * dt * G.BallDirecton.x, G.BallSpeed.y * dt * G.BallDirecton.y);
		return;
	}

	if (InterCercleCercle(G.BumperLeft.C, G.BumperLeft.r, G.BallPos, G.BallRadius))
	{
		G.Score += 100;
		G.BumperLeft.flash = dt;
		G.BallSpeed = G.BallSpeed * 1.2;
		V2 vectDest = G.BallPos - G.BumperLeft.C;
		G.Impact = dt;
		G.BallDirecton = Rebound(G.BallDirecton, vectDest);
	}
	else if (InterCercleCercle(G.BumperMiddle.C, G.BumperMiddle.r, G.BallPos, G.BallRadius))
	{
		G.Score += 100;
		G.BumperMiddle.flash = dt;
		G.BallSpeed = G.BallSpeed * 1.2;
		V2 vectDest = G.BallPos - G.BumperMiddle.C;
		G.Impact = dt;
		G.BallDirecton = Rebound(G.BallDirecton, vectDest);
	}
	else if (InterCercleCercle(G.BumperRight.C, G.BumperRight.r, G.BallPos, G.BallRadius))
	{
		G.Score += 100;
		G.BumperRight.flash = dt;
		G.BallSpeed = G.BallSpeed * 1.2;
		V2 vectDest = G.BallPos - G.BumperRight.C;
		G.Impact = dt;
		G.BallDirecton = Rebound(G.BallDirecton, vectDest);
	}

	//Targets Right
	for (int i = 0; i < 3; i++)
	{
		if (!G.TP[i].state)
			continue;
		p1 = G.TP[i].p1;
		p2 = G.TP[i].p2;
		AllongeSegment(p1, p2, G.BallRadius);
		if (InterSegmentSegment(G.BallPos, calculator2000(G.BallPos, G.BallDirecton, G.BallRadius), p1, p2))
		{
			G.Score += 500;
			V2 n = G.TP[i].p1 - G.TP[i].p2;
			G.TP[i].state = false;
			G.Impact = dt;
			G.BallDirecton = Rebound(G.BallDirecton, normalizator3000(n));
		}
	}

	//Targets Left
	for (int i = 3; i < 6; i++)
	{
		if (!G.TP[i].state)
			continue;

		p1 = G.TP[i].p1;
		p2 = G.TP[i].p2;
		AllongeSegment(p1, p2, G.BallRadius);
		if (InterSegmentSegment(G.BallPos, calculator2000(G.BallPos, G.BallDirecton, G.BallRadius), p1, p2))
		{
			G.Score += 500;
			V2 n = G.TP[i].p1 - G.TP[i].p2;
			G.TP[i].state = false;
			G.Impact = dt;
			G.BallDirecton = Rebound(G.BallDirecton, normalizator3000(n));
		}
	}

	//Top of the board
	for (int i = 0; i < 10; i++)
		if (InterSegmentSegment(G.BallPos, calculator2000(G.BallPos, G.BallDirecton, G.BallRadius), G.LP[i], G.LP[i + 1]))
		{
			G.Impact = dt;
			G.BallDirecton = Rebound(G.BallDirecton, normalizator3000(G.LP[i] - G.LP[i + 1]));
		}

	G.BallPos = G.BallPos + V2(G.BallSpeed.x * dt * G.BallDirecton.x, G.BallSpeed.y * dt * G.BallDirecton.y);

	//Sides of the board
	if (G.BallPos.y + G.BallRadius > 800) //Is now useless ... but just in case
	{
		G.Impact = dt;
		G.BallDirecton = Rebound(G.BallDirecton, normalizator3000(V2(600, 0)));
	}
	if (G.BallPos.x - G.BallRadius < 0)
	{
		G.Impact = dt;
		G.BallDirecton = Rebound(G.BallDirecton, normalizator3000(V2(0, 800)));
	}
	if (G.BallPos.x + G.BallRadius > 600)
	{
		G.Impact = dt;
		G.BallDirecton = Rebound(G.BallDirecton, normalizator3000(V2(0, 800)));
	}
	if (G.BallPos.y - G.BallRadius < 0)
	{
		G.Impact = dt;
		G.BallDirecton = Rebound(G.BallDirecton, normalizator3000(V2(600, 0)));
	}
}

int main(int argc, char* argv[])
{ 
	//-------------------------------------------------------------------------------------------------------------
	//--------------------------------------------  PART 1 : Operators  -------------------------------------------
	//-------------------------------------------------------------------------------------------------------------	
	//V2 A(5, 10);
	//V2 B = V2(6, 15);
	//V2 AB = B - A;
	//cout << AB << endl;

	//V2 u(5, 7);
	//V2 v(2, 3);
	//cout << "Produit scalaire  : " << (u * v) << endl;
	//cout << "Produit vectoriel : " << (u ^ v) << endl;

	//-------------------------------------------------------------------------------------------------------------
	//--------------------------------------------  PART 2 : Collision  -------------------------------------------
	//-------------------------------------------------------------------------------------------------------------

	//-------------------------------------------------------------------------------------------------------------
	//--------------------------------------------  PART 3 : Rebound  ---------------------------------------------
	//-------------------------------------------------------------------------------------------------------------	
	//V2 n1(-1, 1);
	//V2 n2(1, 0);
	//V2 v1(1, 0);
	//V2 v2(1,-1);
	//V2 v3(-1,1);
	//V2 v4(0,1);
	//cout << "Avec vecteur normal n1 = " << n1 << endl;
	//cout << "Cas 1 : v = " << v1 << " --> r = " << Rebond(v1, n1) << endl;
	//cout << "Cas 2 : v = " << v2 << " --> r = " << Rebond(v2, n1) << endl;
	//cout << "Cas 3 : v = " << v3 << " --> r = " << Rebond(v3, n1) << endl;
	//cout << "Cas 4 : v = " << v4 << " --> r = " << Rebond(v4, n1) << endl;

	//V2 n2(0, 1);
	//V2 v5(1,-1);
	//V2 v6(0,-1);
	//V2 v7(-1,1);
	//V2 v8(-1,-1);
	//cout << "Avec vecteur normal n2 = " << n2 << endl;
	//cout << "Cas 5 : v = " << v5 << " --> r = " << Rebond(v5, n2) << endl;
	//cout << "Cas 6 : v = " << v6 << " --> r = " << Rebond(v6, n2) << endl;
	//cout << "Cas 7 : v = " << v7 << " --> r = " << Rebond(v7, n2) << endl;
	//cout << "Cas 8 : v = " << v8 << " --> r = " << Rebond(v8, n2) << endl;

	G2D::InitWindow(argc, argv, V2(G.WidthPix, G.HeighPix), V2(200,200), string("Super Flipper 600 !!"));

	G2D::Run(Logic,render);
}
 
  
 