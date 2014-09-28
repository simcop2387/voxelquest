// f00330_gamegui.h
//

#include "f00330_gamegui.e"
#define LZZ_INLINE inline
GameGUI::GameGUI ()
                  {
		
	}
void GameGUI::init (Singleton * _singleton)
                                         {

		singleton = _singleton;
		isReady = false;
		isLoaded = false;
		
		nodeCount = 0;
		baseComp = new UIComponent();
		baseComp->init(
			singleton,
			-1,
			nodeCount,
			0,
			
			NULL,
			
			"",
			"",
			"defaultSS",

			E_GT_HOLDER,
			0.0f,
			false
		);
		nodeCount++;
		
		
		baseComp->resultDimInPixels.x = singleton->guiWinW;
		baseComp->resultDimInPixels.y = singleton->guiWinH;
		
		
	}
void GameGUI::getJVNodeByString (JSONValue * rootNode, JSONValue * * resultNode, string stringToSplit)
                                                                                                  {
		cout << "getJVNodeByString(" << stringToSplit <<  ")\n";
			
		int i;
		*resultNode = rootNode;
		
		
		vector<string> splitStrings = split(stringToSplit, '.');
		
		for (i = 0; i < splitStrings.size(); i++) {
			cout << splitStrings[i] << "\n";
			
			if ( (*resultNode)->HasChild(splitStrings[i]) ) {
				*resultNode = (*resultNode)->Child(splitStrings[i]);
			}
			else {
				*resultNode = NULL;
				return;
			}
			
		}
		
	}
UIComponent * GameGUI::findNodeById (int _id)
                                           {
		return baseComp->findNodeById(_id);
	}
