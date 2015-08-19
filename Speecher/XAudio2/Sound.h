#pragma once

class AudioEmitter;
class Sound
{
private:
	struct Voice
	{
		IXAudio2SourceVoice* sourceVoice;
		volatile bool isPlaying;

		Voice(IXAudio2SourceVoice* sourceVoice, bool isPlaying) : 
			sourceVoice(sourceVoice), isPlaying(isPlaying) {}

		Voice(Voice&& other) : 
			sourceVoice(other.sourceVoice),
			isPlaying(other.isPlaying)
		{
			other.sourceVoice = nullptr;
			other.isPlaying = false;
		}

	private:
		Voice(const Voice& other);
	};
		
	class SoundCallbacks : public IXAudio2VoiceCallback
	{
	private:
		Sound* m_Sound;

		virtual void __stdcall OnVoiceProcessingPassStart(UINT32) { }
		virtual void __stdcall OnVoiceProcessingPassEnd() { }
		virtual void __stdcall OnStreamEnd() { }

		virtual void __stdcall OnBufferStart(void*) { }
		virtual void __stdcall OnBufferEnd(void* pBufferContext)
		{
			auto index = reinterpret_cast<size_t>(pBufferContext);
			m_Sound->m_Voices[index].isPlaying = false; 
		}

		virtual void __stdcall OnLoopEnd(void*) { }
		virtual void __stdcall OnVoiceError(void*, HRESULT) { __debugbreak(); }
		
	public:
		SoundCallbacks(Sound* sound) : m_Sound(sound) { }
		virtual ~SoundCallbacks() { }
	};

	std::unique_ptr<uint8_t[]> m_SoundDataBuffer;
	XAUDIO2_BUFFER m_AudioBuffer;
	WAVEFORMATEXTENSIBLE m_WaveFormat;

	std::vector<Voice> m_Voices;
	IXAudio2SubmixVoice* m_SubmixVoice;
	SoundCallbacks m_SoundCallbacks;

	size_t GetNotPlayingVoiceIndex();
	size_t CreateVoice();
	Voice& GetVoiceForPlayback();
	void PlayImpl(Voice& voiceToPlay);

	friend class SoundCallbacks;
	Sound(const Sound& other);

public:
	Sound(const std::wstring& waveResourceName, bool loopForever, bool hasReverb);
	Sound(Sound&& other);
	~Sound();

	void Play();
	void Play3D(const AudioEmitter& audioEmitter, float volume = 1.0f);
};