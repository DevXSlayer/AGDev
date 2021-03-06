#include "EntityManager.h"
#include "EntityBase.h"
#include "Collider/Collider.h"
#include "Projectile/Laser.h"
#include "SceneGraph\SceneGraph.h"
#include "PlayerInfo\PlayerInfo.h"

#include <iostream>
using namespace std;

// Update all entities
void EntityManager::Update(double _dt)
{
	// Update all entities
	std::list<EntityBase*>::iterator it, end,itProj,endProj;
	end = entityList.end();
	endProj = projectileList.end();

	vector<EntityBase*> ExportList = CSpatialPartition::GetInstance()->GetObjects(CPlayerInfo::GetInstance()->GetPos(), 1);	int num = 0;
	vector<EntityBase*>::iterator ExportListIt,ExportListEnd;
	ExportListEnd = ExportList.end();
	if (!projectileList.empty())
	{
		for (itProj = projectileList.begin(); itProj != endProj; ++itProj)
		{
			(*itProj)->Update(_dt);
		}
	}

	int count = 0;
	// only Update entities that are in the same grid as player
	//for (ExportListIt = ExportList.begin(); ExportListIt != ExportListEnd; ++ExportListIt)
	//{
	//		(*ExportListIt)->Update(_dt);
	//		count++;
	//}
	for (it = entityList.begin(); it != end; ++it)
	{
			(*it)->Update(_dt);

	}
	std::cout << count << std::endl;

	// Render the Scene Graph
	CSceneGraph::GetInstance()->Update();

	// Render the SpatialPartition
	if (theSpatialPartition)
	{
			theSpatialPartition->Update();
	}
	// Check for Collision amongst entities with collider properties
	CheckForCollision();

	// Clean up entities that are done
	it = entityList.begin();
	while (it != end)
	{

		if ((*it)->IsDone())
		{
			// Delete if done
			delete *it;
			it = entityList.erase(it);			
		}
		else
		{
			// Move on otherwise
			++it;
		}
	}

	if (!projectileList.empty())
	{
		itProj = projectileList.begin();
		while (itProj != endProj)
		{

			if ((*itProj)->IsDone())
			{
				// Delete if done
				delete *itProj;
				itProj = projectileList.erase(itProj);
			}
			else
			{
				// Move on otherwise
				++itProj;
			}
		}
	}
}

// Render all entities
void EntityManager::Render()
{
	// Render all entities
	std::list<EntityBase*>::iterator it, end,projIt,projEnd;
	end = entityList.end();
	projEnd = projectileList.end();
	for (it = entityList.begin(); it != end; ++it)
	{
		(*it)->Render();
	} 
	for (projIt = projectileList.begin(); projIt != projEnd; ++projIt)
	{
		(*projIt)->Render();
	}

	// Render the Scene Graph
	CSceneGraph::GetInstance()->Render();

	//Render the spatial Partition 
	if (theSpatialPartition)
		theSpatialPartition->Render();
}

// Render the UI entities
void EntityManager::RenderUI()
{
	// Render all entities UI
	std::list<EntityBase*>::iterator it, end;
	end = entityList.end();
	for (it = entityList.begin(); it != end; ++it)
	{
		(*it)->RenderUI();
	}
}

// Add an entity to this EntityManager
void EntityManager::AddEntity(EntityBase* _newEntity ,bool b_AddToSpatialPartition)
{
	entityList.push_back(_newEntity);

	//add to the spatialpartition
	if (theSpatialPartition && b_AddToSpatialPartition)
		theSpatialPartition->Add(_newEntity);
}

