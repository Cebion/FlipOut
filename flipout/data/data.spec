/* $Id$ */

for_each "\\.png$"
{
	exclude "^gui\\.png$";
	exclude "^grunge-.*\\.png$";
	write
	{
		texture "$filename"
		{
			min_filter LINEAR;
			image_data "$content";
		};
	};
};

for_each "\\.ogg$"
{
	write
	{
		sound "$filename"
		{
			sound_data "$content";
		};
	};
};

for_each "\\.mesh$"
{
	write
	{
		mesh "$filename"
		{
			_content;
		};
	};
};

for_each "\\.shader$"
{
	write
	{
		shader "$filename"
		{
			_content;
		};
	};
};

file "0.data";
file "awards.data";
file "backgrounds.data";
file "balls.data";
file "bonuses.data";
file "bricks.data";
file "glow.data";
file "paddles.data";

for_each "\\.level$"
{
	write
	{
		level "$filename"
		{
			_content;
		};
	};
};

file "series.data";

for_each "^gui\\.png$" "^grunge-.*\\.png$"
{
	write
	{
		ui_resources
		{
			texture "$filename"
			{
				min_filter LINEAR;
				image_data "$content";
			};
		};
	};
};

for_each "\\.font$"
{
	write
	{
		ui_resources
		{
			font "$filename"
			{
				_content;
			};
		};
	};
};

file "gui.data";
