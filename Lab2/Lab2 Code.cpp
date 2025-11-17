#include <glut.h>
#include <cmath>
#include <vector>
#include <iostream>

//параметри камери
float angleH = 0.5f;
float angleV = 0.5f;
float radius = 15.0f;

bool isPerspective = true;
int w_width = 800;
int w_height = 600;
float speed = 0.05f;

struct Point3D
{
    float x, y, z;
};

std::vector<Point3D> surfaceVertices;
int numVertices = 0;

//генерація поверхні Z = sin(x) + cos(y)
void generateSurface() {
    float step = 0.2f;
    float range = 4.0f;

    //заповнюємо вектор вершин поверхні по 4 вершини за ітерацію
    for (float x = -range; x < range; x += step) {
        for (float y = -range; y < range; y += step) {
            surfaceVertices.push_back({ x, y, sin(x) + cos(y) });
            surfaceVertices.push_back({ x + step, y, sin(x + step) + cos(y) });
            surfaceVertices.push_back({ x + step, y + step, sin(x + step) + cos(y + step) });
            surfaceVertices.push_back({ x, y + step, sin(x) + cos(y + step) });
        }
    }
    numVertices = surfaceVertices.size();
}

//встановлення проекції
void setProjection() {
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    float ratio = (float)w_width / (float)w_height;

    //перспектива (розмір об'єктів залежить від відстані)
    if (isPerspective) {
        glFrustum(-ratio, ratio, -1.0, 1.0, 1.5, 100.0);
    }
    //ортогональна проекція (розмір об'єктів не залежить від відстані)
    else {
        float orthoSize = 10.0f;
        glOrtho(-orthoSize * ratio, orthoSize * ratio, -orthoSize, orthoSize, -100.0, 100.0);
    }
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

//відображення фігур
void display() {
    //очищення буфера кольору
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    //встановлення проекції
    setProjection();

    //переведення зі сферичних координат в декартові
    float camX = radius * cos(angleV) * sin(angleH);
    float camY = radius * sin(angleV);
    float camZ = radius * cos(angleV) * cos(angleH);

    //потрібно для повного вертикального обороту
    float upY = (cos(angleV) >= 0) ? 1.0f : -1.0f;

    //налаштування камери
    //перші 3 - координата камери
    //наступні три - напрямок камери в (0, 0, 0)
    //останні три - напрямок вектора "вгору"
    gluLookAt(camX, camY, camZ, 0.0f, 0.0f, 0.0f, 0.0f, upY, 0.0f);

    //малювання поверхні
    //ізолюємо трансформації для конкретного об'єкта
    glPushMatrix();
    //робимо поверхню горизонтальною
    glRotatef(-90.0f, 1.0f, 0.0f, 0.0f);

    glColor3f(0.0f, 1.0f, 0.5f);
    //режим роботи з масивами
    glEnableClientState(GL_VERTEX_ARRAY);
    glVertexPointer(3, GL_FLOAT, sizeof(Point3D), &surfaceVertices[0].x);

    for (int i = 0; i < numVertices; i += 4) {
        glDrawArrays(GL_LINE_LOOP, i, 4);
    }
    glDisableClientState(GL_VERTEX_ARRAY);
    glPopMatrix();

    //малювання тору
    glPushMatrix();
    glTranslatef(0.0f, 5.0f, 0.0f);

    glColor3f(1.0f, 0.0f, 0.0f);
    //малюємо саме контурний тор
    glutWireTorus(0.5, 1.5, 20, 20);
    glPopMatrix();

    //малювання додекаедру
    glPushMatrix();
    glTranslatef(0.0f, -5.0f, 0.0f);

    //трохи збільшив розмір додекаедру (в 1.5 рази), бо було погано видно
    glScalef(1.5f, 1.5f, 1.5f);
    glColor3f(0.2f, 0.2f, 1.0f);
    //малюємо саме заповнений додекаедр
    glutSolidDodecahedron();
    glPopMatrix();

    glutSwapBuffers();
}

//зміна масштабу вікна
void reshape(int w, int h) {
    if (h == 0) h = 1;
    w_width = w;
    w_height = h;
    glViewport(0, 0, w, h);
}

//спеціальні кнопки(стрілки)
void specialKeys(int key, int x, int y) {
    if(key == GLUT_KEY_LEFT) angleH -= speed;
    if (key == GLUT_KEY_RIGHT) angleH += speed;
    if (key == GLUT_KEY_UP) angleV += speed;
    if (key == GLUT_KEY_DOWN) angleV -= speed;

    //кажемо, щоб перемалювався кадр
    glutPostRedisplay();
}

//події клавіатури
void keyboard(unsigned char key, int x, int y) {
    //зміна проекції
    if(key == 'p' || key == 'P') isPerspective = true;
    if (key == 'o' || key == 'O') isPerspective = false;

    //поворот
    if (key == 'a' || key == 'A') angleH -= speed;
    if (key == 'd' || key == 'D') angleH += speed;
    if (key == 'w' || key == 'W') angleV += speed;
    if (key == 's' || key == 'S') angleV -= speed;

    //закриття вікна
    if(key == 27) exit(0);

    //кажемо, щоб перемалювався кадр
    glutPostRedisplay();
}

//початкова ініціалізація
void init() {
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    //z-буфер
    glEnable(GL_DEPTH_TEST);
    //налаштування освітлення
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    //колір як матеріал
    glEnable(GL_COLOR_MATERIAL);
    generateSurface();
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(800, 600);
    glutCreateWindow("Lab2");
    init();
    //вказання які з наших функцій коли викликати
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutSpecialFunc(specialKeys);
    glutKeyboardFunc(keyboard);
    //головний цикл вікна
    glutMainLoop();
    return 0;
}