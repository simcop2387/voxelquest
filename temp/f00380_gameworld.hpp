
class GameWorld
{
public:

	bool skippedPrim;

	int numProvinces;
	int seaLevel;
	int seaSlack;
	int pageCount;
	int mapSwapFlag;
	int holdersPerBlock;
	int shiftCounter;
	
	int holdersPerChunk;
	int chunksPerWorld;
	int chunksPerBlock;
	

	int renderCount;
	float invalidCount;
	float invalidCountMax;

	int iBlockSize;
	int renderMethod;
	int iBufferSize;
	int maxThreads;
	int availThreads;

	int lightCount;

	int MIN_MIP;
	int MAX_MIP;
	int AVG_MIP;

	int blockMip;

	int hmChannel;
	int idChannel;
	int densityChannel;
	int blockChannel;
	

	int stChannel;
	int btChannel;
	int pathChannel;
	int houseChannel;

	float mapWidth;
	float mapHeight;

	int *curDiagram;
	int *provinceGrid;
	int *provinceX;
	int *provinceY;

	bool doDrawFBO;
	bool mapLockOn;
	bool foundPath;

	float mapStep;
	//float mapTrans;
	
	// int tempCellData[4];
	// int tempCellData2[4];

	int dirFlags[4];
	int dirFlagsOp[4];
	int dirFlagsO[4];
	int dirFlagsOpO[4];
	int dirModX[6];
	int dirModY[6];
	int dirModZ[6];
	int opDir[4];

	GameEnt *activeLights[MAX_EVAL_LIGHTS];

	int dirFlagClear;
	int visFlag;
	int visFlagO;
	int activeFBO;

	bool noiseGenerated;

	std::vector<intTrip> gamePageHolderList;

	std::vector<coordAndIndex> roadCoords;

	

	std::vector<int> ocThreads;
	
	btVector3 offsetVal[4];
	
	string curTargFBO[3];
	string curDepthFBO[3];

	GamePageHolder* blockHolder;
	GamePageHolder* holderInFocus;

	
	FIVector4 lScreenCoords;
	FIVector4 cScreenCoords;
	int cellsPerWorld;
	int holdersPerWorld;
	int blocksPerWorld;

	FIVector4 entMin;
	FIVector4 entMax;

	FIVector4 camHolderPos;
	//FIVector4 cutHolderPos;
	FIVector4 camBlockPos;
	FIVector4 camChunkPos;
	FIVector4 iPixelWorldCoords;
	FIVector4 pagePos;
	FIVector4 unitPos;
	FIVector4 lastUnitPos;
	
	
	FIVector4 tempVec1;
	FIVector4 tempVec2;
	FIVector4 tempVec3;
	FIVector4 tempVec4;
	
	FIVector4 holderLookAt;
	FIVector4 blockPos[2];
	FIVector4 nodePos[2];
	FIVector4 nodePosInPixels[2];
	//PathNode blockAndIndexPath[2];
	//PathNode blockAndIndexSplitPath[2];
	FIVector4 lineSeg[2];
	//PathHolder* finalPath;
	
	int nodeInd[2];
	//int finalInd;
	
	GameBlock* blockRef[2];
	
	
	//FIVector4 minShadowBounds;
	//FIVector4 maxShadowBounds;
	//FIVector4 minShadowBoundsGrow;
	//FIVector4 maxShadowBoundsGrow;

	FIVector4 minv;
	FIVector4 maxv;

	FIVector4 tempVec;
	FIVector4 unitPosMin;
	FIVector4 unitPosMax;
	FIVector4 unitPosCenter;
	//FIVector4 unitPosMinIS;
	//FIVector4 unitPosMaxIS;

	FIVector4 startBounds;
	FIVector4 endBounds;

	//FIVector4 *fogPos;
	//FIVector4 *cutPos;
	FIVector4 *lightPos;
	FIVector4 *globLightPos;
	FIVector4 lightPosBase;

	FIVector4 *curBoxPos;
	FIVector4 tv0;
	FIVector4 tv1;
	FIVector4 tv2;
	FIVector4 tv3;
	FIVector4 tv4;
	FIVector4 tv5;

	//glm::quat myQuat;
	//glm::vec3 paramVec;
	//glm::vec3 spinVec;

	Singleton *singleton;

	GameBlock **blockData;

	FBOWrapper *curFBO;
	FBOWrapper *curFBO2;
	
	BaseObjType lastHitObjUID;


	//Poco::ThreadPool threadpool;




	GameWorld()
	{

		// Notes
		// -----
		// Depth: 16 bits
		// Material: 16 bits
		// AO val: 8 bits, normal: 24 bits

		blockHolder = NULL;
		holderInFocus = NULL;

	}


	void init(Singleton *_singleton)
	{

		singleton = _singleton;
		
		chunksPerBlock = singleton->chunksPerBlock;
		chunksPerWorld = singleton->chunksPerWorld;
		holdersPerChunk = singleton->holdersPerChunk;

		int i;
		int j;
		
		skippedPrim = false;
		
		shiftCounter = 0;
		
		curTargFBO[0] = "terTargFBO";
		curTargFBO[1] = "sphTargFBO";
		curTargFBO[2] = "prmTargFBO";
		
		curDepthFBO[0] = "terDepthFBO";
		curDepthFBO[1] = "sphDepthFBO";
		curDepthFBO[2] = "prmDepthFBO";
		
		lastHitObjUID = 0;
		renderCount = 0;
		invalidCount = 0.0f;
		invalidCountMax = 10.0f;
		
		//finalInd = -1;
		
		lightCount = 1;

		noiseGenerated = false;

		//finalPath = NULL;
		
		
		float deltVal = -1.5f;
		offsetVal[0] = btVector3(deltVal,0.0f,0.0f);
		offsetVal[1] = btVector3(0.0f,deltVal,0.0f);
		offsetVal[2] = btVector3(0.0f,0.0f,deltVal);
		offsetVal[3] = btVector3(0.0f,0.0f,0.0f);
		

		holdersPerBlock = singleton->holdersPerBlock;

		dirFlagsO[0] = 1;
		dirFlagsO[1] = 2;
		dirFlagsO[2] = 4;
		dirFlagsO[3] = 8;

		dirFlagsOpO[0] = 2;
		dirFlagsOpO[1] = 1;
		dirFlagsOpO[2] = 8;
		dirFlagsOpO[3] = 4;

		dirFlags[0] = ~1;
		dirFlags[1] = ~2;
		dirFlags[2] = ~4;
		dirFlags[3] = ~8;

		dirFlagsOp[0] = ~2;
		dirFlagsOp[1] = ~1;
		dirFlagsOp[2] = ~8;
		dirFlagsOp[3] = ~4;

		dirModX[0] = 1;
		dirModX[1] = -1;
		dirModX[2] = 0;
		dirModX[3] = 0;
		dirModX[4] = 0;
		dirModX[5] = 0;

		dirModY[0] = 0;
		dirModY[1] = 0;
		dirModY[2] = 1;
		dirModY[3] = -1;
		dirModY[4] = 0;
		dirModY[5] = 0;

		dirModZ[0] = 0;
		dirModZ[1] = 0;
		dirModZ[2] = 0;
		dirModZ[3] = 0;
		dirModZ[4] = 1;
		dirModZ[5] = -1;

		opDir[0] = 1;
		opDir[1] = 0;
		opDir[2] = 3;
		opDir[3] = 2;

		dirFlagClear = ~15;
		visFlag = 16;
		visFlagO = ~16;

		foundPath = false;

		activeFBO = 0;
		mapLockOn = false;

		numProvinces = 16;
		provinceGrid = new int[numProvinces * numProvinces];
		provinceX = new int[numProvinces];
		provinceY = new int[numProvinces];

		mapSwapFlag = 0;
		mapStep = 0.0f;

		pageCount = 0;
		maxThreads = 7;
		availThreads = maxThreads;

		for (i = 0; i < maxThreads; i++)
		{
			ocThreads.push_back(-1);
		}

		hmChannel = 0;
		idChannel = 1;
		densityChannel = 2;
		blockChannel = 3;

		stChannel = 0;
		btChannel = 1;
		pathChannel = 2;
		houseChannel = 3;




		MIN_MIP = 0;
		MAX_MIP = 1;
		AVG_MIP = 2;


		doDrawFBO = false;


		renderMethod = (int)E_RENDER_VOL;

		cellsPerWorld = singleton->cellsPerWorld;
		holdersPerWorld = singleton->holdersPerWorld;
		blocksPerWorld = singleton->blocksPerWorld;


		iBlockSize = blocksPerWorld * blocksPerWorld * blocksPerWorld;



		blockData = new GameBlock*[iBlockSize];
		for (i = 0; i < iBlockSize; i++)
		{
			blockData[i] = NULL;
		}

		// if (GEN_POLYS_WORLD) {
		// 	blockHolder = new GamePageHolder();
		// 	blockHolder->init(singleton, -1, -1, 0,0,0, true);
		// }
		
		
		
	}


	
	
	// x, y, and z are measured in blocks
	GameBlock *getBlockAtCoords(
		int xInBlocks,
		int yInBlocks,
		int zInBlocks,
		bool createOnNull = false
	) {





		int newX = wrapCoord(xInBlocks, blocksPerWorld);
		int newY = wrapCoord(yInBlocks, blocksPerWorld);
		int newZ = wrapCoord(zInBlocks, blocksPerWorld);

		//zInBlocks;//


		int ind =
			newZ * blocksPerWorld*blocksPerWorld +
			newY * blocksPerWorld +
			newX;

		if (blockData[ind] == NULL) {
			if (createOnNull) {
				blockData[ind] = new GameBlock();
				blockData[ind]->init(singleton, ind, xInBlocks, yInBlocks, zInBlocks, newX, newY, newZ);
			}
		}

		return blockData[ind];

	}

	void clearAllHolders() {
		singleton->stopAllThreads();
		
		
		
		GamePageHolder* gph;
		
		int i;
		
		for (i = 0; i < gamePageHolderList.size(); i++) {
			gph = getHolderAtId(gamePageHolderList[i].v0, gamePageHolderList[i].v1, gamePageHolderList[i].v2);
			if (gph != NULL) {
				gph->reset(true);
			}
		}
		
		TOT_POINT_COUNT = 0;
		
		glFlush();
		glFinish();
		
	}





	// x, y, and z are measured in chunks
	GameChunk* getChunkAtCoords(int x, int y, int z, bool createOnNull = false)
	{

		GameChunk **chunkData;


		int newX = wrapCoord(x, chunksPerWorld);
		int newY = wrapCoord(y, chunksPerWorld);
		int newZ = z;

		int chunkX = newX - intDiv(newX, chunksPerBlock) * chunksPerBlock;
		int chunkY = newY - intDiv(newY, chunksPerBlock) * chunksPerBlock;
		int chunkZ = newZ - intDiv(newZ, chunksPerBlock) * chunksPerBlock;

		int chunkId = chunkZ * chunksPerBlock * chunksPerBlock + chunkY * chunksPerBlock + chunkX;


		GameBlock *curBlock = getBlockAtCoords(
			intDiv(x, chunksPerBlock),
			intDiv(y, chunksPerBlock),
			intDiv(z, chunksPerBlock),
			createOnNull
		);

		if (curBlock == NULL)
		{
			return NULL;
		}
		else
		{
			chunkData = curBlock->chunkData;


			if (chunkData[chunkId] == NULL)
			{
				if (createOnNull)
				{
					chunkData[chunkId] = new GameChunk();
					chunkData[chunkId]->init(singleton, curBlock->blockId, chunkId, x, y, z); //, x, y, z
				}
			}

			return chunkData[chunkId];


		}



	}
	





	// x, y, and z are measured in holders
	GamePageHolder *getHolderAtCoords(int x, int y, int z, bool createOnNull = false)
	{

		GamePageHolder **holderData;

		intTrip ip;

		int newX = wrapCoord(x, holdersPerWorld);
		int newY = wrapCoord(y, holdersPerWorld);
		int newZ = z;

		int holderX = newX - intDiv(newX, holdersPerChunk) * holdersPerChunk;
		int holderY = newY - intDiv(newY, holdersPerChunk) * holdersPerChunk;
		int holderZ = newZ - intDiv(newZ, holdersPerChunk) * holdersPerChunk;

		int holderId = holderZ * holdersPerChunk * holdersPerChunk + holderY * holdersPerChunk + holderX;


		GameBlock *curBlock = getBlockAtCoords(
			intDiv(x, holdersPerBlock),
			intDiv(y, holdersPerBlock),
			intDiv(z, holdersPerBlock),
			createOnNull
		);
		
		GameChunk* curChunk;

		if (curBlock == NULL)
		{
			return NULL;
		}
		else
		{
			
			curChunk = getChunkAtCoords(
				intDiv(x, holdersPerChunk),
				intDiv(y, holdersPerChunk),
				intDiv(z, holdersPerChunk),
				createOnNull
			);
			
			
			if (curChunk == NULL) {
				return NULL;
			}
			else {
				holderData = curChunk->holderData;


				if (holderData[holderId] == NULL)
				{
					if (createOnNull)
					{
						holderData[holderId] = new GamePageHolder();
						ip.v0 = curBlock->blockId;
						ip.v1 = curChunk->chunkId;
						ip.v2 = holderId;
						gamePageHolderList.push_back(ip);
						holderData[holderId]->init(
							singleton,
							curBlock->blockId,
							curChunk->chunkId,
							holderId,
							x, y, z
						); //, x, y, z
					}
				}

				return holderData[holderId];
			}
			
			


		}



	}
	
	
	
	// assumed that holder exists when calling this
	GameChunk *getChunkAtId(int blockId, int chunkId)
	{

		if (blockData[blockId] == NULL)
		{
			return NULL;
		}
		else
		{
			return blockData[blockId]->chunkData[chunkId];
		}
	}

	// assumed that holder exists when calling this
	GamePageHolder *getHolderAtId(int blockId, int chunkId, int holderId)
	{

		if (blockData[blockId] == NULL)
		{
			return NULL;
		}
		else
		{
			if (blockData[blockId]->chunkData[chunkId] == NULL) {
				return NULL;
			}
			else {
				return blockData[blockId]->chunkData[chunkId]->holderData[holderId];
			}
		}
	}

	// assumed that block exists when calling this
	GameBlock *getBlockAtId(int id)
	{


		if ((id < 0) || (id >= iBlockSize))
		{
			doTraceND("BLOCK INDEX OUT OF RANGE");
			return NULL;
		}
		else
		{
			if (blockData[id] == NULL)
			{
				doTraceND("BLOCK DATA IS NULL");
				return NULL;
			}
			else
			{
				return blockData[id];
			}
		}


	}
	
	
	
	
	int getCellInd(
		btVector3 cParam,
		GamePageHolder* &curHolder 
	) {
		
		int xv = cParam.getX();
		int yv = cParam.getY();
		int zv = cParam.getZ();
		
		int cellsPerHolder = singleton->cellsPerHolder;
		
		int newX = wrapCoord(xv,cellsPerWorld);
		int newY = wrapCoord(yv,cellsPerWorld);
		int newZ = zv;
		
		int x2 = intDiv(newX,cellsPerHolder);
		int y2 = intDiv(newY,cellsPerHolder);
		int z2 = intDiv(newZ,cellsPerHolder);
		
		
		curHolder = (getHolderAtCoords(x2, y2, z2, true));
		if (curHolder->wasGenerated) {
			
		}
		else {
			//cout << "attempted getCellInd without generation\n";
			return -1;
		}
		//GamePage* gp = getPageAtCoords(x2, y2, z2, false);
		
		int xr = newX - x2*cellsPerHolder;
		int yr = newY - y2*cellsPerHolder;
		int zr = newZ - z2*cellsPerHolder;
		
		return (zr*cellsPerHolder*cellsPerHolder + yr*cellsPerHolder + xr);
	}
	
	int getCellInd(
		GamePageHolder* &curHolder,
		int xv,
		int yv,
		int zv
	) {
		
		
		
		int cellsPerHolder = singleton->cellsPerHolder;
		
		int newX = wrapCoord(xv,cellsPerWorld);
		int newY = wrapCoord(yv,cellsPerWorld);
		int newZ = zv;
		
		int x2 = intDiv(newX,cellsPerHolder);
		int y2 = intDiv(newY,cellsPerHolder);
		int z2 = intDiv(newZ,cellsPerHolder);
		
		
		curHolder = (getHolderAtCoords(x2, y2, z2, true));
		if (curHolder->wasGenerated) {
			
		}
		else {
			//cout << "attempted getCellInd without generation\n";
			return -1;
		}
		//GamePage* gp = getPageAtCoords(x2, y2, z2, false);
		
		int xr = newX - x2*cellsPerHolder;
		int yr = newY - y2*cellsPerHolder;
		int zr = newZ - z2*cellsPerHolder;
		
		return (zr*cellsPerHolder*cellsPerHolder + yr*cellsPerHolder + xr);
	}
	
	
	
	
	
	
	// void setCellAtCoords(
	// 	int xv, int yv, int zv
	// ) {
	// 	int cellsPerHolder = singleton->cellsPerHolder;
		
	// 	int newX = wrapCoord(xv,cellsPerWorld);
	// 	int newY = wrapCoord(yv,cellsPerWorld);
	// 	int newZ = zv;
		
	// 	int x2 = intDiv(newX,cellsPerHolder);
	// 	int y2 = intDiv(newY,cellsPerHolder);
	// 	int z2 = intDiv(newZ,cellsPerHolder);
		
	// 	GamePageHolder* curHolder = getHolderAtCoords(x2, y2, z2, true);
		
	// 	if (curHolder == NULL) {
	// 		return;
	// 	}
		
	// 	int xr = newX - x2*cellsPerHolder;
	// 	int yr = newY - y2*cellsPerHolder;
	// 	int zr = newZ - z2*cellsPerHolder;
		
	// 	int ind = (zr*cellsPerHolder*cellsPerHolder + yr*cellsPerHolder + xr)*4;
		
	// 	curHolder->setCellAtInd(ind);
	// }
	

	// x, y, z measured in cells
	int getCellAtCoords(
		int xv,
		int yv,
		int zv
	) {
		
		int cellsPerHolder = singleton->cellsPerHolder;
		
		int newX = wrapCoord(xv,cellsPerWorld);
		int newY = wrapCoord(yv,cellsPerWorld);
		int newZ = zv;
		
		int x2 = intDiv(newX,cellsPerHolder);
		int y2 = intDiv(newY,cellsPerHolder);
		int z2 = intDiv(newZ,cellsPerHolder);
		
		int q;
		
		
		GamePageHolder* curHolder = getHolderAtCoords(x2, y2, z2, true);
		
		if (curHolder == NULL) {
			return E_CD_EMPTY;
		}
		
		// if (curHolder->lockRead) {
		// 	return E_CD_EMPTY;
		// }
		
		if (curHolder->wasGenerated) {
			
		}
		else {
			return E_CD_EMPTY;
		}
		
		int xr = newX - x2*cellsPerHolder;
		int yr = newY - y2*cellsPerHolder;
		int zr = newZ - z2*cellsPerHolder;
		
		int ind = (zr*cellsPerHolder*cellsPerHolder + yr*cellsPerHolder + xr)*4;
		
		return curHolder->getCellAtInd(ind);
		
	}
	
	float getCellAtCoordsLin(btVector3 pos) {
		
		int xv = pos.getX();
		int yv = pos.getY();
		int zv = pos.getZ();
		
		float fx = pos.getX()-xv;
		float fy = pos.getY()-yv;
		float fz = pos.getZ()-zv;
		
		float res[8];
		
		int q;
		int i;
		int j;
		int k;
		
		int curRes;
		
		for (q = 0; q < 8; q++) {
			k = q/(2*2);
			j = (q-k*2*2)/2;
			i = q-(k*2*2 + j*2);
			
			curRes = getCellAtCoords(xv+i,yv+j,zv+k);
			
			if (curRes == E_CD_SOLID) {
				res[q] = 1.0f;
			}
			else {
				res[q] = 0.0f;
			}
			
		}
		
		res[0] = res[0]*(1.0f-fz) + res[4]*fz;
		res[1] = res[1]*(1.0f-fz) + res[5]*fz;
		res[2] = res[2]*(1.0f-fz) + res[6]*fz;
		res[3] = res[3]*(1.0f-fz) + res[7]*fz;
		
		res[0] = res[0]*(1.0f-fy) + res[2]*fy;
		res[1] = res[1]*(1.0f-fy) + res[3]*fy;
		
		return res[0]*(1.0f-fx) + res[1]*fx;
	}
	
	btVector3 getNormalAtCoord(btVector3 coord, float* cellVal) {
		
		btVector3 norVal;
		int q;
		
		for (q = 0; q < 4; q++) {
			cellVal[q] = singleton->gw->getCellAtCoordsLin(
				coord + offsetVal[q]
			);
		}
		
		norVal = btVector3(
			(cellVal[0]-cellVal[3]),
			(cellVal[1]-cellVal[3]),
			(cellVal[2]-cellVal[3])
		);
		
		safeNorm(norVal);
		
		
		return norVal;
	}
	
	
	void setArrAtCoords(
		int xv, int yv, int zv,
		int* tempCellData,
		int* tempCellData2
	) {
		int cellsPerHolder = singleton->cellsPerHolder;
		
		int newX = wrapCoord(xv,cellsPerWorld);
		int newY = wrapCoord(yv,cellsPerWorld);
		int newZ = zv;
		
		int x2 = intDiv(newX,cellsPerHolder);
		int y2 = intDiv(newY,cellsPerHolder);
		int z2 = intDiv(newZ,cellsPerHolder);
		
		GamePageHolder* curHolder = getHolderAtCoords(x2, y2, z2, true);
		
		if (curHolder == NULL) {
			return;
		}
		
		if (curHolder->lockWrite) {
			return;
		}
		
		
		int xr = newX - x2*cellsPerHolder;
		int yr = newY - y2*cellsPerHolder;
		int zr = newZ - z2*cellsPerHolder;
		
		int ind = (zr*cellsPerHolder*cellsPerHolder + yr*cellsPerHolder + xr)*4;
		
		// if (singleton->refreshPaths) {	
		// 	curHolder->reset(); // todo: look at this?
		// }
		
		curHolder->setArrAtInd(ind,tempCellData,tempCellData2);
	}
	

