// f00335_gameai.h
//

#include "f00335_gameai.e"
#define LZZ_INLINE inline
GameAI::GameAI ()
                 {
		kbCompiled = NULL;
		qbCompiled = NULL;
		jvKB = NULL;
	}
void GameAI::init (Singleton * _singleton)
                                         {
		singleton = _singleton;
	}
string GameAI::getResolvedString (VNode * tempNode)
                                                  {
		// if (tokenIndexToVar[tempNode->tokenIndex] == NULL) {
		// 	return intToVToken[tempNode->tokenIndex].stringValue;
		// }
		// else {
		// 	return intToVToken[tokenIndexToVar[tempNode->tokenIndex]->tokenIndex].stringValue;
		// }
		
		return intToVToken[tempNode->tokenIndex].stringValue;
		
	}
bool GameAI::setEqualTo (VNode * destNodeFinal, VNode * srceNodeFinal, int curGen)
                                                                                {
		
		bool notAssigned = false;
		
		cout << SPACE_BUFFER[curGen] << "setEqualTo(): " << getResolvedString(destNodeFinal) << " = " << 
			getResolvedString(srceNodeFinal) << "\n";

		// VNode* destNodeFinal = NULL;
		// VNode* srceNodeFinal = NULL;
		
		// if (tokenIndexToVar[destNode->tokenIndex] == NULL) {
		// 	destNodeFinal = destNode;
		// }
		// else {
		// 	destNodeFinal = tokenIndexToVar[destNode->tokenIndex];
		// }
		
		// if (tokenIndexToVar[srceNode->tokenIndex] == NULL) {
		// 	srceNodeFinal = srceNode;
		// }
		// else {
		// 	srceNodeFinal = tokenIndexToVar[srceNode->tokenIndex];
		// }
		
		if (tokenIndexToVar[destNodeFinal->tokenIndex] == NULL) {
			notAssigned = true;
		}
		// else {
		// 	if (intToVToken[tokenIndexToVar[destNodeFinal->tokenIndex]->tokenIndex].type == E_VCT_VARIABLE) {
		// 		notAssigned = true;
		// 	}
		// }
		
		if (notAssigned) {
			// check if dest node is already set
			
			assignStack.push_back(AssignStruct());
			assignStack.back().lastAssign = tokenIndexToVar[destNodeFinal->tokenIndex];
			assignStack.back().newAssign = srceNodeFinal;
			assignStack.back().tokenIndex = destNodeFinal->tokenIndex;
			assignStack.back().genIndex = curGen;
			
			tokenIndexToVar[destNodeFinal->tokenIndex] = srceNodeFinal;
			
		}
		else {
			
			// cout << SPACE_BUFFER[curGen] << "token indices: " << tokenIndexToVar[destNodeFinal->tokenIndex]->tokenIndex <<
			// ", " << srceNodeFinal->tokenIndex << "\n";
			
			if (tokenIndexToVar[destNodeFinal->tokenIndex]->tokenIndex == srceNodeFinal->tokenIndex) {
				// if set value matches, do nothing
				cout << SPACE_BUFFER[curGen] << "Attempted to set variable to same value\n";
				return true;
			}
			else {
				// attempted to set to different value
				cout << SPACE_BUFFER[curGen] << "Variable is already assigned\n";
				return false;
			}
			
			
		}

		return true;
	}
void GameAI::rollBack (int curGen)
                                  {
		// roll back any assignments
		bool foundGen = true;
		do {
			if (assignStack.size() <= 0) {
				foundGen = false;
			}
			else {
				if (assignStack.back().genIndex > curGen) {
					
					
					// assignStack.back().lastAssign = tokenIndexToVar[destNodeFinal->tokenIndex];
					// assignStack.back().newAssign = srceNodeFinal;
					// assignStack.back().tokenIndex = destNodeFinal->tokenIndex;
					// assignStack.back().genIndex = curGen;
					// tokenIndexToVar[destNodeFinal->tokenIndex] = srceNodeFinal;
					
					cout << "Roll back; " << intToVToken[
						assignStack.back().tokenIndex
					].stringValue << " = ";
					
					if (assignStack.back().lastAssign == NULL) {
						cout << "NULL\n";
					}
					else {
						cout << intToVToken[
							assignStack.back().lastAssign->tokenIndex
						].stringValue << "\n";
					}
					
					
					
					tokenIndexToVar[assignStack.back().tokenIndex] = assignStack.back().lastAssign;
					
					assignStack.pop_back();
				}
				else {
					foundGen = false;
				}
			}
		} while (foundGen);
		
	}
