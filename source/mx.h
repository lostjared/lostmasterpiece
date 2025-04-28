
#ifndef _MX_H_
#define _MX_H_

#include<stdio.h>
#include<stdlib.h>
#include"SDL.h"
#include"SDL_timer.h"
#include<assert.h>
#ifdef __EMSCRIPTEN__
#include<emscripten/emscripten.h>
#endif
#include"SDL_mxf.h"
// wrapper class
class mxHwnd {
    
    void (*renderscr)(int screen);
    void (*keyfunc)(int key);
    void (*keyfuncup)(int key);
    void (*mousemove)(int x, int y);
    void (*mousedown)(int button, int x, int y);
    void (*onevent)(SDL_Event *ievent);
    
    int scr,exitok;
    int is_fullscreen;
    
public:
    
    SDL_Window *window;
    
    ~mxHwnd() {
        SDL_FreeSurface(pscr);
        SDL_Quit();
    }
    
    SDL_Surface *pscr;
    
    int setfullscreen(int fullscreen,const char *title, int width, int height, int bpp)
    {
        return 0;
    }
    
    int isFullScreen() const
    {
        return is_fullscreen;
    }
    
    int init(const char *title, int width, int height, int bpp, bool fullscreen)
    {
        if(is_fullscreen)
            window = SDL_CreateWindow("MasterPiece", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, SDL_WINDOW_SHOWN | SDL_WINDOW_FULLSCREEN);
        else
            window = SDL_CreateWindow("MasterPiece", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, SDL_WINDOW_SHOWN);
        
        
        SDL_Surface *temp = SDL_GetWindowSurface(window);
        SDL_Surface* new_surface = SDL_CreateRGBSurfaceWithFormat(0,640,480,temp->format->BitsPerPixel,temp->format->format);
        pscr = new_surface;
        scr = 0;
        
        SDL_Surface *ico = SDL_LoadBMP("./img/block_red.bmp");
        SDL_SetWindowIcon(window, ico);
        SDL_FreeSurface(ico);
        
        keyfunc = 0,keyfuncup = 0,mousemove = 0,mousedown = 0,onevent = 0,exitok = 1;
        is_fullscreen = (int)fullscreen;
        
        return 0;
    }
    
    void setKeydown(void(*keyfuncx)(int key))
    {
        keyfunc = keyfuncx;
    }
    
    int getScreen() { return scr; }
    void setScreen(int scr) { this->scr = scr; }
    
    void setKeyup(void(*keyfuncupx)(int key))
    {
        keyfuncup = keyfuncupx;
    }
    
    void setMouseMove(void (*mousemovex)(int x, int y))
    {
        mousemove = mousemovex;
    }
    
    void setMouseDown(void(*mousedownx)(int button, int x, int y))
    {
        mousedown = mousedownx;
    }
    
    void setEventHandler(void (*oneventx)(SDL_Event *ievent))
    {
        onevent = oneventx;
    }
    
    void clear()
    {
        SDL_Rect rect = {0, 0, (Uint16)pscr->w, (Uint16)pscr->h};
        SDL_FillRect(pscr, &rect, 0);
    }
    
    static mxHwnd *mxhwnd_static;
    
    static void eventProc() {
        mxhwnd_static->loop();
    }

    void flip() {
	SDL_Surface *front = SDL_GetWindowSurface(window);
 	SDL_BlitScaled(pscr, NULL, front, NULL);
	SDL_UpdateWindowSurface(window);
    }    

    SDL_Event event;
    
    void loop() {
        
        
        
        if(scr != 5)
            clear();
        
        renderscr(scr);
        
        while(SDL_PollEvent(&event))
        {
            switch( event.type )
            {
                case SDL_QUIT:
                    printf("Quit called!\n");
                    exitok = 0;
                    return;
                    break;
                case SDL_JOYBUTTONDOWN:
                {
                    
                    
                    switch(event.jbutton.button)
                    {
                        case 11:
                            keyfunc(13);
                            break;
                        case 3:
                            keyfunc(27);
                            break;
                        case 8:
                            keyfunc(273);
                            break;
                        case 9:// right
                            keyfunc(275);
                            break;
                        case 6:// down
                            keyfunc(274);
                            break;
                        case 7://left
                            keyfunc(276);
                            break;
                        case 0:
                            keyfunc('a');
                            break;
                        case 1:
                            keyfunc('s');
                            break;
                    }
                    
                }
                    break;
                case SDL_KEYDOWN:
                    if(keyfunc != 0)
                    {
                        keyfunc(event.key.keysym.sym);
                    }
                    break;
                case SDL_KEYUP:
                    if(keyfuncup != 0)
                    {
                        keyfuncup(event.key.keysym.sym);
                    }
                    break;
                case SDL_MOUSEMOTION:
                    if(mousemove != 0)
                        mousemove(event.motion.x,event.motion.y);
                    break;
                case SDL_MOUSEBUTTONDOWN:
                    if(mousedown != 0)
                        mousedown(event.button.button,event.button.x,event.button.y);
                    break;
            }
            if(onevent != 0)
                onevent(&event);
        }
    }
    
