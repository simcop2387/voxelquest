
class GameWorld
{
public:
	

	int numProvinces;
	int seaLevel;
	int seaSlack;
	int pageCount;
	int mapSwapFlag;
	int blockSizeInHolders;

	int renderCount;

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
	
	int tempCellData[4];
	int tempCellData2[4];

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


	std::vector<coordAndIndex> roadCoords;


	std::vector<int> ocThreads;


	//std::vector<GameEnt> gameActors;
	
	map<BaseObjType, BaseObj> gameObjects;
	vector<BaseObjType> visObjects;
	vector<ObjDef> objDefs;
	
	
	
	
	float ppCell;



	
	FIVector4 lScreenCoords;
	FIVector4 cScreenCoords;
	FIVector4 worldSizeInHolders;
	FIVector4 worldSizeInBlocks;

	FIVector4 entMin;
	FIVector4 entMax;

	FIVector4 camHolderPos;
	//FIVector4 cutHolderPos;
	FIVector4 camBlockPos;
	FIVector4 iPixelWorldCoords;
	FIVector4 pagePos;
	FIVector4 unitPos;
	FIVector4 lastUnitPos;
	
	
	FIVector4 tempVec1;
	FIVector4 tempVec2;
	FIVector4 tempVec3;
	FIVector4 tempVec4;
	
	FIVector4 blockPos[2];
	FIVector4 nodePos[2];
	FIVector4 nodePosInPixels[2];
	PathNode blockAndIndexPath[2];
	PathNode blockAndIndexSplitPath[2];
	FIVector4 lineSeg[2];
	PathHolder* finalPath;
	
	int nodeInd[2];
	//int finalInd;
	
