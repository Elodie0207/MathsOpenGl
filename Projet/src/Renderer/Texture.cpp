//
// Created by ianpo on 29/12/2023.
//

#include "Renderer/Texture.hpp"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#define PX_8_MAX 255
#define PX_8_COUNT 256
#define PX_16_MAX 65355
#define PX_16_COUNT 65356

Ref<Texture> Texture::Create(const std::filesystem::path& path, Texture2DSpecification textureSpecs)
{
	return std::make_shared<Texture>(path, textureSpecs);
}
Ref<Texture> Texture::Create(Vec3 color, int width, int height, Texture2DSpecification textureSpecs)
{
	return std::make_shared<Texture>(color, width, height, textureSpecs);
}
Ref<Texture> Texture::Create(Vec4 color, int width, int height, Texture2DSpecification textureSpecs)
{
	return std::make_shared<Texture>(color, width, height, textureSpecs);
}
Ref<Texture> Texture::Create(const uint8_t* data, int width, int height, int channels, Texture2DSpecification textureSpecs)
{
	return std::make_shared<Texture>(data, width, height, channels, textureSpecs);
}
Ref<Texture> Texture::Create(const uint16_t* data, int width, int height, int channels, Texture2DSpecification textureSpecs)
{
	return std::make_shared<Texture>(data, width, height, channels, textureSpecs);
}
Ref<Texture> Texture::Create(Texture2DSpecification textureSpecs)
{
	return std::make_shared<Texture>(textureSpecs);
}

class TexSpecHelper
{
private:
	Texture2DSpecification& spec;
	stbi_uc* data;

public:
	TexSpecHelper(Texture2DSpecification& specification) : spec(specification), data(nullptr) {}
	~TexSpecHelper()
	{
		// REI_PROFILE_FUNCTION();
		if(data != nullptr)
		{
			stbi_image_free(data);
		}
	}

	void ReadDataFromImage()
	{
		// REI_PROFILE_FUNCTION();
		if(std::holds_alternative<std::filesystem::path>(spec.image) && !data)
		{
			const std::filesystem::path& path = std::get<std::filesystem::path>(spec.image);
			stbi_set_flip_vertically_on_load(true);

			int width, height, channels;
			std::string strPath = path.string();
			REI_ASSERT(std::filesystem::exists(path), "The file {0} do not exist...", strPath);

			data = stbi_load(strPath.c_str(), &width, &height, &channels, static_cast<int>(spec.channels));
			REI_ASSERT(data, "Load of image '{0}' failed.\n{1}.", strPath, stbi_failure_reason());

			if(spec.width && spec.width != width) REI_WARNING("The width set in the spec ({0}) doesn't not correspond to the image ({1}).", spec.width, width);
			if(spec.height && spec.height != height) REI_WARNING("The height set in the spec ({0}) doesn't not correspond to the image ({1}).", spec.height, height);
			if(spec.channels && spec.channels != channels) REI_WARNING("The channels set in the spec ({0}) doesn't not correspond to the image ({1}).", spec.channels, channels);

			spec.width = width;
			spec.height = height;
			spec.channels = channels;

			spec.pixelType = PixelType::PX_8;
			spec.pixelFormat = static_cast<PixelFormat>(channels);
		}
	}

