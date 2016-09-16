#include "stdafx.h"
//#include <OgreRoot.h>
//#include <OISEvents.h>
//#include <OISInputManager.h>
//#include <OISKeyboard.h>
//#include <OISMouse.h>
#include <OgreWindowEventUtilities.h>
#include "PhysicsEngine.h"



class Ass2 : public Ogre::WindowEventListener, public Ogre::FrameListener, public OIS::KeyListener, public OIS::MouseListener, OgreBites::SdkTrayListener{
public:
	Ass2(void);
	virtual ~Ass2(void);
	bool go(void);
	

private:
	Ogre::Root* mRoot;
	Ogre::String mPluginsCfg;
	Ogre::String mResourcesCfg;
	Ogre::RenderWindow* mWindow;
	Ogre::SceneManager* mSceneMgr;
	Ogre::Camera* mCamera;
	// OIS Input devices
	OIS::InputManager* mInputManager;
	OIS::Mouse* mMouse;
	OIS::Keyboard* mKeyboard;

protected:
	// Ogre::WindowEventListener
	virtual void windowResized(Ogre::RenderWindow* rw);
	virtual void windowClosed(Ogre::RenderWindow* rw);

	// Ogre::FrameListener
	virtual bool frameRenderingQueued(const Ogre::FrameEvent& evt);

	virtual bool setup();
	virtual bool configure(void);
	virtual void chooseSceneManager();
	virtual void createCamera();
	virtual void createFrameListener(void);
	virtual void createScene(void) = 0; // Override me
	virtual void destroyScene(void);
	virtual void createViewports(void);
	virtual void setupResources(void);
	virtual void createResourceListener(void);
	virtual void loadResources(void);
	virtual bool frameRenderingQueued(const Ogre::FrameEvent& evt);

	virtual bool keyPressed( const OIS::KeyEvent &arg);
	virtual bool keyReleased( const OIS::KeyEvent &arg);
	virtual bool mouseMoved( const OIS::MouseEvent &arg);
	virtual bool mousePressed( const OIS::MouseEvent &arg, OIS::MouseButtonID id);
	virtual bool mouseReleased( const OIS::MouseEvent &arg, OIS::MouseButtonID id);

	// adjust mouse clipping area
	virtual void windowResized(Ogre::RenderWindow* rw);
	//Unnattach OIS before closing window shutdown (very mportant under linux)
	virtual void windowClosed(Ogre::RenderWindow* rw);


	void shootBox(const btVector3& position, const btQuaternion& orientation, const btVector3& linearVelocity);
	PhysicsEngine* mPhysicsEngine;
	int mBoxCount;

	Ogre::Root *mRoot;
	Ogre::Camera* mCamera;
	Ogre::SceneManager* mSceneMgr;
	Ogre::RenderWindow* mWindow;
	Ogre::String mResourcesCfg;
	Ogre::String mPluginsCfg;

	Ogre::OverlaySystem *mOverlaySystem;

	//OgreBites
	OgreBites::InputContext mInputContext;
	OgreBites::SdkTrayManager* mTrayMgr;
	OgreBites::SdkCameraMan* mCameraMan; //probs wont keep
	OgreBites::ParamsPanel* mDetailsPanel; //sample details panel, probs ditch
	bool mCursorWasVisible; //was cursor visible before dialog appeared
	bool mShutDown;

	//OIS Input devices
	OIS::InputManager* mInputManager;
	OIS::Mouse* mMouse;
	OIS::Keyboard* mKeyboard;

	//Added for Mac compatibility
	Ogre::String m_ResourcePath;

#ifdef OGRE_STATIC_LIB
	Ogre::StaticPluginLoader m_StaticPluginLoader;
#endif
};


class Character {
public:
	Character();
	~Character();
	//KEYBOARD
	//swap weapons, grenade, ball
	
	//MOUSE
	//look left and right
	//look up and down
	void look(void);

private:
	Ogre::SceneNode* head; 
	// rotates left and right
	// moves forwards and backwards with wasd
	Ogre::SceneNode* eyes;
	//Is the camera. is parented to the head
	// looks up and down
	
	//guns?

};