bool GameAI::attemptUnify (VNode * nodeToUnify, VNode * testNode, int curGen)
                                                                           {
		
		int nodeToUnifyType = intToVToken[nodeToUnify->tokenIndex].type;
		int testNodeType = intToVToken[testNode->tokenIndex].type;
		int i;
		
		bool isPredicate1 = nodeToUnifyType == E_VCT_PREDICATE;
		bool isPredicate2 = testNodeType == E_VCT_PREDICATE;
		
		cout << SPACE_BUFFER[curGen] << "attemptUnify(" << getResolvedString(nodeToUnify);
		if (isPredicate1) {
			cout << "[ ";
			for (i = 0; i < nodeToUnify->children.size(); i++) {
				cout << getResolvedString(nodeToUnify->children[i]) << " ";
			}
			cout << "]";
		}
		
		cout << ", " << getResolvedString(testNode);
		
		if (isPredicate2) {
			cout << "[ ";
			for (i = 0; i < testNode->children.size(); i++) {
				cout << getResolvedString(testNode->children[i]) << " ";
			}
			cout << "]";
		}
		
		cout << ")\n";
		
		bool res;
		
		switch (nodeToUnifyType) {
			case E_VCT_FLOAT:
				if (testNodeType == E_VCT_FLOAT) {
					return intToVToken[testNode->tokenIndex].floatValue == intToVToken[nodeToUnify->tokenIndex].floatValue;
				}
				else {
					if (testNodeType == E_VCT_VARIABLE) {
						// set variable to float
						return setEqualTo(testNode, nodeToUnify, curGen);
					}
					else {
						return false;
					}
				}
			break;
			case E_VCT_NOUN:
				switch (testNodeType) {
					case E_VCT_FLOAT:
						return false;
					break;
					case E_VCT_NOUN:
						return testNode->tokenIndex == nodeToUnify->tokenIndex;
					break;
					case E_VCT_OPERATOR:
						return false;
					break;
					case E_VCT_VARIABLE:
						// set variable to noun
						return setEqualTo(testNode, nodeToUnify, curGen);
					break;
					case E_VCT_PREDICATE:
						return false;
					break;
				}
			break;
			case E_VCT_OPERATOR:
				return false;
			break;
			case E_VCT_VARIABLE:
				switch (testNodeType) {
					case E_VCT_FLOAT:
						// set variable to the float value
						setEqualTo(nodeToUnify, testNode, curGen);
					break;
					case E_VCT_NOUN:
						// set the variable to the noun value
						return setEqualTo(nodeToUnify, testNode, curGen);
					break;
					case E_VCT_OPERATOR:
						return false;
					break;
					case E_VCT_VARIABLE:
						// set variables to share values
						return setEqualTo(nodeToUnify, testNode, curGen);
					break;
					case E_VCT_PREDICATE:
						// set the variable to the predicate
						// todo: fix?
						return false;
					break;
				}
			break;
			case E_VCT_PREDICATE:
				switch (testNodeType) {
					case E_VCT_FLOAT:
						return false;
					break;
					case E_VCT_NOUN:
						return false;
					break;
					case E_VCT_OPERATOR:
						return false;
					break;
					case E_VCT_VARIABLE:
						// set variable to the predicate
						// todo: fix this?
						return false;
					break;
					case E_VCT_PREDICATE:
						// unify predicates
						
						if (testNode->tokenIndex == nodeToUnify->tokenIndex) {
							if (testNode->children.size() == nodeToUnify->children.size()) {
								
								for (i = 0; i < nodeToUnify->children.size(); i++) {
									res = attemptUnify(nodeToUnify->children[i], testNode->children[i], curGen);
									
									if (res) {
										
									}
									else {
										return false;
									}
								}
							}
							else {
								return false;
							}
						}
						else {
							return false;
						}
						
					break;
				}
			break;
			
		}
		
		return true;
	}
