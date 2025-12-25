#include <SDL2/SDL.h>
#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <chrono>
#include <thread>
#include <cstdlib>
#include <ctime>

using namespace std;

// Constants
const int WINDOW_WIDTH = 1024;
const int WINDOW_HEIGHT = 768;
const int BUTTON_WIDTH = 200;
const int BUTTON_HEIGHT = 40;
const int CHAR_WIDTH = 8;
const int CHAR_HEIGHT = 16;

// Colors
SDL_Color WHITE = {255, 255, 255, 255};
SDL_Color BLACK = {0, 0, 0, 255};
SDL_Color BLUE = {0, 100, 200, 255};
SDL_Color DARK_BLUE = {0, 50, 100, 255};
SDL_Color GREEN = {0, 150, 0, 255};
SDL_Color RED = {200, 0, 0, 255};
SDL_Color GRAY = {128, 128, 128, 255};
SDL_Color LIGHT_GRAY = {200, 200, 200, 255};
SDL_Color ORANGE = {255, 165, 0, 255};

// Global variables
SDL_Window* window = nullptr;
SDL_Renderer* renderer = nullptr;

// System state
enum AppState {
    STATE_LOGIN,
    STATE_BOOTLOADER,
    STATE_MAIN_MENU,
    STATE_TASK_MANAGER,
    STATE_RECYCLE_BIN,
    STATE_FIREWALL,
    STATE_FILE_SEARCH,
    STATE_BENCHMARK,
    STATE_BLUETOOTH,
    STATE_DEFENDER,
    STATE_SCHEDULER,
    STATE_DEVICE_MANAGER,
    STATE_LOCKED
};

AppState currentState = STATE_LOGIN;
string currentUser = "guest";
string theme = "Light";
bool systemLocked = false;
bool updateAvailable = false;
int bootProgress = 0;
int ramTestProgress = 0;
bool ramTestFailed = false;
string inputBuffer = "";
string passwordBuffer = "";
bool showPassword = false;

// Data structures
map<string, string> files = {
    {"ANDROID_OS.ISO", "System boot image"}, 
    {"WARNING_KNL.EXE", "Critical system file"}, 
    {"README.TXT", "System documentation"}, 
    {"TASKS.LOG", "Task execution history"},
    {"CONFIG.SYS", "System configuration file"}
};

vector<string> recycleBin;
vector<string> users = {"Admin", "Guest", "Developer"};
string password = "viethoang082013";

// Simple bitmap font (8x16 pixels per character)
// This is a minimal font - you could expand this or load from a file
void drawChar(char c, int x, int y, SDL_Color color) {
    // Simple pixel font representation - just basic rectangles for demo
    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, 255);
    
    // Very basic character rendering - you can improve this
    if (c >= 'A' && c <= 'Z') {
        // Draw a simple rectangle for uppercase letters
        SDL_Rect charRect = {x + 1, y + 2, 6, 12};
        SDL_RenderDrawRect(renderer, &charRect);
        if (c == 'A') {
            SDL_RenderDrawLine(renderer, x + 3, y + 6, x + 5, y + 6);
        }
    } else if (c >= 'a' && c <= 'z') {
        // Draw a simple rectangle for lowercase letters
        SDL_Rect charRect = {x + 1, y + 6, 6, 8};
        SDL_RenderDrawRect(renderer, &charRect);
    } else if (c >= '0' && c <= '9') {
        // Draw numbers
        SDL_Rect charRect = {x + 1, y + 2, 6, 12};
        SDL_RenderDrawRect(renderer, &charRect);
        if (c == '8') {
            SDL_RenderDrawLine(renderer, x + 1, y + 8, x + 7, y + 8);
        }
    } else if (c == ' ') {
        // Space - do nothing
    } else {
        // Other characters - simple rectangle
        SDL_Rect charRect = {x + 2, y + 6, 4, 4};
        SDL_RenderFillRect(renderer, &charRect);
    }
}

