
class GameGUI {
private:
	
public:
	Singleton* singleton;

	//Image* textImage;
	//GLuint guiTextureId;

	//Shader shaderGUI;

	
	JSONValue* jvRoot;
	JSONValue* jvTemplates;
	JSONValue* jvSounds;
	
	bool isReady;
	bool isLoaded;
	
	fVector2 mouseTrans;
	
	
	int guiRenderCount;
	vector<UIComponent*> dirtyVec;
	
	
	string tempStrings[10];
	
	
	
	string stringVals[E_GST_LENGTH];
	double floatVals[E_GFT_LENGTH];

	GameGUI() {
		
	}

	void init(Singleton* _singleton) {

		singleton = _singleton;
		isReady = false;
		isLoaded = false;
	}
	
	
	
	
	// inline bool compChildStr(string childStr) {
	// 	return tempStrings[E_GDS_CHILD_TYPE].compare(childStr) == 0;
	// }
	
	
	JSONValue* findNearestKey(JSONValue* jv, string key) {
		int i;
		int numChildren = 0;
		int numFloatingChildren = 0;
		JSONValue* jvChildren = NULL;
		JSONValue* jvFloatingChildren = NULL;
		
		JSONValue* tempJV;
		
		if (jv->HasChild(key)) {
			return jv;
		}
		
		if (jv->HasChild("children")) {
			jvChildren = jv->Child("children");
			numChildren = jvChildren->CountChildren();
		}
		for (i = 0; i < numChildren; i++) {
			tempJV = findNearestKey(jvChildren->Child(i),key);
			
			if (tempJV != NULL) {
				return tempJV;
			}
		}
		
		
		if (jv->HasChild("floatingChildren")) {
			jvFloatingChildren = jv->Child("floatingChildren");
			numFloatingChildren = jvFloatingChildren->CountChildren();
		}
		for (i = 0; i < numFloatingChildren; i++) {
			tempJV = findNearestKey(jvFloatingChildren->Child(i),key);
			
			if (tempJV != NULL) {
				return tempJV;
			}
		}
		
		return NULL;
		
	}
	
	
	void addChildFromJSON(
		int lastIndex,
		JSONValue* jv,
		int curParentId,
		bool isFloating = false
	) {
		int i;
		int j;
		int k;
		//int q;
		joi_type iterator;
		//int numEntries;
		int curIcon = 0;
		int objectId = 0;
		
		JSONValue* curTempl = NULL;
		JSONValue* tempJV = NULL;
		
		
		
	
		if (
			jv->HasChild("template")
		) {
			if (
				jvTemplates->HasChild(jv->Child("template")->string_value)
			) {
				
				curTempl = jvTemplates->Child(jv->Child("template")->string_value);
			}
			else {
				cout << "invalid template \n";
			}
		}
		
		
		for (i = 0; i < E_GST_LENGTH; i++) {
			guiStringValues[i] = "";
		}
		for (i = 0; i < E_GFT_LENGTH; i++) {
			guiFloatValues[i] = 0.0f;
		}
		
		guiFloatValues[E_GFT_HASBACKGROUND] = 1.0;
		guiFloatValues[E_GFT_SINGLELINE] = 1.0;
		guiStringValues[E_GST_SS] = "defaultSS";
		
		
		for (j = 0; j < 2; j++) {
			if (curTempl != NULL) {
				for (i = 0; i < E_GST_LENGTH; i++) {
					if (
						curTempl->HasChild(guiStringTypes[i])
					) {
						guiStringValues[i] = curTempl->Child(guiStringTypes[i])->string_value;
					}
				}
				for (i = 0; i < E_GFT_LENGTH; i++) {
					if (
						curTempl->HasChild(guiFloatTypes[i])
					) {
						guiFloatValues[i] = curTempl->Child(guiFloatTypes[i])->number_value;
					}
				}
			}
			curTempl = jv;
		}
		
		UIComponent* parentPtr = singleton->compStack[curParentId].data;
		
		guiFloatValues[E_GFT_LAYER] = max(
			guiFloatValues[E_GFT_LAYER],
			(double)(parentPtr->layer)
		);
		
		int newParent = parentPtr->addChild(
			lastIndex,
			parentPtr->nodeId,
			guiStringValues,
			guiFloatValues,
			isFloating,
			jv
		);
		
		
		
		
		int numChildren = 0;
		int numFloatingChildren = 0;
		int numDataChildren = 0;
		int numFilters = 0;
		int childType = 0;
		JSONValue* jvChildren = NULL;
		JSONValue* jvFloatingChildren = NULL;
		JSONValue* jvChildTemplate = NULL;
		JSONValue* jvFilter = NULL;
		JSONValue* curFilter = NULL;
		
		JSONValue* jvData = NULL;
		JSONValue* jvDataRoot = NULL;
		JSONValue* curData = NULL;
		
		//tempStrings[E_GDS_CHILD_TYPE] = "";
		
		
		
		
		
		
		
		
		////////////////
		
		
		
		bool doProc = false;
		//bool isInternal = false;
		int totCount = 0;
		int curCT = 0;
		
		
		if (jv->HasChild("childType")) {
			childType = stringToEnum(
				E_GUI_CHILD_TYPE_STRINGS,
				E_GCT_LENGTH,
				jv->Child("childType")->string_value
			);
			//tempStrings[E_GDS_CHILD_TYPE] = jv->Child("childType")->string_value;
			
			
			// if (jv->HasChild("isInternal")) {
			// 	if (jv->Child("isInternal")->number_value != 0 ) {
			// 		// use an internally generated JSON node
			// 		isInternal = true;
			// 	}
			// 	else {
			// 		// todo: load JSON file
			// 	}
			// }
			
			if (jv->HasChild("whereAllEqual")) {
				jvFilter = jv->Child("whereAllEqual");
				numFilters = jvFilter->CountChildren();
			}
			else {
				jvFilter = NULL;
				numFilters = 0;
			}
			
			
			if (jv->HasChild("dataSource")) {
				tempStrings[E_GDS_DATA_SOURCE] = jv->Child("dataSource")->string_value;
				
				// if (isInternal) {
				// 	if (jv->HasChild("dataFile")) {
				// 		tempStrings[E_GDS_DATA_FILE] = jv->Child("dataFile")->string_value;
				// 		jvDataRoot = (singleton->internalJSON[tempStrings[E_GDS_DATA_FILE]]).jv;
				// 	}
					
					
				// }
				// else {
					
					if (jv->HasChild("dataFile")) {
						tempStrings[E_GDS_DATA_FILE] = jv->Child("dataFile")->string_value;
						
						if ( jv->HasChild("dataParams") ) {
							jvDataRoot = singleton->fetchJSONData(tempStrings[E_GDS_DATA_FILE], false, jv->Child("dataParams"));
						}
						else {
							jvDataRoot = singleton->fetchJSONData(tempStrings[E_GDS_DATA_FILE], false, NULL);
						}
					}
					else {
						jvDataRoot = jvRoot;
					}
				//}
				
				if (jvDataRoot != NULL) {
					
					
					singleton->getJVNodeByString(jvDataRoot, &jvData, tempStrings[E_GDS_DATA_SOURCE]);
					
					numDataChildren = jvData->CountChildren();
					if (jv->HasChild("childTemplate")) {
						jvChildTemplate = jv->Child("childTemplate");
					}
				}
			}
			
			
			if ((jvData != NULL)&&(jvChildTemplate != NULL)) {
				
				curCT = childType;
				
				// if (compChildStr("E_GCT_INV_ITEM")) {
				// 	curCT = E_GCT_INV_ITEM;
				// }
				// else if (compChildStr("E_GCT_SHADER_PARAM")) {
				// 	curCT = E_GCT_SHADER_PARAM;
				// }
				// else if (compChildStr("E_GCT_GENERIC")) {
				// 	curCT = E_GCT_GENERIC;
				// }
				
				//////////////////////////////////////////////////////////////////////
				//////////////////////////////////////////////////////////////////////
				
				
				
				
				for (i = 0; i < numDataChildren; i++) {
					
					curData = jvData->Child(i);
					if (curData == NULL) {
						cout << "NULL DATA\n";
						tempStrings[E_GDS_LAST_KEY] = "";
					}
					else {
						tempStrings[E_GDS_LAST_KEY] = jvData->lastKey;
					}
					
					
					doProc = true;
					if (jvFilter != NULL) {
						
						
						for (j = 0; j < numFilters; j++) {
							curFilter = jvFilter->Child(j);
							
							if ( curData->Child(curFilter->Child(0)->string_value)->IsNumber() ) {
								if (
									curData->Child(curFilter->Child(0)->string_value)->number_value !=
									curFilter->Child(1)->number_value
								) {
									doProc = false;
								}
							}
							else {
								// todo: string support
							}
						}
						
						
					}
					
					if (doProc) {
						switch (curCT) {
							case E_GCT_INV_ITEM:
								tempStrings[E_GDS_CHILD_NAME] = curData->Child("name")->string_value;
								
								curIcon = singleton->gem->entIdToIcon[
									(int)(jvRoot->
									Child("itemDefs")->
									Child(tempStrings[E_GDS_CHILD_NAME])->
									Child("iconNum")->
									number_value)
								];
									
									
								
								jvChildTemplate->
									Child("floatingChildren")->
									Child(0)->
									Child("children")->
									Child(0)->
									Child("label")->
									string_value = 
										jvRoot->
										Child("itemDefs")->
										Child(tempStrings[E_GDS_CHILD_NAME])->
										Child("class")->
										string_value;
								
								tempStrings[E_GDS_MATERIAL] = curData->Child("mat")->string_value;
								if (tempStrings[E_GDS_MATERIAL].compare("none") == 0) {
									tempStrings[E_GDS_MATERIAL] = "";
								}
								
								jvChildTemplate->Child("label")->string_value = 
									i__s(curIcon) +
									"& " +
									tempStrings[E_GDS_MATERIAL] +
									" " +
									tempStrings[E_GDS_CHILD_NAME];
							break;
							case E_GCT_SHADER_PARAM:
									
								jvChildTemplate->Child("label")->string_value = 
									curData->Child("shaderName")->string_value +
									"." +
									curData->Child("paramName")->string_value;
								
								jvChildTemplate->Child("uid")->string_value = curData->Child("uid")->string_value;
									
								jvChildTemplate->Child("callbackData")->Child("shaderName")->string_value = 
									curData->Child("shaderName")->string_value;
								jvChildTemplate->Child("callbackData")->Child("paramName")->string_value = 
									curData->Child("paramName")->string_value;
									
							break;
							
							case E_GCT_GENERIC:
							
								singleton->splitStrings.clear();
								singleton->splitStrings = split(tempStrings[E_GDS_LAST_KEY], '_');
								
								if (jvChildTemplate->HasChild("label")) {
									
									switch(singleton->splitStrings.size()) {
										case 0:
										case 1:
											jvChildTemplate->Child("label")->string_value = tempStrings[E_GDS_LAST_KEY];
										break;
										case 2:
										case 3:
											jvChildTemplate->Child("label")->string_value = singleton->splitStrings[1];
										break;
									}
								}
								
								tempJV = findNearestKey(jvChildTemplate,"dataSource");
								if (tempJV != NULL) { // is a branch node
									tempJV->Child("dataSource")->string_value =
										tempStrings[E_GDS_DATA_SOURCE] + "." + tempStrings[E_GDS_LAST_KEY];										
								}
								else { // is a leaf node
									
									if (jvChildTemplate->HasChild("dataRef")) {
										jvChildTemplate->Child("dataRef")->string_value =
											tempStrings[E_GDS_DATA_SOURCE] + "." + tempStrings[E_GDS_LAST_KEY];
									}
									if (jvChildTemplate->HasChild("dataFile")) {
										jvChildTemplate->Child("dataFile")->string_value =
											tempStrings[E_GDS_DATA_FILE];
									}
								}
								
								
								
							break;
							
							case E_GCT_CONTAINER:
							
								// curIcon = singleton->gem->entIdToIcon[
								// 	(int)(curData->Child("objectType")->number_value)
								// ];
								objectId = curData->Child("objectId")->number_value;
								jvChildTemplate->Child("objectId")->number_value = objectId;
								jvChildTemplate->Child("label")->string_value = singleton->gem->getStringForObjectId(objectId);
							break;
							
							case E_GCT_CONTAINER_PARENT:
								
								
								tempJV = findNearestKey(jvChildTemplate,"objectId");
								if (tempJV != NULL) {
									tempJV->Child("objectId")->number_value = curData->Child("objectId")->number_value;
								}
								
								tempJV = findNearestKey(jvChildTemplate,"dataSource");
								if (tempJV != NULL) {
									tempJV->Child("dataSource")->string_value = "objects." + i__s(totCount) + ".children";								
								}
							break;
							
							default:
								//numEntries = curData->CountChildren();
								// for (q = 0; q < numEntries; q++) {
								// 	jvChildTemplate->Child("label")->string_value = singleton->gem->getStringForObjectId(objectId);
								// }
								
								for (
									iterator = curData->object_value.begin();
									iterator != curData->object_value.end();
									iterator++
								) {
									// iterator->first = key
									// iterator->second = value
									
									// if (jvChildTemplate->HasChild(iterator->first)) {
									// 	// todo: report error if key does not exist?
									// }
									
									if (iterator->second->IsNumber()) {
										jvChildTemplate->Child(iterator->first)->number_value = iterator->second->number_value;
									}
									else {
										// todo: support additional JSON types
										jvChildTemplate->Child(iterator->first)->string_value = iterator->second->string_value;
									}
									
									
								}
								
								
								
							break;
						}
						
						
						
						
						
						// copy template to new child
						jv->Child("children")->array_value.push_back(
							JSON::Parse(jvChildTemplate->Stringify().c_str())
						);
						
						
						
						totCount++;
						
						
					}
					
					
				
				}
			}
			
		}
		
		
		
		
		////////////////
		
		
		
		
		
		
		
		
		
		
		if (jv->HasChild("children")) {
			jvChildren = jv->Child("children");
			numChildren = jvChildren->CountChildren();
		}
		for (i = 0; i < numChildren; i++) {
			addChildFromJSON(-1,jvChildren->Child(i),newParent, false);
		}
		
		
		if (jv->HasChild("floatingChildren")) {
			jvFloatingChildren = jv->Child("floatingChildren");
			numFloatingChildren = jvFloatingChildren->CountChildren();
		}
		for (i = 0; i < numFloatingChildren; i++) {
			addChildFromJSON(-1,jvFloatingChildren->Child(i),newParent,true);
		}
		
	}
	
	
	void deleteJSONNodes(JSONValue* jv) {
		JSONValue* jvChildren = NULL;
		JSONValue* jvFloatingChildren = NULL;
		
		int numChildren = 0;
		int numFloatingChildren = 0;
		
		int i;
		
		
		
		
		
		
		if (jv->HasChild("floatingChildren")) {
			jvFloatingChildren = jv->Child("floatingChildren");
			numFloatingChildren = jvFloatingChildren->CountChildren();
		}
		for (i = 0; i < numFloatingChildren; i++) {
			deleteJSONNodes(jvFloatingChildren->Child(i));
		}
		
		if (jv->HasChild("children")) {
			jvChildren = jv->Child("children");
			numChildren = jvChildren->CountChildren();
		}
		for (i = 0; i < numChildren; i++) {
			deleteJSONNodes(jvChildren->Child(i));
		}
		
		if (jv->HasChild("dataSource")) {
				jvChildren->array_value.clear();
		}
		
		
	}
	
	
	void clearRenderOrder() {
		int i;
		for (i = 0; i < singleton->compStack.size(); i++) {
			singleton->compStack[i].data->layerId = -1;
		}
		
		for (i = 0; i < MAX_UI_LAYERS; i++) {
			singleton->guiLayers[i].clear();
		}
		
	}
	
