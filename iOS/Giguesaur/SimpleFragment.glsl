/*
    File: SimpleFragment.glsl
    Author: Ashley Manson
 
    Simple Fragment Shadder.
 */

varying lowp vec4 DestinationColor;

void main(void) {
    gl_FragColor = DestinationColor;
}
