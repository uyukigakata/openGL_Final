#ifdef _WIN32
#include <windows.h>
#endif
#include <stdio.h>
#include <stdlib.h>
#ifndef __APPLE__
#include <GL/gl.h>
#include <GL/glut.h>
#else
#include <OpenGL/gl.h>
#include <GLUT/glut.h>
#endif

#include <math.h>
#include <GL/freeglut.h>


static double cameraAngleX = 0.0;
static double cameraAngleY = 0.0;
static int mouseX, mouseY;
static int isDragging = 0.0;
static double zoom = 1.0f;
static float Rotatex = -90.0f;
static float Rotatey = 0.0f;
static float Rotatez = 0.0f;
static float objx = 0.0f;
static float objy = 0.0f;
static float objz = 0.0f;
static float rotationAngle = 0.0f;
static float movementStep = 0.1f; // �ړ��X�e�b�v�̑傫��
const float stages = 100.0f;
static int textureStep = 0; // �e�N�X�`���̐i���i�K

#define imageWidth 256
#define imageHeight 256

//�e�N�X�`���}�b�s���O��L�������邽�߂̂���
unsigned char texImage[imageHeight][imageWidth][3];//�r�����_�}
unsigned char totyutexImage[imageHeight][imageWidth][3];//�}���}�C���O
unsigned char totyutex2Image[imageHeight][imageWidth][3];//�}���}�C���O2
unsigned char totyutex3Image[imageHeight][imageWidth][3];//�}���}�C���O3
unsigned char sinkatexImage[imageHeight][imageWidth][3];//�}���}�C��
unsigned char stagetexImage[imageHeight][imageWidth][3];//�X�e�[�W1

GLuint textureID, totyutextureID, totyutexture2ID, 
        totyusinkatextureID,sinkatextureID,stagetextureID;
GLuint currentTextureID,currentStageID; // ���݂̃I�u�W�F�N�g�̃e�N�X�`��ID��ێ�����ϐ�


//�e�N�X�`���}�b�s���O�ɕK�v�Ȃ���
void readPPMImage(char* filename, unsigned char image[imageHeight][imageWidth][3])
{
    FILE* fp;
    char header[3];
    int width, height, maxval;

    if (fopen_s(&fp, filename, "rb") != 0) {
        fprintf(stderr, "Cannot open ppm file %s\n", filename);
        exit(1);
    }

    fread(image, 1, imageWidth * imageHeight * 3, fp);    // read RGB data
    fclose(fp);
}
//�e�N�X�`���}�b�s���O�ɕK�v�Ȃ���
void setUpTexture(void)
{
    readPPMImage("�r�����_�}.ppm", texImage);
    readPPMImage("�r�����_�}_�i���r��2.ppm", totyutexImage);
    readPPMImage("�r�����_�}_�i���r��3.ppm", totyutex2Image);
    readPPMImage("�}���}�C��_�i���r��.ppm", totyutex3Image);
    readPPMImage("�}���}�C��.ppm", sinkatexImage);
    readPPMImage("�Ő�.ppm", stagetexImage);

    // �r�����_�}�̃e�N�X�`���ݒ�
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, imageWidth, imageHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, texImage);

    // �r�����_�}�i���r���̃e�N�X�`���ݒ�
    glGenTextures(1, &totyutextureID);
    glBindTexture(GL_TEXTURE_2D, totyutextureID);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, imageWidth, imageHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, totyutexImage);

    // �r�����_�}�̐i���r��2�e�N�X�`���ݒ�
    glGenTextures(1, &totyutexture2ID);
    glBindTexture(GL_TEXTURE_2D, totyutexture2ID);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, imageWidth, imageHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, totyutex2Image);

    // �}���}�C���O�̃e�N�X�`���ݒ�
    glGenTextures(1, &totyusinkatextureID);
    glBindTexture(GL_TEXTURE_2D, totyusinkatextureID);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, imageWidth, imageHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, totyutex3Image);

    // �i���`�Ԃ̃e�N�X�`���ݒ�
    glGenTextures(1, &sinkatextureID);
    glBindTexture(GL_TEXTURE_2D, sinkatextureID);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, imageWidth, imageHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, sinkatexImage);

    // �X�e�[�W�̃e�N�X�`���ݒ�
    glGenTextures(1, &stagetextureID);
    glBindTexture(GL_TEXTURE_2D, stagetextureID);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, imageWidth, imageHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, stagetexImage);

    // ������ԂƂ��ăr�����_�}�̃e�N�X�`����ݒ�
    currentTextureID = textureID;
    currentStageID = stagetextureID;
}
//��ʊ�{���
void myInit(char* progname)
{
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);// �E�B���h�E�̕\�����[�h��ݒ�
    glutInitWindowSize(500, 500);// �E�B���h�E�̃T�C�Y��ݒ�
    glutInitWindowPosition(0, 0);// �E�B���h�E�̏����ʒu��ݒ�
    glutCreateWindow(progname);// �E�B���h�E���쐬���A�E�B���h�E�̃^�C�g����progname�ɐݒ�
    
    glClearColor(0.0, 0.5, 1,0);// �E�B���h�E�̃N���A�J���[�i�w�i�F�j��ݒ�
    glShadeModel(GL_SMOOTH);
    glEnable(GL_DEPTH_TEST); // �[�x�e�X�g��L���ɂ���
    
}

