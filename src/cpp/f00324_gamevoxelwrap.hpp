
class GameVoxelWrap {
public:
	Singleton* singleton;
	
	
	VoxelBuffer* voxelBuffer;
	PaddedData* basePD;
	PaddedDataEntry* baseData;
	//ivec3 octOffsetInVoxels;
	
	int lastFFSteps;
	int curPD;
	//int dimInVoxels;
	//int octInVoxels;
	//float fVoxelsPerCell;
	int voxelsPerCell;
	int cellsPerHolder;
	int cellsPerHolderPad;
	int voxelsPerHolderPad;
	int voxelsPerHolder;
	int paddingInCells;
	int paddingInVoxels;
	
	ivec3 offsetInCells;
	ivec3 offsetInVoxels;
	
	vec3 oneVec;
	vec3 halfOff;
	vec3 crand0;
	vec3 crand1;
	vec3 crand2;
	
	FBOWrapper *hmFBO;
	
	GameVoxelWrap() {
		hmFBO = NULL;
		lastFFSteps = 0;
		voxelBuffer = NULL;
		basePD = NULL;
		baseData = NULL;
		
		oneVec = vec3(1.0f,1.0f,1.0f);
		halfOff = vec3(0.5,0.5,0.5);
		crand0 = vec3(12.989, 78.233, 98.422);
		crand1 = vec3(93.989, 67.345, 54.256);
		crand2 = vec3(43.332, 93.532, 43.734);
	}
	
	void init(
		Singleton* _singleton
		//,int _dimInVoxels
	) {
		singleton = _singleton;
		//dimInVoxels = _dimInVoxels;
		// octInVoxels = dimInVoxels*2;
		
		// octOffsetInVoxels = ivec3(
		// 	(octInVoxels-dimInVoxels)/2,
		// 	(octInVoxels-dimInVoxels)/2,
		// 	(octInVoxels-dimInVoxels)/2	
		// );
		
		voxelsPerCell = singleton->voxelsPerCell;
		//fVoxelsPerCell = voxelsPerCell;
		cellsPerHolder = singleton->cellsPerHolder;
		cellsPerHolderPad = singleton->cellsPerHolderPad;
		paddingInCells = singleton->paddingInCells;
		paddingInVoxels = paddingInCells*voxelsPerCell;
		
		voxelsPerHolderPad = singleton->voxelsPerHolderPad;
		voxelsPerHolder = singleton->voxelsPerHolder;
		
		hmFBO = singleton->getFBOWrapper("hmFBO", 0);
		
		
	}
	
	
	