	// x, y, z measured in cells
	void getArrAtCoords(
		int xv,
		int yv,
		int zv,
		int* tempCellData,
		int* tempCellData2
	) {
		
		int cellsPerHolder = singleton->cellsPerHolder;
		
		int newX = wrapCoord(xv,cellsPerWorld);
		int newY = wrapCoord(yv,cellsPerWorld);
		int newZ = zv;
		
		int x2 = intDiv(newX,cellsPerHolder);
		int y2 = intDiv(newY,cellsPerHolder);
		int z2 = intDiv(newZ,cellsPerHolder);
		
		int q;
		
		if (tempCellData != NULL) {
			for (q = 0; q < 4; q++) {
				tempCellData[q] = FLUID_UNIT_MIN;
				tempCellData2[q] = FLUID_UNIT_MIN;
			}
		}
		
		GamePageHolder* curHolder = getHolderAtCoords(x2, y2, z2, true);
		
		if (curHolder == NULL) {
			return;
		}
		
		int xr = newX - x2*cellsPerHolder;
		int yr = newY - y2*cellsPerHolder;
		int zr = newZ - z2*cellsPerHolder;
		
		int ind = (zr*cellsPerHolder*cellsPerHolder + yr*cellsPerHolder + xr)*4;
		
		curHolder->getArrAtInd(ind,tempCellData,tempCellData2);
		
	}
	
	
	void fireEvent(BaseObjType uid, int opCode, float fParam) {
		BaseObj* ge = &(singleton->gem->gameObjects[uid]);
		switch (opCode) {
			case EV_COLLISION:
			
				switch(ge->entType) {
					case E_ENTTYPE_BULLET:
						singleton->playSoundEnt("bump0", ge,0.0,0.25f);
					break;
					case E_ENTTYPE_WEAPON:
						singleton->playSoundEnt("metalclash0", ge, 0.25, 0.25f);
					break;
					default:
						singleton->playSoundEnt("land0", ge, 0.1, fParam*0.2);
					break;
				}
			
				
				singleton->performCamShake(ge, fParam);
			break;
		}
	}
	
	
	void doMedian() {
		int i;
		for (i = 0; i < singleton->medianCount; i++) {
			singleton->bindShader("MedianShader");
			singleton->bindFBO("resultFBO",activeFBO);
			singleton->sampleFBO("resultFBO", 0, activeFBO);
			singleton->sampleFBO("solidTargFBO", 1);
			singleton->setShaderfVec3("cameraPos", singleton->cameraGetPos());
			singleton->setShaderVec2("bufferDim", singleton->currentFBOResolutionX, singleton->currentFBOResolutionY);
			//singleton->setShaderfVec2("bufferDim", &(singleton->bufferDim) );
			
			
			singleton->drawFSQuad();
			
			singleton->unsampleFBO("solidTargFBO", 1);
			singleton->unsampleFBO("resultFBO", 0, activeFBO);
			singleton->unbindFBO();
			singleton->unbindShader();
			
			activeFBO = 1-activeFBO;
		}
	}
	
	
	void preUpdate() {
		activeFBO = 0;

		
		camChunkPos.copyFrom( singleton->cameraGetPosNoShake() );
		camChunkPos.intDivXYZ(singleton->cellsPerChunk);
		
		camBlockPos.copyFrom( singleton->cameraGetPosNoShake() );
		camBlockPos.intDivXYZ(singleton->cellsPerBlock);

		if (singleton->gem->getCurActor() == NULL) {
			camHolderPos.copyFrom( singleton->cameraGetPosNoShake() );
			camHolderPos.intDivXYZ(singleton->cellsPerHolder);
			camHolderPos.addXYZRef(&(singleton->lookAtVec),4.0);
		}
		else {
			camHolderPos.setBTV(singleton->gem->getCurActor()->getCenterPoint(0));
			camHolderPos.intDivXYZ(singleton->cellsPerHolder);
		}

		
		
		if (singleton->depthInvalidMove) {
			invalidCount += 1.25f + singleton->amountInvalidMove*0.25f;
		}
		// if (singleton->depthInvalidRotate) {
		// 	invalidCount += 1.25f + singleton->amountInvalidRotate*200.0f;
		// }
		invalidCount = min(invalidCount,invalidCountMax);
		invalidCount -= 1.0f;
		if (invalidCount < 0.0f) {
			invalidCount = 0.0f;
		}
		
		// if was moving and just stopped moving, render sphere map once
		
		// if (singleton->gameFluid[E_FID_BIG]->posShifted) {
		// 	drawVol((singleton->volumeWrappers[E_VW_TERGEN]));
		// }
		
		
		if (singleton->gameFluid[E_FID_BIG]->posShifted) {
			shiftCounter = 10;
			singleton->gameFluid[E_FID_BIG]->posShifted = false;
		}
		
		shiftCounter--;
		if (shiftCounter < 0) {
			shiftCounter = 0;
		}
		
		
		singleton->curMVP = singleton->projMatrix*singleton->viewMatrix;
		singleton->curObjMatrix3.set4(singleton->curMVP.get());
		singleton->curObjMatrix3.invert();
		singleton->curObjMatrix3.transpose();
		
		if (singleton->updateHolderLookat) {
			holderLookAt.copyFrom(&camHolderPos);
			holderLookAt.addXYZRef(&singleton->lookAtVec,1.0f);
			holderInFocus = getHolderAtCoords(
				holderLookAt.getIX(),
				holderLookAt.getIY(),
				holderLookAt.getIZ(),
				false
			);
		}
		
		
	}
	

	void update() {
		
		singleton->updateLock = true;
		
		bool postToScreen = true;
		
		if (noiseGenerated) {

		}
		else {
			noiseGenerated = true;
			singleton->bindShader("NoiseShader");
			singleton->bindFBO("noiseFBO");
			singleton->drawFSQuad();
			singleton->unbindFBO();
			singleton->unbindShader();
			
			singleton->copyFBO("noiseFBO","noiseFBOLinear");
			
			tempVec1.setFXYZ(0.0f,0.0f,0.0f);
			tempVec2.setFXYZ(256.0,256.0f,256.0f);
			
			drawVol((singleton->volumeWrappers[E_VW_VORO]), &tempVec1, &tempVec2, true, true, true);
			
			
		}
		
		// get view matrix
		// singleton->perspectiveOn = true;
		// singleton->getMatrixFromFBO("rasterFBO0");
		// singleton->perspectiveOn = false;
		
		
		
		
		if (singleton->fpsTest) {
			
		}
		else {
			
			glEnable(GL_DEPTH_TEST);
			singleton->perspectiveOn = true;
			
			if (singleton->settings[E_BS_DEBUG_VIEW]) { //||(singleton->mouseState == E_MOUSE_STATE_BRUSH)
				//renderGeom();
				renderDebug();
			}
			else {
				singleton->bindShader("GeomShader");
				singleton->bindFBO("debugTargFBO");
				singleton->unbindFBO();
				singleton->unbindShader();
			}
			
			singleton->perspectiveOn = false;
			glDisable(GL_DEPTH_TEST);
			
			
		}
		
		
		singleton->forceShadowUpdate--;
		if (singleton->forceShadowUpdate < 0) {
			singleton->forceShadowUpdate = 0;
		}
		
		if (singleton->settings[E_BS_RENDER_VOXELS]) {
			
			if (
				singleton->lightChanged ||
				(singleton->forceShadowUpdate == 1) ||
				(
					(singleton->lastLightPos.distance(singleton->cameraGetPosNoShake())) >
					singleton->conVals[E_CONST_LIGHTTHRESH]
				)
			) {
				// if (!singleton->lightChanged) {
				// 	cout << "updateShadows\n";
				// }
				
				
				rasterHolders(true);
				singleton->lastLightPos.copyFrom(singleton->cameraGetPosNoShake());
				singleton->lightChanged = false;
				//singleton->updateShadows = false;
			}
			
			rasterHolders(false);
		}
		else {
			
			
			
			
			
			
			// if (
				
			// 	(
				
			// 		( shiftCounter == 1 ) ||
			// 		(
			// 			(singleton->lastDepthInvalidMove) &&
			// 			(!singleton->depthInvalidMove)
			// 		)	
					
			// 	)
			// 	&&
			// 	USE_SPHERE_MAP
			// ) {
			// 	drawPrim(true,true,false);
			// }
			
			
			// if (GEN_POLYS_WORLD||POLY_COLLISION) {
			// 	glEnable(GL_DEPTH_TEST);
			// 	//glEnable(GL_CULL_FACE);
				
			// 	//back face
			// 	//glDepthFunc(GL_GREATER);
			// 	// glCullFace(GL_FRONT);
			// 	// drawPolys(polyFBOStrings[1], 4,-1);
				
			// 	//front face
			// 	//glDepthFunc(GL_LESS);
			// 	//glCullFace(GL_BACK);
			// 	//glDepthFunc(GL_LEQUAL);
				
			// 	//glDepthRange(singleton->clipDist[0],singleton->clipDist[1]);
			// 	singleton->perspectiveOn = true;
				
			// 	if (GEN_POLYS_WORLD) {
			// 		drawPolys(polyFBOStrings[0], 0, 0,true);
			// 	}
			// 	if (POLY_COLLISION) {
			// 		drawPolys(polyFBOStrings[0], 0, DEF_VOL_SIZE/singleton->cellsPerHolder + 1,false);
			// 	}
				
			// 	singleton->perspectiveOn = false;
				
			// 	//glDisable(GL_CULL_FACE);
			// 	glDisable(GL_DEPTH_TEST);
				
			// 	//polyCombine();
			// }
			
			
			
			skippedPrim = false;

			updateLimbTBOData(true);
			drawPrim(false,true,false);
			drawPrim(false,false,false);
				
			//singleton->copyFBO2("solidBaseTargFBO","solidTargFBO");
			
			singleton->bindShader("SolidCombineShader");
			singleton->setShaderInt("skippedPrim", (int)(skippedPrim));
			singleton->bindFBO("solidTargFBO");//, -1, 0);
			singleton->sampleFBO("solidBaseTargFBO",0);
			singleton->sampleFBO("geomTargFBO",2);
			singleton->drawFSQuad();
			singleton->unsampleFBO("geomTargFBO",2);
			singleton->unsampleFBO("solidBaseTargFBO",0);
			singleton->unbindFBO();
			singleton->unbindShader();
			
			postProcess(postToScreen);
		}
		
		
		doMedian();
		
		finalStep(postToScreen);
		
		if (postToScreen) {
			drawMap();
			glutSwapBuffers();
		}
		
		
		singleton->updateLock = false;

	}
	
	
	void toggleVis(GameEnt* se) {
		
		if (se == NULL) {

		}
		else {
			se->visible = !(se->visible);
		}
	}

	
	
	
	void ensureBlocks() {
		// first fetch all the blocks to make sure they get created
		// don't remove this, necessary for overlapping geom
		
		int i;
		int j;
		int k;
		int ii;
		int jj;
		int kk;
		int blockRad = 1;
		
		for (k = -blockRad; k <= blockRad; k++)
		{
			for (j = -blockRad; j <= blockRad; j++)
			{
				for (i = -blockRad; i <= blockRad; i++)
				{
					ii = i + camBlockPos.getIX();
					jj = j + camBlockPos.getIY();
					kk = k + camBlockPos.getIZ();

					getBlockAtCoords(ii, jj, kk, true);

				}
			}
		}
		
	}
	






	// void findNearestEnt(
	// 	EntSelection* entSelection,
	// 	int entType,
	// 	int maxLoadRad,
	// 	int radStep,
	// 	FIVector4 *testPoint,
	// 	bool onlyInteractive = false,
	// 	bool ignoreDistance = false
	// ) {
		
	// 	GameEnt* myEnt;
		
	// 	int curInd = 0;
	// 	float bestDis;
	// 	float curDis;
		
	// 	bool doProc = false;
		
	// 	entSelection->selEntList.clear();
	// 	entSelection->selEntMap.clear();
	// 	entSelection->selEntListInd = 0;

	// 	bestDis = 99999.0f;
		
		
		
		
	// 	//////////////////////
		
		
		
	// 	int i, j, k;
	// 	int ii, jj, kk;
	// 	int incVal;
		
	// 	int tot = 0;


	// 	int mink;
	// 	int maxk;
	// 	int minj;
	// 	int maxj;
	// 	int mini;
	// 	int maxi;
	// 	int curLoadRadius;
	// 	intPair curId;
		
	// 	tempVec.copyFrom(testPoint);
	// 	tempVec.intDivXYZ(singleton->cellsPerHolder);


	// 	GamePageHolder* curHolder;
	// 	GameBlock *curBlock;


	// 	ensureBlocks();
		
		
		

	// 	for (curLoadRadius = 0; curLoadRadius < maxLoadRad; curLoadRadius++) {
			
	// 		mink = max(tempVec.getIZ() - curLoadRadius,0);
	// 		maxk = min(tempVec.getIZ() + curLoadRadius,holdersPerWorld-1);
	// 		minj = tempVec.getIY() - curLoadRadius;
	// 		maxj = tempVec.getIY() + curLoadRadius;
	// 		mini = tempVec.getIX() - curLoadRadius;
	// 		maxi = tempVec.getIX() + curLoadRadius;
			
	// 		for (jj = minj; jj <= maxj; jj += radStep) {
				
	// 			if (curLoadRadius <= 2) {
	// 				incVal = 1;
	// 			}
	// 			else {
	// 				if ( (jj == minj) || (jj == maxj) ) {
	// 					incVal = radStep;
	// 				}
	// 				else {
	// 					incVal = maxi - mini;
	// 				}
	// 			}
				
	// 			for (ii = maxi; ii >= mini; ii -= incVal) {
					
					
	// 				for (kk = mink; kk <= maxk; kk += radStep) {
						
						
						
	// 					curHolder = getHolderAtCoords(ii, jj, kk, true);
	// 					curBlock = getBlockAtId(curHolder->blockId);
						
	// 					if (curBlock == NULL) {
	// 						cout << "NULL BLOCK\n";
	// 					}
	// 					else {
							
	// 						for (k = 0; k < curHolder->containsEntIds[entType].data.size(); k++) { //curBlock->gameEnts[entType].data.size()
								
	// 							curId = curHolder->containsEntIds[entType].data[k];
	// 							myEnt = &(blockData[curId.v0]->gameEnts[entType].data[curId.v1]);
								
								
								
	// 							if (
	// 								ignoreDistance||
	// 								testPoint->inBoundsXYZSlack(
	// 									myEnt->getVisMinInPixelsT(),
	// 									myEnt->getVisMaxInPixelsT(),
	// 									0.0625
	// 								)
	// 							)
	// 							{
									
	// 								if (entSelection->selEntMap.count(curId) == 0 ) {
										
	// 									entSelection->selEntList.push_back(myEnt);
	// 									entSelection->selEntMap[curId] = 1;
										
										
										
	// 									curDis = 
	// 									abs(myEnt->getVisMaxInPixelsT()->getFX()-testPoint->getFX()) +
	// 									abs(myEnt->getVisMaxInPixelsT()->getFY()-testPoint->getFY()) +
	// 									abs(myEnt->getVisMaxInPixelsT()->getFZ()-testPoint->getFZ());
										
	// 									//myEnt->getVisMinInPixelsT()->distance(testPoint) +
	// 									//myEnt->getVisMaxInPixelsT()->distance(testPoint);

	// 									if (myEnt->visible) {
											
	// 									}
	// 									else {
	// 										curDis *= 16.0f;
	// 									}

	// 									if (onlyInteractive) {
	// 										doProc = singleton->isInteractiveEnt[myEnt->buildingType];
	// 									}
	// 									else {
	// 										doProc = true;
	// 									}

	// 									if ((curDis < bestDis)&&doProc) {
	// 										bestDis = curDis;
	// 										entSelection->selEntListInd = curInd;
	// 									}
										
										
	// 									curInd++;
										
	// 								}
									
									

	// 							}
	// 						}
							
							
	// 					}
						
						
						
	// 				}
					
	// 			}
	// 		}
			
	// 	}
		
		
		
		
	// 	//////////////////////
		
		
	// 	//return resEnt;
	// }


	void drawVol(
		VolumeWrapper* curVW,
		FIVector4* minc,
		FIVector4* maxc,
		bool copyToTex,
		bool forceFinish,
		bool getVoro = false,
		bool getBlockHolders = false
	) {
		
		
		curVW->genPosMin.copyFrom( minc );
		curVW->genPosMax.copyFrom( maxc );
		
		//curVW->genPosMin.copyFrom( &(singleton->gameFluid[E_FID_BIG]->volMinReadyInPixels) );
		//curVW->genPosMax.copyFrom( &(singleton->gameFluid[E_FID_BIG]->volMaxReadyInPixels) );
		
		singleton->bindShader("TerGenShader");
		singleton->bindFBODirect(&(curVW->fboSet));
		singleton->sampleFBO("hmFBOLinearBig",2);
		
		if (!getVoro) {
			singleton->setShaderTexture3D(13, singleton->volumeWrappers[E_VW_VORO]->volId);
		}
		
		
		
		// if (!getBlockHolders) {
		// 	singleton->setShaderTexture3D(14, singleton->volumeWrappers[E_VW_WORLD]->volId);
		// }
		
		singleton->setShaderTexture(15, singleton->imageVoro->tid);
		
		singleton->setShaderfVec3("bufferDim", &(curVW->terGenDim) );
		
		singleton->setShaderFloat("voroSize",singleton->volumeWrappers[E_VW_VORO]->terGenDim.getFZ());
		
		singleton->setShaderFloat("mapPitch", singleton->mapPitch);
		singleton->setShaderFloat("seaLevel", singleton->getSLNormalized() );
		singleton->setShaderFloat("heightMapMaxInCells", singleton->heightMapMaxInCells);
		singleton->setShaderfVec4("mapFreqs", &(singleton->mapFreqs) );
		singleton->setShaderfVec4("mapAmps", &(singleton->mapAmps) );
		
		singleton->setShaderfVec3("volMinReadyInPixels", &(curVW->genPosMin) );
		singleton->setShaderfVec3("volMaxReadyInPixels", &(curVW->genPosMax) );
		//singleton->setShaderfVec3("volMinReadyInPixels", &(singleton->gameFluid[E_FID_BIG]->volMinInPixels) );
		//singleton->setShaderfVec3("volMaxReadyInPixels", &(singleton->gameFluid[E_FID_BIG]->volMaxInPixels) );
		
		singleton->setShaderInt("getVoro", (int)(getVoro));
		singleton->setShaderInt("getBlockHolders", (int)(false));
		
		singleton->setShaderFloat("cellsPerWorld", cellsPerWorld );
		
		singleton->fsQuad.draw();
		
		singleton->setShaderTexture(15, 0);
		// if (!getBlockHolders) {
		// 	singleton->setShaderTexture3D(14, 0);
		// }
		if (!getVoro) {
			singleton->setShaderTexture3D(13, 0);
		}
		singleton->unsampleFBO("hmFBOLinearBig",2);
		singleton->unbindFBO();
		singleton->unbindShader();
		
		if (forceFinish) {
			glFlush();
			glFinish();
		}
		
		FBOWrapper* fbow = curVW->fboSet.getFBOWrapper(0);
		fbow->getPixels();
		
		if (forceFinish) {
			glFlush();
			glFinish();
		}
		
		if (copyToTex) {
			if (curVW->isFloat) {
				curVW->copyFloatArr(fbow->pixelsFloat);
			}
			else {
				curVW->copyCharArr(fbow->pixelsChar);
			}
		}
		
	}

	



	void updateLimbTBOData(bool showLimbs) {
		int i;
		int j;
		int q;
		
		BodyStruct* curBody;
		
		BaseObj* ge;
		
		btVector3 centerPoint;
		btVector3 tanVec;
		btVector3 bitVec;
		btVector3 norVec;
		btVector3 len0;
		btVector3 len1;
		btMatrix3x3 basis;
		
		GameOrg* curOrg = NULL;
		GameOrgNode* curOrgNode = NULL;
		
		float buffer = 0.5f;
		
		int dataInd = 0;
		int actorCount = 0;
		int headerStart;
		
		float myMat[16];
		btVector3 basePos;
		btVector3 tempBTV;
		Matrix4 myMatrix4;
		Vector4 myVector4;
		Vector4 resVector4;
		
		
		BaseObj* grabber;
		
		// if (singleton->doPathReport) {
		// 	cout << "\n\n";
		// }
		
		
		singleton->limbTBOData[dataInd] = 0.0f; dataInd++;
		singleton->limbTBOData[dataInd] = 0.0f; dataInd++;
		singleton->limbTBOData[dataInd] = 0.0f; dataInd++;
		singleton->limbTBOData[dataInd] = 0.0f; dataInd++;
		
		dataInd = 0;
		
		if (!showLimbs) {
			return;
		}
		
		
		
		for (i = 0; i < singleton->gem->visObjects.size(); i++) {
			ge = &(singleton->gem->gameObjects[singleton->gem->visObjects[i]]);
			
			if (
				(!(ge->isHidden)) &&
				(ge->orgId > -1) &&
				(ge->actorId > -1)	
			) {
				
				actorCount++;
				
				ge->clearAABB(&(ge->aabbMinVis),&(ge->aabbMaxVis));
				for (j = 0; j < ge->bodies.size(); j++) {
					curBody = &(ge->bodies[j]);
					ge->addAABBPoint(
						&(ge->aabbMinSkel),
						&(ge->aabbMaxSkel),
						curBody->body->getCenterOfMassPosition()
					);
				}
				
				curOrg = singleton->gem->gameOrgs[ge->orgId];
				
				if (ge->isGrabbedById > -1) {
					grabber = &(singleton->gem->gameObjects[ge->isGrabbedById]);
				}
				else {
					grabber = ge;
				}
				
				grabber->bodies[E_BDG_CENTER].body->getWorldTransform().getOpenGLMatrix(myMat);
				myMatrix4 = Matrix4(myMat);
				
				
				// header info
				headerStart = dataInd;
				singleton->limbTBOData[dataInd] = 0.0f; dataInd++;
				singleton->limbTBOData[dataInd] = 0.0f; dataInd++;
				singleton->limbTBOData[dataInd] = 0.0f; dataInd++;
				singleton->limbTBOData[dataInd] = 0.0f; dataInd++;
			
				singleton->limbTBOData[dataInd] = ge->aabbMinVis.getX() - buffer; dataInd++;
				singleton->limbTBOData[dataInd] = ge->aabbMinVis.getY() - buffer; dataInd++;
				singleton->limbTBOData[dataInd] = ge->aabbMinVis.getZ() - buffer; dataInd++;
				singleton->limbTBOData[dataInd] = 0.0f; dataInd++;
				
				
				singleton->limbTBOData[dataInd] = ge->aabbMaxVis.getX() + buffer; dataInd++;
				singleton->limbTBOData[dataInd] = ge->aabbMaxVis.getY() + buffer; dataInd++;
				singleton->limbTBOData[dataInd] = ge->aabbMaxVis.getZ() + buffer; dataInd++;
				singleton->limbTBOData[dataInd] = 0.0f; dataInd++;
				
				float randOff;
				
				for (j = 0; j < ge->bodies.size(); j++) {
					curBody = &(ge->bodies[j]);
					
					if (
						(curBody->jointType != E_JT_LIMB) ||
						(curBody->boneId < 0) ||
						(curBody->boneId == E_BONE_C_BASE) ||
						(
							singleton->gem->firstPerson &&
							(curBody->boneId == E_BONE_C_SKULL) &&
							(ge->uid == singleton->gem->getCurActorUID())
						)
						
					) {
						
					}
					else {
						
						
						
						curOrgNode = curOrg->allNodes[curBody->boneId];
						
						centerPoint = curBody->body->getCenterOfMassPosition();
						//centerPoint += btVector3(0.0,0.0,-0.4f);
						basis = curBody->body->getCenterOfMassTransform().getBasis();
						
						
						
						tempBTV = curOrgNode->tbnTrans[1].getBTV();
						myVector4 = Vector4(
							tempBTV.getX(),
							tempBTV.getY(),
							tempBTV.getZ(),
							1.0f
						);
						resVector4 = myMatrix4*myVector4;
						basePos = btVector3(resVector4.x,resVector4.y,resVector4.z);
						basePos += grabber->skelOffset;
						basePos -= centerPoint;
						safeNorm(basePos);
						
						
						
						tanVec = basis.getColumn(0);//basis*curOrgNode->orgVecs[0].getBTV();
						safeNorm(tanVec);
						// bitVec = basis.getColumn(1);//basis*curOrgNode->orgVecs[1].getBTV();
						// norVec = basis.getColumn(2);//basis*curOrgNode->orgVecs[2].getBTV();
						
						//tanVec = basePos[0];//basis*curOrgNode->orgVecs[0].getBTV();
						bitVec = basePos;//basis*curOrgNode->orgVecs[1].getBTV();
						//norVec = basePos[2];//basis*curOrgNode->orgVecs[2].getBTV();
						
						norVec = tanVec.cross(bitVec);
						safeNorm(norVec);
						
						bitVec = norVec.cross(tanVec);
						safeNorm(bitVec);
						
						
						len0 = curOrgNode->orgVecs[E_OV_TBNRAD0].getBTV();
						len1 = curOrgNode->orgVecs[E_OV_TBNRAD1].getBTV();
						
						
						// datVec
						randOff = abs( fSeedRand2((ge->uid*37.19232f),(curOrgNode->orgVecs[E_OV_MATPARAMS].getFX()*17.89923f)) );
						
						singleton->limbTBOData[dataInd] = ge->uid; dataInd++;
						singleton->limbTBOData[dataInd] = curBody->body->limbUID; dataInd++;
						singleton->limbTBOData[dataInd] = curOrgNode->orgVecs[E_OV_MATPARAMS].getFX(); dataInd++;
						singleton->limbTBOData[dataInd] = randOff; dataInd++;
						
						//cenVec
						
						singleton->limbTBOData[dataInd] = centerPoint.getX(); dataInd++;
						singleton->limbTBOData[dataInd] = centerPoint.getY(); dataInd++;
						singleton->limbTBOData[dataInd] = centerPoint.getZ(); dataInd++;
						singleton->limbTBOData[dataInd] = curOrgNode->orgVecs[E_OV_POWVALS].getFX(); dataInd++;
						
						singleton->limbTBOData[dataInd] = tanVec.getX(); dataInd++;
						singleton->limbTBOData[dataInd] = tanVec.getY(); dataInd++;
						singleton->limbTBOData[dataInd] = tanVec.getZ(); dataInd++;
						singleton->limbTBOData[dataInd] = curOrgNode->orgVecs[E_OV_POWVALS].getFY(); dataInd++;
						
						singleton->limbTBOData[dataInd] = bitVec.getX(); dataInd++;
						singleton->limbTBOData[dataInd] = bitVec.getY(); dataInd++;
						singleton->limbTBOData[dataInd] = bitVec.getZ(); dataInd++;
						singleton->limbTBOData[dataInd] = curOrgNode->orgVecs[E_OV_POWVALS].getFZ(); dataInd++;
						
						// if (singleton->doPathReport) {
						// 	cout << curOrgNode->orgVecs[E_OV_MATPARAMS].getFX() << "\n";
						// }
						
						singleton->limbTBOData[dataInd] = norVec.getX(); dataInd++;
						singleton->limbTBOData[dataInd] = norVec.getY(); dataInd++;
						singleton->limbTBOData[dataInd] = norVec.getZ(); dataInd++;
						singleton->limbTBOData[dataInd] = curOrgNode->orgVecs[E_OV_POWVALS].getFW(); dataInd++;
						
						// ln0Vec
						
						singleton->limbTBOData[dataInd] = len0.getX(); dataInd++;
						singleton->limbTBOData[dataInd] = len0.getY(); dataInd++;
						singleton->limbTBOData[dataInd] = len0.getZ(); dataInd++;
						singleton->limbTBOData[dataInd] = ge->entType; dataInd++;
						
						// ln1Vec
						
						singleton->limbTBOData[dataInd] = len1.getX(); dataInd++;
						singleton->limbTBOData[dataInd] = len1.getY(); dataInd++;
						singleton->limbTBOData[dataInd] = len1.getZ(); dataInd++;
						singleton->limbTBOData[dataInd] = curOrgNode->orgVecs[E_OV_TBNOFFSET].getFW(); dataInd++;
						
						
					}
				}
				
				singleton->limbTBOData[headerStart+0] = dataInd/4;
				singleton->limbTBOData[headerStart+1] = 0.0f;
				singleton->limbTBOData[headerStart+2] = 0.0f;
				singleton->limbTBOData[headerStart+3] = 0.0f;
				
				
				
			}
		}
		
		// pad end with blank data
		
		singleton->limbTBOData[dataInd] = 0.0f; dataInd++;
		singleton->limbTBOData[dataInd] = 0.0f; dataInd++;
		singleton->limbTBOData[dataInd] = 0.0f; dataInd++;
		singleton->limbTBOData[dataInd] = 0.0f; dataInd++;
		
		singleton->actorCount = actorCount;
		
		singleton->limbTBO.update(singleton->limbTBOData,NULL,dataInd*4);
		
		// if (singleton->doPathReport) {
		// 	cout << "\n\n";
		// }
		// singleton->doPathReport = false;
	}

