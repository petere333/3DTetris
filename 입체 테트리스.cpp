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
void clearSpace();

int myBlockID;
int myBlock[3][3][3];
int myRotX, myRotY, myRotZ;
int myX, myY, myZ;

int canMove;
//1=S 2=L 3=I 4=T 5=O
int block1[3][3][3]
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
	//윗면
	{0.5,0.5,0.5},
	{-0.5,0.5,0.5},
	{-0.5,0.5,-0.5},
	{0.5,0.5,-0.5},

	//아랫면
	{0.5,-0.5,0.5},
	{-0.5,-0.5,0.5},
	{-0.5,-0.5,-0.5},
	{0.5,-0.5,-0.5},

	//앞면
	{0.5,0.5,0.5},
	{0.5,-0.5,0.5},
	{-0.5,-0.5,0.5},
	{-0.5,0.5,0.5},

	//뒷면
	{0.5,0.5,-0.5},
	{0.5,-0.5,-0.5},
	{-0.5,-0.5,-0.5},
	{-0.5,0.5,-0.5},

	//왼면
	{-0.5,0.5,0.5},
	{-0.5,0.5,-0.5},
	{-0.5,-0.5,-0.5},
	{-0.5,-0.5,0.5},

	//오른면
	{0.5,0.5,0.5},
	{0.5,0.5,-0.5},
	{0.5,-0.5,-0.5},
	{0.5,-0.5,0.5}
};
GLfloat square[24][3]
=
{
	//윗면
	{0.5,0.5,0.5},
	{-0.5,0.5,0.5},
	{-0.5,0.5,-0.5},
	{0.5,0.5,-0.5},

	//아랫면
	{0.5,-0.5,0.5},
	{-0.5,-0.5,0.5},
	{-0.5,-0.5,-0.5},
	{0.5,-0.5,-0.5},

	//앞면
	{0.5,0.5,0.5},
	{0.5,-0.5,0.5},
	{-0.5,-0.5,0.5},
	{-0.5,0.5,0.5},

	//뒷면
	{0.5,0.5,-0.5},
	{0.5,-0.5,-0.5},
	{-0.5,-0.5,-0.5},
	{-0.5,0.5,-0.5},

	//왼면
	{-0.5,0.5,0.5},
	{-0.5,0.5,-0.5},
	{-0.5,-0.5,-0.5},
	{-0.5,-0.5,0.5},

	//오른면
	{0.5,0.5,0.5},
	{0.5,0.5,-0.5},
	{0.5,-0.5,-0.5},
	{0.5,-0.5,0.5}
};
GLfloat squareColor[24][3]
=
{
	//윗면
	{0,0,0},
	{0,0,0},
	{0,0,0},
	{0,0,0},

	//아랫면
	{0,0,0},
	{0,0,0},
	{0,0,0},
	{0,0,0},

	//앞면
	{0,0,0},
	{0,0,0},
	{0,0,0},
	{0,0,0},

	//뒷면
	{0,0,0},
	{0,0,0},
	{0,0,0},
	{0,0,0},

	//왼면
	{0,0,0},
	{0,0,0},
	{0,0,0},
	{0,0,0},

	//오른면
	{0,0,0},
	{0,0,0},
	{0,0,0},
	{0,0,0}
};
GLfloat originColor[24][3]
=
{
	//윗면
	{0,0,0},
	{0,0,0},
	{0,0,0},
	{0,0,0},

	//아랫면
	{0,0,0},
	{0,0,0},
	{0,0,0},
	{0,0,0},

	//앞면
	{0,0,0},
	{0,0,0},
	{0,0,0},
	{0,0,0},

	//뒷면
	{0,0,0},
	{0,0,0},
	{0,0,0},
	{0,0,0},

	//왼면
	{0,0,0},
	{0,0,0},
	{0,0,0},
	{0,0,0},

	//오른면
	{0,0,0},
	{0,0,0},
	{0,0,0},
	{0,0,0}
};
GLint GameSpace[12][12][12];
GLint tempSpace[12][12][12];
GLint blockSpace[12][12][12];

GLuint MatrixID;

