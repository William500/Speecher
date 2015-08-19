#include "PrecompiledHeader.h"
#include "RiffFile.h"

inline static const uint8_t* GetResourcePointer(const std::wstring& resourceName)
{
	auto resource = FindResourceW(nullptr, resourceName.c_str(), L"WAVE");
	Assert(resource != nullptr);

	auto resourceHandle = LoadResource(nullptr, resource);
	Assert(resourceHandle != nullptr);

	auto resourcePtr = LockResource(resourceHandle);
	Assert(resourcePtr != nullptr);

	return static_cast<uint8_t*>(resourcePtr);
}

inline static unsigned int ReadUInt(const uint8_t* buffer, unsigned int& position)
{
	auto value = *reinterpret_cast<const unsigned int*>(&buffer[position]);
	position += 4;
	return value;
}

RiffFile::RiffFile(int format) :
	fileFormat(format)
{
}

RiffFile::~RiffFile()
{
}

RiffFile::RiffFile(RiffFile&& other) :
	fileFormat(other.fileFormat),
	chunks(std::move(other.chunks))
{
}

void RiffFile::AddChunk(int fourCC, int size, const uint8_t* data)
{
	Assert(chunks.find(*reinterpret_cast<FourCCWrapper*>(&fourCC)) == chunks.end());
	chunks.emplace(fourCC, RiffChunk(size, data));
}

RiffFile RiffFile::Create(const std::wstring& resourceName)
{
	unsigned int dataSize, fileType;
	const auto riffData = GetResourcePointer(resourceName);

#if DEBUG
	unsigned int bufferPosition = 0;
	auto fileFourCC = ReadUInt(riffData, bufferPosition);
	Assert(fileFourCC == RiffFourCC::RIFF);
#else
	unsigned int bufferPosition = 4;
#endif

	dataSize = ReadUInt(riffData, bufferPosition);
	fileType = ReadUInt(riffData, bufferPosition);

	RiffFile riff(fileType);

	while (bufferPosition < dataSize + 8)
	{
		auto chunkFourCC = ReadUInt(riffData, bufferPosition);
		//Assert(chunkFourCC != RiffFourCC::LIST);	// Lists currently not supported

		auto chunkSize = ReadUInt(riffData, bufferPosition);
		
		riff.AddChunk(chunkFourCC, chunkSize, &riffData[bufferPosition]); 
		bufferPosition += chunkSize + (chunkSize % 2);
	}

	return riff;
}


RiffChunk& RiffFile::GetChunk(int fourCC)
{
	auto chunk = chunks.find(*reinterpret_cast<FourCCWrapper*>(&fourCC));
	Assert(chunk != chunks.end());

	return chunk->second;
}