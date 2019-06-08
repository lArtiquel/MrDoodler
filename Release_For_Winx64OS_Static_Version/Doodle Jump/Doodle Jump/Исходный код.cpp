#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>
#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <string.h>
#include <time.h>

#pragma comment(linker, "/SUBSYSTEM:windows /ENTRY:mainCRTStartup")         //������� ������� � ������� ��������� �������������, ����� �� ������
//2 ������: � ��������� �������: Linker->System � � ��������� SubSystem �������� Windows (/SUBSYSTEM:WINDOWS),����� ���� ��������� ��� ���� main ����� Linker->Additional Dependencies
using namespace sf;

void doMenu(RenderWindow &app, Music &music);                                                          
void doGame(RenderWindow &app, bool &close,Music &music);
void doRecords(RenderWindow &app, bool &close);
void doPause(RenderWindow &app, bool &close, bool &toMenu,Music &music);
void doSettings(RenderWindow &app,bool &close,Music &music);
void doMusicOnOrOff(Music &music);
void doNewRecords();
void doGameOver(RenderWindow &app, double &score, bool &close, Music &music);
void doCheckRecords(double&score, bool &newRecord);
void doNewRecord(RenderWindow &app, double &score, bool &close);
void doWriteRecord(std::string &s, double score);
void doRewriteRecordsFromBuffertoRealFile();
void doMusicSelection(RenderWindow &app, bool &close, Music &music);

int forMusicSelection=1;
bool soundsOnOff = true;

class Entity                                                               //����� �����-���� ��������
{
	public:
	float x, y;
};
class point:public Entity                                                  //����� �������� � �� �����������
{     
	public:
	point() { drawObject = 1; moveRight = 1; typePlatform = 0; counter = 0; speed = 0; attribyte_type = 0; }
	bool drawObject,moveRight,counter;
	int typePlatform, speed, attribyte_type;
	float bufferPosition_y,selectPosition_y;
};
class Enemy : public Entity                                                //����� ������
{
	public:
	int isEnemyExists;
};

int main()
{          
	RenderWindow app(VideoMode(400, 533), "DOODLE JUMP");                  //������ ����              
	app.setFramerateLimit(60);                                              //���-�� �ps app.setVerticalSyncEnabled(true); -����. �����.(�������� ����)

	Music music;                                                                           //������� ������ ������
	music.openFromFile("music/1)smash it into pieces.ogg");

	srand(time(NULL));

	doMenu(app, music);                                                                    //����� ������� ����
	app.clear();
	app.close();
	return 0;
}


////////////////////////////////////////////////////////////�������////////////////////////////////////////////////////////////////////////
//////////////////////////////������� �������� ����////////////////////////////////////////////////////////////////////////////////////////
void doMenu(RenderWindow & app, Music &music)                   
{
	Texture t1, t2, t3, t4, t5;
	t1.loadFromFile("images/game.png");
	t2.loadFromFile("images/settings.png");
	t3.loadFromFile("images/records.png");
	t4.loadFromFile("images/quit.png");
	t5.loadFromFile("images/background_menu.png");

	Sprite game(t1), settings(t2), records(t3), quit(t4), background_menu(t5);
	game.setPosition(55, 107);
	settings.setPosition(55, 205);
	records.setPosition(55, 304);
	quit.setPosition(55, 401);
	background_menu.setPosition(0, 0);

	bool isMenu = 1,close = false;
	int menuNum, menuNumForKeyboard=0;
	
	while (isMenu)
	{   
		Event e;                                                                                             
		while (app.pollEvent(e))
			if (e.type == Event::Closed)
				isMenu = false;

		menuNum = 0;
		app.draw(background_menu);

		Vector2i pixelPos = Mouse::getPosition(app);                                                     //�������� ����� �������
		Vector2f pos = app.mapPixelToCoords(pixelPos);                                                   //��������� �� � ������� (������ �� ����� ����)

		if (game.getGlobalBounds().contains(pos.x,pos.y)) { game.setColor(Color::Black); menuNum = 1; }  //�������� � �����, ��������� ��d������� ������� � ������ �������
		else game.setColor(Color::Blue);
		if (settings.getGlobalBounds().contains(pos.x,pos.y)) { settings.setColor(Color::Black); menuNum = 2; }
		else settings.setColor(Color::Blue);
		if (records.getGlobalBounds().contains(pos.x,pos.y)) { records.setColor(Color::Black); menuNum = 3; }
		else records.setColor(Color::Blue);
		if (quit.getGlobalBounds().contains(pos.x,pos.y)) { quit.setColor(Color::Black); menuNum = 4; }
		else quit.setColor(Color::Blue);
		
		if (menuNum == 0)                                                    //���� ��������� ������ �� ����� ����
		{
			if (e.type == Event::KeyReleased && e.key.code == Keyboard::Down)//�������� �� ��������� ������� ������ �����/���� �� ����������
			{
				e.key.code = Keyboard::Q; if (menuNumForKeyboard < 4) menuNumForKeyboard += 1; else if (menuNumForKeyboard == 4) menuNumForKeyboard = 1;
			}
			else
				if (e.type == Event::KeyReleased && e.key.code == Keyboard::Up)
				{
					e.key.code = Keyboard::Q; if (menuNumForKeyboard != 0 && menuNumForKeyboard != 1) menuNumForKeyboard -= 1; else  menuNumForKeyboard = 4;
				}

			if (menuNumForKeyboard != 0)                                     //�������� � �����������
			{
				if (menuNumForKeyboard == 1) game.setColor(Color::Black);  
				else game.setColor(Color::Blue);
				if (menuNumForKeyboard == 2) settings.setColor(Color::Black);
				else settings.setColor(Color::Blue);
				if (menuNumForKeyboard == 3) records.setColor(Color::Black);
				else records.setColor(Color::Blue);
				if (menuNumForKeyboard == 4) quit.setColor(Color::Black);
				else quit.setColor(Color::Blue);
				
				if (e.type == Event::KeyReleased && e.key.code == Keyboard::Return)                                     //����� � ����������
					switch (menuNumForKeyboard)
					{
					case 1: doGame(app, close, music); menuNumForKeyboard = 0;e.mouseButton.button=Mouse::Right; break; //����� ����� ����� ����� �� �� ����� � ����� ����
					case 2: doSettings(app, close, music); e.mouseButton.button = Mouse::Right; break;
					case 3: doRecords(app, close); e.mouseButton.button = Mouse::Right; break;
					case 4: isMenu = false; break;
					}
			}
		}
		else
		{
			menuNumForKeyboard = 0;
			if ((e.type == Event::MouseButtonReleased) && (e.mouseButton.button == Mouse::Left))
			{
				switch (menuNum)                                              //���� ������ ��� ��������� ��� �� ���� ���������� ����� 
				{
				case 1: doGame(app, close, music); break;
				case 2: doSettings(app, close, music); break;
				case 3: doRecords(app, close); break;  
				case 4: isMenu = false; break;
				}
				e.mouseButton.button = Mouse::Right;                         //���� �� ����������� ��������� ������� ���� �� ����� ������ �-�� �������
			}
		}

		if (close == true) return;
		
		app.draw(game);
		app.draw(settings);
		app.draw(records);
		app.draw(quit);

		app.display();
	}
	return;
}

