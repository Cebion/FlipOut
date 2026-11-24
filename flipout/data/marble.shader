vertex_shader "
	uniform vec4 random;

	varying vec3 norm;
	varying vec2 noise_coords[2];
	varying float stripe_pos;

	void main()
	{
		gl_Position=ftransform();

		noise_coords[0]=(gl_Vertex.xy+gl_Vertex.yz+random.yz)*0.05;
		noise_coords[1]=(gl_Vertex.zx-gl_Vertex.xy+random.zy)*0.2;
		stripe_pos=dot(gl_Vertex.xyz, vec3(10.0, 5.0*(random.x-0.5), -2.0))+random.w*6.28;
		norm=(gl_ModelViewMatrix*vec4(gl_Normal, 0.0)).xyz;

		vec4 eye_pos=gl_ModelViewMatrix*gl_Vertex;
		gl_TexCoord[2]=vec4(dot(gl_EyePlaneS[2], eye_pos), dot(gl_EyePlaneT[2], eye_pos), dot(gl_EyePlaneR[2], eye_pos), 0.0);
	}
";

fragment_shader "
	uniform sampler2D noise;

	varying vec3 norm;
	varying vec2 noise_coords[2];
	varying float stripe_pos;

	uniform sampler2DShadow shadow;
	uniform bool use_shadow;

	void main()
	{
		float f=clamp(dot(
				vec4(cos(stripe_pos),
					texture2D(noise, noise_coords[0]).r,
					texture2D(noise, noise_coords[1]).r,
					1),
				vec4(0.5, 1.2, 0.6, 0.25)),
			0.7, 1.0);
		vec3 diffuse_color=gl_FrontMaterial.diffuse.rgb*f;

		float diffuse_value=max(dot(norm, gl_LightSource[0].position.xyz), 0.0);

		gl_FragColor=vec4(gl_FrontMaterial.ambient.rgb*gl_LightModel.ambient.rgb
				+diffuse_color*gl_LightSource[0].diffuse.rgb*diffuse_value
			, gl_FrontMaterial.diffuse.a);
		if(use_shadow)
			gl_FragColor*=shadow2D(shadow, gl_TexCoord[2].xyz);
	}
";
