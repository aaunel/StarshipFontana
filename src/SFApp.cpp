#include "SFApp.h"

SFApp::SFApp(std::shared_ptr<SFWindow> window) : fire(0), is_running(true), sf_window(window) {
  RenderMainMenu();
}

SFApp::~SFApp() {
}

void SFApp::RenderWorld() {
  int canvas_w, canvas_h;
  SDL_GetRendererOutputSize(sf_window->getRenderer(), &canvas_w, &canvas_h);

  app_box = make_shared<SFBoundingBox>(Vector2(canvas_w, canvas_h), canvas_w, canvas_h);
  player  = make_shared<SFAsset>(SFASSET_PLAYER, sf_window);
  auto player_pos = Point2(canvas_w/2, 22);
  player->SetAcceleration(16);
  player->SetPosition(player_pos);

  // columns, stolen from web concept,
  // seemed a slightly tidier way to achieve a layout
  // here I'm dividing up the canvas into columns
  const int number_of_columns = 12;
  const int col_w = canvas_w / number_of_columns;

  // wormhole, end-game objective
  wormhole = make_shared<SFAsset>(SFASSET_WORMHOLE, sf_window);
  auto wormhole_pos = Point2((col_w * 3) - col_w/2, 325.0f);
  wormhole->SetPosition(wormhole_pos);

  // modified aliens, only two in the base,
  // these now sit within the column layout
  auto alien_1 = make_shared<SFAsset>(SFASSET_ALIEN, sf_window);
  auto alien_1_pos = Point2((col_w * 9) - col_w/2, 300.0f);
  alien_1->SetPosition(alien_1_pos);
  aliens.push_back(alien_1);
  auto alien_2 = make_shared<SFAsset>(SFASSET_ALIEN, sf_window);
  auto alien_2_pos = Point2((col_w * 3) - col_w/2, 250.0f);
  alien_2->SetPosition(alien_2_pos);
  aliens.push_back(alien_2);

  // our dangerous wall obstacles; mustn't fly into these,
  // not the tidiest implementation of a level design,
  // could probably be improved using a basic grid matrix,
  // it would then be very tidy to place assets by editing a text file,
  // e.g. { a: alien, w: wall, p: player }
  //    00a00a00
  //    ww0ww0ww
  //    0000p000

  // Row 1
  for(int i=1; i<number_of_columns-1; i++) {
    auto wall = make_shared<SFAsset>(SFASSET_WALL, sf_window);
    auto pos = Point2(col_w * i, 400.0f);
    wall->SetPosition(pos);
    walls.push_back(wall);
  }
  // Row 2
  for(int i=1; i<number_of_columns-1; i++) {
    if(i == 1 || i == 4 || i == 10) {
      auto wall = make_shared<SFAsset>(SFASSET_WALL, sf_window);
      auto pos = Point2(col_w * i, 350.0f);
      wall->SetPosition(pos);
      walls.push_back(wall);
    }
  }
  // Row 3
  for(int i=1; i<number_of_columns-1; i++) {
    if(i == 1 || i == 4 || i == 10) {
      auto wall = make_shared<SFAsset>(SFASSET_WALL, sf_window);
      auto pos = Point2(col_w * i, 300.0f);
      wall->SetPosition(pos);
      walls.push_back(wall);
    }
  }
  // Row 4
  for(int i=1; i<number_of_columns-1; i++) {
    if(i == 1 || i == 4 || i == 7 || i == 10) {
      auto wall = make_shared<SFAsset>(SFASSET_WALL, sf_window);
      auto pos = Point2(col_w * i, 250.0f);
      wall->SetPosition(pos);
      walls.push_back(wall);
    }
  }
  // Row 5
  for(int i=1; i<number_of_columns-1; i++) {
    if(i == 1 || i == 7 || i == 10) {
      auto wall = make_shared<SFAsset>(SFASSET_WALL, sf_window);
      auto pos = Point2(col_w * i, 200.0f);
      wall->SetPosition(pos);
      walls.push_back(wall);
    }
  }
  // Row 6
  for(int i=1; i<number_of_columns-1; i++) {
    if(i == 1 || i == 7 || i == 10) {
      auto wall = make_shared<SFAsset>(SFASSET_WALL, sf_window);
      auto pos = Point2(col_w * i, 150.0f);
      wall->SetPosition(pos);
      walls.push_back(wall);
    }
  }
  // Row 7
  for(int i=1; i<number_of_columns-1; i++) {
    if(i < 8 || i == 10) {
      auto wall = make_shared<SFAsset>(SFASSET_WALL, sf_window);
      auto pos = Point2(col_w * i, 100.0f);
      wall->SetPosition(pos);
      walls.push_back(wall);
    }
  }


  // the meteor adds an additional sprite
  // managed using std::list, which could allow for multiple meteors
  auto meteor = make_shared<SFAsset>(SFASSET_METEOR, sf_window);
  auto meteor_pos = Point2(canvas_w - 100, 100.0f);
  meteor->SetAcceleration(1);
  meteor->SetPosition(meteor_pos);
  meteors.push_back(meteor);

  // the powerup is similar to a coin, but hey - it's a power up
  auto powerup = make_shared<SFAsset>(SFASSET_POWERUP, sf_window);
  auto pos  = Point2((col_w * 6) - col_w/2, 450.0f);
  powerup->SetPosition(pos);
  powerups.push_back(powerup);

  // set world state
  state = WORLD;
}

