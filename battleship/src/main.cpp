

#include <SDL2/SDL.h>
#include <iostream>
#include <ostream>
#include <string>
#include <vector>

enum CellState { EMPTY, HIT, MISS };

const int grid_cell_size = 36;
const int grid_width = 19;
const int grid_height = 13;
const int max_ships = 5;

std::vector<int> ship_sizes = {1, 2, 3, 4, 5}; // Sizes of ships

bool is_player1_turn = true; // Track whose turn it is
bool placing_ships = true;   // Track if we are in the ship placement phase
int current_ship_index = 0;  // Current ship being placed
bool is_vertical = false;    // Ship orientation (vertical or horizontal)
SDL_Point hover_cell = {-1,
                        -1}; // Current hovered cell for shooting or placement

SDL_bool quit = SDL_FALSE;

class Ship {
public:
  SDL_Rect rect;
  bool is_vertical;
  int size;

  Ship(SDL_Rect r, bool v, int s) : rect(r), is_vertical(v), size(s) {}

  bool isSunk(const std::vector<std::vector<CellState>> &grid) const {
    for (int x = rect.x / grid_cell_size;
         x < (rect.x + rect.w) / grid_cell_size; ++x) {
      for (int y = rect.y / grid_cell_size;
           y < (rect.y + rect.h) / grid_cell_size; ++y) {
        if (grid[x][y] != HIT) {
          return false;
        }
      }
    }
    return true;
  }
};

std::vector<std::vector<CellState>>
    player1_grid(grid_width, std::vector<CellState>(grid_height, EMPTY));
std::vector<std::vector<CellState>>
    player2_grid(grid_width, std::vector<CellState>(grid_width, EMPTY));
std::vector<Ship> player1_ships;
std::vector<Ship> player2_ships;

class Player {
public:
  std::vector<Ship> ships;
  std::vector<std::vector<CellState>> grid;

  Player() : grid(grid_width, std::vector<CellState>(grid_height, EMPTY)) {}

  bool hasAllShipsSunk() const {
    for (const auto &ship : ships) {
      if (!ship.isSunk(grid)) {
        return false;
      }
    }
    return true;
  }
};

class Referee {
public:
  static void notifyShipSunk(const std::string &player_name) {
    std::cout << player_name << " has sunk a ship!" << std::endl;
  }

  static void notifyWinner(const std::string &player_name) {
    SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION, "Game Over",
                             (player_name + " wins!").c_str(), NULL);
  }
};

class Game {
private:
  Player player1, player2;
  bool is_player1_turn;
  bool placing_ships;
  int current_ship_index;
  bool is_vertical;
  SDL_Point hover_cell;

public:
  Game()
      : is_player1_turn(true), placing_ships(true), current_ship_index(0),
        is_vertical(false), hover_cell{-1, -1} {}

