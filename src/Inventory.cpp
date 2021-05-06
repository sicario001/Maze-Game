#include "Inventory.hpp"

Inventory::Inventory(){
    gunTexture = new LTexture();
    smokeTexture = new LTexture();
    flashTexture = new LTexture();
    grenadeTexture = new LTexture();
    desTextTexture = new LTexture();
    knifeTexture = new LTexture();
    // bombTexture = new LTexture();
    // defuseKitTexture = new LTexture();

}
void Inventory::useBullet(){
    if (mag_rem>0){
        mag_rem--;
    }
}
void Inventory::useFlash(){
    if (flash_rem>0){
        flash_rem--;
    }
}
void Inventory::useGrenade(){
    if (grenade_rem>0){
        grenade_rem--;
    }
}
void Inventory::useSmoke(){
    if (smoke_rem>0){
        smoke_rem--;
    }
}
bool Inventory::isEmptyAmmo(){
    return (total_rem==0);
}
bool Inventory::isEmptyFlash(){
    return (flash_rem==0);
}
bool Inventory::isEmptyGrenade(){
    return (grenade_rem==0);
}
bool Inventory::isEmptyMag(){
    return (mag_rem==0);
}
bool Inventory::canReload(){
    if (currWeapon==GUN && mag_rem!=mag_lim && total_rem>0){
        return true;
    }
    return false;
}
void Inventory::loadMediaInventory(){
    gFont = TTF_OpenFont( "media/fonts/Amatic-Bold.ttf", 50);
    if( !gunTexture->loadFromFile( "media/texture/inventory/AKM.png" ) )
	{
		printf( "Failed to load gun texture!\n" );
	}
    if( !smokeTexture->loadFromFile( "media/texture/inventory/smoke.png" ) )
	{
		printf( "Failed to load smoke texture!\n" );
	}
    if( !flashTexture->loadFromFile( "media/texture/inventory/Flashbang.png" ) )
	{
		printf( "Failed to load flash texture!\n" );
	}
    if( !grenadeTexture->loadFromFile( "media/texture/inventory/Grenade.png" ) )
	{
		printf( "Failed to load grenade texture!\n" );
	}
    if( !knifeTexture->loadFromFile( "media/texture/inventory/Bayonet.png" ) )
	{
		printf( "Failed to load no weapons texture!\n" );
	}
    
    // if( !bombTexture->loadFromFile( "media/texture/AKM.png" ) )
	// {
	// 	printf( "Failed to load bomb texture!\n" );
	// }
    // if( !defuseKitTexture->loadFromFile( "media/texture/AKM.png" ) )
	// {
	// 	printf( "Failed to load bomb texture!\n" );
	// }
}
void Inventory::reload(){
    if ((total_rem+mag_rem)<=mag_lim){
        mag_rem = total_rem+mag_rem;
        total_rem = 0;
    }
    else{
        total_rem -= mag_lim-mag_rem;
        mag_rem = mag_lim;
    }
}
void Inventory::render(){
    desText.str( "" );
	switch (currWeapon){
        case GUN:
            desText<<mag_rem<<" / "<<total_rem;
            gunTexture->render(30, SCREEN_HEIGHT-100, NULL, 1.5);
            break;

        case SMOKE:
            desText<<smoke_rem;
            smokeTexture->render(30, SCREEN_HEIGHT-100, NULL, 1.5);
            break;
        
        case FLASH:
            desText<<flash_rem;
            flashTexture->render(30, SCREEN_HEIGHT-100, NULL, 1.5);
            break;

        case KNIFE:
            knifeTexture->render(30, SCREEN_HEIGHT-100, NULL, 1.5);
            break;
        
        case GRENADE:
            desText<<grenade_rem;
            grenadeTexture->render(30, SCREEN_HEIGHT-100, NULL, 1.5);
            break;
        
        // case BOMB:
        //     bombTexture->render(50, SCREEN_HEIGHT-100, NULL, 1);
        //     break;
        // case DEFUSE_KIT:
        //     defuseKitTexture->render(50, SCREEN_HEIGHT-100, NULL, 1);
        //     break;

    }
    if (currWeapon!=KNIFE){
        desTextTexture->loadFromRenderedText( desText.str().c_str(), SDL_Color{0, 0, 0, 255}, gFont);
        desTextTexture->render(120, SCREEN_HEIGHT-100, NULL, 1);
    }
}

void Inventory::changeWeapon(WeaponType newWeapon){
    currWeapon = newWeapon;
}
WeaponType Inventory::getCurrWeapon(){
    return currWeapon;
}