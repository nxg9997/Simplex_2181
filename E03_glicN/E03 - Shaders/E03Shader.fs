#version 330

in vec3 Color;

uniform vec3 SolidColor = vec3(-1,-1,-1);
//uniform vec3 Compliment = vec3(-1,-1,-1);
uniform bool useComp = false;

out vec4 Fragment;

void main()
{
	Fragment = vec4(Color,1);
	
	if(useComp){
		vec3 comp;
		if(SolidColor.r != -1.0 && SolidColor.g != -1.0 && SolidColor.b != -1.0){
			comp = vec3(1.0 - SolidColor.r, 1.0 - SolidColor.g, 1.0 - SolidColor.b);
		}
		else{
			comp = vec3(1.0 - Color.r, 1.0 - Color.g, 1.0 - Color.b);
		}
		Fragment = vec4(comp, 1);
	}
	
	else{
		if(SolidColor.r != -1.0 && SolidColor.g != -1.0 && SolidColor.b != -1.0)
			Fragment = vec4(SolidColor, 1);
	}
	return;
}