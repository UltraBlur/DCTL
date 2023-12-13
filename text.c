static inline float4 XYZ_to_JzAzBz(float4 XYZ_D65)
{
  const float4 M[3] = { { 0.41478972f, 0.579999f, 0.0146480f, 0.0f },
                        { -0.2015100f, 1.120649f, 0.0531008f, 0.0f },
                        { -0.0166008f, 0.264800f, 0.6684799f, 0.0f } };

  const float4 A[3] = { { 0.5f, 0.5f, 0.0f, 0.0f },
                        { 3.524000f, -4.066708f, 0.542708f, 0.0f },
                        { 0.199076f, 1.096799f, -1.295875f, 0.0f } };

  float4 temp1, temp2;
  // XYZ -> X'Y'Z
  temp1.x = 1.15f * XYZ_D65.x - 0.15f * XYZ_D65.z;
  temp1.y = 0.66f * XYZ_D65.y + 0.34f * XYZ_D65.x;
  temp1.z = XYZ_D65.z;
  temp1.w = 0.f;
  // X'Y'Z -> LMS
  temp2.x = dot(M[0], temp1);
  temp2.y = dot(M[1], temp1);
  temp2.z = dot(M[2], temp1);
  temp2.w = 0.f;
  // LMS -> L'M'S'
  temp2 = native_powr(fmax(temp2 / 10000.f, 0.0f), 0.159301758f);
  temp2 = native_powr((0.8359375f + 18.8515625f * temp2) / (1.0f + 18.6875f * temp2), 134.034375f);
  // L'M'S' -> Izazbz
  temp1.x = dot(A[0], temp2);
  temp1.y = dot(A[1], temp2);
  temp1.z = dot(A[2], temp2);
  // Iz -> Jz
  temp1.x = fmax(0.44f * temp1.x / (1.0f - 0.56f * temp1.x) - 1.6295499532821566e-11f, 0.f);
  return temp1;
}

static inline float4 JzAzBz_2_XYZ(const float4 JzAzBz)
{
  const float b = 1.15f;
  const float g = 0.66f;
  const float c1 = 0.8359375f; // 3424 / 2^12
  const float c2 = 18.8515625f; // 2413 / 2^7
  const float c3 = 18.6875f; // 2392 / 2^7
  const float n_inv = 1.0f / 0.159301758f; // 2610 / 2^14
  const float p_inv = 1.0f / 134.034375f; // 1.7 x 2523 / 2^5
  const float d = -0.56f;
  const float d0 = 1.6295499532821566e-11f;
  const float4 MI[3] = { {  1.9242264357876067f, -1.0047923125953657f,  0.0376514040306180f, 0.0f },
                         {  0.3503167620949991f,  0.7264811939316552f, -0.0653844229480850f, 0.0f },
                         { -0.0909828109828475f, -0.3127282905230739f,  1.5227665613052603f, 0.0f } };
  const float4 AI[3] = { {  1.0f,  0.1386050432715393f,  0.0580473161561189f, 0.0f },
                         {  1.0f, -0.1386050432715393f, -0.0580473161561189f, 0.0f },
                         {  1.0f, -0.0960192420263190f, -0.8118918960560390f, 0.0f } };

  float4 XYZ, LMS, IzAzBz;
  // Jz -> Iz
  IzAzBz = JzAzBz;
  IzAzBz.x += d0;
  IzAzBz.x = fmax(IzAzBz.x / (1.0f + d - d * IzAzBz.x), 0.f);
  // IzAzBz -> L'M'S'
  LMS.x = dot(AI[0], IzAzBz);
  LMS.y = dot(AI[1], IzAzBz);
  LMS.z = dot(AI[2], IzAzBz);
  LMS.w = 0.f;
  // L'M'S' -> LMS
  LMS = native_powr(fmax(LMS, 0.0f), p_inv);
  LMS = 10000.f * native_powr(fmax((c1 - LMS) / (c3 * LMS - c2), 0.0f), n_inv);
  // LMS -> X'Y'Z
  XYZ.x = dot(MI[0], LMS);
  XYZ.y = dot(MI[1], LMS);
  XYZ.z = dot(MI[2], LMS);
  XYZ.w = 0.f;
  // X'Y'Z -> XYZ_D65
  float4 XYZ_D65;
  XYZ_D65.x = (XYZ.x + (b - 1.0f) * XYZ.z) / b;
  XYZ_D65.y = (XYZ.y + (g - 1.0f) * XYZ_D65.x) / g;
  XYZ_D65.z = XYZ.z;
  XYZ_D65.w = JzAzBz.w;
  return XYZ_D65;
}

{  1.9242264357876067f, -1.0047923125953657f,  0.0376514040306180f },
{  0.3503167620949991f,  0.7264811939316552f, -0.0653844229480850f },
{ -0.0909828109828475f, -0.3127282905230739f,  1.5227665613052603f }