/************************************************************************************

Filename    :   OvrApp.cpp
Content     :   Trivial use of the application framework.
Created     :   
Authors     :   

Copyright   :   Copyright 2014 Oculus VR, LLC. All Rights reserved.

*************************************************************************************/

#include "OvrApp.h"
#include "GuiSys.h"
#include "OVR_Locale.h"
#include "main.h"
#include "gvr.h"
#include "Controls.hpp"
#include "Utils.hpp"
#include "Sound.hpp"

using namespace OVR;

#define YAW_TIMER 8
#define YAW_TURN 15
//#define YAW_TIMER 0
//#define YAW_TURN 15

int g_keys[512];
int g_leftYawTimer = 0;
int g_rightYawTimer = 0;

#if defined( OVR_OS_ANDROID )
extern "C" {

jlong Java_oculus_MainActivity_nativeSetAppInterface( JNIEnv * jni, jclass clazz, jobject activity,
		jstring fromPackageName, jstring commandString, jstring uriString )
{
	LOG( "nativeSetAppInterface" );

    g_objMainActivity = jni->NewGlobalRef(activity);
    g_classMainActivity = clazz;
    g_open = jni->GetMethodID(g_classMainActivity, "open", "(Ljava/lang/String;)Landroid/graphics/Bitmap;");
    g_getWidth = jni->GetMethodID(g_classMainActivity, "getWidth", "(Landroid/graphics/Bitmap;)I");
    g_getHeight = jni->GetMethodID(g_classMainActivity, "getHeight", "(Landroid/graphics/Bitmap;)I");
    g_getPixels = jni->GetMethodID(g_classMainActivity, "getPixels", "(Landroid/graphics/Bitmap;[I)V");
    g_close = jni->GetMethodID(g_classMainActivity, "close", "(Landroid/graphics/Bitmap;)V");
    g_assetOpen = jni->GetMethodID(g_classMainActivity, "assetOpen", "(Ljava/lang/String;)V");
    g_assetReadByte = jni->GetMethodID(g_classMainActivity, "assetReadByte", "()C");
    g_assetReadInt = jni->GetMethodID(g_classMainActivity, "assetReadInt", "()I");
    g_assetReadFloat = jni->GetMethodID(g_classMainActivity, "assetReadFloat", "()F");
    g_getAssetManager = jni->GetMethodID(g_classMainActivity, "getAssetManager", "()Landroid/content/res/AssetManager;");
    g_loadSound = jni->GetMethodID(g_classMainActivity, "loadSound", "(Ljava/lang/String;)I");
    g_playSound = jni->GetMethodID(g_classMainActivity, "playSound", "(I)I");
    g_stopSound = jni->GetMethodID(g_classMainActivity, "stopSound", "(I)V");
    g_playTrack = jni->GetMethodID(g_classMainActivity, "playTrack", "(Ljava/lang/String;)V");
    g_stopTrack = jni->GetMethodID(g_classMainActivity, "stopTrack", "()V");

	return (new OvrTemplateApp::OvrApp())->SetActivity( jni, clazz, activity, fromPackageName, commandString, uriString );
}

} // extern "C"

#endif

