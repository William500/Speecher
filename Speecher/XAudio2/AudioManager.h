#pragma once

#include "SoundCacheKey.h"

class Sound;
class AudioManager
{
private:
	Microsoft::WRL::ComPtr<IXAudio2> m_XAudio2;
	IXAudio2MasteringVoice* m_MasteringVoice;
	XAUDIO2_VOICE_DETAILS m_VoiceDetails;

	X3DAUDIO_HANDLE m_X3DAudio;
	X3DAUDIO_LISTENER m_Listener;
	X3DAUDIO_DSP_SETTINGS m_X3DSettings;
	std::unique_ptr<FLOAT32[]> m_3DAudioMatrixCoeficients;

	std::unordered_map<SoundCacheKey, Sound> m_CachedSounds;
	static std::unique_ptr<AudioManager> s_Instance;

	AudioManager();
	AudioManager(const AudioManager& other);

public:
	~AudioManager();

	static void Initialize();
	static AudioManager& GetInstance();	
	
	IXAudio2SubmixVoice* CreateSubmixVoice(const WAVEFORMATEXTENSIBLE& waveFormat);
	IXAudio2SourceVoice* CreateSourceVoice(const WAVEFORMATEX* waveFormat, IXAudio2VoiceCallback* voiceCallback, IXAudio2SubmixVoice* submixVoice);

	void SetListenerPosition(const DirectX::XMFLOAT3& position, const DirectX::XMFLOAT3& velocity, const DirectX::XMFLOAT3& front, 
		const DirectX::XMFLOAT3& up);
	void Calculate3DAudioForVoice(const X3DAUDIO_EMITTER& audioEmitter, IXAudio2SourceVoice* sourceVoice, int sourceChannels, 
		IXAudio2SubmixVoice* submixVoice);

	static Sound& GetCachedSound(const std::wstring& path, bool loopForever, bool hasReverb);
};