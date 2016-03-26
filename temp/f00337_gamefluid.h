// f00337_gamefluid.h
//

#include "f00337_gamefluid.e"
#define LZZ_INLINE inline
int GameFluid::getFluidId (int groupNum, int ind)
                                              {
		return fluidIds[fsVec[groupNum].fidBegInd + ind];
	}
int GameFluid::getFIDSize (int groupNum)
                                     {
		return fsVec[groupNum].fidEndInd - fsVec[groupNum].fidBegInd;
	}
void GameFluid::fidPushBack (int groupNum, int val)
                                                {
		fluidIds[fsVec[groupNum].fidEndInd] = val;
		fsVec[groupNum].fidEndInd++;
	}
int * GameFluid::fidGetBeg (int groupNum)
                                     {
		return fluidIds + fsVec[groupNum].fidBegInd;
	}
int * GameFluid::fidGetEnd (int groupNum)
                                     {
		return fluidIds + fsVec[groupNum].fidEndInd;
	}
int GameFluid::getIdealCellId (int groupNum, int ind)
                                                  {
		return idealCellIds[fsVec[groupNum].iciBegInd + ind];
	}
int GameFluid::getICISize (int groupNum)
                                     {
		return fsVec[groupNum].iciEndInd - fsVec[groupNum].iciBegInd;
	}
void GameFluid::iciPushBack (int groupNum, int val)
                                                {
		idealCellIds[fsVec[groupNum].iciEndInd] = val;
		fsVec[groupNum].iciEndInd++;
	}
GameFluid::GameFluid ()
                    {
		
		waitingOnThreads = false;
		fluidChanged = false;
		posShifted = false;
		hasRead = false;
		
		UNIT_MIN = FLUID_UNIT_MIN;
		UNIT_MAX = FLUID_UNIT_MAX;
		
		UNIT_INSIDE = FLUID_UNIT_MIN + 8;
		
		invalidated = true;
		
		F_UNIT_MIN = UNIT_MIN;
		F_UNIT_MAX = UNIT_MAX;
		
		watchMinX = -1;
		watchMaxX = -1;
		watchMinY = -1;
		watchMaxY = -1;
		watchMinZ = -1;
		watchMaxZ = -1;
		
	}
void GameFluid::init (Singleton * _singleton, int _mainId)
                                                      {
		int i;
		
		singleton = _singleton;
		mainId = _mainId;
		
		// qqqqqq
		// WAS DOING: LEVEL OUT TOP OF WATER BY SEARCHING FOR WATER RIGHT BELOW TOP
		// AND FILLING WITH ZERO VALUE WATER IF THERE IS
		
		//volSizes[E_FID_SML] = 64;
		volSizes[E_FID_BIG] = DEF_VOL_SIZE;
		mipSizes[E_FID_BIG] = 1;
		
		// volSizes[E_FID_MIP] = 256;
		// mipSizes[E_FID_MIP] = 4;
		
		waterTick = 0;
		waterTickMax = 32;
		waterTickMaxDiv = 64;
		waterTickReady = false;
		
		
		
		volSizePrim = volSizes[mainId]/mipSizes[mainId];
		cellsPerBlock = singleton->cellsPerBlock;
		cellsPerHolder = singleton->cellsPerHolder;
		
		readyForTermination = false;
		cycleTerminated = false;
		proceedingToRead = false;
		fluidReading = false;
		modifiedUnit = false;
		modifiedGeom = false;
		readyForTBOUpdate = false;
		firstVPUpdate = true;
		
		internalPrimFormat = GL_RGBA;
		precPrimFormat = GL_UNSIGNED_BYTE;
		
		curGeomCount = 0;
		waterLerp = 0.0f;
		primDiv = 4;
		volSizePrimMacro = volSizePrim/primDiv;
		primsPerMacro = 8;
		floatsPerPrimEntry = 8;
		floatsInPrimMacro = volSizePrimMacro*volSizePrimMacro*volSizePrimMacro*primsPerMacro*floatsPerPrimEntry;
		
		
		
		
		
		
		threadLoader.init();
		threadTex.init();
		threadFluid.init();
		
		
		camPosVP.setFXYZ(0.0f,0.0f,0.0f);
		lastCamPosVP.setFXYZ(0.0f,0.0f,0.0f);//(-100.0f,-100.0f,-100.0f);
		
		
		bufAmount = 1;
		
		if (mipSizes[mainId] > 1) {
			bufAmount = 0;
		}
		
		volSizePrimBuf = volSizePrim+bufAmount*2;
		
		forceFullRefresh = 2;
		
		shrinkCount = 0;
		
		//curTick = 0;
		//maxTicks = 1;//volSizePrim/8;
		
		vspMin = bufAmount;
		vspMax = volSizePrimBuf-bufAmount;
		
		fluidData = new int[volSizePrimBuf*volSizePrimBuf*volSizePrimBuf*4];
		extraData = new int[volSizePrimBuf*volSizePrimBuf*volSizePrimBuf*4];
		
		fluidIds = new int[volSizePrimBuf*volSizePrimBuf*volSizePrimBuf];
		idealCellIds = new int[volSizePrimBuf*volSizePrimBuf*volSizePrimBuf];
		
		
		totSize = volSizePrimBuf*volSizePrimBuf*volSizePrimBuf;
		
		
		
		for (i = 0; i < E_PL_LENGTH; i++) {
			volDataPrim[i] = new uint[volSizePrim*volSizePrim*volSizePrim];
		}
		
		//maxDirtyRegion();
		
		tboData = new float[floatsInPrimMacro];
		
		
		
		
		setupPrimTexture();
		
	}
void GameFluid::flushActionStack ()
                                {
		PushModStruct* curPM;
		
		
		
		while (pmStack.size() > 0) {
			
			//cout << "flushActionStack()\n";
			
			curPM = &(pmStack.back());
			
			switch(curPM->actionType) {
				case E_PM_EXPLODE_BULLET:
					pushExplodeBullet(
						false,
						&(curPM->data[0]),
						curPM->data[1].getIX(),
						curPM->data[2].getFX()
					);
				break;
				case E_PM_MODIFY_UNIT:
					pushModifyUnit(
						false,
						&(curPM->data[0]),
						curPM->data[1].getIX(),
						curPM->data[1].getIY(),
						curPM->data[1].getIZ()
					);
				break;
				case E_PM_PLACE_TEMPLATE:
					pushPlaceTemplate(false,&(curPM->data[0]), curPM->data[1].getIX());
				break;
			}
			
			pmStack.pop_back();
		}
	}
void GameFluid::pushExplodeBullet (bool isReq, FIVector4 * newPos, int waterBulletOn, float newRad)
                                                                                               {
		
		
		if (isReq) {
			pmStack.push_back(PushModStruct());
			pmStack.back().actionType = E_PM_EXPLODE_BULLET;
			pmStack.back().data[0].copyFrom(newPos);
			pmStack.back().data[1].setIX(waterBulletOn);
			pmStack.back().data[2].setFX(newRad);
			return;
		}
		
		if (waterBulletOn > 0) {
			modifyUnit(newPos, E_BRUSH_ADD, E_PTT_WAT, newRad);
		}
		else {
			modifyUnit(newPos, E_BRUSH_SUB, E_PTT_TER, newRad);
		}
		
		singleton->waitingOnDestruction = true;
		singleton->destructCount = 0;
		
		modifiedUnit = true;
		
		
	}
void GameFluid::pushModifyUnit (bool isReq, FIVector4 * mp, int buttonNum, int earthMod, int curBrushRad)
                                                                                                     {
		
		cout << "pushModifyUnit " << isReq << "\n";
		
		if (isReq) {
			pmStack.push_back(PushModStruct());
			pmStack.back().actionType = E_PM_MODIFY_UNIT;
			pmStack.back().data[0].copyFrom(mp);
			pmStack.back().data[1].setIXYZ(buttonNum,earthMod,curBrushRad);
			return;
		}
		
		//bool lbClicked, bool rbClicked, bool mbClicked
		
		switch (buttonNum) {
			case 0: //lb
				modifyUnit(mp, E_BRUSH_ADD, earthMod, curBrushRad);
			break;	
			case 1: //rb
				modifyUnit(mp, E_BRUSH_SUB, earthMod, curBrushRad);
			break;	
			case 2: //mb
				modifyUnit(mp, E_BRUSH_REF, earthMod, curBrushRad);
			break;	
		}
		
		modifiedUnit = true;
		
		
		
		
	}
void GameFluid::pushPlaceTemplate (bool isReq, FIVector4 * newPos, int pt)
                                                                      {
		
		if (isReq) {
			pmStack.push_back(PushModStruct());
			pmStack.back().actionType = E_PM_PLACE_TEMPLATE;
			pmStack.back().data[0].copyFrom(newPos);
			pmStack.back().data[1].setIX(pt);
			return;
		}
		
		addGeom(newPos,pt);
		
		modifiedGeom = true;
		
		
	}
bool GameFluid::addPrimObj (FIVector4 * pos, int tempId, int uid)
                                                             {
		int baseInd = tempId*E_PRIMTEMP_LENGTH;
		int i;
		int j;
		int k;
		int m;
		int ind;
		float fPrimDiv = 1.0f/primDiv;
		//float cornerDis = singleton->primTemplateStack[baseInd+E_PRIMTEMP_CORNERDIS].getFX();
		
		tempBoundsMin.copyFrom(&(singleton->primTemplateStack[baseInd+E_PRIMTEMP_VISMIN]));
		tempBoundsMax.copyFrom(&(singleton->primTemplateStack[baseInd+E_PRIMTEMP_VISMAX]));
		
		tempBoundsMin.addXYZRef(pos);
		tempBoundsMax.addXYZRef(pos);
		
		tempBoundsMin.addXYZRef(&volMinReadyInPixels,-1.0f);
		tempBoundsMax.addXYZRef(&volMinReadyInPixels,-1.0f);
		
		//tempBoundsMin.addXYZ(-volSizePrim/2);
		//tempBoundsMax.addXYZ(-volSizePrim/2);
		
		
		
		//tempBoundsMin.addXYZ(-cornerDis);
		//tempBoundsMax.addXYZ(cornerDis);
		
		tempBoundsMin.multXYZ(fPrimDiv);
		tempBoundsMax.multXYZ(fPrimDiv);
		
		//tempBoundsMin.intDivXYZ(primDiv);
		//tempBoundsMax.intDivXYZ(primDiv);
		
		
		
		// tempBoundsMin.addXYZ(0.0f, 0.0f, 2.0f);
		// tempBoundsMax.addXYZ(0.0f, 0.0f, 2.0f);
		
		
		
		int iMin = max(tempBoundsMin[0],0.0f);
		int jMin = max(tempBoundsMin[1],0.0f);
		int kMin = max(tempBoundsMin[2],0.0f);
		
		int iMax = min(tempBoundsMax[0],volSizePrimMacro-1.0f);
		int jMax = min(tempBoundsMax[1],volSizePrimMacro-1.0f);
		int kMax = min(tempBoundsMax[2],volSizePrimMacro-1.0f);
		
		bool wasAdded = false;
		
		for (i = iMin; i <= iMax; i++) {
			for (j = jMin; j <= jMax; j++) {
				for (k = kMin; k <= kMax; k++) {
					for (m = 0; m < primsPerMacro; m++) {
						
						ind = (
							(
								i +
								j*volSizePrimMacro +
								k*volSizePrimMacro*volSizePrimMacro
							)*primsPerMacro + m
						)*floatsPerPrimEntry;
						
						if (tboData[ind+3] == 0.0f) {
							
							wasAdded = true;
							
							tboData[ind+0] = pos->getFX();
							tboData[ind+1] = pos->getFY();
							tboData[ind+2] = pos->getFZ();
							tboData[ind+3] = tempId;
							
							tboData[ind+4] = 0;//diagCount;//;
							tboData[ind+5] = uid + 1; // uid of 0 results in blank object
							tboData[ind+6] = 0.0f;
							tboData[ind+7] = 0.0f;
							
							break;
						}
					}
				}
			}
		}
		
		return wasAdded;
	}
