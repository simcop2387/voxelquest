
class PooledResource {
private:
	FBOSet* fboSet[MAX_LAYERS];
	uint* cpuSet[MAX_LAYERS];
public:

	intPair usedById;
	bool isCPU;
	Singleton* singleton;
	GLuint holderDL[MAX_MIP_LEV*MAX_LAYERS];
	bool listGenerated;

	PooledResource() {
		int i;

		for (i = 0; i < MAX_LAYERS; i++) {
			fboSet[i] = NULL;
		}
		
	}

	FBOSet* getFBOS(int fboNum) {
		return fboSet[fboNum];
	}
	
	

	void init(
		Singleton* _singleton,
		bool _isCPU,
		int _sizeX,
		int _sizeY
	) {
		listGenerated = false;
		singleton = _singleton;
		isCPU = _isCPU;
		usedById.v0 = -1;
		usedById.v1 = -1;


		int i;


		//if (freeCam) {
			return;
		//}

		// if (isCPU) {
		// 	for (i = 0; i < MAX_LAYERS; i++) {
		// 		cpuSet[i] = new uint[
		// 			_sizeX *
		// 			_sizeY
		// 		];
		// 	}
		// }
		// else {
		// 	for (i = 0; i < MAX_LAYERS; i++) {
		// 		fboSet[i] = new FBOSet();
		// 	}

		// 	for (i = 0; i < MAX_LAYERS; i++) {
		// 		fboSet[i]->init(
		// 			2,
		// 			_sizeX,
		// 			_sizeY,
		// 			1,
		// 			false //has depth
		// 		);
		// 	}
		// }
	}
	
	void bindHolderDL(int mipLev, int layer) {
		int ci = mipLev*MAX_LAYERS + layer;
		if (listGenerated) {
			glDeleteLists(holderDL[ci], 1);
		}
		
		holderDL[ci] = glGenLists(1);
		glNewList(holderDL[ci], GL_COMPILE);
	}
	void unbindHolderDL(int mipLev, int layer) {
		glEndList();
		
		if (mipLev*MAX_LAYERS+layer == (MAX_MIP_LEV*MAX_LAYERS-1)) {
			listGenerated = true;
		}
		
	}
};