/////////////////////////////////////////GAME Function/////////////////////////////////////////////////////////////////////////////////////
void doGame(RenderWindow & app, bool &close, Music &music)
{	
	double score = 0;
	float x = 188, y = 100, dy = 0, h = 200,buffer_y,buffer_x;
	char q = 0;
	bool toMenu = false, drawDoodler = 1;
	int forRandomNumber, a = 1, b = 0, drawDoodlerWithObject=0, renderingUsedObject=0, platHasAttachedEnemy=0;

	Texture t1, t2, t3,t4,t5,t6,t7,t8,t9,t10,t11,t12,t13,t14,t15;                                                                    //��������� ���������
	t1.loadFromFile("images/background.png");
	t2.loadFromFile("images/platform.png");
	t3.loadFromFile("images/doodle.png");
	t4.loadFromFile("images/blue_platform.png");
	t5.loadFromFile("images/sin_platform.png");
	t6.loadFromFile("images/wood_platform_destroyable.png");
	t7.loadFromFile("images/spring.png");
	t8.loadFromFile("images/trampol1ne.png");
	t9.loadFromFile("images/hat.png");
	t10.loadFromFile("images/rocket.png");
	t11.loadFromFile("images/flyingRocket.png");
	t12.loadFromFile("images/superRocket.png");
	t13.loadFromFile("images/superRocket_wd.png");
	t14.loadFromFile("images/black_hole.png");
	t15.loadFromFile("images/ufo.png");

	Sprite sBackground(t1), sPlat(t2), sPers(t3), sPlatMoveHorizontal(t4), sPlatMoveVertical(t5), sPlatWood(t6),sSpring(t7),sTramp(t8),sHat(t9),sRocket(t10),sFlyingRocket(t11), sSuperRocket(t12), sSuperRocket_wd(t13),sHole(t14),sUfo(t15);
	sPers.setPosition(x, y);

	if (forMusicSelection!=0) music.play();												   //������������� ������
	music.setLoop(true);                                                                   //������ ����� ������

	point plat[10];
	Enemy enemy[3];
	
	Font font;                                                      //����� 
	font.loadFromFile("font/CyrilicOld.ttf");                        //������� ������ ������ ���� ������
	Text text("", font);                                              //������ ������ �����.�����,������
	text.setCharacterSize(12);                                         //������ ������ 
	text.setFillColor(Color::Black);                                    // ���������� ������� ������                                                     
	text.setStyle(Text::Bold);                                           //������ �����

	for (int i = 0; i < 10; i++)
	{
		b += 53;
		plat[i].x = rand() % 370;
		plat[i].y = rand() % (b - a + 1) + a; 
		if (i != 0 && plat[i].y <= 53 * i + 12) plat[i].y += 12;          //����� ��� �������� ������ �������� ��� �� ���������
		a += 53;
	}																			//app.setKeyRepeatEnabled(false);-��������� ������� �������
	a=0;b=0;                                                                    //���� ���e������ ��� ���������� �������������:"�" ����� ������������ ��� ��������� ����������������� ���������(�����/������ �� �����), ����� ��� �� ������������� ���� �� �����

	while (app.isOpen())                         //���� ���� �������-������
	{
		Event e; 
		while (app.pollEvent(e))                 // ��������� ��� ������� ����, ������� ���� ������� � ��������� �������� �����
		{
			if (e.type == Event::Closed)         //������� pollEvent() ���������� ������, ���� � ������� ���� �������, � ���������� ����, ���� � ������� ������� �� ���������.
			{
				close = true;                    //���� ��� ��������� ���������� ������� ����(������ ������, ���������, ������/�������� ����� � ��)
				return;
			}
		}

		if ((e.type == Event::KeyReleased) && (e.key.code == sf::Keyboard::Escape))        //����� ������� ����
		{
			doPause(app, close, toMenu, music);
			if (close == true) return;
			if (toMenu == true) return;
			if (forMusicSelection !=0) music.play(); else music.stop();
			e.key.code = Keyboard::Q;                                                 // ����� ��� ���� �������� ����� ��� ������� �� ���� ����� ES�
		}                                                                                                          

		dy +=0.3 ;                                                                        //������� ����� ������������ ��� �� � ����������������       
		y+=dy;      

		if (y > 530) { doGameOver(app, score, close, music); return; }                                             //death �����
		
		if ((Keyboard::isKeyPressed(Keyboard::Right))) {x+=4; q = 1;sPers.setTextureRect(IntRect(79, 0, -79, 79));}//�������� ������� �������

		if ((Keyboard::isKeyPressed(Keyboard::Left))) {x-=4; q = 2; sPers.setTextureRect(IntRect(0, 0, 79, 79));}

		if (y < h)                                                        //���� �������� ������� �����, ��������� ������������ �� ����� ����� ������
		{                                                   
			y = h;
			for (int l=0;l<3; l++)                                        //������ ������
				if (enemy[l].isEnemyExists!=0)
				{
					enemy[l].y-=dy;
					if (enemy[l].y>533) enemy[l].isEnemyExists=0;         //��������� ����� �� ���� �� ������� ���������
				}

			for (int i = 0; i < 10; i++)
			{
				/////////////////////////������� ������ ��������///////////////////////////////////////////////////
				plat[i].y -= dy;
				if (plat[i].typePlatform == 0 && plat[i].attribyte_type != 0)
				{
					plat[i].bufferPosition_y -= dy;
				}
				else
				if (plat[i].typePlatform == 3) 
				{
					if (plat[i].bufferPosition_y > 533) plat[i].drawObject = 0; else plat[i].drawObject = 1; 
					plat[i].selectPosition_y -= dy; plat[i].bufferPosition_y -= dy;
				}	
				else
				if (plat[i].typePlatform == 1 && plat[i].bufferPosition_y > 533) plat[i].drawObject = 0;
				else
				if (plat[i].typePlatform == 1 && plat[i].drawObject == 1) plat[i].bufferPosition_y -= dy;
				
				/////////////////////////�������� ����� ���������, ������ � ��//////////////////////////////////////
				if (plat[i].y > 533)
				{
					plat[i].counter = 0;
					plat[i].y = plat[i].y - 533;                                                 //����� ��������� �� "���������"
					if (platHasAttachedEnemy==0) {a=0; plat[i].x = rand() % 370;} else
					{
                     platHasAttachedEnemy--;                                                     //��������� ��������� �� ������ ��������� 
					 if (a==3) 
					 {	
						forRandomNumber=rand()%2+1;
						if (forRandomNumber==1) plat[i].x=rand()%((int)enemy[b].x-65); else
						plat[i].x=rand()%(253-(int)enemy[b].x)+81+enemy[b].x;
					 }
					 else
					 if (a==1) plat[i].x=rand()%(253-(int)enemy[b].x)+81+enemy[b].x; else
					 plat[i].x=rand()%((int)enemy[b].x-65);
					}

					if (score>300 && platHasAttachedEnemy==0)                                    //����������� ������
						for (int l=0; l<3; l++) 
						{
							if (enemy[l].isEnemyExists==0)
								{
									forRandomNumber=rand()% (100) + 1;
									if (forRandomNumber>0 && forRandomNumber<=3)                //3% for black hole
									{
										enemy[l].isEnemyExists=1;
										platHasAttachedEnemy=2;                                 //����� �� ���� �� 2 ����������� ���������� ������(����� ������� ������)
										b=l;                                                    //���������� � ������ �� ����� ����� ��������� ����������� ���������
										enemy[l].x=rand()% 320;
										enemy[l].y=plat[i].y-80-rand() % 20;
										if (enemy[l].x-68>=0) a+=2;                              //������������ �������������� ���������,����� �� ������������� ������ ���� � �����������
										if (enemy[l].x+148<=400) a+=1;
										if (a==3)                                                //��������, ��������� ����� ����� �� ��������� � �����������
										{	
											forRandomNumber=rand()%2+1;                         //1-��������� �����,2-������ ������������ ����
											if (forRandomNumber==1) plat[i].x=rand()%((int)enemy[l].x-65); else
												plat[i].x=rand()%(253-(int)enemy[l].x)+80+enemy[l].x;
										}
										else
										if (a==1) plat[i].x=rand()%(253-(int)enemy[l].x)+80+enemy[l].x; else
										plat[i].x=rand()%((int)enemy[l].x-65);
									} else
									if (forRandomNumber>3 && forRandomNumber<=6)                //UFO 3%
									{
										enemy[l].isEnemyExists=2;
										platHasAttachedEnemy=3;                                 //3 ��������� ���������
										b=l;                                                    
										enemy[l].x=rand()% 319;
										enemy[l].y=plat[i].y-140-rand() % 10;
										if (enemy[l].x-68>=0) a+=2;                             //������������ �������������� ���������,����� �� ������������� ufo � �����������
										if (enemy[l].x+149<=400) a+=1;
										if (a==3)                                                
										{	
											forRandomNumber=rand()%2+1;
											if (forRandomNumber==1) plat[i].x=rand()%((int)enemy[l].x-65); else
												plat[i].x=rand()%(252-(int)enemy[l].x)+81+enemy[l].x;
										}
										else
										if (a==1) plat[i].x=rand()%(252-(int)enemy[l].x)+81+enemy[l].x; else
										plat[i].x=rand()%((int)enemy[l].x-65);
									}
									break;
								}
						}	                    

					if (score >= 100 && score < 150) {plat[i].typePlatform = rand() % (2); } else  //�������� ��� ��������� 0-�������,1-����������, 2-�������������� ����, 3-������������ ����
					if (score >= 150 && score < 200) {plat[i].typePlatform = rand() % (3); } else
					if (score >= 200) {plat[i].typePlatform = rand() % (4); }
			
					switch (plat[i].typePlatform)
					{
					case 0: forRandomNumber = rand() % (100) + 1;
						if (forRandomNumber > 43) { plat[i].attribyte_type = 0; } else
						if (forRandomNumber <= 15) { plat[i].attribyte_type = 1; plat[i].selectPosition_y = plat[i].x + rand() % (43) + 3; plat[i].bufferPosition_y =plat[i].y-12; } else
						if (forRandomNumber > 15 && forRandomNumber <= 25) { plat[i].attribyte_type = 2; plat[i].selectPosition_y = plat[i].x + 12; plat[i].bufferPosition_y =plat[i].y-14;}else
						if (forRandomNumber > 25 && forRandomNumber <= 35) { plat[i].attribyte_type = 3; plat[i].selectPosition_y = plat[i].x + 10; plat[i].bufferPosition_y =plat[i].y-15; plat[i].drawObject = 1;} else
						if (forRandomNumber > 35 && forRandomNumber <= 40) { plat[i].attribyte_type = 4; plat[i].selectPosition_y = plat[i].x + 12; plat[i].bufferPosition_y =plat[i].y-46; plat[i].drawObject = 1;} else
						if (forRandomNumber > 40 && forRandomNumber <= 43) { plat[i].attribyte_type = 5; plat[i].selectPosition_y = plat[i].x; plat[i].bufferPosition_y =plat[i].y-97; plat[i].drawObject = 1; }
						break;
					case 1: plat[i].bufferPosition_y = plat[i].y; plat[i].drawObject = 1;  break;
					case 2: plat[i].moveRight = rand() % (2); plat[i].speed = rand() % (3) + 1; break;
					case 3: plat[i].bufferPosition_y = plat[i].y; plat[i].selectPosition_y = rand() % (101) + 50 + plat[i].y; plat[i].drawObject = 1; plat[i].speed = rand() % (3) + 1; break;
					}
					score++; 
				}	
			}
		}

		///////////////////////////////////////////��������� �� c�����������///////////////////////////////////////////////////////////////////////////
		if (q == 1)                                                         //�������������� � ���������� � ��������� ������� �������
		{
			for (int i = 0; i < 10; i++)
			{
				if (plat[i].typePlatform == 0 && plat[i].attribyte_type != 0)
				{
					switch (plat[i].attribyte_type)
					{
					case 1: if (x + 54 > plat[i].selectPosition_y && x<plat[i].selectPosition_y && y + 70>plat[i].bufferPosition_y && y + 70 < plat[i].y && dy > 0) dy = -18; break;
					case 2: if (x + 54 > plat[i].selectPosition_y && x - 22 < plat[i].selectPosition_y && y + 70 > plat[i].bufferPosition_y && y + 70 < plat[i].y && dy > 0) dy = -25; break;
					case 3: if (x + 54 > plat[i].selectPosition_y && x - 27 < plat[i].selectPosition_y && y + 70 > plat[i].bufferPosition_y && y + 70 < plat[i].y && dy > 0 && drawDoodlerWithObject == 0) {drawDoodlerWithObject = 1; plat[i].drawObject = 0; dy = -30;} break;
					case 4: if (x + 54 > plat[i].selectPosition_y && x - 17 < plat[i].selectPosition_y && y + 44 > plat[i].bufferPosition_y && y + 44 < plat[i].y && drawDoodlerWithObject ==0) { drawDoodlerWithObject = 2; plat[i].drawObject = 0; dy = -50; } break;
					case 5: if (x + 54 > plat[i].selectPosition_y && x - 47 < plat[i].selectPosition_y && y + 44 > plat[i].bufferPosition_y && y + 44 < plat[i].y && drawDoodlerWithObject == 0) { drawDoodlerWithObject = 3; plat[i].drawObject = 0; dy = -70; drawDoodler = 0;} break;
					}
				}
				if (plat[i].typePlatform != 3)                               //��� ���,�.�. ���� ����� �� ��������� �� ��������, �� �� ������ ����������� �� ���������
					{
						if (plat[i].counter != 1)
							if ((x + 50 > plat[i].x) && (x - 48 < plat[i].x)
								&& (y + 70 > plat[i].y) && (y + 56 < plat[i].y) && (dy > 0))
							{
								dy = -11;
								if (plat[i].typePlatform == 1) { plat[i].counter = 1; }
							}
					}
					else
						if ((x + 50 > plat[i].x) && (x - 48 < plat[i].x)
							&& (y + 70 > plat[i].bufferPosition_y) && (y + 56 < plat[i].bufferPosition_y) && (dy > 0))
							dy = -11;
			}

			for (int i=0;i<3;i++)                                           //�������� �� ������������ � ���������
			{
				switch(enemy[i].isEnemyExists)
				{
				case 0: break;
				case 1: if (x+40>enemy[i].x && x-52<enemy[i].x && y+55>enemy[i].y && y-40<enemy[i].y
					    && drawDoodlerWithObject==0 && dy>=-5) {doGameOver(app, score, close, music); return;} break;
				case 2: if (x+50>enemy[i].x && x-55<enemy[i].x && y+50>=enemy[i].y && y+38<=enemy[i].y && dy>0) {dy=-18; enemy[i].isEnemyExists=0;} else   //����� ����������� �� ������ ���
						if (x+26>enemy[i].x && x-36<enemy[i].x && y-37>enemy[i].y && y-85<enemy[i].y && dy>=-7) {doGameOver(app, score, close, music); return;} 
						break;
				}
			}
		}
		else
		{
			for (int i = 0; i < 10; i++)                                    //�������������� � ���������� left �������
			{
				if (plat[i].typePlatform == 0 && plat[i].attribyte_type != 0)
				{
					switch (plat[i].attribyte_type)
					{
					case 1: if (x + 7 < plat[i].selectPosition_y && x+60>plat[i].selectPosition_y && y + 70>plat[i].bufferPosition_y && y + 70 < plat[i].y && dy > 0) dy = -18; break;
					case 2: if (x - 15 < plat[i].selectPosition_y && x+60>plat[i].selectPosition_y && y + 70>plat[i].bufferPosition_y && y + 70 < plat[i].y && dy > 0) dy = -25; break;
					case 3: if (x - 20 < plat[i].selectPosition_y && x+60 > plat[i].selectPosition_y && y + 70 > plat[i].bufferPosition_y && y + 70 < plat[i].y && dy > 0 && drawDoodlerWithObject == 0) { drawDoodlerWithObject = 1; plat[i].drawObject = 0; dy = -30;} break;
					case 4: if (x - 6 < plat[i].selectPosition_y && x+65 > plat[i].selectPosition_y && y + 44 > plat[i].bufferPosition_y && y + 44 < plat[i].y && drawDoodlerWithObject == 0) { drawDoodlerWithObject = 2; plat[i].drawObject = 0; dy = -50;} break;
					case 5: if (x - 40 < plat[i].selectPosition_y && x+60 > plat[i].selectPosition_y && y + 44 > plat[i].bufferPosition_y && y + 44 < plat[i].y && drawDoodlerWithObject == 0) { drawDoodlerWithObject = 3; plat[i].drawObject = 0; dy = -70; drawDoodler = 0; } break;
					}
				}
				if (plat[i].typePlatform != 3)
				{
					if (plat[i].counter != 1)
						if ((x + 60 > plat[i].x) && (x - 39 < plat[i].x)
							&& (y + 70 > plat[i].y) && (y + 56 < plat[i].y) && (dy > 0))
						{
							dy = -11;
							if (plat[i].typePlatform == 1) { plat[i].counter = 1; }
						}
				}
				else
					if ((x + 60 > plat[i].x) && (x - 39 < plat[i].x)
						&& (y + 70 > plat[i].bufferPosition_y) && (y + 56 < plat[i].bufferPosition_y) && (dy > 0))
						dy = -11;
			}

			for (int i=0;i<3;i++)                                 //�������� �� ������������ � �������� ������ �����
			{
				switch(enemy[i].isEnemyExists)
				{
				case 0: break;
				case 1:	if (x-43<enemy[i].x && x+53>enemy[i].x && y+55>enemy[i].y && y-40<enemy[i].y
						&& drawDoodlerWithObject==0 && dy>=-5) {doGameOver(app, score, close, music); return;} break;
				case 2: if (x+58>enemy[i].x && x-48<enemy[i].x && y+50>=enemy[i].y && y+38<=enemy[i].y && dy>0){dy=-18; enemy[i].isEnemyExists=0;} else   //����� ����������� �� ������ ���
						if (x+34>enemy[i].x && x-30<enemy[i].x && y-37>enemy[i].y && y-85<enemy[i].y && dy>=-7) {doGameOver(app, score, close, music); return;} 
						break;
				}
			}
		}

		if (x > 372)   x=-28;                                    //������ ��������� ������ � ����� �����
		 else
		if (x < -50)   x = 350;                                   //������ ��������� ����� � ����� ������

//////////////////////////////////������������ �������///////////////////////////////////////////////////////////////////////////////////////////////////
		app.clear();
		app.draw(sBackground);

		std::ostringstream string_score;                                             // �������� ���������� ��� ����� + ������������ � ������ ������ 
		string_score << score;		                                                 //�������������� � ������ ������ � ������������ ���� � ������
		text.setString("" + string_score.str());
		text.setPosition(354, 513);                                                  //������ ������� ������
		app.draw(text);  

		for (int i=0;i<3;i++)                                                        //����� ������
			switch(enemy[i].isEnemyExists)
			{
				case 0: break;
				case 1: sHole.setPosition(enemy[i].x,enemy[i].y); app.draw(sHole); break;   //������ ����
				case 2: sUfo.setPosition(enemy[i].x,enemy[i].y); app.draw(sUfo); break;     //���
			}

		switch (renderingUsedObject)                                                 //������������ �������������� �������
		{
			case 0: break;
			case 1: if (drawDoodlerWithObject == 0) buffer_y += 8; else buffer_y = buffer_y + 8 - dy; if (buffer_y > 533) renderingUsedObject = 0; else { sHat.setPosition(buffer_x, buffer_y); app.draw(sHat); } break;
			case 2: if (drawDoodlerWithObject == 0) buffer_y += 8; else buffer_y = buffer_y + 8 - dy; if (buffer_y > 533) renderingUsedObject = 0; else { sRocket.setPosition(buffer_x, buffer_y); app.draw(sRocket); } break;
			case 3: if (drawDoodlerWithObject == 0) buffer_y += 8; else buffer_y = buffer_y + 8 - dy; if (buffer_y > 533) renderingUsedObject = 0; else { sSuperRocket.setPosition(buffer_x, buffer_y); app.draw(sSuperRocket); } break;
		}

		if (drawDoodler == 1)                                                       //����������� ���������
		{
			sPers.setPosition(x, y);
			app.draw(sPers);
		}
		
		switch (drawDoodlerWithObject)                                               //����� ��������  � ��������  
		{
			case 0: break;
			case 1: if (q == 1) { sHat.setPosition(x+11, y+2); app.draw(sHat);} else { sHat.setPosition(x+23, y+2); app.draw(sHat); }
					if (dy >= -5) 
					{
						drawDoodlerWithObject = 0; renderingUsedObject = 1; 
						if (q == 1) { buffer_y = y + 2; buffer_x = x + 10;} else { buffer_y = y + 1; buffer_x = x + 23; }
					} break;
			case 2: if (q == 1) { sFlyingRocket.setPosition(x, y + 14); app.draw(sFlyingRocket);} else { sFlyingRocket.setPosition(x + 69, y + 14); app.draw(sFlyingRocket); }
					if (dy >= -5) { drawDoodlerWithObject = 0; renderingUsedObject = 2; buffer_y = y + 22; buffer_x = x + 22; } break;
			case 3: sSuperRocket_wd.setPosition(x+8, y-21); app.draw(sSuperRocket_wd);
				if (dy >= -5) { drawDoodlerWithObject = 0; drawDoodler = 1; renderingUsedObject = 3; buffer_y = y - 21; buffer_x = x + 8; } break;
		}

		for (int i = 0; i < 10; i++)                                         //������ � ������� ��������� � ����������� �� �� ����
			switch (plat[i].typePlatform)                                   
			{
			case 0:                                                          //������� ������� 
				if (plat[i].attribyte_type == 1) { sSpring.setPosition(plat[i].selectPosition_y, plat[i].bufferPosition_y); app.draw(sSpring); }else
				if (plat[i].attribyte_type == 2) { sTramp.setPosition(plat[i].selectPosition_y, plat[i].bufferPosition_y); app.draw(sTramp); } else
				if (plat[i].attribyte_type == 3) {if (plat[i].drawObject == 1) { sHat.setPosition(plat[i].selectPosition_y, plat[i].bufferPosition_y); app.draw(sHat); }}else
				if (plat[i].attribyte_type == 4) {if (plat[i].drawObject == 1) { sRocket.setPosition(plat[i].selectPosition_y, plat[i].bufferPosition_y); app.draw(sRocket); }}else
				if (plat[i].attribyte_type == 5) {if (plat[i].drawObject == 1) { sSuperRocket.setPosition(plat[i].selectPosition_y, plat[i].bufferPosition_y); app.draw(sSuperRocket); }}
				sPlat.setPosition(plat[i].x, plat[i].y); app.draw(sPlat); break;
			case 1:                                                          //���������� ���������� 
				if (plat[i].drawObject == 1)
				{
					if (plat[i].counter == 1) plat[i].bufferPosition_y += 8;
						sPlatWood.setPosition(plat[i].x, plat[i].bufferPosition_y);
						app.draw(sPlatWood);
				}
				break;
			case 2:                                                          //����������� ������������� 
				switch (plat[i].moveRight) 
				{
				case 0: if (plat[i].x < plat[i].speed) plat[i].moveRight = 1; break;
				case 1: if (plat[i].x + plat[i].speed > 366) plat[i].moveRight = 0; break;
				}
				if (plat[i].moveRight == 1) plat[i].x += plat[i].speed; else plat[i].x -= plat[i].speed;
				sPlatMoveHorizontal.setPosition(plat[i].x, plat[i].y); 
				app.draw(sPlatMoveHorizontal);
				break;
			case 3:                                                          //����������� �����������
				switch (plat[i].moveRight)
				{
				case 1: if (plat[i].bufferPosition_y + plat[i].speed > plat[i].selectPosition_y) plat[i].moveRight = 0; break;
				case 0: if (plat[i].bufferPosition_y -plat[i].speed < plat[i].y) plat[i].moveRight = 1; break;
				}
				if (plat[i].moveRight==1) plat[i].bufferPosition_y +=plat[i].speed; else plat[i].bufferPosition_y -= plat[i].speed;
				if (plat[i].drawObject == 1)
				{
					sPlatMoveVertical.setPosition(plat[i].x, plat[i].bufferPosition_y);
					app.draw(sPlatMoveVertical);
				}
				break;
			}	
		app.display();
	}
}
////////////////////////////////////////////////////GAME OVER////////////////////////////////////////////////////////////////////////////
void doGameOver(RenderWindow &app, double &score, bool &close, Music &music)
{
	music.stop();
	bool flag = true,newRecord=false;

	Texture t1;
	t1.loadFromFile("images/game_over.png");
	Sprite background(t1);
	background.setPosition(0, 0);

	app.draw(background);
	
	Font font;                                                      //����� 
	font.loadFromFile("font/CyrilicOld.ttf");                        //������� ������ ������ ���� ������
	Text text("", font);                                              //������ ������ �����.�����,������
	text.setCharacterSize(30);                                         //������ ������ 
	text.setFillColor(Color::White);                                    // ���������� ������� ������                                                     
	text.setStyle(Text::Bold);                                           //������ �����

	std::ostringstream string_score;                                       //����
	string_score << score;
	text.setString(string_score.str());
	text.setPosition(120, 170);
	app.draw(text);
	
	app.display();

	doCheckRecords(score,newRecord);

	if (newRecord == true) { doNewRecord(app, score, close); return;}
	else
	{
		while (flag == true)
		{
			Event e;
			if (app.waitEvent(e))
			{
				if (e.type == Event::Closed)
				{
					close = true;
					return;
				}
				else
					if (e.type == Event::KeyReleased && e.key.code == Keyboard::Return) flag = false;
			}
		}
		return;
	}
}



