#pragma once
#include "Component.hpp"


class StartMenuComponent : public Component {

public:

    explicit StartMenuComponent(GameObject* gameObject);
    void onGui() override;

};