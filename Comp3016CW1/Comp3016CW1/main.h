#pragma once
#include <SDL.h>
#include <SDL_ttf.h>
#include <list>
#include <functional>
#include <string>
#include <iostream>

class Game;
class MenuState;
class WorldMapState;
class MazeState;
class Button;

struct Vector2 {
	float x;
	float y;
};
struct HouseData {
	std::string name;
	std::string filePath;
	bool completed;
};
class Texture {
public:
	void load(std::string filePath, SDL_Renderer* renderer);
	void render();
	void free();
};
class House {
public:
	void render(SDL_Renderer* renderer);
private:
	HouseData data;
	Vector2 position;
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
private:
	//position(x,y)
	//texture
	//speed
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
	void onEnter();
	void handleEvents() {};
	void update() {};
	void render() {};
private:
	std::list<House> houses;
	int selecetedHouseIndex;
	Vector2 cursorPosition;
};
class MazeState: public GameState {
public:
	void loadHouse();
	void handleEvents() {};
	void update() {};
	void render() {};

};
class Enemy:public GameObject {
public:
	void Update();
private:
	//movementpath
	int currentStep;
};

class Player :GameObject {
public:
	void handleInput();
	void update();
	void tryMove();
private:
	int health = 3;
	int score = 0;
};

class Resource :public GameObject {
public: 
	void Update();
private:
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
	SDL_Renderer* getRenderer();
	TTF_Font* getFont();
	WorldMapState* GetWorldMapState();
	const int SCREEN_WIDTH = 1600;
	const int SCREEN_HEIGHT = 900;
	const int FONT_SIZE = 84;
private:
	
	
	SDL_Window* window;
	SDL_Renderer* renderer;
	TTF_Font* font;
	bool isRunning;
	bool gameOver;
	SDL_Color textColor = { 255,0,0,128 };
	Player player;
	Map map;
	int resources = 2;
	GameState* currentState;
	MenuState* menuState;
	WorldMapState* worldMapState;
	MazeState* mazeState;
	std::list<HouseData> houseList;
	int currentHouseIndex;
};

class TextureManager {
public:
	void Load(std::string path);
	void Draw(); //draw(texture: Texture, position: Vector2) 
};

class InputHandler {
public:
	void keyPressed(std::string key);
	void mouseClicked(std::string button);
};