void doCheckRecords(double &score, bool &newRecord)      //��������������� �������� �������
{
	double num;
	std::string s;
	std::ifstream f_in("files/num.txt");
	for (int i = 0; i < 20; i++)
	{
		if (i % 2 == 0)
		{
			f_in >> num;
			if (score > num) { newRecord = true; f_in.close(); return; }
		}
		else
			f_in >> s;
	}
	f_in.close();
	return;
}



void doNewRecord(RenderWindow &app, double &score, bool &close)                            //����, ���� ������������ ������ ���
{
	Texture t1,t2,t3;
	t1.loadFromFile("images/game_over.png");
	t2.loadFromFile("images/enter_your_name.png");
	t3.loadFromFile("images/name_space.png");

	Sprite background(t1),newRecord_enterName(t2),space(t3);
	background.setPosition(0, 0);
	newRecord_enterName.setPosition(91, 207);
	space.setPosition(82, 287);
                                                     
	Font font;                                                      //����� 
	font.loadFromFile("font/CyrilicOld.ttf");                        //������� ������ ������ ���� ������
	Text text("", font);                                              //������ ������ �����.�����,������
	text.setCharacterSize(23);                                         //������ ������ 
	text.setFillColor(Color::White);                                    // ���������� ������� ������                                                     
	text.setStyle(Text::Bold);                                           //������ �����

	std::string s = "";
	int maxLenghtStr = 0;
	bool isTrue=true;

	while (isTrue)
	{
		Event e;
		while (app.pollEvent(e))
			if (e.type == Event::Closed)
			{
				close = true;
				return;
			}

		Vector2i pixelPos=Mouse::getPosition(app);
		Vector2f pos=app.mapPixelToCoords(pixelPos);

		if (Keyboard::isKeyPressed(Keyboard::Return)) isTrue=false; else
		if (e.type==Event::MouseButtonReleased && e.mouseButton.button==Mouse::Left && space.getGlobalBounds().contains(pos.x,pos.y)) isTrue=false;

		if ((maxLenghtStr < 10) && (e.type == Event::TextEntered) && (((e.text.unicode > 47) && (e.text.unicode < 58)) || ((e.text.unicode > 64) && (e.text.unicode < 91)) || ((e.text.unicode > 96) && (e.text.unicode < 123)) || (e.text.unicode == 95))) //������ ����� �����������,����� �� ������ ����������� ��������� �� ���� ������ 10�������� ��� �� ���� �����-������ ������ � ���
		{
			++maxLenghtStr;
			s += (char)e.text.unicode;
			e.text.unicode = 126;
		} else
		if ((e.type==Event::TextEntered) && (e.text.unicode == 8) && (maxLenghtStr > 0))     //backspase entered event
		{
			--maxLenghtStr;
			s.erase(s.size()-1);
			e.text.unicode = 126;             //����� ��� ���������� �������� ����� �� ����������� ��� �� ��������
		}

		app.draw(background);
		app.draw(newRecord_enterName);
		app.draw(space);

		std::ostringstream string_score;          //����
		string_score << score;
		text.setString(string_score.str());
		text.setPosition(120, 170);
		app.draw(text);

		std::ostringstream string_name;           //���
		string_name << s;
		text.setString(string_name.str());
		text.setPosition(85, 284);
		app.draw(text);
		app.display();
	}
	if (maxLenghtStr == 0) s = "Noname";
	doWriteRecord(s,score);
	return;
}