	void drawPrim(bool doSphereMap, bool doTer, bool doPoly) {
		int i;
		
		int ind = 0;
		
		if (doSphereMap) {
			
			//cout << "doSphereMap\n";
			
			ind = 1;
		}
		
		
		bool skipPrim =			
			(singleton->gameFluid[E_FID_BIG]->curGeomCount == 0) &&
			(singleton->settings[E_BS_PLACING_GEOM] == false);
		
		
		bool doPrim = !doTer;
		
		int curGeomCount = 0;
		
		
		
		
		
		if (doPrim) {
			ind = 2;
			
			curGeomCount = singleton->gameFluid[E_FID_BIG]->curGeomCount;
			
			if (skipPrim) {
				singleton->copyFBO2("terTargFBO","solidBaseTargFBO", 0, 1);
				singleton->copyFBO2("terTargFBO","waterTargFBO", 2, 3);
				singleton->copyFBO2("terTargFBO","prmDepthFBO", 4, 5);
				singleton->copyFBO("terTargFBO","numstepsFBO", 7);
				skippedPrim = true;
				return;
			}
			
		}
		else {
			curGeomCount = MAX_PRIM_IDS;
		}
		
		
		
		if (doPoly) {
			singleton->bindShader("PrimShader_330_DOTER_DOPOLY");
			singleton->bindFBO("polyFBO");
		}
		else {
			if (doTer) {
				
				if (USE_SPHERE_MAP) {
					singleton->bindShader("PrimShader_330_DOTER_USESPHEREMAP");
				}
				else {
					singleton->bindShader("PrimShader_330_DOTER");
				}
				
			}
			else {
				singleton->bindShader("PrimShader_330_DOPRIM");
			}
			singleton->bindFBO(curTargFBO[ind]);
		}
		
		
		
		
		
		
		
		
		singleton->setShaderTexture3D(0, singleton->gameFluid[E_FID_BIG]->volIdPrim[0]);
		
		if (doPrim) {
			singleton->setShaderTBO(
				1,
				singleton->gameFluid[E_FID_BIG]->tboWrapper.tbo_tex,
				singleton->gameFluid[E_FID_BIG]->tboWrapper.tbo_buf,
				true
			);
		}
		else {
			singleton->setShaderTBO(
				1,
				singleton->limbTBO.tbo_tex,
				singleton->limbTBO.tbo_buf,
				true
			);
		}
		
		
		
		singleton->sampleFBO("hmFBOLinearBig",2);
		
		singleton->sampleFBO("rasterFBO0",3);
		//singleton->sampleFBO("terDepthFBO",3);
		
		//if (USE_SPHERE_MAP) {
		//	singleton->sampleFBO("sphDepthFBO",5);
		//}
		
		singleton->sampleFBO("geomTargFBO",5);
		
		// sample opposite pass
		if (doPrim) {
			singleton->sampleFBO("terTargFBO",7, -1, 0, 6);
		}
		else {
			singleton->sampleFBO("prmTargFBO",7, -1, 0, 6);
		}
		
		singleton->setShaderTexture3D(13, singleton->volumeWrappers[E_VW_VORO]->volId);
		//singleton->setShaderTexture3D(14, singleton->volumeWrappers[E_VW_WORLD]->volId);
		
		
		//singleton->sampleFBO("noiseFBOLinear", 15);
		singleton->setShaderTexture(15, singleton->imageVoro->tid);
		
		// if (!doPoly) {
		// 	singleton->sampleFBO(polyFBOStrings[NUM_POLY_STRINGS],14);
		// }
		
		
		
		
		if ((singleton->gem->getCurActor() == NULL)||singleton->gem->firstPerson) {
			singleton->setShaderFloat("thirdPerson", 0.0f);
			//singleton->setShaderFloat("CAM_BOX_SIZE", 0.5f);
		}
		else {
			singleton->setShaderFloat("thirdPerson", 1.0f);
			//singleton->setShaderFloat("CAM_BOX_SIZE", 0.5f);
			singleton->setShaderfVec3("entPos", singleton->gem->getCurActor()->getCenterPointFIV(0));
		}
		
		
		singleton->setShaderFloat("isUnderWater", -(singleton->getUnderWater()-0.5f)*2.0f );
		
		
		singleton->setShaderMatrix4x4("modelview",singleton->viewMatrix.get(),1);
		singleton->setShaderMatrix4x4("proj",singleton->projMatrix.get(),1);
		singleton->setShaderMatrix4x4("modelviewInverse",singleton->viewMatrixDI,1);
		//singleton->setShaderInt("readPoly", (int)(readPoly));
		singleton->setShaderInt("depthInvalidMove", (int)(singleton->depthInvalidMove));
		singleton->setShaderInt("depthInvalidRotate", (int)(singleton->depthInvalidRotate));
		
		singleton->setShaderFloat("voroSize",singleton->volumeWrappers[E_VW_VORO]->terGenDim.getFZ());
		
		singleton->setShaderInt("actorCount",singleton->actorCount);
		singleton->setShaderInt("MAX_PRIM_IDS", min(curGeomCount,MAX_PRIM_IDS));
		singleton->setShaderInt("MAX_PRIMTEST", min(curGeomCount,MAX_PRIMTEST));
		
		
		singleton->setShaderFloat("invalidCount", invalidCount/invalidCountMax);
		singleton->setShaderInt("doSphereMap",
			// (int)(doSphereMap)
			(int)(singleton->sphereMapOn)
		);
		singleton->setShaderInt("testOn", (int)(singleton->settings[E_BS_TEST_1]));
		singleton->setShaderInt("skipPrim", (int)(skipPrim));
		singleton->setShaderInt("placingGeom", (int)(singleton->settings[E_BS_PLACING_GEOM]));
		
		
		
		singleton->setShaderfVec3("waterMin", &(singleton->gameFluid[E_FID_BIG]->curWaterMin) );
		singleton->setShaderfVec3("waterMax", &(singleton->gameFluid[E_FID_BIG]->curWaterMax) );
		
		
		singleton->setShaderfVec3("volMinReadyInPixels", &(singleton->gameFluid[E_FID_BIG]->volMinReadyInPixels) );
		singleton->setShaderfVec3("volMaxReadyInPixels", &(singleton->gameFluid[E_FID_BIG]->volMaxReadyInPixels) );
		
		
		singleton->setShaderFloat("mapPitch", singleton->mapPitch);
		singleton->setShaderFloat("seaLevel", singleton->getSLNormalized() );
		singleton->setShaderFloat("heightMapMaxInCells", singleton->heightMapMaxInCells);
		singleton->setShaderfVec4("mapFreqs", &(singleton->mapFreqs) );
		singleton->setShaderfVec4("mapAmps", &(singleton->mapAmps) );
		
		singleton->setShaderFloat("SPHEREMAP_SCALE_FACTOR", SPHEREMAP_SCALE_FACTOR);
		singleton->setShaderFloat("UNIT_MAX", FLUID_UNIT_MAX + 1);
		singleton->setShaderFloat("waterLerp", singleton->gameFluid[E_FID_BIG]->waterLerp); //todo: E_FID_SML?
		singleton->setShaderFloat("volSizePrim", singleton->gameFluid[E_FID_BIG]->volSizePrim);
		singleton->setShaderFloat("curTime", singleton->pauseTime/1000.0f);
		singleton->setShaderfVec2("bufferDim", &(singleton->bufferRenderDim) );
		singleton->setShaderfVec3("cameraPos", singleton->cameraGetPos());
		singleton->setShaderfVec3("lookAtVec", &(singleton->lookAtVec) );
		singleton->setShaderfVec3("lightVec", &(singleton->lightVec) );
		singleton->setShaderFloat("cellsPerWorld", cellsPerWorld );
		
		// singleton->setShaderFloat("volSizePrimSmall", singleton->gameFluid[E_FID_SML]->volSizePrim);
		// singleton->setShaderfVec3("volMinReadyInPixelsSmall", &(singleton->gameFluid[E_FID_SML]->volMinReadyInPixels) );
		// singleton->setShaderfVec3("volMaxReadyInPixelsSmall", &(singleton->gameFluid[E_FID_SML]->volMaxReadyInPixels) );
		
		singleton->setShaderVec2("clipDist",singleton->clipDist[0],singleton->clipDist[1]);
		singleton->setShaderFloat("FOV", singleton->FOV*M_PI/180.0f);
		
		
		//paramFetch1: x,y,z: position; w: template number
		//paramFetch2: x:diagMask, y:uid, z:?, w:?
		
		
		
		tempVec1.copyFrom(&(singleton->geomPoints[0]));
		tempVec1.addXYZRef(&(singleton->geomOrigOffset));
		tempVec2.setFXYZW(0.0f,-99.0f,0.0f,0.0f);
		singleton->setShaderfVec4("paramFetch1", &tempVec1 );
		singleton->setShaderfVec4("paramFetch2", &tempVec2 );
		singleton->setShaderArrayfVec4("paramArrGeom", singleton->paramArrGeom, E_PRIMTEMP_LENGTH);
		
		
		if (singleton->gem->getCurActor() != NULL) {
			
			singleton->splashArr[0] = singleton->gem->getCurActor()->getCenterPointFIV(0)->getFX();
			singleton->splashArr[1] = singleton->gem->getCurActor()->getCenterPointFIV(0)->getFX();
			singleton->splashArr[2] = singleton->gem->getCurActor()->getCenterPointFIV(0)->getFX();
			singleton->splashArr[3] = singleton->gem->getCurActor()->getVel(0)->length();
			
			singleton->setShaderInt("numSplashes", 1);
			singleton->setShaderArrayfVec4("splashArr", singleton->splashArr, MAX_SPLASHES);
		}
		else {
			singleton->setShaderInt("numSplashes", 0);
		}
		
		
		
		if (singleton->settings[E_BS_WATER_BULLET]) {
			singleton->setShaderInt("numExplodes", 0);
		}
		else {
			singleton->setShaderInt("numExplodes", singleton->sphereStack.size());
			
			if ( singleton->sphereStack.size() > 0) {
				for (i = 0; i < singleton->sphereStack.size();i++) {
					singleton->explodeArr[i*4+0] = singleton->sphereStack[i].position[0];
					singleton->explodeArr[i*4+1] = singleton->sphereStack[i].position[1];
					singleton->explodeArr[i*4+2] = singleton->sphereStack[i].position[2];
					singleton->explodeArr[i*4+3] = singleton->sphereStack[i].curRad;
				}
				singleton->setShaderArrayfVec4("explodeArr", singleton->explodeArr, MAX_EXPLODES);
			}
		}
		
		
		if (doPoly) {
			//rasterPolys(0,5);
		}
		else {
			singleton->fsQuad.draw();
		}
		
		// if (!doPoly) {
		// 	singleton->unsampleFBO(polyFBOStrings[NUM_POLY_STRINGS],14);
		// }
		
		
		//singleton->unsampleFBO("noiseFBOLinear", 15);
		singleton->setShaderTexture(15, 0);
		//singleton->setShaderTexture3D(14, 0);
		singleton->setShaderTexture3D(13, 0);
		
		
		if (doPrim) {
			singleton->unsampleFBO("terTargFBO",7, -1, 0, 6);
		}
		else {
			singleton->unsampleFBO("prmTargFBO",7, -1, 0, 6);
		}
		
		// if (USE_SPHERE_MAP) {
		// 	singleton->unsampleFBO("sphDepthFBO",5);
		// }
		singleton->unsampleFBO("geomTargFBO",5);
		
		singleton->unsampleFBO("rasterFBO0",3);
		//singleton->unsampleFBO("terDepthFBO",3);
		singleton->unsampleFBO("hmFBOLinearBig",2);
		
		singleton->setShaderTBO(1,0,0,true);
		singleton->setShaderTexture3D(1, 0);
		
		singleton->unbindFBO();
		singleton->unbindShader();
		
		if (doPoly) {
			return;
		}
		
		if (doTer) {
			singleton->copyFBO("terTargFBO", "limbFBO", 6);
		}
		
		
		singleton->copyFBO2(curTargFBO[ind],curDepthFBO[ind], 4, 5);
		
		if (doPrim) {
			singleton->copyFBO2("prmTargFBO","waterTargFBO", 2, 3);
			singleton->copyFBO2("prmTargFBO","solidBaseTargFBO", 0, 1);
		}
		
		
		
		
	}

	
	


	void drawOrg(GameOrg* curOrg, bool drawAll) {
		
		if (curOrg == NULL) {
			return;
		}
		
		float scale = 1.0f;
		
		
		
		
		
		// tangents
		singleton->setShaderVec3("matVal", 255, 0, 0 );
		drawNodeEnt((curOrg->baseNode),&(curOrg->basePosition), scale, 0, drawAll);
		
		// bitangents
		singleton->setShaderVec3("matVal", 0, 255, 0);
		drawNodeEnt((curOrg->baseNode),&(curOrg->basePosition), scale, 1, drawAll);
		
		// normals
		singleton->setShaderVec3("matVal", 0, 0, 255);
		drawNodeEnt((curOrg->baseNode),&(curOrg->basePosition), scale, 2, drawAll);
		
		if (drawAll) {
			// nodes
			singleton->setShaderVec3("matVal", 254, 254, 254);
			drawNodeEnt((curOrg->baseNode),&(curOrg->basePosition), scale, 3, drawAll);
		}
		
		
		
	}


	void drawNodeEnt(
		GameOrgNode* curNode,
		FIVector4* basePosition,
		float scale,
		int drawMode,
		bool drawAll
	) {
		
		
		BaseObj* grabber;
		
		
		bool doProc = false;
		
		if (drawAll) {
			doProc = true;
		}
		else {
			if (curNode == singleton->gem->selectedNode) {
				doProc = true;
			}
		}
		
		int i;
		
		
		if (doProc) {
			lineSeg[0].setFXYZRef(&(curNode->orgTrans[0]));
			lineSeg[0].multXYZ(  scale  );
			
			// if (drawAll) {
			// 	lineSeg[1].setFXYZRef(&(curNode->tbnTrans[drawMode%3]));
			// 	lineSeg[1].multXYZ(  scale  );
			// }
			// else {
				lineSeg[1].setFXYZRef(&(curNode->tbnRotC[drawMode%3]));
				lineSeg[1].multXYZ( 1.0f );//(curNode->orgVecs[E_OV_TBNRAD0][drawMode%3]*scale)  ); //*16.0f
				//lineSeg[1].multXYZ(&(curNode->tbnRadScale0));
				lineSeg[1].addXYZRef(&(lineSeg[0]));
			//}
			
			if (singleton->gem->getCurActor() != NULL) {
				
				
				if (singleton->gem->getCurActor()->isGrabbedById > -1) {
					grabber = &(singleton->gem->gameObjects[singleton->gem->getCurActor()->isGrabbedById]);
				}
				else {
					grabber = singleton->gem->getCurActor();
				}
				
				
				lineSeg[0].addXYZ(0.0f,0.0f,grabber->skelOffset.getZ());
				lineSeg[1].addXYZ(0.0f,0.0f,grabber->skelOffset.getZ());
			}
			
			
			
			
			switch(drawMode) {
				case 0: // tangents
				case 1: // bitangents
				case 2: // normals
					singleton->drawLine(&(lineSeg[0]),&(lineSeg[1]));
				break;
				case 3: // nodes
					singleton->drawCubeCentered(&(lineSeg[1]),0.0125f*scale);
				break;
				
			}
		}
		
		
		for (i = 0; i < curNode->children.size(); i++) {
			drawNodeEnt(curNode->children[i], basePosition, scale, drawMode, drawAll);
		}
		
	}
	
	
	
	
	void polyCombine() {
		int q;
		
		singleton->bindShader("PolyCombineShader");
		singleton->bindFBO(polyFBOStrings[NUM_POLY_STRINGS]);
		
		for (q = 0; q < NUM_POLY_STRINGS; q++) {
			singleton->sampleFBO(polyFBOStrings[q], q);
		}
		
		singleton->fsQuad.draw();
		
		for (q = NUM_POLY_STRINGS-1; q >= 0; q--) {
			singleton->unsampleFBO(polyFBOStrings[q], q);
		}
		
		singleton->unbindFBO();
		singleton->unbindShader();
	}
	
	
	void drawPolys(
		string fboName,
		int minPeel,
		int maxPeel
		//, bool isBlockHolder
	) {
		
		//VolumeWrapper* curVW = (singleton->volumeWrappers[E_VW_VORO]);
		
		singleton->bindShader("PolyShader");
		singleton->bindFBO(fboName);
		
		//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		
		//singleton->sampleFBO("polyFBO",0,fboNum);
		
		///singleton->setShaderTexture3D(0, singleton->gameFluid[E_FID_BIG]->volIdPrim[0]);
		//singleton->sampleFBO("hmFBOLinearBig",2);
		//singleton->setShaderTexture3D(13, curVW->volId);
		
		if (polyFBOStrings[0].compare(fboName) == 0) {
			singleton->setShaderVec4("maskVals", 1.0f, 0.0f, 0.0f, 0.0f);
		}
		else {
			singleton->setShaderVec4("maskVals", 0.0f, 1.0f, 0.0f, 0.0f);
		}
		
		singleton->setShaderFloat("volSizePrim", singleton->gameFluid[E_FID_BIG]->volSizePrim);
		singleton->setShaderFloat("FOV", singleton->FOV*M_PI/180.0f);
		singleton->setShaderVec2("clipDist",singleton->clipDist[0],singleton->clipDist[1]);
		singleton->setShaderfVec2("bufferDim", &(singleton->bufferRenderDim) );
		singleton->setShaderfVec3("cameraPos", singleton->cameraGetPos());
		singleton->setShaderfVec3("volMinReadyInPixels", &(singleton->gameFluid[E_FID_BIG]->volMinReadyInPixels) );
		singleton->setShaderfVec3("lookAtVec", &(singleton->lookAtVec) );
		
		singleton->setShaderMatrix4x4("modelviewInverse",singleton->viewMatrixDI,1);
		singleton->setShaderMatrix4x4("modelview",singleton->viewMatrix.get(),1);
		singleton->setShaderMatrix4x4("proj",singleton->projMatrix.get(),1);
		
		// if (isBlockHolder) {
		// 	rasterWorldPolys();
		// }
		// else {
		//	rasterPolys(minPeel,maxPeel*4, 6);
		//}
		
		
		
		// singleton->setShaderTexture3D(13, 0);
		// singleton->unsampleFBO("hmFBOLinearBig",2);
		// singleton->setShaderTexture3D(0,0);
		
		//singleton->unsampleFBO("polyFBO",0,fboNum);
		singleton->unbindFBO();
		singleton->unbindShader();
	}
	
	// void rasterWorldPolys() {
		
		
	// 	GamePageHolder* curHolder = blockHolder;
		
	// 	if (curHolder == NULL) {
	// 		return;
	// 	}
		
	// 	if (
	// 		(curHolder->readyToRender) &&
	// 		(!(curHolder->listEmpty))
	// 	) {
	// 		curHolder->vboWrapper.draw();
	// 	}
		
		
		
