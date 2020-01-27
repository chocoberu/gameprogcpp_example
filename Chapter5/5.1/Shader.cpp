// ----------------------------------------------------------------
// From Game Programming in C++ by Sanjay Madhav
// Copyright (C) 2017 Sanjay Madhav. All rights reserved.
// 
// Released under the BSD License
// See LICENSE in root directory for full details.
// ----------------------------------------------------------------

#include "Shader.h"
#include "Texture.h"
#include <SDL/SDL.h>
#include <fstream>
#include <sstream>

Shader::Shader()
	: mShaderProgram(0)
	, mVertexShader(0)
	, mFragShader(0)
{
	
}

Shader::~Shader()
{

}

bool Shader::Load(const std::string& vertName, const std::string& fragName)
{
	// Compile vertex and pixel shaders
	// 버텍스/프래그먼트 셰이더를 컴파일
	if (!CompileShader(vertName,
		GL_VERTEX_SHADER,
		mVertexShader) || // 버텍스 셰이더의 오브젝트 ID를 mVertexShader에 저장
		!CompileShader(fragName,
			GL_FRAGMENT_SHADER,
			mFragShader)) // 프래그먼트 셰이더의 오브젝트 ID를 mFragShader에 저장
	{
		return false;
	}
	
	// Now create a shader program that
	// links together the vertex/frag shaders
	// 버텍스/프래그먼트 셰이더를 서로 연결하는 셰이더 프로그램 생성
	mShaderProgram = glCreateProgram(); // 셰이더 프로그램 생성
	glAttachShader(mShaderProgram, mVertexShader); // 셰이더 프로그램에 버텍스 셰이더를 연결
	glAttachShader(mShaderProgram, mFragShader); // 셰이더 프로그램에 프래그먼트 셰이더를 연결
	glLinkProgram(mShaderProgram); // 모든 추가된 셰이더를 최종 셰이더 프로그램에 연결
	
	// Verify that the program linked successfully
	// 프로그램이 성공적으로 링크됐는지 검증
	if (!IsValidProgram())
	{
		return false;
	}
	
	return true;
}

void Shader::Unload()
{
	// Delete the program/shaders
	// 프로그램, 셰이더를 제거
	glDeleteProgram(mShaderProgram);
	glDeleteShader(mVertexShader);
	glDeleteShader(mFragShader);
}

void Shader::SetActive()
{
	// Set this program as the active one
	glUseProgram(mShaderProgram);
}

void Shader::SetMatrixUniform(const char* name, const Matrix4& matrix)
{
	// Find the uniform by this name
	// 해당 이름의 uniform을 찾는다. uniform은 셰이더 프로그램의 수많은 호출 사이에서도 동일하게 유지되는 전역 변수
	GLuint loc = glGetUniformLocation(mShaderProgram, name);
	// Send the matrix data to the uniform
	// 행렬 데이터를 uniform에 전송한다
	glUniformMatrix4fv(loc, // uniform ID
		1, // 행렬의 수 (이번 경우는 오직 하나)
		GL_TRUE, // 행 벡터를 사용하면 TRUE로 설정
		matrix.GetAsFloatPtr()); // 행렬 데이터에 대한 포인터
}

bool Shader::CompileShader(const std::string& fileName,
	GLenum shaderType,
	GLuint& outShader)
{
	// Open file
	// 파일을 연다
	std::ifstream shaderFile(fileName);
	if (shaderFile.is_open())
	{
		// Read all the text into a string
		// 모든 텍스트를 읽어서 string으로 만든다
		std::stringstream sstream;
		sstream << shaderFile.rdbuf();
		std::string contents = sstream.str();
		const char* contentsChar = contents.c_str();
		
		// Create a shader of the specified type
		// 지정된 타입의 셰이더를 생성한다
		outShader = glCreateShader(shaderType); // OpenGL 셰이더 오브젝트 생성, ID를 outshader에 저장
		// Set the source characters and try to compile
		// 소스 문자열을 설정하고 컴파일 시도
		glShaderSource(outShader, 1, &(contentsChar), nullptr); // 셰이더 소스 코드를 포함하는 string을 지정
		glCompileShader(outShader); // 코드 컴파일
		
		if (!IsCompiled(outShader)) // 컴파일 실패
		{
			SDL_Log("Failed to compile shader %s", fileName.c_str());
			return false;
		}
	}
	else // 해당 셰이더 파일이 없을 때
	{
		SDL_Log("Shader file not found: %s", fileName.c_str());
		return false;
	}
	
	return true;
}

bool Shader::IsCompiled(GLuint shader)
{
	GLint status;
	// Query the compile status
	// 컴파일 상태를 질의
	glGetShaderiv(shader, GL_COMPILE_STATUS, &status); // 셰이더 컴파일 상태를 정수값으로 반환
	
	if (status != GL_TRUE) // 컴파일 에러가 생긴 경우
	{
		char buffer[512];
		memset(buffer, 0, 512);
		glGetShaderInfoLog(shader, 511, nullptr, buffer); // 컴파일 에러 메시지
		SDL_Log("GLSL Compile Failed:\n%s", buffer);
		return false;
	}
	
	return true;
}

bool Shader::IsValidProgram()
{
	
	GLint status;
	// Query the link status
	glGetProgramiv(mShaderProgram, GL_LINK_STATUS, &status);
	if (status != GL_TRUE)
	{
		char buffer[512];
		memset(buffer, 0, 512);
		glGetProgramInfoLog(mShaderProgram, 511, nullptr, buffer);
		SDL_Log("GLSL Link Status:\n%s", buffer);
		return false;
	}
	
	return true;
}
