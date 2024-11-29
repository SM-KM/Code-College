
#include <SDL2/SDL.h>
#include <vector>

struct Ship {
  SDL_Rect rect;    // Position and size of the ship
  bool is_vertical; // Orientation of the ship
};

const int grid_cell_size = 36;
const int grid_width = 19;
const int grid_height = 13;
const int max_ships = 5; // Maximum number of ships
std::vector<int> ship_sizes = {2, 3, 4};

std::vector<Ship> ships; // List of placed ships

// Function to check if the new ship placement overlaps with existing ships
bool isOverlapping(const SDL_Rect &new_rect) {
  for (const auto &ship : ships) {
    if (SDL_HasIntersection(&new_rect, &ship.rect)) {
      return true;
    }
  }
  return false;
}

// Function to check if the new ship placement is out of bounds
bool isOutOfBounds(const SDL_Rect &ship_rect) {
  return ship_rect.x < 0 || ship_rect.y < 0 ||
         ship_rect.x + ship_rect.w > grid_width * grid_cell_size ||
         ship_rect.y + ship_rect.h > grid_height * grid_cell_size;
}

int window_width = (grid_width * grid_cell_size) * 2;
int window_height = (grid_height * grid_cell_size) * 1.5;

SDL_Rect grid_cursor = {
    .x = (grid_width - 1) / 2 * grid_cell_size,
    .y = (grid_height - 1) / 2 * grid_cell_size,
    .w = grid_cell_size,
    .h = grid_cell_size,
};

SDL_Rect grid_cursor_ghost = {grid_cursor.x, grid_cursor.y, grid_cell_size,
                              grid_cell_size};

SDL_Color grid_background = {22, 22, 22, 255}; // Dark
SDL_Color grid_line_color = {44, 44, 44, 255};
SDL_Color grid_cursor_ghost_color = {44, 44, 44, 255};
SDL_Color grid_cursor_color = {255, 255, 255, 255}; // White
SDL_Color ship_color = {0, 255, 0, 128}; // Semi-transparent green for ships

int main(int argc, char *argv[]) {
  SDL_Init(SDL_INIT_VIDEO);

  SDL_Window *window;
  SDL_Renderer *renderer;

  // Create window and renderer
  SDL_CreateWindowAndRenderer(window_width, window_height, 0, &window,
                              &renderer);
  SDL_SetWindowTitle(window, "Battleship");

  SDL_bool quit = SDL_FALSE;
  SDL_bool mouse_active = SDL_FALSE;
  SDL_bool mouse_hover = SDL_FALSE;
  SDL_Rect grid_cursor_ghost = {0, 0, grid_cell_size, grid_cell_size};
  bool is_vertical = false;
  int current_ship_size = 3; // Start with a default ship size

  while (!quit) {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
      switch (event.type) {
      case SDL_KEYDOWN:
        switch (event.key.keysym.sym) {
        case SDLK_r: // Toggle rotation
          is_vertical = !is_vertical;
          break;
        case SDLK_c: // Cycle to next ship size
          current_ship_size = (current_ship_size + 1) % ship_sizes.size();
          break;
        }
        break;

      case SDL_MOUSEMOTION: {
        // Update the ghost cursor for ship preview
        int mouse_x = event.motion.x;
        int mouse_y = event.motion.y;

        // Update the preview grid cursor for the ship preview
        grid_cursor_ghost.x = (mouse_x / grid_cell_size) * grid_cell_size;
        grid_cursor_ghost.y = (mouse_y / grid_cell_size) * grid_cell_size;

        // Adjust ship preview size based on orientation
        if (is_vertical) {
          grid_cursor_ghost.w = grid_cell_size;
          grid_cursor_ghost.h = grid_cell_size * current_ship_size;
        } else {
          grid_cursor_ghost.w = grid_cell_size * current_ship_size;
          grid_cursor_ghost.h = grid_cell_size;
        }

        // Ensure the ship preview is within bounds
        if (isOutOfBounds(grid_cursor_ghost)) {
          break;
        }

        // Show ghost ship placement based on mouse
        if (!mouse_active)
          mouse_active = SDL_TRUE;
        break;
      }

      case SDL_MOUSEBUTTONDOWN:
        if (event.button.button == SDL_BUTTON_LEFT &&
            ships.size() < max_ships) {
          // Create a new ship and add it to the list if it's valid
          SDL_Rect ship_rect = grid_cursor_ghost;

          // Ensure placement is valid
          if (isOutOfBounds(ship_rect) || isOverlapping(ship_rect)) {
            break;
          }

          // Add the ship to the list
          ships.push_back({ship_rect, is_vertical});
        }
        break;

      case SDL_QUIT:
        quit = SDL_TRUE;
        break;
      }
    }

    // Draw background
    SDL_SetRenderDrawColor(renderer, 22, 22, 22, 255);
    SDL_RenderClear(renderer);

    // Draw grid lines
    SDL_SetRenderDrawColor(renderer, 44, 44, 44, 255);
    for (int x = 0; x < grid_width * grid_cell_size; x += grid_cell_size) {
      SDL_RenderDrawLine(renderer, x, 0, x, (grid_height * grid_cell_size));
    }
    for (int y = 0; y < grid_height * grid_cell_size; y += grid_cell_size) {
      SDL_RenderDrawLine(renderer, 0, y, (grid_width * grid_cell_size), y);
    }

    // Draw already placed ships
    for (const auto &ship : ships) {
      SDL_SetRenderDrawColor(renderer, 255, 255, 255,
                             255); // Green for placed ships
      SDL_RenderFillRect(renderer, &ship.rect);
    }

    // Draw the ghost ship if hovering
    if (mouse_active && !isOutOfBounds(grid_cursor_ghost)) {
      SDL_SetRenderDrawColor(renderer, 44, 44, 44, 255);
      SDL_RenderFillRect(renderer, &grid_cursor_ghost);
    }

    SDL_RenderPresent(renderer);
  }

  // Clean up
  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  SDL_Quit();

  return 0;
}