	// }
	
	
	
	
	
	
	
// 	void updateTBOPool(int rad) {
		
// 		int q;
// 		int p;
		
// 		int ii;
// 		int jj;
// 		int kk;
		
// 		int cellsPerHolder = singleton->cellsPerHolder;
		
// 		GamePageHolder* curHolder;
		
// 		minv.copyFrom(&camHolderPos);
// 		maxv.copyFrom(&camHolderPos);
		
		
// 		int minK = minv.getIZ() - rad;
// 		int maxK = maxv.getIZ() + rad;
// 		int minJ = minv.getIY() - rad;
// 		int maxJ = maxv.getIY() + rad;
// 		int minI = minv.getIX() - rad;
// 		int maxI = maxv.getIX() + rad;
		
// 		int curTBOPool = 0;
// 		int writeOffset = 0;
// 		int curCubeInd = 0;
		
// 		int writeOffset0;
// 		int writeOffset1;
		
// 		TBOEntry* curTBO = &(singleton->tboPool[0]);
// 		curTBO->lock();
		
// 		int curPtr;
// 		int curSize;
		
// 		float tempData[12];
		
// 		float voxelsPerHolder = singleton->voxelsPerHolder;
		
		
// 		int tboCount = 0;
		
// 		for (kk = minK; kk < maxK; kk++) {
// 			for (jj = minJ; jj < maxJ; jj++) {
// 				for (ii = minI; ii < maxI; ii++) {
// 					curHolder = getHolderAtCoords(ii,jj,kk,true);
					
// 					if (curHolder == NULL) {
						
// 					}
// 					else {
						
// 						if (curHolder->lockWrite) {
							
// 						}
// 						else {
// 							if (
// 								(curHolder->readyToRender) &&
// 								(!(curHolder->listEmpty))
// 							) {
								
// 								writeOffset = curTBO->writeIndex;
							
// 								if (curTBO->cantWrite(CUBE_DATA_SIZE)) {
// 									goto TBO_FULL;
// 								}
								
								
// 								// fill up cells with pointers
// 								for (q = 0; q < CUBE_DATA_SIZE; q++) {
									
// 									curCubeInd = curHolder->cubeData[q];
									
// 									if (curCubeInd == CUBE_DATA_INVALID) {
// 										curTBO->writeData(CUBE_DATA_INVALID);
// 									}
// 									else {
// 										curTBO->writeData(
// 											(curCubeInd)*CUBE_WRAP_SIZE + CUBE_DATA_SIZE + writeOffset
// 										);
// 									}
									
									
// 								}
								
// 								curSize = curHolder->cubeWraps.size();
// 								// fill up list of cubeWraps
// 								for (q = 0; q < curSize; q++) {
									
// 									if (curTBO->cantWrite(CUBE_WRAP_SIZE)) {
// 										goto TBO_FULL;
// 									}
									
// 									curTBO->writeDataArr(
// 										curHolder->cubeWraps[q].data,
// 										CUBE_WRAP_SIZE
// 									);
// 								}
								
								
// 								// curOffset
// 								writeOffset0 = writeOffset/4096;
// 								writeOffset1 = writeOffset - writeOffset0*4096;
								
// 								// vdata0
// 								tempData[0] = writeOffset0;
// 								tempData[1] = writeOffset1;
// 								tempData[2] = 0.0f;
// 								tempData[3] = 0.0f;
								
// 								// vdata1
// 								tempData[4] = curHolder->gphMinInCells[0];
// 								tempData[5] = curHolder->gphMinInCells[1];
// 								tempData[6] = curHolder->gphMinInCells[2];
// 								tempData[7] = 1.0f;
								
// 								// vdata2
// 								tempData[8] = curHolder->gphMaxInCells[0];
// 								tempData[9] = curHolder->gphMaxInCells[1];
// 								tempData[10] = curHolder->gphMaxInCells[2];
// 								tempData[11] = 1.0f;
								
// 								curTBO->vbo.vboBox(
// 									curHolder->gphMinInCells[0],
// 									curHolder->gphMinInCells[1],
// 									curHolder->gphMinInCells[2],
									
// 									0.0f,
// 									cellsPerHolder,
									
// 									ALL_FACES,
// 									tempData,
// 									12
// 								);
								
// 								tboCount++;
								
								
// 							}
// 						}
// 					}
// 				}
// 			}
// 		}
		
// TBO_FULL:
		
// 		if (curTBO->cantWrite(1)) {
// 			cout << "TBO FULL\n";
// 		}
		
// 		curTBO->unlock();
		
		
// 		cout << "writeIndex " << curTBO->writeIndex << "\n";
		
// 		cout << "tboCount " << tboCount << "\n";
		
// 		glFlush();
// 		glFinish();
		
// 	}
	
	
	
	
	
	
	
	
		void rastChunk(
			int rad,
			uint flags
			// bool drawLoading,
			// bool doCheck,
			// bool clipToView
		) {
			
			
			
			int ii;
			int jj;
			int kk;
			
			bool doProc = false;
			
			//GamePageHolder* curHolder;
			GameChunk* curChunk;
			
			
			minv.copyFrom(&camChunkPos);
			maxv.copyFrom(&camChunkPos);
			
			FIVector4 tempFIV;
			
			
			int minK = minv.getIZ() - rad;
			int maxK = maxv.getIZ() + rad;
			int minJ = minv.getIY() - rad;
			int maxJ = maxv.getIY() + rad;
			int minI = minv.getIX() - rad;
			int maxI = maxv.getIX() + rad;
			
			float disClip = singleton->cellsPerChunk*2;
			
			// if (getBounds) {
			// 	minShadowBounds.setFXYZ(16777216.0f,16777216.0f,16777216.0f);
			// 	maxShadowBounds.setFXYZ(0.0f,0.0f,0.0f);
			// }
			
			
			for (kk = minK; kk < maxK; kk++) {
				for (jj = minJ; jj < maxJ; jj++) {
					for (ii = minI; ii < maxI; ii++) {
						//curHolder = getHolderAtCoords(ii,jj,kk,true);
						curChunk = getChunkAtCoords(ii,jj,kk,false);
						
						if (curChunk == NULL) {
							
						}
						else {
							
							if ((flags&RH_FLAG_DRAWLOADING) > 0) {
								
								curChunk->drawLoadingHolders();
								

								
							}
							else {
								
								if ((flags&RH_FLAG_DOCHECK) > 0) {
									
									
									
									curChunk->checkHolders();
								}
								
								
								if (
									(curChunk->readyToRender) &&
									(!(curChunk->listEmpty))
								) {
									
									
									doProc = false;
									if ((flags&RH_FLAG_CLIPTOVIEW) > 0) {
										
										tempFIV.copyFrom(&(curChunk->chunkCenInCells));
										tempFIV.addXYZRef(singleton->cameraGetPosNoShake(),-1.0f);
										tempFIV.normalize();
										
										if (
											(tempFIV.dot(&(singleton->lookAtVec)) > singleton->conVals[E_CONST_DOT_CLIP])
											|| (curChunk->chunkCenInCells.distance(singleton->cameraGetPosNoShake()) < disClip)
										) {
											doProc = true;
										}
									}
									else {
										doProc = true;
									}
									
									
									if (doProc) {
										if (DO_POINTS) {
											curChunk->getCurVBO()->drawPoints();
										}
										else {
											curChunk->getCurVBO()->draw();
										}
									}
									
								}
								
								
								
							}
							
							
							
							
							
							
						}
					}
				}
			}
			
			
			//float boundsDepth = maxShadowBounds.getFZ() - minShadowBounds.getFZ();
			
			// if (getBounds) {
				
			// 	minShadowBoundsGrow.copyFrom(&minShadowBounds);
			// 	minShadowBoundsGrow.addXYZ(
			// 		-abs(singleton->lightVec.getFX())*boundsDepth,
			// 		-abs(singleton->lightVec.getFY())*boundsDepth,
			// 		0.0f
			// 	);
			// 	minShadowBounds.minXYZ(&minShadowBoundsGrow);
				
			// 	maxShadowBoundsGrow.copyFrom(&maxShadowBounds);
			// 	maxShadowBoundsGrow.addXYZ(
			// 		abs(singleton->lightVec.getFX())*boundsDepth,
			// 		abs(singleton->lightVec.getFY())*boundsDepth,
			// 		0.0f
			// 	);
			// 	maxShadowBounds.maxXYZ(&maxShadowBoundsGrow);
			// }
			
		}
	
	
	
	
	
	
	// void rasterPolys(
	// 	int minPeel,
	// 	int maxPeel,
	// 	int extraRad = 0,
	// 	bool doPoints = false
	// ) {
		
	// 	int q;
		
	// 	int ii;
	// 	int jj;
	// 	int kk;
		
	// 	int cellsPerHolder = singleton->cellsPerHolder;
		
		
		
	// 	GamePageHolder* curHolder;
		
	// 	minv.copyIntDiv(&(singleton->gameFluid[E_FID_BIG]->volMinReadyInPixels),cellsPerHolder);
	// 	maxv.copyIntDiv(&(singleton->gameFluid[E_FID_BIG]->volMaxReadyInPixels),cellsPerHolder);
		
		
		
	// 	bool idealDis = false;
		
	// 	int pCount = 0;
		
		
	// 	int bi = singleton->lastHolderPos.getIX();
	// 	int bj = singleton->lastHolderPos.getIY();
	// 	int bk = singleton->lastHolderPos.getIZ();
		
	// 	int curRad = 0;
		
		
	// 	int minK = minv.getIZ() - extraRad;
	// 	int maxK = maxv.getIZ() + extraRad;
	// 	int minJ = minv.getIY() - extraRad;
	// 	int maxJ = maxv.getIY() + extraRad;
	// 	int minI = minv.getIX() - extraRad;
	// 	int maxI = maxv.getIX() + extraRad;
		
	// 	int qmod;
		
	// 	if (minPeel < maxPeel) {
	// 		qmod = 1;
	// 	}
	// 	else {
	// 		qmod = -1;
	// 	}
		
	// 	for (q = minPeel; q != maxPeel; q += qmod) {
			
	// 		// if (q <= 1) {
	// 		// 	doClear = 1;
	// 		// }
	// 		// else {
	// 		// 	doClear = 0;
	// 		// }
			
			
	// 		// switch(q) {
	// 		// 	case 0:
	// 		// 		singleton->setShaderVec4("maskVals", 1.0f, 0.0f, 0.0f, 0.0f);
	// 		// 	break;
	// 		// 	case 1:
	// 		// 		singleton->setShaderVec4("maskVals", 0.0f, 1.0f, 0.0f, 0.0f);
	// 		// 	break;
	// 		// 	case 2:
	// 		// 		singleton->setShaderVec4("maskVals", 0.0f, 0.0f, 1.0f, 0.0f);
	// 		// 	break;
	// 		// 	case 3:
	// 		// 		singleton->setShaderVec4("maskVals", 0.0f, 0.0f, 0.0f, 1.0f);
	// 		// 	break;
	// 		// 	case 4:
	// 		// 		singleton->setShaderVec4("maskVals", 1.0f, 0.0f, 0.0f, 0.0f);
	// 		// 	break;
	// 		// }
			
			
	// 		// singleton->bindFBO("polyFBO");//,swapFlag);
	// 		// //singleton->sampleFBO("polyFBO", 0, swapFlag);
			
	// 		for (kk = minK; kk < maxK; kk++) {
	// 			for (jj = minJ; jj < maxJ; jj++) {
	// 				for (ii = minI; ii < maxI; ii++) {
	// 					curHolder = getHolderAtCoords(ii,jj,kk,true);
						
	// 					if (curHolder == NULL) {
							
	// 					}
	// 					else {
							
	// 						curRad = max(max(abs(bi-ii),abs(bj-jj)),abs(bk-kk));
							
	// 						// if (q == 4) {
	// 						// 	idealDis = true;
	// 						// }
	// 						// else {
	// 						// 	if (q == 3) {
	// 						// 		idealDis = (q>=curRad);
	// 						// 	}
	// 						// 	else {
	// 						// 		idealDis = (q==curRad);
	// 						// 	}
	// 						// }
							
	// 						if (curHolder->lockWrite) {
								
	// 						}
	// 						else {
	// 							if (
	// 								(curHolder->readyToRender) &&
	// 								(!(curHolder->listEmpty)) &&
	// 								((q==curRad) || (q == -1))
	// 							) {
									
	// 								pCount++;
									
	// 								// singleton->setShaderFloat("volSizePrim", singleton->cellsPerHolder);
	// 								// singleton->setShaderfVec3("volMinReadyInPixels", &(curHolder->gphMinInCells) );
	// 								// singleton->setShaderfVec3("volMaxReadyInPixels", &(curHolder->gphMaxInCells) );
	// 								// singleton->setShaderTexture3D(0, curHolder->terVW->volId);
									
									
	// 								if (doPoints) {
	// 									curHolder->vboWrapper.drawPoints();
	// 								}
	// 								else {
	// 									curHolder->vboWrapper.draw();
	// 								}
									
	// 								//
									
									
	// 							}
	// 						}
	// 					}
	// 				}
	// 			}
	// 		}
			
	// 		//singleton->unsampleFBO("polyFBO", 0);//, swapFlag);
	// 		//singleton->unbindFBO();
			
	// 		//swapFlag = 1 - swapFlag;
			
	// 	}
		
		
	// 	// singleton->setShaderTexture3D(13, 0);
	// 	// singleton->unsampleFBO("hmFBOLinearBig",2);
	// 	// singleton->setShaderTexture3D(0,0);
		
	// 	// singleton->unbindShader();
		
	// 	singleton->polyCount = pCount;
		
		
	// }
	

	void renderGeom()
	{

		int i;
		int j;
		int k;
		int n;
		int q;
		
		int ii;
		int jj;
		int kk;
		int ii2;
		int jj2;
		int kk2;
		
		FIVector4 p0;
		FIVector4 p1;
		FIVector4 p2;
		FIVector4 p3;
		FIVector4 rotVec;
		
		int ind;
		int lastId;
		int curId;
		int curInd;
		int cameFromInd;
		
		int cellsPerHolder = singleton->cellsPerHolder;
		
		
		float frameMod;
		int objCount = 0;
		
		int curOr;
		bool doProc;
		int cellVal;
		int xmax,ymax,zmax;
		int xmin,ymin,zmin;
		
		CharStruct* tempCS;
		
		BaseObj* curObj;
		BaseObj* eqObj;
		
		bool showHit = false;
		float dirVecLength = 4.0f;
		
		float x1 = 0.0f;
		float x2 = 0.0f;
		float y1 = 0.0f;
		float y2 = 0.0f;
		float iw = singleton->fontWrappers[EFW_ICONS]->fontImage->width;
		float ih = singleton->fontWrappers[EFW_ICONS]->fontImage->height;



		// singleton->bindShader("GeomShader");
		// singleton->bindFBO("geomBaseTargFBO");
		// singleton->setShaderFloat("objectId",0.0);
		// singleton->setShaderfVec3("cameraPos", singleton->cameraGetPos());
		// singleton->setShaderfVec3("lookAtVec", &(singleton->lookAtVec));
		// singleton->setShaderFloat("isWire", 0.0);
		// singleton->setShaderFloat("clipDist",singleton->clipDist[1]);
		// singleton->setShaderfVec3("offsetPos",&(singleton->origin));
		// singleton->setShaderMatrix4x4("modelview",singleton->viewMatrix.get(),1);
		// singleton->setShaderMatrix4x4("proj",singleton->projMatrix.get(),1);
		
		
		
		
		// singleton->setShaderVec3("matVal", 30, 30, 30);
		
		
		// //singleton->gamePhysics->example->renderScene();
		
		
		
		
		// for(i = 0; i < singleton->gem->visObjects.size(); i++) {
			
		// 	curObj = &(singleton->gem->gameObjects[visObjects[i]]);
			
		// 	objCount++;
			
		// 	if (curObj->isHidden) {
				
		// 	}
		// 	else {
								
				
				
				
		// 		// tempVec1.copyFrom( &(curObj->boundsMinTransInPixels) );
		// 		// tempVec2.copyFrom( &(curObj->boundsMaxTransInPixels) );
				
				
				
		// 		// tempVec3.copyFrom(curObj->getCenterPointFIV(0));
		// 		// tempVec3.setFW(curObj->ang);
				
		// 		// singleton->setShaderfVec4("rotationZ",&tempVec3);
				
		// 		doProc = false;
				
		// 		if (visObjects[i] == singleton->actObjInd) {
					
		// 			if (!singleton->gem->firstPerson) {
						
		// 				// singleton->drawBox(
		// 				// 	&tempVec1,
		// 				// 	&tempVec2
		// 				// );
						
		// 				doProc = true;
						
						
						
						
		// 				// singleton->setShaderFloat("objectId",0);
						
		// 				// tempVec1.copyFrom( curObj->getCenterPointFIV(0) );
						
						
		// 				// curOr = curObj->orientationXYZ.getIX();
		// 				// tempVec2.setFXYZRef( &(singleton->dirVecs[curOr]) );
		// 				// tempVec2.multXYZ(2.0f);
		// 				// tempVec2.addXYZRef(&tempVec1);
		// 				// singleton->setShaderVec3("matVal", 255, 0, 0);
		// 				// singleton->drawLine(&tempVec1,&tempVec2);
						
		// 				// curOr = curObj->orientationXYZ.getIY();
		// 				// tempVec2.setFXYZRef( &(singleton->dirVecs[curOr]) );
		// 				// tempVec2.multXYZ(2.0f);
		// 				// tempVec2.addXYZRef(&tempVec1);
		// 				// singleton->setShaderVec3("matVal", 0, 255, 0);
		// 				// singleton->drawLine(&tempVec1,&tempVec2);
						
		// 				// curOr = curObj->orientationXYZ.getIZ();
		// 				// tempVec2.setFXYZRef( &(singleton->dirVecs[curOr]) );
		// 				// tempVec2.multXYZ(2.0f);
		// 				// tempVec2.addXYZRef(&tempVec1);
		// 				// singleton->setShaderVec3("matVal", 0, 0, 255);
		// 				// singleton->drawLine(&tempVec1,&tempVec2);
						
						
		// 				// singleton->setShaderVec3("matVal", 30, 30, 30);
		// 			}
					
					
					
		// 		}
		// 		else {
					
		// 			if (
		// 				(curObj->entType == E_ENTTYPE_BULLET) ||
		// 				(curObj->entType == E_ENTTYPE_TRACE) ||
		// 				(
		// 					(singleton->gem->firstPerson) &&
		// 					(curObj->uid == singleton->gem->getCurActorUID())
		// 				)
		// 			) {
						
		// 			}
		// 			else {
		// 				// singleton->drawBox(
		// 				// 	&tempVec1,
		// 				// 	&tempVec2
		// 				// );
						
		// 				doProc = true;
		// 			}
					
					
		// 		}
				
		// 		if (doProc) {
					
		// 			singleton->setShaderFloat("objectId",visObjects[i]);
					
		// 			// if (curObj->body != NULL) {
		// 			// 	glBegin( GL_TRIANGLES );
		// 			// 	//curObj->body->Render(&q3Rend);
		// 			// 	glEnd( );
		// 			// }
		// 		}
				
		// 	}
								
						
			
			
			
			
		// }
		
		
		
		// tempVec3.setFXYZW(0.0f,0.0f,0.0f,0.0f);
		// singleton->setShaderfVec4("rotationZ",&tempVec3);
		// singleton->setShaderFloat("objectId",0.0);
		




		
		// // singleton->setShaderFloat("isWire", 1.0);
		// // singleton->setShaderVec3("matVal", 255, 0, 0);
		// // minv.setFXYZRef(&(singleton->gameFluid[E_FID_BIG]->curDirtyMin));
		// // maxv.setFXYZRef(&(singleton->gameFluid[E_FID_BIG]->curDirtyMax));
		// // minv.addXYZRef(&(singleton->gameFluid[E_FID_BIG]->volMinReadyInPixels));
		// // maxv.addXYZRef(&(singleton->gameFluid[E_FID_BIG]->volMinReadyInPixels));
		// // minv.addXYZ(-4.0f,-4.0f,0.0f);
		// // maxv.addXYZ(4.0f,4.0f,0.0f);
		// // singleton->drawBox(&minv, &maxv);
		
		
		// // singleton->setShaderFloat("isWire", 1.0);
		// // singleton->setShaderVec3("matVal", 255, 0, 0);
		// // minv.setFXYZRef(&(singleton->gameFluid[E_FID_BIG]->curWaterMin));
		// // maxv.setFXYZRef(&(singleton->gameFluid[E_FID_BIG]->curWaterMax));
		// // minv.addXYZ(-4.0f,-4.0f,0.0f);
		// // maxv.addXYZ(4.0f,4.0f,0.0f);
		// // singleton->drawBox(&minv, &maxv);
		
		
		

		// switch (singleton->mouseState)
		// {

		// case E_MOUSE_STATE_MOVE:
			
			
		
		// 	// if (singleton->settings[E_BS_PLACING_GEOM]) {
				
				
		// 	// 	singleton->setShaderVec3("matVal", 255, 0, 0);
		// 	// 	singleton->setShaderFloat("isWire", 0.0);
				
		// 	// 	for (i = 0; i <= singleton->geomStep; i++) {
					
		// 	// 		singleton->drawCubeCentered(
		// 	// 			&singleton->geomPoints[i],
		// 	// 			0.25f	
		// 	// 		);
		// 	// 	}
		// 	// }
		
		// 	break;
		
		// case E_MOUSE_STATE_BRUSH:
			
			
		// 	if (singleton->earthMod == E_PTT_TER) {
		// 		singleton->setShaderVec3("matVal", 255, 0, 0);
		// 	}
		// 	else {
		// 		singleton->setShaderVec3("matVal", 0, 0, 255);
		// 	}
			
			
		// 	singleton->setShaderFloat("isWire", 1.0);
		// 	singleton->drawCubeCentered(
		// 		&lastUnitPos,
		// 		((int)singleton->curBrushRad)
		// 	);
			
			
		// 	//glClear(GL_DEPTH_BUFFER_BIT);
		// break;
		// case E_MOUSE_STATE_PICKING:

			
			

		// 	if (singleton->highlightedEnt == NULL)
		// 	{

		// 	}
		// 	else
		// 	{
		// 		singleton->setShaderVec3("matVal", 254, 254, 254);
		// 		singleton->setShaderFloat("isWire", 1.0);

		// 		minv.setFXYZRef(singleton->highlightedEnt->getVisMinInPixelsT());
		// 		maxv.setFXYZRef(singleton->highlightedEnt->getVisMaxInPixelsT());

		// 		minv.addXYZ(-0.25);
		// 		maxv.addXYZ(0.25);


		// 		singleton->drawBox(&minv, &maxv);
		// 	}

		// 	if (singleton->selectedEnts.getSelectedEnt() == NULL)
		// 	{

		// 	}
		// 	else
		// 	{
		// 		singleton->setShaderVec3("matVal", 255, 255, 0);
		// 		singleton->setShaderFloat("isWire", 1.0);

		// 		minv.setFXYZRef(singleton->selectedEnts.getSelectedEnt()->getVisMinInPixelsT());
		// 		maxv.setFXYZRef(singleton->selectedEnts.getSelectedEnt()->getVisMaxInPixelsT());

		// 		singleton->drawBox(&minv, &maxv);
		// 	}


		// 	break;

		// }
		
		// if (singleton->settings[E_BS_PATH_FINDING]) {
		// 	singleton->gameLogic->update();
		// }
		
		
		
		// if (
		// 	(singleton->bCtrl) &&
		// 	(singleton->mouseState != E_MOUSE_STATE_BRUSH) &&
		// 	(!(singleton->anyMenuVisible()))	
		// ) {
		// 	for (i = 1; i < singleton->dynObjects.size(); i++) {
		// 		if (singleton->dynObjects[i]->doRender)
		// 		{

		// 			doProc = false;

		// 			if (i >= E_OBJ_LIGHT0)
		// 			{
		// 				if (i - E_OBJ_LIGHT0 < singleton->numDynLights)
		// 				{
		// 					doProc = true;
		// 				}
		// 			}
		// 			else
		// 			{
		// 				doProc = true;
		// 			}

		// 			if (doProc)
		// 			{
		// 				singleton->setShaderFloat("objectId",i);
		// 				singleton->setShaderfVec3("matVal", &(singleton->dynObjects[i]->color) );
		// 				curBoxPos = &(singleton->dynObjects[i]->pos);
		// 				singleton->drawCubeCentered(curBoxPos, singleton->dynObjects[i]->radius);


		// 				if (i == singleton->activeObject)
		// 				{
		// 					//singleton->drawCrossHairs(singleton->dynObjects[i]->pos,4.0f);
		// 				}
		// 			}


		// 		}
		// 	}
		// }
		
		
		// // singleton->setShaderVec3("matVal", 254, 0, 0);
		// // singleton->setShaderFloat("isWire", 1.0);
		// // for (i = 0; i < singleton->nearestLights.selEntList.size(); i++) {
			

		// // 	minv.setFXYZRef(singleton->nearestLights.selEntList[i]->getVisMinInPixelsT());
		// // 	maxv.setFXYZRef(singleton->nearestLights.selEntList[i]->getVisMaxInPixelsT());


		// // 	singleton->drawBox(&minv, &maxv);
		// // }



		
		
		// singleton->unbindFBO();
		// singleton->unbindShader();



		float yval;



		
		
		//~~~~~~~~~~~~~~
		
		singleton->bindShader("BoxShader");
		singleton->bindFBO("geomBaseTargFBO");//, -1, 0); //solidTargFBO
		singleton->setShaderfVec3("lightVec", &(singleton->lightVec) );
		singleton->setShaderFloat("objectId",0.0);
		singleton->setShaderfVec3("cameraPos", singleton->cameraGetPos());
		singleton->setShaderfVec3("lookAtVec", &(singleton->lookAtVec));
		singleton->setShaderFloat("isWire", 0.0);
		singleton->setShaderFloat("clipDist",singleton->clipDist[1]);
		singleton->setShaderMatrix4x4("modelview",singleton->viewMatrix.get(),1);
		singleton->setShaderMatrix4x4("proj",singleton->projMatrix.get(),1);
		singleton->setShaderVec3("matVal", 1, 1, 1);
					
		// glBegin( GL_TRIANGLES );
		// //m_data->m_gl2ShapeDrawer->drawScene(rbWorld,true);
		// singleton->gamePhysics->myShapeDrawer->drawScene(
		// 	singleton->gamePhysics->example->getDynamicsWorld(), true); //drawAll();
		// glEnd( );
		
		//glMatrixMode(GL_MODELVIEW);		
		// singleton->gamePhysics->myShapeDrawer->drawScene(
		// 	singleton->gamePhysics->example->getDynamicsWorld(),
		// 	false
		// );
		
		
		
		if (singleton->gamePhysics != NULL) {
			singleton->drawOrient = false;
			singleton->gamePhysics->example->renderScene();
		}
		
		
		singleton->unbindFBO();
		singleton->unbindShader();
		
		
		singleton->copyFBO2("geomBaseTargFBO","geomTargFBO", 0, 1);
		
		
		//~~~~~~~~~~~~~~
		
		
		
		
		

		//////////////////////////////
		
		
		// singleton->bindShader("CylBBShader");
		// singleton->bindFBO("geomBaseTargFBO", -1, 0);
		// singleton->setShaderfVec2("bufferDim", &(singleton->bufferModDim) );
		// singleton->setShaderFloat("clipDist",singleton->clipDist[1]);
		// singleton->setShaderfVec3("cameraPos", singleton->cameraGetPos());
		// singleton->setShaderFloat("curTime", singleton->pauseTime/1000.0f);
		// singleton->setShaderTexture(0,singleton->fontWrappers[EFW_ICONS]->fontImage->tid);
		
		// for(i = 0; i < singleton->gem->visObjects.size(); i++) {
			
		// 	curObj = &(singleton->gem->gameObjects[visObjects[i]]);
			
		// 	eqObj = singleton->getEquipped(curObj);
			
		// 	if (
		// 		curObj->isHidden ||
		// 		(eqObj == NULL) ||
		// 		(
		// 			(singleton->gem->firstPerson) &&
		// 			(curObj->uid == singleton->gem->getCurActorUID())
		// 		)
		// 	) {
				
		// 	}
		// 	else {
				
				
				
		// 		/////
		// 		tempVec1.copyFrom( curObj->getCenterPointFIV(0) );
		// 		curOr = curObj->orientationXYZ.getIY();
		// 		tempVec2.setFXYZRef( &(singleton->dirVecs[curOr]) );
		// 		//tempVec2.multXYZ(1.0f);
		// 		tempVec2.addXYZRef(&tempVec1);
		// 		//tempVec3.averageXYZ(&tempVec1,&tempVec2);
				
				
		// 		rotVec.copyFrom(curObj->getCenterPointFIV(0));
		// 		//rotVec.addXYZ(0.0,2.0,0.0);
		// 		rotVec.setFW( curObj->ang + curObj->angRelative );
		// 		singleton->setShaderfVec4("rotZ",&rotVec);
		// 		singleton->setShaderfVec4("rotZ2",&tempVec2);
				
		// 		p0.copyFrom(&tempVec2);
		// 		p1.copyFrom(&tempVec2);
		// 		p2.copyFrom(&tempVec2);
		// 		p3.copyFrom(&tempVec2);
				
		// 		p0.addXYZ(-0.5f, -0.5f,  0.0f);
		// 		p1.addXYZ( 0.5f, -0.5f,  0.0f);
		// 		p2.addXYZ( 0.5f,  0.5f,  0.0f);
		// 		p3.addXYZ(-0.5f,  0.5f,  0.0f);
				
		// 		/////
				
				
				
		// 		tempCS = &(singleton->fontWrappers[EFW_ICONS]->charVals[
		// 			singleton->gem->entIdToIcon[eqObj->objectType] 
		// 		]);
		// 		frameMod = 0;
		// 		if (eqObj->maxFrames != 0) {
		// 			frameMod = ((singleton->currentTick/(20))%(eqObj->maxFrames))*16.0f;
		// 		}
				
				
		// 		x1 = (tempCS->sampX + frameMod )/iw;
		// 		x2 = (tempCS->sampX + tempCS->sampW + frameMod)/iw;
		// 		y1 = 1.0f-(tempCS->sampY)/ih;
		// 		y2 = 1.0f-(tempCS->sampY+tempCS->sampH)/ih;
				
		// 		if (
		// 			(eqObj->entType == E_ENTTYPE_BULLET) ||
		// 			(eqObj->entType == E_ENTTYPE_TRACE)	
		// 		) {
		// 			yval = 1.0f;
		// 		}
		// 		else {
		// 			yval = -1.0f;
		// 		}
				
		// 		// glMultiTexCoord4f(GL_TEXTURE0, visObjects[i], yval, 1.0f, 1.0f);
		// 		// glMultiTexCoord4f(GL_TEXTURE1, x1,y1,x2,y2);
				
				
				
		// 		singleton->drawQuadWithCoords(
		// 				&p0,
		// 				&p1,
		// 				&p2,
		// 				&p3,
						
		// 				x1, y1, x2, y2
		// 		);
				
		// 	}
			
			
			
		// }
		// singleton->setShaderTexture(0,0);
		// singleton->unbindFBO();
		// singleton->unbindShader();
		
		
		
		
		
		
		
		
		
		
		
		
		// singleton->copyFBO2("geomBaseTargFBO","geomTargFBO");
		
		
		// singleton->bindShader("BillboardShader");
		// singleton->setShaderfVec2("bufferDim", &(singleton->bufferModDim) );
		// singleton->setShaderFloat("cellsPerHolder",cellsPerHolder);
		// singleton->setShaderFloat("heightOfNearPlane",singleton->heightOfNearPlane);
		// singleton->setShaderFloat("clipDist",singleton->clipDist[1]);
		// singleton->setShaderfVec3("cameraPos", singleton->cameraGetPos());
		// singleton->setShaderTexture(0,singleton->fontWrappers[EFW_ICONS]->fontImage->tid);
		// singleton->sampleFBO("geomBaseTargFBO",1);
		// singleton->bindFBO("geomTargFBO", -1, 0);
		
		
		// glBegin(GL_POINTS);
		
		// for(i = 0; i < singleton->gem->visObjects.size(); i++) {
			
		// 	curObj = &(singleton->gem->gameObjects[visObjects[i]]);
			
		// 	if (
		// 		curObj->isHidden ||
		// 		(curObj->objectType <= 0) ||
		// 		(
		// 			(singleton->gem->firstPerson) &&
		// 			(curObj->uid == singleton->gem->getCurActorUID())
		// 		)
		// 	) {
				
		// 	}
		// 	else {
		// 		tempCS = &(singleton->fontWrappers[EFW_ICONS]->charVals[
		// 			singleton->gem->entIdToIcon[curObj->objectType] 
		// 		]);
		// 		frameMod = 0;
		// 		if (curObj->maxFrames != 0) {
		// 			frameMod = ((singleton->currentTick/(20))%(curObj->maxFrames))*16.0f;
		// 		}
				
				
		// 		x1 = (tempCS->sampX + frameMod )/iw;
		// 		x2 = (tempCS->sampX + tempCS->sampW + frameMod)/iw;
		// 		y1 = 1.0f-(tempCS->sampY)/ih;
		// 		y2 = 1.0f-(tempCS->sampY+tempCS->sampH)/ih;
				
		// 		if (
		// 			(curObj->entType == E_ENTTYPE_BULLET) ||
		// 			(curObj->entType == E_ENTTYPE_TRACE)	
		// 		) {
		// 			yval = 1.0f;
		// 		}
		// 		else {
		// 			yval = -1.0f;
		// 		}
				
		// 		glMultiTexCoord4f(GL_TEXTURE0, visObjects[i], yval, 1.0f, 1.0f);
		// 		glMultiTexCoord4f(GL_TEXTURE1, x1,y1,x2,y2);
		// 		glVertex3f(
		// 			curObj->getCenterPointFIV(0)->getFX(),
		// 			curObj->getCenterPointFIV(0)->getFY(),
		// 			curObj->getCenterPointFIV(0)->getFZ()
		// 		);
		// 	}
			
			
			
		// }
		// glEnd();
		
		
		
		// singleton->unsampleFBO("geomBaseTargFBO",1);
		// singleton->setShaderTexture(0,0);
		// singleton->unbindFBO();
		// singleton->unbindShader();
		
		//////////////////////////////
		
		

	}


