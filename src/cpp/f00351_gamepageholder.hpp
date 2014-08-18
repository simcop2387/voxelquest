

class GamePageHolder {
public:

	int iPageDataVolume;
	int holderSizeInPages;
	int usingPoolId;
	
	int blockId;
	int holderId;

	bool isDirty;
	bool hasTrans;
	bool hasSolids;
	bool underground;
	
	std::vector<GameGeom *> entityGeom;
	int entityGeomCounter;
	int entType;

	FIVector4 offsetInHolders;

	FIVector4 gphMinInPixels;
	FIVector4 gphMaxInPixels;

	FIVector4 offsetInBlocks;

	PooledResource* gpuRes;

	Singleton* singleton;

	std::vector<intPair> containsGeomIds;
	GamePage** pageData;
	
	bool isEntity;

	GamePageHolder() {
		isEntity = false;
		usingPoolId = -1;
		hasTrans = false;
		hasSolids = false;
		underground = false;
	}
	

	void init(
		Singleton* _singleton,
		int _blockId, 			// MUST BE UNIQUE FOR ENTITES
		int _holderId,
		
		int trueX,
		int trueY,
		int trueZ,
		
		bool _isEntity = false,
		bool _entType = E_ET_TEST

	) {

		entType = _entType;
		entityGeomCounter = 0;

		int i;
		int j;
		int k;
		int ind;

		isEntity = _isEntity;

		blockId = _blockId;
		holderId = _holderId;

		isDirty = true;

		singleton = _singleton;
		usingPoolId = -1;

		offsetInHolders.setIXYZ(trueX,trueY,trueZ);
		offsetInBlocks.copyFrom(&offsetInHolders);
		offsetInBlocks.intDivXYZ(singleton->blockSizeInHolders);
		
		gphMinInPixels.copyFrom(&offsetInHolders);
		gphMaxInPixels.copyFrom(&offsetInHolders);

		gphMaxInPixels.addXYZ(1);

		gphMinInPixels.multXYZ(singleton->holderSizeInPixels);
		gphMaxInPixels.multXYZ(singleton->holderSizeInPixels);


		holderSizeInPages = singleton->holderSizeInPages;
		iPageDataVolume = holderSizeInPages*holderSizeInPages*holderSizeInPages;
		pageData = new GamePage*[iPageDataVolume];
		
		

		for (i = 0; i < iPageDataVolume; i++) {
			pageData[i] = NULL;
		}
		
		
		if (isEntity) {
			fetchEntityGeom();
		}
		else {
			fetchGeom();
		}
		
		for (k = 0; k < holderSizeInPages; k++) {
			for (j = 0; j < holderSizeInPages; j++) {
				for (i = 0; i < holderSizeInPages; i++) {
					ind = k*holderSizeInPages*holderSizeInPages + j*holderSizeInPages + i;
					
					if (pageData[ind] == NULL) {
						
						if (isEntity) {
							pageData[ind] = getPageAtCoordsLocal(
								offsetInHolders.getIX()*holderSizeInPages + i,
								offsetInHolders.getIY()*holderSizeInPages + j,
								offsetInHolders.getIZ()*holderSizeInPages + k,
								true
							);
						}
						else {
							pageData[ind] = singleton->gw->getPageAtCoords(
								offsetInHolders.getIX()*holderSizeInPages + i,
								offsetInHolders.getIY()*holderSizeInPages + j,
								offsetInHolders.getIZ()*holderSizeInPages + k,
								true	
							);
						}
						
						
						
					}
				}
			}
		}
		
	}
	
	
	GamePage* getPageAtCoordsLocal(int x, int y, int z, bool createOnNull = false) {
		

		int hx, hy, hz;
		int px, py, pz;
		int gpInd;
		int newX = x;
		int newY = y;
		int newZ = z;
		int ind =
			newZ * holderSizeInPages * holderSizeInPages +
			newY * holderSizeInPages +
			newX;

		GamePage *gp = NULL;

		px = newX % holderSizeInPages;
		py = newY % holderSizeInPages;
		pz = newZ % holderSizeInPages;
		



		gpInd = pz * holderSizeInPages * holderSizeInPages + py * holderSizeInPages + px;

		if (gpInd == -1)
		{
			// prob
			cout << "Invalid holder index\n";
		}
		else
		{
			gp = pageData[gpInd];

			if (gp)
			{

			}
			else
			{
				if (createOnNull)
				{
					pageData[gpInd] = new GamePage();
					gp = pageData[gpInd];
					gp->init(
						singleton,
						this,
						ind,
						x, y, z,
						px, py, pz,
						true
					);
				}
			}
		}

		

		return gp;

		
	}
	
	

	void clearSet(bool forceClear) {
		int i;

		bool doClear = forceClear;

		if (usingPoolId == -1) {
			
			if (isEntity) {
				usingPoolId = singleton->entityPool->requestPoolId(blockId,holderId);
				gpuRes = singleton->entityPool->holderPoolItems[usingPoolId];
			}
			else {
				usingPoolId = singleton->gpuPool->requestPoolId(blockId,holderId);
				gpuRes = singleton->gpuPool->holderPoolItems[usingPoolId];
			}
			

			doClear = true;
		}

		if (doClear) {
			for (i = 0; i < MAX_LAYERS; i++) {
				// clear fbo by binding it with auto flag
				singleton->bindFBODirect(gpuRes->getFBOS(i));
				singleton->unbindFBO();
			}
		}
		
	}