void mysetLight() {
    float light_position[] = { 0.0, 10.0, 10.0, 1.0 }; // �����̈ʒu4�������f�B���N�V�������C�g 
    float white[] = { 1.0, 1.0, 1.0, 1.0 }; /* ���̐F(RGB) */
    glLightfv(GL_LIGHT0, GL_POSITION, light_position);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, white); /*�g�U��*/
    glLightfv(GL_LIGHT0, GL_SPECULAR, white);/*���ʌ�*/
    glEnable(GL_LIGHTING); /* ���C�e�B���O��L���ɂ��� */
    glEnable(GL_LIGHT0); /* ������L���ɂ��� */
}
//���̕\��
//void drawAxes(double length) {
//    
//    glBegin(GL_LINES);
//    // X  
//    glColor3d(1.0, 0.0, 0.0);
//    glVertex3d(0.0, 0.0, 0.0);
//    glVertex3d(length, 0.0, 0.0);
//    // Y
//    glColor3d(0.0, 1.0, 0.0);
//    glVertex3d(0.0, 0.0, 0.0);
//    glVertex3d(0.0, length, 0.0);
//    // Z
//    glColor3d(0.0, 0.0, 1.0);
//    glVertex3d(0.0, 0.0, 0.0);
//    glVertex3d(0.0, 0.0, length);
//    glEnd();
//}
//�I�u�W�F�N�g����
void drawSphere(GLdouble radius) {
    GLUquadric* quad = gluNewQuadric();
    gluQuadricTexture(quad, GL_TRUE);  // �e�N�X�`���}�b�s���O��L���ɂ���
    gluSphere(quad, radius, 32, 32);
    gluDeleteQuadric(quad);
}
//����
void drawFace() {
    glEnable(GL_TEXTURE_2D); // 2D�L����
    glBindTexture(GL_TEXTURE_2D, currentTextureID); // �e�N�X�`���}�b�s���O
    glPushMatrix();
    
    mysetLight();
    // �e��`�悷��
    drawShadow();
    // �����i���́j�̕`��
    glTranslated(objx,objy,objz);  // �ʒu��{�̂̏�ɒ���
    glRotated(Rotatex, 1.0, 0.0, 0.0);  // x����90�x��]
    glRotated(Rotatey, 0.0, 1.0, 0.0);  // z����0�x��]
    glRotated(Rotatez, 0.0, 0.0, 1.0);  // z����0�x��]
    drawSphere(0.7);              // ���̂�`�攼�a0.7
    //drawAxes(5.0);
    glPopMatrix();
    glDisable(GL_TEXTURE_2D); // 2D�e�N�X�`���}�b�s���O������
}
void drawShadow() {
    // �e�����ɐݒ肷��
    glColor3f(0.0, 0.0, 0.0);
    glPushMatrix();

    // �e�̈ʒu�𒲐�����i�����ł͉E���ɉe��u���j
    glTranslated(objx + 0.5, objy, objz);
    glRotated(Rotatex, 1.0, 0.0, 0.0);
    glRotated(Rotatey, 0.0, 1.0, 0.0);
    glRotated(Rotatez, 0.0, 0.0, 1.0);
    drawSphere(0.7); // �e�Ƃ��ċ��̂�`�悷��
    glPopMatrix();
}

