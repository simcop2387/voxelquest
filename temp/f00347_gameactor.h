// f00347_gameactor.h
//

#include "f00347_gameactor.e"
#define LZZ_INLINE inline
int GameActor::addJoint (int nodeName, int parentId, bool isBall, float rad, float len, float mass, btVector3 targAlignT, btVector3 targAlignB, btVector3 targAlignN, btVector3 begPos, btVector3 midPos, btVector3 endPos)
          {
		
		
		
		int i;
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
		
		curJoint->shape = new btCapsuleShapeX(curJoint->rad, curLength);
		
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
		vectorB.normalize();
		
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
		
		
		int bodyCollidesWith = COL_STATIC|COL_DYN;
		curJoint->body = singleton->gamePhysics->example->createRigidBodyMask(
			mass,
			offset*transform,
			curJoint->shape,
			COL_BODY,
			bodyCollidesWith
		);
		//curJoint->body->bodyUID = uid;
		curJoint->body->setDamping(0.05, 0.85);
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
			
			if (curJoint->isBall) {
				
				localA.setIdentity();
				localB.setIdentity();
				localA.setOrigin(pivotA);
				localB.setOrigin(pivotB);
				localA.getBasis() = parJoint->basis; 
				localB.getBasis() = curJoint->basis; 
				//localA.setRotation(parJoint->quat);
				//localB.setRotation(curJoint->quat);
				
				
				// hingeC = new btHingeConstraint(
				// 	*(parJoint->body),
				// 	*(curJoint->body),
				// 	pivotA,
				// 	pivotB,
				// 	// parJoint->targAlignB,
				// 	// curJoint->targAlignB,
				// 	// false
				// );
				//hingeC->setLimit(-0.1, 0.1);
				
				// springC->enableSpring(0,false);
				// springC->enableSpring(1,false);
				// springC->enableSpring(2,false);
				// springC->enableSpring(3,true);
				// springC->enableSpring(4,true);
				// springC->enableSpring(5,true);
				
				// springC->setStiffness(3,10.0f);
				// springC->setStiffness(4,10.0f);
				// springC->setStiffness(5,10.0f);
				
				//curJoint->joint = hingeC;
				
				ballC = new btPoint2PointConstraint(
					*(parJoint->body),
					*(curJoint->body),
					pivotA,
					pivotB
					//localA,
					//localB
				);
				curJoint->joint = ballC;
			}
			else {
				// localA.setIdentity();
				// localB.setIdentity();
				// vectorA = parJoint->targAlign;
				// vectorB = curJoint->targAlign;
				
				// hingeC =  new btHingeConstraint(
				// 	*(parJoint->body),
				// 	*(curJoint->body),
				// 	//localA,
				// 	//localB
					
				// 	pivotA,
				// 	pivotB,
				// 	(vectorA.cross(vectorB)).normalized(),
				// 	(vectorA.cross(vectorB)).normalized()
					
				// );
				// hingeC->setLimit(-M_PI_4, M_PI_4);
				// curJoint->joint = hingeC;
				
			}
			
			
			m_ownerWorld->addConstraint(curJoint->joint, true);
			
			
			
			
			
			//localA.getBasis().setEulerZYX(0,0,M_PI);
			//localB.getBasis().setEulerZYX(0,0,M_PI);
			
			
			
			// vectorA = curJoint->targAlign;
			// vectorB = parJoint->targAlign;
			// axis = (vectorA.cross(vectorB)).normalized();
			// angle = btAcos(vectorA.dot(vectorB)) / (vectorA.length() * vectorB.length());
			// quat = btQuaternion(axis, angle);
			// transform.setRotation(quat);
			// localA.setRotation(quat);
			
			
			// vectorA = parJoint->targAlign;
			// vectorB = curJoint->targAlign;
			// axis = (vectorA.cross(vectorB)).normalized();
			// angle = btAcos(vectorA.dot(vectorB)) / (vectorA.length() * vectorB.length());
			// quat = btQuaternion(axis, angle);
			// transform.setRotation(quat);
			// localB.setRotation(quat);
			
			// localA.setRotation(quat);
			// localB.setRotation(curJoint->quat);
			
			// localA.setOrigin(
			// 	btVector3(0.0,0.0,-parJoint->length*0.5f)	
			// );
			// localB.setOrigin(
			// 	btVector3(0.0,0.0,curJoint->length*0.5f)
			// );
			
			
			// coneC = new btConeTwistConstraint(*(parJoint->body), *(curJoint->body), localA, localB);
			// coneC->setLimit(0.1, 0.1, 0);
			// curJoint->joint = coneC;
			// m_ownerWorld->addConstraint(curJoint->joint, true);
			
			
			
			
			
			
			
			// hingeC = new btHingeConstraint(
			// 	*(parJoint->body),
			// 	*(curJoint->body),
			// 	btVector3(0.0,0.0,-parJoint->length*0.5f),
			// 	btVector3(0.0,0.0,curJoint->length*0.5f),
			// 	//parJoint->endOrig - parJoint->midOrig,
			// 	//curJoint->begOrig - curJoint->midOrig,
			// 	curJoint->pivotAxis,
			// 	curJoint->pivotAxis
			// 	//parJoint->body->getWorldTransform(),
			// 	//curJoint->body->getWorldTransform()
			// 	// parJoint->begOrig,
			// 	// curJoint->begOrig,
			// 	// parJoint->targAlign,
			// 	// curJoint->targAlign
			// );
			
			
			// localA.setIdentity();
			// localB.setIdentity();
			// localA.getBasis().setEulerZYX(0,-theta,0);
			// localA.setOrigin(btVector3(0.0,0.0,-parJoint->length*0.5f));
			// localB.getBasis().setEulerZYX(0,-theta,0);
			// localB.setOrigin(btVector3(0.0,0.0,curJoint->length*0.5f));
			// hingeC =  new btHingeConstraint(
			// 	*(parJoint->body),
			// 	*(curJoint->body),
			// 	localA,
			// 	localB
			// );
			
			
			// hingeC->setLimit(btScalar(-0.1), btScalar(0.1));
			// curJoint->joint = hingeC;
			// m_ownerWorld->addConstraint(curJoint->joint, true);
			
			
			

			
			
			// hingeC->setLimit(btScalar(-M_PI), btScalar(M_PI));
			// curJoint->joint = hingeC;
			// m_ownerWorld->addConstraint(hingeC, true);
			
			
			// hingeC->setLimit(btScalar(0), btScalar(M_PI_2));
			// m_joints[JOINT_LEFT_KNEE] = hingeC;
			// m_ownerWorld->addConstraint(m_joints[JOINT_LEFT_KNEE], true);
			
			// localA.setIdentity(); localB.setIdentity();
			// localA.getBasis().setEulerZYX(0,M_PI_2,0); localA.setOrigin(scale*btVector3(btScalar(0.), btScalar(-0.225), btScalar(0.)));
			// localB.getBasis().setEulerZYX(0,M_PI_2,0); localB.setOrigin(scale*btVector3(btScalar(0.), btScalar(0.185), btScalar(0.)));
			// hingeC =  new btHingeConstraint(*m_bodies[BODYPART_RIGHT_UPPER_LEG], *m_bodies[BODYPART_RIGHT_LOWER_LEG], localA, localB);
			// hingeC->setLimit(btScalar(0), btScalar(M_PI_2));
			// m_joints[JOINT_RIGHT_KNEE] = hingeC;
			// m_ownerWorld->addConstraint(m_joints[JOINT_RIGHT_KNEE], true);
			
			
			
			// // hip joints
			// localA.setIdentity();
			// localB.setIdentity();
			// localA.getBasis().setEulerZYX(0,-theta,0);
			// localA.setOrigin(
			// 	curJoint->begOrig + curJoint->targAlign*jointSpace
			// 	/*
			// 	btVector3(
			// 					btScalar(fCos*jointSpace),
			// 					btScalar(fSin*jointSpace),
			// 					btScalar(0.)
			// 				)
			// 	*/	
			// );
			// localB = 
			// 	curJoint->body->getWorldTransform().inverse() *
			// 	parJoint->body->getWorldTransform() *
			// 	localA;
			// hingeC = new btHingeConstraint(
			// 	*(parJoint->body),
			// 	*(curJoint->body),
			// 	localA,
			// 	localB
			// );
			
			
			
			
			
			// hingeC->setLimit(btScalar(-M_PI), btScalar(M_PI));
			// curJoint->joint = hingeC;
			// m_ownerWorld->addConstraint(curJoint->joint, true);

			
			/*
			
			// hip joints
			localA.setIdentity();
			localB.setIdentity();
			localA.getBasis().setEulerZYX(0,-fAngle,0);	localA.setOrigin(btVector3(
				btScalar(fCos*fBodySize),
				btScalar(fSin*fBodySize),
				btScalar(0.)
			));
			localB = 
				actorJoints[2*i+1].body->getWorldTransform().inverse() *
				actorJoints[0].body->getWorldTransform() *
				localA;
			hingeC = new btHingeConstraint(
				*(actorJoints[0].body),
				*(actorJoints[1+2*i].body),
				localA,
				localB
			);
			
			// knee joints
			localA.setIdentity();
			localB.setIdentity();
			localC.setIdentity();
			localA.getBasis().setEulerZYX(0,-fAngle,0);
			localA.setOrigin(btVector3(
				btScalar(fCos*(jointSpace+fLegLength)),
				btScalar(fSin*(jointSpace+fLegLength)),
				btScalar(0.)
			));
			localB = 
				parJoint->body->getWorldTransform().inverse() * 
				grdJoint->body->getWorldTransform() * 
				localA;
			localC = 
				curJoint->body->getWorldTransform().inverse() *
				grdJoint->body->getWorldTransform() *
				localA;
			hingeC = new btHingeConstraint(
				*(parJoint->body),
				*(curJoint->body),
				localB,
				localC
			);
			hingeC->setLimit(btScalar(-M_PI_8), btScalar(0.2));
			curJoint->joint = hingeC;
			m_ownerWorld->addConstraint(curJoint->joint, true);*/
			
			
			
			/*

			btTransform offset; offset.setIdentity();
			offset.setOrigin(positionOffset);

			// root
			btVector3 vRoot = btVector3(btScalar(0.0), btScalar(0.0), btScalar(fHeight));
			btTransform transform;
			transform.setIdentity();
			transform.setOrigin(vRoot);
			if (bFixed) {
				actorJoints[0].body = localCreateRigidBody(btScalar(0.0), offset*transform, actorJoints[0].shape);
			} else {
				actorJoints[0].body = localCreateRigidBody(btScalar(10.0), offset*transform, actorJoints[0].shape);
			}

			// legs
			for ( i=0; i<NUM_LEGS_GA; i++) {
				float fAngle = 2 * M_PI * i / NUM_LEGS_GA;
				float fSin = sin(fAngle);
				float fCos = cos(fAngle);

				transform.setIdentity();
				btVector3 vBoneOrigin = btVector3(
					btScalar(fCos*(fBodySize+0.5*fLegLength)),
					btScalar(fSin*(fBodySize+0.5*fLegLength)),
					btScalar(fHeight)
				);
				transform.setOrigin(vBoneOrigin);

				// thigh
				btVector3 vToBone = (vBoneOrigin - vRoot).normalize();
				btVector3 vAxis = vToBone.cross(vUp);			
				transform.setRotation(btQuaternion(vAxis, M_PI_2));
				actorJoints[2*i+1].body = localCreateRigidBody(btScalar(10.0), offset*transform, actorJoints[2*i+1].shape);

				// shin
				transform.setIdentity();
				transform.setOrigin(btVector3(
					btScalar(fCos*(fBodySize+fLegLength)),
					btScalar(fSin*(fBodySize+fLegLength)),
					btScalar(fHeight-0.5*fLegLength)
				));
				actorJoints[2*i+2].body = localCreateRigidBody(btScalar(10.0), offset*transform, actorJoints[2*i+2].shape);
			}
			*/





					
					//transform.setOrigin(midPos);
					//transform.setRotation(btQuaternion(curJoint->targAlign, curJoint->angOrig));
					
					// btVector3 vBoneOrigin = midPos;
					// transform.setOrigin(vBoneOrigin);
					// btVector3 vToBone = (vBoneOrigin - vRoot).normalize();
					// btVector3 vAxis = vToBone.cross(vUp);
					// transform.setRotation(btQuaternion(vAxis, M_PI_2));
					
			
			
		}
		
		
		return curId;
		
	}
