#include "game_element.h"

bool GameElement::IntersectsWith(GameElement *element) {
  return !(GetX() > (*element).GetX() + (*element).GetWidth() ||
           (*element).GetX() > GetX() + GetWidth() ||
           GetY() > (*element).GetY() + (*element).GetHeight() ||
           (*element).GetY() > GetY() + GetHeight());
}

bool GameElement::IsOutOfBounds(const graphics::Image &boundimage) {
  if (GetX() < 0 || GetX() + GetWidth() > boundimage.GetWidth()) {
    return true;
  } else if (GetY() < 0 || GetY() + GetHeight() > boundimage.GetHeight()) {
    return true;
  } else {
    return false;
  }
}
void GameElement::Helper(graphics::Image &background, graphics::Image *drawing,
                         int x_, int y_) {
  for (int i = 0; i < drawing->GetWidth(); i++) {
    int x = i;
    for (int j = 0; j < drawing->GetHeight(); j++) {
      int y = j;
      if (drawing->GetColor(i, j) != graphics::Color(0, 0, 0) &&
          y_ + drawing->GetHeight() < background.GetHeight()) {
        background.SetColor((i + x_), (j + y_), drawing->GetColor(i, j));
      } else {
        continue;
      }
    }
  }
}

void GameElement::rHelper(graphics::Image &background, graphics::Image *drawing,
                          int x_, int y_) {
  for (int i = drawing->GetWidth(); i > 0; i--) {
    int x = i;
    for (int j = drawing->GetHeight(); j > 0; j--) {
      int y = j;
      if (drawing->GetColor(i, j) != graphics::Color(0, 0, 0) &&
          y_ + drawing->GetHeight() < background.GetHeight()) {
        background.SetColor((x_ - i), (j + y_), drawing->GetColor(i, j));
      } else {
        continue;
      }
    }
  }
}
