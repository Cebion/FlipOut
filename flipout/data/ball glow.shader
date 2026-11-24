vertex_shader "
	varying vec3 ncoords[2];

	void main()
	{
		gl_Position=ftransform();

		ncoords[0]=gl_Vertex.xyz*0.1;
		ncoords[1]=gl_Vertex.xyz*0.3;
	}
";

fragment_shader "
	uniform sampler3D noise;

	varying vec3 ncoords[2];

	void main()
	{
		float f=step(0.2, abs(0.7-dot(
				vec2(texture3D(noise, ncoords[0]).r,
					texture3D(noise, ncoords[1]).r),
				vec2(0.8, 0.2))));

		vec3 emission_color=mix(gl_FrontMaterial.diffuse.rgb, vec3(0.0, 0.0, 0.0), f);

		gl_FragColor=vec4(emission_color, 1.0);
	}
";

