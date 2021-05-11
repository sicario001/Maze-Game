#include "game/Inventory.hpp"

Inventory::Inventory(){
    rifleTexture = new LTexture();
    pistolTexture = new LTexture();
    // smokeTexture = new LTexture();
    // flashTexture = new LTexture();
    // grenadeTexture = new LTexture();
    desTextTexture = new LTexture();
    knifeTexture = new LTexture();
    // bombTexture = new LTexture();
    // defuseKitTexture = new LTexture();

}
void Inventory::useBullet(){
    switch (currWeapon)
    {
    case RIFLE:
        if (mag_rem_rifle>0){
            mag_rem_rifle--;
        }
        break;
    case PISTOL:
        if (mag_rem_pistol>0){
            mag_rem_pistol--;
        }
        break;
    
    default:
        break;
    }
}
// void Inventory::useFlash(){
//     if (flash_rem>0){
//         flash_rem--;
//     }
// }
// void Inventory::useGrenade(){
//     if (grenade_rem>0){
//         grenade_rem--;
//     }
// }
// void Inventory::useSmoke(){
//     if (smoke_rem>0){
//         smoke_rem--;
//     }
// }
bool Inventory::isEmptyAmmo(){
    switch (currWeapon)
    {
    case PISTOL:
        return (total_rem_pistol==0);
    case RIFLE:
        return (total_rem_rifle==0);
    default:
        return (total_rem_rifle==0);
    }
}
// bool Inventory::isEmptyFlash(){
//     return (flash_rem==0);
// }
// bool Inventory::isEmptyGrenade(){
//     return (grenade_rem==0);
// }
bool Inventory::isEmptyMag(){
    switch (currWeapon)
    {
    case PISTOL:
        return (mag_rem_pistol==0);
    case RIFLE:
        return (mag_rem_rifle==0);
    default:
        return (mag_rem_rifle==0);
    }
    return (mag_rem_rifle==0);
}
bool Inventory::canReload(){
    if (currWeapon==RIFLE && mag_rem_rifle!=mag_lim_rifle && total_rem_rifle>0){
        return true;
    }
    if (currWeapon==PISTOL && mag_rem_pistol!=mag_lim_pistol && total_rem_pistol>0){
        return true;
    }
    return false;
}
void Inventory::loadMediaInventory(){
    gFont = TTF_OpenFont( "media/fonts/Amatic-Bold.ttf", 50);
    if( !rifleTexture->loadFromFile( "media/texture/inventory/AKM.png" ) )
	{
		printf( "Failed to load gun texture!\n" );
	}
    if( !pistolTexture->loadFromFile( "media/texture/inventory/FN45.png" ) )
	{
		printf( "Failed to load gun texture!\n" );
	}
    // if( !smokeTexture->loadFromFile( "media/texture/inventory/smoke.png" ) )
	// {
	// 	printf( "Failed to load smoke texture!\n" );
	// }
    // if( !flashTexture->loadFromFile( "media/texture/inventory/Flashbang.png" ) )
	// {
	// 	printf( "Failed to load flash texture!\n" );
	// }
    // if( !grenadeTexture->loadFromFile( "media/texture/inventory/Grenade.png" ) )
	// {
	// 	printf( "Failed to load grenade texture!\n" );
	// }
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
    switch (currWeapon)
    {
    case RIFLE:
        if ((total_rem_rifle+mag_rem_rifle)<=mag_lim_rifle){
            mag_rem_rifle = total_rem_rifle+mag_rem_rifle;
            total_rem_rifle = 0;
        }
        else{
            total_rem_rifle -= mag_lim_rifle-mag_rem_rifle;
            mag_rem_rifle = mag_lim_rifle;
        }
        break;
    case PISTOL:
        if ((total_rem_pistol+mag_rem_pistol)<=mag_lim_pistol){
            mag_rem_pistol = total_rem_pistol+mag_rem_pistol;
            total_rem_pistol = 0;
        }
        else{
            total_rem_pistol -= mag_lim_pistol-mag_rem_pistol;
            mag_rem_pistol = mag_lim_pistol;
        }
        break;
    
    default:
        break;
    }
}
void Inventory::render(){
    desText.str( "" );
	switch (currWeapon){
        case RIFLE:
            desText<<mag_rem_rifle<<" / "<<total_rem_rifle;
            rifleTexture->render(30, SCREEN_HEIGHT-100, NULL, 1.5);
            break;

        case PISTOL:
            desText<<mag_rem_pistol<<" / "<<total_rem_pistol;
            pistolTexture->render(30, SCREEN_HEIGHT-100, NULL, 1.5);
            break;

        // case SMOKE:
        //     desText<<smoke_rem;
        //     smokeTexture->render(30, SCREEN_HEIGHT-100, NULL, 1.5);
        //     break;
        
        // case FLASH:
        //     desText<<flash_rem;
        //     flashTexture->render(30, SCREEN_HEIGHT-100, NULL, 1.5);
        //     break;

        case KNIFE:
            knifeTexture->render(30, SCREEN_HEIGHT-100, NULL, 1.5);
            break;
        
        // case GRENADE:
        //     desText<<grenade_rem;
        //     grenadeTexture->render(30, SCREEN_HEIGHT-100, NULL, 1.5);
        //     break;
        
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


int Inventory::getReloadDuration(){
    switch (currWeapon)
    {
    case RIFLE:
        return 10000;    
    case PISTOL:
        return 5000;    
    default:
        return 0; 
    }
}
int Inventory::getFireLockDuration(){
    return firelockvals[currWeapon];
}
int Inventory::getDamage(){
    return damagevals[currWeapon];
}
