/**
* Include this file in every shader when you want to use the default vin layout for uniform and attribute.
* Transform matrice are already defined.
*/

#ifndef VIN_SHADER_UTILS
#define VIN_SHADER_UTILS

//Vertex Attribute Location

#define VIN_POSITION_LOCATION 0
#define VIN_NORMAL_LOCATION 1
#define VIN_TANGENT_LOCATION 2
#define VIN_BITANGENT_LOCATION 3

#define VIN_COLOR0_LOCATION 4
#define VIN_COLOR1_LOCATION 5
#define VIN_COLOR2_LOCATION 6
#define VIN_COLOR3_LOCATION 7

#define VIN_BLENDWEIGHT_LOCATION 8
#define VIN_BLENDINDICES_LOCATION 9

#define VIN_TEXCOORD0_LOCATION 10
#define VIN_TEXCOORD1_LOCATION 11
#define VIN_TEXCOORD2_LOCATION 12
#define VIN_TEXCOORD3_LOCATION 13
#define VIN_TEXCOORD4_LOCATION 14
#define VIN_TEXCOORD5_LOCATION 15
#define VIN_TEXCOORD6_LOCATION 16
#define VIN_TEXCOORD7_LOCATION 17


//Vertex attribute shortcut

#define VIN_POSITION layout (location = VIN_POSITION_LOCATION) in
#define VIN_NORMAL layout (location = VIN_NORMAL_LOCATION) in
#define VIN_TANGENT layout (location = VIN_TANGENT_LOCATION) in
#define VIN_BITANGENT layout (location = VIN_BITANGENT_LOCATION) in

#define VIN_COLOR0 layout (location = VIN_COLOR0_LOCATION) in
#define VIN_COLOR1 layout (location = VIN_COLOR1_LOCATION) in
#define VIN_COLOR2 layout (location = VIN_COLOR2_LOCATION) in
#define VIN_COLOR3 layout (location = VIN_COLOR3_LOCATION) in

#define VIN_BLENDWEIGHT layout (location = VIN_BLENDWEIGHT_LOCATION) in
#define VIN_BLENDINDICES layout (location = VIN_BLENDINDICES_LOCATION) in

#define VIN_TEXCOORD0 layout (location = VIN_TEXCOORD0_LOCATION) in
#define VIN_TEXCOORD1 layout (location = VIN_TEXCOORD1_LOCATION) in
#define VIN_TEXCOORD2 layout (location = VIN_TEXCOORD2_LOCATION) in
#define VIN_TEXCOORD3 layout (location = VIN_TEXCOORD3_LOCATION) in
#define VIN_TEXCOORD4 layout (location = VIN_TEXCOORD4_LOCATION) in
#define VIN_TEXCOORD5 layout (location = VIN_TEXCOORD5_LOCATION) in
#define VIN_TEXCOORD6 layout (location = VIN_TEXCOORD6_LOCATION) in
#define VIN_TEXCOORD7 layout (location = VIN_TEXCOORD7_LOCATION) in

//Transform matrices

#endif