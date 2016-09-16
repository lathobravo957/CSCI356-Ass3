#include "stdafx.h"
#include "DemoApp.h"
 
//-------------------------------------------------------------------------------------
DemoApp::DemoApp(void)
{
}
//-------------------------------------------------------------------------------------
DemoApp::~DemoApp(void)
{
	if(mPhysicsEngine)
		delete mPhysicsEngine;
}

bool DemoApp::setup(void)
{
	mPhysicsEngine = new PhysicsEngine();
	mPhysicsEngine->initPhysics();

	mBoxCount = 0;

	BaseApplication::setup();

	mTrayMgr->showCursor();

	return true;
};

//-------------------------------------------------------------------------------------
void DemoApp::createScene(void)
{
	mCamera->setPosition(Ogre::Vector3(0, 50, 100));
	mCamera->lookAt(Ogre::Vector3(0, 0, 0));

	mSceneMgr->setAmbientLight(Ogre::ColourValue(0.25, 0.25, 0.25));
 
    // create the light
    Ogre::Light *light = mSceneMgr->createLight("Light1");
    light->setType(Ogre::Light::LT_POINT);
    light->setPosition(Ogre::Vector3(250, 150, 250));
    light->setDiffuseColour(Ogre::ColourValue::White);
    light->setSpecularColour(Ogre::ColourValue::White);

	// Create the ground object shape
	btBoxShape* groundShape = new btBoxShape(btVector3(50.0f, 5.0f, 50.0f));

	// Set the object's transformation
	btTransform groundTransform;
	groundTransform.setIdentity();
	groundTransform.setOrigin(btVector3(0, -5, 0));

	Ogre::Entity* cube = mSceneMgr->createEntity("Ground", "cube.mesh");
	Ogre::SceneNode* node = mSceneMgr->getRootSceneNode()->createChildSceneNode();
	node->attachObject(cube);
	node->scale(1, 0.1, 1);
	node->translate(0, -5, 0);

	// Create the rigid body
	mPhysicsEngine->createRigidBody(0.0f, groundTransform, groundShape, node);

/*
	node->setVisible(false);

	// Use a place to represent the ground
	Ogre::Plane plane(Ogre::Vector3::UNIT_Y, 0);
	Ogre::MeshManager::getSingleton().createPlane("ground", Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME, plane, 
		100, 100, 20, 20, true, 1, 5, 5, Ogre::Vector3::UNIT_Z);

	Ogre::Entity* entGround = mSceneMgr->createEntity("GroundEntity", "ground");
	mSceneMgr->getRootSceneNode()->createChildSceneNode()->attachObject(entGround);
	entGround->setMaterialName("Examples/Rockwall");
	entGround->setCastShadows(false);
*/		

}

void DemoApp::createFrameListener(void)
{
	BaseApplication::createFrameListener();
 
	// set the rotation and move speed
	mRotate = 0.13;
	mMove = 250;
 
	mDirection = Ogre::Vector3::ZERO;
}

bool DemoApp::frameRenderingQueued(const Ogre::FrameEvent& evt)
{
    if(mWindow->isClosed())
        return false;
 
	if(mShutDown)
		return false;
 
	//Need to capture/update each device
	mKeyboard->capture();
	mMouse->capture();

	//move the camera
	mCamera->moveRelative(mDirection * mMove * evt.timeSinceLastFrame/1000);

	mTrayMgr->frameRenderingQueued(evt);

	// Update physics simulation
	mPhysicsEngine->update(evt.timeSinceLastFrame);

	return true;
}
 
// OIS::KeyListener
bool DemoApp::keyPressed( const OIS::KeyEvent &arg )
{
    switch (arg.key)
	{
		case OIS::KC_UP:
		case OIS::KC_W:
			mDirection.z = -mMove;
			break;
 
		case OIS::KC_DOWN:
		case OIS::KC_S:
			mDirection.z = mMove;
			break;
 
		case OIS::KC_LEFT:
		case OIS::KC_A:
			mDirection.x = -mMove;
			break;
 
		case OIS::KC_RIGHT:
		case OIS::KC_D:
			mDirection.x = mMove;
			break;
 
		case OIS::KC_PGDOWN:
		case OIS::KC_E:
			mDirection.y = -mMove;
			break;
 
		case OIS::KC_PGUP:
		case OIS::KC_Q:
			mDirection.y = mMove;
			break;
 
		case OIS::KC_ESCAPE: 
			mShutDown = true;
			break;

		default:
			break;
	}

	return true;
}
 
