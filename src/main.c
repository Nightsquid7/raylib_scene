#include "raylib.h"

#define RAYGUI_IMPLEMENTATION
#include "raygui.h"

int main(void)
{
    const int screenWidth = 850;
    const int screenHeight = 480;
		SetConfigFlags(FLAG_WINDOW_UNDECORATED);
    InitWindow(screenWidth, screenHeight, "Dill's World");

		float cameraX = 4.0f;
		float cameraY = 4.0f;
		float cameraZ = 14.0f;
    Camera camera = { 0 };
    camera.position = (Vector3){ -7.0f, 2.1f, 10.2f };    // Camera position
    camera.target = (Vector3){ 5.9f, 4.0f, -1.2f };      // Camera looking at point
    camera.up = (Vector3){ 0.0f, 1.0f, 0.0f };          // Camera up vector (rotation towards target)
    camera.fovy = 45.0f;                                // Camera field-of-view Y
    camera.projection = CAMERA_PERSPECTIVE;             // Camera projection type

    Model model = LoadModel("resources/ac_alien_green_additional_animations.gltf");
    Vector3 position = { 0.0f, 0.0f, -1.0f }; // Set model position		

		Vector3 scenePosition = { 0.0f, -1.0f, 1.0f };	
    // Load gltf model animations
    int animsCount = 0;
    unsigned int animIndex = 0;
    unsigned int animCurrentFrame = 0;
    ModelAnimation *modelAnimations = LoadModelAnimations("resources/ac_alien_green_additional_animations.gltf", &animsCount);

		Vector3 billboardPosition = { 0.0f, 0.0f, 0.0f };
		Texture2D charmanderTexture = LoadTexture("resources/charmander-ok.png");
		Texture2D tex = LoadTexture("resources/pink.png");

    SetTargetFPS(60);


		Model x = LoadModel("resources/scene/pedestal_in_heaven_scene.gltf");
		int _animsCount = 0;
    unsigned int _animIndex = 0;
    unsigned int _animCurrentFrame = 0;
    ModelAnimation *_modelAnimations = LoadModelAnimations("resources/scene/pedestal_in_heaven_scene.gltf", &_animsCount);

		TraceLog(LOG_INFO, TextFormat("_animsCount: %i", _animsCount));

    while (!WindowShouldClose())        
    {


        if (IsMouseButtonPressed(MOUSE_BUTTON_RIGHT)) animIndex = (animIndex + 1)%animsCount;
        else if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) animIndex = (animIndex + animsCount - 1)%animsCount;
        
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
								if (IsKeyDown(KEY_S)) camera.target.y -= 0.1;
								if (IsKeyDown(KEY_D)) camera.target.y += 0.1;
								if (IsKeyDown(KEY_F)) camera.target.x += 0.1;
	
								if (IsKeyDown(KEY_W)) camera.target.z -= 0.1;
								if (IsKeyDown(KEY_F)) camera.target.z += 0.1;
								

            DrawText("Use the LEFT/RIGHT mouse buttons to switch animation", 10, 10, 20, GRAY);
            DrawText(TextFormat("Animation: %s", anim.name), 10, GetScreenHeight() - 20, 10, DARKGRAY);
						DrawText(TextFormat("camera x %.2f", camera.position.x), 10, GetScreenHeight() - 40, 10, DARKGRAY);
						DrawText(TextFormat("y %.2f", camera.position.y), 100, GetScreenHeight() - 40, 10, DARKGRAY);
						DrawText(TextFormat("z %.2f", camera.position.z), 150, GetScreenHeight() - 40, 10, DARKGRAY);

						DrawText(TextFormat("transl x %.2f", camera.target.x), 10, GetScreenHeight() - 50, 10, DARKGRAY);
						DrawText(TextFormat("y %.2f", camera.target.y), 100, GetScreenHeight() - 50, 10, DARKGRAY);
						DrawText(TextFormat("z %.2f", camera.target.z), 150, GetScreenHeight() - 50, 10, DARKGRAY);
  

  
        EndDrawing();
    }

    UnloadModel(model);        

    CloseWindow();              

    return 0;
}
