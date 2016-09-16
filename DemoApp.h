#ifndef __DemoApp_h_
#define __DemoApp_h_
 
#include "stdafx.h"
#include "BaseApplication.h"
#include "PhysicsEngine.h"

#include <fstream>
#include <string>

class DemoApp : public BaseApplication
{
public:
    DemoApp(void);
    virtual ~DemoApp(void);
 
protected:
	virtual bool setup();
    virtual void createScene(void);
	virtual void createFrameListener(void);
	void shootBox(const btVector3& position, const btQuaternion& orientation, const btVector3& linearVelocity);
 
	// Ogre::FrameListener
    virtual bool frameRenderingQueued(const Ogre::FrameEvent& evt);
    // OIS::KeyListener
    virtual bool keyPressed( const OIS::KeyEvent &arg );
    virtual bool keyReleased( const OIS::KeyEvent &arg );
    // OIS::MouseListener
    virtual bool mouseMoved( const OIS::MouseEvent &arg );
    virtual bool mousePressed( const OIS::MouseEvent &arg, OIS::MouseButtonID id );
    virtual bool mouseReleased( const OIS::MouseEvent &arg, OIS::MouseButtonID id );
 
	Ogre::Real mRotate;          // The rotate constant
	Ogre::Real mMove;            // The movement constant
 
	Ogre::Vector3 mDirection;     // Value to move in the correct direction
 
	// for physics
	PhysicsEngine*	mPhysicsEngine;

	int				mBoxCount;
};
 
#endif // #ifndef __DemoApp_h_