void drawText(const string& text, int x, int y, SDL_Color color = BLACK, int size = 1) {
    int currentX = x;
    int currentY = y;
    
    for (char c : text) {
        if (c == '\n') {
            currentY += CHAR_HEIGHT * size;
            currentX = x;
        } else {
            // For larger text, draw multiple times with offset
            for (int sy = 0; sy < size; sy++) {
                for (int sx = 0; sx < size; sx++) {
                    drawChar(c, currentX + sx, currentY + sy, color);
                }
            }
            currentX += CHAR_WIDTH * size;
        }
    }
}

// UI structures
struct Button {
    SDL_Rect rect;
    string text;
    SDL_Color color;
    SDL_Color textColor;
    bool enabled;
    
    Button(int x, int y, int w, int h, string txt, SDL_Color bg = BLUE, SDL_Color fg = WHITE) 
        : text(txt), color(bg), textColor(fg), enabled(true) {
        rect = {x, y, w, h};
    }
    
    bool isClicked(int mouseX, int mouseY) {
        return enabled && mouseX >= rect.x && mouseX <= rect.x + rect.w && 
               mouseY >= rect.y && mouseY <= rect.y + rect.h;
    }
};

struct InputBox {
    SDL_Rect rect;
    string* text;
    bool active;
    bool isPassword;
    
    InputBox(int x, int y, int w, int h, string* textPtr, bool password = false) 
        : text(textPtr), active(false), isPassword(password) {
        rect = {x, y, w, h};
    }
};

// Utility functions
bool initSDL() {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        cout << "SDL could not initialize! SDL_Error: " << SDL_GetError() << endl;
        return false;
    }
    
    window = SDL_CreateWindow("Android OS Simulation", 
                             SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 
                             WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN);
    if (!window) {
        cout << "Window could not be created! SDL_Error: " << SDL_GetError() << endl;
        return false;
    }
    
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer) {
        cout << "Renderer could not be created! SDL_Error: " << SDL_GetError() << endl;
        return false;
    }
    
    return true;
}

void drawButton(const Button& button) {
    // Draw button background
    SDL_SetRenderDrawColor(renderer, button.color.r, button.color.g, button.color.b, 255);
    SDL_RenderFillRect(renderer, &button.rect);
    
    // Draw button border
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderDrawRect(renderer, &button.rect);
    
    // Draw button text (centered)
    int textX = button.rect.x + 10;
    int textY = button.rect.y + (button.rect.h - CHAR_HEIGHT) / 2;
    drawText(button.text, textX, textY, button.textColor);
}

void drawInputBox(const InputBox& inputBox) {
    // Draw input box background
    SDL_Color bgColor = inputBox.active ? WHITE : LIGHT_GRAY;
    SDL_SetRenderDrawColor(renderer, bgColor.r, bgColor.g, bgColor.b, 255);
    SDL_RenderFillRect(renderer, &inputBox.rect);
    
    // Draw border
    SDL_Color borderColor = inputBox.active ? BLUE : GRAY;
    SDL_SetRenderDrawColor(renderer, borderColor.r, borderColor.g, borderColor.b, 255);
    SDL_RenderDrawRect(renderer, &inputBox.rect);
    
    // Draw text
    string displayText = *(inputBox.text);
    if (inputBox.isPassword && !displayText.empty()) {
        displayText = string(displayText.length(), '*');
    }
    drawText(displayText, inputBox.rect.x + 5, inputBox.rect.y + 8, BLACK);
    
    // Draw cursor if active
    if (inputBox.active) {
        int cursorX = inputBox.rect.x + 5 + (displayText.length() * CHAR_WIDTH);
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderDrawLine(renderer, cursorX, inputBox.rect.y + 5,
                          cursorX, inputBox.rect.y + inputBox.rect.h - 5);
    }
}