void GameFluid::addGeom (FIVector4 * newPos, int templateId)
                                                        { //FIVector4* pos, FIVector4* offset
		
		FIVector4 camBlockPos;
		GameBlock* curBlock;
		
		
		
		camBlockPos.copyFrom(newPos);
		camBlockPos.intDivXYZ(cellsPerBlock);
		
		curBlock = singleton->gw->getBlockAtCoords(
			camBlockPos.getIX(),
			camBlockPos.getIY(),
			camBlockPos.getIZ(),
			true
		);
		
		curBlock->gameEnts[E_ET_GEOM].data.push_back(GameEnt());
		
		GameEnt* gameEnt = &(curBlock->gameEnts[E_ET_GEOM].data.back());
		
		gameEnt->templateId = templateId;
		gameEnt->templatePos.copyFrom(newPos);
	}
void GameFluid::fetchGeom ()
                         {
		
		
		int i;
		int j;
		int k;
		
		int m;
		
		GameBlock* curBlock;
		GameEnt* gameEnt;
		FIVector4 start;
		FIVector4 end;
		FIVector4 avg;

		
		
		int geomCount = 0;
		int tempId;
		
		FIVector4 camBlockPos;
		
		camBlockPos.averageXYZ(&volMinReadyInPixels,&volMaxReadyInPixels);
		camBlockPos.intDivXYZ(cellsPerBlock);
		
		for (i = -1; i <= 1; i++) {
			for (j = -1; j <= 1; j++) {
				for (k = -1; k <= 1; k++) {
					
					
					curBlock = singleton->gw->getBlockAtCoords(
						camBlockPos.getIX()+i,
						camBlockPos.getIY()+j,
						camBlockPos.getIZ()+k,
						true
					);

					for (m = 0; m < curBlock->gameEnts[E_ET_GEOM].data.size(); m++) {

						gameEnt = &(curBlock->gameEnts[E_ET_GEOM].data[m]);

						tempId = gameEnt->templateId;
						

						
						

						//if (FIVector4::intersect(&start,&end,&volMinReadyInPixels,&volMaxReadyInPixels)) {
							
							//avg.averageXYZ(&start,&end);
							
							if( addPrimObj(&(gameEnt->templatePos), tempId, geomCount) ) {
								geomCount++;
							}
							
							
						//}
					}
						
				}
			}
		}
		
		curGeomCount = geomCount;
		
		//cout << geomCount << " <-- geomCount\n";


		
	}
void GameFluid::setupPrimTexture ()
                                {
		int i;

		

		for (i = 0; i < E_PL_LENGTH; i++) {
			
			
			glGenTextures(1, &(volIdPrim[i]));
			
			//cout << "vp " << volIdPrim[i] << "\n";
			
			glBindTexture(GL_TEXTURE_3D, volIdPrim[i]);
			glTexImage3D(
				GL_TEXTURE_3D,
				0,
				internalPrimFormat, //GL_RGBA, // GL_RGBA32F
				volSizePrim,
				volSizePrim,
				volSizePrim,
				0,
				GL_RGBA,
				precPrimFormat, // GL_UNSIGNED_BYTE, // GL_FLOAT
				0
			);
			glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			//glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_BORDER_COLOR, 0);
			glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE); //GL_CLAMP_TO_BORDER
			glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
			glBindTexture(GL_TEXTURE_3D, 0);
			
			
		}
		
		
	}
bool GameFluid::anyThreadsRunning ()
                                 {
		return threadLoader.threadRunning||threadTex.threadRunning||threadFluid.threadRunning;
	}
bool GameFluid::updateAll ()
                         {
		
		
		if (cycleTerminated) {
			
			return true;
		}
		
		waterTick++;		
		if (waterTick == waterTickMax) {
			waterTickReady = true;	
		}
		
		if (waterTick > waterTickMaxDiv) {
			waterTick = waterTickMaxDiv;
		}
		
		waterLerp = ((float)waterTick)/((float)waterTickMaxDiv);
		
		
		
		
		//cout << "still going " << mainId << "\n";
		
		
		if (fluidReading) {
			
		}
		else {
			/////////
			
			if (threadTex.threadRunning) {
				
			}
			else {
				
				
				
				
				
				if (waterTickReady) {
					
					
					
					if (threadFluid.isReady()) {
						
						waterTickReady = false;
						
						stopFT();
						
						// cout << "\n\nimmobileHeight " << immobileHeight << "\n";
						// cout << "maxWaterHeight " << maxWaterHeight << "\n\n";
						
						flushActionStack();
						
						if (modifiedUnit) {
							applyMods();
						}
						
						if (modifiedGeom) {	
							fillAllGeom();
							updateTBOData(false,false);
						}
						
						if (modifiedUnit||modifiedGeom) {
							singleton->refreshPaths = true;
						}
						
						//DirtyRegion
						if (modifiedUnit||modifiedGeom||fluidChanged) {
							
							if (fluidChanged) {
								//cout << "fluidChanged (max/imobile)" << maxWaterHeight << " " << immobileHeight << "\n";
							}
							
							
							if (hasRead&&(!firstVPUpdate)) {
								writeMIP.copyFrom(&volMinInPixels); //volMinInPixels
								
								if (singleton->updateFluid) {
									//if (mainId == E_FID_SML) {
										singleton->gameLogic->threadPoolPath->stopAll();
										singleton->gameLogic->threadPoolList->stopAll();
										writeFluidData();
									//}
								}
								
								
							}
							
						}
							
						modifiedUnit = false;
						modifiedGeom = false;
						
						forceFullRefresh = 1; // todo: should not require this?
						
						startTT();
					}
				}
			}
			
			
			if (threadFluid.threadRunning) {
				
			}
			else {
				
				
				if (threadTex.isReady()) {
					stopTT();
					
					
					volMinReadyInPixels.copyFrom(&volMinInPixels);
					volMaxReadyInPixels.copyFrom(&volMaxInPixels);
					
					if (readyForTBOUpdate) {
						readyForTBOUpdate = false;
						fetchGeom();
						tboWrapper.update(tboData,NULL,-1);
					}
					
					waterTick = 0;
					waterLerp = ((float)waterTick)/((float)waterTickMaxDiv);
					
					//if (mainId == E_FID_BIG) {
						copyPrimTexture(
							0,
							0,
							0,
							volSizePrim,
							&(volDataPrim[0])
						);
					//}
					
					
					
					curDirtyMax.copyFrom(&(dirtyMax));
					curDirtyMin.copyFrom(&(dirtyMin));
					curWaterMin.copyFrom(&tempMin2);
					curWaterMax.copyFrom(&tempMax2);
					//curWaterMin.addXYZRef(&volMinReadyInPixels);
					//curWaterMax.addXYZRef(&volMinReadyInPixels);
					//shrinkDirtyRegion();
					resetDirtyRegion();
					
					
					shiftRegion();
					
					if (fluidReading) {
						
					}
					else {
						terminateCycle();
					}
				}
				
				
				
			}
			
			////////////////
		}
		return false;
	}
void GameFluid::copyPrimTexture (int ox, int oy, int oz, int dim, uint * * myData)
                                                                             {
		
		
		int i;
		
		for (i = 0; i < E_PL_LENGTH; i++) {
			
			
			glBindTexture(GL_TEXTURE_3D, volIdPrim[i]);
			glTexSubImage3D(
				GL_TEXTURE_3D,
				0,

				ox,
				oy,
				oz,

				dim,
				dim,
				dim,

				GL_RGBA,
				precPrimFormat, //GL_UNSIGNED_BYTE,

				myData[i]
			);
			glBindTexture(GL_TEXTURE_3D, 0);
		}
		
		//singleton->depthInvalidMove = true;
		
	}
void GameFluid::fillAllGeom ()
                           {
		
		clearAllGeom();
		
		int i;
		int j;
		int k;
		
		int m;
		
		GameBlock* curBlock;
		GameEnt* gameEnt;
		FIVector4 start;
		FIVector4 end;
		FIVector4 avg;

		
		
		int geomCount = 0;
		int tempId;
		
		FIVector4 camBlockPos;
		
		camBlockPos.averageXYZ(&volMinReadyInPixels,&volMaxReadyInPixels);
		camBlockPos.intDivXYZ(cellsPerBlock);
		
		int baseInd;
		
		
		for (i = -1; i <= 1; i++) {
			for (j = -1; j <= 1; j++) {
				for (k = -1; k <= 1; k++) {
					
					curBlock = singleton->gw->getBlockAtCoords(
						camBlockPos.getIX()+i,
						camBlockPos.getIY()+j,
						camBlockPos.getIZ()+k,
						true
					);

					for (m = 0; m < curBlock->gameEnts[E_ET_GEOM].data.size(); m++) {

						gameEnt = &(curBlock->gameEnts[E_ET_GEOM].data[m]);

						tempId = gameEnt->templateId;
						baseInd = tempId*E_PRIMTEMP_LENGTH;

						start.copyFrom(&(singleton->primTemplateStack[baseInd+E_PRIMTEMP_VISMIN]));
						end.copyFrom(&(singleton->primTemplateStack[baseInd+E_PRIMTEMP_VISMAX]));
						start.addXYZRef(&(gameEnt->templatePos));
						end.addXYZRef(&(gameEnt->templatePos));
						
						

						if (FIVector4::intersect(&start,&end,&volMinReadyInPixels,&volMaxReadyInPixels)) {
							
							//cout << "fillCurrentGeom" << tempId;
							
							fillCurrentGeom(tempId, &(gameEnt->templatePos));
							
							// baseInd = curPrimTemplate*E_PRIMTEMP_LENGTH;
							
							// for (j = 0; j < E_PRIMTEMP_LENGTH; j++) {
							// 	setFXYZWGeom(j, &(singleton->primTemplateStack[baseInd+j]) );
							// }
							
						}
					}
				}
				
				
			}
		}
		
		
		clearInsideValues();
		
	}
void GameFluid::updateTBOData (bool firstTime, bool reloadTemplates)
                                                                 {
		int i;
		int ind;
		
		// if (mainId == E_FID_SML) {
		// 	// no need to update geometry visuals for small buffer
		// 	return;
		// }
		
		
		if (firstTime||reloadTemplates) {
			if (singleton->getPrimTemplateString()) {
				
			}
			else {
				return;
			}
		}
		
		int totCount = 0;
		
		
		// clear tbo data
		
		for (i = 0; i < floatsInPrimMacro/floatsPerPrimEntry; i++) {
			
			ind = i*floatsPerPrimEntry;
			
			tboData[ind + 0] = 0.0f;
			tboData[ind + 1] = 0.0f;
			tboData[ind + 2] = 0.0f;
			tboData[ind + 3] = 0.0f;
			
			tboData[ind + 4] = 0.0f;
			tboData[ind + 5] = 0.0f;
			tboData[ind + 6] = 0.0f;
			tboData[ind + 7] = 0.0f;
			
		}
		
		
		
		if (firstTime) {
			tboWrapper.init(true,tboData,NULL,floatsInPrimMacro*4);
		}
		else {
			readyForTBOUpdate = true;
		}
		
	}