  void start() {
    SDL_Init(SDL_INIT_VIDEO);
    SDL_Window *window;
    SDL_Renderer *renderer;

    // Create window and renderer
    SDL_CreateWindowAndRenderer(grid_width * grid_cell_size,
                                grid_height * grid_cell_size, 0, &window,
                                &renderer);
    SDL_SetWindowTitle(window, "Battleship");

    while (!quit) {
      SDL_Event event;
      while (SDL_PollEvent(&event)) {
        switch (event.type) {
        case SDL_KEYDOWN:
          switch (event.key.keysym.sym) {
          case SDLK_r: // Toggle rotation
            is_vertical = !is_vertical;
            break;
          }
          break;

        case SDL_MOUSEMOTION:
          hover_cell = getGridPosition(event.motion.x, event.motion.y);
          break;

        case SDL_MOUSEBUTTONDOWN:
          if (event.button.button == SDL_BUTTON_LEFT) {
            int mouse_x = event.button.x;
            int mouse_y = event.button.y;
            SDL_Point grid_pos = getGridPosition(mouse_x, mouse_y);

            if (placing_ships) {
              handleShipPlacement(grid_pos, renderer);
            } else {
              handleShootingPhase(grid_pos, renderer);
            }
          }
          break;
        case SDL_QUIT:
          quit = SDL_TRUE;
          break;
        }
      }

      renderGame(renderer);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
  }

private:
  void handleShipPlacement(const SDL_Point &grid_pos, SDL_Renderer *renderer) {
    if (placing_ships) {
      int ship_size = ship_sizes[current_ship_index];
      SDL_Rect new_ship_rect = {
          grid_pos.x * grid_cell_size, grid_pos.y * grid_cell_size,
          is_vertical ? grid_cell_size : grid_cell_size * ship_size,
          is_vertical ? grid_cell_size * ship_size : grid_cell_size};

      Player &current_player = is_player1_turn ? player1 : player2;

      if (!isOutOfBounds(new_ship_rect) &&
          !isOverlapping(new_ship_rect, current_player.ships)) {
        current_player.ships.push_back(
            Ship(new_ship_rect, is_vertical, ship_size));

        if (current_ship_index < ship_sizes.size()) {
          current_ship_index++;
        }

        if (current_ship_index >= ship_sizes.size()) {
          current_ship_index = 0;
          is_player1_turn = !is_player1_turn;
          if (player1.ships.size() == ship_sizes.size() &&
              player2.ships.size() == ship_sizes.size()) {
            placing_ships = false;
          }
        }
      }
    }
  }

  void handleShootingPhase(const SDL_Point &grid_pos, SDL_Renderer *renderer) {
    Player &current_player = is_player1_turn ? player1 : player2;
    Player &opponent_player = is_player1_turn ? player2 : player1;

    bool hit = processShot(grid_pos.x, grid_pos.y, opponent_player.ships,
                           opponent_player.grid, current_player.ships);
    if (hit) {
      SDL_Log("%s hits!", is_player1_turn ? "Player 1" : "Player 2");
    } else {
      SDL_Log("%s misses!", is_player1_turn ? "Player 1" : "Player 2");
    }

    if (areAllShipsSunk(opponent_player.ships, opponent_player.grid)) {
      Referee::notifyWinner(is_player1_turn ? "Player 1" : "Player 2");
      quit = SDL_TRUE;
    }

    is_player1_turn = !is_player1_turn;
  }

  void renderGame(SDL_Renderer *renderer) {
    SDL_SetRenderDrawColor(renderer, 22, 22, 22, 255);
    SDL_RenderClear(renderer);

    // Draw the grids and other elements
    SDL_SetRenderDrawColor(renderer, 44, 44, 44, 255);
    drawGrids(renderer);

    // Show the hover and preview ship
    showHoverAndPreview(renderer);

    // Draw the ships and game state (hits, misses)
    drawShipsAndGameState(renderer);

    SDL_RenderPresent(renderer);
    SDL_Delay(1000 / 60); // 60 FPS
  }

  // Render grid lines
  void drawGrids(SDL_Renderer *renderer) {
    for (int x = 0; x < grid_width; x++) {
      for (int y = 0; y < grid_height; y++) {
        SDL_Rect cell_rect = {x * grid_cell_size, y * grid_cell_size,
                              grid_cell_size, grid_cell_size};
        SDL_RenderDrawRect(renderer, &cell_rect);
      }
    }
  }

  // Render ships and game state (hit/miss)
  void drawShipsAndGameState(SDL_Renderer *renderer) {
    for (const auto &ship : (is_player1_turn ? player1.ships : player2.ships)) {
      SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255); // White ships
      SDL_RenderFillRect(renderer, &ship.rect);
    }

    if (!placing_ships) {
      // Draw grid states (hit/miss) for both players

      if (!is_player1_turn) {
        drawCellStates(renderer, player1.grid);
      } else {
        drawCellStates(renderer, player2.grid);
      }
    }
  }