void drawProgressBar(int x, int y, int width, int height, float progress, SDL_Color color = GREEN) {
    // Draw background
    SDL_SetRenderDrawColor(renderer, 200, 200, 200, 255);
    SDL_Rect bgRect = {x, y, width, height};
    SDL_RenderFillRect(renderer, &bgRect);
    
    // Draw progress
    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, 255);
    SDL_Rect progressRect = {x, y, (int)(width * progress), height};
    SDL_RenderFillRect(renderer, &progressRect);
    
    // Draw border
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderDrawRect(renderer, &bgRect);
}

// State handling functions
void handleLoginState() {
    static InputBox usernameBox(300, 250, 200, 30, &inputBuffer);
    static InputBox passwordBox(300, 300, 200, 30, &passwordBuffer, true);
    static Button loginButton(350, 350, 100, 40, "Login");
    static Button guestButton(350, 400, 100, 40, "Guest Login", GREEN);
    static InputBox* activeInput = nullptr;
    static string errorMessage = "";
    
    // Set background
    SDL_SetRenderDrawColor(renderer, 100, 150, 200, 255);
    SDL_RenderClear(renderer);
    
    // Draw title
    drawText("ANDROID OS - LOGIN SYSTEM", 300, 100, WHITE, 2);
    
    // Draw labels
    drawText("Username:", 200, 255, WHITE);
    drawText("Password:", 200, 305, WHITE);
    
    // Draw input boxes
    drawInputBox(usernameBox);
    drawInputBox(passwordBox);
    
    // Draw buttons
    drawButton(loginButton);
    drawButton(guestButton);
    
    // Draw error message if any
    if (!errorMessage.empty()) {
        drawText(errorMessage, 250, 450, RED);
    }
    
    // Draw instructions
    drawText("Click on input fields to type", 200, 500, WHITE);
    drawText("Valid users: Admin, Guest, Developer", 200, 520, WHITE);
    drawText("Password: viethoang082013", 200, 540, WHITE);
    
    // Handle events
    SDL_Event e;
    while (SDL_PollEvent(&e)) {
        if (e.type == SDL_QUIT) {
            exit(0);
        } else if (e.type == SDL_MOUSEBUTTONDOWN) {
            int mouseX = e.button.x;
            int mouseY = e.button.y;
            
            // Check input boxes
            if (mouseX >= usernameBox.rect.x && mouseX <= usernameBox.rect.x + usernameBox.rect.w &&
                mouseY >= usernameBox.rect.y && mouseY <= usernameBox.rect.y + usernameBox.rect.h) {
                activeInput = &usernameBox;
                usernameBox.active = true;
                passwordBox.active = false;
            } else if (mouseX >= passwordBox.rect.x && mouseX <= passwordBox.rect.x + passwordBox.rect.w &&
                       mouseY >= passwordBox.rect.y && mouseY <= passwordBox.rect.y + passwordBox.rect.h) {
                activeInput = &passwordBox;
                passwordBox.active = true;
                usernameBox.active = false;
            } else {
                usernameBox.active = false;
                passwordBox.active = false;
                activeInput = nullptr;
            }
            
            // Check buttons
            if (loginButton.isClicked(mouseX, mouseY)) {
                if (inputBuffer == "Guest") {
                    currentUser = "Guest";
                    currentState = STATE_BOOTLOADER;
                    bootProgress = 0;
                    errorMessage = "";
                } else if ((inputBuffer == "Admin" || inputBuffer == "Developer") && 
                          passwordBuffer == password) {
                    currentUser = inputBuffer;
                    currentState = STATE_BOOTLOADER;
                    bootProgress = 0;
                    errorMessage = "";
                } else {
                    errorMessage = "Invalid credentials!";
                }
            } else if (guestButton.isClicked(mouseX, mouseY)) {
                currentUser = "Guest";
                currentState = STATE_BOOTLOADER;
                bootProgress = 0;
                errorMessage = "";
            }
        } else if (e.type == SDL_TEXTINPUT && activeInput) {
            *(activeInput->text) += e.text.text;
        } else if (e.type == SDL_KEYDOWN && activeInput) {
            if (e.key.keysym.sym == SDLK_BACKSPACE && !activeInput->text->empty()) {
                activeInput->text->pop_back();
            }
        }
    }
}