void GameFluid::terminateCycle ()
                              {
		readyForTermination = true;
	}
void GameFluid::beginFluidRead (FIVector4 * _campPosVPDump)
                                                       {
		
		//fdWritePos = 0;
		//fdReadPos = 0;
		
		//cout << "pos change " << mainId << "\n";
		fluidReading = true;
		campPosVPDump.copyFrom(_campPosVPDump);
		terminateCycle();
	}
bool GameFluid::tryToEndRead ()
                            {
		
		
		if (threadLoader.isReady()) {
			
			
			stopTL();
			endFluidRead();
			//cycleTerminated = false;
			
			
			//if (mainId == E_FID_SML) {
			//	startFT();
			//}
			
			return true;
		}
		else {
			return false;
		}
	}
void GameFluid::tryToEndThreads ()
                               {
		
		if (singleton->gameLogic->anyThreadsRunning()) {
			
		}
		else {
			waitingOnThreads = false;
			// singleton->gameLogic->threadPoolPath->stopAll();
			// singleton->gameLogic->threadPoolList->stopAll();
			prereadFluidData();
			singleton->gameLogic->allowThreadCreation = true;
			
			startTL();
		}
		
		
		
	}
void GameFluid::proceedWithRead ()
                               {
		
		
		proceedingToRead = true;
		//
		camPosVPInPixels.copyFrom( &campPosVPDump );
		camPosVPInPixels.multXYZ( cellsPerHolder );
		volMinInPixels.copyFrom( &camPosVPInPixels );
		volMaxInPixels.copyFrom( &camPosVPInPixels );
		volMaxInPixels.addXYZ(volSizePrim);				
		//
		
		
		readMIP.copyFrom(&volMinInPixels);
		
		singleton->gameLogic->allowThreadCreation = false;
		
		waitingOnThreads = true;
		
	}
void GameFluid::endFluidRead ()
                            {
		
		
		
		//cout << "pos change end " << mainId << "\n";
		fluidReading = false;
		proceedingToRead = false;
		
		
		updateTBOData(false,false);
		lastCamPosVP.setFXYZRef(&campPosVPDump);
		
		if (firstVPUpdate) {
			firstVPUpdate = false;
			volMinReadyInPixels.copyFrom(&volMinInPixels);
			volMaxReadyInPixels.copyFrom(&volMaxInPixels);
		}
		
		hasRead = true;
		posShifted = true;
		
		
	}
void GameFluid::shiftRegion ()
                           {
		
		
		bool notThirdPerson = (singleton->gem->getCurActor() == NULL);
		
		if (notThirdPerson) {
			newCamPos.copyFrom(singleton->cameraGetPosNoShake());
		}
		else {
			newCamPos.setBTV(singleton->gem->getCurActor()->getCenterPoint(0));
		}
		
		if (notThirdPerson&&(volSizePrim < 512)) { // && (mainId==E_FID_SML)
			newCamPos.addXYZRef(&singleton->lookAtVec,volSizePrim*0.4f);
		}
		
		camPosVP.copyFrom(&newCamPos);
		camPosVP.addXYZ(-volSizePrim*0.5f + cellsPerHolder*0.5f);
		
		
		
		camPosVP.intDivXYZ(cellsPerHolder);
		
		
		
		
		
		if (
			camPosVP.iNotEqual(&lastCamPosVP) && 
			(lastCamPos.distance(&newCamPos) > cellsPerHolder/16.0f)
		) {
			lastCamPos.copyFrom(&newCamPos);
			forceFullRefresh = 1;
			
			
			
			beginFluidRead(&camPosVP);
			
		}
		
	}
void GameFluid::funcFT ()
                      {
		threadFluid.setRunningLocked(true);
		
		if (singleton->updateFluid) {
			//if (mainId==E_FID_SML) {
				fluidChanged = updateFluidData();
			//}
		}
		
		
		threadFluid.setRunningLocked(false);
	}
void GameFluid::startFT ()
                       {
		if (threadFluid.threadRunning) {
			
		}
		else {
			threadFluid.threadRunning = true;
			
			if (SINGLE_THREADED) {
				funcFT();
			}
			else {
				threadFluid.threadMain = std::thread(&GameFluid::funcFT, this);
			}
			
			
		}
		
	}
bool GameFluid::stopFT ()
                      {
		bool didStop = false;
		
		if (threadFluid.threadRunning) {
			
			if (SINGLE_THREADED) {
				
			}
			else {
				threadFluid.threadMain.join();
			}
			
			threadFluid.threadRunning = false;
			didStop = true;
		}
		return didStop;
	}
void GameFluid::funcTL ()
                      {
		threadLoader.setRunningLocked(true);
		readFluidData();
		threadLoader.setRunningLocked(false);
	}
void GameFluid::startTL ()
                       {
		if (threadLoader.threadRunning) {
			
		}
		else {
			
			threadLoader.threadRunning = true;
			
			if (SINGLE_THREADED) {
				funcTL();
			}
			else {
				threadLoader.threadMain = std::thread(&GameFluid::funcTL, this);
			}
			
		}
		
	}
bool GameFluid::stopTL ()
                      {
		bool didStop = false;
		if (threadLoader.threadRunning) {
			
			if (SINGLE_THREADED) {
				
			}
			else {
				threadLoader.threadMain.join();
			}
			
			threadLoader.threadRunning = false;
			didStop = true;
		}
		return didStop;
	}
void GameFluid::funcTT ()
                      {
		threadTex.setRunningLocked(true);
		getPrimData(0);
		threadTex.setRunningLocked(false);
	}
void GameFluid::startTT ()
                       {
		if (threadTex.threadRunning) {
			
		}
		else {
			
			threadTex.threadRunning = true;
			
			if (SINGLE_THREADED) {
				funcTT();
			}
			else {
				threadTex.threadMain = std::thread(&GameFluid::funcTT, this);
			}
			
		}
		
	}
bool GameFluid::stopTT ()
                      {
		bool didStop = false;
		if (threadTex.threadRunning) {
			
			if (SINGLE_THREADED) {
				
			}
			else {
				threadTex.threadMain.join();
			}
			
			threadTex.threadRunning = false;
			didStop = true;
		}
		return didStop;
	}
void GameFluid::getPrimData (int n)
                                {
		
		// gggg
		
		//cout << "start\n";
		
		int i;
		int j;
		int k;
		int c;
		
		uint v0;
		uint v1;
		uint v2;
		uint v3;
		
		uint maxVal = 255;
		
		int indAbove;
		int indBelow;
		
		int indSrc;
		int indDest;
		
		int iMin = clamp(dirtyMin.getIX(), vspMin, vspMax);
		int iMax = clamp(dirtyMax.getIX(), vspMin, vspMax);
		int jMin = clamp(dirtyMin.getIY(), vspMin, vspMax);
		int jMax = clamp(dirtyMax.getIY(), vspMin, vspMax);
		int kMin = clamp(dirtyMin.getIZ(), vspMin, vspMax);
		int kMax = clamp(dirtyMax.getIZ(), vspMin, vspMax);
		
		if (forceFullRefresh > 0) {
		 	forceFullRefresh--;
			
			iMin = vspMin;
			iMax = vspMax;
			jMin = vspMin;
			jMax = vspMax;
			kMin = vspMin;
			kMax = vspMax;
		}
		
		
		float NEW_UNIT_MAX = UNIT_MAX + 1;
		
		uint* vdpPtr = (volDataPrim[n]);
		
		switch (n) {
			case E_PL_TERRAIN:
				
				// for (i = 0; i < totSize; i++) {
				// 	indSrc = i*4;
					
				// 	fluidData[indSrc+E_PTT_LST] += (fluidData[indSrc+E_PTT_LST] - fluidData[indSrc+E_PTT_WAT])/16;
					
				// 	if (fluidData[indSrc+E_PTT_LST] > fluidData[indSrc+E_PTT_WAT]) {
				// 		fluidData[indSrc+E_PTT_LST]--;
				// 	}
				// 	if (fluidData[indSrc+E_PTT_LST] < fluidData[indSrc+E_PTT_WAT]) {
				// 		fluidData[indSrc+E_PTT_LST]++;
				// 	}
				// }
				
				for (i = iMin; i < iMax; i++) {
					for (j = jMin; j < jMax; j++) {
						for (k = kMin; k < kMax; k++) {
							indSrc = (i + j*volSizePrimBuf + k*volSizePrimBuf*volSizePrimBuf)*4;
							//indAbove = indSrc+volSizePrimBuf*volSizePrimBuf*4;
							//indBelow = indSrc-volSizePrimBuf*volSizePrimBuf*4;
							indDest = ((i-bufAmount) + (j-bufAmount)*volSizePrim + (k-bufAmount)*volSizePrim*volSizePrim);
							
							v0 = ((fluidData[indSrc+0]+1)*255)/NEW_UNIT_MAX;
							v1 = ((fluidData[indSrc+1]+1)*255)/NEW_UNIT_MAX;
							v2 = ((fluidData[indSrc+2]+1)*255)/NEW_UNIT_MAX;
							v3 = ((fluidData[indSrc+3]+1)*255)/NEW_UNIT_MAX;
							
							
							// if (
							// 	(fluidData[indAbove+1] == -1) &&
							// 	(fluidData[indBelow+1] > -1)
							// ) {
							// 	v1 = 0;
							// }
							// if (
							// 	(fluidData[indAbove+2] == -1) &&
							// 	(fluidData[indBelow+2] > -1)
							// ) {
							// 	v2 = 0;
							// }
							
							
							if (v1 != 0) {
								v1 = (v1+510)/3;
							}
							if (v2 != 0) {
								v2 = (v2+510)/3;
							}
							
							v1 = min(v1,maxVal);
							v2 = min(v2,maxVal);
							
							vdpPtr[indDest] = 
								 (v0) |
								((v1) << 8) |
								((v2) << 16) |
								((v3) << 24);
								
							
							
						}
					}
				}
				
				
			break;
			
			// case E_PL_PRIMIDS:
				
			// break;
			
		}
		//cout << "end\n\n";
	}
void GameFluid::writeFluidData ()
                              {
		int i;
		int j;
		int k;
		
		int ind;
		
		//cout << "writeFluidData " << mainId << "\n";
		
		
		tempMin.copyFrom(&dirtyMin);
		tempMax.copyFrom(&dirtyMax);
		
		int iMin = clamp(tempMin.getIX()-1, vspMin, vspMax);
		int iMax = clamp(tempMax.getIX()+1, vspMin, vspMax);
		int jMin = clamp(tempMin.getIY()-1, vspMin, vspMax);
		int jMax = clamp(tempMax.getIY()+1, vspMin, vspMax);
		int kMin = clamp(tempMin.getIZ()-1, vspMin, vspMax);
		int kMax = clamp(tempMax.getIZ()+1, vspMin, vspMax);
		
		
		
		int ox = writeMIP.getIX();
		int oy = writeMIP.getIY();
		int oz = writeMIP.getIZ();
		
		//bool doFW = (mainId == E_FID_SML);
		
		if (hasRead) {
			
			for (k = kMin; k < kMax; k++) {
				
				for (j = jMin; j < jMax; j++) {
					
					for (i = iMin; i < iMax; i++) {
						
						
						ind = (i + j*volSizePrimBuf + k*volSizePrimBuf*volSizePrimBuf)*4;
						
						singleton->gw->setArrAtCoords(
							i-bufAmount+ox,
							j-bufAmount+oy,
							k-bufAmount+oz,
							//ind,
							&(fluidData[ind]),
							&(extraData[ind])
							//,doFW
						);
					}
				}
			}
			
		}
		
		singleton->refreshPaths = false;
		singleton->gameLogic->testPath.searchedForPath = false;
	}