namespace OvrTemplateApp
{

OvrApp::OvrApp()
	: SoundEffectContext( NULL )
	, SoundEffectPlayer( NULL )
	, GuiSys( OvrGuiSys::Create() )
	, Locale( NULL )
{
}

OvrApp::~OvrApp()
{
	OvrGuiSys::Destroy( GuiSys );
}

void OvrApp::Configure( ovrSettings & settings )
{
	settings.PerformanceParms.CpuLevel = 2;
	settings.PerformanceParms.GpuLevel = 2;
}

void OvrApp::OneTimeInit( const char * fromPackage, const char * launchIntentJSON, const char * launchIntentURI )
{
	OVR_UNUSED( fromPackage );
	OVR_UNUSED( launchIntentJSON );
	OVR_UNUSED( launchIntentURI );

	const ovrJava * java = app->GetJava();
	SoundEffectContext = new ovrSoundEffectContext( *java->Env, java->ActivityObject );
	SoundEffectContext->Initialize();
	SoundEffectPlayer = new OvrGuiSys::ovrDummySoundEffectPlayer();

	Locale = ovrLocale::Create( *app, "default" );

	String fontName;
	GetLocale().GetString( "@string/font_name", "efigs.fnt", fontName );
	GuiSys->Init( this->app, *SoundEffectPlayer, fontName.ToCStr(), &app->GetDebugLines() );
        
	const OvrStoragePaths & paths = app->GetStoragePaths();

	Array<String> SearchPaths;
	paths.PushBackSearchPathIfValid( EST_SECONDARY_EXTERNAL_STORAGE, EFT_ROOT, "RetailMedia/", SearchPaths );
	paths.PushBackSearchPathIfValid( EST_SECONDARY_EXTERNAL_STORAGE, EFT_ROOT, "", SearchPaths );
	paths.PushBackSearchPathIfValid( EST_PRIMARY_EXTERNAL_STORAGE, EFT_ROOT, "RetailMedia/", SearchPaths );
	paths.PushBackSearchPathIfValid( EST_PRIMARY_EXTERNAL_STORAGE, EFT_ROOT, "", SearchPaths );

	const char * scenePath = "Oculus/tuscany.ovrscene";
	String SceneFile;
	if ( GetFullPath( SearchPaths, scenePath, SceneFile ) )
	{
		MaterialParms materialParms;
		materialParms.UseSrgbTextureFormats = false;
		Scene.LoadWorldModel( SceneFile.ToCStr(), materialParms );
		Scene.SetYawOffset( -Mathf::Pi * 0.5f );
	}
	else
	{
		LOG( "OvrApp::OneTimeInit SearchPaths failed to find %s", scenePath );
	}

	// Init gamepad keys
	for (int i = 0; i < 512; i++)
	    g_keys[i] = 0;


    SoundEffectPlayer->Play("touch_up");
	StartApp(0);
}

void OvrApp::OneTimeShutdown()
{
	delete SoundEffectPlayer;
	SoundEffectPlayer = NULL;

	delete SoundEffectContext;
	SoundEffectContext = NULL;
}

bool OvrApp::OnKeyEvent( const int keyCode, const int repeatCount, const KeyEventType eventType )
{
    Controls *controls = g_app->GetControls();

    if (eventType == KEY_EVENT_DOWN)
    {
        g_keys[keyCode] = 1;

        if (keyCode == OVR_KEY_LEFT_TRIGGER || keyCode == OVR_KEY_BUTTON_L1 || keyCode == OVR_KEY_BUTTON_L1 || keyCode == OVR_KEY_BUTTON_A || keyCode == OVR_KEY_BUTTON_B ||
        keyCode == OVR_KEY_BUTTON_X || keyCode == OVR_KEY_BUTTON_Y)
        {
            controls->BtnDown(0);
        }
    }

    if (eventType == KEY_EVENT_UP)
    {
        g_keys[keyCode] = 0;

        if (keyCode == OVR_KEY_LEFT_TRIGGER || keyCode == OVR_KEY_BUTTON_L1 || keyCode == OVR_KEY_BUTTON_L1 || keyCode == OVR_KEY_BUTTON_A || keyCode == OVR_KEY_BUTTON_B ||
        keyCode == OVR_KEY_BUTTON_X || keyCode == OVR_KEY_BUTTON_Y)
        {
            controls->BtnUp(0);
        }
    }

    return false;

/*	if ( GuiSys->OnKeyEvent( keyCode, repeatCount, eventType ) )
	{
		return true;
	}
	return false;*/
}

Matrix4f OvrApp::Frame( const VrFrame & vrFrame )
{
	// Player movement.
	Scene.Frame( vrFrame, app->GetHeadModelParms() );

    // Get head position
	float headYaw = 0;
	float headPitch = 0;
	float headRoll = 0;

	const Quatf quat = vrFrame.Tracking.HeadPose.Pose.Orientation;
	quat.GetEulerAngles<Axis_Y, Axis_X, Axis_Z>( &headYaw, &headPitch, &headRoll );
	SetHeadVector(headYaw, headPitch, headRoll);

	// Process gamepad keys
    Controls *controls = g_app->GetControls();

	if (g_keys[OVR_KEY_DPAD_UP] == 1)
        controls->MoveForward();
	if (g_keys[OVR_KEY_DPAD_DOWN] == 1)
        controls->MoveBackward();
	if (g_keys[OVR_KEY_DPAD_LEFT] == 1)
        controls->MoveLeft();
	if (g_keys[OVR_KEY_DPAD_RIGHT] == 1)
        controls->MoveRight();

	if (g_keys[OVR_KEY_LSTICK_UP] == 1)
        controls->MoveForward();
	if (g_keys[OVR_KEY_LSTICK_DOWN] == 1)
        controls->MoveBackward();

    if (g_leftYawTimer >= 0)
        g_leftYawTimer--;

	if (g_keys[OVR_KEY_LSTICK_LEFT] == 1 && g_leftYawTimer <= 0)
	{
        controls->MoveYaw(-YAW_TURN);
	    g_leftYawTimer = YAW_TIMER;
    }

    if (g_rightYawTimer >= 0)
        g_rightYawTimer--;

	if (g_keys[OVR_KEY_LSTICK_RIGHT] == 1 && g_rightYawTimer <= 0)
	{
        controls->MoveYaw(YAW_TURN);
	    g_rightYawTimer = YAW_TIMER;
    }

	if (g_keys[OVR_KEY_RSTICK_UP] == 1)
        controls->MoveForward();
	if (g_keys[OVR_KEY_RSTICK_DOWN] == 1)
        controls->MoveBackward();
	if (g_keys[OVR_KEY_RSTICK_LEFT] == 1)
        controls->MoveLeft();
	if (g_keys[OVR_KEY_RSTICK_RIGHT] == 1)
        controls->MoveRight();

    // App tick
	AppTick();

	// Update GUI systems after the app frame, but before rendering anything.
	GuiSys->Frame( vrFrame, Scene.GetCenterEyeViewMatrix() );

	return Scene.GetCenterEyeViewMatrix();
}

Matrix4f OvrApp::DrawEyeView( const int eye, const float fovDegreesX, const float fovDegreesY, ovrFrameParms & frameParms )
{
	const Matrix4f viewMatrix = Scene.GetEyeViewMatrix( eye );
	const Matrix4f projectionMatrix = Scene.GetEyeProjectionMatrix( eye, fovDegreesX, fovDegreesY );
	const Matrix4f eyeViewProjection = Scene.DrawEyeView( eye, fovDegreesX, fovDegreesY );

    SetEyeMatrix(viewMatrix);
    SetProjectionMatrix(projectionMatrix);

	Draw(eye + 1);

	frameParms.ExternalVelocity = Scene.GetExternalVelocity();
	frameParms.Layers[VRAPI_FRAME_LAYER_TYPE_WORLD].Flags |= VRAPI_FRAME_LAYER_FLAG_CHROMATIC_ABERRATION_CORRECTION;

	GuiSys->RenderEyeView( Scene.GetCenterEyeViewMatrix(), viewMatrix, projectionMatrix );

	return eyeViewProjection;
}

} // namespace OvrTemplateApp
