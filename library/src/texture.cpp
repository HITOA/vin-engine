#include <vin/scene/resources/texture.h>

Vin::Texture::Texture(const bgfx::Memory* data) {
    handle = bgfx::createTexture(data, BGFX_TEXTURE_NONE | BGFX_SAMPLER_NONE, 0, nullptr);
}

Vin::Texture::~Texture() {
    bgfx::destroy(handle);
}

bgfx::TextureHandle Vin::Texture::GetTextureHandle() {
    return handle;
}