// Remove an entity from this EntityManager
bool EntityManager::RemoveEntity(EntityBase* _existingEntity)
{
	// Find the entity's iterator
	std::list<EntityBase*>::iterator findIter = std::find(entityList.begin(), entityList.end(), _existingEntity);

	// Delete the entity if found
	if (findIter != entityList.end())
	{
		delete *findIter;
		findIter = entityList.erase(findIter);

		// Remove from SceneNode too
		if (CSceneGraph::GetInstance()->DeleteNode(_existingEntity)==false)
		{
			cout << "EntityManager::RemoveEntity: Unable to remove this entity from Scene Graph" << endl;
		}
		else
		{
			//remove the spatialpartition
			if (theSpatialPartition)
				theSpatialPartition->Remove(_existingEntity);
		}

		return true;	
	}
	// Return false if not found
	return false;
}

void EntityManager::AddProjectile(EntityBase * _newEntity, bool b_AddToSpatialPartition)
{
	projectileList.push_back(_newEntity);

	//add to the spatialpartition
	if (theSpatialPartition && b_AddToSpatialPartition)
		theSpatialPartition->Add(_newEntity);
}

bool EntityManager::RemoveProjectile(EntityBase * _existingEntity)
{
	// Find the entity's iterator
	std::list<EntityBase*>::iterator findIter = std::find(projectileList.begin(), projectileList.end(), _existingEntity);

	// Delete the entity if found
	if (findIter != projectileList.end())
	{
		delete *findIter;
		findIter = projectileList.erase(findIter);

		// Remove from SceneNode too
		if (CSceneGraph::GetInstance()->DeleteNode(_existingEntity) == false)
		{
			cout << "EntityManager::RemoveEntity: Unable to remove this entity from Scene Graph" << endl;
		}
		else
		{
			//remove the spatialpartition
			if (theSpatialPartition)
				theSpatialPartition->Remove(_existingEntity);
		}

		return true;
	}
	// Return false if not found
	return false;
}

bool EntityManager::MarkForDeletion(EntityBase * _existingEntity)
{
	//find the entity iterator
	std::list<EntityBase*>::iterator findIter = std::find(entityList.begin(), entityList.end(), _existingEntity);

		if (findIter != entityList.end())
		{
			(*findIter)->SetIsDone(true);
			return true;
		}

	return false;
}

void EntityManager::SetSpatialPartition(CSpatialPartition * theSpatialPartition)
{
	this->theSpatialPartition = theSpatialPartition;
}

// Constructor
EntityManager::EntityManager()
{
}

// Destructor
EntityManager::~EntityManager()
{
	// Clear out the Scene Graph
	CSceneGraph::GetInstance()->Destroy();
}

