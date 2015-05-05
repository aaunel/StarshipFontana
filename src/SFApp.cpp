#include "SFApp.h"

SFApp::SFApp(std::shared_ptr<SFWindow> window) : fire(0), is_running(true), sf_window(window) {
  // render main menu state
  RenderMenu(MAIN_MENU);
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

  // the meteor destroys this once the collectible objective
  // is retrieved by the player
  gate = make_shared<SFAsset>(SFASSET_GATE, sf_window);
  auto gate_pos = Point2((col_w * 9) - col_w/2, 100.0f);
  gate->SetPosition(gate_pos);

  // the meteor adds an additional sprite
  // managed using std::list, which could allow for multiple meteors
  auto meteor = make_shared<SFAsset>(SFASSET_METEOR, sf_window);
  // positioned off-screen to begin with
  auto meteor_pos = Point2((col_w * 9) - col_w/2, -100.0f);
  meteor->SetAcceleration(2);
  meteor->SetPosition(meteor_pos);
  meteors.push_back(meteor);

  // the powerup is similar to a coin, but hey - it's a power up
  auto powerup = make_shared<SFAsset>(SFASSET_POWERUP, sf_window);
  auto pos  = Point2((col_w * 6) - col_w/2, 450.0f);
  powerup->SetPosition(pos);
  powerups.push_back(powerup);

  // set world state
  SetState(WORLD);
}

void SFApp::RenderMenu(int menu_state) {
  // set menu state
  SetState(menu_state);

  // defenses start up
  defenses = true;

  int canvas_w, canvas_h;
  SDL_GetRendererOutputSize(sf_window->getRenderer(), &canvas_w, &canvas_h);

  app_box = make_shared<SFBoundingBox>(Vector2(canvas_w, canvas_h), canvas_w, canvas_h);
  
  // rendering menu title as in-game asset is terribly inefficient,
  // and I did try my best to run this separately
  if(GetState() != LOSE_MENU) {
    title = make_shared<SFAsset>(SFASSET_TITLE, sf_window);
  } else {
    title = make_shared<SFAsset>(SFASSET_RETRY, sf_window);
  }
  auto title_pos = Point2(canvas_w/2, (canvas_h/3)*2);
  title->SetPosition(title_pos);

  // render playable character in main menu
  if(GetState() == MAIN_MENU) {
    player = make_shared<SFAsset>(SFASSET_PLAYER, sf_window);
    auto player_pos = Point2(canvas_w/2, 100.0f);
    player->SetAcceleration(32);
    player->SetPosition(player_pos);
  } else if (GetState() == LOSE_MENU) {
    status = make_shared<SFAsset>(SFASSET_LOSE, sf_window);
    auto status_pos = Point2(canvas_w/2, 100.0f);
    status->SetPosition(status_pos);
  } else if (GetState() == WIN_MENU) {
    status = make_shared<SFAsset>(SFASSET_WIN, sf_window);
    auto status_pos = Point2(canvas_w/2, 100.0f);
    status->SetPosition(status_pos);
  }
}

void SFApp::DownDefenses() {
  defenses = false;
}

/**
 * Handle all events that come from SDL.
 * These are timer or keyboard events.
 */
void SFApp::OnEvent(SFEvent& event) {
  bool collides = false;

  if(GetState() == WORLD) {
    // Detect gate collision,
    // like wall collision,
    // the gate can be rendered as 'empty' but
    // the bounding box remains, so only detect
    // collision when gate is still 'alive'
    if(gate->IsAlive() && gate->CollidesWith(player)) {
      collides = true;
    }

    // Detect wall collisions,
    // player can't move through walls
    for(auto w : walls) {
      if(w->CollidesWith(player)) {
        collides = true;
      }
    }
  }

  SFEVENT the_event = event.GetCode();
  switch (the_event) {
  case SFEVENT_QUIT:
    is_running = false;
    break;
  case SFEVENT_UPDATE:
    // only updating world in appropriate state now,
    // failing to do this actually crashes the game
    if(GetState() == WORLD) {OnUpdateWorld();}
    OnRender();
    break;
  // adds player up movement
  case SFEVENT_PLAYER_UP:
    if(collides) {
      // avoid collision
      player->GoSouth();
    } else {
      player->GoNorth();
    }
    break;
  // adds player down movement
  case SFEVENT_PLAYER_DOWN:
    if(collides) {
      // avoid collision
      player->GoNorth();
    } else {
      player->GoSouth();
    }
    break;
  case SFEVENT_PLAYER_LEFT:
    if(collides) {
      // avoid collision
      player->GoEast();
    } else {
      player->GoWest();
    }
    break;
  case SFEVENT_PLAYER_RIGHT:
    if(collides) {
      // avoid collision
      player->GoWest();
    } else {
      player->GoEast();
    }
    break;
  case SFEVENT_FIRE:
    if (GetState() == WORLD) {
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

  if(defenses == false) {
    // set meteor to take out defenses
    for(auto m: meteors) {
      m->GoNorth();
    }
  }

  // Update enemy positions
  for(auto a : aliens) {
    if(a->CollidesWith(player)) {
      player->HandleCollision();
      RenderMenu(LOSE_MENU);
    }
  }

  // Detect projectile collisions
  for(auto p : projectiles) {
    if(gate->IsAlive() && p->CollidesWith(gate)) {
      p->HandleCollision();
    }
    for(auto w : walls) {
      if(p->CollidesWith(w)) {
        p->HandleCollision();
      }
    }
    for(auto a : aliens) {
      // looks like I'm not alone in
      // taking shortcuts! haha ..
      // dead projectiles can collide
      if(p->IsAlive() && p->CollidesWith(a)) {
        p->HandleCollision();
        a->HandleCollision();
      }
    }
  }

  // Detect objective collision
  if(wormhole->CollidesWith(player)) {
    RenderMenu(WIN_MENU);
  }

  // Detect meteor collisions
  for(auto m : meteors) {
    if(m->CollidesWith(gate)) {
      m->HandleCollision();
      gate->HandleCollision();
    }
    for(auto w : walls) {
      if(m->CollidesWith(w)) {
        m->HandleCollision();
        w->HandleCollision();
      }
    }
  }

  // Detect collectible objective
  for(auto p : powerups) {
    if(p->CollidesWith(player)) {
      p->HandleCollision();
      DownDefenses();
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

  if(GetState() == WORLD || GetState() == MAIN_MENU) {
    // draw the player,
    // who can now also die..
    if(player->IsAlive()) {player->OnRender();}
  }

  if(GetState() != WORLD) {
    // render menu title
    title->OnRender();

    if(GetState() == LOSE_MENU || GetState() == WIN_MENU) {
      // render menu status
      status->OnRender();
    }
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

  // destructible gate to the alien base
  if(gate->IsAlive()) {gate->OnRender();}

  // meteors can 'die'
  for(auto m: meteors) {
    if(m->IsAlive()) {m->OnRender();}
  }

  // and so can collectible powerup,
  // this is loosely used to disappear once picked up
  for(auto u: powerups) {
    if(u->IsAlive()) {u->OnRender();}
  }
  }

  // Switch the off-screen buffer to be on-screen
  SDL_RenderPresent(sf_window->getRenderer());
}

// state set/get
void SFApp::SetState(int val) {
  state = val;
}

int SFApp::GetState() {
  return state;
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
