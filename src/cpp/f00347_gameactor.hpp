

class GameActor {
public:
	btDynamicsWorld*	m_ownerWorld;
	std::vector<ActorJointStruct> actorJoints;
	
	int uid;


	btRigidBody* localCreateRigidBody(btScalar mass, const btTransform& startTransform, btCollisionShape* shape) {
		bool isDynamic = (mass != 0.f);

		btVector3 localInertia(0,0,0);
		if (isDynamic)
			shape->calculateLocalInertia(mass,localInertia);

		btDefaultMotionState* myMotionState = new btDefaultMotionState(startTransform);
		btRigidBody::btRigidBodyConstructionInfo rbInfo(mass,myMotionState,shape,localInertia);
		btRigidBody* body = new btRigidBody(rbInfo);

		m_ownerWorld->addRigidBody(body);

		return body;
	}
	
	btVector3 getStartPosition(int jointId) {
		int parentId = actorJoints[jointId].parentId;
		ActorJointStruct* parJoint;
		
		if (parentId < 0) {
			return btVector3(0.0f,0.0f,0.0f);
		}
		else {
			parJoint = &(actorJoints[parentId]);
			
		}
	}
	
	int addJoint(
		int bodyUID,
		int parentId,
		btVector3 positionOffset, // orgin of actor root
		float rad,
		float len,
		float mass,
		btVector3 axis,
		float angle
	) {
		
		
		
		int i;
		float shapeScale = 4.0f;
		float jointSpace  = 0.25f*shapeScale;
		float fLegLength = 0.45f*shapeScale;
		btVector3 vUp(0, 0, 1);
		
		
		actorJoints.push_back(ActorJointStruct());
		int curId = actorJoints.size()-1;
		
		ActorJointStruct* curJoint = &(actorJoints.back());
		ActorJointStruct* parJoint;
		ActorJointStruct* grdJoint;
		
		curJoint->jointId = curId;
		curJoint->parentId = parentId;
		curJoint->shape = new btCapsuleShapeZ(rad, len);
		
		
		
		
		btTransform offset;
		offset.setIdentity();
		offset.setOrigin(positionOffset);
		btVector3 vRoot = btVector3(0.0f, 0.0f, 0.0f);
		btTransform transform;
		transform.setIdentity();
		transform.setOrigin(vRoot);
		
		float fAngle = theta;//2 * M_PI * i / NUM_LEGS_GA;
		float fSin = sin(fAngle);
		float fCos = cos(fAngle);
		
		btVector3 vBoneOrigin = btVector3(
			btScalar(fCos*(jointSpace+0.5*fLegLength)),
			btScalar(fSin*(jointSpace+0.5*fLegLength)),
			btScalar(0.0)
		);
		btVector3 vToBone = (vBoneOrigin - vRoot).normalize();
		btVector3 vAxis = vToBone.cross(vUp);	
		
		
		if (parentId < 0) {
			curJoint->body = localCreateRigidBody(mass, offset*transform, curJoint->shape);
		}
		else {
			
			
			// thigh
			transform.setIdentity();
			transform.setOrigin(vBoneOrigin);
			transform.setRotation(btQuaternion(vAxis, M_PI_2));
			curJoint->body = localCreateRigidBody(btScalar(10.0), offset*transform, curJoint->shape);

			// shin
			transform.setIdentity();
			transform.setOrigin(btVector3(
				btScalar(fCos*(jointSpace+fLegLength)),
				btScalar(fSin*(jointSpace+fLegLength)),
				btScalar(-0.5*fLegLength)
			));
			curJoint->body = localCreateRigidBody(btScalar(10.0), offset*transform, curJoint->shape);
			
		}
		
		curJoint->body->bodyUID = bodyUID;
		
		curJoint->body->setDamping(0.05, 0.85);
		curJoint->body->setDeactivationTime(0.8);
		curJoint->body->setSleepingThresholds(0.5f, 0.5f);
		
		
		btHingeConstraint* hingeC;
		//btConeTwistConstraint* coneC;

		btTransform localA, localB, localC;

		if (parentId < 0) {
			curJoint->joint = NULL;
		}
		else {
			parJoint = &(actorJoints[parentId]);
			
			
			
			// hip joints
			localA.setIdentity();
			localB.setIdentity();
			localA.getBasis().setEulerZYX(0,-fAngle,0);	localA.setOrigin(btVector3(
				btScalar(fCos*jointSpace),
				btScalar(fSin*jointSpace),
				btScalar(0.)
			));
			localB = 
				curJoint->body->getWorldTransform().inverse() *
				parJoint->body->getWorldTransform() *
				localA;
			hingeC = new btHingeConstraint(
				*(parJoint->body),
				*(curJoint->body),
				localA,
				localB
			);
			
			hingeC->setLimit(btScalar(-0.75 * M_PI_4), btScalar(M_PI_8));
			curJoint->joint = hingeC;
			m_ownerWorld->addConstraint(curJoint->joint, true);

			
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
			m_ownerWorld->addConstraint(curJoint->joint, true);
			
			
			
			
		}
		
		
	}
	

	GameActor(btDynamicsWorld* ownerWorld, const btVector3& positionOffset, bool bFixed, int _uid) {
		
		m_ownerWorld = ownerWorld;
		
		uid = _uid;
		btVector3 vUp(0, 0, 1);

		//
		// Setup geometry
		//
		
		int i;
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
		}
		
	}

	void stepSim(btScalar timeStep) {
		float m_fMuscleStrength = 10.0f;//(sin(singleton->curTime/2000.0)+1.0f)*0.5f;
		float ms = timeStep*1000000.0;
		float minFPS = 1000000.f/60.f;
		if (ms > minFPS) {
			ms = minFPS;
		}

		//m_Time += ms;

		for (int i=0; i < BODYPART_COUNT_GA; i++) {
			
			if (actorJoints[i].joint == NULL) {
				
			}
			else {
				btHingeConstraint* hingeC = static_cast<btHingeConstraint*>(actorJoints[i].joint);
				btScalar fCurAngle = hingeC->getHingeAngle();
				
				btScalar fTargetPercent = 0.5f;//(int(m_Time / 1000) % int(m_fCyclePeriod)) / m_fCyclePeriod;
				btScalar fTargetAngle   = 0.5 * (1 + sin(2 * M_PI * fTargetPercent));
				btScalar fTargetLimitAngle = hingeC->getLowerLimit() + fTargetAngle * (hingeC->getUpperLimit() - hingeC->getLowerLimit());
				btScalar fAngleError  = (fTargetLimitAngle - fCurAngle)*0.25;
				btScalar fDesiredAngularVel = 1000000.f * fAngleError/ms;
				hingeC->enableAngularMotor(true, fDesiredAngularVel, m_fMuscleStrength);
			}
			
			
		}
	}

	virtual	~GameActor ()
	{
		int i;

		// Remove all constraints
		for ( i = 0; i < BODYPART_COUNT_GA; ++i)
		{
			if (actorJoints[i].joint == NULL) {
				
			}
			else {
				m_ownerWorld->removeConstraint(actorJoints[i].joint);
				delete actorJoints[i].joint; actorJoints[i].joint = NULL;
			}
		}

		for ( i = 0; i < BODYPART_COUNT_GA; ++i)
		{
			m_ownerWorld->removeRigidBody(actorJoints[i].body);
			
			delete actorJoints[i].body->getMotionState();
			delete actorJoints[i].body; actorJoints[i].body = NULL;
			delete actorJoints[i].shape; actorJoints[i].shape = NULL;
		}
	}

	//btTypedConstraint** GetJoints() {return &m_joints[0];}

};