void handleBootloaderState() {
    // Set background
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);
    
    // Draw boot messages
    drawText("Copyright Beta Corporation (c). All rights reserved", 50, 100, WHITE);
    drawText("Console AndroidOS", 50, 130, WHITE);
    drawText("Build 234 (Kernel Version 20)", 50, 160, WHITE);
    
    if (bootProgress > 60) { // ~3 seconds at 60fps
        drawText("Launching RAM Test...", 50, 200, WHITE);
        
        // RAM test progress
        if (bootProgress > 120) { // ~6 seconds
            float progress = (float)(bootProgress - 120) / 120.0f; // 2 second test
            if (progress > 1.0f) progress = 1.0f;
            
            drawProgressBar(50, 250, 400, 20, progress, GREEN);
            drawText("RAM Test: " + to_string((int)(progress * 65535)), 50, 280, WHITE);
            
            if (progress >= 1.0f) {
                // Random chance of RAM test failure
                bool testFailed = (rand() % 100 < 20);
                
                if (!testFailed) {
                    drawText("RAM test results: Nominal", 50, 310, GREEN);
                    if (bootProgress > 300) { // Give time to read
                        currentState = STATE_MAIN_MENU;
                        // Reset for next boot
                        bootProgress = 0;
                        inputBuffer = "";
                        passwordBuffer = "";
                    }
                } else {
                    drawText("RAM test failure. Press any key to restart.", 50, 310, RED);
                    // Wait for key press to restart
                    SDL_Event e;
                    while (SDL_PollEvent(&e)) {
                        if (e.type == SDL_KEYDOWN || e.type == SDL_MOUSEBUTTONDOWN) {
                            currentState = STATE_LOGIN;
                            bootProgress = 0;
                            inputBuffer = "";
                            passwordBuffer = "";
                        }
                    }
                }
            }
        }
    }
    
    bootProgress++;
    
    SDL_Event e;
    while (SDL_PollEvent(&e)) {
        if (e.type == SDL_QUIT) {
            exit(0);
        }
    }
}

