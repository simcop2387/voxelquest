class GameBlock {
public:


	
	Singleton* singleton;
	
	int thisIndex;
	int blockSizeInHolders;
	
	FIVector4 offsetInBlocks;

	std::vector<GameGeom*> gameGeom;



	GameBlock() {

	}	

	void init(Singleton* _singleton, int ind, int _x, int _y) {
		singleton = _singleton;
		thisIndex = ind;
		offsetInBlocks.setIXYZ(_x,_y,0);


		int i;

		float x;
		float y;
		float z;

		blockSizeInHolders = singleton->blockSizeInHolders;

		float uvSize = 256.0;

		for (i = 0; i < 8; i++) {
			gameGeom.push_back(new GameGeom());
			
			x = (offsetInBlocks.getFX()+fGenRand()) * singleton->blockSizeInPixels;
			y = (offsetInBlocks.getFY()+fGenRand()) * singleton->blockSizeInPixels;

			x = floor(x/uvSize)*uvSize;

			z = singleton->getHeightAtPixelPos(x,y);
			z = floor(z/uvSize)*uvSize;


			gameGeom.back()->initRand(i,x,y,z,uvSize);

		}
		


	}



	// void addGeom(GameGeom* geom) {
		
	// 	int i;
	// 	int j;
	// 	int k;
	// 	int ind;
	// 	int bufSize = (visPageSizeInPixels*singleton->bufferMult);
	// 	GamePageHolder* gph;
	// 	FIVector4 start;
	// 	FIVector4 end;

	// 	start.copyFrom( &(geom->boundsMinInPixels) );
	// 	end.copyFrom( &(geom->boundsMaxInPixels) );

	// 	start.addXYZ(-bufSize);
	// 	end.addXYZ(bufSize);

	// 	start.intDivXYZ(singleton->holderSizeInPixels);
	// 	end.intDivXYZ(singleton->holderSizeInPixels);

	// 	start.clampZ(&(singleton->origin),&(singleton->worldSizeInHoldersM1));
	// 	end.clampZ(&(singleton->origin),&(singleton->worldSizeInHoldersM1));


	// 	for (k = start.getIZ(); k <= end.getIZ(); k++ ) {
	// 		for (j = start.getIY(); j <= end.getIY(); j++ ) {
	// 			for (i = start.getIX(); i <= end.getIX(); i++ ) {
	// 				gph = getHolderAtCoords(i,j,k,true);
	// 				gph->containsGeomIds.push_back(geom->id);
	// 			}
	// 		}
	// 	}

	// }





}