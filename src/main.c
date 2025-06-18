/*******************************************************************************************
*
*   raylib [models] example - loading gltf with animations
*
*   Example complexity rating: [★☆☆☆] 1/4
*
*   LIMITATIONS:
*     - Only supports 1 armature per file, and skips loading it if there are multiple armatures
*     - Only supports linear interpolation (default method in Blender when checked
*       "Always Sample Animations" when exporting a GLTF file)
*     - Only supports translation/rotation/scale animation channel.path,
*       weights not considered (i.e. morph targets)
*
*   Example originally created with raylib 3.7, last time updated with raylib 4.2
*
*   Example licensed under an unmodified zlib/libpng license, which is an OSI-certified,
*   BSD-like license that allows static linking with closed source software
*
*   Copyright (c) 2020-2025 Ramon Santamaria (@raysan5)
*
********************************************************************************************/

#include "raylib.h"

#define RAYGUI_IMPLEMENTATION
#include "raygui.h"


//------------------------------------------------------------------------------------
// Program main entry point
//------------------------------------------------------------------------------------
int main(void)
{
    // Initialization
    //--------------------------------------------------------------------------------------
    const int screenWidth = 640;
    const int screenHeight = 480;
//		SetConfigFlags(FLAG_WINDOW_UNDECORATED);
    InitWindow(screenWidth, screenHeight, "Dill's World");

    // Define the camera to look into our 3d world
		float cameraX = 4.0f;
		float cameraY = 4.0f;
		float cameraZ = 14.0f;
    Camera camera = { 0 };
    camera.position = (Vector3){ 4.0f, 4.0f, 14.0f };    // Camera position
    camera.target = (Vector3){ 6.0f, 2.0f, -17.0f };      // Camera looking at point
    camera.up = (Vector3){ 0.0f, 1.0f, 0.0f };          // Camera up vector (rotation towards target)
    camera.fovy = 45.0f;                                // Camera field-of-view Y
    camera.projection = CAMERA_PERSPECTIVE;             // Camera projection type

    // Load gltf model

    Model model = LoadModel("resources/ac_alien_green_additional_animations.gltf");
    Vector3 position = { 0.0f, 0.0f, 0.0f }; // Set model position		

		Vector3 scenePosition = { 0.0f, -1.0f, 1.0f };	
    // Load gltf model animations
    int animsCount = 0;
    unsigned int animIndex = 0;
    unsigned int animCurrentFrame = 0;
    ModelAnimation *modelAnimations = LoadModelAnimations("resources/ac_alien_green_additional_animations.gltf", &animsCount);

		Vector3 billboardPosition = { 0.0f, 0.0f, 0.0f };
		Texture2D charmanderTexture = LoadTexture("resources/charmander-ok.png");
		Texture2D tex = LoadTexture("resources/pink.png");

    SetTargetFPS(60);                   // Set our game to run at 60 frames-per-second
    //-------------------------------------------------------------------------------------


		Model x = LoadModel("resources/scene/pedestal_in_heaven_scene.gltf");
		int _animsCount = 0;
    unsigned int _animIndex = 0;
    unsigned int _animCurrentFrame = 0;
    ModelAnimation *_modelAnimations = LoadModelAnimations("resources/scene/pedestal_in_heaven_scene.gltf", &_animsCount);

		TraceLog(LOG_INFO, TextFormat("_animsCount: %i", _animsCount));

    // Main game loop
    while (!WindowShouldClose())        // Detect window close button or ESC key
    {
        // Update
        //--------------------------------------------------------------------------------
//        UpdateCamera(&camera, CAMERA_THIRD_PERSON);

        // Select current animation
//        if (IsMouseButtonPressed(MOUSE_BUTTON_RIGHT)) animIndex = (animIndex + 1)%animsCount;
//        else if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) animIndex = (animIndex + animsCount - 1)%animsCount;
        // Update model animation
        ModelAnimation anim = modelAnimations[animIndex];
        animCurrentFrame = (animCurrentFrame + 1)%anim.frameCount;
        UpdateModelAnimation(model, anim, animCurrentFrame);

				ModelAnimation _anim = _modelAnimations[_animIndex];
        _animCurrentFrame = (_animCurrentFrame + 1)%_anim.frameCount;
        UpdateModelAnimation(x, _anim, _animCurrentFrame);


        BeginDrawing();

						ClearBackground(SKYBLUE);
						
          BeginMode3D(camera);
                DrawModel(model, position, 0.3f, WHITE);    
								DrawModel(x, scenePosition, 1.0f, WHITE);

								if (IsKeyDown(KEY_ONE)) {
									billboardPosition = position;
									billboardPosition.x += 2;
									billboardPosition.y -= 1;

									DrawBillboard(camera, charmanderTexture, billboardPosition, 3.0f, WHITE);
								}
            EndMode3D();

								if (IsKeyDown(KEY_J)) camera.position.x -= 0.1;
								if (IsKeyDown(KEY_SEMICOLON)) camera.position.x += 0.1;
								if (IsKeyDown(KEY_L)) camera.position.y += 0.1;
								if (IsKeyDown(KEY_K)) camera.position.y -= 0.1;
								
								if (IsKeyDown(KEY_I)) camera.position.z -= 0.1;
								if (IsKeyDown(KEY_O)) camera.position.z += 0.1;

								if (IsKeyDown(KEY_A)) camera.target.x -= 0.1;
								if (IsKeyDown(KEY_S)) camera.target.x += 0.1;
								if (IsKeyDown(KEY_D)) camera.target.y += 0.1;
								if (IsKeyDown(KEY_F)) camera.target.y -= 0.1;
								

            DrawText("Use the LEFT/RIGHT mouse buttons to switch animation", 10, 10, 20, GRAY);
            DrawText(TextFormat("Animation: %s", anim.name), 10, GetScreenHeight() - 20, 10, DARKGRAY);
//					GuiUnlock();
//					GuiSetState(STATE_NORMAL);
//						GuiGroupBox((Rectangle){ 10, 400, 165, 400 }, "STANDARD");
//            GuiSlider((Rectangle){ 35, 400, 100, 16 }, TextFormat("%0.2f", cameraX), NULL, &cameraX, -20.0f, 20.0f);
//            GuiSlider((Rectangle){ 96, 48, 200, 16 }, TextFormat("%0.2f", cameraY), NULL, &cameraY, -20.0f, 20.0f);
//            GuiSlider((Rectangle){ 96, 48, 200, 16 }, TextFormat("%0.2f", cameraZ), NULL, &cameraZ, -20.0f, 20.0f);

  
        EndDrawing();
        //----------------------------------------------------------------------------------
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------
    UnloadModel(model);         // Unload model and meshes/material

    CloseWindow();              // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}
