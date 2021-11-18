xof 0302txt 0064
template Header {
 <3D82AB43-62DA-11cf-AB39-0020AF71E433>
 WORD major;
 WORD minor;
 DWORD flags;
}

template Vector {
 <3D82AB5E-62DA-11cf-AB39-0020AF71E433>
 FLOAT x;
 FLOAT y;
 FLOAT z;
}

template Coords2d {
 <F6F23F44-7686-11cf-8F52-0040333594A3>
 FLOAT u;
 FLOAT v;
}

template Matrix4x4 {
 <F6F23F45-7686-11cf-8F52-0040333594A3>
 array FLOAT matrix[16];
}

template ColorRGBA {
 <35FF44E0-6C7C-11cf-8F52-0040333594A3>
 FLOAT red;
 FLOAT green;
 FLOAT blue;
 FLOAT alpha;
}

template ColorRGB {
 <D3E16E81-7835-11cf-8F52-0040333594A3>
 FLOAT red;
 FLOAT green;
 FLOAT blue;
}

template IndexedColor {
 <1630B820-7842-11cf-8F52-0040333594A3>
 DWORD index;
 ColorRGBA indexColor;
}

template Boolean {
 <4885AE61-78E8-11cf-8F52-0040333594A3>
 WORD truefalse;
}

template Boolean2d {
 <4885AE63-78E8-11cf-8F52-0040333594A3>
 Boolean u;
 Boolean v;
}

template MaterialWrap {
 <4885AE60-78E8-11cf-8F52-0040333594A3>
 Boolean u;
 Boolean v;
}

template TextureFilename {
 <A42790E1-7810-11cf-8F52-0040333594A3>
 STRING filename;
}

template Material {
 <3D82AB4D-62DA-11cf-AB39-0020AF71E433>
 ColorRGBA faceColor;
 FLOAT power;
 ColorRGB specularColor;
 ColorRGB emissiveColor;
 [...]
}

template MeshFace {
 <3D82AB5F-62DA-11cf-AB39-0020AF71E433>
 DWORD nFaceVertexIndices;
 array DWORD faceVertexIndices[nFaceVertexIndices];
}

template MeshFaceWraps {
 <4885AE62-78E8-11cf-8F52-0040333594A3>
 DWORD nFaceWrapValues;
 Boolean2d faceWrapValues;
}

template MeshTextureCoords {
 <F6F23F40-7686-11cf-8F52-0040333594A3>
 DWORD nTextureCoords;
 array Coords2d textureCoords[nTextureCoords];
}

template MeshMaterialList {
 <F6F23F42-7686-11cf-8F52-0040333594A3>
 DWORD nMaterials;
 DWORD nFaceIndexes;
 array DWORD faceIndexes[nFaceIndexes];
 [Material]
}

template MeshNormals {
 <F6F23F43-7686-11cf-8F52-0040333594A3>
 DWORD nNormals;
 array Vector normals[nNormals];
 DWORD nFaceNormals;
 array MeshFace faceNormals[nFaceNormals];
}

template MeshVertexColors {
 <1630B821-7842-11cf-8F52-0040333594A3>
 DWORD nVertexColors;
 array IndexedColor vertexColors[nVertexColors];
}

template Mesh {
 <3D82AB44-62DA-11cf-AB39-0020AF71E433>
 DWORD nVertices;
 array Vector vertices[nVertices];
 DWORD nFaces;
 array MeshFace faces[nFaces];
 [...]
}

Header{
1;
0;
1;
}