void GameFluid::prereadFluidData ()
                                {
		
		GamePageHolder* curHolder;
		
		int i;
		int j;
		int k;
		int q;
		
		int ox = readMIP.getIX()/cellsPerHolder;
		int oy = readMIP.getIY()/cellsPerHolder;
		int oz = readMIP.getIZ()/cellsPerHolder;
		
		int maxVal = (volSizePrimBuf/cellsPerHolder) + 2;
		
		
		
		for (k = -2; k < maxVal; k++) {
			
			for (j = -2; j < maxVal; j++) {
				
				for (i = -2; i < maxVal; i++) {
					
					
					curHolder = singleton->gw->getHolderAtCoords(
						i+ox,
						j+oy,
						k+oz,
						true
					);
					
					if (curHolder->wasGenerated) {
						
					}
					else {
						
						curHolder->genCellData();
						
					}
				}
			}
		}
		
		
		
		
		
	}
void GameFluid::readFluidData ()
                             {
		
		int i;
		int j;
		int k;
		int q;
		
		
		int ind;
		int ind2;
		
		
		
		for (i = 0; i < totSize; i++) {
			ind = i*4;
			
			for (q = 0; q < 4; q++) {
				fluidData[ind+q] = UNIT_MIN;
				extraData[ind+q] = UNIT_MIN;
			}
			fluidData[ind+E_PTT_TER] = UNIT_MAX;
			
		}
		
		
		
		
		for (k = vspMin; k < vspMax; k++) {
			
			for (j = vspMin; j < vspMax; j++) {
				
				for (i = vspMin; i < vspMax; i++) {
					
					
					ind = (i + j*volSizePrimBuf + k*volSizePrimBuf*volSizePrimBuf)*4;
					
					singleton->gw->getArrAtCoords(
						(i-bufAmount)+readMIP.getIX(),
						(j-bufAmount)+readMIP.getIY(),
						(k-bufAmount)+readMIP.getIZ(),
						&(fluidData[ind]),
						&(extraData[ind])
					);
					
					
				}
				
				
			}
		}
		
		
	}
void GameFluid::applyMods ()
                         {
		while (modStack.size() > 0) {
			applyUnitModification(
				&(modStack.back().basePos),
				modStack.back().brushAction,
				modStack.back().modType,
				modStack.back().radius
			);
			modStack.pop_back();
		}
	}
bool GameFluid::passesCheck (int n)
                                {
		
		// true; // 
		return (getICISize(n) != 0); //fsVec[n].idealCellIds.size()
	}