// Check for overlap
bool EntityManager::CheckOverlap(Vector3 thisMinAABB, Vector3 thisMaxAABB, Vector3 thatMinAABB, Vector3 thatMaxAABB)
{	
	// Check if this object is overlapping that object
	/*
	if (((thatMinAABB.x >= thisMinAABB.x) && (thatMinAABB.x <= thisMaxAABB.x) &&
	(thatMinAABB.y >= thisMinAABB.y) && (thatMinAABB.y <= thisMaxAABB.y) &&
	(thatMinAABB.z >= thisMinAABB.z) && (thatMinAABB.z <= thisMaxAABB.z))
	||
	((thatMaxAABB.x >= thisMinAABB.x) && (thatMaxAABB.x <= thisMaxAABB.x) &&
	(thatMaxAABB.y >= thisMinAABB.y) && (thatMaxAABB.y <= thisMaxAABB.y) &&
	(thatMaxAABB.z >= thisMinAABB.z) && (thatMaxAABB.z <= thisMaxAABB.z)))
	*/
	if (((thatMinAABB >= thisMinAABB) && (thatMinAABB <= thisMaxAABB))
		||
		((thatMaxAABB >= thisMinAABB) && (thatMaxAABB <= thisMaxAABB)))
	{
		return true;
	}

	// Check if that object is overlapping this object
	/*
	if (((thisMinAABB.x >= thatMinAABB.x) && (thisMinAABB.x <= thatMaxAABB.x) &&
	(thisMinAABB.y >= thatMinAABB.y) && (thisMinAABB.y <= thatMaxAABB.y) &&
	(thisMinAABB.z >= thatMinAABB.z) && (thisMinAABB.z <= thatMaxAABB.z))
	||
	((thisMaxAABB.x >= thatMinAABB.x) && (thisMaxAABB.x <= thatMaxAABB.x) &&
	(thisMaxAABB.y >= thatMinAABB.y) && (thisMaxAABB.y <= thatMaxAABB.y) &&
	(thisMaxAABB.z >= thatMinAABB.z) && (thisMaxAABB.z <= thatMaxAABB.z)))
	*/
	if (((thisMinAABB >= thatMinAABB) && (thisMinAABB <= thatMaxAABB))
		||
		((thisMaxAABB >= thatMinAABB) && (thisMaxAABB <= thatMaxAABB)))
	{
		return true;
	}

	// Check if this object is within that object
	/*
	if (((thisMinAABB.x >= thatMinAABB.x) && (thisMaxAABB.x <= thatMaxAABB.x) &&
	(thisMinAABB.y >= thatMinAABB.y) && (thisMaxAABB.y <= thatMaxAABB.y) &&
	(thisMinAABB.z >= thatMinAABB.z) && (thisMaxAABB.z <= thatMaxAABB.z))
	&&
	((thisMaxAABB.x >= thatMinAABB.x) && (thisMaxAABB.x <= thatMaxAABB.x) &&
	(thisMaxAABB.y >= thatMinAABB.y) && (thisMaxAABB.y <= thatMaxAABB.y) &&
	(thisMaxAABB.z >= thatMinAABB.z) && (thisMaxAABB.z <= thatMaxAABB.z)))
	*/
	if (((thisMinAABB >= thatMinAABB) && (thisMaxAABB <= thatMaxAABB))
		&&
		((thisMaxAABB >= thatMinAABB) && (thisMaxAABB <= thatMaxAABB)))
		return true;

	// Check if that object is within this object
	/*
	if (((thatMinAABB.x >= thisMinAABB.x) && (thatMinAABB.x <= thisMaxAABB.x) &&
	(thatMinAABB.y >= thisMinAABB.y) && (thatMinAABB.y <= thisMaxAABB.y) &&
	(thatMinAABB.z >= thisMinAABB.z) && (thatMinAABB.z <= thisMaxAABB.z))
	&&
	((thatMaxAABB.x >= thisMinAABB.x) && (thatMaxAABB.x <= thisMaxAABB.x) &&
	(thatMaxAABB.y >= thisMinAABB.y) && (thatMaxAABB.y <= thisMaxAABB.y) &&
	(thatMaxAABB.z >= thisMinAABB.z) && (thatMaxAABB.z <= thisMaxAABB.z)))
	*/
	if (((thatMinAABB >= thisMinAABB) && (thatMinAABB <= thisMaxAABB))
		&&
		((thatMaxAABB >= thisMinAABB) && (thatMaxAABB <= thisMaxAABB)))
		return true;

	return false;
}

// Check if this entity's bounding sphere collided with that entity's bounding sphere 
bool EntityManager::CheckSphereCollision(EntityBase *ThisEntity, EntityBase *ThatEntity)
{
	// Get the colliders for the 2 entities
	CCollider *thisCollider = dynamic_cast<CCollider*>(ThisEntity);
	CCollider *thatCollider = dynamic_cast<CCollider*>(ThatEntity);

	// Get the minAABB and maxAABB for each entity
	Vector3 thisMinAABB = ThisEntity->GetPosition() + thisCollider->GetMinAABB();
	Vector3 thisMaxAABB = ThisEntity->GetPosition() + thisCollider->GetMaxAABB();
	Vector3 thatMinAABB = ThatEntity->GetPosition() + thatCollider->GetMinAABB();
	Vector3 thatMaxAABB = ThatEntity->GetPosition() + thatCollider->GetMaxAABB();

	// if Radius of bounding sphere of ThisEntity plus Radius of bounding sphere of ThatEntity is 
	// greater than the distance squared between the 2 reference points of the 2 entities,
	// then it could mean that they are colliding with each other.
	if (DistanceSquaredBetween(thisMinAABB, thisMaxAABB) + DistanceSquaredBetween(thatMinAABB, thatMaxAABB) >
		DistanceSquaredBetween(ThisEntity->GetPosition(), ThatEntity->GetPosition()))
	{
		return true;
	}

	return false;
}

