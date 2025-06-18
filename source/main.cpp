	
#include"compile_mode.h"
#include"theheader.h"
#include"game.h"

#ifdef FOR_PSP
#include <pspkerneltypes.h>	
#include <psppower.h>		
#endif

#include<string>
#include<unistd.h>

mxHwnd mxhwnd;
mxHwnd *mxHwnd::mxhwnd_static = &mxhwnd;
mxFont  font;
mxPaint paint;
SDL_Joystick *stick;
char stemp[280];
void load(int scr);
void unload();

int linenum = 10;

const char *getPath(const char *str)
{
    static char buffer[1024 * 4];
    snprintf(buffer, (1024 * 4),"./img/%s", str);
    return buffer;
}


class Intro {

	mxSprite logo;
	mxSprite start_scr;
	time_t tha_time, ntime;
	int wait;
	int clock_pos,curc_pos,twait;
public:

	void init()
	{ 
		logo.load(getPath("intro.bmp"));
		start_scr.load(getPath("start.bmp"));
		tha_time = time(0);
	}
	void update()
	{

			logo.display(&mxhwnd,0,0);
		for(int i = 0; i < 20000 ; i++)
		{
			int pos1=rand()%logo.sptr->w, pos2 = rand()%logo.sptr->h;
			mxhwnd.setPixel(logo.sptr, pos1, pos2 , mxhwnd.getPixel(start_scr.sptr,pos1,pos2));
		}

		logo.display(&mxhwnd, 0, 0);
		font.printText(10,10, "Press Enter to Continue.. \n");
		ntime = time(0);
		if(ntime > tha_time)
		{
			tha_time = ntime;
			wait++;
		}
		if(wait > 2)
		{
			wait = 0;
			mxhwnd.setScreen(START);
		}
	}

	void free()
	{
		logo.~mxSprite();
		start_scr.~mxSprite();
	}
};

class Options {

public:
	int lines,full_scr,cur_pos;

	Options()
	{
		lines = 10;
		full_scr = 1;
		cur_pos = 0;
	}
	void init()
	{
	}

	void update()
	{
		paint.drawRect(35,82,621-35,440-70, SDL_MapRGBA(mxhwnd.pscr->format, 0, 0, 0, 255));
		char chr[255];
		snprintf(chr,254, "Amount of Lines Until Speed increase: %d", lines);
		font.printText(70,90,chr);
		snprintf(chr,254, "Toggle FullScreen : %s", full_scr == 0 ? "On" : "Off");
		font.printText(70,120,chr);
		switch(cur_pos)
		{
		case 0:
			font.printText(40,90,"=)>");
			break;
		case 1:
			font.printText(40,120,"=)>");
			break;
		}

		font.printText(60,300,"Use the arrow keys to toggle the menus");
		font.printText(60,320,"Press Enter to Apply changes Esc to return");
	}

};

class Credits {

public:
	void init()
	{
	}
	void update()
	{
		paint.drawRect(35,82,621-35,440-70,SDL_MapRGBA(mxhwnd.pscr->format, 0, 0, 0, 255));
		font.printText(40,100,"Lost Master Piece ");
		font.printText(40,120,"This program is free. ");
		font.printText(40,140,"Click or Press Escape to Return");
	}
};

struct Scores {

	struct SDat {

		char user_name[255];
		int score;
	};
	SDat data[8];
};

class HighScores {
	
	int t_score,pos;
	bool in_score;
	char user_name[255];

public:

	Scores score;

	HighScores()
	{
		init();
		strcpy(user_name,"");
		pos = 0;
		in_score = false;
	}

	~HighScores()
	{
		save();
	}

	void init()
	{
		
	}

	void save()
	{
		
	}

	void update()
	{
		paint.drawRect(35,82,621-35,440-70,SDL_MapRGBA(mxhwnd.pscr->format, 0, 0, 0, 255));
		int start_y = 120;
		char dat[1024];
		font.printText(45,90, "High Scores");
		for(int p = 0; p < 8; p++)
		{
			snprintf(dat,1024,"%d) %s: %d", (p+1), score.data[p].user_name, score.data[p].score);
			font.printText(45,start_y,dat);
			start_y += 20;
		}

		if(in_score == true)
		{
			char dat[255];
			snprintf(dat,255, "%s%s", user_name, rand()%5 > 3 ? "_" : "");
			font.printText(280,120,"Input Name: ");
			font.printText(420,120,dat);
		}
		else
			font.printText(280,120,"Press Enter to continue...");
	}

	void input_score(int t_score)
	{
		in_score = true;
		this->t_score = t_score;
		strcpy(user_name,"");
	}

