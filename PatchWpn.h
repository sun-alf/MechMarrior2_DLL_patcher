#ifndef _PATCHWPN_H_
#define _PATCHWPN_H_

#define SHORT_NAME_LENGTH    (8)


typedef struct {
    unsigned int newDword0;
    unsigned int newDword1;
    unsigned int newDword2;
    unsigned int newDword3;
	unsigned int lShotType;
    unsigned int lHitType;
    unsigned int lUnknown1;
    unsigned int lUnknown2;
    unsigned int lContinuousFiring;
    unsigned int lWeaponEnabled;
    unsigned int lHoming;
    unsigned int lShotsPerAttack;
    unsigned int lAttacksPerTon;
	unsigned int lSoundFX;
    unsigned int lShotGravity;
    unsigned int lShotSpeed;
    unsigned int lDamageInflictedPerShot;
    unsigned int lHeatGeneratedPerShot;
    unsigned int lHeatInflictedPerShot;
    unsigned int lMinLockRange;
    unsigned int lRange;
    unsigned int lReloadTime;
    unsigned int lFiringDelay;
    unsigned int lShotLifespan;
    char sShortName[SHORT_NAME_LENGTH];
} tWeapon;

typedef struct {
	unsigned int lShellUnknown0;
	
	unsigned int lHeatGeneratedPerShot;
    unsigned int lDamageInflictedPerShot;
    unsigned int lShellUnknown1;
    unsigned int lShellUnknown2;
    unsigned int lShellUnknown3;
    unsigned int lRange;
    unsigned int lMass;
    unsigned int lCritical;
    unsigned int lAttacksPerTon;
    unsigned int lShellUnknown4;
    
    unsigned int lShellUnknown5;
    unsigned int lShellUnknown6;
    unsigned int lShellUnknown7;
    unsigned int lShellUnknown8;
    unsigned int lShellUnknown9;
} tShellWeapon;

#define NEGATIVEOFFSETFROMNAME (20 * 4)

#endif  // _PATCHWPN_H_
