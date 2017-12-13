#pragma once
#include "../GenericEntity.h"
#include "../GroundEntity.h"
#include "../EntityManager.h"
#include "MeshBuilder.h"

class Mesh;

class CEnemy : public GenericEntity
{
public:
	CEnemy(void);
	virtual ~CEnemy();

	void Init(void);
	//Reset this player instance to default
	void Reset(void);

	//Set pos
	void SetPos(const Vector3& pos);
	//Set target
	void SetTarget(const Vector3& target);
	//Set up
	void SetUp(const Vector3& up);
	//Set the boundary for the player info
	void SetBoundary(Vector3 max, Vector3 min);
	//Set the terrain for the player info
	void SetTerrain(GroundEntity* m_pTerrain);

	// Get position
	Vector3 GetPos(void) const;
	// Get target
	Vector3 GetTarget(void) const;
	// Get up
	Vector3 GetUp(void) const;
	// Get the terrain for the player info
	GroundEntity* GetTerrain(void);

	//Update 
	void Update(double dt = 0.033f);

	//Constrain the position within the borders
	void Constrain(void);
	// Render
	void Render(void);



private:


protected:
	Vector3 defaultPosition, defaultTarget, defaultUp;
	Vector3 target, up;
	Vector3 maxBoundry, minBoundry;

	GroundEntity* m_pTerrain;

	double m_dSpeed;
	double m_dAcceleration;

};