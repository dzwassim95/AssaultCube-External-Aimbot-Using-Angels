#include "math.h"
#include <iostream>


vector3 calcAngle(const vector3& enemyHpos, const vector3& localHpos, const vector3& vA)
{

    vector3 dif = enemyHpos - localHpos;
    vector3 target = { -std::atan2(dif.x, dif.y) * (180.0f / std::numbers::pi_v<float>) + 180,
        std::atan2(dif.z, std::hypot(dif.x, dif.y)) * (180.0f / std::numbers::pi_v<float>),
        0.f
    };

    return target - vA ;
}