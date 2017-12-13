#include "GrenadeThrow.h"
#include "Grenade.h"

CGrenadeThrow::CGrenadeThrow()
{
}

CGrenadeThrow::~CGrenadeThrow()
{
}

void CGrenadeThrow::Init(void)
{

	// Call the parent's init method
	CWeaponInfo::Init();

	//Number of ammmunition in a magazine for this weapon
	magRounds = 3;
	//Max Number of ammunition for this magazine for this weapon
	maxMagRounds = 3;
	//Current total number of rounds currently carried by this player
	totalRounds = 3;
	//Max total number of rounds currently carried by this player
	maxTotalRounds = 3;

	//Time bewtween shots
	timeBetweenShots = 3;
	//The elapsed time (between shots)
	elapsedTime = 3;
	// Boolean flag to indicate if weapon can fire now
	bFire = true;



}

void CGrenadeThrow::Discharge(Vector3 position, Vector3 target, CPlayerInfo * _source)
{	
	if (bFire)
	{
		// If there is still ammo in the magazine, then fire
		if (magRounds > 0)
		{
			Vector3 _direction = (target - position).Normalized();
			// Create a laser with a laser mesh. The length is 10.0f, mesh is also set at 10.0f
			CGrenade* aGrenade = Create::Grenade("cube",position,_direction,10.f,10.f,_source);
			//aGrenade->SetIsLaser(true);
			aGrenade->SetCollider(false);
			//aLaser->SetCollider(true);
			//aLaser->SetAABB(Vector3(0.5f, 0.5f, 0.5f), Vector3(-0.5f, -0.5f, -0.5f));
			bFire = false;
			magRounds--;
		}
	}
}