	void fillVec(GamePageHolder* gph) {
		int totSize = voxelBuffer->voxelList.size();
		
		if (totSize <= 0) {
			return;
		}
		
		int u;
		int t;
		int q;
		int q2;
		int p;
		int p2;
		int m;
		float fVPC = voxelsPerCell;
		fVPC = 1.0f/fVPC;
		
		ivec3 voxOffset;
		ivec3 voxOffset2;
		ivec3 cellOffset;
		ivec3 localVoxOffset;
		
		//vec3 fLocalVoxOffset;
		vec3 fVO;
		
		
		int ii;
		int jj;
		int kk;
		
		int ii2;
		int jj2;
		int kk2;
		
		int ii3;
		int jj3;
		int kk3;
		
		int ind;
		
		int xx;
		int yy;
		int zz;
		
		uint curFlags;
		uint tempFlags;
		uint normFlags;
		
		float tempData[16];
		
		CubeWrap* curCW;
		
		int tempInd;
		int VLIndex;
		
		float normLength;
		
		vec3 curNorm;
		vec3 curNorm2;
		vec3 totNorm;
		vec3 tempPos;
		vec3 tempPos2;
		vec3 tempPos3;
		vec3 zeroVec = vec3(0.0f,0.0f,0.0f);
		
		float weight;
		float weight2;
		
		uint curNID;
		uint testNID;
		
		int voxelNormRad = singleton->iGetConst(E_CONST_VOXEL_NORM_RAD);
		int cellAORad = singleton->iGetConst(E_CONST_CELL_AO_RAD);
		float voxelAORad = voxelsPerCell*cellAORad;
		
		float frad = voxelNormRad;
		float maxRad = (frad*frad + frad*frad + frad*frad)*1.125f;
		
		int dataSize = 4;
		
		if (DO_POINTS) {
			dataSize = 8;
		}
		
		int passNum;
		
		int cellInd;
		int cellInd2;
		int curSize;
		
		//VoxelEntry ve;
		
		float totWeight = 0.0f;
		float weightCount = 1.0f;
		
		int ww;
		int curMat = 0;
		
		int mipAmount;
		int curMipSize;
		int mipInd;
		
		
		
		if (DO_AO) {
			for (p = 0; p < totSize; p++) {
				q = voxelBuffer->voxelList[p].viIndex;
				kk = q/(voxelsPerHolderPad*voxelsPerHolderPad);
				jj = (q-kk*voxelsPerHolderPad*voxelsPerHolderPad)/voxelsPerHolderPad;
				ii = q-(kk*voxelsPerHolderPad*voxelsPerHolderPad + jj*voxelsPerHolderPad);
				
				if (voxelBuffer->getFlag(q,E_OCT_SURFACE)) {
					
					kk2 = kk/voxelsPerCell;
					jj2 = jj/voxelsPerCell;
					ii2 = ii/voxelsPerCell;
					
					cellInd = kk2*cellsPerHolderPad*cellsPerHolderPad + jj2*cellsPerHolderPad + ii2;
					
					voxelBuffer->cellLists[cellInd].indexArr[voxelBuffer->cellLists[cellInd].curSize] = p;
					voxelBuffer->cellLists[cellInd].curSize++;
					
					voxOffset.x = ii;
					voxOffset.y = jj;
					voxOffset.z = kk;
					//voxOffset -= paddingInVoxels;
					
					tempPos.x = voxOffset.x;
					tempPos.y = voxOffset.y;
					tempPos.z = voxOffset.z;
					
					voxelBuffer->voxelList[p].pos = tempPos;
					
				}
			}
		}
		
		
		int maxPass = 1;
		
		if (DO_AO) {
			maxPass = 2;
		}
		
		for (passNum = 0; passNum < maxPass; passNum++) {
			
			// for (u = 0; u < voxelBuffer->mipMaps[1].mipList.size(); u++) {
			// 	p = voxelBuffer->mipMaps[1].mipList[u];
			for (p = 0; p < totSize; p++) {
				
				q = voxelBuffer->voxelList[p].viIndex;
				kk = q/(voxelsPerHolderPad*voxelsPerHolderPad);
				jj = (q-kk*voxelsPerHolderPad*voxelsPerHolderPad)/voxelsPerHolderPad;
				ii = q-(kk*voxelsPerHolderPad*voxelsPerHolderPad + jj*voxelsPerHolderPad);
				
				if (voxelBuffer->getFlag(q,E_OCT_SURFACE)) {
					
					voxOffset.x = ii;
					voxOffset.y = jj;
					voxOffset.z = kk;
					//voxOffset -= paddingInVoxels;
					
					tempPos.x = voxOffset.x;
					tempPos.y = voxOffset.y;
					tempPos.z = voxOffset.z;
					
					
					
					
					if (inBounds(&voxOffset,paddingInVoxels,voxelsPerHolder+paddingInVoxels)) {
						
						
						curNID = voxelBuffer->voxelList[p].normId;
						
						
						
						
						curFlags = voxelBuffer->getFlags(q);
						
						switch (passNum) {
							case 0:
								
								totNorm.set(0.0f,0.0f,0.0f);
								
								for (zz = -voxelNormRad; zz <= voxelNormRad; zz++) {
									for (yy = -voxelNormRad; yy <= voxelNormRad; yy++) {
										for (xx = -voxelNormRad; xx <= voxelNormRad; xx++) {
											
											
											
											// if (
											// 	(zz == 0) && (yy == 0) && (xx == 0)	
											// ) {
												
											// }
											// else {
												
											// }
											
											tempInd = (zz+kk)*voxelsPerHolderPad*voxelsPerHolderPad + (yy+jj)*voxelsPerHolderPad + (xx + ii);
											tempFlags = voxelBuffer->getFlagsAtNode(tempInd);
											VLIndex = voxelBuffer->getIndAtNode(tempInd);
											
											if ((VLIndex == -1)||(curNID == 0)) {
												testNID = 0;
											}
											else {
												testNID = voxelBuffer->voxelList[VLIndex].normId;
											}
											
											// if (p%1000 == 0) {
											// 	cout << "curNID " << curNID << "testNID" << testNID << "\n";
											// }
											
											
											
											// if (
											// 	((tempFlags&E_OCT_SURFACE) > 0)
											// 	&& (curNID == testNID)
											// ) {
											// 	normFlags = (tempFlags&63);
											// 	totNorm += BASE_NORMALS[normFlags];
											// }
											
											
											
											if ((tempFlags&E_OCT_SURFACE) > 0) {
												
												weight = maxRad-(xx*xx + yy*yy + zz*zz);
												
												normFlags = (tempFlags&63);
											
												if (curNID == testNID) {
													totNorm += BASE_NORMALS[normFlags]*weight;
												}
												else {
													totNorm -= BASE_NORMALS[normFlags]*0.75f*weight;
												}
											}
											
											// position(3)
											// normal(3)
											// material(1)
											// id(1)
											
											
											
										}
									}
								}
								
								
								
								
								
								
								
								curMat = voxelBuffer->voxelList[p].matId;

								// if (curMat == TEX_GRASS) {
								// 	tempPos3 = tempPos;
								// 	tempPos3.z = 0.123452f;
								// 	totNorm = randPN(tempPos3);
								// 	totNorm.z = 3.0f;
								// }

								if (totNorm.normalize()) {
									
								}
								else {
									totNorm = vec3(0.0f,0.0f,1.0f);
								}

								voxelBuffer->voxelList[p].normal = totNorm;

								// if (totNorm.z > 0.5f) {
								// 	if (curMat == TEX_EARTH) {
								// 		// if (
								// 		// 	((ii%2)==0) &&
								// 		// 	((jj%2)==0)	
								// 		// ) {
											
								// 		// }
										
								// 		curMat = TEX_GRASS;
								// 	}
								// }


							break;
							
							case 1:
								
								if (DO_AO) {
									curNorm = voxelBuffer->voxelList[p].normal;
									
									kk2 = kk/voxelsPerCell;
									jj2 = jj/voxelsPerCell;
									ii2 = ii/voxelsPerCell;
									
									cellInd = kk2*cellsPerHolderPad*cellsPerHolderPad + jj2*cellsPerHolderPad + ii2;
									
									totWeight = 0.0f;
									weightCount = 0.01f;
									
									for (zz = -cellAORad; zz <= cellAORad; zz++) {
										for (yy = -cellAORad; yy <= cellAORad; yy++) {
											for (xx = -cellAORad; xx <= cellAORad; xx++) {
												cellInd2 =
													(kk2+zz)*cellsPerHolderPad*cellsPerHolderPad +
													(jj2+yy)*cellsPerHolderPad +
													(ii2+xx);
													
												curSize = voxelBuffer->cellLists[cellInd2].curSize;
												
												for (ww = 0; ww < curSize; ww++) {
													p2 = voxelBuffer->cellLists[cellInd2].indexArr[ww];
													q2 = voxelBuffer->voxelList[p2].viIndex;
													
													tempPos2 = voxelBuffer->voxelList[p2].pos;
													curNorm2 = voxelBuffer->voxelList[p2].normal;
													
													totNorm = tempPos2 - tempPos;
													
													normLength = totNorm.length();
													
													if (normLength == 0.0f) {
														
													}
													else {
														totNorm /= normLength;
														
														// weight = how much it occludes; 0: none, 1: all
														
														weight = clampfZO((voxelAORad-normLength)/voxelAORad);
														
														weight2 =
															weight * 
															clampfZO((curNorm.dot(totNorm)+1.0f)*0.5f) * 
															clampfZO(normLength*2.0f/voxelAORad) *
															clampfZO((curNorm.dot(-curNorm2)+1.0f)*0.5f)
														;
														
														totWeight += weight2;
														weightCount += weight;
														
													}
													
													
													
													
													
													// kk3 = q2/(voxelsPerHolderPad*voxelsPerHolderPad);
													// jj3 = (q2-kk*voxelsPerHolderPad*voxelsPerHolderPad)/voxelsPerHolderPad;
													// ii3 = q2-(kk*voxelsPerHolderPad*voxelsPerHolderPad + jj*voxelsPerHolderPad);
													
													// voxOffset2.x = ii3;
													// voxOffset2.y = jj3;
													// voxOffset2.z = kk3;
													//voxOffset2 -= paddingInVoxels;
													
													// voxelBuffer->voxelList[p2]
													/// 
													
													
												}
												
											}
										}
									}
								}
								
								
								
								
								
							
							
							
							break;
						}
						
						
						if (
							(
								DO_AO&&(passNum == 1)
							) ||
							(
								!DO_AO	
							)	
						) {
							
							
							//voxOffset += paddingInVoxels;
							//voxOffset += offsetInVoxels;
							
							fVO.x = voxOffset.x;
							fVO.y = voxOffset.y;
							fVO.z = voxOffset.z;
							//fVO *= fVPC;
							
							totNorm = voxelBuffer->voxelList[p].normal;
							
							
							
							if (DO_POINTS) {
								tempData[0] = fVO.x;
								tempData[1] = fVO.y;
								tempData[2] = fVO.z;
								tempData[3] = 0.0f;// mipLevel 1.0f;
								tempData[4] = totNorm.x;
								tempData[5] = totNorm.y;
								tempData[6] = totNorm.z;
								tempData[7] = curMat*256;
								
								for (m = 0; m < dataSize; m++) {
									gph->vertexVec.push_back(tempData[m]);
								}
								
							}
							// else {
								
							// 	tempData[0] = totNorm.x;
							// 	tempData[1] = totNorm.y;
							// 	tempData[2] = totNorm.z;
							// 	tempData[3] = curMat*256;//totWeight/weightCount;
							// 	gph->vertexVec.push_back(fVO.x);
							// 	gph->vertexVec.push_back(fVO.y);
							// 	gph->vertexVec.push_back(fVO.z);
							// 	gph->vertexVec.push_back(1.0f);
								
							// 	gph->vboWrapper.vboBox(
							// 		fVO.x, fVO.y, fVO.z,
							// 		0.0f,fVPC,
							// 		curFlags,
							// 		tempData,
							// 		4
							// 	);
							// }
						}
						
						
						
						
						
					}
					else {
						// not in bounds
						
						
						
					}
					
					// if (inBounds(&voxOffset,0,voxelsPerHolder)) {
					// 	cellOffset = voxOffset/voxelsPerCell;
					// 	localVoxOffset = voxOffset-(cellOffset*voxelsPerCell);
					// 	//fLocalVoxOffset = toVEC(localVoxOffset);
					// 	//fLocalVoxOffset *= fVPC;
						
					// 	ind = cellOffset.z*cellsPerHolder*cellsPerHolder + cellOffset.y*cellsPerHolder + cellOffset.x;
						
					// 	if (gph->cubeData[ind] == CUBE_DATA_INVALID) {
					// 		gph->cubeWraps.push_back(CubeWrap());
					// 		gph->cubeData[ind] = (gph->cubeWraps.size()-1);
					// 		gph->cubeWraps[gph->cubeData[ind]].init();
					// 	}
						
					// 	curCW = &(gph->cubeWraps[gph->cubeData[ind]]);
						
					// 	curCW->insertValue(&localVoxOffset,voxelBuffer->getFlags(q));//, &fLocalVoxOffset);
					// }
					
					
					
					
					
					
					
					
					
					
					
					
					
				}
				
			}
		}
		
		
		/*
		tempData[0] = fVO.x;
		tempData[1] = fVO.y;
		tempData[2] = fVO.z;
		tempData[3] = 1.0f;
		tempData[4] = totNorm.x;
		tempData[5] = totNorm.y;
		tempData[6] = totNorm.z;
		tempData[7] = curMat*256;
		
		for (m = 0; m < dataSize; m++) {
			gph->vertexVec.push_back(tempData[m]);
		}
		
		
		
		voxOffset += offsetInVoxels;
		
		fVO.x = voxOffset.x;
		fVO.y = voxOffset.y;
		fVO.z = voxOffset.z;
		fVO *= fVPC;
		
		voxOffset += offsetInVoxels;
		fVO *= fVPC;
		
		*/
		
		
		totSize = gph->vertexVec.size();
		gph->begMip[0] = 0;
		gph->endMip[0] = totSize;
		
		
		
		if (DO_MIP) {
			mipAmount = 2;
			
			for (t = 0; t < NUM_MIP_LEVELS; t++) {
				gph->begMip[t+1] = gph->endMip[t];
				
				for (p = 0; p < totSize; p += dataSize) {
					//q = voxelBuffer->voxelList[p].viIndex;
					kk = gph->vertexVec[p+2];//q/(voxelsPerHolderPad*voxelsPerHolderPad);
					jj = gph->vertexVec[p+1];//(q-kk*voxelsPerHolderPad*voxelsPerHolderPad)/voxelsPerHolderPad;
					ii = gph->vertexVec[p+0];//q-(kk*voxelsPerHolderPad*voxelsPerHolderPad + jj*voxelsPerHolderPad);
					
					curMipSize = voxelsPerHolderPad/mipAmount;
					
					kk2 = kk/mipAmount;
					jj2 = jj/mipAmount;
					ii2 = ii/mipAmount;
					
					mipInd = kk2*curMipSize*curMipSize + jj2*curMipSize + ii2;
					
					if (voxelBuffer->mipMaps[t].mipArr[mipInd]) {
						
					}
					else {
						voxelBuffer->mipMaps[t].mipArr[mipInd] = true;
						
						tempData[0] = ii2*mipAmount + mipAmount/2;
						tempData[1] = jj2*mipAmount + mipAmount/2;
						tempData[2] = kk2*mipAmount + mipAmount/2;
						tempData[3] = t+1;//gph->vertexVec[p+3];
						tempData[4] = gph->vertexVec[p+4];
						tempData[5] = gph->vertexVec[p+5];
						tempData[6] = gph->vertexVec[p+6];
						tempData[7] = gph->vertexVec[p+7];
						
						for (m = 0; m < dataSize; m++) {
							gph->vertexVec.push_back(tempData[m]);
						}
						
						// voxelBuffer->mipMaps[t].mipList.push_back(ve);
						// voxelBuffer->mipMaps[t].mipList.back().ind = p;
						// voxelBuffer->mipMaps[t].mipList.back().x = ii2*mipAmount + mipAmount/2;
						// voxelBuffer->mipMaps[t].mipList.back().y = jj2*mipAmount + mipAmount/2;
						// voxelBuffer->mipMaps[t].mipList.back().z = kk2*mipAmount + mipAmount/2;
					}
					
					
				}
				
				gph->endMip[t+1] = gph->vertexVec.size();
				
				mipAmount *= 2;
			}
			
		}
		
		
		totSize = gph->vertexVec.size();
		for (p = 0; p < totSize; p += dataSize) {
				gph->vertexVec[p] = (gph->vertexVec[p] + offsetInVoxels.x)*fVPC; p++;
				gph->vertexVec[p] = (gph->vertexVec[p] + offsetInVoxels.y)*fVPC; p++;
				gph->vertexVec[p] = (gph->vertexVec[p] + offsetInVoxels.z)*fVPC;
				
				p -= 2;
				
		}
		
	}
	
