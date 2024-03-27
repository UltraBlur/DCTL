# DCTL

## SAT_JSh

A tool based on the ZCAM for a more perceptually uniform increase or decrease in image saturation.

INPUT：Linear、CIE XYZ D65. Please using Color Space Transform, Tone mapping method:None, Gamut Mapping method:None.

Luminance Adjust：Adjusting the model's prediction of luminance. It can be adjusted to better predict the H-K Effect when there is significant spillover of color.

Saturation Suppression: Analogous to the Lift Gamma Gain color wheel, if you think of Saturation as a Gain on saturation, then Saturation Suppression is a Gamma operation on saturation. After testing, the recommended setting is 0.2 to get a comfortable result. This is equivalent to controlling the degree of "natural saturation".

## OKLab_GHYC

## AWG_To_JSH

### Test

#### XYZtoOKLCH

#### AWGtoOKLCH
