// f00347_gameactor.h
//

#include "f00347_gameactor.e"
#define LZZ_INLINE inline
int GameActor::addJoint (int nodeName, int parentId, bool isBall, float mass, GameOrgNode * curNode)
          {
		
		int i;
		
		
		float rad = 0.0f;
		float len = 0.0f;
		
		btVector3 begPos;
		btVector3 midPos;
		btVector3 endPos;
		
		if (isBall) {
			rad = 0.05f;
			begPos = curNode->orgTrans[2].getBTV();
			midPos = curNode->orgTrans[2].getBTV();
			endPos = curNode->orgTrans[2].getBTV();
		}
		else {
			rad = 0.1f;
			len = curNode->orgTrans[0].getBTV().distance(curNode->orgTrans[2].getBTV());
			begPos = curNode->orgTrans[0].getBTV();
			midPos = curNode->orgTrans[1].getBTV();
			endPos = curNode->orgTrans[2].getBTV();
		}
		
		btVector3 targAlignT = curNode->tbnRotC[0].getBTV();
		btVector3 targAlignB = curNode->tbnRotC[1].getBTV();
		btVector3 targAlignN = curNode->tbnRotC[2].getBTV();
		
		
		
		
		
		btVector3 vUp(0, 0, 1);
		btVector3 pivotA;
		btVector3 pivotB;
		
		btGeneric6DofSpringConstraint* springC;
		btHingeConstraint* hingeC;
		btPoint2PointConstraint* ballC; //btFixedConstraint
		btConeTwistConstraint* coneC;

		btTransform localA, localB, localC;
		
		actorJoints.push_back(ActorJointStruct());
		int curId = actorJoints.size()-1;
		
		ActorJointStruct* curJoint = &(actorJoints.back());
		ActorJointStruct* parJoint;
		//ActorJointStruct* grdJoint;
		
		curJoint->boneId = nodeName;
		curJoint->isBall = isBall;
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
		
		if (isBall) {
			curJoint->shape = new btSphereShape(curJoint->rad);
		}
		else {
			curJoint->shape = new btCapsuleShapeX(curJoint->rad, curLength);
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
		
		if (!isBall) {
			vectorB.normalize();
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
			COL_BODY,
			bodyCollidesWith
		);
		//curJoint->body->bodyUID = uid;
		//curJoint->body->setDamping(0.05, 0.85);
		curJoint->body->setDeactivationTime(0.8);
		curJoint->body->setSleepingThresholds(0.5f, 0.5f);
		// if (curJoint->isBall) {	
		// 	curJoint->body->setAngularFactor(btVector3(0.0f,0.0f,0.0f));
		// }
		
		
		
		// curJoint->joint = NULL;
		// return curId;
		

		if (parentId < 0) { //
			curJoint->joint = NULL;
			//curJoint->body->setAngularFactor(btVector3(0.0f,0.0f,0.0f));
			//curJoint->body->setLinearFactor(btVector3(0.1f,0.1f,0.1f));
			
		}
		else {
			parJoint = &(actorJoints[parentId]);
			pivotA = btVector3(-parJoint->length*0.5f,0.0,0.0);
			pivotB = btVector3(curJoint->length*0.5f,0.0,0.0);
			

				
				localA.setIdentity();
				localB.setIdentity();
				localA.setOrigin(pivotA);
				localB.setOrigin(pivotB);
				localA.getBasis() = parJoint->basis; 
				localB.getBasis() = curJoint->basis; 
				//localA.setRotation(parJoint->quat);
				//localB.setRotation(curJoint->quat);
				
				
				ballC = new btPoint2PointConstraint(
					*(parJoint->body),
					*(curJoint->body),
					pivotA,
					pivotB
					//localA,
					//localB
				);
				curJoint->joint = ballC;
			
			
			m_ownerWorld->addConstraint(curJoint->joint, true);
			
			
			
			
			
		}
		
		
		return curId;
		
	}
void GameActor::initFromOrg (GameOrgNode * curNode, int curParent)
          {
		
		
		int i;
		
		// FIVector4 lineSeg0;
		// FIVector4 lineSeg1;
		// FIVector4 lineSeg2;
		// lineSeg0.setFXYZRef(&(curNode->orgTrans[0]));

		
		
		int curChild = addJoint(
			curNode->nodeName,
			curParent,					//int parentId,
			false,
			MASS_PER_LIMB,				//float mass,
			//tn, bn, nn,
		
			curNode
		
			// 0.15f,						//float rad,
			// curNode->orgTrans[0].getBTV().distance(curNode->orgTrans[2].getBTV()), //curLen, // +curRad*4.0f,			//float len,
			
		
			// curNode->tbnRotC[0].getBTV(),
			// curNode->tbnRotC[1].getBTV(),
			// curNode->tbnRotC[2].getBTV(),
			
			// curNode->orgTrans[0].getBTV(),
			// curNode->orgTrans[1].getBTV(),
			// curNode->orgTrans[2].getBTV()
			
		);
		
		int curChild2 = addJoint(
			curNode->nodeName,
			curChild,					//int parentId,
			true,
			MASS_PER_LIMB,				//float mass,
			//tn, bn, nn,
			
			curNode
		
			// 0.2f,			//float rad,
			// 0.0f,			//curNode->orgTrans[0].getBTV().distance(curNode->orgTrans[2].getBTV()), //curLen,
			
		
			// curNode->tbnRotC[0].getBTV(),
			// curNode->tbnRotC[1].getBTV(),
			// curNode->tbnRotC[2].getBTV(),
			
			// curNode->orgTrans[2].getBTV(), // beg
			// curNode->orgTrans[2].getBTV(), // mid
			// curNode->orgTrans[2].getBTV()  // end
			
		);
		
		
		
		for (i = 0; i < curNode->children.size(); i++) {
			initFromOrg(
				curNode->children[i],
				curChild2
				//basePosition,
				//scale,
				//drawMode,
				//drawAll
			);
		}
		
	}
GameActor::GameActor (Singleton * _singleton, int _geId, btDynamicsWorld * ownerWorld, btVector3 const & positionOffset, bool bFixed)
          {
		
		int i;
		
		singleton = _singleton;
		m_ownerWorld = ownerWorld;
		geId = _geId;
		//uid = _uid;
		
		origOffset = positionOffset;// - btVector3(0.0f,0.0f,16.0f);
		float actorScale = 1.0f;


		initFromOrg(
			singleton->gameOrgs[
				singleton->gw->gameObjects[geId].orgId	
			]->baseNode,
			-1
		);




		return;


	}
GameActor::~ GameActor ()
        {
		int i;

		// Remove all constraints
		for ( i = 0; i < actorJoints.size(); ++i)
		{
			if (actorJoints[i].joint == NULL) {
				
			}
			else {
				m_ownerWorld->removeConstraint(actorJoints[i].joint);
				delete actorJoints[i].joint; actorJoints[i].joint = NULL;
			}
		}

		for ( i = 0; i < actorJoints.size(); ++i)
		{
			m_ownerWorld->removeRigidBody(actorJoints[i].body);
			
			delete actorJoints[i].body->getMotionState();
			delete actorJoints[i].body; actorJoints[i].body = NULL;
			delete actorJoints[i].shape; actorJoints[i].shape = NULL;
		}
	}
#undef LZZ_INLINE
 