	void process(GamePageHolder* gph) {
		ivec3 cellCoord;
		ivec3 voxResult;
		
		curPD = gph->curPD;
		
		offsetInCells.set(
			gph->gphMinInCells.getIX(),
			gph->gphMinInCells.getIY(),
			gph->gphMinInCells.getIZ()
		);
		
		offsetInVoxels = offsetInCells - paddingInCells;
		offsetInVoxels *= voxelsPerCell;
		

		basePD = (&singleton->pdPool[curPD]);
		baseData = singleton->pdPool[curPD].data;
		
		voxelBuffer = &(basePD->voxelBuffer);
		voxelBuffer->clearAllNodes();
		
		while( findNextCoord(&voxResult) ) {
			floodFill(voxResult);
			//goto DONE_WITH_IT;
		}
		
		DONE_WITH_IT:
		
		fillVec(gph);
		
	}
	
	
	
	
	
	
	
	

	bool findNextCoord(ivec3* voxResult) {
		int i;
		int j;
		int k;
		int q;
		int r;
		
		int ii2;
		int jj2;
		int kk2;
		
		int ii;
		int jj;
		int kk;
		int ikk;
		
		ivec3 curVoxel;
		ivec3 localOffset;
		
		
		int minv = 0;// + paddingInCells;
		int maxv = cellsPerHolderPad; // -paddingInCells;
		
		int lastPtr;
		int cellData;
		int cellData2;
		int voxelsPerCellM1 = voxelsPerCell-1;
		
		bool foundCell = false;
		
		for (i = minv; i < maxv; i++) {
			for (j = minv; j < maxv; j++) {
				for (k = minv; k < maxv; k++) {
					
						
						if (getPadData(i,j,k)->visited) {
							
						}
						else {
							getPadData(i,j,k)->visited = true;						
							
							cellData = getPadData(i,j,k)->cellVal;
							foundCell = false;
							
							if (cellData == E_CD_SOLID) {
								
								for (kk2 = -1; kk2 <= 1; kk2++) {
									for (jj2 = -1; jj2 <= 1; jj2++) {
										for (ii2 = -1; ii2 <= 1; ii2++) {
											cellData2 = getPadData(
												i + ii2,
												j + jj2,
												k + kk2
											)->cellVal;
											
											if (cellData2 != E_CD_SOLID) {
												foundCell = true;
												goto FOUND_CELL_LABEL;
											}
										}
									}
								}
								
							}
							else {
								for (kk2 = -1; kk2 <= 1; kk2++) {
									for (jj2 = -1; jj2 <= 1; jj2++) {
										for (ii2 = -1; ii2 <= 1; ii2++) {
											cellData2 = getPadData(
												i + ii2,
												j + jj2,
												k + kk2
											)->cellVal;
											
											if (cellData2 == E_CD_SOLID) {
												foundCell = true;
												goto FOUND_CELL_LABEL;
											}
										}
									}
								}
							}
								
								
					FOUND_CELL_LABEL:
							
							
							
							
							
							
							
						
							if (foundCell) {
								
								
								localOffset.set(i,j,k);
								localOffset *= voxelsPerCell;
								
								for (kk = 0; kk < voxelsPerCell; kk++) {
									
									ikk = voxelsPerCellM1-kk;
									
									for (r = 0; r < 4; r++) {
										switch (r) {
											case 0:
												curVoxel.set(kk, kk, ikk);
											break;
											case 1:
												curVoxel.set(ikk, kk, ikk);
											break;
											case 2:
												curVoxel.set(kk, ikk, ikk);
											break;
											case 3:
												curVoxel.set(ikk, ikk, ikk);
											break;
										}
										
										//curVoxel += offsetInVoxels;
										curVoxel += localOffset;
										if (isSurfaceVoxel(&curVoxel,lastPtr, false)) {
											voxResult->set(
												curVoxel.x, curVoxel.y, curVoxel.z
											);
											return true;
										}
										
									}
								}
								
								for (jj = 0; jj < voxelsPerCell; jj++) {
									for (ii = 0; ii < voxelsPerCell; ii++) {
										for (r = 0; r < 6; r++) {
											switch (r) {
												case 0:
													curVoxel.set(0, ii, jj);
												break;
												case 1:
													curVoxel.set(voxelsPerCellM1, ii, jj);
												break;
												case 2:
													curVoxel.set( ii, 0, jj );
												break;
												case 3:
													curVoxel.set( ii, voxelsPerCellM1, jj );
												break;
												case 4:
													curVoxel.set(ii,jj,0);
												break;
												case 5:
													curVoxel.set(ii,jj,voxelsPerCellM1);
												break;
											}
											
											//curVoxel += offsetInVoxels;
											curVoxel += localOffset;
											if (isSurfaceVoxel(&curVoxel,lastPtr,false)) {
												voxResult->set(
													curVoxel.x, curVoxel.y, curVoxel.z
												);
												return true;
											}
											
									}
								}
								
								
								
							}
						
						}
						
								
							
					}
						
				}
			}
		}
		
		return false;
	}

