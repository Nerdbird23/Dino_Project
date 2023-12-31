/****************************************************************************
* File:      Dino.c
* Author:    Stephanie Hannis and Gautam Nandakumar
* Purpose:   Group Progect
* Version:   1.0, October 13, 2023 
* Resources: My tutor Adam and my unlce Jer Bear
C Program (80%)

Your finished project should demonstrate the following concepts we learned in class:

thoughtful selection of variable names x
easy to read and well documented code  x
arrays                                 0
robust error checking                  x
if statements                          x
while and/or do-while                  x
for loops                              0
multiple user defined functions        x
file I/O                               0??????
multiple source code files and header files 0
good programming practices             x
C comments describing the code         x
professionalism                        x
style guide Google?s coding guidelines: https://Links to an external site.google.github.io/styleguide/cppguide.htmlLinks to an external site.  Your Style Guide doesn?t have to be nearly this extensive, but when I look at the files produced by different group members, coding styles should match.)
Include a comment section at the top of each file with the name of the Group member that contributed to the code.  Each group member should have an equal role designing/writing/debugging code. 



SDL_image 3.0

The latest version of this library is available from GitHub:
https://github.com/libsdl-org/SDL_image/releases

This is a simple library to load images of various formats as SDL surfaces.
It can load BMP, GIF, JPEG, LBM, PCX, PNG, PNM (PPM/PGM/PBM), QOI, TGA, XCF, XPM, and simple SVG format images. It can also load AVIF, JPEG-XL, TIFF, and WebP images, depending on build options (see the note below for details.)

API:
#include <SDL3_image/SDL_image.h>

	SDL_Surface *IMG_Load(const char *file);
or
	SDL_Surface *IMG_Load_RW(SDL_RWops *src, SDL_bool freesrc);
or
	SDL_Surface *IMG_LoadTyped_RW(SDL_RWops *src, SDL_bool freesrc, char *type);

where type is a string specifying the format (i.e. "PNG" or "pcx").
Note that IMG_Load_RW cannot load TGA images.

To create a surface from an XPM image included in C source, use:

	SDL_Surface *IMG_ReadXPMFromArray(char **xpm);

An example program 'showimage' is included, with source in examples/showimage.c

Documentation is also available online at https://wiki.libsdl.org/SDL_image

This library is under the zlib License, see the file "LICENSE.txt" for details.

Note:
Support for AVIF, JPEG-XL, TIFF, and WebP are not included by default because of the size of the decode libraries, but you can get them by running external/download.sh
- When building with CMake, you can enable the appropriate SDL3IMAGE_* options defined in CMakeLists.txt. SDL3IMAGE_VENDORED allows switching between system and vendored libraries.
- When building with configure/make, you can build and install them normally and the configure script will detect and use them.
- When building with Visual Studio, you will need to build the libraries and then add the appropriate LOAD_* preprocessor define to the Visual Studio project.
- When building with Xcode, you can edit the config at the top of the project to enable them, and you will need to include the appropriate framework in your application.
- For Android, you can edit the config at the top of Android.mk to enable them.

*****************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#define   SDL2_MAIN_HANDLED
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 400
#define DINO_WIDTH 20
#define DINO_HEIGHT 22
#define OBSTACLE_WIDTH 40
#define OBSTACLE_HEIGHT 40
#define GRAVITY 0.5
#define JUMP_STRENGTH -1200


/* Game starting Screen */
int User_Interface(){
  int user_input;
  int z;
  printf("Would you like to play the dinosaur game? (1=\e[1;32myes1\e[0m, \e[1;310=no1\e[0m)\n");
  z = scanf("%d", &user_input);
  while (user_input != 1 && user_input != 0 ){
    printf("Invalid input, please enter either 1 or 0.\n");
    if (z == 0){
      getchar();
    }
    scanf("%d", &user_input);
  }
  return user_input;  
}


/* Instructions for the game MAKE THIS APPEND TO A FILE*/
void instructions(){
  printf("Instructions;\n");
  printf("Press the 'up' key or W to jump.\n");
  printf("Press the 'down' key or S to duck.\n");
  printf("Press P to pause the game.\n");
  printf("Press 'Escape' to exit the game.\n");
  printf("Press any key to go back to the menu.\n");
  getchar();
}

/* Structure for the Dino character */
typedef struct { /*Typedef redefines the name of  an already existing data type, struct groups several variables in one place */
    SDL_Texture* texture;
    SDL_Rect rect;
    SDL_Surface * image;
    double yVelocity;
    int isJumping;
} Dino;

/* Structure for the obstacle */
typedef struct {
    SDL_Texture* texture;
    SDL_Rect rect;
    SDL_Surface * image;
} Obstacle;

