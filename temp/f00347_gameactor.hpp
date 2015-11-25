

class GameActor {
public:
	Singleton* singleton;
	btDynamicsWorld*	m_ownerWorld;
	std::vector<ActorJointStruct> actorJoints;
	int geId;
	
	//int uid;
	btVector3 origOffset;

	// btRigidBody* localCreateRigidBody(
	// 	btScalar mass,
	// 	const btTransform& startTransform,
	// 	btCollisionShape* shape,
	// 	int maskFrom,
	// 	int maskTo
	// ) {
	// 	bool isDynamic = (mass != 0.f);

	// 	btVector3 localInertia(0,0,0);
	// 	if (isDynamic)
	// 		shape->calculateLocalInertia(mass,localInertia);

	// 	btDefaultMotionState* myMotionState = new btDefaultMotionState(startTransform);
	// 	btRigidBody::btRigidBodyConstructionInfo rbInfo(mass,myMotionState,shape,localInertia);
	// 	btRigidBody* body = new btRigidBody(rbInfo);

	// 	m_ownerWorld->addRigidBody(body);//, maskFrom, maskTo);

	// 	return body;
	// }
	
	// btVector3 getStartPosition(int jointId) {
	// 	int parentId = actorJoints[jointId].parentId;
	// 	ActorJointStruct* parJoint;
		
	// 	btVector3 res;
		
	// 	if (parentId < 0) {
	// 		res = btVector3(0.0f,0.0f,0.0f);
	// 	}
	// 	else {
	// 		parJoint = &(actorJoints[parentId]);
	// 		res = parJoint->endOrig;
	// 	}
		
	// 	//traceBTV("Joint ", res);
		
	// 	return res;
	// }
	
	
	int addJoint(
		int nodeName,
		int parentId,
		bool isBall,
		float rad,
		float len,
		float mass,
		btVector3 targAlignT,
		btVector3 targAlignB,
		btVector3 targAlignN,
		
		btVector3 begPos,
		btVector3 midPos,
		btVector3 endPos
		
		//,float theta,
		//float phi
	) {
		
		
		
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
		
		
		int bodyCollidesWith = COL_STATIC|COL_DYN;
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
	
	
	
	
	void initFromOrg(
		GameOrgNode* curNode,
		int curParent
		//FIVector4* basePosition,
		// float scale,
		// int drawMode,
		// bool drawAll
	) {
		
		
		int i;
		
		// FIVector4 lineSeg0;
		// FIVector4 lineSeg1;
		// FIVector4 lineSeg2;
		// lineSeg0.setFXYZRef(&(curNode->orgTrans[0]));

		
		
		int curChild = addJoint(
			curNode->nodeName,
			curParent,					//int parentId,
			false,
			0.15f,			//float rad,
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
		
		int curChild2 = addJoint(
			curNode->nodeName,
			curChild,					//int parentId,
			true,
			0.2f,			//float rad,
			0.0f,//curNode->orgTrans[0].getBTV().distance(curNode->orgTrans[2].getBTV()), //curLen,
			MASS_PER_LIMB,				//float mass,
			//tn, bn, nn,
		
			curNode->tbnRotC[0].getBTV(),
			curNode->tbnRotC[1].getBTV(),
			curNode->tbnRotC[2].getBTV(),
			
			curNode->orgTrans[2].getBTV(), // beg
			curNode->orgTrans[2].getBTV(), // mid
			curNode->orgTrans[2].getBTV()  // end
			
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
	
	
	
	

	GameActor(
		Singleton* _singleton,
		int _geId,
		btDynamicsWorld* ownerWorld,
		const btVector3& positionOffset,
		bool bFixed
		//, int _uid	
	) {
		
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

	void stepSim(btScalar timeStep) {
		
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

	virtual	~GameActor ()
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

	//btTypedConstraint** GetJoints() {return &m_joints[0];}

};