	bool inBounds(ivec3* pos, int minB, int maxB) {
		
		// ivec3 minB = offsetInVoxels - octOffsetInVoxels;
		// ivec3 maxB = offsetInVoxels + octOffsetInVoxels;
		
		// minB += (0);
		// maxB += (dimInVoxels - 0);
		
		
		
		return (
			(pos->x >= minB) && (pos->x < maxB)  &&
			(pos->y >= minB) && (pos->y < maxB)  &&
			(pos->z >= minB) && (pos->z < maxB)
		);
		
	}


	int getNode(ivec3* pos) {
		return pos->x + pos->y*voxelsPerHolderPad + pos->z*voxelsPerHolderPad*voxelsPerHolderPad;
	}

	void floodFill(ivec3 startVox) {
		basePD->fillStack.clear();
		basePD->fillStack.push_back(startVox);
		
		ivec3 curVox;
		ivec3 tempVox;
		int curNode;
		int tempNode;
		
		int q;
		int lastPtr;
		
		bool foundNext;
		
		lastFFSteps = 0;
		
		curNode = getNode(&startVox);
		voxelBuffer->setFlag(curNode,E_OCT_VISITED);
		
		while (basePD->fillStack.size() > 0) {
			
			lastFFSteps++;
			
			curVox = basePD->fillStack.back();
			basePD->fillStack.pop_back();
			
			curNode = getNode(&curVox);
			
			
			//foundNext = false;
			
			for (q = 0; q < NUM_ORIENTATIONS; q++) {
				tempVox = curVox + DIR_VECS_IV[q];
				
				
				if (isSurfaceVoxel(&tempVox,lastPtr,true)) {
					basePD->fillStack.push_back(tempVox);
					voxelBuffer->setFlag(lastPtr,E_OCT_VISITED);
					// foundNext = true;
					// goto NEXT_FF_ITERATION;
				}
				
			}
			
			for (q = 0; q < NUM_ORIENTATIONS; q++) {
				tempVox = curVox + DIR_VECS_IV[q];
				
				
				
				if (isInvSurfaceVoxel(&tempVox,curNode,lastPtr,true)) {
					basePD->fillStack.push_back(tempVox);
					voxelBuffer->setFlag(lastPtr,E_OCT_VISITED);
					// foundNext = true;
					// goto NEXT_FF_ITERATION;
				}
				
				
			}
			
// NEXT_FF_ITERATION:
			
// 			if (foundNext) {
// 				basePD->fillStack.push_back(tempVox);
// 			}
			
		}
		
	}



	

