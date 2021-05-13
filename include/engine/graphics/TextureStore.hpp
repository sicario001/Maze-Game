#pragma once
#include "fwd.hpp"
#include "engine/graphics/LTexture.hpp"
#include "engine/GameEngine.hpp"
extern GameEngine* gEngine;

enum TsTextureType{
    TS_CLOCK,
    TS_BG_CT_WIN,
    TS_BG_T_WIN,
    TS_BG_END,
    TS_W_AKM,
    TS_W_FN45,
    TS_W_BAYONET,
    TS_BG_LOADING,
    TS_LOAD_ICON,
    TS_TILESHEET,
    TS_MENU,
    TS_BG,
    TS_MENU_PAUSE,
    TS_BG_PAUSE,
    TS_BULLET,
    TS_SLASH,
    TS_SPRITESHEET,
    TS_C4
};

class TextureStore{
    LTexture* textures[18];
    public:
        TextureStore();
        void init();
        void free();
        LTexture* getSourceFor(TsTextureType t);
};