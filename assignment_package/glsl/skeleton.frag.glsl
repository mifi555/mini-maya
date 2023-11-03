#version 150

uniform vec3 u_CamPos;

in vec3 fs_Pos;
in vec4 fs_Nor;
in vec4 fs_Col;

out vec3 out_Col;

void main()
{
        // Material base color (before shading)
        vec4 diffuseColor = fs_Col;
        // Calculate the diffuse term for Lambert shading
        vec3 lightVec = normalize(u_CamPos - fs_Pos);
        float diffuseTerm = dot(normalize(fs_Nor.xyz), normalize(lightVec));
        // Clamp values
        diffuseTerm = clamp(diffuseTerm, 0, 1);
        float ambientTerm = 0.2;
        float lightIntensity = diffuseTerm + ambientTerm;
        out_Col = vec3(diffuseColor.rgb * lightIntensity);
}