	void deleteNodes(UIComponent* curNode) {
		int i;
		int curNodeId;
		
		for (i = 0; i < curNode->getChildCount(); i++) {
			deleteNodes(curNode->getChild(i));
		}
		
		for (i = 0; i < curNode->getFloatingChildCount(); i++) {
			deleteNodes(curNode->getFloatingChild(i));
		}
		
		curNodeId = curNode->nodeId;
		
		
		singleton->compMap[curNode->uid].nodeId = -1;
		
		// if (curNode->layerId != -1) {
		// 	singleton->guiLayers[curNode->layer][curNode->layerId] = -1;
		// 	singleton->emptyLayers[curNode->layer].push_back(curNode->layerId);
		// }
		
		//delete (singleton->compStack[curNodeId].data);
		//singleton->compStack[curNodeId].data = NULL;
		singleton->compStack[curNodeId].isValid = false;
		singleton->emptyStack.push_back(curNodeId);
		
		curNode->clearChildren();
		
	}
	
	void refreshNode(UIComponent* oldNode) {
		
		if (oldNode == NULL) {
			cout << "refreshNode is NULL\n";
		}
		
		isReady = false;
		isLoaded = false;
		
		//TEMP_DEBUG = true;
		
		
		// TODO: there is a problem with this - templated data must be cleared
		JSONValue* oldJV = oldNode->jvNodeNoTemplate;
		
		int curParentId = oldNode->parentId;
		int lastIndex = oldNode->index;
		//oldNode->getParent()->getParent()->isDirty = true;
		singleton->compStack[0].data->isDirty = true;
		
		
		deleteNodes(oldNode);
		
		
		deleteJSONNodes(oldJV);
		
		
		addChildFromJSON(
			lastIndex,
			oldJV,
			curParentId,
			false
		);
		
		clearRenderOrder();
		
		
		TEMP_DEBUG = false;
		
		
		testOver(singleton->guiX,singleton->guiY);
		doRefresh();
		testOver(singleton->guiX,singleton->guiY);
		doRefresh();
		singleton->compStack[0].data->updateSS();
		
		
		isReady = true;
		isLoaded = true;
		
		
	}
	
