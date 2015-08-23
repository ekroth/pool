/* Copyright (c) 2010-2011 Andrée Ekroth
 * See the file LICENSE.txt for copying permission.
 */

#include "GameApp.hpp"
#include "Graphics/Shader.hpp"
#include "Graphics/FTManager.hpp"
#include "Graphics/Camera.hpp"
#include "Graphics/FrameBuffer.hpp"
#include "Graphics/GfxManager.hpp"
#include "Input/InputManager.hpp"
#include "IO/IniConfig.hpp"
#include "IO/ArgParser.hpp"
#include "IO/Argument.hpp"
#include "IO/LogManager.hpp"

#include "Events/EventManager.hpp"
#include "States/StateManager.hpp"
#include "Sound/SoundManager.hpp"
#include "Utilities/Factory.hpp"
#include "ResourceFactory.hpp"
#include "GameTime.hpp"

#include "Factory_Register_All.hpp"

#include <sstream>
#include <GL/glfw.h>
#include <cstdio>

using namespace kke;

using std::string;
using std::shared_ptr;

GameApp::GameApp() :
    stateManager(nullptr),
    gameTime(nullptr),
    inputManager(nullptr),
	appRunning (false)
{ }

GameApp::~GameApp()
{ }

int GameApp::Execute(int argc, char** argv)
{
//     Config* sysConfig = ConfigManager::Instance().AddConfig("System");
//     sysConfig->AddDoc("content/Defaults.xml", true);
	

    LOGM() << "GlakkeGame - Working Title\n" << "Andrée Ekroth - 2010-2011";
#ifdef VERSION
    LOGM() << "Version: " << VERSION;
#endif
	
	if (!bRegisterArgs())
	{
		LOGE() << "Error occured when registering arguments.";
		return 1;
	}
	
	if (!ArgParser::Instance().Process(argc, argv))
	{
		LOGE() << "Error occured when parsing arguments.";
        return 1;
	}
	
	if (!bHandleArgs())
	{
		LOGE() << "Error occured when handling arguments.";
        return 1;
	}

    // Initialize
    if(!bInit())
    {
        LOGE() << "Error occured when initializing.";
        return 1;
    }
    
    gameTime->SetCalcFps(true);

    while (appRunning)
    {
        gameTime->Update(GLManager::Instance().GetTime());
        GfxManager::Instance().Update();
	EventManager::Instance().Update(*gameTime);
        
        if (!appRunning)
            break;

        //Run update loop
//         update();
	draw();
    }
    
    LOGD() << "App done!";
    
    bClean();
	

    // Finished successfully
    return 0;
}

void GameApp::ProcessEvent (EventData& event)
{
    // For now application stops as soon as soon as user wants to
	switch (event.GetType())
    {
        case EventHasher("WindowCloseRequest"):
            EventManager::Instance().QueueEvent(new GfxOutPassEventData(new EventData(EventHasher("CloseWindow"), gameTime->GetTime()), gameTime->GetTime()));
            LOGD() << "WindowCloseReq!";
            break;
            
        case EventHasher("WindowClosed"):
            LOGD() << "WindowClosed!";
            appRunning = false;
            break;
    }
}

bool GameApp::bRegisterArgs()
{
	bool result = true;
	result = result && ArgParser::Instance().Register(ArgHasher("Fullscreen"), ArgumentType::Exist, "fullscreen", "fs", "Fullscreen");
	result = result && ArgParser::Instance().Register(ArgHasher("Fps"), ArgumentType::Int, "fps", "fps", "MaxFPS");
	result = result && ArgParser::Instance().Register(ArgHasher("BgFps"), ArgumentType::Int, "bgfps", "bgfps", "BackgroundFPS");
	result = result && ArgParser::Instance().Register(ArgHasher("Showfps"), ArgumentType::Exist, "showfps", "sfps", "ShowFPS");
	result = result && ArgParser::Instance().Register(ArgHasher("Vsync"), ArgumentType::Exist, "vsync", "vsync", "Vsync");
	result = result && ArgParser::Instance().Register(ArgHasher("Width"), ArgumentType::Int, "width", "w", "Width");
	result = result && ArgParser::Instance().Register(ArgHasher("Height"), ArgumentType::Int, "height", "h", "Height");
	result = result && ArgParser::Instance().Register(ArgHasher("Loglevel"), ArgumentType::Int, "loglevel", "lg", "LogLevel");
	
	// virtual
	result = result && registerArgs();
	
	return result;
}