void doWriteRecord(std::string &s,double score)  //���������� ����� ������ � ��������� ���
{
	double num;
	std::string ss;
	bool stop = false;
	std::ifstream f_in("files/num.txt");
	std::ofstream f_out("files/num_rez.txt");

	for (int i = 0; i < 20; i++)
	{
		if (i % 2 == 0)                                    //���������,������������ ���� �������
		{
			f_in >> num;

			if ((score > num) && (stop != false))          //������ �������������� �������� ����� ������������ ������� �� ������
			{
				f_out << num << " ";
				f_in >> ss;
				f_out << ss;
				if (i != 18) f_out << std::endl;
				++i;
				continue;
			}

			if ((score > num) && (stop == false))         //���� ������� ������  ������ ������ �������
			{
				stop = true;
				f_out << score << " " << s;              //������ ����� � ����� ������ � ���
				if (i != 18)
				{
					f_out << std::endl;                  //������ ������� �� ����� ������,���� ������ ��� �� ���������
					i += 2;                              //�������������, ���� �� �������� ��� ���� ���
					f_out << num << " ";                 //�������� ������ �� 1 ������
				}
				else break;
			}
			else f_out << num << " ";
		}

		if (i % 2 != 0)                                   //�������������� �����
		{
			f_in >> ss;
			f_out << ss;
			if (i != 19) f_out << std::endl;
		}
	}
	f_in.close();
	f_out.close();
	doRewriteRecordsFromBuffertoRealFile();
	return;
}