  void drawCellStates(SDL_Renderer *renderer,
                      const std::vector<std::vector<CellState>> &grid) {
    for (int x = 0; x < grid_width; x++) {
      for (int y = 0; y < grid_height; y++) {
        SDL_Rect cell_rect = {x * grid_cell_size, y * grid_cell_size,
                              grid_cell_size, grid_cell_size};
        if (grid[x][y] == HIT) {
          SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255); // Red for hits
          SDL_RenderFillRect(renderer, &cell_rect);
        }
      }
    }

    // Draw misses after (this ensures misses are overwritten by hits if both
    // occur)
    for (int x = 0; x < grid_width; x++) {
      for (int y = 0; y < grid_height; y++) {
        SDL_Rect cell_rect = {x * grid_cell_size, y * grid_cell_size,
                              grid_cell_size, grid_cell_size};

        // Draw misses only if the cell wasn't already hit
        if (grid[x][y] == MISS) {
          SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255); // Blue for misses
          SDL_RenderFillRect(renderer, &cell_rect);
        } else if (grid[x][y] == HIT) {
          SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255); // Red for hits
          SDL_RenderFillRect(renderer, &cell_rect);
        }
      }
    }
  }

  // Convert mouse position to grid position
  SDL_Point getGridPosition(int mouse_x, int mouse_y) {
    return {mouse_x / grid_cell_size, mouse_y / grid_cell_size};
  }

  // Check if a shot hits a ship
  bool processShot(int grid_x, int grid_y, std::vector<Ship> &ships,
                   std::vector<std::vector<CellState>> &grid,
                   std::vector<Ship> &currentShips) {

    for (const auto &ship : currentShips) {
      SDL_Rect cell_rect = {grid_x * grid_cell_size, grid_y * grid_cell_size,
                            grid_cell_size, grid_cell_size};
      if (SDL_HasIntersection(&cell_rect, &ship.rect)) {
        // It's a hit on the player's own ship, return false (invalid shot)
        return false;
      }
    }

    for (auto &ship : ships) {
      SDL_Rect cell_rect = {grid_x * grid_cell_size, grid_y * grid_cell_size,
                            grid_cell_size, grid_cell_size};
      if (SDL_HasIntersection(&cell_rect, &ship.rect)) {
        grid[grid_x][grid_y] = HIT;
        return true;
      }
    }

    grid[grid_x][grid_y] = MISS;
    return false; // Miss
  }

  // Check if all ships are destroyed
  bool areAllShipsSunk(const std::vector<Ship> &ships,
                       const std::vector<std::vector<CellState>> &grid) {
    for (const auto &ship : ships) {
      if (!ship.isSunk(grid)) {
        return false;
      }
    }

    return true;
  }

  bool isOutOfBounds(const SDL_Rect &rect) {
    return rect.x < 0 || rect.y < 0 ||
           rect.x + rect.w > grid_width * grid_cell_size ||
           rect.y + rect.h > grid_height * grid_cell_size;
  }

  bool isOverlapping(const SDL_Rect &rect, const std::vector<Ship> &ships) {
    for (const auto &ship : ships) {
      if (SDL_HasIntersection(&rect, &ship.rect)) {
        return true;
      }
    }
    return false;
  }

  void showHoverAndPreview(SDL_Renderer *renderer) {
    if (hover_cell.x != -1 && hover_cell.y != -1) {
      // Render preview of ship placement (depending on rotation)
      SDL_Rect preview_rect = {
          hover_cell.x * grid_cell_size, hover_cell.y * grid_cell_size,
          is_vertical ? grid_cell_size
                      : grid_cell_size * ship_sizes[current_ship_index],
          is_vertical ? grid_cell_size * ship_sizes[current_ship_index]
                      : grid_cell_size};
      SDL_SetRenderDrawColor(renderer, 255, 255, 0, 128);
      SDL_RenderFillRect(renderer, &preview_rect);
    }
  }

  void drawGameOver(SDL_Renderer *renderer, const std::string &winner) {
    SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255); // Green for win
    SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION, "Game Over",
                             (winner + " wins!").c_str(), NULL);
  }
};

int main(int argc, char *argv[]) {
  Game game;
  game.start();
  return 0;
}
