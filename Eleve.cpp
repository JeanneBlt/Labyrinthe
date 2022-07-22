
#pragma warning( disable : 4996 ) 


#include <cstdlib>
#include <vector>
#include <iostream>
#include <string>
#include "G2D.h"
#ifdef  _WIN32
#include "glut.h" 
#endif


using namespace std;

struct _Heros
{
	string texture =
		"[RRR  ]"
		"[RRWR ]"
		"[RRR  ]"
		"[YY   ]"
		"[YYY  ]"
		"[YY YG]"
		"[GG   ]"
		"[CC   ]"
		"[CC   ]"
		"[CC   ]"
		"[CC   ]";

	string texturebis =
		"[RRR  ]"
		"[RRWR ]"
		"[RRR  ]"
		"[YY   ]"
		"[YYY  ]"
		"[YY YG]"
		"[GG   ]"
		"[CC   ]"
		"[C C  ]"
		"[C   C]"
		"[C   C]";

	V2 Size;
	int IdTex;
	V2 Pos = V2(45, 45);
	bool HasKey = false;
	int vies = 2;
	bool win = false;
};

struct _Momie
{
	string texture =
		"[RRR  ]"
		"[RRWR ]"
		"[RRR  ]"
		"[GG   ]"
		"[GGG  ]"
		"[GG GG]"
		"[GG   ]"
		"[CC   ]"
		"[CC   ]"
		"[C C  ]"
		"[C C  ]";
	V2 Size;
	int IdTex;
	V2 Pos;
	int dir = rand() % 4;
	_Momie::_Momie(V2 Pos);
};

_Momie::_Momie(V2 Pos)
{
	this->Pos = Pos;
}

struct _Key
{
	string texture =
		"[                      ]"
		"[     W W         WWWW ]"
		"[     W W        W    W]"
		"[   WWWWWWWWWWWWWW    W]"
		"[                W    W]"
		"[                 WWWW ]";


	V2 Size;
	int IdTex;
	V2 Pos = V2(440, 450);
};

struct _Coffre
{
	string texture =
		"[WWWWWWWWWWWWWWWWWWWWWW]"
		"[W                    W]"
		"[W         W          W]"
		"[W         W          W]"
		"[W                    W]"
		"[WWWWWWWWWWWWWWWWWWWWWW]";


	V2 Size;
	int IdTex;
	V2 Pos = V2(500, 535);
};

struct GameData
{

	string Map =
		"MMMMMMMMMMMMMMM"
		"M M           M"
		"M M M MMM MMM M"
		"M   M       M M"
		"MMM M M MMM M M"
		"M   M M     M M"
		"M MMM MMM MMMMM"
		"M   M  M      M"
		"M M M  M M MM M"
		"M M M  M M M  M"
		"M M M MM M MMMM"
		"M M M    M    M"
		"M M M MMMMMMM M"
		"M M      M    M"
		"MMMMMMMMMMMMMMM";

	// indique la présence d'un mur à la case (x,y)
	bool Mur(int x, int y) {
		if (0 <= x <= 14 && 0 <= y <= 14)
		{
			return Map[(15 - y - 1) * 15 + x] == 'M';
		}
		else
			return true;
	}

	int Lpix = 40;  // largeur en pixels des cases du labyrinthe

	_Heros Heros; // data du héros
	_Momie Momie1 = _Momie(V2(440, 450)); //data de la momie1
	_Momie Momie2 = _Momie(V2(500, 535)); //data de la momie1
	_Momie Momie3 = _Momie(V2(220, 300)); //data de la momie1
	_Key   Key;
	_Coffre Coffre; //data du coffre

	int NbFrame = 0;
	double Timer = 0.0;

	GameData() {}

};

GameData G;

bool detecteCollisions(V2 Pos1, V2 Pos2, V2 Size1, V2 Size2)
{
	return (((Pos1.x <= Pos2.x) && ((Pos1.x + Size1.x) >= Pos2.x)) ||
		(Pos1.x >= Pos2.x) && (Pos1.x <= (Pos2.x + Size2.x))) &&
		(((Pos1.y <= Pos2.y) && ((Pos1.y + Size1.y) >= Pos2.y)) ||
			(Pos1.y >= Pos2.y && (Pos1.y <= (Pos2.y + Size2.y))));
}

void ecranAccueil()
{
	G2D::ClearScreen(Color::Blue);
	G2D::DrawStringFontRoman(V2(0, 300), "Bienvenue dans le jeu du Labyrinthe", 25, 1, Color::Black);
	G2D::DrawStringFontRoman(V2(0, 180), "Vous devrez récupérer la clee et ouvrir le coffre", 12, 1, Color::Black);
	G2D::DrawStringFontRoman(V2(0, 160), "Evitez les momies qui veulent vous mangez!", 12, 1, Color::Black);
	G2D::DrawStringFontRoman(V2(0, 140), "Epuisez vos deux vies et c'est le Game Over...", 12, 1, Color::Black);
	G2D::DrawStringFontRoman(V2(0, 120), "Lorsque vous epuisez vos vies vous retournez au point de depart.", 12, 1, Color::Black);
	G2D::DrawStringFontRoman(V2(0, 100), "Deplacer vous avec les fleches directionnel", 12, 1, Color::Black);
	G2D::DrawStringFontRoman(V2(0, 80), "Appuyez sur 'Entree' pour commencer le jeu", 12, 1, Color::Black);
	G2D::Show();
}