void doRewriteRecordsFromBuffertoRealFile()                          //������������ �� ��������� ����� � ��������
{
	std::string ss;
	double num;
	std::ifstream f_in("files/num_rez.txt");
	std::ofstream f_out("files/num.txt");
	for (int i = 0; i < 20; i++)
	{
		if (i % 2 == 0)
		{
			f_in >> num;
			f_out << num << " ";
		}
		if (i % 2 != 0)
		{
			f_in >> ss;
			f_out << ss;
			if (i != 19) f_out << std::endl;
		}
	}
	f_in.close();
	f_out.close();
	return;
}

////////////////////////////////////����-�����/////////////////////////////////////////////////////////////////////////////////////////////
void doPause(RenderWindow &app, bool &close, bool &toMenu, Music &music)
{
	Texture t1, t2, t3, t4, t5;
	t1.loadFromFile("images/background_ESC.png");
	t2.loadFromFile("images/continue_pause.png");
	t3.loadFromFile("images/settings_pause.png");
	t4.loadFromFile("images/records_pause.png");
	t5.loadFromFile("images/return_pause.png");

	Sprite background_pause(t1), continue_pause(t2), settings_pause(t3), records_pause(t4), return_pause(t5);
	background_pause.setPosition(0, 0);
	continue_pause.setPosition(57, 177);
	settings_pause.setPosition(57, 266);
	records_pause.setPosition(57, 353);
	return_pause.setPosition(57, 440);

	music.pause();                                                                              //PAUSE music

	bool isMenuPause = true;
	int menuPauseNum,menuNumForKeyboard=0;

	while (isMenuPause)
	{
		Event e;
		while (app.pollEvent(e))
			if (e.type == Event::Closed)
			{
				close = true;
				return;
			}
		menuPauseNum = 0;
		app.draw(background_pause);

		Vector2i pixelPos = Mouse::getPosition(app);                           //����� ���������� ���� � ��������
		Vector2f pos = app.mapPixelToCoords(pixelPos);                         //�������������� � ����������

		if (continue_pause.getGlobalBounds().contains(pos.x,pos.y)) { continue_pause.setColor(Color::Black); menuPauseNum = 1; }
		else continue_pause.setColor(Color::Blue);
		if (settings_pause.getGlobalBounds().contains(pos.x,pos.y)) { settings_pause.setColor(Color::Black); menuPauseNum = 2; }
		else settings_pause.setColor(Color::Blue);
		if (records_pause.getGlobalBounds().contains(pos.x,pos.y)) { records_pause.setColor(Color::Black); menuPauseNum = 3; }
		else records_pause.setColor(Color::Blue);
		if (return_pause.getGlobalBounds().contains(pos.x,pos.y)) { return_pause.setColor(Color::Black); menuPauseNum = 4; }
		else return_pause.setColor(Color::Blue);

		if (menuPauseNum == 0)                                                    //���� ��������� ������ �� ����� ����
		{
			if (e.type == Event::KeyReleased && e.key.code == Keyboard::Down) //�������� �� ��������� ������� ������ �����/���� �� ����������
			{
				e.key.code = Keyboard::Q; if (menuNumForKeyboard < 4) menuNumForKeyboard += 1; else if (menuNumForKeyboard == 4) menuNumForKeyboard = 1;
			}
			else
				if (e.type == Event::KeyReleased && e.key.code == Keyboard::Up)
				{
					e.key.code = Keyboard::Q; if (menuNumForKeyboard != 0 && menuNumForKeyboard != 1) menuNumForKeyboard -= 1; else if (menuNumForKeyboard == 1) menuNumForKeyboard = 4;else if (menuNumForKeyboard == 0) menuNumForKeyboard = 4;
				}

			if (menuNumForKeyboard != 0)                                     //�������� � �����������
			{
				if (menuNumForKeyboard == 1) continue_pause.setColor(Color::Black);  
				else continue_pause.setColor(Color::Blue);
				if (menuNumForKeyboard == 2) settings_pause.setColor(Color::Black);
				else settings_pause.setColor(Color::Blue);
				if (menuNumForKeyboard == 3) records_pause.setColor(Color::Black);
				else records_pause.setColor(Color::Blue);
				if (menuNumForKeyboard == 4) return_pause.setColor(Color::Black);
				else return_pause.setColor(Color::Blue);
				
				if (e.type == Event::KeyReleased && e.key.code == Keyboard::Return)   //����� � ����������
				{
					switch (menuNumForKeyboard)
					{
						case 1: return; break;
						case 2: doSettings(app, close, music); if (close == true) return; break;
						case 3: doRecords(app, close); if (close == true) return; break;
						case 4: toMenu = true; music.stop(); return; break;
					}
					e.key.code = Keyboard::Q;
				}
			}
		}
		else
		{
			menuNumForKeyboard = 0;
			if ((e.type == Event::MouseButtonReleased) && (e.mouseButton.button == Mouse::Left))
			{
				switch (menuPauseNum)                                              //���� ������ ��� ��������� ��� �� ���� ���������� ����� 
				{
					case 1: return; break;
					case 2: doSettings(app, close, music); if (close == true) return; break;
					case 3: doRecords(app, close); if (close == true) return; break;
					case 4: toMenu = true; music.stop(); return; break;
				}
				e.mouseButton.button = Mouse::Right;                         //���� �� ����������� ��������� ������� ���� �� ����� ������ �-�� �������
			}
		}

		if ((e.type == Event::KeyReleased) && (e.key.code == Keyboard::Escape)) {e.key.code = Keyboard::Q; return;  }//�������-��������� � ����������

		app.draw(continue_pause);
		app.draw(settings_pause);
		app.draw(records_pause);
		app.draw(return_pause);

		app.display();
	}
}

