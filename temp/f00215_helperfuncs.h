int traceLevel = 0; int popCount = 0;

std::string i__s(int i) {
	return std::to_string(i);
}
std::string f__s(float f) {
	return std::to_string(f);
}

std::vector<std::string> &split(const std::string &s, char delim, std::vector<std::string> &elems) {
	std::stringstream ss(s);
	std::string item;
	while(getline(ss, item, delim)) {
		elems.push_back(item);
	}
	return elems;
}


std::vector<std::string> split(const std::string &s, char delim) {
	std::vector<std::string> elems;
	return split(s, delim, elems);
}

struct intPair {
	int v0;
	int v1;
};

struct floatAndIndex {
	float value;
	int index1;
	int index2;
};

struct coordAndIndex {
	float x;
	float y;
	float xTen;
	float yTen;
	int index;
};

float quickDis(float x1, float y1, float x2, float y2) {
	//return abs(x1-x2) + abs(y1-y2);//
	return sqrt( (x1-x2)*(x1-x2) + (y1-y2)*(y1-y2) );
}

float coordDis(coordAndIndex* c1, coordAndIndex* c2) {
	//return abs(x1-x2) + abs(y1-y2);//

	float d1 = c1->x-c2->x;
	float d2 = c1->y-c2->y;
	return sqrt( d1*d1 + d2*d2 );
}


void bubbleSortF(floatAndIndex* num, int numLength)
{
	int i, j, flag = 1;

	floatAndIndex fiTemp;

	for(i = 1; (i <= numLength) && flag; i++) {
		flag = 0;
		for (j=0; j < (numLength -1); j++) {
			if (num[j+1].value < num[j].value) { // ascending order simply changes to <
				fiTemp = num[j];
				num[j] = num[j+1];
				num[j+1] = fiTemp;
				flag = 1;
			}
		}
	}
}





std::string intToString(int i) {
	std::string s;
	std::stringstream out;
	out << i;
	s = out.str();
	return s;
}
std::string floatToString(float f) {
	std::string s;
	std::stringstream out;
	out << f;
	s = out.str();
	return s;
}






//////////


void doTraceND(std::string traceVal0 = "", std::string traceVal1 = "",std::string traceVal2 = "",std::string traceVal3 = "",std::string traceVal4 = "",std::string traceVal5 = "", std::string traceVal6 = "",std::string traceVal7 = "",std::string traceVal8 = "",std::string traceVal9 = "",std::string traceVal10 = "") {
	
	int i;
	
	for (i = 0; i < traceLevel; i++) {
		std::cout << "|  ";
	}
	
	std::cout << traceVal0 << " " << traceVal1 << " " << traceVal2 << " " << traceVal3 << " " << traceVal4 << " " << traceVal5 << " " << traceVal6 << " " << traceVal7 << " " << traceVal8 << " " << traceVal9 << " " << traceVal10 << "\n" << std::flush;
	
}
void doTraceVecND(std::string traceVal0, FIVector4 *fv) {
	
	doTraceND(traceVal0, " ", f__s(fv->getFX()), " ", f__s(fv->getFY()), " ", f__s(fv->getFZ())  );
	
}

void pushTraceND(std::string traceVal0 = "", std::string traceVal1 = "",std::string traceVal2 = "",std::string traceVal3 = "",std::string traceVal4 = "",std::string traceVal5 = "", std::string traceVal6 = "",std::string traceVal7 = "",std::string traceVal8 = "",std::string traceVal9 = "",std::string traceVal10 = "") {
	
	doTraceND(traceVal0,traceVal1,traceVal2,traceVal3,traceVal4,traceVal5,traceVal6,traceVal7,traceVal8,traceVal9,traceVal10);
	traceLevel++;
	popCount=0;
	

	
}
void popTraceND() {
	
	traceLevel--;
	popCount++;
	if (popCount >= 2) {
		doTraceND("END");
	}
	
}

//////////