void handleMainMenuState() {
    // Set background based on theme
    if (theme == "Dark") {
        SDL_SetRenderDrawColor(renderer, 30, 30, 30, 255);
    } else {
        SDL_SetRenderDrawColor(renderer, 240, 240, 240, 255);
    }
    SDL_RenderClear(renderer);
    
    SDL_Color textColor = (theme == "Dark") ? WHITE : BLACK;
    
    // Draw title
    drawText("ANDROID OS MAIN MENU", 300, 30, textColor, 2);
    drawText("Current User: " + currentUser, 350, 60, textColor);
    drawText("Theme: " + theme + " Mode", 350, 80, textColor);
    
    // Create menu buttons in a grid layout
    vector<Button> menuButtons = {
        Button(50, 120, 180, 35, "1. Task Manager", BLUE),
        Button(250, 120, 180, 35, "2. Recycle Bin", BLUE),
        Button(450, 120, 180, 35, "3. Firewall", BLUE),
        Button(650, 120, 180, 35, "4. File Search", BLUE),
        
        Button(50, 170, 180, 35, "5. Benchmark", BLUE),
        Button(250, 170, 180, 35, "6. Bluetooth", BLUE),
        Button(450, 170, 180, 35, "7. Toggle Theme", ORANGE),
        Button(650, 170, 180, 35, "8. Defender", BLUE),
        
        Button(50, 220, 180, 35, "9. Scheduler", BLUE),
        Button(250, 220, 180, 35, "10. Hibernate", BLUE),
        Button(450, 220, 180, 35, "11. Lock Screen", BLUE),
        Button(650, 220, 180, 35, "12. Device Mgr", BLUE),
        
        Button(350, 300, 150, 40, "Restart", ORANGE),
        Button(350, 350, 150, 40, "Exit", RED)
    };
    
    // Draw buttons
    for (const auto& button : menuButtons) {
        drawButton(button);
    }
    
    // Draw system info
    drawText("System Status: Running", 50, 420, textColor);
    drawText("Uptime: " + to_string(bootProgress / 60) + " minutes", 50, 440, textColor);
    
    SDL_Event e;
    while (SDL_PollEvent(&e)) {
        if (e.type == SDL_QUIT) {
            exit(0);
        } else if (e.type == SDL_MOUSEBUTTONDOWN) {
            int mouseX = e.button.x;
            int mouseY = e.button.y;
            
            // Check which button was clicked
            for (size_t i = 0; i < menuButtons.size(); i++) {
                if (menuButtons[i].isClicked(mouseX, mouseY)) {
                    switch (i) {
                        case 0: currentState = STATE_TASK_MANAGER; break;
                        case 1: currentState = STATE_RECYCLE_BIN; break;
                        case 2: currentState = STATE_FIREWALL; break;
                        case 3: currentState = STATE_FILE_SEARCH; break;
                        case 4: currentState = STATE_BENCHMARK; break;
                        case 5: currentState = STATE_BLUETOOTH; break;
                        case 6: 
                            theme = (theme == "Light") ? "Dark" : "Light";
                            break;
                        case 7: currentState = STATE_DEFENDER; break;
                        case 8: currentState = STATE_SCHEDULER; break;
                        case 9: 
                            // Hibernate simulation
                            SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
                            SDL_RenderClear(renderer);
                            drawText("HIBERNATING...", 400, 350, WHITE, 2);
                            SDL_RenderPresent(renderer);
                            SDL_Delay(2000);
                            drawText("RESUMING...", 400, 380, GREEN, 2);
                            SDL_RenderPresent(renderer);
                            SDL_Delay(1000);
                            break;
                        case 10: currentState = STATE_LOCKED; break;
                        case 11: currentState = STATE_DEVICE_MANAGER; break;
                        case 12: 
                            // Restart
                            currentState = STATE_BOOTLOADER;
                            bootProgress = 0;
                            break;
                        case 13: exit(0); break;
                    }
                }
            }
        }
    }
    
    bootProgress++; // Keep counting uptime
}

void handleTaskManagerState() {
    SDL_SetRenderDrawColor(renderer, 240, 240, 240, 255);
    SDL_RenderClear(renderer);
    
    drawText("TASK MANAGER - RUNNING PROCESSES", 200, 50, BLACK, 2);
    
    // Draw process list with more details
    vector<pair<string, SDL_Color>> processes = {
        {"SYSTEM_KERNEL.EXE        Running    High      45MB", GREEN},
        {"FILE_MANAGER.EXE         Idle       Normal    12MB", BLUE}, 
        {"NETWORK_SERVICE.EXE      Running    High      23MB", GREEN},
        {"GRAPHICS_ENGINE.EXE      Running    Normal    67MB", GREEN},
        {"BACKGROUND_TASK.EXE      Sleeping   Low        8MB", GRAY},
        {"AUDIO_SERVICE.EXE        Running    Normal    15MB", GREEN},
        {"SECURITY_DAEMON.EXE      Running    High      34MB", GREEN}
    };
    
    drawText("Process Name              Status     Priority  Memory", 50, 100, BLACK);
    drawText("-------------------------------------------------------", 50, 115, BLACK);
    
    int y = 140;
    for (const auto& process : processes) {
        drawText(process.first, 50, y, process.second);
        y += 25;
    }
    
    // Draw system resources
    drawText("SYSTEM RESOURCES:", 50, 350, BLACK);
    drawText("CPU Usage:", 50, 380, BLACK);
    drawProgressBar(150, 380, 200, 15, 0.65f, GREEN);
    drawText("65%", 360, 378, BLACK);
    
    drawText("Memory Usage:", 50, 410, BLACK);
    drawProgressBar(150, 410, 200, 15, 0.78f, ORANGE);
    drawText("78%", 360, 408, BLACK);
    
    drawText("Disk Usage:", 50, 440, BLACK);
    drawProgressBar(150, 440, 200, 15, 0.45f, GREEN);
    drawText("45%", 360, 438, BLACK);
    
    Button backButton(400, 500, 100, 40, "Back", BLUE);
    Button killButton(520, 500, 120, 40, "Kill Process", RED);
    drawButton(backButton);
    drawButton(killButton);
    
    SDL_Event e;
    while (SDL_PollEvent(&e)) {
        if (e.type == SDL_QUIT) {
            exit(0);
        } else if (e.type == SDL_MOUSEBUTTONDOWN) {
            if (backButton.isClicked(e.button.x, e.button.y)) {
                currentState = STATE_MAIN_MENU;
            } else if (killButton.isClicked(e.button.x, e.button.y)) {
                // Simulate killing a process
                drawText("Process terminated!", 400, 470, RED);
                SDL_RenderPresent(renderer);
                SDL_Delay(1000);
            }
        }
    }
}