void GameActor::initFromOrg (GameOrgNode * curNode, int curParent)
          {
		
		
		int i;
		
		FIVector4 lineSeg0;
		FIVector4 lineSeg1;
		FIVector4 lineSeg2;
		
		lineSeg0.setFXYZRef(&(curNode->orgTrans[0]));

		
		//lineSeg1.setFXYZRef(&(curNode->tbnRotC[0]));
		//lineSeg1.multXYZ(  (curNode->orgVecs[E_OV_TBNRAD0][0])  );
		//lineSeg1.addXYZRef(&(lineSeg0));
		
		
		
		
		//float curRad = 0.025f;
		
		// lineSeg1.setFXYZRef(&(curNode->tbnTrans[0]));
		// lineSeg2.copyFrom(&lineSeg1);
		// lineSeg2.addXYZRef(&lineSeg0,-1.0f);
		// lineSeg2.normalize();
		// btVector3 tn = lineSeg2.getBTV();
		
		// float curLen = lineSeg0.distance(&lineSeg1);
		
		// lineSeg1.setFXYZRef(&(curNode->tbnTrans[1]));
		// lineSeg2.copyFrom(&lineSeg1);
		// lineSeg2.addXYZRef(&lineSeg0,-1.0f);
		// lineSeg2.normalize();
		// btVector3 bn = lineSeg2.getBTV();
		
		// lineSeg1.setFXYZRef(&(curNode->tbnTrans[2]));
		// lineSeg2.copyFrom(&lineSeg1);
		// lineSeg2.addXYZRef(&lineSeg0,-1.0f);
		// lineSeg2.normalize();
		// btVector3 nn = lineSeg2.getBTV();
		
		
		int curChild = addJoint(
			curNode->nodeName,
			curParent,					//int parentId,
			true,
			0.25f,			//float rad,
			curNode->orgTrans[0].getBTV().distance(curNode->orgTrans[2].getBTV()), //curLen, // +curRad*4.0f,			//float len,
			MASS_PER_LIMB,				//float mass,
			//tn, bn, nn,
		
			curNode->tbnRotC[0].getBTV(),
			curNode->tbnRotC[1].getBTV(),
			curNode->tbnRotC[2].getBTV(),
			
			curNode->orgTrans[0].getBTV(),
			curNode->orgTrans[1].getBTV(),
			curNode->orgTrans[2].getBTV()
			
		);
		
		
		
		
		for (i = 0; i < curNode->children.size(); i++) {
			initFromOrg(
				curNode->children[i],
				curChild
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





		// int curGrandParent = addJoint(
		// 	-1,								//int parentId,
		// 	true,
		// 	1.0f*actorScale,				//float rad,
		// 	1.1f*actorScale,				//float len,
		// 	MASS_PER_LIMB,					//float mass,
		// 	btVector3(0.0f,0.0f,1.0f)		//btVector3 targAlign,
		// 	//,0.0f, 0.0f
		// );
		
		// int curChild;
		// int curParent;
		// float curTheta;
		// float curPhi;
		
		// btVector3 targAlign;
		
		// for (i = 0; i < 6; i++) {
			
		// 	curTheta = M_PI*((float)i)/3.0f;
			
		// 	curPhi = M_PI_2 + 0.1;
		// 	targAlign = btVector3(
		// 		cos(curTheta)*sin(curPhi),
		// 		sin(curTheta)*sin(curPhi),
		// 		cos(curPhi)	
		// 	);
		// 	curParent = addJoint(
		// 		curGrandParent,				//int parentId,
		// 		true,
		// 		1.0f*actorScale,			//float rad,
		// 		8.0f*actorScale,			//float len,
		// 		MASS_PER_LIMB,				//float mass,
		// 		targAlign					//btVector3 targAlign
		// 		//,curTheta,
		// 		//curPhi
		// 	);
			
		// 	curPhi = M_PI_4 + 0.1;
		// 	targAlign = btVector3(
		// 		cos(curTheta)*sin(curPhi),
		// 		sin(curTheta)*sin(curPhi),
		// 		cos(curPhi)	
		// 	);
		// 	curChild = addJoint(
		// 		curParent,					//int parentId,
		// 		true,
		// 		1.0f*actorScale,			//float rad,
		// 		8.0f*actorScale,			//float len,
		// 		MASS_PER_LIMB,				//float mass,
		// 		targAlign					//btVector3 targAlign
		// 		//,curTheta,
		// 		//curPhi
		// 	);
			
			
		// 	curPhi = M_PI_2 + 0.1;
		// 	targAlign = btVector3(
		// 		cos(curTheta)*sin(curPhi),
		// 		sin(curTheta)*sin(curPhi),
		// 		cos(curPhi)	
		// 	);
		// 	addJoint(
		// 		curChild,					//int parentId,
		// 		true,
		// 		1.0f*actorScale,			//float rad,
		// 		8.0f*actorScale,			//float len,
		// 		MASS_PER_LIMB,				//float mass,
		// 		targAlign					//btVector3 targAlign
		// 		//,curTheta,
		// 		//curPhi
		// 	);
			
		// }
		
		/*
		
		
		//
		// Setup geometry
		//
		btVector3 vUp(0, 0, 1);
		float shapeScale = 4.0f;
		float fBodySize  = 0.25f*shapeScale;
		float fLegLength = 0.45f*shapeScale;
		float fHeight = 0.0;
		
		actorJoints.push_back(ActorJointStruct());
		actorJoints.back().shape = new btCapsuleShapeZ(btScalar(fBodySize), btScalar(0.10));
		actorJoints.back().jointId = actorJoints.size()-1;
				
		for ( i = 0; i < NUM_LEGS_GA; i++ ) {
			actorJoints.push_back(ActorJointStruct());
			actorJoints.back().shape = new btCapsuleShapeZ(btScalar(0.10*shapeScale), btScalar(fLegLength));
			actorJoints.back().jointId = actorJoints.size()-1;
			
			actorJoints.push_back(ActorJointStruct());
			actorJoints.back().shape = new btCapsuleShapeZ(btScalar(0.08*shapeScale), btScalar(fLegLength));
			actorJoints.back().jointId = actorJoints.size()-1;
		}

		//
		// Setup rigid bodies
		//
		
		btTransform offset; offset.setIdentity();
		offset.setOrigin(positionOffset);

		// root
		btVector3 vRoot = btVector3(btScalar(0.0), btScalar(0.0), btScalar(fHeight));
		btTransform transform;
		transform.setIdentity();
		transform.setOrigin(vRoot);
		if (bFixed) {
			actorJoints[0].body = localCreateRigidBody(btScalar(0.0), offset*transform, actorJoints[0].shape);
		} else {
			actorJoints[0].body = localCreateRigidBody(btScalar(10.0), offset*transform, actorJoints[0].shape);
		}
		
		// legs
		for ( i=0; i<NUM_LEGS_GA; i++) {
			float fAngle = 2 * M_PI * i / NUM_LEGS_GA;
			float fSin = sin(fAngle);
			float fCos = cos(fAngle);

			transform.setIdentity();
			btVector3 vBoneOrigin = btVector3(
				btScalar(fCos*(fBodySize+0.5*fLegLength)),
				btScalar(fSin*(fBodySize+0.5*fLegLength)),
				btScalar(fHeight)
			);
			transform.setOrigin(vBoneOrigin);

			// thigh
			btVector3 vToBone = (vBoneOrigin - vRoot).normalize();
			btVector3 vAxis = vToBone.cross(vUp);			
			transform.setRotation(btQuaternion(vAxis, M_PI_2));
			actorJoints[2*i+1].body = localCreateRigidBody(btScalar(10.0), offset*transform, actorJoints[2*i+1].shape);

			// shin
			transform.setIdentity();
			transform.setOrigin(btVector3(
				btScalar(fCos*(fBodySize+fLegLength)),
				btScalar(fSin*(fBodySize+fLegLength)),
				btScalar(fHeight-0.5*fLegLength)
			));
			actorJoints[2*i+2].body = localCreateRigidBody(btScalar(10.0), offset*transform, actorJoints[2*i+2].shape);
		}

		// Setup some damping on the bodies
		for (i = 0; i < BODYPART_COUNT_GA; ++i) {
			actorJoints[i].body->setDamping(0.05, 0.85);
			actorJoints[i].body->setDeactivationTime(0.8);
			actorJoints[i].body->setSleepingThresholds(0.5f, 0.5f);
		}


		//
		// Setup the constraints
		//
		btHingeConstraint* hingeC;
		//btConeTwistConstraint* coneC;

		btTransform localA, localB, localC;

		actorJoints[0].joint = NULL;

		for ( i=0; i<NUM_LEGS_GA; i++) {
			float fAngle = 2 * M_PI * i / NUM_LEGS_GA;
			float fSin = sin(fAngle);
			float fCos = cos(fAngle);

			// hip joints
			localA.setIdentity();
			localB.setIdentity();
			localA.getBasis().setEulerZYX(0,-fAngle,0);	localA.setOrigin(btVector3(
				btScalar(fCos*fBodySize),
				btScalar(fSin*fBodySize),
				btScalar(0.)
			));
			localB = 
				actorJoints[2*i+1].body->getWorldTransform().inverse() *
				actorJoints[0].body->getWorldTransform() *
				localA;
			hingeC = new btHingeConstraint(
				*(actorJoints[0].body),
				*(actorJoints[1+2*i].body),
				localA,
				localB
			);
			
			hingeC->setLimit(btScalar(-0.75 * M_PI_4), btScalar(M_PI_8));
			
			actorJoints[2*i+1].joint = hingeC;
			m_ownerWorld->addConstraint(actorJoints[2*i+1].joint, true);

			// knee joints
			localA.setIdentity();
			localB.setIdentity();
			localC.setIdentity();
			localA.getBasis().setEulerZYX(0,-fAngle,0);
			localA.setOrigin(btVector3(
				btScalar(fCos*(fBodySize+fLegLength)),
				btScalar(fSin*(fBodySize+fLegLength)),
				btScalar(0.)
			));
			localB = 
				actorJoints[2*i+1].body->getWorldTransform().inverse() * 
				actorJoints[0].body->getWorldTransform() * 
				localA;
			localC = 
				actorJoints[2*i+2].body->getWorldTransform().inverse() *
				actorJoints[0].body->getWorldTransform() *
				localA;
			hingeC = new btHingeConstraint(
				*(actorJoints[1+2*i].body),
				*(actorJoints[2+2*i].body),
				localB,
				localC
			);
			
			hingeC->setLimit(btScalar(-M_PI_8), btScalar(0.2));
			
			actorJoints[2*i+2].joint = hingeC;
			m_ownerWorld->addConstraint(actorJoints[2*i+2].joint, true);
		}
		
		
		for (i = 0; i < BODYPART_COUNT_GA; i++) {
			actorJoints[i].body->bodyUID = uid;
		}*/
		
	}
void GameActor::stepSim (btScalar timeStep)
                                        {
		
		//return;
		
		float m_fMuscleStrength = 10.0f;//(sin(singleton->curTime/2000.0)+1.0f)*0.5f;
		float ms = timeStep*1000000.0;
		float minFPS = 1000000.f/60.f;
		if (ms > minFPS) {
			ms = minFPS;
		}

		//m_Time += ms;

		// ge->applyImpulse(
		// 	btVector3(
		// 		( singleton->worldMarker.getFX() - curBody->body->getCenterOfMassPosition().getX() ),
		// 		( singleton->worldMarker.getFY() - curBody->body->getCenterOfMassPosition().getY() ),
		// 		-(curBody->body->getCenterOfMassPosition().getZ() - (8.0f + singleton->worldMarker.getFZ()))
		// 	)*totMass*0.01f,
		// 	false,
		// 	bodInd
		// );


		// GameOrgNode* curNode;
		// BaseObj* ge = &(singleton->gw->gameObjects[geId]);

		// for (int i = 0; i < actorJoints.size(); i++) {
		// 	curNode = singleton->gameOrgs[ge->orgId]->allNodes[actorJoints[i].nodeName];
			
		// }		

		for (int i = 0; i < actorJoints.size(); i++) {
			
			if (actorJoints[i].joint == NULL) {
				
			}
			else {
				
				// if (actorJoints[i].isBall) {
					
				// }
				// else {
					
					
					btHingeConstraint* hingeC = static_cast<btHingeConstraint*>(actorJoints[i].joint);
					btScalar fCurAngle = hingeC->getHingeAngle();
					
					btScalar fTargetPercent = 0.5;//singleton->smoothTime;//(int(m_Time / 1000) % int(m_fCyclePeriod)) / m_fCyclePeriod;
					btScalar fTargetAngle   = 0.5 * (1 + sin(2 * M_PI * fTargetPercent));
					btScalar fTargetLimitAngle = hingeC->getLowerLimit() + fTargetAngle * (hingeC->getUpperLimit() - hingeC->getLowerLimit());
					btScalar fAngleError  = (fTargetLimitAngle - fCurAngle)*0.25;
					btScalar fDesiredAngularVel = 1000000.f * fAngleError/ms;
					hingeC->enableAngularMotor(true, fDesiredAngularVel, m_fMuscleStrength);
				//}
				
				
			}
			
			
		}
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
 