	bool isInvSurfaceVoxel(ivec3* pos, int ignorePtr, int &curPtr, bool checkVisited) {
		int q;
		ivec3 tempVox;
		
		curPtr = getVoxelAtCoord(pos);
		if (curPtr < 0) {
			return false;
		}
		
		
		int tempPtr;
		
		if (checkVisited) {
			if (voxelBuffer->getFlag(curPtr,E_OCT_VISITED)) {
				return false;
			}
		}
		
		if ( !(voxelBuffer->getFlag(curPtr,E_OCT_SOLID)) ) {
			
			for (q = 0; q < NUM_ORIENTATIONS; q++) {
				
				tempVox = (*pos) + DIR_VECS_IV[q];
				tempPtr = getVoxelAtCoord(&tempVox);
				if (tempPtr >= 0) {
					if (tempPtr == ignorePtr) {
						// ignore the voxel we came from
					}
					else {
						if (voxelBuffer->getFlag(tempPtr,E_OCT_SOLID)) {
							
							return true;
						}
					}
				}
				
				
				
			}
		}
		
		return false;
	}

	bool isSurfaceVoxel(ivec3* pos, int &curPtr, bool checkVisited) {
		int q;
		ivec3 tempVox;
		
		curPtr = getVoxelAtCoord(pos);
		if (curPtr < 0) {
			return false;
		}
		
		if (checkVisited) {
			if (voxelBuffer->getFlag(curPtr,E_OCT_VISITED)) {
				return false;
			}
		}
		
		int tempPtr;
		
		bool isSurface = false;
		
		uint curSide = E_OCT_XP;
		
		if ( voxelBuffer->getFlag(curPtr,E_OCT_SOLID) ) {
			
			for (q = 0; q < NUM_ORIENTATIONS; q++) {
				
				tempVox = (*pos) + DIR_VECS_IV[q];
				tempPtr = getVoxelAtCoord(&tempVox);
				
				if (tempPtr >= 0) {
					if (voxelBuffer->getFlag(tempPtr,E_OCT_SOLID)) {
						
					}
					else {
						voxelBuffer->setFlag(curPtr, curSide);
						isSurface = true;
					}
				}
				
				curSide *= 2;
				
			}
		}
		
		if (isSurface) {
			voxelBuffer->setFlag(curPtr, E_OCT_SURFACE);
		}
		
		return isSurface;
	}

