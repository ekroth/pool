/* Copyright (c) 2011 Andrée Ekroth
 * See the file LICENSE.txt for copying permission.
 */

#include <States/OrthoState.hpp>
#include <States/StateManager.hpp>
#include <Events/EventManager.hpp>
#include <Graphics/Graphics.hpp>
#include <Graphics/Shader.hpp>
#include <Graphics/GfxCache.hpp>
#include <Graphics/Buffer.hpp>
#include <Graphics/FrameBuffer.hpp>
#include <Graphics/Camera.hpp>
#include <Graphics/Font.hpp>
#include <Lua/LuaManager.hpp>
#include <Lua/LuaAddon.hpp>
#include <IO/LogManager.hpp>
#include <Input/InputManager.hpp>
#include <GameTime.hpp>
#include <Utilities/Factory.hpp>
#include <ResourceFactory.hpp>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <lua.hpp>
// #include <swiglua_Events.hpp>
#include "Buttfuck.hpp"

#include <sstream>
#include <string>
#include <memory>
#include <Cake.hpp>

#include "Graphics/Material.hpp"

using namespace kke;

using glm::vec2;
using glm::vec3;
using glm::vec4;

using std::string;

OrthoState::OrthoState(StateManager* stateManager) :
	State(stateManager)
{
}