	GLenum GetInternalFormat() const
	{
		// REI_PROFILE_FUNCTION();
		switch (spec.pixelFormat) {
//			case RED:
//			{
//				switch (spec.pixelType) {
//					case PX_8: return GL_R8;
//					case PX_16: return GL_R16;
//
//					case PX_8UI: return GL_R8UI;
//					case PX_16UI: return GL_R16UI;
//					case PX_32UI: return GL_R32UI;
//
//					case PX_8I: return GL_R8I;
//					case PX_16I: return GL_R16I;
//					case PX_32I: return GL_R32I;
//
//					case PX_16F: return GL_R16F;
//					case PX_32F: return GL_R32F;
//				}
//			}
//			case RG:
//			{
//				switch (spec.pixelType) {
//					case PX_8: return GL_RG8;
//					case PX_16: return GL_RG16;
//
//					case PX_8UI: return GL_RG8UI;
//					case PX_16UI: return GL_RG16UI;
//					case PX_32UI: return GL_RG32UI;
//
//					case PX_8I: return GL_RG8I;
//					case PX_16I: return GL_RG16I;
//					case PX_32I: return GL_RG32I;
//
//					case PX_16F: return GL_RG16F;
//					case PX_32F: return GL_RG32F;
//				}
//			}
			case RGB:
			{
				switch (spec.pixelType) {
					case PX_8: return GL_RGB8;
					case PX_16: return GL_RGB16;

//					case PX_8UI: return GL_RGB8UI;
//					case PX_16UI: return GL_RGB16UI;
//					case PX_32UI: return GL_RGB32UI;
//
//					case PX_8I: return GL_RGB8I;
//					case PX_16I: return GL_RGB16I;
//					case PX_32I: return GL_RGB32I;
//
//					case PX_16F: return GL_RGB16F;
//					case PX_32F: return GL_RGB32F;
				}
			}
			case RGBA:
			{
				switch (spec.pixelType) {
					case PX_8: return GL_RGBA8;
					case PX_16: return GL_RGBA16;

//					case PX_8UI: return GL_RGBA8UI;
//					case PX_16UI: return GL_RGBA16UI;
//					case PX_32UI: return GL_RGBA32UI;
//
//					case PX_8I: return GL_RGBA8I;
//					case PX_16I: return GL_RGBA16I;
//					case PX_32I: return GL_RGBA32I;
//
//					case PX_16F: return GL_RGBA16F;
//					case PX_32F: return GL_RGBA32F;
				}
			}
		}

		REI_WARNING("No valid internal format found. Defaulting to GL_RGB8.");
		return GL_RGB8;
	}

	GLenum GetFormat() const
	{
		// REI_PROFILE_FUNCTION();
		switch (spec.pixelFormat) {
//			case RED: return GL_RED;
//			case RG: return GL_RG;
			case RGB: return GL_RGB;
			case RGBA: return GL_RGBA;
		}

		REI_ASSERT(false, "The pixel format {0} do not exist...", (int)spec.pixelType);
		return GL_RGB;
	}

	GLenum GetType() const
	{
		// REI_PROFILE_FUNCTION();
		switch (spec.pixelType) {
			case PX_8: return GL_UNSIGNED_BYTE;
			case PX_16: return GL_UNSIGNED_SHORT;
//			case PX_8UI: return GL_UNSIGNED_BYTE;
//			case PX_16UI: return GL_UNSIGNED_SHORT;
//			case PX_32UI: return GL_UNSIGNED_INT;
//			case PX_8I: return GL_BYTE;
//			case PX_16I: return GL_SHORT;
//			case PX_32I: return GL_INT;
//			case PX_16F: return GL_HALF_FLOAT;
//			case PX_32F: return GL_FLOAT;
		}

		REI_ASSERT(false, "The pixel type {0} do not exist...", (int)spec.pixelType);
		return GL_UNSIGNED_BYTE;
	}

	GLint GetMagFilter() const
	{
		// REI_PROFILE_FUNCTION();
		switch (spec.filterMag) {

			case Nearest: return GL_NEAREST;
			case Linear: return GL_LINEAR;
		}
		REI_WARNING("No value found for MagFilter, defaulting to 'Linear'.");
		return GL_LINEAR;
	}

	GLint GetMinFilter() const
	{
		// REI_PROFILE_FUNCTION();
		switch (spec.filterMin) {

			case Nearest: return GL_NEAREST;
			case Linear: return GL_LINEAR;
		}
		REI_WARNING("No value found for MinFilter, defaulting to 'Linear'.");
		return GL_LINEAR;
	}

	GLint GetWrapS() const
	{
		// REI_PROFILE_FUNCTION();
		switch (spec.wrapperS) {
			case Repeat: return GL_REPEAT;
			case ClampToEdge: return GL_CLAMP;
			case MirroredRepeat: return GL_REPEAT ;
		}
		REI_WARNING("No value found for WrapS, defaulting to 'Repeat'.");
		return GL_REPEAT;
	}