//////////////////////////////////////////������� ����-���������///////////////////////////////////////////////////////////////////////////
void doSettings(RenderWindow &app, bool &close, Music &music)
{
	Texture t1, t2, t3, t4, t5, t6;
	t1.loadFromFile("images/background_settings.png");                                               //��������� ��������
	t2.loadFromFile("images/music.png");
	t3.loadFromFile("images/on.png");
	t4.loadFromFile("images/off.png");
	t5.loadFromFile("images/return_records.png");
	t6.loadFromFile("images/sounds.png");

	Sprite background_settings(t1), music_sprite(t2), on_music(t3), off_music(t4),on_sounds(t3),off_sounds(t4), return_records(t5), sounds(t6);      //��������� ������� ��������

	background_settings.setPosition(0, 0);
	music_sprite.setPosition(69, 80);
	return_records.setPosition(69, 247);
	sounds.setPosition(69, 163);
	on_music.setPosition(224, 80);
	off_music.setPosition(224, 80);
	on_sounds.setPosition(224, 163);
	off_sounds.setPosition(224, 163);

	bool isMenuSettings = 1;
	int menuNum;

	while (isMenuSettings)
	{
		Event e;
		while (app.pollEvent(e))                               //������������ ��� �������
		{
			if (e.type == Event::Closed)
			{
				isMenuSettings = false;
				close = true;
				return;
			}
			if (e.type == Event::KeyReleased && e.key.code == Keyboard::Escape) return;
		}

		menuNum = 0;
		app.clear();
		app.draw(background_settings);

		Vector2i pixelPos = Mouse::getPosition(app);
		Vector2f pos = app.mapPixelToCoords(pixelPos);
		if (music_sprite.getGlobalBounds().contains(pos.x, pos.y)) { music_sprite.setColor(Color::Black); menuNum = 1; }
		else music_sprite.setColor(Color::Blue);
		if (sounds.getGlobalBounds().contains(pos.x, pos.y)) { sounds.setColor(Color::Black); menuNum = 2; }
		else sounds.setColor(Color::Blue);

		if (forMusicSelection)
		{
			if (on_music.getGlobalBounds().contains(pos.x, pos.y))     //�������� �� ���������� ������� � ������ ������������ ������ � ��������� ���������������� ������
			{
				on_music.setColor(Color::Black); app.draw(on_music);
				menuNum = 3;
			}
			else { on_music.setColor(Color::Blue); app.draw(on_music); }
		}
		else 
			if (off_music.getGlobalBounds().contains(pos.x,pos.y)) { off_music.setColor(Color::Black); app.draw(off_music); menuNum = 3;}
			else { off_music.setColor(Color::Blue); app.draw(off_music);}

		if (soundsOnOff)                                              //�������� �� ���������� ������� � ������ ������������ ������ � ��� �� ��������� �������
			{
				if (on_sounds.getGlobalBounds().contains(pos.x, pos.y))
				{
					on_sounds.setColor(Color::Black); app.draw(on_sounds);
					menuNum = 4;
				}
				else { on_sounds.setColor(Color::Blue); app.draw(on_sounds); }
			}
			else
				if (off_sounds.getGlobalBounds().contains(pos.x, pos.y)) { off_sounds.setColor(Color::Black); app.draw(off_sounds); menuNum = 4;}
				else { off_sounds.setColor(Color::Blue); app.draw(off_sounds); }

		if (return_records.getGlobalBounds().contains(pos.x,pos.y)) { return_records.setColor(Color::Black); menuNum = 5; }//�������� ���������� ������� � ������ ������ ��������
		else return_records.setColor(Color::Blue);
		if (IntRect(0, 0, 58, 53).contains(pos.x,pos.y)) menuNum = 6;//�������� �� ���������� ������� � ������ ������� � ����

		if ((e.type == Event::MouseButtonReleased) && (e.mouseButton.button == Mouse::Left))
		{
			switch (menuNum)                                              //���� ������ ��� ��������� ��� �� ���� ���������� ����� 
			{
			case 0: break;
			case 1: doMusicSelection(app,close,music); break;

			case 3: doMusicOnOrOff(music); break;

			case 5: doNewRecords(); break;                                  
			case 6: return; break;
			}
			e.mouseButton.button = Mouse::Right;                         // �������,����� �� ��������� �������� ����� ����� ������� �� �������� � ������� �������� ����� �� ����������� ����������, ���� � ������� �� ��������� ����� �������, �������� �� ������ ����� ���� �� ������ �������
			e.key.code = Keyboard::Q;
			if (close==true) return;
		}

		app.draw(music_sprite);
		app.draw(sounds);
		app.draw(return_records);
		
		app.display();
	}
return;
}

