#include "texture.h"
Texture2D::Texture2D() :format_internal(GL_RGB), format_image(GL_RGB), wrap_s(GL_REPEAT), wrap_t(GL_REPEAT), filter_Min(GL_LINEAR), filter_Max(GL_LINEAR) {
	glGenTextures(1, &this->ID);
}
// 生成纹理赋值ID
void Texture2D::generate(uint width, uint height, uchar* data) {
	this->width = width;
	this->height = height;
	glBindTexture(GL_TEXTURE_2D, this->ID);
	glTexImage2D(GL_TEXTURE_2D, 0, this->format_internal, width, height, 0,this->format_image, GL_UNSIGNED_BYTE, data);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,this->wrap_s);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,this->wrap_t);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,this->filter_Min);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,this->filter_Max);
	glBindTexture(GL_TEXTURE_2D,0);
}
// 绑定纹理
void Texture2D::bind() {
	glBindTexture(GL_TEXTURE_2D,this->ID);
}