bool DemoApp::keyReleased( const OIS::KeyEvent &arg )
{
	switch (arg.key)
	{
		case OIS::KC_UP:
		case OIS::KC_W:
			mDirection.z = 0;
			break;
 
		case OIS::KC_DOWN:
		case OIS::KC_S:
			mDirection.z = 0;
			break;
 
		case OIS::KC_LEFT:
		case OIS::KC_A:
			mDirection.x = 0;
			break;
 
		case OIS::KC_RIGHT:
		case OIS::KC_D:
			mDirection.x = 0;
			break;
 
		case OIS::KC_PGDOWN:
		case OIS::KC_E:
			mDirection.y = 0;
			break;
 
		case OIS::KC_PGUP:
		case OIS::KC_Q:
			mDirection.y = 0;
			break;
 
		default:
			break;
	}

	return true;
}

// OIS::MouseListener
bool DemoApp::mouseMoved( const OIS::MouseEvent &arg )
{
	if (mTrayMgr->injectMouseMove(arg)) return true;

//	if (arg.state.buttonDown(OIS::MB_Right))
	{
		mCamera->yaw(Ogre::Degree(-mRotate * arg.state.X.rel));
		mCamera->pitch(Ogre::Degree(-mRotate * arg.state.Y.rel));
	}

	return true;
}

void DemoApp::shootBox(const btVector3& position, const btQuaternion& orientation, const btVector3& linearVelocity)
{
	// Create unique name
	std::ostringstream oss;
	oss << mBoxCount;
	std::string entityName = "Cube" + oss.str();
	// Increment box count
	mBoxCount++;
		
	// Create cube mesh with unique name
	Ogre::Entity* cube = mSceneMgr->createEntity(entityName, "cube.mesh");
	Ogre::SceneNode* node = mSceneMgr->getRootSceneNode()->createChildSceneNode();
	node->attachObject(cube);
	// Scale it to appropriate size
	node->scale(0.02, 0.02, 0.02);

	// Create a collision shape
	// Note that the size should match the size of the object that will be displayed
	btCollisionShape* collisionShape = new btBoxShape(btVector3(1, 1, 1));

	// The object's starting transformation
	btTransform startingTrans;
	startingTrans.setIdentity();
	startingTrans.setOrigin(position);
	startingTrans.setRotation(orientation);

	// Create the rigid body
	btRigidBody* rigidBody = mPhysicsEngine->createRigidBody(1.0f, startingTrans, collisionShape, node);

	// Give the rigid body an initial velocity
	rigidBody->setLinearVelocity(linearVelocity);
}

bool DemoApp::mousePressed( const OIS::MouseEvent &arg, OIS::MouseButtonID id )
{
	if (mTrayMgr->injectMouseDown(arg, id)) return true;

	switch (id)
	{
		case OIS::MB_Left:{
				// Get the mouse ray, i.e. ray from the mouse cursor 'into' the screen 
				Ogre::Ray mouseRay = mCamera->getCameraToViewportRay(
					static_cast<float>(mMouse->getMouseState().X.abs)/mMouse->getMouseState().width, 
					static_cast<float>(mMouse->getMouseState().Y.abs)/mMouse->getMouseState().height);

				// Get a point one unit from the mouse ray origin, in the direction of the ray
				Ogre::Vector3 destination = mouseRay.getPoint(1);
				
				// Calculate the direction for the linear velocity
				btVector3 linearVelocity(
					destination.x-mouseRay.getOrigin().x, 
					destination.y-mouseRay.getOrigin().y, 
					destination.z-mouseRay.getOrigin().z);
				
				linearVelocity.normalize();
				// Scale to appropriate velocity
				linearVelocity *= 30.0f;

				// Create and shoot the box
				shootBox(convert(mouseRay.getOrigin()), btQuaternion(0,0,0,1), linearVelocity);
			}
			break;
		default:
			break;
	}

	return true;
}

bool DemoApp::mouseReleased( const OIS::MouseEvent &arg, OIS::MouseButtonID id )
{
    if (mTrayMgr->injectMouseUp(arg, id)) return true;

	return true;
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
        DemoApp app;
 
        try {
            app.go();
        } catch( Ogre::Exception& e ) {
#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
            MessageBox( NULL, e.getFullDescription().c_str(), "An exception has occured!", MB_OK | MB_ICONERROR | MB_TASKMODAL);
#else
            std::cerr << "An exception has occured: " <<
                e.getFullDescription().c_str() << std::endl;
#endif
        }
 
        return 0;
    }
 
#ifdef __cplusplus
}
#endif