	int getVoxelAtCoord(ivec3* pos) {
		
		int VLIndex;
		
		//int minB = 0;
		//int maxB = voxelsPerHolderPad;
		if (inBounds(pos,0,voxelsPerHolderPad)) {
			bool wasNew = false;
			int result = voxelBuffer->addNode(pos,wasNew);
			
			if (wasNew) {
				voxelBuffer->setFlag(result, E_OCT_NOTNEW);
				VLIndex = voxelBuffer->addIndex(result);
				calcVoxel(pos,result,VLIndex);
			}
			
			return result;
		}
		else {
			return -1;
		}
		
		
	}



	// todo: 
	// mark cells as visited  <---- important
	// get rid of DONE_WITH_IT
	// should be able to check 6 faces of this holder instead for starting surface point?

	float sampLinear(ivec3* pos, ivec3 offset) {
		int q;
		int i;
		int j;
		int k;
		
		float res[8];
		
		ivec3 newPos = ((*pos) + offset);
		
		int xv = newPos.x/voxelsPerCell;
		int yv = newPos.y/voxelsPerCell;
		int zv = newPos.z/voxelsPerCell;
		
		float fx = newPos.x-xv*voxelsPerCell;
		float fy = newPos.y-yv*voxelsPerCell;
		float fz = newPos.z-zv*voxelsPerCell;
		
		float fVPC = voxelsPerCell;
		
		
		
		fx /= fVPC;
		fy /= fVPC;
		fz /= fVPC;
		
		getPadData(xv,yv,zv)->visited = true;
		
		for (q = 0; q < 8; q++) {
			k = q/(2*2);
			j = (q-k*2*2)/2;
			i = q-(k*2*2 + j*2);
			
			res[q] = getPadData(xv+i,yv+j,zv+k)->terVal;
		}
		
		res[0] = res[0]*(1.0f-fz) + res[4]*fz;
		res[1] = res[1]*(1.0f-fz) + res[5]*fz;
		res[2] = res[2]*(1.0f-fz) + res[6]*fz;
		res[3] = res[3]*(1.0f-fz) + res[7]*fz;
		
		res[0] = res[0]*(1.0f-fy) + res[2]*fy;
		res[1] = res[1]*(1.0f-fy) + res[3]*fy;
		
		return mixf(0.5f,-0.5f,clampfZO(res[0]*(1.0f-fx) + res[1]*fx));
		
	}

	