	void sort()
	{
		int max = 8;
		int pas,compare;

		for(pas = 0; pas < max - 1; pas++)
		{ 
			for(compare = 0; compare < max - 1; compare++)
			{
				if(score.data[compare].score < score.data[compare+1].score)
				{
					Scores::SDat temp;
					temp = score.data[compare];
					score.data[compare] = score.data[compare+1];
					score.data[compare+1] = temp;
				}
			}
		}
	}

	void keypress(int key)
	{
		if(key == 8)
		{
			if(pos - 1 <= 0)
			{
				user_name[pos] = 0;
				pos = 0;
			}
			else
			{
				user_name[--pos] = 0;
			}
		}
		else if (key == 13)
		{
			strcpy(score.data[7].user_name, user_name);
			score.data[7].score = t_score;
			sort();
			in_score = false;

		}
		else
		{
			user_name[pos++] = key;
			user_name[pos] = 0;
		}

	}
	
	bool is_input() const
	{
		return in_score;
	}
};

class Start {


public:

	int cursor_pos;
	Options options;
	Credits credits;
	HighScores scorez;

	void init()
	{
		start_scr.load(getPath("start.bmp"));
		start_scr2.load(getPath("start.bmp"));
		start_cursor.load(getPath("cursor.bmp"), 255,0,255);
		gamebg.load(getPath("gamebg.bmp"));
		cursor_pos = 0;
		options.init();
		credits.init();
	}
	void update()
	{

		start_scr.display(&mxhwnd,0,0);
		switch(mxhwnd.getScreen())
		{
		case START:
			switch(cursor_pos)
			{
			case 0:
				{
					cx = 250;
					cy = 170;
				}
				break;
			case 1:
				{
					cx = 250;
					cy = 170+70;
				}
				break;
			case 2: { cx = 250; cy = 170+70*2; } break;
			case 3: { cx = 250; cy = 170+70*3; } break;
			}
			break;
		case OPTIONS:
			options.update();
			return;
		case CREDITS:
			credits.update();
			return;
		case HIGHSCORES:
			scorez.update();
			return;
		}

		start_cursor.display(&mxhwnd,cx,cy);
	}

	void free()
	{
		start_scr.~mxSprite();
		start_scr2.~mxSprite();
		start_cursor.~mxSprite();
		gamebg.~mxSprite();
	}


	mxSprite start_scr;
	mxSprite gamebg,start_scr2;
private:
	mxSprite start_cursor;
	int cx,cy;

};

Start pstart;
Intro intro;
Game  game;

#include <cstdio>    

void mousemove(int x, int y)
{
    printf("mouse: %3d, %3d\n", x, y);

    if (mxhwnd.getScreen() != START) {
        return;
    }

    int w = mxhwnd.pscr->w;
    int h = mxhwnd.pscr->h;

    float sx = float(w) / 640.0f;
    float sy = float(h) / 480.0f;


    int x0 = int(360 * sx);
    int x1 = int(600 * sx);

    {
        int y0 = int(180 * sy);
        int y1 = int(240 * sy);
        if (y >= y0 && y <= y1) {
            pstart.cursor_pos = 0;
            return;
        }
    }

    {
        int y0 = int(260 * sy);
        int y1 = int(320 * sy);
        if (y >= y0 && y <= y1) {
            pstart.cursor_pos = 1;
            return;
        }
    }

    {
        int y0 = int(340 * sy);
        int y1 = int(400 * sy);
        if (y >= y0 && y <= y1) {
            pstart.cursor_pos = 2;
            return;
        }
    }

    {
        int y0 = int(420 * sy);
        int y1 = int(480 * sy);
        if (y >= y0 && y <= y1) {
            pstart.cursor_pos = 3;
            return;
        }
    }
}


void mousedown(int button, int x, int y)
{
	switch(mxhwnd.getScreen())
	{
	case INTRO:
		mxhwnd.setScreen(START);
		break;
	case START:
		switch(pstart.cursor_pos)
		{
		case 0:
			game.matrix.init_matrix();
			game.paused = false;
			mxhwnd.setScreen(GAME);
			break;
		case 1:
			mxhwnd.setScreen(OPTIONS);
			break;
		case 2:
			mxhwnd.setScreen(CREDITS);
			break;
		case 3:
			printf("Goodbye..\n");
			mxhwnd.kill();
			break;
		}
		break;
	case CREDITS:
		mxhwnd.setScreen(START);
		break;
	}


}