	void updateMouseCoords(FIVector4 *fPixelWorldCoordsBase) {

		FIVector4 fPixelWorldCoords;
		fPixelWorldCoords.copyFrom(fPixelWorldCoordsBase);


		lastUnitPos.copyFrom(&fPixelWorldCoords);
		lastUnitPos.setFW(1.0);
		lastUnitPos.floorXYZ();

	}



	

	float weighPath(float x1, float y1, float x2, float y2, float rad, bool doSet, bool isOcean)
	{

		int i;
		int iMax = (int)min(64.0f, rad * 4.0f);

		float fi;
		float fMax = (float)(iMax - 1);
		float lerp;

		float curX;
		float curY;


		float startVal = curFBO->getPixelAtWrapped((int)x1, (int)y1, hmChannel);
		float lastRes = startVal;
		float curRes;
		float curRes2;
		float tempRes;
		float tot = 0.0f;

		bool startsInWater = startVal <= seaLevel;

		for (i = 1; i < iMax; i++)
		{
			fi = (float)i;
			lerp = fi / fMax;
			curX = (1.0f - lerp) * x1 + (lerp) * x2;
			curY = (1.0f - lerp) * y1 + (lerp) * y2;
			curRes = curFBO->getPixelAtWrapped((int)curX, (int)curY, hmChannel);

			if (isOcean)
			{
				if (doSet)
				{

					if (curRes > seaLevel)
					{

						tempRes = abs(curRes - lastRes);
						tempRes = tempRes * tempRes * tempRes;
						//tempRes = 255-curRes;
					}
					else
					{
						tempRes = curRes;
					}


					//tempRes = abs(curRes-lastRes);
					//tempRes = tempRes*tempRes*tempRes;
					if ((curRes > seaLevel) != (lastRes > seaLevel))
					{
						tempRes += 100000.0f;
					}

					// if (startsInWater) {
					//  if (curRes > seaLevel) {
					//      tempRes += 1000000.0f;
					//  }
					// }

				}
				else
				{
					tempRes = 0.0;
					if (curRes > seaLevel)
					{
						tempRes = 1.0f;
					}
					else
					{
						tempRes = -1.0f;
					}
				}
			}
			else
			{
				tempRes = abs(curRes - lastRes) * 10.0 + curRes;
				//tempRes = tempRes;
				if (curRes <= seaLevel - 20.0f)
				{
					tempRes += 100000.0f;
				}
				if ((curRes > seaLevel) != (lastRes > seaLevel))
				{
					tempRes += 100000.0f;
				}
			}


			tot += tempRes;
			lastRes = curRes;

		}

		return tot;
	}

	float findBestPath(float x1, float y1, float x2, float y2, int generation, int roadIndex, bool doSet, bool isOcean)
	{
		int i;
		int j;

		coordAndIndex baseCoord;
		coordAndIndex bestCoord;
		coordAndIndex testCoord;

		float mpx = (x1 + x2) / 2.0;
		float mpy = (y1 + y2) / 2.0;
		float dis = quickDis(x1, y1, x2, y2);
		float rad = dis / 2.0;
		float mpxTemp;
		float mpyTemp;
		float delta;
		float bestDis;
		float curDis;
		float bestDelta = FLT_MAX;
		float bestX;
		float bestY;
		float genMod;

		int q;
		int p;

		int ibx;
		int iby;
		int ix2;
		int iy2;
		int tot1 = 0;
		int tot2 = 0;
		int iRad;
		int numTries = max((int)(rad), 20);

		if ( (rad < 2.0f) || (generation > 1024) )
		{
			// do manhattan distance

			if (doSet)
			{
				ibx = x1;
				iby = y1;
				ix2 = x2;
				iy2 = y2;


				while (ibx != ix2)
				{
					curFBO2->setPixelAtWrapped(ibx, iby, pathChannel, 255);
					if (ibx < ix2)
					{
						ibx++;
					}
					else
					{
						ibx--;
					}
				}
				while (iby != iy2)
				{
					curFBO2->setPixelAtWrapped(ibx, iby, pathChannel, 255);
					if (iby < iy2)
					{
						iby++;
					}
					else
					{
						iby--;
					}
				}

				curFBO2->setPixelAtWrapped(ibx, iby, pathChannel, 255);
			}
			else
			{

			}

			return 0.0f;

		}



		genMod = 2.0f;

		// if (generation > 32) {
		//  return 0.0f;
		// }




		for (i = 0; i < numTries; i++)
		{
			mpxTemp = mpx + (fGenRand() * dis - rad) / genMod;
			mpyTemp = mpy + (fGenRand() * dis - rad) / genMod;

			delta = weighPath(x1, y1, mpxTemp, mpyTemp, rad / 2.0f, doSet, isOcean);
			delta += weighPath(mpxTemp, mpyTemp, x2, y2, rad / 2.0f, doSet, isOcean);

			if (delta < bestDelta)
			{
				bestDelta = delta;
				bestX = mpxTemp;
				bestY = mpyTemp;
			}

		}



		if (doSet)
		{



			if (generation < 8)
			{

				baseCoord.x = bestX;
				baseCoord.y = bestY;
				baseCoord.index = roadIndex;
				//roadCoords.push_back(baseCoord);

				bestDis = FLT_MAX;
				for (i = 0; i < roadCoords.size(); i++)
				{
					testCoord = roadCoords[i];


					if (baseCoord.index == testCoord.index)
					{

					}
					else
					{
						curDis = coordDis(&baseCoord, &testCoord);

						if (curDis < bestDis)
						{
							bestDis = curDis;
							bestCoord = roadCoords[i];
						}
					}


				}

				if (bestDis != FLT_MAX)
				{
					curDis = coordDis(&bestCoord, &baseCoord);

					if (curDis < min(400.0f, rad) )   //
					{
						baseCoord = bestCoord;
						baseCoord.index = roadIndex;
					}

					bestX = baseCoord.x;
					bestY = baseCoord.y;
				}

				roadCoords.push_back(baseCoord);
			}


			//curFBO2->setPixelAtWrapped((int)bestX, (int)bestY, pathChannel, 255);
			//curFBO2->setPixelAtWrapped((int)bestX, (int)bestY, breadCrumbChannel, 255);
			findBestPath(x1, y1, bestX, bestY, generation + 1, roadIndex, doSet, isOcean);
			findBestPath(bestX, bestY, x2, y2, generation + 1, roadIndex, doSet, isOcean);
		}

		return bestDelta;

	}

