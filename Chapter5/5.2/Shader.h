// ----------------------------------------------------------------
// From Game Programming in C++ by Sanjay Madhav
// Copyright (C) 2017 Sanjay Madhav. All rights reserved.
// 
// Released under the BSD License
// See LICENSE in root directory for full details.
// ----------------------------------------------------------------

#pragma once
#include <GL/glew.h>
#include <string>
#include "Math.h"

class Shader
{
public:
	Shader();
	~Shader();
	// Load the vertex/fragment shaders with the given names
	// 주어진 이름으로 버텍스/프래그먼트 셰이더 로드, Load 함수 내에서 ComplieShader(),IsCompiled(),IsValidProgram() 호출
	bool Load(const std::string& vertName, const std::string& fragName);
	void Unload();
	// Set this as the active shader program
	// 이 셰이더를 활성화된 셰이더 프로그램으로 설정
	void SetActive();
	// Sets a Matrix uniform
	void SetMatrixUniform(const char* name, const Matrix4& matrix);
private:
	// Tries to compile the specified shader
	// 지정된 셰이더를 컴파일
	bool CompileShader(const std::string& fileName,
					   GLenum shaderType,
					   GLuint& outShader);
	
	// Tests whether shader compiled successfully
	// 셰이더가 성공적으로 컴파일 됐는지 확인
	bool IsCompiled(GLuint shader);
	// Tests whether vertex/fragment programs link
	// 버텍스/프래그먼트 프로그램이 연결됐는지 확인
	bool IsValidProgram();
private:
	// Store the shader object IDs
	// 셰이더 오브젝트 ID를 저장
	GLuint mVertexShader;
	GLuint mFragShader;
	GLuint mShaderProgram;
};
