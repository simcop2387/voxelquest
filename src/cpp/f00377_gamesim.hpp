
class GameSim {
public:
	Singleton* singleton;
	
	std::vector<RigidBodyGroup> bodies;
	std::vector<intPair> broadPairs;
	
	GameSim() {
		
	}
	
	void init(Singleton* _singleton)
	{
		singleton = _singleton;
	}
	
	
	// void stepSim(float timeStepInSeconds) {
		
	// }
	
	// void updateAll() {
		
	// 	float subSteps = 4.0f;
	// 	int iSubSteps = subSteps;
		
	// 	broadPhase();
		
	// 	int i;
		
	// 	for (i = 0; i < iSubSteps; i++) {
	// 		stepSim((1.0f/subSteps)/60.0f);
	// 	}
		
	// }
	
	// void addBody() {
		
	// }
	
	// void remBody() {
		
	// }
	
	// inline bool broadContact(int index1, int index2) {
	// 	return (
	// 		bodies[index1].parentBodypos.distance(bodies[index2].pos) <= (
	// 				bodies[index1].broadRadius + bodies[index2].broadRadius
	// 		)
	// 	);
	// }
	
	// void broadPhase() {
	// 	int i;
	// 	int totSize = bodies.size();
	// 	int totSizeM1 = totSize-1;
		
	// 	intPair ip;
		
	// 	for (i = 0; i < totSizeM1; i++) {
	// 		for (j = i + 1; j < totSize; j++) {
				
	// 			if (bodies[i].filterMask & bodies[j].filterMask) {
	// 				if (broadContact(i,j)) {
	// 					ip.v0 = i;
	// 					ip.v1 = j;
	// 					broadPairs.push_back(ip);
	// 				}
	// 			}
	// 		}
	// 	}
	// }
	
	
	~GameSim() {
		
	}
	
	
};







