#include "gms.h"
#include "InterfaceLoading.h"
#include <iostream>
#include "led.h"
#include <tgmath.h>
using namespace std;
// Typedefs and constants for easier understanding
#define PLUGIN_NOT_INITIALIZED 0.0
#define PLUGIN_SUCCESS 1.0
typedef double PLUGIN_RESULT;

// The function pointer for allocating the interface (IYYC_Callback)
typedef IYYC_Callback* (*gI)();
gI getImplementation = NULL;

// Global plugin variables needed to function
IYYC_Callback* gYYC_CallbackHandler = NULL; // the "connection" to the plugin loader
string         gPluginName          = ""; // the ID-Name as which the loader knows this plugin. DO NOT CHANGE THIS MANUALLY!
bool           gInitialized         = false;// a simple boolean that gets set to true if the init() method ran successfully


/*
    Returns a new vec3 of colors
*/
vec3* HSVtoRGB(float H, float S,float V){
    if(H>360 || H<0 || S>100 || S<0 || V>100 || V<0){
        //cout<<"The givem HSV values are not in valid range"<<endl;
        return new vec3(0,0,0);
    }
    float s = S/100;
    float v = V/100;
    float C = s*v;
    float X = C*(1-abs(fmod(H/60.0, 2)-1));
    float m = v-C;
    float r,g,b;
    if(H >= 0 && H < 60){
        r = C,g = X,b = 0;
    }
    else if(H >= 60 && H < 120){
        r = X,g = C,b = 0;
    }
    else if(H >= 120 && H < 180){
        r = 0,g = C,b = X;
    }
    else if(H >= 180 && H < 240){
        r = 0,g = X,b = C;
    }
    else if(H >= 240 && H < 300){
        r = X,g = 0,b = C;
    }
    else{
        r = C,g = 0,b = X;
    }
    int R = (r+m)*255;
    int G = (g+m)*255;
    int B = (b+m)*255;

    return new vec3(R,G,B);
}

/** \brief This method gets called only once upon initializing the plugin. Do not call this method manually.
 *
 * \param loaderpath The full filepath of the PluginLoader.dll
 * \param pluginName The plugin name set by the PluginLoader. Do not change this manually.
 * \return Wether the plugin was loaded successfully.
 *
 */
gmx PLUGIN_RESULT init(const char* loaderpath, const char* pluginName)
{
    cout << "Initializing " << pluginName << endl;
    gPluginName = gmu::string_to_constcharptr(pluginName); // convert the name to a string for easier use
    if(gInitialized) //If the plugin is already initialized return with no error. Usually this method shouldn't be called twice.
    {
        cout << "Plugin " << gPluginName << " already initialized." << endl;
        return PLUGIN_SUCCESS;
    }
    cout << "Hello from plugin init!"<<endl;

    HMODULE hmod = LoadLibraryA(loaderpath); // Load the parent PluginLoader.dll
    if(!hmod)
    {
        cout << "Plugin " << gPluginName << ": Could not get HMODULE." <<endl;
        return PLUGIN_NOT_INITIALIZED;
    }

    getImplementation = (gI)GetProcAddress(hmod, "getImplementation"); // Get a reference to the getImplementation method

    if(getImplementation == NULL) // failed to get the implementation method.
    {
        cout << "Plugin " << gPluginName << ": Could not GetProcAddress."<<endl;
        return PLUGIN_NOT_INITIALIZED;
    }

    gYYC_CallbackHandler = getImplementation(); // Allocate the interface pointer to actually communicate this Plugin <--> PluginLoader
    gInitialized = true;                        // Set the flag to true
    cout << "["<<gPluginName<<"]Successfully initialized Plugin " << gPluginName << endl;

    gYYC_CallbackHandler->setData(gmu::string_to_constcharptr(gPluginName), "count", "0");

    // Custom code for init
    for(int i = 0; i < 31; i++)
    {
        leds[i] = vec3();
    }

    return PLUGIN_SUCCESS;
}

/** \brief This method gets called periodically by the host application (the PluginLoader) to update the plugin.
 *
 * \param arg The host can pass for example the step time. The game runs with 60fps, so this value can range from 0.0 - 60.0
 * \return The exit code of this plugin for this frame. If it doesnt return PLUGIN_SUCCESS, the Loader might eject this plugin to prevent a crash.
 *
 */
gmx PLUGIN_RESULT call(double arg)
{
    if(!gInitialized) // If not initialized, don't go further.
    {
        return PLUGIN_NOT_INITIALIZED;
    }



    
    vec3* color = HSVtoRGB(0, 100,100);

    string asJList = "["+std::to_string(color->red)+","+std::to_string(color->green)+","+std::to_string(color->blue) + "]"

    for( int i = 0; i < 31; i++)
    {
        gYYC_CallbackHandler->setData(  gmu::string_to_constcharptr(gPluginName),
                                        gmu::string_to_constcharptr(std::to_string(i)),
                                        gmu::string_to_constcharptr(asJList));
    }

    gYYC_CallbackHandler->setData(gmu::string_to_constcharptr(gPluginName), "count", "31");

    // Clean before exiting
    delete color;

	return PLUGIN_SUCCESS;
}

