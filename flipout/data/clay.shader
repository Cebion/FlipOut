vertex_shader "
	uniform vec4 random;

	varying vec3 norm;
	varying vec2 noise_coords;

	void main()
	{
		gl_Position=ftransform();

		noise_coords=gl_Vertex.xy+gl_Vertex.zz*0.02+random.yz;
		norm=(gl_ModelViewMatrix*vec4(gl_Normal, 0.0)).xyz;

		vec4 eye_pos=gl_ModelViewMatrix*gl_Vertex;
		gl_TexCoord[2]=vec4(dot(gl_EyePlaneS[2], eye_pos), dot(gl_EyePlaneT[2], eye_pos), dot(gl_EyePlaneR[2], eye_pos), 0.0);
	}
";

fragment_shader "
	uniform sampler2D noise;

	varying vec3 norm;
	varying vec2 noise_coords;

	uniform sampler2DShadow shadow;
	uniform bool use_shadow;

	void main()
	{
		float f=texture2D(noise, noise_coords).r*0.12+0.88;
		vec3 diffuse_color=gl_FrontMaterial.diffuse.rgb*f;

		float diffuse_value=max(dot(norm, gl_LightSource[0].position.xyz), 0.0);

		gl_FragColor=vec4(gl_FrontMaterial.ambient.rgb*gl_LightModel.ambient.rgb*f
				+diffuse_color*gl_LightSource[0].diffuse.rgb*diffuse_value
			, gl_FrontMaterial.diffuse.a);
		if(use_shadow)
			gl_FragColor*=shadow2D(shadow, gl_TexCoord[2].xyz);
	}
";
