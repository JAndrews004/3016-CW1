// Comp3016CW1.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include "main.h"

using namespace std;

Game::Game() : isRunning(true), gameOver(false), window(nullptr), renderer(nullptr), font(nullptr) {};
void Game::init() {
	
		if (SDL_Init(SDL_INIT_VIDEO) < 0) {
			cout << "SDL could not initialize! SDL_ERROR: " << SDL_GetError() << endl;
			isRunning = false;
			return;
		}
		if (TTF_Init() == -1) {
			cout << "SDL_ttf could not initialize! TTF_ERROR: " << TTF_GetError() << endl;
			isRunning = false;
			return;
		}
		window = SDL_CreateWindow("Survive", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
		if (window == nullptr) {
			cout << "Window cannot be created! SDL_ERROR: " << SDL_GetError() << endl;
			isRunning = false;
			return;
		}
		renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
		if (renderer == nullptr) {
			cout << "Renderer cannot be created! SDL_ERROR: " << SDL_GetError() << endl;
			isRunning = false;
			return;
		}
		font = TTF_OpenFont("ARCADECLASSIC.TTF", FONT_SIZE);
		if (font == nullptr) {
			cout << "Font cannot be loaded! TTF_ERROR: " << TTF_GetError() << endl;
			isRunning = false;
			return;
		}
		//load house list from file
		menuState = new MenuState();
		menuState->game = this;
		menuState->init(renderer, font);
		worldMapState = new WorldMapState();
		worldMapState->game = this;
		worldMapState->init(renderer, font);
		mazeState = new MazeState();
		mazeState->game = this;
		//mazeState->init(renderer, font);
		currentState = menuState;
		
}

void Game::run() {
	while (IsRunning()) {
		
		//cout << "Running frame..." << endl;
		handleEvents();
		update();
		render();
		SDL_Delay(16);
		
	}
}
void Game::update(){
	currentState->update();
}
void Game::render(){
	//std::cout << "Game rendering\n";
	currentState->render();
}
void Game::handleEvents(){
	//std::cout << "Handling events for state: " << currentState->getName() << "\n";
	currentState->handleEvents();
}
bool Game::IsRunning() const {
	return isRunning;
}
void Game::quit() {
	isRunning = false;
	TTF_CloseFont(font);
	TTF_Quit();
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
}
void Game::changeState(GameState* newState) {
	currentState = newState;
}
TTF_Font* Game::getFont() {
	return font;
}
SDL_Renderer* Game::getRenderer() {
	return renderer;
}
WorldMapState* Game::GetWorldMapState() {
	return worldMapState;
}
void MenuState::init(SDL_Renderer* renderer, TTF_Font* font) {
	//std::cout << "Initialising buttons\n";
	startButton = new Button();
	endButton = new Button();

	startButton->init(renderer,game, "Start", game->SCREEN_WIDTH/2 - 150, 400, 300, 120);
	endButton->init(renderer,game, "Quit", game->SCREEN_WIDTH / 2 - 150,600 , 300, 120);

	// Connect behavior
	startButton->setOnClick([this]() {
		// Example: change state
		std::cout << "Start clicked!\n";
		game->changeState(game->GetWorldMapState());
		});

	endButton->setOnClick([this]() {
		std::cout << "Quit clicked!\n";
		game->quit();
		});

	SDL_Color white = { 255, 255, 255, 255 };
	SDL_Surface* titleSurface = TTF_RenderText_Solid(game->getFont(), "SURVIVE", white);
	titleTexture = SDL_CreateTextureFromSurface(game->getRenderer(), titleSurface);
	SDL_QueryTexture(titleTexture, nullptr, nullptr, &titleW, &titleH);
	SDL_FreeSurface(titleSurface);

}

void MenuState::handleEvents() {
	//cout << "State handling events\n";
	SDL_Event e;
	while (SDL_PollEvent(&e)) {
		if (e.type == SDL_QUIT)
			game->quit();

		startButton->handleEvents(e);
		endButton->handleEvents(e);
	}
}

void MenuState::update() {
	//button effects or animations here
}
void MenuState::render() {
	
	//std::cout << "Rendering\n";
	SDL_SetRenderDrawColor(game->getRenderer(), 0, 0, 0, 255);
	SDL_RenderClear(game->getRenderer());

	if (titleTexture) {
		SDL_Rect titleRect = { (game->SCREEN_WIDTH - titleW) / 2, 100, titleW, titleH };
		SDL_RenderCopy(game->getRenderer(), titleTexture, nullptr, &titleRect);
	}

	if (startButton) startButton->render(game->getRenderer());
	if (endButton) endButton->render(game->getRenderer());

	SDL_RenderPresent(game->getRenderer());

	if (!titleTexture) std::cerr << "Title texture null!\n";
	if (startButton && startButton->textTexture == nullptr) {
		std::cerr << "Start button texture null!\n";
	}
	if (endButton && endButton->textTexture == nullptr)
		std::cerr << "End button texture null!\n";

}


void Button::init(SDL_Renderer* renderer,Game* game, const std::string& text, int x, int y, int w, int h) {
	rect = { x, y, w, h };
	SDL_Surface* surface = TTF_RenderText_Solid(game->getFont(), text.c_str(), textColor);
	textTexture = SDL_CreateTextureFromSurface(renderer, surface);
	SDL_FreeSurface(surface);
}
void Button::handleEvents(SDL_Event& e) {
	int mx = 0, my = 0;
	if (e.type == SDL_MOUSEMOTION) {
		mx = e.motion.x;
		my = e.motion.y;
	}
	else if (e.type == SDL_MOUSEBUTTONDOWN) {
		mx = e.button.x;
		my = e.button.y;
	}
	else {
		return;
	}

	bool inside = (mx > rect.x && mx < rect.x + rect.w &&
		my > rect.y && my < rect.y + rect.h);

	isHovered = inside;

	if (inside && e.type == SDL_MOUSEBUTTONDOWN && e.button.button == SDL_BUTTON_LEFT) {
		if (onClick) onClick();
	}
}

void Button::render(SDL_Renderer* renderer) {
	if (!textTexture) return;  // for Button
	

	SDL_SetRenderDrawColor(renderer, isHovered ? 200 : 100, 100, 100, 255);
	SDL_RenderFillRect(renderer, &rect);

	int tw, th;
	SDL_QueryTexture(textTexture, nullptr, nullptr, &tw, &th);
	SDL_Rect textRect = { rect.x + (rect.w - tw) / 2, rect.y + (rect.h - th) / 2, tw, th };
	SDL_RenderCopy(renderer, textTexture, nullptr, &textRect);
}
void Button::setOnClick(std::function<void()> callback) {
	onClick = callback;
}

void GameState::handleEvents() {

}
void  GameState::update() {

}
void  GameState::render() {

}

void WorldMapState::init(SDL_Renderer* renderer, TTF_Font* font) {
	int buttonSize = 150;
	int gap = 30;
	int gridCols = 3;
	int gridRows = 3;

	int gridWidth = gridCols * buttonSize + (gridCols - 1) * gap;
	int gridHeight = gridRows * buttonSize + (gridRows - 1) * gap;

	int gridX = (game->SCREEN_WIDTH - gridWidth) / 2;
	int gridY = (game->SCREEN_HEIGHT - gridHeight) / 2;

	for (int i = 0; i < 9; i++) {
		House newHouse = House();
		HouseData newHouseData = HouseData();
		newHouseData.name = "House " + to_string(i);
		newHouseData.completed = false;
		newHouseData.filePath = "";
		newHouse.data = newHouseData;
		this->houses.push_front(newHouse);

		Button houseButton = Button();

		

		
		int col = i % 3; // 0, 1, 2
		int row = i / 3; // 0, 1, 2

		int x = gridX + col * (buttonSize + gap);
		int y = gridY + row * (buttonSize + gap);

		houseButton.init(renderer, game, to_string(i + 1), x, y, buttonSize, buttonSize);

		houseButton.setOnClick([this]() 
			{ std::cout << "House clicked\n"; 
			//game.ChangeState(mazeState); 
			});

		houseButtons.push_front(houseButton);
	}

	SDL_Color white = { 255, 255, 255, 255 };
	SDL_Surface* topTextSurface = TTF_RenderText_Solid(font, "Please  select  a  house. ", white);
	titleTexture = SDL_CreateTextureFromSurface(renderer, topTextSurface);
	SDL_QueryTexture(titleTexture, nullptr, nullptr,&topTextW,&topTextH);
	SDL_FreeSurface(topTextSurface);
}
void WorldMapState::handleEvents() {
	SDL_Event e;
	while (SDL_PollEvent(&e)) {
		if (e.type == SDL_QUIT)
			game->quit();

		
		for (auto& button : houseButtons) {
			button.handleEvents(e);
		}
	}
};

void WorldMapState::update() {

};

void WorldMapState::render() {

	SDL_SetRenderDrawColor(game->getRenderer(), 0, 0, 0, 255);
	SDL_RenderClear(game->getRenderer());

	if (titleTexture) {
		SDL_Rect titleRect = { (game->SCREEN_WIDTH - topTextW) / 2, 100, topTextW, topTextH };
		SDL_RenderCopy(game->getRenderer(), titleTexture, nullptr, &titleRect);
	}

	//loop through house buttons to render all of them
	for (auto& button : houseButtons) {
		button.render(game->getRenderer());
	}

	SDL_RenderPresent(game->getRenderer());

	if (!titleTexture) std::cerr << "Title texture null!\n";
	/*
	if (startButton && startButton->textTexture == nullptr) {
		std::cerr << "Start button texture null!\n";
	}
	if (endButton && endButton->textTexture == nullptr)
		std::cerr << "End button texture null!\n";*/
};

int main(int argc, char* argv[])
{
	Game game;
	game.init();
	game.run();
	game.quit();
	return 0;
}