void keydown(int key)
{
	switch(mxhwnd.getScreen())
	{
	case INTRO:
		if(key == SDLK_RETURN)
			mxhwnd.setScreen(START);
		break;
	case START:
		{
			switch(key)
			{
			case SDLK_UP:
				if(pstart.cursor_pos > 0)
					pstart.cursor_pos--;
				break;
			case SDLK_DOWN:
				if(pstart.cursor_pos < 3)
					pstart.cursor_pos++;
				break;
			case SDLK_RETURN:
				switch(pstart.cursor_pos)
				{
				case 0:
					game.paused = false;
					game.matrix.init_matrix();
					mxhwnd.setScreen(GAME);
					break;
				case 1:
					mxhwnd.setScreen(OPTIONS);
					break;
				case 2:
					mxhwnd.setScreen(CREDITS);
					break;
				case 3:
					mxhwnd.kill();
					break;
				}
				break;
			}
		}
		break;
	case CREDITS:
		if(key == SDLK_ESCAPE)
			mxhwnd.setScreen(START);
		break;
	case OPTIONS:
		switch(key)
		{
		case SDLK_ESCAPE:
			mxhwnd.setScreen(START);
			break;
		case SDLK_UP:
			if(pstart.options.cur_pos > 0)
				pstart.options.cur_pos--;
			break;
		case SDLK_DOWN:
			if(pstart.options.cur_pos < 1)
				pstart.options.cur_pos++;
			break;
        case SDLK_RIGHT:
			switch(pstart.options.cur_pos)
			{
			case 0:
				if(pstart.options.lines < 15)
					pstart.options.lines++;
				break;
			case 1:
				if(pstart.options.full_scr == 0)
					pstart.options.full_scr = 1;
				else
					pstart.options.full_scr = 0;
				break;
			}
			break;
		case SDLK_LEFT:
			switch(pstart.options.cur_pos)
			{
			case 0:
				if(pstart.options.lines > 5)
					pstart.options.lines--;
				break;
			case 1:
				break;
			}
			break;
		case SDLK_RETURN:
			linenum = pstart.options.lines;
			unload();
			mxhwnd.setfullscreen(pstart.options.full_scr,"MpSDL", 640,480, 32);
			load(START);
			break;
		}
		break;
	case GAME:
		if(key == SDLK_ESCAPE)
			mxhwnd.setScreen(START);
		else if(key == 'p' || key == 'P')
			game.paused = !game.paused;
		game.keypress(key);
		break;
	case HIGHSCORES:
		if(pstart.scorez.is_input())
		{
			pstart.scorez.keypress(key);
		}
		else
		{
			if(key == 13)
			{
				mxhwnd.setScreen(START);
			}
		}
		break;
	}
}

void keyup(int key)
{
}

void onevent(SDL_Event *e)
{
	switch(e->type)
	{
	case SDL_KEYDOWN:
		break;
	}
}

void render(int screen)
{
	switch(screen)
	{
	case INTRO:
		intro.update();
		break;
	case START:
		pstart.update();
		break;
	case OPTIONS:
		pstart.update(); 
	case CREDITS:
		pstart.update(); 
		break;
	case HIGHSCORES:
		pstart.update(); 
		break;
	case GAME:
		{
		
			game.update();

		}
		break;
	}
	mxhwnd.flip();
}

void load(int scr)
{

	intro.init();
	pstart.init();
	game.load();
	font.load(&mxhwnd, getPath("arial.mxf"));
	paint.init(&mxhwnd);
	mxhwnd.setScreen(scr);
	mxhwnd.setEventHandler(onevent);
	mxhwnd.setMouseDown(mousedown);
	mxhwnd.setMouseMove(mousemove);
	mxhwnd.setKeydown(keydown);
	mxhwnd.setKeyup(keyup);
}

void unload()
{
	font.~mxFont(); 
	pstart.free(); 
	intro.free();
	game.free();
}

int main(int argc, char **argv)
{
#ifdef FOR_PSP
	scePowerSetClockFrequency(333, 333, 166); 
#endif
	
	
	
	
	if( SDL_Init(SDL_INIT_VIDEO | SDL_INIT_JOYSTICK) < 0 )
	{
		fprintf(stderr, "Could not initialize SDL: %s\n",SDL_GetError());
		return -1;
	}
	

	if(mxhwnd.init("LostMasterPiece", 1440,1080,32, false) != -1)
	{
		stick = SDL_JoystickOpen(0);
		srand((int)time(0));
		load(INTRO);
		
		mxhwnd.initLoop(render);
	}
	SDL_JoystickClose(stick);
	SDL_Quit();
	return (EXIT_SUCCESS);
}

void gameover()
{
	mxhwnd.setScreen(HIGHSCORES);
	if(pstart.scorez.score.data[7].score < (int) game.matrix.Game.score)
	{
		pstart.scorez.input_score(game.matrix.Game.score);
	}
}