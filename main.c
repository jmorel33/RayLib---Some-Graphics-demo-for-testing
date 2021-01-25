#pragma once

#include <raylib.h>
#include <raymath.h>
#include <string.h>
#include "data.h"
#include "rlgl.h"               // raylib OpenGL abstraction layer to OpenGL 1.1, 3.3 or ES2

#include <stdlib.h>
#include <math.h>

#define max(a, b) ((a) > (b) ? (a) : (b))
#define min(a, b) ((a) < (b) ? (a) : (b))

#define __STARFIELD_H__ 
#define MAXSTARS 8

typedef struct Stars {
	float x;
	float y;
	float speedX;
	float speedY;
    float xsin;
    float ysin;
} Stars;

typedef struct Starfield2D {
	Texture2D sprite;
	Vector2 position;
	Vector2 size;
	Stars stars[MAXSTARS];
} Starfield2D;

static int chars_x = 32;
static int chars_y = 12;

static Vector2 grid_pos[12][32];

Starfield2D Init_Starfield2D(Texture2D sprite, Vector2 position, Vector2 size);
void SetSpeed_Starfield2D(Starfield2D *starfield, Vector2 speed);
void Draw_Starfield2D(Starfield2D *starfield, Vector2 velocity);

inline static float Rand(float a) {
	return (float)rand()/(float)(RAND_MAX/a);
}


// Draw a part of a texture (defined by a rectangle) with 'pro' parameters
// NOTE: origin is relative to destination rectangle size
void DrawTextureProSK (Texture2D texture, Rectangle source, Rectangle dest, Vector2 skew, float rotation, Color tint)
{
    // Check if texture is valid
    if (texture.id > 0)
    {
        float width = (float)texture.width;
        float height = (float)texture.height;

        bool flipX = false;

        if (source.width < 0) { flipX = true; source.width *= -1; }
        if (source.height < 0) source.y -= source.height;

        rlEnableTexture(texture.id);

        rlPushMatrix();
            rlTranslatef(dest.x, dest.y, 0.0f);
            rlRotatef(rotation, 0.0f, 0.0f, 1.0f);
            //rlTranslatef(-origin.x, -origin.y, 0.0f);

            rlBegin(RL_QUADS);
                rlColor4ub(tint.r, tint.g, tint.b, tint.a);
                rlNormal3f(0.0f, 0.0f, 1.0f);                          // Normal vector pointing towards viewer

                // Bottom-left corner for texture and quad
                if (flipX) rlTexCoord2f((source.x + source.width)/width, source.y/height);
                else rlTexCoord2f(source.x/width, source.y/height);
                rlVertex2f(0.0f + skew.x, 0.0f);

                // Bottom-right corner for texture and quad
                if (flipX) rlTexCoord2f((source.x + source.width)/width, (source.y + source.height)/height);
                else rlTexCoord2f(source.x/width, (source.y + source.height)/height);
                rlVertex2f(0.0f, dest.height);

                // Top-right corner for texture and quad
                if (flipX) rlTexCoord2f(source.x/width, (source.y + source.height)/height);
                else rlTexCoord2f((source.x + source.width)/width, (source.y + source.height)/height);
                rlVertex2f(dest.width, dest.height + skew.y);

                // Top-left corner for texture and quad
                if (flipX) rlTexCoord2f(source.x/width, source.y/height);
                else rlTexCoord2f((source.x + source.width)/width, (source.y)/height);
                rlVertex2f(dest.width + skew.x, 0.0f + skew.y);
            rlEnd();
        rlPopMatrix();

        rlDisableTexture();
    }
}


// -------------------------------------------------------------------------------------------------------------
Starfield2D Init_Starfield2D(Texture2D sprite, Vector2 position, Vector2 size) {
	Starfield2D p;
	p.sprite = sprite;
	p.position = position;
	p.size = size;
	
	for(int i = 0; i < MAXSTARS; i++) {
		p.stars[i].x = GetRandomValue(p.position.x , p.position.x + p.size.x);
		p.stars[i].y = GetRandomValue(p.position.y , p.position.y + p.size.y);
	}

	return p;
}

