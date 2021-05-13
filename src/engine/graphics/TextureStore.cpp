#include "engine/graphics/TextureStore.hpp"
TextureStore::TextureStore(){
    for(int i = 0;i<18;i++){
        textures[i] = new LTexture();
    }
    textures[TS_C4] = new LTexture(0.1);
}

void checkLoadError(bool x,int i){
    if(!x){
        cout<<"Couldn't load texture "<<i<<endl;
    }
}
void TextureStore::init(){

    checkLoadError(textures[TS_CLOCK]->loadFromFile("media/texture/clock.png"),TS_CLOCK);
    checkLoadError(textures[TS_BG_CT_WIN]->loadFromFile("media/texture/ct_win.png"),TS_CLOCK);
    checkLoadError(textures[TS_BG_T_WIN]->loadFromFile("media/texture/t_win.png"),TS_BG_T_WIN);
    checkLoadError(textures[TS_BG_END]->loadFromFile("media/texture/background_end.png"),TS_BG_END);
    checkLoadError(textures[TS_W_AKM]->loadFromFile("media/texture/inventory/AKM.png"),TS_W_AKM);
    checkLoadError(textures[TS_W_FN45]->loadFromFile("media/texture/inventory/FN45.png"),TS_W_FN45);
    checkLoadError(textures[TS_W_BAYONET]->loadFromFile("media/texture/inventory/Bayonet.png"),TS_W_BAYONET);
    checkLoadError(textures[TS_BG_LOADING]->loadFromFile("media/texture/loadingScreen.png"),TS_BG_LOADING);
    checkLoadError(textures[TS_LOAD_ICON]->loadFromFile("media/texture/loadIcon.png"),TS_LOAD_ICON);
    checkLoadError(textures[TS_TILESHEET]->loadFromFile("media/texture/tilesheet.png"),TS_TILESHEET);
    checkLoadError(textures[TS_MENU]->loadFromFile("media/texture/sprites/menu_new.png"),TS_MENU);
    checkLoadError(textures[TS_BG]->loadFromFile("media/texture/background.png"),TS_BG);
    checkLoadError(textures[TS_MENU_PAUSE]->loadFromFile("media/texture/sprites/pause_menu_new.png"),TS_MENU_PAUSE);
    checkLoadError(textures[TS_BG_PAUSE]->loadFromFile("media/texture/pause_menu_background.png"),TS_BG_PAUSE);
    checkLoadError(textures[TS_BULLET]->loadFromFile("media/texture/bullet.png"),TS_BULLET);
    checkLoadError(textures[TS_SLASH]->loadFromFile("media/texture/slashEffect.png"),TS_SLASH);
    checkLoadError(textures[TS_SPRITESHEET]->loadFromFile("media/texture/spritesheet.png"),TS_SPRITESHEET);
    checkLoadError(textures[TS_C4]->loadFromFile("media/texture/c4.png"),TS_C4);
}
void TextureStore::free(){
    for(auto i : textures){
        i->free();
    }
}
LTexture* TextureStore::getSourceFor(TsTextureType t){
    return textures[t];
}