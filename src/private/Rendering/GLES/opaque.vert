attribute vec2 inPosition;

void main()
{
    gl_Position = vec4(inPosition, 0.0, 1.0);
}