void SetSpeed_Starfield2D(Starfield2D *starfield, Vector2 speed) {
	for(int i = 0; i < MAXSTARS; i++) {
		starfield->stars[i].speedX = speed.x;
		starfield->stars[i].speedY = speed.y;
		starfield->stars[i].xsin = GetRandomValue(0,360);
		starfield->stars[i].ysin = GetRandomValue(0,360);
	}
}

void Draw_Starfield2D(Starfield2D *starfield, Vector2 velocity) {
	for(int i = 0; i < MAXSTARS; i++) {
		starfield->stars[i].x += starfield->stars[i].speedX * velocity.x;
		starfield->stars[i].y += starfield->stars[i].speedY * velocity.y;

		if( starfield->stars[i].x > ( starfield->position.x + starfield->size.x ) ) {
			starfield->stars[i].x = starfield->position.x;
			starfield->stars[i].y = GetRandomValue(starfield->position.y , starfield->position.y + starfield->size.y);
		}

		if( starfield->stars[i].y > ( starfield->position.y + starfield->size.y ) ) {
			starfield->stars[i].y = starfield->position.y;
			starfield->stars[i].x = GetRandomValue(starfield->position.x , starfield->position.x + starfield->size.x);
		}

		if( starfield->stars[i].x < starfield->position.x ) starfield->stars[i].x = starfield->position.x + starfield->size.x;
		if( starfield->stars[i].y > ( starfield->position.y + starfield->size.y ) ) starfield->stars[i].y = starfield->position.y;
		if( starfield->stars[i].y < starfield->position.y ) starfield->stars[i].y = starfield->position.y + starfield->size.y;

		DrawTexture(starfield->sprite, starfield->stars[i].x + sin(starfield->stars[i].xsin)* velocity.y *8, starfield->stars[i].y + sin(starfield->stars[i].ysin)* velocity.y , WHITE);
        starfield->stars[i].xsin += .1 ;
        starfield->stars[i].ysin += .1 ;
	}
}

Vector2 VirtualScreen = (Vector2) { 1280, 720};

void DrawQuadSprite ( Texture2D sprite , Vector2 position, float scaleX, float scaleY, Color color);
void DrawFrameBuffer(RenderTexture2D renderer);
void DrawTextImage(Texture2D texture, char * txt, float x, float y );

