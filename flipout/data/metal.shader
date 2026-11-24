vertex_shader "
	varying vec3 cam_dir;
	varying vec3 norm;

	void main()
	{
		gl_Position=ftransform();

		cam_dir=-normalize((gl_ModelViewMatrix*gl_Vertex).xyz);
		norm=(gl_ModelViewMatrix*vec4(gl_Normal, 0)).xyz;

		vec4 eye_pos=gl_ModelViewMatrix*gl_Vertex;
		gl_TexCoord[2]=vec4(dot(gl_EyePlaneS[2], eye_pos), dot(gl_EyePlaneT[2], eye_pos), dot(gl_EyePlaneR[2], eye_pos), 0.0);
	}
";

fragment_shader "
	varying vec3 cam_dir;
	varying vec3 norm;

	uniform sampler2DShadow shadow;
	uniform bool use_shadow;

	void main()
	{
		vec3 nnorm=normalize(norm);
		vec3 half_vec=normalize(gl_LightSource[0].position.xyz+cam_dir);

		float diffuse_value=max(dot(nnorm, gl_LightSource[0].position.xyz), 0.0);
		float specular_value=pow(max(dot(nnorm, half_vec), 0.0), 30.0);

		gl_FragColor=vec4(gl_FrontMaterial.ambient.rgb*gl_LightModel.ambient.rgb
				+gl_FrontLightProduct[0].diffuse.rgb*diffuse_value
				+gl_FrontLightProduct[0].diffuse.rgb*specular_value*0.5
			, gl_FrontMaterial.diffuse.a);
		if(use_shadow)
			gl_FragColor*=shadow2D(shadow, gl_TexCoord[2].xyz);
	}
";
