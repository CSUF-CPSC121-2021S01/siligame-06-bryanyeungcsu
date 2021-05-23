#include "game.h"
#include "opponent.h"
#include "player.h"
#include <iostream>
#include <memory>
#include <vector>

void Game::UpdateScreen() {
  graphics::Color blue(0, 0, 255);
  GameScreen_.DrawRectangle(0, 0, 800, 600, blue);
  for (int i = 0; i < Opponents_.size(); i++) {
    if (Opponents_[i]->GetIsActive()) {
      Opponents_[i]->Draw(GameScreen_);
    }
  }
  for (int i = 0; i < OpponentProjectiles_.size(); i++) {
    if (OpponentProjectiles_[i]->GetIsActive()) {
      OpponentProjectiles_[i]->Draw(GameScreen_);
    }
  }
  for (int i = 0; i < PlayerProjectiles_.size(); i++) {
    if (PlayerProjectiles_[i]->GetIsActive()) {
      PlayerProjectiles_[i]->Draw(GameScreen_);
    }
  }
  if (Myplayer_.GetIsActive()) {
    Myplayer_.Draw(GameScreen_);
  }
  GameScreen_.DrawText(10, 10, std::to_string(playerScore_), 35, 0, 0, 0);
}

void Game::CreateOpponents() {
  for (int i = 0; i < 5; i++) {
    Opponents_.push_back(std::make_unique<Opponent>(50 * i + 15, 50));
  }
}

void Game::Init() {
  GameScreen_.AddMouseEventListener(*this);
  GameScreen_.AddAnimationEventListener(*this);
  Myplayer_.SetX(20);
  Myplayer_.SetY(GameScreen_.GetHeight() - 50);
}

void Game::MoveGameElements() {
  for (int i = 0; i < Opponents_.size(); i++) {
    Opponents_[i]->Move(GameScreen_);
  }
  for (int j = 0; j < OpponentProjectiles_.size(); j++) {
    OpponentProjectiles_[j]->Move(GameScreen_);
  }
  for (int t = 0; t < PlayerProjectiles_.size(); t++) {
    PlayerProjectiles_[t]->Move(GameScreen_);
  }
}

void Game::LaunchProjectiles() {
  for (int i = 0; i < Opponents_.size(); i++) {
    if (Opponents_[i]->LaunchProjectile() != nullptr) {
      std::unique_ptr<OpponentProjectile> oFire;
      oFire = std::make_unique<OpponentProjectile>();
      oFire->SetX(Opponents_[i]->GetX());
      oFire->SetY(Opponents_[i]->GetY());
      OpponentProjectiles_.push_back(std::move(oFire));
    }
  }
}

void Game::RemoveInactive() {
  for (int i = PlayerProjectiles_.size() - 1; i >= 0; i--) {
    if (!PlayerProjectiles_[i]->GetIsActive()) {
      PlayerProjectiles_.erase(PlayerProjectiles_.begin() + i);
    }
  }
  for (int i = OpponentProjectiles_.size() - 1; i >= 0; i--) {
    if (!OpponentProjectiles_[i]->GetIsActive()) {
      OpponentProjectiles_.erase(OpponentProjectiles_.begin() + i);
    }
  }
  for (int i = Opponents_.size() - 1; i >= 0; i--) {
    if (!Opponents_[i]->GetIsActive()) {
      Opponents_.erase(Opponents_.begin() + i);
    }
  }
}
// siligame06
void Game::FilterIntersections() {
  for (int i = 0; i < Opponents_.size(); i++) {
    for (int j = 0; j < PlayerProjectiles_.size(); j++) {
      if (Opponents_[i]->IntersectsWith(PlayerProjectiles_[j].get())) {
        PlayerProjectiles_[j]->SetIsActive(false);
        Opponents_[i]->SetIsActive(false);
        if (Myplayer_.GetIsActive())
          playerScore_ += 1;
      }
    }
  }
  for (int i = 0; i < OpponentProjectiles_.size(); i++) {
    if (Myplayer_.IntersectsWith(OpponentProjectiles_[i].get())) {
      OpponentProjectiles_[i]->SetIsActive(false);
      GetPlayer().SetIsActive(false);
      gameState = true;
    }
  }
  for (int i = 0; i < Opponents_.size(); i++) {
    if (Myplayer_.IntersectsWith(Opponents_[i].get())) {
      Opponents_[i]->SetIsActive(false);
      GetPlayer().SetIsActive(false);
      gameState = true;
    }
  }
}
void Game::OnAnimationStep() {
  if (Opponents_.size() == 0)

    CreateOpponents();
    if (gameStarted) {
      MoveGameElements();
      FilterIntersections();
      RemoveInactive();
      LaunchProjectiles();
      UpdateScreen();
    } else {
      graphics::Color blue(0, 0, 255);
      GameScreen_.DrawRectangle(0, 0, 800, 600, blue);
      GameScreen_.DrawText(240, 400, "What In Reincarantion!", 50,
                           graphics::Color(255, 255, 255));
      GameScreen_.DrawText(200, 450, "LEFT CLICK TO START", 35,
                           graphics::Color(255, 255, 255));
      for (size_t i = 0; i < Opponents_.size(); i++) {
        Opponents_[i]->Draw(GameScreen_);
        Opponents_[i]->Move(GameScreen_);
      }
    }
    GameScreen_.Flush();
  }
    void Game::OnMouseEvent(const graphics::MouseEvent &mouseEvent) {
  if (!gameStarted &&
      mouseEvent.GetMouseAction() == graphics::MouseAction::kPressed) {
    gameStarted = true;
  }

  if (mouseEvent.GetX() > 0 && mouseEvent.GetY() &&
      mouseEvent.GetX() < GameScreen_.GetWidth() &&
      mouseEvent.GetY() < GameScreen_.GetHeight()) {
    Myplayer_.SetX(mouseEvent.GetX() - 25);
    Myplayer_.SetY(mouseEvent.GetY() - 25);
  }
  if (mouseEvent.GetMouseAction() == graphics::MouseAction::kPressed) {
    std::unique_ptr<PlayerProjectile> pFire =
        std::make_unique<PlayerProjectile>(mouseEvent.GetX(),
                                           mouseEvent.GetY() + 15);
    PlayerProjectiles_.push_back(std::move(pFire));
  }
  if (mouseEvent.GetMouseAction() == graphics::MouseAction::kDragged) {
    std::unique_ptr<PlayerProjectile> pFire =
        std::make_unique<PlayerProjectile>(mouseEvent.GetX(),
                                           mouseEvent.GetY() + 15);
    PlayerProjectiles_.push_back(std::move(pFire));
  }
}
