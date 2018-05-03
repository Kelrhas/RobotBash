#version 420

struct BaseLight
{
    vec3 vColor;
    float fAmbientIntensity;
    float fDiffuseIntensity;
};

struct PointLight
{
	BaseLight oBase;
	vec3 vPosition;
	float fCstAttenuation;
	float fLinAttenuation;
	float fExpAttenuation;
};

struct DirectionalLight
{
    BaseLight oBase;
    vec3 vDirection;
};

in vec2 ex_TexCoord;
in vec4 ex_LightSpacePos;

uniform sampler2D uWorldPos;
uniform sampler2D uAlbedo;
uniform sampler2D uNormal;
uniform sampler2D uUVs;
uniform sampler2D uShadowmap;

//uniform mat4 uCameraWorldMatrix;
uniform mat4 uLightWorldMatrix;
uniform mat4 uLightViewToProjectionMatrix;

uniform vec3 uEyePos;

uniform DirectionalLight uSun;
uniform PointLight uPointLights[16];
uniform int uNbPointLights;

out vec3 out_FragColor;


vec3 vWorldPos;
vec3 vAlbedo;
vec3 vWorldNormal;
vec3 vVertexToEye;

float gMatSpecularIntensity = 1;
float gSpecularPower = 20;


/* TODO use stuffs to remove shadow acne like here
 https://user-images.githubusercontent.com/7088062/36948961-df37690e-1fea-11e8-8999-af8af60403fb.png
	- normal offsetting
	- normal/slope scaled depth bias
*/


float CalcShadowFactor(vec4 LightSpacePos)
{
    LightSpacePos /= LightSpacePos.w;
	LightSpacePos.xyz = LightSpacePos.xyz * vec3(0.5f, 0.5f, 0.5f) + vec3(0.5f, 0.5f, 0.5f);
	
	float cosTheta = clamp(dot(vWorldNormal, uSun.vDirection), 0.0f, 1.0f);
	float bias = 0.005f*tan(acos(cosTheta)); // cosTheta is dot( n,l ), clamped between 0 and 1
	bias = clamp(bias, 0.0f, 0.01f);
    float Depth = texture(uShadowmap, LightSpacePos.xy).z;
    if (Depth < (LightSpacePos.z - bias))
        return 0.03;
    else
        return 1.0;
}

vec3 CalcLightInternal(BaseLight oLight, vec3 vLightDirection, float fShadowFactor)
{
    vec3 vAmbientColor = oLight.vColor * oLight.fAmbientIntensity;
    float fDiffuseFactor = dot(vWorldNormal, -vLightDirection);

    vec3 vDiffuseColor = vec3(0);
    vec3 vSpecularColor = vec3(0);

    if( fDiffuseFactor > 0 )
	{
        vDiffuseColor = oLight.vColor * oLight.fDiffuseIntensity * fDiffuseFactor;
        vec3 vVertexToEye = normalize(uEyePos - vWorldPos);
        vec3 vLightReflect = normalize(reflect(vLightDirection, vWorldNormal));
        float fSpecularFactor = dot(vVertexToEye, vLightReflect);
        if( fSpecularFactor > 0 )
		{
            fSpecularFactor = pow(fSpecularFactor, gSpecularPower);
            vSpecularColor = oLight.vColor * gMatSpecularIntensity * fSpecularFactor;
        }
    }

    return vAmbientColor + fShadowFactor * (vDiffuseColor + vSpecularColor);
} 

vec3 CalcDirectionalLight(DirectionalLight oLight, float fShadow)
{
	return CalcLightInternal(oLight.oBase, oLight.vDirection, fShadow);
}

vec3 CalcPointLight(int Index)
{
    vec3 vLightDirection = vWorldPos - uPointLights[Index].vPosition;
    float fDistance = length(vLightDirection);
    vLightDirection = normalize(vLightDirection);

    vec3 vColor = CalcLightInternal(uPointLights[Index].oBase, vLightDirection, 1.f);
    float fAttenuation = uPointLights[Index].fCstAttenuation +
                        uPointLights[Index].fLinAttenuation * fDistance +
                        uPointLights[Index].fExpAttenuation * fDistance * fDistance;

    return vColor / fAttenuation;
} 


void main()
{
	vWorldPos = texture(uWorldPos, ex_TexCoord).rgb;
	vAlbedo = texture(uAlbedo, ex_TexCoord).rgb;
	vWorldNormal = texture(uNormal, ex_TexCoord).rgb;
	vVertexToEye = normalize(uEyePos - vWorldPos);
	
	vec4 worldSpace = vec4(vWorldPos + vWorldNormal * 0.01, 1.0);
    mat4 cameraViewToProjectedLightSpace = uLightViewToProjectionMatrix * uLightWorldMatrix;
	
	vec4 projectedEyeDir = cameraViewToProjectedLightSpace * worldSpace;
	float fShadowFactor = CalcShadowFactor(projectedEyeDir);
	
	vec3 vTotalColor = CalcDirectionalLight(uSun, fShadowFactor);
	//vTotalColor += CalcPointLight(0);
	
	for(int i=0; i < uNbPointLights; ++i)
	{
		vTotalColor += CalcPointLight(i);
	}
	
	
	out_FragColor = vAlbedo * vTotalColor;
}