void handleGenericState(const string& title, const vector<string>& content, SDL_Color bgColor = {240, 240, 240, 255}) {
    SDL_SetRenderDrawColor(renderer, bgColor.r, bgColor.g, bgColor.b, 255);
    SDL_RenderClear(renderer);
    
    drawText(title, 200, 50, BLACK, 2);
    
    int y = 120;
    for (const auto& line : content) {
        drawText(line, 50, y, BLACK);
        y += 25;
    }
    
    Button backButton(400, 500, 100, 40, "Back");
    drawButton(backButton);
    
    SDL_Event e;
    while (SDL_PollEvent(&e)) {
        if (e.type == SDL_QUIT) {
            exit(0);
        } else if (e.type == SDL_MOUSEBUTTONDOWN) {
            if (backButton.isClicked(e.button.x, e.button.y)) {
                currentState = STATE_MAIN_MENU;
            }
        }
    }
}

void handleLockScreenState() {
    static string lockPassword = "";
    static bool showError = false;
    
    SDL_SetRenderDrawColor(renderer, 20, 20, 50, 255);
    SDL_RenderClear(renderer);
    
    drawText("SYSTEM LOCKED", 350, 200, WHITE, 2);
    drawText("Enter password to unlock:", 300, 280, WHITE);
    
    // Simple password input
    SDL_Rect passwordRect = {300, 320, 200, 30};
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderFillRect(renderer, &passwordRect);
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderDrawRect(renderer, &passwordRect);
    
    string displayPassword = string(lockPassword.length(), '*');
    drawText(displayPassword, 305, 325, BLACK);
    
    if (showError) {
        drawText("Incorrect password!", 320, 380, RED);
    }
    
    drawText("Hint: Try 'password' or 'unlock'", 280, 450, GRAY);
    
    SDL_Event e;
    while (SDL_PollEvent(&e)) {
        if (e.type == SDL_QUIT) {
            exit(0);
        } else if (e.type == SDL_TEXTINPUT) {
            lockPassword += e.text.text;
        } else if (e.type == SDL_KEYDOWN) {
            if (e.key.keysym.sym == SDLK_BACKSPACE && !lockPassword.empty()) {
                lockPassword.pop_back();
                showError = false;
            } else if (e.key.keysym.sym == SDLK_RETURN) {
                if (lockPassword == "password" || lockPassword == "unlock") {
                    currentState = STATE_MAIN_MENU;
                    lockPassword = "";
                    showError = false;
                } else {
                    showError = true;
                }
            }
        }
    }
}

void cleanup() {
    if (renderer) SDL_DestroyRenderer(renderer);
    if (window) SDL_DestroyWindow(window);
    SDL_Quit();
}

