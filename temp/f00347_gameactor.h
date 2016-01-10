// f00347_gameactor.h
//

#include "f00347_gameactor.e"
#define LZZ_INLINE inline
void GameActor::updatePivot (int jointId)
                                      {
		
		
		
		ActorJointStruct* curJoint = &(actorJoints[jointId]);
		ActorJointStruct* parJoint;
		
		curJoint->isFixed = (baseEnt->entType == E_ENTTYPE_WEAPON);
		
		// btPoint2PointConstraint* ballC; //btFixedConstraint
		// btFixedConstraint* fixedC;
		btVector3 pivotA;
		btVector3 pivotB;
		btTransform localA, localB, localC;
		
		if (curJoint->joint == NULL) {
			
		}
		else {
			m_ownerWorld->removeConstraint(curJoint->joint);
			delete curJoint->joint;
			curJoint->joint = NULL;
		}
		
		if (curJoint->parentId < 0) { //
			curJoint->joint = NULL;
			//curJoint->body->setAngularFactor(btVector3(0.0f,0.0f,0.0f));
			//curJoint->body->setLinearFactor(btVector3(0.1f,0.1f,0.1f));
			
		}
		else {
			parJoint = &(actorJoints[curJoint->parentId]);
			pivotA = btVector3(-parJoint->length*0.5f,0.0,0.0);
			pivotB = btVector3( curJoint->length*0.5f,0.0,0.0);
			

			
			localA.setIdentity();
			localB.setIdentity();
			localA.setOrigin(pivotA);
			localB.setOrigin(pivotB);
			localA.getBasis() = parJoint->basis; 
			localB.getBasis() = curJoint->basis; 
			//localA.setRotation(parJoint->quat);
			//localB.setRotation(curJoint->quat);
			
			
			// if (curJoint->isFixed) {
			// 	curJoint->joint = new btFixedConstraint(
			// 		*(parJoint->body),
			// 		*(curJoint->body),
			// 		localA,
			// 		localB
			// 	);
			// }
			// else {
				curJoint->joint = new btPoint2PointConstraint(
					*(parJoint->body),
					*(curJoint->body),
					pivotA,
					pivotB
				);
			//}
			
			
			
			
			m_ownerWorld->addConstraint(curJoint->joint, true);
			
			
		}
	}
int GameActor::addJoint (int nodeName, int parentId, int jointType, float mass, GameOrgNode * curNode)
          {
		
		int i;
		
		
		float rad = 0.0f;
		float len = 0.0f;
		
		btVector3 begPos;
		btVector3 midPos;
		btVector3 endPos;
		
		//int colInd = geId % MAX_COL_BODY;
		int colType = bodyCollidesWith;//[colInd];
		int colBase = COL_BODY;//0<<(colInd);
		
		
		
		switch(jointType) {
			case E_JT_LIMB:
				rad = 0.25f;
				len = curNode->orgTrans[0].getBTV().distance(curNode->orgTrans[2].getBTV());
				begPos = curNode->orgTrans[0].getBTV();
				midPos = curNode->orgTrans[1].getBTV();
				endPos = curNode->orgTrans[2].getBTV();
			break;
			case E_JT_BALL:
				rad = 0.05f;
				begPos = curNode->orgTrans[2].getBTV();
				midPos = curNode->orgTrans[2].getBTV();
				endPos = curNode->orgTrans[2].getBTV();
			break;
			case E_JT_NORM:
				// rad = 0.05f;
				// begPos = curNode->orgTrans[1].getBTV() + curNode->orgVecs[E_OV_NORMAL];
				// midPos = begPos;
				// endPos = begPos;
			break;
		}
		
		
		btVector3 targAlignT = curNode->tbnRotC[0].getBTV();
		btVector3 targAlignB = curNode->tbnRotC[1].getBTV();
		btVector3 targAlignN = curNode->tbnRotC[2].getBTV();
		
		
		
		
		
		btVector3 vUp(0, 0, 1);
		
		// btGeneric6DofSpringConstraint* springC;
		// btHingeConstraint* hingeC;
		// btConeTwistConstraint* coneC;

		btTransform localA, localB, localC;
		
		actorJoints.push_back(ActorJointStruct());
		int curId = actorJoints.size()-1;
		
		ActorJointStruct* curJoint = &(actorJoints.back());
		ActorJointStruct* parJoint;
		//ActorJointStruct* grdJoint;
		
		curJoint->boneId = nodeName;
		curJoint->jointType = jointType;
		curJoint->rad = rad;
		curJoint->length = len;
		//begOrig
		//endOrig
		curJoint->targAlignT = targAlignT;
		curJoint->targAlignB = targAlignB;
		curJoint->targAlignN = targAlignN;
		curJoint->jointId = curId;
		curJoint->parentId = parentId;
		
		//float jointSpace  = curJoint->rad*2.0f;
		
		float curLength = curJoint->length;// - curJoint->rad*4.0;
		
		if (parentId < 0) {
			curLength = curJoint->length;
		}
		
		switch(jointType) {
			case E_JT_LIMB:
				curJoint->shape = new btCapsuleShapeX(curJoint->rad, curLength);
			break;
			case E_JT_BALL:
				curJoint->shape = new btSphereShape(curJoint->rad);
			break;
			case E_JT_NORM:
				
			break;
		}
		
		
		
		btTransform transform;
		btTransform offset;
		
		
		
		
		
		offset.setIdentity();
		offset.setOrigin(origOffset);
		
		//btVector3 begPos = getStartPosition(curId);
		//btVector3 endPos = begPos + curJoint->targAlignT*(curJoint->length);
		//btVector3 midPos = (begPos+endPos)*0.5f;
		
		curJoint->begOrig = begPos;
		curJoint->midOrig = midPos;
		curJoint->endOrig = endPos;


		
		transform.setIdentity();
		transform.setOrigin(midPos);
		
		
		
		btVector3 axis;
		btScalar angle;
		btQuaternion quat;
		
		
		btVector3 vectorA = vUp;
		btVector3 vectorB = endPos-begPos;
		
		
		switch(jointType) {
			case E_JT_LIMB:
				vectorB.normalize();
			break;
			case E_JT_BALL:
				
			break;
			case E_JT_NORM:
				
			break;
		}
		
		
		// if ( abs(vectorA.dot(vectorB)) == 1.0f ) {
		// 	// todo: handle (anti)parallel case
		// 	//curJoint->pivotAxis = btVector3(0.0f,0.0f,0.0f);
		// 	curJoint->quat = btQuaternion(btVector3(0.0f,0.0f,1.0f), 0.0f);
		// }
		// else {
		// 	axis = (vectorA.cross(vectorB)).normalized();
		// 	angle = btAcos(vectorA.dot(vectorB)) / (vectorA.length() * vectorB.length());
		// 	curJoint->quat = btQuaternion(axis, angle);
		// 	transform.setRotation(curJoint->quat);
		// 	//curJoint->pivotAxis = axis;
		// }
		
		btMatrix3x3 basis = btMatrix3x3(
			targAlignT.getX(), targAlignT.getY(), targAlignT.getZ(),
			targAlignB.getX(), targAlignB.getY(), targAlignB.getZ(),
			targAlignN.getX(), targAlignN.getY(), targAlignN.getZ()
		);
		curJoint->basis = basis;
		transform.getBasis() = basis;
		
		
		/*
		setEulerZYX params in order: 
		eulerX	Roll about X axis
		eulerY	Pitch about Y axis
		eulerZ	Yaw about Z axis

		*/
		
		
		// xx, xy, xz,
		// yx, yy, yz,
		// zx, zy, zz
		
		// transform.getBasis().setValue(
			
		// );
		
		
		
		
		
		
		curJoint->body = singleton->gamePhysics->example->createRigidBodyMask(
			mass,
			offset*transform,
			curJoint->shape,
			colBase,
			colType
		);
		//curJoint->body->bodyUID = uid;
		//curJoint->body->setDamping(0.05, 0.85);
		curJoint->body->setDeactivationTime(0.8);
		curJoint->body->setSleepingThresholds(0.5f, 0.5f);

		
		
		// curJoint->joint = NULL;
		// return curId;
		
		curJoint->joint = NULL;
		updatePivot(curJoint->jointId);
		
		
		return curId;
		
	}