    int initLoop(void (*rendery)(int screen))
    {
        renderscr = rendery;
#ifndef __EMSCRIPTEN__
        while(exitok == 1)
        {
            loop();
        }
#else
        emscripten_set_main_loop(mxHwnd::eventProc, 0, 1);
#endif
        return 1;
    }
    
    void kill()
    {
        exitok = 0;
    }
    
    void setPixel(SDL_Surface *screen, int x, int y, Uint8 r, Uint8 g, Uint8 b)
    {
        Uint8 *ubuff8;
        Uint16 *ubuff16;
        Uint32 *ubuff32;
        Uint32 color;
        char c1, c2, c3;
        
        /* Lock the screen, if needed */
        if(SDL_MUSTLOCK(screen)) {
            if(SDL_LockSurface(screen) < 0)
                return;
        }
        
        /* Get the color */
        color = SDL_MapRGB( screen->format, r, g, b );
        
        /* How we draw the pixel depends on the bitdepth */
        switch(screen->format->BytesPerPixel)
        {
            case 1:
                ubuff8 = (Uint8*) screen->pixels;
                ubuff8 += (y * screen->pitch) + x;
                *ubuff8 = (Uint8) color;
                break;
                
            case 2:
                ubuff8 = (Uint8*) screen->pixels;
                ubuff8 += (y * screen->pitch) + (x*2);
                ubuff16 = (Uint16*) ubuff8;
                *ubuff16 = (Uint16) color;
                break;
                
            case 3:
                ubuff8 = (Uint8*) screen->pixels;
                ubuff8 += (y * screen->pitch) + (x*3);
                
                
                if(SDL_BYTEORDER == SDL_LIL_ENDIAN) {
                    c1 = (color & 0xFF0000) >> 16;
                    c2 = (color & 0x00FF00) >> 8;
                    c3 = (color & 0x0000FF);
                } else {
                    c3 = (color & 0xFF0000) >> 16;
                    c2 = (color & 0x00FF00) >> 8;
                    c1 = (color & 0x0000FF);
                }
                
                ubuff8[0] = c3;
                ubuff8[1] = c2;
                ubuff8[2] = c1;
                break;
                
            case 4:
                ubuff8 = (Uint8*) screen->pixels;
                ubuff8 += (y*screen->pitch) + (x*4);
                ubuff32 = (Uint32*)ubuff8;
                *ubuff32 = color;
                break;
                
            default:
                fprintf(stderr, "Error: Unknown bitdepth!\n");
        }
        
        /* Unlock the screen if needed */
        if(SDL_MUSTLOCK(screen)) {
            SDL_UnlockSurface(screen);
        }
    }
    
    void setPixel(SDL_Surface *screen, int x, int y, Uint32 color)
    {
        Uint8 *ubuff8;
        Uint16 *ubuff16;
        Uint32 *ubuff32;
        char c1, c2, c3;
        
        /* Lock the screen, if needed */
        if(SDL_MUSTLOCK(screen)) {
            if(SDL_LockSurface(screen) < 0)
                return;
        }
        
        /* Get the color */
        //color = SDL_MapRGB( screen->format, r, g, b );
        
        /* How we draw the pixel depends on the bitdepth */
        switch(screen->format->BytesPerPixel)
        {
            case 1:
                ubuff8 = (Uint8*) screen->pixels;
                ubuff8 += (y * screen->pitch) + x;
                *ubuff8 = (Uint8) color;
                break;
                
            case 2:
                ubuff8 = (Uint8*) screen->pixels;
                ubuff8 += (y * screen->pitch) + (x*2);
                ubuff16 = (Uint16*) ubuff8;
                *ubuff16 = (Uint16) color;
                break;
                
            case 3:
                ubuff8 = (Uint8*) screen->pixels;
                ubuff8 += (y * screen->pitch) + (x*3);
                
                
                if(SDL_BYTEORDER == SDL_LIL_ENDIAN) {
                    c1 = (color & 0xFF0000) >> 16;
                    c2 = (color & 0x00FF00) >> 8;
                    c3 = (color & 0x0000FF);
                } else {
                    c3 = (color & 0xFF0000) >> 16;
                    c2 = (color & 0x00FF00) >> 8;
                    c1 = (color & 0x0000FF);
                }
                
                ubuff8[0] = c3;
                ubuff8[1] = c2;
                ubuff8[2] = c1;
                break;
                
            case 4:
                ubuff8 = (Uint8*) screen->pixels;
                ubuff8 += (y*screen->pitch) + (x*4);
                ubuff32 = (Uint32*)ubuff8;
                *ubuff32 = color;
                break;
                
            default:
                fprintf(stderr, "Error: Unknown bitdepth!\n");
        }
        
        /* Unlock the screen if needed */
        if(SDL_MUSTLOCK(screen)) {
            SDL_UnlockSurface(screen);
        }
    }
    
