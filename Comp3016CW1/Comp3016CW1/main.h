#pragma once
#include <SDL.h>
#include <SDL_ttf.h>
#include <list>
#include <functional>
#include <string>
#include <iostream>
#include <vector>

class Game;
class MenuState;
class WorldMapState;
class MazeState;
class Button;
class Player;
class Enemy;
class Resource;

struct Vector2 {
	float x;
	float y;
};

class Texture {
public:
	void load(std::string filePath, SDL_Renderer* renderer);
	void render();
	void free();
};
class House {
public:
	std::string filePath;
	SDL_Texture* iconTexture;
	bool isCompleted;

};

class Button {
public:
	void init(SDL_Renderer* renderer, Game* game, const std::string& text, int x, int y, int w, int h);
	void handleEvents(SDL_Event& e);
	void render(SDL_Renderer* renderer);
	void setOnClick(std::function<void()> callback);
	SDL_Texture* textTexture;

private:
	SDL_Rect rect;
	
	SDL_Color textColor = { 255,255,255,255 };
	std::function<void()> onClick;
	bool isHovered = false;
};
class GameObject {
public:
	void Update();
	void render();
	bool checkCollision(SDL_Rect& nextPos, SDL_Rect& wall);

private:
	SDL_Texture* texture;
};
class GameState {
public:
	virtual void handleEvents();
	virtual void update();
	virtual void render();
	Game* game;
	virtual std::string getName() const { return "GameState"; }
};
class MenuState : public GameState {
public:
	void handleEvents();
	void update();
	void render();
	void init(SDL_Renderer* renderer, TTF_Font* font);
	~MenuState() {
		if (startButton) delete startButton;
		if (endButton) delete endButton;
		if (titleTexture) SDL_DestroyTexture(titleTexture);
	}
	SDL_Texture* titleTexture = nullptr;
	int titleW = 0, titleH = 0;
	std::string getName() const override { return "MenuState"; }
private:
	Button* startButton;
	Button* endButton;
};
class WorldMapState :public GameState {
public:
	void init(SDL_Renderer* renderer, TTF_Font* font);
	void onEnter();
	void handleEvents();
	void update();
	void render();

	std::list<Button> houseButtons;
	SDL_Texture* titleTexture = nullptr;
	int topTextW = 500, topTextH = 75;
	std::vector<House> houses;
	SDL_Texture* scoreText = nullptr;
	SDL_Texture* healthText = nullptr;
private:
	
	int selecetedHouseIndex;
	Vector2 cursorPosition;
};
class MazeState: public GameState {
public:
	void init(SDL_Renderer* renderer, TTF_Font* font);
	void loadHouse();
	void handleEvents();
	void update();
	void render();
	void buildWalls();
	void MakeEnemies();
	void MakeResources();
	void OnEnter();

	std::vector<std::vector<int>> mazeGrid;
	std::vector<Enemy*> enemies;
	std::vector<Resource*> resources;
	int rows = 12, cols = 18;
	const int tileSize = 64;

	Player* player;

	SDL_Rect exitRect;
	bool showExitMessage = false;
	SDL_Texture* exitText = nullptr;

	SDL_Texture* scoreText = nullptr;
	SDL_Texture* healthText = nullptr;
	SDL_Texture* FloorTexture = nullptr;
	SDL_Texture* WallTexture = nullptr;
	SDL_Texture* PlayerStartTexture = nullptr;
	SDL_Texture* EnemyStartTexture = nullptr;
	SDL_Texture* ResourceTexture = nullptr;
	SDL_Texture* EndTexture = nullptr;
	Game* game;

};

class WinLossState :public GameState {
public:
	void init(SDL_Renderer* renderer, TTF_Font* font);
	void render();
	void handleEvents();
	void update();
	Game* game;
	bool win = false;
	SDL_Texture* titleTexture = nullptr;
	SDL_Texture* scoreTexture = nullptr;
	

private:
	Button* restartButton = nullptr;
};
class Enemy:public GameObject {
public:
	void Update();
	Enemy(int Dir, SDL_Rect Collider, MazeState* CurrentMaze) {
		this->dir = Dir;
		std::cout << dir << std::endl;
		this->collider = Collider;
		this->currentMaze = CurrentMaze;
		if (currentMaze == nullptr) {
			std::cout << "No Maze set" << std::endl;
		}
	}
	void tryMove();
	Uint32 lastTime;
	SDL_Rect collider;
	
private:
	MazeState* currentMaze;
	int speed = 150;
	int dir;
	
};

class Player: public GameObject {
public:
	void handleInput(const Uint8* keys);
	void update() {};
	void tryMove();
	void ExitMaze();
	
	float velX = 0, velY = 0;
	SDL_Rect playerRect = { 100, 100, 40, 40 };
	Uint32 lastTime;
	std::vector<SDL_Rect> walls;
	MazeState* currentMaze;
	

private:
	
	int health = 3;
	int score = 0;
	float speed = 200;
};

class Resource :public GameObject {
public: 
	Resource(SDL_Rect Collider) {
		this->collider = Collider;
	}
	void Update();
	SDL_Rect collider;
	bool collected = false;

	
};
class Map {
public:
	void loadFromFile(std::string file);
	void draw();
private:
	//grid[rows][columns]
	//walltexture
	//floortexture
	//Vector2 playerStart
	//Vector2 exitpos
};

class Game {
public:
	Game();
	void init();
	void run();
	void update();
	void render();
	void handleEvents();
	bool IsRunning() const;
	void quit();
	void changeState(GameState* newState);
	void addResource() { resources += 1; }
	void takeLife() { 
		playerLives -= 1;
	}
	void resetHealth() {
		playerLives = 3;
	}
	SDL_Renderer* getRenderer();
	TTF_Font* getFont();
	WorldMapState* GetWorldMapState();
	MazeState* GetMazeState();
	MenuState* GetMenuState() {
		return menuState;
	}
	WinLossState* GetWinLossState() {
		return winLossState;
	}
	
	int GetResources() {
		return resources;
	}
	int GetHealth() {
		return playerLives;
	}

	const int SCREEN_WIDTH = 1600;
	const int SCREEN_HEIGHT = 900;
	const int FONT_SIZE = 84;
	std::vector<std::string> mazeFileNames;
private:
	
	
	SDL_Window* window;
	SDL_Renderer* renderer;
	TTF_Font* font;
	bool isRunning;
	bool gameOver;
	SDL_Color textColor = { 255,0,0,128 };
	Map map;
	int resources = 0;
	int playerLives = 3;
	GameState* currentState;
	MenuState* menuState;
	WorldMapState* worldMapState;
	MazeState* mazeState;
	WinLossState* winLossState;
	int currentHouseIndex;
};


