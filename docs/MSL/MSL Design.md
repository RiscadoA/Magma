# Magma Shading Language Design

Version 1.0.0;

Magma shaders have:

- [OPTIONAL] Textures;
- [OPTIONAL] Constant buffers;
- [OPTIONAL] Functions - Not yet supported;
- Vertex shader output variables;
- A vertex shader function;
- A pixel shader function;

## Variable Types

| Type  | Format     | Components (Color) | Components (Axis) |
| ----- | ---------- | ------------------ | ----------------- |
| int   | F32        | R                  | X                 |
| float | F32        | R                  | X                 |
| vec2  | F32[2]     | RG                 | XY                |
| vec3  | F32[3]     | RGB                | XYZ               |
| vec4  | F32[4]     | RGBA               | XYZW              |
| ivec2 | I32[2]     | RG                 | XY                |
| ivec3 | I32[3]     | RGB                | XYZ               |
| ivec4 | I32[4]     | RGBA               | XYZW              |
| mat2  | F32[2]\[2] |                    |                   |
| mat3  | F32[3]\[3] |                    |                   |
| mat4  | F32[4]\[4] |                    |                   |

## Comments

A comment line is initiated by "//".

Comment examples:

```
// This is a test comment. This will be ignored by the compiler.

vec4 vec = vec4(1.0f, 1.0f, 1.0f, 1.0f); // Another comment.
```

Multi line comments are started by "/\*" and ended by "\*/". Example:

```
/*
Multi line comment.
All of this text will be ignored by the compiler.
*/

int x = 9; /* Sample text */ int y = 0;
```

## Textures

Textures are parameters defined in the global scope as:

```
Texture2D textureName;
```
### Methods

```
texture.Sample(vec2 textureCoords);
```

Samples a pixel from the texture on the UV coordinates `vec2 textureCoords`.

## Constant Buffers

Constant buffers contain variables that can be set by the program.

They are defined in the global scope as:

```
Constant constantBufferName
{
	mat4x4 matrix;
	vec4 vector;
}
```

Their variables can be accessed as:

```
float4 x = constantBufferName.vector;
```

## Functions

Not yet implemented.

## Vertex Output

Used to send data from the vertex shader function to the pixel shader function.

The variables are declared on the global scope as:

```
VertexOutput vertexOut
{
	vec2 uvs;
	vec4 color;
}
```

They can be set by the vertex shader function as:

```
vertexOut.uvs = uvs;
vertexOut.color = vec4(1.0f, 1.0f, 1.0f, 1.0f);
```

And can be read by the pixel shader function as:

```
vec4 pixelColor = vertexOut.color;
```

## Vertex Shader Function

This function is the entry point to the vertex shader pipeline stage.

A vertex shader function is defined as:

```
vec4 VertexShader([PARAMS])
{
    // Calculate vertex position on screen and return it.
    return [position];
}
```

The function takes as parameters the vertex attributes and returns a vec4 containing the position of the vertex on the screen in normalized coordinates.

Example:

```
vec4 VertexShader(vec2 screenPosition)
{
    return vec4(screenPosition, 0.0f, 1.0f);
}
```

## Pixel Shader Function

This function is the entry point to the pixel shader pipeline stage.

A pixel shader function is defined as:

```
vec4 PixelShader()
{
	// Calculate pixel color and return it.
	return [pixel color];
}
```

It takes no parameters and returns the color of the pixel.

Example:

```
VertexOutput vertexOut
{
    vec4 color;
}

vec4 PixelShader()
{
    return vertexOut.color;
}
```

## Preprocessor directives

````
#msl version 1.0.0
````



This preprocessor directive is obligatory in every Magma shader header.

It is used to check if the shader version is compatible with the program compiling it.

```
#define MAX_BONES 64
```

 This preprocessor directive will store the value defined in front of it (64) and replace every occurrence of the string "MAX_BONES" with its value (64).

## Example Shader

```
#msl version 1.0.0

Texture2D texture;

Constant cbTransform
{
    mat4x4 mvp;
}

VertexOutput vertexOut
{
    vec2 uvs;
}

vec4 VertexShader(vec3 position, vec2 uvs)
{
	vertexOut.uvs = uvs;
	return mvp * vec4(position, 1.0f);
}

vec4 PixelShader()
{
	return texture.Sample(vertexOut.uvs);
}
```

## Math functions