VNode * GameAI::drillDown (VNode * curVar)
                                        {
		VNode* tempVar = curVar;
		
		if (tempVar == NULL) {
			return NULL;
		}
		else {
			while ( tokenIndexToVar[tempVar->tokenIndex] != NULL ) {
				tempVar = tokenIndexToVar[tempVar->tokenIndex];
			}
			return tempVar;
		}
		
	}
bool GameAI::testEqual (VNode * testNode)
                                        {
		
		VNode* term1;
		VNode* term2;
		bool res = false;
		
		if (testNode->children.size() != 2) {
			cout << "Invalid number of arguments for testEqual()\n";
			return false;
		}
		else {
			term1 = drillDown(testNode->children[0]);
			term2 = drillDown(testNode->children[1]);
			
			if (term1 == NULL) {
				return (term2 == NULL);
			}
			
			res = (term1->tokenIndex == term2->tokenIndex);
			
			cout << "testEqual(): " << res << "\n";
			
			return res;
		}
	}
bool GameAI::isBetween (VNode * testNode)
                                        {
		
		cout << "isBetween()\n";
		
		VNode* term0;
		VNode* term1;
		VNode* term2;
		
		float fv0;
		float fv1;
		float fv2;
		
		if (testNode->children.size() != 3) {
			cout << "Invalid number of arguments for isBetween()\n";
			return false;
		}
		else {
			term0 = drillDown(testNode->children[0]);
			term1 = drillDown(testNode->children[1]);
			term2 = drillDown(testNode->children[2]);
			
			if (
				(term0 == NULL) ||
				(term1 == NULL) ||
				(term2 == NULL)	
			) {
				cout << "null term\n";
				return false;
			}
			else {
				
				if (
					(intToVToken[term0->tokenIndex].type == E_VCT_FLOAT) &&
					(intToVToken[term1->tokenIndex].type == E_VCT_FLOAT) &&
					(intToVToken[term2->tokenIndex].type == E_VCT_FLOAT)
				) {
					
					fv0 = intToVToken[term0->tokenIndex].floatValue;
					fv1 = intToVToken[term1->tokenIndex].floatValue;
					fv2 = intToVToken[term2->tokenIndex].floatValue;
					
					cout << "success, compare: " << fv0 << " " << fv1 << " " << fv2 << "\n";
					
					return ((fv0 >= fv1)&&(fv0<=fv2));
					
				}
				else {
					cout << "non float term\n";
					
					return false;
				}
				
			}
			
			
			
		}
	}