void win()
{
	G2D::ClearScreen(Color::Black);
	G2D::DrawStringFontMono(V2(150, 300), "Vous avez gagne!", 95, 1, Color::Yellow);
	G2D::Show();
}

void gameOver()
{
	G2D::ClearScreen(Color::Black);
	G2D::DrawStringFontMono(V2(0, 300), "Game Over ;(", 95, 1, Color::Red);
	G2D::Show();
}

bool CollisionsMur(V2 Pos, V2 dir, V2 pSize)
{
	if (dir.x == 1) { return !G.Mur(((Pos.x + 1) + pSize.x) / 40, (Pos.y / 40)) && !G.Mur(((Pos.x + 1) + pSize.x) / 40, ((Pos.y + pSize.y) / 40)); }
	if (dir.x == -1) { return !G.Mur((Pos.x - 1) / 40, (Pos.y / 40)) && (!G.Mur((Pos.x - 1) / 40, ((Pos.y + pSize.y) / 40))); }
	if (dir.y == 1) { return !G.Mur((Pos.x / 40), ((Pos.y + 1) + pSize.y) / 40) && !G.Mur(((Pos.x + pSize.x) / 40), ((Pos.y + 1) + pSize.y) / 40); }
	if (dir.y == -1) { return !G.Mur((Pos.x / 40), (Pos.y - 1) / 40) && !G.Mur(((Pos.x + pSize.x) / 40), (Pos.y - 1) / 40); }
}

void GestionDeplacementHeros()
{
	if (G2D::IsKeyPressed(Key::LEFT) && CollisionsMur(G.Heros.Pos, V2(-1, 0), G.Heros.Size)) { G.Heros.Pos.x--; }
	if (G2D::IsKeyPressed(Key::RIGHT) && CollisionsMur(G.Heros.Pos, V2(1, 0), G.Heros.Size)) { G.Heros.Pos.x++; }
	if (G2D::IsKeyPressed(Key::UP) && CollisionsMur(G.Heros.Pos, V2(0, 1), G.Heros.Size)) { G.Heros.Pos.y++; }
	if (G2D::IsKeyPressed(Key::DOWN) && CollisionsMur(G.Heros.Pos, V2(0, -1), G.Heros.Size)) { G.Heros.Pos.y--; }

	if (detecteCollisions(G.Heros.Pos, G.Momie1.Pos, G.Heros.Size, G.Momie1.Size))
	{
		G.Heros.Pos = V2(45, 45);
		--G.Heros.vies;
		if (G.Heros.vies == 0)
		{
			glutDisplayFunc(gameOver);
		}
	}
	if (detecteCollisions(G.Heros.Pos, G.Momie2.Pos, G.Heros.Size, G.Momie2.Size))
	{
		G.Heros.Pos = V2(45, 45);
		G.Heros.Pos = V2(45, 45);
		--G.Heros.vies;
		if (G.Heros.vies == 0)
		{
			glutDisplayFunc(gameOver);
		}
	}
	if (detecteCollisions(G.Heros.Pos, G.Momie3.Pos, G.Heros.Size, G.Momie3.Size))
	{
		G.Heros.Pos = V2(45, 45);
		G.Heros.Pos = V2(45, 45);
		--G.Heros.vies;
		if (G.Heros.vies == 0)
		{
			glutDisplayFunc(gameOver);
		}
	}
	if (detecteCollisions(G.Heros.Pos, G.Key.Pos, G.Heros.Size, G.Key.Size))
	{
		G.Key.Pos = V2(1000, 1000);
		G.Heros.HasKey = true;
	}
	if (detecteCollisions(G.Heros.Pos, G.Coffre.Pos, G.Heros.Size, G.Coffre.Size) && G.Heros.HasKey)
	{
		G.Coffre.Pos = V2(1000, 1000);
		G.Timer = 0;
		G.Heros.win = true;
		glutDisplayFunc(win);
	}

	G.NbFrame++;

	if (G.NbFrame == 50)
	{
		G.Heros.IdTex = G2D::InitTextureFromString(G.Heros.Size, G.Heros.texturebis);
		G.Heros.Size = G.Heros.Size * 2; // on peut zoomer la taille du sprite;
		G2D::DrawRectWithTexture(G.Heros.IdTex, G.Heros.Pos, G.Heros.Size);
	}

	if (G.NbFrame == 100)
	{
		G.Heros.IdTex = G2D::InitTextureFromString(G.Heros.Size, G.Heros.texture);
		G.Heros.Size = G.Heros.Size * 2; // on peut zoomer la taille du sprite;
		G2D::DrawRectWithTexture(G.Heros.IdTex, G.Heros.Pos, G.Heros.Size);
		G.NbFrame = 0;
	}
}

