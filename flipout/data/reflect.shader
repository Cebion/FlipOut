vertex_shader "
	varying vec4 screen_pos;

	void main()
	{
		gl_Position=ftransform();
		screen_pos=gl_Position;
		gl_TexCoord[0]=gl_MultiTexCoord0;
		gl_FrontColor=gl_Color*(gl_LightModel.ambient+dot(gl_Normal, gl_LightSource[0].position.xyz)*gl_LightSource[0].diffuse);

		vec4 eye_pos=gl_ModelViewMatrix*gl_Vertex;
		gl_TexCoord[2]=vec4(dot(gl_EyePlaneS[2], eye_pos), dot(gl_EyePlaneT[2], eye_pos), dot(gl_EyePlaneR[2], eye_pos), 0.0);
	}
";

fragment_shader "
	uniform sampler2D texture;
	uniform sampler2D refl_tex;
	uniform float refl_str;
	varying vec4 screen_pos;

	uniform sampler2DShadow shadow;
	uniform bool use_shadow;

	void main()
	{
		gl_FragColor=gl_Color*(texture2D(texture, gl_TexCoord[0].xy)
			+texture2D(refl_tex, screen_pos.xy/screen_pos.w*0.5+0.5)*refl_str);
		if(use_shadow)
			gl_FragColor*=shadow2D(shadow, gl_TexCoord[2].xyz);
	}
";
