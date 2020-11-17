#include <iostream>
#include <gl/glew.h> 
#include <gl/freeglut.h>
#include <gl/freeglut_ext.h>
#include <glm/glm/glm.hpp>
#include <glm/glm/ext.hpp>
#include <glm/glm/gtc/matrix_transform.hpp>

#include "filein.h"


using namespace std;

GLchar* vertexShaderSource,
* fragmentShaderSource;

GLuint vertexShader,
fragmentShader;

GLuint shaderID;

GLuint VAO[12][12][12],
VBO_position[12][12][12],
VBO_color[12][12][12];
GLuint VAO_bg, VBO_bg, VBO_bgColor;

GLvoid drawScene(GLvoid);
GLvoid Reshape(int w, int h);
GLvoid Keyboard(unsigned char key, int x, int y);
GLvoid GameLoop(int value);

void make_vertexShaders();
void make_fragmentShaders();
void InitBuffer();
void initGame();
void updateGame();

GLuint make_shaderProgram();

GLfloat background[24][3]
=
{
	{-6,-6,-6},
	{6,-6,-6},
	{6,6,-6},
	{-6,6,-6},

	{-6,-6,-6},
	{6,-6,-6},
	{6,-6,6},
	{-6,-6,6},
	
	{-6,-6,-6},
	{-6,6,-6},
	{-6,6,6},
	{-6,-6,6},

	{-6,-6,6},
	{6,-6,6},
	{6,6,6},
	{-6,6,6},

	{6,-6,6},
	{6,-6,-6},
	{6,6,-6},
	{6,6,6},

	{-6,6,-6},
	{6,6,-6},
	{6,6,6},
	{-6,6,6}
};
GLfloat bgColor[24][3]
=
{
	{1,1,1},
	{1,1,1},
	{1,1,1},
	{1,1,1},

	{1,1,1},
	{1,1,1},
	{1,1,1},
	{1,1,1},

	{1,1,1},
	{1,1,1},
	{1,1,1},
	{1,1,1},

	{1,1,1},
	{1,1,1},
	{1,1,1},
	{1,1,1},

	{1,1,1},
	{1,1,1},
	{1,1,1},
	{1,1,1},

	{1,1,1},
	{1,1,1},
	{1,1,1},
	{1,1,1}
};
GLfloat squarePosition[24][3]
=
{
	//����
	{0.5,0.5,0.5},
	{-0.5,0.5,0.5},
	{-0.5,0.5,-0.5},
	{0.5,0.5,-0.5},

	//�Ʒ���
	{0.5,-0.5,0.5},
	{-0.5,-0.5,0.5},
	{-0.5,-0.5,-0.5},
	{0.5,-0.5,-0.5},

	//�ո�
	{0.5,0.5,0.5},
	{0.5,-0.5,0.5},
	{-0.5,-0.5,0.5},
	{-0.5,0.5,0.5},

	//�޸�
	{0.5,0.5,-0.5},
	{0.5,-0.5,-0.5},
	{-0.5,-0.5,-0.5},
	{-0.5,0.5,-0.5},

	//�޸�
	{-0.5,0.5,0.5},
	{-0.5,0.5,-0.5},
	{-0.5,-0.5,-0.5},
	{-0.5,-0.5,0.5},

	//������
	{0.5,0.5,0.5},
	{0.5,0.5,-0.5},
	{0.5,-0.5,-0.5},
	{0.5,-0.5,0.5}
};
GLfloat square[24][3]
=
{
	//����
	{0.5,0.5,0.5},
	{-0.5,0.5,0.5},
	{-0.5,0.5,-0.5},
	{0.5,0.5,-0.5},

	//�Ʒ���
	{0.5,-0.5,0.5},
	{-0.5,-0.5,0.5},
	{-0.5,-0.5,-0.5},
	{0.5,-0.5,-0.5},

	//�ո�
	{0.5,0.5,0.5},
	{0.5,-0.5,0.5},
	{-0.5,-0.5,0.5},
	{-0.5,0.5,0.5},

	//�޸�
	{0.5,0.5,-0.5},
	{0.5,-0.5,-0.5},
	{-0.5,-0.5,-0.5},
	{-0.5,0.5,-0.5},

	//�޸�
	{-0.5,0.5,0.5},
	{-0.5,0.5,-0.5},
	{-0.5,-0.5,-0.5},
	{-0.5,-0.5,0.5},

	//������
	{0.5,0.5,0.5},
	{0.5,0.5,-0.5},
	{0.5,-0.5,-0.5},
	{0.5,-0.5,0.5}
};
GLfloat squareColor[24][3]
=
{
	//����
	{0,0,0},
	{0,0,0},
	{0,0,0},
	{0,0,0},

	//�Ʒ���
	{0,0,0},
	{0,0,0},
	{0,0,0},
	{0,0,0},

	//�ո�
	{0,0,0},
	{0,0,0},
	{0,0,0},
	{0,0,0},

	//�޸�
	{0,0,0},
	{0,0,0},
	{0,0,0},
	{0,0,0},

	//�޸�
	{0,0,0},
	{0,0,0},
	{0,0,0},
	{0,0,0},

	//������
	{0,0,0},
	{0,0,0},
	{0,0,0},
	{0,0,0}
};
GLfloat originColor[24][3]
=
{
	//����
	{0,0,0},
	{0,0,0},
	{0,0,0},
	{0,0,0},

	//�Ʒ���
	{0,0,0},
	{0,0,0},
	{0,0,0},
	{0,0,0},

	//�ո�
	{0,0,0},
	{0,0,0},
	{0,0,0},
	{0,0,0},

	//�޸�
	{0,0,0},
	{0,0,0},
	{0,0,0},
	{0,0,0},

	//�޸�
	{0,0,0},
	{0,0,0},
	{0,0,0},
	{0,0,0},

	//������
	{0,0,0},
	{0,0,0},
	{0,0,0},
	{0,0,0}
};
GLint GameSpace[12][12][12];
GLint tempSpace[12][12][12];


