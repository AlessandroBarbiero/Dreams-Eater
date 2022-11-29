#pragma once
#include "Component.hpp"


class EndMenuComponent : public Component {

public:

    explicit EndMenuComponent(GameObject* gameObject);
    void onGui() override;

};