	void guiFromJSON(JSONValue* jv) {
		
		int i;
		
		isLoaded = false;
		isReady = false;
		
		guiRenderCount = 0;
		
		
		for (i = 0 ; i < singleton->compStack.size(); i++) {
				if (singleton->compStack[i].data == NULL) {
					
				}
				else {
					delete (singleton->compStack[i].data);
					singleton->compStack[i].data = NULL;
					singleton->compStack[i].isValid = false;
				}
				
		}
		
		singleton->compStack.clear();
		singleton->emptyStack.clear();
		for (i = 0; i < MAX_UI_LAYERS; i++) {
			singleton->guiLayers[i].clear();
			//singleton->emptyLayers[i].clear();
		}
		
		
		singleton->compStack.push_back(Singleton::CompStruct());
		singleton->compStack[0].data = new UIComponent;
		
		for (i = 0; i < E_GST_LENGTH; i++) {
			stringVals[i] = "";
		}
		for (i = 0; i < E_GFT_LENGTH; i++) {
			floatVals[i] = 0.0f;
		}
		
		stringVals[E_GST_LABEL] = "";
		stringVals[E_GST_UID] = "";
		stringVals[E_GST_SS] = "defaultSS";
		
		floatVals[E_GFT_TYPE] = E_GT_HOLDER;
		floatVals[E_GFT_DIVISIONS] = 0.0f;
		floatVals[E_GFT_HASBACKGROUND] = 0.0f;
		floatVals[E_GFT_SINGLELINE] = 1.0f;
		floatVals[E_GFT_FILLRATIOX] = 0.0f;
		floatVals[E_GFT_FILLRATIOY] = 0.0f;
		floatVals[E_GFT_FILLDIR] = E_FILL_HORIZONTAL;
		floatVals[E_GFT_ALIGNX] = E_ALIGNH_LEFT;
		floatVals[E_GFT_ALIGNY] = E_ALIGNV_TOP;
		floatVals[E_GFT_VALUE] = 0.0f;
		floatVals[E_GFT_LAYER] = 0.0f;
		floatVals[E_GFT_HOVERTYPE] = E_HT_NORMAL;
		floatVals[E_GFT_MAXDIMX] = 0.0f;
		floatVals[E_GFT_MAXDIMY] = 0.0f;
		floatVals[E_GFT_MINDIMX] = 0.0f;
		floatVals[E_GFT_MINDIMY] = 0.0f;
		floatVals[E_GFT_FLAGS] = 0.0f;
		
		singleton->compStack[0].data->init(
			singleton,
			-1,
			0,
			0,
			
			NULL,
			false,
			
			stringVals,
			floatVals
		);
		
		// singleton->nullComp->init(
		// 	singleton,
		// 	-1,
		// 	0,
		// 	0,
			
		// 	NULL,
		// 	false,
			
		// 	stringVals,
		// 	floatVals
		// );
		
		
		
		singleton->compStack[0].data->resultDimInPixels.x = singleton->guiWinW;
		singleton->compStack[0].data->resultDimInPixels.y = singleton->guiWinH;
		
		
		
		
		
		
		
		///////
		
		
		
		jvRoot = jv;
		jvTemplates = jv->Child("templates");
		jvSounds = jv->Child("sounds");
		
		
		addChildFromJSON(
			-1,
			jv->Child("baseGUI"),  //jv->Child("curMenu")->string_value
			0,
			false
		);
		
		singleton->compStack[0].data->isDirty = true;
		isReady = true;
		isLoaded = true;
	}
	
	
	
