#include "Combat.h"
#include <cstdio>
#include <raylib.h>

void Combat::update(Game& game) {
  if (animationDuration != 0) {
    animationDuration -= 1;
    fprintf(stderr, "[ANIMATION]: %d\n", animationDuration);
    return;
  }
  if (isRoundOver) {
    // start new round
    fprintf(stderr, "[ROUND END]: %d\n", currentRound);
    currentRound += 1;
    fprintf(stderr, "----------------\n");
    fprintf(stderr, "[ROUND START]: %d\n", currentRound);
    for (Unit* hero : heroes) {
      animationDuration += hero->onRoundStart();
      fprintf(stderr, "%s speed: %d\n", hero->id.c_str(), hero->getSpeed());
    }
    for (Unit* foe : foes) {
      animationDuration += foe->onRoundStart();
      fprintf(stderr, "%s speed: %d\n", foe->id.c_str(), foe->getSpeed());
    }
    // sort by speed -> add to turnQueue

    // PLACEHOLDER: TO BE REPLACE. DO NOT USE //
    // turnQueue.push(heroes[0]);
    // // fprintf(stderr, "success\n");
    // turnQueue.push(foes[0]);
    // fprintf(stderr, "a\n");
    ///////////////////////////////
    // Sort characters by speed
    std::vector<Unit*> units;
    units.insert(units.end(), heroes.begin(), heroes.end());
    units.insert(units.end(), foes.begin(), foes.end());

    // favor player if same speed
    // std::sort(units.begin(), units.end(), [](Unit* a, Unit* b) {
    //     return a->getSpeed() > b->getSpeed();
    //     });

    // random sorting if same speed (need testing)
    std::sort(units.begin(), units.end(), [](Unit* a, Unit* b) {
        // Compare speeds
        if (a->getSpeed() > b->getSpeed()) {
        return true;
        }
        else if (a->getSpeed() < b->getSpeed()) {
        return false;
        }
        else {
        // If speeds are equal, introduce randomness
        // double randomValue = distribution(generator);
        return GetRandomValue(0, 1) == 0; // 50% chance for each unit to be first
        }
        });

    // Add characters to the turn queue
    for (Unit* unit : units) {
      turnQueue.push(unit);
    }
    isRoundOver = false;

    animationDuration += 8;
    return;
  }
  if (currentUnit == nullptr) {
    if (!turnQueue.empty()) {
      fprintf(stderr, "---\n");
      currentUnit = turnQueue.front();
      turnQueue.pop();
      animationDuration += currentUnit->onTurnStart();
      // currentUnit->animationTimer = 0.5;
      // currentUnit->startAnimationTime = GetTime();
      return;
    }

    animationDuration += 2;
    isRoundOver = true;
    return;
  }
  else {
    if (currentUnit->energy < 1) {
      animationDuration += currentUnit->onTurnEnd();
      currentUnit = nullptr;
      return;
    }
    if (isFoe(currentUnit)) {
      dynamic_cast<Foe*>(currentUnit)->decideAction();
      targets = dynamic_cast<Foe*>(currentUnit)->decideTarget(heroes);
    }
    Action* action = currentUnit->getAction();
    if (action == nullptr) return; // extra, maybe need later?
    if (action != nullptr && !targets.empty()) {
      animationDuration += action->perform(currentUnit, targets, game);
      highlightedAction = nullptr;
      targets = {};
    }
  }
}