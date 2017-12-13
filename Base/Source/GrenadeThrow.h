#ifndef GRENADE_THROW_H
#define GRENADE_THROW_H

#include "WeaponInfo\WeaponInfo.h"

class CGrenadeThrow : public CWeaponInfo
{
public:
	CGrenadeThrow();
	virtual ~CGrenadeThrow();
	
	//Initialise this instance to default values
	void Init(void);
	//Discharge this weapon
	void Discharge(Vector3 position, Vector3 target, CPlayerInfo* _source = NULL);

private:
	 

};


#endif // !
