/*
    File: SimpleFragment.glsl
    Author: Ashley Manson
 
    Description: Simple Fragment Shadder, doesn't do much.
 */

varying lowp vec4 DestinationColor;

void main(void) {
    gl_FragColor = DestinationColor;
}