    Uint32 getPixel(SDL_Surface *Surface, Sint32 X, Sint32 Y)
    {
        Uint8  *bits;
        Uint32 Bpp;
        
        assert(X>=0);
        assert(X<Surface->w);
        
        Bpp = Surface->format->BytesPerPixel;
        bits = ((Uint8 *)Surface->pixels)+Y*Surface->pitch+X*Bpp;
        
        // Get the pixel
        switch(Bpp) {
            case 1:
                return *((Uint8 *)Surface->pixels + Y * Surface->pitch + X);
                break;
            case 2:
                return *((Uint16 *)Surface->pixels + Y * Surface->pitch/2 + X);
                break;
            case 3: { // Format/endian independent
                Uint8 r, g, b;
                r = *((bits)+Surface->format->Rshift/8);
                g = *((bits)+Surface->format->Gshift/8);
                b = *((bits)+Surface->format->Bshift/8);
                return SDL_MapRGB(Surface->format, r, g, b);
            }
                break;
            case 4:
                return *((Uint32 *)Surface->pixels + Y * Surface->pitch/4 + X);
                break;
        }
        return (Uint32)-1;
    }
    
};

// base class for objects
class mxObject {
    
public:
    void init(mxHwnd *mx)
    {
        this->mx = mx;
    }
    mxHwnd *mx;
};

// Wrapper around SDL_Surface for bitmaps
class mxSprite : public mxObject {
    
public:
    SDL_Surface *sptr;
    
    mxSprite()
    {
        sptr = 0;
    }
    ~mxSprite()
    {
        if(sptr != 0)
            SDL_FreeSurface(sptr);
        sptr = 0;
    }
    
    bool load(mxHwnd *mx, const char *str)
    {
        this->mx = mx;
        return load(str);
    }
    
    bool load(const char *str)
    {
        sptr = SDL_LoadBMP(str);
        if(sptr == 0)
        {
            fprintf(stderr, "Unable to load bitmap..  %s\n",str);
            return false;
        }
        else
            return true;
    }
    
    bool load(const char *str, int r, int g, int b)
    {
        sptr = SDL_LoadBMP(str);
        if(sptr == 0)
        {
            fprintf(stderr, "Unable to load bitmap ..%s", str);
            return false;
        }
        SDL_SetColorKey(sptr , SDL_TRUE, SDL_MapRGB(sptr->format, r,g,b));
        
        return true;
    }
    
    void display(class mxHwnd *mxhwnd, int x, int y, int x2, int y2, int w, int h)
    {
        SDL_Rect src;
        SDL_Rect dest;
        src.x = x2;
        src.y = y2;
        src.w = w;
        src.h = h;
        dest.x = x; dest.y = y;
        dest.w = h;
        dest.h = w;
        SDL_BlitSurface(sptr,&src,mxhwnd->pscr,&dest);
    }
    
    void display(class mxHwnd *mxhwnd, int x, int y)
    {
        display(mxhwnd,x,y,0,0,sptr->w,sptr->h);
    }
    
    // requires a call to init before use
    void display(int x, int y, int x2, int y2, int w, int h)
    {
        this->display(mx,x,y,x2,y2,w,h);
    }
    // same with this one
    void display(int x, int y)
    {
        this->display(mx,x,y);
    }
    
};


class mxFont : public mxObject {
public:
    SDL_Font* Font;
    Uint32 color;
    
    mxFont()
    {
        Font = 0;;
    }
    
    ~mxFont()
    {
        if(Font != 0)
            SDL_FreeFont(Font);
    }
    
    void load(mxHwnd *mx, const char *sfont)
    {
        this->mx = mx;
	color = SDL_MapRGBA(this->mx->pscr->format, 255,255,255,255);
        Font = SDL_InitFont(sfont);
        if(!Font)
        {
            fprintf(stderr, "An error occured while loading the font.");
            exit(1);
        }
        
    }
    void printText(int x, int y, const char *str)
    {
        SDL_PrintText(mx->pscr, Font,x,y,color, str);
    }
    
};


class mxPaint : public mxObject {
    
public:
    void drawRect(int x, int y, int x2, int y2, Uint32 color)
    {
        SDL_Rect rect = {(Sint16)x, (Sint16)y, (Uint16)x2, (Uint16)y2};
        SDL_FillRect(mx->pscr, &rect, color);
    }
};

#endif
