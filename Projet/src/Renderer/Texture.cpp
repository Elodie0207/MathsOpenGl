//
// Created by ianpo on 29/12/2023.
//

#include "Renderer/Texture.hpp"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

Ref<Texture> Texture::Create(const std::filesystem::path& path)
{
	return std::make_shared<Texture>(path);
}
Ref<Texture> Texture::Create(Vec3 color, int width, int height)
{
	return std::make_shared<Texture>(color, width, height);
}
Ref<Texture> Texture::Create(Vec4 color, int width, int height)
{
	return std::make_shared<Texture>(color, width, height);
}
Ref<Texture> Texture::Create(const uint8_t* data, int width, int height, int channels)
{
	return std::make_shared<Texture>(data, width, height, channels);
}
Ref<Texture> Texture::Create(const uint16_t* data, int width, int height, int channels)
{
	return std::make_shared<Texture>(data, width, height, channels);
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

Texture::Texture(const std::filesystem::path& path) : m_Path(path)
{
	// REI_PROFILE_FUNCTION();
	m_TextureSpecification.name = m_Path.string();
	m_TextureSpecification.image = m_Path;

	Create();
	Texture::Unbind();
}

Texture::Texture(Vec3 color, int width, int height) : pixels3(width * height, Vec3UB(color.x * 255, color.y * 255, color.z * 255))
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

Texture::Texture(Vec4 color, int width, int height) : pixels4(width * height, Vec4UB(color.x * 255, color.y * 255, color.z * 255, color.w * 255))
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


Texture::Texture(const uint8_t* data, int width, int height, int channels) : m_Width(width), m_Height(height), m_Channels(channels)
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

Texture::Texture(const uint16_t* data, int width, int height, int channels) : m_Width(width), m_Height(height), m_Channels(channels)
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

	//TODO: Add parameter on the Texture API to be able to change this type of parameters.
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
	auto* data = new unsigned char[m_Width * m_Height * m_Channels];
	auto helper = TexSpecHelper(m_TextureSpecification);
	glBindTexture(GL_TEXTURE_2D, m_RendererID);
	glGetTexImage(GL_TEXTURE_2D, 0, helper.GetFormat(), helper.GetType(), &data);
	for (int i = 0; i < m_Width * m_Height; i+= m_Channels)
	{
		int index = i / m_Channels;
		for (int j = 0; j < 3; ++j) {
			pixels[index][j] = (float)data[i + j] / 255.0f;
		}
	}
	delete[] data;
	return pixels;
}

std::vector<Vec4> Texture::GetPixels4()
{
	std::vector<Vec4> pixels(m_Width * m_Height, Vec4(1.0));
	auto* data = new unsigned char[m_Width * m_Height * m_Channels];
	auto helper = TexSpecHelper(m_TextureSpecification);
	glBindTexture(GL_TEXTURE_2D, m_RendererID);
	glGetTexImage(GL_TEXTURE_2D, 0, helper.GetFormat(), helper.GetType(), &data);
	for (int i = 0; i < m_Width * m_Height; i+= m_Channels)
	{
		int index = i / m_Channels;
		for (int j = 0; j < std::min(4, (int)m_Channels); ++j) {
			pixels[index][j] = (float)data[i + j] / 255.0f;
		}
	}
	delete[] data;
	return pixels;
}