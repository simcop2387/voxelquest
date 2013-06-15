int traceLevel = 0;
int popCount = 0;

vector<string> &split(const string &s, char delim, vector<string> &elems) {
	stringstream ss(s);
	string item;
	while(getline(ss, item, delim)) {
		elems.push_back(item);
	}
	return elems;
}


vector<string> split(const string &s, char delim) {
	vector<string> elems;
	return split(s, delim, elems);
}


inline float clamp(float val) {
	float retval = val;
	if (retval < 0.0f) retval = 0.0f;
	if (retval > 1.0f) retval = 1.0f;
	return retval;
}

inline float fGenRand() {
	return ((float)(rand()%100000))/100000.0f;
}

inline int iGenRand(int val) {
	return rand()%(val+1) - val/2;
}

string intToString(int i) {
	string s;
	stringstream out;
	out << i;
	s = out.str();
	return s;
}
string floatToString(float f) {
	string s;
	stringstream out;
	out << f;
	s = out.str();
	return s;
}

void doTrace(string traceVal0 = "", string traceVal1 = "",string traceVal2 = "",string traceVal3 = "",string traceVal4 = "",string traceVal5 = "", string traceVal6 = "",string traceVal7 = "",string traceVal8 = "",string traceVal9 = "",string traceVal10 = "") {
	int i;
	
	for (i = 0; i < traceLevel; i++) {
		cout << "|  ";
	}
	
	cout << traceVal0 << " " << traceVal1 << " " << traceVal2 << " " << traceVal3 << " " << traceVal4 << " " << traceVal5 << " " << traceVal6 << " " << traceVal7 << " " << traceVal8 << " " << traceVal9 << " " << traceVal10 << "\n";
	
}

void pushTrace(string traceVal0 = "", string traceVal1 = "",string traceVal2 = "",string traceVal3 = "",string traceVal4 = "",string traceVal5 = "", string traceVal6 = "",string traceVal7 = "",string traceVal8 = "",string traceVal9 = "",string traceVal10 = "") {
	doTrace(traceVal0,traceVal1,traceVal2,traceVal3,traceVal4,traceVal5,traceVal6,traceVal7,traceVal8,traceVal9,traceVal10);
	traceLevel++;
	popCount=0;
}
void popTrace() {
	traceLevel--;
	popCount++;
	if (popCount >= 2) {
		doTrace("END");
	}
}

float distance(Vector3* v1, Vector3* v2) {
	float val1 = v1->e[0] - v2->e[0];
	float val2 = v1->e[1] - v2->e[1];
	float val3 = v1->e[2] - v2->e[2];
	
	return sqrt(
		val1*val1 + val2*val2 + val3*val3
	);
	
	//return length(v1-v2);
}