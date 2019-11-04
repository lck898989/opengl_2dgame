#include "myfont.h"

MyFont::MyFont(Shader& shader) {
	//this->fontShader = shader;
	this->fontShader = shader;
	// ��ʼ������
	if (FT_Init_FreeType(&this->ft)) {
		cout << "could not init freetype library " << endl;
	}
	if (FT_New_Face(this->ft, "fonts/arial.ttf", 0, &this->face)) {
		cout << "failed to load font" << endl;
	}
	// ���������С �������Ϊ���ʾ����Ҫ��������ͨ�������ĸ߶��ж�̬�ļ������εĿ��
	FT_Set_Pixel_Sizes(this->face, 0, 48);
	// Ĭ�����characters
	// �����ֽڶ�������
	glPixelStorei(GL_UNPACK_ALIGNMENT,1);
	for (uint c = 0; c < 128; c++) {
		// �����ַ�������
		if (FT_Load_Char(this->face, c, FT_LOAD_RENDER)) {
			cout << "failed to load glyph" << endl;
			continue;
		}
		// ��������
		uint texture;
		glGenTextures(1,&texture);
		glBindTexture(GL_TEXTURE_2D, texture);
		// ������ɫ��������ɫ����ֻ��ȡ��r��ɫֵ�ķ���
		glTexImage2D(GL_TEXTURE_2D,0,GL_RED,this->face->glyph->bitmap.width,this->face->glyph->bitmap.rows,0,GL_RED,GL_UNSIGNED_BYTE,this->face->glyph->bitmap.buffer);
		// ��������
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
// ��������
MyFont::~MyFont() {
	// ����ft��Դ
	FT_Done_FreeType(this->ft);
	// ��������Ϣ
	FT_Done_Face(this->face);
}
// ��ʼ����Ⱦ����
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
// �����ı�
void MyFont::draw(string text,float x,float y,float scale,glm::vec3 color) {
	// ���û��
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

		// ��ÿ���ַ�����VBO
		float vertices[6][4] = {
			{ xpos,     ypos + h,   0.0, 0.0 },
			{ xpos,     ypos,       0.0, 1.0 },
			{ xpos + w, ypos,       1.0, 1.0 },

			{ xpos,     ypos + h,   0.0, 0.0 },
			{ xpos + w, ypos,       1.0, 1.0 },
			{ xpos + w, ypos + h,   1.0, 0.0 }
		};
		glBindTexture(GL_TEXTURE_2D,ch.textureId);
		// ����VBO����
		glBindBuffer(GL_ARRAY_BUFFER,this->VBO);
		// ����֮ǰ��bufferһ��������
		glBufferSubData(GL_ARRAY_BUFFER,0,sizeof(vertices),vertices);
		glBindBuffer(GL_ARRAY_BUFFER,0);
		glDrawArrays(GL_TRIANGLES,0,6);
		// ������һ�����ε�x���� Ƶ�����¶������� ��ҪGL_DYNAMIC_DRAW
		x += (ch.advance >> 6) * scale;
	}
	glBindVertexArray(0);
	glBindTexture(GL_TEXTURE_2D,0);
}