#version 330 core

in vec3 ex_Normal;
in vec2 ex_UV;

out vec4 ColorOut;

uniform int uTarget;

uniform sampler2D uAlbedo;
uniform sampler2D uNormal;
uniform sampler2D uDepth;
uniform sampler2D uFinalLit;

uniform float uNearPlane;
uniform float uFarPlane;

void main()
{
	if(uTarget == 0) // multiple (albedo + depth + normal + final)
	{
		vec2 uv = ex_UV;
		if(ex_UV.x < 0.5)
		{
			uv.x *= 2;
			if(ex_UV.y < 0.5)
			{
				uv.y *= 2;
				// convert to linear values 
				float depth = texture(uDepth, uv).r;
				float c = (2.0 * uNearPlane) / (uFarPlane + uNearPlane - depth * (uFarPlane - uNearPlane));
				ColorOut = vec4(c, c, c, 1);
				//ColorOut = vec4(depth, depth, depth, 1);
			}
			else
			{
				uv.y = uv.y * 2 - 1;
				ColorOut = vec4(texture(uAlbedo, uv).rgb, 1);
			}
		}
		else
		{
			uv.x = uv.x * 2 - 1;
			if(ex_UV.y < 0.5)
			{
				uv.y *= 2;
				ColorOut = vec4(texture(uFinalLit, uv).rgb, 1);
			}
			else
			{
				uv.y = uv.y * 2 - 1;
				ColorOut = vec4(texture(uNormal, uv).rgb, 1);

			}
		}
	}
	else if(uTarget == 1) // anything
	{
		ColorOut = vec4(texture(uAlbedo, ex_UV).rgb, 1);
		//ColorOut = vec4(texture(uAlbedo, ex_UV).rg, 0, 1);
	}
	else if(uTarget == 2) // depth
	{
		// convert to linear values 
		float depth = texture(uDepth, ex_UV).r;
		float c = (2.0 * uNearPlane) / (uFarPlane + uNearPlane - depth * (uFarPlane - uNearPlane));
		ColorOut = vec4(c, c, c, 1);
	}
	else // directionnal shadowmap
	{
		float depth = texture(uDepth, ex_UV).r;
		float c = depth;
		ColorOut = vec4(c, c, c, 1);
	}
} 