void drawstage() {
    glEnable(GL_TEXTURE_2D); // 2D�L����
    glBindTexture(GL_TEXTURE_2D, currentStageID); // �e�N�X�`���}�b�s���O
    
    glBegin(GL_QUADS);
    glTexCoord2f(0.0, 0.0); 
    glVertex3f(-stages, -0.7, -stages);
    glTexCoord2f(1.0, 0.0); 
    glVertex3f(stages, -0.7, -stages);
    glTexCoord2f(1.0, 1.0); 
    glVertex3f(stages, -0.7, stages);
    glTexCoord2f(0.0, 0.0); 
    glVertex3f(-stages, -0.7, stages);
    glEnd();
    glDisable(GL_TEXTURE_2D);
}
void myDisplay(void)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();//���Z�b�g
    // �J�����̉�]��K�p
    gluLookAt(0.0, 2.0 * zoom, 5.0 * zoom,
        0.0, 0.0, 0.0,
        0.0, 1.0, 0.0);
    glRotated(cameraAngleY, 1.0, 0.0, 0.0);
    glRotated(cameraAngleX, 0.0, 1.0, 0.0);
    //��
    glPushMatrix();
    drawFace();
    glPopMatrix();
    //�X�e�[�W
    drawstage();


    glutSwapBuffers();
}
//�}�E�X�̉����A���Ō��o�����_�ړ�
void myMouse(int button, int state, int x, int y) {
    if (button == GLUT_LEFT_BUTTON) {
        if (state == GLUT_DOWN) {
            isDragging = 1;
            mouseX = x;
            mouseY = y;
        }
        else {
            isDragging = 0;
        }
    }
}
//�}�E�X�h���b�O�ňړ�
void myMouseMotion(int x, int y) {
    if (isDragging) {
        cameraAngleX += (x - mouseX) * 0.5;
        cameraAngleY += (y - mouseY) * 0.5;
        mouseX = x;
        mouseY = y;
        glutPostRedisplay();
    }
}
//�}�E�X�z�C�[�����g��k��
void myMouseWheel(int button, int dir, int x, int y) {
    if (dir > 0) {
        zoom /= 1.1; // Zoom in
    }
    else {
        zoom *= 1.1; // Zoom out
    }
    glutPostRedisplay();
}
//�E�B���h�E�Y�̃��T�C�Y
void myReshape(int width, int height)
{
    // �r���[�|�[�g��ݒ�B�E�B���h�E�S�̂��r���[�|�[�g�ɐݒ�,��,����
    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION); // �s�񃂁[�h�𓊉e�s��ɐݒ�
    glLoadIdentity();// �P�ʍs��Ƀ��Z�b�g
    // �������e�s���ݒ�B����p60�x�A�A�X�y�N�g��A�߂��Ɖ����̃N���b�s���O�ʂ��w��
    gluPerspective(60.0, (double)width / (double)height, 0.1, 120.0);
    glMatrixMode(GL_MODELVIEW); // �s�񃂁[�h�����f���r���[�s��ɐݒ�
    glLoadIdentity();// �P�ʍs��Ƀ��Z�b�g
    // �J�����ʒu�Ǝ��_��ݒ�B�J������(0.0, 5.0, 10.0)�ɂ���A���_(0.0, 0.0, 0.0)�����Ă���B
    // �������(0.0, 1.0, 0.0)
    gluLookAt(0.0, 5.0, 30.0, 0.0, 0.0, 0.0, 0.0, 20.0, 0.0);
}
void mykeyboard(unsigned char key, int x, int y) {
    
    switch (key)
    {
    case 'w':
    case 'W': //�O�i
        Rotatex -= 20.0;
        objz -= 1.0;
        glutPostRedisplay();
        break;
    case 's':
    case 'S': //���
        Rotatex += 20.0;
        objz += 1.0;
        glutPostRedisplay();
        break;

    case 'a':
    case 'A': //�� 
        Rotatez -= 20.0;
        objx -= 1.0f;
        glutPostRedisplay();
        break;

    case 'd':
    case 'D': //�E
        Rotatez += 20.0;
        objx += 1.0f;
        glutPostRedisplay();
        break;
    default:
        break;
    }
}
//UI�{�^���\��
void getValueFromMenu(int option) {
    switch (option) {
    case 1:
        // �r�����_�}�i���r���̃e�N�X�`���ɐ؂�ւ��ĉ�]
        currentTextureID = totyutextureID;
        glutPostRedisplay(); // �ĕ`����g���K�[
        break;
    case 2:
        // �r�����_�}�i���r��2�̃e�N�X�`���ɐ؂�ւ��ĉ�]
        currentTextureID = totyutexture2ID;
        glutPostRedisplay(); // �ĕ`����g���K�[
        break;
    case 3:
        // �i���`�Ԃ̃e�N�X�`���ɐ؂�ւ��ĉ�]
        currentTextureID = totyusinkatextureID;
        glutPostRedisplay(); // �ĕ`����g���K�[
        break;
    case 4:
        // �i���`�Ԃ̃e�N�X�`���ɐ؂�ւ��ĉ�]
        currentTextureID = sinkatextureID;
        glutPostRedisplay(); // �ĕ`����g���K�[
        break;
    default:
        break;
    }
}
//���j���[�쐬�A
void mySetMenu() {
    glutCreateMenu(getValueFromMenu);//���j���[�ݒ�
    glutAddMenuEntry("evolution", 1);
    glutAddMenuEntry("evolution2", 2);
    glutAddMenuEntry("evolution3", 3);
    glutAddMenuEntry("evolution4", 4);
    glutAttachMenu(GLUT_RIGHT_BUTTON);
}


int main(int argc, char** argv)
{
    glutInit(&argc, argv); // GLUT�̏�����
    myInit(argv[0]); // �������֐����Ăяo��
    setUpTexture(); // �e�N�X�`���̐ݒ���s���֐����Ăяo��
    glutMouseFunc(myMouse); // �}�E�X���쎞�̃R�[���o�b�N�֐����w��
    glutMotionFunc(myMouseMotion); // �}�E�X�ړ����̃R�[���o�b�N�֐����w��
    glutMouseWheelFunc(myMouseWheel); // �}�E�X�z�C�[�����쎞�̃R�[���o�b�N�֐����w��
    glutKeyboardFunc(mykeyboard); // �L�[�{�[�h���쎞�̃R�[���o�b�N�֐����w��i�R�����g�A�E�g����Ă���j
    glutReshapeFunc(myReshape); // �E�B���h�E�̃��T�C�Y���̃R�[���o�b�N�֐����w��
    glutDisplayFunc(myDisplay); // �`�掞�̃R�[���o�b�N�֐����w��
    mySetMenu(); // ���j���[�̐ݒ���s���֐����Ăяo��
    glutMainLoop(); // GLUT�̃��C�����[�v���J�n
    return 0;
}