glm::mat4 proj = glm::perspective(glm::radians(45.0f), 4.0f / 3.0f, 0.1f, 100.0f);
glm::mat4 camera = glm::lookAt(glm::vec3(12, 12, 24), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
glm::mat4 model = glm::mat4(1.0f);
glm::mat4 trans;
glm::mat4 rot = glm::mat4(1.0f);


int main(int argc, char** argv) //--- 윈도우 출력하고 콜백함수 설정
{
	//--- 윈도우 생성하기
	glutInit(&argc, argv); // glut 초기화
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH); // 디스플레이 모드 설정
	glutInitWindowPosition(0, 0); // 윈도우의 위치 지정
	glutInitWindowSize(800, 600); // 윈도우의 크기 지정
	glutCreateWindow("project"); // 윈도우 생성(윈도우 이름)

	//--- GLEW 초기화하기
	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK)
	{
		std::cerr << "GLEW 초기화 불가능" << std::endl;
		exit(EXIT_FAILURE);
	}
	else
		std::cout << "GLEW가 초기화됨\n";

	glutDisplayFunc(drawScene); // 출력 콜백함수의 지정
	glutReshapeFunc(Reshape); // 다시 그리기 콜백함수 지정
	glutKeyboardFunc(Keyboard); // 키보드 입력 콜백함수 지정
	glutTimerFunc(30, GameLoop, 1);
	vertexShaderSource = filetobuf("vertex1.glsl");
	fragmentShaderSource = filetobuf("fragment1.glsl");

	make_vertexShaders();
	make_fragmentShaders();
	shaderID = make_shaderProgram();
	InitBuffer();
	initGame();
	glutMainLoop(); // 이벤트 처리 시작
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
		cerr << "ERROR: vertex shader 컴파일 실패\n" << errorLog << endl;
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
		cerr << "ERROR: fragment shader 컴파일 실패\n" << errorLog << endl;
	}
}