int main(int argc, char* argv[]) {
    srand(time(nullptr));
    
    if (!initSDL()) {
        return 1;
    }
    
    bool running = true;
    while (running) {
        switch (currentState) {
            case STATE_LOGIN:
                handleLoginState();
                break;
            case STATE_BOOTLOADER:
                handleBootloaderState();
                break;
            case STATE_MAIN_MENU:
                handleMainMenuState();
                break;
            case STATE_TASK_MANAGER:
                handleTaskManagerState();
                break;
            case STATE_RECYCLE_BIN:
                handleGenericState("RECYCLE BIN MANAGER", {
                    "Current items in recycle bin: " + to_string(recycleBin.size()),
                    "",
                    "Files deleted:",
                    "- temp_file.txt (deleted 2 hours ago)",
                    "- old_document.doc (deleted yesterday)",
                    "- screenshot.png (deleted 3 days ago)",
                    "",
                    "Total space used: 45.2 MB",
                    "Space that can be freed: 45.2 MB",
                    "",
                    "Operations:",
                    "- Empty Recycle Bin",
                    "- Restore Selected Items",
                    "- Properties"
                });
                break;
            case STATE_FIREWALL:
                handleGenericState("FIREWALL SETTINGS", {
                    "Windows Defender Firewall Status: ENABLED",
                    "",
                    "Network Profiles:",
                    "  Domain Networks: ON",
                    "  Private Networks: ON", 
                    "  Public Networks: ON",
                    "",
                    "Recent Activity:",
                    "  Blocked Connections: 23 (last hour)",
                    "  Allowed Connections: 156 (last hour)",
                    "",
                    "Allowed Programs:",
                    "  - File Explorer (System)",
                    "  - Calculator (Trusted)",
                    "  - Weather App (Trusted)",
                    "  - Chrome Browser (Trusted)",
                    "",
                    "Blocked Programs:",
                    "  - Unknown Sources",
                    "  - Suspicious Network Activity"
                });
                break;
            case STATE_FILE_SEARCH:
                handleGenericState("FILE SEARCH UTILITY", {
                    "Search Results for System Files:",
                    "",
                    "Found Files:",
                    "  ANDROID_OS.ISO     - System boot image",
                    "  WARNING_KNL.EXE    - Critical system file", 
                    "  README.TXT         - System documentation",
                    "  TASKS.LOG          - Task execution history",
                    "  CONFIG.SYS         - System configuration",
                    "",
                    "Search Statistics:",
                    "  Files Scanned: 1,247",
                    "  Folders Scanned: 156", 
                    "  Total Size: 2.3 GB",
                    "  Search Time: 0.8 seconds",
                    "",
                    "Recent Searches:",
                    "  - *.exe",
                    "  - config*",
                    "  - *.log"
                });
                break;
            case STATE_BENCHMARK:
                handleGenericState("SYSTEM BENCHMARK RESULTS", {
                    "Hardware Performance Analysis:",
                    "",
                    "CPU Performance:",
                    "  Single Core Score: 1,245 points",
                    "  Multi Core Score: 4,567 points",
                    "  Overall CPU Rating: 85% (Excellent)",
                    "",
                    "Memory Performance:",
                    "  RAM Speed: 3200MHz DDR4",
                    "  Memory Bandwidth: 45.6 GB/s",
                    "  Latency: 14.2 ns",
                    "",
                    "Storage Performance:",
                    "  Sequential Read: 520 MB/s",
                    "  Sequential Write: 480 MB/s",
                    "  Random Read IOPS: 75,000",
                    "  Random Write IOPS: 68,000",
                    "",
                    "Graphics Performance:",
                    "  DirectX 12: Supported",
                    "  OpenGL: 4.6",
                    "  Frame Rate (1080p): 144 FPS",
                    "",
                    "Overall System Score: 8.5/10"
                });
                break;
            case STATE_BLUETOOTH:
                handleGenericState("BLUETOOTH DEVICE MANAGER", {
                    "Bluetooth Status: ENABLED",
                    "Version: 5.2",
                    "Discoverable: Yes",
                    "",
                    "Paired Devices:",
                    "  [Connected] Wireless Headphones",
                    "    Battery: 78%",
                    "    Audio Quality: High",
                    "",
                    "  [Available] Samsung Galaxy Watch",
                    "    Last Connected: 2 hours ago",
                    "    Status: Ready to connect",
                    "",
                    "  [Available] Bluetooth Speaker",
                    "    Last Connected: Yesterday",
                    "    Status: Out of range",
                    "",
                    "Available Devices:",
                    "  iPhone 13 Pro",
                    "  AirPods Pro",
                    "  Nintendo Switch Pro Controller",
                    "",
                    "Recent Activity:",
                    "  File transfer completed to Galaxy Watch",
                    "  Audio stream started to Headphones"
                });
                break;
            case STATE_DEFENDER:
                handleGenericState("WINDOWS DEFENDER ANTIVIRUS", {
                    "Last Scan: Today at 3:42 PM",
                    "Scan Type: Quick Scan",
                    "Status: Your device is protected",
                    "",
                    "Threat Protection:",
                    "  Real-time Protection: ON",
                    "  Cloud Protection: ON", 
                    "  Sample Submission: ON",
                    "  Tamper Protection: ON",
                    "",
                    "Scan Results:",
                    "  Files Scanned: 127,543",
                    "  Threats Found: 0",
                    "  Time Elapsed: 2 minutes 34 seconds",
                    "",
                    "Protection History:",
                    "  Sept 10: Blocked suspicious download",
                    "  Sept 8: Quarantined malware sample",
                    "  Sept 5: Real-time scan blocked threat",
                    "",
                    "Virus Definitions:",
                    "  Last Updated: Today 2:15 PM",
                    "  Version: 1.397.1234.0"
                }, {220, 250, 220, 255}); // Light green background
                break;
            case STATE_SCHEDULER:
                handleGenericState("TASK SCHEDULER", {
                    "Active Scheduled Tasks:",
                    "",
                    "System Tasks:",
                    "  Windows Update Check - Daily 3:00 AM",
                    "  System File Cleanup - Weekly Sunday 2:00 AM", 
                    "  Registry Backup - Weekly Friday 1:00 AM",
                    "  Defragmentation - Monthly 1st Sunday 12:00 AM",
                    "",
                    "User Tasks:",
                    "  Backup Documents - Daily 6:00 PM",
                    "  Email Sync - Every 15 minutes",
                    "  Weather Update - Every 2 hours",
                    "",
                    "Recently Executed:",
                    "  System Update Check - Success (1 hour ago)",
                    "  Backup Documents - Success (2 hours ago)",
                    "  Registry Cleanup - Success (Yesterday)",
                    "",
                    "Next Scheduled Tasks:",
                    "  Email Sync - In 8 minutes",
                    "  Weather Update - In 45 minutes",
                    "  System Backup - Tonight 11:00 PM"
                });
                break;
            case STATE_DEVICE_MANAGER:
                handleGenericState("DEVICE MANAGER", {
                    "System Hardware Overview:",
                    "",
                    "Audio Devices:",
                    "  Realtek High Definition Audio - Working",
                    "  Bluetooth Audio Device - Working",
                    "",
                    "Display Adapters:",
                    "  NVIDIA GeForce RTX 3070 - Working",
                    "  Intel UHD Graphics 630 - Working",
                    "",
                    "Network Adapters:",
                    "  Intel Ethernet Controller - Connected",
                    "  Wi-Fi 6 AX200 Adapter - Connected",
                    "",
                    "USB Devices:",
                    "  USB Serial Port (COM2) - Working",
                    "    Driver: AndroidOS USB Driver v1.4.1",
                    "  USB Mass Storage Device - Working",
                    "  USB Wireless Mouse - Working",
                    "",
                    "Storage Controllers:",
                    "  NVMe SSD Controller - Working",
                    "  SATA AHCI Controller - Working",
                    "",
                    "System Summary: All devices working properly"
                });
                break;
            case STATE_LOCKED:
                handleLockScreenState();
                break;
        }
        
        SDL_RenderPresent(renderer);
        SDL_Delay(16); // ~60 FPS
    }
    
    cleanup();
    return 0;
}