void OrthoState::LoadContent()
{
	renderer = Resource<TextureBatch<>>(new TextureBatch<>());
// 	renderer = new TextureBatch<>("content/Shaders/HeatoHazo.vert|content/Shaders/HeatoHazo.frag");
	GfxCache::Instance().SetEnable(GL_BLEND, true);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	texture = ResourceFactory::Instance().Create<Texture2D>("content/lada.png");
    renderShader = ResourceFactory::Instance().Create<Shader>("content/Shaders/RenderTex.frag", "content/Shaders/RenderTex.vert");
    heatShader = ResourceFactory::Instance().Create<Shader>("content/Shaders/HeatoHazo.vert", "content/Shaders/HeatoHazo.frag");
    blurShader = ResourceFactory::Instance().Create<Shader>("content/Shaders/Bluro.vert", "content/Shaders/Bluro.frag");
	
// 	Factory<State>::Instance();
// 	Factory<State>::Instance().Register("hey", []() { return new State(nullptr); });
	
// 	Factory<State, StateManager*>::Instance().Register("OrthoState", [](StateManager* sm) { return new OrthoState(sm); });
// 	std::shared_ptr<State> state = Factory<State, StateManager*>::Instance().Create("OrthoState", stateManager);
    
// 	textureSmoke = ResourceFactory::Instance().Create<Texture2D>("content/tux.png");
// 	fbo = new FrameBuffer(true);
// 	font = ResourceFactory::Instance().Create<Font>("content/Fonts/Wuty.xml");
// 	heatHaze = ResourceFactory::Instance().Create<Shader>("content/Shaders/HeatHaze.frag", "content/Shaders/HeatHaze.vert");
// 	vbo = new BufferObject(true, GL_ARRAY_BUFFER);
// 	elementBo = new BufferObject(true, GL_ELEMENT_ARRAY_BUFFER);
	
// 	blurshader = ResourceFactory::Instance().Create<Shader>("content/Shaders/Blur.frag", "content/Shaders/Blur.vert");
	
// 	lightMap = new Texture2D(0, GL_RGBA, 
// 		GLManager::Instance().GetWindowWidth(), 
// 		GLManager::Instance().GetWindowHeight(), 
// 		0, GL_RGBA, GL_UNSIGNED_BYTE, 0);
// 	
// 	blurOutTex = new Texture2D(0, GL_RGBA, 
// 		GLManager::Instance().GetWindowWidth(), 
// 		GLManager::Instance().GetWindowHeight(), 
// 		0, GL_RGBA, GL_UNSIGNED_BYTE, 0);
// 	
// 	lightMapper = ResourceFactory::Instance().Create<Shader>("content/Shaders/LightMapper.frag", "content/Shaders/LightMapper.vert");
// 	lightMerger = ResourceFactory::Instance().Create<Shader>("content/Shaders/LightMerger.frag", "content/Shaders/LightMerger.vert");
// 	
// 	for (int i = 0; i < 2; i++)
// 	{
// 		fboTex[i] = new Texture2D(0, GL_RGBA, 
// 			GLManager::Instance().GetWindowWidth(), 
// 			GLManager::Instance().GetWindowHeight(), 
// 			0, GL_RGBA, GL_UNSIGNED_BYTE, 0);
// 		
// 		fboTex[i]->SetParameteri(GL_TEXTURE_MAG_FILTER, GL_LINEAR);
// 		fboTex[i]->SetParameteri(GL_TEXTURE_MAG_FILTER, GL_LINEAR);
// 		fboTex[i]->SetParameterf(GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
// 		fboTex[i]->SetParameterf(GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
// 	}
// 	
// 	
// 	
// 	fbo->Bind();
// 	fbo->AttachTexture(fboTex[0]);
	
// 	EventManager::Instance().AddListener(this, EventHasher("WindowClose"));
// 	EventManager::Instance().AddListener(this, EventHasher("fucker"));
// 	
// 	lua_State* L = LuaManager::Instance().GetState();
// 	
// 	// Test lua
// 	tolua_Buttfuck_open(L);
// 	tolua_Cake_open(L);
// 	tolua_EventData_open(L);
// 	tolua_FNV1aHash_open(L);
// 	tolua_LuaManager_open(L);
// 	tolua_EventManager_open(L);
// 	
// 	luaL_loadfile(L, "enginescripts/ScriptEnvironment.lua");
// 	lua_pcall(L, 0, 0, 0);
// 	
// 	LuaAddon addon;
// 	
// 	addon.SetName("Wat");
// 	addon.Load("enginescripts/AddonWat.lua");
	
	
	
// 	luaL_loadfile(L, "enginescripts/ScriptEnvironment.lua");
// 	lua_pcall(L, 0, 0, 0);
// 	luaL_loadfile(L, "glakkegamelua/CameraTest.lua");
// 	lua_getfield(L, LUA_GLOBALSINDEX, "addon_environment");
// 	lua_setfenv(L, -2);
// 	lua_pcall(L, 0, 0, 0);
// 	lua_getfield(L, LUA_GLOBALSINDEX, "blou");
// 	LuaManager::Instance().StackOutput();
// 	lua_pcall(L, 0, 0, 0);
// 	LuaManager::Instance().StackOutput();
// 	luaL_loadfile(L, "glakkegamelua/CameraTest.lua");
// 	lua_pcall(L, 0, 0, 0);
// 	lua_getfield(L, LUA_GLOBALSINDEX, "blou");
// 	lua_getfield(L, LUA_GLOBALSINDEX, "addon_environment");
// 	lua_setfenv(L, -2);
// 	lua_pcall(L, 0, 0, 0);
	
// 	lua_getglobal(L, "butt");
// 	Buttfuck *r = (Buttfuck*)tolua_touserdata(L, -1, 0);
// 	LOGGROUP(LogLevel::Message, "Lua") << "Butt: " << r->xy();
	
	
// // 	LOGGER(LogLevel::Warning) << "Popping..";
// // 	lua_pop(LuaVM::Instance().GetState(), 1);
// // 	LOGGER(LogLevel::Warning) << "Popped.";
// 	
// 	LOGGER(LogLevel::Warning) << "Pcall..";
// 	LuaVM::Instance().StackOutput();
// 	lua_rawgeti(LuaVM::Instance().GetState(), LUA_REGISTRYINDEX, ref);
// 	LuaVM::Instance().StackOutput();
// 	lua_pcall(LuaVM::Instance().GetState(), 0, 0, 0);
// 	LuaVM::Instance().StackOutput();
// 	lua_getfield(LuaVM::Instance().GetState(), LUA_GLOBALSINDEX, "blou");
// 	LuaVM::Instance().StackOutput();
// 	int ref2 = luaL_ref(LuaVM::Instance().GetState(), LUA_REGISTRYINDEX);
// 	LuaVM::Instance().StackOutput();
// 	lua_rawgeti(LuaVM::Instance().GetState(), LUA_REGISTRYINDEX, ref2);
// 	LuaVM::Instance().StackOutput();
// 	lua_pcall(LuaVM::Instance().GetState(), 0, 0, 0);
// 	LuaVM::Instance().StackOutput();
// 	
// // 	lua_call(LuaVM::Instance().GetState(), 0, 0);
// 	LOGGER(LogLevel::Warning) << "Pcalled.";
// 	 
// // 	LOGGER(LogLevel::Warning) << "Get global..";
// // 	lua_getglobal(LuaVM::Instance().GetState(), "butt");
// // 	LOGGER(LogLevel::Warning) << "Got global.";
// // 	Buttfuck *r = (Buttfuck*)tolua_touserdata(LuaVM::Instance().GetState(), -1, 0);
// 	
// 	
// // 	LOGGROUP(LogLevel::Message, "Lua") << "Butt: " << r->xy();
	
	
// 	lua_State* L = LuaManager::Instance().GetState();
// 	
// 	luaopen_All_Events(L);
// 	
// 	std::shared_ptr<Resource<LuaScript>> script(ResourceFactory::Instance().Create<LuaScript>("content/Lua/Testo.lua"));
// 	LuaManager::Instance().StackOutput();
// 	lua_rawgeti(L, LUA_REGISTRYINDEX, script->GetReference());
// 	lua_pcall(L, 0, LUA_MULTRET, 0);
// 	LuaManager::Instance().StackOutput();
}

