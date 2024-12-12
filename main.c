#include <raylib.h>
#include <math.h>   /* abs */
#include <string.h> /* memset */
#include "tk_math.h"

#define CELL_SIZE 48

#define LANE_SIZE 64
#define LANE_COUNT 10

#define SCREEN_WIDTH CELL_SIZE * 24
#define SCREEN_HEIGHT CELL_SIZE * 10

const int FIELD_COL = 24;
const int FIELD_ROW = 10;

typedef struct lane{
    float velocity;
    char contents[LANE_SIZE + 1];
}lane_t;

char danger_buffer[SCREEN_WIDTH * SCREEN_HEIGHT];

/* Function Prototypes */
void handle_input(float *frog_x, float *frog_y);

int main(void)
{
    
    lane_t *lanes[LANE_COUNT];
    float frog_x = 11.0;
    float frog_y = 9.0; /* Frog position in a game unit */
    int y, x;
    int h, w;
    int start_from;
    int tile_offset;
    const int TILE_SIZE = CELL_SIZE;
    float time_since_start = 0.0;
    
    InitWindow(SCREEN_WIDTH,SCREEN_HEIGHT, "Frogger");
    SetTargetFPS(60);               
    
    /* Create lanes. 64 elements in each lane */
    lane_t lane_goal = 
    {0.0,  "wwhhwwwhhwwwhhwwwhhwwwhhwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwww"};
    lane_t lane_log3 = 
    {0.5,  "xxxllllxxxxxllllxxllllxxxxxxxxllllxxllllxxxllllxxxxxxllllxxllllx"};
    lane_t lane_log2 = 
    {0.5,  "xxlllxxlllxxxxlllxxlllxlllxxxxxlllxxxxxlllxxxlllxxxxlllxxxxxlllx"};
    lane_t lane_log1 = 
    {-0.5, "xxlllxxxxxxxlllxxxxlllxxlllxxlllxxxxxlllxxlllxxxlllxxlllxxxxlllx"};
    lane_t lane_pavement2 = 
    {0.0,  "pppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppp"};
    lane_t lane_bus2 = 
    {0.5, "..bbb....bbb...bbb.....bbb....bbb....bbb...bbb.bbb........bbb..."};
    lane_t lane_car2 = 
    {-0.5, ".cc........cc....cc...cc..cc.cc.......cc.......cc....cc.....cc.."};
    lane_t lane_bus1 = 
    {0.5,  "..bbb.bbb........bbb...bbb...bbb......bbb.bbb....bbb.....bbb...."};
    lane_t lane_car1 = 
    {-0.5, ".cc.....cc....cc..cc...cc.....cc....cc.....cc...cc.....cc...cc.."};
    lane_t lane_pavement1 = 
    {0.0,  "pppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppp"};
    
    lanes[0] = &lane_goal;
    lanes[1] = &lane_log3;
    lanes[2] = &lane_log2;
    lanes[3] = &lane_log1;
    lanes[4] = &lane_pavement2;
    lanes[5] = &lane_bus2;
    lanes[6] = &lane_car2;
    lanes[7] = &lane_bus1;
    lanes[8] = &lane_car1;
    lanes[9] = &lane_pavement1;
    
    memset(danger_buffer, 0, sizeof(int) * FIELD_ROW * FIELD_COL);
    
    while (!WindowShouldClose())    
    {
        Color color = PINK; /* Debug color */
        char tl = 0, tr = 0, bl = 0, br = 0; /* Top left, bottom right, bottom left, bottom right of Frog */
        
        time_since_start += (float)GetFrameTime();
        
        /* If the Frog on the log lanes, move with it */
        if (frog_y <= 3 && frog_y != 0){
            frog_x -= (float)GetFrameTime() * lanes[(int)frog_y]->velocity;
        }
        
        handle_input(&frog_x, &frog_y);
        
        // Check collition : check for 4 corner of the frog, 1 pixel pudding
        tl = danger_buffer[(((int)frog_y * TILE_SIZE)+ 1) * SCREEN_WIDTH + ((int)frog_x * TILE_SIZE) + 1];
        tr = danger_buffer[(((int)frog_y * TILE_SIZE)+ 1) * SCREEN_WIDTH + (((int)frog_x + 1) * TILE_SIZE) - 1];
        bl = danger_buffer[((((int)frog_y + 1) * TILE_SIZE) - 1) * SCREEN_WIDTH + ((int)frog_x * TILE_SIZE) + 1];
        br = danger_buffer[((((int)frog_y + 1) * TILE_SIZE) -1) * SCREEN_WIDTH + (((int)frog_x + 1) * TILE_SIZE) - 1];
        
        /* If detecting collision, bring back the Frog at start position.*/
        /*
        if (tl == 1 || tr == 1 || bl == 1 || br == 1){
            frog_y = 9.0;
            frog_x = 11.0;
        }
*/
        
        BeginDrawing();
        
        ClearBackground(color);
        
        /* Draw lanes */
        for (y = 0; y < LANE_COUNT; y++){
            /* Find a element offset */
            start_from = (int)(time_since_start * lanes[y]->velocity) % LANE_SIZE;
            if (start_from < 0){ /* if start_from_x is negative value */
                start_from = LANE_SIZE - abs(start_from);
            }
            
            /* Calculate offset value with in the tile. If distance = time * vel is 0.5, it means the tile should move half of the tile length. So, first multiply 0.5 with tile size to get the length in pixel, and use the remainder to get the offset "within" the tile */
            tile_offset = (int)((float)TILE_SIZE * time_since_start * lanes[y]->velocity) % TILE_SIZE;
            
            
            /* Here, to mitigate the influence that caused by offsetting on x-axis, I draw extra 1 tile on the left hand side, and 2 on the right hand side */
            for (x = 0; x < LANE_SIZE; x++){
                if (x * TILE_SIZE > SCREEN_WIDTH + (2 * TILE_SIZE)) break;
                
                char symbol = lanes[y]->contents[(x + start_from) % LANE_SIZE];
                
                switch (symbol){
                    case 'c':{ color = ORANGE;}   break;
                    case 'b':{ color = BLUE;}     break;
                    case 'p':{ color = GRAY;}     break;
                    case 'w':{ color = DARKGRAY;} break;
                    case 'l':{ color = BROWN;}    break;
                    case '.':{ color = BLACK;}    break;
                    case 'x':{ color = DARKBLUE;} break;
                    case 'h':{ color = BLACK;}    break;
                }
                
                /* Filed sprite drawing */
                DrawRectangle((x - 1) * TILE_SIZE - tile_offset, y * TILE_SIZE, TILE_SIZE, TILE_SIZE, color);
                
                /* Fill danger buffer. h & w represents the pixel position on screen */
                for (h = y * TILE_SIZE; h < (y + 1) * TILE_SIZE; h++){
                    for (w = (x - 1) * TILE_SIZE - tile_offset; w < x * TILE_SIZE - tile_offset; w++){
                        
                        if (h >= 0 && w >= 0 && h < SCREEN_HEIGHT && w <= SCREEN_WIDTH){
                            danger_buffer[h * SCREEN_WIDTH + w] = (symbol  != '.' &&
                                                                   symbol != 'p' &&
                                                                   symbol != 'h' &&
                                                                   symbol != 'l');
                        }
                    }
                }
                
                for (h = 0; h < FIELD_ROW; h++){
                    for (w = 0; w < FIELD_COL; w++){
                        DrawText(TextFormat("%d", danger_buffer[(h*TILE_SIZE) * SCREEN_WIDTH + (w*TILE_SIZE)]), w*TILE_SIZE, h*TILE_SIZE, 2, WHITE);
                    }
                }
                
                DrawText(TextFormat("tl: %d, tr: %d bl: %d, br: %d", tl, tr, bl, br), 0, 0 , 20, WHITE);
            }
        }
        
        /* Draw a Frog */
        DrawRectangleV((Vector2){frog_x * TILE_SIZE, frog_y * TILE_SIZE}, (Vector2){(float)TILE_SIZE, (float)TILE_SIZE}, DARKGREEN);
        
        EndDrawing();
    }
    
    CloseWindow();        
    
    return 0;
}

void handle_input(float *frog_x, float *frog_y)
{
    /* Only take Arrow_up key when the Frog is on logs */
    if (*frog_y > 3){
        if (IsKeyReleased(KEY_LEFT)) *frog_x = tkmt_clampf(*frog_x - 1.0f, 0.0f, (float)FIELD_COL - 1.0f);
        if (IsKeyReleased(KEY_RIGHT)) *frog_x = tkmt_clampf(*frog_x + 1.0f, 0.0f, (float)FIELD_COL - 1.0f);
        if (IsKeyReleased(KEY_DOWN)) *frog_y = tkmt_clampf(*frog_y + 1.0f, 0.0f,(float)FIELD_ROW - 1.0f);
    }
    
    if (IsKeyReleased(KEY_UP)) *frog_y = tkmt_clampf(*frog_y - 1.0f, 0.0f, (float)FIELD_ROW - 1.0f);
    
    
}