	GLint GetWrapT() const
	{
		// REI_PROFILE_FUNCTION();
		switch (spec.wrapperT) {
			case Repeat: return GL_REPEAT;
			case ClampToEdge: return GL_CLAMP;
			case MirroredRepeat: return GL_REPEAT;
		}
		REI_WARNING("No value found for WrapT, defaulting to 'Repeat'.");
		return GL_REPEAT;
	}

	const void* GetData()
	{
		// REI_PROFILE_FUNCTION();
		REI_ASSERT(spec.image.index() != std::variant_npos, "The spec don't contain any image.");
		if(std::holds_alternative<const void*>(spec.image))
		{
			return std::get<const void*>(spec.image);
		}
		else if(data == nullptr)
		{
			const std::filesystem::path& path = std::get<std::filesystem::path>(spec.image);
			stbi_set_flip_vertically_on_load(true);

			int width, height, channels;
			std::string strPath = path.string();
			REI_ASSERT(std::filesystem::exists(path), "The file {0} do not exist...", strPath);

			data = stbi_load(strPath.c_str(), &width, &height, &channels, 0);
			REI_ASSERT(data, "Load of image '{0}' failed.\n{1}.", strPath, stbi_failure_reason());
			REI_ASSERT(spec.width == width,"The width set in the spec ({0}) doesn't not correspond to the image ({1}).", spec.width, width);
			REI_ASSERT(spec.height == height,"The height set in the spec ({0}) doesn't not correspond to the image ({1}).", spec.height, height);
			REI_ASSERT(spec.channels == channels,"The channels set in the spec ({0}) doesn't not correspond to the image ({1}).", spec.channels, channels);
			REI_ASSERT(spec.pixelType == PixelType::PX_8, "The Pixel Type in the spec ({0}) doesn't not correspond to the image ({1})", (int)spec.pixelType, (int)PixelType::PX_8);
			REI_ASSERT(spec.pixelFormat == static_cast<PixelFormat>(channels), "The Pixel Format in the spec ({0}) doesn't not correspond to the image ({1})", (int)spec.pixelFormat, channels);
		}

		if(data == nullptr) { REI_WARNING("We return a null data pointer for the texture '{0}'", spec.name); }
		return data;
	}
};

Texture::Texture(const std::filesystem::path& path, Texture2DSpecification textureSpecs) : m_TextureSpecification(textureSpecs), m_Path(path)
{
	// REI_PROFILE_FUNCTION();
	m_TextureSpecification.name = m_Path.string();
	m_TextureSpecification.image = m_Path;

	Create();
	Texture::Unbind();
}

Texture::Texture(Vec3 color, int width, int height, Texture2DSpecification textureSpecs) : m_TextureSpecification(textureSpecs), pixels3(width * height, Vec3UB(color.x * PX_8_MAX, color.y * PX_8_MAX, color.z * PX_8_MAX))
{
	// REI_PROFILE_FUNCTION();
	m_TextureSpecification.name = std::to_string(m_TextureSpecification.id);
	m_TextureSpecification.image = pixels3.data();
	m_TextureSpecification.channels = 3;
	m_TextureSpecification.width = width;
	m_TextureSpecification.height = height;
	m_TextureSpecification.pixelType = PX_8;
	m_TextureSpecification.pixelFormat = PixelFormat::RGB;

	Create();
	Texture::Unbind();
}

Texture::Texture(Vec4 color, int width, int height, Texture2DSpecification textureSpecs) : m_TextureSpecification(textureSpecs), pixels4(width * height, Vec4UB(color.x * PX_8_MAX, color.y * PX_8_MAX, color.z * PX_8_MAX, color.w * PX_8_MAX))
{
	// REI_PROFILE_FUNCTION();
	m_TextureSpecification.name = std::to_string(m_TextureSpecification.id);
	m_TextureSpecification.image = pixels4.data();
	m_TextureSpecification.channels = 4;
	m_TextureSpecification.width = width;
	m_TextureSpecification.height = height;
	m_TextureSpecification.pixelType = PX_8;
	m_TextureSpecification.pixelFormat = PixelFormat::RGBA;

	Create();
	Texture::Unbind();
}