bool GameFluid::updateFluidData ()
                               {
		
		// rrrr
		
		//maxDirtyRegion(); // todo: utilize dirty regions instad?
		
		int indAbove;
		int indBelow;
		int indSrc;
		
		int i;
		int j;
		int k;
		int c;
		int dir;
		int n;
		int p;
		int q;
		
		int tempv;
		
		int testInd;
		int testInd2;
		int testInd3;
		int testI;
		int testJ;
		int testK;
		
		int numCells;
		
		int ind;
		int ind2;
		
		int totWat;
		
		float fVSP = volSizePrimBuf;
		
		bool doProc;
		
		float fi;
		float fj;
		float fk;
		
		int ox;
		int oy;
		int oz;
		
		int curId;
		int tempi;
		
		int* bldVal;
		int* terVal;
		int* watVal;
		int* watVal2;
		int* watVal3;
		int* terVal2;
		int* bldVal2;
		int* ideVal;
		int* edgVal;
		
		int curCollectedWater;
		
		int fluidBodyCount;
		
		
		// k is largest at the top
		
		bool bTouchesAir;
		
		// float fSimp[4];
		// int iSimp[4];
		
		float disFromTop;
		float disFromBot;
		float spanZ;
		float minZ;
		float maxZ;
		
		float maxDif = 1.0f/4.0f;// /8.0f;
		
		float zv;
		
		bool isAir;
		bool isEmptyWater;
		bool didPrint = false;
		
		bool isInBounds;
		
		int minV0 = 0;//min( ((curTick)*totSize)/maxTicks, totSize-1 );
		int minV1 = totSize;//min( ((curTick+1)*totSize)/maxTicks, totSize-1 );
		
		
		
		if (hasRead) {
			
		}
		else {
			return false;
		}
		
		
		//if (curTick == 0) {
			// clear water ids and edges
			for (i = 0; i < totSize; i++) {
				ind = i;
				extraData[ind*4+E_PTT_IDE] = 0;
				extraData[ind*4+E_PTT_STB] = 0;
				fluidData[ind*4+E_PTT_LST] = fluidData[ind*4+E_PTT_WAT];
			}
			
			// for (i = 0; i < fsVec.size(); i++) {
			// 	fsVec[i].fluidIds.clear();
			// 	fsVec[i].idealCellIds.clear();
			// }
			fsVec.clear();
			
			// for (i = 0; i < fsPlaneVec.size(); i++) {
			// 	fsPlaneVec[i].fluidIds.clear();
			// }
			// fsPlaneVec.clear();
			
			
			
		//}
		
		
		
		
		// find the immobile height
		bool notFound = true;
		ind = 0;
		immobileHeight = 0;
		while (notFound && (ind < totSize)) {
			watVal = &(fluidData[ind*4+E_PTT_WAT]);
			bldVal = &(extraData[ind*4+E_PTT_BLD]);
			terVal = &(fluidData[ind*4+E_PTT_TER]);
			
			if (
				(*watVal > UNIT_MIN) ||
				(*bldVal > UNIT_MIN) ||
				(*terVal > UNIT_MIN)
			) {
				ind++;
			}
			else {
				notFound = false;
			}
		}
		immobileHeight = max((ind/(volSizePrimBuf*volSizePrimBuf)),0);
		immobileInd = immobileHeight*(volSizePrimBuf*volSizePrimBuf);
		
		
		// find the max water height
		notFound = true;
		ind = totSize-1;
		maxWaterHeight = 0;
		while (notFound && (ind >= 0)) {
			watVal = &(fluidData[ind*4+E_PTT_WAT]);
			
			if (*watVal != UNIT_MIN) {
				notFound = false;
			}
			else {
				ind--;
			}
		}
		maxWaterHeight = (ind/(volSizePrimBuf*volSizePrimBuf));
		maxWaterInd = ind;//maxWaterHeight*(volSizePrimBuf*volSizePrimBuf);
		
		
		if ((maxWaterHeight - immobileHeight) <= 1) {
			
			tempMin2.setFXYZ(0,0,0);
			tempMax2.setFXYZ(volSizePrimBuf,volSizePrimBuf,maxWaterHeight+1);
			
			
			return false;
		}
		
		
		minV0 = immobileInd;
		minV1 = maxWaterInd;
		
		
		
		
		// add zero water above all existing water
		
		// p = max(
		// 	immobileInd - volSizePrimBuf*volSizePrimBuf,
		// 	0
		// );
		
		// for (i = p; i <= minV1; i++) {
		// 	ind = i;
		// 	indAbove = i + volSizePrimBuf*volSizePrimBuf;
			
		// 	if (indAbove < totSize) {
		// 		watVal = &(fluidData[ind*4+E_PTT_WAT]);
				
		// 		watVal2 = &(fluidData[indAbove*4+E_PTT_WAT]);
		// 		terVal = &(fluidData[indAbove*4+E_PTT_TER]);
		// 		bldVal = &(extraData[indAbove*4+E_PTT_BLD]);
				
		// 		if (
		// 			(*watVal > 0) &&
		// 			(*watVal2 == UNIT_MIN) &&
		// 			(*terVal == UNIT_MIN) &&
		// 			(*bldVal == UNIT_MIN)
		// 		) {
		// 			*watVal2 = 0;
		// 		}
		// 	}
		// }
		
		
		
		
		
		// find stable regions
		fluidBodyCount = 1;
		for (i = minV0; i <= minV1; i++) {
			ind = i;
			
			
			watVal = &(fluidData[ind*4+E_PTT_WAT]);
			ideVal = &(extraData[ind*4+E_PTT_STB]);
			
			if (
				(*watVal >= 0) &&
				(*ideVal == 0)
			) {
				
				if (findStableRegions(ind,fluidBodyCount)) {
					fluidBodyCount++;
				}
			}
		}
		
		
		
		
		// flood fill water ids
		fluidBodyCount = 1;
		for (i = minV0; i <= minV1; i++) {
			ind = i;
			
			watVal = &(fluidData[ind*4+E_PTT_WAT]);
			ideVal = &(extraData[ind*4+E_PTT_IDE]);
			
			if (
				(*watVal >= 0) &&
				(*ideVal == 0)
			) {
				
				if (floodFillId(ind,fluidBodyCount)) {
					
					//if (fsVec.back().minZ > vspMin) {
					//	sort(fsVec.back().fluidIds.begin(), fsVec.back().fluidIds.end());
					//}
					
					
					// int intArray[SIZE] = {5, 3, 32, -1, 1, 104, 53};
					// sort(intArray, intArray + SIZE);
					
					j = fsVec.size()-1;
					
					sort(fidGetBeg(j),fidGetEnd(j));
					
					fluidBodyCount++;
				}
				
			}
			
		}
		
		
		
		
		
		
		// find most ideal areas to distribute water
		for (n = 0; n < fsVec.size(); n++) {
			
			spanZ = fsVec[n].maxZ - fsVec[n].minZ;
			
			#ifdef DEBUG_BOUNDS
			isInBounds = false;
			#endif
			
			for (p = 0; p < getFIDSize(n); p++) { //fsVec[n].fluidIds.size()
				ind = getFluidId(n,p);//fsVec[n].fluidIds[p];
				
				k = ind/(volSizePrimBuf*volSizePrimBuf);
				j = (ind - k*volSizePrimBuf*volSizePrimBuf)/volSizePrimBuf;
				i = ind - ( j*volSizePrimBuf + k*volSizePrimBuf*volSizePrimBuf );
				
				#ifdef DEBUG_BOUNDS
				isInBounds = isInBounds||inBounds(i,j,k);
				#endif
				
				
				bldVal = &(extraData[ind*4+E_PTT_BLD]);
				terVal = &(fluidData[ind*4+E_PTT_TER]);
				watVal = &(fluidData[ind*4+E_PTT_WAT]);
				
				isAir = 
					(*watVal == UNIT_MIN) &&
					(*bldVal == UNIT_MIN) &&
					(*terVal == UNIT_MIN);
				
				isEmptyWater = (*watVal == 0);
				
				#ifdef DEBUG_BOUNDS
				if (isInBounds) {
					if (isAir) {cout << "isAir\n";}
					cout << "watVal " << *watVal << "\n";
				}
				#endif
				
				
				if (
					isAir
					// && ((k != fsVec[n].maxZ)||(spanZ==1)) // water on top is not ideal for redist
				) {
					// check for water above
					testInd = ind + (volSizePrimBuf*volSizePrimBuf);
					watVal2 = &(fluidData[testInd*4+E_PTT_WAT]);
					
					if (*watVal2 > 0) {
						iciPushBack(n,ind);
						//fsVec[n].idealCellIds.push_back(ind);
					}
					else {
						for (dir = 0; dir < 4; dir++) {
							testI = (i+DIR_VECS_I[dir][0]);
							testJ = (j+DIR_VECS_I[dir][1]);
							testK = (k+DIR_VECS_I[dir][2]);
							testInd2 = 
								testI +
								testJ*volSizePrimBuf +
								testK*volSizePrimBuf*volSizePrimBuf;
							
							watVal3 = &(fluidData[testInd2*4+E_PTT_WAT]);
							
							// has (partially) full water cell to side
							if (*watVal3 >= 0) {
								
								// if unit below that one is earth
								testInd3 = testInd2 - volSizePrimBuf*volSizePrimBuf;
								if (
									(fluidData[testInd3*4+E_PTT_TER] > UNIT_MIN) ||
									(extraData[testInd3*4+E_PTT_BLD] > UNIT_MIN)
									// || (fluidData[testInd3*4+E_PTT_WAT] == UNIT_MIN)
								) {
									iciPushBack(n,ind);
									//fsVec[n].idealCellIds.push_back(ind);
									break;
								}
							}
						}
					}
				}
			}
			
			#ifdef DEBUG_BOUNDS
			if (isInBounds) {
				didPrint = true;
				cout << "cells considered " << getFIDSize(n) << "\n"; //fsVec[n].fluidIds.size()
				cout << "cells pushed " << getICISize(n) << "\n"; //fsVec[n].idealCellIds.size()
			}
			#endif
			
			
		}
		
		
		
		
		
		// if no ideal spots exist, find next most ideal spots
		for (n = 0; n < fsVec.size(); n++) {
			
			spanZ = fsVec[n].maxZ - fsVec[n].minZ;
			
			#ifdef DEBUG_BOUNDS
			isInBounds = false;
			#endif
			
			if (passesCheck(n)) {
				
			}
			else {
				for (p = 0; p < getFIDSize(n); p++) { //fsVec[n].fluidIds.size()
					ind = getFluidId(n,p);//fsVec[n].fluidIds[p];
					
					k = ind/(volSizePrimBuf*volSizePrimBuf);
					j = (ind - k*volSizePrimBuf*volSizePrimBuf)/volSizePrimBuf;
					i = ind - ( j*volSizePrimBuf + k*volSizePrimBuf*volSizePrimBuf );
					
					bldVal = &(extraData[ind*4+E_PTT_BLD]);
					terVal = &(fluidData[ind*4+E_PTT_TER]);
					watVal = &(fluidData[ind*4+E_PTT_WAT]);
					
					isAir = 
						(*watVal == UNIT_MIN) &&
						(*bldVal == UNIT_MIN) &&
						(*terVal == UNIT_MIN);
					
					isEmptyWater = (*watVal == 0);
					
					
					if (
						isAir
						// && ((k != fsVec[n].maxZ)||(spanZ==1)) // water on top is not ideal for redist
					) {
						// check for water above
						testInd = ind + (volSizePrimBuf*volSizePrimBuf);
						watVal2 = &(fluidData[testInd*4+E_PTT_WAT]);
						
						if (*watVal2 > 0) {
							iciPushBack(n,ind);
							//fsVec[n].idealCellIds.push_back(ind);
						}
						else {
							for (dir = 0; dir < 4; dir++) {
								testI = (i+DIR_VECS_I[dir][0]);
								testJ = (j+DIR_VECS_I[dir][1]);
								testK = (k+DIR_VECS_I[dir][2]);
								testInd2 = 
									testI +
									testJ*volSizePrimBuf +
									testK*volSizePrimBuf*volSizePrimBuf;
								
								watVal3 = &(fluidData[testInd2*4+E_PTT_WAT]);
								
								// has (partially) full water cell to side
								if (*watVal3 >= 0) {
									
									// if unit below that one is earth < IGNORED FOR THIS PASS
									// testInd3 = testInd2 - volSizePrimBuf*volSizePrimBuf;
									// if (
									// 	(fluidData[testInd3*4+E_PTT_TER] > UNIT_MIN) ||
									// 	(extraData[testInd3*4+E_PTT_BLD] > UNIT_MIN)
									// 	// || (fluidData[testInd3*4+E_PTT_WAT] == UNIT_MIN)
									// ) {
										iciPushBack(n,ind);
										//fsVec[n].idealCellIds.push_back(ind);
										break;
									//}
								}
							}
						}
					}
				}
			}
			
			
			
		}
		
		
		
		
	
		// remove water from top of fluid body (and only the very top)
		for (n = 0; n < fsVec.size(); n++) {
			
			if (passesCheck(n)) {
				fsVec[n].collectedWater = 0;
				curCollectedWater = 0;
				
				minZ = fsVec[n].minZ;
				maxZ = fsVec[n].maxZ;
				spanZ = maxZ-minZ;
				
				#ifdef DEBUG_BOUNDS
				isInBounds = false;
				#endif
				
				totWat = 0;
				
				for (p = 0; p < getFIDSize(n); p++) { //fsVec[n].fluidIds.size()
					ind = getFluidId(n,p);//fsVec[n].fluidIds[p];
					
					k = ind/(volSizePrimBuf*volSizePrimBuf);
					j = (ind - k*volSizePrimBuf*volSizePrimBuf)/volSizePrimBuf;
					i = ind - ( j*volSizePrimBuf + k*volSizePrimBuf*volSizePrimBuf );
					
					#ifdef DEBUG_BOUNDS
					isInBounds = isInBounds||inBounds(i,j,k);
					#endif
					
					fk = k;
					
					
					
					if (fk >= (maxZ-1.0f)) {
						watVal = &(fluidData[ind*4+E_PTT_WAT]);
						if ((*watVal) > 0) {
							curCollectedWater = (*watVal);
							*watVal -= curCollectedWater;
							fsVec[n].collectedWater += curCollectedWater;
						}
					}
					// else {
						
					// 	disFromBot = (fk-minZ)/(spanZ);
					// 	disFromTop = 1.0f - disFromBot;
						
					// 	disFromBot = mixf(0.25,0.75,disFromBot);
					// 	disFromTop = mixf(0.25,0.75,disFromTop);
						
					// 	if (spanZ <= 3.0f) {
					// 		disFromBot = 1.0f;
					// 	}
						
						
					// 	//terVal = &(fluidData[ind*4+E_PTT_TER]);
					// 	watVal = &(fluidData[ind*4+E_PTT_WAT]);
					// 	//ideVal = &(extraData[ind*4+E_PTT_IDE]);
						
					// 	totWat += max(*watVal,0);
						
					// 	// make sure no water is above or that there is earth above
					// 	testInd = ind + (volSizePrimBuf*volSizePrimBuf);
					// 	watVal2 = &(fluidData[testInd*4+E_PTT_WAT]);
					// 	terVal2 = &(fluidData[testInd*4+E_PTT_TER]);
					// 	bldVal2 = &(extraData[testInd*4+E_PTT_BLD]);
						
					// 	if (
					// 		(*watVal > 0) &&
					// 		(
					// 			(*watVal2 == UNIT_MIN) || // todo: examine this
					// 			(*terVal2 != UNIT_MIN) ||
					// 			(*bldVal2 != UNIT_MIN)
					// 		)
					// 	) {
					// 		curCollectedWater = 
					// 			min(
					// 				((int)(disFromBot*F_UNIT_MAX*maxDif + 1.0f)),
					// 				*watVal
					// 			);
					// 		*watVal -= curCollectedWater;
					// 		fsVec[n].collectedWater += curCollectedWater;
					// 	}
						
					// }
					
					
					
					
					
					
					
					
					
				}
				
				
				
				// cout << "fsVec[n].collectedWater " << fsVec[n].collectedWater << "\n";
				
				// if (fsVec[n].collectedWater == 0) {
				// 	cout << "fsVec[n].fluidIds.size() " << getFIDSize(n) << "\n"; //fsVec[n].fluidIds.size()
				// }
				
				
				
				#ifdef DEBUG_BOUNDS
				if (isInBounds) {
					didPrint = true;
					cout << "totWat " << totWat << "\n";
					cout << "fsVec[n].collectedWater " << fsVec[n].collectedWater << "\n";
				}
				#endif
				
			}
			
			fsVec[n].didCollectWater = (fsVec[n].collectedWater > 0);
			
		}
		
		
		
		
		
		// add water to ideal spots
		for (n = 0; n < fsVec.size(); n++) {
			
			numCells = getICISize(n);//fsVec[n].idealCellIds.size();
			
			if (numCells > 0) {
				
				for (p = 0; p < numCells; p++) {
					ind = getIdealCellId(n,p);//fsVec[n].idealCellIds[p];
					watVal = &(fluidData[ind*4+E_PTT_WAT]);
					
					if (*watVal == UNIT_MIN) {
						*watVal = 0;
					}
				}
				
				
				
				if ( (fsVec[n].collectedWater/numCells) > UNIT_MAX ) {
					curCollectedWater = UNIT_MAX;
				}
				else {
					curCollectedWater = fsVec[n].collectedWater/numCells;
				}
				
				//cout << "curCollectedWater " << curCollectedWater << "\n";
				
				curCollectedWater = curCollectedWater*maxDif;
				
				#ifdef DEBUG_BOUNDS
				isInBounds = false;
				#endif
				
				
				for (p = 0; p < numCells; p++) {
					ind = getIdealCellId(n,p);//fsVec[n].idealCellIds[p];
					
					k = ind/(volSizePrimBuf*volSizePrimBuf);
					j = (ind - k*volSizePrimBuf*volSizePrimBuf)/volSizePrimBuf;
					i = ind - ( j*volSizePrimBuf + k*volSizePrimBuf*volSizePrimBuf );
					
					#ifdef DEBUG_BOUNDS
					isInBounds = isInBounds||inBounds(i,j,k);
					#endif
					
					
					watVal = &(fluidData[ind*4+E_PTT_WAT]);
					
					
					if (curCollectedWater > 0) {
						
						tempi = min(UNIT_MAX - *watVal,curCollectedWater);
						
						*watVal += tempi;
						fsVec[n].collectedWater -= tempi;
					}
					
					
					
					
					
					
					if (fsVec[n].collectedWater <= 0) {
						break;
					}
					
				}
				
				#ifdef DEBUG_BOUNDS
				if (isInBounds) {
					didPrint = true;
					cout << "dist water " << fsVec[n].collectedWater << "\n\n";
				}
				#endif
				
			}
		}
		
		
		
		
		// add water to bottom of fluid body
		for (n = 0; n < fsVec.size(); n++) {
			
			if (passesCheck(n)) {
				curCollectedWater = 0;
				minZ = fsVec[n].minZ;
				
				
				while (fsVec[n].collectedWater > 0) {
					
					
					//for (q = fsVec[n].minZ; q <= fsVec[n].maxZ; q++) {
						for (p = 0; p < getFIDSize(n); p++) { //fsVec[n].fluidIds.size()
							ind = getFluidId(n,p);//fsVec[n].fluidIds[p];
							
							k = ind/(volSizePrimBuf*volSizePrimBuf);
							watVal = &(fluidData[ind*4+E_PTT_WAT]);
							
							//if (k == q) {
								
								if (fsVec[n].collectedWater > UNIT_MAX) {
									tempv = UNIT_MAX;
								}
								else {
									tempv = fsVec[n].collectedWater;
								}
								
								//if (fluidData[ind*4+E_PTT_TER] == UNIT_MIN) {
									
									curCollectedWater = min(
										((int)((UNIT_MAX - max(*watVal,0) )*maxDif + 1.0f )),
										tempv
									);
									
									if (curCollectedWater > 0) {
										if (*watVal == UNIT_MIN) {
											*watVal = 0;
										}
										
										tempi = min(UNIT_MAX - *watVal,curCollectedWater);
										
										*watVal += tempi;
										fsVec[n].collectedWater -= tempi;
									}
									
								//}
							//}
							
							
							// out of water, break both parent loops
							if (fsVec[n].collectedWater <= 0) {
								break;
								//p = fsVec[n].fluidIds.size() + 1;
								//q = fsVec[n].maxZ + 1;
							}
						}
					//}
				}
			}
		}
		
		if (didPrint) {
			watchMinX = -1;
		}
		
		
		// curTick++;
		// if (curTick == maxTicks) {
		// 	curTick = 0;
		// }
		
		
		
		/////////////
		
		
		tempMax.setFXYZ(vspMin);
		tempMin.setFXYZ(vspMax);
		
		tempMax2.setFXYZ(vspMin);
		tempMin2.setFXYZ(vspMax);
		
		for (n = 0; n < fsVec.size(); n++) {
			
			minV.setIXYZ(fsVec[n].minX,fsVec[n].minY,fsVec[n].minZ);
			maxV.setIXYZ(fsVec[n].maxX,fsVec[n].maxY,fsVec[n].maxZ);
			
			if (
				
				passesCheck(n)
				
				// || (
				// 	(fsVec[n].fluidIds.size() > 0) &&
				// 	(fsVec[n].fluidIds.size() < 8)
				// )
				
			) {
				FIVector4::growBoundary(&tempMin,&tempMax,&minV,&maxV);
			}
			
			FIVector4::growBoundary(&tempMin2,&tempMax2,&minV,&maxV);
		}
		
		tempMin.addXYZ(-1.0f);
		tempMax.addXYZ(1.0f);
		
		
		if (maxWaterHeight > 0) {
			minV.setIXYZ(0,0,0);
			maxV.setIXYZ(cellsPerHolder,cellsPerHolder,maxWaterHeight);
			FIVector4::growBoundary(&tempMin2,&tempMax2,&minV,&maxV);
		}
		
		
		
		FIVector4::growBoundary(&dirtyMin,&dirtyMax,&tempMin,&tempMax);
		
		//dirtyMin.setFZ(max(dirtyMin[2],(float)(immobileHeight)));
		
		
		
		
		// sink water into immobile area
		
		// int kMin = clamp(immobileHeight, vspMin, vspMax);
		// int kMax = clamp(immobileHeight+1, vspMin, vspMax);
		
		// k = immobileHeight;
		
		// for (k = kMin; k < kMax; k++) {
		// 	for (j = vspMin; j < vspMax; j++) {
		// 		for (i = vspMin; i < vspMax; i++) {
		// 			indSrc = (i + j*volSizePrimBuf + k*volSizePrimBuf*volSizePrimBuf);
		// 			indAbove = indSrc + volSizePrimBuf*volSizePrimBuf;
					
		// 			watVal = &(fluidData[indSrc*4 + E_PTT_WAT]);
		// 			watVal2 = &(fluidData[indAbove*4 + E_PTT_WAT]);
					
		// 			if ((*watVal) > UNIT_MIN) {
		// 				if ((*watVal2) <= 0) {
		// 					(*watVal2) = UNIT_MIN;
		// 				}
		// 				else {
							
		// 					curCollectedWater = 0;
							
		// 					if ((*watVal2) == 1) {
		// 						curCollectedWater = 1;
		// 					}
		// 					else {
		// 						curCollectedWater = (*watVal2)/2;
		// 					}
							
		// 					if (curCollectedWater >= 1) {
		// 						if (((*watVal) + curCollectedWater) < UNIT_MAX) {
		// 							*watVal2 = UNIT_MIN;
		// 							*watVal += curCollectedWater;
		// 						}
		// 					}
							
		// 				}
		// 			}
		// 		}
		// 	}
		// }
		
		// todo: expand water within immobile area if necessary
		
		
		
		// int iMin = clamp(tempMin.getIX(), vspMin, vspMax);
		// int iMax = clamp(tempMax.getIX(), vspMin, vspMax);
		// int jMin = clamp(tempMin.getIY(), vspMin, vspMax);
		// int jMax = clamp(tempMax.getIY(), vspMin, vspMax);
		// int kMin = clamp(tempMin.getIZ(), vspMin, vspMax);
		// int kMax = clamp(tempMax.getIZ(), vspMin, vspMax);
		
		
		// int iMin2 = vspMax;
		// int iMax2 = vspMin;
		// int jMin2 = vspMax;
		// int jMax2 = vspMin;
		// int kMin2 = vspMax;
		// int kMax2 = vspMin;
		
		
		// for (k = kMin; k <= kMax; k++) {
		// 	for (j = jMin; j <= jMax; j++) {
		// 		for (i = iMin; i <= iMax; i++) {
		// 			indSrc = (i + j*volSizePrimBuf + k*volSizePrimBuf*volSizePrimBuf)*4;
		
		
		// for (p = 0; p < (totSize - volSizePrimBuf); p++) {
		// 	ind = p;
		// 	indAbove = p + volSizePrimBuf*volSizePrimBuf;
			
		// 	k = ind/(volSizePrimBuf*volSizePrimBuf);
		// 	j = (ind - k*volSizePrimBuf*volSizePrimBuf)/volSizePrimBuf;
		// 	i = ind - ( j*volSizePrimBuf + k*volSizePrimBuf*volSizePrimBuf );
		
		// 			if (
					
		// 				fluidData[ind*4 + E_PTT_WAT] != fluidData[ind*4 + E_PTT_LST]
						
		// 			) {
						
		// 				// if (fluidData[indSrc + E_PTT_LST] < fluidData[indSrc + E_PTT_WAT]) {
		// 				// 	fluidData[indSrc + E_PTT_LST]++;
		// 				// }
		// 				// if (fluidData[indSrc + E_PTT_LST] > fluidData[indSrc + E_PTT_WAT]) {
		// 				// 	fluidData[indSrc + E_PTT_LST]--;
		// 				// }
						
		// 				if (i < iMin2) {iMin2 = i;}
		// 				if (j < jMin2) {jMin2 = j;}
		// 				if (k < kMin2) {kMin2 = k;}
		// 				if (i > iMax2) {iMax2 = i;}
		// 				if (j > jMax2) {jMax2 = j;}
		// 				if (k > kMax2) {kMax2 = k;}
						
		// 			}
		// }
		// 		}
		// 	}
		// }
		
		// minV.setIXYZ(iMin2,jMin2,kMin2);
		// maxV.setIXYZ(iMax2,jMax2,kMax2);
		// FIVector4::growBoundary(&dirtyMin,&dirtyMax,&minV,&maxV);
		
		
		return true;
		
	}