GLuint make_shaderProgram()
{
	GLuint ShaderProgramID;
	GLint result;
	GLchar errorLog[512];
	ShaderProgramID = glCreateProgram();
	glAttachShader(ShaderProgramID, vertexShader); //--- 세이더 프로그램에 버텍스 세이더 붙이기
	glAttachShader(ShaderProgramID, fragmentShader); //--- 세이더 프로그램에 프래그먼트 세이더 붙이기
	glLinkProgram(ShaderProgramID); //--- 세이더 프로그램 링크하기
	glDeleteShader(vertexShader); //--- 세이더 프로그램에 링크하여 세이더 객체 자체는 삭제 가능
	glDeleteShader(fragmentShader);
	glGetProgramiv(ShaderProgramID, GL_LINK_STATUS, &result); // ---세이더가 잘 연결되었는지 체크하기
	if (!result) {
		glGetProgramInfoLog(ShaderProgramID, 512, NULL, errorLog);
		cerr << "ERROR: shader program 연결 실패\n" << errorLog << endl;
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
				//--- 위치 속성
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
	case 'o'://회전
		rot = glm::mat4(1.0f);
		rot = glm::rotate(rot, glm::radians(3.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = model * rot;
		break;
	case 'P':
	case 'p'://역회전
		rot = glm::mat4(1.0f);
		rot = glm::rotate(rot, glm::radians(3.0f), glm::vec3(0.0f, -1.0f, 0.0f));
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
		myRotX = myRotX % 4;// 회전할 횟수 1 증가시키고 4번단위로 0으로 초기화
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
		myRotY = myRotY % 4;// 회전할 횟수 1 증가시키고 4번단위로 0으로 초기화
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
		myRotZ = myRotZ % 4;// 회전할 횟수 1 증가시키고 4번단위로 0으로 초기화
		break;
	case 'w':
	case 'W':
		for (int i = 0; i < 3; ++i)
		{
			for (int j = 0; j < 3; ++j)
			{
				for (int k = 0; k < 3; ++k)
				{
					if ((myX + i <= 11 && myY + j <= 11 && myZ + k <= 11) && (myX + i >= 0 && myY + j >= 0 && myZ + k >= 0))
						blockSpace[myX + i][myY + j][myZ + k] = 0;
				}
			}
		}
		canMove = 1;
		for (int i = 0; i < 3; ++i)
		{
			for (int j = 0; j < 3; ++j)
			{
				for (int k = 0; k < 3; ++k)
				{
					if (myBlock[i][j][k] > 0 && myY + j <= 0)
					{
						canMove = 0;
					}
				}
			}
		}
		if (canMove)
		{
			myY -= 1;
		}
		break;
	case 's':
	case 'S':
		for (int i = 0; i < 3; ++i)
		{
			for (int j = 0; j < 3; ++j)
			{
				for (int k = 0; k < 3; ++k)
				{
					if ((myX + i <= 11 && myY + j <= 11 && myZ + k <= 11) && (myX + i >= 0 && myY + j >= 0 && myZ + k >= 0))
					blockSpace[myX + i][myY + j][myZ + k] = 0;
				}
			}
		}
		canMove = 1;
		for (int i = 0; i < 3; ++i)
		{
			for (int j = 0; j < 3; ++j)
			{
				for (int k = 0; k < 3; ++k)
				{
					if (myBlock[i][j][k] > 0 && myY + j >= 11)
					{
						canMove = 0;
					}
				}
			}
		}
		if (canMove)
		{
			myY += 1;
		}
		break;
	case 'a':
	case 'A':
		for (int i = 0; i < 3; ++i)
		{
			for (int j = 0; j < 3; ++j)
			{
				for (int k = 0; k < 3; ++k)
				{
					if ((myX + i <= 11 && myY + j <= 11 && myZ + k <= 11) && (myX + i >= 0 && myY + j >= 0 && myZ + k >= 0))
					blockSpace[myX + i][myY + j][myZ + k] = 0;
				}
			}
		}
		canMove = 1;
		for (int i = 0; i < 3; ++i)
		{
			for (int j = 0; j < 3; ++j)
			{
				for (int k = 0; k < 3; ++k)
				{
					if (myBlock[i][j][k] > 0 && myX + i <= 0)
					{
						canMove = 0;
					}
				}
			}
		}
		if (canMove)
		{
			myX -= 1;
		}
		break;
	case 'd':
	case 'D':
		for (int i = 0; i < 3; ++i)
		{
			for (int j = 0; j < 3; ++j)
			{
				for (int k = 0; k < 3; ++k)
				{
					if ((myX + i <= 11 && myY + j <= 11 && myZ + k <= 11) && (myX + i >= 0 && myY + j >= 0 && myZ + k >= 0))
					blockSpace[myX + i][myY + j][myZ + k] = 0;
				}
			}
		}
		canMove = 1;
		for (int i = 0; i < 3; ++i)
		{
			for (int j = 0; j < 3; ++j)
			{
				for (int k = 0; k < 3; ++k)
				{
					if (myBlock[i][j][k] > 0 && myX + i >= 11)
					{
						canMove = 0;
					}
				}
			}
		}
		if (canMove)
		{
			myX += 1;
		}
		break;
	}
	glutPostRedisplay();
}

void initGame()
{
	//게임 공간 지우기
	for (int i = 0; i < 12; ++i)
	{
		for (int j = 0; j < 12; ++j)
		{
			for (int k = 0; k < 12; ++k)
			{
				GameSpace[i][j][k] = 0;
				tempSpace[i][j][k] = 0;
				blockSpace[i][j][k] = 0;
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
	//블록이 S자
	int ok;
	if (myBlockID == 1)
	{
		for (int i = 0; i < 3; ++i)//S자 블록 데이터 복사
		{
			for (int j = 0; j < 3; ++j)
			{
				for (int k = 0; k < 3; ++k)
				{
					myBlock[i][j][k] = block1[i][j][k];
				}
			}
		}
		//x축 기준으로 키보드 누른만큼 회전
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
			
		//y축 기준으로 키보드 누른만큼 회전
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
		
		//z축 기준으로 키보드 누른만큼 회전
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
	//현재 떨구고 있는 블록을 임시공간에 대입
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
	//임시공간에 처리 완료 후 게임공간에 대입
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
}
