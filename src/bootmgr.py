import pygame
import sys
import os
from enum import Enum

class BootState(Enum):
    SPLASH = 1
    MENU = 2
    LOADING = 3
    RUNNING = 4
    SHUTDOWN = 5

class BootManager:
    def __init__(self, width=800, height=600):
        pygame.init()
        self.width = width
        self.height = height
        self.screen = pygame.display.set_mode((width, height))
        pygame.display.set_caption("Console AndroidOS - Boot Manager")
        self.clock = pygame.time.Clock()
        
        # BIOS-style monospace font (use Courier New or fallback to monospace)
        self.font_large = pygame.font.Font("src\\fonts\\ttf - Px (pixel outline)\\Px437_DOS-V_TWN19.ttf", 60)
        self.font_med = pygame.font.Font("src\\fonts\\ttf - Px (pixel outline)\\Px437_DOS-V_TWN19.ttf", 39)
        self.font_small = pygame.font.Font("src\\fonts\\ttf - Px (pixel outline)\\Px437_DOS-V_TWN19.ttf", 20)
        
        self.state = BootState.SPLASH
        self.splash_timer = 0
        self.selected_option = 0
        self.running = True

    def handle_events(self):
        for event in pygame.event.get():
            if event.type == pygame.QUIT:
                self.running = False
            elif event.type == pygame.KEYDOWN:
                if self.state == BootState.SPLASH:
                    self.state = BootState.MENU
                elif self.state == BootState.MENU:
                    if event.key == pygame.K_UP:
                        self.selected_option = (self.selected_option - 1) % 3
                    elif event.key == pygame.K_DOWN:
                        self.selected_option = (self.selected_option + 1) % 3
                    elif event.key == pygame.K_RETURN:
                        self.handle_menu_selection()

    def handle_menu_selection(self):
        if self.selected_option == 0:
            # Boot normal
            self.state = BootState.LOADING
        elif self.selected_option == 1:
            # Boot safe mode (placeholder)
            print("Safe Mode selected")
            self.state = BootState.LOADING
        elif self.selected_option == 2:
            # Shutdown
            self.state = BootState.SHUTDOWN
            self.running = False

    def draw_splash(self):
        self.screen.fill((0, 0, 0))
        title = self.font_large.render("Console AndroidOS", True, (0, 255, 0))
        subtitle = self.font_med.render("Build 398", True, (0, 200, 0))
        copyright_text = self.font_small.render("Copyright Beta Corporation (c). All rights reserved.", True, (100, 100, 100))
        
        self.screen.blit(title, (self.width // 2 - title.get_width() // 2, self.height // 2 - 100))
        self.screen.blit(subtitle, (self.width // 2 - subtitle.get_width() // 2, self.height // 2 - 20))
        self.screen.blit(copyright_text, (self.width // 2 - copyright_text.get_width() // 2, self.height - 100))
        
        press_key = self.font_small.render("Press any key to continue...", True, (100, 255, 100))
        self.screen.blit(press_key, (self.width // 2 - press_key.get_width() // 2, self.height - 50))

    def draw_menu(self):
        self.screen.fill((0, 0, 0))
        title = self.font_large.render("Boot Options", True, (0, 255, 0))
        self.screen.blit(title, (self.width // 2 - title.get_width() // 2, 50))

        options = [
            "1. Boot Normal",
            "2. Boot Safe Mode",
            "3. Shutdown"
        ]

        for idx, option in enumerate(options):
            color = (255, 255, 0) if idx == self.selected_option else (0, 200, 0)
            marker = ">>> " if idx == self.selected_option else "    "
            text = self.font_med.render(marker + option, True, color)
            self.screen.blit(text, (100, 200 + idx * 80))

        hint = self.font_small.render("Use UP/DOWN to select, ENTER to confirm", True, (100, 100, 100))
        self.screen.blit(hint, (self.width // 2 - hint.get_width() // 2, self.height - 50))

    def draw_loading(self):
        self.screen.fill((0, 0, 0))
        loading_text = self.font_large.render("Loading...", True, (0, 255, 0))
        self.screen.blit(loading_text, (self.width // 2 - loading_text.get_width() // 2, self.height // 2))
        pygame.display.flip()
        pygame.time.wait(2000)
        self.state = BootState.RUNNING

    def draw_running(self):
        os.system("secup2")
        exit(0)

    def update(self):
        if self.state == BootState.SPLASH:
            self.splash_timer += 1
        elif self.state == BootState.RUNNING:
            self.draw_running()

    def draw(self):
        if self.state == BootState.SPLASH:
            self.draw_splash()
        elif self.state == BootState.MENU:
            self.draw_menu()
        elif self.state == BootState.LOADING:
            self.draw_loading()
        elif self.state == BootState.RUNNING:
            pass  # drawn in update

        pygame.display.flip()

    def run(self):
        while self.running:
            self.handle_events()
            self.update()
            self.draw()
            self.clock.tick(60)

        pygame.quit()
        sys.exit(0)

if __name__ == "__main__":
    manager = BootManager()
    manager.run()