bool GameAI::searchToUnify (VNode * nodeToUnifyBase, int curGen, int curOffset)
                                                                                  {
		
		
		int i;
		int j;
		int k;
		
		bool tempRes = false;
		bool tempRes2 = false;
		bool foundUni;
		bool isHead;
		bool wasSuccessful = false;
		bool foundGen;
		
		VNode* curKBRule = NULL;
		VNode* curKBRuleHead = NULL;
		VNode* curKBRuleTail = NULL;
		VNode* nodeToUnify = nodeToUnifyBase;
		
		// if (goalList.size() == 0) {
		// 	wasSuccessful = true;
		// 	goto DO_RETURN;
		// }
		
		//nodeToUnify = goalList.back();
		//removeGoal();
		//goalList.pop_back();
		
		cout << SPACE_BUFFER[curGen]  << "searchToUnify(" <<
		 intToVToken[nodeToUnify->tokenIndex].stringValue << ") curOffset = " << curOffset << "\n";
		
		for (j = 0; j < qbCompiled->children[0]->children.size(); j++) {
			printChain(qbCompiled->children[0]->children[j]);
			cout << "\n";
		}
		cout << "\n";
		
		
		
		if (intToVToken[nodeToUnify->tokenIndex].type == E_VCT_OPERATOR) {
			switch (nodeToUnify->tokenIndex) {
				case E_VC_ALL:
				case E_VC_NONE:
					nodeToUnify = nodeToUnify->children[curOffset];
				break;
				case E_VC_ANY:
					wasSuccessful = false;
					for (i = 0; i < nodeToUnify->children.size(); i++) {
						wasSuccessful = wasSuccessful || searchToUnify(nodeToUnify->children[i], curGen + 1);
					}
					goto DO_RETURN;
				break;
			}
		}
		
		if (intToVToken[nodeToUnify->tokenIndex].type == E_VCT_OPERATOR) {
			switch (nodeToUnify->tokenIndex) {
				case E_VC_EQUAL:
					wasSuccessful = testEqual(nodeToUnify);
					goto DO_RETURN;
				break;
				case E_VC_NOTEQUAL:
					wasSuccessful = !( testEqual(nodeToUnify) );
					goto DO_RETURN;
				break;
				case E_VC_ISBETWEEN:
					wasSuccessful = isBetween(nodeToUnify);
					goto DO_RETURN;
				break;				
			}
		}
		
		
		
		for (i = 0; i < tokenToRules[nodeToUnify->tokenIndex].data.size(); i++) {
			
			curKBRule = kbCompiled->children[
				tokenToRules[nodeToUnify->tokenIndex].data[i]	
			];
			
			
			isHead = false;
			
			// if dealing with an if then statement, set rule to head
			if (curKBRule->tokenIndex == E_VC_IFTHEN) {
				
				if (curKBRule->children.size() != 2) {
					cout << "Error: If/Then statement requires exactly two arguments.\n";
					wasSuccessful = false;
								goto DO_RETURN;
				}
				else {
					isHead = true;
					curKBRuleHead = curKBRule->children[1]; // then
					curKBRuleTail = curKBRule->children[0]; // if
				}
			}
			else {
				curKBRuleHead = curKBRule;
			}
			
			foundUni = false;
			if (curKBRuleHead->tokenIndex == E_VC_LIST) {
				for (j = 0; j < curKBRule->children[1]->children.size(); j++) {
					curKBRuleHead = curKBRule->children[1]->children[j];
					
					tempRes = attemptUnify(nodeToUnify, curKBRuleHead, curGen + 1);
					
					if (tempRes) {
						cout << SPACE_BUFFER[curGen] << "Unify success\n";
						foundUni = true;
						break;
					}
					else {
						cout << SPACE_BUFFER[curGen] << "Unify failure\n";
						rollBack(curGen);
					}
				}
			}
			else {
				
				tempRes = attemptUnify(nodeToUnify, curKBRuleHead, curGen + 1);
				// if (nodeToUnifyBase->tokenIndex == E_VC_NONE) {
				// 	tempRes = !tempRes;
				// }
				
				
				if (tempRes) {
					
					if (intToVToken[nodeToUnifyBase->tokenIndex].type == E_VCT_OPERATOR) {
						switch (nodeToUnifyBase->tokenIndex) {
							case E_VC_ALL:
							case E_VC_NONE:
							
								if (curOffset < (nodeToUnifyBase->children.size()-1) ) {
									// we still have more items to prove
									
									tempRes2 = searchToUnify(nodeToUnifyBase, curGen + 1, curOffset + 1);
									// if (nodeToUnifyBase->tokenIndex == E_VC_NONE) {
									// 	tempRes2 = !tempRes2;
									// }
									
									if (tempRes2) {
										cout << SPACE_BUFFER[curGen] << "Unify success\n";
										foundUni = true;
									}
									else {
										cout << SPACE_BUFFER[curGen] << "Unify failure\n";
										rollBack(curGen);
									}
									
								}
								else {
									// done proving items
									cout << SPACE_BUFFER[curGen] << "Total unify success\n";
									foundUni = true;
								}
							
								
							break;
							case E_VC_ANY:
								// should not hit this
								cout << SPACE_BUFFER[curGen] << "Error: should not reach this segment\n";
							break;
						}
					}
					else {
						cout << SPACE_BUFFER[curGen] << "Unify success\n";
						foundUni = true;
					}
					
				}
				else {
					cout << SPACE_BUFFER[curGen] << "Unify failure\n";
					rollBack(curGen);
				}
			}
			
			
			if (foundUni) {
				// rule or head of rule successfully unified
				
				
				if (isHead) {
					// attempt to prove all tail conditions true
					
					tempRes = searchToUnify(curKBRuleTail, curGen + 1);
					if (tempRes) {
						wasSuccessful = true;
									goto DO_RETURN;
					}
					else {
						rollBack(curGen);
					}
				}
				else {
					wasSuccessful = true;
								goto DO_RETURN;
				}
			}
			else {
				rollBack(curGen);
			}
			
			
			
			
			
			
		}
		
DO_RETURN:

		if (nodeToUnifyBase->tokenIndex == E_VC_NONE) {
			if (curOffset == 0) {
				wasSuccessful = !wasSuccessful;
			}
		}

		cout << SPACE_BUFFER[curGen] << "wasSuccessful: " << wasSuccessful << "\n";
		return wasSuccessful;
		
	}