V2 intToV2(int i) {
	if (i == 0) return V2(-1, 0);
	if (i == 1) return V2(1, 0);
	if (i == 2) return V2(0, 1);
	if (i == 3) return V2(0, -1);
}
void GestionDeplacementMomies()
{
	V2 dir1 = intToV2(G.Momie1.dir);
	V2 dir2 = intToV2(G.Momie2.dir);
	V2 dir3 = intToV2(G.Momie3.dir);
	if (CollisionsMur(G.Momie1.Pos, intToV2(G.Momie1.dir), G.Momie1.Size))
	{
		G.Momie1.Pos = G.Momie1.Pos + dir1;
	}
	else G.Momie1.dir = rand() % 4;
	if (CollisionsMur(G.Momie2.Pos, intToV2(G.Momie2.dir), G.Momie2.Size))
	{
		G.Momie2.Pos = G.Momie2.Pos + dir2;
	}
	else G.Momie2.dir = rand() % 4;
	if (CollisionsMur(G.Momie3.Pos, intToV2(G.Momie3.dir), G.Momie3.Size))
	{
		G.Momie3.Pos = G.Momie3.Pos + dir3;
	}
	else G.Momie3.dir = rand() % 4;
}

void render()
{
	G2D::ClearScreen(Color::Black);

	for (int x = 0; x < 15; x++)
		for (int y = 0; y < 15; y++)
		{
			int xx = x * G.Lpix;
			int yy = y * G.Lpix;
			if (G.Mur(x, y))
				G2D::DrawRectangle(V2(xx, yy), V2(G.Lpix, G.Lpix), Color::Blue, true);
		}

	// affichage du héro avec boite englobante et zoom x 2
	G2D::DrawRectangle(G.Heros.Pos, G.Heros.Size, Color::Red);
	G2D::DrawRectWithTexture(G.Heros.IdTex, G.Heros.Pos, G.Heros.Size);

	//affichage momies
	G2D::DrawRectWithTexture(G.Momie1.IdTex, G.Momie1.Pos, G.Momie1.Size);
	G2D::DrawRectWithTexture(G.Momie2.IdTex, G.Momie2.Pos, G.Momie2.Size);
	G2D::DrawRectWithTexture(G.Momie3.IdTex, G.Momie3.Pos, G.Momie3.Size);

	// affichage de la clef
	G2D::DrawRectWithTexture(G.Key.IdTex, G.Key.Pos, G.Key.Size);

	// affichage du coffre
	G2D::DrawRectWithTexture(G.Coffre.IdTex, G.Coffre.Pos, G.Coffre.Size);

	G2D::Show();
}

void GestionJeux()
{
	GestionDeplacementMomies();
	GestionDeplacementHeros();
	if (G2D::IsKeyPressed(Key::ENTER))
	{
		glutDisplayFunc(render);
	}
	G.Timer += G2D::ElapsedTimeFromLastCallbackSeconds();
	if (G.Timer >= 3 && G.Heros.win)
	{
		glutDisplayFunc(ecranAccueil);
	}
}

void AssetsInit()
{
	// Size passé en ref et texture en param
	G.Heros.IdTex = G2D::InitTextureFromString(G.Heros.Size, G.Heros.texture);
	G.Heros.Size = G.Heros.Size * 2; // on peut zoomer la taille du sprite

	G.Momie1.IdTex = G2D::InitTextureFromString(G.Momie1.Size, G.Momie1.texture);
	G.Momie1.Size = G.Momie1.Size * 2; // on peut zoomer la taille du sprite

	G.Momie2.IdTex = G2D::InitTextureFromString(G.Momie2.Size, G.Momie2.texture);
	G.Momie2.Size = G.Momie2.Size * 2; // on peut zoomer la taille du sprite

	G.Momie3.IdTex = G2D::InitTextureFromString(G.Momie3.Size, G.Momie3.texture);
	G.Momie3.Size = G.Momie3.Size * 2; // on peut zoomer la taille du sprite

	G.Key.IdTex = G2D::InitTextureFromString(G.Key.Size, G.Key.texture);
	G.Key.Size = G.Key.Size * 1.5; // on peut zoomer la taille du sprite

	G.Coffre.IdTex = G2D::InitTextureFromString(G.Coffre.Size, G.Coffre.texture);
	G.Coffre.Size = G.Coffre.Size * 1.5; // on peut zoomer la taille du sprite

}

int main(int argc, char* argv[])
{

	G2D::InitWindow(argc, argv, V2(G.Lpix * 15, G.Lpix * 15), V2(200, 200), string("Labyrinthe"));

	AssetsInit();

	G2D::Run(GestionJeux, ecranAccueil);

}