void GameActor::initFromOrg (GameOrgNode * curNode, int curParent)
          {
		
		
		int i;
		
		float curMass = MASS_PER_LIMB;
		
		
		
		int curChild = addJoint(
			curNode->nodeName,
			curParent,
			E_JT_LIMB,
			curMass,
			curNode
		);
		
		int curChild2 = addJoint(
			curNode->nodeName,
			curChild,
			E_JT_BALL,
			curMass,
			curNode
			
		);
		
		// int curChild3 = addJoint(
		// 	curNode->nodeName,
		// 	curChild,
		// 	E_JT_NORM,
		// 	MASS_PER_LIMB,
		// 	curNode
		// );
		
		
		
		for (i = 0; i < curNode->children.size(); i++) {
			initFromOrg(
				curNode->children[i],
				curChild2
			);
		}
		
	}
void GameActor::reinit ()
                      {
		removeAllBodies();
		initFromOrg(
			baseOrg->baseNode,
			-1
		);
	}
GameActor::GameActor (Singleton * _singleton, int _geId, btDynamicsWorld * ownerWorld, btVector3 const & positionOffset)
          {
		
		int i;
		
		singleton = _singleton;
		m_ownerWorld = ownerWorld;
		geId = _geId;
		//uid = _uid;
		
		origOffset = positionOffset;// - btVector3(0.0f,0.0f,16.0f);
		float actorScale = 1.0f;

		baseOrg = singleton->gem->gameOrgs[
			singleton->gem->gameObjects[geId].orgId	
		];
		
		baseEnt = &(singleton->gem->gameObjects[geId]);

		initFromOrg(
			baseOrg->baseNode,
			-1
		);

	}
void GameActor::removeAllBodies ()
                               {
		int i;

		// Remove all constraints
		for ( i = 0; i < actorJoints.size(); ++i)
		{
			if (actorJoints[i].joint == NULL) {
				
			}
			else {
				m_ownerWorld->removeConstraint(actorJoints[i].joint);
				delete actorJoints[i].joint;
				actorJoints[i].joint = NULL;
			}
		}

		for ( i = 0; i < actorJoints.size(); ++i)
		{
			m_ownerWorld->removeRigidBody(actorJoints[i].body);
			
			delete actorJoints[i].body->getMotionState();
			delete actorJoints[i].body;
			actorJoints[i].body = NULL;
			delete actorJoints[i].shape;
			actorJoints[i].shape = NULL;
		}
		
		actorJoints.clear();
	}
GameActor::~ GameActor ()
        {
		removeAllBodies();
	}
#undef LZZ_INLINE
 
