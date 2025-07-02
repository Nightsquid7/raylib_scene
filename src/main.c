#include "raylib.h"

#define RAYGUI_IMPLEMENTATION
#include "raygui.h"

Vector3 difference(Vector3 from, Vector3 to, float seconds, int framerate) {
	if (from.x == to.x && from.y == from.y && from.z == from.z) {
		TraceLog(LOG_INFO, TextFormat("identity"));
		return (Vector3){ 0, 0, 0 };
	}
	float duration = seconds * (float)framerate;
	float x = (to.x - from.x) / duration;	
	float y = (to.y - from.y) / duration;	
	float z = (to.z - from.z) / duration;	
	TraceLog(LOG_INFO, TextFormat("diff x %.4f, y %.4f, z %.4f", x, y, z));
	return (Vector3){ x, y, z }; 
}

typedef struct {
	Vector3 position;
	Vector3 target;
} CameraPosition;

typedef enum {
	PLAY_THEN_STOP = 0,
	REPEAT,
	PLAY_THEN_REVERSE		
} AnimationMode;

typedef struct {
	float duration;
	int index;

	AnimationMode mode;
	CameraPosition positions[];
} Animation;

int main(void)
{
		int framerate = 60;
    const int screenWidth = 850;
    const int screenHeight = 480;
//		SetConfigFlags(FLAG_WINDOW_UNDECORATED);
    InitWindow(screenWidth, screenHeight, "Dill's World");

		Vector3 cameraPoint_1 = (Vector3){ -7.0f, 2.1f, 10.2f };
		Vector3 cameraTarget_1 = (Vector3){ 5.9f, 4.0f, -1.2f };

		Vector3 cameraPoint_2 = (Vector3){ 7.4f, 3.4f, 10.0f };
		Vector3 cameraTarget_2 = (Vector3){ 5.7f, 3.21f, -2.5f };
		
		Vector3 cameraPoint_3 = (Vector3){ 5.3f, 0.7f, 8.0f };
		Vector3 cameraTarget_3 = (Vector3){ -5.7f, -3.21f, 2.5f };


		Vector3 diff = difference(cameraPoint_1, cameraPoint_2, 5, framerate);
		Vector3 diffTarget = difference(cameraTarget_1, cameraTarget_2, 5, framerate);

		int duration = 300;	
		int cameraAnimationCurrentFrame = 0;


		Animation currentAnimation = { .duration = 14, .index = 0, .mode = PLAY_THEN_STOP,  };
 		currentAnimation.positions[0] = (CameraPosition){ .position = cameraPoint_1, .target = cameraTarget_1 };
 		currentAnimation.positions[1] = (CameraPosition){ .position = cameraPoint_2, .target = cameraTarget_1 };
 		currentAnimation.positions[2] = (CameraPosition){ .position = cameraPoint_3, .target = cameraTarget_2 };

		int caCount = 2;
   

		Camera camera = { 0 };
    camera.position = cameraPoint_1; 
    camera.target = cameraTarget_1; 
    camera.up = (Vector3){ 0.0f, 1.0f, 0.0f };          // Camera up vector (rotation towards target)
    camera.fovy = 45.0f;                                // Camera field-of-view Y
    camera.projection = CAMERA_PERSPECTIVE;             // Camera projection type

		char alienPath[] = "resources/alien/ac_alien_green_meeting_buddy.gltf";
    Model model = LoadModel(alienPath);
    Vector3 position = { 0.0f, 0.0f, -1.0f }; // Set model position		

		Vector3 scenePosition = { 0.0f, -1.0f, 1.0f };	
    // Load gltf model animations
    int animsCount = 0;
    unsigned int animIndex = 1;
    unsigned int animCurrentFrame = 1;
    ModelAnimation *modelAnimations = LoadModelAnimations(alienPath, &animsCount);

		Vector3 billboardPosition = { 0.0f, 0.0f, 0.0f };
		Texture2D charmanderTexture = LoadTexture("resources/charmander-ok.png");

    SetTargetFPS(framerate);

		Model x = LoadModel("resources/scene/pedestal_in_heaven_scene.gltf");
		int _animsCount = 0;
    unsigned int _animIndex = 0;
    unsigned int _animCurrentFrame = 0;
    ModelAnimation *_modelAnimations = LoadModelAnimations("resources/scene/pedestal_in_heaven_scene.gltf", &_animsCount);
	
		bool isAnimating = false;
		

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


			if (isAnimating) {
			// animation	
			if (cameraAnimationCurrentFrame == 0) {

				int fromIndex = currentAnimation.index % caCount;
				int toIndex = (currentAnimation.index + 1) % caCount;

				CameraPosition from = currentAnimation.positions[fromIndex];
				CameraPosition to = currentAnimation.positions[toIndex];
			
				TraceLog(LOG_INFO, TextFormat("-- fromIndex %d,  toIndex %d", fromIndex, toIndex));	

				TraceLog(LOG_INFO, TextFormat("-- from x: %.2f y %.2f z %.2f""", from.position.x, from.position.y, from.position.z));
				TraceLog(LOG_INFO, TextFormat("-- fromTx: %.2f y %.2f z %.2f""", from.target.x, from.target.y, from.target.z));

				TraceLog(LOG_INFO, TextFormat("-- to   x: %.2f y %.2f z %.2f""", to.position.x, to.position.y, to.position.z));
				TraceLog(LOG_INFO, TextFormat("-- to  Tx: %.2f y %.2f z %.2f""", to.target.x, to.target.y, to.target.z));


				diff = difference(from.position, to.position, currentAnimation.duration, framerate);
				diffTarget = difference(from.target, to.target, currentAnimation.duration, framerate);
			}
			
			cameraAnimationCurrentFrame++;
			camera.position.x += diff.x;
			camera.position.y += diff.y;								
			camera.position.z += diff.z;								
			camera.target.x += diffTarget.x;
			camera.target.y += diffTarget.y;
			camera.target.z += diffTarget.z;
			
			if ((float)cameraAnimationCurrentFrame >= currentAnimation.duration * (float)framerate) {

				currentAnimation.index++;	
				cameraAnimationCurrentFrame = 0;
			}
		}

			if (IsKeyPressed(KEY_ONE)) isAnimating = !isAnimating; 
	

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


			BeginDrawing();

				ClearBackground(SKYBLUE);
				
			BeginMode3D(camera);
						DrawModel(model, position, 0.3f, WHITE);    
						DrawModel(x, scenePosition, 1.0f, WHITE);


				EndMode3D();


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