int main() {

    InitWindow(VirtualScreen.x, VirtualScreen.y, "wow that is fun !");
    SetExitKey(NULL);

    int current_monitor = GetCurrentMonitor();
    int screenWidth = GetMonitorWidth(current_monitor);
    int screenHeight = GetMonitorHeight(current_monitor);
    SetWindowState(FLAG_FULLSCREEN_MODE);
    SetWindowSize(screenWidth,screenHeight);
    SetTargetFPS(GetMonitorRefreshRate(current_monitor));
	
    int framecount = 0;

	HideCursor();

    enum { STATE_WAITING, STATE_LOADING, STATE_FINISHED } state = STATE_WAITING;

	// -------------------------------------------------------------------------------------------------------------
	// Icone window
	Image icon = {&icon_data, 32, 32, 1, UNCOMPRESSED_R8G8B8A8};
	SetWindowIcon(icon);

	// -------------------------------------------------------------------------------------------------------------
	// Copper
	Texture2D copper[11];
	for(int i = 0; i < 11; i++) {
        Image _copper = {&copper_data[i], 4, 56, 1, UNCOMPRESSED_R8G8B8A8};
		copper[i] = LoadTextureFromImage(_copper);
	}

	// -------------------------------------------------------------------------------------------------------------
	// Copper Bar
	Image cop1 = {&copper_bar_data, 8, 34, 1, UNCOMPRESSED_R8G8B8A8};
	Texture2D copper_bar = LoadTextureFromImage(cop1);

	// -------------------------------------------------------------------------------------------------------------
	// Logo
	Image _logo = {&logo_data, 636, 108, 1, UNCOMPRESSED_R8G8B8A8};
	Texture2D logo = LoadTextureFromImage(_logo);

	// -------------------------------------------------------------------------------------------------------------
	// Fonte
	Image fontData = {&font_data, 2048, 32, 1, UNCOMPRESSED_R8G8B8A8};
	Texture2D characters = LoadTextureFromImage(fontData);

	Image _font2_data = {&font2_data, 16, 946, 1, UNCOMPRESSED_R8G8B8A8};
	Texture2D font2_data = LoadTextureFromImage(_font2_data);

	// -------------------------------------------------------------------------------------------------------------
	// Balles
	Image _balle1_data = {&ball1_data, 30, 30, 1, UNCOMPRESSED_R8G8B8A8};
	Texture2D balle1 = LoadTextureFromImage(_balle1_data);
	Image _balle2_data = {&ball2_data, 22, 22, 1, UNCOMPRESSED_R8G8B8A8};
	Texture2D balle2 = LoadTextureFromImage(_balle2_data);
	Image _balle3_data = {&ball3_data, 18, 18, 1, UNCOMPRESSED_R8G8B8A8};
	Texture2D balle3 = LoadTextureFromImage(_balle3_data);

	// -------------------------------------------------------------------------------------------------------------
	// Music
    
	InitAudioDevice();

	Music music = LoadMusicStream("NTMMEG.ogg");
	PlayMusicStream(music);
//	SetMusicVolume(music, 1.0f);

	// -------------------------------------------------------------------------------------------------------------
	// Framebuffer
	RenderTexture2D frameBuffer = LoadRenderTexture( VirtualScreen.x, VirtualScreen.y );
	SetTextureFilter(frameBuffer.texture, FILTER_POINT);

	// -------------------------------------------------------------------------------------------------------------
	// Divers
	char * text1 =  "    M A  N  T  R  O  N  I  C    " \
					"       WISHES TO EVERYONE       " \
					"                A               " \                    
					"  H   H  AAA  PPPP  PPPP  Y   Y " \
                    "  H   H A   A P   P P   P Y   Y " \
					"  HHHHH AAAAA PPPP  PPPP   YYY  " \
					"  H   H A   A P     P       Y   " \
					"  H   H A   A P     P       Y   " \
					"        N E W    Y E A R        " \
					"          - 2 0 2 1 -           " \
					"                                " \
					"        MADE WITH RAYLIB        ";


	char *scrollText = ".........................................HEY!!!  .....             !!!        -----*****-----                  .... THIS IS ......              / / /    - M - A - N - T - R - O - N - I - C -    / / /                       :) :) :) :) :) :) :)                      FINALLY I MADE IT TO RAYLIB!!             AND SO HAPPY I DID!.....................                     NOW I'M BACK TO MY ROOTS...                  C LANGUAGE..!!!                           AND I THINK I AM WELL SERVED WITH RAYLIB,                  SOOO....               IF YOU THINK YOU GOT A BIT OF INSPIRATION FOR ANY PROJECT AND NEED A HELPING HAND,                CHECK IT OUT,                              AND CHECK THE RAYLIB DISCORD CHANNEL ALSO.                IT DOES A LOT OF GOOD THINGS FOR YOU AND A LOT OF RESSOURCES AND EXAMPLES TO KICK OFF YOUR PROJECT!......                      ...             ALSO I AM WORKING ON AN ACTUAL GAME, BUT I'LL KEEP IT QUIET UNTIL I ACTUALLY HAVE SOMETHING TO SHOW OFF !!                                                    :D :D :D                                          !!!!!                                                    ";
    char *scrollText2 = "- - - M A N T R O N I C - - -         LIKES TO MAKE RETRO LOOKING DEMO SCREENS.....    SOMETIMES HE GRABS ASSETS FROM OTHER PLACES AND TRIES THINGS OUT!  :) :) :)   WELL ANYWAY IT WAS LAYING THERE COLLECTING DUST!  MIGHT AS WELL TRY SOMETHIUNG WITH IT :P         ";
	float scrollTextX = VirtualScreen.x;
	int textLen = strlen(scrollText);
	int textLen2 = strlen(scrollText2);
    int x_offset, y_offset;

	float rastsin=0;
	float rastoffset=0.07;
	float amp=300;

	Starfield2D starfield0 = Init_Starfield2D(balle1, (Vector2) {-32,-32}, (Vector2) {VirtualScreen.x+32,VirtualScreen.y+32});
	Starfield2D starfield1 = Init_Starfield2D(balle1, (Vector2) {-32,-32}, (Vector2) {VirtualScreen.x+32,VirtualScreen.y+32});
	Starfield2D starfield2 = Init_Starfield2D(balle1, (Vector2) {-32,-32}, (Vector2) {VirtualScreen.x+32,VirtualScreen.y+32});
	Starfield2D starfield3 = Init_Starfield2D(balle2, (Vector2) {-32,-32}, (Vector2) {VirtualScreen.x+32,VirtualScreen.y+32});
	Starfield2D starfield4 = Init_Starfield2D(balle2, (Vector2) {-32,-32}, (Vector2) {VirtualScreen.x+32,VirtualScreen.y+32});
	Starfield2D starfield5 = Init_Starfield2D(balle3, (Vector2) {-32,-32}, (Vector2) {VirtualScreen.x+32,VirtualScreen.y+32});
	Starfield2D starfield6 = Init_Starfield2D(balle3, (Vector2) {-32,-32}, (Vector2) {VirtualScreen.x+32,VirtualScreen.y+32});
	Starfield2D starfield7 = Init_Starfield2D(balle3, (Vector2) {-32,-32}, (Vector2) {VirtualScreen.x+32,VirtualScreen.y+32});

	SetSpeed_Starfield2D(&starfield0, (Vector2){4.0,4.0});
	SetSpeed_Starfield2D(&starfield1, (Vector2){3.5,3.5});
	SetSpeed_Starfield2D(&starfield2, (Vector2){3.0,3.0});
	SetSpeed_Starfield2D(&starfield3, (Vector2){2.5,2.5});
	SetSpeed_Starfield2D(&starfield4, (Vector2){2.0,2.0});
	SetSpeed_Starfield2D(&starfield5, (Vector2){1.5,1.5});
	SetSpeed_Starfield2D(&starfield6, (Vector2){1.0,1.0});
	SetSpeed_Starfield2D(&starfield7, (Vector2){0.5,0.5});

	float sinx = 0;
	float siny = 0;
	float oldsinx = 0;
	float oldsiny = 0;
    float sinparam = 0;

	float ySin[strlen(scrollText2)];
	float textX = VirtualScreen.x;
    float curve;

    bool stay_in_loop = true;

	// -------------------------------------------------------------------------------------------------------------
	// Game Loop
	while(!WindowShouldClose() & stay_in_loop) {
		UpdateMusicStream(music);

		sinparam += 0.1;
		float x = sinparam;
		for (int i = 0; i < textLen2; i++) {
			ySin[i] = (float)sin(x)*20;
			x += ((PI*2) / 24);
		}

		// -------------------------------------------------------------------------------------------------------------
		// Scroll Text
		scrollTextX -= GetFrameTime() * 500;
		if(scrollTextX < -textLen*32+32 ) scrollTextX = VirtualScreen.x+32;

		rastsin += GetFrameTime();

		// -------------------------------------------------------------------------------------------------------------
		// Framebuffer
		BeginTextureMode(frameBuffer);
		{
			ClearBackground(BLACK);

			Draw_Starfield2D(&starfield7, (Vector2){0,-1});

			// -------------------------------------------------------------------------------------------------------------
			// Draw copper
			int scale = VirtualScreen.x / 160;
            y_offset = 200;
            int plasmaY = 112;
            curve = sin(cos(sin(rastsin )*sin(sinparam * 0.1) * 0.1) * cos(sinparam * 0.015) * 0.1 ) * 0.05 + 0.001;
			for(int x = 0; x < 160; x++) {
                for(int y = 10; y >= 0; y--) {
				DrawTexturePro( copper[y], (Rectangle) { 0, 0, 4, 56 }, (Rectangle) {x*scale, (296/2) + sin(rastsin-rastoffset*(y*5)-(x*curve))*amp+y_offset, scale, plasmaY }, (Vector2) {0}, 0, WHITE );
                }
			}

			// -------------------------------------------------------------------------------------------------------------
			// Draw Starfield with balle texture
			Draw_Starfield2D(&starfield6, (Vector2){0,-1});
			Draw_Starfield2D(&starfield5, (Vector2){0,-1});
			Draw_Starfield2D(&starfield4, (Vector2){0,-1});
			Draw_Starfield2D(&starfield3, (Vector2){0,-1});
			Draw_Starfield2D(&starfield2, (Vector2){0,-1});
			Draw_Starfield2D(&starfield1, (Vector2){0,-1});

			// -------------------------------------------------------------------------------------------------------------
			// Draw Logo (636x108)
            Vector2 logosize = {636,108};
            x_offset = (VirtualScreen.x-logosize.x)*0.5 ;
            y_offset = 0;
            
			for(int i = 0; i < logosize.y; i++) {
				DrawTexturePro( logo,
                (Rectangle) { 0, i, logosize.x, 1 }, 
                (Rectangle) {x_offset+sin(sinparam*0.1 + curve*i)*64 - 32, y_offset+i, logosize.x, 1 }, 
                (Vector2) { 0, 0 }, 0, WHITE );
			}


			// -------------------------------------------------------------------------------------------------------------
			// Draw Sine Flag
 
            int cell_size = 32;

            x_offset = (VirtualScreen.x-((chars_x+1)*cell_size))*0.5;
            y_offset = 5 * cell_size;
            float x_sin = 0;
            float y_sin = 0;
            Vector2 cellsize;
            int char_i = 0;
            oldsinx = sinx;
            oldsiny = siny;

			for(int y = 0; y < chars_y; y += 1) {
				for(int x = 0; x < chars_x; x += 1) {
                    x_sin = sin(sinx);
                    y_sin = sin(siny);
                    grid_pos[y][x].x = (x+x_sin)*cell_size + x_offset;
                    grid_pos[y][x].y = (y+y_sin)*cell_size + y_offset;
					siny += 0.2;

                    if (x<(chars_x-1)) {cellsize.x = ((grid_pos[y][x+1].x - grid_pos[y][x].x));} else {cellsize.x = cell_size;};
                    if (y<(chars_y-1)) {cellsize.y = ((grid_pos[y+1][x].y - grid_pos[y][x].y) + cos(siny)+1.5);} else {cellsize.y = cell_size;};
                    
					DrawRectangle(grid_pos[y][x].x, grid_pos[y][x].y, cellsize.x, cellsize.y, (Color) { abs(y_sin*128.0)+127,abs(y_sin*128.0)+127,abs(y_sin*128.0),255 } );
                    
                            DrawTexturePro(font2_data,
                                (Rectangle) {0, (text1[y*32+x] - 32) * 16, 16, 16 },
                                (Rectangle) {grid_pos[y][x].x, grid_pos[y][x].y , cellsize.x, cellsize.y},
                                (Vector2) {0},0,(Color) {abs(y_sin*255.0),abs(y_sin*128.0),abs(y_sin*128.0),255});
                //    sinx +=sin(siny);
				//	sinx += 0.2;
				}

				siny += 0.4;  // this is the depth of the waves
				sinx = oldsinx;
			}

			siny = oldsiny + 0.02;  // this is the vertical wave movement per frame

			// -------------------------------------------------------------------------------------------------------------
			// Draw Copper Bar
			DrawTextureQuad(copper_bar, (Vector2){80,1}, (Vector2){0},(Rectangle){0,580,VirtualScreen.x,68},WHITE);

			// -------------------------------------------------------------------------------------------------------------
			// Draw Scroll Text
			for(int i=0; i < textLen; i++) {
				if (scrollTextX + ( i << 5 ) > -32 && scrollTextX + ( i << 5 ) < VirtualScreen.x + 32) {
					DrawTextureProSK(characters,
						(Rectangle) { (scrollText[i] - 32) << 5, 0, 32, 32 },
						(Rectangle) { scrollTextX + (i << 5) , 580, 32, 64 },
						(Vector2) {32,0},0,WHITE);
				}
			}
            // -------------------------------------------------------------------------------------------------------------
			// Scroll Text2
            textX -= GetFrameTime() * 300;
            if(textX < -textLen2*16 ) textX = VirtualScreen.x;

			for(int i=0; i < textLen2; i++) {
				if(textX + ( i * 16 + 1 ) < VirtualScreen.x-16 && textX + ( i * 16 + 1 ) > 16) {
					DrawTexturePro(font2_data,
						(Rectangle) { 0, (scrollText2[i] - 32) * 16, 16, 16 },
						(Rectangle) { textX + ( i * 16 + 1 ) , 680 + ySin[i] , 16, 16 },
						(Vector2) {0,0},ySin[i]*.5, WHITE);
				}
			}
            			Draw_Starfield2D(&starfield0, (Vector2){0,-1});


		}
        
		EndTextureMode();

		BeginDrawing();
		{
			ClearBackground(BLACK);

			// Draw final frameBuffer
			DrawFrameBuffer(frameBuffer);
            
            // debug
            if (IsKeyDown(KEY_KP_ENTER)) {
            DrawText(FormatText("curve %i", (float)curve), 0, 200, 20, DARKGRAY);
            
            DrawText(FormatText("FRAMES=%i", (int)framecount), 0, 0, 20, DARKGRAY);

            int current_monitor = GetCurrentMonitor();

            DrawText(FormatText("monitors = %i, current = %i", (int)GetMonitorCount(), (int)current_monitor), 0, 60, 20, DARKGRAY);

            DrawText(GetMonitorName(current_monitor), 0, 80, 20, DARKGRAY);
            DrawText(FormatText("screen is %ix%i at %i fps", (int)GetMonitorWidth(current_monitor), (int)GetMonitorHeight(current_monitor), (int)GetMonitorRefreshRate(current_monitor)), 0, 100, 20, DARKGRAY);
            DrawText(FormatText("screen is %ix%i mm", (int)GetMonitorPhysicalWidth(current_monitor), (int)GetMonitorPhysicalHeight(current_monitor)), 0, 120, 20, DARKGRAY);
            }

            if (IsKeyDown(KEY_FOUR) & IsKeyDown(KEY_ZERO) & IsKeyDown(KEY_ONE)) {
            stay_in_loop = false;
            }

		}
		EndDrawing();
        
        framecount++;

	}

	UnloadRenderTexture(frameBuffer);
	UnloadMusicStream(music);
	CloseWindow();
	return 0;
}