void OrthoState::UnloadContent()
{ }

void OrthoState::HandleInput(InputManager& inputManager, GameTime& gameTime)
{	
	if (inputManager.KeyDown(GLFW_KEY_F1))
		ResourceFactory::Instance().ReloadCategory("Shader");
}

void OrthoState::Update(GameTime& gameTime)
{
}

void OrthoState::Draw(GameTime& gameTime)
{
	LOGD() << "DRAW\n";
	
	renderer->SetBlendDataIndex(0);
        renderer->SetShader(renderShader);
        renderer->Begin((RenderMode)TextureBatchMode::Blend);
            renderer->Draw(*texture, vec2(100, 100));
//         renderer->End();
	
		renderer->SetBlendDataIndex(1);
			renderer->Begin((RenderMode)TextureBatchMode::Blend);
				renderer->Draw(*texture, vec2(150, 200));	
	
	renderer->SetBlendDataIndex(0);
		renderer->SetShader(heatShader);
		renderer->PreEnd();
			renderer->GetShader()->Uniform("totalElapsed", (float)gameTime.TotalElapsed());
			renderer->GetShader()->Uniform("heatPosition", 200.0f, 200.0f);
			renderer->GetShader()->Uniform("heatDistance", 10.0f);
		renderer->End();
		
		renderer->SetBlendDataIndex(1);
			renderer->SetShader(blurShader);
			renderer->End();
}

