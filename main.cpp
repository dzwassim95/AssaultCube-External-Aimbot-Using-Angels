#include "memory.h"
#include "offsets.h"
#include "math.h"
#include <thread>
#include <iostream>




int main() {

    memory game("ac_client.exe");
    const auto client = game.GetModuleBaseAddr("ac_client.exe");

    if (client)
    {
        std::cout << "aimbot is running ...\n";
    }


    ////////////////////////

    DWORD exitCode{};
    vector3 clcAng{};

    ///////////////////////


    while (true)
    {
        GetExitCodeProcess(game.GetProcH(), &exitCode);
        if (exitCode != STILL_ACTIVE)
        {
            std::cerr << "game not found, exiting . . .\n";
            system("pause");
            break;
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(1));

        if (!(GetKeyState(VK_LSHIFT) & 0x8000))
        {
            continue;
        }


        const auto localPayer = game.read<uintptr_t>(client + offsets::iLocalPlayer);
        const auto localHead = game.read<vector3>(localPayer + offsets::vHead);

        vector3 viewAngle = { game.read<float>(localPayer + offsets::pYaw), game.read<float>(localPayer + offsets::pPitch), 0.f };

        const auto entity = game.read<uintptr_t>(client + offsets::iEntityList);
        auto isTeam = game.read<int>(localPayer + offsets::iTeam);
        auto playerCount = game.read<int>(client + offsets::iEntityList + 0x8);

        //////////////////////

        auto bestFov{ 45.f };
        Angles bestAngle{};

        ////////////////////

        for (int i = 0; i < playerCount; ++i)
        {
            auto player = game.read<uintptr_t>(entity + 0x4 * i);

            if (game.read<int>(player + offsets::iTeam) == isTeam)
            {
                continue;
            }

            if (game.read<int>(player + offsets::isDead))
            {
                continue;
            }

            auto headPos3D = game.read<vector3>(player + offsets::vHead);

            clcAng = calcAngle(headPos3D, localHead, viewAngle);

            auto fov = std::hypot(clcAng.x, clcAng.y);

            if (fov < bestFov)
            {
               bestFov = fov;
               //clcAng.x /= 5.f; (smooth)
               //clcAng.y /= 5.f;
               bestAngle = { (viewAngle.x + clcAng.x), (viewAngle.y + clcAng.y) };
            }
           
        }
        ///aimbot
        if (!(bestAngle.yaw == 0.f && bestAngle.pitch == 0.f))
        {
            game.write<Angles>(localPayer + offsets::pYaw, bestAngle);
        }
    }


    if (game.GetProcH())
    {
        CloseHandle(game.GetProcH());
    }

    ExitProcess(0);
}