Mesh {
 203;
 0.00000;11.86360;0.00000;,
 3.03489;11.42261;-2.07741;,
 0.00000;11.42261;-2.93791;,
 0.00000;11.86360;0.00000;,
 4.29199;11.42261;0.00000;,
 0.00000;11.86360;0.00000;,
 3.03489;11.42261;2.07741;,
 0.00000;11.86360;0.00000;,
 0.00000;11.42261;2.93791;,
 0.00000;11.86360;0.00000;,
 -3.03489;11.42261;2.07741;,
 0.00000;11.86360;0.00000;,
 -4.29199;11.42261;0.00000;,
 0.00000;11.86360;0.00000;,
 -3.03489;11.42261;-2.07741;,
 0.00000;11.86360;0.00000;,
 0.00000;11.42261;-2.93791;,
 5.60775;9.40108;-3.83856;,
 0.00000;9.40108;-5.42854;,
 7.93056;9.40108;0.00000;,
 5.60775;9.40108;3.83856;,
 0.00000;9.40108;5.42854;,
 -5.60776;9.40108;3.83856;,
 -7.93056;9.40108;0.00000;,
 -5.60776;9.40108;-3.83856;,
 0.00000;9.40108;-5.42854;,
 7.32688;6.37565;-5.01532;,
 0.00000;6.37565;-7.09274;,
 9.75404;6.37565;0.00000;,
 7.32688;6.37565;5.01532;,
 0.00000;6.37565;7.09274;,
 -7.32688;6.37565;5.01532;,
 -9.98921;6.37565;0.00000;,
 -7.32688;6.37565;-5.01532;,
 0.00000;6.37565;-7.09274;,
 7.93056;2.80691;-5.42854;,
 0.00000;2.80691;-7.67712;,
 10.60777;2.80691;0.00000;,
 7.93056;2.80691;5.42854;,
 0.00000;2.80691;7.67712;,
 -7.93056;2.80691;5.42854;,
 -10.84294;2.80691;0.00000;,
 -7.93056;2.80691;-5.42854;,
 0.00000;2.80691;-7.67712;,
 7.32688;0.48139;-5.01532;,
 0.00000;0.48139;-7.09274;,
 9.75404;0.48139;0.00000;,
 7.32688;0.48139;5.01532;,
 0.00000;0.48139;7.09274;,
 -7.32688;0.48139;5.01532;,
 -9.98921;0.48139;0.00000;,
 -7.32688;0.48139;-5.01532;,
 0.00000;0.48139;-7.09274;,
 5.60775;-0.52343;-3.83856;,
 0.00000;-0.52343;-5.42854;,
 7.93056;-0.52343;0.00000;,
 5.60775;-0.52343;3.83856;,
 -0.00000;-0.52343;5.42854;,
 -5.60776;-0.52343;3.83856;,
 -7.93056;-0.52343;0.00000;,
 -5.60776;-0.52343;-3.83856;,
 0.00000;-0.52343;-5.42854;,
 3.03489;-1.19483;-2.07741;,
 0.00000;-1.19483;-2.93791;,
 4.29199;-1.19483;0.00000;,
 3.03489;-1.19483;2.07741;,
 -0.00000;-1.19483;2.93791;,
 -3.03489;-1.19483;2.07741;,
 -4.29199;-1.19483;0.00000;,
 -3.03489;-1.19483;-2.07741;,
 0.00000;-1.19483;-2.93791;,
 0.00000;-1.43059;-0.00000;,
 0.00000;-1.43059;-0.00000;,
 0.00000;-1.43059;-0.00000;,
 0.00000;-1.43059;-0.00000;,
 0.00000;-1.43059;-0.00000;,
 0.00000;-1.43059;-0.00000;,
 0.00000;-1.43059;-0.00000;,
 0.00000;-1.43059;-0.00000;,
 -1.87451;5.91889;-6.84371;,
 -1.80415;5.87826;-6.65293;,
 -1.86387;5.71180;-6.66637;,
 -1.93423;5.75243;-6.85714;,
 -5.72390;7.19032;-4.86840;,
 -5.83140;7.25240;-5.15991;,
 -5.92265;6.99806;-5.18043;,
 -5.81514;6.93598;-4.88892;,
 -2.65372;6.42215;-6.53260;,
 -1.87451;5.91889;-6.84371;,
 -1.93423;5.75243;-6.85714;,
 -2.74277;6.20410;-6.54619;,
 -2.60006;6.38104;-6.26165;,
 -2.68912;6.16299;-6.27525;,
 -1.86387;5.71180;-6.66637;,
 -1.80415;5.87826;-6.65293;,
 -2.60006;6.38104;-6.26165;,
 -1.87451;5.91889;-6.84371;,
 -2.65372;6.42215;-6.53260;,
 -2.74277;6.20410;-6.54619;,
 -1.93423;5.75243;-6.85714;,
 -2.68912;6.16299;-6.27525;,
 -3.84618;6.54489;-6.08926;,
 -5.92265;6.99806;-5.18043;,
 -5.83140;7.25240;-5.15991;,
 -3.75065;6.79785;-6.07061;,
 -3.70381;6.75538;-5.75255;,
 -5.72390;7.19032;-4.86840;,
 -5.81514;6.93598;-4.88892;,
 -3.79934;6.50243;-5.77120;,
 -3.75065;6.79785;-6.07061;,
 -5.83140;7.25240;-5.15991;,
 -3.70381;6.75538;-5.75255;,
 -3.79934;6.50243;-5.77120;,
 -5.92265;6.99806;-5.18043;,
 -3.84618;6.54489;-6.08926;,
 -2.72847;7.07754;-5.86666;,
 -2.72269;7.13035;-6.17902;,
 -2.86526;6.90356;-6.22000;,
 -2.87103;6.85075;-5.90764;,
 -1.65427;6.54000;-6.74929;,
 -1.71877;6.52320;-6.43193;,
 -1.86548;6.29896;-6.47257;,
 -1.80098;6.31576;-6.78992;,
 -2.35685;6.71954;-6.50581;,
 -2.86526;6.90356;-6.22000;,
 -2.72269;7.13035;-6.17902;,
 -2.26701;6.96495;-6.43432;,
 -1.65427;6.54000;-6.74929;,
 -1.80098;6.31576;-6.78992;,
 -2.31086;6.93221;-6.11248;,
 -2.72847;7.07754;-5.86666;,
 -2.87103;6.85075;-5.90764;,
 -2.40028;6.68746;-6.18575;,
 -2.26701;6.96495;-6.43432;,
 -2.72269;7.13035;-6.17902;,
 -2.31086;6.93221;-6.11248;,
 -1.71877;6.52320;-6.43193;,
 -2.40028;6.68746;-6.18575;,
 -2.86526;6.90356;-6.22000;,
 -2.35685;6.71954;-6.50581;,
 -1.86548;6.29896;-6.47257;,
 1.87451;5.91889;-6.84371;,
 1.93423;5.75243;-6.85714;,
 1.86387;5.71180;-6.66637;,
 1.80415;5.87826;-6.65293;,
 5.72390;7.19032;-4.86840;,
 5.81514;6.93598;-4.88892;,
 5.92265;6.99806;-5.18043;,
 5.83140;7.25240;-5.15991;,
 2.65372;6.42215;-6.53260;,
 2.74277;6.20410;-6.54619;,
 1.93423;5.75243;-6.85714;,
 1.87451;5.91889;-6.84371;,
 2.60006;6.38104;-6.26165;,
 1.80415;5.87826;-6.65293;,
 1.86387;5.71180;-6.66637;,
 2.68912;6.16299;-6.27525;,
 2.60006;6.38104;-6.26165;,
 2.65372;6.42215;-6.53260;,
 1.87451;5.91889;-6.84371;,
 2.74277;6.20410;-6.54619;,
 2.68912;6.16299;-6.27525;,
 1.93423;5.75243;-6.85714;,
 3.84618;6.54489;-6.08926;,
 3.75065;6.79785;-6.07061;,
 5.83140;7.25240;-5.15991;,
 5.92265;6.99806;-5.18043;,
 3.70381;6.75538;-5.75255;,
 3.79934;6.50243;-5.77120;,
 5.81514;6.93598;-4.88892;,
 5.72390;7.19032;-4.86840;,
 3.75065;6.79785;-6.07061;,
 3.70381;6.75538;-5.75255;,
 5.83140;7.25240;-5.15991;,
 3.79934;6.50243;-5.77120;,
 3.84618;6.54489;-6.08926;,
 5.92265;6.99806;-5.18043;,
 2.72847;7.07754;-5.86666;,
 2.87103;6.85075;-5.90764;,
 2.86526;6.90356;-6.22000;,
 2.72269;7.13035;-6.17902;,
 1.65427;6.54000;-6.74929;,
 1.80098;6.31576;-6.78992;,
 1.86548;6.29896;-6.47257;,
 1.71877;6.52320;-6.43193;,
 2.35685;6.71954;-6.50581;,
 2.26701;6.96495;-6.43432;,
 2.72269;7.13035;-6.17902;,
 2.86526;6.90356;-6.22000;,
 1.80098;6.31576;-6.78992;,
 1.65427;6.54000;-6.74929;,
 2.31086;6.93221;-6.11248;,
 2.40028;6.68746;-6.18575;,
 2.87103;6.85075;-5.90764;,
 2.72847;7.07754;-5.86666;,
 2.26701;6.96495;-6.43432;,
 2.31086;6.93221;-6.11248;,
 2.72269;7.13035;-6.17902;,
 1.71877;6.52320;-6.43193;,
 2.40028;6.68746;-6.18575;,
 2.35685;6.71954;-6.50581;,
 2.86526;6.90356;-6.22000;,
 1.86548;6.29896;-6.47257;;
 
 112;
 3;0,1,2;,
 3;3,4,1;,
 3;5,6,4;,
 3;7,8,6;,
 3;9,10,8;,
 3;11,12,10;,
 3;13,14,12;,
 3;15,16,14;,
 4;2,1,17,18;,
 4;1,4,19,17;,
 4;4,6,20,19;,
 4;6,8,21,20;,
 4;8,10,22,21;,
 4;10,12,23,22;,
 4;12,14,24,23;,
 4;14,16,25,24;,
 4;18,17,26,27;,
 4;17,19,28,26;,
 4;19,20,29,28;,
 4;20,21,30,29;,
 4;21,22,31,30;,
 4;22,23,32,31;,
 4;23,24,33,32;,
 4;24,25,34,33;,
 4;27,26,35,36;,
 4;26,28,37,35;,
 4;28,29,38,37;,
 4;29,30,39,38;,
 4;30,31,40,39;,
 4;31,32,41,40;,
 4;32,33,42,41;,
 4;33,34,43,42;,
 4;36,35,44,45;,
 4;35,37,46,44;,
 4;37,38,47,46;,
 4;38,39,48,47;,
 4;39,40,49,48;,
 4;40,41,50,49;,
 4;41,42,51,50;,
 4;42,43,52,51;,
 4;45,44,53,54;,
 4;44,46,55,53;,
 4;46,47,56,55;,
 4;47,48,57,56;,
 4;48,49,58,57;,
 4;49,50,59,58;,
 4;50,51,60,59;,
 4;51,52,61,60;,
 4;54,53,62,63;,
 4;53,55,64,62;,
 4;55,56,65,64;,
 4;56,57,66,65;,
 4;57,58,67,66;,
 4;58,59,68,67;,
 4;59,60,69,68;,
 4;60,61,70,69;,
 3;63,62,71;,
 3;62,64,72;,
 3;64,65,73;,
 3;65,66,74;,
 3;66,67,75;,
 3;67,68,76;,
 3;68,69,77;,
 3;69,70,78;,
 4;79,80,81,82;,
 4;83,84,85,86;,
 4;87,88,89,90;,
 4;91,92,93,94;,
 4;95,80,96,97;,
 4;98,99,81,100;,
 4;101,102,103,104;,
 4;101,104,87,90;,
 4;105,106,107,108;,
 4;105,108,92,91;,
 4;109,110,83,111;,
 4;109,111,95,97;,
 4;112,86,113,114;,
 4;112,114,98,100;,
 4;115,116,117,118;,
 4;119,120,121,122;,
 4;123,124,125,126;,
 4;123,126,127,128;,
 4;129,130,131,132;,
 4;129,132,121,120;,
 4;133,134,115,135;,
 4;133,135,136,119;,
 4;137,118,138,139;,
 4;137,139,122,140;,
 4;141,142,143,144;,
 4;145,146,147,148;,
 4;149,150,151,152;,
 4;153,154,155,156;,
 4;157,158,159,144;,
 4;160,161,143,162;,
 4;163,164,165,166;,
 4;163,150,149,164;,
 4;167,168,169,170;,
 4;167,153,156,168;,
 4;171,172,145,173;,
 4;171,158,157,172;,
 4;174,175,176,146;,
 4;174,161,160,175;,
 4;177,178,179,180;,
 4;181,182,183,184;,
 4;185,186,187,188;,
 4;185,189,190,186;,
 4;191,192,193,194;,
 4;191,184,183,192;,
 4;195,196,177,197;,
 4;195,181,198,196;,
 4;199,200,201,178;,
 4;199,202,182,200;;
 
 MeshMaterialList {
  2;
  112;
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1;;
  Material {
   0.542400;0.693600;0.800000;0.900000;;
   5.000000;
   0.100000;0.100000;0.100000;;
   0.094920;0.121380;0.140000;;
  }
  Material {
   0.191200;0.388800;0.646400;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
  }
 }
 MeshNormals {
  122;
  0.000000;1.000000;0.000000;,
  0.000000;0.915770;-0.401702;,
  0.204986;0.934774;-0.290134;,
  0.296190;0.955129;0.000000;,
  0.204986;0.934774;0.290134;,
  0.000000;0.915770;0.401702;,
  -0.204986;0.934774;0.290134;,
  -0.296190;0.955129;0.000000;,
  -0.204986;0.934774;-0.290134;,
  0.000000;0.642177;-0.766557;,
  0.435565;0.691916;-0.575792;,
  0.666029;0.745925;0.000000;,
  0.435565;0.691916;0.575792;,
  0.000000;0.642177;0.766557;,
  -0.429570;0.694963;0.576625;,
  -0.655431;0.755255;0.000000;,
  -0.429570;0.694963;-0.576625;,
  0.000000;0.327134;-0.944978;,
  0.585627;0.369581;-0.721423;,
  0.913201;0.407510;0.000000;,
  0.585627;0.369581;0.721423;,
  0.000000;0.327134;0.944978;,
  -0.573866;0.372642;0.729256;,
  -0.906263;0.422715;0.000000;,
  -0.573866;0.372642;-0.729256;,
  0.000000;-0.042058;-0.999115;,
  0.638066;-0.050609;-0.768317;,
  0.998321;-0.057916;-0.000000;,
  0.638066;-0.050609;0.768317;,
  0.000000;-0.042058;0.999115;,
  -0.624434;-0.050060;0.779472;,
  -0.998279;-0.058648;0.000000;,
  -0.624434;-0.050060;-0.779472;,
  0.000000;-0.598968;-0.800773;,
  0.480188;-0.644455;-0.595061;,
  0.733743;-0.679427;-0.000000;,
  0.480188;-0.644455;0.595061;,
  0.000000;-0.598968;0.800773;,
  -0.468913;-0.645767;0.602582;,
  -0.721283;-0.692640;0.000000;,
  -0.468913;-0.645767;-0.602582;,
  0.000000;-0.920007;-0.391902;,
  0.209576;-0.936050;-0.282646;,
  0.308637;-0.951180;-0.000000;,
  0.209576;-0.936050;0.282646;,
  0.000000;-0.920007;0.391902;,
  -0.203744;-0.937488;0.282144;,
  -0.296962;-0.954889;0.000000;,
  -0.203744;-0.937488;-0.282144;,
  0.000000;-0.985316;-0.170740;,
  0.083321;-0.989134;-0.121128;,
  0.118222;-0.992987;-0.000000;,
  0.083321;-0.989134;0.121128;,
  0.000000;-0.985316;0.170740;,
  -0.083321;-0.989134;0.121127;,
  -0.118222;-0.992987;0.000000;,
  -0.083321;-0.989134;-0.121127;,
  0.000000;-1.000000;-0.000000;,
  -0.878354;0.284097;0.384427;,
  0.878347;-0.284091;-0.384447;,
  -0.296895;0.180857;-0.937627;,
  0.342589;-0.197213;0.918553;,
  -0.340205;0.196049;-0.919688;,
  0.354654;-0.201284;0.913075;,
  -0.268701;0.170864;-0.947948;,
  0.331831;-0.193360;0.923310;,
  0.447562;0.893485;0.037044;,
  0.539262;0.842137;0.000926;,
  -0.409113;-0.911141;-0.049474;,
  -0.490668;-0.871118;-0.019940;,
  -0.355512;0.201368;-0.912722;,
  0.356012;-0.201531;0.912491;,
  0.302056;0.949309;0.087032;,
  0.252940;0.962183;0.101120;,
  -0.288502;-0.953247;-0.089922;,
  -0.252751;-0.962229;-0.101155;,
  -0.850239;0.521390;0.072430;,
  0.817463;-0.559568;0.136521;,
  0.610077;-0.518459;0.599171;,
  -0.289502;0.355544;-0.888694;,
  0.306047;-0.365175;0.879194;,
  -0.346501;0.375206;-0.859743;,
  -0.231207;0.334292;-0.913670;,
  0.365729;-0.385326;0.847211;,
  0.517311;0.840673;0.160182;,
  0.408168;0.899912;0.153484;,
  0.618454;0.768431;0.164402;,
  -0.525871;-0.835286;-0.160494;,
  -0.407248;-0.900468;-0.152663;,
  -0.634827;-0.754749;-0.165375;,
  0.878354;0.284097;0.384427;,
  -0.878347;-0.284091;-0.384447;,
  0.296895;0.180857;-0.937627;,
  -0.342589;-0.197213;0.918553;,
  0.340205;0.196049;-0.919688;,
  -0.354654;-0.201284;0.913075;,
  0.268701;0.170864;-0.947948;,
  -0.331831;-0.193360;0.923310;,
  -0.447562;0.893485;0.037044;,
  -0.539262;0.842137;0.000926;,
  0.409113;-0.911141;-0.049474;,
  0.490668;-0.871118;-0.019940;,
  0.355512;0.201368;-0.912722;,
  -0.356012;-0.201531;0.912491;,
  -0.302056;0.949309;0.087032;,
  -0.252940;0.962183;0.101120;,
  0.288502;-0.953247;-0.089922;,
  0.252751;-0.962229;-0.101155;,
  0.850239;0.521390;0.072430;,
  -0.817463;-0.559568;0.136521;,
  -0.610077;-0.518459;0.599171;,
  0.289502;0.355544;-0.888694;,
  -0.306047;-0.365175;0.879194;,
  0.346501;0.375206;-0.859743;,
  0.231207;0.334292;-0.913670;,
  -0.365729;-0.385326;0.847211;,
  -0.517311;0.840673;0.160182;,
  -0.408168;0.899912;0.153484;,
  -0.618454;0.768431;0.164402;,
  0.525871;-0.835286;-0.160494;,
  0.407248;-0.900468;-0.152663;,
  0.634827;-0.754749;-0.165375;;
  112;
  3;0,2,1;,
  3;0,3,2;,
  3;0,4,3;,
  3;0,5,4;,
  3;0,6,5;,
  3;0,7,6;,
  3;0,8,7;,
  3;0,1,8;,
  4;1,2,10,9;,
  4;2,3,11,10;,
  4;3,4,12,11;,
  4;4,5,13,12;,
  4;5,6,14,13;,
  4;6,7,15,14;,
  4;7,8,16,15;,
  4;8,1,9,16;,
  4;9,10,18,17;,
  4;10,11,19,18;,
  4;11,12,20,19;,
  4;12,13,21,20;,
  4;13,14,22,21;,
  4;14,15,23,22;,
  4;15,16,24,23;,
  4;16,9,17,24;,
  4;17,18,26,25;,
  4;18,19,27,26;,
  4;19,20,28,27;,
  4;20,21,29,28;,
  4;21,22,30,29;,
  4;22,23,31,30;,
  4;23,24,32,31;,
  4;24,17,25,32;,
  4;25,26,34,33;,
  4;26,27,35,34;,
  4;27,28,36,35;,
  4;28,29,37,36;,
  4;29,30,38,37;,
  4;30,31,39,38;,
  4;31,32,40,39;,
  4;32,25,33,40;,
  4;33,34,42,41;,
  4;34,35,43,42;,
  4;35,36,44,43;,
  4;36,37,45,44;,
  4;37,38,46,45;,
  4;38,39,47,46;,
  4;39,40,48,47;,
  4;40,33,41,48;,
  4;41,42,50,49;,
  4;42,43,51,50;,
  4;43,44,52,51;,
  4;44,45,53,52;,
  4;45,46,54,53;,
  4;46,47,55,54;,
  4;47,48,56,55;,
  4;48,41,49,56;,
  3;49,50,57;,
  3;50,51,57;,
  3;51,52,57;,
  3;52,53,57;,
  3;53,54,57;,
  3;54,55,57;,
  3;55,56,57;,
  3;56,49,57;,
  4;59,59,59,59;,
  4;58,58,58,58;,
  4;60,64,64,60;,
  4;61,61,65,65;,
  4;66,67,67,66;,
  4;68,69,69,68;,
  4;62,70,70,62;,
  4;62,62,60,60;,
  4;63,71,71,63;,
  4;63,63,61,61;,
  4;72,73,73,72;,
  4;72,72,66,66;,
  4;74,75,75,74;,
  4;74,74,68,68;,
  4;76,76,76,76;,
  4;77,78,78,77;,
  4;79,81,81,79;,
  4;79,79,82,82;,
  4;80,83,83,80;,
  4;80,80,78,78;,
  4;84,85,85,84;,
  4;84,84,86,86;,
  4;87,88,88,87;,
  4;87,87,89,89;,
  4;91,91,91,91;,
  4;90,90,90,90;,
  4;92,92,96,96;,
  4;93,97,97,93;,
  4;98,98,99,99;,
  4;100,100,101,101;,
  4;94,94,102,102;,
  4;94,92,92,94;,
  4;95,95,103,103;,
  4;95,93,93,95;,
  4;104,104,105,105;,
  4;104,98,98,104;,
  4;106,106,107,107;,
  4;106,100,100,106;,
  4;108,108,108,108;,
  4;109,109,110,110;,
  4;111,111,113,113;,
  4;111,114,114,111;,
  4;112,112,115,115;,
  4;112,110,110,112;,
  4;116,116,117,117;,
  4;116,118,118,116;,
  4;119,119,120,120;,
  4;119,121,121,119;;
 }
 MeshTextureCoords {
  203;
  0.062500;0.000000;,
  0.125000;0.125000;,
  0.000000;0.125000;,
  0.187500;0.000000;,
  0.250000;0.125000;,
  0.312500;0.000000;,
  0.375000;0.125000;,
  0.437500;0.000000;,
  0.500000;0.125000;,
  0.562500;0.000000;,
  0.625000;0.125000;,
  0.687500;0.000000;,
  0.750000;0.125000;,
  0.812500;0.000000;,
  0.875000;0.125000;,
  0.937500;0.000000;,
  1.000000;0.125000;,
  0.125000;0.250000;,
  0.000000;0.250000;,
  0.250000;0.250000;,
  0.375000;0.250000;,
  0.500000;0.250000;,
  0.625000;0.250000;,
  0.750000;0.250000;,
  0.875000;0.250000;,
  1.000000;0.250000;,
  0.125000;0.375000;,
  0.000000;0.375000;,
  0.250000;0.375000;,
  0.375000;0.375000;,
  0.500000;0.375000;,
  0.625000;0.375000;,
  0.750000;0.375000;,
  0.875000;0.375000;,
  1.000000;0.375000;,
  0.125000;0.500000;,
  0.000000;0.500000;,
  0.250000;0.500000;,
  0.375000;0.500000;,
  0.500000;0.500000;,
  0.625000;0.500000;,
  0.750000;0.500000;,
  0.875000;0.500000;,
  1.000000;0.500000;,
  0.125000;0.625000;,
  0.000000;0.625000;,
  0.250000;0.625000;,
  0.375000;0.625000;,
  0.500000;0.625000;,
  0.625000;0.625000;,
  0.750000;0.625000;,
  0.875000;0.625000;,
  1.000000;0.625000;,
  0.125000;0.750000;,
  0.000000;0.750000;,
  0.250000;0.750000;,
  0.375000;0.750000;,
  0.500000;0.750000;,
  0.625000;0.750000;,
  0.750000;0.750000;,
  0.875000;0.750000;,
  1.000000;0.750000;,
  0.125000;0.875000;,
  0.000000;0.875000;,
  0.250000;0.875000;,
  0.375000;0.875000;,
  0.500000;0.875000;,
  0.625000;0.875000;,
  0.750000;0.875000;,
  0.875000;0.875000;,
  1.000000;0.875000;,
  0.062500;1.000000;,
  0.187500;1.000000;,
  0.312500;1.000000;,
  0.437500;1.000000;,
  0.562500;1.000000;,
  0.687500;1.000000;,
  0.812500;1.000000;,
  0.937500;1.000000;,
  0.000000;0.000000;,
  1.000000;0.000000;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;0.000000;,
  1.000000;0.000000;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  0.804320;0.000000;,
  1.000000;0.000000;,
  1.000000;1.000000;,
  0.801500;1.000000;,
  0.207270;0.000000;,
  0.210090;1.000000;,
  0.000000;1.000000;,
  0.000000;0.000000;,
  0.792730;0.000000;,
  1.000000;1.000000;,
  0.804320;1.000000;,
  0.801500;0.000000;,
  1.000000;0.000000;,
  0.789910;1.000000;,
  0.525140;1.000000;,
  0.000000;1.000000;,
  0.000000;0.000000;,
  0.526230;0.000000;,
  0.489110;0.000000;,
  1.000000;0.000000;,
  1.000000;1.000000;,
  0.490200;1.000000;,
  0.526230;1.000000;,
  0.000000;1.000000;,
  0.510890;0.000000;,
  0.509800;1.000000;,
  0.000000;0.000000;,
  0.525140;0.000000;,
  0.000000;0.000000;,
  1.000000;0.000000;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  0.526230;1.000000;,
  0.489110;0.000000;,
  0.490200;1.000000;,
  0.525140;0.000000;,
  0.257270;1.000000;,
  0.000000;1.000000;,
  0.000000;0.000000;,
  0.230150;0.000000;,
  0.526230;0.000000;,
  0.525140;1.000000;,
  0.783000;0.000000;,
  1.000000;0.000000;,
  1.000000;1.000000;,
  0.754880;1.000000;,
  0.230150;1.000000;,
  0.000000;1.000000;,
  0.217000;0.000000;,
  0.510890;0.000000;,
  0.245120;1.000000;,
  0.000000;0.000000;,
  0.257270;0.000000;,
  0.509800;1.000000;,
  0.000000;0.000000;,
  0.000000;1.000000;,
  1.000000;1.000000;,
  1.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;1.000000;,
  1.000000;1.000000;,
  1.000000;0.000000;,
  0.804320;0.000000;,
  0.801500;1.000000;,
  1.000000;1.000000;,
  1.000000;0.000000;,
  0.207270;0.000000;,
  0.000000;0.000000;,
  0.000000;1.000000;,
  0.210090;1.000000;,
  0.792730;0.000000;,
  0.804320;1.000000;,
  1.000000;1.000000;,
  0.801500;0.000000;,
  0.789910;1.000000;,
  1.000000;0.000000;,
  0.525140;1.000000;,
  0.526230;0.000000;,
  0.000000;0.000000;,
  0.000000;1.000000;,
  0.489110;0.000000;,
  0.490200;1.000000;,
  1.000000;1.000000;,
  1.000000;0.000000;,
  0.526230;1.000000;,
  0.510890;0.000000;,
  0.000000;1.000000;,
  0.509800;1.000000;,
  0.525140;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;1.000000;,
  1.000000;1.000000;,
  1.000000;0.000000;,
  0.526230;1.000000;,
  0.525140;0.000000;,
  0.490200;1.000000;,
  0.489110;0.000000;,
  0.257270;1.000000;,
  0.230150;0.000000;,
  0.000000;0.000000;,
  0.000000;1.000000;,
  0.525140;1.000000;,
  0.526230;0.000000;,
  0.783000;0.000000;,
  0.754880;1.000000;,
  1.000000;1.000000;,
  1.000000;0.000000;,
  0.230150;1.000000;,
  0.217000;0.000000;,
  0.000000;1.000000;,
  0.510890;0.000000;,
  0.245120;1.000000;,
  0.257270;0.000000;,
  0.000000;0.000000;,
  0.509800;1.000000;;
 }
}
