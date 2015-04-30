#include "SFAsset.h"

int SFAsset::SFASSETID=0;

SFAsset::SFAsset(SFASSETTYPE type, std::shared_ptr<SFWindow> window): type(type), sf_window(window) {
  this->id   = ++SFASSETID;

  switch (type) {
  case SFASSET_TITLE:
    sprite = IMG_LoadTexture(sf_window->getRenderer(), "assets/title.png");
    break;
  case SFASSET_RETRY:
    sprite = IMG_LoadTexture(sf_window->getRenderer(), "assets/retry.png");
    break;
  case SFASSET_WIN:
    sprite = IMG_LoadTexture(sf_window->getRenderer(), "assets/win.png");
    break;
  case SFASSET_LOSE:
    sprite = IMG_LoadTexture(sf_window->getRenderer(), "assets/lose.png");
    break;
  case SFASSET_PLAYER:
    sprite = IMG_LoadTexture(sf_window->getRenderer(), "assets/player.png");
    break;
  case SFASSET_PROJECTILE:
    sprite = IMG_LoadTexture(sf_window->getRenderer(), "assets/projectile.png");
    break;
  case SFASSET_WORMHOLE:
    sprite = IMG_LoadTexture(sf_window->getRenderer(), "assets/wormhole.png");
    break;
  case SFASSET_ALIEN:
    sprite = IMG_LoadTexture(sf_window->getRenderer(), "assets/alien.png");
    break;
  // here I'm loading in some new assets,
  // doesn't make for the most interesting code
  // but snags all the eye-catching glory
  case SFASSET_WALL:
    sprite = IMG_LoadTexture(sf_window->getRenderer(), "assets/wall.png");
    break;
  case SFASSET_METEOR:
    sprite = IMG_LoadTexture(sf_window->getRenderer(), "assets/meteor.png");
    break;
  case SFASSET_POWERUP:
    sprite = IMG_LoadTexture(sf_window->getRenderer(), "assets/powerup.png");
    break;
  default:
    break;
  }

  if(!sprite) {
    cerr << "Could not load asset of type " << type << endl;
    throw SF_ERROR_LOAD_ASSET;
  }

  // Get texture width & height
  int w, h;
  SDL_QueryTexture(sprite, NULL, NULL, &w, &h);

  // Initialise bounding box
  bbox = make_shared<SFBoundingBox>(SFBoundingBox(Vector2(0.0f, 0.0f), w, h));
}

SFAsset::SFAsset(const SFAsset& a) {
  sprite = a.sprite;
  sf_window = a.sf_window;
  bbox   = a.bbox;
  type   = a.type;
}

SFAsset::~SFAsset() {
  bbox.reset();
  if(sprite) {
    SDL_DestroyTexture(sprite);
    sprite = nullptr;
  }
}

/**
 * The logical coordinates in the game assume that the screen
 * is indexed from 0,0 in the bottom left corner.  The blittable
 * coordinates of the screen map 0,0 to the top left corner. We
 * need to convert between the two coordinate spaces.  We assume
 * that there is a 1-to-1 quantisation.
 */
Vector2 GameSpaceToScreenSpace(SDL_Renderer* renderer, Vector2 &r) {
  int w, h;
  SDL_GetRendererOutputSize(renderer, &w, &h);

  return Vector2 (
                  r.getX(),
                  (h - r.getY())
                  );
}

void SFAsset::SetPosition(Point2 & point) {
  Vector2 v(point.getX(), point.getY());
  bbox->SetCentre(v);
}

Point2 SFAsset::GetPosition() {
  return Point2(bbox->centre->getX(), bbox->centre->getY());
}

// acceleration multipliers implemented here
// allow tweaks to be made to make the game feel more fluid,
// around this time I was also trying to smoothen out the movement
// but found I was limited without introducing a concept like deltaTime
// with velocity, momentum, etc .. may have jeopardized getting this done!
void SFAsset::SetAcceleration(int num) {
  acc = num;
}

int SFAsset::GetAcceleration() {
  return acc;
}

SFAssetId SFAsset::GetId() {
  return id;
}

void SFAsset::OnRender() {
  // 1. Get the SDL_Rect from SFBoundingBox
  SDL_Rect rect;

  Vector2 gs = (*(bbox->centre) + (*(bbox->extent_x) * -1)) + (*(bbox->extent_y) * 1);
  Vector2 ss = GameSpaceToScreenSpace(sf_window->getRenderer(), gs);
  rect.x = ss.getX();
  rect.y = ss.getY();
  rect.w = bbox->extent_x->getX() * 2;
  rect.h = bbox->extent_y->getY() * 2;

  // 2. Blit the sprite onto the level
  SDL_RenderCopy(sf_window->getRenderer(), sprite, NULL, &rect);
}

// extended for omni-directional movement,
// this involved extending the existing methods to include complete
// GoNorth() and GoSouth(), and binding those actions to key events
void SFAsset::GoNorth() {
  Vector2 c = *(bbox->centre) + Vector2(0.0f, 1.0f * GetAcceleration());
  bbox->centre.reset();
  bbox->centre = make_shared<Vector2>(c);
}

void SFAsset::GoSouth() {
  Vector2 c = *(bbox->centre) + Vector2(0.0f, -1.0f * GetAcceleration());
  bbox->centre.reset();
  bbox->centre = make_shared<Vector2>(c);
}

void SFAsset::GoWest() {
  Vector2 c = *(bbox->centre) + Vector2(-1.0f * GetAcceleration(), 0.0f);
  if(!(c.getX() < 0)) {
    bbox->centre.reset();
    bbox->centre = make_shared<Vector2>(c);
  }
}

void SFAsset::GoEast() {
  int w, h;
  SDL_GetRendererOutputSize(sf_window->getRenderer(), &w, &h);

  Vector2 c = *(bbox->centre) + Vector2(1.0f * GetAcceleration(), 0.0f);
  if(!(c.getX() > w)) {
    bbox->centre.reset();
    bbox->centre = make_shared<Vector2>(c);
  }
}

bool SFAsset::CollidesWith(shared_ptr<SFAsset> other) {
  return bbox->CollidesWith(other->bbox);
}

shared_ptr<SFBoundingBox> SFAsset::GetBoundingBox() {
  return bbox;
}

void SFAsset::SetNotAlive() {
  type = SFASSET_DEAD;
}

bool SFAsset::IsAlive() {
  return (SFASSET_DEAD != type);
}

void SFAsset::HandleCollision() {
  SetNotAlive();
}
