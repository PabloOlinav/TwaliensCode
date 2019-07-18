// All rights reserved.

#include "BasicWarriorAnimationData.h"

FBasicWarriorAnimationData::FBasicWarriorAnimationData()
{
	HasTakenDamage = false;
	IsDead = false;
	IsUsingTheirWeapon = false;
	IsDisconnected = false;
}

FBasicWarriorAnimationData::FBasicWarriorAnimationData(bool NewHasTakenDamage, bool NewIsDeath, bool NewIsUsingTheirWeapon, bool NewIsDisconnected)
{
	HasTakenDamage = NewHasTakenDamage;
	IsDead = NewIsDeath;
	IsUsingTheirWeapon = NewIsUsingTheirWeapon;
	IsDisconnected = NewIsDisconnected;
}
