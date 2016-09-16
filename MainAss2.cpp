

#include "stdafx.h"
#include "MainAss2.h"
//#include <OgreConfigFile.h>
#include <OgreException.h>
//#include <OgreCamera.h>
//#include <OgreViewport.h>
//#include <OgreSceneManager.h>
//#include <OgreRenderWindow.h>
//#include <OgreEntity.h>
#include <OgreWindowEventUtilities.h>
//----------------------------------------------------------------------------------

Ass2::Ass2(void) 
	: mRoot(0),
	mCamera(0),
	mSceneMgr(0),
	mWindow(0),
	mResourcesCfg(Ogre::StringUtil::BLANK), 
	mPluginsCfg(Ogre::StringUtil::BLANK),
	mTrayMgr(0),
	mCameraMan(0),
	mDetailsPanel(0),
	mCursorWasVisible(false),
	mShutDown(false),
	mInputManager(0),
	mMouse(0),
	mKeyboard(0),
	mOverlaySystem(0)
{
#if OGRE_PLATFORM == OGRE_PLATFORM_APPLE
	m_ResourcePath = Ogre::macBundlePath() + "/Contents/Resources/";
#else
	m_ResourcePath = "";
#endif
}

Ass2::~Ass2(void)
{
	//Remove ourself as a Window listener
	Ogre::WindowEventUtilities::removeWindowEventListener(mWindow, this);
	windowClosed(mWindow);
	if (mTrayMgr) delete mTrayMgr;
	if (mCameraMan) delete mCameraMan;
	if (mOverlaySystem) delete mOverlaySystem;

	//Remove ourself as a Window Listener
	Ogre::WindowEventUtilities::removeWindowEventListener(mWindow, this);
	windowClosed(mWindow);
	delete mRoot;
}

bool Ass2::go(void)
{
#ifdef _DEBUG
	mResourcesCfg = "resources_d.cfg";
	mPluginsCfg = "plugins_d.cfg";
#else
	mResourcesCfg = "resources_d.cfg";
	mPluginsCfg = "plugins.cfg";
#endif
	// construct Ogre::Root
	mRoot = new Ogre::Root(mPluginsCfg);
	

	// configure
	// Show the configuration dialog and initialise the system
	if(!(mRoot->restoreConfig() || mRoot->showConfigDialog()))
	{
		return false;
	}
	mWindow = mRoot->initialise(true, "BasicTutorial6 Render Window");

	// Set default mipmap level (note: some APIs ignore this)
	Ogre::TextureManager::getSingleton().setDefaultNumMipmaps(5);
	// initialise all resource groups
	Ogre::ResourceGroupManager::getSingleton().initialiseAllResourceGroups();
	
	// Create the SceneManager, in this case a generic one
	mSceneMgr = mRoot->createSceneManager("DefaultSceneManager");

	// Create the camera
	mCamera = mSceneMgr->createCamera("PlayerCam");
	// Position it at 80 in Z direction
	mCamera->setPosition(Ogre::Vector3(0,0,80));
	// Look back along -Z
	mCamera->lookAt(Ogre::Vector3(0,0,-300));
	mCamera->setNearClipDistance(5);

	// Create one viewport, entire window
	Ogre::Viewport* vp = mWindow->addViewport(mCamera);
	vp->setBackgroundColour(Ogre::ColourValue(0,0,0));
	// Alter the camera aspect ratio to match the viewport
	mCamera->setAspectRatio(Ogre::Real(vp->getActualWidth()) / Ogre::Real(vp->getActualHeight()));

	// Set ambient light
	mSceneMgr->setAmbientLight(Ogre::ColourValue(0.5, 0.5, 0.5));
	// Create a light
	Ogre::Light* l = mSceneMgr->createLight("MainLight");
	l->setPosition(20,80,50);

	Ogre::LogManager::getSingletonPtr()->logMessage("*** Initializing OIS***");
	OIS::ParamList pl;
	size_t windowHnd = 0;
	std::ostringstream windowHndStr;
	mWindow->getCustomAttribute("WINDOW", &windowHnd);
	windowHndStr << windowHnd;
	pl.insert(std::make_pair(std::string("WINDOW"), windowHndStr.str()));
	mInputManager = OIS::InputManager::createInputSystem( pl );

	mKeyboard = static_cast<OIS::Keyboard*>(mInputManager->createInputObject( OIS::OISKeyboard, false ));
	mMouse = static_cast<OIS::Mouse*>(mInputManager->createInputObject( OIS::OISMouse, false ));

	//Set initial mouse clipping size
	windowResized(mWindow);
	//Register as a Window listener
	Ogre::WindowEventUtilities::addWindowEventListener(mWindow, this);

	mRoot->addFrameListener(this);
	mRoot->startRendering();

	return true;
}




