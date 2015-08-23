/* Copyright (c) 2011 Andrée Ekroth
 * See the file LICENSE.txt for copying permission.
 */

#include "Widgets/MainGLWidget.hpp"


#include "Graphics/GLManager.hpp"
#include "Graphics/IRenderer.hpp"

#include "Editor/Selectable.hpp"

#include "Level/Layer.hpp"
#include "Level/LayerItem.hpp"
#include "Level/LayerTexture.hpp"
#include "Level/Level.hpp"
#include "Level/LevelItem.hpp"
#include "Editor/Commands.hpp"

#include <vector>
#include <iostream>
#include <fstream>
#include <boost/archive/xml_iarchive.hpp>
#include <boost/archive/xml_oarchive.hpp>
#include <glm/glm.hpp>
#include <QtOpenGL/QGLWidget>
#include <QtGui/QMouseEvent>
#include <QtCore/QTime>

using namespace kke;

MainGLWidget::MainGLWidget(QWidget* parent) : QGLWidget(parent), renderer(0), totalElapsed(0.0)
{
}

MainGLWidget::~MainGLWidget()
{
	GLManager::Instance().Terminate();
	delete renderer;
// 	delete texTest;
	
	std::ofstream stream("/tmp/wat.xml");
	
	boost::archive::xml_oarchive oar(stream);
	oar << BOOST_SERIALIZATION_NVP(level);
	
	delete level;
	delete time;
	delete gameTime;
}

void MainGLWidget::initializeGL()
{
    if (glewInit() != GLEW_OK)
    {
		LOGE() << "Unable to initialize GLEW.\n";
    }

    glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	
	renderer = new Renderer();
// 	texTest = ResourceFactory::Instance().Create<kke::Texture2D>("content/lada.png");
	
	setMouseTracking(true);
    
    LOGM() << "MainGLWidget initialized.\n";
	
	time = new QTime(0, 0);
	gameTime = new GameTime();
	QTimer::singleShot(60, this, SLOT(update()));
	
	level = new Level("Name");
	Layer *layer = new Layer("Back", level);
	Layer *layer2 = new Layer("Front", level);
	
	layer->Items().push_back(new LayerTexture("Lada1", layer));	
	
	LayerTexture *tex = (LayerTexture*)layer->Items()[0];
	
// 	tex->SetSource("content/lada.png");
	tex->SetPosition(glm::vec3(100, 100, 0));
	
	level->Items().push_back(layer);
	level->Items().push_back(layer2);
	
	level->LoadContent();
}

void MainGLWidget::resizeGL(int width, int height)
{
	LOGM() << "MainGLWidget resized.\n";
	
	GLManager::Instance().SetWindowSize(width, height);
// 	GLManager::Instance().SetupPerspective();
}
void MainGLWidget::paintGL()
{
	// Update
	{
		level->Update(*gameTime);
	}
	
	// Draw
	{
		glClearColor(0.5f, 0.4f, 0.5f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		
		renderer->Begin();	
		level->Draw(*renderer);
		renderer->End();
	}
}

void MainGLWidget::mouseMoveEvent(QMouseEvent* e)
{
	// Redraw when mouse enters widget
	QWidget::mouseMoveEvent(e);
/*	Uncomment to draw frame when moving mouse, might result in better input?
	totalElapsed += time->elapsed() * 1000.0;
	gameTime->Update(totalElapsed);
	updateGL();
	time->restart();*/
}

void MainGLWidget::update()
{
	totalElapsed += time->elapsed() * 1000.0;
	gameTime->Update(totalElapsed);
	updateGL();
	
	QTimer::singleShot(16, this, SLOT(update()));
	time->restart();
}
