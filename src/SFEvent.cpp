#include "SFEvent.h"

/**
 * Effectively wraps an SDL_Event in our custom event type.
 */
SFEvent::SFEvent(const SDL_Event & event) {
  switch (event.type) {
  case SDL_QUIT:
    code = SFEVENT_QUIT;
    break;
  case SDL_USEREVENT:
    code = SFEVENT_UPDATE;
    break;
  // here I was able to extend player up-down movement,
  // from keyboard events
  case SDL_KEYDOWN:
    switch (event.key.keysym.sym) {
    // handle 'up' key
    case SDLK_UP:
      code = SFEVENT_PLAYER_UP;
      break;
    // handle 'down' key
    case SDLK_DOWN:
      code = SFEVENT_PLAYER_DOWN;
      break;
    case SDLK_LEFT:
      code = SFEVENT_PLAYER_LEFT;
      break;
    case SDLK_RIGHT:
      code = SFEVENT_PLAYER_RIGHT;
      break;
    case SDLK_SPACE:
      code = SFEVENT_FIRE;
      break;
    case SDLK_q:
      code = SFEVENT_QUIT;
      break;
    }
    break;
  default:
    code = SFEVENT_NULL;
    break;
  }
}


SFEVENT SFEvent::GetCode() {
  return code;
}
