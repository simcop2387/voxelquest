class GameBlock {
public:


	
	Singleton* singleton;
	
	int blockID;
	int blockSizeInHolders;
	int blockSizeInLots;
	
	FIVector4 offsetInBlocks;
	FIVector4 offsetInBlocksWrapped;

	std::vector<GameGeom*> gameGeom;



	///////////////////////


	int iHolderSize;
	int maxFloors;
	GamePageHolder** holderData;

	int iNodeDivsPerLot;
	int iBuildingNodeSize;
	int iBuildingNodesPerSide;
	int iBuildingNodesPerSideM1;
	float fBuildingNodesPerSideM1;
	
	BuildingNode* buildingData;
	GLubyte* terrainHeights;

	GameWorld* gw;

	BuildingNode nullNode;


	///////////////////////


	GameBlock() {

	}

	void init(Singleton* _singleton, int _blockID, int _x, int _y, int _xw, int _yw) {
		singleton = _singleton;
		blockID = _blockID;
		offsetInBlocks.setIXYZ(_x,_y,0);
		offsetInBlocksWrapped.setIXYZ(_xw,_yw,0);



		//cout << "Init block " << _xw << " " << _yw << "\n";

		//int treeCount = 0;

		int i;
		int j;
		int k;
		int m;
		int n;

		int curAlign = 0;

		float x1;
		float y1;

		float x2;
		float y2;

		float z;

		maxFloors = singleton->maxFloors;

		blockSizeInHolders = singleton->blockSizeInHolders;
		blockSizeInLots = singleton->blockSizeInLots;

		int blockSizeInPixels = blockSizeInLots* singleton->pixelsPerLot;
		float fBlockSizeInPixels = (float)blockSizeInPixels;

		float pixelsPerMeter = singleton->pixelsPerMeter;

		
		float uvSizeInMeters = 1.0;
		float uvSizeInPixels = uvSizeInMeters*pixelsPerMeter; // 64

		float offsetPerFloor = 0.25;
		float floorOffset;

		bool isTopOfWing;
		bool isWingTip;

		float cr1;
		float cr2;
		float cr3;
		float cr4;

		float doorMod = 0.0f;

		gw = singleton->gw;

		FBOWrapper* fbow = singleton->getFBOWrapper("hmFBO",0);
		FBOWrapper* fbow2 = singleton->getFBOWrapper("cityFBO",0);

		int maxLoop;
		int lotX;
		int lotY;
		int xmod1;
		int ymod1;
		int xmod2;
		int ymod2;


		int curLev = 0;
		int kt = 0;
		int curInd = 0;
		int kmax = 0;
		int kmin = 0;
		int kstep = 0;

		int testX;
		int testY;


		int baseI = 0;
		int baseJ = 0;

		int counter = 0;
		int res;
		int res2;
		int res3;

		int houseColor = 0;
		int lotSizeO2 = singleton->pixelsPerLot/2;

		int XP = 0;
		int XN = 1;
		int YP = 2;
		int YN = 3;
		int curChannel = 0;


		int scalexm1 = 1;
		int scaleym1 = 1;
		int scalexm2 = 1;
		int scaleym2 = 1;

		int curId = 0;
		int curBT = 0;
		int curHeight = 0;

		int wingDir;
		int openDir;

		float nFloors;
		float floorHeight;
		float fi;
		float fj;

		float tempf;
		float pv1;
		float pv2;
		float pv3;
		float pv4;

		float percs[5];
		percs[0] = 0.0f;
		percs[1] = 0.25f;
		percs[2] = 0.5f;
		percs[3] = 0.75f;
		percs[4] = 1.0f;
		float perc1;
		float perc2;
		float roofHeight = 0.0f;
		float baseOffset = 0.0f;
		float holderSizeInPixels = singleton->holderSizeInPixels;

		bool doProc = false;
		bool notFound = false;
		bool isVert;
		bool isDif;

		int begHeight = 0;
		int endHeight = 0;

		float offsetX = 0.0f;
		float offsetY = 0.0f;


		FIVector4 anchorPointInPixels;
		FIVector4 moveMinInPixels;
		FIVector4 moveMaxInPixels;

		FIVector4 p1;
		FIVector4 p2;
		FIVector4 rad;
		FIVector4 visInsetFromMin;
		FIVector4 visInsetFromMax;
		FIVector4 cornerRad;
		FIVector4 thickVals;
		FIVector4 powerVals;
		FIVector4 powerVals2;
		FIVector4 matParams;

		FIVector4 tempVec;
		FIVector4 tempVec2;

		iNodeDivsPerLot = singleton->iNodeDivsPerLot;
		iBuildingNodesPerSideM1 = blockSizeInLots*iNodeDivsPerLot;
		fBuildingNodesPerSideM1 = (float)iBuildingNodesPerSideM1;
		iBuildingNodesPerSide = iBuildingNodesPerSideM1 + 1;
		iBuildingNodeSize = (iBuildingNodesPerSide)*(iBuildingNodesPerSide);
		buildingData = new BuildingNode[iBuildingNodeSize];
		terrainHeights = new GLubyte[iBuildingNodesPerSideM1*iBuildingNodesPerSideM1*4];

		for (i = 0; i < iBuildingNodeSize; i++) {
			buildingData[i].id = 0;
			buildingData[i].centerProp.typeVal = E_BT_NULL;
			buildingData[i].centerProp.begHeight = 0;
			buildingData[i].centerProp.endHeight = 0;

			for (j = 0; j < 4; j++) {
				buildingData[i].connectionProps[j].typeVal = E_BT_NULL;
				
				for (k = 0; k < maxFloors; k++) {
					getPropAtIndLevel(i,j,k,E_NT_SHORTPROP)->typeVal = E_BT_NULL;
					getPropAtIndLevel(i,j,k,E_NT_DYNPROP)->typeVal = E_BT_NULL;
				}
				
			}

		}


		nullNode.id = 0;
		nullNode.centerProp.typeVal = E_BT_NULL;
		for (j = 0; j < 4; j++) {
			nullNode.connectionProps[j].typeVal = E_BT_NULL;
			
			for (k = 0; k < maxFloors; k++) {
				nullNode.shortProps[j + k*4].typeVal = E_BT_NULL;
				nullNode.dynProps[j + k*4].typeVal = E_BT_NULL;
			}
			
		}


		for (i = 0; i < blockSizeInLots; i++) {
			for (j = 0; j < blockSizeInLots; j++) {
				

				for (m = 0; m < 3; m++) {

					switch (m) {
						case 0:
							curChannel = gw->stChannel;
						break;
						case 1:
							curChannel = gw->btChannel;
						break;
						case 2:
							curChannel = gw->pathChannel;
						break;
					}

					//one lot = one map pixel
					lotX = blockSizeInLots*(offsetInBlocks.getIX()) + i;
					lotY = blockSizeInLots*(offsetInBlocks.getIY()) + j;
					res = fbow2->getPixelAtWrapped(lotX, lotY, curChannel);

					// roads
					for (k = 0; k < 4; k++) {

						if ( (res & gw->dirFlagsO[k]) > 0) { // has road

							baseI = i*iNodeDivsPerLot;
							baseJ = j*iNodeDivsPerLot;

							for (m = 0; m < iNodeDivsPerLot; m++) {
								switch (k) {
									case 0: // x+
										connectNodes(baseI+iNodeDivsPerLot,baseJ+m,baseI+iNodeDivsPerLot,baseJ+m+1,E_BT_ROAD,-1);
									break;
									case 1: // x-
										connectNodes(baseI,baseJ+m,baseI,baseJ+m+1,E_BT_ROAD,-1);
									break;
									case 2: // y+
										connectNodes(baseI+m,baseJ+iNodeDivsPerLot,baseI+m+1,baseJ+iNodeDivsPerLot,E_BT_ROAD,-1);
									break;
									case 3: // y-
										connectNodes(baseI+m,baseJ,baseI+m+1,baseJ,E_BT_ROAD,-1);
									break;

								}
							}

						}
					}
				}

				
			}
		}

		// houses

		for (i = 0; i < blockSizeInLots; i++) {
			for (j = 0; j < blockSizeInLots; j++) {

				lotX = blockSizeInLots*(offsetInBlocks.getIX()) + i;
				lotY = blockSizeInLots*(offsetInBlocks.getIY()) + j;
				

				res = fbow2->getPixelAtWrapped(lotX, lotY, gw->houseChannel);
				res3 = fbow2->getPixelAtWrapped(lotX, lotY, gw->btChannel);

				if (res != 0) {
					for (k = 0; k < 4; k++) {
						
						res2 = fbow2->getPixelAtWrapped(lotX + gw->dirModX[k], lotY + gw->dirModY[k], gw->houseChannel);
						
						if ( (res == res2) ) { // house id matches, there is a house in that direction




							baseI = i*iNodeDivsPerLot + iNodeDivsPerLot/2;
							baseJ = j*iNodeDivsPerLot + iNodeDivsPerLot/2;

							for (m = 0; m < iNodeDivsPerLot/2; m++) {
								
								//if ( fGenRand() > 0.25f ) {
									switch (k) {
										case 0: // x+
											connectNodes(baseI+m,baseJ,baseI+m+1,baseJ,E_BT_MAINHALL,res);
										break;
										case 1: // x-
											connectNodes(baseI-(m+1),baseJ,baseI-m,baseJ,E_BT_MAINHALL,res);
										break;
										case 2: // y+
											connectNodes(baseI,baseJ+m,baseI,baseJ+m+1,E_BT_MAINHALL,res);
										break;
										case 3: // y-
											connectNodes(baseI,baseJ-(m+1),baseI,baseJ-m,E_BT_MAINHALL,res);
										break;

									}


								//}
									
							}


						}
					}
				}

			}
		}



		// connect wings

		for (i = 1; i < iBuildingNodesPerSideM1; i++) {
			for (j = 1; j < iBuildingNodesPerSideM1; j++) {
				if ( touches(i,j,E_BT_NULL) == 4 ) {

					notFound = true;
					for (k = 0; (k < 4)&&notFound; k++) {

						isVert = k >= 2;

						testX = i + gw->dirModX[k];
						testY = j + gw->dirModY[k];

						if ( touches(testX,testY,E_BT_MAINHALL) >= 1 ) {

							if (touches2(i,j,E_BT_WING) == 0) {

								if (true) {//(fGenRand() > 0.5) {
									connectNodes(i, j, testX, testY, E_BT_WING, getNode(testX,testY)->id );

									testX = i + gw->dirModX[ gw->opDir[k] ];
									testY = j + gw->dirModY[ gw->opDir[k] ];

									if (touches(testX,testY,E_BT_ROAD) >= 1) {
										connectNodes(i, j, testX, testY, E_BT_ROAD, -1);

										getPropAtLevel(i,j, gw->opDir[k], 1, E_NT_SHORTPROP)->typeVal = E_BT_DOORWAY;
										getPropAtLevel(i,j, gw->opDir[k], 1, E_NT_DYNPROP)->typeVal = E_BT_DOOR;
										

									}

									notFound = false;
								}

								
							}

							
						}

					}
				}
			}
		}

		for (i = 1; i < iBuildingNodesPerSideM1; i++) {
			for (j = 1; j < iBuildingNodesPerSideM1; j++) {
				if ( touches(i,j,E_BT_NULL) == 4 ) {
					getNode(i,j)->centerProp.typeVal = E_BT_TREE;
				}
			}
		}


		// towers

		for (i = 1; i < iBuildingNodesPerSideM1; i++) {
			for (j = 1; j < iBuildingNodesPerSideM1; j++) {
				if ( touches(i,j,E_BT_MAINHALL) >= 1 ) {

					if (touchesCenter(i,j,E_BT_TOWER) == 0) {

						curHeight = sameHeight(i,j);
						if (curHeight != -1) {
							getNode(i,j)->centerProp.typeVal = E_BT_TOWER;
							getNode(i,j)->centerProp.endHeight = min(curHeight+iGenRand(1,maxFloors-curHeight),maxFloors);
							getNode(i,j)->centerProp.begHeight = curHeight;
						}

						
					}
					
				}
			}
		}


		// overpasses

		for (i = 1; i < iBuildingNodesPerSideM1; i++) {
			for (j = 1; j < iBuildingNodesPerSideM1; j++) {

				if ( touches(i,j,E_BT_MAINHALL) + touches(i,j,E_BT_WING) >= 2 ) {
					for (k = 0; k < 4; k++) {
						testX = i + gw->dirModX[k];
						testY = j + gw->dirModY[k];

						if ( touches(testX,testY,E_BT_MAINHALL) + touches(testX,testY,E_BT_WING) >= 2) {

							for (m = maxFloors-2; m > 1; m--) {
								if (getNode(i,j)->connectionProps[k].endHeight > m) {
									if ( testHeight(i,j,testX,testY,m) ) {
										getNode(i,j)->connectionProps[k].begHeight = m;
										getNode(testX,testY)->connectionProps[gw->opDir[k]].begHeight = m;
										break;
									}
								}	
							}
						}
					}
				}
			}
		}


		// powervals;

		for (i = 1; i < iBuildingNodesPerSideM1; i++) {
			for (j = 1; j < iBuildingNodesPerSideM1; j++) {

				if ( touches(i,j,E_BT_MAINHALL) + touches(i,j,E_BT_WING) >= 1 ) {
					
					tempf = fGenRand();

					if (tempf < 0.33333f) {
						pv1 = 1.0;
					}
					else {
						if (tempf < 0.66666f) {
							pv1 = 2.0;
						}
						else {
							pv1 = 9.0;
						}
					}

					getNode(i,j)->powerValU = pv1;

					// tempf = fGenRand();
					// if (tempf < 0.5f) {
					// 	pv2 = 1.0f;
					// }
					// else {
					// 	pv2 = 2.0f;
					// }

					curId = getNode(i,j)->id;
					if (curId%16 < 12) {
						pv2 = 1.0f;
					}
					else {
						pv2 = 2.0f;
					}

					getNode(i,j)->powerValV = pv2;

				}
			}
		}

		// wing windows

		for (i = 1; i < iBuildingNodesPerSideM1; i++) {
			for (j = 1; j < iBuildingNodesPerSideM1; j++) {
				
				if ( (touches(i,j,E_BT_WING) == 1) && (touches(i,j,E_BT_MAINHALL) == 0) ) {

					wingDir = touchDir(i,j,E_BT_WING);
					openDir = gw->opDir[wingDir];
					curHeight = getNode(i,j)->connectionProps[wingDir].endHeight;

					for (m = 1; m < curHeight; m++) {
						if ( getPropAtLevel(i,j,openDir,m, E_NT_SHORTPROP)->typeVal == E_BT_NULL ) {
							if (fGenRand() >= 0.25f) {
								getPropAtLevel(i,j,openDir,m, E_NT_SHORTPROP)->typeVal = E_BT_WINDOWFRAME;
								getPropAtLevel(i,j,openDir,m, E_NT_DYNPROP)->typeVal = E_BT_WINDOW;
							}
							
						}
					}

				}
			}
		}


		// other windows

		for (i = 1; i < iBuildingNodesPerSideM1; i++) {
			for (j = 1; j < iBuildingNodesPerSideM1; j++) {
				
				if (getNode(i,j)->powerValU != 1.0f) {
					if ( touches(i,j,E_BT_MAINHALL) >= 1 ) {
						
						begHeight = 1;
						endHeight = 1;

						for (k = 0; k < 4; k++) {
							curBT = getNode(i,j)->connectionProps[k].typeVal;
							if ((curBT == E_BT_MAINHALL) || (curBT == E_BT_WING)) {
								begHeight = min(begHeight,getNode(i,j)->connectionProps[k].begHeight);
								endHeight = max(endHeight,getNode(i,j)->connectionProps[k].endHeight);
							}
							
						}

						if (getNode(i,j)->centerProp.typeVal == E_BT_TOWER) {
							//begHeight = min(begHeight,getNode(i,j)->centerProp.begHeight);
							endHeight = max(endHeight,getNode(i,j)->centerProp.endHeight);
						}


						for (m = max(begHeight,1); m < endHeight; m++) {
							for (k = 0; k < 4; k++) {
								if (getNode(i,j)->connectionProps[k].typeVal == E_BT_NULL) {
									if (fGenRand() >= 0.25f) {
										getPropAtLevel(i,j,k,m, E_NT_SHORTPROP)->typeVal = E_BT_WINDOWFRAME;
										getPropAtLevel(i,j,k,m, E_NT_DYNPROP)->typeVal = E_BT_WINDOW;
									}
									
								}
							}
						}

						

					}
				}				

			}
		}


		// wing tips

		for (i = 1; i < iBuildingNodesPerSideM1; i++) {
			for (j = 1; j < iBuildingNodesPerSideM1; j++) {

				getNode(i,j)->isWingTip = (
					(touches(i,j,E_BT_MAINHALL) == 0) &&
					(touches(i,j,E_BT_WING) == 1)
				);
			}
		}




		// get terrain heights

		floorHeight = 4.0*pixelsPerMeter;

		for (i = 0; i < iBuildingNodesPerSide; i++) {
			for (j = 0; j < iBuildingNodesPerSide; j++) {
					
				

				lotX = blockSizeInPixels*offsetInBlocks.getIX();
				lotY = blockSizeInPixels*offsetInBlocks.getIY();

				x1 = lotX + (i*blockSizeInPixels)/iBuildingNodesPerSideM1;
				y1 = lotY + (j*blockSizeInPixels)/iBuildingNodesPerSideM1;


				getNode(i,j)->terHeight = max(
					singleton->getHeightAtPixelPos(x1,y1),
					singleton->getSeaLevelInPixels() + 2.0f*pixelsPerMeter
				);

				if ( ( touches(i,j,E_BT_WING) + touches(i,j,E_BT_MAINHALL) ) >= 1) {
					getNode(i,j)->terHeight = floor( getNode(i,j)->terHeight/floorHeight)*floorHeight + floorHeight*0.25f;
				}

			}
		}


		// equalize terrain heights

		isDif = true;
		while (isDif) {

			isDif = false;
			for (i = 1; i < iBuildingNodesPerSideM1; i++) {
				for (j = 1; j < iBuildingNodesPerSideM1; j++) {

					for (k = 0; k < 4; k++) {

						testX = i + gw->dirModX[k];
						testY = j + gw->dirModY[k];

						curBT = getNode(i,j)->connectionProps[k].typeVal;

						if ( (curBT == E_BT_MAINHALL) || (curBT == E_BT_WING) ) {
							if (getNode(i,j)->terHeight < getNode(testX,testY)->terHeight) {
								isDif = true;
								getNode(i,j)->terHeight = getNode(testX,testY)->terHeight;
							}
						}
					}
				}
			}
		}
		

		for (i = 0; i < iBuildingNodesPerSideM1; i++) {
			for (j = 0; j < iBuildingNodesPerSideM1; j++) {

				curHeight = getNode(i,j)->terHeight;

				terrainHeights[ (i + j*iBuildingNodesPerSideM1)*4 + 0 ] = curHeight%256;
				terrainHeights[ (i + j*iBuildingNodesPerSideM1)*4 + 1 ] = curHeight/256;

				if (touches(i,j,E_BT_ROAD) + touches(i,j,E_BT_WING) + touches(i,j,E_BT_MAINHALL) >= 1) {
					terrainHeights[ (i + j*iBuildingNodesPerSideM1)*4 + 2 ] = 255;
				}
				else {
					terrainHeights[ (i + j*iBuildingNodesPerSideM1)*4 + 2 ] = 0;
				}

				terrainHeights[ (i + j*iBuildingNodesPerSideM1)*4 + 3 ] = 0;

			}
		}

		






		// place nodes

		for (i = 0; i < iBuildingNodesPerSideM1; i++) {
			for (j = 0; j < iBuildingNodesPerSideM1; j++) {
					
				isWingTip = getNode(i,j)->isWingTip;


				fi = (float)i;
				fj = (float)j;

				for (m = 0; m < E_NT_LENGTH; m++) {
					switch (m) {
						case E_NT_CONNECTIONPROP:
							kmax = 4;
							kstep = 2;
						break;

						case E_NT_SHORTPROP:
						case E_NT_DYNPROP:
							kmax = 4*maxFloors;
							kstep = 1;
						break;

						case E_NT_CENTERPROP:
							kmax = 1;
							kstep = 1;
						break;

						
					}


					for (kt = 0; kt < kmax; kt += kstep) {
						k = kt%4;
						curLev = kt/4;

						isTopOfWing = touchesHeight(i,j,E_BT_WING) == curLev+1;

						testX = i + gw->dirModX[k];
						testY = j + gw->dirModY[k];


						isVert = (k >= 2);
						
						scalexm1 = 1;
						scaleym1 = 1;
						scalexm2 = 1;
						scaleym2 = 1;

						offsetX = 0.0f;
						offsetY = 0.0f;

						switch (m) {
							case E_NT_CONNECTIONPROP:
								curBT = getNode(i,j)->connectionProps[k].typeVal;

								xmod1 = 0;
								ymod1 = 0;

								xmod2 = gw->dirModX[k];
								ymod2 = gw->dirModY[k];

								
							break;

							case E_NT_SHORTPROP:
							case E_NT_DYNPROP:
								curBT = getPropAtLevel(i,j,k,curLev, m)->typeVal;
								
								xmod1 = ((float)(gw->dirModX[ gw->opDir[k] ]));
								ymod1 = ((float)(gw->dirModY[ gw->opDir[k] ]));
								xmod2 = ((float)(gw->dirModX[k]));
								ymod2 = ((float)(gw->dirModY[k]));


								if (
									(curBT == E_BT_WINDOWFRAME) ||
									(curBT == E_BT_DOORWAY)
								) {
									if (isVert) {
										scaleym1 = 16;
										scaleym2 = 32;

									}
									else {
										scalexm1 = 16;
										scalexm2 = 32;
									}
								}
								else {
									if (isVert) {
										scaleym1 = 64;
										scaleym2 = 64;

									}
									else {
										scalexm1 = 64;
										scalexm2 = 64;
									}
								}


								if (isWingTip) {
									offsetX = -0.125*gw->dirModX[k]*pixelsPerMeter;
									offsetY = -0.125*gw->dirModY[k]*pixelsPerMeter;
								}
								else {
									floorOffset = offsetPerFloor*((float)(maxFloors-curLev));
									offsetX = gw->dirModX[k]*(3.75f - floorOffset)*pixelsPerMeter;
									offsetY = gw->dirModY[k]*(3.75f - floorOffset)*pixelsPerMeter;
								}
								
							break;

							case E_NT_CENTERPROP:
								curBT = getNode(i,j)->centerProp.typeVal;
								
								xmod1 = 0;
								ymod1 = 0;

								xmod2 = 0;
								ymod2 = 0;


								
							break;

						}


						if (curBT != E_BT_NULL) {
							//one lot = one map pixel
							lotX = blockSizeInPixels*offsetInBlocks.getIX();
							lotY = blockSizeInPixels*offsetInBlocks.getIY();

							x1 = lotX + (i*blockSizeInPixels+(xmod1*blockSizeInPixels)/scalexm1)/iBuildingNodesPerSideM1 + offsetX;
							y1 = lotY + (j*blockSizeInPixels+(ymod1*blockSizeInPixels)/scaleym1)/iBuildingNodesPerSideM1 + offsetY;
							x2 = lotX + (i*blockSizeInPixels+(xmod2*blockSizeInPixels)/scalexm2)/iBuildingNodesPerSideM1 + offsetX;
							y2 = lotY + (j*blockSizeInPixels+(ymod2*blockSizeInPixels)/scaleym2)/iBuildingNodesPerSideM1 + offsetY;


							z = getNode(i,j)->terHeight;//singleton->getCityHeight();////singleton->getHeightAtPixelPos(x1,y1);

							p1.setFXYZ(x1,y1,z);
							p2.setFXYZ(x2,y2,z);


							doProc = true;
							
							curId = getNode(i,j)->id;

							

							cr1 = 0.0f;
							cr2 = 0.0f;
							cr3 = 0.0f;
							cr4 = 0.0f;

							curAlign = E_ALIGN_BOTTOM;



							switch(curBT) {
								case E_BT_ROAD:

									
									

									baseOffset = 0.0f;//0.25f*pixelsPerMeter;
									matParams.setFXYZ(E_MAT_PARAM_ROAD, 0.0f, 0.0f);
									rad.setFXYZ(
										2.0f*pixelsPerMeter,
										2.0f*pixelsPerMeter,
										max(abs( getNode(testX,testY)->terHeight-z ),holderSizeInPixels)
									);

									curAlign = E_ALIGN_MIDDLE;

									powerVals.setFXYZ(10.0f,10.0f,0.0);
									powerVals2.setFXYZRef(&powerVals);
									visInsetFromMin.setFXYZ(0.0f,0.0f,0.0f);
									visInsetFromMax.setFXYZ(0.0f,0.0f,0.0f);//rad.getFZ()*2.0f - 0.625f*pixelsPerMeter);
									cornerRad.setFXYZ(
										1.0f*pixelsPerMeter,
										1.0f*pixelsPerMeter,
										1.0f*pixelsPerMeter
									);
									thickVals.setFXYZ(0.0f,1.0f*pixelsPerMeter,0.0);//getNode(i,j)->terHeight,getNode(testX,testY)->terHeight);//getNode(testX,testY)->terHeight-z
									
								break;
								case E_BT_MAINHALL:
								case E_BT_WING:
								case E_BT_TOWER:


									// if (curBT == E_BT_MAINHALL) {
									// 	nFloors = 3;
									// }
									// else {
									// 	nFloors = 2;
									// }

									//iGenRand(2,3);

									switch (m) {
										case E_NT_CONNECTIONPROP:
										case E_NT_SHORTPROP:
											nFloors = getNode(i,j)->connectionProps[k].endHeight;
											begHeight = getNode(i,j)->connectionProps[k].begHeight;
										break;
										case E_NT_CENTERPROP:
											nFloors = getNode(i,j)->centerProp.endHeight;
											begHeight = getNode(i,j)->centerProp.begHeight;
										break;

									}


									floorHeight = 4.0f;
									roofHeight = 4.0f;

									houseColor = curId%6;
									matParams.setFXYZ(E_MAT_PARAM_BUILDING, houseColor, 0.0f);


									
									

									
									rad.setFXYZ(
										4.0f*pixelsPerMeter,
										4.0f*pixelsPerMeter,
										(nFloors*floorHeight*0.5 + roofHeight)*pixelsPerMeter
									);
									cornerRad.setFXYZ(
										4.0f*pixelsPerMeter,
										4.0f*pixelsPerMeter,
										roofHeight*pixelsPerMeter
									);

									

									
									if (curBT == E_BT_WING) {
										

										if (isVert) {
											if (touches(i, j, E_BT_MAINHALL) == 0) {
												cr3 = cornerRad.getFX();
												cr4 = 0.0f;
											}
											if (touches(testX, testY, E_BT_MAINHALL) == 0) {
												cr4 = cornerRad.getFX();
												cr3 = 0.0f;
											}
										}
										else {
											if (touches(i, j, E_BT_MAINHALL) == 0) {
												cr1 = cornerRad.getFY();
												cr2 = 0.0f;
											}
											if (touches(testX, testY, E_BT_MAINHALL) == 0) {
												cr2 = cornerRad.getFY();
												cr1 = 0.0f;
											}
										}


										
									}


									if (curBT == E_BT_TOWER) {
										pv1 = getNode(i,j)->powerValU;
										pv2 = pv1;
										pv3 = getNode(i,j)->powerValV;
										pv4 = pv3;
									}
									else {

										pv1 = getNode(i,j)->powerValU;
										pv2 = getNode(testX,testY)->powerValU;
										pv3 = getNode(i,j)->powerValV;
										pv4 = getNode(testX,testY)->powerValV;
									}

									baseOffset = -(floorHeight-1.0f)*pixelsPerMeter + floorHeight*(begHeight)*pixelsPerMeter;
									visInsetFromMin.setFXYZ(cr1,cr3,cornerRad.getFZ() + floorHeight*(begHeight)*pixelsPerMeter);
									visInsetFromMax.setFXYZ(cr2,cr4,0.0f);

									
									thickVals.setFXYZ(0.0f,0.0f,0.0f);
									powerVals.setFXYZ(pv1, pv3, 0.0);
									powerVals2.setFXYZ(pv2, pv4, 0.0);

									


								break;

								case E_BT_WINDOWFRAME:
								case E_BT_WINDOW:
								case E_BT_DOORWAY:
								case E_BT_DOOR:

									nFloors = 1;
									

									switch(curBT) {
										case E_BT_WINDOWFRAME:
										case E_BT_WINDOW:

											// matParams.setFXYZ(E_MAT_PARAM_DOORWAY, 0.0, 0.0f);

											// floorHeight = 1.5f;
											// roofHeight = 1.0f;
											// baseOffset = (2.25f + (curLev-1.0)*4.0)*pixelsPerMeter;
											// rad.setFXYZ(
											// 	1.0f*pixelsPerMeter,
											// 	1.0f*pixelsPerMeter,
											// 	(nFloors*floorHeight*0.5 + roofHeight)*pixelsPerMeter
											// );
											// cornerRad.setFXYZ(
											// 	1.0f*pixelsPerMeter,
											// 	1.0f*pixelsPerMeter,
											// 	roofHeight*pixelsPerMeter
											// );
											// thickVals.setFXYZ(0.25f*pixelsPerMeter, 0.0f, 0.0f);


											if (curBT == E_BT_WINDOW) {
												matParams.setFXYZ(E_MAT_PARAM_SLATS, 1.0, 0.0f);
												doorMod = 0.125f;

											}
											else {
												matParams.setFXYZ(E_MAT_PARAM_DOORWAY, 1.0, 0.0f);
												doorMod = 0.0f;
											}

											if (isWingTip && isTopOfWing) {
												floorHeight = 3.0f-doorMod;
											}
											else {
												floorHeight = 1.5f-doorMod;
											}
											
											roofHeight = 1.0f-doorMod;
											baseOffset = (2.25f+doorMod + (curLev-1.0)*4.0)*pixelsPerMeter;
											rad.setFXYZ(
												(1.0f - doorMod)*pixelsPerMeter,
												(1.0f - doorMod)*pixelsPerMeter,
												(nFloors*floorHeight*0.5 + roofHeight)*pixelsPerMeter
											);
											cornerRad.setFXYZ(
												(1.0f - doorMod)*pixelsPerMeter,
												(1.0f - doorMod)*pixelsPerMeter,
												roofHeight*pixelsPerMeter
											);
											thickVals.setFXYZ(0.125f*pixelsPerMeter, 0.0f, 0.0f);


										break;
										case E_BT_DOORWAY:
										case E_BT_DOOR:

											if (curBT == E_BT_DOOR) {
												matParams.setFXYZ(E_MAT_PARAM_SLATS, 0.0, 0.0f);
												doorMod = 0.25f;

											}
											else {
												matParams.setFXYZ(E_MAT_PARAM_DOORWAY, 0.0, 0.0f);
												doorMod = 0.0f;
											}

											
											floorHeight = 2.0f-doorMod;
											roofHeight = 1.5f-doorMod;
											baseOffset = (1.5f + doorMod)*pixelsPerMeter;
											rad.setFXYZ(
												(1.5f - doorMod)*pixelsPerMeter,
												(1.5f - doorMod)*pixelsPerMeter,
												(nFloors*floorHeight*0.5 + roofHeight)*pixelsPerMeter
											);
											cornerRad.setFXYZ(
												(1.5f - doorMod)*pixelsPerMeter,
												(1.5f - doorMod)*pixelsPerMeter,
												roofHeight*pixelsPerMeter
											);
											thickVals.setFXYZ(0.25f*pixelsPerMeter, 0.0f, 0.0f);
										break;
									}

									

									if (isVert) {
										cr3 = cornerRad.getFX();
										cr4 = cornerRad.getFX();
									}
									else {
										cr1 = cornerRad.getFY();
										cr2 = cornerRad.getFY();
									}

									visInsetFromMin.setFXYZ(cr1,cr3,cornerRad.getFZ());
									visInsetFromMax.setFXYZ(cr2,cr4,0.0f);

									
									
									powerVals.setFXYZ(2.0f, 2.0f, 0.0f);
									powerVals2.setFXYZRef(&powerVals);


								break;

								case E_BT_TREE:
									baseOffset = 0.0f;
									matParams.setFXYZ(E_MAT_PARAM_TREE, 0.0, 0.0f);

									visInsetFromMin.setFXYZ(0.0f,0.0f,0.0f);
									visInsetFromMax.setFXYZ(0.0f,0.0f,0.0f);

								break;

								default:
									doProc = false;
								break;
							}



							if (doProc) {


								if (curBT == E_BT_TREE) {

									//treeCount++;

									tempVec.setFXYZRef(&p1);
									tempVec2.setFXYZRef(&p2);

									tempVec2.addXYZ(
										0.0f*pixelsPerMeter,
										0.0f*pixelsPerMeter,
										3.0f*pixelsPerMeter
									);

									gameGeom.push_back(new GameGeom());
									gameGeom.back()->initTree(
										curBT,
										counter,
										singleton->geomCounter,
										curAlign,
										baseOffset,
										
										&tempVec,
										&tempVec2,

										2.0f*pixelsPerMeter,
										1.0f*pixelsPerMeter,

										//&rad,
										//&cornerRad,
										&visInsetFromMin,
										&visInsetFromMax,
										// &powerVals,
										// &powerVals2,
										// &thickVals,
										
										&matParams
									);
									singleton->geomCounter++;
									counter++;



								}
								else {

									
									if (
										(curBT == E_BT_DOOR) ||
										(curBT == E_BT_WINDOW)
									) {
										maxLoop = 2;
									}
									else {
										maxLoop = 1;
									}

									for (n = 0; n < maxLoop; n++) {
										if (maxLoop == 2) {
											

											if (n == 0) {
												if (isVert) {
													visInsetFromMax.addXYZ( rad.getFX() + 1.0f, 0.0f, 0.0f );
												}
												else {
													visInsetFromMax.addXYZ( 0.0f, rad.getFY() + 1.0f, 0.0f );
												}
												
											}
											else {
												if (isVert) {
													visInsetFromMax.addXYZ( -(rad.getFX() + 1.0f), 0.0f, 0.0f );
													visInsetFromMin.addXYZ( rad.getFX() + 1.0f, 0.0f, 0.0f );
												}
												else {
													visInsetFromMax.addXYZ( 0.0f, -(rad.getFY() + 1.0f), 0.0f );
													visInsetFromMin.addXYZ( 0.0f, rad.getFY() + 1.0f, 0.0f );
												}
											}


											
										}
										else {

										}


										gameGeom.push_back(new GameGeom());
										gameGeom.back()->initBounds(
											curBT,
											counter,
											singleton->geomCounter,
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
											&matParams
										);
										singleton->geomCounter++;
										counter++;


										if (maxLoop == 2) {

											
											tempVec2.setFXYZRef( gameGeom.back()->getVisMaxInPixels() );
											tempVec2.addXYZRef( gameGeom.back()->getVisMinInPixels(), -1.0f );
											tempVec2.multXYZ(0.5f);
											tempf = min(min(tempVec2.getFX(), tempVec2.getFY()),tempVec2.getFZ());

											if (n == 0) {
												tempVec.setFXYZRef( gameGeom.back()->getVisMinInPixels() );
												tempVec.addXYZ(tempf,tempf,0.0f);

												if (isVert) {
													tempVec.addXYZ(-tempf*3.0,0.0,0.0f);
												}
												else {
													tempVec.addXYZ(0.0,-tempf*3.0,0.0f);
												}

												if ( (k == 1) || (k == 2) ) {
													gameGeom.back()->initAnchorPoint( &tempVec, 0, 1 );
												}
												else {
													gameGeom.back()->initAnchorPoint( &tempVec, -1, 0 );
												}
											}
											else {
												tempVec.setFXYZRef( gameGeom.back()->getVisMaxInPixels() );
												tempVec.addXYZ(-tempf,-tempf,0.0f);

												if (isVert) {
													tempVec.addXYZ(tempf*3.0,0.0,0.0f);
												}
												else {
													tempVec.addXYZ(0.0,tempf*3.0,0.0f);
												}

												if ( (k == 0) || (k == 3) ) {
													gameGeom.back()->initAnchorPoint( &tempVec, 0, 1 );
												}
												else {
													gameGeom.back()->initAnchorPoint( &tempVec, -1, 0 );
												}
											}
									
										}

									}



									
								}



							}


							

							

							
						}
						



					}


				}
			}
		}


		glBindTexture(GL_TEXTURE_2D, singleton->terrainID);
			glTexSubImage2D(
				GL_TEXTURE_2D,
				0,
				
				blockSizeInLots*offsetInBlocksWrapped.getIX()*singleton->iNodeDivsPerLot, // xoffset,
				blockSizeInLots*offsetInBlocksWrapped.getIY()*singleton->iNodeDivsPerLot, // yoffset,

				iBuildingNodesPerSideM1, // width
				iBuildingNodesPerSideM1, // height

				GL_RGBA,
				GL_UNSIGNED_BYTE,

				terrainHeights
			);
		glBindTexture(GL_TEXTURE_2D, 0);

		// FBOWrapper* fbow = getFBOWrapper("terrainFBO",0);
		// fbow->bind(0);
		// glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);


		//doTraceND("treeCount", i__s(treeCount));
		

		///////////////////////

		iHolderSize = blockSizeInHolders*blockSizeInHolders*blockSizeInHolders;
		holderData = new GamePageHolder*[iHolderSize];
		for (i = 0; i < iHolderSize; i++) {
			holderData[i] = NULL;
		}


		///////////////////////




	}

	BuildingNode* getNode(int x, int y) {

		if (x >= 0 && x < iBuildingNodesPerSide && y >= 0 && y < iBuildingNodesPerSide ) {
			return &(buildingData[x+y*iBuildingNodesPerSide]);
		}
		else {
			cout << "Accessed null node at: " << x << ", " << y << "\n";
			return &nullNode;
		}

		
	}

	BuildingNodeProp* getPropAtLevel(int x, int y, int dir, int lev, int nodeType ) {
		return getPropAtIndLevel(x+y*iBuildingNodesPerSide, dir, lev, nodeType);//&(buildingData[x+y*iBuildingNodesPerSide].shortProps[dir + lev*4]);
	}
	BuildingNodeProp* getPropAtIndLevel(int i, int dir, int lev, int nodeType ) {
		
		switch(nodeType) {
			case E_NT_SHORTPROP:
				return &(buildingData[i].shortProps[dir + lev*4]);
			break;
			case E_NT_DYNPROP:
				return &(buildingData[i].dynProps[dir + lev*4]);
			break;
			default:
				return &(buildingData[i].shortProps[dir + lev*4]);
			break;
		}


		
	}

	int touches(int x, int y, int buildingType) {
		int i;
		int tot = 0;


		for (i = 0; i < 4; i++) {
			if (getNode(x,y)->connectionProps[i].typeVal == buildingType) {
				tot++;
			}
		}

		return tot;
	}

	int touchesHeight(int x, int y, int buildingType) {
		int i;
		int tot = 0;


		for (i = 0; i < 4; i++) {
			if (getNode(x,y)->connectionProps[i].typeVal == buildingType) {
				return getNode(x,y)->connectionProps[i].endHeight;
			}
		}

		return -1;
	}

	int touchDir(int x, int y, int buildingType) {
		int i;

		for (i = 0; i < 4; i++) {
			if (getNode(x,y)->connectionProps[i].typeVal == buildingType) {
				return i;
			}
		}

		return -1;
	}



	bool testHeight(int _x1, int _y1, int _x2, int _y2, int heightVal) {
			

		int x[2];
		int y[2];

		x[0] = _x1;
		y[0] = _y1;
		x[1] = _x2;
		y[1] = _y2;


		int boff = 0;
		int i;
		int j;
		int testX;
		int testY;

		// if (x1 > x2) {
		// 	std::swap(x1,x2);
		// }
		// if (y1 > y2) {
		// 	std::swap(y1,y2);
		// }

		bool foundHigher[2];
		foundHigher[0] = false;
		foundHigher[1] = false;



		for (j = 0; j < 2; j++) {
			for (i = 0; i < 4; i++) {

				if ( (x[j] + gw->dirModX[i] == x[1-j]) && (y[j] + gw->dirModY[i] == y[1-j]) ) {
					// this is the connecting branch, do nothing
				}
				else {

					testX = x[j] + gw->dirModX[i];
					testY = y[j] + gw->dirModY[i];

					if (getNode(testX,testY)->connectionProps[i].begHeight >= heightVal) {
						return false;
					}

					if (getNode(testX,testY)->connectionProps[i].endHeight > heightVal) {
						foundHigher[j] = true;
					}
				}

			}
		}

		if (foundHigher[0]&&foundHigher[1]) {
			return true;
		}
		else {
			return false;
		}

	}


	int touchesCenter(int x, int y, int buildingType) {
		int i;
		int tot = 0;
		int testX;
		int testY;

		for (i = 0; i < 4; i++) {

			testX = x + gw->dirModX[i];
			testY = y + gw->dirModY[i];

			if (getNode(testX,testY)->centerProp.typeVal == buildingType) {
				tot++;
			}
		}

		return tot;
	}


	int sameHeight(int x, int y) {
		int i;
		int curType;
		int lastHeight = -1;


		for (i = 0; i < 4; i++) {

			curType = getNode(x,y)->connectionProps[i].typeVal;
			if (curType == E_BT_MAINHALL || curType == E_BT_WING) {
				if (lastHeight == -1) {
					lastHeight = getNode(x,y)->connectionProps[i].endHeight;
				}
				else {

					if (getNode(x,y)->connectionProps[i].endHeight != lastHeight) {
						return -1;
					}

					
				}
			}
		}

		return lastHeight;
	}


	int touches2(int x, int y, int buildingType) {
		int i;
		int tot = 0;
		int testX;
		int testY;

		for (i = 0; i < 4; i++) {

			testX = x + gw->dirModX[i];
			testY = y + gw->dirModY[i];

			tot += touches(testX,testY,buildingType);

		}

		return tot;

	}

	void connectNodes(int _x1, int _y1, int _x2, int _y2, int buildingType, int id) {
		

		// 0: x+
		// 1: x-
		// 2: y+
		// 3: y-

		int x1 = _x1;
		int y1 = _y1;
		int x2 = _x2;
		int y2 = _y2;

		int boff = 0;

		int i;

		if (x1 > x2) {
			std::swap(x1,x2);
		}
		if (y1 > y2) {
			std::swap(y1,y2);
		}

		BuildingNode* n[2];

		int rNum = iGenRand(2,maxFloors-1);


		if (
			x1 < 0 || x1 >= iBuildingNodesPerSide || 
			x2 < 0 || x2 >= iBuildingNodesPerSide || 
			y1 < 0 || y1 >= iBuildingNodesPerSide || 
			y2 < 0 || y2 >= iBuildingNodesPerSide
		) {
			doTraceND("out of range");
		}
		else {

			n[0] = getNode(x1,y1);
			n[1] = getNode(x2,y2);

			if (id >= 0) {
				n[0]->id = id;
				n[1]->id = id;
			}

			if (x1 == x2) { // is vertical
				boff = 2;
			}
			else {
				boff = 0;
			}

			for (i = 0; i < 2; i++) {
				n[i]->connectionProps[i+boff].typeVal = buildingType;
				n[i]->connectionProps[i+boff].endHeight = rNum;
				n[i]->connectionProps[i+boff].begHeight = 0;
			}



		}

		
	}


};