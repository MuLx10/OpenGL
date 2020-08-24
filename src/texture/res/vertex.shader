#version 330 core

// Input vertex data, different for all executions of this shader.
layout(location = 0) in vec3 v_position;
layout(location = 1) in vec3 v_color;
layout(location = 2) in vec2 v_coords;

out vec3 f_color;
out vec2 f_coords;

void main(){

    gl_Position.xyz = v_position.xyz;
    gl_Position.w = 1.0;
    f_color = v_color;
    f_coords = v_coords;
}

