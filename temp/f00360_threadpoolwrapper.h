// f00360_threadpoolwrapper.h
//

#include "f00360_threadpoolwrapper.e"
#define LZZ_INLINE inline
ThreadPoolWrapper::ThreadPoolWrapper ()
                            {
		
	}
void ThreadPoolWrapper::init (Singleton * _singleton, int _maxThreads, bool _singleThreaded)
                                                                                {
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
		}
	}
void ThreadPoolWrapper::funcTP (int threadId)
                                  {
		
		GamePageHolder* curHolder;
		
		threadPool[threadId].setRunningLocked(true);
		
		curHolder = singleton->gw->getHolderAtId(
			threadPool[threadId].threadDataInt[1],
			threadPool[threadId].threadDataInt[2]
		);
		
		switch(threadPool[threadId].threadDataInt[0]) {
			case E_TT_GENPATHS:
				curHolder->refreshPaths();
			break;
			case E_TT_GENLIST:
				curHolder->generateList();
			break;
		}
		
		
		threadPool[threadId].setRunningLocked(false);
	}
void ThreadPoolWrapper::startTP (int threadId)
                                   {
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
bool ThreadPoolWrapper::stopTP (int threadId)
                                  {
		
		GamePageHolder* curHolder;
		
		bool didStop = false;
		if (threadPool[threadId].threadRunning) {
			
			if (singleThreaded) {
				
			}
			else {
				threadPool[threadId].threadMain.join();
			}
			
			curHolder = singleton->gw->getHolderAtId(
				threadPool[threadId].threadDataInt[1],
				threadPool[threadId].threadDataInt[2]
			);
			
			switch(threadPool[threadId].threadDataInt[0]) {
				case E_TT_GENPATHS:
					
				break;
				case E_TT_GENLIST:
					curHolder->fillVBO();
					
					//cout << "unlocking pdPool " << curHolder->curPD << "\n";
					
					curHolder->listGenerated = true;
				break;
			}
			
			curHolder->lockWrite = false;
			threadPool[threadId].threadRunning = false;
			didStop = true;
			
			availIds.push_back(threadId);
			
		}
		
		// if (didStop) {
		// 	cout << "didStop\n";
		// }
		
		
		return didStop;
	}
bool ThreadPoolWrapper::startThread ()
                           {
		int i;
		int q;
		
		
		if (availIds.size() == 0) {
			anyRunning();
		}
		
		if (availIds.size() == 0) {
			return false;
		}
		
		int curId = availIds.back();
		availIds.pop_back();
		
		for (i = 0; i < THREAD_DATA_COUNT; i++) {
			threadPool[curId].threadDataInt[i] = intData[i];
		}
		
		GamePageHolder* curHolder = singleton->gw->getHolderAtId(
			threadPool[curId].threadDataInt[1],
			threadPool[curId].threadDataInt[2]
		);
		
		curHolder->lockWrite = true;
		
		startTP(curId);
		
		return true;
	}
bool ThreadPoolWrapper::anyRunning ()
                          {
		
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
void ThreadPoolWrapper::stopAll ()
                       {
		int q;
		for (q = 0; q < maxThreads; q++) {
			stopTP(q);
		}
	}
ThreadPoolWrapper::~ ThreadPoolWrapper ()
                             {
		stopAll();
		delete[] threadPool;
	}
#undef LZZ_INLINE
 