void doNewRecords()                                    //������� �������� �������
{
	std::ofstream f_out("files/num.txt");
	for (int i = 0; i < 20; i++)
	{
		if (i % 2 == 0)
			f_out << "0 ";
		if (i % 2 != 0)
		{
			f_out << '-';
			if (i != 19) f_out << std::endl;
		}
	}
	f_out.close();
	return;
}

void doMusicOnOrOff(Music &music)                                       //������� ������ ������ �� ������
{
	if (forMusicSelection) forMusicSelection = 0; else forMusicSelection = rand() % (8)+1; //������ ������ ������ ��� ���������
	switch(forMusicSelection)
	{
		case 0: return; break;
		case 1: music.openFromFile("music/1)smash it into pieces.ogg"); return; break;
		case 2: music.openFromFile("music/2)The rasmus-Empire.ogg"); return; break;
		case 3: music.openFromFile("music/3)maduk-ghost assasin.ogg"); return; break;
		case 4: music.openFromFile("music/4)ID-Mouth of The River.ogg"); return; break;
		case 5: music.openFromFile("music/5)Tonight Alive-World Away.ogg"); return; break;
		case 6: music.openFromFile("music/6)Bon Jovi-It_s my life.ogg"); return; break;
		case 7: music.openFromFile("music/7)Poets-Carnival of Rust.ogg"); return; break;
		case 8: music.openFromFile("music/8)RAMMSTEIN-ENGEL.ogg"); return; break;
	}
}

void doMusicSelection(RenderWindow &app, bool &close, Music &music)    //������� ������ ������//
{
	Texture t1,t2,t3,t4,t5,t6,t7,t8,t9,t10;
	t1.loadFromFile("images/background_for_function_music_selection.png");
	t2.loadFromFile("images/1)smash it into pieces.png");
	t3.loadFromFile("images/2)The rasmus-Empire.png");
	t4.loadFromFile("images/3)maduk-ghost assasin.png");
	t5.loadFromFile("images/4)ID-Mouth of The River.png");
	t6.loadFromFile("images/5)Tonight Alive-World Away.png");
	t7.loadFromFile("images/6)Bon Jovi-It_s my life.png");
	t8.loadFromFile("images/7)Poets-Carnival of Rust.png");
	t9.loadFromFile("images/8)RAMMSTEIN-ENGEL.png");
	t10.loadFromFile("images/strelka.png");

	Sprite sBackground(t1), sSmash(t2), sTheRasmus(t3), sMaduk(t4), sID(t5), sTonight(t6), sBonJovi(t7), sPoets(t8), sRammstein(t9), sStrelka(t10);
	sBackground.setPosition(0,0);
	sStrelka.setPosition(0,0);
	sSmash.setPosition(55,78);
	sTheRasmus.setPosition(55,135);
	sMaduk.setPosition(55,191);
	sID.setPosition(55,247);
	sTonight.setPosition(55,303);
	sBonJovi.setPosition(55,359);
	sPoets.setPosition(55,415);
	sRammstein.setPosition(55,471);

	bool isMenu=true;
	int menuNum,menuNumForKeyboard;

	while (isMenu)
	{
		Event e;                                                                                             
		while (app.pollEvent(e))
		{
			if (e.type == Event::Closed)
				isMenu = false;
			else
			if (e.type == Event::KeyReleased && e.key.code==Keyboard::Escape) 
			isMenu=false;
		}

		menuNum = 0;
		app.clear();
		app.draw(sBackground);

		Vector2i pixelPos = Mouse::getPosition(app);                                                    
		Vector2f pos = app.mapPixelToCoords(pixelPos);                                                   

		if (sStrelka.getGlobalBounds().contains(pos.x,pos.y)) {menuNum=9; sStrelka.setColor(Color::Green); }
		else sStrelka.setColor(Color::Black);
		if (sSmash.getGlobalBounds().contains(pos.x,pos.y)) {if (forMusicSelection==1) sSmash.setColor(Color::Red); else sSmash.setColor(Color::Black); menuNum = 1; }
		else if (forMusicSelection==1) sSmash.setColor(Color::Magenta); else sSmash.setColor(Color::Blue);
		if (sTheRasmus.getGlobalBounds().contains(pos.x,pos.y)) {if (forMusicSelection==2) sTheRasmus.setColor(Color::Red); else sTheRasmus.setColor(Color::Black); menuNum = 2; }
		else if (forMusicSelection==2) sTheRasmus.setColor(Color::Magenta); else sTheRasmus.setColor(Color::Blue);
		if (sMaduk.getGlobalBounds().contains(pos.x,pos.y)) {if (forMusicSelection==3) sMaduk.setColor(Color::Red); else sMaduk.setColor(Color::Black); menuNum = 3; }
		else if (forMusicSelection==3) sMaduk.setColor(Color::Magenta); else sMaduk.setColor(Color::Blue);
		if (sID.getGlobalBounds().contains(pos.x,pos.y)) {if (forMusicSelection==4) sID.setColor(Color::Red); else sID.setColor(Color::Black); menuNum = 4; }
		else if (forMusicSelection==4) sID.setColor(Color::Magenta); else sID.setColor(Color::Blue);
		if (sTonight.getGlobalBounds().contains(pos.x,pos.y)) {if (forMusicSelection==5) sTonight.setColor(Color::Red); else sTonight.setColor(Color::Black); menuNum = 5; }
		else if (forMusicSelection==5) sTonight.setColor(Color::Magenta); else sTonight.setColor(Color::Blue);
		if (sBonJovi.getGlobalBounds().contains(pos.x,pos.y)) {if (forMusicSelection==6) sBonJovi.setColor(Color::Red); else sBonJovi.setColor(Color::Black); menuNum = 6; }
		else if (forMusicSelection==6) sBonJovi.setColor(Color::Magenta); else sBonJovi.setColor(Color::Blue);
		if (sPoets.getGlobalBounds().contains(pos.x,pos.y)) {if (forMusicSelection==7) sPoets.setColor(Color::Red); else sPoets.setColor(Color::Black); menuNum = 7; }
		else if (forMusicSelection==7) sPoets.setColor(Color::Magenta); else sPoets.setColor(Color::Blue);
		if (sRammstein.getGlobalBounds().contains(pos.x,pos.y)) {if (forMusicSelection==8) sRammstein.setColor(Color::Red); else sRammstein.setColor(Color::Black); menuNum = 8; }
		else if (forMusicSelection==8) sRammstein.setColor(Color::Magenta); else sRammstein.setColor(Color::Blue);

		if (menuNum == 0)                                                    //���� ��������� ������ �� ����� ����
		{
			if (e.type == Event::KeyReleased && e.key.code == Keyboard::Down)//�������� �� ��������� ������� ������ �����/���� �� ����������
			{
				e.key.code = Keyboard::Q; if (menuNumForKeyboard < 8) menuNumForKeyboard += 1; else if (menuNumForKeyboard == 8) menuNumForKeyboard = 1;
			}
			else
				if (e.type == Event::KeyReleased && e.key.code == Keyboard::Up)
				{
					e.key.code = Keyboard::Q; if (menuNumForKeyboard != 0 && menuNumForKeyboard != 1) menuNumForKeyboard -= 1; else menuNumForKeyboard = 8;
				}

			if (menuNumForKeyboard != 0)                                     //�������� � �����������
			{
				if (menuNumForKeyboard==1) {if (forMusicSelection==1) sSmash.setColor(Color::Red); else sSmash.setColor(Color::Black); }
				else if (forMusicSelection==1) sSmash.setColor(Color::Magenta); else sSmash.setColor(Color::Blue);
				if (menuNumForKeyboard==2) {if (forMusicSelection==2) sTheRasmus.setColor(Color::Red); else sTheRasmus.setColor(Color::Black); }
				else if (forMusicSelection==2) sTheRasmus.setColor(Color::Magenta); else sTheRasmus.setColor(Color::Blue);
				if (menuNumForKeyboard==3) {if (forMusicSelection==3) sMaduk.setColor(Color::Red); else sMaduk.setColor(Color::Black); }
				else if (forMusicSelection==3) sMaduk.setColor(Color::Magenta); else sMaduk.setColor(Color::Blue);
				if (menuNumForKeyboard==4) {if (forMusicSelection==4) sID.setColor(Color::Red); else sID.setColor(Color::Black); }
				else if (forMusicSelection==4) sID.setColor(Color::Magenta); else sID.setColor(Color::Blue);
				if (menuNumForKeyboard==5) {if (forMusicSelection==5) sTonight.setColor(Color::Red); else sTonight.setColor(Color::Black); }
				else if (forMusicSelection==5) sTonight.setColor(Color::Magenta); else sTonight.setColor(Color::Blue);
				if (menuNumForKeyboard==6) {if (forMusicSelection==6) sBonJovi.setColor(Color::Red); else sBonJovi.setColor(Color::Black);}
				else if (forMusicSelection==6) sBonJovi.setColor(Color::Magenta); else sBonJovi.setColor(Color::Blue);
				if (menuNumForKeyboard==7) {if (forMusicSelection==7) sPoets.setColor(Color::Red); else sPoets.setColor(Color::Black);}
				else if (forMusicSelection==7) sPoets.setColor(Color::Magenta); else sPoets.setColor(Color::Blue);
				if (menuNumForKeyboard==8) {if (forMusicSelection==8) sRammstein.setColor(Color::Red); else sRammstein.setColor(Color::Black);}
				else if (forMusicSelection==8) sRammstein.setColor(Color::Magenta); else sRammstein.setColor(Color::Blue);	
				
				if (e.type == Event::KeyReleased && e.key.code == Keyboard::Return)//����� � ����������
					switch (menuNumForKeyboard)
					{
					case 1: forMusicSelection=1; music.openFromFile("music/1)smash it into pieces.ogg"); return; break;
					case 2: forMusicSelection=2; music.openFromFile("music/2)The rasmus-Empire.ogg"); return; break;
					case 3: forMusicSelection=3; music.openFromFile("music/3)maduk-ghost assasin.ogg"); return; break;
					case 4: forMusicSelection=4; music.openFromFile("music/4)ID-Mouth of The River.ogg"); return; break;
					case 5: forMusicSelection=5; music.openFromFile("music/5)Tonight Alive-World Away.ogg"); return; break;
					case 6: forMusicSelection=6; music.openFromFile("music/6)Bon Jovi-It_s my life.ogg"); return; break;
					case 7: forMusicSelection=7; music.openFromFile("music/7)Poets-Carnival of Rust.ogg"); return; break;
					case 8: forMusicSelection=8; music.openFromFile("music/8)RAMMSTEIN-ENGEL.ogg"); return; break;
					}
			}
		}
		else
		{
			menuNumForKeyboard = 0;
			if (menuNum!=0)                                                       //��������� ��������� �� ����� �� �����-���� �� �����
			{
			if ((e.type == Event::MouseButtonReleased) && (e.mouseButton.button == Mouse::Left))
				switch (menuNum)                                             
				{
					case 1: forMusicSelection=1; music.openFromFile("music/1)smash it into pieces.ogg"); return; break;
					case 2: forMusicSelection=2; music.openFromFile("music/2)The rasmus-Empire.ogg"); return; break;
					case 3: forMusicSelection=3; music.openFromFile("music/3)maduk-ghost assasin.ogg"); return; break;
					case 4: forMusicSelection=4; music.openFromFile("music/4)ID-Mouth of The River.ogg"); return; break;
					case 5: forMusicSelection=5; music.openFromFile("music/5)Tonight Alive-World Away.ogg"); return; break;
					case 6: forMusicSelection=6; music.openFromFile("music/6)Bon Jovi-It_s my life.ogg"); return; break;
					case 7: forMusicSelection=7; music.openFromFile("music/7)Poets-Carnival of Rust.ogg"); return; break;
					case 8: forMusicSelection=8; music.openFromFile("music/8)RAMMSTEIN-ENGEL.ogg"); return; break;
					case 9: return;
				}
			}
		}
		app.draw(sSmash);
		app.draw(sTheRasmus);
		app.draw(sMaduk);
		app.draw(sID);
		app.draw(sTonight);
		app.draw(sBonJovi);
		app.draw(sPoets);
		app.draw(sRammstein);
		app.draw(sStrelka);

		app.display();
	}
return;
}

