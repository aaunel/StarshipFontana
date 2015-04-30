#ifndef SFAPP_H
#define SFAPP_H

#include <memory>   // Pull in std::shared_ptr
#include <iostream> // Pull in std::cerr, std::endl
#include <list>     // Pull in list
#include <sstream>

using namespace std;

#include "SFCommon.h"
#include "SFEvent.h"
#include "SFAsset.h"

enum STATES {WORLD, MAIN_MENU, LOSE_MENU, WIN_MENU};

/**
 * Represents the StarshipFontana application.  It has responsibilities for
 * * Creating and destroying the app window
 * * Processing game events
 */
class SFApp {
public:
  SFApp(std::shared_ptr<SFWindow>);
  virtual ~SFApp();
  void    OnEvent(SFEvent &);
  void    OnExecute();
  void    OnUpdateWorld();
  void    OnRender();

  void    FireProjectile();
private:
  int                     state;
  bool                    is_running;

  // manage game states
  void    SetState(int STATES);
  int     GetState();
  // render different screens
  void    RenderWorld();
  void    RenderMenu();

  shared_ptr<SFWindow>       sf_window;

  // added menu title and status pointers
  shared_ptr<SFAsset>        title;
  shared_ptr<SFAsset>        status;
  shared_ptr<SFAsset>        player;
  // added wormhole pointer
  shared_ptr<SFAsset>        wormhole;
  shared_ptr<SFBoundingBox>  app_box;
  list<shared_ptr<SFAsset> > projectiles;
  list<shared_ptr<SFAsset> > aliens;
  // added walls std::list
  list<shared_ptr<SFAsset> > walls;
  // added meteors std::list
  list<shared_ptr<SFAsset> > meteors;
  // renamed coins to powerups
  list<shared_ptr<SFAsset> > powerups;

  int fire;
};
#endif