	void doRefresh() {
		
		int i;
		
		singleton->guiDirty = false;
		dirtyVec.clear();
		singleton->compStack[0].data->gatherDirty(&dirtyVec);
		singleton->compStack[0].data->clearDirty();
		
		for (i = 0; i < dirtyVec.size(); i++) {
			dirtyVec[i]->layout();
		}
		
		singleton->compStack[0].data->renderAll();
		
		
	}
	

	void testOver(int x, int y) {
		singleton->maxLayerOver = -1;
		
		int i;
		
		mouseTrans.x = x;
		mouseTrans.y = y;
		mouseTrans.x /= singleton->guiWinW;
		mouseTrans.y /= singleton->guiWinH;
		mouseTrans.x = ((1.0f-mouseTrans.x) - 0.5f)*2.0f;
		mouseTrans.y = ((1.0f-mouseTrans.y) - 0.5f)*2.0f;		
		
		for (i = 0; i < singleton->compStack.size(); i++) {
			singleton->compStack[i].data->overSelf = false;
		}
		//singleton->compStack[0].data->clearOver();
		singleton->compStack[0].data->findMaxLayer(x, y, mouseTrans.x, mouseTrans.y);
		singleton->compStack[0].data->testOver(x, y);
	}

	bool testHit(int button, int state, int x, int y) {
		return singleton->compStack[0].data->testHit(button, state, x, y);
	}
	
	
	UIComponent* findNodeByString(string _uid) {
		return singleton->compStack[0].data->findNodeByString(_uid);
	}

