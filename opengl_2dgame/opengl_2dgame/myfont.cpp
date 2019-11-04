#include "myfont.h"

MyFont::MyFont(Shader& shader) {
	//this->fontShader = shader;
	this->fontShader = shader;
	// 初始化字体
	if (FT_Init_FreeType(&this->ft)) {
		cout << "could not init freetype library " << endl;
	}
	if (FT_New_Face(this->ft, "fonts/arial.ttf", 0, &this->face)) {
		cout << "failed to load font" << endl;
	}
	// 定义字体大小 宽度设置为零表示我们要从字体面通过给定的高度中动态的计算字形的宽度
	FT_Set_Pixel_Sizes(this->face, 0, 48);
	// 默认填充characters
	// 禁用字节对齐限制
	glPixelStorei(GL_UNPACK_ALIGNMENT,1);
	for (uint c = 0; c < 128; c++) {
		// 加载字符的字形
		if (FT_Load_Char(this->face, c, FT_LOAD_RENDER)) {
			cout << "failed to load glyph" << endl;
			continue;
		}
		// 生成纹理
		uint texture;
		glGenTextures(1,&texture);
		glBindTexture(GL_TEXTURE_2D, texture);
		// 单个红色分量在着色器中只能取到r红色值的分量
		glTexImage2D(GL_TEXTURE_2D,0,GL_RED,this->face->glyph->bitmap.width,this->face->glyph->bitmap.rows,0,GL_RED,GL_UNSIGNED_BYTE,this->face->glyph->bitmap.buffer);
		// 设置纹理
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		Character character = {
			texture,glm::ivec2(this->face->glyph->bitmap.width,this->face->glyph->bitmap.rows),
			glm::ivec2(this->face->glyph->bitmap_left,this->face->glyph->bitmap_top),
			this->face->glyph->advance.x
		};
		this->characters.insert(pair<char, Character>(c, character));
		
		
	}
	/*this->fontShader->use();*/
	this->fontShader.use();
	glm::mat4 projection = glm::ortho(0.0f, 800.0f, 0.0f, 600.0f);
	this->fontShader.setMat4("projection", projection);
	this->initRenderData();
}
// 析构函数
MyFont::~MyFont() {
	// 清理ft资源
	FT_Done_FreeType(this->ft);
	// 清理面信息
	FT_Done_Face(this->face);
}
// 初始化渲染数据
void MyFont::initRenderData() {
	glGenVertexArrays(1,&this->VAO);
	glGenBuffers(1,&this->VBO);

	glBindBuffer(GL_ARRAY_BUFFER,this->VBO);
	glBufferData(GL_ARRAY_BUFFER,sizeof(float) * 6 * 4,NULL,GL_DYNAMIC_DRAW);
	glBindVertexArray(this->VAO);
	glVertexAttribPointer(0,4,GL_FLOAT,GL_FALSE,4 * sizeof(float),(void*)0);
	glEnableVertexAttribArray(0);

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER,0);
}
Character MyFont::getCharByName(const char& name) {
	return this->characters[name];
}
// 绘制文本
void MyFont::draw(string text,float x,float y,float scale,glm::vec3 color) {
	// 启用混合
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
	this->fontShader.use();
	this->fontShader.setInt("tex",0);
	glUniform3f(glGetUniformLocation(this->fontShader.ID,"textColor"),color.x,color.y,color.z);
	glActiveTexture(GL_TEXTURE0);
	glBindVertexArray(this->VAO);

	string::const_iterator c;
	for (c = text.begin(); c != text.end(); c++) {
		Character ch = characters[*c];
		float xpos = x + ch.bearing.x * scale;
		float ypos = y - (ch.size.y - ch.bearing.y) * scale;

		float w = ch.size.x * scale;
		float h = ch.size.y * scale;

		// 对每个字符更新VBO
		float vertices[6][4] = {
			{ xpos,     ypos + h,   0.0, 0.0 },
			{ xpos,     ypos,       0.0, 1.0 },
			{ xpos + w, ypos,       1.0, 1.0 },

			{ xpos,     ypos + h,   0.0, 0.0 },
			{ xpos + w, ypos,       1.0, 1.0 },
			{ xpos + w, ypos + h,   1.0, 0.0 }
		};
		glBindTexture(GL_TEXTURE_2D,ch.textureId);
		// 更新VBO内容
		glBindBuffer(GL_ARRAY_BUFFER,this->VBO);
		// 更新之前的buffer一部分数据
		glBufferSubData(GL_ARRAY_BUFFER,0,sizeof(vertices),vertices);
		glBindBuffer(GL_ARRAY_BUFFER,0);
		glDrawArrays(GL_TRIANGLES,0,6);
		// 更新下一个字形的x坐标 频繁更新顶点数据 需要GL_DYNAMIC_DRAW
		x += (ch.advance >> 6) * scale;
	}
	glBindVertexArray(0);
	glBindTexture(GL_TEXTURE_2D,0);
}