GLuint MatrixID;

glm::mat4 proj = glm::perspective(glm::radians(45.0f), 4.0f / 3.0f, 0.1f, 100.0f);
glm::mat4 camera = glm::lookAt(glm::vec3(12, 12, 24), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
glm::mat4 model = glm::mat4(1.0f);
glm::mat4 trans;
glm::mat4 rot = glm::mat4(1.0f);


int main(int argc, char** argv) //--- ������ ����ϰ� �ݹ��Լ� ����
{
	//--- ������ �����ϱ�
	glutInit(&argc, argv); // glut �ʱ�ȭ
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH); // ���÷��� ��� ����
	glutInitWindowPosition(0, 0); // �������� ��ġ ����
	glutInitWindowSize(800, 600); // �������� ũ�� ����
	glutCreateWindow("project"); // ������ ����(������ �̸�)

	//--- GLEW �ʱ�ȭ�ϱ�
	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK)
	{
		std::cerr << "GLEW �ʱ�ȭ �Ұ���" << std::endl;
		exit(EXIT_FAILURE);
	}
	else
		std::cout << "GLEW�� �ʱ�ȭ��\n";

	glutDisplayFunc(drawScene); // ��� �ݹ��Լ��� ����
	glutReshapeFunc(Reshape); // �ٽ� �׸��� �ݹ��Լ� ����
	glutKeyboardFunc(Keyboard); // Ű���� �Է� �ݹ��Լ� ����
	glutTimerFunc(30, GameLoop, 1);
	vertexShaderSource = filetobuf("vertex1.glsl");
	fragmentShaderSource = filetobuf("fragment1.glsl");

	make_vertexShaders();
	make_fragmentShaders();
	shaderID = make_shaderProgram();
	InitBuffer();
	initGame();
	glutMainLoop(); // �̺�Ʈ ó�� ����
}


GLvoid GameLoop(int value)
{
	updateGame();
	glutPostRedisplay();
	glutTimerFunc(30, GameLoop, 1);
}
void make_vertexShaders()
{
	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	glCompileShader(vertexShader);
	GLint result;
	GLchar errorLog[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &result);
	if (!result)
	{
		glGetShaderInfoLog(vertexShader, 512, NULL, errorLog);
		cerr << "ERROR: vertex shader ������ ����\n" << errorLog << endl;
	}
}

void make_fragmentShaders()
{
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragmentShader);
	GLint result;
	GLchar errorLog[512];
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &result);
	if (!result)
	{
		glGetShaderInfoLog(fragmentShader, 512, NULL, errorLog);
		cerr << "ERROR: fragment shader ������ ����\n" << errorLog << endl;
	}
}

GLuint make_shaderProgram()
{
	GLuint ShaderProgramID;
	GLint result;
	GLchar errorLog[512];
	ShaderProgramID = glCreateProgram();
	glAttachShader(ShaderProgramID, vertexShader); //--- ���̴� ���α׷��� ���ؽ� ���̴� ���̱�
	glAttachShader(ShaderProgramID, fragmentShader); //--- ���̴� ���α׷��� �����׸�Ʈ ���̴� ���̱�
	glLinkProgram(ShaderProgramID); //--- ���̴� ���α׷� ��ũ�ϱ�
	glDeleteShader(vertexShader); //--- ���̴� ���α׷��� ��ũ�Ͽ� ���̴� ��ü ��ü�� ���� ����
	glDeleteShader(fragmentShader);
	glGetProgramiv(ShaderProgramID, GL_LINK_STATUS, &result); // ---���̴��� �� ����Ǿ����� üũ�ϱ�
	if (!result) {
		glGetProgramInfoLog(ShaderProgramID, 512, NULL, errorLog);
		cerr << "ERROR: shader program ���� ����\n" << errorLog << endl;
	}
	return ShaderProgramID;
}

