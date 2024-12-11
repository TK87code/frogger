#include <raylib.h>
#include <math.h> /* abs */

#define CELL_SIZE 48

#define LANE_SIZE 64
#define LANE_COUNT 10
#define FIELD_COL 24

#define SCREEN_WIDTH CELL_SIZE * FIELD_COL
#define SCREEN_HEIGHT CELL_SIZE * LANE_COUNT

typedef struct lane{
    float velocity;
    char contents[LANE_SIZE + 1];
}lane_t;

lane_t *lanes[LANE_COUNT];

int main(void)
{
    int i, j;
    int start_from;
    int tile_offset;
    const int TILE_SIZE = CELL_SIZE;
    float time_since_start = 0.0;
    
    InitWindow(SCREEN_WIDTH,SCREEN_HEIGHT, "Frogger");
    SetTargetFPS(60);               
    
    /* Create lanes. 64 elements in each lane */
    lane_t lane_goal = 
    {0.0,  "ww.wwww.wwww.wwww.wwww.wwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwww"};
    lane_t lane_log3 = 
    {4.0,  "...llll.....llll..llll........llll..llll...llll......llll..llll."};
    lane_t lane_log2 = 
    {2.0,  "..lll..lll....lll..lll.lll.....lll.....lll...lll....lll.....lll."};
    lane_t lane_log1 = 
    {-2.0, "..lll.......lll....lll..lll..lll.....lll..lll...lll..lll....lll."};
    lane_t lane_pavement2 = 
    {0.0,  "pppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppp"};
    lane_t lane_bus2 = 
    {2.0, "..bbb....bbb...bbb.....bbb....bbb....bbb...bbb.bbb........bbb..."};
    lane_t lane_car2 = 
    {-3.0, ".cc........cc....cc...cc..cc.cc.......cc.......cc....cc.....cc.."};
    lane_t lane_bus1 = 
    {4.0,  "..bbb.bbb........bbb...bbb...bbb......bbb.bbb....bbb.....bbb...."};
    lane_t lane_car1 = 
    {-2.0, ".cc.....cc....cc..cc...cc.....cc....cc.....cc...cc.....cc...cc.."};
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
    
    
    while (!WindowShouldClose())    
    {
        Color color = PINK;
        
        time_since_start += (float)GetFrameTime();
        
        /* UPDATE */
        
        BeginDrawing();
        
        ClearBackground(color);
        
        /* Draw lanes */
        for (i = 0; i < LANE_COUNT; i++){
            start_from = (int)(time_since_start * lanes[i]->velocity) % LANE_SIZE;
            if (start_from < 0){ /* if start_from_x is negative value */
                start_from = LANE_SIZE - abs(start_from);
            }
            
            /* Calculate offset value with in the tile. If distance = time * vel is 0.5, it means the tile should move half of the tile length. So, first multiply 0.5 with tile size to get the length in pixel, and use the remainder to get the offset "within" the tile */
            tile_offset = (int)((float)TILE_SIZE * time_since_start * lanes[i]->velocity) % TILE_SIZE;
            
            
            /* Here, to mitigate the influence that caused by offsetting on x-axis, I draw extra 1 tile on the left hand side, and 2 on the right hand side */
            for (j = 0; j < LANE_SIZE; j++){
                if (j * TILE_SIZE > SCREEN_WIDTH + (2 * TILE_SIZE)) break;
                
                switch (lanes[i]->contents[(j + start_from) % LANE_SIZE]){
                    case 'c':{ color = ORANGE; }break;
                    case 'b':{ color = BLUE; }break;
                    case 'p':{ color = GRAY; }break;
                    case 'w':{ color = DARKGRAY; }break;
                    case 'l':{ color = BROWN; }break;
                    case '.':{ color = BLACK; }break;
                }
                
                DrawRectangle( (j - 1) * TILE_SIZE - tile_offset, i * TILE_SIZE, TILE_SIZE, TILE_SIZE, color);
                
            }
        }
        
        EndDrawing();
    }
    
    CloseWindow();        
    
    return 0;
}