bool GameFluid::findStableRegions (int startInd, int newId)
                                                        {
		indexStack.clear();
		indexStack.push_back(startInd);
		
		int dir;
		
		int ind;
		int testInd;
		int testInd2;
		int* bldVal;
		int* bldVal2;
		int* terVal;
		int* terVal2;
		int* watVal;
		int* watVal2;
		int* ideVal;
		
		int i;
		int j;
		int k;
		int n;
		
		
		int foundInd;
		
		int testI;
		int testJ;
		int testK;
		
		bool notFound;
		bool isAir;
		bool terBelow;
		
		int watCount;
		int maxSize;
		
		long long int totWat;
		long long int totWat2;
		long long int divWat;
		
		fluidPlane.planeIds.clear();
		
		int emptyWaterCount = 0;
		int airCount = 0;
		int targWat;
		
		while (indexStack.size() > 0) {
			
			ind = indexStack.back();
			extraData[ind*4+E_PTT_STB] = newId;
			fluidPlane.planeIds.push_back(ind);
			
			if (fluidData[ind*4+E_PTT_WAT] == 0) {
				emptyWaterCount++;
			}
			
			
			k = ind/(volSizePrimBuf*volSizePrimBuf);
			j = (ind - k*volSizePrimBuf*volSizePrimBuf)/volSizePrimBuf;
			i = ind - ( j*volSizePrimBuf + k*volSizePrimBuf*volSizePrimBuf );
						
			
			foundInd = -1;
			
			for (dir = 0; dir < 4; dir++) {
				testI = (i+DIR_VECS_I[dir][0]);
				testJ = (j+DIR_VECS_I[dir][1]);
				testK = (k+DIR_VECS_I[dir][2]);
				testInd = 
					testI +
					testJ*volSizePrimBuf +
					testK*volSizePrimBuf*volSizePrimBuf;
				
				testInd2 = testInd - volSizePrimBuf*volSizePrimBuf;
				
				terVal = &(fluidData[testInd*4+E_PTT_TER]);
				watVal = &(fluidData[testInd*4+E_PTT_WAT]);
				ideVal = &(extraData[testInd*4+E_PTT_STB]);
				bldVal = &(extraData[testInd*4+E_PTT_BLD]);
				
				
				// isAir = 
				// 	(*watVal == UNIT_MIN) &&
				// 	(*terVal == UNIT_MIN) &&
				// 	(*bldVal == UNIT_MIN);
					
				//terVal2 = &(fluidData[testInd2*4+E_PTT_TER]);
				//watVal2 = &(fluidData[testInd2*4+E_PTT_WAT]);
				// terBelow = 
				// 	//(*watVal2 == UNIT_MIN) &&
				// 	(*terVal2 == UNIT_MIN);
				// if (isAir&&terBelow) {
				// 	airCount++;
				// }
				
				if (
					(*ideVal == 0) &&
					(*watVal >= 0)
				) {
					foundInd = testInd;
				}
			}
			
			if (foundInd >= 0) {
				indexStack.push_back(foundInd);
			}
			else {
				indexStack.pop_back();
			}
			
		}
		
		
		maxSize = fluidPlane.planeIds.size();
		
		
		
		if (
			(emptyWaterCount == maxSize)
			// && (airCount == 0)	
		) {
			// all fluid cells in this plane are empty,
			// and there are no adjacent air cells,
			// so get rid of all empty water cells
			
			
			for (i = 0; i < maxSize; i++) {
				ind = fluidPlane.planeIds[i];
				
				fluidData[ind*4+E_PTT_WAT] = UNIT_MIN;
			}
			
			fluidPlane.planeIds.clear();
			
		}
		else {
			
			
			/*
			
			
			// equalize fluid density in this plane
			
			if (maxSize > 0) {
				
				totWat = 0;
				for (i = 0; i < maxSize; i++) {
					ind = fluidPlane.planeIds[i];
					watVal = &(fluidData[ind*4+E_PTT_WAT]);
					totWat += *watVal;
				}
				
				divWat = maxSize;
				if (divWat > 0) {
					
					targWat = (totWat/divWat);
					
					if ( targWat > 0 ) {
						
						totWat2 = 0;
						for (i = 0; i < maxSize; i++) {
							ind = fluidPlane.planeIds[i];
							watVal = &(fluidData[ind*4+E_PTT_WAT]);
							*watVal += (targWat - *watVal)/2;
							totWat2 += *watVal;
						}
						
						i = 0;
						
						while (totWat > totWat2) {
							
							ind = fluidPlane.planeIds[i];
							watVal = &(fluidData[ind*4+E_PTT_WAT]);
							
							*watVal += 1;
							totWat2 += 1;
							
							i++;
							if (i==maxSize) { i = 0; }
							
						}
						
						while (totWat < totWat2) {
							
							ind = fluidPlane.planeIds[i];
							watVal = &(fluidData[ind*4+E_PTT_WAT]);
							
							*watVal -= 1;
							totWat2 -= 1;
							
							i++;
							if (i==maxSize) { i = 0; }
						}
						
					}
				}
				
			}
			
			
			
			
			
			
			*/
			
			
			
		}
		
		//fsPlaneVec.pop_back();
		
		if ( fluidPlane.planeIds.size() == 0 ) {
			
			return false;
		}
		else {
			return true;
		}
		
		
		
	}