Texture::Texture(const uint8_t* data, int width, int height, int channels, Texture2DSpecification textureSpecs) : m_TextureSpecification(textureSpecs), m_Width(width), m_Height(height), m_Channels(channels)
{
	// REI_PROFILE_FUNCTION();
	m_TextureSpecification.name = std::to_string(m_TextureSpecification.id);
	REI_ASSERT(channels > 0 && channels < 5, "The number of channel {0} is not handle at the moment.", channels);
	m_TextureSpecification.pixelFormat = static_cast<PixelFormat>(channels);
	m_TextureSpecification.pixelType = PX_8;
	m_TextureSpecification.width = width;
	m_TextureSpecification.height = height;
	m_TextureSpecification.channels = channels;
	m_TextureSpecification.image = data;

	Create();
	Texture::Unbind();
}

Texture::Texture(const uint16_t* data, int width, int height, int channels, Texture2DSpecification textureSpecs) : m_TextureSpecification(textureSpecs), m_Width(width), m_Height(height), m_Channels(channels)
{
	// REI_PROFILE_FUNCTION();
	m_TextureSpecification.name = std::to_string(m_TextureSpecification.id);
	REI_ASSERT(channels > 0 && channels < 5, "The number of channel {0} is not handle at the moment.", channels);
	m_TextureSpecification.pixelFormat = static_cast<PixelFormat>(channels);
	m_TextureSpecification.pixelType = PX_16;
	m_TextureSpecification.width = width;
	m_TextureSpecification.height = height;
	m_TextureSpecification.channels = channels;
	m_TextureSpecification.image = data;

	Create();
	Texture::Unbind();
}

Texture::Texture(Texture2DSpecification textureSpecs) : m_TextureSpecification(std::move(textureSpecs))
{
	// REI_PROFILE_FUNCTION();
	Create();
	Texture::Unbind();
}

void Texture::Create()
{
	// REI_PROFILE_FUNCTION();
	TexSpecHelper helper(m_TextureSpecification);
	helper.ReadDataFromImage();

	m_Width = m_TextureSpecification.width;
	m_Height = m_TextureSpecification.height;
	m_Channels = m_TextureSpecification.channels;

	REI_ASSERT(m_TextureSpecification.channels > 0 && m_TextureSpecification.channels < 5, "The number of channel {0} is not handle at the moment.", m_TextureSpecification.channels);

	GLint internalFormat = helper.GetInternalFormat();
	GLenum dataFormat = helper.GetFormat();
	GLenum pixelType = helper.GetType();
	const void* data = helper.GetData();
	REI_ASSERT(data != nullptr, "No data where found on the image {0}.", m_TextureSpecification.name);

	glGenTextures(1, &m_RendererID);
	glBindTexture(GL_TEXTURE_2D, m_RendererID);

	//TODO: Add parameter on the m_Texture API to be able to change this type of parameters.
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, helper.GetMinFilter());
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, helper.GetMagFilter());
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, helper.GetWrapS());
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, helper.GetWrapT());

	glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, m_Width, m_Height, 0, dataFormat, pixelType, helper.GetData());
}

Texture::~Texture() {
	// REI_PROFILE_FUNCTION();
	glDeleteTextures(1, &m_RendererID);
}

void Texture::Bind() const {
	// REI_PROFILE_FUNCTION();
	glBindTexture(GL_TEXTURE_2D, m_RendererID);
}

void Texture::Unbind() {
	// REI_PROFILE_FUNCTION();
	glBindTexture(GL_TEXTURE_2D, 0);
}

std::vector<Vec3> Texture::GetPixels3()
{
	std::vector<Vec3> pixels(m_Width * m_Height);
	int pxChannelSize = m_TextureSpecification.pixelType == PixelType::PX_8 ? 1 : 2;
	int pxSize = m_Channels * pxChannelSize;
	auto* data = new uint8_t [m_Width * m_Height * m_Channels * pxChannelSize];
	auto helper = TexSpecHelper(m_TextureSpecification);
	glBindTexture(GL_TEXTURE_2D, m_RendererID);
	glGetTexImage(GL_TEXTURE_2D, 0, helper.GetFormat(), helper.GetType(), data);
	for (int i = 0; i < m_Width * m_Height; i+= pxSize)
	{
		int index = i / pxSize;
		for (int j = 0; j < 3; j+= pxChannelSize) {
			if(m_TextureSpecification.pixelType == PixelType::PX_8) pixels[index][j] = (float)data[i + j] / PX_8_MAX;
			else pixels[index][j] = (float)*((uint16_t*)(&data[i + j])) / PX_16_MAX;
		}
	}
	delete[] data;
	return pixels;
}

