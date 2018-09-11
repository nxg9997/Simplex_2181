#version 330

in vec3 Color;

uniform vec3 SolidColor = vec3(-1,-1,-1);
uniform vec3 Compliment = vec3(-1,-1,-1);
uniform bool useComp = false;

out vec4 Fragment;

void main()
{
	Fragment = vec4(Color,1);
	
	if(useComp){
		if(Compliment.r != -1.0 && Compliment.g != -1.0 && Compliment.b != -1.0)
			Fragment = vec4(Compliment, 1);
		return;
	}
	
	if(SolidColor.r != -1.0 && SolidColor.g != -1.0 && SolidColor.b != -1.0)
		Fragment = vec4(SolidColor, 1);

	return;
}