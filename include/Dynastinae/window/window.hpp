#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_events.h>
#include <SDL2/SDL_log.h>
#include <SDL2/SDL_timer.h>
#include <SDL2/SDL_video.h>

#include <SDL2/SDL_image.h>
#include <SDL2/SDL_audio.h>
#include <SDL2/SDL_ttf.h>

#include <GL/glew.h>

#include <algorithm>
#include <string>
#include <vector>

#include "Dynastinae/types/color.hpp"
#include "Dynastinae/input/keyboard.hpp"
#include "Dynastinae/input/mouse.hpp"
#include "Dynastinae/window/events.hpp"

class Window {
	public:
		Window(const std::string& title, const uint32 width, const uint32 height, const bool vsync = true, const bool debug_info = false);
		~Window() {
			// Delete objs
			delete this->keyboard_handler;
			delete this->mouse_handler;

			// Delete window
			SDL_Log("Window %d destroyed", SDL_GetWindowID(this->window));
			SDL_GL_DeleteContext(this->glContext);
			SDL_DestroyWindow(this->window);

			// Close all
			IMG_Quit();
			SDL_CloseAudio();
			TTF_Quit();
			SDL_Quit();
		}

		/*
		 * Runs in the frame beggining
		 *
		 * This method should run on once per frame
		 */
		void process_events();

		/**
		 * WINDOW PROCESS */

		// True while the window is open
		inline bool is_open() {
			return this->window_open;
		}

		/*
		 * Runs in the frame ending
		 *
		 * Swap buffers between frames, so the screen updates
		 */
		inline void swap_buffers() {
			this->frame_events.clear(); // Clear events
			SDL_GL_SwapWindow(this->window);
		}

		// Clear the screen with some color
		inline void clear(const Color& color) {
			glClearColor(color.r / 255.0f, color.g / 255.0f, color.b / 255.0f, color.a / 255.0f);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		}


		/**
		 * GETTERS */
		// inline SDL_Window* get_reference() const {
		// 	return this->window;
		// }

		inline Keyboard* keyboard() const {
			return this->keyboard_handler;
		}

		inline Mouse* mouse() const {
			return this->mouse_handler;
		}

		inline std::string get_title() const {
			return this->title;
		}

		inline uint32 get_width() const {
			return this->width;
		}

		inline uint32 get_height() const {
			return this->height;
		}

		// Check if event in array is not 0, if not, event is activated
		inline bool trigger_event(Event event) {
			return std::find(this->frame_events.begin(), this->frame_events.end(),
					static_cast<uint32>(event)) != this->frame_events.end();
		}

		/**
		 * SETTERS */
		inline void close() {
			this->window_open = false;
		}

		inline void set_title(const std::string& title) {
			this->title = title.c_str();
			SDL_SetWindowTitle(this->window, title.c_str());
		}

		// TODO -- Put on mouse struct, but how? how can i avoid circular?
		void set_cursor_position(const int x, const int y);


		/**
		 * TIMER */

		// Automatically calculates FPS
		float fps();

		/*
		 * Get the number of milliseconds since SDL library initialization.
		 *
		 * This value wraps if the program runs for more than ~49 days.
		 */
		inline uint32 time() {
			return SDL_GetTicks();
		}

		// Get the time elapsed between frames
		inline double dt(const float time = 1000.0f) const {
			// CURRENT - LAST to seconds
			return static_cast<double>(SDL_GetTicks() - this->last_update) / time;
		}

		/**
		* STATIC */
		static inline int static_width() {
			int width;
			SDL_GetWindowSize(SDL_GL_GetCurrentWindow(), &width, NULL);
			return width;
		}

		static inline int static_height() {
			int height;
			SDL_GetWindowSize(SDL_GL_GetCurrentWindow(), NULL, &height);
			return height;
		}

	private:
		// Window information
		std::string title;
		uint32 width;
		uint32 height;
		bool window_open = true;

		std::vector<uint32> frame_events;

		// SDL2
		SDL_Window* window;
		SDL_GLContext glContext;

		// FPS
		uint32 start_time = SDL_GetTicks();
		uint32 last_update = 0;
		uint32 frame_count = 0;
		float FPS = 0.0f;

		// KEYS
		// Since it has a get for these objects, it need to be pointers, so i can pass as reference to other methods
		Keyboard* keyboard_handler = new Keyboard();
		Mouse* mouse_handler = new Mouse(); // To not have the same name as the "mouse" function
};
