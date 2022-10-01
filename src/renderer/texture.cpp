#include "texture.hpp"

#include <assert.hpp>
#include "renderer/renderer.hpp"
#include "opengl/texture_opengl.hpp"

std::shared_ptr<Vin::Texture> Vin::Texture::Create(size_t width, size_t height, TextureFormat format, bool mipmap)
{
    VIN_ASSERT(Renderer::GetApi() != Renderer::None, "Rendering api not initialized.");

    switch (Renderer::GetApi()) {
    case Renderer::OpenGL: return std::make_shared<Vin::OpenGLTexture>(width, height, format, mipmap);
    }
}