// -------------------------------------------------------------------------------------------------------------
// UPDATE BUFFER , DRAW QUAD AND TEXT
void DrawQuadSprite ( Texture2D sprite , Vector2 position, float scaleX, float scaleY, Color color) {
	Rectangle src = (Rectangle) { 0, 0, sprite.width * scaleX, sprite.height*scaleY };
	Rectangle dest = (Rectangle) { position.x, position.y, sprite.width * scaleX, sprite.height*scaleY };
	DrawTexturePro ( sprite , src , dest , (Vector2) { 0,0 } , 0 , color );
}

void DrawFrameBuffer(RenderTexture2D renderer) {
	float verticalScale = GetScreenHeight () / VirtualScreen.y;
	float horizontalScale = GetScreenWidth () / VirtualScreen.x;
	float scale = min (horizontalScale, verticalScale);

	DrawTexturePro (renderer.texture,(Rectangle) { 0.0f, 0.0f, (float)renderer.texture.width, (float)-renderer.texture.height },(Rectangle) { ( GetScreenWidth () - ( VirtualScreen.x*scale) ) * 0.5 , ( GetScreenHeight () - (VirtualScreen.y * scale) ) * 0.5, VirtualScreen.x * scale, VirtualScreen.y * scale },(Vector2) { 0, 0 }, 0.0f, WHITE);
}

void DrawTextImage(Texture2D texture, char * txt, float x, float y ) {
	DrawTexturePro(texture,(Rectangle) { (txt[0] - 32) * 24, 0, 24, 24 } ,(Rectangle) { x , y, 64, 64 },(Vector2) {0},0,WHITE);
}
