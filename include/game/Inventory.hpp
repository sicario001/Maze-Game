#pragma once

#include <string>
#include <sstream>

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

#include "fwd.hpp"
#include "LTimer.hpp"
#include "engine/graphics/LTexture.hpp"


class Inventory{
    private:
        WeaponType currWeapon = KNIFE;
        const int firelockvals[3] = {20, 5, 20};
        const int damagevals[3] = {5, 10, 15};
        int mag_rem_rifle = 10;
        int mag_lim_rifle = 10;
        int total_rem_rifle = 20;
        int mag_rem_pistol = 7;
        int mag_lim_pistol = 7;
        int total_rem_pistol = 21;
        LTexture* rifleTexture = NULL;
        LTexture* pistolTexture = NULL;
        // LTexture* bombTexture = NULL;
        LTexture* knifeTexture = NULL;
        // LTexture* smokeTexture = NULL;
        // LTexture* grenadeTexture = NULL;
        // LTexture* flashTexture = NULL;
        // LTexture* defuseKitTexture = NULL;
        LTexture* desTextTexture = NULL;

		std::stringstream desText;

        TTF_Font *gFont = NULL;

    public:
        Inventory();
        ~Inventory();
        void useBullet();
        bool isEmptyMag();
        bool canReload();
        int getReloadDuration();
        int getFireLockDuration();
        int getDamage();
        void reload();
        bool isEmptyAmmo();
        // void useSmoke();
        // bool isEmptySmoke();
        // void useFlash();
        // bool isEmptyFlash();
        // void useGrenade();
        // bool isEmptyGrenade();
        void render();
        void loadMediaInventory();
        void changeWeapon(WeaponType newWeapon);
        WeaponType getCurrWeapon();
};