	void initMap()
	{
		mapLockOn = true;

		mapSwapFlag = 0;

		mapStep = 0.0f;



		FBOWrapper *fbow = singleton->getFBOWrapper("hmFBO", 0);
		FBOWrapper *fbow2 = singleton->getFBOWrapper("cityFBO", 0);

		curFBO = fbow;
		curFBO2 = fbow2;

		

		FIVector4 startVec;
		FIVector4 endVec;
		FIVector4 midPointVec;

		int w = fbow2->width;
		int h = fbow2->height;


		//0:r
		//1:g
		//2:b
		//3:a




		int i;
		int j;
		int k;
		int m;
		int q;
		
		int totSize = w * h;
		int *btStack = new int[totSize];
		int btStackInd = 0;
		int curInd;
		int curX;
		int curY;
		int destX;
		int destY;
		int lastDir;
		int curXRight;
		int curXLeft;
		int curYUp;
		int curYDown;
		int testX;
		int testY;
		int testInd;
		int basePix;
		int basePix2;
		int testPix;
		int testPix1;
		int testPix2;
		int testPix3;
		int testPix4;
		int count;
		int p1;
		int p2;



		// int alleyFlag = 32;
		// int streetFlag = 64;
		// int highwayFlag = 128;

		int startDir;
		int curDir;
		blockMip = intLogB2(singleton->holdersPerBlock);

		int cx1;
		int cy1;
		int cx2;
		int cy2;

		int histogram[256];

		float delta;
		float bestDelta;
		float nextBestDelta;

		int nextBestInd;
		int bestDir;
		int nextBestDir;

		int bestInd;
		int tempVal;

		int xind;
		int yind;
		int curHeight;

		bool isValid;
		bool notFound;
		bool touchesWater;
		bool doBreak;

		float mult;
		float tempDis;

		int minSL = 0;
		int maxSL = 0;
		int curSL = 0;
		int avgSL = 0;
		//int totFilled = 0;



		for (i = 0; i < 16; i++)
		{
			singleton->paramArrMap[i * 3 + 0] = fGenRand();
			singleton->paramArrMap[i * 3 + 1] = fGenRand();
			singleton->paramArrMap[i * 3 + 2] = fGenRand();
		}

		for (i = 0; i < 6; i++)
		{
			singleton->paramArrMap[i * 3 + 0] = i;
		}

		for (i = 0; i < 30; i++)
		{
			p1 = iGenRand(0, 5);
			p2 = iGenRand(0, 5);

			if (p1 != p2)
			{
				tempVal = singleton->paramArrMap[p1];
				singleton->paramArrMap[p1] = singleton->paramArrMap[p2];
				singleton->paramArrMap[p2] = tempVal;
			}
		}



		for (q = 0; q < 2; q++) {
			
			
			singleton->bindShader("Simplex2D");
			singleton->bindFBO("simplexFBO");
			singleton->setShaderFloat("curTime", fGenRand() * 100.0f);
			singleton->drawFSQuad();
			singleton->unbindFBO();
			singleton->unbindShader();

			singleton->bindShader("TerrainMix");
			singleton->bindFBO("hmFBOLinear");
			singleton->sampleFBO("simplexFBO", 0);
			singleton->setShaderTexture(1, singleton->imageHM0->tid);
			singleton->setShaderTexture(2, singleton->imageHM1->tid);
			singleton->setShaderInt("passNum",q);
			singleton->setShaderVec2("minAndMax",((float)minSL)/255.0f,((float)maxSL)/255.0f);
			singleton->setShaderArrayfVec3("paramArrMap", singleton->paramArrMap, 16 );
			//singleton->setShaderFloat("mapSampScale", 1.0f); //singleton->mapSampScale
			singleton->drawFSQuad();
			singleton->setShaderTexture(2, 0);
			singleton->setShaderTexture(1, 0);
			singleton->unsampleFBO("simplexFBO", 0);
			singleton->unbindFBO();
			singleton->unbindShader();

			singleton->copyFBO("hmFBOLinear", "hmFBO");
			
			glFlush();
			glFinish();

			fbow->getPixels(true);
			fbow->setAllPixels(densityChannel, 255);
			fbow->setAllPixels(idChannel, 0);
			fbow->setAllPixels(blockChannel, 0);


			// determine sea level

			for (i = 0; i < 256; i++)
			{
				histogram[i] = 0;
			}
			
			minSL = 255;
			maxSL = 0;
			curSL = 0;
			avgSL = 0;
			//totFilled = 0;
			
			for (i = 0; i < totSize; i++)
			{
				curSL = fbow->getPixelAtIndex(i, hmChannel);
				
				if (curSL < minSL) {
					minSL = curSL;
				}
				if (curSL > maxSL) {
					maxSL = curSL;
				}
				
				histogram[curSL]++;
			}

			avgSL = (minSL+maxSL)/2;
		}

		seaLevel = 100;

		
		singleton->copyFBO("hmFBOLinear", "hmFBOLinearBig");
		
		
		seaSlack = seaLevel - 1;
		cout << "Sea Level: " << seaLevel << "\n";

		mapWidth = fbow->width;
		mapHeight = fbow->height;

		goto DONE_WITH_MAP;

		
		cout << "start place cities\n";

		for (i = 1; i < numProvinces; i++)
		{

			isValid = false;

			do
			{

				xind = (int)(fGenRand() * fbow->width);
				yind = (int)(fGenRand() * fbow->height);

				tempVec1.setFXYZ(xind, yind, 0.0f);

				notFound = false;
				for (j = 1; j < i; j++)
				{
					tempVec2.setFXYZ(provinceX[j], provinceY[j], 0.0f);
					tempDis = tempVec2.wrapDistance(&tempVec1, w, false);

					if (tempDis < 10.0f)
					{
						notFound = true;
					}

				}

				if (notFound)
				{

				}
				else
				{
					if (fbow->getPixelAtC(xind, yind, idChannel) == 0)
					{
						curHeight = fbow->getPixelAtC(xind, yind, hmChannel);

						if (
							(curHeight > seaLevel)
						)
						{

							provinceX[i] = xind;
							provinceY[i] = yind;
							fbow->setPixelAtC(xind, yind, idChannel, i);
							fbow->setPixelAtC(xind, yind, densityChannel, 0);
							isValid = true;

						}
					}
				}



			}
			while (!isValid);

		}

		cout << "end place cities\n";


		cout << "start grow provinces\n";

		fbow->cpuToGPU();

		singleton->copyFBO("hmFBO", "swapFBO0");
		singleton->bindShader("MapBorderShader");
		mapStep = 0.0f;
		for (i = 0; i < 1024; i++)
		{

			singleton->bindFBO("swapFBO", mapSwapFlag);
			singleton->sampleFBO("swapFBO", 0, mapSwapFlag);
			singleton->setShaderFloat("seaSlack", ((float)seaSlack) / 255.0 );
			singleton->setShaderFloat("mapStep", mapStep);
			singleton->setShaderFloat("texPitch", w);
			singleton->drawFSQuad();
			singleton->unsampleFBO("swapFBO", 0, mapSwapFlag);
			singleton->unbindFBO();


			mapSwapFlag = 1 - mapSwapFlag;
			mapStep += 1.0f;
		}
		singleton->unbindShader();


		singleton->bindShader("MapBorderShader");
		mapStep = 1.0f;
		for (i = 0; i < 256; i++)
		{

			singleton->bindFBO("swapFBO", mapSwapFlag);
			singleton->sampleFBO("swapFBO", 0, mapSwapFlag);
			singleton->setShaderFloat("seaSlack", ((float)seaSlack) / 255.0 );
			singleton->setShaderFloat("mapStep", -mapStep);
			singleton->setShaderFloat("texPitch", w);
			singleton->drawFSQuad();
			singleton->unsampleFBO("swapFBO", 0, mapSwapFlag);
			singleton->unbindFBO();


			mapSwapFlag = 1 - mapSwapFlag;
			mapStep += 1.0f;
		}
		singleton->unbindShader();


		singleton->copyFBO("swapFBO0", "hmFBO");


		fbow->getPixels();
		fbow->updateMips();

		cout << "end grow provinces\n";


		cout << "start find neighboring cities\n";

		for (i = 0; i < numProvinces * numProvinces; i++)
		{
			provinceGrid[i] = 0;
		}

		for (k = 0; k < totSize; k++)
		{
			curInd = k;
			curY = curInd / w;
			curX = curInd - curY * w;

			basePix = fbow->getPixelAtIndex(curInd, idChannel);

			testPix = fbow->getPixelAtIndex(fbow->getIndex(curX + 1, curY), idChannel);
			testPix2 = fbow->getPixelAtIndex(fbow->getIndex(curX, curY + 1), idChannel);


			if (basePix != 0)
			{
				if (testPix != 0)
				{
					if (basePix != testPix)
					{

						provinceGrid[basePix * numProvinces + testPix] = 1;
						provinceGrid[basePix + testPix * numProvinces] = 1;

					}
				}
				if (testPix2 != 0)
				{
					if (basePix != testPix2)
					{
						provinceGrid[basePix * numProvinces + testPix2] = 1;
						provinceGrid[basePix + testPix2 * numProvinces] = 1;
					}
				}
			}


		}

		cout << "end find neighboring cities\n";




		// 1 - x+
		// 2 - x-
		// 4 - y+
		// 8 - y-


		cout << "start find city blocks\n";


		fbow2->getPixels(true);
		fbow2->setAllPixels(btChannel, 15);
		fbow2->setAllPixels(stChannel, 0);
		fbow2->setAllPixels(pathChannel, 0);
		fbow2->setAllPixels(houseChannel, 0);


		int blockMod = singleton->holdersPerBlock;
		for (k = 0; k < totSize; k++)
		{
			curInd = k;
			curY = curInd / w;
			curX = curInd - curY * w;

			basePix = fbow->getMipVal(curX, curY, blockMip, idChannel, MAX_MIP);
			testPix = fbow->getMipVal(curX, curY, blockMip, densityChannel, AVG_MIP);

			testPix1 = fbow->getMipVal(curX, curY, blockMip, idChannel, MAX_MIP, -1, -1, 0);
			testPix2 = fbow->getMipVal(curX, curY, blockMip, idChannel, MAX_MIP, -1, 1, 0);
			testPix3 = fbow->getMipVal(curX, curY, blockMip, idChannel, MAX_MIP, -1, 0, -1);
			testPix4 = fbow->getMipVal(curX, curY, blockMip, idChannel, MAX_MIP, -1, 0, 1);


			// TODO: EDIT CITY

			//testPix > xxx <- (xxx = 0: no city, xxx = 255: all city, def: 220)
			if ( testPix1 != testPix2 || testPix3 != testPix4 || testPix > 0 )
			{
				fbow->setPixelAtIndex(curInd, blockChannel, 0);
			}
			else
			{
				fbow->setPixelAtIndex(curInd, blockChannel, basePix);
			}

		}

		fbow->cpuToGPU();
		singleton->copyFBO("hmFBO", "hmFBOLinear");

		cout << "end find city blocks\n";


		cout << "start add in city roads\n";
		//add in city roads

		for (i = 0; i < numProvinces; i++)
		{
			// recursive backtrack
			btStack[0] = fbow2->getIndex(provinceX[i], provinceY[i]);
			btStackInd = 0;

			while (btStackInd > -1)
			{

				curInd = btStack[btStackInd];
				curY = curInd / w;
				curX = curInd - curY * w;

				fbow2->orPixelAtIndex(curInd, btChannel, visFlag);

				startDir = 0;
				count = 0;
				notFound = true;
				bestDelta = FLT_MAX;


				testPix2 = fbow->getPixelAtIndex(curInd, blockChannel);

				//testPix2 = fbow->getMipVal(curX,curY,blockMip,densityChannel,AVG_MIP);
				//testPix3 = fbow->getMipVal(curX,curY,blockMip,idChannel,MIN_MIP);
				//testPix4 = fbow->getMipVal(curX,curY,blockMip,idChannel,MAX_MIP);


				if ( testPix2 != 0 )
				{
					do
					{
						curDir = (startDir + count) % 4;

						testX = curX + dirModX[curDir];
						testY = curY + dirModY[curDir];
						testInd = fbow2->getIndex(testX, testY);
						testPix = fbow2->getPixelAtIndex(testInd, btChannel);
						testPix3 = fbow->getPixelAtIndex(testInd, blockChannel);

						if ( (testPix & visFlag) == 0 && (testPix3 != 0))
						{
							//not visited, proceed
							notFound = false;


							delta = abs(
												fbow->getPixelAtIndex(curInd, hmChannel) -
												fbow->getPixelAtIndex(testInd, hmChannel)
											);

							if (delta < bestDelta)
							{
								bestDelta = delta;
								bestDir = curDir;
								bestInd = testInd;
							}

						}

						count++;
					}
					while (count < 4); //notFound &&
				}

				if (notFound)
				{
					btStackInd--;
				}
				else
				{

					// join the two and remove walls
					fbow2->andPixelAtIndex(curInd, btChannel, dirFlags[bestDir]);
					fbow2->andPixelAtIndex(bestInd, btChannel, dirFlagsOp[bestDir]);

					btStackInd++;
					btStack[btStackInd] = bestInd;
				}

			}
		}

		cout << "end add in city roads\n";



		// clear visited
		for (k = 0; k < totSize; k++)
		{
			testPix = fbow2->getPixelAtIndex(k, btChannel);
			if ( (testPix & visFlag) == 0)
			{
				//not visited
				for (i = 0; i < 4; i++)
				{
					fbow2->andPixelAtIndex(k, btChannel, dirFlags[i]);
				}
			}
			else
			{
				//visited
			}

			fbow2->andPixelAtIndex(k, btChannel, visFlagO );
		}



		cout << "start link close cities\n";

		// link close cities

		for (i = 1; i < numProvinces - 1; i++)
		{
			for (j = i + 1; j < numProvinces; j++)
			{

				curInd = i + j * numProvinces;

				if (provinceGrid[curInd] == 1)
				{
					p1 = i;
					p2 = j;



					tempVec1.setIXYZ(provinceX[p1], provinceY[p1], 0);
					tempVec2.setIXYZ(provinceX[p2], provinceY[p2], 0);

					tempVec2.wrapDistance(&tempVec1, w);
					tempVec3.copyFrom(&tempVec1);

					findBestPath(
						tempVec2.getFX(),
						tempVec2.getFY(),
						tempVec3.getFX(),
						tempVec3.getFY(),
						0,
						curInd,
						true,
						false
					);

				}

			}
		}



		cout << "end link close cities\n";



		floatAndIndex *oceanRes = new floatAndIndex[numProvinces * numProvinces];

		for (i = 0; i < numProvinces * numProvinces; i++)
		{
			oceanRes[i].value = FLT_MAX;
			oceanRes[i].index1 = 0;
			oceanRes[i].index2 = 0;
		}

		cout << "start find biggest ocean gaps\n";


		for (k = 0; k < 2; k++)
		{

			cout << "iteration: " << k << "\n";

			count = 0;

			if (k == 0)
			{
				for (i = 1; i < numProvinces - 1; i++)
				{
					for (j = i + 1; j < numProvinces; j++)
					{
						curInd = i + j * numProvinces;
						if (provinceGrid[curInd] != 1)
						{
							p1 = i;
							p2 = j;



							tempVec1.setIXYZ(provinceX[p1], provinceY[p1], 0);
							tempVec2.setIXYZ(provinceX[p2], provinceY[p2], 0);

							tempVec2.wrapDistance(&tempVec1, w);
							tempVec3.copyFrom(&tempVec1);


							oceanRes[count].value = findBestPath(
																				tempVec2.getFX(),
																				tempVec2.getFY(),
																				tempVec3.getFX(),
																				tempVec3.getFY(),
																				0,
																				curInd,
																				false,
																				true
																			);
							oceanRes[count].index1 = i;
							oceanRes[count].index2 = j;

							count++;

						}

					}
				}
			}
			else
			{
				bubbleSortF(oceanRes, numProvinces * numProvinces);


				for (i = 0; i < 30; i++)
				{

					p1 = oceanRes[i].index1;
					p2 = oceanRes[i].index2;

					curInd = p1 + p2 * numProvinces;

					tempVec1.setIXYZ(provinceX[p1], provinceY[p1], 0);
					tempVec2.setIXYZ(provinceX[p2], provinceY[p2], 0);

					tempVec2.wrapDistance(&tempVec1, w);
					tempVec3.copyFrom(&tempVec1);

					findBestPath(
						tempVec2.getFX(),
						tempVec2.getFY(),
						tempVec3.getFX(),
						tempVec3.getFY(),
						0,
						curInd,
						true,
						true
					);
				}

			}


		}



		cout << "end find biggest ocean gaps\n";








		mapSwapFlag = 0;
		mapStep = 0.0f;

		fbow2->cpuToGPU();
		singleton->copyFBO("cityFBO", "swapFBO0");
		singleton->bindShader("DilateShader");
		for (i = 0; i < 2; i++)
		{

			singleton->bindFBO("swapFBO", mapSwapFlag);
			singleton->sampleFBO("swapFBO", 0, mapSwapFlag);
			singleton->sampleFBO("hmFBO", 1);
			singleton->setShaderFloat("seaLevel", singleton->getSLNormalized() );
			singleton->setShaderFloat("mapStep", 1.0);
			singleton->setShaderFloat("doDilate", 1.0);
			singleton->setShaderFloat("texPitch", w);
			singleton->drawFSQuad();
			singleton->unsampleFBO("hmFBO", 1);
			singleton->unsampleFBO("swapFBO", 0, mapSwapFlag);
			singleton->unbindFBO();

			mapSwapFlag = 1 - mapSwapFlag;
			mapStep += 1.0f;
		}
		singleton->unbindShader();
		singleton->copyFBO("swapFBO0", "cityFBO");
		fbow2->getPixels();
		//fbow2->updateMips();






		//bool notCovered = true;
		int id = 1;
		int totCount;
		int fillColor;
		bool incId;

		cout << "start road regions\n";

		for (i = 0; i < totSize; i++)
		{

			if (fbow2->getPixelAtIndex(i, pathChannel) == 0)
			{

				for (j = 0; j < 2; j++)
				{
					btStack[0] = i;
					btStackInd = 0;
					totCount = 0;

					if (j == 0)
					{
						fillColor = id;
					}
					else
					{
						fillColor = 255;
					}


					while (btStackInd > -1)
					{

						curInd = btStack[btStackInd];
						curY = curInd / w;
						curX = curInd - curY * w;

						if (j == 0)
						{
							fbow2->orPixelAtIndex(curInd, btChannel, visFlag);
						}
						else
						{
							fbow2->andPixelAtIndex(curInd, btChannel, visFlagO );
						}


						fbow2->setPixelAtIndex(curInd, pathChannel, fillColor );


						count = 0;
						notFound = true;



						do
						{
							curDir = count;

							testX = curX + dirModX[curDir];
							testY = curY + dirModY[curDir];
							testInd = fbow2->getIndex(testX, testY);
							testPix = fbow2->getPixelAtIndex(testInd, btChannel);
							testPix2 = fbow2->getPixelAtIndex(testInd, pathChannel);


							if (j == 0)
							{
								if ( ( (testPix & visFlag) == 0 ) && (testPix2 == 0))
								{
									notFound = false;
									totCount++;

								}
							}
							else
							{
								if ( ( (testPix & visFlag) > 0) && (testPix2 == id))
								{
									notFound = false;
									totCount++;
								}
							}




							count++;
						}
						while (notFound && count < 4);

						if (notFound)
						{
							btStackInd--;
						}
						else
						{

							// join the two and remove walls
							//fbow2->andPixelAtIndex(curInd, btChannel, dirFlags[bestDir]);
							//fbow2->andPixelAtIndex(bestInd, btChannel, dirFlagsOp[bestDir]);


							btStackInd++;
							btStack[btStackInd] = testInd;
						}

					}

					incId = false;

					if (j == 0)
					{
						if (totCount < 1000)
						{
							//cout << "Too Small\n";
						}
						else
						{
							incId = true;
							j++;
						}
					}
					else
					{
						incId = true;
					}

					if (incId)
					{
						//cout << "Id: " << id << "\n";
						id++;
						if (id > 254)
						{
							id = 1;
						}
					}
				}




			}

		}


		cout << "end road regions\n";

		// clear visited
		for (k = 0; k < totSize; k++)
		{
			fbow2->andPixelAtIndex(k, btChannel, visFlagO );
		}

		fbow2->cpuToGPU();


		mapSwapFlag = 0;
		mapStep = 0.0f;
		singleton->copyFBO("cityFBO", "swapFBO0");



		singleton->bindShader("SkeletonShader");
		for (k = 0; k < 19; k++)
		{

			singleton->bindFBO("swapFBO", mapSwapFlag);
			singleton->sampleFBO("swapFBO", 0, mapSwapFlag);
			//singleton->sampleFBO("hmFBO",1);
			singleton->setShaderFloat("seaLevel", singleton->getSLNormalized() );
			singleton->setShaderFloat("mapStep", 0.0);
			singleton->setShaderFloat("texPitch", w);
			singleton->drawFSQuad();
			//singleton->unsampleFBO("hmFBO",1);
			singleton->unsampleFBO("swapFBO", 0, mapSwapFlag);
			singleton->unbindFBO();
			mapSwapFlag = 1 - mapSwapFlag;
			mapStep += 1.0f;

		}
		singleton->unbindShader();


		singleton->bindShader("RoadShader");
		for (k = 0; k < 1; k++)
		{

			singleton->bindFBO("swapFBO", mapSwapFlag);
			singleton->sampleFBO("swapFBO", 0, mapSwapFlag);
			//singleton->sampleFBO("hmFBO",1);
			singleton->setShaderFloat("seaLevel", singleton->getSLNormalized() );
			singleton->setShaderFloat("mapStep", 0.0);
			singleton->setShaderFloat("texPitch", w);
			singleton->drawFSQuad();
			//singleton->unsampleFBO("hmFBO",1);
			singleton->unsampleFBO("swapFBO", 0, mapSwapFlag);
			singleton->unbindFBO();
			mapSwapFlag = 1 - mapSwapFlag;
			mapStep += 1.0f;

		}
		singleton->unbindShader();


		singleton->copyFBO("swapFBO0", "cityFBO");
		fbow2->getPixels();







		// generate streets

		int *streetFlagsV = new int[w]; //runs vertical
		int *streetFlagsH = new int[h]; //runs horizontal

		for (i = 0; i < w; i++)
		{
			streetFlagsV[i] = 0;
		}
		for (i = 0; i < h; i++)
		{
			streetFlagsH[i] = 0;
		}

		// 1 - x+
		// 2 - x-
		// 4 - y+
		// 8 - y-

		for (i = 0; i < w; i += singleton->holdersPerBlock )
		{

			curInd = i - 1;

			while (curInd < 0)
			{
				curInd += w;
			}

			streetFlagsV[curInd] |= 1;
			streetFlagsV[i] |= 2;
		}
		for (i = 0; i < h; i += singleton->holdersPerBlock )
		{

			curInd = i - 1;

			while (curInd < 0)
			{
				curInd += h;
			}

			streetFlagsH[curInd] |= 4;
			streetFlagsH[i] |= 8;
		}












		//add in main streets
		for (k = 0; k < totSize; k++)
		{
			curInd = k;
			curY = curInd / w;
			curX = curInd - curY * w;

			testPix = fbow->getPixelAtIndex(curInd, blockChannel);

			if (testPix != 0)
			{
				fbow2->orPixelAtIndex(curInd, btChannel, streetFlagsH[curY] | streetFlagsV[curX]);
				fbow2->orPixelAtIndex(curInd, stChannel, streetFlagsH[curY] | streetFlagsV[curX]); //stChannel
			}


		}


		// for (k = 0; k < totSize; k++) {

		//  // int stChannel;
		//  // int btChannel;
		//  // int pathChannel;
		//  // int houseChannel;

		//  testPix = fbow2->getPixelAtIndex(k, stChannel);
		//  fbow2->orPixelAtIndex(k, btChannel, testPix );

		//  testPix = fbow2->getPixelAtIndex(k, pathChannel);
		//  fbow2->orPixelAtIndex(k, btChannel, testPix );
		// }


		/*
		for (i = 0; i < numProvinces-1; i++) {
		    for (j = i + 1; j < numProvinces; j++) {
		        if (provinceGrid[i + j*numProvinces] == 1) {

		            k = fbow->numMips-1;

		            cx1 = provinceX[i];
		            cy1 = provinceY[i];
		            cx2 = provinceX[j];
		            cy2 = provinceY[j];

		            while (getMipInd(cx1,cy1,k) == getMipInd(cx2,cy2,k)) {
		                k--;
		            }

		        }
		    }
		}



		// remove any road that touches water or is out of town

		int cityLevel = 0;
		for (k = 0; k < totSize; k++) {
		    curInd = k;
		    curY = curInd/w;
		    curX = curInd-curY*w;

		    touchesWater = false;

		    for (j = -1; j <= 1; j++) {
		        for (i = -1; i <= 1; i++) {
		            testX = curX + i;
		            testY = curY + j;
		            testInd = fbow->getIndex(testX,testY);
		            testPix = fbow->getPixelAtIndex(testInd, hmChannel);


		            if (testPix < seaLevel + 10) {
		                touchesWater = true;
		                break;
		            }

		        }
		        if (touchesWater) {
		            break;
		        }
		    }

		    testPix2 = fbow->getPixelAtIndex(curInd, densityChannel);


		    if (touchesWater) { // || (testPix2 > 120)
		        fbow2->andPixelAtIndex(curInd, btChannel, dirFlagClear);
		        fbow2->andPixelAtIndex(curInd, stChannel, dirFlagClear);

		        //TODO: repair all broken (half) edges
		    }
		    // if (testPix2 > 80) {
		    //  fbow2->andPixelAtIndex(curInd, stChannel, dirFlagClear);
		    // }


		}



		// fix half streets

		for (k = 0; k < totSize; k++) {
		    curInd = k;
		    curY = curInd/w;
		    curX = curInd-curY*w;

		    basePix = fbow2->getPixelAtIndex(curInd, btChannel);
		    basePix2 = fbow2->getPixelAtIndex(curInd, stChannel);

		    for (i = 0; i < 4; i++) {



		        curDir = i;

		        testX = curX + dirModX[curDir];
		        testY = curY + dirModY[curDir];
		        testInd = fbow2->getIndex(testX,testY);
		        testPix = fbow2->getPixelAtIndex(testInd, btChannel);
		        testPix2 = fbow2->getPixelAtIndex(testInd, stChannel);


		        if ( (basePix & dirFlagsO[curDir]) != (testPix & dirFlagsOpO[curDir]) ) {
		            fbow2->orPixelAtIndex(curInd, btChannel, dirFlagsO[curDir]);
		            fbow2->orPixelAtIndex(testInd, btChannel, dirFlagsOpO[curDir]);
		        }

		        if ( (basePix2 & dirFlagsO[curDir]) != (testPix2 & dirFlagsOpO[curDir]) ) {
		            fbow2->orPixelAtIndex(curInd, stChannel, dirFlagsO[curDir]);
		            fbow2->orPixelAtIndex(testInd, stChannel, dirFlagsOpO[curDir]);
		        }

		    }

		}

		*/


















		// ?????




		cout << "start filling houses\n";

		// clear visited
		for (k = 0; k < totSize; k++)
		{
			fbow2->andPixelAtIndex(k, btChannel, visFlagO );
		}

		fbow->updateMips();




		int wb = blocksPerWorld;
		int hb = blocksPerWorld;
		int blockInd;

		for (i = 0; i < wb; i++)
		{
			for (j = 0; j < hb; j++)
			{
				blockInd = i + j * wb;

				testPix = fbow->getMipAtIndex(blockInd, blockMip, blockChannel, MIN_MIP);

				if (testPix != 0)
				{



					id = 1;

					for (k = i * blockMod; k < (i + 1)*blockMod; k++)
					{
						for (m = j * blockMod; m < (j + 1)*blockMod; m++)
						{
							curInd = k + m * w;
							curX = k;
							curY = m;


							testPix = fbow2->getPixelAtIndex(curInd, btChannel);

							if (testPix & visFlag)
							{

							}
							else
							{
								btStack[0] = curInd;
								btStackInd = 0;
								totCount = 0;


								while ( (btStackInd > -1) && (totCount < 3) )
								{

									curInd = btStack[btStackInd];
									curY = curInd / w;
									curX = curInd - curY * w;

									fbow2->orPixelAtIndex(curInd, btChannel, visFlag);
									fbow2->setPixelAtIndex(curInd, houseChannel, id );


									count = 0;
									notFound = true;

									testPix2 = fbow2->getPixelAtIndex(curInd, btChannel);
									//testPix3 = fbow2->getPixelAtIndex(curInd, stChannel);
									testPix4 = fbow2->getPixelAtIndex(curInd, pathChannel);

									do
									{
										curDir = count;

										testX = curX + dirModX[curDir];
										testY = curY + dirModY[curDir];

										testInd = fbow2->getIndex(testX, testY);

										testPix = fbow2->getPixelAtIndex(testInd, btChannel);

										if (
											( (testPix & visFlag) == 0 ) &&
											( (testPix2 & dirFlagsO[curDir]) == 0) &&
											//( (testPix3 & dirFlagsO[curDir]) == 0) &&
											( (testPix4 & dirFlagsO[curDir]) == 0)
										)
										{
											notFound = false;
											totCount++;

										}


										count++;
									}
									while (notFound && count < 4);

									if (notFound)
									{
										btStackInd--;
									}
									else
									{

										// join the two and remove walls
										//fbow2->andPixelAtIndex(curInd, btChannel, dirFlags[bestDir]);
										//fbow2->andPixelAtIndex(bestInd, btChannel, dirFlagsOp[bestDir]);


										btStackInd++;
										btStack[btStackInd] = testInd;
									}

								}

								id++;
								if (id > 254)
								{
									id = 1;
								}




							}


						}
					}

				}

			}
		}


		cout << "end filling houses\n";

		// clear visited
		for (k = 0; k < totSize; k++)
		{
			fbow2->andPixelAtIndex(k, btChannel, visFlagO );
		}



		// ?????










		fbow2->cpuToGPU();

		delete[] btStack;
		delete[] streetFlagsH;
		delete[] streetFlagsV;
		delete[] oceanRes;

		//////////

DONE_WITH_MAP:

		singleton->mapInvalid = false;

		

		cout << "DONE WITH MAP\n";

		mapLockOn = false;

	}


	void drawMap()
	{

		if (singleton->menuList[E_FM_MAINMENU] == NULL) {
			return;
		}
		if (singleton->menuList[E_FM_MAINMENU]->visible){
			
		}
		else {
			return;
		}
		if (singleton->mapComp == NULL) {
			return;
		}
		if (singleton->mapComp->visible) {
			
		}
		else {
			return;
		}


		FBOWrapper *fbow = singleton->getFBOWrapper("hmFBOLinear", 0);

		


		singleton->bindShader("TopoShader");
		
		//singleton->bindFBO("resultFBO0");
		
		//singleton->sampleFBO("palFBO", 0);
		singleton->setShaderTexture3D(0,singleton->volIdMat);
		singleton->sampleFBO("hmFBO", 1); //Linear
		singleton->sampleFBO("cityFBO", 2);
		singleton->sampleFBO("hmFBOLinear",3);
		singleton->setShaderTexture(4, singleton->imageVoro->tid);
		//singleton->sampleFBO("frontFaceMapFBO",4);

		
		singleton->setShaderFloat("timeOfDay", singleton->timeOfDay);
		singleton->setShaderfVec4("mapFreqs", &(singleton->mapFreqs) );
		singleton->setShaderfVec4("mapAmps", &(singleton->mapAmps) );
		singleton->setShaderfVec3("cameraPos", singleton->cameraGetPos());
		singleton->setShaderfVec3("lookAtVec", &(singleton->lookAtVec));
		
		singleton->setShaderFloat("cameraZoom", singleton->cameraZoom);
		//singleton->setShaderFloat("mapTrans", mapTrans);
		singleton->setShaderFloat("seaLevel", singleton->getSLNormalized() );
		singleton->setShaderFloat("curTime", singleton->curTime);
		singleton->setShaderVec2("mapDimInPixels", fbow->width, fbow->height);

		singleton->setShaderFloat("mapPitch", singleton->mapPitch);
		singleton->setShaderFloat("heightMapMaxInCells", singleton->heightMapMaxInCells);
		singleton->setShaderFloat("cellsPerWorld", cellsPerWorld );
		
		
		singleton->mainGUI->renderQuadDirect(singleton->mapComp);
		

		//singleton->drawQuadBounds

		//singleton->drawFSQuadOffset(0.75f,0.75f,0.5f,1.0f/singleton->aspectRatio);
		//singleton->drawFSQuad();

		
		singleton->setShaderTexture(4, 0);
		//singleton->unsampleFBO("frontFaceMapFBO",4);
		singleton->unsampleFBO("hmFBOLinear",3);
		singleton->unsampleFBO("cityFBO", 2);
		singleton->unsampleFBO("hmFBO", 1);
		//singleton->unsampleFBO("palFBO", 0);
		singleton->setShaderTexture3D(0,0);
		
		//singleton->unbindFBO();
		
		singleton->unbindShader();

		
		
		//glEnable(GL_BLEND);
		//singleton->drawFBO("resultFBO0",0,1.0f);
		//glDisable(GL_BLEND);


	}

	void doBlur(string fboName, int _baseFBO = 0)
	{
		int i;
		int baseFBO = _baseFBO;
		
		singleton->bindShader("BlurShader");
		singleton->setShaderFloat("numBlurPixelsPerSide", 4.0); // value of 4 is a 9x9 kernel (4*2+1)
		singleton->setShaderFloat("sigma", 4.0);
		
		for (i = 0; i < 2; i++)
		{
			
			singleton->bindFBO(fboName, baseFBO);
			singleton->sampleFBO(fboName, 0, baseFBO);
			
			if (i == 0) { // horizontal
				singleton->setShaderFloat("blurSize", 1.0f/(singleton->currentFBOResolutionX));
				singleton->setShaderVec2("blurMultiplyVec", 1.0f, 0.0f);
			}
			else { // vertical
				singleton->setShaderFloat("blurSize", 1.0f/(singleton->currentFBOResolutionY));
				singleton->setShaderVec2("blurMultiplyVec", 0.0f, 1.0f);
			}

			
			singleton->drawFSQuad();
			
			singleton->unsampleFBO(fboName, 0, baseFBO);
			singleton->unbindFBO();
			

			baseFBO = 1 - baseFBO;

		}
		
		
		singleton->unbindShader();
		
	}

// 	void updateLights()
// 	{
		
		
// 		int i;
// 		int j;
// 		int k;
// 		bool flag = true;
// 		GameEnt *tempLight;
// 		GameEnt *curLight;
		
		
// 		lightCount = singleton->numDynLights;

// 		if (singleton->targetTimeOfDay <= 0.5)
// 		{
			
			
// 			findNearestEnt(&(singleton->nearestLights),E_ET_LIGHT,4,2,singleton->cameraGetPosNoShake(),false,true);
			
			
// 			for (i = 0; i < singleton->nearestLights.selEntList.size(); i++) {
				
				
// 				curLight = singleton->nearestLights.selEntList[i];//&(curBlock->gameEnts[E_ET_LIGHT].data[k]);
// 				curLight->camDistance = singleton->cameraGetPosNoShake()->distance(&(curLight->geomParams[E_LP_POSITION]));

// 				if (curLight->toggled) {
// 					activeLights[lightCount] = singleton->nearestLights.selEntList[i];//&(curBlock->gameEnts[E_ET_LIGHT].data[k]);
// 					lightCount++;
// 				}

// 				if (lightCount >= MAX_EVAL_LIGHTS)
// 				{
// 					goto UPDATE_LIGHTS_END;
// 				}
				
				
// 			}
			
			

// UPDATE_LIGHTS_END:

// 			for (i = singleton->numDynLights + 1; (i <= lightCount) && flag; i++)
// 			{
// 				flag = false;
// 				for (j = singleton->numDynLights; j < (lightCount - 1); j++)
// 				{
// 					if (activeLights[j + 1]->camDistance < activeLights[j]->camDistance) // ascending order simply changes to <
// 					{
// 						tempLight = activeLights[j];
// 						activeLights[j] = activeLights[j + 1];
// 						activeLights[j + 1] = tempLight;
// 						flag = true;
// 					}
// 				}
// 			}

// 			lightCount = min(lightCount, MAX_LIGHTS);
// 		}



// 	}