bool GameFluid::floodFillId (int startInd, int newId)
                                                  {
		
		indexStack.clear();
		indexStack.push_back(startInd);
		
		int dir;
		
		int ind;
		int testInd;
		int* bldVal;
		int* terVal;
		int* watVal;
		int* watVal2;
		int* ideVal;
		
		int i;
		int j;
		int k;
		int n;
		
		
		int foundInd;
		
		int testI;
		int testJ;
		int testK;
		
		bool notFound;
		bool isAir;
		bool isEmptyWater;
		
		fsVec.push_back(FluidStruct());
		
		
		
		FluidStruct* fsPtr = &(fsVec.back());
		FluidStruct* fsPtrLast;
		
		int groupId = fsVec.size()-1;
		
		
		if (groupId == 0) {
			fsPtr->fidBegInd = 0;
			fsPtr->fidEndInd = 0;
			fsPtr->iciBegInd = 0;
			fsPtr->iciBegInd = 0;
		}
		else {
			fsPtrLast = &(fsVec[groupId-1]);
			
			fsPtr->fidBegInd = fsPtrLast->fidEndInd;
			fsPtr->fidEndInd = fsPtrLast->fidEndInd;
			fsPtr->iciBegInd = fsPtrLast->iciEndInd;
			fsPtr->iciBegInd = fsPtrLast->iciEndInd;
		}
		
		fsPtr->minX = volSizePrimBuf*2;
		fsPtr->minY = volSizePrimBuf*2;
		fsPtr->minZ = volSizePrimBuf*2;
		fsPtr->maxX = -volSizePrimBuf*2;
		fsPtr->maxY = -volSizePrimBuf*2;
		fsPtr->maxZ = -volSizePrimBuf*2;
		
		while (indexStack.size() > 0) {
			
			ind = indexStack.back();
			extraData[ind*4+E_PTT_IDE] = newId;
			
			k = ind/(volSizePrimBuf*volSizePrimBuf);
			j = (ind - k*volSizePrimBuf*volSizePrimBuf)/volSizePrimBuf;
			i = ind - ( j*volSizePrimBuf + k*volSizePrimBuf*volSizePrimBuf );
			
			watVal2 = &(fluidData[ind*4+E_PTT_WAT]);
			
			notFound = true;
			
			if (k > immobileHeight) {
				if (*watVal2 < UNIT_MAX) {
					// if water is partially filled, add to fluidId stack
					fidPushBack(groupId,ind); //fsPtr->fluidIds.push_back(ind);
					notFound = false;
				}
				else {
					// if its full and there is earth above, add it
					testInd = ind + volSizePrimBuf*volSizePrimBuf;
					
					if (
						(fluidData[testInd*4+E_PTT_TER] != UNIT_MIN) ||
						(extraData[testInd*4+E_PTT_BLD] != UNIT_MIN)	
					) {
						fidPushBack(groupId,ind); //fsPtr->fluidIds.push_back(ind);
						notFound = false;
					}
					
				}
			}
			
			
			foundInd = -1;
			
			
			for (dir = 0; dir < 6; dir++) {
				testI = (i+DIR_VECS_I[dir][0]);
				testJ = (j+DIR_VECS_I[dir][1]);
				testK = (k+DIR_VECS_I[dir][2]);
				testInd = 
					testI +
					testJ*volSizePrimBuf +
					testK*volSizePrimBuf*volSizePrimBuf;
				
				
				if (testK > immobileHeight) {
					bldVal = &(extraData[testInd*4+E_PTT_BLD]);
					terVal = &(fluidData[testInd*4+E_PTT_TER]);
					watVal = &(fluidData[testInd*4+E_PTT_WAT]);
					ideVal = &(extraData[testInd*4+E_PTT_IDE]);
					
					isAir = 
						(*watVal == UNIT_MIN) &&
						(*terVal == UNIT_MIN) &&
						(*bldVal == UNIT_MIN);
					
					isEmptyWater = (*watVal == 0);
					
					// check if water touches air or an empty water cell, if so add to fluidId stack
					if (
						(isAir||isEmptyWater)	&&
						notFound
					) {
						fidPushBack(groupId,ind); //fsPtr->fluidIds.push_back(ind);
						notFound = false;
					}
					
					if (*ideVal == 0) {
						if (isAir) {
							// it is an unmarked air value or empty water cell adjacent to a water cell
							// push back immediately without growing main stack
							
							fidPushBack(groupId,testInd); //fsPtr->fluidIds.push_back(testInd);
							*ideVal = newId;
							
						}
						
						if (*watVal >= 0) {
							foundInd = testInd;
						}
						
					}
				}
			}
			
			if (foundInd >= 0) {
				indexStack.push_back(foundInd);
			}
			else {
				indexStack.pop_back();
			}
			
		}
		
		
		for (n = 0; n < getFIDSize(groupId); n++) { //fsPtr->fluidIds.size()
			
			ind = getFluidId(groupId,n); //fsPtr->fluidIds[n];
			
			k = ind/(volSizePrimBuf*volSizePrimBuf);
			j = (ind - k*volSizePrimBuf*volSizePrimBuf)/volSizePrimBuf;
			i = ind - ( j*volSizePrimBuf + k*volSizePrimBuf*volSizePrimBuf );
			
			bldVal = &(extraData[ind*4+E_PTT_BLD]);
			terVal = &(fluidData[ind*4+E_PTT_TER]);
			watVal = &(fluidData[ind*4+E_PTT_WAT]);
			
			// remove ids from any air values so they can be reused
			if (
				(*watVal == UNIT_MIN) &&
				(*terVal == UNIT_MIN) &&
				(*bldVal == UNIT_MIN)
			) {
				extraData[ind*4+E_PTT_IDE] = 0;
			}
			
			//fluidData[ind*4+E_PTT_EDG] = UNIT_MAX;
			
			
			if (i < fsPtr->minX) {fsPtr->minX = i;}
			if (j < fsPtr->minY) {fsPtr->minY = j;}
			if (k < fsPtr->minZ) {fsPtr->minZ = k;}
			if (i > fsPtr->maxX) {fsPtr->maxX = i;}
			if (j > fsPtr->maxY) {fsPtr->maxY = j;}
			if (k > fsPtr->maxZ) {fsPtr->maxZ = k;}
			
		}
		
		if ( getFIDSize(groupId) == 0) { //fsPtr->fluidIds.size() 
			fsVec.pop_back();
			return false;
		}
		else {
			return true;
		}
		
	}
bool GameFluid::inBounds (int i, int j, int k)
                                           {
		return (
			(watchMinX >= 0) &&
			(i >= watchMinX) &&
			(i <= watchMaxX) &&
			(j >= watchMinY) &&
			(j <= watchMaxY) &&
			(k >= watchMinZ) &&
			(k <= watchMaxZ)
		);
	}
void GameFluid::modifyUnit (FIVector4 * fPixelWorldCoordsBase, int brushAction, int modType, int radius)
          {
		modStack.push_back(ModUnitStruct());
		modStack.back().basePos.copyFrom(fPixelWorldCoordsBase);
		modStack.back().brushAction = brushAction;
		modStack.back().modType = modType;
		modStack.back().radius = radius;
	}
void GameFluid::roundBox (FIVector4 * absVecFromCenter, FIVector4 * innerBoxRad, FIVector4 * cornerDisThicknessPower, bool & isInObj, bool & isInside)
          {
		
		//doTraceVecND("absVecFromCenter ", absVecFromCenter);
		//doTraceVecND("innerBoxRad ", innerBoxRad);
		
		
		FIVector4 newP;
		FIVector4 orig1;
		FIVector4 orig2;
		
		orig1.setFXYZ(0.0f,0.0f,0.0f);
		
		//vec3 newP = abs(max( absVecFromCenter-(box_dim.xyz),0.0));
		
		orig2.copyFrom(absVecFromCenter);
		orig2.addXYZRef(innerBoxRad,-1.0f);
		newP.maxXYZ(&orig2,&orig1);
		newP.absXYZ();
		
		float powX = cornerDisThicknessPower->getFZ();
		float powY = cornerDisThicknessPower->getFW();
		


		//newP.xy = pow(newP.xy, box_power.xx );
		newP.powXYZ(powX,powX,1.0f);
		//newP.x = pow( newP.x + newP.y, 1.0/box_power.x );
		newP.setFX(newP[0] + newP[1]);
		newP.powXYZ(1.0/powX,1.0f,1.0f);

		//newP.xz = pow(newP.xz, box_power.yy );
		newP.powXYZ(powY,1.0f,powY);
		//newP.x = pow( newP.x + newP.z, 1.0/box_power.y );
		newP.setFX(newP[0] + newP[2]);
		newP.powXYZ(1.0/powY,1.0f,1.0f);
		
		//newP.setFX(newP.length());
		
		float rad = cornerDisThicknessPower->getFX();
		float wallThickness = cornerDisThicknessPower->getFY();
		
		// float dis = 
		// 	max(
		// 		(newP[0]-rad),
		// 		( (rad-wallThickness)-newP[0] )	 //newP[0] - (rad-wallThickness)
		// 	);
		
		// //cout << newP[0] << " - " << cornerDisThicknessPower->getFX() << " = " << dis << "\n";
		
		// return dis < 0.0f;
		
		isInObj = (newP[0]-rad) < 0.0f;
		isInside = (newP[0] - (rad-wallThickness)) < 0.0f;

	}
void GameFluid::clearAllGeom ()
                            {
		int i;
		int ind;
		for (i = 0; i < totSize; i++) {
			ind = i;
			extraData[ind*4+E_PTT_BLD] = UNIT_MIN;
		}
	}
void GameFluid::clearInsideValues ()
                                 {
		int i;
		int ind;
		for (i = 0; i < totSize; i++) {
			ind = i;
			if (extraData[ind*4+E_PTT_BLD] == UNIT_INSIDE) {
				extraData[ind*4+E_PTT_BLD] = UNIT_MIN;
			}
			
		}
	}