// void doTrace(std::string traceVal0 = "", std::string traceVal1 = "",std::string traceVal2 = "",std::string traceVal3 = "",std::string traceVal4 = "",std::string traceVal5 = "", std::string traceVal6 = "",std::string traceVal7 = "",std::string traceVal8 = "",std::string traceVal9 = "",std::string traceVal10 = "") {
// 	#ifdef DEBUG_MODE
// 	int i;
	
// 	for (i = 0; i < traceLevel; i++) {
// 		std::cout << "|  ";
// 	}
	
// 	std::cout << traceVal0 << " " << traceVal1 << " " << traceVal2 << " " << traceVal3 << " " << traceVal4 << " " << traceVal5 << " " << traceVal6 << " " << traceVal7 << " " << traceVal8 << " " << traceVal9 << " " << traceVal10 << "\n" << std::flush;
// 	#endif
// }
// void doTraceVec(std::string traceVal0, FIVector4 *fv) {
// 	#ifdef DEBUG_MODE
// 	doTrace(traceVal0, " ", f__s(fv->getFX()), " ", f__s(fv->getFY()), " ", f__s(fv->getFZ())  );
// 	#endif
// }

// void pushTrace(std::string traceVal0 = "", std::string traceVal1 = "",std::string traceVal2 = "",std::string traceVal3 = "",std::string traceVal4 = "",std::string traceVal5 = "", std::string traceVal6 = "",std::string traceVal7 = "",std::string traceVal8 = "",std::string traceVal9 = "",std::string traceVal10 = "") {
// 	#ifdef DEBUG_MODE
// 	doTrace(traceVal0,traceVal1,traceVal2,traceVal3,traceVal4,traceVal5,traceVal6,traceVal7,traceVal8,traceVal9,traceVal10);
// 	traceLevel++;
// 	popCount=0;
// 	#endif

	
// }
// void popTrace() {
// 	#ifdef DEBUG_MODE
// 	traceLevel--;
// 	popCount++;
// 	if (popCount >= 2) {
// 		doTrace("END");
// 	}
// 	#endif
// }



void doTrace(std::string traceVal0 = "", std::string traceVal1 = "",std::string traceVal2 = "",std::string traceVal3 = "",std::string traceVal4 = "",std::string traceVal5 = "", std::string traceVal6 = "",std::string traceVal7 = "",std::string traceVal8 = "",std::string traceVal9 = "",std::string traceVal10 = "") {
	int i;

	if (TRACE_ON) {
		for (i = 0; i < traceLevel; i++) {
			std::cout << "|  ";
		}
		
		std::cout << traceVal0 << " " << traceVal1 << " " << traceVal2 << " " << traceVal3 << " " << traceVal4 << " " << traceVal5 << " " << traceVal6 << " " << traceVal7 << " " << traceVal8 << " " << traceVal9 << " " << traceVal10 << "\n" << std::flush;
	}
	
	
}
void doTraceVec(std::string traceVal0, FIVector4 *fv) {
	if (TRACE_ON) {
		doTrace(traceVal0, " ", f__s(fv->getFX()), " ", f__s(fv->getFY()), " ", f__s(fv->getFZ())  );
	}
	
	
}

void pushTrace(std::string traceVal0 = "", std::string traceVal1 = "",std::string traceVal2 = "",std::string traceVal3 = "",std::string traceVal4 = "",std::string traceVal5 = "", std::string traceVal6 = "",std::string traceVal7 = "",std::string traceVal8 = "",std::string traceVal9 = "",std::string traceVal10 = "") {
	
	if (TRACE_ON) {
		doTrace(traceVal0,traceVal1,traceVal2,traceVal3,traceVal4,traceVal5,traceVal6,traceVal7,traceVal8,traceVal9,traceVal10);
			traceLevel++;
			popCount=0;
	}
	
}
void popTrace() {
	
	if (TRACE_ON) {
		traceLevel--;
		popCount++;
		if (popCount >= 2) {
			doTrace("END");
		}
	}

	
	
}



 