// Check if this entity collided with another entity, but both must have collider
bool EntityManager::CheckAABBCollision(EntityBase *ThisEntity, EntityBase *ThatEntity)
{
	// Get the colliders for the 2 entities
	CCollider *thisCollider = dynamic_cast<CCollider*>(ThisEntity);
	CCollider *thatCollider = dynamic_cast<CCollider*>(ThatEntity);

	// Get the minAABB and maxAABB for each entity
	Vector3 thisMinAABB = ThisEntity->GetPosition() + thisCollider->GetMinAABB();
	Vector3 thisMaxAABB = ThisEntity->GetPosition() + thisCollider->GetMaxAABB();
	Vector3 thatMinAABB = ThatEntity->GetPosition() + thatCollider->GetMinAABB();
	Vector3 thatMaxAABB = ThatEntity->GetPosition() + thatCollider->GetMaxAABB();

	// Check for overlap
	if (CheckOverlap(thisMinAABB, thisMaxAABB, thatMinAABB, thatMaxAABB))
		return true;

	// if AABB collision check fails, then we need to check the other corners of the bounding boxes to 
	// Do more collision checks with other points on each bounding box
	Vector3 altThisMinAABB = Vector3(thisMinAABB.x, thisMinAABB.y, thisMaxAABB.z);
	Vector3 altThisMaxAABB = Vector3(thisMaxAABB.x, thisMaxAABB.y, thisMinAABB.z);
//	Vector3 altThatMinAABB = Vector3(thatMinAABB.x, thatMinAABB.y, thatMaxAABB.z);
//	Vector3 altThatMaxAABB = Vector3(thatMaxAABB.x, thatMaxAABB.y, thatMinAABB.z);

	// Check for overlap
	if (CheckOverlap(altThisMinAABB, altThisMaxAABB, thatMinAABB, thatMaxAABB))
		return true;

	return false;
}

// Check where a line segment between two positions intersects a plane
bool EntityManager::GetIntersection(const float fDst1, const float fDst2, Vector3 P1, Vector3 P2, Vector3 &Hit)
{
	if ((fDst1 * fDst2) >= 0.0f) 
		return false;
	if (fDst1 == fDst2) 
		return false;
	Hit = P1 + (P2 - P1) * (-fDst1 / (fDst2 - fDst1));
	return true;
}

// Check two positions are within a box region
bool EntityManager::InBox(Vector3 Hit, Vector3 B1, Vector3 B2, const int Axis)
{
	if (Axis == 1 && Hit.z > B1.z && Hit.z < B2.z && Hit.y > B1.y && Hit.y < B2.y) return true;
	if (Axis == 2 && Hit.z > B1.z && Hit.z < B2.z && Hit.x > B1.x && Hit.x < B2.x) return true;
	if (Axis == 3 && Hit.x > B1.x && Hit.x < B2.x && Hit.y > B1.y && Hit.y < B2.y) return true;
	return false;
}

