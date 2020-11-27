#include <iostream>
#include <gl/glew.h> 
#include <gl/freeglut.h>
#include <gl/freeglut_ext.h>
#include <glm/glm/glm.hpp>
#include <glm/glm/ext.hpp>
#include <glm/glm/gtc/matrix_transform.hpp>
#include <cstdlib>
#include <cstdio>
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
GLuint VAO_preview, VBO_top, VBO_bottom, VBO_pvColor;

GLvoid drawScene(GLvoid);
GLvoid Reshape(int w, int h);
GLvoid Keyboard(unsigned char key, int x, int y);
GLvoid GameLoop(int value);

int frameCount = 0;
int frames_needed = 33;
int current_rot = 0;
void make_vertexShaders();
void make_fragmentShaders();
void InitBuffer();
void initGame();
void updateGame();
int isEnded();
int collides(int dir);
int isGameOver();
void killBlock();
void preview();

int myBlockID;
int myBlock[3][3][3];
int myRotX, myRotY, myRotZ;
int myX, myY, myZ;
int quit,bye;
int canMove;

int blockCount = 0;

//1=S 2=L 3=I 4=T 5=O
int block1[3][3][3]//S
=
{
	{{0,0,0},
	{0,0,0},
	{0,0,0}},

	{{0,0,0},
	{0,2,2},
	{2,2,0}},

	{{0,0,0},
	{0,0,0},
	{0,0,0}}
};
int block2[3][3][3]//L
=
{
	{{0,0,0},
	{0,0,0},
	{0,0,0}},

	{{0,1,0},
	{0,1,0},
	{0,1,1}},

	{{0,0,0},
	{0,0,0},
	{0,0,0}}
};
int block3[3][3][3]//I
=
{
	{{0,0,0},
	{0,0,0},
	{0,0,0}},

	{{0,3,0},
	{0,3,0},
	{0,3,0}},

	{{0,0,0},
	{0,0,0},
	{0,0,0}}
};
int block4[3][3][3]//T
=
{
	{{0,0,0},
	{0,0,0},
	{0,0,0}},

	{{0,0,0},
	{5,5,5},
	{0,5,0}},

	{{0,0,0},
	{0,0,0},
	{0,0,0}}
};
int block5[3][3][3]//O
=
{
	{{0,0,0},
	{0,0,0},
	{0,0,0}},

	{{0,0,0},
	{0,4,4},
	{0,4,4}},

	{{0,0,0},
	{0,0,0},
	{0,0,0}}
};
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

GLfloat top[4][3]
=
{
	{0.5,0.55,0.5},
	{-0.5,0.55,0.5},
	{-0.5,0.55,-0.5},
	{0.5,0.55,-0.5}
};
GLfloat bottom[4][3]
=
{
	{0.5,-0.5,0.5},
	{-0.5,-0.5,0.5},
	{-0.5,-0.5,-0.5},
	{0.5,-0.5,-0.5}
};
GLfloat pvColor[4][3]
=
{
{ 1,1,1 },
{ 1,1,1 },
{ 1,1,1 },
{ 1,1,1 }
};
GLfloat pvTarget[4][3];

GLint GameSpace[12][12][12];
GLint tempSpace[12][12][12];
GLint blockSpace[12][12][12];
GLint previewSpace[12][12][12];

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
	srand((unsigned int)time(NULL));
	InitBuffer();
	initGame();
	glutMainLoop(); // �̺�Ʈ ó�� ����
}

