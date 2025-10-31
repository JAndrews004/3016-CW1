// Comp3016CW1.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include "main.h"
#include <fstream>
#include <sstream>
#include <random>

using namespace std;

Game::Game() : isRunning(true), gameOver(false), window(nullptr), renderer(nullptr), font(nullptr) {};
void Game::init() {
	
		if (SDL_Init(SDL_INIT_VIDEO) < 0) {
			cerr << "SDL could not initialize! SDL_ERROR: " << SDL_GetError() << endl;
			isRunning = false;
			return;
		}
		if (TTF_Init() == -1) {
			cerr << "SDL_ttf could not initialize! TTF_ERROR: " << TTF_GetError() << endl;
			isRunning = false;
			return;
		}
		window = SDL_CreateWindow("Survive", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
		if (window == nullptr) {
			cerr << "Window cannot be created! SDL_ERROR: " << SDL_GetError() << endl;
			isRunning = false;
			return;
		}
		renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
		if (renderer == nullptr) {
			cerr << "Renderer cannot be created! SDL_ERROR: " << SDL_GetError() << endl;
			isRunning = false;
			return;
		}
		font = TTF_OpenFont("ARCADECLASSIC.TTF", FONT_SIZE);
		if (font == nullptr) {
			cerr << "Font cannot be loaded! TTF_ERROR: " << TTF_GetError() << endl;
			isRunning = false;
			return;
		}

		if (!(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG)) {
			cerr << "SDL_image could not initialize! " << IMG_GetError() << endl;
		}
		SDL_Init(SDL_INIT_AUDIO);

		srand(SDL_GetTicks());

		Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048);

		AddMazes();

		bgImage = IMG_LoadTexture(renderer, "Images/background.png");
		//load house list from file
		menuState = new MenuState();
		menuState->game = this;
		menuState->init(renderer, font);
		worldMapState = new WorldMapState();
		worldMapState->game = this;
		worldMapState->init(renderer, font);
		mazeState = new MazeState();
		mazeState->game = this;
		mazeState->init(renderer, font);

		winLossState = new WinLossState();
		winLossState->game = this;
		winLossState->init(renderer, font);

		bgMusic = Mix_LoadMUS("audio/bgMusic.mp3");
		collectSound = Mix_LoadWAV("audio/collect.wav");
		clickSound = Mix_LoadWAV("audio/click.mp3");
		if (bgMusic != nullptr) {
			Mix_VolumeMusic(96);
			Mix_PlayMusic(bgMusic, -1);
		}
			
		currentState = menuState;
		
}
void Game::AddMazes() {
	mazeFileNames.push_back("Mazes/house1.txt");
	mazeFileNames.push_back("Mazes/house2.txt");
	mazeFileNames.push_back("Mazes/house3.txt");
	mazeFileNames.push_back("Mazes/house4.txt");
	mazeFileNames.push_back("Mazes/house5.txt");
	mazeFileNames.push_back("Mazes/house6.txt");
	mazeFileNames.push_back("Mazes/house7.txt");
	mazeFileNames.push_back("Mazes/house8.txt");
	mazeFileNames.push_back("Mazes/house9.txt");
	mazeFileNames.push_back("Mazes/house10.txt");
	mazeFileNames.push_back("Mazes/house11.txt");
	mazeFileNames.push_back("Mazes/house12.txt");
	mazeFileNames.push_back("Mazes/house13.txt");
	mazeFileNames.push_back("Mazes/house14.txt");
	mazeFileNames.push_back("Mazes/house15.txt");
	mazeFileNames.push_back("Mazes/house16.txt");
	mazeFileNames.push_back("Mazes/house17.txt");
	mazeFileNames.push_back("Mazes/house18.txt");
	mazeFileNames.push_back("Mazes/house19.txt");
	mazeFileNames.push_back("Mazes/house20.txt");
	mazeFileNames.push_back("Mazes/house21.txt");
	mazeFileNames.push_back("Mazes/house22.txt");
	mazeFileNames.push_back("Mazes/house23.txt");
	mazeFileNames.push_back("Mazes/house24.txt");
	mazeFileNames.push_back("Mazes/house25.txt");
	mazeFileNames.push_back("Mazes/house26.txt");
	mazeFileNames.push_back("Mazes/house27.txt");
	mazeFileNames.push_back("Mazes/house28.txt");
	mazeFileNames.push_back("Mazes/house29.txt");
	mazeFileNames.push_back("Mazes/house30.txt");

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
	Mix_HaltMusic();
	Mix_FreeMusic(bgMusic);
	Mix_FreeChunk(collectSound);
	Mix_FreeChunk(clickSound);
	Mix_CloseAudio();
	TTF_CloseFont(font);
	TTF_Quit();
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
}
void Game::changeState(GameState* newState) {
	currentState = newState;

	MazeState* ms = dynamic_cast<MazeState*>(newState);
	if (ms) ms->OnEnter();
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
MazeState* Game::GetMazeState() {
	return mazeState;
}
void MenuState::init(SDL_Renderer* renderer, TTF_Font* font) {
	//std::cout << "Initialising buttons\n";
	startButton = new Button();
	endButton = new Button();


	startButton->init(renderer,game, "Start", game->SCREEN_WIDTH/2 - 150, 400, 300, 120);
	endButton->init(renderer,game, "Quit", game->SCREEN_WIDTH / 2 - 150,600 , 300, 120);

	// Connect behavior
	startButton->setOnClick([this]() {
		//std::cout << "Start clicked!\n";
		Mix_PlayChannel(-1, game->clickSound, 0);
		game->resetParams();
		game->changeState(game->GetWorldMapState());
		});

	endButton->setOnClick([this]() {
		//std::cout << "Quit clicked!\n";
		Mix_PlayChannel(-1, game->clickSound, 0);
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
void MenuState::render() {
	
	//std::cout << "Rendering\n";
	SDL_SetRenderDrawColor(game->getRenderer(), 0, 0, 0, 255);
	SDL_RenderClear(game->getRenderer());

	
	SDL_Rect bgRect = { 0, 0, game->SCREEN_WIDTH, game->SCREEN_HEIGHT };
	SDL_RenderCopy(game->getRenderer(), game->bgImage, nullptr, &bgRect);
	

	if (titleTexture) {
		titleW = 400;
		titleH = 200;
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
		//pick random maze from list in game object that hasnt been picked
		string FilePath = "";
		newHouse.filePath = FilePath;
		this->houses.push_back(newHouse);

		Button houseButton = Button();

		int col = i % 3; // 0, 1, 2
		int row = i / 3; // 0, 1, 2

		int x = gridX + col * (buttonSize + gap);
		int y = gridY + row * (buttonSize + gap);

		houseButton.init(renderer, game, to_string(i + 1), x, y, buttonSize, buttonSize);
		houseButton.setOnClick([this]() 
			{ //std::cout << "House clicked\n"; 
			  Mix_PlayChannel(-1, game->clickSound, 0);
		      game->GetMazeState()->init(game->getRenderer(), game->getFont());
			  game->changeState(game->GetMazeState()); 
			});

		houseButtons.push_front(houseButton);
	}

	SDL_Color white = { 255, 255, 255, 255 };
	SDL_Surface* topTextSurface = TTF_RenderText_Solid(font, "Please  select  a  house", white);
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

void WorldMapState::render() {

	SDL_SetRenderDrawColor(game->getRenderer(), 0, 0, 0, 255);
	SDL_RenderClear(game->getRenderer());

	SDL_Rect bgRect = { 0, 0, game->SCREEN_WIDTH, game->SCREEN_HEIGHT };
	SDL_RenderCopy(game->getRenderer(), game->bgImage, nullptr, &bgRect);

	if (titleTexture) {
		SDL_Rect titleRect = { (game->SCREEN_WIDTH - topTextW) / 2, 100, topTextW, topTextH };
		SDL_RenderCopy(game->getRenderer(), titleTexture, nullptr, &titleRect);
	}

	//loop through house buttons to render all of them
	for (auto& button : houseButtons) {
		button.render(game->getRenderer());
	}

	SDL_Color white = { 255, 255, 255, 255 };
	string message = "Score " + to_string(game->GetResources() * 10);
	SDL_Surface* scoreTextSurface = TTF_RenderText_Solid(game->getFont(), message.c_str(), white);
	scoreText = SDL_CreateTextureFromSurface(game->getRenderer(), scoreTextSurface);

	message = "HP " + to_string(game->GetHealth());
	SDL_Surface* healthTextSurface = TTF_RenderText_Solid(game->getFont(), message.c_str(), white);
	healthText = SDL_CreateTextureFromSurface(game->getRenderer(), healthTextSurface);

	int topTextW = 100, topTextH = 50;
	SDL_QueryTexture(scoreText, nullptr, nullptr, &topTextW, &topTextH);
	SDL_FreeSurface(scoreTextSurface);
	SDL_QueryTexture(healthText, nullptr, nullptr, &topTextW, &topTextH);
	SDL_FreeSurface(healthTextSurface);

	topTextW = 200;
	topTextH = 100;
	if (scoreText) {
		SDL_Rect scoreRect = { 10, 50, topTextW, topTextH };
		SDL_RenderCopy(game->getRenderer(), scoreText, nullptr, &scoreRect);
	}
	topTextH = 75;
	if (healthText) {
		SDL_Rect healthRect = { 10, 125, topTextW, topTextH };
		SDL_RenderCopy(game->getRenderer(), healthText, nullptr, &healthRect);
	}

	SDL_RenderPresent(game->getRenderer());

	if (!titleTexture) std::cerr << "Title texture null!\n";
	
};

void MazeState::init(SDL_Renderer* renderer, TTF_Font* font) {
	loadHouse();
	player = new Player();
	player->lastTime = SDL_GetTicks();
	player->currentMaze = this;

	for (int i = 0; i < mazeGrid.size(); i++) {
		for (int j = 0; j < mazeGrid[i].size(); j++) {
			if (mazeGrid[i][j] == 2) {
				int offsetX = game->SCREEN_WIDTH / 2 - (cols / 2) * tileSize;
				int offsetY = game->SCREEN_HEIGHT / 2 - (rows / 2) * tileSize;
				player->playerRect = { j * tileSize + offsetX + 10, i * tileSize + offsetY + 10,
									   tileSize - 20, tileSize - 20 };
			}
		}
	}
	player->Texture = IMG_LoadTexture(renderer, "Images/player.png");
	MakeEnemies();
	buildWalls();
	MakeResources();
	FloorTexture = IMG_LoadTexture(renderer, "Images/floor.png");

	WallTexture = IMG_LoadTexture(renderer, "Images/wall.png");

	PlayerStartTexture = IMG_LoadTexture(renderer, "Images/floor.png");

	EnemyStartTexture = IMG_LoadTexture(renderer, "Images/floor.png");
	
	//pick one of 3 random resource textures

	ResourceTexture = IMG_LoadTexture(renderer, "Images/floor.png");
	
	EndTexture = IMG_LoadTexture(renderer, "Images/exit.png");
	
	
	SDL_Color white = { 255, 255, 255, 255 };
	string message = "Score: " + to_string(game->GetResources() * 10);
	SDL_Surface* scoreTextSurface = TTF_RenderText_Solid(font, message.c_str(), white);
	scoreText = SDL_CreateTextureFromSurface(renderer, scoreTextSurface);

	message = "HP: " + to_string(game->GetHealth());
	SDL_Surface* healthTextSurface = TTF_RenderText_Solid(font, message.c_str(), white);
	healthText = SDL_CreateTextureFromSurface(renderer, healthTextSurface);

	int topTextW = 100, topTextH = 50;
	SDL_QueryTexture(scoreText, nullptr, nullptr, &topTextW, &topTextH);
	SDL_FreeSurface(scoreTextSurface);
	SDL_QueryTexture(healthText, nullptr, nullptr, &topTextW, &topTextH);
	SDL_FreeSurface(healthTextSurface);
	
	SDL_SetRenderTarget(renderer, NULL);
	
	
}
void MazeState::buildWalls() {
	int offsetX = game->SCREEN_WIDTH / 2 - (cols / 2) * tileSize;
	int offsetY = game->SCREEN_HEIGHT / 2 - (rows / 2) * tileSize;

	for (int i = 0; i < mazeGrid.size(); i++) {
		for (int j = 0; j < mazeGrid[i].size(); j++) {
			if (mazeGrid[i][j] == 1) {
				SDL_Rect wallRect = { j * tileSize + offsetX, i * tileSize + offsetY, tileSize, tileSize };
				player->walls.push_back(wallRect);
			}
		}
	}
}

void MazeState::MakeResources() {
	int offsetX = game->SCREEN_WIDTH / 2 - (cols / 2) * tileSize;
	int offsetY = game->SCREEN_HEIGHT / 2 - (rows / 2) * tileSize;

	for (int i = 0; i < mazeGrid.size(); i++) {
		for (int j = 0; j < mazeGrid[i].size(); j++) {
			if (mazeGrid[i][j] == 4) {
				
				SDL_Rect ResourceCollider = { j * tileSize + offsetX + 10, i * tileSize + offsetY +10, tileSize-20, tileSize-20 };
				Resource* resource = new Resource(ResourceCollider);

				int textureNum = rand() % 3;

				if (textureNum == 0) {
					resource->Texture = IMG_LoadTexture(game->getRenderer(), "Images/resource1.png");
				}
				else if (textureNum == 0) {
					resource->Texture = IMG_LoadTexture(game->getRenderer(), "Images/resource2.png");
				}
				else {
					resource->Texture = IMG_LoadTexture(game->getRenderer(), "Images/resource3.png");
				}

				resources.push_back(resource);
				
			}
		}
	}
}

void MazeState::MakeEnemies() {
	int offsetX = game->SCREEN_WIDTH / 2 - (cols / 2) * tileSize;
	int offsetY = game->SCREEN_HEIGHT / 2 - (rows / 2) * tileSize;

	for (int i = 0; i < mazeGrid.size(); i++) {
		for (int j = 0; j < mazeGrid[i].size(); j++) {
			if (mazeGrid[i][j] == 3) {

				SDL_Rect EnemyCollider = { j * tileSize + offsetX + 10, i * tileSize + offsetY + 10, tileSize - 20, tileSize - 20 };
				Enemy* enemy = new Enemy(rand() % 2, EnemyCollider, this);
				enemy->lastTime = SDL_GetTicks();
				enemy->Texture = IMG_LoadTexture(game->getRenderer(), "Images/enemy.png");
				enemies.push_back(enemy);

			}
		}
	}
}
void MazeState::OnEnter() {
	Uint32 now = SDL_GetTicks();
	if (player) player->lastTime = now;
	for (auto& e : enemies) e->lastTime = now;
}
void MazeState :: loadHouse() {
	mazeGrid = {};
	enemies = {};
	resources = {};
	string mazeFile = {};
	if (!game->mazeFileNames.empty()) {
		mazeFile = game->mazeFileNames[rand() % game->mazeFileNames.size()];
	}

	
	ifstream file(mazeFile);
	//cout << mazeFile << endl;
	if (!file.is_open()) {
		std::cerr << "Failed to open file: " << mazeFile << std::endl;
	}
	else {
		std::string line;
		while (std::getline(file, line)) {
			std::vector<int> row;
			std::istringstream iss(line);
			int number;
			while (iss >> number) {   // splits by spaces automatically
				row.push_back(number);
			}
			mazeGrid.push_back(row);
		}
		file.close();
	}

	//loading default maze layout if file is empty
	if (mazeGrid.size() == 0) {
		mazeGrid = {
		{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
		{1,2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
		{1,0,1,1,0,1,1,1,1,0,1,1,1,1,3,1,1,0,0,1},
		{1,0,1,0,0,0,0,0,1,0,0,0,0,1,0,0,1,0,0,1},
		{1,0,1,0,1,1,1,0,1,1,1,1,0,1,1,0,1,1,0,1},
		{1,0,0,0,1,0,0,0,0,0,0,1,0,0,1,0,0,0,0,1},
		{1,1,1,0,1,0,1,1,1,1,0,1,1,0,1,1,1,1,0,1},
		{1,0,0,0,0,0,1,0,0,1,0,3,0,0,0,0,0,1,0,1},
		{1,0,1,1,1,0,1,0,1,1,1,1,1,1,0,1,0,1,0,1},
		{1,0,0,4,1,0,0,0,0,0,0,0,0,1,0,0,0,1,0,1},
		{1,1,1,0,1,1,1,1,1,0,1,1,1,1,1,1,0,1,0,1},
		{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,5,1},
		{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1}
		};;
	}

	// checking file has player start and end if not load default
	bool PlayerStartExists = false;
	bool PlayerExitExists = false;
	for (const auto& row : mazeGrid) {
		for (int num : row) {
			if (num == 2) {
				PlayerStartExists = true;
			}
			if (num == 5) {
				PlayerExitExists = true;
			}
		}
		
	}
	if (!PlayerStartExists || !PlayerExitExists) {
		mazeGrid = {
		{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
		{1,2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
		{1,0,1,1,0,1,1,1,1,0,1,1,1,1,3,1,1,0,0,1},
		{1,0,1,0,0,0,0,0,1,0,0,0,0,1,0,0,1,0,0,1},
		{1,0,1,0,1,1,1,0,1,1,1,1,0,1,1,0,1,1,0,1},
		{1,0,0,0,1,0,0,0,0,0,0,1,0,0,1,0,0,0,0,1},
		{1,1,1,0,1,0,1,1,1,1,0,1,1,0,1,1,1,1,0,1},
		{1,0,0,0,0,0,1,0,0,1,0,3,0,0,0,0,0,1,0,1},
		{1,0,1,1,1,0,1,0,1,1,1,1,1,1,0,1,0,1,0,1},
		{1,0,0,4,1,0,0,0,0,0,0,0,0,1,0,0,0,1,0,1},
		{1,1,1,0,1,1,1,1,1,0,1,1,1,1,1,1,0,1,0,1},
		{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,5,1},
		{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1}
		};;
	}
	/* testing to see if file loads into mazeGrid correctly
	for (const auto& row : mazeGrid) {
		for (int num : row) {
			std::cout << num << " ";
		}
		std::cout << std::endl;
	}*/
}
void MazeState::handleEvents() {
	SDL_Event e;
	while (SDL_PollEvent(&e)) {
		if (e.type == SDL_QUIT) {
			game->quit();
		}
	}
}
void MazeState::update() {
	
	const Uint8* keys = SDL_GetKeyboardState(NULL);
	player->handleInput(keys);
	player->tryMove();
	
	for (int i = 0; i < resources.size(); i++) {
		if (player->checkCollision(resources[i]->collider, player->playerRect)&& !resources[i]->collected) {
			game->addResource();
			Mix_PlayChannel(-1, game->collectSound, 0);
			resources[i]->collected = true;
		}
	}
	for (int i = 0; i < enemies.size(); i++) {
		enemies[i]->tryMove();
		if (enemies[i]->checkCollision(enemies[i]->collider, player->playerRect)) {
			game->takeLife();
			if (game->GetHealth() <= 0) {
				game->changeState(game->GetWinLossState());
			}
			else {
				game->changeState(game->GetWorldMapState());
			}

		}
	}

	showExitMessage = player->checkCollision(player->playerRect, exitRect);
}
void MazeState::render() {
	SDL_SetRenderDrawColor(game->getRenderer(), 0, 0, 0, 255);
	SDL_RenderClear(game->getRenderer());

	//loop through mazeGrid and make rectangle of tileSize x tileSize with colour dependant on number

	int offsetX = game->SCREEN_WIDTH / 2 - (cols/2) * tileSize, offsetY = game->SCREEN_HEIGHT / 2 - (rows / 2) * tileSize;
	for (int i = 0; i < mazeGrid.size(); i++) {
		for (int j = 0; j < mazeGrid[i].size(); j++) {
			SDL_Rect tileRect = { j * tileSize + offsetX, i * tileSize + offsetY, tileSize, tileSize };

			switch (mazeGrid[i][j]) {
			case 0:
				SDL_RenderCopy(game->getRenderer(), FloorTexture, nullptr, &tileRect);
				break;
			case 1:
				SDL_RenderCopy(game->getRenderer(), WallTexture, nullptr, &tileRect);
				break;
			case 2:
				SDL_RenderCopy(game->getRenderer(), PlayerStartTexture, nullptr, &tileRect);
				
				break;
			case 3:
				SDL_RenderCopy(game->getRenderer(), EnemyStartTexture, nullptr, &tileRect);
				break;
			case 4:
				SDL_RenderCopy(game->getRenderer(), ResourceTexture, nullptr, &tileRect);
				break;
			case 5:
				SDL_RenderCopy(game->getRenderer(), EndTexture, nullptr, &tileRect);
				exitRect = tileRect;
				break;
			}
			
		}
	}
	SDL_RenderCopy(game->getRenderer(), player->Texture, nullptr, &player->playerRect);

	
	for (int i = 0; i < enemies.size(); i++) {
		
		SDL_RenderCopy(game->getRenderer(), enemies[i]->Texture, nullptr, &enemies[i]->collider);

	}
	for (int i = 0; i < resources.size(); i++) {
		if (!resources[i]->collected) {
			SDL_RenderCopy(game->getRenderer(), resources[i]->Texture, nullptr, &resources[i]->collider);
		}
	}

	SDL_Color white = { 255, 255, 255, 255 };
	string message = "Score " + to_string(game->GetResources() * 10);
	SDL_Surface* scoreTextSurface = TTF_RenderText_Solid(game->getFont(), message.c_str(), white);
	scoreText = SDL_CreateTextureFromSurface(game->getRenderer(), scoreTextSurface);

	message = "HP " + to_string(game->GetHealth());
	SDL_Surface* healthTextSurface = TTF_RenderText_Solid(game->getFont(), message.c_str(), white);
	healthText = SDL_CreateTextureFromSurface(game->getRenderer(), healthTextSurface);

	message = "Press E To exit house";
	SDL_Surface* exitTextSurface = TTF_RenderText_Solid(game->getFont(), message.c_str(), white);
	exitText = SDL_CreateTextureFromSurface(game->getRenderer(), exitTextSurface);

	int topTextW = 100, topTextH = 50;
	SDL_QueryTexture(scoreText, nullptr, nullptr, &topTextW, &topTextH);
	SDL_FreeSurface(scoreTextSurface);
	SDL_QueryTexture(healthText, nullptr, nullptr, &topTextW, &topTextH);
	SDL_FreeSurface(healthTextSurface);
	//topTextW = 100, topTextH = 50;
	SDL_QueryTexture(exitText, nullptr, nullptr, &topTextW, &topTextH);
	SDL_FreeSurface(exitTextSurface);

	topTextW = 200;
	topTextH = 100;
	if (scoreText) {
		SDL_Rect scoreRect = { 10, 50, topTextW, topTextH };
		SDL_RenderCopy(game->getRenderer(), scoreText, nullptr, &scoreRect);
	}
	topTextH = 75;
	if (healthText) {
		SDL_Rect healthRect = { 10, 125, topTextW, topTextH };
		SDL_RenderCopy(game->getRenderer(), healthText, nullptr, &healthRect);
	}

	if (exitText && showExitMessage) {
		SDL_Rect exitTextRect = { (game->SCREEN_WIDTH - topTextW) / 2, 100, topTextW, topTextH };
		SDL_RenderCopy(game->getRenderer(), exitText, nullptr, &exitTextRect);
	}
	SDL_RenderPresent(game->getRenderer());
}

void WinLossState::init(SDL_Renderer* renderer, TTF_Font* font) {
	
	SDL_Color red = { 255, 0, 0, 255 };
	string message = "You died";
	SDL_Surface* titleTextSurface = TTF_RenderText_Solid(font, message.c_str(), red);
	titleTexture = SDL_CreateTextureFromSurface(renderer, titleTextSurface);

	SDL_Color white = { 255, 255, 255, 255 };
	message = "Final score " + to_string(game->GetResources() * 10);
	SDL_Surface* ScoreTextSurface = TTF_RenderText_Solid(font, message.c_str(), white);
	scoreTexture = SDL_CreateTextureFromSurface(renderer, ScoreTextSurface);

	int topTextW = 400, topTextH = 200;

	SDL_QueryTexture(titleTexture, nullptr, nullptr, &topTextW, &topTextH);
	SDL_FreeSurface(titleTextSurface);

	SDL_QueryTexture(scoreTexture, nullptr, nullptr, &topTextW, &topTextH);
	SDL_FreeSurface(ScoreTextSurface);

	restartButton = new Button();
	restartButton->init(renderer, game, "Restart", game->SCREEN_WIDTH / 2 - 275, 550, 550, 180);
	restartButton->setOnClick([this]() {
		Mix_PlayChannel(-1, game->clickSound, 0);
		game->resetHealth();
		game->changeState(game->GetMenuState());
		});

	
}
void WinLossState::render() {
	
	if (win) {
		SDL_Color green = { 0, 255, 0, 255 };
		string message = "Congratulations you survived!";
		SDL_Surface* titleTextSurface = TTF_RenderText_Solid(game->getFont(), message.c_str(), green);
		titleTexture = SDL_CreateTextureFromSurface(game->getRenderer(), titleTextSurface);
		int topTextW = 800, topTextH = 300;
		SDL_QueryTexture(titleTexture, nullptr, nullptr, &topTextW, &topTextH);
		SDL_FreeSurface(titleTextSurface);
	}
	
	SDL_SetRenderDrawColor(game->getRenderer(), 0, 0, 0, 255);
	SDL_RenderClear(game->getRenderer());

	
	int topTextW = 800, topTextH = 350;
	if (titleTexture) {
		SDL_Rect titleRect = { (game->SCREEN_WIDTH - topTextW) / 2, 50, topTextW, topTextH };
		SDL_RenderCopy(game->getRenderer(), titleTexture, nullptr, &titleRect);
	}

	SDL_Color white = { 255, 255, 255, 255 };
	string message = "Final score " + to_string(game->GetResources() * 10);
	SDL_Surface* ScoreTextSurface = TTF_RenderText_Solid(game->getFont(), message.c_str(), white);
	scoreTexture = SDL_CreateTextureFromSurface(game->getRenderer(), ScoreTextSurface);
	topTextW = 400, topTextH = 200;
	if (scoreTexture) {
		SDL_Rect scoreRect = { (game->SCREEN_WIDTH - topTextW) / 2, 300, topTextW, topTextH };
		SDL_RenderCopy(game->getRenderer(), scoreTexture, nullptr, &scoreRect);
	}

	if (restartButton) restartButton->render(game->getRenderer());
	
	
	SDL_RenderPresent(game->getRenderer());
	
}
void WinLossState::handleEvents() {
	SDL_Event e;
	while (SDL_PollEvent(&e)) {
		if (e.type == SDL_QUIT)
			game->quit();

		restartButton->handleEvents(e);
		
	}
}
void Player::handleInput(const Uint8 * keys) {
	velX = 0;
	velY = 0;

	if (keys[SDL_SCANCODE_W]) velY = -speed;
	if (keys[SDL_SCANCODE_S]) velY = speed;
	if (keys[SDL_SCANCODE_A]) velX = -speed;
	if (keys[SDL_SCANCODE_D]) velX = speed;
	if(keys[SDL_SCANCODE_E]) 
	{
		ExitMaze();
	}
}

void Player::ExitMaze() {
	if (checkCollision(playerRect, currentMaze->exitRect)) {
		currentMaze->game->IncHousesVisited();
		if (!currentMaze->game->GetWinLossState()->win) {
			currentMaze->game->changeState(currentMaze->game->GetWorldMapState());
		}
		
	}
}


void Player::tryMove() {
	Uint32 currentTime = SDL_GetTicks();
	float deltaTime = (currentTime - lastTime) / 1000.0f;
	lastTime = currentTime;

	SDL_Rect nextPos = playerRect;

	nextPos.x += velX * deltaTime;
	bool collidedX = false;
	for (auto& wall : walls) {
		if (checkCollision(nextPos, wall)) {
			collidedX = true;
			break;
		}
	}
	if (!collidedX) playerRect.x = nextPos.x;

	nextPos = playerRect;
	nextPos.y += velY * deltaTime;
	bool collidedY = false;
	for (auto& wall : walls) {
		if (checkCollision(nextPos, wall)) {
			collidedY = true;
			break;
		}
	}
	if (!collidedY) playerRect.y = nextPos.y;
}

void Enemy::tryMove() {
	Uint32 currentTime = SDL_GetTicks();
	float deltaTime = (currentTime - lastTime) / 1000.0f;
	lastTime = currentTime;

	//std::cout << "deltaTime: " << deltaTime << ", speed: " << speed << std::endl;

	SDL_Rect nextPos = collider;

	if (dir == 0) {
		nextPos.x += speed * deltaTime;
		bool collidedX = false;
		for (auto& wall : currentMaze->player->walls) {
			if (checkCollision(nextPos, wall)) {
				collidedX = true;
				break;
			}
		}
		if (!collidedX) {
			collider.x = nextPos.x;
		}
		else { 
			if (rand() % 2) {
				dir = 1;
			}
			speed = -speed; 
		
		}
	}
	else {
		nextPos = collider;
		nextPos.y += speed * deltaTime;
		bool collidedY = false;
		for (auto& wall : currentMaze->player->walls) {
			if (checkCollision(nextPos, wall)) {
				collidedY = true;
				break;
			}
		}
		//std::cout << "collidedX: " << collidedY << std::endl;

		if (!collidedY) {
			collider.y = nextPos.y;
		}
		else { 
			if (rand() % 2) {
				dir = 0;
			}
			speed = -speed; 
		}
	}

	
}
bool GameObject::checkCollision(SDL_Rect& a, SDL_Rect& b) {
	return (a.x < b.x + b.w &&
		a.x + a.w > b.x &&
		a.y < b.y + b.h &&
		a.y + a.h > b.y);
}
int main(int argc, char* argv[])
{
	Game game;
	game.init();
	game.run();
	game.quit();
	return 0;
}