	PaddedDataEntry* getPadData(int ii, int jj, int kk) {
		
		
		int i = ii;
		int j = jj;
		int k = kk;
		int cphM1 = cellsPerHolderPad-1;
		
		if (i < 0) {
			i = 0;
		}
		if (i > cphM1) {
			i = cphM1;
		}
		
		if (j < 0) {
			j = 0;
		}
		if (j > cphM1) {
			j = cphM1;
		}
		
		if (k < 0) {
			k = 0;
		}
		if (k > cphM1) {
			k = cphM1;
		}
		
		
		return &(
			baseData[
				i + j*cellsPerHolderPad + k*cellsPerHolderPad*cellsPerHolderPad
			]	
		);
	}


	
	
	float rand2D(vec3 co) {
		vec3 myres = co;
		myres.z = 0.1725f;
		return fract(sin(myres.dot(crand0))*43758.8563f);
	}
	
	vec3 randPN(vec3 co) {
			
			vec3 myres = vec3(
					co.dot(crand0),
					co.dot(crand1),
					co.dot(crand2)
			);
			
			myres.doSin();
			myres *= 43758.8563f;
			myres.doFract();
			
			return myres*2.0f - oneVec;
	}
	
	
	
	void getVoro(ivec3* worldPos, ivec3* worldClosestCenter, vec3* otherData, int iSpacing) {
		
		vec3 fWorldPos = vec3(
			worldPos->x,
			worldPos->y,
			worldPos->z
		);
		
		float fSpacing = iSpacing;
		
		fWorldPos *= 1.0f/fSpacing;
		
		vec3 fWorldCellPos = vec3(
			worldPos->x/iSpacing,
			worldPos->y/iSpacing,
			worldPos->z/iSpacing
		);
		
		fWorldPos -= vec3(
			fWorldCellPos.x,
			fWorldCellPos.y,
			fWorldCellPos.z
		);
		
		int i;
		
		vec3 testPos;
		float testDis;
		float variance = 0.4f;
		
		
		vec3 bestPos = VORO_OFFSETS[0] + randPN(fWorldCellPos+VORO_OFFSETS[0])*variance;
		vec3 nextBestPos = bestPos;
		
		float bestDis = fWorldPos.distance(bestPos);
		float nextBestDis = 999999.0f;//fWorldPos.distance(nextBestPos);
		
		for (i = 1; i < 27; i++) {
			testPos = VORO_OFFSETS[i] + randPN(fWorldCellPos+VORO_OFFSETS[i])*variance;
			testDis = fWorldPos.distance(testPos);
			
			if (testDis < bestDis) {
				nextBestDis = bestDis;
				nextBestPos = bestPos;
				
				bestDis = testDis;
				bestPos = testPos;
			}
			else {
				if (testDis < nextBestDis) {
					nextBestDis = testDis;
					nextBestPos = testPos;
				}
			}
		}
		
		worldClosestCenter->set(
			(bestPos.x + fWorldCellPos.x)*fSpacing,
			(bestPos.y + fWorldCellPos.y)*fSpacing,
			(bestPos.z + fWorldCellPos.z)*fSpacing
		);
		
		otherData->x = (bestDis * 2.0f) / (bestDis + nextBestDis);//clampfZO((1.0f - (bestDis * 2.0f / (bestDis + nextBestDis))));
		
		// *norVal = normalize(nextBestPos.xyz-bestPos.xyz);
	}
	