SDL_Window* window;
SDL_Renderer* renderer;

Dino dino;
Obstacle obstacle;

/* Initialize SDL and create a window and renderer */
int initialize(){
    if (SDL_Init(SDL_INIT_VIDEO) != 0){
        fprintf(stderr, "SDL_Init failed: %s\n", SDL_GetError());
        return 1;
    }

    window = SDL_CreateWindow("Dino Game", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, 0);
    if (window == NULL){
        fprintf(stderr, "SDL_CreateWindow failed: %s\n", SDL_GetError());
        SDL_Quit();
        return 1;
    }

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (renderer == NULL){
        fprintf(stderr, "SDL_CreateRenderer failed: %s\n", SDL_GetError());
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    return 0;
}

/* Load a PNG image as a texture */
SDL_Texture* loadTexture(const char* imagePath){
    SDL_Surface* image = IMG_Load(imagePath);
    if (image == NULL){
        fprintf(stderr, "IMG_Load failed: %s\n", SDL_GetError());
        return NULL;
    }

    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, image);
    SDL_FreeSurface(image);

    return texture;
}

/* Load game textures (this is also for error handling) */
int loadGameTextures(){
    SDL_Surface* image;
    dino.texture = loadTexture("Normal_Bean.png");
    if (dino.texture == NULL){
        return 1;
    }

    obstacle.texture = loadTexture("Cactus_1.png");
    if (obstacle.texture == NULL){
        return 1;
    }

    return 0;
}

/* Initialize the Dino character */
void initializeDino(){
    dino.rect.x = 100;
    dino.rect.y = SCREEN_HEIGHT - DINO_HEIGHT;
    dino.rect.w = DINO_WIDTH;
    dino.rect.h = DINO_HEIGHT;
    dino.yVelocity = 0;
    dino.isJumping = 0;
}

/* Initialize the obstacle */
void initializeObstacle(){
    obstacle.rect.x = SCREEN_WIDTH;
    obstacle.rect.y = SCREEN_HEIGHT - OBSTACLE_HEIGHT;
    obstacle.rect.w = OBSTACLE_WIDTH;
    obstacle.rect.h = OBSTACLE_HEIGHT;
}


/* destroys the obstacles and dino */
void destroy()
{
  if (dino.texture  != NULL){
    SDL_DestroyTexture(dino.texture);
  }
  if (dino.image != NULL){
    SDL_FreeSurface(dino.image);
  }
  if (obstacle.texture != NULL){
    SDL_DestroyTexture(obstacle.texture);
  }
  if (obstacle.image != NULL){
    SDL_FreeSurface(obstacle.image);
  }
}

/* Handle user input and character jump */
void handleInput(){
    SDL_Event event;
    while (SDL_PollEvent(&event)){
        if (event.type == SDL_QUIT){
            SDL_Quit();
            exit(0);
        }
        else if (event.type == SDL_KEYDOWN || event.type == 's'){
            if (event.key.keysym.sym == SDLK_SPACE && !dino.isJumping){
                dino.yVelocity = JUMP_STRENGTH;
                dino.isJumping = 1; 
            }
        }
    }
}

/* Update Dino character's position */
void updateDino() {
    if (dino.isJumping) {
        dino.rect.y += dino.yVelocity;
        dino.yVelocity += GRAVITY;

        if (dino.rect.y >= SCREEN_HEIGHT - DINO_HEIGHT){
            dino.rect.y = SCREEN_HEIGHT - DINO_HEIGHT;
            dino.isJumping = 0;
        }
    }
}

/* Update obstacle's position */
void updateObstacle(){
    obstacle.rect.x -= 5;

    if (obstacle.rect.x + OBSTACLE_WIDTH < 0){
        obstacle.rect.x = SCREEN_WIDTH;
    }
}

/* Render Dino character and obstacle */
void render(){
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderClear(renderer);

    SDL_RenderCopy(renderer, dino.texture, NULL, &dino.rect);
    SDL_RenderCopy(renderer, obstacle.texture, NULL, &obstacle.rect);

    SDL_RenderPresent(renderer);
}

/* Main function */
int main(int argc, char* argv[]){
  int user_output = User_Interface();
  
    if (user_output == 1){ /* If the user agrees to play the game */
      if (initialize() != 0){ 
        return 1;
      }

      if (loadGameTextures() != 0){ /* Loads the png files */
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
      }

      initializeDino(); /* Initializes the Dino */
      initializeObstacle(); /* Initializes the obsticales */

      while (1){
        handleInput(); /* While everything is running (or true) */
        updateDino();
        updateObstacle();
        render();
      }
  
    } 
    else if (user_output == '0'){
        printf("Exited Dino.c\n");
    }   
  return 0;
}