/*
void OrthoState::Draw(GameTime& gameTime)
{	
// 	renderer->Begin();
// 	std::ostringstream ss;
// 	ss << x << ' ' << y;
// 	
// 	font->Draw(*renderer, ss.str(), vec2(x, y), 1024);
// 	renderer->End(false);
// 	return;
// 	if (gameTime.Frames() == 1)
// 	{
// 		Texture2D *texture = renderer->GetFrameBuffer().GetTexture();
// 		texture->Bind();
// 		texture->Allocate(0, GL_RGBA, 
// 		GLManager::Instance().GetWindowWidth(), GLManager::Instance().GetWindowHeight(), 
// 		0, GL_RGBA, GL_UNSIGNED_BYTE, 0);
// 	}


	
	renderer->SetClearColor(vec4(0, 0, 0, 0));
	renderer->Begin();
// 		
		renderer->Draw(
		textureSmoke->GetObject(), 
		vec2(460, 260), 
		rotation, 
		scale,
		Rectf(0.0, 0.0, 1.0, 1.0),
		vec4(1, 1, 1, 1),
		vec2(textureSmoke->GetWidth() / 2, textureSmoke->GetHeight() / 2 / 2),
		RenderEffect::RGBA, RenderFlip::Both);
		
		renderer->Draw(
		texture->GetObject(), 
		vec2(300, 300), 
		rotation, 
		vec2(1.8, 1.8),
		Rectf(0.0, 0.0, 1.0, 1.0),
		vec4(1, 1, 1, 0.6),
		vec2(texture->GetWidth() / 2, texture->GetHeight() / 2 / 2));
		
	renderer->End();
// 	if (gameTime.Frames() > 10)
// 	renderer->BlitFBO();
	
// 	if (gameTime.Frames() == 4)
// 	{
// 		Texture2D *texture = renderer->GetFrameBuffer().GetTexture();
// 		texture->Bind();
// 		texture->Allocate(0, GL_RGBA, 
// 		GLManager::Instance().GetWindowWidth(), GLManager::Instance().GetWindowHeight(), 
// 		0, GL_RGBA, GL_UNSIGNED_BYTE, 0);
// 	}
	
// 	test 
// // 	renderer->BlitFBO();
// // 	return;
// 	
// // 		renderer->ForceDraw();
// // 	
// // 		font->Draw(*renderer, L"Andrée\nI MEAN WTF IS THIS\nSHITTYFUCK\nIN THE FUCK Waaat?!<<", vec2(0, 500), 1024);
// 	
// // 	Camera::Instance().PushView();	
// // 	DrawHeat(gameTime);
// // 	Camera::Instance().PopView();
	DrawLight(gameTime);
// // 	
// 	renderer->BlitFBO();
// // 	
// // 	
	
	renderer->Begin();
	renderer->Draw(
		*fboTex[1],
		vec2(0, 0));
	Camera::Instance().PushView();
	renderer->End(false);
	Camera::Instance().PopView();
}

void OrthoState::DrawLight (GameTime& gameTime)
{
	// Pass 1, draw lightmap
	fbo->Bind();
	fbo->AttachTexture(fboTex[0]);
	GLManager::Instance().SetClearColor(0, 0, 0, 1);
	GLManager::Instance().Clear(GL_COLOR_BUFFER_BIT);
	lightMapper->Bind();
	lightMapper->Uniform("light.Intensity", 1.0f);
	lightMapper->Uniform("light.Type", 1.0f);
	lightMapper->Uniform("light.Direction", 900.0f, 100.0f);
	lightMapper->Uniform("light.Position", x, y);
	lightMapper->UniformMat("matrix", 1, false, Camera::Instance().GetFullPtr());
	lightMapper->SetVertexAttrib("in_Vertex", true);
	
		std::vector<PrimitiveVertex> vertices;
		std::vector<unsigned int> elements;
		renderer->Transform<PrimitiveVertex>(elements, vertices, *renderer->GetFrameBuffer().GetTexture(), vec2(0, 0));
		vbo->Bind();
		if (vbo->GetSize() >= sizeof(PrimitiveVertex) * vertices.size())
			vbo->BufferSubData(0, sizeof(PrimitiveVertex) * vertices.size(), &vertices[0]);
		else
			vbo->BufferData(sizeof(PrimitiveVertex) * vertices.size(), &vertices[0], GL_DYNAMIC_DRAW);
		
		elementBo->Bind();
		if (elementBo->GetSize() >= sizeof(int) * elements.size())
			elementBo->BufferSubData(0, sizeof(int) * elements.size(), &elements[0]);
		else
			elementBo->BufferData(sizeof(int) * elements.size(), &elements[0], GL_DYNAMIC_DRAW);
	
	lightMapper->VertexAttribPointer("in_Vertex", 2, GL_FLOAT, false, sizeof(PrimitiveVertex), (const GLvoid*)0);
	
	glDrawElements(
		GL_TRIANGLES,
		elements.size(),
		GL_UNSIGNED_INT,
		0
	);
	
	lightMapper->SetVertexAttrib("in_Vertex", false);
	
	// Pass 2 combine lightmap with texture
	fbo->DetachTexture();
	fbo->AttachTexture(fboTex[1]);
	GLManager::Instance().SetClearColor(1, 1, 1, 1);
	GLManager::Instance().Clear(GL_COLOR_BUFFER_BIT);
	Camera::Instance().PushView();
	
	lightMerger->Bind();
	lightMerger->UniformMat("matrix", 1, false, Camera::Instance().GetFullPtr());
	lightMerger->SetVertexAttrib("in_Vertex", true);
	lightMerger->SetVertexAttrib("in_Texcoord", true);
	lightMerger->Uniform("lightMap", 0);
	lightMerger->Uniform("tex", 1);
	
	Texture2D::Activate(GL_TEXTURE0);
	fboTex[0]->Bind();
	Texture2D::Activate(GL_TEXTURE1);
	Texture2D *rendtex = renderer->GetFrameBuffer().GetTexture();
	rendtex->Bind();
	
	
		vertices.clear();
		elements.clear();
		renderer->Transform<PrimitiveVertex>(elements, vertices, *rendtex, vec2(0, 0));
		vbo->Bind();
		if (vbo->GetSize() >= sizeof(PrimitiveVertex) * vertices.size())
			vbo->BufferSubData(0, sizeof(PrimitiveVertex) * vertices.size(), &vertices[0]);
		else
			vbo->BufferData(sizeof(PrimitiveVertex) * vertices.size(), &vertices[0], GL_DYNAMIC_DRAW);
		
		elementBo->Bind();
		if (elementBo->GetSize() >= sizeof(int) * elements.size())
			elementBo->BufferSubData(0, sizeof(int) * elements.size(), &elements[0]);
		else
			elementBo->BufferData(sizeof(int) * elements.size(), &elements[0], GL_DYNAMIC_DRAW);
	
	
	size_t offset = 0;
	blurshader->VertexAttribPointer("in_Vertex", 2, GL_FLOAT, false, sizeof(PrimitiveVertex), (const GLvoid*)offset);
	offset += sizeof(vec2);
	blurshader->VertexAttribPointer("in_Texcoord", 2, GL_FLOAT, false, sizeof(PrimitiveVertex), (const GLvoid*)offset);
	
	glDrawElements(
		GL_TRIANGLES,
		elements.size(),
		GL_UNSIGNED_INT,
		0
	);
	
	
	Texture2D::Activate(GL_TEXTURE0);
	lightMerger->SetVertexAttrib("in_Vertex", false);
	lightMerger->SetVertexAttrib("in_Texcoord", false);
	
	Camera::Instance().PopView();
}

void OrthoState::DrawHeat (GameTime& gameTime)
{
// 	renderer->ForceDraw();
// 	renderer->GetFrameBuffer().GetTexture()->Bind();
// 	fbo->Bind();
// 	GLManager::Instance().Clear(GL_COLOR_BUFFER_BIT);
// 	
// 	heatHaze->Bind();
// 	
// 	heatHaze->SetVertexAttrib("in_Vertex", true);
// 	heatHaze->SetVertexAttrib("in_Texcoord", true);
// 	
// 	heatHaze->UniformMat("matrix", 1, false, Camera::Instance().GetFullPtr());
// 	heatHaze->Uniform("tex", 0);
// 	heatHaze->Uniform("heatPosition", x, y);
// 	heatHaze->Uniform("heatDistance", 100.0f);
// 	heatHaze->Uniform("totalElapsed", (float)gameTime.TotalElapsed());
	
// 	std::vector<PrimitiveVertex> vertices;
// 	std::vector<unsigned int> elements;
// 	renderer->Transform<PrimitiveVertex>(elements, vertices, *renderer->GetFrameBuffer().GetTexture(), vec2(0, 0));
// 	vbo->Bind();
// 	if (vbo->GetSize() >= sizeof(PrimitiveVertex) * vertices.size())
// 		vbo->BufferSubData(0, sizeof(PrimitiveVertex) * vertices.size(), &vertices[0]);
// 	else
// 		vbo->BufferData(sizeof(PrimitiveVertex) * vertices.size(), &vertices[0], GL_DYNAMIC_DRAW);
// 	
// 	elementBo->Bind();
// 	if (elementBo->GetSize() >= sizeof(int) * elements.size())
// 		elementBo->BufferSubData(0, sizeof(int) * elements.size(), &elements[0]);
// 	else
// 		elementBo->BufferData(sizeof(int) * elements.size(), &elements[0], GL_DYNAMIC_DRAW);
	
// 	size_t offset = 0;
// 	heatHaze->VertexAttribPointer("in_Vertex", 2, GL_FLOAT, false, sizeof(PrimitiveVertex), (const GLvoid*)offset);
// 	offset += sizeof(vec2);
// 	heatHaze->VertexAttribPointer("in_Texcoord", 2, GL_FLOAT, false, sizeof(PrimitiveVertex), (const GLvoid*)offset);
// 	
// 	glDrawElements(
// 		GL_TRIANGLES,
// 		elements.size(),
// 		GL_UNSIGNED_INT,
// 		0
// 	);
// 	
// 	heatHaze->SetVertexAttrib("in_Vertex", false);
// 	heatHaze->SetVertexAttrib("in_Texcoord", false);
// 	
// 	fbo->Disable();
// 	
// 	renderer->Draw(
// 		*fbo->GetTexture(), 
// 		vec2(0, 0), 
// 		0, 
// 		vec2(1, 1),
// 		Rectf(0.0, 0.0, 1.0, 1.0),
// 		vec4(1, 1, 1, 1));
	
	// Second pass blur
	renderer->ForceDraw();
	renderer->GetFrameBuffer().GetTexture()->Bind();
	fbo->Bind();
	GLManager::Instance().Clear(GL_COLOR_BUFFER_BIT);
	blurshader->Bind();
	
	blurshader->Uniform("tex", 0);
	blurshader->UniformMat("matrix", 1, false, Camera::Instance().GetFullPtr());
	
	blurshader->SetVertexAttrib("in_Vertex", true);
	blurshader->SetVertexAttrib("in_Texcoord", true);
	
	std::vector<PrimitiveVertex> vertices;
	std::vector<unsigned int> elements;
	renderer->Transform<PrimitiveVertex>(elements, vertices, *renderer->GetFrameBuffer().GetTexture(), vec2(0, 0));
	vbo->Bind();
	if (vbo->GetSize() >= sizeof(PrimitiveVertex) * vertices.size())
		vbo->BufferSubData(0, sizeof(PrimitiveVertex) * vertices.size(), &vertices[0]);
	else
		vbo->BufferData(sizeof(PrimitiveVertex) * vertices.size(), &vertices[0], GL_DYNAMIC_DRAW);
	
	elementBo->Bind();
	if (elementBo->GetSize() >= sizeof(int) * elements.size())
		elementBo->BufferSubData(0, sizeof(int) * elements.size(), &elements[0]);
	else
		elementBo->BufferData(sizeof(int) * elements.size(), &elements[0], GL_DYNAMIC_DRAW);
	
	size_t offset = 0;
	blurshader->VertexAttribPointer("in_Vertex", 2, GL_FLOAT, false, sizeof(PrimitiveVertex), (const GLvoid*)offset);
	offset += sizeof(vec2);
	blurshader->VertexAttribPointer("in_Texcoord", 2, GL_FLOAT, false, sizeof(PrimitiveVertex), (const GLvoid*)offset);
	
	glDrawElements(
		GL_TRIANGLES,
		elements.size(),
		GL_UNSIGNED_INT,
		0
	);
	
	blurshader->SetVertexAttrib("in_Vertex", false);
	blurshader->SetVertexAttrib("in_Texcoord", false);
	
	fbo->Disable();
	
// 	renderer->GetFrameBuffer().Bind();
// 	GLManager::Instance().Clear(GL_COLOR_BUFFER_BIT);
	
	renderer->Draw(
		*fbo->GetTexture(), 
		vec2(0, 0), 
		0, 
		vec2(1, 1),
		Rectf(0.0, 0.0, 1.0, 1.0),
		vec4(1, 1, 1, 1));
}

void OrthoState::DrawBlur (GameTime& gameTime)
{
	fbo->Bind();
	fbo->AttachTexture(blurOutTex);
	GLManager::Instance().SetClearColor(1, 1, 1, 1);
	GLManager::Instance().Clear(GL_COLOR_BUFFER_BIT);
	Camera::Instance().PushView();
	
	Texture2D::Activate(GL_TEXTURE0);
	fboTex[1]->Bind();
	
	blurshader->Bind();
	blurshader->UniformMat("matrix", 1, false, Camera::Instance().GetFullPtr());
	blurshader->SetVertexAttrib("in_Vertex", true);
	blurshader->SetVertexAttrib("in_Texcoord", true);
	
		std::vector<PrimitiveVertex> vertices;
		std::vector<unsigned int> elements;
		renderer->Transform<PrimitiveVertex>(elements, vertices, fboTex[1], vec2(0, 0));
		vbo->Bind();
		if (vbo->GetSize() >= sizeof(PrimitiveVertex) * vertices.size())
			vbo->BufferSubData(0, sizeof(PrimitiveVertex) * vertices.size(), &vertices[0]);
		else
			vbo->BufferData(sizeof(PrimitiveVertex) * vertices.size(), &vertices[0], GL_DYNAMIC_DRAW);
		
		elementBo->Bind();
		if (elementBo->GetSize() >= sizeof(int) * elements.size())
			elementBo->BufferSubData(0, sizeof(int) * elements.size(), &elements[0]);
		else
			elementBo->BufferData(sizeof(int) * elements.size(), &elements[0], GL_DYNAMIC_DRAW);
	
	lightMapper->VertexAttribPointer("in_Vertex", 2, GL_FLOAT, false, sizeof(PrimitiveVertex), (const GLvoid*)0);
	
	glDrawElements(
		GL_TRIANGLES,
		elements.size(),
		GL_UNSIGNED_INT,
		0
	);
	
	blurshader->SetVertexAttrib("in_Vertex", false);
	blurshader->SetVertexAttrib("in_Texcoord", false);
	
	Camera::Instance().PopView();
}
*/
void OrthoState::ProcessEvent (EventData& event)
{
	// 	OrthoState *state = (OrthoState*)user;
	// 	
	// 	FrameBuffer *fbo = state->fbo;
	// 	fbo->Bind();
	// 	Texture2D* fboTex = state->fbo->GetTexture();
	// 	
	// 	fboTex->Bind();
	// 	fboTex->Allocate(0, GL_RGBA, 
	// 		GLManager::Instance().GetWindowWidth(), 
	// 		GLManager::Instance().GetWindowHeight(), 
	// 		0, GL_RGBA, GL_UNSIGNED_BYTE, 0);
	// 	
	// 	fboTex->SetParameteri(GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// 	fboTex->SetParameteri(GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// 	fboTex->SetParameterf(GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	// 	fboTex->SetParameterf(GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	// 	
	// 	fbo->GetStatus();
	// 	fbo->Disable();
	
	if (event.GetType() == EventHasher("fucker"))
	{
	    LOGM() << "Event received, time: " << event.GetTime();
	}
}