GLvoid GameLoop(int value)
{
	updateGame();
	killBlock();
	glutPostRedisplay();
	if (quit == 1)
	{
		initGame();
	}
	if (bye == 1)
	{
		PostQuitMessage(0);
	}
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
	glGenVertexArrays(1, &VAO_preview);
	glGenBuffers(1, &VBO_top);
	glGenBuffers(1, &VBO_bottom);
	glGenBuffers(1, &VBO_pvColor);

	glBindVertexArray(VAO_preview);
	glBindBuffer(GL_ARRAY_BUFFER, VBO_top);
	glBufferData(GL_ARRAY_BUFFER, sizeof(top), top, GL_STATIC_DRAW);

	GLint pAttribute3 = glGetAttribLocation(shaderID, "vPos");
	glVertexAttribPointer(pAttribute3, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
	glEnableVertexAttribArray(pAttribute3);

	glBindBuffer(GL_ARRAY_BUFFER, VBO_bottom);
	glBufferData(GL_ARRAY_BUFFER, sizeof(bottom), bottom, GL_STATIC_DRAW);

	GLint pAttribute4 = glGetAttribLocation(shaderID, "vPos");
	glVertexAttribPointer(pAttribute4, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
	glEnableVertexAttribArray(pAttribute4);

	glBindBuffer(GL_ARRAY_BUFFER, VBO_pvColor);
	glBufferData(GL_ARRAY_BUFFER, sizeof(pvColor), pvColor, GL_STATIC_DRAW);

	GLint cAttribute3 = glGetAttribLocation(shaderID, "vColor");
	glVertexAttribPointer(cAttribute3, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
	glEnableVertexAttribArray(cAttribute3);

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
	
	// 1=brown 2=green 3=blue 4=yellow 5=purple
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
						
						squareColor[i][0] = 0.8;
						squareColor[i][1] = 0.4;
						squareColor[i][2] = 0.4;
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
						squareColor[i][1] = 0.5;
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
				case 5:
					for (int i = 0; i < 24; ++i)
					{
						squarePosition[i][0] += (float)x - 5.5f;
						squarePosition[i][1] += (float)y - 5.5f;
						squarePosition[i][2] += (float)z - 5.5f;

						squareColor[i][0] = 1.0;
						squareColor[i][1] = 0.0;
						squareColor[i][2] = 1.0;
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

	for (int i = 0; i < 12; ++i)
	{
		for (int j = 0; j < 12; ++j)
		{
			for (int k = 0; k < 12; ++k)
			{
				
				if (previewSpace[i][j][k] == 1)
				{
					for (int t = 0; t < 4; ++t)
					{
						pvTarget[t][0] = top[t][0] + i - 5.5f;
						pvTarget[t][1] = top[t][1] + k - 5.5f;
						pvTarget[t][2] = top[t][2] + j - 5.5f;
					}
					glBindVertexArray(VAO_preview);

					glBindBuffer(GL_ARRAY_BUFFER, VBO_top);
					glBufferData(GL_ARRAY_BUFFER, sizeof(pvTarget), pvTarget, GL_STATIC_DRAW);

					GLint pAttribute3 = glGetAttribLocation(shaderID, "vPos");
					glVertexAttribPointer(pAttribute3, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
					glEnableVertexAttribArray(pAttribute3);

					glBindBuffer(GL_ARRAY_BUFFER, VBO_pvColor);
					glBufferData(GL_ARRAY_BUFFER, sizeof(pvColor), pvColor, GL_STATIC_DRAW);

					GLint cAttribute3 = glGetAttribLocation(shaderID, "vColor");
					glVertexAttribPointer(cAttribute3, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
					glEnableVertexAttribArray(cAttribute3);

					glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
					glDrawArrays(GL_QUADS, 0, 4);
				}
				else if (previewSpace[i][j][k] == -1)
				{
					for (int t = 0; t < 4; ++t)
					{
						pvTarget[t][0] = bottom[t][0] + i - 5.5f;
						pvTarget[t][1] = bottom[t][1] + k - 5.5f;
						pvTarget[t][2] = bottom[t][2] + j - 5.5f;
					}
					glBindVertexArray(VAO_preview);

					glBindBuffer(GL_ARRAY_BUFFER, VBO_bottom);
					glBufferData(GL_ARRAY_BUFFER, sizeof(pvTarget), pvTarget, GL_STATIC_DRAW);

					GLint pAttribute3 = glGetAttribLocation(shaderID, "vPos");
					glVertexAttribPointer(pAttribute3, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
					glEnableVertexAttribArray(pAttribute3);

					glBindBuffer(GL_ARRAY_BUFFER, VBO_pvColor);
					glBufferData(GL_ARRAY_BUFFER, sizeof(pvColor), pvColor, GL_STATIC_DRAW);

					GLint cAttribute3 = glGetAttribLocation(shaderID, "vColor");
					glVertexAttribPointer(cAttribute3, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
					glEnableVertexAttribArray(cAttribute3);

					glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
					glDrawArrays(GL_QUADS, 0, 4);
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
	case 'o'://ȸ��
		rot = glm::mat4(1.0f);
		rot = glm::rotate(rot, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		current_rot = (current_rot + 1) % 4;
		model = model * rot;
		break;
	case 'j':
	case 'J':
		if (myY < 0)
		{
			myY = 0;
		}
		if (myZ < 0)
		{
			myZ = 0;
		}
		if (myY > 9)
		{
			myY = 9;
		}
		if (myZ > 9)
		{
			myZ = 9;
		}
		myRotX += 1;
		myRotX = myRotX % 4;// ȸ���� Ƚ�� 1 ������Ű�� 4�������� 0���� �ʱ�ȭ
		break;
	case 'k':
	case 'K':
		if (myX < 0)
		{
			myX = 0;
		}
		if (myZ < 0)
		{
			myZ = 0;
		}
		if (myX > 9)
		{
			myX = 9;
		}
		if (myZ > 9)
		{
			myZ = 9;
		}
		myRotY += 1;
		myRotY = myRotY % 4;// ȸ���� Ƚ�� 1 ������Ű�� 4�������� 0���� �ʱ�ȭ
		break;
	case 'l':
	case 'L':
		if (myY < 0)
		{
			myY = 0;
		}
		if (myX < 0)
		{
			myX = 0;
		}
		if (myY > 9)
		{
			myY = 9;
		}
		if (myX > 9)
		{
			myX = 9;
		}
		myRotZ += 1;
		myRotZ = myRotZ % 4;// ȸ���� Ƚ�� 1 ������Ű�� 4�������� 0���� �ʱ�ȭ
		break;
	case 'w':
	case 'W':
		if (current_rot == 0)
		{
			if (collides(1) == 0)
			{
				myY -= 1;
			}
		}
		else if (current_rot == 1)
		{
			if (collides(3) == 0)
			{
				myX -= 1;
			}
		}
		else if (current_rot == 2)
		{
			if (collides(2) == 0)
			{
				myY += 1;
			}
		}
		else if (current_rot == 3)
		{
			if (collides(4) == 0)
			{
				myX += 1;
			}
		}
		break;
	case 's':
	case 'S':
		if (current_rot == 0)
		{
			if (collides(2) == 0)
			{
				myY += 1;
			}
		}
		else if (current_rot == 1)
		{
			if (collides(4) == 0)
			{
				myX += 1;
			}
		}
		else if (current_rot == 2)
		{
			if (collides(1) == 0)
			{
				myY -= 1;
			}
		}
		else if (current_rot == 3)
		{
			if (collides(3) == 0)
			{
				myX -= 1;
			}
		}
		break;
	case 'a':
	case 'A':
		if (current_rot == 0)
		{
			if (collides(3) == 0)
			{
				myX -= 1;
			}
		}
		else if (current_rot == 1)
		{
			if (collides(2) == 0)
			{
				myY += 1;
			}
		}
		else if (current_rot == 2)
		{
			if (collides(4) == 0)
			{
				myX += 1;
			}
		}
		else if (current_rot == 3)
		{
			if (collides(1) == 0)
			{
				myY -= 1;
			}
		}
		break;
	case 'd':
	case 'D':
		if (current_rot == 0)
		{
			if (collides(4) == 0)
			{
				myX += 1;
			}
		}
		else if (current_rot == 1)
		{
			if (collides(1) == 0)
			{
				myY -= 1;
			}
		}
		else if (current_rot == 2)
		{
			if (collides(3) == 0)
			{
				myX -= 1;
			}
		}
		else if (current_rot == 3)
		{
			if (collides(2) == 0)
			{
				myY += 1;
			}
		}
		break;
	case 32:
		while (isEnded() == 0)
		{
			myZ -= 1;
		}
		for (int i = 0; i < 3; ++i)
		{
			for (int j = 0; j < 3; ++j)
			{
				for (int k = 0; k < 3; ++k)
				{
					tempSpace[myX + i][myY + j][myZ + k] += myBlock[i][j][k];
				}
			}
		}
		myBlockID = (rand() % 5) + 1;
		myRotX = 0;
		myRotY = 0;
		myRotZ = 0;
		myZ = 9;
		if (myX > 9)
		{
			myX = 9;
		}
		if (myX < 0)
		{
			myX = 0;
		}
		if (myY > 9)
		{
			myY = 9;
		}
		if (myY < 0)
		{
			myY = 0;
		}
		if (isGameOver() == 1)
		{
			quit = 1;
		}
		break;
	case 27:
		quit = 1;
		break;
	}
	glutPostRedisplay();
}


void initGame()
{
	//���� ���� �����
	for (int i = 0; i < 12; ++i)
	{
		for (int j = 0; j < 12; ++j)
		{
			for (int k = 0; k < 12; ++k)
			{
				GameSpace[i][j][k] = 0;
				tempSpace[i][j][k] = 0;
				blockSpace[i][j][k] = 0;
				previewSpace[i][j][k] = 0;
			}
		}
	}
	quit = 0;
	bye = 0;
	blockCount = 0;
	myBlockID = 1;
	myRotX = 0;
	myRotY = 0;
	myRotZ = 0;
	myX = 5;
	myY = 5;
	myZ = 9;
}
void updateGame()
{
	//����� S��
	int ok;
	if (myBlockID == 1)
	{
		for (int i = 0; i < 3; ++i)//S�� ��� ������ ����
		{
			for (int j = 0; j < 3; ++j)
			{
				for (int k = 0; k < 3; ++k)
				{
					myBlock[i][j][k] = block1[i][j][k];
				}
			}
		}
		//x�� �������� Ű���� ������ŭ ȸ��
		for (int n = 0; n < myRotX; ++n)
		{
			for (int x = 0; x < 3; ++x)
			{
				for (int i = 0; i < 2; ++i)
				{
					int temp00 = myBlock[x][0][0];
					myBlock[x][0][0] = myBlock[x][1][0];
					myBlock[x][1][0] = myBlock[x][2][0];
					myBlock[x][2][0] = myBlock[x][2][1];
					myBlock[x][2][1] = myBlock[x][2][2];
					myBlock[x][2][2] = myBlock[x][1][2];
					myBlock[x][1][2] = myBlock[x][0][2];
					myBlock[x][0][2] = myBlock[x][0][1];
					myBlock[x][0][1] = temp00;
				}
			}
		}
			
		//y�� �������� Ű���� ������ŭ ȸ��
		for (int n = 0; n < myRotY; ++n)
		{
			for (int y = 0; y < 3; ++y)
			{
				for (int i = 0; i < 2; ++i)
				{
					int temp = myBlock[0][y][0];
					myBlock[0][y][0] = myBlock[1][y][0];
					myBlock[1][y][0] = myBlock[2][y][0];
					myBlock[2][y][0] = myBlock[2][y][1];
					myBlock[2][y][1] = myBlock[2][y][2];
					myBlock[2][y][2] = myBlock[1][y][2];
					myBlock[1][y][2] = myBlock[0][y][2];
					myBlock[0][y][2] = myBlock[0][y][1];
					myBlock[0][y][1] = temp;
				}
			}
		}
		
		//z�� �������� Ű���� ������ŭ ȸ��
		for (int n = 0; n < myRotZ; ++n)
		{
			for (int z = 0; z < 3; ++z)
			{
				for (int i = 0; i < 2; ++i)
				{
					int temp = myBlock[0][0][z];
					myBlock[0][0][z] = myBlock[1][0][z];
					myBlock[1][0][z] = myBlock[2][0][z];
					myBlock[2][0][z] = myBlock[2][1][z];
					myBlock[2][1][z] = myBlock[2][2][z];
					myBlock[2][2][z] = myBlock[1][2][z];
					myBlock[1][2][z] = myBlock[0][2][z];
					myBlock[0][2][z] = myBlock[0][1][z];
					myBlock[0][1][z] = temp;
				}
			}
		}
	}
	else if (myBlockID == 2)
	{
		for (int i = 0; i < 3; ++i)//S�� ��� ������ ����
		{
			for (int j = 0; j < 3; ++j)
			{
				for (int k = 0; k < 3; ++k)
				{
					myBlock[i][j][k] = block2[i][j][k];
				}
			}
		}
		//x�� �������� Ű���� ������ŭ ȸ��
		for (int n = 0; n < myRotX; ++n)
		{
			for (int x = 0; x < 3; ++x)
			{
				for (int i = 0; i < 2; ++i)
				{
					int temp00 = myBlock[x][0][0];
					myBlock[x][0][0] = myBlock[x][1][0];
					myBlock[x][1][0] = myBlock[x][2][0];
					myBlock[x][2][0] = myBlock[x][2][1];
					myBlock[x][2][1] = myBlock[x][2][2];
					myBlock[x][2][2] = myBlock[x][1][2];
					myBlock[x][1][2] = myBlock[x][0][2];
					myBlock[x][0][2] = myBlock[x][0][1];
					myBlock[x][0][1] = temp00;
				}
			}
		}

		//y�� �������� Ű���� ������ŭ ȸ��
		for (int n = 0; n < myRotY; ++n)
		{
			for (int y = 0; y < 3; ++y)
			{
				for (int i = 0; i < 2; ++i)
				{
					int temp = myBlock[0][y][0];
					myBlock[0][y][0] = myBlock[1][y][0];
					myBlock[1][y][0] = myBlock[2][y][0];
					myBlock[2][y][0] = myBlock[2][y][1];
					myBlock[2][y][1] = myBlock[2][y][2];
					myBlock[2][y][2] = myBlock[1][y][2];
					myBlock[1][y][2] = myBlock[0][y][2];
					myBlock[0][y][2] = myBlock[0][y][1];
					myBlock[0][y][1] = temp;
				}
			}
		}

		//z�� �������� Ű���� ������ŭ ȸ��
		for (int n = 0; n < myRotZ; ++n)
		{
			for (int z = 0; z < 3; ++z)
			{
				for (int i = 0; i < 2; ++i)
				{
					int temp = myBlock[0][0][z];
					myBlock[0][0][z] = myBlock[1][0][z];
					myBlock[1][0][z] = myBlock[2][0][z];
					myBlock[2][0][z] = myBlock[2][1][z];
					myBlock[2][1][z] = myBlock[2][2][z];
					myBlock[2][2][z] = myBlock[1][2][z];
					myBlock[1][2][z] = myBlock[0][2][z];
					myBlock[0][2][z] = myBlock[0][1][z];
					myBlock[0][1][z] = temp;
				}
			}
		}
	}
	else if (myBlockID == 3)
	{
		for (int i = 0; i < 3; ++i)//S�� ��� ������ ����
		{
			for (int j = 0; j < 3; ++j)
			{
				for (int k = 0; k < 3; ++k)
				{
					myBlock[i][j][k] = block3[i][j][k];
				}
			}
		}
		//x�� �������� Ű���� ������ŭ ȸ��
		for (int n = 0; n < myRotX; ++n)
		{
			for (int x = 0; x < 3; ++x)
			{
				for (int i = 0; i < 2; ++i)
				{
					int temp00 = myBlock[x][0][0];
					myBlock[x][0][0] = myBlock[x][1][0];
					myBlock[x][1][0] = myBlock[x][2][0];
					myBlock[x][2][0] = myBlock[x][2][1];
					myBlock[x][2][1] = myBlock[x][2][2];
					myBlock[x][2][2] = myBlock[x][1][2];
					myBlock[x][1][2] = myBlock[x][0][2];
					myBlock[x][0][2] = myBlock[x][0][1];
					myBlock[x][0][1] = temp00;
				}
			}
		}

		//y�� �������� Ű���� ������ŭ ȸ��
		for (int n = 0; n < myRotY; ++n)
		{
			for (int y = 0; y < 3; ++y)
			{
				for (int i = 0; i < 2; ++i)
				{
					int temp = myBlock[0][y][0];
					myBlock[0][y][0] = myBlock[1][y][0];
					myBlock[1][y][0] = myBlock[2][y][0];
					myBlock[2][y][0] = myBlock[2][y][1];
					myBlock[2][y][1] = myBlock[2][y][2];
					myBlock[2][y][2] = myBlock[1][y][2];
					myBlock[1][y][2] = myBlock[0][y][2];
					myBlock[0][y][2] = myBlock[0][y][1];
					myBlock[0][y][1] = temp;
				}
			}
		}

		//z�� �������� Ű���� ������ŭ ȸ��
		for (int n = 0; n < myRotZ; ++n)
		{
			for (int z = 0; z < 3; ++z)
			{
				for (int i = 0; i < 2; ++i)
				{
					int temp = myBlock[0][0][z];
					myBlock[0][0][z] = myBlock[1][0][z];
					myBlock[1][0][z] = myBlock[2][0][z];
					myBlock[2][0][z] = myBlock[2][1][z];
					myBlock[2][1][z] = myBlock[2][2][z];
					myBlock[2][2][z] = myBlock[1][2][z];
					myBlock[1][2][z] = myBlock[0][2][z];
					myBlock[0][2][z] = myBlock[0][1][z];
					myBlock[0][1][z] = temp;
				}
			}
		}
	}
	else if (myBlockID == 4)
	{
		for (int i = 0; i < 3; ++i)//S�� ��� ������ ����
		{
			for (int j = 0; j < 3; ++j)
			{
				for (int k = 0; k < 3; ++k)
				{
					myBlock[i][j][k] = block4[i][j][k];
				}
			}
		}
		//x�� �������� Ű���� ������ŭ ȸ��
		for (int n = 0; n < myRotX; ++n)
		{
			for (int x = 0; x < 3; ++x)
			{
				for (int i = 0; i < 2; ++i)
				{
					int temp00 = myBlock[x][0][0];
					myBlock[x][0][0] = myBlock[x][1][0];
					myBlock[x][1][0] = myBlock[x][2][0];
					myBlock[x][2][0] = myBlock[x][2][1];
					myBlock[x][2][1] = myBlock[x][2][2];
					myBlock[x][2][2] = myBlock[x][1][2];
					myBlock[x][1][2] = myBlock[x][0][2];
					myBlock[x][0][2] = myBlock[x][0][1];
					myBlock[x][0][1] = temp00;
				}
			}
		}

		//y�� �������� Ű���� ������ŭ ȸ��
		for (int n = 0; n < myRotY; ++n)
		{
			for (int y = 0; y < 3; ++y)
			{
				for (int i = 0; i < 2; ++i)
				{
					int temp = myBlock[0][y][0];
					myBlock[0][y][0] = myBlock[1][y][0];
					myBlock[1][y][0] = myBlock[2][y][0];
					myBlock[2][y][0] = myBlock[2][y][1];
					myBlock[2][y][1] = myBlock[2][y][2];
					myBlock[2][y][2] = myBlock[1][y][2];
					myBlock[1][y][2] = myBlock[0][y][2];
					myBlock[0][y][2] = myBlock[0][y][1];
					myBlock[0][y][1] = temp;
				}
			}
		}

		//z�� �������� Ű���� ������ŭ ȸ��
		for (int n = 0; n < myRotZ; ++n)
		{
			for (int z = 0; z < 3; ++z)
			{
				for (int i = 0; i < 2; ++i)
				{
					int temp = myBlock[0][0][z];
					myBlock[0][0][z] = myBlock[1][0][z];
					myBlock[1][0][z] = myBlock[2][0][z];
					myBlock[2][0][z] = myBlock[2][1][z];
					myBlock[2][1][z] = myBlock[2][2][z];
					myBlock[2][2][z] = myBlock[1][2][z];
					myBlock[1][2][z] = myBlock[0][2][z];
					myBlock[0][2][z] = myBlock[0][1][z];
					myBlock[0][1][z] = temp;
				}
			}
		}
	}
	else if (myBlockID == 5)
	{
		for (int i = 0; i < 3; ++i)//S�� ��� ������ ����
		{
			for (int j = 0; j < 3; ++j)
			{
				for (int k = 0; k < 3; ++k)
				{
					myBlock[i][j][k] = block5[i][j][k];
				}
			}
		}
		//x�� �������� Ű���� ������ŭ ȸ��
		for (int n = 0; n < myRotX; ++n)
		{
			for (int x = 0; x < 3; ++x)
			{
				for (int i = 0; i < 2; ++i)
				{
					int temp00 = myBlock[x][0][0];
					myBlock[x][0][0] = myBlock[x][1][0];
					myBlock[x][1][0] = myBlock[x][2][0];
					myBlock[x][2][0] = myBlock[x][2][1];
					myBlock[x][2][1] = myBlock[x][2][2];
					myBlock[x][2][2] = myBlock[x][1][2];
					myBlock[x][1][2] = myBlock[x][0][2];
					myBlock[x][0][2] = myBlock[x][0][1];
					myBlock[x][0][1] = temp00;
				}
			}
		}

		//y�� �������� Ű���� ������ŭ ȸ��
		for (int n = 0; n < myRotY; ++n)
		{
			for (int y = 0; y < 3; ++y)
			{
				for (int i = 0; i < 2; ++i)
				{
					int temp = myBlock[0][y][0];
					myBlock[0][y][0] = myBlock[1][y][0];
					myBlock[1][y][0] = myBlock[2][y][0];
					myBlock[2][y][0] = myBlock[2][y][1];
					myBlock[2][y][1] = myBlock[2][y][2];
					myBlock[2][y][2] = myBlock[1][y][2];
					myBlock[1][y][2] = myBlock[0][y][2];
					myBlock[0][y][2] = myBlock[0][y][1];
					myBlock[0][y][1] = temp;
				}
			}
		}

		//z�� �������� Ű���� ������ŭ ȸ��
		for (int n = 0; n < myRotZ; ++n)
		{
			for (int z = 0; z < 3; ++z)
			{
				for (int i = 0; i < 2; ++i)
				{
					int temp = myBlock[0][0][z];
					myBlock[0][0][z] = myBlock[1][0][z];
					myBlock[1][0][z] = myBlock[2][0][z];
					myBlock[2][0][z] = myBlock[2][1][z];
					myBlock[2][1][z] = myBlock[2][2][z];
					myBlock[2][2][z] = myBlock[1][2][z];
					myBlock[1][2][z] = myBlock[0][2][z];
					myBlock[0][2][z] = myBlock[0][1][z];
					myBlock[0][1][z] = temp;
				}
			}
		}
	}
	
	//�ӽð��� ����
	for (int i = 0; i < 12; ++i)
	{
		for (int j = 0; j < 12; ++j)
		{
			for (int k = 0; k < 12; ++k)
			{
				blockSpace[i][j][k] = 0;
				previewSpace[i][j][k] = 0;
			}
		}
	}
	//���� ������ �ִ� ��ϸ� �ӽð����� ����
	for (int i = 0; i < 3; ++i)
	{
		for (int j = 0; j < 3; ++j)
		{
			for (int k = 0; k < 3; ++k)
			{
				if ((myX + i <= 11 && myY + j <= 11 && myZ + k <= 11) && (myX+i>=0 && myY+j>=0 &&myZ+k>=0))
				blockSpace[myX + i][myY + j][myZ + k] = myBlock[i][j][k];
			}
		}
	}
	//�ӽð����� ó�� �Ϸ� �� ���Ӱ����� ����
	for (int i = 0; i < 12; ++i)
	{
		for (int j = 0; j < 12; ++j)
		{
			for (int k = 0; k < 12; ++k)
			{
				GameSpace[i][j][k] = tempSpace[i][j][k]+blockSpace[i][j][k];
			}
		}
	}
	preview();
	frameCount += 1;
	/*
	if (frameCount == frames_needed)
	{
		if (isEnded() == 0)
		{
			myZ -= 1;
		}
		else
		{
			for (int i = 0; i < 3; ++i)
			{
				for (int j = 0; j < 3; ++j)
				{
					for (int k = 0; k < 3; ++k)
					{
						tempSpace[myX + i][myY + j][myZ + k] += myBlock[i][j][k];
					}
				}
			}
			myBlockID = (rand() % 5) + 1;
			myRotX = 0;
			myRotY = 0;
			myRotZ = 0;
			myZ = 9;
			if (myX > 9)
			{
				myX = 9;
			}
			if (myX < 0)
			{
				myX = 0;
			}
			if (myY > 9)
			{
				myY = 9;
			}
			if (myY < 0)
			{
				myY = 0;
			}
			if (isGameOver() == 1)
			{
				quit = 1;
			}
		}
		frameCount = 0;
	}
	*/
}

void killBlock()
{
	int target = -1;
	int t = 1;
	for (int z = 0; z < 12; ++z)
	{
		for (int x = 0; x < 12; ++x)
		{
			for (int y = 0; y < 12; ++y)
			{
				if (tempSpace[x][y][z] == 0)
				{
					t = 0;
				}
			}
		}
		if (t)
		{
			target = z;
			break;
		}
	}
	if (target != -1)
	{
		for (int z = target; z < 11; ++z)
		{
			for (int x = 0; x < 12; ++x)
			{
				for (int y = 0; y < 12; ++y)
				{
					int temp;
					temp = tempSpace[x][y][z];
					tempSpace[x][y][z] = tempSpace[x][y][z + 1];
					tempSpace[x][y][z + 1] = temp;
				}
			}
		}
		for (int x = 0; x < 12; ++x)
		{
			for (int y = 0; y < 12; ++y)
			{
				tempSpace[x][y][11] = 0;
			}
		}
		if(frames_needed > 15)
			frames_needed -= 1;
	}
}


void preview()
{
	
	for (int z = 0; z < 12; ++z)
	{
		for (int x = 0; x < 12; ++x)
		{
			for (int y = 0; y < 12; ++y)
			{
				if (blockSpace[x][y][z] > 0)
				{
					int exists = 0;
					for (int tz = z; tz >= 0; --tz)
					{
						if (tempSpace[x][y][tz] > 0)
						{
							previewSpace[x][y][tz] = 1;
							exists = 1;
							break;
						}
					}
					if (exists == 0)
					{
						previewSpace[x][y][0] = -1;
					}
				}
			}
		}
	}
}

int isEnded()
{
	int ended=0;
	for (int x = 0; x < 3; ++x)
	{
		for (int y = 0; y < 3; ++y)
		{
			for (int z = 0; z < 3; ++z)
			{
				if (myBlock[x][y][z] > 0)
				{
					if (myZ + z > 0)
					{
						if (tempSpace[myX + x][myY + y][myZ + z - 1] > 0)
						{
							ended= 1;
						}
					}
					else if (myZ + z == 0)
					{
						ended = 1;
					}
				}
			}
		}
	}
	return ended;
}

int collides(int direction)
{
	int collide = 0;
	for (int x = 0; x < 3; ++x)
	{
		for (int y = 0; y < 3; ++y)
		{
			for (int z = 0; z < 3; ++z)
			{
				if (direction == 1)//up
				{
					if (myBlock[x][y][z] > 0)//���� �ְ�
					{
						if (myY + y > 0)//y��ǥ�� 0 �̻�
						{
							if (tempSpace[myX + x][myY - 1 + y][myZ + z] > 0)//���Ӱ����� �̹� ���� ������
							{
								collide = 1;
							}
						}
						else
						{
							collide = 1;
						}
					}
				}
				else if (direction == 2)//down
				{
					if (myBlock[x][y][z] > 0)//���� �ְ�
					{
						if (myY + y < 11)//y��ǥ�� 11 �̸�
						{
							if (tempSpace[myX + x][myY + 1 + y][myZ + z] > 0)//���Ӱ����� �̹� ���� ������
							{
								collide = 1;
							}
						}
						else
						{
							collide = 1;
						}
					}
				}
				else if (direction == 3)//left
				{
					if (myBlock[x][y][z] > 0)//���� �ְ�
					{
						if (myX + x > 0)//x��ǥ�� 0���� ŭ
						{
							if (tempSpace[myX + x-1][myY + y][myZ + z] > 0)//���Ӱ����� �̹� ���� ������
							{
								collide = 1;
							}
						}
						else
						{
							collide = 1;
						}
					}
				}
				else if (direction == 4)//right
				{
					if (myBlock[x][y][z] > 0)//���� �ְ�
					{
						if (myX + x < 11)//x��ǥ�� 11 �̸�
						{
							if (tempSpace[myX + x+1][myY + y][myZ + z] > 0)//���Ӱ����� �̹� ���� ������
							{
								collide = 1;
							}
						}
						else
						{
							collide = 1;
						}
					}
				}


			}
		}
	}
	return collide;
}

int isGameOver()
{
	int over = 0;
	for (int x = 0; x < 3; ++x)
	{
		for (int y = 0; y < 3; ++y)
		{
			for (int z = 0; z < 3; ++z)
			{
				if (myBlock[x][y][z] > 0)
				{
					if (tempSpace[myX + x][myY + y][myZ + z] > 0)
					{
						over = 1;
					}
				}
			}
		}
	}
	return over;
}