void GameAI::printChain (VNode * curVar)
                                       {
		
		if (curVar == NULL) {
			cout << "NULL";
		}
		else {
			cout << intToVToken[curVar->tokenIndex].stringValue;
			
			if ( tokenIndexToVar[curVar->tokenIndex] == NULL ) {
				return;
			}
			else {
				cout << " -> ";
				printChain(tokenIndexToVar[curVar->tokenIndex]);
			}
		}
		
	}
void GameAI::runQueries ()
                          {
		int i;
		int j;
		
		bool res = false;
		
		
		cout << "\n\n";
		
		for (i = 0; i < qbCompiled->children.size(); i++) {
			
			
			//goalList.clear();
			assignStack.clear();
			
			
			res = searchToUnify(qbCompiled->children[i], 0);
			
			for (j = 0; j < qbCompiled->children[i]->children.size(); j++) {
				printChain(qbCompiled->children[i]->children[j]);
				cout << "\n";
			}
			
			cout << "final result: " << res << "\n";
			
			
		}
		
		cout << "\n\n";
	}
bool GameAI::compileVocab (JSONValue * uncompiledNode, VNode * curVNode)
                                                                      {
		int i;
		int curInd;
		int tokIndex;
		bool result = true;
		JSONValue* curJVNode = NULL;
		JSONValue* lastJVNode = NULL;
		
		
		string tempNumberStr;
		
		if (uncompiledNode->IsArray()) {
			for (i = 0; i < uncompiledNode->array_value.size(); i++) {
				curJVNode = uncompiledNode->array_value[i];
				
				//compiledNode.array_value.push_back( new JSONValue(JSONArray()) );
				
				if (curJVNode->IsArray()) {
					if (lastJVNode == NULL) {
						cout << "Array must be preceded by string value\n";
						return false;
					}
					else {
						if (lastJVNode->IsString()) {
							
							tokIndex = stringToVTokenIndex[lastJVNode->string_value];
							
							if (
								(intToVToken[tokIndex].type == E_VCT_OPERATOR) ||
								(intToVToken[tokIndex].type == E_VCT_PREDICATE)
							) {
								curVNode->children.push_back(new VNode(tokIndex));
								result = result && compileVocab(curJVNode, curVNode->children.back());
							}
							else {
								cout << "Array must be preceded by operator or predicate\n";
								return false;
							}
							
							
						}
						else {
							cout << "Array must be preceded by string value\n";
							return false;
						}
					}
				}
				else if (curJVNode->IsString()) {
					
					
					tokIndex = stringToVTokenIndex[curJVNode->string_value];
					
					if (
						(intToVToken[tokIndex].type == E_VCT_OPERATOR) ||
						(intToVToken[tokIndex].type == E_VCT_PREDICATE)
					) {
						// wait to consume array
					}
					else {
						curVNode->children.push_back(new VNode(tokIndex));
					}
					
					
				}
				else if (curJVNode->IsNumber()) {
					
					tempNumberStr = f__s(curJVNode->number_value);
					if (stringToVTokenIndex.find( tempNumberStr ) == stringToVTokenIndex.end()) {
						intToVToken.push_back(VToken());
						curInd = intToVToken.size()-1;
						intToVToken[curInd].index = curInd;
						intToVToken[curInd].stringValue = tempNumberStr;
						intToVToken[curInd].floatValue = curJVNode->number_value;
						intToVToken[curInd].type = E_VCT_FLOAT;
						intToVToken[curInd].maxParam = 0;
						stringToVTokenIndex[tempNumberStr] = curInd;
					}
					
					tokIndex = stringToVTokenIndex[tempNumberStr];
					curVNode->children.push_back(new VNode(tokIndex));
					
				}
				
				
				lastJVNode = uncompiledNode->array_value[i];
				
			}
		}
		
		return result;
	}
bool GameAI::checkVocab (JSONValue * jv)
                                       {
		
		int i;
		
		bool result = true;
		
		if (jv->IsArray()) {
			for (i = 0; i < jv->array_value.size(); i++) {
				result = result && checkVocab(jv->array_value[i]);
			}
		}
		else {
			if (jv->IsString()) {
				if (stringToVTokenIndex.find( jv->string_value ) == stringToVTokenIndex.end()) {
						result = false;
						cout << "Undeclared token " << jv->string_value << "\n";
				}
			}
		}
		
		return result;
	}