std::vector<Vec4> Texture::GetPixels4()
{
	std::vector<Vec4> pixels(m_Width * m_Height, Vec4(1.0));

	int pxChannelSize = m_TextureSpecification.pixelType == PixelType::PX_8 ? 1 : 2;
	int pxSize = m_Channels * pxChannelSize;
	auto* data = new uint8_t [m_Width * m_Height * m_Channels * pxChannelSize];
	auto helper = TexSpecHelper(m_TextureSpecification);
	glBindTexture(GL_TEXTURE_2D, m_RendererID);
	glGetTexImage(GL_TEXTURE_2D, 0, helper.GetFormat(), helper.GetType(), data);
	for (int i = 0; i < m_Width * m_Height; i+= pxSize)
	{
		int index = i / pxSize;
		for (int j = 0; j < std::min(4, (int)m_Channels); j += pxChannelSize) {
			if(m_TextureSpecification.pixelType == PixelType::PX_8) pixels[index][j] = (float)data[i + j] / PX_8_MAX;
			else pixels[index][j] = (float)*((uint16_t*)(&data[i + j])) / PX_16_MAX;
		}
	}
	delete[] data;
	return pixels;
}


Vec3 Texture::GetPixel3(Vec2Int pixel)
{
	return GetPixels3()[GetIndex(pixel)];
}
Vec4 Texture::GetPixel4(Vec2Int pixel)
{
	return GetPixels4()[GetIndex(pixel)];
}
std::vector<uint8_t> Texture::GetNativePixels()
{
	int pxChannelSize = m_TextureSpecification.pixelType == PixelType::PX_8 ? 1 : 2;
	int pxSize = m_Channels * pxChannelSize;
	std::vector<uint8_t> result(m_Width * m_Height * pxSize);
	auto helper = TexSpecHelper(m_TextureSpecification);
	glBindTexture(GL_TEXTURE_2D, m_RendererID);
	glGetTexImage(GL_TEXTURE_2D, 0, helper.GetFormat(), helper.GetType(), &result[0]);
	return result;
}

void Texture::SetPixel4(Vec2Int pixel, Vec4 color)
{
	if(m_TextureSpecification.pixelFormat == PixelFormat::RGB) return SetPixel3(pixel, {color.x, color.y, color.z});

	// Get the all images
	int pxChannelSize = m_TextureSpecification.pixelType == PixelType::PX_8 ? 1 : 2;
	int pxSize = m_Channels * pxChannelSize;
	auto* data = new uint8_t [m_Width * m_Height * m_Channels * pxChannelSize];
	auto helper = TexSpecHelper(m_TextureSpecification);
	glBindTexture(GL_TEXTURE_2D, m_RendererID);
	glGetTexImage(GL_TEXTURE_2D, 0, helper.GetFormat(), helper.GetType(), data);

	//Edit only one px.
	int px = (pixel.y * m_Width * pxSize) + (pixel.x * pxSize);
	REI_ASSERT(px < m_Width * m_Height * m_Channels * pxChannelSize, "The position cannot be right...");
	if(pxChannelSize == 1)
	{
		Vec4UB c = Vec4UB(color.x * PX_8_MAX, color.y * PX_8_MAX, color.z * PX_8_MAX, color.w * PX_8_MAX);
		for (int i = 0; i < 4; ++i) {
			data[px + i*pxChannelSize] = c[i];
		}
	}
	else
	{
		Vec4US c = Vec4US(color.x * PX_16_MAX, color.y * PX_16_MAX, color.z * PX_16_MAX, color.w * PX_16_MAX);
		for (int i = 0; i < 4; ++i) {
			*(uint16_t*)&data[px + i*pxChannelSize] = c[i];
		}
	}

	ReCreate(data);
	delete[] data;
}