	// void renderWaveHeight() {
	// 	singleton->bindShader("WaveHeightShader");
	// 	singleton->bindFBO("waveFBO");
		
	// 	singleton->sampleFBO("solidTargFBO",0);
	// 	singleton->sampleFBO("waterTargFBO",2);
		
	// 	singleton->setShaderVec2("resolution", singleton->currentFBOResolutionX, singleton->currentFBOResolutionY);
	// 	singleton->setShaderFloat("curTime", singleton->curTime);
	// 	singleton->drawFSQuad();

	// 	singleton->unsampleFBO("waterTargFBO",2);
	// 	singleton->unsampleFBO("solidTargFBO",0);
		
	// 	singleton->unbindFBO();
	// 	singleton->unbindShader();
	// }




	void rasterHolders(bool doShadow) {
		
		// get view matrix
		// singleton->perspectiveOn = true;
		// singleton->getMatrixFromFBO("rasterFBO0");
		// singleton->perspectiveOn = false;

		int activeRaster = 0;

		int q;

		int numCubes = singleton->tempPrimList.size();
		if (singleton->settings[E_BS_PLACING_GEOM]) {
			numCubes++;
		}

		glEnable(GL_DEPTH_TEST);
		
		if (!DO_POINTS) {
			glEnable(GL_CULL_FACE);
		}
		
		
		if (doShadow) {
			
			// glDepthRangef(
			// 	singleton->clipDist[0],
			// 	singleton->clipDist[1]+singleton->conVals[E_CONST_LIGHTDIS]
			// );
			
			singleton->updateLightPos();
			singleton->getLightMatrix();
			
			
			
			singleton->bindShader("ShadowMapShader");
			singleton->bindFBO("shadowMapFBO");
			
			singleton->setShaderVec2("clipDist",singleton->clipDist[0],singleton->clipDist[1]);
			singleton->setShaderVec2("bufferDim", singleton->currentFBOResolutionX, singleton->currentFBOResolutionY);
			singleton->setShaderfVec3("lightPos", &(singleton->lightPos));
			singleton->setShaderfVec3("cameraPos", singleton->cameraGetPos());
			singleton->setShaderMatrix4x4("lightSpaceMatrix",singleton->lightSpaceMatrix.get(),1);
			
			
			// singleton->setShaderfVec3("minBounds",&(minShadowBounds));
			// singleton->setShaderfVec3("maxBounds",&(maxShadowBounds));
			// singleton->setShaderfVec3("lightVec",&(singleton->lightVec));

			rastChunk(singleton->iGetConst(E_CONST_RASTER_CHUNK_RAD), 0);

			singleton->unbindFBO();
			singleton->unbindShader();
			
			// glDepthRangef(
			// 	singleton->clipDist[0],
			// 	singleton->clipDist[1]	
			// );
		}
		else {
			singleton->bindShader("HolderShader");
			singleton->bindFBO("rasterFBO",activeRaster);
			
			singleton->setShaderFloat("heightOfNearPlane",singleton->heightOfNearPlane);
			singleton->setShaderFloat("FOV", singleton->FOV*M_PI/180.0f);
			singleton->setShaderVec2("clipDist",singleton->clipDist[0],singleton->clipDist[1]);
			singleton->setShaderVec2("bufferDim", singleton->currentFBOResolutionX, singleton->currentFBOResolutionY);
			//singleton->setShaderfVec3("cameraPos", singleton->cameraGetPos());
			
			//singleton->setShaderMatrix4x4("lightSpaceMatrix",singleton->lightSpaceMatrix.get(),1);
			singleton->setShaderMatrix4x4("pmMatrix",singleton->pmMatrix.get(),1);

			rastChunk(singleton->iGetConst(E_CONST_RASTER_CHUNK_RAD), RH_FLAG_CLIPTOVIEW);

			singleton->unbindFBO();
			singleton->unbindShader();
			
			activeRaster = 1 - activeRaster;
			
			
			
			
			
			glEnable(GL_CULL_FACE);
			glCullFace(GL_FRONT);
			
			singleton->bindShader("BasicPrimShader");
			singleton->bindFBO("rasterLowFBO");
			
			if (numCubes > 0) {
				
				singleton->setShaderTBO(
					0,
					singleton->primTBO.tbo_tex,
					singleton->primTBO.tbo_buf,
					true
				);
				
				singleton->setShaderFloat("heightOfNearPlane",singleton->heightOfNearPlane);
				singleton->setShaderFloat("FOV", singleton->FOV*M_PI/180.0f);
				singleton->setShaderVec2("clipDist",singleton->clipDist[0],singleton->clipDist[1]);
				singleton->setShaderVec2("bufferDim", singleton->currentFBOResolutionX, singleton->currentFBOResolutionY);
				singleton->setShaderfVec3("cameraPos", singleton->cameraGetPos());
				singleton->setShaderMatrix4x4("pmMatrix",singleton->pmMatrix.get(),1);
				
				// singleton->setShaderfVec4("paramFetch1", &tempVec1 );
				// singleton->setShaderfVec4("paramFetch2", &tempVec2 );
				// singleton->setShaderFloat("primArrLength",numCubes);
				
				singleton->setShaderArrayfVec4("primArr", singleton->primArr, numCubes*2);
				singleton->zoCubes.drawCubes(numCubes);
				
				singleton->setShaderTBO(0,0,0,true);
			}
			
			
			
			singleton->unbindFBO();
			singleton->unbindShader();
			
			
			glCullFace(GL_BACK);
			glDisable(GL_CULL_FACE);
		}
		
		
		
		if (!DO_POINTS) {
			glDisable(GL_CULL_FACE);
		}
		
		glDisable(GL_DEPTH_TEST);

		
		if (doShadow) {
			return;	
		}
		
		
		
		if (
			(singleton->iGetConst(E_CONST_GROWPOINTSTEPS) > 0) &&
			(DO_POINTS)	
		) {
			
			singleton->bindShader("PointShader");
			
			singleton->setShaderFloat("voxelsPerCell",singleton->voxelsPerCell);
			singleton->setShaderFloat("FOV", singleton->FOV*M_PI/180.0f);
			singleton->setShaderVec2("clipDist",singleton->clipDist[0],singleton->clipDist[1]);
			singleton->setShaderfVec3("cameraPos", singleton->cameraGetPos());
			singleton->setShaderInt("totRad",singleton->iGetConst(E_CONST_HVRAD));
			singleton->setShaderMatrix4x4("modelviewInverse",singleton->viewMatrixDI,1);
			
			for (q = 0; q < singleton->iGetConst(E_CONST_GROWPOINTSTEPS); q++) {
				
				
				if ((q % 2) == 0) {
					singleton->setShaderVec2("hvMult", 1.0f, 0.0f);
				}
				else {
					singleton->setShaderVec2("hvMult", 0.0f, 1.0f);
				}
				
				singleton->bindFBO("rasterFBO", activeRaster);
				singleton->sampleFBO("rasterFBO",0,activeRaster);
				
				singleton->setShaderInt("lastPass", (int)(q == (singleton->iGetConst(E_CONST_GROWPOINTSTEPS)-1)) );
				singleton->setShaderVec2("bufferDim", singleton->currentFBOResolutionX, singleton->currentFBOResolutionY);
				
				singleton->fsQuad.draw();

				singleton->unsampleFBO("rasterFBO",0,activeRaster);
				singleton->unbindFBO();
				
				activeRaster = 1 - activeRaster;	
			}
			
			singleton->unbindShader();
			
			
			singleton->bindShader("NearestShader");
			singleton->bindFBO("rasterFBO", activeRaster);
			singleton->sampleFBO("rasterFBO",0,activeRaster);
			singleton->sampleFBO("rasterLowFBO",2);
			singleton->setShaderVec2("bufferDim", singleton->currentFBOResolutionX, singleton->currentFBOResolutionY);
			singleton->setShaderfVec3("cameraPos", singleton->cameraGetPos());
			singleton->setShaderInt("totRad",singleton->iGetConst(E_CONST_FILLNEARESTRAD));
			singleton->fsQuad.draw();
			singleton->unsampleFBO("rasterLowFBO",2);
			singleton->unsampleFBO("rasterFBO",0,activeRaster);
			singleton->unbindFBO();
			singleton->unbindShader();
			activeRaster = 1 - activeRaster;
			
		}
		
		
		
		
		
		
		singleton->copyFBO("rasterFBO"+i__s(activeRaster), "solidTargFBO");
		
		
		singleton->bindShader("LightShader");
		singleton->bindFBO("resultFBO", activeFBO);
		singleton->sampleFBO("rasterFBO",0,activeRaster);
		singleton->sampleFBO("debugTargFBO", 2);
		singleton->setShaderTexture3D(4,singleton->volIdMat);
		singleton->sampleFBO("shadowMapFBO",5);
		
		
		if (singleton->mouseState == E_MOUSE_STATE_BRUSH) {
			lastUnitPos.setFW(((int)singleton->curBrushRad));
			singleton->setShaderfVec4("brushPos", &(lastUnitPos));
			
			switch (singleton->earthMod) {
				case E_PTT_TER:
					singleton->setShaderVec3("brushCol", 1.0f,0.0f,0.0f);
				break;
				case E_PTT_WAT:
					singleton->setShaderVec3("brushCol", 0.0f,0.0f,1.0f);
				break;
				case E_PTT_BLD:
					singleton->setShaderVec3("brushCol", 0.0f,1.0f,0.0f);
				break;
			}
			
		}
		else {
			singleton->setShaderVec4("brushPos", 0.0f,0.0f,0.0f,0.0f);
		}
		
		/*
		if (singleton->mouseState == E_MOUSE_STATE_BRUSH) {
			if (singleton->earthMod == E_PTT_TER) {
				singleton->setShaderVec3("matVal", 255, 0, 0);
			}
			else {
				singleton->setShaderVec3("matVal", 0, 0, 255);
			}
			
			
			singleton->setShaderFloat("isWire", 1.0);
			singleton->drawCubeCentered(
				&lastUnitPos,
				((int)singleton->curBrushRad)
			);
			
		}
		
		*/
		
		singleton->setShaderInt("gridOn", singleton->settings[E_BS_SHOW_GRID]);
		singleton->setShaderFloat("gammaVal", singleton->gammaVal);
		singleton->setShaderFloat("cellsPerChunk",singleton->cellsPerChunk);
		singleton->setShaderfVec3("lightPos", &(singleton->lightPos));
		singleton->setShaderInt("testOn3", (int)(singleton->settings[E_BS_TEST_3]));
		// singleton->setShaderfVec3("minBounds",&(minShadowBounds));
		// singleton->setShaderfVec3("maxBounds",&(maxShadowBounds));
		singleton->setShaderfVec3("lookAtVec", &(singleton->lookAtVec));
		singleton->setShaderInt("iNumSteps", singleton->iNumSteps);
		singleton->setShaderFloat("voxelsPerCell",singleton->voxelsPerCell);
		singleton->setShaderInt("cellsPerHolder",singleton->cellsPerHolder);
		singleton->setShaderFloat("FOV", singleton->FOV*M_PI/180.0f);
		singleton->setShaderVec2("clipDist",singleton->clipDist[0],singleton->clipDist[1]);
		singleton->setShaderVec2("shadowBias", 
				singleton->conVals[E_CONST_SHADOWBIASMIN],
				singleton->conVals[E_CONST_SHADOWBIASMAX]
			);
		singleton->setShaderVec2("bufferDim", singleton->currentFBOResolutionX, singleton->currentFBOResolutionY);
		singleton->setShaderfVec3("cameraPos", singleton->cameraGetPos());
		singleton->setShaderfVec3("lightVec", &(singleton->lightVec) );
		singleton->setShaderMatrix4x4("modelviewInverse",singleton->viewMatrixDI,1);
		singleton->setShaderMatrix4x4("lightSpaceMatrix",singleton->lightSpaceMatrix.get(),1);
		singleton->setShaderMatrix4x4("pmMatrix",singleton->pmMatrix.get(),1);
		
		singleton->fsQuad.draw();

		singleton->unsampleFBO("shadowMapFBO",5);
		singleton->setShaderTexture3D(4,0);
		singleton->unsampleFBO("debugTargFBO", 2);
		singleton->unsampleFBO("rasterFBO",0,activeRaster);
		singleton->unbindFBO();
		
		singleton->unbindShader();
		
		activeFBO = 1-activeFBO;
		
		
		
		
	}

	void rasterGrid(VBOGrid* vboGrid, bool showResults) {
		
		// get view matrix
		singleton->perspectiveOn = true;
		singleton->getMatrixFromFBO("rasterFBO0");
		singleton->perspectiveOn = false;


		glEnable(GL_DEPTH_TEST);

		singleton->bindShader("GridShader");
		singleton->bindFBO("rasterFBO0");

		singleton->sampleFBO("rasterPosFBO",0);
		singleton->sampleFBO("rasterSourceFBO",1);

		singleton->setShaderFloat("FOV", singleton->FOV*M_PI/180.0f);
		singleton->setShaderVec2("clipDist",singleton->clipDist[0],singleton->clipDist[1]);
		singleton->setShaderfVec2("bufferDim", &(singleton->bufferDim));
		singleton->setShaderfVec3("cameraPos", singleton->cameraGetPos());
		
		
		singleton->setShaderMatrix4x4("modelviewInverse",singleton->viewMatrixDI,1);
		singleton->setShaderMatrix4x4("modelview",singleton->viewMatrix.get(),1);
		singleton->setShaderMatrix4x4("proj",singleton->projMatrix.get(),1);

		//singleton->fsQuad.draw();
		vboGrid->vboWrapper.draw();


		singleton->unsampleFBO("rasterSourceFBO",1);
		singleton->unsampleFBO("rasterPosFBO",0);
		singleton->unbindFBO();
		singleton->unbindShader();
		
		glDisable(GL_DEPTH_TEST);

		
		if (showResults) {
			singleton->drawFBO("rasterFBO0", 0, 1.0f);
			
			glutSwapBuffers();
			
			
		}
		
		
	}

	// void rasterOct(GameVoxelWrap* gameOct, bool showResults) {
		
	// 	// get view matrix
	// 	singleton->perspectiveOn = true;
	// 	singleton->getMatrixFromFBO("rasterFBO0");
	// 	singleton->perspectiveOn = false;


	// 	glEnable(GL_DEPTH_TEST);
	// 	//glDepthMask(GL_TRUE);

	// 	singleton->bindShader("RasterShader");
	// 	singleton->bindFBO("rasterFBO0");

	// 	// singleton->setShaderTBO(
	// 	// 	0,
	// 	// 	gameOct->octTBO.tbo_tex,
	// 	// 	gameOct->octTBO.tbo_buf,
	// 	// 	false
	// 	// );

	// 	singleton->setShaderFloat("curTime", singleton->curTime);
	// 	singleton->setShaderFloat("heightOfNearPlane",singleton->heightOfNearPlane);
	// 	singleton->setShaderFloat("dimInVoxels", gameOct->dimInVoxels);
	// 	singleton->setShaderInt("renderLevel", gameOct->renderLevel);
	// 	singleton->setShaderInt("vDataSize", gameOct->vDataSize);
	// 	singleton->setShaderInt("rootPtr", gameOct->rootPtr);
	// 	singleton->setShaderInt("nodeSize", gameOct->nodeSize);
	// 	singleton->setShaderFloat("FOV", singleton->FOV*M_PI/180.0f);
	// 	singleton->setShaderVec2("clipDist",singleton->clipDist[0],singleton->clipDist[1]);
	// 	singleton->setShaderfVec2("bufferDim", &(singleton->bufferDim));
	// 	singleton->setShaderfVec3("cameraPos", singleton->cameraGetPos());
		
		
	// 	singleton->setShaderMatrix4x4("modelviewInverse",singleton->viewMatrixDI,1);
	// 	singleton->setShaderMatrix4x4("modelview",singleton->viewMatrix.get(),1);
	// 	singleton->setShaderMatrix4x4("proj",singleton->projMatrix.get(),1);

	// 	gameOct->vboWrapper.drawPoints();

	// 	//singleton->setShaderTBO(0,0,0,false);
	// 	singleton->unbindFBO();
	// 	singleton->unbindShader();
		
		
	// 	//glDepthMask(GL_FALSE);
	// 	glDisable(GL_DEPTH_TEST);

		
	// 	if (showResults) {
	// 		singleton->drawFBO("rasterFBO0", 0, 1.0f);
			
	// 		glutSwapBuffers();
	// 	}
		
		
	// }

	// void renderOct(GameVoxelWrap* gameOct) {
		
	// 	// get view matrix
	// 	singleton->perspectiveOn = true;
	// 	singleton->getMatrixFromFBO("resultFBO0");
	// 	singleton->perspectiveOn = false;
	// 	// 
		

	// 	singleton->bindShader("OctShader");
	// 	singleton->bindFBO("resultFBO0");

	// 	singleton->setShaderTBO(
	// 		0,
	// 		gameOct->octTBO.tbo_tex,
	// 		gameOct->octTBO.tbo_buf,
	// 		false
	// 	);

	// 	singleton->setShaderFloat("dimInVoxels", gameOct->dimInVoxels);
	// 	singleton->setShaderInt("renderLevel", gameOct->renderLevel);
	// 	singleton->setShaderInt("vDataSize", gameOct->vDataSize);
	// 	singleton->setShaderInt("rootPtr", gameOct->rootPtr);
	// 	singleton->setShaderInt("nodeSize", gameOct->nodeSize);
	// 	singleton->setShaderFloat("FOV", singleton->FOV*M_PI/180.0f);
	// 	singleton->setShaderVec2("clipDist",singleton->clipDist[0],singleton->clipDist[1]);
	// 	singleton->setShaderfVec2("bufferDim", &(singleton->bufferDim));
	// 	singleton->setShaderfVec3("cameraPos", singleton->cameraGetPos());
	// 	singleton->setShaderMatrix4x4("modelviewInverse",singleton->viewMatrixDI,1);

	// 	singleton->fsQuad.draw();


	// 	singleton->setShaderTBO(0,0,0,false);
	// 	singleton->unbindFBO();
	// 	singleton->unbindShader();

	// 	singleton->drawFBO("resultFBO0", 0, 1.0f);
		
	// 	glutSwapBuffers();
		
	// }

	void renderDebug() {
		
		BaseObj* ge = singleton->gem->getCurActor();
		
		int i;
		
		float identMat[16];
		
		for (i = 0; i < 16; i++) {
			identMat[i] = 0.0f;
		}
		
		identMat[0] = 1.0f;
		identMat[5] = 1.0f;
		identMat[10] = 1.0f;
		identMat[15] = 1.0f;
		
		float myMat[16];
		Matrix4 myMatrix4;
		Vector4 myVector0;
		Vector4 myVector1;
		Vector4 resVector0;
		Vector4 resVector1;
		btVector3 basePos;
		float rad0 = 1.0f;
		float rad1 = 3.0f;
		
		btVector3 boxCenter;
		btVector3 boxRadius;
		BaseObj* grabber;
		
		
		float xrotrad = singleton->getCamRot(0);
		
		singleton->bindShader("GeomShader");
		singleton->bindFBO("debugTargFBO");
		
		
		singleton->setShaderVec4(
			"rotationZ",0.0f,0.0f,0.0f,0.0f
		);
		singleton->setShaderfVec3("cameraPos", singleton->cameraGetPos());
		singleton->setShaderfVec3("lookAtVec", &(singleton->lookAtVec));
		singleton->setShaderFloat("isWire", 0.0);
		singleton->setShaderFloat("clipDist",singleton->clipDist[1]);
		singleton->setShaderMatrix4x4("modelview",singleton->viewMatrix.get(),1);
		singleton->setShaderMatrix4x4("proj",singleton->projMatrix.get(),1);
		singleton->setShaderFloat("objectId",0.0);
		singleton->setShaderVec3("matVal", 255, 0, 0);
		
		
		
		// if (singleton->gamePhysics != NULL) {
		// 	singleton->drawOrient = true;
		// 	singleton->gamePhysics->example->renderScene();
		// }
		
		
		
		glLineWidth(1.0f);
		
		singleton->setShaderFloat("objectId",0.0);
		
		
		
		// skeleton outline		
		if (singleton->gem->getCurActor() != NULL) {
			if (singleton->gem->getCurActor()->orgId > -1) {
				
				if (singleton->gem->getCurActor()->isGrabbedById > -1) {
					grabber = &(singleton->gem->gameObjects[singleton->gem->getCurActor()->isGrabbedById]);
				}
				else {
					grabber = singleton->gem->getCurActor();
				}
				
				
				grabber->bodies[E_BDG_CENTER].body->getWorldTransform().getOpenGLMatrix(myMat);
				
				singleton->setShaderMatrix4x4("objmat",myMat,1);
				
				
				drawOrg(singleton->gem->gameOrgs[singleton->gem->getCurActor()->orgId], false);
			}
		}
		
		glLineWidth(4.0f);
		
		
		singleton->setShaderMatrix4x4("objmat",identMat,1);
		
		float healthMeterScale = 0.5f;
		
		if (singleton->settings[E_BS_SHOW_HEALTH]) {
			for (i = 0; i < singleton->gem->visObjects.size(); i++) {
				ge = &(singleton->gem->gameObjects[singleton->gem->visObjects[i]]);
				if (ge->entType == E_ENTTYPE_NPC) {
					
					//ge->bodies[E_BDG_CENTER].body->getWorldTransform().getOpenGLMatrix(myMat);
					
					if (ge->isAlive()) {
						singleton->setShaderVec3("matVal", 1, 1, 1);
						boxCenter = ge->getCenterPoint(E_BDG_CENTER);
						boxCenter += btVector3(0.0f,0.0f,5.0f);
						boxRadius = btVector3(1.95f,0.2f,0.2f)*healthMeterScale;
						
						singleton->setShaderVec4(
							"rotationZ",
							boxCenter.getX(),
							boxCenter.getY(),
							boxCenter.getZ(),
							xrotrad	
						);
						
						singleton->drawBoxRad(boxCenter,boxRadius);
						
						singleton->setShaderVec3("matVal", 255, 0, 0);
						boxCenter = ge->getCenterPoint(E_BDG_CENTER);
						boxCenter += btVector3(-2.0f*(1.0f-ge->healthPerc())*healthMeterScale,0.0f,5.0f);
						boxRadius = btVector3(2.0f*ge->healthPerc(),0.25f,0.25f)*healthMeterScale;
						singleton->drawBoxRad(boxCenter,boxRadius);
					}
					
					if (singleton->settings[E_BS_TURN_BASED]) {
						boxCenter = (ge->tbPos + btVector3(0.5f,0.5f,0.5f));
						singleton->setShaderVec4(
							"rotationZ",
							boxCenter.getX(),
							boxCenter.getY(),
							boxCenter.getZ(),
							0.0f	
						);
						singleton->setShaderVec3("matVal", 255, 0, 0);
						singleton->drawBoxMinMax(ge->tbPos, ge->tbPos + btVector3(1.0f,1.0f,1.0f));
					}
					
					
					
				}
			}
		}
		
		
		
		singleton->setShaderVec4(
			"rotationZ",0.0f,0.0f,0.0f,0.0f
		);
		singleton->setShaderMatrix4x4("objmat",identMat,1);
		
		
		if (singleton->settings[E_BS_PATH_FINDING]) {
			singleton->gameLogic->update();
		}
		
		if (singleton->settings[E_BS_RENDER_VOXELS]) {
			singleton->setShaderFloat("isWire", 1.0);
			singleton->setShaderVec3("matVal", 255, 0, 0);
			
			rastChunk(singleton->iGetConst(E_CONST_RASTER_CHUNK_RAD), RH_FLAG_DRAWLOADING);
			
			if (holderInFocus != NULL) {
				singleton->setShaderVec3("matVal", 0, 0, 255);
				singleton->drawBox(&(holderInFocus->gphMinInCells),&(holderInFocus->gphMaxInCells));
			}
			
			// singleton->setShaderVec3("matVal", 0, 255, 0);
			// singleton->drawBox(&(minShadowBounds),&(maxShadowBounds));
			
		}
		
		
		
		// if (singleton->mouseState == E_MOUSE_STATE_BRUSH) {
		// 	if (singleton->earthMod == E_PTT_TER) {
		// 		singleton->setShaderVec3("matVal", 255, 0, 0);
		// 	}
		// 	else {
		// 		singleton->setShaderVec3("matVal", 0, 0, 255);
		// 	}
			
			
		// 	singleton->setShaderFloat("isWire", 1.0);
		// 	singleton->drawCubeCentered(
		// 		&lastUnitPos,
		// 		((int)singleton->curBrushRad)
		// 	);
			
		// }
			
			
			
			
			

		
		
		
		
		// if (singleton->settings[E_BS_RENDER_OCT_BOUNDS]) {
		// 	singleton->setShaderFloat("isWire", 1.0);
		// 	singleton->setShaderVec3("matVal", 255, 0, 0);
		// 	singleton->gameOct->startRender();
		// }
		
		
		
		// btVector3 begPos = btVector3(0.0f,0.0f,0.0f);
		// btVector3 endPos = btVector3(0.0f,0.0f,0.0f);
		// btVector3 rayDir = btVector3(0.0f,0.0f,0.0f);
		
		// singleton->getRay(
		// 	singleton->lastMouseX,
		// 	singleton->lastMouseY,
		// 	begPos,
		// 	endPos,
		// 	rayDir
		// );
		//lastBodyPick = example->bodyPick(begPos,endPos);
		
		// tempVec1.setBTV(rayDir);
		// tempVec2.setBTV(rayDir);
		
		
		
		
		
		// tempVec1.setBTV(rayFrom*0.99 + rayTo*0.01);
		// tempVec2.setBTV(rayTo);
		
		// singleton->setShaderFloat("objectId",0.0);
		// singleton->setShaderVec3("matVal", 255, 0, 0);
		// singleton->drawBox(&tempVec1,&tempVec2);
		
		
		
		
		// tempVec1.copyFrom(singleton->cameraGetPosNoShake());
		// tempVec2.copyFrom(singleton->cameraGetPosNoShake());
		
		// tempVec1.addXYZ(
		// 	20.0f,
		// 	20.0f,
		// 	20.0f	
		// );
		
		// tempVec2.addXYZ(
		// 	200.0f,
		// 	200.0f,
		// 	singleton->smoothTime*200.0f	
		// );
		
		// singleton->drawLine(&tempVec1,&tempVec2);
		
		
		
		
		
		
		
		// btVector3 rayFrom = singleton->cameraGetPosNoShake()->getBTV();
		// btVector3 rayTo = singleton->getRayTo(
		// 	singleton->origWinW - singleton->lastMouseOrigX,
		// 	singleton->origWinH - singleton->lastMouseOrigY
		// );
		
		// tempVec1.setBTV(rayFrom*(0.995+singleton->smoothTime*0.005) + rayTo*(0.005-(1.0-singleton->smoothTime)*0.005));
		// tempVec2.copyFrom(&tempVec1);
		// //tempVec3.setBTV(rayDir);
		
		
		// tempVec1.addXYZ(-1.0f);
		// tempVec2.addXYZ( 1.0f);
		
		// singleton->setShaderFloat("objectId",0.0);
		// singleton->setShaderVec3("matVal", 255, 0, 0);
		// singleton->drawBox(&tempVec1,&tempVec2);
		
		
		
		
		
		singleton->unbindFBO();
		singleton->unbindShader();
		
	}