	void refreshChildren(bool refreshImmediate) {
		int i;

		clearSet(true);
		

		for (i = 0; i < iPageDataVolume; i++) {
			if (pageData[i] == NULL) {

			}
			else {

				if (refreshImmediate) {
					
					if (underground) {
						
					}
					else {
						if (hasSolids || hasTrans) {
							pageData[i]->generateVolume();
						}
					}
					
					
					
					isDirty = false;
					
				}
				else {
					isDirty = true;
				}

				
			}
		}
	}


	void addNewGeom(
		int _curBT,
		int _curAlign,
		float _baseOffset,
		FIVector4* _p1,
		FIVector4* _p2,
		FIVector4* _rad,
		FIVector4* _cornerRad,
		FIVector4* _visInsetFromMin,
		FIVector4* _visInsetFromMax,
		FIVector4* _powerVals,
		FIVector4* _powerVals2,
		FIVector4* _thickVals,
		FIVector4* _matParams,
		FIVector4* _centerPoint,
		FIVector4* _anchorPoint,
		int _minRot,
		int _maxRot
		
	) {
		entityGeom.push_back(new GameGeom());
		entityGeom.back()->initBounds(
			_curBT,
			entityGeomCounter,
			entityGeomCounter,
			_curAlign,
			_baseOffset,
			_p1,
			_p2,
			_rad,
			_cornerRad,
			_visInsetFromMin,
			_visInsetFromMax,
			_powerVals,
			_powerVals2,
			_thickVals,
			_matParams,
			_centerPoint,
			_anchorPoint,
			_minRot,
			_maxRot
		);
		entityGeomCounter++;
	}

	void fetchEntityGeom() {
		int curBT;
		int curAlign;
		float baseOffset;
		FIVector4 p1;
		FIVector4 p2;
		FIVector4 rad;
		FIVector4 cornerRad;
		FIVector4 visInsetFromMin;
		FIVector4 visInsetFromMax;
		FIVector4 powerVals;
		FIVector4 powerVals2;
		FIVector4 thickVals;
		FIVector4 matParams;
		FIVector4 centerPoint;
		FIVector4 anchorPoint;
		int minRot;
		int maxRot;
		
		
		FIVector4 orig;
		FIVector4 maxRad;
		
		orig.setFXYZRef(&gphMinInPixels);
		orig.addXYZRef(&gphMaxInPixels);
		orig.multXYZ(0.5f);
		
		
		
		float halfHolderSize = singleton->holderSizeInPixels*0.5f;
		
		
		curBT = E_CT_OBJECT;
		curAlign = E_ALIGN_MIDDLE;
		baseOffset = 0.0f;
		p1.setFXYZRef(&orig);
		p2.setFXYZRef(&orig);
		rad.setFXYZ(
			halfHolderSize,
			halfHolderSize,
			halfHolderSize	
		);
		cornerRad.setFXYZ(
			halfHolderSize,
			halfHolderSize,
			halfHolderSize	
		);
		visInsetFromMin.setFXYZ(0.0f,0.0f,0.0f);
		visInsetFromMax.setFXYZ(0.0f,0.0f,0.0f);
		
		powerVals.setFXYZ(2.0f,2.0f,0.0f);
		powerVals2.setFXYZ(2.0f,2.0f,0.0f);
		thickVals.setFXYZ(0.0f,0.0f,0.0f);
		matParams.setFXYZ(E_MAT_PARAM_OBJECT,0.0f,0.0f);
		centerPoint.setFXYZRef(&orig);
		anchorPoint.setFXYZRef(&orig);
		minRot = 0;
		maxRot = 0;
		
		
		
		
		addNewGeom(
			curBT,
			curAlign,
			baseOffset,
			&p1,
			&p2,
			&rad,
			&cornerRad,
			&visInsetFromMin,
			&visInsetFromMax,
			&powerVals,
			&powerVals2,
			&thickVals,
			&matParams,
			&centerPoint,
			&anchorPoint,
			minRot,
			maxRot
		);
		
		
	}

	void fetchGeom() {
		int i;
		int j;
		int k;
		int bufSize = (singleton->visPageSizeInPixels*singleton->bufferMult)*2;
		
		GameBlock* curBlock;
		GamePageHolder* gph;
		FIVector4 start;
		FIVector4 end;
		GameGeom* geom;

		containsGeomIds.clear();

		for (i = -1; i <= 1; i++) {
			for (j = -1; j <= 1; j++) {
				curBlock = singleton->gw->getBlockAtCoords(
					offsetInBlocks.getIX()+i,
					offsetInBlocks.getIY()+j,
					true
				);

				for (k = 0; k < curBlock->gameGeom.size(); k++) {


					geom = curBlock->gameGeom[k];


					start.copyFrom( &(geom->moveMinInPixels) );
					end.copyFrom( &(geom->moveMaxInPixels) );

					start.addXYZ(-bufSize);
					end.addXYZ(bufSize);

					//start.intDivXYZ(singleton->holderSizeInPixels);
					//end.intDivXYZ(singleton->holderSizeInPixels);

					start.clampZ(0.0,singleton->worldSizeInPixels.getFZ()-1.0f);
					end.clampZ(0.0,singleton->worldSizeInPixels.getFZ()-1.0f);

					if (FIVector4::intersect(&start,&end,&gphMinInPixels,&gphMaxInPixels)) {
						containsGeomIds.push_back(intPair());
						containsGeomIds.back().v0 = curBlock->blockId;
						containsGeomIds.back().v1 = k;
					}
				}
			}
		}
	}


	void unbindGPUResources() {

		usingPoolId = -1;
		gpuRes = NULL;
		isDirty = true;

	}

};

