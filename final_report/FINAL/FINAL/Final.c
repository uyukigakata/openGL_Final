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
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);// ウィンドウの表示モードを設定
    glutInitWindowSize(500, 500);// ウィンドウのサイズを設定
    glutInitWindowPosition(0, 0);// ウィンドウの初期位置を設定
    glutCreateWindow(progname);// ウィンドウを作成し、ウィンドウのタイトルをprognameに設定
    glClearColor(0.0, 0.0, 0.0, 0.0);// ウィンドウのクリアカラー（背景色）を設定
}

void myDisplay(void)

{
    glClear(GL_COLOR_BUFFER_BIT);//初期設定
    glLoadIdentity();//リセット
    gluLookAt(0.0, 2.0 * zoom, 5.0 * zoom,
        0.0, 0.0, 0.0,
        0.0, 1.0, 0.0);
    glRotated(cameraAngleY, 1.0, 0.0, 0.0);
    glRotated(cameraAngleX, 0.0, 1.0, 0.0);

    // 赤色 (太陽)
    glPushMatrix();
    glColor3d(1.0, 0.0, 0.0);
    glRotated((double)day / 25.0, 0.0, 1.0, 0.0); // 自転
    glutWireSphere(1.0, 20, 16);   /* sun */
    glPopMatrix();

    

    glPopMatrix();

    glutSwapBuffers();

}

void myReshape(int width, int height)
{
    // ビューポートを設定。ウィンドウ全体をビューポートに設定,幅,高さ
    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION); // 行列モードを投影行列に設定
    glLoadIdentity();// 単位行列にリセット
    // 透視投影行列を設定。視野角60度、アスペクト比、近くと遠くのクリッピング面を指定
    gluPerspective(60.0, (double)width / (double)height, 0.1, 120.0);
    glMatrixMode(GL_MODELVIEW); // 行列モードをモデルビュー行列に設定
    glLoadIdentity();// 単位行列にリセット
    // カメラ位置と視点を設定。カメラは(0.0, 5.0, 15.0)にあり、原点(0.0, 0.0, 0.0)を見ている。
    // 上方向は(0.0, 1.0, 0.0)
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