	// should only be called when a new node is inserted!
	void calcVoxel(ivec3* pos, int octPtr, int VLIndex) {
		
		int i;
		int curInd;
		ObjectStruct* curObj;
		
		ivec3 worldPos = (*pos) + offsetInVoxels;
		vec3 fWorldPos = vec3(
			worldPos.x,
			worldPos.y,
			worldPos.z
		);
		//worldPos -= paddingInVoxels;
		
		ivec3 worldClosestCenter;// = worldPos;
		ivec3 localClosestCenter;
		
		vec3 otherData;
		
		float voroSize = voxelsPerCell;
		
		getVoro(&worldPos,&worldClosestCenter, &otherData, voroSize);
		
		float fVPC = voxelsPerCell;
		fVPC = 1.0f/fVPC;
		vec3 fWorldPosCell = fWorldPos*fVPC;
		
		localClosestCenter = worldClosestCenter - offsetInVoxels;
		//localClosestCenter += paddingInVoxels;
		
		
		int vOff = 8;
		float terSampVoro = sampLinear(&localClosestCenter, ivec3(0,0,0));
		float terSampOrig =  sampLinear(pos, ivec3(0,0,0));
		
		bool isGrass = false;
		uint finalMat = TEX_NULL;
		
		// if (isTer) {
		// 	finalMat = TEX_EARTH;
		// }
		float voroJut = clampfZO((fWorldPos.z - (worldClosestCenter.z-voroSize*0.5f))/voroSize);
		
		
		float terSampOrigX = sampLinear(pos, ivec3(vOff,0,0));
		float terSampOrigY = sampLinear(pos, ivec3(0,vOff,0));
		float terSampOrigZ = sampLinear(pos, ivec3(0,0,vOff));
		vec3 terNorm = vec3(
			terSampOrigX-terSampOrig,
			terSampOrigY-terSampOrig,
			terSampOrigZ-terSampOrig
		);
		// terNorm *= -1.0f;
		terNorm.normalize();
		
		// float divVal = singleton->conVals[E_CONST_DIV_VAL];
		
		// vec3 sinPos = vec3(
		// 	sin(fWorldPos.x/divVal),
		// 	sin(fWorldPos.y/divVal),
		// 	sin(fWorldPos.z/divVal)
		// );
		
		
		
		vec3 absNorm = terNorm;
		absNorm.doAbs();
		
		float xv = fWorldPos.x;
		float yv = fWorldPos.y;
		float zv = fWorldPos.z;
		
		float sampScale = singleton->conVals[E_CONST_SAMP_SCALE];
		float hmSampYZ = hmFBO->getPixelAtLinear(yv*sampScale,zv*sampScale, 0);
		float hmSampXZ = hmFBO->getPixelAtLinear(xv*sampScale,zv*sampScale, 0);
		float hmSampXY = hmFBO->getPixelAtLinear(xv*sampScale,yv*sampScale, 0);
		
		float hmSamp = mixf(hmSampYZ,hmSampXZ,absNorm.y);//(hmSampYZ*absNorm.x + hmSampXZ*absNorm.y + hmSampXY*absNorm.z);
		hmSamp = mixf(hmSamp,hmSampXY,absNorm.z);
		
		//terSampVoro += sin(hmSamp*18.0f)*0.25f;
		
		float voroMod1 = clampfZO(1.0f-otherData.x); // the less this is, the farther from center
		float voroMod2 = clampfZO(otherData.x); // the less this is, the closer to center
		
		float hmMod = (sin(hmSamp*9.0f)*singleton->conVals[E_CONST_VORO_STRENGTH]);
		
		//float rockMod = 0.0f;
		bool isRock = (
			(terSampVoro <= 0.4f)	&&
			(voroMod2 < (0.96f + hmMod*0.03f))
		) &&
		((terSampOrig) < (0.3f+voroJut*singleton->conVals[E_CONST_DIV_VAL])); // + (0.2f*(1.0-abs(terNorm.z)))
		
		//+hmMod
		
		//
		
		bool isTer = ((terSampOrig+hmMod) < 0.0f); //(terSampOrig - voroMod1*rockMod*8.0f)
		
		if (isRock) {
			finalMat = TEX_EARTH;
			voxelBuffer->voxelList[VLIndex].normId = 0;//worldClosestCenter.x*3 + worldClosestCenter.y*7 + worldClosestCenter.z*11;
		}
		else {
			if (isTer) {
				finalMat = TEX_EARTH;
				voxelBuffer->voxelList[VLIndex].normId = 0;
			}
			else {
				
			}
		}
		
		if (finalMat == TEX_NULL) {
			if ((terNorm.z > 0.5f)
				// && (
				// 	((worldPos.x%2) == 0) &&
				// 	((worldPos.y%2) == 0)
				// )
			) { //&&(sin(hmSamp*18.0f) > 0.0f)
				int grassOff = rand2D(fWorldPos)*clampfZO((terNorm.z-0.5f)*2.0f)*8.0f;
				float terSampGrass = sampLinear(pos, ivec3(0,0,-grassOff));
				
				if ((terSampGrass-(hmMod+1.0f)*0.15) <= 0.0f) {
					isGrass = true;
					finalMat = TEX_GRASS;
					voxelBuffer->voxelList[VLIndex].normId = 0;
				}
			}
		}
		
		
		// for (i = 0; i < basePD->objectOrder.size(); i++) {
		// 	curInd = basePD->objectOrder[i].v0;
		// 	curObj = &(basePD->tempObjects[curInd]);
			
			
		// 	if (fWorldPosCell.distance(curObj->data[E_OSD_CENTER]) <= curObj->data[E_OSD_RADIUS].x) {
		// 		finalMat = TEX_NULL;
		// 		voxelBuffer->voxelList[VLIndex].normId = 0;
		// 	}
			
			
		// }
		
		
		
		bool isSolid = (finalMat != TEX_NULL);
		
		//clampfZO(terNorm.z)*0.5f + 0.5f
		
		
		//bool isSolid = (terSamp >= 0.5f);
		
		
		if (isSolid) {
			voxelBuffer->setFlag(octPtr, E_OCT_SOLID);
			
			// x = base tex, y = variant
			//floor(curTex.x*256.0*255.0) + floor(curTex.y*255.0);
			
			voxelBuffer->voxelList[VLIndex].matId = finalMat;
		}
		
		
		
	}
	
	
	
	
	
	
};







