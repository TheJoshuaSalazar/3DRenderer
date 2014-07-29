#version 400

uniform vec3 Ambient;
uniform vec3 LightColor;
uniform float Shininess;
uniform float Strength;
uniform vec3 EyeDirection;
uniform vec3 eyeWorldPosition;

uniform vec3 materialAmbient;

uniform vec3 LightPosition; //WorldSpace
uniform float specularOn;

in vec4 Color;
in vec3 Normal;
in vec2 TexCoord;
in vec4 Position;
in mat3 TBN;

uniform sampler2D shadowMap;
in vec4 ShadowCoord;

out vec4 FragColor;

uniform sampler2D NormalMap;
uniform sampler2D DiffuseMap;

uniform bool doNormal;
uniform bool doDiffuse;
uniform bool doShadows;

void main()
{
	vec3 textureNormal = normalize(texture2D(NormalMap, vec2(TexCoord.x, -TexCoord.y)).rgb * 2.0 - 1);
	vec3 shapeNormal = TBN * textureNormal;
	vec4 texColor = texture(NormalMap, TexCoord);
	vec4 diffuseColor = texture(DiffuseMap, TexCoord);

	float cosTheta = clamp( dot( Normal, normalize(LightPosition) ), 0,1 );
	float bias = 0.005 * tan(acos(cosTheta));
	bias = clamp(bias, 0.0, 0.01);
	float visibility = 1.0;

	if(texture(shadowMap, ShadowCoord.xy).z < ShadowCoord.z - bias)
	{
			visibility = 0.6;
	}
	
	vec3 lightVector = normalize(LightPosition - vec3(Position));
	float diffuseBrightness = dot(shapeNormal, lightVector);
	vec4 diffuseLight = vec4(vec3(diffuseBrightness),1);
	
	vec3 lightReflectedVector = reflect(-lightVector , shapeNormal);
	vec3 eyeVector = normalize(eyeWorldPosition - vec3(Position));
	float eyeAndReflectedAligmentAmount = dot(lightReflectedVector, eyeVector);
	eyeAndReflectedAligmentAmount = pow(eyeAndReflectedAligmentAmount, 10);  
	
	vec4 specular = vec4(eyeAndReflectedAligmentAmount);
	
	specular = clamp(specular, 0, 1);
	
	if(specularOn == 0)
		specular = vec4(0);

	if(doNormal)
	 {
		FragColor = (specular * visibility +
		 (vec4(Ambient,1) * vec4(materialAmbient,1)) +
		 (diffuseLight)) * visibility * vec4(LightColor,0.5) ;
	 }
	 else
	 {
		FragColor = vec4(vec3(0.4 * visibility), 1);
	 }

	 if(doDiffuse && doNormal)	
	 	FragColor *= diffuseColor;
	 else if(doDiffuse)
	 {
		vec3 lightVector = normalize(LightPosition - vec3(Position));
		float diffuseBrightness = dot(Normal, lightVector);
		vec4 diffuseLight = vec4(diffuseBrightness, diffuseBrightness, diffuseBrightness,1);
		
		vec3 lightReflectedVector = reflect(-lightVector , Normal);
		vec3 eyeVector = normalize(eyeWorldPosition - vec3(Position));
		float eyeAndReflectedAligmentAmount = dot(lightReflectedVector, eyeVector);
		eyeAndReflectedAligmentAmount = pow(eyeAndReflectedAligmentAmount, 10);  
		
		vec4 specular = vec4(eyeAndReflectedAligmentAmount);
		
		specular = clamp(specular, 0, 1);
		
		if(specularOn == 0)
			specular = vec4(0);

		FragColor = (specular * visibility +
			(vec4(Ambient,1) * vec4(materialAmbient,1))  +
			(diffuseLight)) * visibility * vec4(LightColor,0.5) ;

		FragColor *= diffuseColor;
	}
}