// Check for intersection between a line segment and a plane
bool EntityManager::CheckLineSegmentPlane(	Vector3 line_start, Vector3 line_end, 
											Vector3 minAABB, Vector3 maxAABB,
											Vector3 &Hit)
{
	if ((GetIntersection(line_start.x - minAABB.x, line_end.x - minAABB.x, line_start, line_end, Hit) && 
			InBox(Hit, minAABB, maxAABB, 1))
		|| (GetIntersection(line_start.y - minAABB.y, line_end.y - minAABB.y, line_start, line_end, Hit) && 
			InBox(Hit, minAABB, maxAABB, 2))
		|| (GetIntersection(line_start.z - minAABB.z, line_end.z - minAABB.z, line_start, line_end, Hit) && 
			InBox(Hit, minAABB, maxAABB, 3))
		|| (GetIntersection(line_start.x - maxAABB.x, line_end.x - maxAABB.x, line_start, line_end, Hit) && 
			InBox(Hit, minAABB, maxAABB, 1))
		|| (GetIntersection(line_start.y - maxAABB.y, line_end.y - maxAABB.y, line_start, line_end, Hit) && 
			InBox(Hit, minAABB, maxAABB, 2))
		|| (GetIntersection(line_start.z - maxAABB.z, line_end.z - maxAABB.z, line_start, line_end, Hit) && 
			InBox(Hit, minAABB, maxAABB, 3)))
		return true;

	return false;
}

