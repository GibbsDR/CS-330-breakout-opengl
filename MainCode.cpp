#include <GLFW/glfw3.h>
#include <cmath>
#include <cstdlib>
#include <ctime>
#include <vector>

const float DEG2RAD = 3.14159f / 180.0f;

// ===================== GAME STATE =====================
int lives = 3;
bool gameOver = false;
bool win = false;

bool spaceWasPressed = false;
bool waitingForLaunch = true;

// ===================== PADDLE =====================
float paddleX = 0.0f;
float paddleWidth = 0.25f;
float paddleY = -0.85f;

float paddleSpeed = 0.0015f;

// ===================== BALL =====================
struct Ball {
    float x, y;
    float vx, vy;
    bool alive;
};

float ballRadius = 0.05f;

Ball ball1 = { 0.0f, -0.3f, 0.0f, 0.0f, true };
Ball ball2 = { 0.15f, -0.3f, 0.0f, 0.0f, true };

// ===================== BRICKS =====================
struct Brick {
    float x, y;
    float size;
    int hits;
    bool active;
};

std::vector<Brick> bricks;

// ===================== RESET BALLS =====================
void resetBalls()
{
    ball1 = { 0.0f, -0.3f, 0.0f, 0.0f, true };
    ball2 = { 0.15f, -0.3f, 0.0f, 0.0f, true };
    waitingForLaunch = true;
}

// ===================== RESET GAME =====================
void resetGame()
{
    lives = 3;
    gameOver = false;
    win = false;
    waitingForLaunch = true;
    paddleX = 0.0f;

    bricks.clear();

    int rows = 5;
    float spacing = 0.22f;
    float startY = 0.75f;

    for (int r = 0; r < rows; r++)
    {
        int count = rows - r;
        float startX = -((count - 1) * spacing) / 2.0f;

        for (int c = 0; c < count; c++)
        {
            Brick b;
            b.x = startX + c * spacing;
            b.y = startY - r * spacing;
            b.size = 0.18f;
            b.active = true;

            if (r == 0) b.hits = 3;
            else if (r == 1) b.hits = 2;
            else b.hits = 1;

            bricks.push_back(b);
        }
    }

    resetBalls();
}

// ===================== DRAW =====================
void drawRect(float x1, float y1, float x2, float y2)
{
    glBegin(GL_QUADS);
    glVertex2f(x1, y1);
    glVertex2f(x2, y1);
    glVertex2f(x2, y2);
    glVertex2f(x1, y2);
    glEnd();
}

void drawBall(Ball& b)
{
    if (!b.alive) return;

    glBegin(GL_POLYGON);
    for (int i = 0; i < 360; i++)
    {
        float r = i * DEG2RAD;
        glVertex2f(b.x + cos(r) * ballRadius,
            b.y + sin(r) * ballRadius);
    }
    glEnd();
}

// ===================== INPUT =====================
void processInput(GLFWwindow* window)
{
    if (gameOver || win)
    {
        if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS)
            resetGame();
        return;
    }

    // paddle movement (slow)
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        paddleX -= paddleSpeed;

    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        paddleX += paddleSpeed;

    if (paddleX < -1 + paddleWidth)
        paddleX = -1 + paddleWidth;

    if (paddleX > 1 - paddleWidth)
        paddleX = 1 - paddleWidth;

    // SPACE launch
    bool spaceNow = glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS;

    if (spaceNow && !spaceWasPressed && waitingForLaunch)
    {
        ball1.vx = 0.0007f;
        ball1.vy = 0.0007f;

        ball2.vx = -0.0006f;
        ball2.vy = 0.0006f;

        waitingForLaunch = false;
    }

    spaceWasPressed = spaceNow;
}

// ===================== MAIN =====================
int main()
{
    srand((unsigned)time(NULL));

    glfwInit();
    GLFWwindow* window = glfwCreateWindow(600, 600, "Breakout", NULL, NULL);
    glfwMakeContextCurrent(window);

    resetGame();

    while (!glfwWindowShouldClose(window))
    {
        glClear(GL_COLOR_BUFFER_BIT);

        processInput(window);

        // ===================== GAME LOGIC =====================
        if (!gameOver && !win && !waitingForLaunch)
        {
            Ball* balls[2] = { &ball1, &ball2 };

            for (auto b : balls)
            {
                if (!b->alive) continue;

                b->x += b->vx;
                b->y += b->vy;

                if (b->x > 1 || b->x < -1) b->vx *= -1;
                if (b->y > 1) b->vy *= -1;

                if (b->y < -1)
                    b->alive = false;

                // paddle
                if (b->y <= paddleY + 0.05f &&
                    b->x >= paddleX - paddleWidth &&
                    b->x <= paddleX + paddleWidth &&
                    b->vy < 0)
                {
                    b->vy *= -1;
                }

                // bricks
                for (auto& br : bricks)
                {
                    if (!br.active) continue;

                    float h = br.size / 2;

                    if (b->x > br.x - h && b->x < br.x + h &&
                        b->y > br.y - h && b->y < br.y + h)
                    {
                        b->vy *= -1;
                        br.hits--;

                        if (br.hits <= 0)
                            br.active = false;
                    }
                }
            }

            // lives
            if (!ball1.alive && !ball2.alive)
            {
                lives--;

                if (lives <= 0)
                    gameOver = true;
                else
                    resetBalls();
            }

            // win check
            bool allGone = true;
            for (auto& b : bricks)
                if (b.active) allGone = false;

            if (allGone)
                win = true;
        }

        // ===================== DRAW =====================

        // paddle
        glColor3f(0.2f, 0.8f, 1.0f);
        drawRect(paddleX - paddleWidth, paddleY,
            paddleX + paddleWidth, paddleY + 0.05f);

        // balls
        glColor3f(1, 0.3f, 0.3f);
        drawBall(ball1);

        glColor3f(0.3f, 1, 0.3f);
        drawBall(ball2);

        // bricks
        for (auto& b : bricks)
        {
            if (!b.active) continue;

            if (b.hits == 3) glColor3f(1, 0, 0);
            else if (b.hits == 2) glColor3f(1, 0.5f, 0);
            else glColor3f(0, 1, 0);

            float h = b.size / 2;
            drawRect(b.x - h, b.y - h,
                b.x + h, b.y + h);
        }

        // ===================== WIN / LOSE DISPLAY =====================
        if (gameOver)
        {
            glfwSetWindowTitle(window, "GAME OVER - Press R to Restart");

            // red background panel
            glColor3f(1.0f, 0.0f, 0.0f);
            drawRect(-0.8f, -0.25f, 0.8f, 0.25f);

            // inner highlight box (gives depth)
            glColor3f(0.6f, 0.0f, 0.0f);
            drawRect(-0.75f, -0.2f, 0.75f, 0.2f);
        }

        if (win)
        {
            glfwSetWindowTitle(window, "YOU WIN! - Press R to Restart");

            // green background panel
            glColor3f(0.0f, 1.0f, 0.0f);
            drawRect(-0.8f, -0.25f, 0.8f, 0.25f);

            // inner highlight box
            glColor3f(0.0f, 0.6f, 0.0f);
            drawRect(-0.75f, -0.2f, 0.75f, 0.2f);
        }

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}