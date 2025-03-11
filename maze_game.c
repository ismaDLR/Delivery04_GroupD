/*******************************************************************************************
*
*   raylib maze game
*
*   Procedural maze generator using Maze Grid Algorithm
*
*   This game has been created using raylib (www.raylib.com)
*   raylib is licensed under an unmodified zlib/libpng license (View raylib.h for details)
*
*   Copyright (c) 2024-2025 Ramon Santamaria (@raysan5)
*
********************************************************************************************/

#include "raylib.h"

#include <stdlib.h>     // Required for: malloc(), free()

#define MAZE_WIDTH          64
#define MAZE_HEIGHT         64
#define MAZE_SCALE          10.0f

#define MAX_MAZE_ITEMS      16

#define PLAYER_WIDTH        4
#define PLAYER_HEIGHT       4
#define PLAYER_SPEED        3

// Declare new data type: Point
typedef struct Point {
    int x;
    int y;
} Point;

// Generate procedural maze image, using grid-based algorithm
// NOTE: Functions defined as static are internal to the module
static Image GenImageMaze(int width, int height, int spacingRows, int spacingCols, float pointChance);

//----------------------------------------------------------------------------------
// Main entry point
//----------------------------------------------------------------------------------
int main(void)
{
    // Initialization
    //---------------------------------------------------------
    const int screenWidth = 1280;
    const int screenHeight = 720;

    InitWindow(screenWidth, screenHeight, "Delivery04 - maze game");

    // Current application mode
    int currentMode = 1;    // 0-Game, 1-Editor

    // Random seed defines the random numbers generation,
    // always the same if using the same seed
    SetRandomSeed(67218);

    // Generate maze image using the grid-based generator
    // TODO: [1p] Implement GenImageMaze() function with required parameters
    Image imMaze = GenImageMaze(MAZE_WIDTH, MAZE_HEIGHT, 4, 4, 0.75f);

    // Load a texture to be drawn on screen from our image data
    // WARNING: If imMaze pixel data is modified, texMaze needs to be re-loaded
    Texture texMaze = LoadTextureFromImage(imMaze);

    // Player start-position and end-position initialization
    Point startCell = { 1, 1 };
    Point endCell = { imMaze.width - 2, imMaze.height - 2 };

    // Maze drawing position (editor mode)
    Vector2 mazePosition = {
        GetScreenWidth()/2 - texMaze.width*MAZE_SCALE/2,
        GetScreenHeight()/2 - texMaze.height*MAZE_SCALE/2
    };

    // Define player position and size
    Rectangle player = { mazePosition.x + 1*MAZE_SCALE + 2, mazePosition.y + 1*MAZE_SCALE + 2, PLAYER_WIDTH, PLAYER_HEIGHT };

    // Camera 2D for 2d gameplay mode
    // TODO: [2p] Initialize camera parameters as required
    Camera2D camera2d = { 0 };
    camera2d.target = (Vector2){ player.x + 2, player.y + 2 };
    camera2d.offset = (Vector2){ screenWidth/2.0f, screenHeight/2.0f };
    camera2d.rotation = 0.0f;
    camera2d.zoom = 10.0f;

    // Mouse selected cell for maze editing
    // Point selectedCell = { 0 };

    // Maze items position and state
    Point mazeItems[MAX_MAZE_ITEMS] = { 0 };
    bool mazeItemPicked[MAX_MAZE_ITEMS] = { 0 };
    int countItems = 0;
    
    // Define textures to be used as our "biomes"
    Texture texBiomes[4] = { 0 };
    texBiomes[0] = LoadTexture("resources/maze_atlas01.png");
    texBiomes[1] = LoadTexture("resources/maze_atlas02.png");
    texBiomes[2] = LoadTexture("resources/maze_atlas03.png");
    texBiomes[3] = LoadTexture("resources/maze_atlas04.png");
    // TODO: Load additional textures for different biomes
    int currentBiome = 0;

    // TODO: Define all variables required for game UI elements (sprites, fonts...)
    int score = 0;

    SetTargetFPS(60);       // Set our game to run at 60 frames-per-second
    //--------------------------------------------------------------------------------------

    // Main game loop
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
        // Update
        //----------------------------------------------------------------------------------
        // Select current mode as desired
        if (IsKeyPressed(KEY_SPACE)) currentMode = !currentMode; // Toggle mode: 0-Game, 1-Editor

        if (currentMode == 0) // Game mode
        {
            // TODO: [2p] Player 2D movement from predefined Start-point to End-point
            // Implement maze 2D player movement logic (cursors || WASD)
            // Use imMaze pixel information to check collisions
            // Detect if current playerCell == endCell to finish game
            
            
            if (IsKeyDown(KEY_UP) || IsKeyDown(KEY_W))
            {
                if (!ColorIsEqual(GetImageColor(imMaze, (player.x - mazePosition.x) / MAZE_SCALE, ((player.y - PLAYER_SPEED) - mazePosition.y) / MAZE_SCALE), WHITE) && !ColorIsEqual(GetImageColor(imMaze, (player.x + PLAYER_WIDTH - mazePosition.x) / MAZE_SCALE, ((player.y - PLAYER_SPEED) - mazePosition.y) / MAZE_SCALE), WHITE))
                {
                    if ((player.x - mazePosition.x) / MAZE_SCALE > 0 && (player.y - mazePosition.y) / MAZE_SCALE > 0)
                    {
                        player.y -= PLAYER_SPEED;
                    }
                }
            } 
            else if (IsKeyDown(KEY_DOWN) || IsKeyDown(KEY_S))
            {
                if (!ColorIsEqual(GetImageColor(imMaze, (player.x - mazePosition.x) / MAZE_SCALE, ((player.y + PLAYER_HEIGHT + PLAYER_SPEED) - mazePosition.y) / MAZE_SCALE), WHITE) && !ColorIsEqual(GetImageColor(imMaze, (player.x + PLAYER_WIDTH - mazePosition.x) / MAZE_SCALE, ((player.y + PLAYER_HEIGHT + PLAYER_SPEED) - mazePosition.y) / MAZE_SCALE), WHITE))
                {
                    if ((player.x - mazePosition.x) / MAZE_SCALE < MAZE_WIDTH && (player.y - mazePosition.y) / MAZE_SCALE < MAZE_HEIGHT)
                    {
                        player.y += PLAYER_SPEED;
                    }
                }
            }
            else if (IsKeyDown(KEY_LEFT) || IsKeyDown(KEY_A))
            {
                if (!ColorIsEqual(GetImageColor(imMaze, ((player.x - PLAYER_SPEED) - mazePosition.x) / MAZE_SCALE, (player.y - mazePosition.y) / MAZE_SCALE), WHITE) && !ColorIsEqual(GetImageColor(imMaze, ((player.x - PLAYER_SPEED) - mazePosition.x) / MAZE_SCALE, (player.y + PLAYER_HEIGHT - mazePosition.y) / MAZE_SCALE), WHITE))
                {
                    if ((player.x - mazePosition.x) / MAZE_SCALE > 0 && (player.y - mazePosition.y) / MAZE_SCALE > 0)
                    {
                        player.x -= PLAYER_SPEED;
                    }
                }
            }
            else if (IsKeyDown(KEY_RIGHT) || IsKeyDown(KEY_D))
            {
                if (!ColorIsEqual(GetImageColor(imMaze, ((player.x + PLAYER_WIDTH + PLAYER_SPEED) - mazePosition.x) / MAZE_SCALE, (player.y - mazePosition.y) / MAZE_SCALE), WHITE) && !ColorIsEqual(GetImageColor(imMaze, ((player.x + PLAYER_WIDTH + PLAYER_SPEED) - mazePosition.x) / MAZE_SCALE, (player.y + PLAYER_HEIGHT - mazePosition.y) / MAZE_SCALE), WHITE))
                {
                    if ((player.x - mazePosition.x) / MAZE_SCALE < MAZE_WIDTH && (player.y - mazePosition.y) / MAZE_SCALE < MAZE_HEIGHT)
                    {
                        player.x += PLAYER_SPEED;
                    }
                }
            }

            // TODO: [1p] Camera 2D system following player movement around the map
            // Update Camera2D parameters as required to follow player and zoom control
            camera2d.target = (Vector2){ player.x + 2, player.y + 2 };

            // TODO: [2p] Maze items pickup logic
            if (ColorIsEqual(GetImageColor(imMaze, ((player.x + PLAYER_WIDTH / 2) - mazePosition.x) / MAZE_SCALE, ((player.y + PLAYER_HEIGHT / 2) - mazePosition.y) / MAZE_SCALE), RED))
            {
                Point playerPos = {
                    (int)(((player.x + PLAYER_WIDTH / 2) - mazePosition.x)/MAZE_SCALE),
                    (int)(((player.y + PLAYER_HEIGHT / 2) - mazePosition.y)/MAZE_SCALE),
                };
                
                for (int i = 0; i < MAX_MAZE_ITEMS; i++)
                {
                    if (playerPos.x > mazeItems[i].x && playerPos.x < mazeItems[i].x + MAZE_WIDTH && playerPos.y > mazeItems[i].y && playerPos.y < mazeItems[i].y + MAZE_HEIGHT)
                    {
                        mazeItems[i] = (Point){0};
                        countItems--;
                        score += 50;
                        
                        ImageDrawPixel(&imMaze, playerPos.x, playerPos.y, BLACK);
                        UnloadTexture(texMaze);
                        texMaze = LoadTextureFromImage(imMaze);
                        break;
                    }
                }
            }
            
            
            if (ColorIsEqual(GetImageColor(imMaze, ((player.x + PLAYER_WIDTH / 2) - mazePosition.x) / MAZE_SCALE, ((player.y + PLAYER_HEIGHT / 2) - mazePosition.y) / MAZE_SCALE), GREEN))
            {
                CloseWindow();
            }
        }
        else if (currentMode == 1) // Editor mode
        {
            // TODO: [2p] Maze editor mode, edit image pixels with mouse.
            // Implement logic to selecte image cell from mouse position -> TIP: GetMousePosition()
            // NOTE: Mouse position is returned in screen coordinates and it has to 
            // transformed into image coordinates
            // Once the cell is selected, if mouse button pressed add/remove image pixels
            
            // WARNING: Remember that when imMaze changes, texMaze must be also updated!
            
            if (IsKeyDown(KEY_LEFT_CONTROL) && IsMouseButtonDown(MOUSE_BUTTON_RIGHT))
            {
                Vector2 mousePos = GetMousePosition();
                            
                if ((mousePos.x >= mazePosition.x) && (mousePos.y >= mazePosition.y))
                {
                    Point mapCoord = {
                        (int)((mousePos.x - mazePosition.x)/MAZE_SCALE),
                        (int)((mousePos.y - mazePosition.y)/MAZE_SCALE),
                    };
                    
                    if (ColorIsEqual(GetImageColor(imMaze, mapCoord.x, mapCoord.y), RED))
                    {
                        for (int i = 0; i < MAX_MAZE_ITEMS; i++)
                        {
                            if (mazeItems[i].x == mapCoord.x && mazeItems[i].y == mapCoord.y)
                            {
                                mazeItems[i] = (Point){0};
                                countItems--;
                            }
                        }
                    }
                    
                    ImageDrawPixel(&imMaze, mapCoord.x, mapCoord.y, GREEN);
                    
                    UnloadTexture(texMaze);
                    texMaze = LoadTextureFromImage(imMaze);
                }
            }
            else if (IsMouseButtonDown(MOUSE_BUTTON_LEFT))
            {
                Vector2 mousePos = GetMousePosition();
                
                if ((mousePos.x >= mazePosition.x) && (mousePos.y >= mazePosition.y))
                {
                    Point mapCoord = {
                        (int)((mousePos.x - mazePosition.x)/MAZE_SCALE),
                        (int)((mousePos.y - mazePosition.y)/MAZE_SCALE),
                    };
                    
                    if (ColorIsEqual(GetImageColor(imMaze, mapCoord.x, mapCoord.y), RED))
                    {
                        for (int i = 0; i < MAX_MAZE_ITEMS; i++)
                        {
                            if (mazeItems[i].x == mapCoord.x && mazeItems[i].y == mapCoord.y)
                            {
                                mazeItems[i] = (Point){0};
                                countItems--;
                            }
                        }
                    }
                    
                    ImageDrawPixel(&imMaze, mapCoord.x, mapCoord.y, BLACK);
                    
                    UnloadTexture(texMaze);
                    texMaze = LoadTextureFromImage(imMaze);
                }
            }
            else if (IsMouseButtonDown(MOUSE_BUTTON_RIGHT))
            {
               Vector2 mousePos = GetMousePosition();
                
                if ((mousePos.x >= mazePosition.x) && (mousePos.y >= mazePosition.y))
                {
                    Point mapCoord = {
                        (int)((mousePos.x - mazePosition.x)/MAZE_SCALE),
                        (int)((mousePos.y - mazePosition.y)/MAZE_SCALE),
                    };
                    
                    if (ColorIsEqual(GetImageColor(imMaze, mapCoord.x, mapCoord.y), RED))
                    {
                        for (int i = 0; i < MAX_MAZE_ITEMS; i++)
                        {
                            if (mazeItems[i].x == mapCoord.x && mazeItems[i].y == mapCoord.y)
                            {
                                mazeItems[i] = (Point){0};
                                countItems--;
                            }
                        }
                    }
                    
                    ImageDrawPixel(&imMaze, mapCoord.x, mapCoord.y, WHITE);
                    
                    UnloadTexture(texMaze);
                    texMaze = LoadTextureFromImage(imMaze);
                }
            }
            else if (IsMouseButtonDown(MOUSE_BUTTON_MIDDLE))
            {
                if (countItems < MAX_MAZE_ITEMS)
                {
                    Vector2 mousePos = GetMousePosition();
                
                    if ((mousePos.x >= mazePosition.x) && (mousePos.y >= mazePosition.y))
                    {
                        Point mapCoord = {
                            (int)((mousePos.x - mazePosition.x)/MAZE_SCALE),
                            (int)((mousePos.y - mazePosition.y)/MAZE_SCALE),
                        };
                        
                        bool repeatPosItem = 0;
                        
                        for (int i = 0; i < MAX_MAZE_ITEMS; i++)
                        {
                            if (mazeItems[i].x == mapCoord.x && mazeItems[i].y == mapCoord.y)
                            {
                                repeatPosItem = 1;
                                break;
                            }
                        }
                        
                        if (!repeatPosItem)
                        {
                            mazeItems[countItems] = mapCoord;
                            mazeItemPicked[countItems] = 0;
                            countItems++;
                        }
                        
                        ImageDrawPixel(&imMaze, mapCoord.x, mapCoord.y, RED);
                        
                        UnloadTexture(texMaze);
                        texMaze = LoadTextureFromImage(imMaze);
                    }
                }
            }

            // TODO: [2p] Collectible map items: player score
            // Using same mechanism than maze editor, implement an items editor, registering
            // points in the map where items should be added for player pickup -> TIP: Use mazeItems[]
        }

        // TODO: [1p] Multiple maze biomes supported
        // Implement changing between the different textures to be used as biomes
        // NOTE: For the 3d model, the current selected texture must be applied to the model material  

        if (IsKeyDown(KEY_ONE))
        {
            currentBiome = 0;
        }
        else if (IsKeyDown(KEY_TWO))
        {
            currentBiome = 1;
        }
        else if (IsKeyDown(KEY_THREE))
        {
            currentBiome = 2;
        }
        else if (IsKeyDown(KEY_FOUR))
        {
            currentBiome = 3;
        }
        //----------------------------------------------------------------------------------

        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();

            ClearBackground(RAYWHITE);

            if (currentMode == 0) // Game mode
            {
                // Draw maze using camera2d (for automatic positioning and scale)
                BeginMode2D(camera2d);
                
                // TODO: Draw maze walls and floor using current texture biome 
                for (int y = 0; y < imMaze.height; y++)
                {
                    for (int x = 0; x < imMaze.width; x++)
                    {
                        if (ColorIsEqual(GetImageColor(imMaze, x, y), WHITE))
                        {
                            DrawTexturePro(texBiomes[currentBiome], (Rectangle){ 0, texBiomes[currentBiome].height/2, texBiomes[currentBiome].width/2, texBiomes[currentBiome].height/2 },
                                (Rectangle){ mazePosition.x + x*MAZE_SCALE, mazePosition.y + y*MAZE_SCALE, MAZE_SCALE, MAZE_SCALE }, 
                                (Vector2){ 0 }, 0.0f, WHITE);
                        }
                        else if (ColorIsEqual(GetImageColor(imMaze, x, y), BLACK))
                        {
                            DrawTexturePro(texBiomes[currentBiome], (Rectangle){ texBiomes[currentBiome].width/2, texBiomes[currentBiome].height/2, texBiomes[currentBiome].width/2, texBiomes[currentBiome].height/2 },
                                (Rectangle){ mazePosition.x + x*MAZE_SCALE, mazePosition.y + y*MAZE_SCALE, MAZE_SCALE, MAZE_SCALE }, 
                                (Vector2){ 0 }, 0.0f, WHITE);
                        }
                        else if (ColorIsEqual(GetImageColor(imMaze, x, y), RED))
                        {
                            DrawTexturePro(texBiomes[currentBiome], (Rectangle){ 0, 0, texBiomes[currentBiome].width/2, texBiomes[currentBiome].height/2 },
                                (Rectangle){ mazePosition.x + x*MAZE_SCALE, mazePosition.y + y*MAZE_SCALE, MAZE_SCALE, MAZE_SCALE }, 
                                (Vector2){ 0 }, 0.0f, WHITE);
                        }
                    }
                }
             
                // TODO: Draw player rectangle or sprite at player position
                DrawRectangleRec(player, BLUE);

                // TODO: Draw maze items 2d (using sprite texture?)

                EndMode2D();

                // TODO: Draw game UI (score, time...) using custom sprites/fonts
                
                // NOTE: Game UI does not receive the camera2d transformations,
                // it is drawn in screen space coordinates directly
            }
            else if (currentMode == 1) // Editor mode
            {
                // Draw generated maze texture, scaled and centered on screen 
                DrawTextureEx(texMaze, mazePosition, 0.0f, MAZE_SCALE, WHITE);

                // Draw lines rectangle over texture, scaled and centered on screen 
                DrawRectangleLines(mazePosition.x, mazePosition.y, MAZE_WIDTH*MAZE_SCALE, MAZE_HEIGHT*MAZE_SCALE, RED);

                // TODO: Draw player using a rectangle, consider maze screen coordinates!
                DrawRectangleRec(player, BLUE);

                // TODO: Draw editor UI required elements
            }

            DrawText(TextFormat("SCORE: %i", score), 10, 50, 32, YELLOW);
            DrawText("Change Bioma", 10, 90, 32, BLUE);
            DrawText("[1, 2, 3, 4]", 10, 120, 32, BLUE);
            
            DrawFPS(10, 10);

        EndDrawing();
        //----------------------------------------------------------------------------------
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------
    UnloadTexture(texMaze);     // Unload maze texture from VRAM (GPU)
    UnloadImage(imMaze);        // Unload maze image from RAM (CPU)

    // TODO: Unload all loaded resources
    
    CloseWindow();              // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}

// Generate procedural maze image, using grid-based algorithm
// NOTE: Color scheme used: WHITE = Wall, BLACK = Walkable, RED = Item
Image GenImageMaze(int width, int height, int spacingRows, int spacingCols, float pointChance)
{
    Image imMaze = { 0 };
    
    // TODO: [1p] Implement maze image generation algorithm
    imMaze = GenImageColor(width, height, BLACK);

    // STEP 1.2: Draw image border
    for (int y = 0; y < imMaze.height; y++)
    {
        for (int x = 0; x < imMaze.width; x++)
        {
            if ((x == 0) || (x == (imMaze.width - 1)) ||
                (y == 0) || (y == (imMaze.height - 1))) 
            {
                ImageDrawPixel(&imMaze, x, y, WHITE);
            }
        }
    }
    //---------------------------------------------------------------------------------
    
    // STEP 2: Set some random point in image at specific row-column distances
    //---------------------------------------------------------------------------------
    // STEP 2.1: Define an array of point used for maze generation
    // NOTE A: Static array allocation, memory allocated in STACK (MAX: 1MB)
    // NOTE B: Dynamic array allocation, memory allocated in HEAP (MAX: Available RAM)
    //Point mazePoints[64] = { 0 }; // Static array
    Point *mazePoints = malloc(256*sizeof(Point)); // Dynamic array (MAX: 256 points)
    int mazePointCounter = 0;
    
    // STEP 2.2: Store specific points, at specific row-column distances
    for (int y = 0; y < imMaze.height; y++)
    {
        for (int x = 0; x < imMaze.width; x++)
        {
            if ((x != 0) && (x != (imMaze.width - 1)) &&
                (y != 0) && (y != (imMaze.height - 1)))
            {
                if ((x%spacingCols == 0) && (y%spacingRows == 0))
                {
                    if (GetRandomValue(0, 100) <= (int)(pointChance*100)) 
                    {
                        mazePoints[mazePointCounter] = (Point){ x, y };
                        mazePointCounter++;
                    }
                }
            }
        }
    }

    // STEP 2.3: Draw our points in image
    for (int i = 0; i < mazePointCounter; i++) 
    {
        ImageDrawPixel(&imMaze, mazePoints[i].x, mazePoints[i].y, WHITE);
    }
    //---------------------------------------------------------------------------------

    // STEP 3: Draw lines from every point in a random direction
    //---------------------------------------------------------------------------------
    // STEP 3.1: Define an array of 4 directions for convenience
    Point directions[4] = {
        { 1, 0 },   // East
        { -1, 0 },  // West
        { 0, 1 },   // South
        { 0, -1 },  // North
    };

    // STEP 3.2: Load a random sequence of points, to be used as indices, so,
    // we can access maze-points randomly indexed, instead of following the order we stored them
    int *pointOrder = LoadRandomSequence(mazePointCounter, 0, mazePointCounter - 1);

    // STEP 3.3: Process every random maze point, drawing pixels in one random direction,
    // until we collision with another wall (WHITE pixel)
    for (int i = 0; i < mazePointCounter; i++) 
    {
        Point currentPoint = mazePoints[pointOrder[i]];
        Point currentDir = directions[GetRandomValue(0, 3)];
        Point nextPoint = { currentPoint.x + currentDir.x, currentPoint.y + currentDir.y };
        
        while(ColorIsEqual(GetImageColor(imMaze, nextPoint.x, nextPoint.y), BLACK))
        {
            ImageDrawPixel(&imMaze, nextPoint.x, nextPoint.y, WHITE);
            
            nextPoint.x += currentDir.x;
            nextPoint.y += currentDir.y;
        }
    }
    
    // If using dynamic memory allocation for mazePoints,
    // they allocated memory must be manually freed 
    free(mazePoints);
    
    // Free pointOrder array, allocated internally by LoadRandomSequence()
    UnloadRandomSequence(pointOrder);
    
    return imMaze;
}