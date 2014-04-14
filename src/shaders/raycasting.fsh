#define OP_THRESHOLD 0.95
#define MIN_COORD_POS 0.0
#define MAX_COORD_POS 1.0

uniform sampler3D volume;
uniform sampler1D tf;
uniform float voxelSize;

// Lighting Stuff
const vec3 LightPosition = vec3(0.0, 1.0, 1.0);
const float SpecularContribution = 0.1;
const float DiffuseContribution = 1.0 - SpecularContribution;
vec3 ec;

// Compute the Normal around the current voxel
vec3 getNormal(vec3 at)
{
    vec3 n = vec3(texture3D(volume, at - vec3(voxelSize, 0.0, 0.0)).w - texture3D(volume, at + vec3(voxelSize, 0.0, 0.0)).w,
                  texture3D(volume, at - vec3(0.0, voxelSize, 0.0)).w - texture3D(volume, at + vec3(0.0, voxelSize, 0.0)).w,
                  texture3D(volume, at - vec3(0.0, 0.0, voxelSize)).w - texture3D(volume, at + vec3(0.0, 0.0, voxelSize)).w
                 );

    return normalize(n);
}

float getLightIntensity(vec3 norm)
{
    float LightIntensity = 1.0;
    vec3 lightVec = normalize(LightPosition - ec);
    vec3 viewVec = normalize(-ec);

    vec3 reflectVec = reflect(-lightVec, norm);
    float diffuse = max(dot(lightVec, norm), 0.3);
    float spec = 0.0;

    if (diffuse > 0.0)
    {
        spec = max(dot(reflectVec, viewVec), 0.2);
        spec = pow(spec, 16.0);
    }

    return SpecularContribution*spec + DiffuseContribution*diffuse;
}

void main()
{
        vec4  result=vec4(0.0, 0.0, 0.0, 0.0);
        vec4  col=vec4(0.0, 0.0, 0.0, 0.0);

        vec4 posRay  = gl_TexCoord[0];
        vec4 view  = normalize(gl_TexCoord[0] - gl_ModelViewMatrixInverse * vec4(0.0, 0.0, 0.0, 1.0));
        view.w = 0.0;

        ec = view.xyz;

        vec3 minTexCoord = vec3(MIN_COORD_POS);
        vec3 maxTexCoord = vec3(MAX_COORD_POS);

        view *= voxelSize;
        float LightIntensity = 1.0;

        while (result.a<=OP_THRESHOLD)
        {
            posRay += view;

            vec3 signos = (maxTexCoord-posRay.xyz)*(posRay.xyz-minTexCoord);
            if (signos[0]>=0.0 && signos[1]>=0.0 && signos[2]>=0.0)
                col=clamp(texture1D(tf, texture3D(volume, posRay.xyz).r), 0.0, 1.0);
            else
                break;

            LightIntensity = getLightIntensity(getNormal(posRay));

            // Accumulate RGB : acc.rgb = voxelColor.rgb*voxelColor.a + (1.0 - voxelColor.a)*acc.rgb;
            result.rgb = mix(result.rgb, col.rgb, col.a)*LightIntensity;

            // Accumulate Opacity: acc.a = acc.a + (1.0 - acc.a)*voxelColor.a;
            result.a = mix(col.a, 1.0, result.a);

/*
            col.a *= op_step;
            col.rgb *= col.a;
            result = (1.0 - result.a)*col + result;*/
         }

        if(result.a<(1.0-OP_THRESHOLD)) discard;
        gl_FragColor = clamp(result+vec4(0.2,0.2,0.2,0.0), 0.0, 1.0);
        gl_FragDepth = gl_FragCoord.z;
}
