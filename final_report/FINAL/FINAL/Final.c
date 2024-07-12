/* List: p3-planet.c
 * The planet turns around the sun.
 * Examination of world-local coordinates, modeling transfomation and
 * operation of matrix stack.
 */
#include <stdlib.h>
#include <GL/glut.h>
#include <math.h>
#include <stdbool.h>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

static int year = 0, day = 0;
int samplingTime = 50;
static double zoom = 1.0;
static double cameraAngleX = 0.0;
static double cameraAngleY = 0.0;
static int mouseX, mouseY;
static int isDragging = 0;


void myInit(char* progname)
{
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);// �E�B���h�E�̕\�����[�h��ݒ�
    glutInitWindowSize(500, 500);// �E�B���h�E�̃T�C�Y��ݒ�
    glutInitWindowPosition(0, 0);// �E�B���h�E�̏����ʒu��ݒ�
    glutCreateWindow(progname);// �E�B���h�E���쐬���A�E�B���h�E�̃^�C�g����progname�ɐݒ�
    glClearColor(0.0, 0.0, 0.0, 0.0);// �E�B���h�E�̃N���A�J���[�i�w�i�F�j��ݒ�
}

void myDisplay(void)

{
    glClear(GL_COLOR_BUFFER_BIT);//�����ݒ�
    glLoadIdentity();//���Z�b�g
    gluLookAt(0.0, 2.0 * zoom, 5.0 * zoom,
        0.0, 0.0, 0.0,
        0.0, 1.0, 0.0);
    glRotated(cameraAngleY, 1.0, 0.0, 0.0);
    glRotated(cameraAngleX, 0.0, 1.0, 0.0);

    // �ԐF (���z)
    glPushMatrix();
    glColor3d(1.0, 0.0, 0.0);
    glRotated((double)day / 25.0, 0.0, 1.0, 0.0); // ���]
    glutWireSphere(1.0, 20, 16);   /* sun */
    glPopMatrix();

    

    glPopMatrix();

    glutSwapBuffers();

}

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
    // �J�����ʒu�Ǝ��_��ݒ�B�J������(0.0, 5.0, 15.0)�ɂ���A���_(0.0, 0.0, 0.0)�����Ă���B
    // �������(0.0, 1.0, 0.0)
    gluLookAt(0.0, 5.0, 40.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
}

void myKeyboard(unsigned char key, int x, int y)
{
    switch (key) {
    case 'd':
        day = (day + 10);
        glutPostRedisplay();
        break;
    case 'D':
        day = (day - 10);
        glutPostRedisplay();
        break;
    case 'y':
        year = (year + 5);
        glutPostRedisplay();
        break;
    case 'Y':
        year = (year - 5);
        glutPostRedisplay();
        break;
    case 27:
        exit(0);
        break;
    default:
        break;
    }
}

void myTimer(int value)
{
    if (value == 1)
    {
        glutTimerFunc(samplingTime, myTimer, 1);
        year = (year + 1);
        day = (day + 5);

        glutPostRedisplay();
    }
}

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
void myMouseMotion(int x, int y) {
    if (isDragging) {
        cameraAngleX += (x - mouseX) * 0.5;
        cameraAngleY += (y - mouseY) * 0.5;
        mouseX = x;
        mouseY = y;
        glutPostRedisplay();
    }
}
void myMouseWheel(int button, int dir, int x, int y) {
    if (dir > 0) {
        zoom /= 1.1; // Zoom in
    }
    else {
        zoom *= 1.1; // Zoom out
    }
    glutPostRedisplay();
}

int main(int argc, char** argv)
{
    glutInit(&argc, argv);
    myInit(argv[0]);
    glutKeyboardFunc(myKeyboard);
    glutTimerFunc(samplingTime, myTimer, 1);
    glutReshapeFunc(myReshape);
    glutMouseFunc(myMouse);
    glutMotionFunc(myMouseMotion);
    glutMouseWheelFunc(myMouseWheel);
    glutDisplayFunc(myDisplay);
    glutMainLoop();
    return 0;
}