uniform vec2 pixelSize;
uniform sampler2D texture;
uniform int isVertical;

void main()
{
	vec2 pos=gl_FragCoord.xy*pixelSize;
	
	// 1 2 1
	// 2 3 2  / 15.0
	// 1 2 1
	
	float values[9];
	values[0]=0.05;
	values[1]=0.09;
	values[2]=0.11;
	values[3]=0.15;
	values[4]=0.2;
	values[5]=0.15;
	values[6]=0.11;
	values[7]=0.09;
	values[8]=0.05;
	vec4 result;
	if(isVertical==1)
	{
		vec2 curSamplePos=vec2(pos.x,pos.y-4.0*pixelSize.y);
		for(int i=0;i<9;i++)
		{
			result+=texture2D(texture,curSamplePos)*values[i];
			curSamplePos.y+=pixelSize.y;
		}
	}else{
		vec2 curSamplePos=vec2(pos.x-4.0*pixelSize.x,pos.y);
		for(int i=0;i<9;i++)
		{
			result+=texture2D(texture,curSamplePos)*values[i];
			curSamplePos.x+=pixelSize.x;
		}		
	}	
	gl_FragColor=vec4(result.xyz,1.0);
}