// Check if any Collider is colliding with another Collider
bool EntityManager::CheckForCollision(void)
{
	// Check for Collision
	std::list<EntityBase*>::iterator colliderThis, colliderThisEnd;
	std::list<EntityBase*>::iterator colliderThat, colliderThatEnd;

	int counter = 0;

	if (!projectileList.empty())
	{
		for (std::list<EntityBase*>::iterator projIT = projectileList.begin(); projIT != projectileList.end(); projIT++)
		{
			if ((*projIT)->GetIsLaser())
			{
			EntityBase *projEntity = dynamic_cast<EntityBase*>(*projIT);
			//CLaser* thisEntity = dynamic_cast<EntityBase*>(*colliderThis);

				for (std::list<EntityBase*>::iterator entityIT = entityList.begin(); entityIT != entityList.end(); entityIT++)
				{
					if ((*entityIT)->HasCollider())
					{
						EntityBase *itEntity = dynamic_cast<EntityBase*>(*entityIT);

						if (CheckSphereCollision(projEntity, itEntity))
						{
							if (CheckAABBCollision(projEntity, itEntity))
							{
								cout << "HERE";
								projEntity->SetIsDone(true);
								itEntity->SetIsDone(true);

								CSceneGraph::GetInstance()->DeleteNode((*entityIT));
								CSceneGraph::GetInstance()->DeleteNode((*projIT));
								// Remove from Scene Graph
								if (CSceneGraph::GetInstance()->DeleteNode((*entityIT)) == true)
								{
									cout << "*** This Entity removed ***" << endl;
								}
								// Remove from Scene Graph
								if (CSceneGraph::GetInstance()->DeleteNode((*projIT)) == true)
								{
									cout << "*** That Entity removed ***" << endl;
								}
							}
						}
					}
				}
			}

			else if ((*projIT)->HasCollider())
			{
				EntityBase *projEntity = dynamic_cast<EntityBase*>(*projIT);

					for (std::list<EntityBase*>::iterator entityIT = entityList.begin(); entityIT != entityList.end(); entityIT++)
					{
						if ((*entityIT)->HasCollider())
						{
							EntityBase *itEntity = dynamic_cast<EntityBase*>(*entityIT);

						if (CheckSphereCollision(projEntity, itEntity))
						{
							if (CheckAABBCollision(projEntity, itEntity))
							{
								cout << "HERE";
								projEntity->SetIsDone(true);
								itEntity->SetIsDone(true);

								CSceneGraph::GetInstance()->DeleteNode((*entityIT));
								CSceneGraph::GetInstance()->DeleteNode((*projIT));
								// Remove from Scene Graph
								if (CSceneGraph::GetInstance()->DeleteNode((*entityIT)) == true)
								{
									cout << "*** This Entity removed ***" << endl;
								}
								// Remove from Scene Graph
								if (CSceneGraph::GetInstance()->DeleteNode((*projIT)) == true)
								{
									cout << "*** That Entity removed ***" << endl;
								}
							}
						}
					}
				}
			}
		}
	}
	else
	{
		colliderThisEnd = entityList.end();
		for (colliderThis = entityList.begin(); colliderThis != colliderThisEnd; ++colliderThis)
		{
			// Check if this entity is a CLaser type
			{
				//if ((*colliderThis)->GetIsLaser())
				//{
				//	// Dynamic cast it to a CLaser class
				//	CLaser* thisEntity = dynamic_cast<CLaser*>(*colliderThis);

				//	// Check for collision with another collider class
				//	colliderThatEnd = entityList.end();
				//	for (colliderThat = entityList.begin(); colliderThat != colliderThatEnd; ++colliderThat)
				//	{
				//		if (colliderThat == colliderThis)
				//			continue;

				//		if ((*colliderThat)->HasCollider())
				//		{
				//			Vector3 hitPosition = Vector3(0, 0, 0);

				//			// Get the minAABB and maxAABB for (*colliderThat)
				//			CCollider *thatCollider = dynamic_cast<CCollider*>(*colliderThat);
				//			Vector3 thatMinAABB = (*colliderThat)->GetPosition() + thatCollider->GetMinAABB();
				//			Vector3 thatMaxAABB = (*colliderThat)->GetPosition() + thatCollider->GetMaxAABB();

				//			if (CheckLineSegmentPlane(thisEntity->GetPosition(),
				//				thisEntity->GetPosition() - thisEntity->GetDirection() * thisEntity->GetLength(),
				//				thatMinAABB, thatMaxAABB,
				//				hitPosition) == true)
				//			{
				//				(*colliderThis)->SetIsDone(true);
				//				(*colliderThat)->SetIsDone(true);


				//				// Remove from Scene Graph
				//				if (CSceneGraph::GetInstance()->DeleteNode((*colliderThis)) == true)
				//				{
				//					cout << "*** This Entity removed ***" << endl;
				//				}
				//				// Remove from Scene Graph
				//				if (CSceneGraph::GetInstance()->DeleteNode((*colliderThat)) == true)
				//				{
				//					cout << "*** That Entity removed ***" << endl;
				//				}

				//			}
				//		}
				//	}
				//}
			}
			if ((*colliderThis)->HasCollider())
			{
				// This object was derived from a CCollider class, then it will have Collision Detection methods
				//CCollider *thisCollider = dynamic_cast<CCollider*>(*colliderThis);
				EntityBase *thisEntity = dynamic_cast<EntityBase*>(*colliderThis);

				// Check for collision with another collider class
				colliderThatEnd = entityList.end();
				for (colliderThat = entityList.begin(); colliderThat != colliderThatEnd; ++colliderThat)
				{

					if (colliderThat == colliderThis)
						continue;
					if ((*colliderThat)->HasCollider())
					{
						EntityBase *thatEntity = dynamic_cast<EntityBase*>(*colliderThat);
						if (CheckSphereCollision(thisEntity, thatEntity))
						{
							if (CheckAABBCollision(thisEntity, thatEntity))

							{
								cout << "HERE";
								thisEntity->SetIsDone(true);
								thatEntity->SetIsDone(true);

								CSceneGraph::GetInstance()->DeleteNode((*colliderThis));
								CSceneGraph::GetInstance()->DeleteNode((*colliderThat));

								// Remove from Scene Graph
								if (CSceneGraph::GetInstance()->DeleteNode((*colliderThis)) == true)
								{
									cout << "*** This Entity removed ***" << endl;
								}
								// Remove from Scene Graph
								if (CSceneGraph::GetInstance()->DeleteNode((*colliderThat)) == true)
								{
									cout << "*** That Entity removed ***" << endl;
								}
							}

						}
					}
				}
			}
		}
	}

	return false;
}