	void renderCharAt(
		UIComponent* uiComp,
		CharStruct* cs,
		FontWrapper* activeFont,
		float px,
		float py,
		float shadowOffset
	) {
		
		StyleSheetResult* resSS = &(uiComp->resSS);
		
		float sampX = cs->sampX;
		float sampY = cs->sampY;
		float sampW = cs->sampW;
		float sampH = cs->sampH;
		float offsetX = (cs->offsetX)*activeFont->fontScale;
		float offsetY = ((activeFont->fontHeight - cs->offsetY) + activeFont->descender)*activeFont->fontScale;
		float sourceW = activeFont->fontImage->width;
		float sourceH = activeFont->fontImage->height;
		
		fBoundingBox destPos;
		fBoundingBox srcPos;
		
		

		destPos.xMin = (px+offsetX)+uiComp->totOffset.x;
		destPos.yMin = (py+offsetY)+uiComp->totOffset.y+shadowOffset;
		destPos.xMax = (px+offsetX+sampW*activeFont->fontScale)+uiComp->totOffset.x;
		destPos.yMax = (py+offsetY+sampH*activeFont->fontScale)+uiComp->totOffset.y+shadowOffset;
		
		srcPos.xMin = (sampX)/sourceW;
		srcPos.yMin = (sourceH-(sampY+sampH))/sourceH;
		srcPos.xMax = (sampX+sampW)/sourceW;
		srcPos.yMax = ( sourceH-sampY )/sourceH;
		
		

		float x0 = destPos.xMin/singleton->guiWinW;
		float x1 = destPos.xMax/singleton->guiWinW;
		float y0 = destPos.yMin/singleton->guiWinH;
		float y1 = destPos.yMax/singleton->guiWinH;

		x0 = (x0-0.5f)*2.0f;
		x1 = (x1-0.5f)*2.0f;
		y0 = ((1.0f-y0) - 0.5f)*2.0f;
		y1 = ((1.0f-y1) - 0.5f)*2.0f;

		float iconVal = 0.0;
		if (activeFont->isIcons) {
			iconVal = 1.0f;
		}
		
		
		
		
		//dimensions
		glMultiTexCoord4f(1, sampW, sampH, 0.0f, 0.0f);

		glMultiTexCoord4f(4, 1.0f, 1.0f, 1.0f, 1.0f);
		glMultiTexCoord4f(5, iconVal, shadowOffset, uiComp->scrollMaskY.x, uiComp->scrollMaskY.y);
		//border color
		glMultiTexCoord4f(6, 1.0f, 1.0f, 1.0f, 1.0f);
		//misc
		glMultiTexCoord4f(7, 0.0f, 0.0f, 0.0f, 0.0f);
		

		glMultiTexCoord4f(2, resSS->props[E_SS_BGCOLTEXT1_R], resSS->props[E_SS_BGCOLTEXT1_G], resSS->props[E_SS_BGCOLTEXT1_B], resSS->props[E_SS_BGCOLTEXT1_A]);
		glMultiTexCoord4f(3, resSS->props[E_SS_FGCOLTEXT1_R], resSS->props[E_SS_FGCOLTEXT1_G], resSS->props[E_SS_FGCOLTEXT1_B], resSS->props[E_SS_FGCOLTEXT1_A]);
		
		
		glMultiTexCoord4f(0, srcPos.xMin, srcPos.yMin, 0.0f, 1.0f);
		glVertex3f (  x0, y1, -1.0f );
		glMultiTexCoord4f(0, srcPos.xMax, srcPos.yMin, 1.0f, 1.0f);
		glVertex3f (  x1, y1, -1.0f );

		glMultiTexCoord4f(2, resSS->props[E_SS_BGCOLTEXT0_R], resSS->props[E_SS_BGCOLTEXT0_G], resSS->props[E_SS_BGCOLTEXT0_B], resSS->props[E_SS_BGCOLTEXT0_A]);
		glMultiTexCoord4f(3, resSS->props[E_SS_FGCOLTEXT0_R], resSS->props[E_SS_FGCOLTEXT0_G], resSS->props[E_SS_FGCOLTEXT0_B], resSS->props[E_SS_FGCOLTEXT0_A]);		

		glMultiTexCoord4f(0, srcPos.xMax, srcPos.yMax, 1.0f, 0.0f);
		glVertex3f (  x1, y0, -1.0f );
		glMultiTexCoord4f(0, srcPos.xMin, srcPos.yMax, 0.0f, 0.0f);
		glVertex3f (  x0, y0, -1.0f );

	}
	
