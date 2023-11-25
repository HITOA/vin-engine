#ifndef VIN_EDITOR_ASSETIMPORTER_H
#define VIN_EDITOR_ASSETIMPORTER_H

#include <vin/vin.h>
#include "../config/editorimportsettings.h"

inline Vin::AssetType GetType(Vin::StringView ext) {
    static const char* textExt = ".txt";
    static const char* textureExt = ".bmp.dds.exr.gif.jpg.jpeg.hdr.ktx.png.psd.pvr.tga";
    static const char* shaderExt = ".glsl.compute.frag.vert";
    static const char* meshExt = ".obj.gtlf.glb.3mf.dae.blend.bvh.3ds.ase.fbx.ply.dxf.ifc.iqm.nff.smd.vta.mdl.md2.md3.pk3.mdc.md5mesh.md5anim.md5camera.x.q3o.q3s.raw.ac.ac3d.stl.dxf.irrmesh.irr.off.ter.mdl.hmp.ogex.ms3D.lwa.lws.lxo.csm.ply.cob.scn.xgl";

    if (std::string{ textExt }.find(ext) != std::string::npos)
        return Vin::AssetType::Text;
    if (std::string{ textureExt }.find(ext) != std::string::npos)
        return Vin::AssetType::Texture;
    if (std::string{ shaderExt }.find(ext) != std::string::npos)
        return Vin::AssetType::Shader;
    if (std::string{ meshExt }.find(ext) != std::string::npos)
        return Vin::AssetType::Mesh;

    return Vin::AssetType::None;
}

template<Vin::AssetType type>
struct AssetImporter {};

#endif //VIN_EDITOR_ASSETIMPORTER_H
