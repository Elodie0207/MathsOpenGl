//
// Created by ianpo on 29/12/2023.
//

#pragma once
#include <Core.hpp>
#include <Math.hpp>
#include <string>
#include <vector>
#include <unordered_map>
#include <filesystem>
#include <variant>

enum TextureFilter
{
	Nearest,
	Linear,
};

enum TextureWrapper
{
	Repeat,
	ClampToEdge,
	MirroredRepeat,
};

enum PixelFormat
{
//	RED = 1,
//	RG = 2,
	RGB = 3,
	RGBA = 4,
};

enum PixelType
{
	PX_8,
	PX_16,

//	PX_8UI,
//	PX_16UI,
//	PX_32UI,
//
//	PX_8I,
//	PX_16I,
//	PX_32I,
//
//	PX_16F, // This is a Half Float.
//	PX_32F,
};

struct Texture2DSpecification
{
	Texture2DSpecification() = default;
	~Texture2DSpecification() = default;

	UUID id;
	std::string name;

	std::variant<std::filesystem::path, const void*> image;
	uint32_t width = 0, height = 0, channels = 0;

	TextureFilter filterMag = TextureFilter::Linear;
	TextureFilter filterMin = TextureFilter::Linear;

	TextureWrapper wrapperS = TextureWrapper::Repeat;
	TextureWrapper wrapperT = TextureWrapper::Repeat;

	PixelFormat pixelFormat = PixelFormat::RGBA;
	PixelType pixelType = PixelType::PX_8;
};

class Texture
{
public:
	static Ref<Texture> Create(const std::filesystem::path& path);
	static Ref<Texture> Create(Vec3 color, int width, int height);
	static Ref<Texture> Create(Vec4 color, int width, int height);
	static Ref<Texture> Create(const uint8_t* data, int width, int height, int channels);
	static Ref<Texture> Create(const uint16_t* data, int width, int height, int channels);
	static Ref<Texture> Create(Texture2DSpecification textureSpecs);
public:
	Texture(const std::filesystem::path& path);
	Texture(Vec3 color, int width, int height);
	Texture(Vec4 color, int width, int height);
	Texture(const uint8_t* data, int width, int height, int channels);
	Texture(const uint16_t* data, int width, int height, int channels);
	Texture(Texture2DSpecification textureSpecs);
public:
	~Texture();

	inline uint32_t GetWidth() const {return m_Width; }
	inline uint32_t GetHeight() const {return m_Height; }
	inline uint32_t GetDepth() const { return 0; }
	inline uint32_t GetRendererID() const { return m_RendererID; };
	inline uint32_t GetChannelCount() const {return m_Channels; }

	void Bind() const;
	static void Unbind();

	std::vector<Vec3> GetPixels3();
	std::vector<Vec4> GetPixels4();
private:
	void Create();
private:
	UUID id;
	Texture2DSpecification m_TextureSpecification;
	std::filesystem::path m_Path;
	uint32_t m_Width, m_Height, m_Channels;
	uint32_t m_RendererID;
	std::vector<Vec3UB> pixels3;
	std::vector<Vec4UB> pixels4;
};