void GameAI::buildRule (VNode * curNode, int ruleNumber, bool isIfThen)
                                                                      {
		int i;
		VToken* vt =  &(intToVToken[curNode->tokenIndex]);
		
		std::vector<int>::iterator it;
		
		if (curNode->tokenIndex == E_VC_IFTHEN) {
			buildRule(curNode->children[1], ruleNumber, true);
		}
		else {
			
			if (
				(vt->type == E_VCT_NOUN) ||
				(vt->type == E_VCT_PREDICATE)
			) {
				
				if (isIfThen) { // push if-then rules to the back
					tokenToRules[curNode->tokenIndex].data.push_back(ruleNumber);
				}
				else { // all other rules go to the front
					it = tokenToRules[curNode->tokenIndex].data.begin();
					it = tokenToRules[curNode->tokenIndex].data.insert ( it , ruleNumber );
				}
				
				
			}
			
			for (i = 0; i < curNode->children.size(); i++) {
				buildRule(curNode->children[i], ruleNumber, isIfThen);
			}
		}
	}
void GameAI::buildRuleSet (VNode * curNode)
                                          {
		
		int i;
		
		for (i = 0; i < curNode->children.size(); i++) {
			buildRule(curNode->children[i], i, false);
		}
		
	}
void GameAI::numberRule (VNode * curNode, int ruleNumber)
                                                        {
		int i;
		int curInd;
		
		
		curNode->ruleNumber = ruleNumber;
		
		if (intToVToken[curNode->tokenIndex].type == E_VCT_VARIABLE) {
			// check if variable already registered
			
			tempVarStr = intToVToken[curNode->tokenIndex].stringValue;
			tempVarStr.append("__");
			tempVarStr.append(i__s(ruleNumber));
			
			if (stringToVTokenIndex.find( tempVarStr ) == stringToVTokenIndex.end()) {
				// variable not yet created, make it
				
				intToVToken.push_back(VToken());
				curInd = intToVToken.size()-1;
				intToVToken[curInd].index = curInd;
				intToVToken[curInd].stringValue = tempVarStr;
				intToVToken[curInd].floatValue = 0.0f;
				intToVToken[curInd].type = E_VCT_VARIABLE;
				intToVToken[curInd].maxParam = 0;
				stringToVTokenIndex[tempVarStr] = curInd;
				
				
			}
			
			curNode->tokenIndex = stringToVTokenIndex[tempVarStr];
			
		}
		
		for (i = 0; i < curNode->children.size(); i++) {
			numberRule(curNode->children[i], ruleNumber);
		}		
	}
int GameAI::numberRuleSet (VNode * curNode, int offset)
                                                      {
		
		int i;
		
		for (i = 0; i < curNode->children.size(); i++) {
			numberRule(curNode->children[i], i + offset);
		}
		
		return i;
	}
void GameAI::printRule (VNode * curNode, int offset)
                                                   {
		int i;
		int curInd;
		
		
		curNode->tokenIndex;
		
		
		
		if (curNode->children.size() > 0) {
			cout << SPACE_BUFFER[offset] << intToVToken[curNode->tokenIndex].stringValue << "[\n";
		}
		else {
			cout << SPACE_BUFFER[offset] << intToVToken[curNode->tokenIndex].stringValue << "\n";
		}
		
		for (i = 0; i < curNode->children.size(); i++) {
			printRule(curNode->children[i], offset + 1);
		}	
		
		if (curNode->children.size() > 0) {
			cout << SPACE_BUFFER[offset] << "]\n";
		}	
	}
void GameAI::printRuleSet (VNode * curNode)
                                          {
		
		int i;
		
		for (i = 0; i < curNode->children.size(); i++) {
			printRule(curNode->children[i],0);
			
			cout << "\n\n";
			
		}
		
	}
