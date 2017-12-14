#pragma once
#include "Enemy.h"
#include "RenderHelper.h"
#include "GraphicsManager.h"
#include "../LevelOfDetails.h"

CEnemy::CEnemy() 
	: GenericEntity(NULL)
	, defaultPosition(Vector3(0.0f, 0.0f, 0.0f))
	, defaultTarget(Vector3(0.0f, 0.0f, 0.0f))
	, defaultUp(Vector3(0.0f, 0.0f, 0.0f))
	, target(Vector3(0.0f, 0.0f, 0.0f))
	, up(Vector3(0.0f, 0.0f, 0.0f))
	, maxBoundry(Vector3(0.0f, 0.0f, 0.0f))
	, minBoundry(Vector3(0.0f, 0.0f, 0.0f))
	, m_pTerrain(NULL)
{
}

CEnemy::~CEnemy()
{
}

void CEnemy::Init(void)
{
	// Set the default values
	defaultPosition.Set(0, 0, 10);
	defaultTarget.Set(0, 0, 0);
	defaultUp.Set(0, 1, 0);

	// Set the current values
	position.Set(1.f, 0.0f, 1.0f);
	target.Set(10.f, 0.0f, 450.0f);
	up.Set(0.0f, 1.0f, 0.0f);

	// Set boundary
	maxBoundry.Set(1, 1, 1);
	minBoundry.Set(-1, -1, -1);

	// Set speed
	m_dSpeed = 1.0;

	scale.Set(10.f, 10.f, 10.f);

	// Initialise the LOD meshes
	InitLOD("cube", "cubeSG", "sphere");


	// Initialise the Collider
	this->SetCollider(true);
	this->SetAABB(Vector3(1, 1, 1), Vector3(-1, -1, -1));

	//Add to entity manager
	EntityManager::GetInstance()->AddEntity(this, true);

}

void CEnemy::Reset(void)
{
	// Set the current values to default values
	position = defaultPosition;
	target = defaultTarget;
	up = defaultUp;
}

void CEnemy::SetPos(const Vector3 & pos)
{
	position = pos;
}

void CEnemy::SetTarget(const Vector3 & target)
{
	this->target = target;
}

void CEnemy::SetUp(const Vector3 & up)
{
	this->up = up;
}

void CEnemy::SetBoundary(Vector3 max, Vector3 min)
{
	maxBoundry = max;
	minBoundry = min;
}

void CEnemy::SetTerrain(GroundEntity * m_pTerrain)
{
	if (m_pTerrain != NULL)
	{
		this->m_pTerrain = m_pTerrain;

		SetBoundary(this->m_pTerrain->GetMaxBoundary(), this->m_pTerrain->GetMinBoundary());
	}
}

Vector3 CEnemy::GetPos(void) const
{
	return position;
}

Vector3 CEnemy::GetTarget(void) const
{
	return target;
}

Vector3 CEnemy::GetUp(void) const
{
	return up;
}

GroundEntity * CEnemy::GetTerrain(void)
{
	return m_pTerrain;
}

void CEnemy::Update(double dt)
{
	Vector3 viewVector = (target - position).Normalized();
	position += viewVector * (float)m_dSpeed * (float)dt;
	//cout << position << "..." /*<< viewVector*/ <<endl;

	//Constrain the position
	Constrain();

	//Update the target
	if (position.z > 400.f)
	{
	
		target.z = position.z * -1;
	}
	else if (position.z < -400.f)
	{
		target.z = position.z * -1;
	}
}

void CEnemy::Constrain(void)
{
	//Constrain player within boundary
	//if (position.x > maxBoundry.x - 1.f)
	//	position.x = maxBoundry.x - 1.f;
	//if (position.x < maxBoundry.x + 1.f)
	//	position.x = maxBoundry.x + 1.f;
	//if (position.z > maxBoundry.z - 1.f)
	//	position.z = maxBoundry.z - 1.f;
	//if (position.z < maxBoundry.z + 1.f)
	//	position.z = maxBoundry.z + 1.f;

	if (position.x > maxBoundry.x - 1.f)
		position.x = maxBoundry.x - 1.f;
	if (position.z > maxBoundry.z - 1.f)
		position.z = maxBoundry.z - 1.f;
	if (position.x < minBoundry.x + 1.f)
		position.x = minBoundry.x + 1.f;
	if (position.z < minBoundry.z + 1.f)
		position.z = minBoundry.z + 1.f;

	// if the y position is not equal to terrain heigh at that position, 
	// then update y position to the terrain height
	if (position.y != m_pTerrain->GetTerrainHeight(position))
		position.y = m_pTerrain->GetTerrainHeight(position);
}

void CEnemy::Render(void)
{

	MS& modelStack = GraphicsManager::GetInstance()->GetModelStack();
	modelStack.PushMatrix();
	modelStack.Translate(position.x, position.y, position.z);
	modelStack.Scale(scale.x, scale.y, scale.z);
	if (GetLODStatus())
	{
		if (theDetailLevel != NO_DETAILS)
		{
			//cout << theDetailLevel<< endl;
			RenderHelper::RenderMesh(GetLODMesh());
		}
	}
	modelStack.PopMatrix();
}