//Adjust mouse clipping area
void Ass2::windowResized(Ogre::RenderWindow* rw)
{
	unsigned int width, height, depth;
	int left, top;
	rw->getMetrics(width, height, depth, left, top);
	const OIS::MouseState &ms = mMouse->getMouseState();
	ms.width = width;
	ms.height = height;
}
//Unattach OIS before window shutdown (very important under Linux)
void Ass2::windowClosed(Ogre::RenderWindow* rw)
{
	//Only close for window that created OIS (the main window in these demos)
	if (rw == mWindow)
	{
		if(mInputManager)
		{
			mInputManager->destroyInputObject( mMouse );
			mInputManager->destroyInputObject( mKeyboard );
			OIS::InputManager::destroyInputSystem(mInputManager);
			mInputManager = 0;
		}
	}
}

bool Ass2::frameRenderingQueued(const Ogre::FrameEvent& evt)
{
	if(mWindow->isClosed())
		return false;
	//Need to capture/update each device
	mKeyboard->capture();
	mMouse->capture();
	if(mKeyboard->isKeyDown(OIS::KC_ESCAPE))
		return false;
	return true;
}

bool Ass2::configure(void)
{
	//Show the con
}




void Ass2::chooseSceneManager()
{

}
void Ass2::createCamera()
{

}
void Ass2::createFrameListener(void)
{

}
void Ass2::createScene(void)
{
	//ground
	btBoxShape* groundShape = new btBoxShape(btVector3(50.0f, 5.0f, 50.0f));

	btTransform groundTransform;
	groundTransform.setIdentity();
	groundTransform.setOrigin(btVector3(0, -5, 0));

	Ogre::Entity* cube = mSceneMgr->createEntity("Ground", "cub.mesh");
	Ogre::SceneNode* node = mSceneMgr->getRootSceneNode()->createChildSceneNode();
	node->attachObject(cube);
	node->scale(1, 0.1, 1);
	node->translate(0, -5, 0);

	mPhysicsEngine->createRigidBody(0.0f, groundTransform, groundShape, node);

	node->setVisible(false);

	//Use a place to represent the ground
	Ogre::Plane plane(Ogre::Vector3::UNIT_Y, 0);
	Ogre::MeshManager::getSingleton().createPlane("ground", Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME, plane, 100, 100, 20, 20, true, 1, 5, 5, OGre::Vector3::UNIT_Z);

	Ogre::Entity* entGround = mSceneMgr->createEntity("GroundEntity", "ground");
	mSceneMgr->getRootSceneNode()->createChildSceneNode()->attachObject(entGround);
	entGround->setMaterialName("Examples/Rockwall");
	entGround->setCastShadows(false);
}
void Ass2::destroyScene(void)
{

}
void Ass2::createViewports(void)
{

}
void Ass2::setupResources(void)
{
	// set up resources
	// Load resource paths from config file
	Ogre::ConfigFile cf;
	cf.load(mResourcesCfg);
	
	// Go through all sections & settings in the file
	Ogre::ConfigFile::SectionIterator seci = cf.getSectionIterator();
	Ogre::String secName, typeName, archName;
	while (seci.hasMoreElements())
	{
		secName = seci.peekNextKey();
		Ogre::ConfigFile::SettingsMultiMap *settings = seci.getNext();
		Ogre::ConfigFile::SettingsMultiMap::iterator i;
		for (i = settings->begin(); i != settings->end(); ++i)
		{
			typeName = i->first;
			archName = i->second;
			Ogre::ResourceGroupManager::getSingleton().addResourceLocation(archName, typeName, secName);
		}
	}
}
void Ass2::createResourceListener(void)
{

}
void Ass2::loadResources(void)
{

}
bool Ass2::frameRenderingQueued(const Ogre::FrameEvent& evt)
{

}

