// f00347_gamephysrig.h
//

#include "f00347_gamephysrig.e"
#define LZZ_INLINE inline
void GamePhysRig::updatePivot (int jointId)
                                      {
		
		
		
		RigJointStruct* curJoint = &(rigJoints[jointId]);
		RigJointStruct* parJoint;
		
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
			parJoint = &(rigJoints[curJoint->parentId]);
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
int GamePhysRig::addJoint (int nodeName, int parentId, int jointType, float mass, GameOrgNode * curNode)
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
		
		rigJoints.push_back(RigJointStruct());
		int curId = rigJoints.size()-1;
		
		RigJointStruct* curJoint = &(rigJoints.back());
		RigJointStruct* parJoint;
		//RigJointStruct* grdJoint;
		
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
void GamePhysRig::initFromOrg (GameOrgNode * curNode, int curParent)
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
void GamePhysRig::reinit ()
                      {
		removeAllBodies();
		initFromOrg(
			baseOrg->baseNode,
			-1
		);
	}
GamePhysRig::GamePhysRig (Singleton * _singleton, int _geId, btDynamicsWorld * ownerWorld, btVector3 const & positionOffset)
          {
		
		int i;
		
		singleton = _singleton;
		m_ownerWorld = ownerWorld;
		geId = _geId;
		//uid = _uid;
		
		origOffset = positionOffset;// - btVector3(0.0f,0.0f,16.0f);
		

		baseOrg = singleton->gem->gameOrgs[
			singleton->gem->gameObjects[geId].orgId	
		];
		
		baseEnt = &(singleton->gem->gameObjects[geId]);

		initFromOrg(
			baseOrg->baseNode,
			-1
		);

	}
void GamePhysRig::removeAllBodies ()
                               {
		int i;

		// Remove all constraints
		for ( i = 0; i < rigJoints.size(); ++i)
		{
			if (rigJoints[i].joint == NULL) {
				
			}
			else {
				m_ownerWorld->removeConstraint(rigJoints[i].joint);
				delete rigJoints[i].joint;
				rigJoints[i].joint = NULL;
			}
		}

		for ( i = 0; i < rigJoints.size(); ++i)
		{
			m_ownerWorld->removeRigidBody(rigJoints[i].body);
			
			delete rigJoints[i].body->getMotionState();
			delete rigJoints[i].body;
			rigJoints[i].body = NULL;
			delete rigJoints[i].shape;
			rigJoints[i].shape = NULL;
		}
		
		rigJoints.clear();
	}
GamePhysRig::~ GamePhysRig ()
        {
		removeAllBodies();
	}
#undef LZZ_INLINE
 