	void renderQuad(
		UIComponent* uiComp,
		fBoundingBox fbb,
		float shadowOffset
	) {

		StyleSheetResult* resSS = &(uiComp->resSS);
		
		float fMatCode = uiComp->matCode;
		bool isHSL = uiComp->matCode == E_MC_HSV;
		
		
		float selMod = 0.0f;
		float selMod2 = 1.0f;
		
		
		
		float x0 = (fbb.xMin+uiComp->totOffset.x)/singleton->guiWinW;
		float x1 = (fbb.xMax+uiComp->totOffset.x)/singleton->guiWinW;
		float y0 = (fbb.yMin+uiComp->totOffset.y+shadowOffset)/singleton->guiWinH;
		float y1 = (fbb.yMax+uiComp->totOffset.y+shadowOffset)/singleton->guiWinH;

		x0 = (x0-0.5f)*2.0f;
		x1 = (x1-0.5f)*2.0f;
		y0 = ((1.0f-y0) - 0.5f)*2.0f;
		y1 = ((1.0f-y1) - 0.5f)*2.0f;

		
		//dimensions
		glMultiTexCoord4f(1, fbb.xMax-fbb.xMin, fbb.yMax-fbb.yMin, resSS->props[E_SS_BORDER], resSS->props[E_SS_CORNERRAD]);

		glMultiTexCoord4f(5, 0.0f, shadowOffset, uiComp->scrollMaskY.x, uiComp->scrollMaskY.y);
		
		
		// todo: fix this to use style sheet
		if (uiComp->selected) {
			selMod = 0.5f;
			selMod2 = 0.5f;
		}
		
		//border color
		glMultiTexCoord4f(
			6,
			resSS->props[E_SS_BDCOL_R]*selMod2 + selMod,
			resSS->props[E_SS_BDCOL_G]*selMod2 + selMod,
			resSS->props[E_SS_BDCOL_B]*selMod2,
			resSS->props[E_SS_BDCOL_A]*selMod2 + selMod
		);
		
		
		
		//misc
		glMultiTexCoord4f(7, uiComp->getValue(), uiComp->getValueY(), resSS->props[E_SS_ROUNDNESS],  fMatCode);

		if ( isHSL ) { // bg with hsv
			
			glMultiTexCoord4f(
				2,
				mixf(uiComp->getValueIndexPtr(0),-1.0f,uiComp->valVecMask[0]),
				mixf(uiComp->getValueIndexPtr(1),-1.0f,uiComp->valVecMask[1]),
				mixf(uiComp->getValueIndexPtr(2),-1.0f,uiComp->valVecMask[2]),
				1.0f
			);
			
			
		}

		if ( !isHSL ) {
			//bg
			glMultiTexCoord4f(2, resSS->props[E_SS_BGCOL1_R], resSS->props[E_SS_BGCOL1_G], resSS->props[E_SS_BGCOL1_B], resSS->props[E_SS_BGCOL1_A]);
		}
		//fg
		glMultiTexCoord4f(3, resSS->props[E_SS_FGCOL1_R], resSS->props[E_SS_FGCOL1_G], resSS->props[E_SS_FGCOL1_B], resSS->props[E_SS_FGCOL1_A]);
		//tg
		glMultiTexCoord4f(4, resSS->props[E_SS_TGCOL1_R], resSS->props[E_SS_TGCOL1_G], resSS->props[E_SS_TGCOL1_B], resSS->props[E_SS_TGCOL1_A]);
		
		
		
		
		glMultiTexCoord4f(0, 0.0f, 0.0f, 0.0f, 1.0f);
		glVertex3f (  x0, y1, -1.0f );
		glMultiTexCoord4f(0, 0.0f, 0.0f, 1.0f, 1.0f);
		glVertex3f (  x1, y1, -1.0f );

		if ( !isHSL ) {
			//bg
			glMultiTexCoord4f(2, resSS->props[E_SS_BGCOL0_R], resSS->props[E_SS_BGCOL0_G], resSS->props[E_SS_BGCOL0_B], resSS->props[E_SS_BGCOL0_A]);
		}
		//fg
		glMultiTexCoord4f(3, resSS->props[E_SS_FGCOL0_R], resSS->props[E_SS_FGCOL0_G], resSS->props[E_SS_FGCOL0_B], resSS->props[E_SS_FGCOL0_A]);
		//tg
		glMultiTexCoord4f(4, resSS->props[E_SS_TGCOL0_R], resSS->props[E_SS_TGCOL0_G], resSS->props[E_SS_TGCOL0_B], resSS->props[E_SS_TGCOL0_A]);

		glMultiTexCoord4f(0, 0.0f, 0.0f, 1.0f, 0.0f);
		glVertex3f (  x1, y0, -1.0f );
		glMultiTexCoord4f(0, 0.0f, 0.0f, 0.0f, 0.0f);
		glVertex3f (  x0, y0, -1.0f );


	}
	
	
	void renderQuadDirect(
		UIComponent* uiComp
	) {
		
		if (uiComp == NULL) {
			return;
		}
		if (uiComp->visible && uiComp->enabled) {
			
		}
		else {
			return;
		}
		
		
		
		float x0 = (uiComp->hitBounds.xMin + uiComp->totOffset.x)/singleton->guiWinW;
		float x1 = (uiComp->hitBounds.xMax + uiComp->totOffset.x)/singleton->guiWinW;
		float y0 = (uiComp->hitBounds.yMin + uiComp->totOffset.y)/singleton->guiWinH;
		float y1 = (uiComp->hitBounds.yMax + uiComp->totOffset.y)/singleton->guiWinH;

		x0 = (x0-0.5f)*2.0f;
		x1 = (x1-0.5f)*2.0f;
		y0 = ((1.0f-y0) - 0.5f)*2.0f;
		y1 = ((1.0f-y1) - 0.5f)*2.0f;

		// glBegin(GL_QUADS);

		// glTexCoord2f(0.0f, 1.0f);
		// glVertex3f (  x0, y1, -1.0f );
		// glTexCoord2f(1.0f, 1.0f);
		// glVertex3f (  x1, y1, -1.0f );
		// glTexCoord2f(1.0f, 0.0f);
		// glVertex3f (  x1, y0, -1.0f );
		// glTexCoord2f(0.0f, 0.0f);
		// glVertex3f (  x0, y0, -1.0f );

		// glEnd();
		
		singleton->setShaderVec4("blitCoords", x0,y0,x1,y1 );
		singleton->fsQuad.draw();

	}
	
	
	void runReport() {
		singleton->compStack[0].data->runReport();
	}

