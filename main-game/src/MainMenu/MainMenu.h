#pragma once
#include "../GameState.h"
#include "../Game.h"

class MainMenu : public GameState {
   private:
    bool isNewGame = true;
    double appStartTime;  // Declaration for app start time
    Texture2D start_img;
    Font font;
    Font titlefont;
    Texture2D treeTexture;
    Texture2D foxTexture;
    Rectangle frameRec;
    Rectangle destRec;
    Rectangle destRec2;
    Rectangle foxFrameRec;
    Vector2 foxPosition;
    int foxFrameWidth;
    int foxFrameHeight;
    bool foxReachedCenter;  // Declaration for fox reached center
    int framesCounter;      // Declaration for frames counter
    float bounceHeight;
    float bounceSpeed;
    float bounceTime;
    int frameWidth;
    int frameHeight;
    float scaleFactor;
    float foxSpeed;
    Rectangle exitButton;
    const char* exitButtonText;
    float titlePosY;

    // Newly added member variables for positioning and text
    Vector2 treePosition;
    Vector2 treePosition2;
    // Variables for dynamic text and positions in render method
    std::string projectText;
    std::string foxText;
    std::string startText;

  public:
    int screenWidth = 1920 / 2;
    int screenHeight = 1080 / 2;
    MainMenu();
    ~MainMenu() override; // destructor

    void processInput(Game& game) override;
    void update(Game& game) override;
    void render(Game& game) override;
};