void GameFluid::fillCurrentGeom (int templateId, FIVector4 * templatePos)
                                                                     {
		
		
		
		
		int i;
		int j;
		int k;
		
		
		FIVector4* paramArrGeom = &(singleton->primTemplateStack[templateId*E_PRIMTEMP_LENGTH]);
		
		FIVector4 innerBoxRad;
		FIVector4 absVecFromCenter;
		//FIVector4 centerCoord;
		
		FIVector4 baseVec;
		baseVec.averageXYZ(
			&(paramArrGeom[E_PRIMTEMP_BOUNDSMIN]),
			&(paramArrGeom[E_PRIMTEMP_BOUNDSMAX])
		);
		baseVec.addXYZRef(templatePos);
		baseVec.addXYZRef(&(volMinReadyInPixels),-1.0f);
		baseVec.addXYZ(1.0f);
		
		FIVector4 curCoord;
		
		FIVector4 newCoordMin;
		FIVector4 newCoordMax;
		newCoordMin.copyFrom(&baseVec);
		newCoordMax.copyFrom(&baseVec);
		
		// centerCoord.averageXYZ(
		// 	&(paramArrGeom[E_PRIMTEMP_BOUNDSMAX]),
		// 	&(paramArrGeom[E_PRIMTEMP_BOUNDSMIN])
		// );
		innerBoxRad.averageNegXYZ(
			&(paramArrGeom[E_PRIMTEMP_BOUNDSMAX]),
			&(paramArrGeom[E_PRIMTEMP_BOUNDSMIN])
		);
		
		innerBoxRad.addXYZ(paramArrGeom[E_PRIMTEMP_CORNERDIS].getFX(),-1.0f);
		
		newCoordMin.addXYZRef(&(paramArrGeom[E_PRIMTEMP_VISMIN]));
		newCoordMax.addXYZRef(&(paramArrGeom[E_PRIMTEMP_VISMAX]));
		
		//newCoordMin.addXYZ(2.0f,2.0f,2.0f);
		//newCoordMax.addXYZ(2.0f,2.0f,2.0f);
		
		int iMin = clamp(newCoordMin.getIX(), vspMin, vspMax);
		int iMax = clamp(newCoordMax.getIX(), vspMin, vspMax);
		int jMin = clamp(newCoordMin.getIY(), vspMin, vspMax);
		int jMax = clamp(newCoordMax.getIY(), vspMin, vspMax);
		int kMin = clamp(newCoordMin.getIZ(), vspMin, vspMax);
		int kMax = clamp(newCoordMax.getIZ(), vspMin, vspMax);
		
		int* empVal;
		int* bldVal;
		int* terVal;
		int* watVal;
		
		int ind;
		
		bool isInObj;
		bool isInside;
		
		minV.setIXYZ(iMin,jMin,kMin);
		maxV.setIXYZ(iMax,jMax,kMax);
		FIVector4::growBoundary(&dirtyMin,&dirtyMax,&minV,&maxV);
		
		
		
		// cout << "cornerDis " << paramArrGeom[E_PRIMTEMP_CORNERDIS].getFX() << "\n";
		// doTraceVecND("E_PRIMTEMP_BOUNDSMIN ", &(paramArrGeom[E_PRIMTEMP_BOUNDSMIN]));
		// doTraceVecND("E_PRIMTEMP_BOUNDSMAX ", &(paramArrGeom[E_PRIMTEMP_BOUNDSMAX]));
		// doTraceVecND("innerBoxRad ", &innerBoxRad);
		
		for (k = kMin; k < kMax; k++) {
			for (j = jMin; j < jMax; j++) {
				for (i = iMin; i < iMax; i++) {
					
					curCoord.setFXYZ(i,j,k);
					curCoord.addXYZ(0.5f);
					absVecFromCenter.copyFrom(&curCoord);
					absVecFromCenter.addXYZRef(&baseVec,-1.0f);
					absVecFromCenter.absXYZ();
					
					roundBox(
						&absVecFromCenter,
						&innerBoxRad,
						&(paramArrGeom[E_PRIMTEMP_CORNERDIS]),
						isInObj,
						isInside
					);
					
					ind = (i + j*volSizePrimBuf + k*volSizePrimBuf*volSizePrimBuf);
					bldVal = &(extraData[ind*4+E_PTT_BLD]);
					terVal = &(fluidData[ind*4+E_PTT_TER]);
					watVal = &(fluidData[ind*4+E_PTT_WAT]);
					empVal = &(fluidData[ind*4+E_PTT_EMP]);
					
					if (*bldVal == UNIT_INSIDE) {
						
					}
					else {
						
						if (isInObj) {
							if (isInside) {
								*bldVal = UNIT_INSIDE;
							}
							else {
								*bldVal = UNIT_MAX;
								*watVal = UNIT_MIN; // todo: collect water for redist instead of destroying it
							}
							
						}
						else {
							
						}
					}
					
					
					*empVal = UNIT_MIN;
					
					// if (*bldVal == UNIT_MAX) {
					// 	*terVal = UNIT_MAX;
					// }
					// else {
					// 	*terVal = UNIT_MIN;
					// }
					
					
				}
			}
		}
		
		
		
	}
void GameFluid::resetDirtyRegion ()
                                {
		
		dirtyMax.setFXYZ(vspMin);
		dirtyMin.setFXYZ(vspMax);
	}
void GameFluid::shrinkDirtyRegion ()
                                 {
		
		if (shrinkCount == 4) {
			shrinkCount = 0;
			dirtyMax.addXYZ(-1.0f);
			dirtyMin.addXYZ(1.0f);
		}
		
		if (
			(dirtyMax[0] <= dirtyMin[0]) ||
			(dirtyMax[1] <= dirtyMin[1]) ||
			(dirtyMax[2] <= dirtyMin[2])
		) {
			resetDirtyRegion();
		}
		
		shrinkCount++;
	}
void GameFluid::maxDirtyRegion ()
                              {
		dirtyMax.setFXYZ(vspMax);
		dirtyMin.setFXYZ(vspMin);
	}
void GameFluid::applyUnitModification (FIVector4 * fPixelWorldCoordsBase, int brushAction, int modType, int radius)
          {
		
			
		
			FIVector4 baseVec;
			baseVec.copyFrom(fPixelWorldCoordsBase);
			baseVec.addXYZRef(&(volMinReadyInPixels),-1.0f);
			
			
			FIVector4 curCoord;
			
			FIVector4 newCoordMin;
			FIVector4 newCoordMax;
			newCoordMin.copyFrom(&baseVec);
			newCoordMax.copyFrom(&baseVec);
			
			
			newCoordMin.addXYZ(-radius + 1);
			newCoordMax.addXYZ( radius + 1);
			
			int iMin = clamp(newCoordMin.getIX(), vspMin, vspMax);
			int iMax = clamp(newCoordMax.getIX(), vspMin, vspMax);
			int jMin = clamp(newCoordMin.getIY(), vspMin, vspMax);
			int jMax = clamp(newCoordMax.getIY(), vspMin, vspMax);
			int kMin = clamp(newCoordMin.getIZ(), vspMin, vspMax);
			int kMax = clamp(newCoordMax.getIZ(), vspMin, vspMax);
			
			minV.setIXYZ(iMin,jMin,kMin);
			maxV.setIXYZ(iMax,jMax,kMax);
			FIVector4::growBoundary(&dirtyMin,&dirtyMax,&minV,&maxV);
			
			
			int i;
			int j;
			int k;
			
			int* empVal;
			int* bldVal;
			int* terVal;
			int* watVal;
			// int* ideVal;
			// int* stbVal;
			
			int ind;
			int ind2;
			
			int i2;
			int j2;
			int k2;
			
			int i3;
			int j3;
			int k3;
			
			bool touchesBuilding;
			
			for (k = kMin; k < kMax; k++) {
				for (j = jMin; j < jMax; j++) {
					for (i = iMin; i < iMax; i++) {
						
						curCoord.setFXYZ(i,j,k);
						
						if (baseVec.distance(&curCoord) <= radius) {
							
							
							ind = (i + j*volSizePrimBuf + k*volSizePrimBuf*volSizePrimBuf);
							
							bldVal = &(extraData[ind*4+E_PTT_BLD]);
							empVal = &(fluidData[ind*4+E_PTT_EMP]);
							terVal = &(fluidData[ind*4+E_PTT_TER]);
							watVal = &(fluidData[ind*4+E_PTT_WAT]);
							// ideVal = &(extraData[ind*4+E_PTT_IDE]);
							// stbVal = &(extraData[ind*4+E_PTT_STB]);
							
							switch (brushAction) {
								case E_BRUSH_MOVE:
									return;
								break;
								case E_BRUSH_ADD:
								
									if (modType == E_PTT_WAT) {
										//if (mainId == E_FID_SML) {
											if (
												(*terVal == UNIT_MIN) &&
												(*bldVal == UNIT_MIN)	
											) {
												*watVal = UNIT_MAX;
											}
										//}
									}
									else {
										*terVal = UNIT_MAX;
										*watVal = UNIT_MIN;
										*empVal = UNIT_MIN;
									}
									
									
									
								break;
								case E_BRUSH_SUB:
									
									// touchesBuilding = false;
									
									// for (k2 = -1; k2 <= 1; k2++) {
									// 	for (j2 = -1; j2 <= 1; j2++) {
									// 		for (i2 = -1; i2 <= 1; i2++) {
												
									// 			i3 = clamp(i+i2,vspMin,vspMax);
									// 			j3 = clamp(j+j2,vspMin,vspMax);
									// 			k3 = clamp(k+k2,vspMin,vspMax);
												
												
									// 			ind2 = 
									// 				i3 +
									// 				j3*volSizePrimBuf +
									// 				k3*volSizePrimBuf*volSizePrimBuf;
												
									// 			touchesBuilding = touchesBuilding || (extraData[ind2*4+E_PTT_BLD] != UNIT_MIN);
												
									// 		}
									// 	}
									// }
									
									// if (
									// 	touchesBuilding ||
									// 	(*terVal != UNIT_MIN) ||
									// 	(*watVal != UNIT_MIN)
									// ) {
									// 	*empVal = UNIT_MAX;
									// 	*bldVal = UNIT_MIN;
									// 	*terVal = UNIT_MIN;
									// 	*watVal = UNIT_MIN;
									// }
									
									if (
										(*bldVal == UNIT_MAX) ||
										(*terVal == UNIT_MAX)
									) {
										
										if (
											((i%2)==0) &&
											((j%2)==0) &&
											((k%2)==0)	
											
										) {
											
											if (GEN_DEBRIS) {
												singleton->debrisStack.push_back(DebrisStruct());
												singleton->debrisStack.back().pos = btVector3(
													i + volMinReadyInPixels[0] - bufAmount,
													j + volMinReadyInPixels[1] - bufAmount,
													k + volMinReadyInPixels[2] - bufAmount
												);
											}
																					
										}
										
										
										
									}
									
									*empVal = UNIT_MAX;
									*bldVal = UNIT_MIN;
									*terVal = UNIT_MIN;
									*watVal = UNIT_MIN;
								
									
									
								break;
								case E_BRUSH_REF:
									watchMinX = iMin;
									watchMaxX = iMax;
									
									watchMinY = jMin;
									watchMaxY = jMax;
									
									watchMinZ = kMin;
									watchMaxZ = kMax;
									
								break;
								
							}
							
							
						}
						
						
					}
				}
			}
			
	}
#undef LZZ_INLINE
 