void Texture::SetPixel3(Vec2Int pixel, Vec3 color)
{
	if(m_TextureSpecification.pixelFormat == PixelFormat::RGBA) return SetPixel4(pixel, {color.x, color.y, color.z, 1.0});

	// Get the all images
	int pxChannelSize = m_TextureSpecification.pixelType == PixelType::PX_8 ? 1 : 2;
	int pxSize = m_Channels * pxChannelSize;
	auto* data = new uint8_t [m_Width * m_Height * m_Channels * pxChannelSize];
	auto helper = TexSpecHelper(m_TextureSpecification);
	glBindTexture(GL_TEXTURE_2D, m_RendererID);
	glGetTexImage(GL_TEXTURE_2D, 0, helper.GetFormat(), helper.GetType(), data);

	//Edit only one px.
	int px = (pixel.y * m_Width * pxSize) + (pixel.x * pxSize);
	REI_ASSERT(px < m_Width * m_Height * m_Channels * pxChannelSize, "The position cannot be right...");
	if(pxChannelSize == 1)
	{
		Vec3UB c = Vec3UB(color.x * PX_8_MAX, color.y * PX_8_MAX, color.z * PX_8_MAX);
		for (int i = 0; i < 3; ++i) {
			data[px + i*pxChannelSize] = c[i];
		}
	}
	else
	{
		Vec3US c = Vec3US(color.x * PX_16_MAX, color.y * PX_16_MAX, color.z * PX_16_MAX);
		for (int i = 0; i < 3; ++i) {
			*(uint16_t*)&data[px + i*pxChannelSize] = c[i];
		}
	}

	ReCreate(data);
	delete[] data;
}


void Texture::SetPixels4(const std::vector<Vec4>& color)
{
	REI_ASSERT(color.size() == (m_Width * m_Height), "There should be {0} pixels in the array and there is {1}.", m_Width * m_Height, color.size());
	int pxChannelSize = m_TextureSpecification.pixelType == PixelType::PX_8 ? 1 : 2;
	int pxSize = m_Channels * pxChannelSize;
	uint8_t* data = new uint8_t [m_Width * m_Height * pxSize];
	auto helper = TexSpecHelper(m_TextureSpecification);

	for (int i = 0; i < m_Width * m_Height; i+= pxSize)
	{
		int index = i / pxSize;
		for (int j = 0; j < std::min(4, (int)m_Channels); j += pxChannelSize) {
			if(m_TextureSpecification.pixelType == PixelType::PX_8) {
				data[i + j] = static_cast<uint8_t>(color[index][j] * PX_8_MAX);
			}
			else {
				*((uint16_t*)&data[i + j]) = static_cast<uint16_t>(color[index][j] * PX_16_MAX);
			}
		}
	}

	ReCreate(data);
	delete[] data;
}
void Texture::SetPixels3(const std::vector<Vec3>& color)
{
	REI_ASSERT(color.size() == (m_Width * m_Height), "There should be {0} pixels in the array and there is {1}.", m_Width * m_Height, color.size());
	int pxChannelSize = m_TextureSpecification.pixelType == PixelType::PX_8 ? 1 : 2;
	int pxSize = m_Channels * pxChannelSize;
	uint8_t* data = new uint8_t [m_Width * m_Height * pxSize];
	auto helper = TexSpecHelper(m_TextureSpecification);

	for (int i = 0; i < m_Width * m_Height; i+= pxSize)
	{
		int index = i / pxSize;
		for (int j = 0; j < 3; j += pxChannelSize) {
			if(m_TextureSpecification.pixelType == PixelType::PX_8) {
				data[i + j] = static_cast<uint8_t>(color[index][j] * PX_8_MAX);
			}
			else {
				*((uint16_t*)&data[i + j]) = static_cast<uint16_t>(color[index][j] * PX_16_MAX);
			}
		}
	}

	ReCreate(data);
	delete[] data;
}

