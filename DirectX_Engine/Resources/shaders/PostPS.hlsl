#include "Sprite.hlsli"




Texture2D<float4> tex0: register(t0);//0�Ԗڂɐݒ肳�ꂽ�e�N�X�`��
Texture2D<float4> tex1 : register(t1);//1�Ԗڂɐݒ肳�ꂽ�e�N�X�`��

SamplerState smp : register(s0);//0�Ԗڂɐݒ肳�ꂽ�T���v���[

float WhiteNoise(float2 coord) {
	return frac(sin(dot(coord, float2(8.7819, 3.255))) * 437.645);
}





float4 main(VSOutput input) : SV_TARGET
{

	//

	
	//float density = 200;
	float2 samplePoint = input.uv;
	float4 colortex0 = tex0.Sample(smp, input.uv);
	
	samplePoint.x -= x;
	samplePoint.y -= y;
	float dist = sqrt(samplePoint.x * samplePoint.x + samplePoint.y * samplePoint.y);
	float ang = atan2(samplePoint.y, samplePoint.x);

	samplePoint.x = x + cos(ang) * (dist * (1.0 + sin(dist * span + frame * 31.4) * scale));
	samplePoint.y = y + sin(ang) * (dist * (1.0 + sin(dist * span + frame * 31.4) * scale));

	float4 Tex1 = tex1.Sample(smp, input.uv);


	Tex1.a = a;

	//float4 color = Tex0;
	//if (fmod(input.uv.y, 0.1f) < 0.05f)
	//{
	//	color = Tex1;
	//}
	//�O���C�X�P�[��
	//float grayScale = Tex0.r * 0.299 +Tex0.g * 0.587 + Tex0 * 0.114;
	//Tex0 = float4(grayScale, grayScale, grayScale, 1);

	/*
	float _ShiftWidth = 0.005;

	colortex0 += tex0.Sample(smp, input.uv + float2(-_ShiftWidth, -_ShiftWidth));
	colortex0 += tex0.Sample(smp, input.uv + float2(0, -_ShiftWidth));
	colortex0 += tex0.Sample(smp, input.uv + float2(_ShiftWidth, -_ShiftWidth));
	colortex0 += tex0.Sample(smp, input.uv + float2(-_ShiftWidth, 0));
	colortex0 += tex0.Sample(smp, input.uv + float2(0, 0));
	colortex0 += tex0.Sample(smp, input.uv + float2(_ShiftWidth, 0));
	colortex0 += tex0.Sample(smp, input.uv + float2(-_ShiftWidth, -_ShiftWidth));
	colortex0 += tex0.Sample(smp, input.uv + float2(0, -_ShiftWidth));

	colortex0.rgb = colortex0.rgb / 9;
	*/

	//�O���b�`
	/*
	float vertNoise = WhiteNoise(float2(floor((input.uv.x) / 0.3) * 0.3, Time * 0.1));
	float horzNoise = WhiteNoise(float2(floor((input.uv.y) / 0.3) * 0.3, Time * 0.2));
	float vertGlitchStrength = vertNoise / 0.1;
	float horzGlitchStrength = horzNoise / 0.1;
	vertGlitchStrength = vertGlitchStrength * 2.0 - 1.0;
	horzGlitchStrength = horzGlitchStrength * 2.0 - 1.0;
	float V = step(vertNoise, 0.1 * 0.2) * vertGlitchStrength;
	float H = step(horzNoise, 0.1) * horzGlitchStrength;

	samplePoint = input.uv;
	float sinv = sin(samplePoint.y * 2 - Time * -0.01);
	float steped = 1 - step(0.99, sinv * sinv);
	float timeFrac = steped * step(0.9, frac(Time));
	samplePoint.x += 1 * timeFrac * (V + H);
	Tex0 = tex0.Sample(smp, samplePoint);
	*/


	
	//�p��
	/*
	samplePoint -= float2(0.5, 0.5);
	float distPower = pow(length(samplePoint), 0.11);
	samplePoint *= float2(distPower, distPower);
	samplePoint += float2(0.5, 0.5);
	Tex0 = tex0.Sample(smp, samplePoint);
	*/


	
	//������Â�
	/*
	float vignette = length(float2(0.5, 0.5) - input.uv);
	vignette = clamp(vignette - 0.2, 0, 1);
	Tex0.rgb -= vignette;
	*/

	

	//�m�C�Y
//	float noise = WhiteNoise(input.uv * 0.1) - 0.5;
//	Tex0.rgb += float3(noise, noise, noise);
	

	//������
	//float sinv = sin(samplePoint.y * 2 - 0.4 * -0.01);
	//float steped = 1 - step(0.99, sinv * sinv);
	// sinv = sin(input.uv.y * 2 + 0.3 * -0.1);
	// steped = step(0.99, sinv * sinv);

	////Tex0.rgb -= (1 - steped) * abs(sin(input.uv.y * 50.0 + 0.3 * 1.0)) * 0.05;
	//Tex0.rgb -= (1 - steped) * abs(sin(input.uv.y * 100.0 + 0.3 * 2.0)) * 0.08;
	//Tex0.rgb += steped * 0.1;

	//�F����

	//Tex.r = tex.Sample(smp, samplePoint).r;
	//Tex.g += tex.Sample(smp, samplePoint).g;
	//samplePoint.x += 2 * timeFrac * (V + H);
	//Tex.b += tex.Sample(smp, samplePoint).b;

	 //�G�t�F�N�g���������ĂȂ������𔒂��ۂ�
	//Tex.rgb += (1 - steped) * 0.1;


	//float noise = vertNoise * 0.1;
	//Tex.rgb += steped * float3(noise, noise, noise);


	//*/
	return colortex0;
	//return Tex0;
	

}