//////////////////////////////������� ������ ����-�������//////////////////////////////////////////////////////////////////////////////////
void doRecords(RenderWindow & app,bool &close)
{
	Texture t1;                                                                  
	t1.loadFromFile("images/background_records.png");
	Sprite sBackground_records(t1);
	sBackground_records.setPosition(0, 0);

	Font font;                                                      //����� 
	font.loadFromFile("font/CyrilicOld.ttf");                        //������� ������ ������ ���� ������
	Text text("", font);                                              //������ ������ �����.�����,������
	text.setCharacterSize(24);                                         //������ ������ 
	text.setFillColor(Color::Black);                                    // ���������� ������� ������                                                     
	text.setStyle(Text::Bold);                                           //������ �����

	bool flag = true;
	int num;
	std::string ss;

	app.draw(sBackground_records);
	std::ifstream f_in("files/num.txt");
	for (int i = 0; i < 20; i++)
	{
		std::ostringstream string_score;    
		if (i % 2 == 0)
		{
			f_in >> num;
			string_score << num;
			text.setString(string_score.str());
			switch (i)
			{
			case 0:	text.setPosition(209, 115); break;
			case 2: text.setPosition(209, 169); break;
			case 4: text.setPosition(209, 225); break;
			case 6: text.setPosition(209, 272); break;
			case 8: text.setPosition(209, 309); break;
			case 10: text.setPosition(209, 344); break;
			case 12: text.setPosition(209, 376); break;
			case 14: text.setPosition(209, 412); break;
			case 16: text.setPosition(209, 449); break;
			case 18: text.setPosition(209, 491); break;
			}
		}
		if (i % 2 != 0)
		{
			f_in >> ss;
			string_score << ss;
			text.setString(string_score.str());
			switch (i)
			{
			case 1:	text.setPosition(55, 115); break;
			case 3: text.setPosition(55, 169); break;
			case 5: text.setPosition(55, 225); break;
			case 7: text.setPosition(55, 272); break;
			case 9: text.setPosition(55, 309); break;
			case 11: text.setPosition(55, 344); break;
			case 13: text.setPosition(55, 376); break;
			case 15: text.setPosition(55, 412); break;
			case 17: text.setPosition(55, 449); break;
			case 19: text.setPosition(55, 491); break;
			}
		}
			app.draw(text);
	}
	f_in.close();
	app.display();
	while (flag == true)
	{
		Event e;                                                                                             
		if (app.waitEvent(e))
		{
			if (e.type == Event::Closed)
			{
				close = true;
				return;
			}
			else
				if (e.type == Event::MouseButtonReleased && e.mouseButton.button==Mouse::Left && IntRect(0, 0, 41, 37).contains(Mouse::getPosition(app))) flag = false; else
				if (e.type == Event::KeyReleased && e.key.code == Keyboard::Escape) flag = false;
		}
	}
	return;
}