	GameBlock* blockRef[2];
	
	


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

	}


	void init(Singleton *_singleton)
	{

		singleton = _singleton;
		

		int i;
		int j;
		
		lastHitObjUID = 0;
		renderCount = 0;
		
		//finalInd = -1;
		
		lightCount = 1;

		noiseGenerated = false;

		finalPath = NULL;

		blockSizeInHolders = singleton->blockSizeInHolders;

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

		numProvinces = 32;
		provinceGrid = new int[numProvinces * numProvinces];
		provinceX = new int[numProvinces];
		provinceY = new int[numProvinces];

		ppCell = singleton->pixelsPerCell;

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

		worldSizeInHolders.copyFrom( &(singleton->worldSizeInHolders) );
		worldSizeInBlocks.copyFrom( &(singleton->worldSizeInBlocks) );

		iBlockSize = worldSizeInBlocks.getIX() * worldSizeInBlocks.getIY();



		blockData = new GameBlock*[iBlockSize];
		for (i = 0; i < iBlockSize; i++)
		{
			blockData[i] = NULL;
		}

		
		
	}


	
	
	// x, y, and z are measured in blocks
	GameBlock *getBlockAtCoords(int xInBlocks, int yInBlocks, bool createOnNull = false)
	{





		int newX = wrapCoord(xInBlocks, worldSizeInBlocks.getIX());
		int newY = wrapCoord(yInBlocks, worldSizeInBlocks.getIY());

		//cout << "xy " << newX << " " << newY << "\n";

		int ind =
			newY * worldSizeInBlocks.getIX() +
			newX;

		if (blockData[ind] == NULL) {
			if (createOnNull) {
				blockData[ind] = new GameBlock();
				blockData[ind]->init(singleton, ind, xInBlocks, yInBlocks, newX, newY);
			}
		}

		return blockData[ind];

	}

	// x, y, and z are measured in holders
	GamePageHolder *getHolderAtCoords(int x, int y, int z, bool createOnNull = false)
	{

		GamePageHolder **holderData;

		int newX = wrapCoord(x, worldSizeInHolders.getIX());
		int newY = wrapCoord(y, worldSizeInHolders.getIY());
		int newZ = z;

		int holderX = newX - intDiv(newX, blockSizeInHolders) * blockSizeInHolders;
		int holderY = newY - intDiv(newY, blockSizeInHolders) * blockSizeInHolders;
		int holderZ = newZ - intDiv(newZ, blockSizeInHolders) * blockSizeInHolders;

		int holderId = holderZ * blockSizeInHolders * blockSizeInHolders + holderY * blockSizeInHolders + holderX;


		GameBlock *curBlock = getBlockAtCoords(
														intDiv(x, blockSizeInHolders),
														intDiv(y, blockSizeInHolders),
														createOnNull
													);

		if (curBlock == NULL)
		{
			return NULL;
		}
		else
		{
			holderData = curBlock->holderData;


			if (holderData[holderId] == NULL)
			{
				if (createOnNull)
				{
					holderData[holderId] = new GamePageHolder();
					holderData[holderId]->init(singleton, curBlock->blockId, holderId, x, y, z); //, x, y, z
				}
			}

			return holderData[holderId];


		}



	}
	
	
	// x, y, and z are measured in terDataBufSize
	uint getTerDataAtCoords(int x, int y, int z)
	{

		int tdvpXY = singleton->terDataVisPitchXY;
		int tdvpZ = singleton->terDataVisPitchZ;

		int newX = wrapCoord(x, singleton->worldSizeInTerData.getIX());
		int newY = wrapCoord(y, singleton->worldSizeInTerData.getIY());
		int newZ = z;


		int holderX = newX - intDiv(newX, tdvpXY) * tdvpXY;
		int holderY = newY - intDiv(newY, tdvpXY) * tdvpXY;
		int holderZ = newZ - intDiv(newZ, tdvpZ) * tdvpZ;

		int holderId = holderZ * tdvpXY * tdvpXY + holderY * tdvpXY + holderX;


		GameBlock *curBlock = getBlockAtCoords(
														intDiv(x, tdvpXY),
														intDiv(y, tdvpXY),
														true
													);

		return curBlock->terData[holderId];


	}
	
	
	

	// assumed that holder exists when calling this
	GamePageHolder *getHolderAtId(intPair id)
	{

		if (blockData[id.v0] == NULL)
		{
			return NULL;
		}
		else
		{
			return blockData[id.v0]->holderData[id.v1];
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
	
	
	void setCellAtCoords(
		int xv, int yv, int zv,
		int readInd,
		int* fluidData,
		int* extraData
		//int v0, int v1, int v2, int v3
	) {
		
		int q;
		
		int cellsPerHolder = singleton->cellsPerHolder;
		
		int newX = wrapCoord(xv,singleton->worldSizeInCells.getIX());
		int newY = wrapCoord(yv,singleton->worldSizeInCells.getIY());
		int newZ = zv;
		
		int x2 = intDiv(newX,cellsPerHolder);
		int y2 = intDiv(newY,cellsPerHolder);
		int z2 = intDiv(newZ,cellsPerHolder);
		
		GamePageHolder* curHolder = getHolderAtCoords(x2, y2, z2, true);
		if (curHolder->wasGenerated) {
			
		}
		else {
			curHolder->genCellData();
		}
		//GamePage* gp = getPageAtCoords(x2, y2, z2, false);
		
		int xr = newX - x2*cellsPerHolder;
		int yr = newY - y2*cellsPerHolder;
		int zr = newZ - z2*cellsPerHolder;
		
		int ind = (zr*cellsPerHolder*cellsPerHolder + yr*cellsPerHolder + xr)*4;
		
		
		if (curHolder == NULL) {
			
		}
		else {
			if (curHolder->cellData == NULL) {
				
			}
			else {
				
				for (q = 0; q < 4; q++) {
					curHolder->cellData[ind+q] = fluidData[readInd+q];
					curHolder->extrData[ind+q] = extraData[readInd+q];
				}
				
				// curHolder->cellData[ind+0] = v0;
				// curHolder->cellData[ind+1] = v1;
				// curHolder->cellData[ind+2] = v2;
				// curHolder->cellData[ind+3] = v3;
			}
		}
	}
	
	

	// x, y, z measured in cells
	int getCellAtCoords(int xv, int yv, int zv) {
		
		int cellsPerHolder = singleton->cellsPerHolder;
		
		int newX = wrapCoord(xv,singleton->worldSizeInCells.getIX());
		int newY = wrapCoord(yv,singleton->worldSizeInCells.getIY());
		int newZ = zv;
		
		int x2 = intDiv(newX,cellsPerHolder);
		int y2 = intDiv(newY,cellsPerHolder);
		int z2 = intDiv(newZ,cellsPerHolder);
		
		int q;
		
		GamePageHolder* curHolder = getHolderAtCoords(x2, y2, z2, true);
		if (curHolder->wasGenerated) {
			
		}
		else {
			curHolder->genCellData();
		}
		//GamePage* gp = getPageAtCoords(x2, y2, z2, false);
		
		int xr = newX - x2*cellsPerHolder;
		int yr = newY - y2*cellsPerHolder;
		int zr = newZ - z2*cellsPerHolder;
		
		int ind = (zr*cellsPerHolder*cellsPerHolder + yr*cellsPerHolder + xr)*4;
		
		for (q = 0; q < 4; q++) {
			tempCellData[q] = FLUID_UNIT_MIN;
			tempCellData2[q] = FLUID_UNIT_MIN;
		}
		
		
		if (curHolder == NULL) {
			return E_CD_SOLID;
		}
		else {
			if (curHolder->cellData == NULL) {
				return E_CD_EMPTY;
			}
			else {
				
				for (q = 0; q < 4; q++) {
					tempCellData[q] = curHolder->cellData[ind+q];
					tempCellData2[q] = curHolder->extrData[ind+q];
				}
				
				if (
					(curHolder->cellData[ind+E_PTT_TER] != FLUID_UNIT_MIN) ||
					(curHolder->extrData[ind+E_PTT_BLD] != FLUID_UNIT_MIN)
				) {
					return E_CD_SOLID;
				}
				else {
					
					if (curHolder->cellData[ind+E_PTT_WAT] != FLUID_UNIT_MIN) {
						return E_CD_WATER;
					}
					else {
						return E_CD_EMPTY;
					}
					
					
				}
			}
		}
	}
	
	
	int testMoveHit(BaseObj* ge, int x, int y, int z) {
		ge->positionInCells.addXYZ(x,y,z);
		ge->updateBounds();
		int res = testHit(ge);
		ge->positionInCells.addXYZ(-x,-y,-z);
		ge->updateBounds();
		
		return res;
	}
	
	bool makeMove(BaseObj* ge) {
		
		bool hitGround = false;
		bool onGround = false;
		
		float fPixelsPerCell = singleton->pixelsPerCell;
		bool res;
		int ires;
		int i;
		
		
		tempVec1.copyFrom(&(ge->positionInCells));
		tempVec2.copyFrom(&(ge->posOffsetInPixels));
		
		ires = testHit(ge);
		if (ires == E_CD_SOLID) {
			// make it float
			ge->toggleGrav(0.0f);
			ge->vel.setFZ(0.0f);
			ge->posOffsetInPixels.addXYZ(
				ge->vel[0]*singleton->timeDelta,
				ge->vel[0]*singleton->timeDelta,
				1.0f*fPixelsPerCell*singleton->timeDelta	
			);
		}
		else {
			
			if (ires == E_CD_WATER) {
				
				
				if (ge->inWater) {
					
				}
				else {
					ge->inWater = true;
					singleton->playSoundPosAndPitch(
						"splash0",
						singleton->cameraPos,
						&(ge->centerPointInPixels),
						0.3f
					);
				}
				
				// make it float
				ge->toggleGrav(-0.5f);
				
				if ( abs(ge->vel[2]) > 0.5) {
					ge->vel.multXYZ(0.95);
				}
				
				
				// ge->posOffsetInPixels.addXYZ(
				// 	ge->vel[0]*singleton->timeDelta,
				// 	ge->vel[0]*singleton->timeDelta,
				// 	0.25f*fPixelsPerCell*singleton->timeDelta	
				// );
				
				ge->posOffsetInPixels.addXYZRef(&(ge->vel),singleton->timeDelta);
			}
			else {
				
				if (ge->inWater) {
					ge->inWater = false;
					singleton->playSoundPosAndPitch(
						"splashout0",
						singleton->cameraPos,
						&(ge->centerPointInPixels),
						0.3f
					);
				}
				else {
					
				}
				
				
				ge->toggleGrav(-1.0f);
				ge->vel.addXYZRef(&(ge->acc),singleton->timeDelta);
				ge->posOffsetInPixels.addXYZRef(&(ge->vel),singleton->timeDelta);
			}
			
			
		}
		
		
		
		if (ge->posOffsetInPixels[2] > 0.0f) {
			
			if (testMoveHit(ge,0,0,1) == E_CD_SOLID) {
				
				// if (ge->vel[2] > 0.0f) {
				// 	ge->vel.setFZ(0.0f);
				// 	ge->posOffsetInPixels.setFZ(0.0f);
				// }
				ge->vel.multXYZ(1.0f,1.0f,-ge->bounciness);
				ge->posOffsetInPixels.setFZ(0.0f);
			}
		}
		if (ge->posOffsetInPixels[2] < 0.0f) {
			
			if (testMoveHit(ge,0,0,-1) == E_CD_SOLID) {
				
				onGround = abs(ge->vel[2]) < 2.0f*fPixelsPerCell;
				
				if (ge->vel[2] < 0.0f) {
					
					if (ge->vel[2] < -20.0*fPixelsPerCell) {
						hitGround = true;
					}
					
					ge->vel.multXYZ(1.0f,1.0f,-ge->bounciness);
					ge->posOffsetInPixels.setFZ(0.0f);
					ge->isFalling = false;
					ge->isJumping = false;
				}
				
			}
		}
		
		if (onGround) {
			ge->vel.multXYZ(ge->friction,ge->friction,1.0f);
		}
		
		
		if (ge->posOffsetInPixels[0] > 0.0f) {
			res = moveCell(ge,1,0,0);
			ge->positionInCells.copyFrom(&tempVec1);
			ge->updateBounds();
			
			if (!res) {
				ge->vel.multXYZ(-ge->bounciness,1.0f,1.0f);
				ge->posOffsetInPixels.setFX(0.0f);
			}
		}
		if (ge->posOffsetInPixels[0] < 0.0f) {
			res = moveCell(ge,-1,0,0);
			ge->positionInCells.copyFrom(&tempVec1);
			ge->updateBounds();
			
			if (!res) {
				ge->vel.multXYZ(-ge->bounciness,1.0f,1.0f);
				ge->posOffsetInPixels.setFX(0.0f);
			}
		}
		
		if (ge->posOffsetInPixels[1] > 0.0f) {
			res = moveCell(ge,0,1,0);
			ge->positionInCells.copyFrom(&tempVec1);
			ge->updateBounds();
			
			if (!res) {
				ge->vel.multXYZ(1.0f,-ge->bounciness,1.0f);
				ge->posOffsetInPixels.setFY(0.0f);
			}
		}
		if (ge->posOffsetInPixels[1] < 0.0f) {
			res = moveCell(ge,0,-1,0);
			ge->positionInCells.copyFrom(&tempVec1);
			ge->updateBounds();
			
			if (!res) {
				ge->vel.multXYZ(1.0f,-ge->bounciness,1.0f);
				ge->posOffsetInPixels.setFY(0.0f);
			}
		}
		
		
		
		ge->updateBounds();
		
		
		
		if (ge->posOffsetInPixels[0] >= fPixelsPerCell) {
			ge->posOffsetInPixels.addXYZ(-fPixelsPerCell,0.0f,0.0f);
			res = moveCell(ge,1,0,0);
		}
		if (ge->posOffsetInPixels[0] <= -fPixelsPerCell) {
			ge->posOffsetInPixels.addXYZ(fPixelsPerCell,0.0f,0.0f);
			res = moveCell(ge,-1,0,0);
		}
		
		if (ge->posOffsetInPixels[1] >= fPixelsPerCell) {
			ge->posOffsetInPixels.addXYZ(0.0f,-fPixelsPerCell,0.0f);
			res = moveCell(ge,0,1,0);
		}
		if (ge->posOffsetInPixels[1] <= -fPixelsPerCell) {
			ge->posOffsetInPixels.addXYZ(0.0f,fPixelsPerCell,0.0f);
			res = moveCell(ge,0,-1,0);
		}
		
		if (ge->posOffsetInPixels[2] >= fPixelsPerCell) {
			ge->posOffsetInPixels.addXYZ(0.0f,0.0f,-fPixelsPerCell);
			res = moveCell(ge,0,0,1);
		}
		if (ge->posOffsetInPixels[2] <= -fPixelsPerCell) {
			ge->posOffsetInPixels.addXYZ(0.0f,0.0f,fPixelsPerCell);
			res = moveCell(ge,0,0,-1);
			
			
		}
		
		ge->updateBounds();
		
		return hitGround;
	}
	

	void updatePhys() {
		int i;
		BaseObj* ge;
		
		FIVector4 modv;
		
		float smoothMod = 0.125f;
		
		modv.setFXYZ(smoothMod,smoothMod,smoothMod);
		
		bool hitGround;
		
		SphereStruct* ss;
		
		for(i = 0; i < visObjects.size(); i++) {
			ge = &(gameObjects[visObjects[i]]);
			
			modv.setFZ(smoothMod);
			
			hitGround = false;
			
			if (singleton->moveMode == E_MM_UNIT) {
				makeFloat(ge);
				makeFall(ge);
			}
			else {
				hitGround = makeMove(ge);
				if (ge->vel[2] != 0.0f) {
					modv.setFZ(1.0f);
				}
				
				if (hitGround) {
					singleton->playBump(ge);
					singleton->performCamShake(ge);
				}
			}
			
			ge->updatePixelBounds();
			
			ge->updateTargets(&modv);
			
			if (ge->isBullet) {
				if (ge->vel.length() < (2.0f*singleton->pixelsPerCell) ) {
					singleton->explodeBullet(ge);
				}
			}
			
		}
		
		for (i = 0; i < singleton->sphereStack.size(); i++) {
			ss = &(singleton->sphereStack[i]);
			
			ss->radVel += ss->radAcc*singleton->timeDelta;
			ss->curRad += ss->radVel*singleton->timeDelta;
			
			if (ss->curRad >= ss->maxRad) {
				ss->curRad = ss->maxRad;
				ss->radVel = 0.0f;
			}
		}
		
		for (i = 0; i < singleton->sphereStack.size(); i++) {
			ss = &(singleton->sphereStack[i]);
			if (ss->curRad <= 0.0) {
				singleton->sphereStack.erase(singleton->sphereStack.begin() + i);
			}
		}
		
		
		singleton->resultShake = -singleton->cameraShake*sin(singleton->shakeTimer.getElapsedTimeInMilliSec()/20.0f);
		
		singleton->resultCameraPos.copyFrom(singleton->cameraPos);
		singleton->resultCameraPos.addXYZ(0.0f,0.0f,singleton->resultShake*0.5f*singleton->pixelsPerCell);
		
		singleton->cameraShake += (0.0f - singleton->cameraShake)*singleton->timeDelta*8.0f;
		
	}

	void update() {
		
		singleton->updateLock = true;


		if (noiseGenerated) {

		}
		else {
			noiseGenerated = true;
			singleton->bindShader("NoiseShader");
			singleton->bindFBO("noiseFBO");
			singleton->drawFSQuad();
			singleton->unbindFBO();
			singleton->unbindShader();
		}

		
		
		camBlockPos.copyFrom( singleton->cameraPos );
		camBlockPos.intDivXYZ(singleton->blockSizeInPixels);

		if (singleton->currentActor == NULL) {
			camHolderPos.copyFrom( singleton->cameraPos );
			camHolderPos.intDivXYZ(singleton->holderSizeInPixels);
			camHolderPos.addXYZRef(&(singleton->lookAtVec),4.0);
		}
		else {
			camHolderPos.copyFrom(&(singleton->currentActor->centerPointInPixels));
			camHolderPos.intDivXYZ(singleton->holderSizeInPixels);
		}

		
		
		loadNearestHolders();
		
		
		
		
		
		drawPrim();
		singleton->perspectiveOn = true;
		glEnable(GL_DEPTH_TEST);
		renderGeom();
		glDisable(GL_DEPTH_TEST);
		singleton->perspectiveOn = false;
		
		
		postProcess();
		drawMap();

		glutSwapBuffers();
		//glFlush();
		
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
		int ii;
		int jj;
		int blockRad = 1;
		
		for (j = -blockRad; j <= blockRad; j++)
		{
			for (i = -blockRad; i <= blockRad; i++)
			{
				ii = i + camBlockPos.getIX();
				jj = j + camBlockPos.getIY();

				getBlockAtCoords(ii, jj, true);

			}
		}
	}
	





	void loadNearestHolders() {
		
		
		int i, j, k;
		int ii, jj, kk;
		int incVal;
		int maxLoadRad = 5;
		int genCount = 0;
		int mink;
		int maxk;
		int minj;
		int maxj;
		int mini;
		int maxi;
		int curLoadRadius;
		int radStep = 1;
		intPair curId;
		
		
		
		tempVec.copyFrom(singleton->cameraPos);
		tempVec.intDivXYZ(singleton->holderSizeInPixels);

		GamePageHolder* curHolder;
		GameBlock *curBlock;

		ensureBlocks();
		
		//todo: get rid of this
		return;
		
		
		for (curLoadRadius = 0; curLoadRadius < maxLoadRad; curLoadRadius++) {
			
			mink = max(tempVec.getIZ() - curLoadRadius,0);
			maxk = min(tempVec.getIZ() + curLoadRadius,worldSizeInHolders.getIZ()-1);
			minj = tempVec.getIY() - curLoadRadius;
			maxj = tempVec.getIY() + curLoadRadius;
			mini = tempVec.getIX() - curLoadRadius;
			maxi = tempVec.getIX() + curLoadRadius;
			
			for (jj = minj; jj <= maxj; jj += radStep) {
				
				if (curLoadRadius <= 2) {
					incVal = 1;
				}
				else {
					if ( (jj == minj) || (jj == maxj) ) {
						incVal = radStep;
					}
					else {
						incVal = maxi - mini;
					}
				}
				
				for (ii = maxi; ii >= mini; ii -= incVal) {
					
					
					for (kk = mink; kk <= maxk; kk += radStep) {
						
						
						curHolder = getHolderAtCoords(ii, jj, kk, true);
						curBlock = getBlockAtId(curHolder->blockId);
						
						if (curBlock == NULL) {
							cout << "NULL BLOCK\n";
						}
						else {
							if (curHolder->wasGenerated) {
								
							}
							else {
								curHolder->genCellData();
								genCount++;
							}
						}
						
						
						if (genCount >= 2) {
							return;
						}
						
					}
					
				}
			}
			
		}
		
		
	}












	void findNearestEnt(
		EntSelection* entSelection,
		int entType,
		int maxLoadRad,
		int radStep,
		FIVector4 *testPoint,
		bool onlyInteractive = false,
		bool ignoreDistance = false
	) {
		
		GameEnt* myEnt;
		
		int curInd = 0;
		float bestDis;
		float curDis;
		
		bool doProc = false;
		
		entSelection->selEntList.clear();
		entSelection->selEntMap.clear();
		entSelection->selEntListInd = 0;

		bestDis = 99999.0f;
		
		
		
		
		//////////////////////
		
		
		
		int i, j, k;
		int ii, jj, kk;
		int incVal;
		
		int tot = 0;


		int mink;
		int maxk;
		int minj;
		int maxj;
		int mini;
		int maxi;
		int curLoadRadius;
		intPair curId;
		
		tempVec.copyFrom(testPoint);
		tempVec.intDivXYZ(singleton->holderSizeInPixels);


		GamePageHolder* curHolder;
		GameBlock *curBlock;


		ensureBlocks();
		
		
		

		for (curLoadRadius = 0; curLoadRadius < maxLoadRad; curLoadRadius++) {
			
			mink = max(tempVec.getIZ() - curLoadRadius,0);
			maxk = min(tempVec.getIZ() + curLoadRadius,worldSizeInHolders.getIZ()-1);
			minj = tempVec.getIY() - curLoadRadius;
			maxj = tempVec.getIY() + curLoadRadius;
			mini = tempVec.getIX() - curLoadRadius;
			maxi = tempVec.getIX() + curLoadRadius;
			
			for (jj = minj; jj <= maxj; jj += radStep) {
				
				if (curLoadRadius <= 2) {
					incVal = 1;
				}
				else {
					if ( (jj == minj) || (jj == maxj) ) {
						incVal = radStep;
					}
					else {
						incVal = maxi - mini;
					}
				}
				
				for (ii = maxi; ii >= mini; ii -= incVal) {
					
					
					for (kk = mink; kk <= maxk; kk += radStep) {
						
						
						
						curHolder = getHolderAtCoords(ii, jj, kk, true);
						curBlock = getBlockAtId(curHolder->blockId);
						
						if (curBlock == NULL) {
							cout << "NULL BLOCK\n";
						}
						else {
							
							for (k = 0; k < curHolder->containsEntIds[entType].data.size(); k++) { //curBlock->gameEnts[entType].data.size()
								
								curId = curHolder->containsEntIds[entType].data[k];
								myEnt = &(blockData[curId.v0]->gameEnts[entType].data[curId.v1]);
								
								
								
								if (
									ignoreDistance||
									testPoint->inBoundsXYZSlack(
										myEnt->getVisMinInPixelsT(),
										myEnt->getVisMaxInPixelsT(),
										0.0625*singleton->pixelsPerCell
									)
								)
								{
									
									if (entSelection->selEntMap.count(curId) == 0 ) {
										
										entSelection->selEntList.push_back(myEnt);
										entSelection->selEntMap[curId] = 1;
										
										
										
										curDis = 
										abs(myEnt->getVisMaxInPixelsT()->getFX()-testPoint->getFX()) +
										abs(myEnt->getVisMaxInPixelsT()->getFY()-testPoint->getFY()) +
										abs(myEnt->getVisMaxInPixelsT()->getFZ()-testPoint->getFZ());
										
										//myEnt->getVisMinInPixelsT()->distance(testPoint) +
										//myEnt->getVisMaxInPixelsT()->distance(testPoint);

										if (myEnt->visible) {
											
										}
										else {
											curDis *= 16.0f;
										}

										if (onlyInteractive) {
											doProc = singleton->isInteractiveEnt[myEnt->buildingType];
										}
										else {
											doProc = true;
										}

										if ((curDis < bestDis)&&doProc) {
											bestDis = curDis;
											entSelection->selEntListInd = curInd;
										}
										
										
										curInd++;
										
									}
									
									

								}
							}
							
							
						}
						
						
						
					}
					
				}
			}
			
		}
		
		
		
		
		//////////////////////
		
		
		//return resEnt;
	}


	void drawPrim() {
		int i;
		
		singleton->bindShader("PrimShader");
		
		//if (k == 0) {
			singleton->bindFBO("pagesAndWaterTargFBO");
		// }
		// else {
		// 	singleton->bindFBO("waterTargFBO");
		// }
		
		for (i = 0; i < E_PL_LENGTH; i++) {
			singleton->setShaderTexture3D(i, singleton->volIdPrim[i]);
		}
		singleton->setShaderTBO(
			E_PL_LENGTH,
			singleton->tboWrapper.tbo_tex,
			singleton->tboWrapper.tbo_buf
		);
		
		singleton->sampleFBO("hmFBOLinear",2);
		
		if ((singleton->currentActor == NULL)||singleton->firstPerson) {
			singleton->setShaderFloat("thirdPerson", 0.0f);
			
			// if (singleton->firstPerson) {
			// 	singleton->setShaderFloat("CAM_BOX_SIZE", 0.5f);
			// }
			// else {
			// 	singleton->setShaderFloat("CAM_BOX_SIZE", 2.0f);
			// }
			
			singleton->setShaderFloat("CAM_BOX_SIZE", 0.5f);
			
			
		}
		else {
			singleton->setShaderFloat("thirdPerson", 1.0f);
			singleton->setShaderFloat("CAM_BOX_SIZE", 8.0f);
			singleton->setShaderfVec3("entPos", &(singleton->currentActor->centerPointInPixels));
		}
		
		singleton->setShaderFloat("isUnderWater", -(singleton->getUnderWater()-0.5f)*2.0f );
		
		singleton->setShaderMatrix4x4("modelview",singleton->viewMatrix.get(),1);
		singleton->setShaderMatrix4x4("proj",singleton->projMatrix.get(),1);
		singleton->setShaderMatrix4x4("modelviewInverse",singleton->viewMatrixDI,1);
		singleton->setShaderInt("testOn", (int)(singleton->testOn));
		singleton->setShaderFloat("pixelsPerCell", singleton->pixelsPerCell);
		singleton->setShaderFloat("UNIT_MAX", FLUID_UNIT_MAX + 1);
		singleton->setShaderFloat("waterLerp", singleton->waterLerp);
		singleton->setShaderFloat("volSizePrim", singleton->volSizePrim);
		singleton->setShaderFloat("curTime", singleton->pauseTime/1000.0f);
		singleton->setShaderfVec2("bufferDim", &(singleton->bufferRenderDim) );
		singleton->setShaderfVec3("cameraPos", singleton->cameraPos);
		singleton->setShaderfVec3("lookAtVec", &(singleton->lookAtVec) );
		singleton->setShaderfVec3("lightVec", &(singleton->lightVec) );
		singleton->setShaderfVec3("volMinInPixels", &(singleton->volMinInPixels) );
		singleton->setShaderfVec3("volMaxInPixels", &(singleton->volMaxInPixels) );
		singleton->setShaderfVec3("worldSizeInCells", &(singleton->worldSizeInCells) );
		
		singleton->setShaderVec2("clipDist",singleton->clipDist[0],singleton->clipDist[1]);
		singleton->setShaderFloat("FOV", singleton->FOV*M_PI/180.0f);
		//singleton->setShaderFloat("geomOn", singleton->frameCount % 2);
		
		
		//paramFetch1: x,y,z: position; w: template number
		//paramFetch2: x:diagMask, y:uid, z:?, w:?
		
		
		
		tempVec1.copyFrom(&(singleton->geomPoints[0]));
		tempVec1.addXYZ(0.0f,0.0f,singleton->geomOrigOffset);
		
		tempVec2.setFXYZW(0.0f,-99.0f,0.0f,0.0f);
		
		singleton->setShaderfVec4("paramFetch1", &tempVec1 );
		singleton->setShaderfVec4("paramFetch2", &tempVec2 );
		singleton->setShaderArrayfVec4("paramArrGeom", singleton->paramArrGeom, E_PRIMTEMP_LENGTH);
		
		
		if (singleton->currentActor != NULL) {
			
			singleton->splashArr[0] = singleton->currentActor->centerPointInPixels[0];
			singleton->splashArr[1] = singleton->currentActor->centerPointInPixels[1];
			singleton->splashArr[2] = singleton->currentActor->centerPointInPixels[2];
			singleton->splashArr[3] = singleton->currentActor->vel.length();
			
			singleton->setShaderInt("numSplashes", 1);
			singleton->setShaderArrayfVec4("splashArr", singleton->splashArr, MAX_SPLASHES);
		}
		else {
			singleton->setShaderFloat("numSplashes", 0);
		}
		
		
		
		
		singleton->fsQuad.draw();
		//singleton->drawFSQuad();
		
		singleton->unsampleFBO("hmFBOLinear",2);
		
		singleton->setShaderTBO(E_PL_LENGTH,0,0);
		for (i = 0; i < E_PL_LENGTH; i++) {
			singleton->setShaderTexture3D(i, 0);
		}
		
		singleton->unbindFBO();
		singleton->unbindShader();
		
		int curFBO = 0;
		
		//singleton->copyFBO2("pagesAndWaterTargFBO","pagesTargFBO", 0, 1);
		
		singleton->copyFBO2("pagesAndWaterTargFBO","waterTargFBO", 2, 3);
		
		//if (singleton->medianCount == 0) {
			singleton->copyFBO2("pagesAndWaterTargFBO","pagesTargFBO", 0, 1);
		// }
		// else {
		// 	singleton->copyFBO2("pagesAndWaterTargFBO","swapTargFBO0", 0, 1);
			
		// 	for (i = 0; i < singleton->medianCount; i++) {
		// 		singleton->bindShader("MedianShader");
		// 		singleton->bindFBO("swapTargFBO",curFBO);
		// 		singleton->sampleFBO("swapTargFBO", 0, curFBO);
		// 		singleton->setShaderfVec2("bufferDim", &(singleton->bufferModDim) );
				
		// 		singleton->drawFSQuad();
				
		// 		singleton->unsampleFBO("swapTargFBO", 0, curFBO);
		// 		singleton->unbindFBO();
		// 		singleton->unbindShader();
				
		// 		curFBO = 1-curFBO;
		// 	}
			
		// 	if (curFBO == 0) {
		// 		singleton->copyFBO2("swapTargFBO0","pagesTargFBO", 0, 1);
		// 	}
		// 	else {
		// 		singleton->copyFBO2("swapTargFBO1","pagesTargFBO", 0, 1);
		// 	}
			
		// }
		
		
		
		
		
		
		
		
		
		
		
	}

	
	


	void drawOrg(GameOrg* curOrg, bool drawAll) {
		
		
		float scale = 1.0f*((float)(singleton->pixelsPerCell));
		
		
		glLineWidth(0);
		
		
		
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
		
		
		bool doProc = false;
		
		if (drawAll) {
			doProc = true;
		}
		else {
			if (curNode == singleton->selectedNode) {
				doProc = true;
			}
		}
		
		int i;
		
		
		if (doProc) {
			lineSeg[0].setFXYZRef(&(curNode->orgTrans[0]));
			lineSeg[0].multXYZ(  scale  );
			
			if (drawAll) {
				lineSeg[1].setFXYZRef(&(curNode->tbnTrans[drawMode%3]));
				lineSeg[1].multXYZ(  scale  );
			}
			else {
				lineSeg[1].setFXYZRef(&(curNode->tbnRotC[drawMode%3]));
				lineSeg[1].multXYZ(  (curNode->orgVecs[E_OV_TBNRAD0][drawMode%3]*scale*16.0f)  );
				//lineSeg[1].multXYZ(&(curNode->tbnRadScale0));
				lineSeg[1].addXYZRef(&(lineSeg[0]));
			}
			
			
			lineSeg[0].addXYZRef(basePosition);
			lineSeg[1].addXYZRef(basePosition);
			
			
			
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














	void clearVisitedPaths(PathHolder* pathHolder) {
		while (pathHolder->visitedList.size() > 0) {
			blockData[
				pathHolder->visitedList.back().blockId
			]->buildingData[
				pathHolder->visitedList.back().index
			].visited = 0;
			pathHolder->visitedList.pop_back();
		}
	}

	
	
	void clearPathList(PathHolder* pathHolder) {
		pathHolder->pathList.clear();
	}
	
	float getIdealPathLength(PathNode* blockAndIndex) {
		return 		
			abs(blockAndIndex[0].x-blockAndIndex[1].x) +
			abs(blockAndIndex[0].y-blockAndIndex[1].y) +
			abs(blockAndIndex[0].z-blockAndIndex[1].z);
	}

	int findAIPathRBT(
		PathHolder* pathHolder,
		PathNode* blockAndIndex,
		float _pathSlack
	) {


		//int begInd, int endInd

		float pathSlack = _pathSlack;

		int i;
		//int rbInd = 0;
		PathNode curInd;

		int testX = 0;
		int testY = 0;
		int testZ = 0;
		
		int wrappedX;
		int wrappedY;
		int wrappedZ;

		int curX = 0;
		int curY = 0;
		int curZ = 0;
		
		float idealPathLength = getIdealPathLength(blockAndIndex);
		
		int bestBlockId;
		int bestX;
		int bestY;
		int bestZ;
		int bestInd = 0;

		int testInd = 0;
		int curDir = 0;
		

		int endZ = blockAndIndex[1].z;
		int endY = blockAndIndex[1].y;
		int endX = blockAndIndex[1].x;
		
		int terDataVisPitchXY = singleton->terDataVisPitchXY;
		
		int conType;
		
		int pathCost;
		int bestCost;
		int invalidCost = 1<<30;

		int maxPops;

		PathNode tempNode;

		
		
		
		int numPathSteps = 0;
		
		
		
		//cout << "\n\n\nSTART COORD " << blockAndIndex[0].x << " " << blockAndIndex[0].y << " " << blockAndIndex[0].z << "\n";
		//cout << "END COORD " << blockAndIndex[1].x << " " << blockAndIndex[1].y << " " << blockAndIndex[1].z << "\n";
		

		BuildingNode* curBuildingData;
		GameBlock* testBlock;

		while ( (pathHolder->pathList.size() > 0) ) { // && (numPathSteps < 10)
			
			numPathSteps++;
			bestInd = -1;
			bestCost = invalidCost;

			curInd = pathHolder->pathList.back();
			
			
			
			if (curInd == blockAndIndex[1]) {
				goto DONE_FINDING_PATH;
			}
			
			//cout << "curBlockId " << curInd.blockId << "\n";
			
			curBuildingData = &(blockData[curInd.blockId]->buildingData[curInd.index]); 
			curBuildingData->visited = 1;
			pathHolder->visitedList.push_back(curInd);
			
			
			if (pathHolder->pathList.size() > idealPathLength*pathSlack) {
				cout << "Exceeded Path Slack\n";
				return false;
			}
				
			// 	maxPops = pathHolder->pathList.size() / 2;
			// 	pathSlack *= 1.1f;
				
			// 	if (pathSlack > 5.0f) {
			// 		cout << "Exceeded max path length\n";
			// 		return false;
			// 	}
				
			// 	for (i = 0; i < maxPops; i++) {
					
			// 		if (i < maxPops/2) {
			// 			blockData[
			// 				pathHolder->pathList.back().blockId
			// 			]->buildingData[
			// 				pathHolder->pathList.back().index
			// 			].visited = 0;
			// 		}
					
					
			// 		pathHolder->pathList.pop_back();
			// 	}
				
			// 	goto SKIP_PATH;
			
			

			curZ = curInd.z;
			curY = curInd.y;
			curX = curInd.x;

			
			for (curDir = 0; curDir < 4; curDir++) {


				testX = curX + dirModX[curDir];
				testY = curY + dirModY[curDir];
				testZ = curZ + dirModZ[curDir] + curBuildingData->con[curDir].heightDelta;
				
				
				wrappedX = (wrapCoord(testX,terDataVisPitchXY*worldSizeInBlocks.getIX())%terDataVisPitchXY)+singleton->terDataBufAmount;
				wrappedY = (wrapCoord(testY,terDataVisPitchXY*worldSizeInBlocks.getIY())%terDataVisPitchXY)+singleton->terDataBufAmount;
				wrappedZ = testZ+singleton->terDataBufAmount;
				
				testBlock = getBlockAtCoords(
					intDiv(testX,terDataVisPitchXY),
					intDiv(testY,terDataVisPitchXY),
					true
				);
				
				
				if (testBlock != NULL) {
					testInd = testBlock->getNodeIndex(wrappedX, wrappedY, wrappedZ, 0);

					if (testInd >= 0) {

						conType = curBuildingData->con[curDir].conType;
						
						//cout << "conType " << conType << " visited " << testBlock->buildingData[testInd].visited << "\n";
						
						if (
							( testBlock->isTraversable[conType] ) &&
							( testBlock->buildingData[testInd].visited == 0 )
						) {
							
							
							pathCost = abs(testX-endX) + abs(testY-endY) + abs(testZ-endZ);

							//cout << "pathCost " << pathCost << " bestCost " << bestCost << "\n";

							if (pathCost < bestCost) {
								bestInd = testInd;
								bestBlockId = testBlock->blockId;
								bestCost = pathCost;
								bestX = testX;
								bestY = testY;
								bestZ = testZ;
							}
						}
					}
				}
				
				
			}


			
			if (bestInd == -1) { // dead end, back up
				pathHolder->pathList.pop_back();
				
				//cout << "pop_back\n";
				//cout << "new length: " << pathHolder->pathList.size() << "\n\n";
			} else {
				tempNode.index = bestInd;
				tempNode.blockId = bestBlockId;
				tempNode.x = bestX;
				tempNode.y = bestY;
				tempNode.z = bestZ;
				pathHolder->pathList.push_back(tempNode);
				
				//cout << bestInd << " " << bestBlockId << " " << bestX << " " << bestY << " " << bestZ << "\n\n";
				
			}

SKIP_PATH:
			;
			
			

		}
		
DONE_FINDING_PATH:
		;
		
		return pathHolder->pathList.size() > 0;

	}


	void drawPathLine(PathHolder* curPath, int r, int g, int b, float lw, float zoff) {
		// IMPORTANT: size is a uint, do not let underflow happen!
		int totSize = curPath->pathList.size();
		totSize -= 1;
		int i;
		
		glLineWidth(lw);
		
		singleton->setShaderVec3("matVal", r,g,b);
		for (i = 0; i < totSize; i++) {
			blockData[curPath->pathList[i].blockId]->nodeIndexToWorldSpaceInPixels(curPath->pathList[i].index, &(lineSeg[0]));
			blockData[curPath->pathList[i+1].blockId]->nodeIndexToWorldSpaceInPixels(curPath->pathList[i+1].index, &(lineSeg[1]));
			lineSeg[0].addXYZ(0.0f,0.0f,zoff);
			lineSeg[1].addXYZ(0.0f,0.0f,zoff);
			singleton->drawLine(&(lineSeg[0]),&(lineSeg[1]));
		}
	}

	void drawAIPath(PathHolder* pathHolder, PathHolder* splitPathHolder) {
		
		
		
		
		int i;
		int curStep = singleton->currentStep;
		int curInd;
		int totSize;
		bool pathResult;
		bool doCont;
		
		float maxSlack = 8.0f;
		
		int numDivisions;
		int splitSize = 100;
		
		//int terDataBufPitchXY2 = singleton->terDataBufPitchXY*singleton->terDataBufPitchXY;
		//int endSegInd;
		
		// 0 - nothing
		// 1 - first marker placed
		// 2 - second marker placed
		// 3 - clear markers
		
		
		singleton->setShaderFloat("isWire", 0.0f);
		for (i = 0; i < curStep; i++) {
			
			blockPos[i].copyFrom(&(singleton->moveNodes[i]));
			blockPos[i].intDivXYZ(singleton->blockSizeInPixels);
			blockRef[i] = getBlockAtCoords(blockPos[i].getIX(), blockPos[i].getIY(), true);
			nodeInd[i] = blockRef[i]->findNearestNode(
				&(singleton->moveNodes[i]),
				&(nodePos[i]),
				&(nodePosInPixels[i])
			);
			blockAndIndexPath[i].index = nodeInd[i];
			blockAndIndexPath[i].blockId = blockRef[i]->blockId;
			blockAndIndexPath[i].x = nodePos[i].getIX();
			blockAndIndexPath[i].y = nodePos[i].getIY();
			blockAndIndexPath[i].z = nodePos[i].getIZ();
			
			singleton->setShaderVec3("matVal", i*255, (1-i)*255, 0);
			singleton->drawCubeCentered(
				&(nodePosInPixels[i]),
				1.0f * singleton->pixelsPerCell
			);
		}
		
		
		
		if (curStep == 2) {
			
			if (!foundPath) {
				
				finalPath = NULL;
				foundPath = true;
				
				if (
					//(blockRef[0] == blockRef[1]) &&
					(nodeInd[0] != -1) &&
					(nodeInd[1] != -1)
				) {
					
					clearVisitedPaths( pathHolder );
					clearPathList( pathHolder );
					clearVisitedPaths( splitPathHolder );
					clearPathList( splitPathHolder );
					
					
					pathHolder->pathList.push_back(blockAndIndexPath[0]);
					
					pathResult = findAIPathRBT(
						pathHolder,
						blockAndIndexPath,
						maxSlack
					);
					
					clearVisitedPaths( pathHolder );
					if (pathResult) {
						finalPath = pathHolder;
					}
					
					
					
					// if (pathResult) {	
					// 	numDivisions = pathHolder->pathList.size()/splitSize;
					// 	if (
					// 		(numDivisions >= 2) &&
					// 		(pathHolder->pathList.size() > getIdealPathLength(blockAndIndexPath)*1.5f)
					// 	) {
							
					// 		cout << "splitting path " << numDivisions << " times\n";
					// 		cout << "total path size is " << pathHolder->pathList.size() << "\n";
							
							
					// 		splitPathHolder->pathList.push_back(blockAndIndexPath[0]);
							
							
					// 		i = 0;
					// 		doCont = true;
					// 		while (doCont) {
					// 			blockAndIndexSplitPath[0] = pathHolder->pathList[i*splitSize];
								
								
					// 			cout << "split number " << i << "\n";
								
					// 			if ( (i+1)*splitSize >= pathHolder->pathList.size() ) {
					// 				blockAndIndexSplitPath[1] = pathHolder->pathList.back();
					// 				doCont = false;
					// 			}
					// 			else {
					// 				blockAndIndexSplitPath[1] = pathHolder->pathList[(i+1)*splitSize];
					// 			}
								
									
					// 			pathResult = findAIPathRBT(
					// 				splitPathHolder,
					// 				blockAndIndexSplitPath,
					// 				maxSlack
					// 			);
								
					// 			if (pathResult) {
									
					// 			}
					// 			else {
					// 				doCont = false;
					// 				cout << "Failed to find path at split #" << i << "\n";
					// 			}
								
					// 			i++;
								
					// 		}
							
					// 		finalPath = splitPathHolder;
							
					// 	}
						
					// }
					
				
					cout << "DONE FINDING PATH\n";
					
					//finalInd = blockRef[0]->findPath(nodeInd[0],nodeInd[1]);
					//doTraceND("finalInd ", i__s(finalInd), " nodeInd ", i__s(nodeInd[0]), " ", i__s(nodeInd[1]));
				}
				else {
					// doTraceND("Different Blocks");
				}
				
			}
			
		}
		else {
			foundPath = false;
			//finalInd = -1;
		}
		
		
		if ( (finalPath != NULL) && foundPath) {
			drawPathLine(pathHolder, 255, 255, 0, 2.0f, 0.0f);
			drawPathLine(splitPathHolder, 255, 0, 0, 2.0f, 4.0f*((float)singleton->pixelsPerCell));
		}
		
		
		
		
	}


	void makeFloat(BaseObj *ge) {
		
		bool doProc = false;
		
		if (ge == NULL) {
			return;
		}
		
		// make it float
		int res = testHit(ge);
		if (res == E_CD_SOLID) {
			
			if (lastHitObjUID == 0) {
				doProc = true;
			}
			else {
				doProc = (ge->uid > lastHitObjUID);
			}
			
			if (doProc) {
				ge->positionInCells.addXYZ(0,0,1);
				ge->updateBounds();
				//ge->isFalling = true;
			}
			else {
				//ge->isFalling = false;
			}
		}
	}
	
	void makeFall(BaseObj *ge, bool justTesting = false) {
		
		bool doProc = false;
		
		if (ge == NULL) {
			return;
		}
		
		
		//make it fall
		ge->positionInCells.addXYZ(0,0,-1);
		ge->updateBounds();
		
		int res = testHit(ge);
		
		if ( (res == E_CD_EMPTY) || (res == E_CD_WATER) ) {
			// let it fall
			
			ge->isFalling = true;
		}
		else {
			// correct the fall
			
			ge->isFalling = false;
			
			doProc = true;
			
		}
		
		if (doProc || justTesting) {
			ge->positionInCells.addXYZ(0,0,1);
			ge->updateBounds();
		}
		
	}
	
	
	
	bool rotateCell(
		BaseObj *ge,
		int dir,
		int axis
	) {
		
		if (ge == NULL) {
			return false;
		}
		
		// if (ge->isFalling) {
		// 	return false;
		// }
		
		bool moveSuccessful = false;
		bool moveSuc = false;
		
		ge->rotate(dir,axis);
		ge->updateBounds();
		
		int res = testHit(ge);
		int res2 = E_CD_EMPTY;
		int res3 = E_CD_EMPTY;
		
		if (res == E_CD_SOLID) {
			
			ge->positionInCells.addXYZ(0,0,1);
			ge->updateBounds();
			res2 = testHit(ge);
			
			if (res2 == E_CD_SOLID) {
				
				ge->positionInCells.addXYZ(0,0,-1);
				ge->rotate(1-dir,axis);
				ge->updateBounds();
				
				//singleton->playBump(ge);
				
			}
			else {
				moveSuccessful = true;
			}
			
		}
		else {
			moveSuccessful = true;
		}
		
		return moveSuccessful;
		
	}

	bool moveCell(
		BaseObj *ge,
		int x,
		int y,
		int z
	) {
		
		if (ge == NULL) {
			return false;
		}
		
		// if (ge->isFalling) {
		// 	return false;
		// }
		
		
		
		
		
		bool moveSuccessful = false;
		bool moveSuc = false;
		bool notBullet = !(ge->isBullet);
		
		ge->positionInCells.addXYZ(x,y,z);
		ge->updateBounds();
		
		int res = testHit(ge);
		int res2 = E_CD_EMPTY;
		int res3 = E_CD_EMPTY;
		
		if (res == E_CD_SOLID) {
			
			if (notBullet) { ge->positionInCells.addXYZ(0,0,1); }
			ge->updateBounds();
			res2 = testHit(ge);
			
			//res2 = testMoveHit(ge,0,0,1);
			
			if (res2 == E_CD_SOLID) {
				
				if (notBullet) { ge->positionInCells.addXYZ(0,0,-1); }
				ge->positionInCells.addXYZ(-x,-y,-z);
				ge->updateBounds();
				
				if (lastHitObjUID != 0) {
					moveSuc = moveCell(&(gameObjects[lastHitObjUID]),x,y,z);
				}
				
				if (moveSuc == false) {
					//singleton->playBump(ge);
				}
				else {
					moveSuccessful = moveCell(ge,x,y,z);
				}
				
			}
			else {
				moveSuccessful = true;
			}
			
		}
		else {
			moveSuccessful = true;
		}
		
		if (notBullet) {
			if (ge->vel[2] == 0.0f) {
				res = testMoveHit(ge,0,0,-1);
				if (res != E_CD_SOLID) {
						res2 = testMoveHit(ge,0,0,-2);
						if (res2 == E_CD_SOLID) {
								ge->positionInCells.addXYZ(0,0,-1);
								ge->updateBounds();
						}
				}
			}
		}
		
		
		
		return moveSuccessful;
		
	}
	
	bool removeVisObject(BaseObjType _uid) {
		int i;
		
		for (i = 0; i < visObjects.size(); i++) {
			if (visObjects[i] == _uid) {
				visObjects.erase(visObjects.begin() + i);
				return true;
			}
		}
		
		return false;
	}
	
	
	void moveCellRotated(BaseObj *ge, int dirMod) {
		
		if (ge == NULL) {
			return;
		}
		
		int yOrient = ge->orientationXYZ.getIY();
		
		moveCell(
			ge,
			DIR_VECS[yOrient][0]*dirMod,
			DIR_VECS[yOrient][1]*dirMod,
			DIR_VECS[yOrient][2]*dirMod
		);
		
	}
	
	


	int testHit(BaseObj *ge) {
		lastHitObjUID = 0;
		if (ge == NULL) {
			return E_CD_EMPTY;
		}
		
		int cellVal;
		int xmax,ymax,zmax;
		int xmin,ymin,zmin;
		
		int i, j, k;
		
		int tempVal = E_CD_EMPTY;
		
		bool hitObj = false;
		
		BaseObj* testObj;
		
		if (ge->isBullet) {
			
		}
		else {
			for(i = 0; i < visObjects.size(); i++) {
				
				if (visObjects[i] == ge->uid) {
					// do not test hit against self
				}
				else {
					
					testObj = &(gameObjects[visObjects[i]]);
					
					hitObj = FIVector4::intersectInt(
						&(testObj->boundsMinTransInCells),
						&(testObj->boundsMaxTransInCells),
						&(ge->boundsMinTransInCells),
						&(ge->boundsMaxTransInCells)
					);
					
					if (hitObj) {
						lastHitObjUID = testObj->uid;
						return E_CD_SOLID;
					}
						
				}
			}
		}
		
		
		
		
		
		xmin = ge->boundsMinTransInCells.getIX();
		ymin = ge->boundsMinTransInCells.getIY();
		zmin = ge->boundsMinTransInCells.getIZ();
		
		xmax = ge->boundsMaxTransInCells.getIX();
		ymax = ge->boundsMaxTransInCells.getIY();
		zmax = ge->boundsMaxTransInCells.getIZ();
		
		for (k = zmin; k < zmax; k++) {
			for (j = ymin; j < ymax; j++) {
				for (i = xmin; i < xmax; i++) {
					
					cellVal = getCellAtCoords(i,j,k);
					
					if (cellVal == E_CD_SOLID) {
						return cellVal;
					}
					
					if (cellVal == E_CD_WATER) {
						tempVal = E_CD_WATER;
					}
					
				}	
			}
		}
		
		return tempVal;
		
		
	}

	void renderGeom()
	{

		int i;
		int j;
		int k;
		int n;
		
		float frameMod;
		int objCount = 0;
		
		int curOr;
		bool doProc;
		int cellVal;
		int xmax,ymax,zmax;
		int xmin,ymin,zmin;
		
		CharStruct* tempCS;
		
		BaseObj* curObj;
		
		bool showHit = false;
		float dirVecLength = 4.0f*singleton->pixelsPerCell;
		
		float x1 = 0.0f;
		float x2 = 0.0f;
		float y1 = 0.0f;
		float y2 = 0.0f;
		float iw = singleton->fontWrappers[EFW_ICONS]->fontImage->width;
		float ih = singleton->fontWrappers[EFW_ICONS]->fontImage->height;


		singleton->bindShader("GeomShader");
		singleton->setShaderFloat("objectId",0.0);
		singleton->setShaderfVec3("cameraPos", singleton->cameraPos);
		singleton->setShaderFloat("isWire", 0.0);
		singleton->setShaderFloat("clipDist",singleton->clipDist[1]);
		singleton->setShaderfVec3("offsetPos",&(singleton->origin));
		
		singleton->bindFBO("geomBaseTargFBO");
		
		
		glLineWidth(4.0f);
		
		
		singleton->setShaderVec3("matVal", 30, 30, 30);
		
		for(i = 0; i < visObjects.size(); i++) {
			
			curObj = &(gameObjects[visObjects[i]]);
			
			objCount++;
			
							
			singleton->setShaderFloat("objectId",visObjects[i]);
			
			
			tempVec1.copyFrom( &(curObj->boundsMinTransInPixels) );
			tempVec2.copyFrom( &(curObj->boundsMaxTransInPixels) );
			
			
			// if (visObjects[i] == singleton->actObjInd) {
			// 	singleton->vecToAngle(&(singleton->lookAtVec),&tempVec4);
			// 	curObj->ang = tempVec4.getFX();
			// }
			
			tempVec3.copyFrom(&(curObj->centerPointInPixels));
			tempVec3.setFW(curObj->ang);
			
			singleton->setShaderfVec4("rotationZ",&tempVec3);
			
			
			if (visObjects[i] == singleton->actObjInd) {
				
				if (!singleton->firstPerson) {
					
					singleton->drawBox(
						&tempVec1,
						&tempVec2
					);
					
					singleton->setShaderFloat("objectId",0);
					
					tempVec1.copyFrom( &(curObj->centerPointInPixels) );
					
					
					curOr = curObj->orientationXYZ.getIX();
					tempVec2.setFXYZRef( &(singleton->dirVecs[curOr]) );
					tempVec2.multXYZ(singleton->pixelsPerCell*4.0f);
					tempVec2.addXYZRef(&tempVec1);
					singleton->setShaderVec3("matVal", 255, 0, 0);
					singleton->drawLine(&tempVec1,&tempVec2);
					
					curOr = curObj->orientationXYZ.getIY();
					tempVec2.setFXYZRef( &(singleton->dirVecs[curOr]) );
					tempVec2.multXYZ(singleton->pixelsPerCell*4.0f);
					tempVec2.addXYZRef(&tempVec1);
					singleton->setShaderVec3("matVal", 0, 255, 0);
					singleton->drawLine(&tempVec1,&tempVec2);
					
					curOr = curObj->orientationXYZ.getIZ();
					tempVec2.setFXYZRef( &(singleton->dirVecs[curOr]) );
					tempVec2.multXYZ(singleton->pixelsPerCell*4.0f);
					tempVec2.addXYZRef(&tempVec1);
					singleton->setShaderVec3("matVal", 0, 0, 255);
					singleton->drawLine(&tempVec1,&tempVec2);
					
					
					singleton->setShaderVec3("matVal", 30, 30, 30);
				}
				
				
				
			}
			else {
				singleton->drawBox(
					&tempVec1,
					&tempVec2
				);
			}
							
						
			
			
			
			
		}
		
		tempVec3.setFXYZW(0.0f,0.0f,0.0f,0.0f);
		singleton->setShaderfVec4("rotationZ",&tempVec3);
		singleton->setShaderFloat("objectId",0.0);
		glLineWidth(0);

		//cout << "objectCount " << objCount << "\n";
		singleton->lastObjectCount = objCount;		

		switch (singleton->mouseState)
		{

		case E_MOUSE_STATE_MOVE:
			
			// singleton->setShaderFloat("isWire", 1.0);
			// singleton->setShaderVec3("matVal", 255, 0, 0);
			// singleton->drawBox(&(singleton->volMinInPixels), &(singleton->volMaxInPixels));
		
			if (singleton->placingGeom) {
				
				
				singleton->setShaderVec3("matVal", 255, 0, 0);
				singleton->setShaderFloat("isWire", 0.0);
				
				for (i = 0; i <= singleton->geomStep; i++) {
					
					singleton->drawCubeCentered(
						&singleton->geomPoints[i],
						0.25f	
					);
				}
			}
		
			break;
		
		case E_MOUSE_STATE_BRUSH:
			
			
			if (singleton->earthMod == E_PTT_TER) {
				singleton->setShaderVec3("matVal", 255, 0, 0);
			}
			else {
				singleton->setShaderVec3("matVal", 0, 0, 255);
			}
			
			
			singleton->setShaderFloat("isWire", 1.0);
			singleton->drawCubeCentered(
				&lastUnitPos,
				((int)singleton->curBrushRad) * (singleton->unitSizeInPixels)
			);
			
			
			//glClear(GL_DEPTH_BUFFER_BIT);
		break;
		case E_MOUSE_STATE_PICKING:

			
			

			if (singleton->highlightedEnt == NULL)
			{

			}
			else
			{
				singleton->setShaderVec3("matVal", 254, 254, 254);
				singleton->setShaderFloat("isWire", 1.0);

				minv.setFXYZRef(singleton->highlightedEnt->getVisMinInPixelsT());
				maxv.setFXYZRef(singleton->highlightedEnt->getVisMaxInPixelsT());

				minv.addXYZ(-0.25 * singleton->pixelsPerCell);
				maxv.addXYZ(0.25 * singleton->pixelsPerCell);


				singleton->drawBox(&minv, &maxv);
			}

			if (singleton->selectedEnts.getSelectedEnt() == NULL)
			{

			}
			else
			{
				singleton->setShaderVec3("matVal", 255, 255, 0);
				singleton->setShaderFloat("isWire", 1.0);

				minv.setFXYZRef(singleton->selectedEnts.getSelectedEnt()->getVisMinInPixelsT());
				maxv.setFXYZRef(singleton->selectedEnts.getSelectedEnt()->getVisMaxInPixelsT());

				singleton->drawBox(&minv, &maxv);
			}


			break;

		}
		
		if (singleton->pathfindingOn) {
			drawAIPath( &(singleton->charPathHolder), &(singleton->splitPathHolder) );
		}
		
		if (
			(singleton->bCtrl) &&
			(singleton->mouseState != E_MOUSE_STATE_BRUSH) &&
			(!(singleton->anyMenuVisible()))	
		) {
			for (i = 1; i < singleton->dynObjects.size(); i++) {
				if (singleton->dynObjects[i]->doRender)
				{

					doProc = false;

					if (i >= E_OBJ_LIGHT0)
					{
						if (i - E_OBJ_LIGHT0 < singleton->numDynLights)
						{
							doProc = true;
						}
					}
					else
					{
						doProc = true;
					}

					if (doProc)
					{
						singleton->setShaderFloat("objectId",i);
						singleton->setShaderfVec3("matVal", &(singleton->dynObjects[i]->color) );
						curBoxPos = &(singleton->dynObjects[i]->pos);
						singleton->drawCubeCentered(curBoxPos, singleton->dynObjects[i]->radius);


						if (i == singleton->activeObject)
						{
							//singleton->drawCrossHairs(singleton->dynObjects[i]->pos,4.0f);
						}
					}


				}
			}
		}
		
		// singleton->setShaderVec3("matVal", 254, 0, 0);
		// singleton->setShaderFloat("isWire", 1.0);
		// for (i = 0; i < singleton->nearestLights.selEntList.size(); i++) {
			

		// 	minv.setFXYZRef(singleton->nearestLights.selEntList[i]->getVisMinInPixelsT());
		// 	maxv.setFXYZRef(singleton->nearestLights.selEntList[i]->getVisMaxInPixelsT());


		// 	singleton->drawBox(&minv, &maxv);
		// }



		// draw volume around organism
		// GameOrg* activeOrg = singleton->testHuman;
		// GamePageHolder* gphOrg = activeOrg->gph;
		// if (singleton->orgOn) {
			
		// 	singleton->setShaderFloat("objectId",0.0);
		// 	drawOrg(singleton->testHuman, false);
			
		// 	tempVec.copyFrom(&(activeOrg->basePosition));
		// 	tempVec.addXYZRef(&gphOrg->gphCenInPixels,-1.0f);
		// 	singleton->setShaderfVec3("offsetPos",&tempVec);
			
		// 	singleton->setShaderFloat("isWire", 1.0);
		// 	singleton->setShaderVec3("matVal", 255, 0, 0 );
			
		// 	singleton->drawBox( &(gphOrg->gphMinInPixels), &(gphOrg->gphMaxInPixels) );
			
		// }

		
		
		singleton->unbindFBO();
		singleton->unbindShader();







		//////////////////////////////
		
		singleton->copyFBO2("geomBaseTargFBO","geomTargFBO");
		
		//glDisable(GL_DEPTH_TEST);
		
		singleton->bindShader("BillboardShader");
		singleton->setShaderfVec2("bufferDim", &(singleton->bufferModDim) );
		singleton->setShaderFloat("holderSizeInPixels",singleton->holderSizeInPixels);
		singleton->setShaderFloat("pixelsPerCell", singleton->pixelsPerCell);
		singleton->setShaderFloat("heightOfNearPlane",singleton->heightOfNearPlane);
		singleton->setShaderFloat("clipDist",singleton->clipDist[1]);
		singleton->setShaderfVec3("cameraPos", singleton->cameraPos);
		singleton->setShaderTexture(0,singleton->fontWrappers[EFW_ICONS]->fontImage->tid);
		singleton->sampleFBO("geomBaseTargFBO",1);
		singleton->bindFBO("geomTargFBO", -1, 0);
		
		glBegin(GL_POINTS);
		for(i = 0; i < visObjects.size(); i++) {
			
			curObj = &(gameObjects[visObjects[i]]);
			
			tempCS = &(singleton->fontWrappers[EFW_ICONS]->charVals[
				singleton->entIdToIcon[curObj->objectType] 
			]);
			frameMod = 0;
			if (curObj->maxFrames != 0) {
				frameMod = ((singleton->currentTick/(20))%(curObj->maxFrames))*16.0f;
			}
			
			
			x1 = (tempCS->sampX + frameMod )/iw;
			x2 = (tempCS->sampX + tempCS->sampW + frameMod)/iw;
			y1 = 1.0f-(tempCS->sampY)/ih;
			y2 = 1.0f-(tempCS->sampY+tempCS->sampH)/ih;
			
			glMultiTexCoord4f(GL_TEXTURE0, visObjects[i], -1.0f, 1.0f, 1.0f);
			glMultiTexCoord4f(GL_TEXTURE1, x1,y1,x2,y2);
			glVertex3f(
				curObj->centerPointInPixels[0],
				curObj->centerPointInPixels[1],
				curObj->centerPointInPixels[2]
			);
			
		}
		glEnd();
		
		singleton->unsampleFBO("geomBaseTargFBO",1);
		singleton->setShaderTexture(0,0);
		singleton->unbindFBO();
		singleton->unbindShader();
		
		//////////////////////////////






	}


	void updateMouseCoords(FIVector4 *fPixelWorldCoordsBase) {

		FIVector4 fPixelWorldCoords;
		fPixelWorldCoords.copyFrom(fPixelWorldCoordsBase);


		lastUnitPos.copyFrom(&fPixelWorldCoords);
		lastUnitPos.intDivXYZ(singleton->unitSizeInPixels);
		lastUnitPos.multXYZ(singleton->unitSizeInPixels);
		lastUnitPos.setFW(singleton->unitSizeInPixels);
		

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
		blockMip = intLogB2(singleton->blockSizeInLots);

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
		singleton->setShaderArrayfVec3("paramArrMap", singleton->paramArrMap, 16 );
		singleton->setShaderFloat("mapSampScale", 1.0f); //singleton->mapSampScale
		singleton->drawFSQuad();
		singleton->setShaderTexture(2, 0);
		singleton->setShaderTexture(1, 0);
		singleton->unsampleFBO("simplexFBO", 0);
		singleton->unbindFBO();
		singleton->unbindShader();

		singleton->copyFBO("hmFBOLinear", "hmFBO");


		fbow->getPixels(true);
		fbow->setAllPixels(densityChannel, 255);
		fbow->setAllPixels(idChannel, 0);
		fbow->setAllPixels(blockChannel, 0);


		// determine sea level

		for (i = 0; i < 256; i++)
		{
			histogram[i] = 0;
		}
		
		int minSL = 0;
		int maxSL = 0;
		int curSL = 0;
		int avgSL = 0;
		
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

		int totFilled = 0;
		i = 0;
		while (totFilled < (totSize * 40) / 100 )
		{
			totFilled += histogram[i];
			i++;
		}
		
		avgSL = (minSL*3+maxSL*1)/4;

		
		// TODO: FIX SEA LEVEL
		
		if (RAY_MODE) {
			seaLevel = 0;
		}
		else {
			seaLevel = i+20; //avgSL;//1
		}
		
		
		seaSlack = seaLevel - 1;
		cout << "Sea Level: " << seaLevel << "\n";

		mapWidth = fbow->width;
		mapHeight = fbow->height;

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

					if (tempDis < 200.0f * singleton->mapSampScale)
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


		int blockMod = singleton->blockSizeInLots;
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
			if ( testPix1 != testPix2 || testPix3 != testPix4 || testPix > 255 )
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

		for (i = 0; i < w; i += singleton->blockSizeInLots )
		{

			curInd = i - 1;

			while (curInd < 0)
			{
				curInd += w;
			}

			streetFlagsV[curInd] |= 1;
			streetFlagsV[i] |= 2;
		}
		for (i = 0; i < h; i += singleton->blockSizeInLots )
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




		int wb = worldSizeInBlocks.getIX();
		int hb = worldSizeInBlocks.getIY();
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



		singleton->mapInvalid = false;

		singleton->setCameraToElevation();

		cout << "DONE WITH MAP\n";

		mapLockOn = false;

	}


	void drawMap()
	{

		if (singleton->mainMenu == NULL) {
			return;
		}
		if (singleton->mainMenu->visible){
			
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
		//singleton->sampleFBO("backFaceMapFBO",3);
		//singleton->sampleFBO("frontFaceMapFBO",4);

		
		singleton->setShaderFloat("timeOfDay", singleton->timeOfDay);
		singleton->setShaderfVec4("mapFreqs", &(singleton->mapFreqs) );
		singleton->setShaderfVec4("mapAmps", &(singleton->mapAmps) );
		singleton->setShaderfVec3("cameraPos", singleton->cameraPos);
		singleton->setShaderfVec3("lookAtVec", &(singleton->lookAtVec));
		
		singleton->setShaderfVec3("maxBoundsInPixels", &(singleton->worldSizeInPixels));
		singleton->setShaderFloat("cameraZoom", singleton->cameraZoom);
		//singleton->setShaderFloat("mapTrans", mapTrans);
		singleton->setShaderFloat("seaLevel", singleton->getSLNormalized() );
		singleton->setShaderFloat("curTime", singleton->curTime);
		singleton->setShaderVec2("mapDimInPixels", fbow->width, fbow->height);
		singleton->setShaderfVec3("worldSizeInPixels", &(singleton->worldSizeInPixels) );

		
		
		
		singleton->mainGUI->renderQuadDirect(singleton->mapComp);
		

		//singleton->drawQuadBounds

		//singleton->drawFSQuadOffset(0.75f,0.75f,0.5f,1.0f/singleton->aspectRatio);
		//singleton->drawFSQuad();

		
		//singleton->unsampleFBO("frontFaceMapFBO",4);
		//singleton->unsampleFBO("backFaceMapFBO",3);
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

	void updateLights()
	{
		
		
		int i;
		int j;
		int k;
		bool flag = true;
		GameEnt *tempLight;
		GameEnt *curLight;
		
		
		lightCount = singleton->numDynLights;
		
		
		
		
		

		
		



		if (singleton->targetTimeOfDay <= 0.5)
		{
			
			
			findNearestEnt(&(singleton->nearestLights),E_ET_LIGHT,4,2,singleton->cameraPos,false,true);
			
			
			for (i = 0; i < singleton->nearestLights.selEntList.size(); i++) {
				
				
				curLight = singleton->nearestLights.selEntList[i];//&(curBlock->gameEnts[E_ET_LIGHT].data[k]);
				curLight->camDistance = singleton->cameraPos->distance(&(curLight->geomParams[E_LP_POSITION]));

				if (curLight->toggled) {
					activeLights[lightCount] = singleton->nearestLights.selEntList[i];//&(curBlock->gameEnts[E_ET_LIGHT].data[k]);
					lightCount++;
				}

				if (lightCount >= MAX_EVAL_LIGHTS)
				{
					goto UPDATE_LIGHTS_END;
				}
				
				
			}
			
			

UPDATE_LIGHTS_END:

			for (i = singleton->numDynLights + 1; (i <= lightCount) && flag; i++)
			{
				flag = false;
				for (j = singleton->numDynLights; j < (lightCount - 1); j++)
				{
					if (activeLights[j + 1]->camDistance < activeLights[j]->camDistance) // ascending order simply changes to <
					{
						tempLight = activeLights[j];
						activeLights[j] = activeLights[j + 1];
						activeLights[j + 1] = tempLight;
						flag = true;
					}
				}
			}

			lightCount = min(lightCount, MAX_LIGHTS);
		}



	}

	void renderWaveHeight() {
		singleton->bindShader("WaveHeightShader");
		singleton->bindFBO("waveFBO");
		
		singleton->sampleFBO("pagesTargFBO",0);
		singleton->sampleFBO("waterTargFBO",2);
		
		//singleton->setShaderFloat("waveSpacing", singleton->pixelsPerCell * 4.0f);
		singleton->setShaderVec2("resolution", singleton->currentFBOResolutionX, singleton->currentFBOResolutionY);
		singleton->setShaderFloat("curTime", singleton->curTime);
		singleton->drawFSQuad();
		
		
		singleton->unsampleFBO("waterTargFBO",2);
		singleton->unsampleFBO("pagesTargFBO",0);
		
		singleton->unbindFBO();
		singleton->unbindShader();
	}

	void postProcess()
	{

		

		int i;
		int k;
		int curCount;
		int baseInd;

		bool doProc = false;

		GameEnt *curLight;



		for (i = 0; i < singleton->numDynLights; i++)
		{
			activeLights[i] = singleton->dynObjects[E_OBJ_LIGHT0 + i]->getLight();
		}

		curCount = 0;
		for (k = 0; k < lightCount; k++)
		{
			baseInd = curCount * FLOATS_PER_LIGHT;
			curLight = activeLights[k];
			lightPos = &(curLight->geomParams[E_LP_POSITION]);

			if (curLight->toggled) {
				singleton->worldToScreenBase(&lScreenCoords, lightPos);

				singleton->lightArr[baseInd + 0] = lightPos->getFX();
				singleton->lightArr[baseInd + 1] = lightPos->getFY();
				singleton->lightArr[baseInd + 2] = lightPos->getFZ();
				singleton->lightArr[baseInd + 3] = lScreenCoords.getFZ();


				singleton->lightArr[baseInd + 4] = lScreenCoords.getFX();
				singleton->lightArr[baseInd + 5] = lScreenCoords.getFY();
				singleton->lightArr[baseInd + 6] = lScreenCoords.getFZ();
				singleton->lightArr[baseInd + 7] = curLight->geomParams[E_LP_RADIUS].getFX();


				// light color

				singleton->lightArr[baseInd + 8] = curLight->geomParams[E_LP_COLOR].getFX(); // light red
				singleton->lightArr[baseInd + 9] = curLight->geomParams[E_LP_COLOR].getFY(); // light green
				singleton->lightArr[baseInd + 10] = curLight->geomParams[E_LP_COLOR].getFZ(); // light blue

				// switch (k)
				// {
				// case 0:
				// 	singleton->lightArr[baseInd + 11] = 1.0f; // light intensity (unused?)
				// 	singleton->lightArr[baseInd + 12] = 0.0f; // light colorization (0-1)
				// 	singleton->lightArr[baseInd + 13] = 0.0f; // light flooding (colorizes regardless of shadows) (0-1)
				// 	break;
				// default:
				// 	singleton->lightArr[baseInd + 11] = 1.0f;
				// 	singleton->lightArr[baseInd + 12] = 1.0f;
				// 	singleton->lightArr[baseInd + 13] = 0.0f;
				// 	break;

				// }

				curCount++;
			}

			
		}

		

		activeFBO = 0;

		renderWaveHeight();

		if (singleton->waterOn)	{


			


			singleton->bindShader("WaterShaderCombine");
			singleton->bindFBO("combineWithWaterTargFBO");
			singleton->sampleFBO("pagesTargFBO",0);
			singleton->sampleFBO("waterTargFBO",2);
			singleton->sampleFBO("geomTargFBO", 4);
			singleton->sampleFBO("waveFBO", 6);
			
			singleton->setShaderFloat("clipDist",singleton->clipDist[1]);
			singleton->setShaderFloat("pixelsPerCell", singleton->pixelsPerCell);
			singleton->setShaderVec2("resolution", singleton->currentFBOResolutionX, singleton->currentFBOResolutionY);
			singleton->setShaderfVec3("cameraPos", singleton->cameraPos);
			singleton->setShaderfVec2("bufferDim", &(singleton->bufferModDim));
			singleton->setShaderFloat("curTime", singleton->curTime);
			singleton->drawFSQuad();
			
			singleton->unsampleFBO("waveFBO", 6);
			singleton->unsampleFBO("geomTargFBO", 4);
			singleton->unsampleFBO("waterTargFBO",2);
			singleton->unsampleFBO("pagesTargFBO",0);
			singleton->unbindFBO();
			singleton->unbindShader();
			
			
			
		}
		else
		{
			// TODO: copy over any necessary buffers when water is off
			
			singleton->copyFBO2("pagesTargFBO","combineWithWaterTargFBO");
			
		}




		singleton->bindShader("PreLightingShader");
		singleton->bindFBO("prelightFBO");
		singleton->sampleFBO("pagesTargFBO",0);
		singleton->sampleFBO("combineWithWaterTargFBO",2);
		singleton->setShaderfVec3("lookAtVec", &(singleton->lookAtVec));
		singleton->setShaderVec2("bufferDim", singleton->currentFBOResolutionX, singleton->currentFBOResolutionY); //MUST BE CALLED AFTER FBO IS BOUND
		singleton->setShaderfVec3("cameraPos", singleton->cameraPos);
		singleton->setShaderInt("testOn", (int)(singleton->testOn));
		singleton->setShaderInt("iNumSteps", singleton->iNumSteps);
		singleton->setShaderArrayfVec4("lightArr", singleton->lightArr, (FLOATS_PER_LIGHT * lightCount) / 4);
		singleton->setShaderInt("vecsPerLight", FLOATS_PER_LIGHT / 4);
		singleton->setShaderFloat("pixelsPerCell", singleton->pixelsPerCell);
		singleton->setShaderFloat("lightCount", lightCount);
		singleton->setShaderFloat("timeOfDay", singleton->timeOfDay);
		singleton->drawFSQuad();
		singleton->unsampleFBO("combineWithWaterTargFBO",2);
		singleton->unsampleFBO("pagesTargFBO",0);
		singleton->unbindFBO();
		singleton->unbindShader();
		
			
		
		
		
		singleton->bindShader("PostLightingShader");
		singleton->bindFBO("resultFBO", activeFBO);
		
		singleton->sampleFBO("pagesTargFBO",0);
		singleton->sampleFBO("prelightFBO", 2);
		singleton->sampleFBO("geomTargFBO", 6);
		singleton->setShaderTexture3D(8,singleton->volIdMat);
		
		singleton->setShaderfVec4("worldMarker",&(singleton->worldMarker));
		singleton->setShaderInt("markerFound", (int)(singleton->markerFound));
		
		
		singleton->setShaderfVec3("lookAtVec", &(singleton->lookAtVec));
		singleton->setShaderVec2("bufferDim", singleton->currentFBOResolutionX, singleton->currentFBOResolutionY); //MUST BE CALLED AFTER FBO IS BOUND
		singleton->setShaderfVec3("cameraPos", singleton->cameraPos);
		singleton->setShaderInt("gridOn", (int)(singleton->gridOn));
		singleton->setShaderInt("testOn", (int)(singleton->testOn));
		singleton->setShaderFloat("curTime", singleton->curTime);
		singleton->setShaderFloat("pixelsPerCell", singleton->pixelsPerCell);
		singleton->setShaderFloat("blockSizeInCells", singleton->blockSizeInCells);
		singleton->setShaderFloat("timeOfDay", singleton->timeOfDay);
		singleton->drawFSQuad();
		
		singleton->setShaderTexture3D(8,0);
		singleton->unsampleFBO("geomTargFBO", 6);
		singleton->unsampleFBO("prelightFBO", 2);
		singleton->unsampleFBO("pagesTargFBO",0);
		
		singleton->unbindFBO();
		singleton->unbindShader();
		
		activeFBO = 1 - activeFBO;
		
		

		if (singleton->waterOn)
		{
			singleton->copyFBO("resultFBO1", "swapFBOLinHalf0");
			doBlur("swapFBOLinHalf");
			
			singleton->bindShader("WaterShader");
			singleton->bindFBO("resultFBO", activeFBO);
			singleton->sampleFBO("pagesTargFBO",0);
			singleton->sampleFBO("combineWithWaterTargFBO",2);
			singleton->sampleFBO("resultFBO", 4, activeFBO);
			singleton->sampleFBO("swapFBOLinHalf0", 5);
			singleton->sampleFBO("noiseFBO", 6);
			singleton->sampleFBO("waveFBO", 7);
			singleton->setShaderTexture3D(8,singleton->volIdMat);
			singleton->sampleFBO("prelightFBO", 9);
			singleton->sampleFBO("geomTargFBO", 13);
			
			singleton->setShaderFloat("clipDist",singleton->clipDist[1]);
			singleton->setShaderFloat("pixelsPerCell", singleton->pixelsPerCell);
			singleton->setShaderFloat("timeOfDay", singleton->timeOfDay);
			singleton->setShaderVec2("resolution", singleton->currentFBOResolutionX, singleton->currentFBOResolutionY);
			singleton->setShaderfVec3("cameraPos", singleton->cameraPos);
			singleton->setShaderfVec2("bufferDim", &(singleton->bufferModDim));
			singleton->setShaderFloat("curTime", singleton->curTime);
			singleton->setShaderFloat("isUnderWater", singleton->getUnderWater() );
			singleton->drawFSQuad();
			
			
			singleton->unsampleFBO("geomTargFBO", 13);
			singleton->unsampleFBO("prelightFBO", 9);
			singleton->setShaderTexture3D(8,0);
			singleton->unsampleFBO("waveFBO", 7);
			singleton->unsampleFBO("noiseFBO", 6);
			singleton->unsampleFBO("swapFBOLinHalf0", 5);
			singleton->unsampleFBO("resultFBO", 4, activeFBO);
			singleton->unsampleFBO("waterTargFBO",2);
			singleton->unsampleFBO("pagesTargFBO",0);
			singleton->unbindFBO();
			singleton->unbindShader();

			activeFBO = 1 - activeFBO;
		}


		if (singleton->radiosityOn || singleton->fogOn)
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



		if (singleton->radiosityOn)
		{

			singleton->bindShader("RadiosityShader");

			singleton->bindFBO("swapFBOLinHalf0");
			singleton->sampleFBO("combineWithWaterTargFBO",0);
			singleton->sampleFBO("swapFBOBLin0", 2);
			singleton->setShaderVec2("resolution", singleton->currentFBOResolutionX, singleton->currentFBOResolutionY); //MUST BE CALLED AFTER FBO IS BOUND
			singleton->setShaderfVec2("bufferDim", &(singleton->bufferModDim));
			singleton->setShaderfVec3("cameraPos", singleton->cameraPos);
			singleton->setShaderfVec3("lightPosWS", lightPos);
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
			singleton->sampleFBO("geomTargFBO", 4);
			singleton->setShaderInt("testOn", (int)(singleton->testOn));
			singleton->drawFSQuad();
			singleton->unsampleFBO("geomTargFBO", 4);
			singleton->unsampleFBO("combineWithWaterTargFBO",2);
			singleton->unsampleFBO("swapFBOLinHalf0", 1);
			singleton->unsampleFBO("resultFBO", 0, activeFBO);
			singleton->unbindFBO();
			singleton->unbindShader();

			activeFBO = 1 - activeFBO;


		}

		if (singleton->fogOn == 1.0f)
		{

			

			singleton->bindShader("FogShader");
			singleton->bindFBO("resultFBO", activeFBO);

			singleton->sampleFBO("combineWithWaterTargFBO",0);
			singleton->sampleFBO("resultFBO", 2, activeFBO);
			singleton->sampleFBO("swapFBOBLin0", 3);
			singleton->sampleFBO("geomTargFBO", 4);
			singleton->setShaderTexture3D(6,singleton->volIdMat);
			singleton->sampleFBO("waveFBO", 7);
			
			
			if ((singleton->currentActor == NULL)||singleton->firstPerson) {
				singleton->setShaderFloat("thirdPerson", 0.0f);
			}
			else {
				singleton->setShaderFloat("thirdPerson", 1.0f);
				singleton->setShaderfVec3("entPos", &(singleton->currentActor->centerPointInPixels));
				singleton->setShaderFloat("volSizePrim", singleton->volSizePrim);
			}
			
			singleton->setShaderInt("iNumSteps", singleton->iNumSteps);
			singleton->setShaderFloat("curTime", singleton->curTime);
			singleton->setShaderFloat("selObjInd",singleton->selObjInd);
			singleton->setShaderFloat("actObjInd",singleton->actObjInd);
			singleton->setShaderFloat("isUnderWater", singleton->getUnderWater() );
			singleton->setShaderFloat("timeOfDay", singleton->timeOfDay);
			singleton->setShaderVec2("resolution", singleton->currentFBOResolutionX, singleton->currentFBOResolutionY); //MUST BE CALLED AFTER FBO IS BOUND
			singleton->setShaderfVec2("bufferDim", &(singleton->bufferModDim));
			singleton->setShaderfVec3("cameraPos", singleton->cameraPos);
			singleton->setShaderfVec3("lookAtVec", &(singleton->lookAtVec));
			//singleton->setShaderfVec4("fogPos", fogPos);

			singleton->drawFSQuad();

			singleton->unsampleFBO("waveFBO", 7);
			singleton->setShaderTexture3D(6,0);
			singleton->unsampleFBO("geomTargFBO", 4);
			singleton->unsampleFBO("swapFBOBLin0", 3);
			singleton->unsampleFBO("resultFBO", 2, activeFBO);
			singleton->unsampleFBO("combineWithWaterTargFBO",0);

			singleton->unbindFBO();
			singleton->unbindShader();


			activeFBO = 1 - activeFBO;

		}
		
		
		for (i = 0; i < singleton->medianCount; i++) {
			singleton->bindShader("MedianShader");
			singleton->bindFBO("resultFBO",activeFBO);
			singleton->sampleFBO("resultFBO", 0, activeFBO);
			singleton->sampleFBO("pagesTargFBO", 1);
			singleton->setShaderfVec2("bufferDim", &(singleton->bufferModDim) );
			
			singleton->drawFSQuad();
			
			singleton->unsampleFBO("pagesTargFBO", 1);
			singleton->unsampleFBO("resultFBO", 0, activeFBO);
			singleton->unbindFBO();
			singleton->unbindShader();
			
			activeFBO = 1-activeFBO;
		}
		
		
		if (singleton->testOn) {			
			singleton->drawFBO("pagesTargFBO", 0, 1.0f); //waterTargFBO
			
		}
		else {
			singleton->drawFBO("resultFBO", 0, 1.0f, 1 - activeFBO);
						
		}

		


		if (singleton->anyMenuVisible()) {
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

	~GameWorld()
	{

		//threadpool.stopAll();

		int i;
		int j;

	}


};

