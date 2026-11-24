vertex_shader "
	varying vec3 cam_dir;
	varying vec3 norm;
	varying vec3 ncoords[2];

	void main()
	{
		gl_Position=ftransform();

		cam_dir=-normalize((gl_ModelViewMatrix*gl_Vertex).xyz);
		norm=(gl_ModelViewMatrix*vec4(gl_Normal, 0)).xyz;

		ncoords[0]=gl_Vertex.xyz*0.1;
		ncoords[1]=gl_Vertex.xyz*0.3;

		vec4 eye_pos=gl_ModelViewMatrix*gl_Vertex;
		gl_TexCoord[2]=vec4(dot(gl_EyePlaneS[2], eye_pos), dot(gl_EyePlaneT[2], eye_pos), dot(gl_EyePlaneR[2], eye_pos), 0.0);
	}
";

fragment_shader "
	uniform sampler3D noise;

	varying vec3 cam_dir;
	varying vec3 norm;
	varying vec3 ncoords[2];

	uniform sampler2DShadow shadow;
	uniform bool use_shadow;

	void main()
	{
		float f=step(0.2, abs(0.7-dot(
				vec2(texture3D(noise, ncoords[0]).r,
					texture3D(noise, ncoords[1]).r),
				vec2(0.8, 0.2))));

		vec3 nnorm=normalize(norm);
		vec3 half_vec=normalize(gl_LightSource[0].position.xyz+cam_dir);

		vec3 diffuse_color=mix(gl_FrontMaterial.diffuse.rgb, vec3(0.7, 0.7, 0.7), f);
		float diffuse_value=max(dot(nnorm, gl_LightSource[0].position.xyz), 0.0);

		gl_FragColor=vec4(gl_FrontMaterial.ambient.rgb*gl_LightModel.ambient.rgb
				+diffuse_color*gl_LightSource[0].diffuse.rgb*diffuse_value
			, 1.0);
		if(use_shadow)
			gl_FragColor*=shadow2D(shadow, gl_TexCoord[2].xyz);
	}
";
