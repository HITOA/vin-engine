/**
* Include this file in every shader when you want to use the default vin layout for uniform and attribute.
* Transform matrice are already defined.
*/

#ifndef VIN_SHADER_UTILS
#define VIN_SHADER_UTILS

//Vertex Attribute Location

#define POSITION_LOCATION 0
#define NORMAL_LOCATION 1
#define TANGENT_LOCATION 2
#define COLOR_LOCATION 3

#define TEXCOORD0_LOCATION 4
#define TEXCOORD1_LOCATION 5
#define TEXCOORD2_LOCATION 6
#define TEXCOORD3_LOCATION 7
#define TEXCOORD4_LOCATION 8
#define TEXCOORD5_LOCATION 9
#define TEXCOORD6_LOCATION 10
#define TEXCOORD7_LOCATION 11

#define BLENDWEIGHT_LOCATION 12
#define BLENDINDICES_LOCATION 13

//Vertex attribute shortcut

#define VIN_POSITION layout (location = POSITION_LOCATION) in
#define VIN_NORMAL layout (location = NORMAL_LOCATION) in
#define VIN_TANGENT layout (location = TANGENT_LOCATION) in
#define VIN_COLOR layout (location = COLOR_LOCATION) in

#define VIN_TEXCOORD0 layout (location = TEXCOORD0_LOCATION) in
#define VIN_TEXCOORD1 layout (location = TEXCOORD1_LOCATION) in
#define VIN_TEXCOORD2 layout (location = TEXCOORD2_LOCATION) in
#define VIN_TEXCOORD3 layout (location = TEXCOORD3_LOCATION) in
#define VIN_TEXCOORD4 layout (location = TEXCOORD4_LOCATION) in
#define VIN_TEXCOORD5 layout (location = TEXCOORD5_LOCATION) in
#define VIN_TEXCOORD6 layout (location = TEXCOORD6_LOCATION) in
#define VIN_TEXCOORD7 layout (location = TEXCOORD7_LOCATION) in

//Transform matrices

uniform mat4 vin_matrix_model;
uniform mat4 vin_matrix_view;
uniform mat4 vin_matrix_projection;
uniform mat4 vin_matrix_mvp;
uniform mat4 vin_matrix_lightspace;

uniform vec3 _WorldSpaceCameraPosition;

#endif
