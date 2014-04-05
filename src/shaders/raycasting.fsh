#define OP_THRESHOLD 0.95
#define MIN_COORD_POS 0.0
#define MAX_COORD_POS 1.0

uniform sampler3D volume;
uniform sampler1D tf;
uniform float voxelSize;

void main()
{
        vec4  result=vec4(0.0, 0.0, 0.0, 0.0);
        vec4  col=vec4(0.0, 0.0, 0.0, 0.0);

        vec4 posRay  = gl_TexCoord[0];
        vec4 view  = normalize(gl_TexCoord[0] - gl_ModelViewMatrixInverse * vec4(0.0, 0.0, 0.0, 1.0));
        view.w = 0.0;

        vec3 minTexCoord = vec3(MIN_COORD_POS);
        vec3 maxTexCoord = vec3(MAX_COORD_POS);

        float ray_step = voxelSize * 1.5;
        float op_step  = voxelSize * 1.5;

        while (result.a<=OP_THRESHOLD)
        {
            posRay += voxelSize * view;
            vec3 signos = (maxTexCoord-posRay.xyz)*(posRay.xyz-minTexCoord);
            if (signos[0]>=0.0 && signos[1]>=0.0 && signos[2]>=0.0)
                col=clamp(texture1D(tf, texture3D(volume, posRay.xyz).r), 0.0, 1.0);
            else
                break;

            col.a *= op_step;
            col.rgb *= col.a;
            result = (1.0 - result.a)*col + result;
         }

        if(result.a<(1.0-OP_THRESHOLD)) discard;
        gl_FragColor = clamp(result, 0.0, 1.0);
        gl_FragDepth = gl_FragCoord.z;
}
