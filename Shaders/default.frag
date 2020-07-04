#version 450 core

layout(location=0) in vec3 vClrCoord;
layout(location=1) in vec2 vTexCoord;

layout (location=0) out vec4 fFragClr;

uniform int task;
uniform int modulate;
uniform float size;
uniform sampler2D uTex2d;

void main () {
    switch (task)
    {
        case 0:
            fFragClr = vec4(vClrCoord, 1.0);
            break;

        case 1:
            
            vec3 color;

            if (modulate == 1)
            {
                if ((int(gl_FragCoord.x / 32) + int(gl_FragCoord.y / 32)) % 2 == 1)
                {
                    color = vec3(0.7 * vClrCoord.x, 0.5 * vClrCoord.y, 0.7 * vClrCoord);
                    fFragClr = vec4(color, 1.0);
                }

                else
                {
                    fFragClr = vec4(vClrCoord, 1.0);
                }
            }
            
            else
            {
                if ((int(gl_FragCoord.x / 32) + int(gl_FragCoord.y / 32)) % 2 == 1)
                {
                    fFragClr = vec4(0.0, 0.68, 0.94, 1.0);
                }

                else
                {
                    fFragClr = vec4(1.0, 0.0, 1.0, 1.0);
                }
            }

            break;

        case 2:
            
            if (modulate == 1)
            {
                if ((int(gl_FragCoord.x / size) + int(gl_FragCoord.y / size)) % 2 == 1)
                {
                    color = vec3(0.7 * vClrCoord.x, 0.5 * vClrCoord.y, 0.7 * vClrCoord);
                    fFragClr = vec4(color, 1.0);
                }

                else
                {
                    fFragClr = vec4(vClrCoord, 1.0);
                }
            }
            
            else
            {
                if ((int(gl_FragCoord.x / size) + int(gl_FragCoord.y / size)) % 2 == 1)
                {
                    fFragClr = vec4(0.0, 0.68, 0.94, 1.0);
                }

                else
                {
                    fFragClr = vec4(1.0, 0.0, 1.0, 1.0);
                }
            }

            break;

        default:
        
            if (modulate == 1)
            {
                fFragClr = texture(uTex2d, vTexCoord) * vec4(vClrCoord, 1.0);
            }

            else
            {
                fFragClr = texture(uTex2d, vTexCoord);
            }
            
            break;
    }
}