void GameGUI::addChildFromJSON (JSONValue * jv, UIComponent * curParent, bool isFloating)
          {
		int i;
		int j;
		int curIcon = 0;
		
		JSONValue* curTempl = NULL;
		
		
	
		if (
			jv->HasChild("template")
		) {
			if (
				jvTemplates->HasChild(jv->Child("template")->string_value)
			) {
				
				curTempl = jvTemplates->Child(jv->Child("template")->string_value);
			}
			else {
				cout << "invalid template \n";// << jv->Child("template")->string_value << "\n";
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
		
		
		guiFloatValues[E_GFT_LAYER] = max(guiFloatValues[E_GFT_LAYER],(float)(curParent->layer));
		
		UIComponent* newParent = curParent->addChild(
			curParent->nodeId,
			nodeCount,
			guiStringValues,
			guiFloatValues,
			isFloating,
			jv
		);
		nodeCount++;
		
		
		int numChildren = 0;
		int numFloatingChildren = 0;
		int numDataChildren = 0;
		JSONValue* jvChildren = NULL;
		JSONValue* jvFloatingChildren = NULL;
		JSONValue* jvChildTemplate = NULL;
		
		JSONValue* jvData = NULL;
		JSONValue* jvDataRoot = NULL;
		JSONValue* curData = NULL;
		
		
		if (jv->HasChild("children")) {
			jvChildren = jv->Child("children");
			numChildren = jvChildren->CountChildren();
		}
		for (i = 0; i < numChildren; i++) {
			addChildFromJSON(jvChildren->Child(i),newParent, false);
		}
		
		
		if (jv->HasChild("floatingChildren")) {
			jvFloatingChildren = jv->Child("floatingChildren");
			numFloatingChildren = jvFloatingChildren->CountChildren();
		}
		for (i = 0; i < numFloatingChildren; i++) {
			addChildFromJSON(jvFloatingChildren->Child(i),newParent,true);
		}
		
		
		
		bool isInternal = false;
		E_GUI_CHILD_TYPES curCT = E_GCT_LENGTH;;
		
		
		if (jv->HasChild("childType")) {
			tempStrings[E_GDS_CHILD_TYPE] = jv->Child("childType")->string_value;
			
			
			if (jv->HasChild("isInternal")) {
				if (jv->Child("isInternal")->number_value != 0 ) {
					// use an internally generated JSON node
					isInternal = true;
				}
				else {
					// todo: load JSON file
				}
			}
			
			
			
			
			if (jv->HasChild("dataSource")) {
				tempStrings[E_GDS_DATA_SOURCE] = jv->Child("dataSource")->string_value;
				
				if (isInternal) {
					if (jv->HasChild("dataFile")) {
						tempStrings[E_GDS_DATA_FILE] = jv->Child("dataFile")->string_value;
						jvDataRoot = (singleton->internalJSON[tempStrings[E_GDS_DATA_FILE]]).jv;
					}
					
					
				}
				else {
					jvDataRoot = jvRoot;
				}
				
				if (jvDataRoot != NULL) {
					getJVNodeByString(jvDataRoot, &jvData, tempStrings[E_GDS_DATA_SOURCE]);
					
					numDataChildren = jvData->CountChildren();
					if (jv->HasChild("childTemplate")) {
						jvChildTemplate = jv->Child("childTemplate");
					}
				}
			}
			
			
			if ((jvData != NULL)&&(jvChildTemplate != NULL)) {
				
				
				if (compChildStr("inventoryItem")) {
					curCT = E_GCT_INV_ITEM;
				}
				if (compChildStr("shaderParams")) {
					curCT = E_GCT_SHADER_PARAM;
				}
				
				//////////////////////////////////////////////////////////////////////
				//////////////////////////////////////////////////////////////////////
				
				
				
				
				for (i = 0; i < numDataChildren; i++) {
					
					curData = jvData->Child(i);
					
					switch (curCT) {
						case E_GCT_INV_ITEM:
							tempStrings[E_GDS_CHILD_NAME] = curData->Child("name")->string_value;
							
							curIcon = jvRoot->
								Child("itemDefs")->
								Child(tempStrings[E_GDS_CHILD_NAME])->
								Child("iconNum")->
								number_value;
							
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
								
							jvChildTemplate->Child("label")->string_value = 
								i__s(curIcon) +
								"& " +
								curData->Child("mat")->string_value +
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
						case E_GCT_LENGTH:
							
						break;
					}
					
					
					
					
					
					// copy template to new child
					jv->Child("children")->array_value.push_back(
						JSON::Parse(jvChildTemplate->Stringify().c_str())
					);
					
					
					addChildFromJSON(jv->Child("children")->Child(i),newParent,false); //jvChildTemplate //jv->Child("children")->Child(i)
				}
			}
			
			
			
			
			
			
		}
		
		
	}
void GameGUI::guiFromJSON (JSONValue * jv)
                                        {
		
		isLoaded = false;
		isReady = false;
		
		
		int i;
		
		nodeCount = 1;
		baseComp->children.clear();
		baseComp->floatingChildren.clear();
		
		
		for (i = 0; i < MAX_UI_LAYERS; i++) {
			singleton->guiLayers[i].clear();
		}
		
		jvRoot = jv;
		jvTemplates = jv->Child("templates");
		jvSounds = jv->Child("sounds");
		
		
		addChildFromJSON(
			jv->Child(jv->Child("curMenu")->string_value), //"inventoryMenu" "shaderParamMenu"
			baseComp,
			false
		);
		
		baseComp->isDirty = true;
		isReady = true;
		isLoaded = true;
	}
void GameGUI::doRefresh ()
                         {
		
		int i;
		
		//if (singleton->guiDirty) {
			
			//cout << "guiDirty\n";
			
			singleton->guiDirty = false;
			dirtyVec.clear();
			baseComp->gatherDirty(&dirtyVec);
			baseComp->clearDirty();
			
			for (i = 0; i < dirtyVec.size(); i++) {
				dirtyVec[i]->layout();
			}
			
			//dirtyVec.clear();
			baseComp->renderAll();
		//}
		
		
	}
void GameGUI::testOver (int x, int y)
                                    {
		singleton->maxLayerOver = -1;
		
		baseComp->clearOver();
		baseComp->findMaxLayer(x, y);
		baseComp->testOver(x, y);
	}
bool GameGUI::testHit (int button, int state, int x, int y)
                                                          {
		return baseComp->testHit(button, state, x, y);
	}
UIComponent * GameGUI::findNodeByString (string _uid)
                                                   {
		return baseComp->findNodeByString(_uid);
	}
void GameGUI::renderCharAt (UIComponent * uiComp, CharStruct * cs, FontWrapper * activeFont, float px, float py, float shadowOffset)
          {
		
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

		destPos.xMin = (px+offsetX)+uiComp->floatOffset.x;
		destPos.yMin = (py+offsetY)+uiComp->floatOffset.y+shadowOffset;
		destPos.xMax = (px+offsetX+sampW*activeFont->fontScale)+uiComp->floatOffset.x;
		destPos.yMax = (py+offsetY+sampH*activeFont->fontScale)+uiComp->floatOffset.y+shadowOffset;
		
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
		glMultiTexCoord4f(5, iconVal, shadowOffset, 1.0f, 1.0f);
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
void GameGUI::runReport ()
                         {
		baseComp->runReport();
	}
void GameGUI::renderGUI (float newZoom, int activeFBO)
                                                     {
		
		
		int i;
		
		int j;
		int k;
		int m;
		int n;
		
		int maxLoop = 0;
		
		float shadowOffset = 0.0;
		
		testOver(singleton->guiX,singleton->guiY);
		doRefresh();
		

		Singleton::UICont* curCont = NULL;
		
		
		
		baseComp->updateSS();
		

		singleton->bindFBO("guiFBO");
		singleton->drawFBO("resultFBO", 0, newZoom, activeFBO);
		
		glEnable (GL_BLEND);

		singleton->bindShader("GUIShader");
		singleton->setShaderTexture(0,singleton->fontWrappers[EFW_TEXT]->fontImage->tid);
		singleton->setShaderTexture(1,singleton->fontWrappers[EFW_ICONS]->fontImage->tid);
		singleton->sampleFBO("swapFBOBLin0", 2);
		
		for (i = 0; i < 2; i++) {
			
			
			if (i == 0) {
				maxLoop = 1;
			}
			else {
				maxLoop = 2;
			}
			
			singleton->setShaderFloat("passNum", i);
			singleton->setShaderFloat("zoom", singleton->cameraZoom);
			glBegin (GL_QUADS);
				//baseComp->renderAll(i == 0);
			
				for (j = 0; j < MAX_UI_LAYERS; j++) {
					for (k = 0; k < singleton->guiLayers[j].size(); k++) {
						
						curCont = (singleton->guiLayers[j][k]);
						
						if (curCont->uiComp->visible) {
							
							for (m = 0; m < maxLoop; m++) {
								
								shadowOffset = ((1-m)*i)*4.0f;
								
								
								// only shadow text
								if (shadowOffset == 0.0f) {
									
									
									if (curCont->bg.fontId > -1) {
										renderQuad(
											curCont->uiComp,
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
												curCont->uiComp,
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
			
			glEnd ();
		}
		
		
		singleton->unsampleFBO("swapFBOBLin0", 2);
		singleton->setShaderTexture(1,0);
		singleton->setShaderTexture(0,0);
		singleton->unbindShader();
		
		singleton->unbindFBO();
		
		glDisable(GL_BLEND);
		
		singleton->drawFBO("guiFBO", 0, 1.0f);
		
	}
#undef LZZ_INLINE
 
