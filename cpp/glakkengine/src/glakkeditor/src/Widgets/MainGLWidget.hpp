/* Copyright (c) 2011 Andrée Ekroth
 * See the file LICENSE.txt for copying permission.
 */

#ifndef KKE_MAINGLWIDGET_HPP
#define KKE_MAINGLWIDGET_HPP

#include "Graphics/Texture2D.hpp"
#include "Resource.hpp"

#include <GL/glew.h>
#include <QtOpenGL/QGLWidget>
#include <QtCore/QTime>
#include <QtCore/QTimer>

namespace kke
{
	class Renderer;
	class Level;
	class GameTime;

	class MainGLWidget : public QGLWidget
	{
		Q_OBJECT

	public:
		MainGLWidget(QWidget* parent = nullptr);
		virtual ~MainGLWidget();

	protected:
		void initializeGL();
		void resizeGL(int width, int height);
		void paintGL();

	private:
		void mouseMoveEvent(QMouseEvent* e);

		Renderer *renderer;
		Level *level;
		QTime *time;
		GameTime *gameTime;
		double totalElapsed;
		
	public slots:
		void update();
	};
}

#endif // KKE_MAINGLWIDGET_HPP
