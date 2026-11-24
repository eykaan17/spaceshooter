#include "raylib.h"
//#include <fstream>
//#include <cstdlib>
//#include <ctime>
//#include <cmath>
//using namespace std;
//// CONSTANTS & DIMENSIONS
//const int SCREEN_WIDTH = 1000;
//const int SCREEN_HEIGHT = 700;
//const int ARENA_WIDTH = 680;
//const int ARENA_HEIGHT = SCREEN_HEIGHT;
//
//const int MAX_PROJECTILES = 60;
//const int MAX_HAZARDS = 25;
//const int MAX_DROPS = 10;
//const int MAX_STARS = 60;
//const int LEADERBOARD_CAPACITY = 5;
//
//// ==========================================
//// GAME DATA STRUCTURES
//// ==========================================
//enum GameState {
//    STATE_MENU,
//    STATE_PLAYING,
//    STATE_INSTRUCTIONS,
//    STATE_LEADERBOARD,
//    STATE_GAMEOVER,
//    STATE_VICTORY
//};
//
//struct ShipEntity {
//    Rectangle rect;
//    float speed;
//    int shields;      // 0 to 100%
//    bool spreadShot;
//    int spreadTimer;
//    int empCharges;
//};
//
//struct Projectile {
//    Rectangle rect;
//    float speed;
//    bool active;
//};
//
//struct Hazard {
//    Rectangle rect;
//    int type;        // 1: Asteroid, 2: Raider, 3: Boss Mothership
//    int hp;
//    int maxHp;
//    int points;
//    float speedX;
//    float speedY;
//    bool active;
//};
//
//struct SalvageDrop {
//    Rectangle rect;
//    int type;        // 1: Shield Kit, 2: Spread Shot Battery
//    float speedY;
//    bool active;
//};
//
//struct Star {
//    Vector2 pos;
//    float speed;
//    float size;
//};
//
//struct ScoreEntry {
//    char initials[4];
//    int score;
//};
//
//// ==========================================
//// GLOBAL STATE
//// ==========================================
//GameState currentState = STATE_MENU;
//ShipEntity starship;
//Projectile lasers[MAX_PROJECTILES];
//Hazard hazards[MAX_HAZARDS];
//SalvageDrop drops[MAX_DROPS];
//Star stars[MAX_STARS];
//ScoreEntry leaderboard[LEADERBOARD_CAPACITY];
//
//int currentWave = 1;
//int scoreTotal = 0;
//bool isPaused = false;
//
//// Alert / Diagnostic Banner
//char diagMsg[64] = "";
//int diagTimer = 0;
//Color diagColor = GREEN;
//
//// User Initials Input Buffer
//char inputInitials[4] = "\0";
//int letterCount = 0;
//
//// Audio Sounds
//Sound sndLaser;
//Sound sndExplosion;
//Sound sndEmp;
//Sound sndPowerup;
//
//// ==========================================
//// PROCEDURAL AUDIO SYNTHESIZERS
//// ==========================================
//Sound CreateCustomTone(float freq, float duration, float volume, bool isNoise) {
//    int sampleRate = 44100;
//    int sampleCount = (int)(sampleRate * duration);
//    short* data = (short*)malloc(sampleCount * sizeof(short));
//
//    for (int i = 0; i < sampleCount; ++i) {
//        float t = (float)i / (float)sampleRate;
//        float decay = 1.0f - (t / duration);
//        if (isNoise) {
//            float r = ((float)rand() / (float)RAND_MAX) * 2.0f - 1.0f;
//            data[i] = (short)(r * 32000.0f * volume * decay);
//        }
//        else {
//            float val = sinf(2.0f * PI * freq * t);
//            data[i] = (short)(val * 32000.0f * volume * decay);
//        }
//    }
//
//    Wave wave = { 0 };
//    wave.frameCount = sampleCount;
//    wave.sampleRate = sampleRate;
//    wave.sampleSize = 16;
//    wave.channels = 1;
//    wave.data = data;
//
//    Sound snd = LoadSoundFromWave(wave);
//    UnloadWave(wave); // Frees the raw buffer memory
//    return snd;
//}
//
//void InitGameAudio() {
//    InitAudioDevice();
//    sndLaser = CreateCustomTone(880.0f, 0.08f, 0.35f, false);
//    sndExplosion = CreateCustomTone(120.0f, 0.25f, 0.5f, true);
//    sndEmp = CreateCustomTone(220.0f, 0.6f, 0.6f, false);
//    sndPowerup = CreateCustomTone(1200.0f, 0.18f, 0.4f, false);
//}
//
//void CloseGameAudio() {
//    UnloadSound(sndLaser);
//    UnloadSound(sndExplosion);
//    UnloadSound(sndEmp);
//    UnloadSound(sndPowerup);
//    CloseAudioDevice();
//}
//
//// ==========================================
//// FILE I/O LEADERBOARD
//// ==========================================
//void LoadLeaderboard() {
//    for (int i = 0; i < LEADERBOARD_CAPACITY; ++i) {
//        leaderboard[i].initials[0] = 'N';
//        leaderboard[i].initials[1] = '/';
//        leaderboard[i].initials[2] = 'A';
//        leaderboard[i].initials[3] = '\0';
//        leaderboard[i].score = 0;
//    }
//
//    ifstream fin("leaderboard.txt");
//    if (fin) {
//        for (int i = 0; i < LEADERBOARD_CAPACITY; ++i) {
//            if (!(fin >> leaderboard[i].initials >> leaderboard[i].score)) {
//                break;
//            }
//        }
//        fin.close();
//    }
//}
//
//void SaveLeaderboard() {
//    ofstream fout("leaderboard.txt");
//    if (fout) {
//        for (int i = 0; i < LEADERBOARD_CAPACITY; ++i) {
//            fout << leaderboard[i].initials << " " << leaderboard[i].score << "\n";
//        }
//        fout.close();
//    }
//}
//
//void InsertScore(int finalScore, const char init[4]) {
//    int idx = -1;
//    for (int i = 0; i < LEADERBOARD_CAPACITY; ++i) {
//        if (finalScore > leaderboard[i].score) {
//            idx = i;
//            break;
//        }
//    }
//
//    if (idx != -1) {
//        for (int i = LEADERBOARD_CAPACITY - 1; i > idx; --i) {
//            leaderboard[i] = leaderboard[i - 1];
//        }
//        leaderboard[idx].score = finalScore;
//        leaderboard[idx].initials[0] = init[0];
//        leaderboard[idx].initials[1] = init[1];
//        leaderboard[idx].initials[2] = init[2];
//        leaderboard[idx].initials[3] = '\0';
//        SaveLeaderboard();
//    }
//}
//
//// ==========================================
//// ARENA & ENTITY INITIALIZATION
//// ==========================================
//void InitStars() {
//    for (int i = 0; i < MAX_STARS; ++i) {
//        stars[i].pos = { (float)(rand() % ARENA_WIDTH), (float)(rand() % SCREEN_HEIGHT) };
//        stars[i].speed = 1.0f + (float)(rand() % 4);
//        stars[i].size = 1.0f + (float)(rand() % 3);
//    }
//}
//
//void InitWave(int wave) {
//    currentWave = wave;
//    starship.rect = { (float)(ARENA_WIDTH / 2 - 25), (float)(SCREEN_HEIGHT - 70), 50.0f, 40.0f };
//    starship.speed = 420.0f;
//    starship.spreadShot = false;
//    starship.spreadTimer = 0;
//
//    if (wave == 1) {
//        starship.shields = 100;
//        starship.empCharges = 1;
//        scoreTotal = 0;
//    }
//    else {
//        if (starship.empCharges < 2) starship.empCharges++;
//    }
//
//    for (int i = 0; i < MAX_PROJECTILES; ++i) lasers[i].active = false;
//    for (int i = 0; i < MAX_HAZARDS; ++i) hazards[i].active = false;
//    for (int i = 0; i < MAX_DROPS; ++i) drops[i].active = false;
//
//    // Spawn Boss on Wave 3
//    if (wave == 3) {
//        hazards[0].active = true;
//        hazards[0].rect = { (float)(ARENA_WIDTH / 2 - 120), 40.0f, 240.0f, 70.0f };
//        hazards[0].type = 3;
//        hazards[0].hp = 50;
//        hazards[0].maxHp = 50;
//        hazards[0].points = 500;
//        hazards[0].speedX = 140.0f;
//        hazards[0].speedY = 0.0f;
//    }
//}
//
//// ==========================================
//// SPAWNING ACTIONS
//// ==========================================
//void FireLaser() {
//    PlaySound(sndLaser);
//    if (!starship.spreadShot) {
//        for (int i = 0; i < MAX_PROJECTILES; ++i) {
//            if (!lasers[i].active) {
//                lasers[i].active = true;
//                lasers[i].rect = { starship.rect.x + starship.rect.width / 2.0f - 3.0f, starship.rect.y - 12.0f, 6.0f, 16.0f };
//                lasers[i].speed = 650.0f;
//                break;
//            }
//        }
//    }
//    else {
//        int spawned = 0;
//        for (int i = 0; i < MAX_PROJECTILES && spawned < 2; ++i) {
//            if (!lasers[i].active) {
//                lasers[i].active = true;
//                float offX = (spawned == 0) ? starship.rect.x + 6.0f : starship.rect.x + starship.rect.width - 12.0f;
//                lasers[i].rect = { offX, starship.rect.y - 12.0f, 6.0f, 16.0f };
//                lasers[i].speed = 650.0f;
//                spawned++;
//            }
//        }
//    }
//}
//
//void TriggerEMP() {
//    if (starship.empCharges <= 0) return;
//
//    starship.empCharges--;
//    PlaySound(sndEmp);
//
//    for (int i = 0; i < MAX_HAZARDS; ++i) {
//        if (hazards[i].active && hazards[i].type != 3) {
//            hazards[i].active = false;
//            scoreTotal += hazards[i].points / 2;
//        }
//    }
//
//    strcpy_s(diagMsg, "TACTICAL EMP DISCHARGED!");
//    diagColor = SKYBLUE;
//    diagTimer = 90;
//}
//
//void SpawnSalvage(float x, float y) {
//    if (rand() % 100 < 35) { // 35% drop rate
//        for (int i = 0; i < MAX_DROPS; ++i) {
//            if (!drops[i].active) {
//                drops[i].active = true;
//                drops[i].rect = { x, y, 24.0f, 24.0f };
//                drops[i].type = (rand() % 2 == 0) ? 1 : 2;
//                drops[i].speedY = 120.0f;
//                break;
//            }
//        }
//    }
//}
//
//void SpawnHazards() {
//    if (currentWave == 3) {
//        // Boss fight: occasional debris
//        if (rand() % 100 < 4) {
//            for (int i = 1; i < MAX_HAZARDS; ++i) {
//                if (!hazards[i].active) {
//                    hazards[i].active = true;
//                    hazards[i].rect = { (float)(20 + rand() % (ARENA_WIDTH - 60)), 120.0f, 32.0f, 32.0f };
//                    hazards[i].type = 1;
//                    hazards[i].hp = 1;
//                    hazards[i].maxHp = 1;
//                    hazards[i].points = 15;
//                    hazards[i].speedX = 0;
//                    hazards[i].speedY = 180.0f;
//                    break;
//                }
//            }
//        }
//        return;
//    }
//
//    int rate = (currentWave == 1) ? 3 : 5;
//    if (rand() % 100 < rate) {
//        for (int i = 0; i < MAX_HAZARDS; ++i) {
//            if (!hazards[i].active) {
//                hazards[i].active = true;
//                float rx = (float)(20 + rand() % (ARENA_WIDTH - 70));
//
//                if (rand() % 100 < 55) {
//                    hazards[i].rect = { rx, -35.0f, 36.0f, 36.0f };
//                    hazards[i].type = 1; // Asteroid
//                    hazards[i].hp = 1;
//                    hazards[i].maxHp = 1;
//                    hazards[i].points = 20;
//                    hazards[i].speedX = 0;
//                    hazards[i].speedY = 150.0f;
//                }
//                else {
//                    hazards[i].rect = { rx, -40.0f, 44.0f, 36.0f };
//                    hazards[i].type = 2; // Raider
//                    hazards[i].hp = 2;
//                    hazards[i].maxHp = 2;
//                    hazards[i].points = 40;
//                    hazards[i].speedX = 0;
//                    hazards[i].speedY = 190.0f;
//                }
//                break;
//            }
//        }
//    }
//}
//
//// ==========================================
//// UPDATE LOGIC
//// ==========================================
//void UpdatePlayingSession(float dt) {
//    if (IsKeyPressed(KEY_P)) {
//        isPaused = !isPaused;
//    }
//
//    if (isPaused) return;
//
//    if (IsKeyPressed(KEY_Q) || IsKeyPressed(KEY_ESCAPE)) {
//        currentState = STATE_MENU;
//        return;
//    }
//
//    // Input Movement
//    if (IsKeyDown(KEY_LEFT) || IsKeyDown(KEY_A)) {
//        starship.rect.x -= starship.speed * dt;
//        if (starship.rect.x < 10) starship.rect.x = 10;
//    }
//    if (IsKeyDown(KEY_RIGHT) || IsKeyDown(KEY_D)) {
//        starship.rect.x += starship.speed * dt;
//        if (starship.rect.x + starship.rect.width > ARENA_WIDTH - 10) {
//            starship.rect.x = ARENA_WIDTH - 10 - starship.rect.width;
//        }
//    }
//
//    if (IsKeyPressed(KEY_SPACE)) FireLaser();
//    if (IsKeyPressed(KEY_B)) TriggerEMP();
//
//    // Spread Shot Expiration
//    if (starship.spreadShot) {
//        starship.spreadTimer--;
//        if (starship.spreadTimer <= 0) starship.spreadShot = false;
//    }
//
//    // Background Stars
//    for (int i = 0; i < MAX_STARS; ++i) {
//        stars[i].pos.y += stars[i].speed;
//        if (stars[i].pos.y > SCREEN_HEIGHT) {
//            stars[i].pos.y = 0;
//            stars[i].pos.x = (float)(rand() % ARENA_WIDTH);
//        }
//    }
//
//    // Update Lasers
//    for (int i = 0; i < MAX_PROJECTILES; ++i) {
//        if (lasers[i].active) {
//            lasers[i].rect.y -= lasers[i].speed * dt;
//            if (lasers[i].rect.y < -20) lasers[i].active = false;
//        }
//    }
//
//    // Update Salvage Drops
//    for (int i = 0; i < MAX_DROPS; ++i) {
//        if (drops[i].active) {
//            drops[i].rect.y += drops[i].speedY * dt;
//            if (drops[i].rect.y > SCREEN_HEIGHT) drops[i].active = false;
//
//            if (CheckCollisionRecs(starship.rect, drops[i].rect)) {
//                drops[i].active = false;
//                PlaySound(sndPowerup);
//                if (drops[i].type == 1) {
//                    starship.shields = min(100, starship.shields + 25);
//                    strcpy_s(diagMsg, "SHIELD CHARGED (+25%)");
//                }
//                else {
//                    starship.spreadShot = true;
//                    starship.spreadTimer = 300; // ~5 seconds
//                    strcpy_s(diagMsg, "SPREAD LASER ENGAGED!");
//                }
//                diagColor = GREEN;
//                diagTimer = 75;
//            }
//        }
//    }
//
//    // Spawning Hazards
//    SpawnHazards();
//
//    // Update Hazards & Collisions
//    for (int i = 0; i < MAX_HAZARDS; ++i) {
//        if (!hazards[i].active) continue;
//
//        if (hazards[i].type == 3) {
//            // Boss lateral motion
//            hazards[i].rect.x += hazards[i].speedX * dt;
//            if (hazards[i].rect.x <= 15 || hazards[i].rect.x + hazards[i].rect.width >= ARENA_WIDTH - 15) {
//                hazards[i].speedX *= -1.0f;
//            }
//        }
//        else {
//            hazards[i].rect.y += hazards[i].speedY * dt;
//            if (hazards[i].rect.y > SCREEN_HEIGHT) {
//                hazards[i].active = false;
//                starship.shields = max(0, starship.shields - 5); // Base breach penalty
//            }
//        }
//
//        // Hull Collision with Player
//        if (CheckCollisionRecs(starship.rect, hazards[i].rect)) {
//            PlaySound(sndExplosion);
//            if (hazards[i].type != 3) hazards[i].active = false;
//            starship.shields = max(0, starship.shields - 25);
//            strcpy_s(diagMsg, "CRITICAL HULL IMPACT!");
//            diagColor = RED;
//            diagTimer = 75;
//        }
//
//        // Projectile Collisions
//        for (int p = 0; p < MAX_PROJECTILES; ++p) {
//            if (lasers[p].active && CheckCollisionRecs(lasers[p].rect, hazards[i].rect)) {
//                lasers[p].active = false;
//                hazards[i].hp--;
//
//                if (hazards[i].hp <= 0) {
//                    hazards[i].active = false;
//                    scoreTotal += hazards[i].points;
//                    PlaySound(sndExplosion);
//
//                    if (hazards[i].type != 3) {
//                        SpawnSalvage(hazards[i].rect.x, hazards[i].rect.y);
//                    }
//                }
//                break;
//            }
//        }
//    }
//
//    if (diagTimer > 0) diagTimer--;
//
//    // Victory / Defeat Evaluations
//    if (starship.shields <= 0) {
//        letterCount = 0;
//        inputInitials[0] = '\0';
//        currentState = STATE_GAMEOVER;
//    }
//
//    if (currentWave == 1 && scoreTotal >= 350) {
//        InitWave(2);
//    }
//    else if (currentWave == 2 && scoreTotal >= 800) {
//        InitWave(3);
//    }
//    else if (currentWave == 3 && !hazards[0].active) {
//        letterCount = 0;
//        inputInitials[0] = '\0';
//        currentState = STATE_VICTORY;
//    }
//}
//
//// ==========================================
//// RENDERING FUNCTIONS
//// ==========================================
//void DrawTacticalHUD() {
//    int hX = ARENA_WIDTH + 20;
//
//    DrawRectangle(ARENA_WIDTH, 0, SCREEN_WIDTH - ARENA_WIDTH, SCREEN_HEIGHT, GetColor(0x11161dff));
//    DrawLineEx({ (float)ARENA_WIDTH, 0 }, { (float)ARENA_WIDTH, (float)SCREEN_HEIGHT }, 3.0f, DARKGREEN);
//
//    DrawText("COSMIC DEFENDER", hX, 25, 22, GREEN);
//    DrawText("DEFENSE PROTOCOL", hX, 50, 14, GRAY);
//    DrawLine(hX, 75, SCREEN_WIDTH - 20, 75, DARKGRAY);
//
//    // Wave status
//    if (currentWave < 3) {
//        DrawText(TextFormat("SECTOR WAVE: %i / 3", currentWave), hX, 90, 18, WHITE);
//    }
//    else {
//        DrawText("ALERT: MOTHERSHIP BOSS", hX, 90, 18, RED);
//    }
//
//    DrawText(TextFormat("TOTAL SCORE: %i PTS", scoreTotal), hX, 125, 18, YELLOW);
//
//    // Shield Integrity Bar
//    DrawText("SHIELD HULL:", hX, 160, 15, LIGHTGRAY);
//    DrawRectangle(hX, 182, 220, 20, DARKGRAY);
//    Color sColor = (starship.shields > 50) ? GREEN : (starship.shields > 25 ? ORANGE : RED);
//    DrawRectangle(hX, 182, (int)(2.2f * starship.shields), 20, sColor);
//    DrawRectangleLines(hX, 182, 220, 20, WHITE);
//    DrawText(TextFormat("%i%%", starship.shields), hX + 230, 184, 15, WHITE);
//
//    // Weapon Readout
//    DrawText("PRIMARY BATTERY:", hX, 220, 15, LIGHTGRAY);
//    if (starship.spreadShot) {
//        DrawText("TWIN SPREAD LASER", hX, 240, 16, SKYBLUE);
//    }
//    else {
//        DrawText("STANDARD BEAM", hX, 240, 16, WHITE);
//    }
//
//    // EMP Readout
//    DrawText("EMP CHARGES:", hX, 280, 15, LIGHTGRAY);
//    for (int i = 0; i < 2; ++i) {
//        if (i < starship.empCharges) {
//            DrawRectangle(hX + (i * 35), 305, 26, 26, BLUE);
//            DrawRectangleLines(hX + (i * 35), 305, 26, 26, SKYBLUE);
//            DrawText("B", hX + (i * 35) + 8, 309, 16, WHITE);
//        }
//        else {
//            DrawRectangleLines(hX + (i * 35), 305, 26, 26, DARKGRAY);
//        }
//    }
//
//    // Legend & Controls
//    DrawLine(hX, 355, SCREEN_WIDTH - 20, 355, DARKGRAY);
//    DrawText("CONTROLS:", hX, 370, 15, LIGHTGRAY);
//    DrawText("A / D or <- -> : Maneuver", hX, 395, 14, GRAY);
//    DrawText("SPACE          : Fire Lasers", hX, 420, 14, GRAY);
//    DrawText("B              : EMP Wipe", hX, 445, 14, GRAY);
//    DrawText("P              : Pause Sector", hX, 470, 14, GRAY);
//    DrawText("Q / ESC        : Abort Mission", hX, 495, 14, GRAY);
//
//    DrawText("SALVAGE DROPS:", hX, 540, 15, LIGHTGRAY);
//    DrawRectangle(hX, 565, 18, 18, GREEN);
//    DrawText("S", hX + 4, 567, 14, BLACK);
//    DrawText("Shield Recharge (+25%)", hX + 26, 567, 13, LIGHTGRAY);
//
//    DrawRectangle(hX, 595, 18, 18, SKYBLUE);
//    DrawText("W", hX + 3, 597, 14, BLACK);
//    DrawText("Spread Shot Upgrade", hX + 26, 597, 13, LIGHTGRAY);
//}
//
//void DrawPlayingSession() {
//    // Starfield Background
//    for (int i = 0; i < MAX_STARS; ++i) {
//        DrawCircle((int)stars[i].pos.x, (int)stars[i].pos.y, stars[i].size, Fade(WHITE, 0.7f));
//    }
//
//    // Lasers
//    for (int i = 0; i < MAX_PROJECTILES; ++i) {
//        if (lasers[i].active) {
//            DrawRectangleRec(lasers[i].rect, (starship.spreadShot ? SKYBLUE : YELLOW));
//        }
//    }
//
//    // Drops
//    for (int i = 0; i < MAX_DROPS; ++i) {
//        if (drops[i].active) {
//            Color dCol = (drops[i].type == 1) ? GREEN : SKYBLUE;
//            DrawRectangleRec(drops[i].rect, dCol);
//            DrawRectangleLinesEx(drops[i].rect, 1.5f, WHITE);
//            DrawText((drops[i].type == 1 ? "S" : "W"), (int)drops[i].rect.x + 6, (int)drops[i].rect.y + 4, 16, BLACK);
//        }
//    }
//
//    // Hazards
//    for (int i = 0; i < MAX_HAZARDS; ++i) {
//        if (!hazards[i].active) continue;
//
//        if (hazards[i].type == 1) { // Asteroid
//            DrawPoly({ hazards[i].rect.x + 18, hazards[i].rect.y + 18 }, 6, 18, 0, ORANGE);
//            DrawPolyLines({ hazards[i].rect.x + 18, hazards[i].rect.y + 18 }, 6, 18, 0, WHITE);
//        }
//        else if (hazards[i].type == 2) { // Raider
//            DrawTriangle(
//                { hazards[i].rect.x + hazards[i].rect.width / 2.0f, hazards[i].rect.y + hazards[i].rect.height },
//                { hazards[i].rect.x, hazards[i].rect.y },
//                { hazards[i].rect.x + hazards[i].rect.width, hazards[i].rect.y },
//                MAGENTA
//            );
//        }
//        else if (hazards[i].type == 3) { // Mothership Boss
//            DrawRectangleRec(hazards[i].rect, RED);
//            DrawRectangleLinesEx(hazards[i].rect, 3.0f, MAROON);
//            DrawRectangle((int)hazards[i].rect.x + 15, (int)hazards[i].rect.y + 15, 210, 10, DARKGRAY);
//            DrawRectangle((int)hazards[i].rect.x + 15, (int)hazards[i].rect.y + 15, (int)(4.2f * hazards[i].hp), 10, GREEN);
//            DrawText("MOTHERSHIP FLAGSHIP", (int)hazards[i].rect.x + 35, (int)hazards[i].rect.y + 35, 16, WHITE);
//        }
//    }
//
//    // Starship Player
//    Vector2 p1 = { starship.rect.x + starship.rect.width / 2.0f, starship.rect.y };
//    Vector2 p2 = { starship.rect.x, starship.rect.y + starship.rect.height };
//    Vector2 p3 = { starship.rect.x + starship.rect.width, starship.rect.y + starship.rect.height };
//    DrawTriangle(p1, p2, p3, SKYBLUE );
//    DrawTriangleLines(p1, p2, p3, WHITE);
//
//    // Diagnostics Alert Banner
//    if (diagTimer > 0) {
//        DrawRectangle(100, 20, ARENA_WIDTH - 200, 35, Fade(BLACK, 0.7f));
//        DrawRectangleLines(100, 20, ARENA_WIDTH - 200, 35, diagColor);
//        int tw = MeasureText(diagMsg, 16);
//        DrawText(diagMsg, ARENA_WIDTH / 2 - tw / 2, 28, 16, diagColor);
//    }
//
//    // Tactical HUD
//    DrawTacticalHUD();
//
//    // Pause Screen Overlay
//    if (isPaused) {
//        DrawRectangle(0, 0, ARENA_WIDTH, SCREEN_HEIGHT, Fade(BLACK, 0.65f));
//        DrawText("TACTICAL FREEZE", ARENA_WIDTH / 2 - 120, SCREEN_HEIGHT / 2 - 30, 28, YELLOW);
//        DrawText("Press 'P' to resume defense session", ARENA_WIDTH / 2 - 145, SCREEN_HEIGHT / 2 + 10, 16, LIGHTGRAY);
//    }
//}
//
//// ==========================================
//// SUB-SCREENS (MENUS & LEADERBOARD)
//// ==========================================
//void DrawMainMenu() {
//    DrawText("COSMIC DEFENDER", SCREEN_WIDTH / 2 - MeasureText("COSMIC DEFENDER", 40) / 2, 100, 40, GREEN);
//    DrawText("Orbital Defense System - Fall 2026", SCREEN_WIDTH / 2 - MeasureText("Orbital Defense System - Fall 2026", 16) / 2, 155, 16, GRAY);
//
//    int mY = 240;
//    DrawRectangle(SCREEN_WIDTH / 2 - 180, mY, 360, 50, DARKBLUE);
//    DrawRectangleLines(SCREEN_WIDTH / 2 - 180, mY, 360, 50, SKYBLUE);
//    DrawText("[1] Launch Mission", SCREEN_WIDTH / 2 - MeasureText("[1] Launch Mission", 20) / 2, mY + 15, 20, WHITE);
//
//    mY += 75;
//    DrawRectangle(SCREEN_WIDTH / 2 - 180, mY, 360, 50, DARKBLUE);
//    DrawRectangleLines(SCREEN_WIDTH / 2 - 180, mY, 360, 50, SKYBLUE);
//    DrawText("[2] Tactical Briefing", SCREEN_WIDTH / 2 - MeasureText("[2] Tactical Briefing", 20) / 2, mY + 15, 20, WHITE);
//
//    mY += 75;
//    DrawRectangle(SCREEN_WIDTH / 2 - 180, mY, 360, 50, DARKBLUE);
//    DrawRectangleLines(SCREEN_WIDTH / 2 - 180, mY, 360, 50, SKYBLUE);
//    DrawText("[3] Hall of Fame", SCREEN_WIDTH / 2 - MeasureText("[3] Hall of Fame", 20) / 2, mY + 15, 20, WHITE);
//
//    mY += 75;
//    DrawRectangle(SCREEN_WIDTH / 2 - 180, mY, 360, 50, DARKBLUE);
//    DrawRectangleLines(SCREEN_WIDTH / 2 - 180, mY, 360, 50, SKYBLUE);
//    DrawText("[4] Power Down (Exit)", SCREEN_WIDTH / 2 - MeasureText("[4] Power Down (Exit)", 20) / 2, mY + 15, 20, WHITE);
//
//    if (IsKeyPressed(KEY_ONE)) {
//        InitWave(1);
//        currentState = STATE_PLAYING;
//    }
//    else if (IsKeyPressed(KEY_TWO)) {
//        currentState = STATE_INSTRUCTIONS;
//    }
//    else if (IsKeyPressed(KEY_THREE)) {
//        currentState = STATE_LEADERBOARD;
//    }
//    else if (IsKeyPressed(KEY_FOUR)) {
//        CloseWindow();
//        exit(0);
//    }
//}
//
//void DrawInstructions() {
//    DrawText("TACTICAL BRIEFING", SCREEN_WIDTH / 2 - MeasureText("TACTICAL BRIEFING", 32) / 2, 70, 32, GREEN);
//
//    int textY = 150;
//    DrawText("MISSION OUTLINE:", 200, textY, 20, YELLOW);
//    textY += 35;
//    DrawText("- Sector 1 & 2: Repel asteroid fields and raider craft.", 220, textY, 17, LIGHTGRAY);
//    textY += 30;
//    DrawText("- Sector 3: Eliminate the incoming Mothership flagship.", 220, textY, 17, LIGHTGRAY);
//
//    textY += 50;
//    DrawText("EQUIPMENT & SALVAGE:", 200, textY, 20, YELLOW);
//    textY += 35;
//    DrawText("- [S] Shield Canister: Restores 25% hull integrity.", 220, textY, 17, LIGHTGRAY);
//    textY += 30;
//    DrawText("- [W] Spread Battery: Equips dual-laser spread fire.", 220, textY, 17, LIGHTGRAY);
//    textY += 30;
//    DrawText("- [B] EMP Bomb: Screen-clearing pulse (1 charge per wave).", 220, textY, 17, LIGHTGRAY);
//
//    DrawText("Press [BACKSPACE] or [ESC] to return to terminal", SCREEN_WIDTH / 2 - 180, 580, 16, GRAY);
//    if (IsKeyPressed(KEY_BACKSPACE) || IsKeyPressed(KEY_ESCAPE)) currentState = STATE_MENU;
//}
//
//void DrawLeaderboard() {
//    DrawText("TOP 5 DEFENDERS", SCREEN_WIDTH / 2 - MeasureText("TOP 5 DEFENDERS", 32) / 2, 80, 32, GREEN);
//    DrawLine(200, 130, 800, 130, DARKGRAY);
//
//    DrawText("RANK", 250, 150, 20, LIGHTGRAY);
//    DrawText("PILOT", 470, 150, 20, LIGHTGRAY);
//    DrawText("SCORE", 680, 150, 20, LIGHTGRAY);
//
//    for (int i = 0; i < LEADERBOARD_CAPACITY; ++i) {
//        DrawText(TextFormat("#%i", i + 1), 260, 200 + i * 45, 18, YELLOW);
//        DrawText(leaderboard[i].initials, 480, 200 + i * 45, 18, WHITE);
//        DrawText(TextFormat("%i PTS", leaderboard[i].score), 680, 200 + i * 45, 18, GREEN);
//    }
//
//    DrawText("Press [BACKSPACE] or [ESC] to return to terminal", SCREEN_WIDTH / 2 - 180, 550, 16, GRAY);
//    if (IsKeyPressed(KEY_BACKSPACE) || IsKeyPressed(KEY_ESCAPE)) currentState = STATE_MENU;
//}
//
//void HandleEndGameScreen(bool isVictory) {
//    DrawText(isVictory ? "MISSION ACCOMPLISHED!" : "DEFENSE GRID COLLAPSED!",
//        SCREEN_WIDTH / 2 - 220, 120, 32, isVictory ? GREEN : RED);
//    DrawText(TextFormat("FINAL SCORE: %i PTS", scoreTotal), SCREEN_WIDTH / 2 - 100, 180, 22, YELLOW);
//
//    DrawText("Enter 3-Letter Pilot Initials:", SCREEN_WIDTH / 2 - 130, 260, 18, LIGHTGRAY);
//
//    // Initial input reader
//    int key = GetCharPressed();
//    while (key > 0) {
//        if ((key >= 'a' && key <= 'z') || (key >= 'A' && key <= 'Z')) {
//            if (letterCount < 3) {
//                inputInitials[letterCount] = (char)toupper(key);
//                letterCount++;
//                inputInitials[letterCount] = '\0';
//            }
//        }
//        key = GetCharPressed();
//    }
//
//    if (IsKeyPressed(KEY_BACKSPACE) && letterCount > 0) {
//        letterCount--;
//        inputInitials[letterCount] = '\0';
//    }
//
//    DrawRectangle(SCREEN_WIDTH / 2 - 60, 305, 120, 45, DARKGRAY);
//    DrawRectangleLines(SCREEN_WIDTH / 2 - 60, 305, 120, 45, WHITE);
//    DrawText(inputInitials, SCREEN_WIDTH / 2 - 25, 316, 24, WHITE);
//
//    DrawText("Press [ENTER] to Record Record", SCREEN_WIDTH / 2 - 120, 380, 16, GRAY);
//
//    if (IsKeyPressed(KEY_ENTER) && letterCount > 0) {
//        InsertScore(scoreTotal, inputInitials);
//        currentState = STATE_LEADERBOARD;
//    }
//}
//
//// ==========================================
//// MAIN FUNCTION
//// ==========================================
//int main() {
//    srand((unsigned)time(0));
//    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Cosmic Defender - Space Shooter");
//    SetTargetFPS(60);
//
//    InitGameAudio();
//    InitStars();
//    LoadLeaderboard();
//
//    while (!WindowShouldClose()) {
//        float dt = GetFrameTime();
//
//        // State Machine Updates
//        if (currentState == STATE_PLAYING) {
//            UpdatePlayingSession(dt);
//        }
//
//        // State Machine Drawing
//        BeginDrawing();
//        ClearBackground(GetColor(0x0a0c10ff));
//
//        switch (currentState) {
//        case STATE_MENU:
//            DrawMainMenu();
//            break;
//        case STATE_PLAYING:
//            DrawPlayingSession();
//            break;
//        case STATE_INSTRUCTIONS:
//            DrawInstructions();
//            break;
//        case STATE_LEADERBOARD:
//            DrawLeaderboard();
//            break;
//        case STATE_GAMEOVER:
//            HandleEndGameScreen(false);
//            break;
//        case STATE_VICTORY:
//            HandleEndGameScreen(true);
//            break;
//        }
//
//        EndDrawing();
//    }
//
//    CloseGameAudio();
//    CloseWindow();
//    return 0;
//}