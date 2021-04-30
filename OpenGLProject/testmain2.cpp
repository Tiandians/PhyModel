#include "stdafx.h"
#include <GL/glut.h>
#include <stdlib.h>   //srand��rand
#include <time.h>   //time(int)

const int N = 2000;
float particles[N][3];
float rtri = 0;

// ��ʼ���������ԡ���Դ������ģ�͡���Ȼ�����
void init(void)
{
    //���ʷ���������
    GLfloat mat_specular[] = { 1.0, 1.0, 1.0, 1.0 };  //���淴�����
    GLfloat mat_shininess[] = { 50.0 };               //�߹�ָ��
    GLfloat light_position[] = { 1.0, 1.0, 1.0, 0.0 };
    GLfloat white_light[] = { 1.0, 1.0, 1.0, 1.0 };   //��λ��(1,1,1), ���1-����
    GLfloat Light_Model_Ambient[] = { 1.0, 0.2, 0.5, 0.1 }; //���������

    glClearColor(0.0, 0.0, 0.0, 0.0);  //����ɫ
    glShadeModel(GL_SMOOTH);           //��������ģʽ

    //��������
    glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
    glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);

    //�ƹ�����
    glLightfv(GL_LIGHT0, GL_POSITION, light_position);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, white_light);   //ɢ�������
    glLightfv(GL_LIGHT0, GL_SPECULAR, white_light);  //���淴���
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, Light_Model_Ambient);  //���������

    glEnable(GL_LIGHTING);   //����:ʹ�ù�
    glEnable(GL_LIGHT0);     //��0#��
    glEnable(GL_DEPTH_TEST); //����Ȳ���
}

void display(void)
{
    //ͨ��ѭ������ÿ�����ӵ�λ��
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < 3; j++) {
            particles[i][j] += (rand() % 800 / 800.0 - 0.5) / 100;
        }
    }

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);            //�����ɫ�������Ȼ���

    glLoadIdentity();   //װ�뵥λת������

    //ƽ��
    glTranslatef(0.0f, 0.0f, -4.0f);//������ת
    glRotatef(rtri, 0.1, 1.0, 0.1);
    //ÿ�����ӵ�λ�ñ䶯
    for (int i = 0; i < N; i++) {
        glPushMatrix();
        //ƽ�Ƶ���i������λ��
        glTranslatef(particles[i][0], particles[i][1], particles[i][2]);
        glutSolidSphere(0.02, 20, 16);
        glPopMatrix();
    }

    rtri += 0.5f;//��һ���Ƕ�
    glutSwapBuffers();//����˫����
}

void reshape(int width, int height)
{
    glViewport(0, 0, width, height);

    //ͶӰģʽ
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    //����бͶӰ�������
    gluPerspective(45.0f, (GLfloat)width / (GLfloat)height, 0.1f, 600.0f);

    //ģ��-��ͼ����ģʽ
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void keyboard(unsigned char key, int x, int y)
{
    switch (key)
    {
    case 'x':
    case 'X':
    case 27:   //ESC��
        exit(0);
        break;
    default:
        break;
    }
}

int main(int argc, char** argv)
{
    //seed��ʼ�����������
    srand((unsigned int)time(0));    //random seeds
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < 3; j++) {
            particles[i][j] = (rand() % 800 / 800.0 - 0.5) * 2;
        }
    }
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);//ʹ��˫����ģʽ����Ȼ���
    glutInitWindowSize(800, 800);
    glutInitWindowPosition(0, 0);
    glutCreateWindow("����Ⱥ");
    init();
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(keyboard);
    glutIdleFunc(display);//���ÿ���ʱ���õĺ���
    glutMainLoop();
    return 0;
}