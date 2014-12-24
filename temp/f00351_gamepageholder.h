// f00351_gamepageholder.h
//

#include "f00351_gamepageholder.e"
#define LZZ_INLINE inline
GamePageHolder::GamePageHolder ()
                         {
		isEntity = false;
		usingPoolId = -1;
		hasTrans = false;
		hasSolids = false;
	}
void GamePageHolder::init (Singleton * _singleton, int _blockId, int _holderId, int trueX, int trueY, int trueZ, bool _isEntity)
          {

		
		refreshDL = true;
		entityGeomCounter = 0;

		int i;
		int j;
		int k;
		int ind;
		
		for (i = 0; i < MAX_LAYERS; i++) {
			hasVerts[i] = false;
		}
		
		
		readyForClear = true;

		isEntity = _isEntity;

		blockId = _blockId;
		holderId = _holderId;

		childrenDirty = true;

		singleton = _singleton;
		usingPoolId = -1;

		sres = singleton->volGenSuperRes;

		holderSizeInPixels = singleton->holderSizeInPixels;
		halfHolderSizeInPixels = holderSizeInPixels*0.5f;
		
		origOffset.setFXYZ(halfHolderSizeInPixels,halfHolderSizeInPixels,halfHolderSizeInPixels);

		offsetInHolders.setIXYZ(trueX,trueY,trueZ);
		offsetInBlocks.copyFrom(&offsetInHolders);
		offsetInBlocks.intDivXYZ(singleton->blockSizeInHolders);
		
		gphMinInPixels.copyFrom(&offsetInHolders);
		gphMaxInPixels.copyFrom(&offsetInHolders);

		gphMaxInPixels.addXYZ(1);

		gphMinInPixels.multXYZ(singleton->holderSizeInPixels);
		gphMaxInPixels.multXYZ(singleton->holderSizeInPixels);

		gphCenInPixels.copyFrom(&gphMaxInPixels);
		gphCenInPixels.addXYZRef(&gphMinInPixels);
		gphCenInPixels.multXYZ(0.5f);
		
		holderSizeInPages = singleton->holderSizeInPages;
		iPageDataVolume = holderSizeInPages*holderSizeInPages*holderSizeInPages;
		pageData = new GamePage*[iPageDataVolume];
		
		

		for (i = 0; i < iPageDataVolume; i++) {
			pageData[i] = NULL;
		}
		
		
		refreshGeom();
		
		
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
void GamePageHolder::doRefreshDL ()
                           {
		
		refreshDL = false;
		
		int i;
		int j;
		int ind;
		int numVert = 8;
		int maxVal;
		
		int p;
		int q;
		int ci;
		
		
		// TODO: if the holder was cleared entirely, must make sure to clear all displaylists
		
		
		if (gpuRes != NULL) {
			
			for (q = 0; q < MAX_LAYERS; q++) {
				for (p = 0; p < MAX_MIP_LEV; p++) {
					
					ci = p*MAX_LAYERS+q;
					
					gpuRes->bindHolderDL(p,q);
					glBegin(GL_POINTS);
					
					
					for (j = 0; j < iPageDataVolume; j++) {
						ind = j;
						
						if (pageData[ind] != NULL) {
							
							
							hasVerts[q] = hasVerts[q] || (pageData[ind]->vertices[ci].data.size() > 0);
							
							maxVal = (pageData[ind]->vertices[ci].data.size())/numVert;
							
							for (i = 0; i < maxVal; i++ ) {
								
								glMultiTexCoord4f(
									GL_TEXTURE0,
									pageData[ind]->vertices[ci].data[i*numVert + 0],
									pageData[ind]->vertices[ci].data[i*numVert + 1],
									pageData[ind]->vertices[ci].data[i*numVert + 2],
									pageData[ind]->vertices[ci].data[i*numVert + 3]
									
								);
								
								glVertex3f(
									pageData[ind]->vertices[ci].data[i*numVert + 4],
									pageData[ind]->vertices[ci].data[i*numVert + 5],
									pageData[ind]->vertices[ci].data[i*numVert + 6]	
								);
								
								
							}
						}
					}
					
					
					
					
					glEnd();
					gpuRes->unbindHolderDL(p,q);
				}
			}
			
			
			
			
			
		}
		
	}
GamePage * GamePageHolder::getPageAtCoordsLocal (int x, int y, int z, bool createOnNull)
                                                                                       {
		

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
void GamePageHolder::refreshGeom ()
                           {
		if (isEntity) {
			entityGeomCounter = 0;
			addNewLinesGeom(singleton->testHuman->baseNode, singleton->pixelsPerCell);
		}
		else {
			fetchGeom();
		}
	}
void GamePageHolder::clearSet ()
                        { //bool forceClear
		int i;
		

		

		//bool doClear = forceClear;

		if (usingPoolId == -1) {
			
			if (isEntity) {
				usingPoolId = singleton->entityPool->requestPoolId(blockId,holderId);
				gpuRes = singleton->entityPool->holderPoolItems[usingPoolId];
			}
			else {
				usingPoolId = singleton->gpuPool->requestPoolId(blockId,holderId);
				gpuRes = singleton->gpuPool->holderPoolItems[usingPoolId];
			}
			

			readyForClear = true;
		}

		if (readyForClear) {
			readyForClear = false;
			
			// if (true) { //freeCam
				
			// }
			// else {
			// 	for (i = 0; i < MAX_LAYERS; i++) {
			// 		// clear fbo by binding it with auto flag
			// 		singleton->bindFBODirect(gpuRes->getFBOS(i));
			// 		singleton->unbindFBO();
			// 	}
			// }
			
		}
		
	}
int GamePageHolder::passiveRefresh ()
                             {
		int i;
		int changeCount;
		
		childrenDirty = false;
		changeCount = 0;

		//bool finished = true;
				

		for (i = 0; i < iPageDataVolume; i++) {
			
			if (changeCount >= singleton->maxChangesInPages) {
				childrenDirty = true;
				//finished = false;
				break;
			}
			
			if (pageData[i] == NULL) {
				
			}
			else {
				pageData[i]->addAllGeom();
				if (
					(pageData[i]->hasSolids || pageData[i]->hasTrans)
				) {
					if (pageData[i]->isDirty) {
						pageData[i]->generateVolume();
						changeCount++;
					}
				}
				
				
				
			}
		}
		
		if (refreshDL&&(!childrenDirty)) {
			doRefreshDL();
		}
		
		
		
		
		return changeCount;	

	}
void GamePageHolder::refreshChildren (bool refreshImmediate, bool clearEverything, bool refreshUnderground)
          {
		int i;
		
		
		
		
		
		
		readyForClear = true;
		if (clearEverything) {
			clearSet();
		}
		
		for (i = 0; i < iPageDataVolume; i++) {
			if (pageData[i] == NULL) {

			}
			else {
				
				
				if (refreshImmediate) {
					
					pageData[i]->addAllGeom();
					
					if (
						(
							(pageData[i]->hasSolids || pageData[i]->hasTrans)
						) ||
						refreshUnderground ||
						isEntity
					) {
						if (refreshUnderground) {
							pageData[i]->isDirty = true;
						}
						pageData[i]->generateVolume(refreshUnderground);
					}
					childrenDirty = false;
				}
				else {
					childrenDirty = true;
					pageData[i]->isDirty = true;
				}
				
				
				
			}
		}
		
		if (refreshImmediate) {
			doRefreshDL();
		}
		

		
	}
void GamePageHolder::addNewGeom (int _curBT, int _curAlign, float _baseOffset, FIVector4 * _p1, FIVector4 * _p2, FIVector4 * _rad, FIVector4 * _cornerRad, FIVector4 * _visInsetFromMin, FIVector4 * _visInsetFromMax, FIVector4 * _powerVals, FIVector4 * _powerVals2, FIVector4 * _thickVals, FIVector4 * _matParams, FIVector4 * _centerPoint, FIVector4 * _anchorPoint, int _minRot, int _maxRot)
          {
		
		if (entityGeomCounter >= entityGeom.size()) {
			entityGeom.push_back(new GameEnt());
		}
		
		
		entityGeom[entityGeomCounter]->initBounds(
			_curBT,
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
void GamePageHolder::addNewLinesGeom (GameOrgNode * curNode, float scale)
          {
		
		int i;
		
		
		if (curNode->parent == NULL) {
			
		}
		else {
			
			if (curNode == singleton->selectedNode) {
				tempVec.setFXYZ(E_ORG_PARAM_LINES,10.0f,entityGeomCounter);
			}
			else {
				tempVec.setFXYZ(E_ORG_PARAM_LINES,curNode->orgVecs[E_OV_MATPARAMS].getFX(),entityGeomCounter);
			}
			
			//tempVec2.copyFrom(&(curNode->tbnRotC[0]));
			//tempVec2.multXYZ(curNode->boneLengthHalf*scale); //*curNode->boneLengthScale
			
			
			if (entityGeomCounter >= entityGeom.size()) {
				entityGeom.push_back(new GameEnt());
			}
			
			entityGeom[entityGeomCounter]->initLines(
				E_CT_LINES,
				scale,
				
				&origOffset,
				
				&(curNode->orgTrans[1]),
				&(curNode->tbnRotC[0]), //&(tempVec2),
				&(curNode->tbnRotC[1]),
				&(curNode->tbnRotC[2]),
				&(curNode->orgVecs[E_OV_TBNRAD0]),
				&(curNode->orgVecs[E_OV_TBNRAD1]),
				//&(curNode->tbnRadScale0),
				//&(curNode->tbnRadScale1),
				&tempVec
				
			);
			entityGeomCounter++;
		}
		
		
		
		for (i = 0; i < curNode->children.size(); i++) {
			addNewLinesGeom(curNode->children[i],scale);
		}
		
		
	}
void GamePageHolder::fetchGeom ()
                         {
		int i;
		int j;
		int k;
		int n;
		int bufSize = (singleton->visPageSizeInPixels*singleton->bufferMult)*2;
		
		GameBlock* curBlock;
		GamePageHolder* gph;
		FIVector4 start;
		FIVector4 end;
		GameEnt* gameEnt;


		for (n = 0; n < E_ET_LENGTH; n++) {
			containsEntIds[n].data.clear();
			
			for (i = -1; i <= 1; i++) {
				for (j = -1; j <= 1; j++) {
					curBlock = singleton->gw->getBlockAtCoords(
						offsetInBlocks.getIX()+i,
						offsetInBlocks.getIY()+j,
						true
					);

					for (k = 0; k < curBlock->gameEnts[n].data.size(); k++) {


						gameEnt = &(curBlock->gameEnts[n].data[k]);


						start.copyFrom( &(gameEnt->moveMinInPixels) );
						end.copyFrom( &(gameEnt->moveMaxInPixels) );

						start.addXYZ(-bufSize);
						end.addXYZ(bufSize);

						//start.intDivXYZ(singleton->holderSizeInPixels);
						//end.intDivXYZ(singleton->holderSizeInPixels);

						start.clampZ(0.0,singleton->worldSizeInPixels.getFZ()-1.0f);
						end.clampZ(0.0,singleton->worldSizeInPixels.getFZ()-1.0f);

						if (FIVector4::intersect(&start,&end,&gphMinInPixels,&gphMaxInPixels)) {
							containsEntIds[n].data.push_back(intPair());
							containsEntIds[n].data.back().v0 = curBlock->blockId;
							containsEntIds[n].data.back().v1 = k;
						}
					}
				}
			}
		}


		
	}
void GamePageHolder::unbindGPUResources ()
                                  {

		usingPoolId = -1;
		gpuRes = NULL;
		childrenDirty = true;

	}
#undef LZZ_INLINE
 
