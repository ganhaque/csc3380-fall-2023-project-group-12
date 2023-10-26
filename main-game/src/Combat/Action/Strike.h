#pragma once
#include "Action.h"

class Strike : public Action {
  public:
    // Strike() {
    //   energyCost = 1;
    // }
    Strike();
    ~Strike();

    int perform(Unit* user, std::vector<Unit*> targets, Game& game) override;
};