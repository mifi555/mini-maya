#version 150

uniform mat4[100] bindMats;
uniform mat4[100] overallTransforms;
uniform mat4 u_ViewProj;

in vec4 vs_Pos;
in vec4 vs_Nor;
in vec4 vs_Col;
in ivec2 jointIDs;
in vec2 jointWeights;

out vec3 fs_Pos;
out vec4 fs_Nor;
out vec4 fs_Col;

mat3 calcInverseTranspose(mat3 mat)
{
    float det = mat[0][0] * (mat[1][1] * mat[2][2] - mat[2][1] * mat[1][2])
              - mat[1][0] * (mat[0][1] * mat[2][2] - mat[2][1] * mat[0][2])
              + mat[2][0] * (mat[0][1] * mat[1][2] - mat[1][1] * mat[0][2]);

    vec3 row1 = vec3((mat[1][1] * mat[2][2] - mat[2][1] * mat[1][2]),
                     (mat[2][0] * mat[1][2] - mat[1][0] * mat[2][2]),
                     (mat[1][0] * mat[2][1] - mat[2][0] * mat[1][1]));

    vec3 row2 = vec3((mat[2][1] * mat[0][2] - mat[0][1] * mat[2][2]),
                     (mat[0][0] * mat[2][2] - mat[2][0] * mat[0][2]),
                     (mat[2][0] * mat[0][1] - mat[0][0] * mat[2][1]));

    vec3 row3 = vec3((mat[0][1] * mat[1][2] - mat[1][1] * mat[0][2]),
                     (mat[1][0] * mat[0][2] - mat[0][0] * mat[1][2]),
                     (mat[0][0] * mat[1][1] - mat[1][0] * mat[0][1]));

    mat3 inverted = mat3(row1, row2, row3) / det;
    return inverted;
}


void main()
{
    fs_Col = vs_Col;

    mat4 bindMat1 = bindMats[jointIDs[0]];
    mat4 bindMat2 = bindMats[jointIDs[1]];

    mat4 transform1 = overallTransforms[jointIDs[0]];
    mat4 transform2 = overallTransforms[jointIDs[1]];

    vec4 joint1WorldPosition = transform1 * bindMat1 * vs_Pos;
    vec4 joint2WorldPosition = transform2 * bindMat2 * vs_Pos;

    vec4 finalPos = jointWeights[0] * joint1WorldPosition + jointWeights[1] * joint2WorldPosition;
    fs_Pos = vec3(finalPos);

    vec3 normal1 = normalize(calcInverseTranspose(mat3(transform1)) * mat3(bindMat1) * vs_Nor.xyz);
    vec3 normal2 = normalize(calcInverseTranspose(mat3(transform2)) * mat3(bindMat2) * vs_Nor.xyz);

    fs_Nor = vec4(mix(normal1, normal2, jointWeights[0]), 1);

    gl_Position = u_ViewProj * finalPos;
}