	void renderGUI() {
		
		
		int i;
		
		int j;
		int k;
		int m;
		int n;
		
		int maxLoop = 0;
		float shadowOffset = 0.0;
		Singleton::UICont* curCont = NULL;
		UIComponent* curComp = NULL;
		
		
		
		testOver(singleton->guiX,singleton->guiY);
		doRefresh();
		singleton->compStack[0].data->updateSS();
		
		guiRenderCount++;
		
		if (guiRenderCount < 5) {
			return;
		}
		
		for (i = 0; i < 2; i++) {
			
			
			if (i == 0) {
				maxLoop = 1;
			}
			else {
				maxLoop = 2;
			}
			
			singleton->setShaderFloat("passNum", i);
			singleton->setShaderVec2("resolution", singleton->currentFBOResolutionX, singleton->currentFBOResolutionY);
			
			glBegin (GL_QUADS);
			
				for (j = 0; j < MAX_UI_LAYERS; j++) {
					for (k = 0; k < singleton->guiLayers[j].size(); k++) {
						
						
						
						if (singleton->guiLayers[j][k] >= 0) {
							
							curComp = singleton->compStack[ singleton->guiLayers[j][k] ].data;
							curCont = &(curComp->uiCont);
							
							if (curComp->visible  && curComp->enabled && (curComp->layerId >= 0)) {
								
								for (m = 0; m < maxLoop; m++) {
									
									shadowOffset = ((1-m)*i)*2.0f;
									
									
									// only shadow text
									if (shadowOffset == 0.0f) {
										
										
										if (curCont->bg.fontId > -1) {
											renderQuad(
												curComp,
												curCont->bg.hitBounds,
												shadowOffset
											);
										}
										
										
									}
									
									if (i == 0) {
										// don't render text in first pass
									}
									else {
									
										if (false) { //curCont->locked) {
											// busy updating characters
										}
										else {
											for (n = 0; n < curCont->charVec.size(); n++) {
												renderCharAt(
													curComp,
													curCont->charVec[n].cs,
													singleton->fontWrappers[curCont->charVec[n].fontId],
													curCont->charVec[n].hitBounds.xMin,
													curCont->charVec[n].hitBounds.yMin,
													shadowOffset
												);
											}
										}
									}
								}
							}
							
							
							
							
						}
						
						
						
					}
				}
			
			glEnd ();
		}
		
		
		
		
	}

};