void InitBuffer()
{
	glGenVertexArrays(1, &VAO_bg);
	glGenBuffers(1, &VBO_bg);
	glGenBuffers(1, &VBO_bgColor);

	glBindVertexArray(VAO_bg);
	
	glBindBuffer(GL_ARRAY_BUFFER, VBO_bg);
	glBufferData(GL_ARRAY_BUFFER, sizeof(background), background, GL_STATIC_DRAW);

	GLint pAttribute2 = glGetAttribLocation(shaderID, "vPos");
	glVertexAttribPointer(pAttribute2, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
	glEnableVertexAttribArray(pAttribute2);

	glBindBuffer(GL_ARRAY_BUFFER, VBO_bgColor);
	glBufferData(GL_ARRAY_BUFFER, sizeof(bgColor), bgColor, GL_STATIC_DRAW);

	GLint cAttribute2 = glGetAttribLocation(shaderID, "vColor");
	glVertexAttribPointer(cAttribute2, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
	glEnableVertexAttribArray(cAttribute2);
	for (int i = 0; i < 12; ++i)
	{
		for (int j = 0; j < 12; ++j)
		{
			for (int k = 0; k < 12; ++k)
			{
				glGenVertexArrays(1, &VAO[i][j][k]);
				glGenBuffers(1, &VBO_position[i][j][k]);
				glGenBuffers(1, &VBO_color[i][j][k]);
				
				glBindVertexArray(VAO[i][j][k]);
				//--- ��ġ �Ӽ�
				glBindBuffer(GL_ARRAY_BUFFER, VBO_position[i][j][k]);
				glBufferData(GL_ARRAY_BUFFER, sizeof(squarePosition), squarePosition, GL_STATIC_DRAW);

				GLint pAttribute1 = glGetAttribLocation(shaderID, "vPos");
				glVertexAttribPointer(pAttribute1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
				glEnableVertexAttribArray(pAttribute1);

				glBindBuffer(GL_ARRAY_BUFFER, VBO_color[i][j][k]);
				glBufferData(GL_ARRAY_BUFFER, sizeof(squareColor), squareColor, GL_STATIC_DRAW);

				GLint cAttribute1 = glGetAttribLocation(shaderID, "vColor");
				glVertexAttribPointer(cAttribute1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
				glEnableVertexAttribArray(cAttribute1);
			}
		}
	}
}

GLvoid drawScene()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);
	glUseProgram(shaderID);

	
	trans = proj * camera * model;
	MatrixID = glGetUniformLocation(shaderID, "trans");
	glUniformMatrix4fv(MatrixID, 1, FALSE, glm::value_ptr(trans));

	glBindVertexArray(VAO_bg);

	glBindBuffer(GL_ARRAY_BUFFER, VBO_bg);
	glBufferData(GL_ARRAY_BUFFER, sizeof(background), background, GL_STATIC_DRAW);

	GLint pAttribute2 = glGetAttribLocation(shaderID, "vPos");
	glVertexAttribPointer(pAttribute2, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
	glEnableVertexAttribArray(pAttribute2);

	glBindBuffer(GL_ARRAY_BUFFER, VBO_bgColor);
	glBufferData(GL_ARRAY_BUFFER, sizeof(bgColor), bgColor, GL_STATIC_DRAW);

	GLint cAttribute2 = glGetAttribLocation(shaderID, "vColor");
	glVertexAttribPointer(cAttribute2, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
	glEnableVertexAttribArray(cAttribute2);

	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glDrawArrays(GL_QUADS, 0, 24);

	// 1=red 2=blue 3=green 4=yellow
	for (int x = 0; x < 12; ++x)
	{
		for (int y = 0; y < 12; ++y)
		{
			for (int z = 0; z < 12; ++z)
			{
				glBindVertexArray(VAO[x][y][z]);
				switch (GameSpace[x][z][y])
				{
				case 1:
					for (int i = 0; i < 24; ++i)
					{
						squarePosition[i][0] += (float)x-5.5f;
						squarePosition[i][1] += (float)y-5.5f;
						squarePosition[i][2] += (float)z-5.5f;
						
						squareColor[i][0] = 1.0;
						squareColor[i][1] = 0.0;
						squareColor[i][2] = 0.0;
					}
					break;
				case 2:
					for (int i = 0; i < 24; ++i)
					{
						squarePosition[i][0] += (float)x-5.5f;
						squarePosition[i][1] += (float)y-5.5f;
						squarePosition[i][2] += (float)z-5.5f;

						squareColor[i][0] = 0.0;
						squareColor[i][1] = 1.0;
						squareColor[i][2] = 0.0;
					}
					break;
				case 3:
					for (int i = 0; i < 24; ++i)
					{
						squarePosition[i][0] += (float)x-5.5f;
						squarePosition[i][1] += (float)y-5.5f;
						squarePosition[i][2] += (float)z-5.5f;

						squareColor[i][0] = 0.0;
						squareColor[i][1] = 0.0;
						squareColor[i][2] = 1.0;
					}
					break;
				case 4:
					for (int i = 0; i < 24; ++i)
					{
						squarePosition[i][0] += (float)x-5.5f;
						squarePosition[i][1] += (float)y-5.5f;
						squarePosition[i][2] += (float)z-5.5f;

						squareColor[i][0] = 1.0;
						squareColor[i][1] = 1.0;
						squareColor[i][2] = 0.0;
					}
					break;
				}

				glBindBuffer(GL_ARRAY_BUFFER, VBO_position[x][y][z]);
				glBufferData(GL_ARRAY_BUFFER, sizeof(squarePosition), squarePosition, GL_STATIC_DRAW);

				GLint pAttribute1 = glGetAttribLocation(shaderID, "vPos");
				glVertexAttribPointer(pAttribute1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
				glEnableVertexAttribArray(pAttribute1);

				glBindBuffer(GL_ARRAY_BUFFER, VBO_color[x][y][z]);
				glBufferData(GL_ARRAY_BUFFER, sizeof(squareColor), squareColor, GL_STATIC_DRAW);

				GLint cAttribute1 = glGetAttribLocation(shaderID, "vColor");
				glVertexAttribPointer(cAttribute1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
				glEnableVertexAttribArray(cAttribute1);

				glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
				glDrawArrays(GL_QUADS, 0, 24);


				for (int i = 0; i < 24; ++i)
				{
					for (int j = 0; j < 3; ++j)
					{
						squareColor[i][j] = 0;
					}
				}

				glBindBuffer(GL_ARRAY_BUFFER, VBO_position[x][y][z]);
				glBufferData(GL_ARRAY_BUFFER, sizeof(squarePosition), squarePosition, GL_STATIC_DRAW);

				pAttribute1 = glGetAttribLocation(shaderID, "vPos");
				glVertexAttribPointer(pAttribute1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
				glEnableVertexAttribArray(pAttribute1);

				glBindBuffer(GL_ARRAY_BUFFER, VBO_color[x][y][z]);
				glBufferData(GL_ARRAY_BUFFER, sizeof(squareColor), squareColor, GL_STATIC_DRAW);

				cAttribute1 = glGetAttribLocation(shaderID, "vColor");
				glVertexAttribPointer(cAttribute1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
				glEnableVertexAttribArray(cAttribute1);

				glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
				glDrawArrays(GL_QUADS, 0, 24);
				for (int i = 0; i < 24; ++i)
				{
					for (int j = 0; j < 3; ++j)
					{
						squarePosition[i][j] = square[i][j];
						squareColor[i][j] = originColor[i][j];
					}
				}
			}
		}
	}
	glutSwapBuffers();
}
GLvoid Reshape(int w, int h)
{
	glViewport(0, 0, w, h);
}
GLvoid Keyboard(unsigned char key, int x, int y)
{
	switch (key)
	{
	case 'O':
	case 'o':
		rot = glm::mat4(1.0f);
		rot = glm::rotate(rot, glm::radians(3.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = model * rot;
		break;
	case 'P':
	case 'p':
		rot = glm::mat4(1.0f);
		rot = glm::rotate(rot, glm::radians(3.0f), glm::vec3(0.0f, -1.0f, 0.0f));
		model = model * rot;
		break;

	}
	glutPostRedisplay();
}

void initGame()
{
	for (int i = 0; i < 12; ++i)
	{
		for (int j = 0; j < 12; ++j)
		{
			for (int k = 0; k < 12; ++k)
			{
				GameSpace[i][j][k] = 0;
				tempSpace[i][j][k] = 0;
			}
		}
	}
	tempSpace[0][0][0] = 1;
	tempSpace[0][0][1] = 2;
	tempSpace[0][0][2] = 3;
	tempSpace[0][0][3] = 4;
	tempSpace[1][1][0] = 4;
	tempSpace[2][2][0] = 4;
	tempSpace[3][3][0] = 4;
}
void updateGame()
{
	for (int i = 0; i < 12; ++i)
	{
		for (int j = 0; j < 12; ++j)
		{
			for (int k = 0; k < 12; ++k)
			{
				GameSpace[i][j][k] = tempSpace[i][j][k];
			}
		}
	}
}