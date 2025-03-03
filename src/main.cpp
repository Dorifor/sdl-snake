#define SDL_MAIN_USE_CALLBACKS 1
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <cstdlib>

SDL_Window *window;
SDL_Renderer *renderer;
const int WIDTH = 800;
const int HEIGHT = 600;
const int CELL_SIZE = 20;
const int GRID_MAX_X = WIDTH / CELL_SIZE;
const int GRID_MAX_Y = HEIGHT / CELL_SIZE;
const int STEP_INTERVAL = 60;

int apple_x, apple_y;

struct SnakeCell {
  char x;
  char y;
  char next_x;
  char next_y;
  SnakeCell *next;
};

enum class Direction { UP, DOWN, RIGHT, LEFT };
Direction current_direction = Direction::RIGHT;

SnakeCell tail{GRID_MAX_X / 2 - 4, GRID_MAX_Y / 2, 0, 0};
SnakeCell tail2{GRID_MAX_X / 2 - 3, GRID_MAX_Y / 2, 0, 0, &tail};
SnakeCell tail3{GRID_MAX_X / 2 - 2, GRID_MAX_Y / 2, 0, 0, &tail2};
SnakeCell tail4{GRID_MAX_X / 2 - 1, GRID_MAX_Y / 2, 0, 0, &tail3};
SnakeCell head{GRID_MAX_X / 2, GRID_MAX_Y / 2, 0, 0, &tail4};

unsigned int last_time, current_time;

void get_random_position(int &x, int &y) {
  x = std::rand() % GRID_MAX_X + 1;
  y = std::rand() % GRID_MAX_Y + 1;
}

void _add_tail() {
  SnakeCell *temp = &head;
  while (temp->next) {
    temp = temp->next;
  }
  SnakeCell *new_tail = new SnakeCell{temp->x, temp->y};
  temp->next = new_tail;
}

void _move_head(int x_axis, int y_axis) {
  SnakeCell *temp = &head;
  temp->next_x = temp->x + x_axis;
  temp->next_y = temp->y + y_axis;
  while (temp) {
    if (temp->next) {
      temp->next->next_x = temp->x;
      temp->next->next_y = temp->y;
    }
    temp->x = temp->next_x;
    temp->y = temp->next_y;
    temp = temp->next;
  }
}

bool is_head_on_apple() { return head.x == apple_x && head.y == apple_y; }

bool is_head_inside_body() {
  SnakeCell *temp = head.next;
  while (temp) {
    if (temp->x == head.x && temp->y == head.y)
      return true;
    temp = temp->next;
  }

  return false;
}

bool should_die() {
  bool is_inside_x = 0 <= head.x && head.x < GRID_MAX_X;
  bool is_inside_y = 0 <= head.y && head.y < GRID_MAX_Y;
  return !is_inside_x || !is_inside_y || is_head_inside_body();
}

void _move_in_current_direction() {
  switch (current_direction) {
  case Direction::RIGHT:
    _move_head(1, 0);
    break;

  case Direction::LEFT:
    _move_head(-1, 0);
    break;

  case Direction::UP:
    _move_head(0, -1);
    break;

  case Direction::DOWN:
    _move_head(0, 1);
    break;
  }
}

void _handle_key_input(SDL_Scancode key_code) {
  switch (key_code) {
  case SDL_SCANCODE_RIGHT:
    if (current_direction != Direction::LEFT)
      current_direction = Direction::RIGHT;
    break;

  case SDL_SCANCODE_LEFT:
    if (current_direction != Direction::RIGHT)
      current_direction = Direction::LEFT;
    break;

  case SDL_SCANCODE_UP:
    if (current_direction != Direction::DOWN)
      current_direction = Direction::UP;
    break;

  case SDL_SCANCODE_DOWN:
    if (current_direction != Direction::UP)
      current_direction = Direction::DOWN;
    break;

  default:
    break;
  }
}

SDL_AppResult SDL_AppInit(void **appstate, int argc, char *argv[]) {
  SDL_Init(SDL_INIT_VIDEO);

  SDL_CreateWindowAndRenderer("Snake: The Game", 800, 600,
                              SDL_WINDOW_KEYBOARD_GRABBED, &window, &renderer);

  get_random_position(apple_x, apple_y);

  return SDL_APP_CONTINUE;
}

void draw_rect(SDL_Renderer *renderer, int x, int y) {
  SDL_FRect rect;
  rect.x = x * CELL_SIZE;
  rect.y = y * CELL_SIZE;
  rect.w = CELL_SIZE;
  rect.h = CELL_SIZE;
  SDL_RenderFillRect(renderer, &rect);
}

SDL_AppResult SDL_AppIterate(void *appstate) {
  SDL_SyncWindow(window);
  SDL_SetRenderDrawColor(renderer, 239, 241, 245, 0);
  SDL_RenderClear(renderer);

  SDL_SetRenderDrawColor(renderer, 64, 160, 43, 0);

  // Draw Snake
  SnakeCell *cell = &head;
  while (cell) {
    draw_rect(renderer, cell->x, cell->y);
    cell = cell->next;
  }

  // Draw Apple
  SDL_SetRenderDrawColor(renderer, 254, 100, 11, 0);
  draw_rect(renderer, apple_x, apple_y);

  SDL_RenderPresent(renderer);
  current_time = SDL_GetTicks();
  if (current_time > last_time + STEP_INTERVAL) {
    _move_in_current_direction();
    last_time = current_time;
  }

  if (is_head_on_apple()) {
    get_random_position(apple_x, apple_y);
    _add_tail();
  }

  if (should_die())
    return SDL_APP_SUCCESS;

  return SDL_APP_CONTINUE;
}

SDL_AppResult SDL_AppEvent(void *appstate, SDL_Event *event) {
  switch (event->type) {
  case SDL_EVENT_QUIT:
    return SDL_APP_SUCCESS;
    break;
  case SDL_EVENT_KEY_UP:
    if (!event->key.down)
      _handle_key_input(event->key.scancode);
    break;
  }
  return SDL_APP_CONTINUE;
}

void SDL_AppQuit(void *appstate, SDL_AppResult result) {}
