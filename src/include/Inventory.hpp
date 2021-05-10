#pragma once
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include "LTexture.hpp"
#include "LTimer.hpp"
#include "fwd.hpp"
#include <string>
#include <sstream>

class Inventory{
    private:
        WeaponType currWeapon = KNIFE;
        int mag_rem = 10;
        int mag_lim = 10;
        int total_rem = 20;
        int smoke_rem = 0;
        int flash_rem = 0;
        int grenade_rem = 0;
        LTexture* gunTexture = NULL;
        LTexture* bombTexture = NULL;
        LTexture* knifeTexture = NULL;
        LTexture* smokeTexture = NULL;
        LTexture* grenadeTexture = NULL;
        LTexture* flashTexture = NULL;
        LTexture* defuseKitTexture = NULL;
        LTexture* desTextTexture = NULL;

		std::stringstream desText;

        TTF_Font *gFont = NULL;

    public:
        Inventory();
        ~Inventory();
        void useBullet();
        bool isEmptyMag();
        bool canReload();
        void reload();
        bool isEmptyAmmo();
        void useSmoke();
        bool isEmptySmoke();
        void useFlash();
        bool isEmptyFlash();
        void useGrenade();
        bool isEmptyGrenade();
        void render();
        void loadMediaInventory();
        void changeWeapon(WeaponType newWeapon);
        WeaponType getCurrWeapon();
};