void SFApp::RenderMainMenu() {
  int canvas_w, canvas_h;
  SDL_GetRendererOutputSize(sf_window->getRenderer(), &canvas_w, &canvas_h);

  app_box = make_shared<SFBoundingBox>(Vector2(canvas_w, canvas_h), canvas_w, canvas_h);
  
  // rendering menu title as in-game asset is terribly inefficient,
  // and I did try my best to run this separately
  title = make_shared<SFAsset>(SFASSET_POWERUP, sf_window);
  auto title_pos = Point2(canvas_w/2, canvas_h/2);
  title->SetPosition(title_pos);

  player = make_shared<SFAsset>(SFASSET_PLAYER, sf_window);
  auto player_pos = Point2(canvas_w/2, 100.0f);
  player->SetAcceleration(32);
  player->SetPosition(player_pos);

  // set main menu state
  state = MAIN_MENU;
}

/**
 * Handle all events that come from SDL.
 * These are timer or keyboard events.
 */
void SFApp::OnEvent(SFEvent& event) {
  SFEVENT the_event = event.GetCode();
  switch (the_event) {
  case SFEVENT_QUIT:
    is_running = false;
    break;
  case SFEVENT_UPDATE:
    OnUpdateWorld();
    OnRender();
    break;
  // adds player up movement
  case SFEVENT_PLAYER_UP:
    player->GoNorth();
    break;
  // adds player down movement
  case SFEVENT_PLAYER_DOWN:
    player->GoSouth();
    break;
  case SFEVENT_PLAYER_LEFT:
    player->GoWest();
    break;
  case SFEVENT_PLAYER_RIGHT:
    player->GoEast();
    break;
  case SFEVENT_FIRE:
    if (state == WORLD) {
      fire ++;
      FireProjectile();
    } else {
      RenderWorld();
    }
    break;
  default:
    break;
  }
}

void SFApp::OnExecute() {
  // Execute the app
  SDL_Event event;
  while (SDL_WaitEvent(&event) && is_running) {
    // wrap an SDL_Event with our SFEvent
    SFEvent sfevent((const SDL_Event) event);
    // handle our SFEvent
    OnEvent(sfevent);
  }
}

void SFApp::OnUpdateWorld() {
  // Update projectile positions
  for(auto p: projectiles) {
    p->GoNorth();
  }

  for(auto m: meteors) {
    m->GoNorth();
  }

  // Update enemy positions
  for(auto a : aliens) {
    // do something here
  }

  // Detect projectile collisions
  for(auto p : projectiles) {
    for(auto a : aliens) {
      if(p->CollidesWith(a)) {
        p->HandleCollision();
        a->HandleCollision();
      }
    }
  }

  // Detect wall collisions
  for(auto w : walls) {
    if(w->CollidesWith(player)) {
      player->HandleCollision();
      RenderMainMenu();
    }
  }

  // Detect meteor collisions
  for(auto m : meteors) {
    for(auto a : aliens) {
      if(m->CollidesWith(a)) {
        m->HandleCollision();
        a->HandleCollision();
      }
    }
  }

  // garbage collection (?) here,
  // not carried on with other assets,
  // probably not a good thing!

  // remove dead aliens (the long way)
  list<shared_ptr<SFAsset>> tmp;
  for(auto a : aliens) {
    if(a->IsAlive()) {
      tmp.push_back(a);
    }
  }
  aliens.clear();
  aliens = list<shared_ptr<SFAsset>>(tmp);
}

void SFApp::OnRender() {
  SDL_RenderClear(sf_window->getRenderer());

  // draw the player,
  // who can now also die..
  if(player->IsAlive()) {player->OnRender();}

  if(state != WORLD) {
    // render menu title
    title->OnRender();
  } else {
  // wormhole
  wormhole->OnRender();

  for(auto p: projectiles) {
    if(p->IsAlive()) {p->OnRender();}
  }

  for(auto a: aliens) {
    if(a->IsAlive()) {a->OnRender();}
  }

  // the walls
  for(auto w: walls) {
    w->OnRender();
  }

  // meteors can 'die'
  for(auto m: meteors) {
    if(m->IsAlive()) {m->OnRender();}
  }

  // and so can collectable powerup,
  // this is loosely used to disappear once picked up
  for(auto u: powerups) {
    if(u->IsAlive()) {u->OnRender();}
  }
  }

  // Switch the off-screen buffer to be on-screen
  SDL_RenderPresent(sf_window->getRenderer());
}

void SFApp::FireProjectile() {
  if(player->IsAlive()) {
    auto pb = make_shared<SFAsset>(SFASSET_PROJECTILE, sf_window);
    auto v  = player->GetPosition();
    pb->SetAcceleration(8);
    pb->SetPosition(v);
    projectiles.push_back(pb);
  }
}
