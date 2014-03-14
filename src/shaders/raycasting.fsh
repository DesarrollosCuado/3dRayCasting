#define OP_THRESHOLD 0.95
#define OP_THRESHOLD 0.95
#define RAT_STEP 0.05
#define MIN_COORD_POS 0.0
#define MAX_COORD_POS 1.0

uniform sampler3D volume;
uniform sampler1D tf;

void main()
{
        vec4  result=vec4(0.0, 0.0, 0.0, 1.0);
        vec4  col=vec4(0.0, 0.0, 0.0, 0.0);

        float opacity = 0.0;
        vec4 posRay  = gl_TexCoord[0];
        vec4 view  = normalize(gl_TexCoord[0] - gl_ModelViewMatrixInverse * vec4(0.0, 0.0, 0.0, 1.0));
        view.w = 0.0;

        //gl_FragColor = clamp(vec4(posRay.xyz, 1.0), 0.0, 1.0);
        //gl_FragDepth = gl_FragCoord.z;
        //return;

        vec3 minTexCoord = vec3(MIN_COORD_POS);
        vec3 maxTexCoord = vec3(MAX_COORD_POS);

        float ray_step_acel=RAT_STEP;

        while (opacity<=OP_THRESHOLD)
        {
            posRay += ray_step_acel * view;
            vec3 signos = (maxTexCoord-posRay.xyz)*(posRay.xyz-minTexCoord);
            if (signos[0]>=0.0 && signos[1]>=0.0 && signos[2]>=0.0)
                col=clamp(texture1D(tf, texture3D(volume, posRay.xyz).r), 0.0, 1.0);
            else
                break;

            col.a       *= ray_step_acel;
            result.rgb  += result.a * col.rgb;
            result.a    *= 1.0-col.a;
            opacity     += col.a*(1.0-opacity);
        }

        result.a = 1.0 - result.a;
        if(result.a<(1.0-OP_THRESHOLD)) discard; //Fragmento demasiado transparente

        gl_FragColor = clamp(vec4(result.xyz*result.a, 1.0), 0.0, 1.0);
        gl_FragDepth = gl_FragCoord.z;
}
