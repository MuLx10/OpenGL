// Include standard headers
#include <stdio.h>
#include <stdlib.h>
#include <vector>
// Include GLEW
#include <GL/glew.h>

// Include GLFW
#include <GLFW/glfw3.h>


#include <src/utils/improved_shader.hpp>
#include <src/utils/texture_loader.hpp>

float vertices[] = {
	//x     y     z     r     g     b        sampler_id tex_cords   
	-0.5f, -0.5f, 0.0f, 0.5f, 1.0f, 1.0f,    0.0f,      0.0f, 0.0f, // bottom left
	 0.5f, -0.5f, 0.0f, 0.5f, 1.0f, 1.0f,    0.0f,      1.0f, 0.0f, // bottom right
	 0.5f,  0.5f, 0.0f, 1.0f, 0.5f, 1.0f,    0.0f,      1.0f, 1.0f, // top right

	 0.5f,  0.5f, 0.0f, 1.0f, 0.5f, 1.0f,    0.0f,      1.0f, 1.0f, // top right
	-0.5f,  0.5f, 0.0f, 1.0f, 1.0f, 0.5f,    0.0f,      0.0f, 1.0f, // top left 
	-0.5f, -0.5f, 0.0f, 1.0f, 1.0f, 0.5f,    0.0f,      0.0f, 0.0f,  // bottom left



	-0.25f, -0.25f, 0.0f, 0.5f, 1.0f, 1.0f, 1.0f,      0.0f, 0.0f, // bottom left
	 0.25f, -0.25f, 0.0f, 0.5f, 1.0f, 1.0f, 1.0f,      1.0f, 0.0f, // bottom right
	 0.25f,  0.25f, 0.0f, 1.0f, 0.5f, 1.0f, 1.0f,      1.0f, 1.0f, // top right

	 0.25f,  0.25f, 0.0f, 1.0f, 0.5f, 1.0f, 1.0f,      1.0f, 1.0f, // top right
	-0.25f,  0.25f, 0.0f, 1.0f, 1.0f, 0.5f, 1.0f,      0.0f, 1.0f, // top left 
	-0.25f, -0.25f, 0.0f, 1.0f, 1.0f, 0.5f, 1.0f,      0.0f, 0.0f  // bottom left
};


void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;


class MultiOGLTextures
{
private:
    unsigned int VBO, VAO;
    unsigned int textures[2];

    Shader shader;
public:
	MultiOGLTextures(){

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    // glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);


		// 1st attribute buffer : vertices
    glEnableVertexAttribArray(0);
		glVertexAttribPointer(
			0,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
			3,                  // size
			GL_FLOAT,           // type
			GL_FALSE,           // normalized?
			9 * sizeof(float),                  // stride
			(void*)0            // array buffer offset
		);


		// 2nd attribute buffer : vertices
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)(3 * sizeof(float)));

		// 3rd attribute buffer : vertices
		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 1, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)(6 * sizeof(float)));

		// 4th attribute buffer : vertices
		glEnableVertexAttribArray(3);
		glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)(7 * sizeof(float)));

	}
	~MultiOGLTextures(){
		 // optional: de-allocate all resources once they've outlived their purpose:
    // ------------------------------------------------------------------------
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
	}

	GLuint InitShaders(const char *vertex_file_path, const char *fragment_file_path){
    shader.Init(vertex_file_path, fragment_file_path);
	}
	void LoadTextures(const char *imagepath1, const char *imagepath2){
		    // load and create a texture 
    // -------------------------
    int width, height, channels;
    textures[0]=load_texture(imagepath1, &width, &height, &channels);
    textures[1]=load_texture(imagepath2, &width, &height, &channels);

    // tell opengl for each sampler to which texture unit it belongs to (only has to be done once)
    // -------------------------------------------------------------------------------------------
    shader.use(); // don't forget to activate/use the shader before setting uniforms!
    // either set it manually like so:
    glUniform1i(glGetUniformLocation(shader.ID, "texture1"), 0);
    glUniform1i(glGetUniformLocation(shader.ID, "texture2"), 1);
	}

	void Bind(){
		// bind textures on corresponding texture units
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, textures[0]);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, textures[1]);

    // render container
    shader.use();
    glBindVertexArray(VAO);
	}

	void Render(){
		// Draw the triangle !
		glDrawArrays(GL_TRIANGLES, 0, 3*2*2); // 6 indices 2 triangle
	}

	void UnBind(){
	
	}
};

int main()
{
    // glfw: initialize and configure
    // ------------------------------
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);



    // glfw window creation
    // --------------------
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // Initialize GLEW
		glewExperimental = true; // Needed for core profile
		if (glewInit() != GLEW_OK) {
			fprintf(stderr, "Failed to initialize GLEW\n");
			getchar();
			glfwTerminate();
			return -1;
		}
    // build and compile our shader zprogram
    // ------------------------------------
    MultiOGLTextures ogl_texture;
    ogl_texture.InitShaders("src/multi-texture/res/vertex.shader", "src/multi-texture/res/fragment.shader");
    ogl_texture.LoadTextures("src/multi-texture/res/white.jpg", "src/multi-texture/res/mehul.jpg");



    // render loop
    // -----------
    while (!glfwWindowShouldClose(window))
    {
        // input
        // -----
        processInput(window);

        // render
        // ------
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        
				
				ogl_texture.Bind();
				ogl_texture.Render();
				ogl_texture.UnBind();

        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();
    }


    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
    glfwTerminate();
    return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and 
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}