
class GameSound {
	public:
		
		
		sf::SoundBuffer buffer;
		sf::Sound sound;
		
		float curVolume;
		
		GameSound() {
			curVolume = 0.0;
		}
		
		void init(string path) {
			if (!buffer.loadFromFile(path)) {
				cout << "error loading sound";
				return;
			}
			
			sound.setBuffer(buffer);
			
		}
		
		void setPitch(float val) {
			sound.setPitch(val);
		}
		
		void setPositionAndMinDis(
			float x,
			float y,
			float z,
			float w	
		) {
			sound.setMinDistance(w);
			sound.setPosition(x,y,z);
		}
		
		void setLoop(bool val) {
			sound.setLoop(val);
		}

		void stop() {
			
		}
		
		void setVolumeSmooth(float volume=1.0f, float decay = 0.01f)
		{
			
			curVolume += (volume-curVolume)*decay;
			
			
			int intVol = curVolume*100.0f;
			sound.setVolume(intVol);
					
		}
		
		void setVolume(float volume=1.0f)
		{
			curVolume = volume;
			int intVol = volume*100.0f;
			sound.setVolume(intVol);			
		}
		
		void play(float volume=1.0f)
		{
			curVolume = volume;
			int intVol = volume*100.0f;
			
			sound.setVolume(intVol);
			sound.play();
			
		}
};