	void finalStep(bool postToScreen) {
		if (singleton->settings[E_BS_TEST_1]) {			
			
			
			
			
			//terGenFBO
			//solidTargFBO
			
			
			
			//solidBaseTargFBO
			//"solidTargFBO" //"polyFBO"
			singleton->drawFBO("terTargFBO", 0, 1.0f);//solidTargFBO //waterTargFBO //solidTargFBO
			
			// leave this here to catch errors
			//cout << "Getting Errors: \n";
			
			
			
			
			//glError();
			
		}
		else {
			
			// if (singleton->frameCount > 1) {
			// 	singleton->bindShader("MergeShader");
			// 	singleton->bindFBO("resultFBO",activeFBO);
			// 	singleton->sampleFBO("resultFBO", 0, activeFBO);
			// 	singleton->sampleFBO("lastFBO", 1);
			// 	singleton->drawFSQuad();
			// 	singleton->unsampleFBO("lastFBO", 1);
			// 	singleton->unsampleFBO("resultFBO", 0, activeFBO);
			// 	singleton->unbindFBO();
			// 	singleton->unbindShader();
				
			// 	activeFBO = 1-activeFBO;
			// }
			// if (activeFBO == 0) {
			// 	singleton->copyFBO("resultFBO0", "lastFBO");
			// }
			// else {
			// 	singleton->copyFBO("resultFBO1", "lastFBO");
			// }
			
			
			if (singleton->settings[E_BS_FXAA]) {
				singleton->bindShader("FXAAShader");
				singleton->bindFBO("resultFBO",activeFBO);
				singleton->sampleFBO("resultFBO", 0, activeFBO);
				singleton->setShaderfVec2("resolution",&(singleton->bufferDim));
				singleton->drawFSQuad();
				singleton->unsampleFBO("resultFBO", 0, activeFBO);
				singleton->unbindFBO();
				singleton->unbindShader();
				
				activeFBO = 1 - activeFBO;
			}
			
			
			
			if (postToScreen) {
				singleton->drawFBO("resultFBO", 0, 1.0f, 1 - activeFBO);
			}
			else {
				singleton->copyFBO("solidTargFBO", "rasterPosFBO");
				singleton->copyFBO("resultFBO"+i__s(activeFBO), "rasterSourceFBO");
			}
			
			
		}

		


		if (singleton->anyMenuVisible()&&postToScreen) {
			glEnable (GL_BLEND);

			singleton->bindShader("GUIShader");
			singleton->setShaderTexture(0,singleton->fontWrappers[EFW_TEXT]->fontImage->tid);
			singleton->setShaderTexture(1,singleton->fontWrappers[EFW_ICONS]->fontImage->tid);
			singleton->sampleFBO("swapFBOBLin0", 2);
			singleton->setShaderTexture3D(3,singleton->volIdMat);
			
			singleton->mainGUI->renderGUI();
			
			
			singleton->setShaderTexture3D(3,0);
			singleton->unsampleFBO("swapFBOBLin0", 2);
			singleton->setShaderTexture(1,0);
			singleton->setShaderTexture(0,0);
			singleton->unbindShader();
			
			glDisable(GL_BLEND);
		}
	}

	void postProcess(bool postToScreen)
	{

		

		int i;
		int k;
		int curCount;
		int baseInd;

		bool doProc = false;

		//GameEnt *curLight;



		// for (i = 0; i < singleton->numDynLights; i++)
		// {
		// 	activeLights[i] = singleton->dynObjects[E_OBJ_LIGHT0 + i]->getLight();
		// }

		// curCount = 0;
		// for (k = 0; k < lightCount; k++)
		// {
		// 	baseInd = curCount * FLOATS_PER_LIGHT;
		// 	curLight = activeLights[k];
		// 	lightPos = &(curLight->geomParams[E_LP_POSITION]);

		// 	// if (curLight->toggled) {
		// 	// 	singleton->worldToScreenBase(&lScreenCoords, lightPos);

		// 	// 	singleton->lightArr[baseInd + 0] = lightPos->getFX();
		// 	// 	singleton->lightArr[baseInd + 1] = lightPos->getFY();
		// 	// 	singleton->lightArr[baseInd + 2] = lightPos->getFZ();
		// 	// 	singleton->lightArr[baseInd + 3] = lScreenCoords.getFZ();


		// 	// 	singleton->lightArr[baseInd + 4] = lScreenCoords.getFX();
		// 	// 	singleton->lightArr[baseInd + 5] = lScreenCoords.getFY();
		// 	// 	singleton->lightArr[baseInd + 6] = lScreenCoords.getFZ();
		// 	// 	singleton->lightArr[baseInd + 7] = curLight->geomParams[E_LP_RADIUS].getFX();


		// 	// 	// light color

		// 	// 	singleton->lightArr[baseInd + 8] = curLight->geomParams[E_LP_COLOR].getFX(); // light red
		// 	// 	singleton->lightArr[baseInd + 9] = curLight->geomParams[E_LP_COLOR].getFY(); // light green
		// 	// 	singleton->lightArr[baseInd + 10] = curLight->geomParams[E_LP_COLOR].getFZ(); // light blue

		// 	// 	// switch (k)
		// 	// 	// {
		// 	// 	// case 0:
		// 	// 	// 	singleton->lightArr[baseInd + 11] = 1.0f; // light intensity (unused?)
		// 	// 	// 	singleton->lightArr[baseInd + 12] = 0.0f; // light colorization (0-1)
		// 	// 	// 	singleton->lightArr[baseInd + 13] = 0.0f; // light flooding (colorizes regardless of shadows) (0-1)
		// 	// 	// 	break;
		// 	// 	// default:
		// 	// 	// 	singleton->lightArr[baseInd + 11] = 1.0f;
		// 	// 	// 	singleton->lightArr[baseInd + 12] = 1.0f;
		// 	// 	// 	singleton->lightArr[baseInd + 13] = 0.0f;
		// 	// 	// 	break;

		// 	// 	// }

		// 	// 	curCount++;
		// 	// }

			
		// }

		

		

		//renderWaveHeight();

		if (singleton->settings[E_BS_WATER])	{


			


			singleton->bindShader("WaterShaderCombine");
			singleton->bindFBO("combineWithWaterTargFBO");
			singleton->sampleFBO("solidTargFBO",0);
			singleton->sampleFBO("waterTargFBO",2);
			//singleton->sampleFBO("geomTargFBO", 4);
			//singleton->sampleFBO("waveFBO", 6);
			
			// singleton->setShaderFloat("clipDist",singleton->clipDist[1]);
			// singleton->setShaderVec2("resolution", singleton->currentFBOResolutionX, singleton->currentFBOResolutionY);
			// singleton->setShaderfVec3("cameraPos", singleton->cameraGetPos());
			// singleton->setShaderfVec2("bufferDim", &(singleton->bufferModDim));
			// singleton->setShaderFloat("curTime", singleton->curTime);
			singleton->drawFSQuad();
			
			//singleton->unsampleFBO("waveFBO", 6);
			//singleton->unsampleFBO("geomTargFBO", 4);
			singleton->unsampleFBO("waterTargFBO",2);
			singleton->unsampleFBO("solidTargFBO",0);
			singleton->unbindFBO();
			singleton->unbindShader();
			
			
			
		}
		else
		{
			// TODO: copy over any necessary buffers when water is off
			
			singleton->copyFBO2("solidTargFBO","combineWithWaterTargFBO");
			
		}
		
		
		//singleton->projMatrix*

		
		

		

		singleton->bindShader("PreLightingShader");
		singleton->bindFBO("prelightFBO");
		singleton->sampleFBO("solidTargFBO",0);
		singleton->sampleFBO("combineWithWaterTargFBO",2);
		singleton->sampleFBO("geomTargFBO",4);
		singleton->sampleFBO("prmDepthFBO",6);
		singleton->setShaderMatrix3x3("rotMat",singleton->curObjMatrix3.get(),1);
		singleton->setShaderMatrix4x4("modelview",singleton->viewMatrix.get(),1);
		singleton->setShaderMatrix4x4("proj",singleton->projMatrix.get(),1);
		singleton->setShaderVec2("clipDist",singleton->clipDist[0],singleton->clipDist[1]);
		singleton->setShaderfVec3("lookAtVec", &(singleton->lookAtVec));
		singleton->setShaderVec2("bufferDim", singleton->currentFBOResolutionX, singleton->currentFBOResolutionY); //MUST BE CALLED AFTER FBO IS BOUND
		singleton->setShaderfVec3("cameraPos", singleton->cameraGetPos());
		singleton->setShaderInt("testOn", (int)(singleton->settings[E_BS_TEST_1]));
		singleton->setShaderInt("iNumSteps", singleton->iNumSteps);
		singleton->setShaderArrayfVec4("lightArr", singleton->lightArr, (FLOATS_PER_LIGHT * lightCount) / 4);
		singleton->setShaderInt("vecsPerLight", FLOATS_PER_LIGHT / 4);
		singleton->setShaderFloat("lightCount", lightCount);
		singleton->setShaderFloat("timeOfDay", singleton->timeOfDay);
		singleton->setShaderfVec3("baseLightVec", &(singleton->lightVec) );
		
		singleton->drawFSQuad();
		
		singleton->unsampleFBO("prmDepthFBO",6);
		singleton->unsampleFBO("geomTargFBO",4);
		singleton->unsampleFBO("combineWithWaterTargFBO",2);
		singleton->unsampleFBO("solidTargFBO",0);
		singleton->unbindFBO();
		singleton->unbindShader();
		
			
		
		
		
		singleton->bindShader("PostLightingShader");
		singleton->bindFBO("resultFBO", activeFBO);
		
		singleton->sampleFBO("solidTargFBO",0);
		singleton->sampleFBO("prelightFBO", 2);
		// singleton->sampleFBO("geomTargFBO", 6);
		// singleton->sampleFBO("debugTargFBO", 8);
		singleton->setShaderTexture3D(10,singleton->volIdMat);
		
		singleton->setShaderfVec4("worldMarker",&(singleton->worldMarker));
		singleton->setShaderInt("markerFound", (int)(singleton->markerFound));
		
		
		singleton->setShaderMatrix4x4("modelviewInverse",singleton->viewMatrixDI,1);
		singleton->setShaderFloat("FOV", singleton->FOV*M_PI/180.0f);
		singleton->setShaderVec2("clipDist",singleton->clipDist[0],singleton->clipDist[1]);
		singleton->setShaderfVec3("lightVec", &(singleton->lightVec) );
		singleton->setShaderfVec3("lightVecOrig", &(singleton->lightVecOrig) );
		
		
		singleton->setShaderfVec3("lookAtVec", &(singleton->lookAtVec));
		singleton->setShaderVec2("bufferDim", singleton->currentFBOResolutionX, singleton->currentFBOResolutionY); //MUST BE CALLED AFTER FBO IS BOUND
		singleton->setShaderfVec3("cameraPos", singleton->cameraGetPos());
		singleton->setShaderInt("testOn", (int)(singleton->settings[E_BS_TEST_1]));
		singleton->setShaderInt("testOn2", (int)(singleton->settings[E_BS_TEST_2]));
		singleton->setShaderFloat("curTime", singleton->curTime);
		singleton->setShaderFloat("cellsPerBlock", singleton->cellsPerBlock);
		singleton->setShaderFloat("timeOfDay", singleton->timeOfDay);
		singleton->drawFSQuad();
		
		singleton->setShaderTexture3D(10,0);
		// singleton->unsampleFBO("debugTargFBO", 8);
		// singleton->unsampleFBO("geomTargFBO", 6);
		singleton->unsampleFBO("prelightFBO", 2);
		singleton->unsampleFBO("solidTargFBO",0);
		
		singleton->unbindFBO();
		singleton->unbindShader();
		
		activeFBO = 1 - activeFBO;
		
		

		if (singleton->settings[E_BS_WATER])
		{
			singleton->copyFBO("resultFBO1", "swapFBOLinHalf0");
			doBlur("swapFBOLinHalf");
			
			singleton->bindShader("WaterShader");
			singleton->bindFBO("resultFBO", activeFBO);
			singleton->sampleFBO("solidTargFBO",0);
			singleton->sampleFBO("combineWithWaterTargFBO",2);
			singleton->sampleFBO("resultFBO", 4, activeFBO);
			singleton->sampleFBO("swapFBOLinHalf0", 5);
			singleton->sampleFBO("noiseFBO", 6);
			//singleton->sampleFBO("waveFBO", 7);
			singleton->setShaderTexture3D(7,singleton->volIdMat);
			singleton->sampleFBO("prelightFBO", 9);
			//singleton->sampleFBO("geomTargFBO", 13);
			
			
			singleton->setShaderMatrix4x4("modelviewInverse",singleton->viewMatrixDI,1);
			singleton->setShaderFloat("seaLevel", singleton->getSeaHeightScaled() );
			singleton->setShaderFloat("FOV", singleton->FOV*M_PI/180.0f);
			singleton->setShaderVec2("clipDist",singleton->clipDist[0],singleton->clipDist[1]);
			singleton->setShaderfVec2("bufferDim", &(singleton->bufferModDim));
			
			singleton->setShaderfVec3("lookAtVec", &(singleton->lookAtVec) );
			singleton->setShaderFloat("timeOfDay", singleton->timeOfDay);
			singleton->setShaderVec2("resolution", singleton->currentFBOResolutionX, singleton->currentFBOResolutionY);
			singleton->setShaderfVec3("cameraPos", singleton->cameraGetPos());
			singleton->setShaderFloat("curTime", singleton->curTime);
			singleton->setShaderFloat("isUnderWater", singleton->getUnderWater() );
			singleton->drawFSQuad();
			
			
			//singleton->unsampleFBO("geomTargFBO", 13);
			singleton->unsampleFBO("prelightFBO", 9);
			singleton->setShaderTexture3D(7,0);
			//singleton->unsampleFBO("waveFBO", 7);
			singleton->unsampleFBO("noiseFBO", 6);
			singleton->unsampleFBO("swapFBOLinHalf0", 5);
			singleton->unsampleFBO("resultFBO", 4, activeFBO);
			singleton->unsampleFBO("waterTargFBO",2);
			singleton->unsampleFBO("solidTargFBO",0);
			singleton->unbindFBO();
			singleton->unbindShader();

			activeFBO = 1 - activeFBO;
		}


		if (singleton->settings[E_BS_RADIOSITY] || singleton->settings[E_BS_FOG])
		{

			if (activeFBO == 0)
			{
				singleton->copyFBO("resultFBO0", "swapFBOBLin0");
			}
			else
			{
				singleton->copyFBO("resultFBO1", "swapFBOBLin0");
			}


			doBlur("swapFBOBLin");
		}



		if (singleton->settings[E_BS_RADIOSITY])
		{

			singleton->bindShader("RadiosityShader");

			singleton->bindFBO("swapFBOLinHalf0");
			singleton->sampleFBO("combineWithWaterTargFBO",0);
			singleton->sampleFBO("swapFBOBLin0", 2);
			singleton->setShaderVec2("resolution", singleton->currentFBOResolutionX, singleton->currentFBOResolutionY); //MUST BE CALLED AFTER FBO IS BOUND
			singleton->setShaderfVec2("bufferDim", &(singleton->bufferModDim));
			singleton->setShaderfVec3("cameraPos", singleton->cameraGetPos());
			singleton->setShaderfVec3("lightVec", &(singleton->lightVec) );
			//singleton->setShaderfVec3("lightPosWS", lightPos);
			singleton->setShaderInt("iNumSteps", singleton->iNumSteps);
			singleton->drawFSQuad();
			singleton->unsampleFBO("swapFBOBLin0", 2);
			singleton->unsampleFBO("combineWithWaterTargFBO",0);
			singleton->unbindFBO();
			singleton->unbindShader();


			doBlur("swapFBOLinHalf");

			singleton->bindShader("RadiosityCombineShader");
			singleton->bindFBO("resultFBO", activeFBO);
			singleton->sampleFBO("resultFBO", 0, activeFBO);
			singleton->sampleFBO("swapFBOLinHalf0", 1);
			singleton->sampleFBO("combineWithWaterTargFBO",2);
			//singleton->sampleFBO("geomTargFBO", 4);
			singleton->setShaderInt("testOn", (int)(singleton->settings[E_BS_TEST_1]));
			singleton->drawFSQuad();
			//singleton->unsampleFBO("geomTargFBO", 4);
			singleton->unsampleFBO("combineWithWaterTargFBO",2);
			singleton->unsampleFBO("swapFBOLinHalf0", 1);
			singleton->unsampleFBO("resultFBO", 0, activeFBO);
			singleton->unbindFBO();
			singleton->unbindShader();

			activeFBO = 1 - activeFBO;


		}

		if (singleton->settings[E_BS_FOG])
		{

			

			singleton->bindShader("FogShader");
			singleton->bindFBO("resultFBO", activeFBO);

			singleton->sampleFBO("combineWithWaterTargFBO",0);
			singleton->sampleFBO("resultFBO", 2, activeFBO);
			singleton->sampleFBO("swapFBOBLin0", 3);
			
			singleton->sampleFBO("limbFBO", 4);//, -1, 4, 7);
			//singleton->sampleFBO("geomBaseTargFBO", 4);
			
			singleton->setShaderTexture3D(7,singleton->volIdMat);
			singleton->sampleFBO("noiseFBOLinear", 8);
			singleton->sampleFBO("debugTargFBO", 9);
			
			singleton->sampleFBO("numstepsFBO", 11);
			
			
			if ((singleton->gem->getCurActor() == NULL)||singleton->gem->firstPerson) {
				singleton->setShaderFloat("thirdPerson", 0.0f);
				singleton->setShaderVec3("entPos", 0.0f, 0.0f, 0.0f);
			}
			else {
				singleton->setShaderFloat("thirdPerson", 1.0f);
				singleton->setShaderfVec3("entPos", singleton->gem->getCurActor()->getCenterPointFIV(0));
				singleton->setShaderFloat("volSizePrim", singleton->gameFluid[E_FID_BIG]->volSizePrim);
			}
			
			if (singleton->settings[E_BS_PLACING_PATTERN]) {
				singleton->setShaderArray(
					"patternCells",
					singleton->patterns[
						singleton->curPattern*4 + singleton->curPatternRot
					].patternVals,
					PATTERN_SIZE*PATTERN_SIZE
				);
				singleton->setShaderfVec3("patternTarg", &(singleton->mouseMovePD));
				
			}
			singleton->setShaderInt("placingPattern", singleton->settings[E_BS_PLACING_PATTERN]);
			
			
			singleton->setShaderInt("testOn2", (int)(singleton->settings[E_BS_TEST_2]));
			
			singleton->setShaderInt("gridOn", singleton->settings[E_BS_SHOW_GRID]);
			
			if (singleton->gem->getCurActor() == NULL) {
				singleton->setShaderInt("isFalling",false);
				singleton->setShaderInt("isJumping",false);
			}
			else {
				singleton->setShaderInt("isFalling",singleton->gem->getCurActor()->allFalling());
				singleton->setShaderInt("isJumping",singleton->gem->getCurActor()->getActionState(E_ACT_ISJUMPING,RLBN_NEIT));
			}
			
			
			singleton->setShaderFloat("seaLevel", singleton->getSeaHeightScaled() );
			singleton->setShaderMatrix4x4("modelviewInverse",singleton->viewMatrixDI,1);
			singleton->setShaderFloat("FOV", singleton->FOV*M_PI/180.0f);
			singleton->setShaderVec2("clipDist",singleton->clipDist[0],singleton->clipDist[1]);
			singleton->setShaderfVec3("lightVec", &(singleton->lightVec) );
			singleton->setShaderfVec3("lightVecOrig", &(singleton->lightVecOrig) );
			singleton->setShaderInt("iNumSteps", singleton->iNumSteps);
			singleton->setShaderFloat("curTime", singleton->curTime);
			singleton->setShaderFloat("selLimbInd",singleton->gem->highlightedLimb);
			singleton->setShaderFloat("selObjInd",singleton->gem->selObjInd);
			singleton->setShaderFloat("actObjInd",singleton->gem->actObjInd);
			singleton->setShaderFloat("isUnderWater", singleton->getUnderWater() );
			singleton->setShaderFloat("timeOfDay", singleton->timeOfDay);
			singleton->setShaderVec2("resolution", singleton->currentFBOResolutionX, singleton->currentFBOResolutionY); //MUST BE CALLED AFTER FBO IS BOUND
			singleton->setShaderfVec2("bufferDim", &(singleton->bufferModDim));
			singleton->setShaderfVec3("cameraPos", singleton->cameraGetPos());
			singleton->setShaderfVec3("lookAtVec", &(singleton->lookAtVec));
			//singleton->setShaderfVec4("fogPos", fogPos);

			singleton->drawFSQuad();

			singleton->unsampleFBO("numstepsFBO", 11);			
			singleton->unsampleFBO("debugTargFBO", 9);
			singleton->unsampleFBO("noiseFBOLinear", 8);
			singleton->setShaderTexture3D(7,0);
			
			singleton->unsampleFBO("limbFBO", 4);//, -1, 4, 7);
			//singleton->unsampleFBO("geomBaseTargFBO", 4);
			
			singleton->unsampleFBO("swapFBOBLin0", 3);
			singleton->unsampleFBO("resultFBO", 2, activeFBO);
			singleton->unsampleFBO("combineWithWaterTargFBO",0);

			singleton->unbindFBO();
			singleton->unbindShader();


			activeFBO = 1 - activeFBO;

		}
		
		
		
		

	}

	~GameWorld()
	{

		//threadpool.stopAll();

		int i;
		int j;

	}


};

