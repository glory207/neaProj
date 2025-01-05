#include "texture.h"
std::vector<const char*> texturesNames = {"img/brick.jpg","img/playerAni/walk0001-Sheet.png","img/playerAni/idle0001-Sheet.png",
    "img/playerAni/crouchWalk0001-Sheet.png",
    "img/playerAni/crouchWalkNormal0001-Sheet.png",
    "img/brickN.png",
    "img/playerAni/walkNormal0001-Sheet.png",
    "img/playerAni/idleNormal0001-Sheet.png",
    "img/playerAni/run0001-Sheet.png",
    "img/playerAni/runNormal0001-Sheet.png",
    "img/MapOverlay.png",
    "img/scary.jpg",
    "img/wood-normal.jpg",//0
    "img/treasureCh.png",//7
    "img/SpriteNorm-0000.png",
    "img/cubeTableTexture.png",//1
    "img/cubeBedTexture.png",//2
    "img/playerAni/crouch0001-Sheet.png",
    "img/playerAni/crouchNormal0001-Sheet.png",
    "img/playerAni/roll0001-Sheet.png",
    "img/playerAni/rollNormal0001-Sheet.png",
    "img/playerAni/dive0001-Sheet.png",
    "img/playerAni/diveNormal0001-Sheet.png",
    "img/torch.png",//3
    "img/chair.png",//4
    "img/bookshelf.png",//5
    "img/Pillar.png"//6,
};

std::vector<int> texturesID = { -5,
    -5,
    -5,
    -5,
    -5,
    -5,
    -5,
    -5,
    -5,
    -5,
    -5,
    -5,
    -5,
    -5,
    -5,
    -5,
    -5,
    -5,
    -5,
    -5,
    -5,
    -5,
    -5,
    -5,
    -5,
    -5,
    -5,
    -5,
    -5,
    -5,
};

std::vector<GLuint64> texturesHandle = { 0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
};
GLuint texture(int imj, bool h) {
    if (texturesID[imj] == -5) {

        GLuint ID;
        GLuint64 handle;
        int widthImg, heightImg, numColCh;
        //stbi_set_flip_vertically_on_load(true);
        unsigned char* bytes = stbi_load(texturesNames[imj], &widthImg, &heightImg, &numColCh, 0);
        GLint internalFormat;
        GLenum format;

        switch (numColCh) {
        case 1: // Grayscale
            internalFormat = GL_RED;
            format = GL_RED;
            break;
        case 3: // RGB
            internalFormat = GL_RGB;
            format = GL_RGB;
            break;
        case 4: // RGBA
            internalFormat = GL_RGBA;
            format = GL_RGBA;
            break;
        default:

            stbi_image_free(bytes);
            return -1;
        }
        glGenTextures(1, &ID);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, ID);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);


        glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, widthImg, heightImg, 0, format, GL_UNSIGNED_BYTE, bytes);
        glGenerateMipmap(GL_TEXTURE_2D);
        handle = glGetTextureHandleARB(ID);
        glMakeTextureHandleResidentARB(handle);
        stbi_image_free(bytes);

        glBindTexture(GL_TEXTURE_2D, 0);
        texturesID[imj] = ID;
        texturesHandle[imj] = handle;
        if (h) return handle;
        else return ID;
    }
    else
    {
        if (h) return texturesHandle[imj];
        else return texturesID[imj];
    }
}
GLuint64 GetTexturesHandle(int imj) {
    if (texturesID[imj] == -5) {

      GLuint ID;
      GLuint64 handle;
      int widthImg, heightImg, numColCh;
      //stbi_set_flip_vertically_on_load(true);
      unsigned char* bytes = stbi_load(texturesNames[imj], &widthImg, &heightImg, &numColCh, 0);
      GLint internalFormat;
      GLenum format;

      switch (numColCh) {
      case 1: // Grayscale
          internalFormat = GL_RED;
          format = GL_RED;
          break;
      case 3: // RGB
          internalFormat = GL_RGB;
          format = GL_RGB;
          break;
      case 4: // RGBA
          internalFormat = GL_RGBA;
          format = GL_RGBA;
          break;
      default:

          stbi_image_free(bytes);
          return -1;
      }
      glGenTextures(1, &ID);
      glActiveTexture(GL_TEXTURE0);
      glBindTexture(GL_TEXTURE_2D, ID);

      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);


      glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, widthImg, heightImg, 0, format, GL_UNSIGNED_BYTE, bytes);
      glGenerateMipmap(GL_TEXTURE_2D);
      handle = glGetTextureHandleARB(ID);
      glMakeTextureHandleResidentARB(handle);
      stbi_image_free(bytes);

      glBindTexture(GL_TEXTURE_2D, 0);
      texturesID[imj] = ID;
      texturesHandle[imj] = handle;
  }
    return texturesHandle[imj];
}