bool Ass2::keyPressed( const OIS::KeyEvent &arg)
{
	if (mTrayMgr->isDialogVisible()) return true; //dont process any more keys if dialog is up

	if (arg.key == OIS::KC_F) //toggle visibility of advanced frame stats
	{
		mTrayMgr->toggleAdvancedFrameStats();
	}
	else if (arg.key == OIS::KC_G) // toggle visibility of even rarer debugging details
	{
		if (mDetailsPanel->gtTrayLocation() == OgreBites::TL_NONE)
		{
			mTrayMgr->moveWidgetToTray(mDetailsPanel, OgreBites::TL_TOPRIGHT, 0);
			mDetailsPanel->show();
		}
		else
		{
			mTrayMgr->removeWidgetFromTray(mDetailsPanel);
			mDetailsPanel->hide();
		}
	}
	else if (arg.key == OIS::KC_T) //cycle polygone rendering mode
	{
		Ogre::String newVal;
		Ogre::TextureFilterOptions tfo;
		unsigned int aniso;

		switch (mDetailsPanel->getParamValue(9).asUTF8()[0])
		{
		case 'B':
			newVal = "Trilinear";
			tfo = Ogre::TFO_TRILINEAR;
			aniso = 1;
			break;
		case 'T':
			newVal = "Anisotropic";
			tfo = Ogre::TFO_ANISOTROPIC;
			aniso = 8;
			break;
		case 'A':
			newVal = "None";
			tfo = Ogre::TFO_NONE;
			aniso = 1;
			break;
		default:
			newVal = "Bilinear";
			tfo = Ogre::TFO_BILINEAR;
			aniso = 1;
			break;
	}
//other crap too
	else if (arg.key == OIS::KC_F5) //refresh all textures
	{
		Ogre::TextureManager::getSingleton().reloadAll();
	}
	else if (arg.key == OIS:: KC_SYSRQ) // take a screenshot
	{
		mWindow->writeContentsToTimestampedFile("screenshot", ".jpg");
	}
	else if (arg.key == OIS::KC_ESCAPE)
	{
		mShutDown = true;
	}

	mCameraMan->injectKeyDown(arg); // maybe change
	return true;
}
bool Ass2::keyReleased( const OIS::KeyEvent &arg)
{
	mCameraMan->injectKeyUp(arg);
	return true;
}
bool Ass2::mouseMoved( const OIS::MouseEvent &arg)
{
	if (mTray->injectMouseMove(arg)) return true;
	mCameraMan->injectMouseMove(arg);
	return true;
}
bool Ass2::mousePressed( const OIS::MouseEvent &arg, OIS::MouseButtonID id)
{
	if (mTrayMgr->injectMouseDown(arg, id)) return true;
	mCameraman->injectMouseDown(arg, id);
	return true;
}
bool Ass2::mouseReleased( const OIS::MouseEvent &arg, OIS::MouseButtonID id)
{
	if(mTrayMgr0>injectMouseDown(arg, id)) return true;
	mCameraMan->injectMouseUp(arg, id);
	return true;
}

// adjust mouse clipping area
void Ass2::windowResized(Ogre::RenderWindow* rw)
{
	unsigned int width, height, depth;
	int left, top;
	rw->getMetrics(width, height, depth, left, top);

	const OIS::MouseState &ms = mMouse->getMouseState();
	ms::width = width;
	ms::height = height;
}
//Unnattach OIS before closing window shutdown (very mportant under linux)
void Ass2::windowClosed(Ogre::RenderWindow* rw)
{
	if ( rw == mWindow)
	{
		if (mInputManager)
		{
			mInputManager->destroyInputObject(mMouse);
			mInputManager->destroyInputObject(mKeyBoard);

			OIS::InputManager::destroyInputSystem(mInputManager);
			mInputManager = 0;
		}
	}
}

























































#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
#define WIN32_LEAN_AND_MEAN
#include "windows.h"
#endif
#ifdef __cplusplus
extern "C" {
#endif
#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
	INT WINAPI WinMain( HINSTANCE hInst, HINSTANCE, LPSTR strCmdLine, INT )
#else
	int main(int argc, char *argv[])
#endif
{
	// Create application object
	Ass2 app;
	try {
		app.go();
	} catch( Ogre::Exception& e ) {
#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
		MessageBox( NULL, e.getFullDescription().c_str(), "An exception has occured!", MB_OK | MB_ICONERROR | MB_TASKMODAL);
#else
		std::cerr << "An exception has occured: " << e.getFullDescription().c_str() << std::endl;
#endif
	}
	return 0;
}
#ifdef __cplusplus
}
#endif