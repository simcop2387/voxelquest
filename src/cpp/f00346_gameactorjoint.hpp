

class GameActorJoint {
public:
	btDynamicsWorld* m_ownerWorld;
	int uid;

	//btDynamicsWorld* ownerWorld, const btVector3& positionOffset, bool bFixed, int _uid	
	// m_ownerWorld = ownerWorld;
	// uid = _uid;
	
	GameActorJoint* parentJoint;
	std::vector<GameActorJoint*> childrenJoints;
	
	btCollisionShape* ajShape;
	btRigidBody* ajBody;
	btTypedConstraint* ajJoint;
	
	GameActorJoint() {
		
	}

	void init() {
		
	}

};