void Texture::SetPixels(const std::vector<PixelColor>& pixels)
{
	int pxChannelSize = m_TextureSpecification.pixelType == PixelType::PX_8 ? 1 : 2;
	int pxSize = m_Channels * pxChannelSize;
	uint8_t* data = new uint8_t[m_Width * m_Height * pxSize];
	auto helper = TexSpecHelper(m_TextureSpecification);
	glBindTexture(GL_TEXTURE_2D, m_RendererID);
	glGetTexImage(GL_TEXTURE_2D, 0, helper.GetFormat(), helper.GetType(), data);

	for (auto& px : pixels)
	{
		auto index = GetIndex(px.Position);
		if (m_Channels == 3)
		{
			if (m_TextureSpecification.pixelType == PixelType::PX_8)
			{
				Vec3UB color = Vec3UB(px.Color.x * PX_8_MAX, px.Color.y * PX_8_MAX, px.Color.z * PX_8_MAX);
				(*((Vec3UB*)&data[index])) = color;
			}
			else
			{
				Vec3US color = Vec3US(px.Color.x * PX_16_MAX, px.Color.y * PX_16_MAX, px.Color.z * PX_16_MAX);
				(*((Vec3US*)&data[index])) = color;
			}
		}
		else // if (m_Channels == 4)
		{
			if (m_TextureSpecification.pixelType == PixelType::PX_8)
			{
				Vec4UB color = Vec4UB(px.Color.x * PX_8_MAX, px.Color.y * PX_8_MAX, px.Color.z * PX_8_MAX, px.Color.w * PX_8_MAX);
				(*((Vec4UB*)&data[index])) = color;
			}
			else
			{
				Vec4US color = Vec4US(px.Color.x * PX_16_MAX, px.Color.y * PX_16_MAX, px.Color.z * PX_16_MAX, px.Color.w * PX_16_MAX);
				(*((Vec4US*)&data[index])) = color;
			}
		}
	}

	ReCreate(data);
	delete[] data;
}


void Texture::SetPixels(const std::unordered_map<Vec2Int, Vec4>& pixels)
{
	int pxChannelSize = m_TextureSpecification.pixelType == PixelType::PX_8 ? 1 : 2;
	int pxSize = m_Channels * pxChannelSize;
	uint8_t* data = new uint8_t[m_Width * m_Height * pxSize];
	auto helper = TexSpecHelper(m_TextureSpecification);
	glBindTexture(GL_TEXTURE_2D, m_RendererID);
	glGetTexImage(GL_TEXTURE_2D, 0, helper.GetFormat(), helper.GetType(), data);

	for (auto&& [position, color] : pixels)
	{
		auto index = GetIndex(position);
		if (m_Channels == 3)
		{
			if (m_TextureSpecification.pixelType == PixelType::PX_8)
			{
				Vec3UB c= Vec3UB(color.x * PX_8_MAX, color.y * PX_8_MAX, color.z * PX_8_MAX);
				(*((Vec3UB*)&data[index])) = c;
			}
			else
			{
				Vec3US c= Vec3US(color.x * PX_16_MAX, color.y * PX_16_MAX, color.z * PX_16_MAX);
				(*((Vec3US*)&data[index])) = c;
			}
		}
		else // if (m_Channels == 4)
		{
			if (m_TextureSpecification.pixelType == PixelType::PX_8)
			{
				Vec4UB c= Vec4UB(color.x * PX_8_MAX, color.y * PX_8_MAX, color.z * PX_8_MAX, color.w * PX_8_MAX);
				(*((Vec4UB*)&data[index])) = c;
			}
			else
			{
				Vec4US c= Vec4US(color.x * PX_16_MAX, color.y * PX_16_MAX, color.z * PX_16_MAX, color.w * PX_16_MAX);
				(*((Vec4US*)&data[index])) = c;
			}
		}
	}

	ReCreate(data);
	delete[] data;
}

void Texture::ReCreate(const void *data)
{
	auto helper = TexSpecHelper(m_TextureSpecification);
	glBindTexture(GL_TEXTURE_2D, m_RendererID);
//	glDeleteTextures(1, &m_RendererID);
//	glGenTextures(1, &m_RendererID);
//	glBindTexture(GL_TEXTURE_2D, m_RendererID);
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, helper.GetMinFilter());
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, helper.GetMagFilter());
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, helper.GetWrapS());
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, helper.GetWrapT());
//	glTexImage2D(GL_TEXTURE_2D, 0, 0, 0, m_Width, m_Height, helper.GetFormat(), helper.GetType(), data);
	glTexImage2D(GL_TEXTURE_2D, 0, helper.GetInternalFormat(), m_Width, m_Height, 0, helper.GetFormat(), helper.GetType(), data);
}