bool GameAI::parseKB ()
                       {
		int i;
		int j;
		int tokIndex;
		int curType;
		int arraySize;
		int tempInt0;
		int curInd;
		int tempInt = 0;
		string tempStr0;
		JSONValue* curJV;
		
		bool res0;
		bool res1;
		
		
		
		
		
		for (i = 0; i < jvKB->Child("vocabulary")->array_value.size(); i++) {
			
			curJV = jvKB->Child("vocabulary")->array_value[i];
			
			if ( curJV->IsArray() ) {
				
				arraySize = curJV->array_value.size();
				
				if (arraySize == 2) {
					
					tempStr0 = curJV->array_value[0]->string_value;
					tempInt0 = curJV->array_value[1]->number_value;
					
					switch(tempStr0[0]) {
						case 'f':
							curType = E_VCT_FLOAT;
						break;
						case 'o':
							curType = E_VCT_OPERATOR;
						break;
						case 'v':
							curType = E_VCT_VARIABLE;
						break;
						case 'n':
							curType = E_VCT_NOUN;
						break;
						case 'p':
							curType = E_VCT_PREDICATE;
						break;
						default:
							cout << "Unsupported type in token " << tempStr0 << "\n";
							return false;
						break;
					}
					
					intToVToken.push_back(VToken());
					curInd = intToVToken.size()-1;
					intToVToken[curInd].index = curInd;
					intToVToken[curInd].stringValue = tempStr0;
					intToVToken[curInd].floatValue = 0.0f;
					intToVToken[curInd].type = curType;
					intToVToken[curInd].maxParam = tempInt0;
					stringToVTokenIndex[tempStr0] = curInd;
					
				}
				else {
					cout << "Invalid Array Size of " << arraySize << " in vocabulary.\n";
					return false;
				}
				
				
				
			}
			else {
				cout << "Vocabulary entry " << i << " is not an array.\n";
				return false;
			}
		}
		
		res0 = checkVocab(jvKB->Child("kb"));
		res1 = checkVocab(jvKB->Child("qb"));
		
		if (res0&&res1) {
			
		}
		else {
			cout << "checkVocab() failed\n";
			return false;
		}
		
		
		if (kbCompiled != NULL) {
			delete kbCompiled;
		}
		if (qbCompiled != NULL) {
			delete qbCompiled;
		}
		
		
		
		tokIndex = stringToVTokenIndex["oList"];
		
		kbCompiled = new VNode(tokIndex);
		qbCompiled = new VNode(tokIndex);
		
		cout << "Begin vocab compile\n";
		
		res0 = compileVocab(jvKB->Child("kb"),kbCompiled);
		res1 = compileVocab(jvKB->Child("qb"),qbCompiled);
		
		if (res0&&res1) {
			
		}
		else {
			cout << "compileVocab() failed\n";
			return false;
		}
		
		
		for (i = 0; i < intToVToken.size();i++) {
			tokenToRules.push_back(intVec());
		}
		buildRuleSet(kbCompiled);
		tempInt = numberRuleSet(kbCompiled, 0);
		numberRuleSet(qbCompiled,tempInt);
		
		
		
		for (i = 0; i < intToVToken.size();i++) {
			tokenIndexToVar.push_back(NULL);
		}
		
		
		
		traceRules();
		
		
		return true;
	}
void GameAI::traceRules ()
                          {
		int i;
		int j;
		
		cout << "TokenToRules:\n";
		for (i = 0; i < tokenToRules.size(); i++) {
			
			cout << intToVToken[i].stringValue << ": ";
			
			for (j = 0; j < tokenToRules[i].data.size(); j++) {
				cout << tokenToRules[i].data[j] << " ";
			}
			
			cout << "\n";
		}
		
		cout << "\nAll Vocab:\n";
		for (i = 0; i < intToVToken.size(); i++) {
			cout << intToVToken[i].stringValue << "\n";
		}
		
		
		cout << "kb\n";
		printRuleSet(kbCompiled);
		cout << "qb\n";
		printRuleSet(qbCompiled);
		cout << "\n\n";
		
	}
void GameAI::getKB ()
                     {
		
		bool res;
		
		cout << "getKB()\n";
		tokenToRules.clear();
		stringToVTokenIndex.clear();
		tokenIndexToVar.clear();
		intToVToken.clear();
		
		jvKB = singleton->fetchJSONData("kb.js", true);
		
		if (jvKB == NULL) {
			return;
		}
		
		res = parseKB();
		
		if (res) {
			cout << "Successfully compiled KB\n";
			
			runQueries();
			
		}
		else {
			cout << "Failed to compile KB\n";
		}
		
	}
#undef LZZ_INLINE
 
