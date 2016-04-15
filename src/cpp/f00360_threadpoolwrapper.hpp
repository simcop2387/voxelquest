class ThreadPoolWrapper {
	public:
	
	int maxThreads;
	int intData[THREAD_DATA_COUNT];
	string stringData[THREAD_DATA_COUNT];
	
	ThreadWrapper* threadPool;
	Singleton* singleton;
	
	bool singleThreaded;
	
	std::vector<int> availIds;
	
	ThreadPoolWrapper() {
		
	}
	
	void init(Singleton* _singleton, int _maxThreads, bool _singleThreaded) {
		int i;
		
		singleton = _singleton;
		
		singleThreaded = _singleThreaded;
		maxThreads = _maxThreads;
		threadPool = new ThreadWrapper[maxThreads];
		
		
		
		for (i = 0; i < maxThreads; i++) {
			threadPool[i].init();
			availIds.push_back(i);
		}
		
		for (i = 0; i < THREAD_DATA_COUNT; i++) {
			intData[i] = 0;
			stringData[i] = "";
		}
	}
	
	void doSpeak(string speechString) {
		
		HRESULT hrVoice;
		ISpVoice * pVoice;

		wstring speechStringW = s2ws(speechString);

		if (FAILED(::CoInitialize(NULL))) {
			return;
		}

		hrVoice = CoCreateInstance(CLSID_SpVoice, NULL, CLSCTX_ALL, IID_ISpVoice, (void **)&pVoice);
		if( SUCCEEDED( hrVoice ) )
		{
		    
		    hrVoice = pVoice->Speak(speechStringW.c_str(), 0, NULL);

		    // Change pitch
		    //hrVoice = pVoice->Speak(L"This sounds normal <pitch middle = '-10'/> but the pitch drops half way through", SPF_IS_XML, NULL );
		    
		    pVoice->Release();
		    pVoice = NULL;
		    
		}
		::CoUninitialize();
	}
	
	void funcTP(int threadId) {
		
		GamePageHolder* curHolder = NULL;
		
		threadPool[threadId].setRunningLocked(true);
		
		switch(threadPool[threadId].threadDataInt[0]) {
			case E_TT_GENPATHS:
			case E_TT_GENLIST:
				curHolder = singleton->gw->getHolderAtId(
					threadPool[threadId].threadDataInt[1],
					threadPool[threadId].threadDataInt[2],
					threadPool[threadId].threadDataInt[3]
				);
			break;	
		}
		
		switch(threadPool[threadId].threadDataInt[0]) {
			case E_TT_GENPATHS:
				curHolder->refreshPaths();
			break;
			case E_TT_GENLIST:
				curHolder->generateList();
			break;
			case E_TT_SPEECH:
				doSpeak(threadPool[threadId].threadDataString[0]);
			break;
		}
		
		
		threadPool[threadId].setRunningLocked(false);
	}
	void startTP(int threadId) {
		if (threadPool[threadId].threadRunning) {
			
		}
		else {
			threadPool[threadId].threadRunning = true;
			
			if (singleThreaded) {
				funcTP(threadId);
			}
			else {
				threadPool[threadId].threadMain = std::thread(&ThreadPoolWrapper::funcTP, this, threadId);
			}
			
		}
		
	}
	bool stopTP(int threadId) {
		
		GamePageHolder* curHolder = NULL;
		GameChunk* curChunk;
		
		bool didStop = false;
		if (threadPool[threadId].threadRunning) {
			
			if (singleThreaded) {
				
			}
			else {
				threadPool[threadId].threadMain.join();
			}
			
			
			
			switch(threadPool[threadId].threadDataInt[0]) {
				case E_TT_GENPATHS:
				case E_TT_GENLIST:
					curHolder = singleton->gw->getHolderAtId(
						threadPool[threadId].threadDataInt[1],
						threadPool[threadId].threadDataInt[2],
						threadPool[threadId].threadDataInt[3]
					);
				break;	
			}
			
			
			switch(threadPool[threadId].threadDataInt[0]) {
				case E_TT_GENPATHS:
					
				break;
				case E_TT_GENLIST:
					curHolder->fillVBO();
					
					//cout << "unlocking pdPool " << curHolder->curPD << "\n";
					
					curHolder->listGenerated = true;
					curChunk = singleton->gw->getChunkAtId(curHolder->blockId,curHolder->chunkId);
					
					if (curHolder->listEmpty) {
						
					}
					else {
						curChunk->changeFlag = true;
					}
					
					
				break;
				case E_TT_SPEECH:
					
				break;
			}
			
			switch(threadPool[threadId].threadDataInt[0]) {
				case E_TT_GENPATHS:
				case E_TT_GENLIST:
					curHolder->lockWrite = false;
				break;	
			}
			
			
			threadPool[threadId].threadRunning = false;
			didStop = true;
			
			availIds.push_back(threadId);
			
		}
		
		// if (didStop) {
		// 	cout << "didStop\n";
		// }
		
		
		return didStop;
	}
	
	// must set intData first
	// must set stringData as well
	
	bool anyThreadAvail() {
		if (availIds.size() == 0) {
			anyRunning();
		}
		
		if (availIds.size() == 0) {
			return false;
		}
		
		return true;
	}
	
	bool startThread(bool checkAvail = true) {
		int i;
		int q;
		
		if (checkAvail) {
			if (anyThreadAvail()) {
				
			}
			else {
				return false;
			}
		}
		
		
		int curId = availIds.back();
		availIds.pop_back();
		
		for (i = 0; i < THREAD_DATA_COUNT; i++) {
			threadPool[curId].threadDataInt[i] = intData[i];
			threadPool[curId].threadDataString[i] = stringData[i];
		}
		
		GamePageHolder* curHolder = NULL;
		
		
		switch(threadPool[curId].threadDataInt[0]) {
			case E_TT_GENPATHS:
			case E_TT_GENLIST:
				curHolder = singleton->gw->getHolderAtId(
					threadPool[curId].threadDataInt[1],
					threadPool[curId].threadDataInt[2],
					threadPool[curId].threadDataInt[3]
				);
				curHolder->lockWrite = true;
			break;	
		}
		
		
		startTP(curId);
		
		return true;
	}
	
	bool anyRunning() {
		
		int q;
		
		bool bAnyRunning = false;
		for (q = 0; q < maxThreads; q++) {
			if (threadPool[q].threadRunning) {
				if (threadPool[q].isReady()) {
					stopTP(q);
				}
				else {
					bAnyRunning = true;
				}
			}
		}
		
		return bAnyRunning;
	}
	
	void stopAll() {
		int q;
		for (q = 0; q < maxThreads; q++) {
			stopTP(q);
		}
	}
	
	~ThreadPoolWrapper() {
		stopAll();
		delete[] threadPool;
	}
};
