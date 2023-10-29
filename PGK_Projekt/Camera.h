#pragma once
#ifndef camera_h
#define camera_h
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "gtc/matrix_transform.hpp"
/**
 * @brief Klasa reprezentująca kamerę
 * 
 */
class Camera {
private:
	/**
	 * @brief pozycja kamery
	 * 
	 */
	glm::vec3 cameraPos;
	/**
	 * @brief przód kamery
	 * 
	 */
	glm::vec3 cameraFront;
	/**
	 * @brief góra kamery
	 * 
	 */
	glm::vec3 cameraUp;
	/**
	 * @brief matryca model
	 * 
	 */
	glm::mat4 model; 
	/**
	 * @brief matryca view
	 * 
	 */
	glm::mat4 view; 
	/**
	 * @brief matryca projection
	 * 
	 */
	glm::mat4 projection;
	/**
	 * @brief Czy pierwszy raz odczytano stan myszy
	 * 
	 */
	bool firstMouse;
	/**
	 * @brief ostatnia pozycja x myszki
	 * 
	 */
	float lastX;
	/**
	 * @brief ostatnia pozycja y myszki
	 * 
	 */
	float lastY;
	/**
	 * @brief kąt pitch eulera
	 * 
	 */
	float pitch;
	/**
	 * @brief kąt yaw eulera
	 * 
	 */
	float yaw;
public:
/**
 * @brief Domyślny konstruktor klasy kamera
 * 
 */
	Camera();
	/**
	 * @brief Konstruktor z parametrami klasy kamera
	 * 
	 * @param screenWidth szerokość ekranu
	 * @param screenHeight wysokość ekranu
	 * @param programShader id shadera 1
	 * @param programShader2 id shadera 2
	 * @param window wskaźnik na okno
	 */
	Camera(float screenWidth, float screenHeight, unsigned int programShader, unsigned int programShader2, GLFWwindow* window)
	{
		cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
		cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
		cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
		projection = glm::perspective(glm::radians(45.0f), (float)screenWidth / (float)screenHeight, 0.1f, 100.0f);
		glUseProgram(programShader);
		glUniformMatrix4fv(glGetUniformLocation(programShader, "projection"), 1, GL_FALSE, &projection[0][0]);
		glUseProgram(programShader2);
		glUniformMatrix4fv(glGetUniformLocation(programShader2, "projection"), 1, GL_FALSE, &projection[0][0]);
		//glfwMakeContextCurrent(window);
		firstMouse = true;
		lastX = screenWidth / 2;
		lastY = screenHeight / 2;
		pitch = 0.0f;
		yaw = -90.0f;
	}
	/**
	 * @brief Aktualizuje pozycje kamery
	 * 
	 * @param programShader id shadera 1
	 * @param programShader2 id shadera 2
	 */
	void UpdateCamera(unsigned int programShader, unsigned int programShader2);
	/**
	 * @brief Funkcja ruchu wertykalnego
	 * 
	 * @param isUp czy kamera w górę
	 * @param cameraSpeed prędkość kamery
	 */
	void VerticalMove(bool isUp, float cameraSpeed);
	/**
	 * @brief Funkcja ruchu horyzontalneog
	 * 
	 * @param isRight czy w prawo
	 * @param cameraSpeed prędkość kamery
	 */
	void HorizontalMove(bool isRight, float cameraSpeed);
	/**
	 * @brief Aktualizacja pozycji myszy
	 * 
	 * @param xpos pozycja x kamery
	 * @param ypos pozycja y kamery
	 */
	void UpdateMouse(double xpos, double ypos);
	/**
	 * @brief pobranie matrycy view
	 * 
	 * @return matryca view
	 */
	glm::mat4 getView();
	/**
	 * @brief pobierz pozycję przodu kamery
	 * 
	 * @return wektor pozycji kamery
	 */
	glm::vec3 getCameraFront();
	/**
	 * @brief pobranie pozycji kamery
	 * 
	 * @return wktor pozycji kamery
	 */
	glm::vec3 getCameraPos();
	/**
	 * @brief ustawienie pozycji kamery
	 * 
	 * @param pos wektor pozycji kamery
	 */
	void setCameraPos(glm::vec3 pos);
};

#endif // !camera_h
