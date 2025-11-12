#include <glfw3.h>
#include <vector>
#include <iostream>

float creatureX = 0.0f;
float creatureY = 0.0f;
const float moveSpeed = 0.005f;

struct Point
{
    float x, y;
};

struct Shape
{
    GLenum type;
    std::vector<Point> vertices;
};

//наш "фламінго" у примітивах
std::vector<Shape> pictureToDraw = {
    {GL_TRIANGLES, {{0.0f, -0.3f}, {0.1f, -0.3f}, {0.1f, -0.2f}}},
    {GL_TRIANGLES, {{0.05f, -0.25f}, {0.05f, -0.05f}, {0.15f, -0.15f}}},
    {GL_TRIANGLES, {{0.1f, -0.1f}, {0.1f, 0.1f}, {-0.1f, 0.1f}}},
    {GL_TRIANGLES, {{-0.1f, 0.1f}, {0.2f, 0.1f}, {0.05f, 0.25f}}},
    {GL_QUADS, {{-0.1f, 0.1f}, {-0.17f, 0.17f}, {-0.1f, 0.24f}, {-0.03f, 0.17f}}},
    {GL_QUADS, {{-0.1f, 0.24f}, {-0.1f, 0.37f}, {-0.03f, 0.3f}, {-0.03f, 0.17f}}},
    {GL_TRIANGLES, {{-0.1f, 0.37f}, {-0.1f, 0.27f}, {-0.2f, 0.27f}}}
};

//малювання примітивів + обводки
void drawFigure(const std::vector<Shape>& figureDetails)
{
    //товщина обводки
    glLineWidth(1.0f);

    //примітиви
    glColor3f(1.0f, 0.6f, 0.2f);
    for (const Shape& shape : figureDetails)
    {
        glBegin(shape.type);
        for (const Point& vertex : shape.vertices)
        {
            glVertex2f(vertex.x, vertex.y);
        }
        glEnd();
    }

    //обводка
    glColor3f(0.0f, 0.0f, 0.0f);
    for (const Shape& shape : figureDetails)
    {
        glBegin(GL_LINE_LOOP);
        for (const Point& vertex : shape.vertices)
        {
            glVertex2f(vertex.x, vertex.y);
        }
        glEnd();
    }
}

//обробка натискання клавіш
void processInput(GLFWwindow* window)
{
    //рух вгору
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
    {
        float nextY = creatureY + moveSpeed;
        creatureY = nextY;
    }

    //рух вниз
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
    {
        float nextY = creatureY - moveSpeed;
        creatureY = nextY;
    }

    //рух вліво
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
    {
        float nextX = creatureX - moveSpeed;
        creatureX = nextX;
    }

    //рух вправо
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
    {
        float nextX = creatureX + moveSpeed;
        creatureX = nextX;
    }

    //закриття вікна
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, true);
    }
}

int main(void)
{
    GLFWwindow* window;

    //ініціалізація glfw
    if (!glfwInit())
    {
        std::cerr << "Error with GLFW initialization!" << std::endl;
        return -1;
    }

    //створення вікна
    window = glfwCreateWindow(800, 800, "Lab1", NULL, NULL);
    if (!window)
    {
        std::cerr << "Error with creating GLFW window!" << std::endl;
        glfwTerminate();
        return -1;
    }

    //кажемо, щоб всі наступні операції виконувались для нашого вікна
    glfwMakeContextCurrent(window);

    //згладжування ліній
    glEnable(GL_LINE_SMOOTH);
    //перевага якості над швидкістю згладжування
    glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);

    //перетворення координат на пікселі
    int width, height;
    glfwGetFramebufferSize(window, &width, &height);
    glViewport(0, 0, width, height);

    //переходимо до налаштування "камери"
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    //встановлення меж екрану у внутрішніх координатах [-1; 1]
    glOrtho(-1.0, 1.0, -1.0, 1.0, -1.0, 1.0);

    //перестаємо налаштовувати камеру та повертаємось до "малювання"
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    while (!glfwWindowShouldClose(window))
    {
        //колір фону
        glClearColor(0.2f, 0.3f, 0.8f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        //обробка натискань
        processInput(window);

        //повертає "курсор" малювання на (0;0)
        glLoadIdentity();

        //застосування зсуву
        glTranslatef(creatureX, creatureY, 0.0f);

        //будуємо фігури
        drawFigure(pictureToDraw);

        //зміна місцями "прихованого" кадру та "видимого" кадру
        glfwSwapBuffers(window);

        //перевіряємо події
        glfwPollEvents();
    }

    //завершення роботи
    glfwTerminate();
    return 0;
}