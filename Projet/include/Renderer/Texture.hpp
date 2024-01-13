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
#include <limits>

struct PixelColor
{
	inline PixelColor(Vec2Int position, Vec4 color) : Position(position), Color(color) {}
	inline PixelColor() = default;
	inline ~PixelColor() = default;
	Vec2Int Position;
	Vec4 Color;
};

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
	int width = 0, height = 0, channels = 0;

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
	static Ref<Texture> Create(const std::filesystem::path& path, Texture2DSpecification textureSpecs = Texture2DSpecification());
	static Ref<Texture> Create(Vec3 color, int width, int height, Texture2DSpecification textureSpecs = Texture2DSpecification());
	static Ref<Texture> Create(Vec4 color, int width, int height, Texture2DSpecification textureSpecs = Texture2DSpecification());
	static Ref<Texture> Create(const uint8_t* data, int width, int height, int channels, Texture2DSpecification textureSpecs = Texture2DSpecification());
	static Ref<Texture> Create(const uint16_t* data, int width, int height, int channels, Texture2DSpecification textureSpecs = Texture2DSpecification());
	static Ref<Texture> Create(Texture2DSpecification textureSpecs);
public:
	Texture(const std::filesystem::path& path, Texture2DSpecification textureSpecs);
	Texture(Vec3 color, int width, int height, Texture2DSpecification textureSpecs);
	Texture(Vec4 color, int width, int height, Texture2DSpecification textureSpecs);
	Texture(const uint8_t* data, int width, int height, int channels, Texture2DSpecification textureSpecs);
	Texture(const uint16_t* data, int width, int height, int channels, Texture2DSpecification textureSpecs);
	Texture(Texture2DSpecification textureSpecs);
public:
	~Texture();

	inline int GetWidth() const {return m_Width; }
	inline int GetHeight() const {return m_Height; }
	inline int GetDepth() const { return 0; }
	inline uint32_t GetRendererID() const { return m_RendererID; };
	inline int GetChannelCount() const {return m_Channels; }

	void Bind() const;
	static void Unbind();

	std::vector<Vec3> GetPixels3();
	std::vector<Vec4> GetPixels4();

	Vec3 GetPixel3(Vec2Int pixel);
	Vec4 GetPixel4(Vec2Int pixel);

	const std::vector<uint8_t>& GetNativePixels();

	void SetPixel4(Vec2Int pixel, Vec4 color);
	void SetPixel3(Vec2Int pixel, Vec3 color);

	void SetPixels4(const std::vector<Vec4>& color);
	void SetPixels3(const std::vector<Vec3>& color);

	void SetPixels(const std::vector<PixelColor>& pixels);
	void SetPixels(const std::unordered_map<Vec2Int, Vec4>& pixels);

	inline int GetIndex(Vec2Int pixel);

	void UpdateTexture();

	inline bool IsDirty() const {return dirty;}
private:
	void Create();
	void ReCreate(const void* data);
private:
	UUID id;
	Texture2DSpecification m_TextureSpecification;
	std::filesystem::path m_Path;
	int m_Width, m_Height, m_Channels;
	uint32_t m_RendererID;
	std::vector<uint8_t> pixels;
	bool dirty = false;
};