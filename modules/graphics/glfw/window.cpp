#include "window.h"

#include <utility>

Window* Window::_currentWindow = nullptr;

Window::Window() {
    _window = nullptr;

    //Get image data from pomegranate_png unsigned char array using stb_image
    unsigned char* data = pomegranate_png;
    int width, height, channels;
    stbi_uc* image = stbi_load_from_memory(data, sizeof(pomegranate_png), &width, &height, &channels, 0);

    if(image == nullptr)
    {
        std::cerr << "Failed to load image" << std::endl;
    }

    //Set the window icon
    icon = new GLFWimage();
    icon->width = width;
    icon->height = height;
    icon->pixels = image;
}

Window::~Window() {
    if (_open) {
        close();
    }
}

void Window::setSize(int width, int height) {
    this->_width = width;
    this->_height = height;
}

void Window::setTitle(const char* title) {
    this->_title = title;
}

void Window::setIcon(const char *path) {
    //Get image data from path using stb_image
    int width, height, channels;
    stbi_uc* image = stbi_load(path, &width, &height, &channels, 0);

    if(image == nullptr)
    {
        std::cerr << "Failed to load image" << std::endl;
    }

    if(icon != nullptr)
    {
        delete icon;
    }

    //Set the window icon
    icon = new GLFWimage();
    icon->width = width;
    icon->height = height;
    icon->pixels = image;
}

void Window::open() {
    _window = glfwCreateWindow(_width, _height, _title, nullptr, nullptr);
    if (!_window) {
        glfwTerminate();
    }
    _open = true;

    glfwMakeContextCurrent(_window);
    glfwSwapInterval(1);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        glfwTerminate();
    }
    if(!gladLoadGL())
    {
        std::cerr << "Failed to initialize GLAD" << std::endl;
    }


    glfwSetWindowIcon(_window, 1, icon);

    draw._window = _window;
    draw.init();
    makeCurrent();

    Graphics::setViewport(_width, _height);
}

void Window::close() {
    glfwDestroyWindow(_window);
    _open = false;
}

bool Window::isOpen() const {
    return _open;
}

int Window::getWidth() const {
    return _width;
}

int Window::getHeight() const {
    return _height;
}

void Window::pollEvents() {
    glfwPollEvents();
    //Close window if close button is pressed
    if (glfwWindowShouldClose(_window)) {
        close();
    }
}

void Window::makeCurrent() {
    glfwMakeContextCurrent(_window);
    Window::_currentWindow = this;
}

GLFWwindow *Window::getGLFWwindow() const {
    return _window;
}

Window *Window::current() {
    return _currentWindow;
}

void Window::Draw::begin() {
    glfwMakeContextCurrent(_window);
    _color = Vector4(0.0f, 0.0f, 0.0f, 1.0f);
    Graphics::clearDrawCalls();
}

void Window::Draw::end() {
    glfwSwapBuffers(_window);
}

void Window::Draw::clear() const {
    glClearColor(_color.x,_color.y,_color.z,_color.w);
    glClear(GL_COLOR_BUFFER_BIT);
    glClear(GL_DEPTH_BUFFER_BIT);
}

void Window::Draw::drawTexture(Texture2D* texture, Vector2 position, Vector2 size, float rotation) {
    texture->bind(0);
    Matrix3x3 modelMatrix = Matrix3x3::createTransform(position, size, rotation);

    _currentShader->use();
    _currentShader->set("SCREEN_RESOLUTION", Vector2((float)Graphics::getViewportWidth(), (float)Graphics::getViewportHeight()));
    _currentShader->set("TEXTURE", texture);
    _currentShader->set("MODEL_MATRIX", modelMatrix);
    _currentShader->set("VIEW_MATRIX", getCameraMatrix());
    _currentShader->set("Z_INDEX", _z_index);
    _rect->draw();
}

void Window::Draw::setCamera(Vector2 position, Vector2 size, float rotation) {
    _cameraPosition = position;
    _cameraSize = size;
    _cameraRotation = rotation;
}

void Window::Draw::setColor(Vector4 color) {
    _color = std::move(color);
}

void Window::Draw::setColor(const Vector3& color, float a) {
    _color = Vector4(color.x, color.y, color.z, a);
}

void Window::Draw::setShader(Shader *shader) {
    if(shader == nullptr)
    {
        _currentShader = _shader;
        return;
    }
    _currentShader = shader;
}

void Window::Draw::setZIndex(float z_index) {
    _z_index = z_index;
}

void Window::Draw::init() {
    _color = Vector4(0.0f, 0.0f, 0.0f, 1.0f);
    _rect = new Model2D();
    //Create a rectangle model
    _rect->addVertex({
                            {-1.0,-1.0},
                            {1.0},
                            {0.0,0.0}
                    });
    _rect->addVertex({
                            {-1.0,1.0},
                            {1.0},
                            {0.0,1.0}
    });
    _rect->addVertex({
                            {1.0,1.0},
                            {1.0},
                            {1.0,1.0}
    });
    _rect->addVertex({
                            {1.0,-1.0},
                            {1.0},
                            {1.0,0.0}
    });

    _rect->addIndex(2);
    _rect->addIndex(1);
    _rect->addIndex(0);

    _rect->addIndex(3);
    _rect->addIndex(2);
    _rect->addIndex(0);

    _rect->regenerateBuffers();

    //Create a tileMapShader program
    std::string vertexShaderSource;
    std::string fragmentShaderSource;

    //Load from file
    _shader = new Shader("assets/shaders/default2d/vertex.glsl", "assets/shaders/default2d/fragment.glsl");
    _currentShader = _shader;

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
}

Matrix3x3 Window::Draw::getCameraMatrix() const {
    Vector2 position = _cameraPosition;
    //Adjust to screen pixel coordinates
    position.x = position.x / (float)Graphics::getViewportWidth() * _cameraSize.x;
    position.y = position.y / (float)Graphics::getViewportHeight() * _cameraSize.y;

    return Matrix3x3::createOrthographic(0.0f, (float)Graphics::getViewportWidth(), 0.0f, (float)Graphics::getViewportHeight()).scale(_cameraSize).rotate(_cameraRotation).translate(position);
}

Shader *Window::Draw::getShader() const {
    return _currentShader;
}