bool GameApp::bHandleArgs()
{
//     TiXmlDocument* doc = new TiXmlDocument("Arguments");
//     doc->LinkEndChild(new TiXmlElement("Config"));
// 	
// 	Logger::SetColorOutput(false);
// 	Logger::SetLogLevel(LogLevel::Warning);
// 
// 	if (ArgParser::Instance().Exist(Arg_Fps))
// 	{
// 		TiXmlElement* ele = new TiXmlElement(ArgParser::Instance().GetInfo(Arg_Fps));
// 		ele->LinkEndChild(new TiXmlText(ArgParser::Instance().GetString(Arg_Fps)));
// 		((TiXmlElement*)doc->RootElement())->LinkEndChild(ele);
// 	}
// 	if (ArgParser::Instance().Exist(Arg_BgFps))
// 	{
// 		TiXmlElement* ele = new TiXmlElement(ArgParser::Instance().GetInfo(Arg_BgFps));
// 		ele->LinkEndChild(new TiXmlText(ArgParser::Instance().GetString(Arg_BgFps)));
// 		((TiXmlElement*)doc->RootElement())->LinkEndChild(ele);
// 	}
// 	if (ArgParser::Instance().Exist(Arg_Showfps))
// 	{
// 		TiXmlElement* ele = new TiXmlElement(ArgParser::Instance().GetInfo(Arg_Showfps));
// 		ele->LinkEndChild(new TiXmlText("true"));
// 		((TiXmlElement*)doc->RootElement())->LinkEndChild(ele);
// 	}
// 	if (ArgParser::Instance().Exist(Arg_Vsync))
// 	{
// 		TiXmlElement* ele = new TiXmlElement(ArgParser::Instance().GetInfo(Arg_Vsync));
// 		ele->LinkEndChild(new TiXmlText("true"));
// 		((TiXmlElement*)doc->RootElement())->LinkEndChild(ele);
// 	}
// 	if (ArgParser::Instance().Exist(Arg_Width))
// 	{
// 		TiXmlElement* ele = new TiXmlElement(ArgParser::Instance().GetInfo(Arg_Width));
// 		ele->LinkEndChild(new TiXmlText(ArgParser::Instance().GetString(Arg_Width)));
// 		((TiXmlElement*)doc->RootElement())->LinkEndChild(ele);
// 	}
// 	if (ArgParser::Instance().Exist(Arg_Height))
// 	{
// 		TiXmlElement* ele = new TiXmlElement(ArgParser::Instance().GetInfo(Arg_Height));
// 		ele->LinkEndChild(new TiXmlText(ArgParser::Instance().GetString(Arg_Height)));
// 		((TiXmlElement*)doc->RootElement())->LinkEndChild(ele);
// 	}
// 	if (ArgParser::Instance().Exist(Arg_Fullscreen))
// 	{
// 		TiXmlElement* ele = new TiXmlElement(ArgParser::Instance().GetInfo(Arg_Fullscreen));
// 		ele->LinkEndChild(new TiXmlText("true"));
// 		((TiXmlElement*)doc->RootElement())->LinkEndChild(ele);
// 	}
// 	if (ArgParser::Instance().Exist(Arg_Loglevel))
// 	{
// 		TiXmlElement* ele = new TiXmlElement(ArgParser::Instance().GetInfo(Arg_Loglevel));
// 		ele->LinkEndChild(new TiXmlText(ArgParser::Instance().GetString(Arg_Loglevel)));
// 		((TiXmlElement*)doc->RootElement())->LinkEndChild(ele);
// 	}
// 
//     // Add document to highest priority of System config
//     ConfigManager::Instance().GetConfig("System")->AddDoc(doc);

// 	Config *conf = ConfigManager::Instance().AddConfig("System");

    return true;
}

bool GameApp::bInit()
{
    Resource<IniConfig> conf = ResourceFactory::Instance().Create<IniConfig>("content/Config.ini");
    const IniData& config = conf->GetIni();
	LogManager::Instance().SetMinLevel((LogLevel)config.GetLongValue("Debug", "LogLevel", 1));

	int mode = config.GetBoolValue("Video", "Fullscreen", false) ? GLFW_FULLSCREEN : GLFW_WINDOW;
    
    winCloseReqListen = ScopedEMListener(this, &GameApp::ProcessEvent, EventHasher("WindowCloseRequest"));
    winClosedListen = ScopedEMListener(this, &GameApp::ProcessEvent, EventHasher("WindowClosed"));
    appRunning = true;
	Camera::Instance().SetupViewport();
	// Set settings..
	GfxManager::Instance().Run();

	gameTime = shared_ptr<GameTime>(new GameTime());
// 	stateManager = shared_ptr<StateManager>(new StateManager());
// 	inputManager = shared_ptr<InputManager>(new InputManager());
	
// 	Factory_Register_All();
	
//     return init();
    return true;
}

void GameApp::bClean()
{
	clean();
	
	stateManager.reset();
	inputManager.reset();

    Camera::Terminate();
    SoundManager::Terminate();
    FTManager::Terminate();
    ResourceFactory::Terminate();
    ArgParser::Terminate();
    LogManager::Terminate();
    GfxManager::Terminate();
}

bool GameApp::registerArgs()
{
	return true;
}

bool GameApp::init()
{
	return true;
}

void GameApp::update()
{
    //SoundManager::Update(elapsed);
    inputManager->HandleInput(gameTime->Elapsed());
    stateManager->HandleInput(*inputManager, *gameTime);
    stateManager->Update(*gameTime);
}

void GameApp::draw()
{
    glClear(GL_DEPTH_BUFFER | GL_COLOR_BUFFER_BIT);
    stateManager->Draw(*gameTime);
}

bool GameApp::clean()
{
	return true;
}