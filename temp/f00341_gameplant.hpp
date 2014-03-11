
class GamePlant {

public:

	Singleton* singleton;

	FIVector4 origin;
	FIVector4 tempv0;
	FIVector4 tempv1;
	FIVector4 tempv2;
	FIVector4 tempv3;


	GamePlantNode* trunkNode;
	GamePlantNode* rootsNode;

	PlantRules* rootRules;
	PlantRules* trunkRules;




	static PlantRules allPlantRules[E_PT_LENGTH];
	
	// static void sv(float* vals, float v0, float v1) {
	// 	vals[0] = v0;
	// 	vals[1] = v1;
	// }

	static void initAllPlants(Singleton* _singleton) {
		int i;
		int j;


		float pi = 3.14159;

		PlantRules* pr;

		pr = &(allPlantRules[E_PT_OAK_TRUNK]);
		pr->numChildren[0] = 1.0f;
		pr->numChildren[1] = 5.0f;
		pr->divergenceAngleV[0] = pi/6.0f;
		pr->divergenceAngleV[1] = pi/6.0f;
		pr->begThickness = 1.0f;
		pr->endThickness = 0.0f;
		pr->baseLength = 4.0f;
		pr->nodeLengthMultiplier = 0.75f;
		pr->numGenerations = 5.0f;
		pr->angleUniformityU = 1.0f;
		pr->isInit = 0.0;

		
		for (i = 0; i < E_PT_LENGTH; i++) {
			if (allPlantRules[i].isInit == 0.0f) {

				allPlantRules[i].begThickness *= _singleton->pixelsPerMeter;
				allPlantRules[i].endThickness *= _singleton->pixelsPerMeter;
				allPlantRules[i].baseLength *= _singleton->pixelsPerMeter;
				

			}
		}
	}


	float gv(float* vals) {
		float lerp = fGenRand();
		return vals[0]*lerp + vals[1]*(1.0f-lerp);
	}

	GamePlant() {
		trunkNode = NULL;
		rootsNode = NULL;
	}

	void init(
		Singleton* _singleton,
		PlantRules* _rootRules,
		PlantRules* _trunkRules,
		FIVector4* _origin
	) {
		singleton = _singleton;
		rootRules = _rootRules;
		trunkRules = _trunkRules;
		origin.setFXYZRef(_origin);

		if (trunkNode == NULL) {
			trunkNode = new GamePlantNode();
		}

		
		trunkNode->init(NULL,trunkRules->numChildren[1],gv(trunkRules->numChildren));
		trunkNode->tangent.setFXYZ(0.0f,0.0f,1.0f);
		trunkNode->begPoint.setFXYZRef(&origin);
		trunkNode->endPoint.setFXYZRef(&origin);
		trunkNode->endPoint.addXYZ(0.0,0.0,trunkRules->baseLength);
		trunkNode->updateTangent(gv(trunkRules->divergenceAngleV));

		int i;
		float maxLength = 0.0f;
		float curMult = 1.0f;

		int maxGen = trunkRules->numGenerations;

		for (i = 0; i < maxGen; i++) {
			maxLength += trunkRules->baseLength*curMult;
			curMult *= trunkRules->nodeLengthMultiplier;
		}

		float curLerp;
		
		curLerp = (0.0f);
		trunkNode->begThickness = (1.0f-curLerp)*trunkRules->begThickness + curLerp*trunkRules->endThickness;

		curLerp = ((trunkRules->baseLength)/maxLength);
		trunkNode->endThickness = (1.0f-curLerp)*trunkRules->begThickness + curLerp*trunkRules->endThickness;

		applyRules(
			trunkRules,
			trunkNode,
			0,
			maxGen,
			trunkRules->baseLength*(trunkRules->nodeLengthMultiplier),
			trunkRules->baseLength,
			maxLength
		);
	}

	void applyRules(
		PlantRules* rules,
		GamePlantNode* curParent,
		int curGen,
		int maxGen,
		float curLength,
		float totLength,
		float maxLength
	) {


		//cout << "curLength: " << curLength << "\n";

		int i;
		float fi;
		float fNumChildren;
		float curLerp;


		GamePlantNode* curChild;

		fNumChildren = (float)(curParent->numChildren);

		for (i = 0; i < curParent->numChildren; i++) {

			fi = ((float)i)/fNumChildren;

			curChild = &(curParent->children[i]);


			if (curGen == maxGen-1) {
				curChild->init(curParent,0,0);
			}
			else {
				curChild->init(curParent,rules->numChildren[1], max( gv(rules->numChildren)-(float)curGen, 1.0f) );
			}

			
			curChild->begPoint.setFXYZRef(&(curParent->endPoint));
			curChild->endPoint.setFXYZRef(&(curParent->endPoint));

			axisRotationInstance.doRotation(&tempv0,&(curParent->baseShoot),&(curParent->tangent),fi*2.0*3.14159);
			curChild->endPoint.addXYZRef(&tempv0,curLength);
			curChild->updateTangent(gv(rules->divergenceAngleV));

			curLerp = (totLength/maxLength);
			curChild->begThickness = (1.0f-curLerp)*rules->begThickness + curLerp*rules->endThickness;

			curLerp = ((totLength+curLength)/maxLength);
			curChild->endThickness = (1.0f-curLerp)*rules->begThickness + curLerp*rules->endThickness;

			if (curGen < maxGen) {
				applyRules(rules, curChild, curGen + 1, maxGen, curLength*(trunkRules->nodeLengthMultiplier), totLength+curLength, maxLength);
			}

		}
	}

};
